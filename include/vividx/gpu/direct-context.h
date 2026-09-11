#ifndef VIVIDX_GPU_DIRECT_CONTEXT_H
#define VIVIDX_GPU_DIRECT_CONTEXT_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct vx_gl_interface_t;

typedef struct vx_direct_context_t vx_direct_context_t;

vx_direct_context_t* vx_direct_context_new_gl(vx_gl_interface_t *gl_interface);

void* vx_direct_context_sk_sp(const vx_direct_context_t *direct_context);

void vx_direct_context_free(vx_direct_context_t *direct_context);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* VIVIDX_GPU_DIRECT_CONTEXT_H */
