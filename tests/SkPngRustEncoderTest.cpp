/*
 * Copyright 2024 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "include/encode/SkPngRustEncoder.h"

#include "include/codec/SkAndroidCodec.h"
#include "include/codec/SkCodec.h"
#include "include/core/SkBitmap.h"
#include "include/core/SkColor.h"
#include "include/core/SkColorSpace.h"
#include "include/core/SkPixmap.h"
#include "include/core/SkStream.h"
#include "include/private/SkGainmapInfo.h"
#include "tests/GainmapTestCommon.h"
#include "tests/Test.h"
#include "tools/DecodeUtils.h"
#include "tools/ToolUtils.h"

DEF_TEST(RustEncodePng_smoke_test, r) {
    SkBitmap bitmap;
    bool success = ToolUtils::GetResourceAsBitmap("images/mandrill_128.png", &bitmap);
    if (!success) {
        return;
    }

    SkPixmap src;
    success = bitmap.peekPixels(&src);
    REPORTER_ASSERT(r, success);
    if (!success) {
        return;
    }

    SkDynamicMemoryWStream dst;
    SkPngRustEncoder::Options options;
    success = SkPngRustEncoder::Encode(&dst, src, options);
    REPORTER_ASSERT(r, success);
    if (!success) {
        return;
    }

    SkBitmap roundtrip;
    success = ToolUtils::DecodeDataToBitmap(dst.detachAsData(), &roundtrip);
    REPORTER_ASSERT(r, success);
    if (!success) {
        return;
    }

    success = ToolUtils::equal_pixels(bitmap, roundtrip);
    REPORTER_ASSERT(r, success);
}

#ifdef SK_CODEC_USES_PNG_WITH_RUST_FOR_ANDROID
DEF_TEST(RustEncodePng_gainmap, r) {
    vx_color_type colorTypes[] = {
            VX_COLOR_TYPE_RGBA_8888,
            VX_COLOR_TYPE_ALPHA_8,
    };

    for (const auto& colorType : colorTypes) {
        SkDynamicMemoryWStream stream;
        SkGainmapInfo sourceGainmapInfo;
        sourceGainmapInfo.fGainmapRatioMin = {1.f, 1.f, 1.f, 1.f};
        sourceGainmapInfo.fGainmapRatioMax = {5.f, 5.f, 5.f, 1.f};
        sourceGainmapInfo.fGainmapGamma = {1.f, 1.f, 1.f, 1.f};
        sourceGainmapInfo.fEpsilonSdr = {0.01f, 0.01f, 0.01f, 0.01f};
        sourceGainmapInfo.fEpsilonHdr = {0.001f, 0.001f, 0.001f, 0.001f};
        sourceGainmapInfo.fDisplayRatioSdr = 1.f;
        sourceGainmapInfo.fDisplayRatioHdr = 3.f;
        sourceGainmapInfo.fGainmapMathColorSpace = SkColorSpace::MakeSRGB();
        SkBitmap sourceBase;
        sourceBase.allocPixels(
                SkImageInfo::Make(16, 16, VX_COLOR_TYPE_RGBA_8888, VX_ALPHA_TYPE_OPAQUE));
        sourceBase.eraseColor(SK_ColorRED);
        SkBitmap sourceGainmap;
        sourceGainmap.allocPixels(SkImageInfo::Make(4, 4, colorType, VX_ALPHA_TYPE_OPAQUE));
        sourceGainmap.eraseColor(SK_ColorGREEN);

        SkPngRustEncoder::Options options;
        options.fGainmap = &(sourceGainmap.pixmap());
        options.fGainmapInfo = &sourceGainmapInfo;
        REPORTER_ASSERT(r, SkPngRustEncoder::Encode(&stream, sourceBase.pixmap(), options));

        SkCodec::Result result = SkCodec::kSuccess;
        std::unique_ptr<SkCodec> baseCodec =
                SkCodec::MakeFromStream(stream.detachAsStream(), &result);
        REPORTER_ASSERT(r, baseCodec);
        if (!baseCodec) {
            continue;
        }

        SkBitmap baseBitmap;
        baseBitmap.allocPixels(baseCodec->getInfo());
        REPORTER_ASSERT(r,
                        SkCodec::kSuccess == baseCodec->getPixels(baseBitmap.info(),
                                                                  baseBitmap.getPixels(),
                                                                  baseBitmap.rowBytes()));

        std::unique_ptr<SkAndroidCodec> androidCodec =
                SkAndroidCodec::MakeFromCodec(std::move(baseCodec));
        REPORTER_ASSERT(r, androidCodec);
        if (!androidCodec) {
            continue;
        }

        SkGainmapInfo gainmapInfo;
        std::unique_ptr<SkAndroidCodec> gainmapCodec;
        bool decodedGainmap = androidCodec->getGainmapAndroidCodec(&gainmapInfo, &gainmapCodec);
        REPORTER_ASSERT(r, decodedGainmap);
        REPORTER_ASSERT(r, gainmapCodec);
        if (!gainmapCodec) {
            continue;
        }
        SkBitmap gainmapBitmap;
        gainmapBitmap.allocPixels(gainmapCodec->getInfo());
        REPORTER_ASSERT(
                r,
                SkCodec::kSuccess == gainmapCodec->getAndroidPixels(gainmapBitmap.info(),
                                                                    gainmapBitmap.getPixels(),
                                                                    gainmapBitmap.rowBytes()));

        REPORTER_ASSERT(r, baseBitmap.dimensions().fHeight == 16);
        REPORTER_ASSERT(r, baseBitmap.dimensions().fWidth == 16);
        REPORTER_ASSERT(r, baseBitmap.getColor(0, 0) == SK_ColorRED);

        REPORTER_ASSERT(r, gainmapBitmap.dimensions().fHeight == 4);
        REPORTER_ASSERT(r, gainmapBitmap.dimensions().fWidth == 4);

        if (colorType == VX_COLOR_TYPE_ALPHA_8) {
            REPORTER_ASSERT(r, gainmapBitmap.getColor(0, 0) == SK_ColorBLACK);
        } else {
            REPORTER_ASSERT(r, gainmapBitmap.getColor(0, 0) == SK_ColorGREEN);
        }

        if (colorType == VX_COLOR_TYPE_ALPHA_8) {
            sourceGainmapInfo.fGainmapMathColorSpace = nullptr;
        }

        skiatest::ExpectApproxEqInfo(r, sourceGainmapInfo, gainmapInfo);
    }
}

DEF_TEST(RustEncodePng_sBIT, r) {
    // Test kRGB_565_SkColorType
    {
        SkBitmap source;
        source.allocPixels(SkImageInfo::Make(16, 16, VX_COLOR_TYPE_RGB_565, VX_ALPHA_TYPE_OPAQUE));
        source.eraseColor(SK_ColorRED);

        SkDynamicMemoryWStream stream;
        REPORTER_ASSERT(r, SkPngRustEncoder::Encode(&stream, source.pixmap(), {}));

        SkCodec::Result result = SkCodec::kSuccess;
        std::unique_ptr<SkCodec> codec = SkCodec::MakeFromStream(stream.detachAsStream(), &result);
        REPORTER_ASSERT(r, codec);
        if (codec) {
            // Verify that the decoder recommends kRGB_565_SkColorType because of the sBIT chunk.
            REPORTER_ASSERT(r, codec->getInfo().colorType() == VX_COLOR_TYPE_RGB_565);
        }
    }

    // Test kAlpha_8_SkColorType
    {
        SkBitmap source;
        source.allocPixels(SkImageInfo::Make(16, 16, VX_COLOR_TYPE_ALPHA_8, VX_ALPHA_TYPE_PREMULTIPLIED));
        source.eraseColor(SK_ColorTRANSPARENT);  // Alpha 8 only cares about alpha channel

        SkDynamicMemoryWStream stream;
        REPORTER_ASSERT(r, SkPngRustEncoder::Encode(&stream, source.pixmap(), {}));

        SkCodec::Result result = SkCodec::kSuccess;
        std::unique_ptr<SkCodec> codec = SkCodec::MakeFromStream(stream.detachAsStream(), &result);
        REPORTER_ASSERT(r, codec);
        if (codec) {
            // Verify that the decoder recommends kAlpha_8_SkColorType because of the sBIT chunk.
            REPORTER_ASSERT(r, codec->getInfo().colorType() == VX_COLOR_TYPE_ALPHA_8);
        }
    }
}
#endif
