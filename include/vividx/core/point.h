#ifndef VIVIDX_CORE_POINT_H
#define VIVIDX_CORE_POINT_H

#include <stdbool.h>
#include <stdint.h>

#include <vividx/common.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct vx_point_t vx_point_t;
typedef struct vx_point_i_t vx_point_i_t;

struct vx_point_t {
    float x;
    float y;
};

struct vx_point_i_t {
    int32_t x;
    int32_t y;
};

VX_PUBLIC
vx_point_t vx_point_make(float x, float y);

VX_PUBLIC
bool vx_point_is_zero(vx_point_t p);


VX_PUBLIC
vx_point_i_t vx_point_i_make(int32_t x, int32_t y);

VX_PUBLIC
bool vx_point_i_is_zero(vx_point_t p);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* VIVIDX_CORE_POINT_H */
