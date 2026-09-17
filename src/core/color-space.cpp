#include <vividx/core/color-space.h>

#include <math.h>

#include "include/core/SkColorSpace.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct vx_color_space_t {
    sk_sp<SkColorSpace> p;
};

vx_color_space_t* vx_color_space_new_srgb()
{
    vx_color_space_t *cs = new vx_color_space_t;

    cs->p = SkColorSpace::MakeSRGB();

    return cs;
}

vx_color_space_t* vx_color_space_new_srgb_linear()
{
    vx_color_space_t *cs = new vx_color_space_t;

    cs->p = SkColorSpace::MakeSRGBLinear();

    return cs;
}

vx_color_space_t* vx_color_space_new_rgb(
    const skcms_TransferFunction transferFn,
    const skcms_Matrix3x3 toXYZ)
{
    vx_color_space_t *cs = new vx_color_space_t;

    cs->p = SkColorSpace::MakeRGB(transferFn, toXYZ);

    return cs;
}

void* vx_color_space_cxx_ptr(const vx_color_space_t *cs)
{
    return (void*)(cs->p.get());
}

bool vx_color_space_gamma_close_to_srgb(const vx_color_space_t *color_space)
{
    // Nearly-equal transfer functions were snapped at construction time, so just do an exact test
    return memcmp(&color_space->p->fTransferFn,
        &SkNamedTransferFn::kSRGB, 7*sizeof(float)) == 0;
}

bool vx_color_space_gamma_is_linear(const vx_color_space_t *color_space)
{
    // Nearly-equal transfer functions were snapped at construction time, so just do an exact test
    return memcmp(&color_space->p->fTransferFn,
        &SkNamedTransferFn::kLinear, 7*sizeof(float)) == 0;
}

bool vx_color_space_eq(const vx_color_space_t *x,
                       const vx_color_space_t *y)
{
    if (x == y) {
        return true;
    }

    if (!x || !y) {
        return false;
    }

    if (vx_color_space_hash(x) == vx_color_space_hash(y)) {
    #if defined(SK_DEBUG)
        // Do these floats function equivalently?
        // This returns true more often than simple float comparison   (NaN vs. NaN) and,
        // also returns true more often than simple bitwise comparison (+0 vs. -0) and,
        // even returns true more often than those two OR'd together   (two different NaNs).
        auto equiv = [](float X, float Y) {
            return (X==Y)
                || (isnan(X) && isnan(Y));
        };

        for (int i = 0; i < 7; i++) {
            float X = (&x->p->fTransferFn.g)[i],
                  Y = (&y->p->fTransferFn.g)[i];
            SkASSERTF(equiv(X,Y), "Hash collision at tf[%d], !equiv(%g,%g)\n", i, X,Y);
        }
        for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++) {
            float X = x->p->fToXYZD50.vals[r][c],
                  Y = y->p->fToXYZD50.vals[r][c];
            SkASSERTF(equiv(X,Y), "Hash collision at toXYZD50[%d][%d], !equiv(%g,%g)\n", r,c, X,Y);
        }
    #endif
        return true;
    }
    return false;
}

void vx_color_space_transfer_fn(const vx_color_space_t *cs,
                                skcms_TransferFunction *fn)
{
    *fn = cs->p->fTransferFn;
}

void vx_color_space_inv_transfer_fn(const vx_color_space_t *cs,
                                    skcms_TransferFunction* fn)
{
    cs->p->computeLazyDstFields();
    *fn = cs->p->fInvTransferFn;
}

void vx_color_space_gamut_transform_to(const vx_color_space_t *cs,
                                       const vx_color_space_t *dst,
                                       skcms_Matrix3x3 *src_to_dst)
{
    dst->p->computeLazyDstFields();
    *src_to_dst = skcms_Matrix3x3_concat(&dst->p->fFromXYZD50,
        &cs->p->fToXYZD50);
}

uint32_t vx_color_space_transfer_fn_hash(const vx_color_space_t *cs)
{
    return cs->p->fTransferFnHash;
}

uint64_t vx_color_space_hash(const vx_color_space_t *cs)
{
    return (uint64_t)(cs->p->fTransferFnHash) << 32 | cs->p->fToXYZD50Hash;
}

void vx_color_space_compute_lazy_dst_fields(const vx_color_space_t *cs)
{
    cs->p->fLazyDstFieldsOnce([cs] {

        // Invert 3x3 gamut, defaulting to sRGB if we can't.
        {
            if (!skcms_Matrix3x3_invert(&cs->p->fToXYZD50, &cs->p->fFromXYZD50)) {
                SkAssertResult(skcms_Matrix3x3_invert(&skcms_sRGB_profile()->toXYZD50,
                                                      &cs->p->fFromXYZD50));
            }
        }

        // Invert transfer function, defaulting to sRGB if we can't.
        {
            if (!skcms_TransferFunction_invert(&cs->p->fTransferFn, &cs->p->fInvTransferFn)) {
                cs->p->fInvTransferFn = *skcms_sRGB_Inverse_TransferFunction();
            }
        }

    });
}

void vx_color_space_free(vx_color_space_t *cs)
{
    delete cs;
}

#ifdef __cplusplus
}
#endif // __cplusplus