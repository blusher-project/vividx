/*
 * Copyright 2023 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "src/gpu/DitherUtils.h"

#ifndef SK_IGNORE_GPU_DITHER

#include "include/core/SkBitmap.h"
#include <vividx/core/color-type.h>
#include "include/core/SkImageInfo.h"

#include <cstdint>

namespace skgpu {

float DitherRangeForConfig(vx_color_type dstColorType) {
    SkASSERT(dstColorType != VX_COLOR_TYPE_UNKNOWN);

    // We use 1 / (2^bitdepth-1) as the range since each channel can hold 2^bitdepth values
    switch (dstColorType) {
        // 4 bit
        case VX_COLOR_TYPE_ARGB_4444:
            return 1 / 15.f;

        // 6 bit
        case VX_COLOR_TYPE_RGB_565:
            return 1 / 63.f;

        // 8 bit
        case VX_COLOR_TYPE_ALPHA_8:
        case VX_COLOR_TYPE_GRAY_8:
        case VX_COLOR_TYPE_R8_UNORM:
        case VX_COLOR_TYPE_R8G8_UNORM:
        case VX_COLOR_TYPE_RGB_888X:
        case VX_COLOR_TYPE_RGBA_8888:
        case VX_COLOR_TYPE_SRGBA_8888:
        case VX_COLOR_TYPE_BGRA_8888:
            return 1 / 255.f;

        // 10 bit
        case VX_COLOR_TYPE_RGBA_1010102:
        case VX_COLOR_TYPE_BGRA_1010102:
        case VX_COLOR_TYPE_RGB_101010X:
        case VX_COLOR_TYPE_BGR_101010X:
        case VX_COLOR_TYPE_BGR_101010X_XR:
        case VX_COLOR_TYPE_BGRA_10101010_XR:
        case VX_COLOR_TYPE_RGBA_10X6:
            return 1 / 1023.f;

        // 16 bit
        case VX_COLOR_TYPE_A16_UNORM:
        case VX_COLOR_TYPE_R16_UNORM:
        case VX_COLOR_TYPE_R16G16_UNORM:
        case VX_COLOR_TYPE_R16G16B16A16_UNORM:
            return 1 / 32767.f;

        // Unknown
        case VX_COLOR_TYPE_UNKNOWN:
        // Half
        case VX_COLOR_TYPE_A16_FLOAT:
        case VX_COLOR_TYPE_R16_FLOAT:
        case VX_COLOR_TYPE_R16G16_FLOAT:
        case VX_COLOR_TYPE_RGBA_F16:
        case VX_COLOR_TYPE_RGB_F16F16F16X:
        case VX_COLOR_TYPE_RGBA_F16NORM:
        // Float
        case VX_COLOR_TYPE_RGBA_F32:
            return 0.f; // no dithering
    }
    SkUNREACHABLE;
}

SkBitmap MakeDitherLUT() {
    static constexpr struct DitherTable {
        constexpr DitherTable() : data() {
            constexpr int kImgSize = 8; // if changed, also change value in sk_dither_shader

            for (int x = 0; x < kImgSize; ++x) {
                for (int y = 0; y < kImgSize; ++y) {
                    // The computation of 'm' and 'value' is lifted from CPU backend.
                    unsigned int m = (y & 1) << 5 | (x & 1) << 4 |
                                     (y & 2) << 2 | (x & 2) << 1 |
                                     (y & 4) >> 1 | (x & 4) >> 2;
                    float value = static_cast<float>(m) * (1.0f / 64.0f) - (63.0f / 128.0f);
                    // Bias by 0.5 to be in 0..1, mul by 255 and round to nearest int to make byte.
                    data[y * 8 + x] = (uint8_t)((value + 0.5f) * 255.f + 0.5f);
                }
            }
        }
        uint8_t data[64];
    } gTable;

    SkBitmap bmp;
    bmp.installPixels(SkImageInfo::MakeA8(8, 8), const_cast<uint8_t*>(gTable.data), 8);
    bmp.setImmutable();
    return bmp;
}

}  // namespace skgpu

#endif // SK_IGNORE_GPU_DITHER
