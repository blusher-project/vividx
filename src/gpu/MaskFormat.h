/*
 * Copyright 2026 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef skgpu_MaskFormat_DEFINED
#define skgpu_MaskFormat_DEFINED

#include <vividx/core/color-type.h>

namespace skgpu {

/**
 *  Formats for masks, used by the font cache. Important that these are 0-based.
 */
enum class MaskFormat : int {
    kA8,    //!< 1-byte per pixel
    kA565,  //!< 2-bytes per pixel, RGB represent 3-channel LCD coverage
    kARGB,  //!< 4-bytes per pixel, color format

    kLast = kARGB
};
static const int kMaskFormatCount = static_cast<int>(MaskFormat::kLast) + 1;

/**
 *  Return the number of bytes-per-pixel for the specified mask format.
 */
constexpr int MaskFormatBytesPerPixel(MaskFormat format) {
    switch (format) {
        case MaskFormat::kA8:   return 1;
        case MaskFormat::kA565: return 2;
        case MaskFormat::kARGB: return 4;
    }
    SkUNREACHABLE;
}

static constexpr vx_color_type MaskFormatToColorType(MaskFormat format) {
    switch (format) {
        case MaskFormat::kA8:   return VX_COLOR_TYPE_ALPHA_8;
        case MaskFormat::kA565: return VX_COLOR_TYPE_RGB_565;
        case MaskFormat::kARGB: return VX_COLOR_TYPE_RGBA_8888;
    }
    SkUNREACHABLE;
}

}  // namespace skgpu

#endif  // skgpu_MaskFormat_DEFINED
