#include <vividx/core/rect.h>

#include <math.h>

#include <vividx/common.h>

// From `sys/sys/param.h` in FreeBSD source code.
/* Macros for min/max. */
#define	MIN(a,b) (((a)<(b))?(a):(b))
#define	MAX(a,b) (((a)>(b))?(a):(b))

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//!<==================
//!< Float Rect
//!<==================

bool vx_rect_is_empty(vx_rect_t rect)
{
    // We write it as the NOT of a non-empty rect, so we will return true if any values
    // are NaN.
    return !(rect.left < rect.right && rect.top < rect.bottom);
}

static vx_rect_t vx_rect_intersected_impl(float al, float at, float ar, float ab,
                        float bl, float bt, float br, float bb)
{
    float l = fmax(al, bl);
    float r = fmin(ar, br);
    float t = fmax(at, bt);
    float b = fmin(ab, bb);

    return vx_rect_make_ltrb(l, t, r, b);
}

vx_rect_t vx_rect_intersected(vx_rect_t a, vx_rect_t b)
{
    return vx_rect_intersected_impl(
        a.left, a.top, a.right, a.bottom,
        b.left, b.top, b.right, b.bottom
    );
}

bool vx_rect_intersects(vx_rect_t a, vx_rect_t b)
{
    float left = fmaxf(a.left, b.left);
    float right = fminf(a.right, b.right);
    float top = fmaxf(a.top, b.top);
    float bottom = fminf(a.bottom, b.bottom);
    return left < right && top < bottom;
}

vx_rect_t vx_rect_sorted(vx_rect_t r)
{
    return vx_rect_make_ltrb(
        fminf(r.left, r.right), fminf(r.top, r.bottom),
        fmaxf(r.left, r.right), fmaxf(r.top, r.bottom)
    );
}


//!<==================
//!< Integer Rect
//!<==================

bool vx_rect_i_is_empty(vx_rect_i_t rect)
{
    int64_t w = vx_rect_i_width64(rect);
    int64_t h = vx_rect_i_height64(rect);
    if (w <= 0 || h <= 0) {
        return true;
    }
    // Return true if either exceeds int32_t
    // return !SkTFitsIn<int32_t>(w | h);
    return vx_int64_fits_in_int32(w | h);
}

vx_rect_i_t vx_rect_i_intersected(vx_rect_i_t a, vx_rect_i_t b)
{
    vx_rect_i_t tmp = {
        MAX(a.left,   b.left),
        MAX(a.top,    b.top),
        MIN(a.right,  b.right),
        MIN(a.bottom, b.bottom)
    };

    return tmp;
}

bool vx_rect_i_intersects(vx_rect_i_t a, vx_rect_i_t b) {
    vx_rect_i_t tmp = vx_rect_i_intersected(a, b);
    if (vx_rect_i_is_empty(tmp)) {
        return false;
    }

    return true;
}

vx_rect_i_t vx_rect_i_joined(vx_rect_i_t lhs, vx_rect_i_t rhs)
{
    // do nothing if the params are empty
    if (rhs.left >= rhs.right || rhs.top >= rhs.bottom) {
        return lhs;
    }

    // if we are empty, just assign
    if (lhs.left >= lhs.right || lhs.top >= lhs.bottom) {
        lhs = rhs;
    } else {
        if (rhs.left < lhs.left)     lhs.left = rhs.left;
        if (rhs.top < lhs.top)       lhs.top = rhs.top;
        if (rhs.right > lhs.right)   lhs.right = rhs.right;
        if (rhs.bottom > lhs.bottom) lhs.bottom = rhs.bottom;
    }

    return lhs;
}

vx_rect_i_t vx_rect_i_sorted(vx_rect_i_t rect)
{
    return vx_rect_i_make_ltrb(
        MIN(rect.left, rect.right), MIN(rect.top, rect.bottom),
        MAX(rect.left, rect.right), MAX(rect.top, rect.bottom)
    );
}

#ifdef __cplusplus
}
#endif // __cplusplus
