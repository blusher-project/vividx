#include <vividx/gpu/backend-render-target.h>

#include <GL/gl.h>

#include "include/gpu/ganesh/GrBackendSurface.h"
#include "include/gpu/ganesh/gl/GrGLTypes.h"
#include "include/gpu/ganesh/gl/GrGLBackendSurface.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct vx_backend_render_target_t {
    GrBackendRenderTarget v;
};

vx_backend_render_target_t*
vx_backend_render_target_new_gl(int width,
                                int height,
                                int sample_cnt,
                                int stencil_bits,
                                const vx_gl_framebuffer_info_t *gl_fb_info)
{
    vx_backend_render_target_t *target = new vx_backend_render_target_t;

    (void)gl_fb_info;

    GrGLFramebufferInfo fb_info;
    fb_info.fFBOID = 0;
    fb_info.fFormat = GL_RGBA8;

    target->v = GrBackendRenderTargets::MakeGL(
        width, height, sample_cnt, stencil_bits, fb_info);

    return target;
}

void* vx_backend_render_target_cxx_val(const vx_backend_render_target_t *t)
{
    return (void*)&(t->v);
}

void vx_backend_render_target_free(vx_backend_render_target_t *target)
{
    delete target;
}

#ifdef __cplusplus
}
#endif // __cplusplus
