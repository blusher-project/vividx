#ifndef VIVIDX_GPU_GPU_SURFACES_H
#define VIVIDX_GPU_GPU_SURFACES_H

#include <vividx/core/color-type.h>
#include <vividx/gpu/types.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct vx_surface_t vx_surface_t;
typedef struct vx_direct_context_t vx_direct_context_t;
typedef struct vx_color_space_t vx_color_space_t;
typedef struct vx_surface_props_t vx_surface_props_t;

typedef struct vx_backend_render_target_t vx_backend_render_target_t;

vx_surface_t* vx_surface_wrap_backend_render_target(
    const vx_direct_context_t *dc,
    const vx_backend_render_target_t *target,
    enum vx_gpu_surface_origin origin,
    enum vx_color_type color_type,
    const vx_color_space_t *color_space,
    const vx_surface_props_t *surface_props);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* VIVIDX_GPU_GPU_SURFACES_H */
