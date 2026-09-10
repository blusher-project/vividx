#ifndef VIVIDX_CORE_ALPHA_TYPE_H
#define VIVIDX_CORE_ALPHA_TYPE_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

enum vx_alpha_type {
    VX_ALPHA_TYPE_UNKNOWN,          //!< uninitialized
    VX_ALPHA_TYPE_OPAQUE,           //!< pixel is opaque
    VX_ALPHA_TYPE_PREMULTIPLIED,    //!< pixel components are premultiplied by alpha
    VX_ALPHA_TYPE_UNPREMULTIPLIED,  //!< pixel components are independent of alpha
};

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* VIVIDX_CORE_ALPHA_TYPE_H */
