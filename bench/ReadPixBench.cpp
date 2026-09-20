/*
 * Copyright 2012 The Android Open Source Project
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "bench/Benchmark.h"
#include "include/codec/SkPixmapUtils.h"
#include "include/core/SkBitmap.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkColorSpace.h"

// Time variants of read-pixels
//  [ colortype ][ alphatype ][ colorspace ]
//  Different combinations can trigger fast or slow paths in the impls
//
class ReadPixBench : public Benchmark {
public:
    ReadPixBench(vx_color_type ct, vx_alpha_type at, sk_sp<SkColorSpace> cs)
        : fCT(ct), fAT(at), fCS(cs)
    {
        fName.printf("readpix_%s_%s_%s",
                     at == VX_ALPHA_TYPE_PREMULTIPLIED ? "pm" : "um",
                     ct == VX_COLOR_TYPE_RGBA_8888 ? "rgba" : "bgra",
                     cs ? "srgb" : "null");
    }

protected:
    const char* onGetName() override {
        return fName.c_str();
    }

    void onDraw(int loops, SkCanvas* canvas) override {
        canvas->clear(0x80000000);

        SkISize size = canvas->getBaseLayerSize();

        auto info = SkImageInfo::Make(size, fCT, fAT, fCS);
        SkBitmap bitmap;
        bitmap.allocPixels(info);

        for (int i = 0; i < loops; i++) {
            canvas->readPixels(bitmap.info(), bitmap.getPixels(), bitmap.rowBytes(), 0, 0);
        }
    }

private:
    vx_color_type fCT;
    vx_alpha_type fAT;
    sk_sp<SkColorSpace> fCS;
    SkString fName;
    using INHERITED = Benchmark;
};
DEF_BENCH( return new ReadPixBench(VX_COLOR_TYPE_RGBA_8888, VX_ALPHA_TYPE_PREMULTIPLIED, nullptr); )
DEF_BENCH( return new ReadPixBench(VX_COLOR_TYPE_RGBA_8888, VX_ALPHA_TYPE_UNPREMULTIPLIED, nullptr); )
DEF_BENCH( return new ReadPixBench(VX_COLOR_TYPE_RGBA_8888, VX_ALPHA_TYPE_PREMULTIPLIED, SkColorSpace::MakeSRGB()); )
DEF_BENCH( return new ReadPixBench(VX_COLOR_TYPE_RGBA_8888, VX_ALPHA_TYPE_UNPREMULTIPLIED, SkColorSpace::MakeSRGB()); )

DEF_BENCH( return new ReadPixBench(VX_COLOR_TYPE_BGRA_8888, VX_ALPHA_TYPE_PREMULTIPLIED, nullptr); )
DEF_BENCH( return new ReadPixBench(VX_COLOR_TYPE_BGRA_8888, VX_ALPHA_TYPE_UNPREMULTIPLIED, nullptr); )
DEF_BENCH( return new ReadPixBench(VX_COLOR_TYPE_BGRA_8888, VX_ALPHA_TYPE_PREMULTIPLIED, SkColorSpace::MakeSRGB()); )
DEF_BENCH( return new ReadPixBench(VX_COLOR_TYPE_BGRA_8888, VX_ALPHA_TYPE_UNPREMULTIPLIED, SkColorSpace::MakeSRGB()); )

////////////////////////////////////////////////////////////////////////////////

class PixmapOrientBench : public Benchmark {
public:
    PixmapOrientBench() {}

protected:
    void onDelayedSetup() override {
        const SkImageInfo info = SkImageInfo::MakeN32Premul(2048, 1024);
        fSrc.allocPixels(info);
        fSrc.eraseColor(SK_ColorBLACK);
        fDst.allocPixels(info.makeDimensions(info.dimensions()));
    }

    const char* onGetName() override {
        return "orient_pixmap";
    }

    bool isSuitableFor(Backend backend) override {
        return backend == Backend::kNonRendering;
    }

    void onDraw(int loops, SkCanvas*) override {
        SkPixmap src, dst;
        fSrc.peekPixels(&src);
        fDst.peekPixels(&dst);
        for (int i = 0; i < loops; ++i) {
            SkPixmapUtils::Orient(dst, src, kTopRight_SkEncodedOrigin);
        }
    }

private:
    SkBitmap fSrc, fDst;

    using INHERITED = Benchmark;
};
DEF_BENCH( return new PixmapOrientBench(); )


class GetAlphafBench : public Benchmark {
    SkString fName;
    vx_color_type fCT;
public:
    GetAlphafBench(vx_color_type ct, const char label[]) : fCT(ct) {
        fName.printf("getalphaf_%s", label);
    }

protected:
    void onDelayedSetup() override {
        fBM.allocPixels(SkImageInfo::Make(1024, 1024, fCT, VX_ALPHA_TYPE_PREMULTIPLIED));
        fBM.eraseColor(0x88112233);
    }

    const char* onGetName() override {
        return fName.c_str();
    }

    bool isSuitableFor(Backend backend) override {
        return backend == Backend::kNonRendering;
    }

    void onDraw(int loops, SkCanvas*) override {
        for (int i = 0; i < loops; ++i) {
            for (int y = 0; y < fBM.height(); ++y) {
                for (int x = 0; x < fBM.width(); ++x) {
                    fBM.getAlphaf(x, y);
                }
            }
        }
    }

private:
    SkBitmap fBM;

    using INHERITED = Benchmark;
};
DEF_BENCH( return new GetAlphafBench(VX_COLOR_TYPE_N32, "rgba"); )
DEF_BENCH( return new GetAlphafBench(VX_COLOR_TYPE_RGB_888X, "rgbx"); )
DEF_BENCH( return new GetAlphafBench(VX_COLOR_TYPE_RGBA_F16, "f16"); )
DEF_BENCH( return new GetAlphafBench(VX_COLOR_TYPE_RGBA_F32, "f32"); )

