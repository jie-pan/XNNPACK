// Auto-generated file. Do not edit!
//   Template: src/f32-gemm/wasmsimd-loadsplat.c.in
//   Generator: tools/xngen
//
// Copyright 2020 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <wasm_simd128.h>

#include <xnnpack/gemm.h>

void xnn_f32_gemm_relu_ukernel_4xN__wasmsimd_loadsplat(
    size_t mr,
    size_t nc,
    size_t kc,
    const float*restrict a,
    size_t a_stride,
    const float*restrict w,
    float*restrict c,
    size_t cm_stride,
    size_t cn_stride,
    size_t N,
    const union xnn_f32_relu_params params[restrict XNN_MIN_ELEMENTS(1)]);


void xnn_f32_gemm_relu_ukernel_4xN__wasmsimd_loadsplat_wrapper(
    size_t mr,
    size_t nc,
    size_t kc,
    const float*restrict a,
    size_t a_stride,
    const float*restrict w,
    float*restrict c,
    size_t cm_stride,
    size_t cn_stride,
    const union xnn_f32_relu_params params[restrict XNN_MIN_ELEMENTS(1)])
{

  //TODO, get N from custom section
  const size_t N = 8 * max_vector_width / default_vector_width;

  //assert(N == 8 || N == 16 || N == 32);
  assert(N == 8 || N == 16);

  if(nc % N != 0) {
      if(N == 8) {
        xnn_f32_gemm_relu_ukernel_4x8__wasmsimd_loadsplat(
            mr, nc, kc, a, a_stride,
            w, c, cm_stride, cn_stride, params);
      }
      else if(N == 16){
        xnn_f32_gemm_relu_ukernel_4xN__wasmsimd_loadsplat(
            //mr, nc/N * N,
            mr, nc & ~(N-1),
            kc, a, a_stride,
            w, c, cm_stride, cn_stride, N, params);

        size_t outer_count = nc/N;
        size_t inner_count = kc/sizeof(float);
        w += outer_count * (inner_count * N + N);
        c = (float* restrict) ((uintptr_t) c + outer_count * cn_stride);

        xnn_f32_gemm_relu_ukernel_4x16__wasmsimd_loadsplat(
            //mr, nc % N,
            mr, nc & (N-1),
            kc, a, a_stride,
            w, c, cm_stride, cn_stride, params);
      }

  }
  else {
        xnn_f32_gemm_relu_ukernel_4xN__wasmsimd_loadsplat(
            mr, nc, kc, a, a_stride,
            w, c, cm_stride, cn_stride, N, params);
  }

}
