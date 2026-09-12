#include <vividx/gpu/gl-interface.h>

#include <stdlib.h>

#include "include/gpu/ganesh/gl/GrGLInterface.h"
#include "include/gpu/ganesh/gl/egl/GrGLMakeEGLInterface.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct vx_gl_interface_t {
    sk_sp<const GrGLInterface> p;
};

vx_gl_interface_t* vx_gl_interface_new_egl()
{
    vx_gl_interface_t *gl_interface = new vx_gl_interface_t;

    gl_interface->p = GrGLInterfaces::MakeEGL();
    if (gl_interface->p == nullptr) {
        delete gl_interface;
        return nullptr;
    }

    return gl_interface;
}

void* vx_gl_interface_sk_sp(const vx_gl_interface_t *gl_interface)
{
    return (void*)&gl_interface->p;
}

bool vx_gl_interface_validate(const vx_gl_interface_t *gl_interface)
{
    return gl_interface->p->validate();
}

void vx_gl_interface_free(vx_gl_interface_t *gl_interface)
{
    delete gl_interface;
}

#ifdef __cplusplus
}
#endif // __cplusplus
