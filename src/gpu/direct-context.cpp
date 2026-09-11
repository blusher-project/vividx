#include <vividx/gpu/direct-context.h>

#include <stdlib.h>

#include "include/gpu/ganesh/GrDirectContext.h"
#include "include/gpu/ganesh/gl/GrGLInterface.h"
#include "include/gpu/ganesh/gl/GrGLDirectContext.h"

#include <vividx/gpu/gl-interface.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct vx_direct_context_t {
    sk_sp<GrDirectContext> p;
};

vx_direct_context_t* vx_direct_context_new_gl(vx_gl_interface_t *gl_interface)
{
    vx_direct_context_t *direct_context = new vx_direct_context_t;

    direct_context->p = GrDirectContexts::MakeGL(
        *(sk_sp<GrGLInterface>*)vx_gl_interface_sk_sp(gl_interface)
    );

    return direct_context;
}

void* vx_direct_context_sk_sp(const vx_direct_context_t *direct_context)
{
    return (void*)&direct_context->p;
}

void vx_direct_context_free(vx_direct_context_t *direct_context)
{
    delete direct_context;
}

#ifdef __cplusplus
}
#endif // __cplusplus
