#ifndef VIVIDX_GPU_BACKEND_RENDER_TARGET_H
#define VIVIDX_GPU_BACKEND_RENDER_TARGET_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct vx_backend_render_target_t vx_backend_render_target_t;

typedef void vx_gl_framebuffer_info_t;

vx_backend_render_target_t*
vx_backend_render_target_new_gl(int width,
                                int height,
                                int sample_cnt,
                                int stencil_bits,
                                const vx_gl_framebuffer_info_t *gl_fb_info);

void* vx_backend_render_target_cxx_val(const vx_backend_render_target_t *t);

void vx_backend_render_target_free(vx_backend_render_target_t *target);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* VIVIDX_GPU_BACKEND_RENDER_TARGET_H */
