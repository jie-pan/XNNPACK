// Copyright (c) Facebook, Inc. and its affiliates.
// All rights reserved.
//
// Copyright 2019 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <algorithm>
#include <cfloat>
#include <chrono>
#include <cmath>
#include <functional>
#include <mutex>
#include <random>
#include <vector>


#include <benchmark/benchmark.h>
#include "bench/gemm.h"
#include <xnnpack/AlignedAllocator.h>
#include <xnnpack/common.h>
#include <xnnpack/gemm.h>
#include <xnnpack/pack.h>
#include <xnnpack/packx.h>
#include <xnnpack/params-init.h>
#include <xnnpack/params.h>
#include <xnnpack/ppmm.h>
#include <ctime>

template <class T>
inline T DivideRoundUp(T x, T q) {
  return x / q + T(x % q != 0);
}

template <class T>
inline T RoundUp(T x, T q) {
  return q * DivideRoundUp(x, q);
}

template <class T>
inline T Doz(T a, T b) {
  return a >= b ? a - b : T(0);
}

size_t GetMaxCacheSize() {
    return 14417920;//13.75Mb
}

uint32_t PrefetchToL1(const void* ptr, size_t size) {
  uint32_t step = 64;// cpuinfo_get_l1d_cache(0)->line_size;
  const uint8_t* u8_ptr = static_cast<const uint8_t*>(ptr);
  // Compute and return sum of data to prevent compiler from removing data reads.
  uint32_t sum = 0;
  while (size >= step) {
    sum += uint32_t(*u8_ptr);
    u8_ptr += step;
    size -= step;
  }
  return sum;
}


// Return clockrate in Hz
uint64_t GetCurrentCpuFrequency() {
  return 4300000*(uint64_t)1000;
}

static void GEMMBenchmark(benchmark::State& state,
  xnn_f32_gemm_minmax_ukernel_function gemm,
  size_t mr, size_t nr, size_t kr, size_t sr)
{

  const size_t mc = state.range(0);
  const size_t nc = state.range(1);
  const size_t kc = state.range(2);

  const size_t nc_stride = RoundUp(nc, nr);
  const size_t kc_stride = RoundUp(kc, kr);

  //std::random_device random_device;
  //auto rng = std::mt19937(random_device());

  std::mt19937::result_type seed = time(0);
  auto rng = std::mt19937(seed);
  auto f32rng = std::bind(std::uniform_real_distribution<float>(), rng);

  std::vector<float> a(mc * kc);
  std::generate(a.begin(), a.end(), std::ref(f32rng));
  std::vector<float> k(nc * kc);
  std::generate(k.begin(), k.end(), std::ref(f32rng));
  std::vector<float> b(nc);
  std::generate(b.begin(), b.end(), std::ref(f32rng));

  const size_t w_elements = nc_stride * kc_stride + nc_stride;
  const size_t c_elements = mc * nc;
  const size_t num_buffers = 1 +
    DivideRoundUp<size_t>(GetMaxCacheSize(),
      sizeof(float) * (w_elements + c_elements));

  std::vector<float, AlignedAllocator<float, 32>> w(w_elements * num_buffers);
  std::fill(w.begin(), w.end(), 0.0f);
  xnn_pack_f32_gemm_goi_w(1 /* groups */, nc, kc, nr, kr, sr, k.data(), b.data(), w.data());
  std::vector<float> c(c_elements * num_buffers);
  std::fill(c.begin(), c.end(), std::nanf(""));

  xnn_f32_minmax_params params =
    xnn_init_f32_minmax_params(-std::numeric_limits<float>::infinity(), +std::numeric_limits<float>::infinity());

  size_t buffer_index = 0;
  for (auto _ : state) {
    // Use circular buffers (exceeding cache size) and prefetch to control cache state:
    // - A is always in L1 cache (if fits, otherwise L2, L3, etc)
    // - W is not in cache (for any cache level)
    // - C is not in cache (for any cache level)
    state.PauseTiming();
    PrefetchToL1(a.data(), a.size() * sizeof(float));
    buffer_index = (buffer_index + 1) % num_buffers;
    state.ResumeTiming();

    for (uint32_t m = 0; m < mc; m += mr) {
      const uint32_t mb = min(mc - m, mr);
      gemm(
        mb, nc, kc * sizeof(float),
        a.data() + m * kc, kc * sizeof(float),
        w.data() + buffer_index * nc_stride * (kc_stride + 1),
        c.data() + (buffer_index * mc + m) * nc, nc * sizeof(float), nr * sizeof(float),
        &params);
    }
  }

  state.counters["Freq"] = GetCurrentCpuFrequency();
  state.counters["FLOPS"] = benchmark::Counter(
    uint64_t(state.iterations()) * 2 * mc * nc * kc, benchmark::Counter::kIsRate);
}

static void PPMM1PBenchmark(benchmark::State& state,
  xnn_f32_ppmm_minmax_ukernel_function ppmm,
  xnn_x32_packx_ukernel_function packx,
  size_t mr, size_t nr)
{

  const size_t mc = state.range(0);
  const size_t nc = state.range(1);
  const size_t kc = state.range(2);

  const size_t nc_stride = RoundUp(nc, nr);

  std::random_device random_device;
  auto rng = std::mt19937(random_device());
  auto f32rng = std::bind(std::uniform_real_distribution<float>(), rng);

  std::vector<float> a(mc * kc);
  std::generate(a.begin(), a.end(), std::ref(f32rng));
  std::vector<float> k(nc * kc);
  std::generate(k.begin(), k.end(), std::ref(f32rng));
  std::vector<float> b(nc);
  std::generate(b.begin(), b.end(), std::ref(f32rng));

  std::vector<uint32_t, AlignedAllocator<uint32_t, 32>> t(mr * kc);

  const size_t w_elements = nc_stride * kc + nc_stride;
  const size_t c_elements = mc * nc;
  const size_t num_buffers = 1 +
    DivideRoundUp<size_t>(GetMaxCacheSize(),
      sizeof(float) * (w_elements + c_elements));

  std::vector<float, AlignedAllocator<float, 32>> w(w_elements * num_buffers);
  std::fill(w.begin(), w.end(), 0.0f);
  xnn_pack_f32_gemm_goi_w(1 /* groups */, nc, kc, nr, 1 /* kr */, 1 /* sr */, k.data(), b.data(), w.data());
  std::vector<float> c(c_elements * num_buffers);
  std::fill(c.begin(), c.end(), std::nanf(""));

  xnn_f32_minmax_params params =
    xnn_init_f32_minmax_params(-std::numeric_limits<float>::infinity(), +std::numeric_limits<float>::infinity());

  size_t buffer_index = 0;
  for (auto _ : state) {
    // Use circular buffers (exceeding cache size) and prefetch to control cache state:
    // - A is always in L1 cache (if fits, otherwise L2, L3, etc)
    // - W is not in cache (for any cache level)
    // - C is not in cache (for any cache level)
    state.PauseTiming();
    PrefetchToL1(a.data(), a.size() * sizeof(float));
    buffer_index = (buffer_index + 1) % num_buffers;
    state.ResumeTiming();

    for (uint32_t m = 0; m < mc; m += mr) {
      const uint32_t mb = min(mc - m, mr);
      packx(mb, kc, reinterpret_cast<const uint32_t*>(a.data() + m * kc), kc, t.data());
      ppmm(
        mb, nc, kc * sizeof(float),
        reinterpret_cast<const float*>(t.data()),
        w.data() + nc_stride * buffer_index * (kc + 1),
        c.data() + (mc * buffer_index + m) * nc, nc * sizeof(float), nr * sizeof(float),
        &params);
    }
  }

  state.counters["Freq"] = GetCurrentCpuFrequency();
  state.counters["FLOPS"] = benchmark::Counter(
    uint64_t(state.iterations()) * 2 * mc * nc * kc, benchmark::Counter::kIsRate);
}

static void PPMM2PBenchmark(benchmark::State& state,
  xnn_f32_ppmm_minmax_ukernel_function ppmm,
  xnn_x32_packx_ukernel_function packx,
  size_t mr, size_t nr)
{
  const size_t mc = state.range(0);
  const size_t nc = state.range(1);
  const size_t kc = state.range(2);

  const size_t mc_stride = RoundUp(mc, mr);
  const size_t nc_stride = RoundUp(nc, nr);

  std::random_device random_device;
  auto rng = std::mt19937(random_device());
  auto f32rng = std::bind(std::uniform_real_distribution<float>(), rng);

  std::vector<float> a(mc * kc);
  std::generate(a.begin(), a.end(), std::ref(f32rng));
  std::vector<float> k(nc * kc);
  std::generate(k.begin(), k.end(), std::ref(f32rng));
  std::vector<float> b(nc);
  std::generate(b.begin(), b.end(), std::ref(f32rng));

  std::vector<uint32_t, AlignedAllocator<uint32_t, 32>> t(mc_stride * kc);

  const size_t w_elements = nc_stride * kc + nc_stride;
  const size_t c_elements = mc * nc;
  const size_t num_buffers = 1 +
    DivideRoundUp<size_t>(GetMaxCacheSize(),
      sizeof(float) * (w_elements + c_elements));

  std::vector<float, AlignedAllocator<float, 32>> w(w_elements * num_buffers);
  std::fill(w.begin(), w.end(), 0.0f);
  xnn_pack_f32_gemm_goi_w(1 /* groups */, nc, kc, nr, 1 /* kr */, 1 /* sr */, k.data(), b.data(), w.data());
  std::vector<float> c(c_elements * num_buffers);
  std::fill(c.begin(), c.end(), std::nanf(""));

  xnn_f32_minmax_params params =
    xnn_init_f32_minmax_params(-std::numeric_limits<float>::infinity(), +std::numeric_limits<float>::infinity());

  size_t buffer_index = 0;
  for (auto _ : state) {
    // Use circular buffers (exceeding cache size) and prefetch to control cache state:
    // - A is always in L1 cache (if fits, otherwise L2, L3, etc)
    // - W is not in cache (for any cache level)
    // - C is not in cache (for any cache level)
    state.PauseTiming();
    PrefetchToL1(a.data(), a.size() * sizeof(float));
    buffer_index = (buffer_index + 1) % num_buffers;
    state.ResumeTiming();

    for (uint32_t m = 0; m < mc; m += mr) {
      const uint32_t mb = min(mc - m, mr);
      packx(mb, kc, reinterpret_cast<const uint32_t*>(a.data() + m * kc), kc, t.data() + m * kc);
    }
    for (uint32_t m = 0; m < mc; m += mr) {
      const uint32_t mb = min(mc - m, mr);
      ppmm(
        mb, nc, kc * sizeof(float),
        reinterpret_cast<const float*>(t.data() + m * kc),
        w.data() + nc_stride * buffer_index * (kc + 1),
        c.data() + (mc * buffer_index + m) * nc, nc * sizeof(float), nr * sizeof(float),
        &params);
    }
  }

  state.counters["Freq"] = GetCurrentCpuFrequency();
  state.counters["FLOPS"] = benchmark::Counter(
    uint64_t(state.iterations()) * 2 * mc * nc * kc, benchmark::Counter::kIsRate);
}


#if !XNN_ARCH_ASMJS && !XNN_ARCH_WASM && !XNN_COMPILER_MSVC && !XNN_COMPILER_ICC
  static void f32_gemm_4x8__psimd_loadsplat(benchmark::State& state, const char* net) {
    GEMMBenchmark(state, xnn_f32_gemm_minmax_ukernel_4x8__psimd_loadsplat, 4, 8, 1, 1);
  }

  static void f32_gemm_6x8__psimd_loadsplat(benchmark::State& state, const char* net) {
    GEMMBenchmark(state, xnn_f32_gemm_minmax_ukernel_6x8__psimd_loadsplat, 6, 8, 1, 1);
  }

  static void f32_gemm_4x8__psimd_splat(benchmark::State& state, const char* net) {
    GEMMBenchmark(state, xnn_f32_gemm_minmax_ukernel_4x8__psimd_splat, 4, 8, 1, 1);
  }

  static void f32_gemm_6x8__psimd_splat(benchmark::State& state, const char* net) {
    GEMMBenchmark(state, xnn_f32_gemm_minmax_ukernel_6x8__psimd_splat, 6, 8, 1, 1);
  }

  static void f32_gemm_4x8s4__psimd(benchmark::State& state, const char* net) {
    GEMMBenchmark(state, xnn_f32_gemm_minmax_ukernel_4x8s4__psimd, 4, 8, 1, 4);
  }

  static void f32_gemm_6x8s4__psimd(benchmark::State& state, const char* net) {
    GEMMBenchmark(state, xnn_f32_gemm_minmax_ukernel_6x8s4__psimd, 6, 8, 1, 4);
  }

  static void f32_ppmm_4x8_unipass__psimd(benchmark::State& state, const char* net) {
    PPMM1PBenchmark(state, xnn_f32_ppmm_minmax_ukernel_4x8__psimd, xnn_x32_packx_ukernel_4x__psimd, 4, 8);
  }

  static void f32_ppmm_4x8_twopass__psimd(benchmark::State& state, const char* net) {
    PPMM2PBenchmark(state, xnn_f32_ppmm_minmax_ukernel_4x8__psimd, xnn_x32_packx_ukernel_4x__psimd, 4, 8);
  }

  //BENCHMARK_GEMM(f32_gemm_4x8__psimd_loadsplat)
  //BENCHMARK_GEMM(f32_gemm_6x8__psimd_loadsplat)
  //BENCHMARK_GEMM(f32_gemm_4x8__psimd_splat)
  //BENCHMARK_GEMM(f32_gemm_6x8__psimd_splat)
  //BENCHMARK_GEMM(f32_gemm_4x8s4__psimd)
  //BENCHMARK_GEMM(f32_gemm_6x8s4__psimd)
  //BENCHMARK_GEMM(f32_ppmm_4x8_unipass__psimd)
  //BENCHMARK_GEMM(f32_ppmm_4x8_twopass__psimd)

  BENCHMARK_GEMM_MOBILENETV2(f32_gemm_4x8__psimd_loadsplat)

#endif  // !XNN_ARCH_ASMJS && !XNN_ARCH_WASM && !XNN_COMPILER_MSVC && !XNN_COMPILER_ICC


#if 0
static void f32_gemm_1x4__scalar(benchmark::State& state, const char* net) {
  GEMMBenchmark(state, xnn_f32_gemm_minmax_ukernel_1x4__scalar, 1, 4, 1, 1);
}

static void f32_gemm_2x4__scalar(benchmark::State& state, const char* net) {
  GEMMBenchmark(state, xnn_f32_gemm_minmax_ukernel_2x4__scalar, 2, 4, 1, 1);
}

static void f32_gemm_4x4__scalar(benchmark::State& state, const char* net) {
  GEMMBenchmark(state, xnn_f32_gemm_minmax_ukernel_4x4__scalar, 4, 4, 1, 1);
}

static void f32_ppmm_2x4_unipass__scalar(benchmark::State& state, const char* net) {
  PPMM1PBenchmark(state, xnn_f32_ppmm_minmax_ukernel_2x4__scalar, xnn_x32_packx_ukernel_2x__scalar, 2, 4);
}

static void f32_ppmm_4x2_unipass__scalar(benchmark::State& state, const char* net) {
  PPMM1PBenchmark(state, xnn_f32_ppmm_minmax_ukernel_4x2__scalar, xnn_x32_packx_ukernel_4x__scalar, 4, 2);
}

static void f32_ppmm_4x4_unipass__scalar(benchmark::State& state, const char* net) {
  PPMM1PBenchmark(state, xnn_f32_ppmm_minmax_ukernel_4x4__scalar, xnn_x32_packx_ukernel_4x__scalar, 4, 4);
}

static void f32_ppmm_3x3_unipass__scalar(benchmark::State& state, const char* net) {
  PPMM1PBenchmark(state, xnn_f32_ppmm_minmax_ukernel_3x3__scalar, xnn_x32_packx_ukernel_3x__scalar, 3, 3);
}

static void f32_ppmm_2x4_twopass__scalar(benchmark::State& state, const char* net) {
  PPMM2PBenchmark(state, xnn_f32_ppmm_minmax_ukernel_2x4__scalar, xnn_x32_packx_ukernel_2x__scalar, 2, 4);
}

static void f32_ppmm_4x2_twopass__scalar(benchmark::State& state, const char* net) {
  PPMM2PBenchmark(state, xnn_f32_ppmm_minmax_ukernel_4x2__scalar, xnn_x32_packx_ukernel_4x__scalar, 4, 2);
}

static void f32_ppmm_4x4_twopass__scalar(benchmark::State& state, const char* net) {
  PPMM2PBenchmark(state, xnn_f32_ppmm_minmax_ukernel_4x4__scalar, xnn_x32_packx_ukernel_4x__scalar, 4, 4);
}

static void f32_ppmm_3x3_twopass__scalar(benchmark::State& state, const char* net) {
  PPMM2PBenchmark(state, xnn_f32_ppmm_minmax_ukernel_3x3__scalar, xnn_x32_packx_ukernel_3x__scalar, 3, 3);
}

BENCHMARK_GEMM(f32_gemm_1x4__scalar)
BENCHMARK_GEMM(f32_gemm_2x4__scalar)
BENCHMARK_GEMM(f32_gemm_4x4__scalar)

BENCHMARK_GEMM(f32_ppmm_2x4_unipass__scalar)
BENCHMARK_GEMM(f32_ppmm_4x2_unipass__scalar)
BENCHMARK_GEMM(f32_ppmm_4x4_unipass__scalar)
BENCHMARK_GEMM(f32_ppmm_3x3_unipass__scalar)

BENCHMARK_GEMM(f32_ppmm_2x4_twopass__scalar)
BENCHMARK_GEMM(f32_ppmm_4x2_twopass__scalar)
BENCHMARK_GEMM(f32_ppmm_4x4_twopass__scalar)
BENCHMARK_GEMM(f32_ppmm_3x3_twopass__scalar)
#endif


#ifndef XNNPACK_BENCHMARK_NO_MAIN
BENCHMARK_MAIN();
#endif
