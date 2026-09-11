#ifndef VIVIDX_CORE_CONVERTS_H
#define VIVIDX_CORE_CONVERTS_H

#include <vividx/core/color-type.h>

#include "include/core/SkColorType.h"

SkColorType vx_color_type_to_sk(enum vx_color_type type)
{
    switch (type) {
    case VX_COLOR_TYPE_UNKNOWN:
        return kUnknown_SkColorType;
    case VX_COLOR_TYPE_ALPHA_8:
        return kAlpha_8_SkColorType;
    case VX_COLOR_TYPE_RGB_565:
        return kRGB_565_SkColorType;
    case VX_COLOR_TYPE_ARGB_4444:
        return kARGB_4444_SkColorType;
    case VX_COLOR_TYPE_RGBA_8888:
        return kRGBA_8888_SkColorType;
    case VX_COLOR_TYPE_RGB_888X:
        return kRGB_888x_SkColorType;
    case VX_COLOR_TYPE_BGRA_8888:
        return kBGRA_8888_SkColorType;
    case VX_COLOR_TYPE_RGBA_1010102:
        return kRGBA_1010102_SkColorType;
    case VX_COLOR_TYPE_BGRA_1010102:
        return kBGRA_1010102_SkColorType;
    case VX_COLOR_TYPE_RGB_101010X:
        return kRGB_101010x_SkColorType;
    case VX_COLOR_TYPE_BGR_101010X:
        return kBGR_101010x_SkColorType;
    case VX_COLOR_TYPE_BGR_101010X_XR:
        return kBGR_101010x_XR_SkColorType;
    case VX_COLOR_TYPE_BGRA_10101010_XR:
        return kBGRA_10101010_XR_SkColorType;
    case VX_COLOR_TYPE_RGBA_10X6:
        return kRGBA_10x6_SkColorType;
    case VX_COLOR_TYPE_GRAY_8:
        return kGray_8_SkColorType;
    case VX_COLOR_TYPE_RGBA_F16NORM:
        return kRGBA_F16Norm_SkColorType;
    case VX_COLOR_TYPE_RGBA_F16:
        return kRGBA_F16_SkColorType;
    case VX_COLOR_TYPE_RGB_F16F16F16X:
        return kRGB_F16F16F16x_SkColorType;
    case VX_COLOR_TYPE_RGBA_F32:
        return kRGBA_F32_SkColorType;
    case VX_COLOR_TYPE_R8G8_UNORM:
        return kR8G8_unorm_SkColorType;
    case VX_COLOR_TYPE_A16_FLOAT:
        return kA16_float_SkColorType;
    case VX_COLOR_TYPE_R16G16_FLOAT:
        return kR16G16_float_SkColorType;
    case VX_COLOR_TYPE_A16_UNORM:
        return kA16_unorm_SkColorType;
    case VX_COLOR_TYPE_R16_UNORM:
        return kR16_unorm_SkColorType;
    case VX_COLOR_TYPE_R16G16_UNORM:
        return kR16G16_unorm_SkColorType;
    case VX_COLOR_TYPE_R16G16B16A16_UNORM:
        return kR16G16B16A16_unorm_SkColorType;
    case VX_COLOR_TYPE_SRGBA_8888:
        return kSRGBA_8888_SkColorType;
    case VX_COLOR_TYPE_R8_UNORM:
        return kR8_unorm_SkColorType;
    }

    return kUnknown_SkColorType;
}

#endif /* VIVIDX_CORE_CONVERTS_H */
