#ifndef VIVIDX_CORE_ALPHA_TYPE_H
#define VIVIDX_CORE_ALPHA_TYPE_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/** \enum vx_alpha_type
    Describes how to interpret the alpha component of a pixel. A pixel may
    be opaque, or alpha, describing multiple levels of transparency.

    In simple blending, alpha weights the draw color and the destination
    color to create a new color. If alpha describes a weight from zero to one:

    new color = draw color * alpha + destination color * (1 - alpha)

    In practice alpha is encoded in two or more bits, where 1.0 equals all bits set.

    RGB may have alpha included in each component value; the stored
    value is the original RGB multiplied by alpha. Premultiplied color
    components improve performance.
*/
enum vx_alpha_type {
    VX_ALPHA_TYPE_UNKNOWN,          //!< uninitialized
    VX_ALPHA_TYPE_OPAQUE,           //!< pixel is opaque
    VX_ALPHA_TYPE_PREMULTIPLIED,    //!< pixel components are premultiplied by alpha
    VX_ALPHA_TYPE_UNPREMULTIPLIED,  //!< pixel components are independent of alpha

    VX_ALPHA_TYPE_LASTENUM = VX_ALPHA_TYPE_UNPREMULTIPLIED,
};

static inline bool vx_alpha_type_is_opaque(enum vx_alpha_type at) {
    return VX_ALPHA_TYPE_OPAQUE == at;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* VIVIDX_CORE_ALPHA_TYPE_H */
