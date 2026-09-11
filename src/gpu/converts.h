#ifndef VIVIDX_GPU_CONVERTS_H
#define VIVIDX_GPU_CONVERTS_H

#include <vividx/gpu/types.h>

#include "include/gpu/ganesh/GrTypes.h"

GrSurfaceOrigin vx_gpu_surface_origin_to_sk(enum vx_gpu_surface_origin origin)
{
    switch (origin) {
    case VX_GPU_SURFACE_ORIGIN_TOP_LEFT:
        return kTopLeft_GrSurfaceOrigin;
    case VX_GPU_SURFACE_ORIGIN_BOTTOM_LEFT:
        return kBottomLeft_GrSurfaceOrigin;
    }

    return kTopLeft_GrSurfaceOrigin;
}

#endif /* VIVIDX_GPU_CONVERTS_H */
