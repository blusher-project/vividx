/*
 * Copyright 2019 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include <vividx/core/alpha-type.h>
#include "include/core/SkCanvas.h"
#include "include/core/SkColor.h"
#include "include/core/SkColorSpace.h"
#include <vividx/core/color-type.h>
#include "include/core/SkImage.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkRefCnt.h"
#include "include/core/SkSurface.h"
#include "include/core/SkTypes.h"
#include "include/gpu/GpuTypes.h"
#include "include/private/SkTo.h"
#include "src/core/SkAutoPixmapStorage.h"
#include "src/core/SkImageInfoPriv.h"
#include "src/gpu/ganesh/GrDirectContextPriv.h"
#include "tests/ComparePixels.h"
#include "tests/CtsEnforcement.h"
#include "tests/Test.h"
#include "tools/ToolUtils.h"

#if defined(SK_GANESH)
#include "include/gpu/ganesh/GrBackendSurface.h"
#include "include/gpu/ganesh/GrDirectContext.h"
#include "include/gpu/ganesh/GrTypes.h"
#include "include/gpu/ganesh/SkImageGanesh.h"
#include "include/gpu/ganesh/SkSurfaceGanesh.h"
#endif

#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <initializer_list>

class SkPixmap;
struct GrContextOptions;

static constexpr int kSize = 32;

static SkColor4f get_trans_black_expected_color(vx_color_channel_flag channels) {
    float a = 0;
    if (!(channels & VX_COLOR_CHANNEL_FLAG_ALPHA)) {
        a = 1;
    }

    return { 0, 0, 0, a };
}

static SkColor4f get_opaque_white_expected_color(vx_color_channel_flag channels) {
    if (channels & VX_COLOR_CHANNEL_FLAG_GRAY) {
        return { 1, 1, 1, 1 };
    }

    float r = 1, g = 1, b = 1;
    if (!(channels & VX_COLOR_CHANNEL_FLAG_RED)) {
        r = 0;
    }
    if (!(channels & VX_COLOR_CHANNEL_FLAG_GREEN)) {
        g = 0;
    }
    if (!(channels & VX_COLOR_CHANNEL_FLAG_BLUE)) {
        b = 0;
    }

    return { r, g, b, 1.0f };
}

struct TestCase {
    vx_color_type        fColorType;
    vx_alpha_type        fAlphaType;
    vx_color_channel_flag fChannels;
    bool               fGpuCanMakeSurfaces;
};

static const TestCase gTests[] = {
    { VX_COLOR_TYPE_ALPHA_8,            VX_ALPHA_TYPE_PREMULTIPLIED, VX_COLOR_CHANNEL_FLAG_ALPHA, true },
    { VX_COLOR_TYPE_A16_UNORM,          VX_ALPHA_TYPE_PREMULTIPLIED, VX_COLOR_CHANNEL_FLAG_ALPHA, false},
    { VX_COLOR_TYPE_A16_FLOAT,          VX_ALPHA_TYPE_PREMULTIPLIED, VX_COLOR_CHANNEL_FLAG_ALPHA, false},
    { VX_COLOR_TYPE_RGB_565,            VX_ALPHA_TYPE_OPAQUE, VX_COLOR_CHANNEL_FLAGS_RGB,  true },
    { VX_COLOR_TYPE_ARGB_4444,          VX_ALPHA_TYPE_PREMULTIPLIED, VX_COLOR_CHANNEL_FLAGS_RGBA, true },
    { VX_COLOR_TYPE_RGBA_8888,          VX_ALPHA_TYPE_PREMULTIPLIED, VX_COLOR_CHANNEL_FLAGS_RGBA, true },
    { VX_COLOR_TYPE_RGB_888X,           VX_ALPHA_TYPE_OPAQUE, VX_COLOR_CHANNEL_FLAGS_RGB,  true },
    { VX_COLOR_TYPE_BGRA_8888,          VX_ALPHA_TYPE_PREMULTIPLIED, VX_COLOR_CHANNEL_FLAGS_RGBA, true },
    { VX_COLOR_TYPE_RGBA_1010102,       VX_ALPHA_TYPE_PREMULTIPLIED, VX_COLOR_CHANNEL_FLAGS_RGBA, true },
    { VX_COLOR_TYPE_RGB_101010X,        VX_ALPHA_TYPE_OPAQUE, VX_COLOR_CHANNEL_FLAGS_RGB,  true },
    { VX_COLOR_TYPE_GRAY_8,             VX_ALPHA_TYPE_OPAQUE, VX_COLOR_CHANNEL_FLAG_GRAY,  true },
    { VX_COLOR_TYPE_RGBA_F16NORM,       VX_ALPHA_TYPE_PREMULTIPLIED, VX_COLOR_CHANNEL_FLAGS_RGBA, true },
    { VX_COLOR_TYPE_RGBA_F16,           VX_ALPHA_TYPE_PREMULTIPLIED, VX_COLOR_CHANNEL_FLAGS_RGBA, true },
    { VX_COLOR_TYPE_RGB_F16F16F16X,     VX_ALPHA_TYPE_OPAQUE, VX_COLOR_CHANNEL_FLAGS_RGB,  true },
    { VX_COLOR_TYPE_RGBA_F32,           VX_ALPHA_TYPE_PREMULTIPLIED, VX_COLOR_CHANNEL_FLAGS_RGBA, true },
    { VX_COLOR_TYPE_R8G8_UNORM,         VX_ALPHA_TYPE_OPAQUE, VX_COLOR_CHANNEL_FLAGS_RG,   true },
    { VX_COLOR_TYPE_R16_UNORM,          VX_ALPHA_TYPE_OPAQUE, VX_COLOR_CHANNEL_FLAG_RED,   false},
    { VX_COLOR_TYPE_R16G16_UNORM,       VX_ALPHA_TYPE_OPAQUE, VX_COLOR_CHANNEL_FLAGS_RG,   false},
    { VX_COLOR_TYPE_R16_FLOAT,          VX_ALPHA_TYPE_OPAQUE, VX_COLOR_CHANNEL_FLAG_RED,   false},
    { VX_COLOR_TYPE_R16G16_FLOAT,       VX_ALPHA_TYPE_OPAQUE, VX_COLOR_CHANNEL_FLAGS_RG,   false},
    { VX_COLOR_TYPE_R16G16B16A16_UNORM, VX_ALPHA_TYPE_PREMULTIPLIED, VX_COLOR_CHANNEL_FLAGS_RGBA, false},
};

static void raster_tests(skiatest::Reporter* reporter, const TestCase& test) {

    const SkImageInfo nativeII = SkImageInfo::Make(kSize, kSize, test.fColorType, test.fAlphaType);
    const SkImageInfo f32Unpremul = SkImageInfo::Make(kSize, kSize, VX_COLOR_TYPE_RGBA_F32,
                                                      VX_ALPHA_TYPE_UNPREMULTIPLIED);

    uint32_t actualChannels = vx_color_type_channel_flags(test.fColorType);
    REPORTER_ASSERT(reporter, test.fChannels == actualChannels);

    // all colorTypes can be drawn to
    {
        auto s = SkSurfaces::Raster(nativeII);
        REPORTER_ASSERT(reporter, SkToBool(s));
    }

    // opaque formats should make transparent black become opaque
    {
        SkAutoPixmapStorage pm;
        pm.alloc(nativeII);
        pm.erase(SkColors::kTransparent);
        SkColor actual = pm.getColor(0, 0);
        SkColor4f expected = get_trans_black_expected_color(test.fChannels);
        REPORTER_ASSERT(reporter, expected.toSkColor() == actual);
    }

    // unused channels should drop out
    {
        SkAutoPixmapStorage pm;
        pm.alloc(nativeII);
        pm.erase(SkColors::kWhite);
        SkColor actual = pm.getColor(0, 0);
        SkColor4f expected = get_opaque_white_expected_color(test.fChannels);
        REPORTER_ASSERT(reporter, expected.toSkColor() == actual);
    }

    // Reading back from an image to the same colorType should always work
    {
        SkAutoPixmapStorage srcPM;
        srcPM.alloc(nativeII);
        srcPM.erase(SkColors::kWhite);
        auto i = SkImages::RasterFromPixmap(srcPM, nullptr, nullptr);
        REPORTER_ASSERT(reporter, SkToBool(i));

        SkAutoPixmapStorage readbackPM;
        readbackPM.alloc(nativeII);
        readbackPM.erase(SkColors::kTransparent);

        REPORTER_ASSERT(reporter, i->readPixels(nullptr, readbackPM, 0, 0));

        SkColor expected = srcPM.getColor(0, 0);
        SkColor actual = readbackPM.getColor(0, 0);
        REPORTER_ASSERT(reporter, expected == actual);
    }

    // Rendering to an F32 surface should always work
    {
        SkAutoPixmapStorage srcPM;
        srcPM.alloc(nativeII);
        srcPM.erase(SkColors::kWhite);
        auto i = SkImages::RasterFromPixmap(srcPM, nullptr, nullptr);
        REPORTER_ASSERT(reporter, SkToBool(i));

        auto s = SkSurfaces::Raster(f32Unpremul);
        REPORTER_ASSERT(reporter, SkToBool(s));

        {
            auto c = s->getCanvas();
            c->drawImage(i, 0, 0);
        }

        SkAutoPixmapStorage readbackPM;
        readbackPM.alloc(f32Unpremul);
        readbackPM.erase(SkColors::kTransparent);

        REPORTER_ASSERT(reporter, i->readPixels(nullptr, readbackPM, 0, 0));

        SkColor expected = srcPM.getColor(0, 0);
        SkColor actual = readbackPM.getColor(0, 0);
        REPORTER_ASSERT(reporter, expected == actual);
    }
}

#if defined(SK_GANESH)
static void compare_pixmaps(skiatest::Reporter* reporter,
                            const SkPixmap& expected, const SkPixmap& actual,
                            vx_color_type ct, const char* label) {
    const float tols[4] = {0.0f, 0.0f, 0.0f, 0};

    auto error = std::function<ComparePixmapsErrorReporter>(
        [reporter, ct, label](int x, int y, const float diffs[4]) {
            SkASSERT(x >= 0 && y >= 0);
            ERRORF(reporter, "%s %s - mismatch at %d, %d (%f, %f, %f %f)",
                   ToolUtils::colortype_name(ct), label, x, y,
                   diffs[0], diffs[1], diffs[2], diffs[3]);
        });

    ComparePixels(expected, actual, tols, error);
}

static void ganesh_tests(GrDirectContext* dContext,
                         skiatest::Reporter* reporter,
                         const TestCase& test) {
    using namespace skgpu;

    const SkImageInfo nativeII = SkImageInfo::Make(kSize, kSize, test.fColorType, test.fAlphaType);
    const SkImageInfo f32Unpremul = SkImageInfo::Make(kSize, kSize, VX_COLOR_TYPE_RGBA_F32,
                                                      VX_ALPHA_TYPE_UNPREMULTIPLIED);

    // We had better not be able to render to prohibited colorTypes
    if (!test.fGpuCanMakeSurfaces) {
        auto s = SkSurfaces::RenderTarget(dContext, Budgeted::kNo, nativeII);
        REPORTER_ASSERT(reporter, !SkToBool(s));
    }

    if (!dContext->colorTypeSupportedAsImage(test.fColorType)) {
        return;
    }

    Protected isProtected = Protected(dContext->priv().caps()->supportsProtectedContent());

    SkAutoPixmapStorage nativeExpected;
    nativeExpected.alloc(nativeII);
    nativeExpected.erase(SkColors::kWhite);

    for (bool fullInit : { false, true }) {
        GrBackendTexture backendTex;

        bool finishedBECreate = false;
        auto markFinished = [](void* context) {
            *(bool*)context = true;
        };
        if (fullInit) {
            backendTex = dContext->createBackendTexture(nativeExpected, kTopLeft_GrSurfaceOrigin,
                                                        GrRenderable::kNo, isProtected,
                                                        markFinished, &finishedBECreate);
        } else {
            backendTex = dContext->createBackendTexture(kSize,
                                                        kSize,
                                                        test.fColorType,
                                                        SkColors::kWhite,
                                                        Mipmapped::kNo,
                                                        GrRenderable::kNo,
                                                        isProtected,
                                                        markFinished,
                                                        &finishedBECreate);
        }
        REPORTER_ASSERT(reporter, backendTex.isValid());
        dContext->submit();
        while (backendTex.isValid() && !finishedBECreate) {
            dContext->checkAsyncWorkCompletion();
        }

        auto img = SkImages::BorrowTextureFrom(dContext,
                                               backendTex,
                                               kTopLeft_GrSurfaceOrigin,
                                               test.fColorType,
                                               test.fAlphaType,
                                               nullptr);
        REPORTER_ASSERT(reporter, SkToBool(img));

        {
            SkAutoPixmapStorage nativeActual;
            nativeActual.alloc(nativeII);
            nativeActual.erase(SkColors::kTransparent);

            if (img->readPixels(dContext, nativeActual, 0, 0)) {
                compare_pixmaps(reporter, nativeExpected, nativeActual,
                                test.fColorType, "SkImage::readPixels to native CT");
            }

            // SkSurface::readPixels with the same colorType as the source pixels round trips
            // (when allowed)
            if (dContext->colorTypeSupportedAsSurface(test.fColorType)) {
                auto s = SkSurfaces::RenderTarget(dContext, Budgeted::kNo, nativeII);
                REPORTER_ASSERT(reporter, SkToBool(s));

                {
                    SkCanvas* c = s->getCanvas();
                    c->drawImage(img, 0, 0);
                }

                nativeActual.erase(SkColors::kTransparent);
                REPORTER_ASSERT(reporter, s->readPixels(nativeActual, 0, 0));

                compare_pixmaps(reporter, nativeExpected, nativeActual,
                                test.fColorType, "SkSurface::readPixels to native CT");
            }
        }

        {
            SkAutoPixmapStorage f32Expected;
            f32Expected.alloc(f32Unpremul);
            f32Expected.erase(get_opaque_white_expected_color(test.fChannels));

            // read back to F32 if possible
            {
                SkAutoPixmapStorage f32Actual;
                f32Actual.alloc(f32Unpremul);
                f32Actual.erase(SkColors::kTransparent);
                if (img->readPixels(dContext, f32Actual, 0, 0)) {
                    compare_pixmaps(reporter, f32Expected, f32Actual,
                                    test.fColorType, "SkImage::readPixels to F32");
                }
            }

            // drawing a native SkImage works appropriately (as assessed by reading back from an
            // RGBA8 surface to an F32 pixmap)
            {
                const SkImageInfo rgba8888Premul = SkImageInfo::Make(kSize, kSize,
                                                                     VX_COLOR_TYPE_RGBA_8888,
                                                                     VX_ALPHA_TYPE_PREMULTIPLIED);

                auto s = SkSurfaces::RenderTarget(dContext, Budgeted::kNo, rgba8888Premul);
                REPORTER_ASSERT(reporter, SkToBool(s));

                {
                    SkCanvas* c = s->getCanvas();
                    c->drawImage(img, 0, 0);
                }

                SkAutoPixmapStorage f32Actual;
                f32Actual.alloc(f32Unpremul);
                f32Actual.erase(SkColors::kTransparent);
                REPORTER_ASSERT(reporter, s->readPixels(f32Actual, 0, 0));

                compare_pixmaps(reporter, f32Expected, f32Actual,
                                test.fColorType, "SkSurface::drawn to RGBA8888");
            }
        }

        img.reset();
        dContext->flushAndSubmit();
        dContext->deleteBackendTexture(backendTex);
    }
}

DEF_GANESH_TEST_FOR_RENDERING_CONTEXTS(ExtendedSkColorTypeTests_gpu,
                                       reporter,
                                       ctxInfo,
                                       CtsEnforcement::kApiLevel_T) {
    auto context = ctxInfo.directContext();

    for (size_t i = 0; i < std::size(gTests); ++i) {
        ganesh_tests(context, reporter, gTests[i]);
    }
}
#endif

DEF_TEST(ExtendedSkColorTypeTests_raster, reporter) {
    for (size_t i = 0; i < std::size(gTests); ++i) {
        raster_tests(reporter, gTests[i]);
    }}
