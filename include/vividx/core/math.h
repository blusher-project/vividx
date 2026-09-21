#ifndef VIVIDX_CORE_MATH_H
#define VIVIDX_CORE_MATH_H

#ifndef __cplusplus
#include <math.h>
#else
#include <cmath>
#endif

#include <vividx/assert.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//!<===========
//!< Float
//!<===========

static const float VX_FLOAT_SQRT2 = 1.41421356f;
static const float VX_FLOAT_PI    = 3.14159265f;
static const double VX_DOUBLE_PI  = 3.14159265358979323846264338327950288;

static const float VX_FLOAT_NAN = NAN;
static const float VX_FLOAT_INFINITY = INFINITY;
static const float VX_FLOAT_NEGATIVE_INFINITY = -VX_FLOAT_INFINITY;

#define VX_FLOAT_1                  1.0f
#define VX_FLOAT_HALF               0.5f
#define VX_FLOAT_TAN_PI_OVER_8      0.414213562f
#define VX_FLOAT_ROOT2_OVER2        0.707106781f
#define VX_FLOAT_MAX                3.402823466e+38f
#define VX_FLOAT_MIN                (-VX_FLOAT_MAX)


#define vx_float_floor(x)           std::floor(x)
#define vx_float_ceil(x)            std::ceil(x)
#define vx_float_round(x)           sk_float_round(x)
#define vx_float_trunc(x)           std::trunc(x)

#define vx_float_floor_to_int(x)    sk_float_floor2int(x)
#define vx_float_ceil_to_int(x)     sk_float_ceil2int(x)
#define vx_float_round_to_int(x)    sk_float_round2int(x)

#define vx_float_abs(x)             std::fabs(x)
#define vx_float_copy_sign(x, y)    std::copysign(x, y)
#define vx_float_mod(x, y)          std::fmod(x,y)
#define vx_float_sqrt(x)            std::sqrt(x)
#define vx_float_pow(b, e)          std::pow(b, e)

#define vx_float_sin(radians)        ((float)std::sin(radians))
#define vx_float_cos(radians)        ((float)std::cos(radians))
#define vx_float_tan(radians)        ((float)std::tan(radians))
#define vx_float_asin(val)           ((float)std::asin(val))
#define vx_float_acos(val)           ((float)std::acos(val))
#define vx_float_atan2(y, x)         ((float)std::atan2(y,x))
#define vx_float_exp(x)              ((float)std::exp(x))
#define vx_float_log(x)              ((float)std::log(x))
#define vx_float_log2(x)             ((float)std::log2(x))

/** Returns the fractional part of the scalar. */
static inline float vx_float_fraction(float x)
{
    return x - truncf(x);
}

static inline float vx_float_square(float x)
{
    return x * x;
}

#define vx_float_invert(x)              (VX_FLOAT_1 / (x))
#define SkScalarAve(a, b)               sk_float_midpoint(a, b)

#define vx_degrees_to_radians(degrees)  ((degrees) * (VX_FLOAT_PI / 180))
#define vx_radians_to_degrees(radians)  ((radians) * (180 / VX_FLOAT_PI))

static inline bool vx_float_is_int(float x)
{
    return x == floorf(x);
}

/**
 *  Returns -1 || 0 || 1 depending on the sign of value:
 *  -1 if x < 0
 *   0 if x == 0
 *   1 if x > 0
 */
static inline int vx_float_sign_as_int(float x)
{
    return x < 0 ? -1 : (x > 0);
}

// Scalar result version of above
static inline float vx_float_sign_as_float(float x)
{
    return x < 0 ? -VX_FLOAT_1 : ((x > 0) ? VX_FLOAT_1 : 0);
}

#define VX_FLOAT_NEARLY_ZERO        (VX_FLOAT_1 / (1 << 12))


static inline bool vx_float_nearly_zero_2(float x, float tolerance)
{
    VX_ASSERT(tolerance >= 0);
    return fabsf(x) <= tolerance;
}

static inline bool vx_float_nearly_zero_1(float x)
{
    return vx_float_nearly_zero_2(x, VX_FLOAT_NEARLY_ZERO);
}

#define VX_OVERLOAD2(_1, _2, NAME, ...) NAME
#define VX_OVERLOAD3(_1, _2, _3, NAME, ...) NAME

#define vx_float_nearly_zero(...) \
    VX_OVERLOAD2( \
        __VA_ARGS__, \
        vx_float_nearly_zero_2, \
        vx_float_nearly_zero_1 \
    )(__VA_ARGS__)


static inline bool vx_float_nearly_equal_3(float x, float y, float tolerance)
{
    VX_ASSERT(tolerance >= 0);
    return fabsf(x-y) <= tolerance;
}

static inline bool vx_float_nearly_equal_2(float x, float y)
{
    return vx_float_nearly_equal_3(x, y, VX_FLOAT_NEARLY_ZERO);
}

#define vx_float_nearly_equal(...) \
    VX_OVERLOAD3( \
        __VA_ARGS__, \
        vx_float_nearly_equal_3, \
        vx_float_nearly_equal_2, \
    )(__VA_ARGS__)

#define VX_FLOAT_SIN_COS_NEARLY_ZERO    (VX_FLOAT_1 / (1 << 16))

static inline float vx_float_sin_snap_to_zero(float radians)
{
    float v = sinf(radians);
    return vx_float_nearly_zero(v, VX_FLOAT_SIN_COS_NEARLY_ZERO) ? 0.0f : v;
}

static inline float vx_float_cos_snap_to_zero(float radians)
{
    float v = cosf(radians);
    return vx_float_nearly_zero(v, VX_FLOAT_SIN_COS_NEARLY_ZERO) ? 0.0f : v;
}

/** Linearly interpolate between A and B, based on t.
    If t is 0, return A
    If t is 1, return B
    else interpolate.
    t must be [0..VX_FLOAT_1]
*/
static inline float vx_float_lerp(float A, float B, float t)
{
    VX_ASSERT(t >= 0 && t <= VX_FLOAT_1);
    return A + (B - A) * t;
}

/*
 *  Helper to compare an array of scalars.
 */
static inline bool vx_float_array_eq(const float a[], const float b[], int n)
{
    VX_ASSERT(n >= 0);
    for (int i = 0; i < n; ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}


//!<===========
//!< Integer
//!<===========

#define vx_int_to_float(x)          (float)(x)

static inline int64_t vx_int64_mul(int64_t a, int64_t b)
{
    return a * b;
}

static inline int32_t vx_int32_left_shift(int32_t value, int32_t shift)
{
    return (int32_t) ((uint32_t) value << shift);
}

static inline int64_t vx_int64_left_shift(int64_t value, int32_t shift)
{
    return (int64_t) ((uint64_t) value << shift);
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* VIVIDX_CORE_MATH_H */
