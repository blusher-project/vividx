#include <vividx/core/point.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

vx_point_t vx_point_make(float x, float y)
{
    vx_point_t p = { .x = x, .y = y };

    return p;
}

bool vx_point_is_zero(vx_point_t p)
{
    return (p.x == 0 && p.y == 0);
}


vx_point_i_t vx_point_i_make(int32_t x, int32_t y)
{
    vx_point_i_t p = { .x = x, .y = y };

    return p;
}

bool vx_point_i_is_zero(vx_point_t p)
{
    return (p.x == 0 && p.y == 0);
}

#ifdef __cplusplus
}
#endif // __cplusplus