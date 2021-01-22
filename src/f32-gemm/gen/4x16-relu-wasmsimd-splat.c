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


void xnn_f32_gemm_relu_ukernel_4x16__wasmsimd_splat(
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
  assert(mr != 0);
  assert(mr <= 4);
  assert(nc != 0);
  assert(kc != 0);
  assert(kc % sizeof(float) == 0);
  assert(a != NULL);
  assert(w != NULL);
  assert(c != NULL);

  const float* a0 = a;
  float* c0 = c;
  const float* a1 = (const float*) ((uintptr_t) a0 + a_stride);
  float* c1 = (float*) ((uintptr_t) c0 + cm_stride);
  if XNN_UNPREDICTABLE(mr < 2) {
    a1 = a0;
    c1 = c0;
  }
  const float* a2 = (const float*) ((uintptr_t) a1 + a_stride);
  float* c2 = (float*) ((uintptr_t) c1 + cm_stride);
  if XNN_UNPREDICTABLE(mr <= 2) {
    a2 = a1;
    c2 = c1;
  }
  const float* a3 = (const float*) ((uintptr_t) a2 + a_stride);
  float* c3 = (float*) ((uintptr_t) c2 + cm_stride);
  if XNN_UNPREDICTABLE(mr != 4) {
    a3 = a2;
    c3 = c2;
  }

  do {
    v128_t vacc0x0123 = wasm_v128_load(w + 0);
    v128_t vacc0x4567 = wasm_v128_load(w + 4);
    v128_t vacc0x89AB = wasm_v128_load(w + 8);
    v128_t vacc0xCDEF = wasm_v128_load(w + 12);
    v128_t vacc1x0123 = vacc0x0123;
    v128_t vacc1x4567 = vacc0x4567;
    v128_t vacc1x89AB = vacc0x89AB;
    v128_t vacc1xCDEF = vacc0xCDEF;
    v128_t vacc2x0123 = vacc0x0123;
    v128_t vacc2x4567 = vacc0x4567;
    v128_t vacc2x89AB = vacc0x89AB;
    v128_t vacc2xCDEF = vacc0xCDEF;
    v128_t vacc3x0123 = vacc0x0123;
    v128_t vacc3x4567 = vacc0x4567;
    v128_t vacc3x89AB = vacc0x89AB;
    v128_t vacc3xCDEF = vacc0xCDEF;
    w += 16;

    size_t k = kc;
    while (k >= 4 * sizeof(float)) {
      const v128_t va0 = wasm_v128_load(a0);
      a0 += 4;
      const v128_t va1 = wasm_v128_load(a1);
      a1 += 4;
      const v128_t va2 = wasm_v128_load(a2);
      a2 += 4;
      const v128_t va3 = wasm_v128_load(a3);
      a3 += 4;

      const v128_t va0c0 = wasm_v32x4_shuffle(va0, va0, 0, 0, 0, 0);
      const v128_t va1c0 = wasm_v32x4_shuffle(va1, va1, 0, 0, 0, 0);
      const v128_t va2c0 = wasm_v32x4_shuffle(va2, va2, 0, 0, 0, 0);
      const v128_t va3c0 = wasm_v32x4_shuffle(va3, va3, 0, 0, 0, 0);

      const v128_t vb0123c0 = wasm_v128_load(w + 0);
      const v128_t vb4567c0 = wasm_v128_load(w + 4);
      const v128_t vb89ABc0 = wasm_v128_load(w + 8);
      const v128_t vbCDEFc0 = wasm_v128_load(w + 12);

      vacc0x0123 = wasm_f32x4_add(vacc0x0123, wasm_f32x4_mul(va0c0, vb0123c0));
      vacc1x0123 = wasm_f32x4_add(vacc1x0123, wasm_f32x4_mul(va1c0, vb0123c0));
      vacc2x0123 = wasm_f32x4_add(vacc2x0123, wasm_f32x4_mul(va2c0, vb0123c0));
      vacc3x0123 = wasm_f32x4_add(vacc3x0123, wasm_f32x4_mul(va3c0, vb0123c0));
      vacc0x4567 = wasm_f32x4_add(vacc0x4567, wasm_f32x4_mul(va0c0, vb4567c0));
      vacc1x4567 = wasm_f32x4_add(vacc1x4567, wasm_f32x4_mul(va1c0, vb4567c0));
      vacc2x4567 = wasm_f32x4_add(vacc2x4567, wasm_f32x4_mul(va2c0, vb4567c0));
      vacc3x4567 = wasm_f32x4_add(vacc3x4567, wasm_f32x4_mul(va3c0, vb4567c0));
      vacc0x89AB = wasm_f32x4_add(vacc0x89AB, wasm_f32x4_mul(va0c0, vb89ABc0));
      vacc1x89AB = wasm_f32x4_add(vacc1x89AB, wasm_f32x4_mul(va1c0, vb89ABc0));
      vacc2x89AB = wasm_f32x4_add(vacc2x89AB, wasm_f32x4_mul(va2c0, vb89ABc0));
      vacc3x89AB = wasm_f32x4_add(vacc3x89AB, wasm_f32x4_mul(va3c0, vb89ABc0));
      vacc0xCDEF = wasm_f32x4_add(vacc0xCDEF, wasm_f32x4_mul(va0c0, vbCDEFc0));
      vacc1xCDEF = wasm_f32x4_add(vacc1xCDEF, wasm_f32x4_mul(va1c0, vbCDEFc0));
      vacc2xCDEF = wasm_f32x4_add(vacc2xCDEF, wasm_f32x4_mul(va2c0, vbCDEFc0));
      vacc3xCDEF = wasm_f32x4_add(vacc3xCDEF, wasm_f32x4_mul(va3c0, vbCDEFc0));
      const v128_t va0c1 = wasm_v32x4_shuffle(va0, va0, 1, 1, 1, 1);
      const v128_t va1c1 = wasm_v32x4_shuffle(va1, va1, 1, 1, 1, 1);
      const v128_t va2c1 = wasm_v32x4_shuffle(va2, va2, 1, 1, 1, 1);
      const v128_t va3c1 = wasm_v32x4_shuffle(va3, va3, 1, 1, 1, 1);

      const v128_t vb0123c1 = wasm_v128_load(w + 16);
      const v128_t vb4567c1 = wasm_v128_load(w + 20);
      const v128_t vb89ABc1 = wasm_v128_load(w + 24);
      const v128_t vbCDEFc1 = wasm_v128_load(w + 28);

      vacc0x0123 = wasm_f32x4_add(vacc0x0123, wasm_f32x4_mul(va0c1, vb0123c1));
      vacc1x0123 = wasm_f32x4_add(vacc1x0123, wasm_f32x4_mul(va1c1, vb0123c1));
      vacc2x0123 = wasm_f32x4_add(vacc2x0123, wasm_f32x4_mul(va2c1, vb0123c1));
      vacc3x0123 = wasm_f32x4_add(vacc3x0123, wasm_f32x4_mul(va3c1, vb0123c1));
      vacc0x4567 = wasm_f32x4_add(vacc0x4567, wasm_f32x4_mul(va0c1, vb4567c1));
      vacc1x4567 = wasm_f32x4_add(vacc1x4567, wasm_f32x4_mul(va1c1, vb4567c1));
      vacc2x4567 = wasm_f32x4_add(vacc2x4567, wasm_f32x4_mul(va2c1, vb4567c1));
      vacc3x4567 = wasm_f32x4_add(vacc3x4567, wasm_f32x4_mul(va3c1, vb4567c1));
      vacc0x89AB = wasm_f32x4_add(vacc0x89AB, wasm_f32x4_mul(va0c1, vb89ABc1));
      vacc1x89AB = wasm_f32x4_add(vacc1x89AB, wasm_f32x4_mul(va1c1, vb89ABc1));
      vacc2x89AB = wasm_f32x4_add(vacc2x89AB, wasm_f32x4_mul(va2c1, vb89ABc1));
      vacc3x89AB = wasm_f32x4_add(vacc3x89AB, wasm_f32x4_mul(va3c1, vb89ABc1));
      vacc0xCDEF = wasm_f32x4_add(vacc0xCDEF, wasm_f32x4_mul(va0c1, vbCDEFc1));
      vacc1xCDEF = wasm_f32x4_add(vacc1xCDEF, wasm_f32x4_mul(va1c1, vbCDEFc1));
      vacc2xCDEF = wasm_f32x4_add(vacc2xCDEF, wasm_f32x4_mul(va2c1, vbCDEFc1));
      vacc3xCDEF = wasm_f32x4_add(vacc3xCDEF, wasm_f32x4_mul(va3c1, vbCDEFc1));
      const v128_t va0c2 = wasm_v32x4_shuffle(va0, va0, 2, 2, 2, 2);
      const v128_t va1c2 = wasm_v32x4_shuffle(va1, va1, 2, 2, 2, 2);
      const v128_t va2c2 = wasm_v32x4_shuffle(va2, va2, 2, 2, 2, 2);
      const v128_t va3c2 = wasm_v32x4_shuffle(va3, va3, 2, 2, 2, 2);

      const v128_t vb0123c2 = wasm_v128_load(w + 32);
      const v128_t vb4567c2 = wasm_v128_load(w + 36);
      const v128_t vb89ABc2 = wasm_v128_load(w + 40);
      const v128_t vbCDEFc2 = wasm_v128_load(w + 44);

      vacc0x0123 = wasm_f32x4_add(vacc0x0123, wasm_f32x4_mul(va0c2, vb0123c2));
      vacc1x0123 = wasm_f32x4_add(vacc1x0123, wasm_f32x4_mul(va1c2, vb0123c2));
      vacc2x0123 = wasm_f32x4_add(vacc2x0123, wasm_f32x4_mul(va2c2, vb0123c2));
      vacc3x0123 = wasm_f32x4_add(vacc3x0123, wasm_f32x4_mul(va3c2, vb0123c2));
      vacc0x4567 = wasm_f32x4_add(vacc0x4567, wasm_f32x4_mul(va0c2, vb4567c2));
      vacc1x4567 = wasm_f32x4_add(vacc1x4567, wasm_f32x4_mul(va1c2, vb4567c2));
      vacc2x4567 = wasm_f32x4_add(vacc2x4567, wasm_f32x4_mul(va2c2, vb4567c2));
      vacc3x4567 = wasm_f32x4_add(vacc3x4567, wasm_f32x4_mul(va3c2, vb4567c2));
      vacc0x89AB = wasm_f32x4_add(vacc0x89AB, wasm_f32x4_mul(va0c2, vb89ABc2));
      vacc1x89AB = wasm_f32x4_add(vacc1x89AB, wasm_f32x4_mul(va1c2, vb89ABc2));
      vacc2x89AB = wasm_f32x4_add(vacc2x89AB, wasm_f32x4_mul(va2c2, vb89ABc2));
      vacc3x89AB = wasm_f32x4_add(vacc3x89AB, wasm_f32x4_mul(va3c2, vb89ABc2));
      vacc0xCDEF = wasm_f32x4_add(vacc0xCDEF, wasm_f32x4_mul(va0c2, vbCDEFc2));
      vacc1xCDEF = wasm_f32x4_add(vacc1xCDEF, wasm_f32x4_mul(va1c2, vbCDEFc2));
      vacc2xCDEF = wasm_f32x4_add(vacc2xCDEF, wasm_f32x4_mul(va2c2, vbCDEFc2));
      vacc3xCDEF = wasm_f32x4_add(vacc3xCDEF, wasm_f32x4_mul(va3c2, vbCDEFc2));
      const v128_t va0c3 = wasm_v32x4_shuffle(va0, va0, 3, 3, 3, 3);
      const v128_t va1c3 = wasm_v32x4_shuffle(va1, va1, 3, 3, 3, 3);
      const v128_t va2c3 = wasm_v32x4_shuffle(va2, va2, 3, 3, 3, 3);
      const v128_t va3c3 = wasm_v32x4_shuffle(va3, va3, 3, 3, 3, 3);

      const v128_t vb0123c3 = wasm_v128_load(w + 48);
      const v128_t vb4567c3 = wasm_v128_load(w + 52);
      const v128_t vb89ABc3 = wasm_v128_load(w + 56);
      const v128_t vbCDEFc3 = wasm_v128_load(w + 60);

      vacc0x0123 = wasm_f32x4_add(vacc0x0123, wasm_f32x4_mul(va0c3, vb0123c3));
      vacc1x0123 = wasm_f32x4_add(vacc1x0123, wasm_f32x4_mul(va1c3, vb0123c3));
      vacc2x0123 = wasm_f32x4_add(vacc2x0123, wasm_f32x4_mul(va2c3, vb0123c3));
      vacc3x0123 = wasm_f32x4_add(vacc3x0123, wasm_f32x4_mul(va3c3, vb0123c3));
      vacc0x4567 = wasm_f32x4_add(vacc0x4567, wasm_f32x4_mul(va0c3, vb4567c3));
      vacc1x4567 = wasm_f32x4_add(vacc1x4567, wasm_f32x4_mul(va1c3, vb4567c3));
      vacc2x4567 = wasm_f32x4_add(vacc2x4567, wasm_f32x4_mul(va2c3, vb4567c3));
      vacc3x4567 = wasm_f32x4_add(vacc3x4567, wasm_f32x4_mul(va3c3, vb4567c3));
      vacc0x89AB = wasm_f32x4_add(vacc0x89AB, wasm_f32x4_mul(va0c3, vb89ABc3));
      vacc1x89AB = wasm_f32x4_add(vacc1x89AB, wasm_f32x4_mul(va1c3, vb89ABc3));
      vacc2x89AB = wasm_f32x4_add(vacc2x89AB, wasm_f32x4_mul(va2c3, vb89ABc3));
      vacc3x89AB = wasm_f32x4_add(vacc3x89AB, wasm_f32x4_mul(va3c3, vb89ABc3));
      vacc0xCDEF = wasm_f32x4_add(vacc0xCDEF, wasm_f32x4_mul(va0c3, vbCDEFc3));
      vacc1xCDEF = wasm_f32x4_add(vacc1xCDEF, wasm_f32x4_mul(va1c3, vbCDEFc3));
      vacc2xCDEF = wasm_f32x4_add(vacc2xCDEF, wasm_f32x4_mul(va2c3, vbCDEFc3));
      vacc3xCDEF = wasm_f32x4_add(vacc3xCDEF, wasm_f32x4_mul(va3c3, vbCDEFc3));

      w += 64;
      k -= 4 * sizeof(float);
    }
    if XNN_UNLIKELY(k != 0) {
      do {
        const v128_t va0 = wasm_v32x4_load_splat(a0);
        a0 += 1;
        const v128_t va1 = wasm_v32x4_load_splat(a1);
        a1 += 1;
        const v128_t va2 = wasm_v32x4_load_splat(a2);
        a2 += 1;
        const v128_t va3 = wasm_v32x4_load_splat(a3);
        a3 += 1;

        const v128_t vb0123 = wasm_v128_load(w);
        const v128_t vb4567 = wasm_v128_load(w + 4);
        const v128_t vb89AB = wasm_v128_load(w + 8);
        const v128_t vbCDEF = wasm_v128_load(w + 12);
        w += 16;

        vacc0x0123 = wasm_f32x4_add(vacc0x0123, wasm_f32x4_mul(va0, vb0123));
        vacc1x0123 = wasm_f32x4_add(vacc1x0123, wasm_f32x4_mul(va1, vb0123));
        vacc2x0123 = wasm_f32x4_add(vacc2x0123, wasm_f32x4_mul(va2, vb0123));
        vacc3x0123 = wasm_f32x4_add(vacc3x0123, wasm_f32x4_mul(va3, vb0123));
        vacc0x4567 = wasm_f32x4_add(vacc0x4567, wasm_f32x4_mul(va0, vb4567));
        vacc1x4567 = wasm_f32x4_add(vacc1x4567, wasm_f32x4_mul(va1, vb4567));
        vacc2x4567 = wasm_f32x4_add(vacc2x4567, wasm_f32x4_mul(va2, vb4567));
        vacc3x4567 = wasm_f32x4_add(vacc3x4567, wasm_f32x4_mul(va3, vb4567));
        vacc0x89AB = wasm_f32x4_add(vacc0x89AB, wasm_f32x4_mul(va0, vb89AB));
        vacc1x89AB = wasm_f32x4_add(vacc1x89AB, wasm_f32x4_mul(va1, vb89AB));
        vacc2x89AB = wasm_f32x4_add(vacc2x89AB, wasm_f32x4_mul(va2, vb89AB));
        vacc3x89AB = wasm_f32x4_add(vacc3x89AB, wasm_f32x4_mul(va3, vb89AB));
        vacc0xCDEF = wasm_f32x4_add(vacc0xCDEF, wasm_f32x4_mul(va0, vbCDEF));
        vacc1xCDEF = wasm_f32x4_add(vacc1xCDEF, wasm_f32x4_mul(va1, vbCDEF));
        vacc2xCDEF = wasm_f32x4_add(vacc2xCDEF, wasm_f32x4_mul(va2, vbCDEF));
        vacc3xCDEF = wasm_f32x4_add(vacc3xCDEF, wasm_f32x4_mul(va3, vbCDEF));

        k -= sizeof(float);
      } while (k != 0);
    }

    const v128_t vzero = wasm_f32x4_splat(0.0f);
    vacc0x0123 = wasm_i32x4_max(vacc0x0123, vzero);
    vacc1x0123 = wasm_i32x4_max(vacc1x0123, vzero);
    vacc2x0123 = wasm_i32x4_max(vacc2x0123, vzero);
    vacc3x0123 = wasm_i32x4_max(vacc3x0123, vzero);
    vacc0x4567 = wasm_i32x4_max(vacc0x4567, vzero);
    vacc1x4567 = wasm_i32x4_max(vacc1x4567, vzero);
    vacc2x4567 = wasm_i32x4_max(vacc2x4567, vzero);
    vacc3x4567 = wasm_i32x4_max(vacc3x4567, vzero);
    vacc0x89AB = wasm_i32x4_max(vacc0x89AB, vzero);
    vacc1x89AB = wasm_i32x4_max(vacc1x89AB, vzero);
    vacc2x89AB = wasm_i32x4_max(vacc2x89AB, vzero);
    vacc3x89AB = wasm_i32x4_max(vacc3x89AB, vzero);
    vacc0xCDEF = wasm_i32x4_max(vacc0xCDEF, vzero);
    vacc1xCDEF = wasm_i32x4_max(vacc1xCDEF, vzero);
    vacc2xCDEF = wasm_i32x4_max(vacc2xCDEF, vzero);
    vacc3xCDEF = wasm_i32x4_max(vacc3xCDEF, vzero);

    if XNN_LIKELY(nc >= 16) {
      wasm_v128_store(c3, vacc3x0123);
      wasm_v128_store(c3 + 4, vacc3x4567);
      wasm_v128_store(c3 + 8, vacc3x89AB);
      wasm_v128_store(c3 + 12, vacc3xCDEF);
      c3 = (float*) ((uintptr_t) c3 + cn_stride);
      wasm_v128_store(c2, vacc2x0123);
      wasm_v128_store(c2 + 4, vacc2x4567);
      wasm_v128_store(c2 + 8, vacc2x89AB);
      wasm_v128_store(c2 + 12, vacc2xCDEF);
      c2 = (float*) ((uintptr_t) c2 + cn_stride);
      wasm_v128_store(c1, vacc1x0123);
      wasm_v128_store(c1 + 4, vacc1x4567);
      wasm_v128_store(c1 + 8, vacc1x89AB);
      wasm_v128_store(c1 + 12, vacc1xCDEF);
      c1 = (float*) ((uintptr_t) c1 + cn_stride);
      wasm_v128_store(c0, vacc0x0123);
      wasm_v128_store(c0 + 4, vacc0x4567);
      wasm_v128_store(c0 + 8, vacc0x89AB);
      wasm_v128_store(c0 + 12, vacc0xCDEF);
      c0 = (float*) ((uintptr_t) c0 + cn_stride);

      a3 = (const float*) ((uintptr_t) a3 - kc);
      a2 = (const float*) ((uintptr_t) a2 - kc);
      a1 = (const float*) ((uintptr_t) a1 - kc);
      a0 = (const float*) ((uintptr_t) a0 - kc);

      nc -= 16;
    } else {
      if (nc & 8) {
        wasm_v128_store(c3, vacc3x0123);
        wasm_v128_store(c3 + 4, vacc3x4567);
        wasm_v128_store(c2, vacc2x0123);
        wasm_v128_store(c2 + 4, vacc2x4567);
        wasm_v128_store(c1, vacc1x0123);
        wasm_v128_store(c1 + 4, vacc1x4567);
        wasm_v128_store(c0, vacc0x0123);
        wasm_v128_store(c0 + 4, vacc0x4567);

        vacc3x0123 = vacc3x89AB;
        vacc2x0123 = vacc2x89AB;
        vacc1x0123 = vacc1x89AB;
        vacc0x0123 = vacc0x89AB;

        c3 += 8;
        c2 += 8;
        c1 += 8;
        c0 += 8;
      }
      if (nc & 4) {
        wasm_v128_store(c3, vacc3x0123);
        wasm_v128_store(c2, vacc2x0123);
        wasm_v128_store(c1, vacc1x0123);
        wasm_v128_store(c0, vacc0x0123);

        vacc3x0123 = vacc3x4567;
        vacc2x0123 = vacc2x4567;
        vacc1x0123 = vacc1x4567;
        vacc0x0123 = vacc0x4567;

        c3 += 4;
        c2 += 4;
        c1 += 4;
        c0 += 4;
      }
      if (nc & 2) {
        *((double*) c3) = wasm_f64x2_extract_lane(vacc3x0123, 0);
        *((double*) c2) = wasm_f64x2_extract_lane(vacc2x0123, 0);
        *((double*) c1) = wasm_f64x2_extract_lane(vacc1x0123, 0);
        *((double*) c0) = wasm_f64x2_extract_lane(vacc0x0123, 0);

        vacc3x0123 = wasm_v32x4_shuffle(vacc3x0123, vacc3x0123, 2, 3, 2, 3);
        vacc2x0123 = wasm_v32x4_shuffle(vacc2x0123, vacc2x0123, 2, 3, 2, 3);
        vacc1x0123 = wasm_v32x4_shuffle(vacc1x0123, vacc1x0123, 2, 3, 2, 3);
        vacc0x0123 = wasm_v32x4_shuffle(vacc0x0123, vacc0x0123, 2, 3, 2, 3);

        c3 += 2;
        c2 += 2;
        c1 += 2;
        c0 += 2;
      }
      if (nc & 1) {
        *c3 = wasm_f32x4_extract_lane(vacc3x0123, 0);
        *c2 = wasm_f32x4_extract_lane(vacc2x0123, 0);
        *c1 = wasm_f32x4_extract_lane(vacc1x0123, 0);
        *c0 = wasm_f32x4_extract_lane(vacc0x0123, 0);
      }

      nc = 0;
    }
  } while (nc != 0);
}
