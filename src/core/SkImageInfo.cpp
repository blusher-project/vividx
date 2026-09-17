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

#include <vividx/core/color-type.h>


///////////////////////////////////////////////////////////////////////////////////////////////////

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

int SkColorInfo::bytesPerPixel() const { return vx_color_type_bytes_per_pixel(fColorType); }

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
