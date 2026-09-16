#include <vividx/core/color-space.h>

#include "include/core/SkColorSpace.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct vx_color_space_t {
    sk_sp<SkColorSpace> p;
};

vx_color_space_t* vx_color_space_new_srgb()
{
    vx_color_space_t *cs = new vx_color_space_t;

    cs->p = SkColorSpace::MakeSRGB();

    return cs;
}

vx_color_space_t* vx_color_space_new_srgb_linear()
{
    vx_color_space_t *cs = new vx_color_space_t;

    cs->p = SkColorSpace::MakeSRGBLinear();

    return cs;
}

void* vx_color_space_cxx_ptr(const vx_color_space_t *cs)
{
    return (void*)(cs->p.get());
}

void vx_color_space_free(vx_color_space_t *cs)
{
    delete cs;
}

#ifdef __cplusplus
}
#endif // __cplusplus