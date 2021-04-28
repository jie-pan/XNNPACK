// Auto-generated file. Do not edit!
//   Template: src/f32-igemm/wasmsimd-splat.c.in
//   Generator: tools/xngen
//
// Copyright 2020 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <wasm_simd128.h>

#include <xnnpack/igemm.h>


void xnn_f32_igemm_relu_ukernel_1xN__wasmsimd_splat(
    size_t mr,
    size_t nc,
    size_t kc,
    size_t ks,
    const float**restrict a,
    const float*restrict w,
    float*restrict c,
    size_t cm_stride,
    size_t cn_stride,
    size_t a_offset,
    const float* zero,
    const union xnn_f32_relu_params params[restrict XNN_MIN_ELEMENTS(1)])
{

    const size_t N = 8 * max_vector_width / default_vector_width;

    assert(N == 8 || N == 16 || N == 32); //N == 8, baseline, do nothing,
    if( N == 8) {
        xnn_f32_igemm_relu_ukernel_1x8__wasmsimd_splat(
            mr, nc, kc, ks, a, w, c,
            cm_stride, cn_stride, a_offset, zero, params);
    } else if (N == 16) {
        xnn_f32_igemm_relu_ukernel_1x16__wasmsimd_splat(
            mr, nc, kc, ks, a, w, c,
            cm_stride, cn_stride, a_offset, zero, params);
    }
}
