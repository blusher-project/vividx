#ifndef VIVIDX_CORE_CONVERTS_H
#define VIVIDX_CORE_CONVERTS_H

#include <vividx/core/color-type.h>

#include "include/core/SkColorType.h"

vx_color_type vx_color_type_to_sk(enum vx_color_type type)
{
    switch (type) {
    case VX_COLOR_TYPE_UNKNOWN:
        return VX_COLOR_TYPE_UNKNOWN;
    case VX_COLOR_TYPE_ALPHA_8:
        return VX_COLOR_TYPE_ALPHA_8;
    case VX_COLOR_TYPE_RGB_565:
        return VX_COLOR_TYPE_RGB_565;
    case VX_COLOR_TYPE_ARGB_4444:
        return VX_COLOR_TYPE_ARGB_4444;
    case VX_COLOR_TYPE_RGBA_8888:
        return VX_COLOR_TYPE_RGBA_8888;
    case VX_COLOR_TYPE_RGB_888X:
        return VX_COLOR_TYPE_RGB_888X;
    case VX_COLOR_TYPE_BGRA_8888:
        return VX_COLOR_TYPE_BGRA_8888;
    case VX_COLOR_TYPE_RGBA_1010102:
        return VX_COLOR_TYPE_RGBA_1010102;
    case VX_COLOR_TYPE_BGRA_1010102:
        return VX_COLOR_TYPE_BGRA_1010102;
    case VX_COLOR_TYPE_RGB_101010X:
        return VX_COLOR_TYPE_RGB_101010X;
    case VX_COLOR_TYPE_BGR_101010X:
        return VX_COLOR_TYPE_BGR_101010X;
    case VX_COLOR_TYPE_BGR_101010X_XR:
        return VX_COLOR_TYPE_BGR_101010X_XR;
    case VX_COLOR_TYPE_BGRA_10101010_XR:
        return VX_COLOR_TYPE_BGRA_10101010_XR;
    case VX_COLOR_TYPE_RGBA_10X6:
        return VX_COLOR_TYPE_RGBA_10X6;
    case VX_COLOR_TYPE_GRAY_8:
        return VX_COLOR_TYPE_GRAY_8;
    case VX_COLOR_TYPE_RGBA_F16NORM:
        return VX_COLOR_TYPE_RGBA_F16NORM;
    case VX_COLOR_TYPE_RGBA_F16:
        return VX_COLOR_TYPE_RGBA_F16;
    case VX_COLOR_TYPE_RGB_F16F16F16X:
        return VX_COLOR_TYPE_RGB_F16F16F16X;
    case VX_COLOR_TYPE_RGBA_F32:
        return VX_COLOR_TYPE_RGBA_F32;
    case VX_COLOR_TYPE_R8G8_UNORM:
        return VX_COLOR_TYPE_R8G8_UNORM;
    case VX_COLOR_TYPE_A16_FLOAT:
        return VX_COLOR_TYPE_A16_FLOAT;
    case VX_COLOR_TYPE_R16G16_FLOAT:
        return VX_COLOR_TYPE_R16G16_FLOAT;
    case VX_COLOR_TYPE_A16_UNORM:
        return VX_COLOR_TYPE_A16_UNORM;
    case VX_COLOR_TYPE_R16_UNORM:
        return VX_COLOR_TYPE_R16_UNORM;
    case VX_COLOR_TYPE_R16G16_UNORM:
        return VX_COLOR_TYPE_R16G16_UNORM;
    case VX_COLOR_TYPE_R16G16B16A16_UNORM:
        return VX_COLOR_TYPE_R16G16B16A16_UNORM;
    case VX_COLOR_TYPE_SRGBA_8888:
        return VX_COLOR_TYPE_SRGBA_8888;
    case VX_COLOR_TYPE_R8_UNORM:
        return VX_COLOR_TYPE_R8_UNORM;
    }

    return VX_COLOR_TYPE_UNKNOWN;
}

#endif /* VIVIDX_CORE_CONVERTS_H */
