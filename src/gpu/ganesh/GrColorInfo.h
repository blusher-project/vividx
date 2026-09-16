/*
 * Copyright 2017 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrColorInfo_DEFINED
#define GrColorInfo_DEFINED

#include <vividx/core/alpha-type.h>
#include "include/core/SkRefCnt.h"
#include "include/private/gpu/ganesh/GrTypesPriv.h"
#include "src/gpu/ganesh/GrColorSpaceXform.h"

class SkColorInfo;
class SkColorSpace;
/**
 * All the info needed to interpret a color: Color type + alpha type + color space. Also caches
 * the GrColorSpaceXform from sRGB. */
class GrColorInfo {
public:
    GrColorInfo();
    GrColorInfo(const GrColorInfo&);
    GrColorInfo& operator=(const GrColorInfo&);
    GrColorInfo(GrColorType, vx_alpha_type, sk_sp<SkColorSpace>);
    /* implicit */ GrColorInfo(const SkColorInfo&);
    ~GrColorInfo();

    bool operator==(const GrColorInfo& that) const;
    bool operator!=(const GrColorInfo& that) const { return !(*this == that); }

    GrColorInfo makeColorType(GrColorType ct) const;

    bool isLinearlyBlended() const;

    SkColorSpace* colorSpace() const;
    sk_sp<SkColorSpace> refColorSpace() const;

    GrColorSpaceXform* colorSpaceXformFromSRGB() const { return fColorXformFromSRGB.get(); }
    sk_sp<GrColorSpaceXform> refColorSpaceXformFromSRGB() const { return fColorXformFromSRGB; }

    GrColorType colorType() const { return fColorType; }
    vx_alpha_type alphaType() const { return fAlphaType; }

    bool isAlphaOnly() const { return GrColorTypeIsAlphaOnly(fColorType); }

    bool isValid() const {
        return fColorType != GrColorType::kUnknown && fAlphaType != VX_ALPHA_TYPE_UNKNOWN;
    }

private:
    sk_sp<SkColorSpace> fColorSpace;
    sk_sp<GrColorSpaceXform> fColorXformFromSRGB;
    GrColorType fColorType = GrColorType::kUnknown;
    vx_alpha_type fAlphaType = VX_ALPHA_TYPE_UNKNOWN;
};

#endif
