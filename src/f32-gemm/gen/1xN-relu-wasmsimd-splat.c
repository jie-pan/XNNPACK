// Auto-generated file. Do not edit!
//   Template: src/f32-gemm/wasmsimd-splat.c.in
//   Generator: tools/xngen
//
// Copyright 2020 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <wasm_simd128.h>

#include <xnnpack/gemm.h>


void xnn_f32_gemm_relu_ukernel_1xN__wasmsimd_splat(
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
  const size_t N = 16;

  //assert(N == 8 || N == 16 || N == 32);
  assert(N == 8 || N == 16);

  if(N == 8) {
    xnn_f32_gemm_relu_ukernel_1x8__wasmsimd_splat(
        mr, nc, kc, a, a_stride,
        w, c, cm_stride, cn_stride, params);
  } else if(N == 16) {
    xnn_f32_gemm_relu_ukernel_1x16__wasmsimd_splat(
        mr, nc, kc, a, a_stride,
        w, c, cm_stride, cn_stride, params);
  }
}
