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

int SkColorTypeBytesPerPixel(vx_color_type ct) {
    switch (ct) {
        case VX_COLOR_TYPE_UNKNOWN:            return 0;
        case VX_COLOR_TYPE_ALPHA_8:            return 1;
        case VX_COLOR_TYPE_RGB_565:            return 2;
        case VX_COLOR_TYPE_ARGB_4444:          return 2;
        case VX_COLOR_TYPE_RGBA_8888:          return 4;
        case VX_COLOR_TYPE_BGRA_8888:          return 4;
        case VX_COLOR_TYPE_RGB_888X:           return 4;
        case VX_COLOR_TYPE_RGBA_1010102:       return 4;
        case VX_COLOR_TYPE_RGB_101010X:        return 4;
        case VX_COLOR_TYPE_BGRA_1010102:       return 4;
        case VX_COLOR_TYPE_BGR_101010X:        return 4;
        case VX_COLOR_TYPE_BGR_101010X_XR:     return 4;
        case VX_COLOR_TYPE_BGRA_10101010_XR:   return 8;
        case VX_COLOR_TYPE_RGBA_10X6:          return 8;
        case VX_COLOR_TYPE_GRAY_8:             return 1;
        case VX_COLOR_TYPE_RGBA_F16NORM:       return 8;
        case VX_COLOR_TYPE_RGBA_F16:           return 8;
        case VX_COLOR_TYPE_RGB_F16F16F16X:     return 8;
        case VX_COLOR_TYPE_RGBA_F32:           return 16;
        case VX_COLOR_TYPE_R8G8_UNORM:         return 2;
        case VX_COLOR_TYPE_A16_UNORM:          return 2;
        case VX_COLOR_TYPE_R16_UNORM:          return 2;
        case VX_COLOR_TYPE_R16G16_UNORM:       return 4;
        case VX_COLOR_TYPE_A16_FLOAT:          return 2;
        case VX_COLOR_TYPE_R16_FLOAT:          return 2;
        case VX_COLOR_TYPE_R16G16_FLOAT:       return 4;
        case VX_COLOR_TYPE_R16G16B16A16_UNORM: return 8;
        case VX_COLOR_TYPE_SRGBA_8888:         return 4;
        case VX_COLOR_TYPE_R8_UNORM:           return 1;
    }
    SkUNREACHABLE;
}

bool SkColorTypeIsAlwaysOpaque(vx_color_type ct) {
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

SkColorInfo::SkColorInfo(vx_color_type ct, vx_alpha_type at, sk_sp<SkColorSpace> cs)
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

SkColorInfo SkColorInfo::makeColorType(vx_color_type newColorType) const {
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

SkImageInfo SkImageInfo::Make(int width, int height, vx_color_type ct, vx_alpha_type at) {
    return Make(width, height, ct, at, nullptr);
}

SkImageInfo SkImageInfo::Make(int width, int height, vx_color_type ct, vx_alpha_type at,
                              sk_sp<SkColorSpace> cs) {
    return SkImageInfo({width, height}, {ct, at, std::move(cs)});
}

SkImageInfo SkImageInfo::Make(SkISize dimensions, vx_color_type ct, vx_alpha_type at) {
    return Make(dimensions, ct, at, nullptr);
}

SkImageInfo SkImageInfo::Make(SkISize dimensions, vx_color_type ct, vx_alpha_type at,
                        sk_sp<SkColorSpace> cs) {
    return SkImageInfo(dimensions, {ct, at, std::move(cs)});
}

SkImageInfo SkImageInfo::MakeN32(int width, int height, vx_alpha_type at) {
    return MakeN32(width, height, at, nullptr);
}

SkImageInfo SkImageInfo::MakeN32(int width, int height, vx_alpha_type at, sk_sp<SkColorSpace> cs) {
    return Make({width, height}, VX_COLOR_TYPE_N32, at, std::move(cs));
}

SkImageInfo SkImageInfo::MakeS32(int width, int height, vx_alpha_type at) {
    return SkImageInfo({width, height}, {VX_COLOR_TYPE_N32, at, SkColorSpace::MakeSRGB()});
}

SkImageInfo SkImageInfo::MakeN32Premul(int width, int height) {
    return MakeN32Premul(width, height, nullptr);
}

SkImageInfo SkImageInfo::MakeN32Premul(int width, int height, sk_sp<SkColorSpace> cs) {
    return Make({width, height}, VX_COLOR_TYPE_N32, VX_ALPHA_TYPE_PREMULTIPLIED, std::move(cs));
}

SkImageInfo SkImageInfo::MakeN32Premul(SkISize dimensions) {
    return MakeN32Premul(dimensions, nullptr);
}

SkImageInfo SkImageInfo::MakeN32Premul(SkISize dimensions, sk_sp<SkColorSpace> cs) {
    return Make(dimensions, VX_COLOR_TYPE_N32, VX_ALPHA_TYPE_PREMULTIPLIED, std::move(cs));
}

SkImageInfo SkImageInfo::MakeA8(int width, int height) {
    return Make({width, height}, VX_COLOR_TYPE_ALPHA_8, VX_ALPHA_TYPE_PREMULTIPLIED, nullptr);
}

SkImageInfo SkImageInfo::MakeA8(SkISize dimensions) {
    return Make(dimensions, VX_COLOR_TYPE_ALPHA_8, VX_ALPHA_TYPE_PREMULTIPLIED, nullptr);
}

SkImageInfo SkImageInfo::MakeUnknown(int width, int height) {
    return Make({width, height}, VX_COLOR_TYPE_UNKNOWN, VX_ALPHA_TYPE_UNKNOWN, nullptr);
}

#ifdef SK_DEBUG
void SkImageInfo::validate() const {
    SkASSERT(fDimensions.width() >= 0);
    SkASSERT(fDimensions.height() >= 0);
    SkASSERT(SkColorTypeIsValid(this->colorType()));
    SkASSERT(SkAlphaTypeIsValid(this->alphaType()));
}
#endif

bool SkColorTypeValidateAlphaType(vx_color_type colorType, vx_alpha_type alphaType,
                                  vx_alpha_type* canonical) {
    switch (colorType) {
        case VX_COLOR_TYPE_UNKNOWN:
            alphaType = VX_ALPHA_TYPE_UNKNOWN;
            break;
        case VX_COLOR_TYPE_ALPHA_8:         // fall-through
        case VX_COLOR_TYPE_A16_UNORM:       // fall-through
        case VX_COLOR_TYPE_A16_FLOAT:
            if (VX_ALPHA_TYPE_UNPREMULTIPLIED == alphaType) {
                alphaType = VX_ALPHA_TYPE_PREMULTIPLIED;
            }
            [[fallthrough]];
        case VX_COLOR_TYPE_ARGB_4444:
        case VX_COLOR_TYPE_RGBA_8888:
        case VX_COLOR_TYPE_SRGBA_8888:
        case VX_COLOR_TYPE_BGRA_8888:
        case VX_COLOR_TYPE_RGBA_1010102:
        case VX_COLOR_TYPE_BGRA_1010102:
        case VX_COLOR_TYPE_RGBA_10X6:
        case VX_COLOR_TYPE_RGBA_F16NORM:
        case VX_COLOR_TYPE_RGBA_F16:
        case VX_COLOR_TYPE_RGBA_F32:
        case VX_COLOR_TYPE_BGRA_10101010_XR:
        case VX_COLOR_TYPE_R16G16B16A16_UNORM:
            if (VX_ALPHA_TYPE_UNKNOWN == alphaType) {
                return false;
            }
            break;
        case VX_COLOR_TYPE_GRAY_8:
        case VX_COLOR_TYPE_R8G8_UNORM:
        case VX_COLOR_TYPE_R16_UNORM:
        case VX_COLOR_TYPE_R16_FLOAT:
        case VX_COLOR_TYPE_R16G16_UNORM:
        case VX_COLOR_TYPE_R16G16_FLOAT:
        case VX_COLOR_TYPE_RGB_565:
        case VX_COLOR_TYPE_RGB_888X:
        case VX_COLOR_TYPE_RGB_101010X:
        case VX_COLOR_TYPE_BGR_101010X:
        case VX_COLOR_TYPE_BGR_101010X_XR:
        case VX_COLOR_TYPE_RGB_F16F16F16X:
        case VX_COLOR_TYPE_R8_UNORM:
            alphaType = VX_ALPHA_TYPE_OPAQUE;
            break;
    }
    if (canonical) {
        *canonical = alphaType;
    }
    return true;
}
