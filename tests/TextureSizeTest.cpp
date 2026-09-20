/*
 * Copyright 2023 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include <vividx/core/alpha-type.h>
#include "include/core/SkBitmap.h"
#include "include/core/SkColor.h"
#include <vividx/core/color-type.h>
#include "include/core/SkImage.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkRefCnt.h"
#include "tests/CtsEnforcement.h"
#include "tests/Test.h"

#if defined(SK_GRAPHITE)
#include "include/gpu/graphite/Context.h"
#include "include/gpu/graphite/Image.h"
#include "include/gpu/graphite/Recorder.h"
#endif

#if defined(SK_GANESH)
#include "include/gpu/GpuTypes.h"
#include "include/gpu/ganesh/GrDirectContext.h"
#include "include/gpu/ganesh/SkImageGanesh.h"
#include "src/gpu/ganesh/GrCaps.h"
#include "src/gpu/ganesh/GrDirectContextPriv.h"
struct GrContextOptions;
#endif

#include <cstddef>
#include <functional>
#include <initializer_list>

namespace {

void run_test(skiatest::Reporter* reporter, bool testMipmaps,
              std::function<sk_sp<SkImage>(SkImage*, skgpu::Mipmapped)> convert2gpu) {

    for (auto mm : { skgpu::Mipmapped::kYes, skgpu::Mipmapped::kNo }) {
        if (!testMipmaps && mm == skgpu::Mipmapped::kYes) {
            continue;
        }

        for (auto ct : { VX_COLOR_TYPE_RGBA_8888, VX_COLOR_TYPE_ALPHA_8 }) {
            SkImageInfo ii = SkImageInfo::Make(9, 9, ct, VX_ALPHA_TYPE_PREMULTIPLIED);

            SkBitmap src;
            src.allocPixels(ii);
            src.eraseColor(SK_ColorWHITE);

            sk_sp<SkImage> raster = src.asImage();

            sk_sp<SkImage> gpu = convert2gpu(raster.get(), mm);

            int bytesPerPixel = vx_color_type_bytes_per_pixel(ct);

            size_t expectedSize = bytesPerPixel * gpu->width() * gpu->height();
            if (mm == skgpu::Mipmapped::kYes) {
                expectedSize += expectedSize/3;
            }

            size_t actualSize = gpu->textureSize();

            REPORTER_ASSERT(reporter, actualSize == expectedSize,
                            "Expected: %zu  Actual: %zu", expectedSize, actualSize);
        }
    }
}

} // anonymous namespace

#if defined(SK_GANESH)

DEF_GANESH_TEST_FOR_ALL_CONTEXTS(ImageSizeTest_Ganesh,
                                 reporter,
                                 ctxInfo,
                                 CtsEnforcement::kApiLevel_202404) {
    auto dContext = ctxInfo.directContext();

    bool testMipmaps = dContext->priv().caps()->mipmapSupport();

    run_test(reporter, testMipmaps,
             [&](SkImage* src, skgpu::Mipmapped mipmapped) -> sk_sp<SkImage> {
                 return SkImages::TextureFromImage(dContext, src, mipmapped);
             });
}

#endif // SK_GANESH

#if defined(SK_GRAPHITE)

DEF_GRAPHITE_TEST_FOR_ALL_CONTEXTS(ImageSizeTest_Graphite, reporter, context,
                                   CtsEnforcement::kApiLevel_202404) {
    using namespace skgpu::graphite;

    std::unique_ptr<Recorder> recorder = context->makeRecorder();

    run_test(reporter, /* testMipmaps= */ true,
             [&](SkImage* src, skgpu::Mipmapped mipmapped) -> sk_sp<SkImage> {
                 return SkImages::TextureFromImage(recorder.get(), src,
                                                   { mipmapped == skgpu::Mipmapped::kYes });
             });
}

#endif // SK_GRAPHITE
