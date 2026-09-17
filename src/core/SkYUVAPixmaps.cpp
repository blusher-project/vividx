/*
 * Copyright 2020 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "include/core/SkYUVAPixmaps.h"

#include <vividx/core/alpha-type.h>
#include "include/private/SkDebug.h"
#include "src/core/SkImageInfoPriv.h"
#include "src/core/SkRectMemcpy.h"
#include "src/core/SkYUVAInfoLocation.h"

#include <algorithm>
#include <cstdint>
#include <utility>

void SkYUVAPixmapInfo::SupportedDataTypes::enableDataType(DataType type, int numChannels) {
    if (numChannels < 1 || numChannels > 4) {
        return;
    }
    fDataTypeSupport[static_cast<size_t>(type) + (numChannels - 1)*kDataTypeCnt] = true;
}

//////////////////////////////////////////////////////////////////////////////

std::tuple<int, SkYUVAPixmapInfo::DataType> SkYUVAPixmapInfo::NumChannelsAndDataType(
        vx_color_type ct) {
    // We could allow BGR[A] color types, but then we'd have to decide whether B should be the 0th
    // or 2nd channel. Our docs currently say channel order is always R=0, G=1, B=2[, A=3].
    switch (ct) {
        case VX_COLOR_TYPE_R8_UNORM:
        case VX_COLOR_TYPE_ALPHA_8:
        case VX_COLOR_TYPE_GRAY_8:    return {1, DataType::kUnorm8 };
        case VX_COLOR_TYPE_R16_UNORM:
        case VX_COLOR_TYPE_A16_UNORM: return {1, DataType::kUnorm16};
        case VX_COLOR_TYPE_R16_FLOAT:
        case VX_COLOR_TYPE_A16_FLOAT: return {1, DataType::kFloat16};

        case VX_COLOR_TYPE_R8G8_UNORM:   return {2, DataType::kUnorm8  };
        case VX_COLOR_TYPE_R16G16_UNORM: return {2, DataType::kUnorm16 };
        case VX_COLOR_TYPE_R16G16_FLOAT: return {2, DataType::kFloat16 };

        case VX_COLOR_TYPE_RGB_888X:       return {3, DataType::kUnorm8          };
        case VX_COLOR_TYPE_RGB_101010X:    return {3, DataType::kUnorm10_Unorm2  };
        case VX_COLOR_TYPE_RGB_F16F16F16X: return {3, DataType::kFloat16         };

        case VX_COLOR_TYPE_RGBA_8888:          return {4, DataType::kUnorm8  };
        case VX_COLOR_TYPE_R16G16B16A16_UNORM: return {4, DataType::kUnorm16 };
        case VX_COLOR_TYPE_RGBA_F16:           return {4, DataType::kFloat16 };
        case VX_COLOR_TYPE_RGBA_F16NORM:       return {4, DataType::kFloat16 };
        case VX_COLOR_TYPE_RGBA_1010102:       return {4, DataType::kUnorm10_Unorm2 };

        default: return {0, DataType::kUnorm8 };
    }
}

SkYUVAPixmapInfo::SkYUVAPixmapInfo(const SkYUVAInfo& yuvaInfo,
                                   const vx_color_type colorTypes[kMaxPlanes],
                                   const size_t rowBytes[kMaxPlanes])
        : fYUVAInfo(yuvaInfo) {
    if (!yuvaInfo.isValid()) {
        *this = {};
        SkASSERT(!this->isValid());
        return;
    }
    SkISize planeDimensions[4];
    int n = yuvaInfo.planeDimensions(planeDimensions);
    size_t tempRowBytes[kMaxPlanes];
    if (!rowBytes) {
        for (int i = 0; i < n; ++i) {
            tempRowBytes[i] = vx_color_type_bytes_per_pixel(colorTypes[i]) * planeDimensions[i].width();
        }
        rowBytes = tempRowBytes;
    }
    bool ok = true;
    for (size_t i = 0; i < static_cast<size_t>(n); ++i) {
        fRowBytes[i] = rowBytes[i];
        // Use kUnpremul so that we never multiply alpha when copying data in.
        fPlaneInfos[i] = SkImageInfo::Make(planeDimensions[i],
                                           colorTypes[i],
                                           VX_ALPHA_TYPE_UNPREMULTIPLIED);
        int numRequiredChannels = yuvaInfo.numChannelsInPlane(i);
        SkASSERT(numRequiredChannels > 0);
        auto [numColorTypeChannels, colorTypeDataType] = NumChannelsAndDataType(colorTypes[i]);
        ok &= i == 0 || colorTypeDataType == fDataType;
        ok &= numColorTypeChannels >= numRequiredChannels;
        ok &= fPlaneInfos[i].validRowBytes(fRowBytes[i]);
        fDataType = colorTypeDataType;
    }
    if (!ok) {
        *this = {};
        SkASSERT(!this->isValid());
    } else {
        SkASSERT(this->isValid());
    }
}

SkYUVAPixmapInfo::SkYUVAPixmapInfo(const SkYUVAInfo& yuvaInfo,
                                   DataType dataType,
                                   const size_t rowBytes[kMaxPlanes]) {
    vx_color_type colorTypes[kMaxPlanes] = {};
    int numPlanes = yuvaInfo.numPlanes();
    for (int i = 0; i < numPlanes; ++i) {
        int numChannels = yuvaInfo.numChannelsInPlane(i);
        colorTypes[i] = DefaultColorTypeForDataType(dataType, numChannels);
    }
    *this = SkYUVAPixmapInfo(yuvaInfo, colorTypes, rowBytes);
}

bool SkYUVAPixmapInfo::operator==(const SkYUVAPixmapInfo& that) const {
    bool result = fYUVAInfo   == that.fYUVAInfo   &&
                  fPlaneInfos == that.fPlaneInfos &&
                  fRowBytes   == that.fRowBytes;
    SkASSERT(!result || fDataType == that.fDataType);
    return result;
}

size_t SkYUVAPixmapInfo::computeTotalBytes(size_t planeSizes[kMaxPlanes]) const {
    if (!this->isValid()) {
        if (planeSizes) {
            std::fill_n(planeSizes, kMaxPlanes, 0);
        }
        return 0;
    }
    return fYUVAInfo.computeTotalBytes(fRowBytes.data(), planeSizes);
}

bool SkYUVAPixmapInfo::initPixmapsFromSingleAllocation(void* memory,
                                                       SkPixmap pixmaps[kMaxPlanes]) const {
    if (!this->isValid()) {
        return false;
    }
    SkASSERT(pixmaps);
    char* addr = static_cast<char*>(memory);
    int n = this->numPlanes();
    for (int i = 0; i < n; ++i) {
        SkASSERT(fPlaneInfos[i].validRowBytes(fRowBytes[i]));
        pixmaps[i].reset(fPlaneInfos[i], addr, fRowBytes[i]);
        size_t planeSize = pixmaps[i].rowBytes()*pixmaps[i].height();
        SkASSERT(planeSize);
        addr += planeSize;
    }
    for (int i = n; i < kMaxPlanes; ++i) {
        pixmaps[i] = {};
    }
    return true;
}

bool SkYUVAPixmapInfo::isSupported(const SupportedDataTypes& supportedDataTypes) const {
    if (!this->isValid()) {
        return false;
    }
    return supportedDataTypes.supported(fYUVAInfo.planeConfig(), fDataType);
}

//////////////////////////////////////////////////////////////////////////////

vx_color_type SkYUVAPixmaps::RecommendedRGBAColorType(DataType dataType) {
    switch (dataType) {
        case DataType::kUnorm8:         return VX_COLOR_TYPE_RGBA_8888;
        // F16 has better GPU support than 16 bit unorm. Often "16" bit unorm values are actually
        // lower precision.
        case DataType::kUnorm16:        return VX_COLOR_TYPE_RGBA_F16;
        case DataType::kFloat16:        return VX_COLOR_TYPE_RGBA_F16;
        case DataType::kUnorm10_Unorm2: return VX_COLOR_TYPE_RGBA_1010102;
    }
    SkUNREACHABLE;
}

SkYUVAPixmaps SkYUVAPixmaps::Allocate(const SkYUVAPixmapInfo& yuvaPixmapInfo) {
    if (!yuvaPixmapInfo.isValid()) {
        return {};
    }
    return SkYUVAPixmaps(yuvaPixmapInfo,
                         SkData::MakeUninitialized(yuvaPixmapInfo.computeTotalBytes()));
}

SkYUVAPixmaps SkYUVAPixmaps::FromData(const SkYUVAPixmapInfo& yuvaPixmapInfo, sk_sp<SkData> data) {
    if (!yuvaPixmapInfo.isValid()) {
        return {};
    }
    if (yuvaPixmapInfo.computeTotalBytes() > data->size()) {
        return {};
    }
    return SkYUVAPixmaps(yuvaPixmapInfo, std::move(data));
}

SkYUVAPixmaps SkYUVAPixmaps::MakeCopy(const SkYUVAPixmaps& src) {
    if (!src.isValid()) {
        return {};
    }
    SkYUVAPixmaps result = Allocate(src.pixmapsInfo());
    int n = result.numPlanes();
    for (int i = 0; i < n; ++i) {
        // We use SkRectMemCpy rather than readPixels to ensure that we don't do any alpha type
        // conversion.
        const SkPixmap& s = src.plane(i);
        const SkPixmap& d = result.plane(i);
        SkRectMemcpy(d.writable_addr(),
                     d.rowBytes(),
                     s.addr(),
                     s.rowBytes(),
                     s.info().minRowBytes(),
                     s.height());
    }
    return result;
}

SkYUVAPixmaps SkYUVAPixmaps::FromExternalMemory(const SkYUVAPixmapInfo& yuvaPixmapInfo,
                                                void* memory) {
    if (!yuvaPixmapInfo.isValid()) {
        return {};
    }
    SkPixmap pixmaps[kMaxPlanes];
    yuvaPixmapInfo.initPixmapsFromSingleAllocation(memory, pixmaps);
    return SkYUVAPixmaps(yuvaPixmapInfo.yuvaInfo(), yuvaPixmapInfo.dataType(), pixmaps);
}

SkYUVAPixmaps SkYUVAPixmaps::FromExternalPixmaps(const SkYUVAInfo& yuvaInfo,
                                                 const SkPixmap pixmaps[kMaxPlanes]) {
    vx_color_type colorTypes[kMaxPlanes] = {};
    size_t rowBytes[kMaxPlanes] = {};
    int numPlanes = yuvaInfo.numPlanes();
    for (int i = 0; i < numPlanes; ++i) {
        colorTypes[i] = pixmaps[i].colorType();
        rowBytes[i] = pixmaps[i].rowBytes();
    }
    SkYUVAPixmapInfo yuvaPixmapInfo(yuvaInfo, colorTypes, rowBytes);
    if (!yuvaPixmapInfo.isValid()) {
        return {};
    }
    return SkYUVAPixmaps(yuvaInfo, yuvaPixmapInfo.dataType(), pixmaps);
}

SkYUVAPixmaps::SkYUVAPixmaps(const SkYUVAPixmapInfo& yuvaPixmapInfo, sk_sp<SkData> data)
        : fData(std::move(data))
        , fYUVAInfo(yuvaPixmapInfo.yuvaInfo())
        , fDataType(yuvaPixmapInfo.dataType()) {
    SkASSERT(yuvaPixmapInfo.isValid());
    SkASSERT(yuvaPixmapInfo.computeTotalBytes() <= fData->size());
    SkAssertResult(yuvaPixmapInfo.initPixmapsFromSingleAllocation(fData->writable_data(),
                                                                  fPlanes.data()));
}

SkYUVAPixmaps::SkYUVAPixmaps(const SkYUVAInfo& yuvaInfo,
                             DataType dataType,
                             const SkPixmap pixmaps[kMaxPlanes])
        : fYUVAInfo(yuvaInfo), fDataType(dataType) {
    std::copy_n(pixmaps, yuvaInfo.numPlanes(), fPlanes.data());
}

SkYUVAPixmapInfo SkYUVAPixmaps::pixmapsInfo() const {
    if (!this->isValid()) {
        return {};
    }
    vx_color_type colorTypes[kMaxPlanes] = {};
    size_t rowBytes[kMaxPlanes] = {};
    int numPlanes = this->numPlanes();
    for (int i = 0; i < numPlanes; ++i) {
        colorTypes[i] = fPlanes[i].colorType();
        rowBytes[i] = fPlanes[i].rowBytes();
    }
    return {fYUVAInfo, colorTypes, rowBytes};
}

SkYUVAInfo::YUVALocations SkYUVAPixmaps::toYUVALocations() const {
    uint32_t channelFlags[] = {vx_color_type_channel_flags(fPlanes[0].colorType()),
                               vx_color_type_channel_flags(fPlanes[1].colorType()),
                               vx_color_type_channel_flags(fPlanes[2].colorType()),
                               vx_color_type_channel_flags(fPlanes[3].colorType())};
    auto result = fYUVAInfo.toYUVALocations(channelFlags);
    SkDEBUGCODE(int numPlanes;)
    SkASSERT(SkYUVAInfo::YUVALocation::AreValidLocations(result, &numPlanes));
    SkASSERT(numPlanes == this->numPlanes());
    return result;
}
