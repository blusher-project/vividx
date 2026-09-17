#ifndef VIVIDX_COMMON_H
#define VIVIDX_COMMON_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//!<==================
//!< API Visibility
//!<==================

#define VX_PUBLIC __attribute__((visibility("default")))


//!<==================
//!< Common Math
//!<==================

static const int16_t VX_INT16_MAX = INT16_MAX;
static const int16_t VX_INT16_MIN = -VX_INT16_MAX;

static const int32_t VX_INT32_MAX = INT32_MAX;
static const int32_t VX_INT32_MIN = -VX_INT32_MAX;
static const int32_t VX_INT32_NAN = INT32_MIN;

static const int64_t VX_INT64_MAX = INT64_MAX;
static const int64_t VX_INT64_MIN = -VX_INT64_MAX;

static int32_t vx_int64_pin_to_int32(int64_t x) {
    return x < VX_INT32_MIN ? VX_INT32_MIN : (x > VX_INT32_MAX ? VX_INT32_MAX : (int32_t)x);
}

static int32_t vx_int32_sat_add(int32_t a, int32_t b) {
    return vx_int64_pin_to_int32((int64_t)a + (int64_t)b);
}

static int32_t vx_int32_sat_sub(int32_t a, int32_t b) {
    return vx_int64_pin_to_int32((int64_t)a - (int64_t)b);
}

// To avoid UBSAN complaints about 2's compliment overflows
//
static int32_t vx_int32_can_overflow_add(int32_t a, int32_t b) {
    return (int32_t)((uint32_t)a + (uint32_t)b);
}
static int32_t vx_int32_can_overflow_sub(int32_t a, int32_t b) {
    return (int32_t)((uint32_t)a - (uint32_t)b);
}

/**
 * This is a 'safe' abs for 32-bit integers that asserts when undefined behavior would occur.
 * SkTAbs (in SkTemplates.h) is a general purpose absolute-value function.
 */
static inline int32_t vx_int32_abs(int32_t value) {
    // The most negative int32_t can't be negated.
    // SkASSERT(value != VX_INT32_NAN);
    if (value != VX_INT32_NAN) {
        // fprintf(stderr, "vx_int32_abs() - error!\n");
    }
    if (value < 0) {
        value = -value;
    }
    return value;
}

// Calculate the midpoint between a and b. Similar to std::midpoint in c++20.
static float vx_float_midpoint(float a, float b)
{
    // Use double math to avoid underflow and overflow.
    return (float)(0.5 * ((double)(a) + b));
}


//!<==================
//!< Floating Point
//!<==================

typedef float vx_scalar_t;

// Subtracting a value from itself will result in zero, except for NAN or ±Inf, which make NAN.
// Multiplying a group of values against zero will result in zero for each product, except for
// NAN or ±Inf, which will result in NAN and continue resulting in NAN for the rest of the elements.
// This generates better code than `std::isfinite` when building with clang-cl (April 2024).
/*
template <typename T, typename... Pack, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
static inline bool SkIsFinite(T x, Pack... values) {
    T prod = x - x;
    prod = (prod * ... * values);
    // At this point, `prod` will either be NaN or 0.
    return prod == prod;
}
*/
static inline bool vx_float_is_finite_1f(float f1)
{
    float prod = f1 - f1;
    return prod == prod;
}

static inline bool vx_float_is_finite_2f(float f1, float f2)
{
    float prod = f1 - f1;
    prod = (prod * f2);
    return prod == prod;
}

static inline bool vx_float_is_finite_3f(float f1, float f2, float f3)
{
    float prod = f1 - f1;
    prod = (prod * f2 * f3);
    return prod == prod;
}

static inline bool vx_float_is_finite_4f(float f1, float f2, float f3, float f4)
{
    float prod = f1 - f1;
    prod = (prod * f2 * f3 * f4);
    // At this point, `prod` will either be NaN or 0.
    return prod == prod;
}


//!<==============
//!< Fits In
//!<==============

VX_PUBLIC
bool vx_int64_fits_in_int32(int64_t val);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* VIVIDX_COMMON_H */
