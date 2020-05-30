// Auto-generated file. Do not edit!
//   Template: src/f16-dwconv/up-neonfp16arith.c.in
//   Generator: tools/xngen
//
// Copyright 2020 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <arm_neon.h>

#include <xnnpack/dwconv.h>


void xnn_f16_dwconv_minmax_ukernel_up16x8__neonfp16arith_acc2(
    size_t channels,
    size_t output_width,
    const void** input,
    const void* weights,
    void* output,
    size_t input_stride,
    size_t output_increment,
    size_t input_offset,
    const void* zero,
    const struct xnn_f16_minmax_params params[restrict XNN_MIN_ELEMENTS(1)]) XNN_DISABLE_TSAN
{
  assert(channels != 0);
  assert(output_width != 0);

  const float16x8_t vmax = vld1q_dup_f16(&params->max);
  const float16x8_t vmin = vld1q_dup_f16(&params->min);
  do {
    const __fp16* i0 = input[0];
    assert(i0 != NULL);
    if XNN_UNPREDICTABLE(i0 != zero) {
      i0 = (const __fp16*) ((uintptr_t) i0 + input_offset);
    }
    const __fp16* i1 = input[1];
    assert(i1 != NULL);
    if XNN_UNPREDICTABLE(i1 != zero) {
      i1 = (const __fp16*) ((uintptr_t) i1 + input_offset);
    }
    const __fp16* i2 = input[2];
    assert(i2 != NULL);
    if XNN_UNPREDICTABLE(i2 != zero) {
      i2 = (const __fp16*) ((uintptr_t) i2 + input_offset);
    }
    const __fp16* i3 = input[3];
    assert(i3 != NULL);
    if XNN_UNPREDICTABLE(i3 != zero) {
      i3 = (const __fp16*) ((uintptr_t) i3 + input_offset);
    }
    const __fp16* i4 = input[4];
    assert(i4 != NULL);
    if XNN_UNPREDICTABLE(i4 != zero) {
      i4 = (const __fp16*) ((uintptr_t) i4 + input_offset);
    }
    const __fp16* i5 = input[5];
    assert(i5 != NULL);
    if XNN_UNPREDICTABLE(i5 != zero) {
      i5 = (const __fp16*) ((uintptr_t) i5 + input_offset);
    }
    const __fp16* i6 = input[6];
    assert(i6 != NULL);
    if XNN_UNPREDICTABLE(i6 != zero) {
      i6 = (const __fp16*) ((uintptr_t) i6 + input_offset);
    }
    const __fp16* i7 = input[7];
    assert(i7 != NULL);
    if XNN_UNPREDICTABLE(i7 != zero) {
      i7 = (const __fp16*) ((uintptr_t) i7 + input_offset);
    }

    input = (const void**) ((uintptr_t) input + input_stride);

    size_t c = channels;
    const __fp16* w = weights;
    for (; c >= 16; c -= 16) {
      float16x8_t vacc01234567p0 = vld1q_f16(w); w += 8;
      float16x8_t vacc456789ABp0 = vld1q_f16(w); w += 8;


      const float16x8_t vi0x01234567 = vld1q_f16(i0); i0 += 8;
      const float16x8_t vi0x456789AB = vld1q_f16(i0); i0 += 8;
      const float16x8_t vk0x01234567 = vld1q_f16(w); w += 8;
      const float16x8_t vk0x456789AB = vld1q_f16(w); w += 8;
      vacc01234567p0 = vfmaq_f16(vacc01234567p0, vi0x01234567, vk0x01234567);
      vacc456789ABp0 = vfmaq_f16(vacc456789ABp0, vi0x456789AB, vk0x456789AB);

      const float16x8_t vi1x01234567 = vld1q_f16(i1); i1 += 8;
      const float16x8_t vi1x456789AB = vld1q_f16(i1); i1 += 8;
      const float16x8_t vk1x01234567 = vld1q_f16(w); w += 8;
      const float16x8_t vk1x456789AB = vld1q_f16(w); w += 8;
      float16x8_t vacc01234567p1 = vmulq_f16(vi1x01234567, vk1x01234567);
      float16x8_t vacc456789ABp1 = vmulq_f16(vi1x456789AB, vk1x456789AB);

      const float16x8_t vi2x01234567 = vld1q_f16(i2); i2 += 8;
      const float16x8_t vi2x456789AB = vld1q_f16(i2); i2 += 8;
      const float16x8_t vk2x01234567 = vld1q_f16(w); w += 8;
      const float16x8_t vk2x456789AB = vld1q_f16(w); w += 8;
      vacc01234567p0 = vfmaq_f16(vacc01234567p0, vi2x01234567, vk2x01234567);
      vacc456789ABp0 = vfmaq_f16(vacc456789ABp0, vi2x456789AB, vk2x456789AB);

      const float16x8_t vi3x01234567 = vld1q_f16(i3); i3 += 8;
      const float16x8_t vi3x456789AB = vld1q_f16(i3); i3 += 8;
      const float16x8_t vk3x01234567 = vld1q_f16(w); w += 8;
      const float16x8_t vk3x456789AB = vld1q_f16(w); w += 8;
      vacc01234567p1 = vfmaq_f16(vacc01234567p1, vi3x01234567, vk3x01234567);
      vacc456789ABp1 = vfmaq_f16(vacc456789ABp1, vi3x456789AB, vk3x456789AB);

      const float16x8_t vi4x01234567 = vld1q_f16(i4); i4 += 8;
      const float16x8_t vi4x456789AB = vld1q_f16(i4); i4 += 8;
      const float16x8_t vk4x01234567 = vld1q_f16(w); w += 8;
      const float16x8_t vk4x456789AB = vld1q_f16(w); w += 8;
      vacc01234567p0 = vfmaq_f16(vacc01234567p0, vi4x01234567, vk4x01234567);
      vacc456789ABp0 = vfmaq_f16(vacc456789ABp0, vi4x456789AB, vk4x456789AB);

      const float16x8_t vi5x01234567 = vld1q_f16(i5); i5 += 8;
      const float16x8_t vi5x456789AB = vld1q_f16(i5); i5 += 8;
      const float16x8_t vk5x01234567 = vld1q_f16(w); w += 8;
      const float16x8_t vk5x456789AB = vld1q_f16(w); w += 8;
      vacc01234567p1 = vfmaq_f16(vacc01234567p1, vi5x01234567, vk5x01234567);
      vacc456789ABp1 = vfmaq_f16(vacc456789ABp1, vi5x456789AB, vk5x456789AB);

      const float16x8_t vi6x01234567 = vld1q_f16(i6); i6 += 8;
      const float16x8_t vi6x456789AB = vld1q_f16(i6); i6 += 8;
      const float16x8_t vk6x01234567 = vld1q_f16(w); w += 8;
      const float16x8_t vk6x456789AB = vld1q_f16(w); w += 8;
      vacc01234567p0 = vfmaq_f16(vacc01234567p0, vi6x01234567, vk6x01234567);
      vacc456789ABp0 = vfmaq_f16(vacc456789ABp0, vi6x456789AB, vk6x456789AB);

      const float16x8_t vi7x01234567 = vld1q_f16(i7); i7 += 8;
      const float16x8_t vi7x456789AB = vld1q_f16(i7); i7 += 8;
      const float16x8_t vk7x01234567 = vld1q_f16(w); w += 8;
      const float16x8_t vk7x456789AB = vld1q_f16(w); w += 8;
      vacc01234567p1 = vfmaq_f16(vacc01234567p1, vi7x01234567, vk7x01234567);
      vacc456789ABp1 = vfmaq_f16(vacc456789ABp1, vi7x456789AB, vk7x456789AB);

      // Add up all accumulators to vacc01234567456789ABp0
      vacc01234567p0 = vaddq_f16(vacc01234567p0, vacc01234567p1);
      vacc456789ABp0 = vaddq_f16(vacc456789ABp0, vacc456789ABp1);

      float16x8_t vacc01234567 = vmaxq_f16(vacc01234567p0, vmin);
      float16x8_t vacc456789AB = vmaxq_f16(vacc456789ABp0, vmin);
      vacc01234567 = vminq_f16(vacc01234567, vmax);
      vacc456789AB = vminq_f16(vacc456789AB, vmax);

      vst1q_f16(output, vacc01234567); output += 8;
      vst1q_f16(output, vacc456789AB); output += 8;
    }
    for (; c >= 8; c -= 8) {
      float16x8_t vacc01234567p0 = vld1q_f16(w); w += 8;


      const float16x8_t vi0x01234567 = vld1q_f16(i0); i0 += 8;
      const float16x8_t vk0x01234567 = vld1q_f16(w + 8);
      vacc01234567p0 = vfmaq_f16(vacc01234567p0, vi0x01234567, vk0x01234567);

      const float16x8_t vi1x01234567 = vld1q_f16(i1); i1 += 8;
      const float16x8_t vk1x01234567 = vld1q_f16(w + 24);
      float16x8_t vacc01234567p1 = vmulq_f16(vi1x01234567, vk1x01234567);

      const float16x8_t vi2x01234567 = vld1q_f16(i2); i2 += 8;
      const float16x8_t vk2x01234567 = vld1q_f16(w + 40);
      vacc01234567p0 = vfmaq_f16(vacc01234567p0, vi2x01234567, vk2x01234567);

      const float16x8_t vi3x01234567 = vld1q_f16(i3); i3 += 8;
      const float16x8_t vk3x01234567 = vld1q_f16(w + 56);
      vacc01234567p1 = vfmaq_f16(vacc01234567p1, vi3x01234567, vk3x01234567);

      const float16x8_t vi4x01234567 = vld1q_f16(i4); i4 += 8;
      const float16x8_t vk4x01234567 = vld1q_f16(w + 72);
      vacc01234567p0 = vfmaq_f16(vacc01234567p0, vi4x01234567, vk4x01234567);

      const float16x8_t vi5x01234567 = vld1q_f16(i5); i5 += 8;
      const float16x8_t vk5x01234567 = vld1q_f16(w + 88);
      vacc01234567p1 = vfmaq_f16(vacc01234567p1, vi5x01234567, vk5x01234567);

      const float16x8_t vi6x01234567 = vld1q_f16(i6); i6 += 8;
      const float16x8_t vk6x01234567 = vld1q_f16(w + 104);
      vacc01234567p0 = vfmaq_f16(vacc01234567p0, vi6x01234567, vk6x01234567);

      const float16x8_t vi7x01234567 = vld1q_f16(i7); i7 += 8;
      const float16x8_t vk7x01234567 = vld1q_f16(w + 120);
      vacc01234567p1 = vfmaq_f16(vacc01234567p1, vi7x01234567, vk7x01234567);

      // Add up all accumulators to vacc01234567p0
      vacc01234567p0 = vaddq_f16(vacc01234567p0, vacc01234567p1);

      float16x8_t vacc01234567 = vmaxq_f16(vacc01234567p0, vmin);
      vacc01234567 = vminq_f16(vacc01234567, vmax);

      vst1q_f16(output, vacc01234567); output += 8;
    }
    if XNN_UNLIKELY(c != 0) {
      float16x8_t vacc01234567p0 = vld1q_f16(w);


      const float16x8_t vi0x01234567 = vld1q_f16(i0);
      const float16x8_t vk0x01234567 = vld1q_f16(w + 16);
      vacc01234567p0 = vfmaq_f16(vacc01234567p0, vi0x01234567, vk0x01234567);

      const float16x8_t vi1x01234567 = vld1q_f16(i1);
      const float16x8_t vk1x01234567 = vld1q_f16(w + 32);
      float16x8_t vacc01234567p1 = vmulq_f16(vi1x01234567, vk1x01234567);

      const float16x8_t vi2x01234567 = vld1q_f16(i2);
      const float16x8_t vk2x01234567 = vld1q_f16(w + 48);
      vacc01234567p0 = vfmaq_f16(vacc01234567p0, vi2x01234567, vk2x01234567);

      const float16x8_t vi3x01234567 = vld1q_f16(i3);
      const float16x8_t vk3x01234567 = vld1q_f16(w + 64);
      vacc01234567p1 = vfmaq_f16(vacc01234567p1, vi3x01234567, vk3x01234567);

      const float16x8_t vi4x01234567 = vld1q_f16(i4);
      const float16x8_t vk4x01234567 = vld1q_f16(w + 80);
      vacc01234567p0 = vfmaq_f16(vacc01234567p0, vi4x01234567, vk4x01234567);

      const float16x8_t vi5x01234567 = vld1q_f16(i5);
      const float16x8_t vk5x01234567 = vld1q_f16(w + 96);
      vacc01234567p1 = vfmaq_f16(vacc01234567p1, vi5x01234567, vk5x01234567);

      const float16x8_t vi6x01234567 = vld1q_f16(i6);
      const float16x8_t vk6x01234567 = vld1q_f16(w + 112);
      vacc01234567p0 = vfmaq_f16(vacc01234567p0, vi6x01234567, vk6x01234567);

      const float16x8_t vi7x01234567 = vld1q_f16(i7);
      const float16x8_t vk7x01234567 = vld1q_f16(w + 128);
      vacc01234567p1 = vfmaq_f16(vacc01234567p1, vi7x01234567, vk7x01234567);

      // Add up all accumulators to vacc01234567p0
      vacc01234567p0 = vaddq_f16(vacc01234567p0, vacc01234567p1);

      float16x8_t vacc01234567 = vmaxq_f16(vacc01234567p0, vmin);
      vacc01234567 = vminq_f16(vacc01234567, vmax);

      float16x4_t vacc0123 = vget_low_f16(vacc01234567);
      if (c & 4) {
        vst1_f16(output, vacc0123); output += 4;
        vacc0123 = vget_high_f16(vacc01234567);
      }
      if (c & 1) {
        vst1_lane_f16(output, vacc0123, 0); output += 1;
      }
    }

    output = (__fp16*) ((uintptr_t) output + output_increment);
  } while (--output_width != 0);
}
