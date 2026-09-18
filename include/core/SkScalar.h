/*
 * Copyright 2006 The Android Open Source Project
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkScalar_DEFINED
#define SkScalar_DEFINED

#include "include/private/SkFloatingPoint.h"

#include <cmath>

#include <vividx/core/math.h>

typedef float SkScalar;

#define SK_Scalar1                  VX_FLOAT_1
#define SK_ScalarHalf               0.5f
#define SK_ScalarSqrt2              SK_FloatSqrt2
#define SK_ScalarPI                 SK_FloatPI
#define SK_ScalarTanPIOver8         0.414213562f
#define SK_ScalarRoot2Over2         0.707106781f
#define SK_ScalarMax                3.402823466e+38f
#define SK_ScalarMin                (-SK_ScalarMax)
#define SK_ScalarInfinity           SK_FloatInfinity
#define SK_ScalarNegativeInfinity   SK_FloatNegativeInfinity
#define SK_ScalarNaN                SK_FloatNaN

#define SkScalarFloorToScalar(x)    std::floor(x)
#define SkScalarCeilToScalar(x)     std::ceil(x)
#define SkScalarRoundToScalar(x)    sk_float_round(x)
#define SkScalarTruncToScalar(x)    std::trunc(x)

#define SkScalarFloorToInt(x)       sk_float_floor2int(x)
#define SkScalarCeilToInt(x)        sk_float_ceil2int(x)
#define SkScalarRoundToInt(x)       sk_float_round2int(x)

#define SkScalarAbs(x)              std::fabs(x)
#define SkScalarCopySign(x, y)      std::copysign(x, y)
#define SkScalarMod(x, y)           std::fmod(x,y)
#define SkScalarSqrt(x)             std::sqrt(x)
#define SkScalarPow(b, e)           std::pow(b, e)

#define SkScalarSin(radians)        ((float)std::sin(radians))
#define SkScalarCos(radians)        ((float)std::cos(radians))
#define SkScalarTan(radians)        ((float)std::tan(radians))
#define SkScalarASin(val)           ((float)std::asin(val))
#define SkScalarACos(val)           ((float)std::acos(val))
#define SkScalarATan2(y, x)         ((float)std::atan2(y,x))
#define SkScalarExp(x)              ((float)std::exp(x))
#define SkScalarLog(x)              ((float)std::log(x))
#define SkScalarLog2(x)             ((float)std::log2(x))

//////////////////////////////////////////////////////////////////////////////////////////////////

#define SkIntToScalar(x)        static_cast<SkScalar>(x)
#define SkIntToFloat(x)         static_cast<float>(x)
#define SkScalarTruncToInt(x)   sk_float_saturate2int(x)

#define SkScalarToFloat(x)      static_cast<float>(x)
#define SkFloatToScalar(x)      static_cast<SkScalar>(x)
#define SkScalarToDouble(x)     static_cast<double>(x)
#define SkDoubleToScalar(x)     sk_double_to_float(x)

/** Returns the fractional part of the scalar. */
#define SkScalarFraction            vx_float_fraction

#define SkScalarSquare              vx_float_square

#define SkScalarInvert(x)           vx_float_invert
#define SkScalarAve(a, b)           sk_float_midpoint(a, b)

#define SkDegreesToRadians(degrees) vx_degrees_to_radians
#define SkRadiansToDegrees(radians) vx_radians_to_degrees

#define SkScalarIsInt               vx_float_is_int

/**
 *  Returns -1 || 0 || 1 depending on the sign of value:
 *  -1 if x < 0
 *   0 if x == 0
 *   1 if x > 0
 */
#define SkScalarSignAsInt           vx_float_sign_as_int

// Scalar result version of above
#define SkScalarSignAsScalar        vx_float_sign_as_float

#define SK_ScalarNearlyZero         VX_FLOAT_NEARLY_ZERO

#define SkScalarNearlyZero          vx_float_nearly_zero

#define SkScalarNearlyEqual         vx_float_nearly_equal

#define SK_ScalarSinCosNearlyZero   (SK_Scalar1 / (1 << 16))

#define SkScalarSinSnapToZero       vx_float_sin_snap_to_zero

#define SkScalarCosSnapToZero       vx_float_cos_snap_to_zero

/** Linearly interpolate between A and B, based on t.
    If t is 0, return A
    If t is 1, return B
    else interpolate.
    t must be [0..SK_Scalar1]
*/
#define SkScalarInterp vx_float_lerp

/*
 *  Helper to compare an array of scalars.
 */
#define SkScalarsEqual vx_float_array_eq

#endif
