#include <vividx/core/image-info.h>

#include <stdlib.h>

#include <vividx/core/color-space.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct vx_color_info_t {
    vx_color_space_t *color_space;
    enum vx_color_type color_type; // = kUnknown_SkColorType;
    enum vx_alpha_type alpha_type; // = VX_ALPHA_TYPE_UNKNOWN;
};

vx_color_info_t* vx_color_info_new(enum vx_color_type ct,
                                   enum vx_alpha_type at,
                                   vx_color_space_t *cs)
{
    vx_color_info_t *color_info = malloc(sizeof(vx_color_info_t));

    color_info->color_space = cs;
    color_info->color_type = ct;
    color_info->alpha_type = at;

    return color_info;
}

vx_color_space_t* vx_color_info_color_space(
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

void vx_color_info_free(vx_color_info_t *color_info)
{
    vx_color_space_free(color_info->color_space);
    free(color_info);
}

#ifdef __cplusplus
}
#endif // __cplusplus