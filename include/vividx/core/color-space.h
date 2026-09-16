/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * 2026, Aspen Schneider.
 */
#ifndef VIVIDX_CORE_COLOR_SPACE_H
#define VIVIDX_CORE_COLOR_SPACE_H

#include <vividx/common.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct vx_color_space_t vx_color_space_t;


/// Colorspace with the sRGB primaries, but a linear (1.0) gamma.
VX_PUBLIC vx_color_space_t* vx_color_space_new_srgb();

/// Create an SkColorSpace from a transfer function and
// a row-major 3x3 transformation to XYZ.
VX_PUBLIC vx_color_space_t* vx_color_space_new_srgb_linear();

// TODO: Remove.
VX_PUBLIC void* vx_color_space_cxx_ptr(const vx_color_space_t *cs);

VX_PUBLIC void vx_color_space_free(vx_color_space_t *cs);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* VIVIDX_CORE_COLOR_SPACE_H */
