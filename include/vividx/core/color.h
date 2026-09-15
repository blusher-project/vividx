#ifndef VIVIDX_CORE_COLOR_H
#define VIVIDX_CORE_COLOR_H

#include <vividx/common.h>
#include <vividx/core/alpha-type.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef uint8_t vx_alpha_t;
typedef uint32_t vx_color32_t;
typedef struct vx_color_t vx_color_t;

struct vx_color_t {
    float r;
    float g;
    float b;
    float a;
};

static vx_color_t vx_color_make(float r, float g, float b, float a)
{
    vx_color_t color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
    return color;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* VIVIDX_CORE_COLOR_H */
