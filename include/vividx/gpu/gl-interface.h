#ifndef VIVIDX_GPU_GL_INTERFACE_H
#define VIVIDX_GPU_GL_INTERFACE_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct vx_gl_interface_t vx_gl_interface_t;

vx_gl_interface_t* vx_gl_interface_new_egl();

void* vx_gl_interface_sk_sp(const vx_gl_interface_t *gl_interface);

bool vx_gl_interface_validate(const vx_gl_interface_t *gl_interface);

void vx_gl_interface_free(vx_gl_interface_t *iface);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* VIVIDX_GPU_GL_INTERFACE_H */
