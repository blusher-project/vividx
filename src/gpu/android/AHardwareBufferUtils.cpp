/*
 * Copyright 2023 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "include/android/AHardwareBufferUtils.h"

#if __ANDROID_API__ >= 26

#include <android/hardware_buffer.h>

namespace AHardwareBufferUtils {

vx_color_type GetSkColorTypeFromBufferFormat(uint32_t bufferFormat) {
    switch (bufferFormat) {
        case AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNORM:
            return VX_COLOR_TYPE_RGBA_8888;
        case AHARDWAREBUFFER_FORMAT_R8G8B8X8_UNORM:
            return VX_COLOR_TYPE_RGB_888X;
#if __ANDROID_API__ >= 34
        case AHARDWAREBUFFER_FORMAT_R10G10B10A10_UNORM:
            return VX_COLOR_TYPE_RGBA_10X6;
#endif
        case AHARDWAREBUFFER_FORMAT_R16G16B16A16_FLOAT:
            return VX_COLOR_TYPE_RGBA_F16;
        case AHARDWAREBUFFER_FORMAT_R5G6B5_UNORM:
            return VX_COLOR_TYPE_RGB_565;
        case AHARDWAREBUFFER_FORMAT_R8G8B8_UNORM:
            return VX_COLOR_TYPE_RGB_888X;
        case AHARDWAREBUFFER_FORMAT_R10G10B10A2_UNORM:
            return VX_COLOR_TYPE_RGBA_1010102;
#if __ANDROID_API__ >= 33
        case AHARDWAREBUFFER_FORMAT_R8_UNORM:
            return VX_COLOR_TYPE_ALPHA_8;
#endif
        default:
            // Given that we only use this texture as a source, colorType will not impact how Skia
            // uses the texture. The only potential affect this is anticipated to have is that for
            // some format types if we are not bound as an OES texture we may get invalid results
            // for SKP capture if we read back the texture.
            return kExternalFormatColorType;
    }
}

}  // namespace AHardwareBufferUtils

#endif
