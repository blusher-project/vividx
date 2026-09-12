#include <vividx/core/surface.h>

#include "include/core/SkSurface.h"

#include <vividx/core/canvas.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct vx_surface_t {
    sk_sp<SkSurface> p;
};

vx_surface_t* vx_surface_new()
{
    vx_surface_t *surface = new vx_surface_t;

    return surface;
}

void vx_surface_set_SkSurface(vx_surface_t *surface, void *sk_sp_SkSurface_p)
{
    surface->p = *(sk_sp<SkSurface>*)sk_sp_SkSurface_p;
}

void* vx_surface_cxx_ptr(const vx_surface_t *surface)
{
    return (void*)(surface->p.get());
}

vx_canvas_t* vx_surface_canvas(const vx_surface_t *surface)
{
    vx_canvas_t *canvas = vx_canvas_new_from_SkCanvas(surface->p->getCanvas());

    return canvas;
}

void vx_surface_free(vx_surface_t *surface)
{
    delete surface;
}

#ifdef __cplusplus
}
#endif // __cplusplus