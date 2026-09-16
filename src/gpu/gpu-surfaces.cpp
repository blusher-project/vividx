#include <vividx/gpu/gpu-surfaces.h>

#include "include/core/SkColorSpace.h"
#include <vividx/core/color-type.h>
#include "include/gpu/ganesh/GrBackendSurface.h"
#include "include/gpu/ganesh/GrDirectContext.h"
// WrapBackendRenderTarget
#include "include/gpu/ganesh/SkSurfaceGanesh.h"

#include <vividx/core/surface-props.h>
#include <vividx/core/surface.h>

#include <vividx/gpu/backend-render-target.h>
#include <vividx/gpu/direct-context.h>

#include "../core/converts.h"
#include "converts.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

vx_surface_t* vx_surface_wrap_backend_render_target(
    const vx_direct_context_t *dc,
    const vx_backend_render_target_t *target,
    enum vx_gpu_surface_origin origin,
    enum vx_color_type color_type,
    const vx_color_space_t *color_space,
    const vx_surface_props_t *surface_props)
{
    vx_surface_t *surface = vx_surface_new();

    sk_sp<GrDirectContext> gr_direct_context =
        *(sk_sp<GrDirectContext>*)vx_direct_context_sk_sp(dc);
    sk_sp<SkSurface> sk_surface = SkSurfaces::WrapBackendRenderTarget(
        gr_direct_context.get(),
        *(GrBackendRenderTarget*)vx_backend_render_target_cxx_val(target),
        vx_gpu_surface_origin_to_sk(origin),
        vx_color_type_to_sk(color_type),
        nullptr,
        nullptr
    );

    vx_surface_set_SkSurface(surface, (void*)&sk_surface);

    return surface;
}

#ifdef __cplusplus
}
#endif // __cplusplus
