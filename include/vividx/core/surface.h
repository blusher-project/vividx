#ifndef VIVIDX_CORE_SURFACE_H
#define VIVIDX_CORE_SURFACE_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct vx_canvas_t vx_canvas_t;

typedef struct vx_surface_t vx_surface_t;

vx_surface_t* vx_surface_new();

void vx_surface_set_SkSurface(vx_surface_t *surface, void *sk_sp_SkSurface_p);

void* vx_surface_cxx_ptr(const vx_surface_t *surface);

vx_canvas_t* vx_surface_canvas(const vx_surface_t *surface);

void vx_surface_free(vx_surface_t *surface);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* VIVIDX_CORE_SURFACE_H */
