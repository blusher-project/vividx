#include <vividx/core/color-type.h>

#include <stdbool.h>

#include <vividx/core/color.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

int vx_color_type_bytes_per_pixel(enum vx_color_type ct)
{
    switch (ct) {
    case VX_COLOR_TYPE_UNKNOWN:            return 0;
    case VX_COLOR_TYPE_ALPHA_8:            return 1;
    case VX_COLOR_TYPE_RGB_565:            return 2;
    case VX_COLOR_TYPE_ARGB_4444:          return 2;
    case VX_COLOR_TYPE_RGBA_8888:          return 4;
    case VX_COLOR_TYPE_BGRA_8888:          return 4;
    case VX_COLOR_TYPE_RGB_888X:           return 4;
    case VX_COLOR_TYPE_RGBA_1010102:       return 4;
    case VX_COLOR_TYPE_RGB_101010X:        return 4;
    case VX_COLOR_TYPE_BGRA_1010102:       return 4;
    case VX_COLOR_TYPE_BGR_101010X:        return 4;
    case VX_COLOR_TYPE_BGR_101010X_XR:     return 4;
    case VX_COLOR_TYPE_BGRA_10101010_XR:   return 8;
    case VX_COLOR_TYPE_RGBA_10X6:          return 8;
    case VX_COLOR_TYPE_GRAY_8:             return 1;
    case VX_COLOR_TYPE_RGBA_F16NORM:       return 8;
    case VX_COLOR_TYPE_RGBA_F16:           return 8;
    case VX_COLOR_TYPE_RGB_F16F16F16X:     return 8;
    case VX_COLOR_TYPE_RGBA_F32:           return 16;
    case VX_COLOR_TYPE_R8G8_UNORM:         return 2;
    case VX_COLOR_TYPE_A16_UNORM:          return 2;
    case VX_COLOR_TYPE_R16_UNORM:          return 2;
    case VX_COLOR_TYPE_R16G16_UNORM:       return 4;
    case VX_COLOR_TYPE_A16_FLOAT:          return 2;
    case VX_COLOR_TYPE_R16_FLOAT:          return 2;
    case VX_COLOR_TYPE_R16G16_FLOAT:       return 4;
    case VX_COLOR_TYPE_R16G16B16A16_UNORM: return 8;
    case VX_COLOR_TYPE_SRGBA_8888:         return 4;
    case VX_COLOR_TYPE_R8_UNORM:           return 1;
    }
    __builtin_trap();
}

bool vx_color_type_is_always_opaque(enum vx_color_type ct)
{
    return !(vx_color_type_channel_flags(ct) & VX_COLOR_CHANNEL_FLAG_ALPHA);
    // return !(SkColorTypeChannelFlags(ct) & kAlpha_SkColorChannelFlag);
}

bool vx_color_type_validate_alpha_type(enum vx_color_type color_type,
                                       enum vx_alpha_type alpha_type,
                                       enum vx_alpha_type *canonical)
{
    switch (color_type) {
        case VX_COLOR_TYPE_UNKNOWN:
            alpha_type = VX_ALPHA_TYPE_UNKNOWN;
            break;
        case VX_COLOR_TYPE_ALPHA_8:         // fall-through
        case VX_COLOR_TYPE_A16_UNORM:       // fall-through
        case VX_COLOR_TYPE_A16_FLOAT:
            if (VX_ALPHA_TYPE_UNPREMULTIPLIED == alpha_type) {
                alpha_type = VX_ALPHA_TYPE_PREMULTIPLIED;
            }
            [[fallthrough]];
        case VX_COLOR_TYPE_ARGB_4444:
        case VX_COLOR_TYPE_RGBA_8888:
        case VX_COLOR_TYPE_SRGBA_8888:
        case VX_COLOR_TYPE_BGRA_8888:
        case VX_COLOR_TYPE_RGBA_1010102:
        case VX_COLOR_TYPE_BGRA_1010102:
        case VX_COLOR_TYPE_RGBA_10X6:
        case VX_COLOR_TYPE_RGBA_F16NORM:
        case VX_COLOR_TYPE_RGBA_F16:
        case VX_COLOR_TYPE_RGBA_F32:
        case VX_COLOR_TYPE_BGRA_10101010_XR:
        case VX_COLOR_TYPE_R16G16B16A16_UNORM:
            if (VX_ALPHA_TYPE_UNKNOWN == alpha_type) {
                return false;
            }
            break;
        case VX_COLOR_TYPE_GRAY_8:
        case VX_COLOR_TYPE_R8G8_UNORM:
        case VX_COLOR_TYPE_R16_UNORM:
        case VX_COLOR_TYPE_R16_FLOAT:
        case VX_COLOR_TYPE_R16G16_UNORM:
        case VX_COLOR_TYPE_R16G16_FLOAT:
        case VX_COLOR_TYPE_RGB_565:
        case VX_COLOR_TYPE_RGB_888X:
        case VX_COLOR_TYPE_RGB_101010X:
        case VX_COLOR_TYPE_BGR_101010X:
        case VX_COLOR_TYPE_BGR_101010X_XR:
        case VX_COLOR_TYPE_RGB_F16F16F16X:
        case VX_COLOR_TYPE_R8_UNORM:
            alpha_type = VX_ALPHA_TYPE_OPAQUE;
            break;
    }
    if (canonical) {
        *canonical = alpha_type;
    }
    return true;
}

uint32_t vx_color_type_channel_flags(enum vx_color_type ct) {
    switch (ct) {
    case VX_COLOR_TYPE_UNKNOWN:            return 0;
    case VX_COLOR_TYPE_ALPHA_8:            return VX_COLOR_CHANNEL_FLAG_ALPHA;
    case VX_COLOR_TYPE_RGB_565:            return VX_COLOR_CHANNEL_FLAGS_RGB;
    case VX_COLOR_TYPE_ARGB_4444:          return VX_COLOR_CHANNEL_FLAGS_RGBA;
    case VX_COLOR_TYPE_RGBA_8888:          return VX_COLOR_CHANNEL_FLAGS_RGBA;
    case VX_COLOR_TYPE_RGB_888X:           return VX_COLOR_CHANNEL_FLAGS_RGB;
    case VX_COLOR_TYPE_BGRA_8888:          return VX_COLOR_CHANNEL_FLAGS_RGBA;
    case VX_COLOR_TYPE_RGBA_1010102:       return VX_COLOR_CHANNEL_FLAGS_RGBA;
    case VX_COLOR_TYPE_RGB_101010X:        return VX_COLOR_CHANNEL_FLAGS_RGB;
    case VX_COLOR_TYPE_BGRA_1010102:       return VX_COLOR_CHANNEL_FLAGS_RGBA;
    case VX_COLOR_TYPE_BGR_101010X:        return VX_COLOR_CHANNEL_FLAGS_RGB;
    case VX_COLOR_TYPE_BGR_101010X_XR:     return VX_COLOR_CHANNEL_FLAGS_RGB;
    case VX_COLOR_TYPE_BGRA_10101010_XR:   return VX_COLOR_CHANNEL_FLAGS_RGBA;
    case VX_COLOR_TYPE_RGBA_10X6:          return VX_COLOR_CHANNEL_FLAGS_RGBA;
    case VX_COLOR_TYPE_GRAY_8:             return VX_COLOR_CHANNEL_FLAG_GRAY;
    case VX_COLOR_TYPE_RGBA_F16NORM:       return VX_COLOR_CHANNEL_FLAGS_RGBA;
    case VX_COLOR_TYPE_RGBA_F16:           return VX_COLOR_CHANNEL_FLAGS_RGBA;
    case VX_COLOR_TYPE_RGB_F16F16F16X:     return VX_COLOR_CHANNEL_FLAGS_RGB;
    case VX_COLOR_TYPE_RGBA_F32:           return VX_COLOR_CHANNEL_FLAGS_RGBA;
    case VX_COLOR_TYPE_R8G8_UNORM:         return VX_COLOR_CHANNEL_FLAGS_RG;
    case VX_COLOR_TYPE_A16_UNORM:          return VX_COLOR_CHANNEL_FLAG_ALPHA;
    case VX_COLOR_TYPE_R16_UNORM:          return VX_COLOR_CHANNEL_FLAG_RED;
    case VX_COLOR_TYPE_R16G16_UNORM:       return VX_COLOR_CHANNEL_FLAGS_RG;
    case VX_COLOR_TYPE_A16_FLOAT:          return VX_COLOR_CHANNEL_FLAG_ALPHA;
    case VX_COLOR_TYPE_R16_FLOAT:          return VX_COLOR_CHANNEL_FLAG_RED;
    case VX_COLOR_TYPE_R16G16_FLOAT:       return VX_COLOR_CHANNEL_FLAGS_RG;
    case VX_COLOR_TYPE_R16G16B16A16_UNORM: return VX_COLOR_CHANNEL_FLAGS_RGBA;
    case VX_COLOR_TYPE_SRGBA_8888:         return VX_COLOR_CHANNEL_FLAGS_RGBA;
    case VX_COLOR_TYPE_R8_UNORM:           return VX_COLOR_CHANNEL_FLAG_RED;
    }
    __builtin_trap();
}

int vx_color_type_shift_per_pixel(enum vx_color_type ct)
{
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
    __builtin_trap();
}

#ifdef __cplusplus
}
#endif // __cplusplus
