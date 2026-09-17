/*
 * Copyright 2006 The Android Open Source Project
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * 2026, Aspen Schneider.
 */

#ifndef VIVIDX_CORE_COLOR_H
#define VIVIDX_CORE_COLOR_H

#include <assert.h>

#include <vividx/common.h>
#include <vividx/core/alpha-type.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef uint8_t vx_alpha_t;
typedef uint32_t vx_color32_t;
typedef struct vx_color_t vx_color_t;

/// \enum vx_color_channel
///
/// Describes different color channels one can manipulate
enum vx_color_channel {
    VX_COLOR_CHANNEL_R,  // the red channel
    VX_COLOR_CHANNEL_G,  // the green channel
    VX_COLOR_CHANNEL_B,  // the blue channel
    VX_COLOR_CHANNEL_A,  // the alpha channel

    VX_COLOR_CHANNEL_LASTENUM = VX_COLOR_CHANNEL_A,
};

/// \enum vx_color_channel_flag
///
/// Used to represent the channels available in a color type or texture format
/// as a mask.
enum vx_color_channel_flag {
    VX_COLOR_CHANNEL_FLAG_RED    = 1 << VX_COLOR_CHANNEL_R,
    VX_COLOR_CHANNEL_FLAG_GREEN  = 1 << VX_COLOR_CHANNEL_G,
    VX_COLOR_CHANNEL_FLAG_BLUE   = 1 << VX_COLOR_CHANNEL_B,
    VX_COLOR_CHANNEL_FLAG_ALPHA  = 1 << VX_COLOR_CHANNEL_A,
    VX_COLOR_CHANNEL_FLAG_GRAY   = 0x10,
    // Convenience values
    VX_COLOR_CHANNEL_FLAGS_GRAY_ALPHA = VX_COLOR_CHANNEL_FLAG_GRAY | VX_COLOR_CHANNEL_FLAG_ALPHA,
    VX_COLOR_CHANNEL_FLAGS_RG        = VX_COLOR_CHANNEL_FLAG_RED | VX_COLOR_CHANNEL_FLAG_GREEN,
    VX_COLOR_CHANNEL_FLAGS_RGB       = VX_COLOR_CHANNEL_FLAGS_RG | VX_COLOR_CHANNEL_FLAG_BLUE,
    VX_COLOR_CHANNEL_FLAGS_RGBA      = VX_COLOR_CHANNEL_FLAGS_RGB | VX_COLOR_CHANNEL_FLAG_ALPHA,
};
static_assert(0 == (VX_COLOR_CHANNEL_FLAG_GRAY & VX_COLOR_CHANNEL_FLAGS_RGBA), "bitfield conflict");

typedef enum vx_color_channel_flag vx_color_channel_flags;


struct vx_color_t {
    float r;
    float g;
    float b;
    float a;
};

static vx_color_t vx_color_make(float r, float g, float b, float a)
{
    vx_color_t color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
    return color;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* VIVIDX_CORE_COLOR_H */
