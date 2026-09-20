/*
 * Copyright 2026 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "include/core/SkCanvas.h"
#include "include/core/SkColor.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkSize.h"
#include "include/core/SkSurface.h"
#include "src/core/SkAutoPixmapStorage.h"
#include "src/core/SkConvertPixels.h"
#include "tests/Test.h"

#include <vividx/core/alpha-type.h>
#include <vividx/core/color-type.h>

DEF_TEST(ConvertPixels_in_place, r) {
    static constexpr SkISize kTestSize = { 256, 256 };
    static constexpr vx_color_type gTestCTs[] = {
        VX_COLOR_TYPE_ALPHA_8,
        VX_COLOR_TYPE_RGB_565,
        VX_COLOR_TYPE_ARGB_4444,
        VX_COLOR_TYPE_RGBA_8888,
        VX_COLOR_TYPE_BGRA_8888,
        VX_COLOR_TYPE_RGBA_1010102,
        VX_COLOR_TYPE_BGRA_1010102,
        VX_COLOR_TYPE_GRAY_8,
        VX_COLOR_TYPE_RGBA_F16NORM,
        VX_COLOR_TYPE_RGBA_F16,
        VX_COLOR_TYPE_RGBA_F32,
    };

    auto surface = SkSurfaces::Raster(SkImageInfo::MakeN32Premul(kTestSize));
    surface->getCanvas()->drawColor(SK_ColorGREEN);
    auto image = surface->makeImageSnapshot();

    for (const auto& srcCT :gTestCTs) {
        const auto srcInfo = SkImageInfo::Make(kTestSize, srcCT, VX_ALPHA_TYPE_PREMULTIPLIED);
        SkAutoPixmapStorage pm;
        pm.alloc(srcInfo);
        REPORTER_ASSERT(r, image->readPixels(nullptr, pm, 0, 0));

        for (const auto& dstCT : gTestCTs) {
            const auto dstInfo = SkImageInfo::Make(kTestSize, dstCT, VX_ALPHA_TYPE_PREMULTIPLIED);
            // Expected to succeed iff bpp matches.
            const bool should_succeed = srcInfo.bytesPerPixel() == dstInfo.bytesPerPixel();
            REPORTER_ASSERT(r, SkConvertPixels(dstInfo, pm.writable_addr(), dstInfo.minRowBytes(),
                                               srcInfo, pm.addr()         , srcInfo.minRowBytes())
                                    == should_succeed);
        }
    }
}
