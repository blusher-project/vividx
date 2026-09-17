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

#include <skcms/skcms.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct vx_color_space_t vx_color_space_t;


/// Colorspace with the sRGB primaries, but a linear (1.0) gamma.
VX_PUBLIC vx_color_space_t* vx_color_space_new_srgb();

/// Create an SkColorSpace from a transfer function and
// a row-major 3x3 transformation to XYZ.
VX_PUBLIC vx_color_space_t* vx_color_space_new_srgb_linear();

/// Create an SkColorSpace from a transfer function and a row-major 3x3 transformation to XYZ.
VX_PUBLIC vx_color_space_t* vx_color_space_new_rgb(
    const skcms_TransferFunction transferFn,
    const skcms_Matrix3x3 toXYZ);

// TODO: Remove.
VX_PUBLIC void* vx_color_space_cxx_ptr(const vx_color_space_t *cs);

VX_PUBLIC bool vx_color_space_gamma_close_to_srgb(const vx_color_space_t *cs);

VX_PUBLIC bool vx_color_space_gamma_is_linear(const vx_color_space_t *cs);

VX_PUBLIC bool vx_color_space_eq(const vx_color_space_t*,
                                 const vx_color_space_t*);

// DEPRECATED: Remove when webview usage is gone
[[deprecated]]
VX_PUBLIC void vx_color_space_transfer_fn_gabcdef(const vx_color_space_t *cs,
                                                  float gabcdef[7]);

VX_PUBLIC void vx_color_space_transfer_fn(const vx_color_space_t *cs,
                                          skcms_TransferFunction *fn);

VX_PUBLIC void vx_color_space_inv_transfer_fn(const vx_color_space_t *cs,
                                              skcms_TransferFunction* fn);

VX_PUBLIC void vx_color_space_gamut_transform_to(const vx_color_space_t *cs,
                                                 const vx_color_space_t *dst,
                                                 skcms_Matrix3x3 *src_to_dst);

VX_PUBLIC uint32_t vx_color_space_transfer_fn_hash(const vx_color_space_t *cs);

VX_PUBLIC uint64_t vx_color_space_hash(const vx_color_space_t *cs);

VX_PUBLIC void vx_color_space_compute_lazy_dst_fields(
    const vx_color_space_t *cs);

VX_PUBLIC void vx_color_space_free(vx_color_space_t *cs);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* VIVIDX_CORE_COLOR_SPACE_H */
