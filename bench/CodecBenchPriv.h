/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef CodecBenchPriv_DEFINED
#define CodecBenchPriv_DEFINED

#include "include/core/SkImageInfo.h"

inline const char* color_type_to_str(vx_color_type colorType) {
    switch (colorType) {
        case VX_COLOR_TYPE_N32:
            return "N32";
        case VX_COLOR_TYPE_RGB_565:
            return "565";
        case VX_COLOR_TYPE_GRAY_8:
            return "Gray8";
        case VX_COLOR_TYPE_ALPHA_8:
            return "Alpha8";
        default:
            return "Unknown";
    }
}

inline const char* alpha_type_to_str(vx_alpha_type alphaType) {
    switch (alphaType) {
        case VX_ALPHA_TYPE_OPAQUE:
            return "";
        case VX_ALPHA_TYPE_PREMULTIPLIED:
            return "Premul";
        case VX_ALPHA_TYPE_UNPREMULTIPLIED:
            return "Unpremul";
        default:
            SkASSERT(false);
            return "Unknown";
    }
}

#endif // CodecBenchPriv_DEFINED
