/*
 * Copyright 2017 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkImageInfoPriv_DEFINED
#define SkImageInfoPriv_DEFINED

#include "include/core/SkColor.h"
#include <vividx/core/color-type.h>
#include "include/core/SkImageInfo.h"


static inline int SkColorTypeNumChannels(vx_color_type ct) {
    switch (vx_color_type_channel_flags(ct)) {
        case VX_COLOR_CHANNEL_FLAG_RED        : return 1;
        case VX_COLOR_CHANNEL_FLAG_ALPHA      : return 1;
        case VX_COLOR_CHANNEL_FLAG_GRAY       : return 1;
        case VX_COLOR_CHANNEL_FLAGS_GRAY_ALPHA : return 2;
        case VX_COLOR_CHANNEL_FLAGS_RG        : return 2;
        case VX_COLOR_CHANNEL_FLAGS_RGB       : return 3;
        case VX_COLOR_CHANNEL_FLAGS_RGBA      : return 4;
        case 0                              : return 0;
        default:
           SkDEBUGFAIL("unexpected color channel flags");
           return 0;
    }
    SkUNREACHABLE;
}

static inline bool SkColorTypeIsAlphaOnly(vx_color_type ct) {
    return vx_color_type_channel_flags(ct) == VX_COLOR_CHANNEL_FLAG_ALPHA;
}

static inline bool SkAlphaTypeIsValid(unsigned value) {
    return value <= VX_ALPHA_TYPE_LASTENUM;
}

static int SkColorTypeShiftPerPixel(vx_color_type ct) {
    switch (ct) {
        case VX_COLOR_TYPE_UNKNOWN:            return 0;
        case VX_COLOR_TYPE_ALPHA_8:            return 0;
        case VX_COLOR_TYPE_RGB_565:            return 1;
        case VX_COLOR_TYPE_ARGB_4444:          return 1;
        case VX_COLOR_TYPE_RGBA_8888:          return 2;
        case VX_COLOR_TYPE_RGB_888X:           return 2;
        case VX_COLOR_TYPE_BGRA_8888:          return 2;
        case VX_COLOR_TYPE_RGBA_1010102:       return 2;
        case VX_COLOR_TYPE_RGB_101010X:        return 2;
        case VX_COLOR_TYPE_BGRA_1010102:       return 2;
        case VX_COLOR_TYPE_BGR_101010X:        return 2;
        case VX_COLOR_TYPE_BGR_101010X_XR:     return 2;
        case VX_COLOR_TYPE_BGRA_10101010_XR:   return 3;
        case VX_COLOR_TYPE_RGBA_10X6:          return 3;
        case VX_COLOR_TYPE_GRAY_8:             return 0;
        case VX_COLOR_TYPE_RGBA_F16NORM:       return 3;
        case VX_COLOR_TYPE_RGBA_F16:           return 3;
        case VX_COLOR_TYPE_RGB_F16F16F16X:     return 3;
        case VX_COLOR_TYPE_RGBA_F32:           return 4;
        case VX_COLOR_TYPE_R8G8_UNORM:         return 1;
        case VX_COLOR_TYPE_A16_UNORM:          return 1;
        case VX_COLOR_TYPE_R16_UNORM:          return 1;
        case VX_COLOR_TYPE_R16G16_UNORM:       return 2;
        case VX_COLOR_TYPE_A16_FLOAT:          return 1;
        case VX_COLOR_TYPE_R16_FLOAT:          return 1;
        case VX_COLOR_TYPE_R16G16_FLOAT:       return 2;
        case VX_COLOR_TYPE_R16G16B16A16_UNORM: return 3;
        case VX_COLOR_TYPE_SRGBA_8888:         return 2;
        case VX_COLOR_TYPE_R8_UNORM:           return 0;
    }
    SkUNREACHABLE;
}

static inline size_t SkColorTypeMinRowBytes(vx_color_type ct, int width) {
    return (size_t)(width * vx_color_type_bytes_per_pixel(ct));
}

static inline bool SkColorTypeIsValid(unsigned value) {
    return value <= VX_COLOR_TYPE_LASTENUM;
}

static inline size_t SkColorTypeComputeOffset(vx_color_type ct, int x, int y, size_t rowBytes) {
    SkASSERT(x >= 0);
    SkASSERT(y >= 0);
    if (VX_COLOR_TYPE_UNKNOWN == ct) {
        return 0;
    }
    return (size_t)y * rowBytes + ((size_t)x << SkColorTypeShiftPerPixel(ct));
}

static inline bool SkColorTypeIsNormalized(vx_color_type ct) {
    switch (ct) {
        case VX_COLOR_TYPE_UNKNOWN:
        case VX_COLOR_TYPE_ALPHA_8:
        case VX_COLOR_TYPE_RGB_565:
        case VX_COLOR_TYPE_ARGB_4444:
        case VX_COLOR_TYPE_RGBA_8888:
        case VX_COLOR_TYPE_RGB_888X:
        case VX_COLOR_TYPE_BGRA_8888:
        case VX_COLOR_TYPE_RGBA_1010102:
        case VX_COLOR_TYPE_RGB_101010X:
        case VX_COLOR_TYPE_BGRA_1010102:
        case VX_COLOR_TYPE_BGR_101010X:
        case VX_COLOR_TYPE_RGBA_10X6:
        case VX_COLOR_TYPE_GRAY_8:
        case VX_COLOR_TYPE_RGBA_F16NORM:
        case VX_COLOR_TYPE_R8G8_UNORM:
        case VX_COLOR_TYPE_A16_UNORM:
        case VX_COLOR_TYPE_A16_FLOAT:          /*subtle... alpha is always [0,1]*/
        case VX_COLOR_TYPE_R16_UNORM:
        case VX_COLOR_TYPE_R16G16_UNORM:
        case VX_COLOR_TYPE_R16G16B16A16_UNORM:
        case VX_COLOR_TYPE_SRGBA_8888:
        case VX_COLOR_TYPE_R8_UNORM:
            return true;

        case VX_COLOR_TYPE_BGRA_10101010_XR:
        case VX_COLOR_TYPE_BGR_101010X_XR:
        case VX_COLOR_TYPE_RGB_F16F16F16X:
        case VX_COLOR_TYPE_RGBA_F16:
        case VX_COLOR_TYPE_RGBA_F32:
        case VX_COLOR_TYPE_R16_FLOAT:
        case VX_COLOR_TYPE_R16G16_FLOAT:
            return false;
    }
    SkUNREACHABLE;
}

static inline int SkColorTypeMaxBitsPerChannel(vx_color_type ct) {
    switch (ct) {
        case VX_COLOR_TYPE_UNKNOWN:
            return 0;

        case VX_COLOR_TYPE_ARGB_4444:
            return 4;

        case VX_COLOR_TYPE_RGB_565:
            return 6;

        case VX_COLOR_TYPE_ALPHA_8:
        case VX_COLOR_TYPE_RGBA_8888:
        case VX_COLOR_TYPE_RGB_888X:
        case VX_COLOR_TYPE_BGRA_8888:
        case VX_COLOR_TYPE_GRAY_8:
        case VX_COLOR_TYPE_R8G8_UNORM:
        case VX_COLOR_TYPE_SRGBA_8888:
        case VX_COLOR_TYPE_R8_UNORM:
            return 8;

        case VX_COLOR_TYPE_RGBA_1010102:
        case VX_COLOR_TYPE_RGB_101010X:
        case VX_COLOR_TYPE_BGRA_1010102:
        case VX_COLOR_TYPE_BGR_101010X:
        case VX_COLOR_TYPE_BGR_101010X_XR:
        case VX_COLOR_TYPE_BGRA_10101010_XR:
        case VX_COLOR_TYPE_RGBA_10X6:
            return 10;

        case VX_COLOR_TYPE_RGBA_F16NORM:
        case VX_COLOR_TYPE_A16_UNORM:
        case VX_COLOR_TYPE_A16_FLOAT:
        case VX_COLOR_TYPE_R16_UNORM:
        case VX_COLOR_TYPE_R16_FLOAT:
        case VX_COLOR_TYPE_R16G16_UNORM:
        case VX_COLOR_TYPE_R16G16B16A16_UNORM:
        case VX_COLOR_TYPE_RGBA_F16:
        case VX_COLOR_TYPE_RGB_F16F16F16X:
        case VX_COLOR_TYPE_R16G16_FLOAT:
            return 16;

        case VX_COLOR_TYPE_RGBA_F32:
            return 32;
    }
    SkUNREACHABLE;
}

/**
 *  Returns true if |info| contains a valid colorType and alphaType.
 */
static inline bool SkColorInfoIsValid(const SkColorInfo& info) {
    return info.colorType() != VX_COLOR_TYPE_UNKNOWN && info.alphaType() != VX_ALPHA_TYPE_UNKNOWN;
}

/**
 *  Returns true if |info| contains a valid combination of width, height and colorInfo.
 */
static inline bool SkImageInfoIsValid(const SkImageInfo& info) {
    if (info.width() <= 0 || info.height() <= 0) {
        return false;
    }

    const int kMaxDimension = SK_MaxS32 >> 2;
    if (info.width() > kMaxDimension || info.height() > kMaxDimension) {
        return false;
    }

    return SkColorInfoIsValid(info.colorInfo());
}

/**
 *  Returns true if Skia has defined a pixel conversion from the |src| to the |dst|.
 *  Returns false otherwise.
 */
static inline bool SkImageInfoValidConversion(const SkImageInfo& dst, const SkImageInfo& src) {
    return SkImageInfoIsValid(dst) && SkImageInfoIsValid(src);
}
#endif  // SkImageInfoPriv_DEFINED
