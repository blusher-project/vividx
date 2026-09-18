#include <vividx/core/image-info.h>

#include <stdlib.h>

#include <vividx/core/color-space.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

bool vx_yuv_color_space_is_limited_range(enum vx_yuv_color_space cs)
{
    switch (cs) {
    case VX_YUV_COLOR_SPACE_REC601_LIMITED:
    case VX_YUV_COLOR_SPACE_REC709_LIMITED:
    case VX_YUV_COLOR_SPACE_BT2020_8BIT_LIMITED:
    case VX_YUV_COLOR_SPACE_BT2020_10BIT_LIMITED:
    case VX_YUV_COLOR_SPACE_BT2020_12BIT_LIMITED:
    case VX_YUV_COLOR_SPACE_BT2020_16BIT_LIMITED:
    case VX_YUV_COLOR_SPACE_FCC_LIMITED:
    case VX_YUV_COLOR_SPACE_SMPTE240_LIMITED:
    case VX_YUV_COLOR_SPACE_YDZDX_LIMITED:
    case VX_YUV_COLOR_SPACE_GBR_LIMITED:
    case VX_YUV_COLOR_SPACE_YCGCO_8BIT_LIMITED:
    case VX_YUV_COLOR_SPACE_YCGCO_10BIT_LIMITED:
    case VX_YUV_COLOR_SPACE_YCGCO_12BIT_LIMITED:
    case VX_YUV_COLOR_SPACE_YCGCO_16BIT_LIMITED:
        return true;

    case VX_YUV_COLOR_SPACE_JPEG_FULL:
    case VX_YUV_COLOR_SPACE_REC709_FULL:
    case VX_YUV_COLOR_SPACE_BT2020_8BIT_FULL:
    case VX_YUV_COLOR_SPACE_BT2020_10BIT_FULL:
    case VX_YUV_COLOR_SPACE_BT2020_12BIT_FULL:
    case VX_YUV_COLOR_SPACE_BT2020_16BIT_FULL:
    case VX_YUV_COLOR_SPACE_FCC_FULL:
    case VX_YUV_COLOR_SPACE_SMPTE240_FULL:
    case VX_YUV_COLOR_SPACE_YDZDX_FULL:
    case VX_YUV_COLOR_SPACE_GBR_FULL:
    case VX_YUV_COLOR_SPACE_YCGCO_8BIT_FULL:
    case VX_YUV_COLOR_SPACE_YCGCO_10BIT_FULL:
    case VX_YUV_COLOR_SPACE_YCGCO_12BIT_FULL:
    case VX_YUV_COLOR_SPACE_YCGCO_16BIT_FULL:
    case VX_YUV_COLOR_SPACE_IDENTITY:
        return false;

    default:
        __builtin_trap();
    }
}


vx_color_info_t vx_color_info_copy_with_alpha_type(
    const vx_color_info_t *color_info, enum vx_alpha_type at)
{
    vx_color_info_t info;
    info.color_space = color_info->color_space;
    info.color_type = color_info->color_type;
    info.alpha_type = at;
    return info;
}

vx_color_info_t vx_color_info_copy_with_color_type(
    const vx_color_info_t *color_info, enum vx_color_type ct)
{
    vx_color_info_t info;
    info.color_space = color_info->color_space;
    info.color_type = ct;
    info.alpha_type = color_info->alpha_type;
    return info;
}

vx_color_info_t vx_color_info_copy_with_color_space(
    const vx_color_info_t *color_info, const vx_color_space_t *cs)
{
    vx_color_info_t info;
    info.color_space = cs;
    info.color_type = color_info->color_type;
    info.alpha_type = color_info->alpha_type;
    return info;
}

const vx_color_space_t* vx_color_info_color_space(
    const vx_color_info_t *color_info)
{
    return color_info->color_space;
}

enum vx_color_type vx_color_info_color_type(
    const vx_color_info_t *color_info)
{
    return color_info->color_type;
}

enum vx_alpha_type vx_color_info_alpha_type(
    const vx_color_info_t *color_info)
{
    return color_info->alpha_type;
}

bool vx_color_info_is_opaque(const vx_color_info_t *color_info)
{
    return vx_alpha_type_is_opaque(color_info->alpha_type)
        || vx_color_type_is_always_opaque(color_info->color_type);
}

bool vx_color_info_gamma_close_to_srgb(const vx_color_info_t *color_info)
{
    return color_info->color_space &&
        vx_color_space_gamma_close_to_srgb(color_info->color_space);
}

bool vx_color_info_eq(const vx_color_info_t *info,
                      const vx_color_info_t *other)
{
    return info->color_type == other->color_type &&
        info->alpha_type == other->alpha_type &&
        vx_color_space_eq(info->color_space, other->color_space);
}

bool vx_color_info_ne(const vx_color_info_t *info,
                      const vx_color_info_t *other)
{
    return !vx_color_info_eq(info, other);
}

int vx_color_info_bytes_per_pixel(const vx_color_info_t *ct)
{
    return vx_color_type_bytes_per_pixel(ct->color_type);
}

int vx_color_info_shift_per_pixel(const vx_color_info_t *ct)
{
    return vx_color_type_shift_per_pixel(ct->color_type);
}


#ifdef __cplusplus
}
#endif // __cplusplus