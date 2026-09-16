/*
 * Copyright 2010 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "include/core/SkImageInfo.h"

#include "include/core/SkColor.h"
#include "include/core/SkColorSpace.h"
#include "include/private/SkAssert.h"
#include "src/core/SkImageInfoPriv.h"
#include "src/core/SkSafeMath.h"

int SkColorTypeBytesPerPixel(SkColorType ct) {
    switch (ct) {
        case kUnknown_SkColorType:            return 0;
        case kAlpha_8_SkColorType:            return 1;
        case kRGB_565_SkColorType:            return 2;
        case kARGB_4444_SkColorType:          return 2;
        case kRGBA_8888_SkColorType:          return 4;
        case kBGRA_8888_SkColorType:          return 4;
        case kRGB_888x_SkColorType:           return 4;
        case kRGBA_1010102_SkColorType:       return 4;
        case kRGB_101010x_SkColorType:        return 4;
        case kBGRA_1010102_SkColorType:       return 4;
        case kBGR_101010x_SkColorType:        return 4;
        case kBGR_101010x_XR_SkColorType:     return 4;
        case kBGRA_10101010_XR_SkColorType:   return 8;
        case kRGBA_10x6_SkColorType:          return 8;
        case kGray_8_SkColorType:             return 1;
        case kRGBA_F16Norm_SkColorType:       return 8;
        case kRGBA_F16_SkColorType:           return 8;
        case kRGB_F16F16F16x_SkColorType:     return 8;
        case kRGBA_F32_SkColorType:           return 16;
        case kR8G8_unorm_SkColorType:         return 2;
        case kA16_unorm_SkColorType:          return 2;
        case kR16_unorm_SkColorType:          return 2;
        case kR16G16_unorm_SkColorType:       return 4;
        case kA16_float_SkColorType:          return 2;
        case kR16_float_SkColorType:          return 2;
        case kR16G16_float_SkColorType:       return 4;
        case kR16G16B16A16_unorm_SkColorType: return 8;
        case kSRGBA_8888_SkColorType:         return 4;
        case kR8_unorm_SkColorType:           return 1;
    }
    SkUNREACHABLE;
}

bool SkColorTypeIsAlwaysOpaque(SkColorType ct) {
    return !(SkColorTypeChannelFlags(ct) & kAlpha_SkColorChannelFlag);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool SkYUVColorSpaceIsLimitedRange(vx_yuv_color_space cs) {
    switch (cs) {
        case VX_YUV_COLOR_SPACE_REC601_LIMITED:
        case VX_YUV_COLOR_SPACE_REC709_LIMITED:
        case VX_YUV_COLOR_SPACE_BT2020_8BIT_LIMITED:
        case VX_YUV_COLOR_SPACE_BT2020_10BIT_LIMITED:
        case VX_YUV_COLOR_SPACE_BT2020_12BIT_LIMITED:
        case VX_YUV_COLOR_SPACE_BT2020_16BIT_LIMITED:
        case VX_YUV_COLOR_SPACE_FCC_LIMITED:
        case VX_YUV_COLOR_SPACE_SMPTE240_LIMITED:
        case VX_YUV_COLOR_SPACE_YDZDX_LIMITED:
        case VX_YUV_COLOR_SPACE_GBR_LIMITED:
        case VX_YUV_COLOR_SPACE_YCGCO_8BIT_LIMITED:
        case VX_YUV_COLOR_SPACE_YCGCO_10BIT_LIMITED:
        case VX_YUV_COLOR_SPACE_YCGCO_12BIT_LIMITED:
        case VX_YUV_COLOR_SPACE_YCGCO_16BIT_LIMITED:
            return true;

        case VX_YUV_COLOR_SPACE_JPEG_FULL:
        case VX_YUV_COLOR_SPACE_REC709_FULL:
        case VX_YUV_COLOR_SPACE_BT2020_8BIT_FULL:
        case VX_YUV_COLOR_SPACE_BT2020_10BIT_FULL:
        case VX_YUV_COLOR_SPACE_BT2020_12BIT_FULL:
        case VX_YUV_COLOR_SPACE_BT2020_16BIT_FULL:
        case VX_YUV_COLOR_SPACE_FCC_FULL:
        case VX_YUV_COLOR_SPACE_SMPTE240_FULL:
        case VX_YUV_COLOR_SPACE_YDZDX_FULL:
        case VX_YUV_COLOR_SPACE_GBR_FULL:
        case VX_YUV_COLOR_SPACE_YCGCO_8BIT_FULL:
        case VX_YUV_COLOR_SPACE_YCGCO_10BIT_FULL:
        case VX_YUV_COLOR_SPACE_YCGCO_12BIT_FULL:
        case VX_YUV_COLOR_SPACE_YCGCO_16BIT_FULL:
        case VX_YUV_COLOR_SPACE_IDENTITY:
            return false;

        default:
            SkUNREACHABLE;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

SkColorInfo::SkColorInfo() = default;
SkColorInfo::~SkColorInfo() = default;

SkColorInfo::SkColorInfo(SkColorType ct, vx_alpha_type at, sk_sp<SkColorSpace> cs)
            : fColorSpace(std::move(cs)), fColorType(ct), fAlphaType(at) {}

SkColorInfo::SkColorInfo(const SkColorInfo&) = default;
SkColorInfo::SkColorInfo(SkColorInfo&&) = default;

SkColorInfo& SkColorInfo::operator=(const SkColorInfo&) = default;
SkColorInfo& SkColorInfo::operator=(SkColorInfo&&) = default;

SkColorSpace* SkColorInfo::colorSpace() const { return fColorSpace.get(); }
sk_sp<SkColorSpace> SkColorInfo::refColorSpace() const { return fColorSpace; }

bool SkColorInfo::operator==(const SkColorInfo& other) const {
    return fColorType == other.fColorType && fAlphaType == other.fAlphaType &&
           SkColorSpace::Equals(fColorSpace.get(), other.fColorSpace.get());
}

bool SkColorInfo::operator!=(const SkColorInfo& other) const { return !(*this == other); }

SkColorInfo SkColorInfo::makeAlphaType(vx_alpha_type newAlphaType) const {
    return SkColorInfo(this->colorType(), newAlphaType, this->refColorSpace());
}

SkColorInfo SkColorInfo::makeColorType(SkColorType newColorType) const {
    return SkColorInfo(newColorType, this->alphaType(), this->refColorSpace());
}

SkColorInfo SkColorInfo::makeColorSpace(sk_sp<SkColorSpace> cs) const {
    return SkColorInfo(this->colorType(), this->alphaType(), std::move(cs));
}

int SkColorInfo::bytesPerPixel() const { return SkColorTypeBytesPerPixel(fColorType); }

bool SkColorInfo::gammaCloseToSRGB() const {
    return fColorSpace && fColorSpace->gammaCloseToSRGB();
}

int SkColorInfo::shiftPerPixel() const { return SkColorTypeShiftPerPixel(fColorType); }

///////////////////////////////////////////////////////////////////////////////////////////////////

size_t SkImageInfo::computeOffset(int x, int y, size_t rowBytes) const {
    SkASSERTF(x >= 0 && x < this->width(), "x=%d; width=%d\n", x, this->width());
    SkASSERTF(y >= 0 && y < this->height(), "y=%d; height=%d\n", y, this->height());
    return SkColorTypeComputeOffset(this->colorType(), x, y, rowBytes);
}

size_t SkImageInfo::computeByteSize(size_t rowBytes) const {
    if (0 == this->height()) {
        return 0;
    }
    SkSafeMath safe;
    size_t bytes = safe.add(safe.mul(safe.addInt(this->height(), -1), rowBytes),
                            safe.mul(this->width(), this->bytesPerPixel()));

    // The CPU backend implements some memory operations on images using instructions that take a
    // signed 32-bit offset from the base. If we ever make an image larger than that, overflow can
    // cause us to read/write memory that starts 2GB *before* the buffer. (crbug.com/1264705)
    constexpr size_t kMaxSigned32BitSize = SK_MaxS32;
    return (safe.ok() && (bytes <= kMaxSigned32BitSize)) ? bytes : SIZE_MAX;
}

SkColorSpace* SkImageInfo::colorSpace() const { return fColorInfo.colorSpace(); }

sk_sp<SkColorSpace> SkImageInfo::refColorSpace() const { return fColorInfo.refColorSpace(); }

SkImageInfo SkImageInfo::makeColorSpace(sk_sp<SkColorSpace> cs) const {
    return Make(fDimensions, fColorInfo.makeColorSpace(std::move(cs)));
}

SkImageInfo SkImageInfo::Make(int width, int height, SkColorType ct, vx_alpha_type at) {
    return Make(width, height, ct, at, nullptr);
}

SkImageInfo SkImageInfo::Make(int width, int height, SkColorType ct, vx_alpha_type at,
                              sk_sp<SkColorSpace> cs) {
    return SkImageInfo({width, height}, {ct, at, std::move(cs)});
}

SkImageInfo SkImageInfo::Make(SkISize dimensions, SkColorType ct, vx_alpha_type at) {
    return Make(dimensions, ct, at, nullptr);
}

SkImageInfo SkImageInfo::Make(SkISize dimensions, SkColorType ct, vx_alpha_type at,
                        sk_sp<SkColorSpace> cs) {
    return SkImageInfo(dimensions, {ct, at, std::move(cs)});
}

SkImageInfo SkImageInfo::MakeN32(int width, int height, vx_alpha_type at) {
    return MakeN32(width, height, at, nullptr);
}

SkImageInfo SkImageInfo::MakeN32(int width, int height, vx_alpha_type at, sk_sp<SkColorSpace> cs) {
    return Make({width, height}, kN32_SkColorType, at, std::move(cs));
}

SkImageInfo SkImageInfo::MakeS32(int width, int height, vx_alpha_type at) {
    return SkImageInfo({width, height}, {kN32_SkColorType, at, SkColorSpace::MakeSRGB()});
}

SkImageInfo SkImageInfo::MakeN32Premul(int width, int height) {
    return MakeN32Premul(width, height, nullptr);
}

SkImageInfo SkImageInfo::MakeN32Premul(int width, int height, sk_sp<SkColorSpace> cs) {
    return Make({width, height}, kN32_SkColorType, VX_ALPHA_TYPE_PREMULTIPLIED, std::move(cs));
}

SkImageInfo SkImageInfo::MakeN32Premul(SkISize dimensions) {
    return MakeN32Premul(dimensions, nullptr);
}

SkImageInfo SkImageInfo::MakeN32Premul(SkISize dimensions, sk_sp<SkColorSpace> cs) {
    return Make(dimensions, kN32_SkColorType, VX_ALPHA_TYPE_PREMULTIPLIED, std::move(cs));
}

SkImageInfo SkImageInfo::MakeA8(int width, int height) {
    return Make({width, height}, kAlpha_8_SkColorType, VX_ALPHA_TYPE_PREMULTIPLIED, nullptr);
}

SkImageInfo SkImageInfo::MakeA8(SkISize dimensions) {
    return Make(dimensions, kAlpha_8_SkColorType, VX_ALPHA_TYPE_PREMULTIPLIED, nullptr);
}

SkImageInfo SkImageInfo::MakeUnknown(int width, int height) {
    return Make({width, height}, kUnknown_SkColorType, VX_ALPHA_TYPE_UNKNOWN, nullptr);
}

#ifdef SK_DEBUG
void SkImageInfo::validate() const {
    SkASSERT(fDimensions.width() >= 0);
    SkASSERT(fDimensions.height() >= 0);
    SkASSERT(SkColorTypeIsValid(this->colorType()));
    SkASSERT(SkAlphaTypeIsValid(this->alphaType()));
}
#endif

bool SkColorTypeValidateAlphaType(SkColorType colorType, vx_alpha_type alphaType,
                                  vx_alpha_type* canonical) {
    switch (colorType) {
        case kUnknown_SkColorType:
            alphaType = VX_ALPHA_TYPE_UNKNOWN;
            break;
        case kAlpha_8_SkColorType:         // fall-through
        case kA16_unorm_SkColorType:       // fall-through
        case kA16_float_SkColorType:
            if (VX_ALPHA_TYPE_UNPREMULTIPLIED == alphaType) {
                alphaType = VX_ALPHA_TYPE_PREMULTIPLIED;
            }
            [[fallthrough]];
        case kARGB_4444_SkColorType:
        case kRGBA_8888_SkColorType:
        case kSRGBA_8888_SkColorType:
        case kBGRA_8888_SkColorType:
        case kRGBA_1010102_SkColorType:
        case kBGRA_1010102_SkColorType:
        case kRGBA_10x6_SkColorType:
        case kRGBA_F16Norm_SkColorType:
        case kRGBA_F16_SkColorType:
        case kRGBA_F32_SkColorType:
        case kBGRA_10101010_XR_SkColorType:
        case kR16G16B16A16_unorm_SkColorType:
            if (VX_ALPHA_TYPE_UNKNOWN == alphaType) {
                return false;
            }
            break;
        case kGray_8_SkColorType:
        case kR8G8_unorm_SkColorType:
        case kR16_unorm_SkColorType:
        case kR16_float_SkColorType:
        case kR16G16_unorm_SkColorType:
        case kR16G16_float_SkColorType:
        case kRGB_565_SkColorType:
        case kRGB_888x_SkColorType:
        case kRGB_101010x_SkColorType:
        case kBGR_101010x_SkColorType:
        case kBGR_101010x_XR_SkColorType:
        case kRGB_F16F16F16x_SkColorType:
        case kR8_unorm_SkColorType:
            alphaType = VX_ALPHA_TYPE_OPAQUE;
            break;
    }
    if (canonical) {
        *canonical = alphaType;
    }
    return true;
}
