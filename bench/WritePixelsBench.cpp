/*
 * Copyright 2013 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "bench/Benchmark.h"
#include "include/core/SkBitmap.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkColorSpace.h"
#include "include/core/SkString.h"

// Time variants of write-pixels
//  [ colortype ][ alphatype ][ colorspace ]
//  Different combinations can trigger fast or slow paths in the impls
//
class WritePixelsBench : public Benchmark {
public:
    WritePixelsBench(vx_color_type ct, vx_alpha_type at, sk_sp<SkColorSpace> cs)
        : fColorType(ct)
        , fAlphaType(at)
        , fCS(cs)
    {
        fName.printf("writepix_%s_%s_%s",
                     at == VX_ALPHA_TYPE_PREMULTIPLIED ? "pm" : "um",
                     ct == VX_COLOR_TYPE_RGBA_8888 ? "rgba" : "bgra",
                     cs ? "srgb" : "null");
    }

protected:
    const char* onGetName() override {
        return fName.c_str();
    }

    void onDraw(int loops, SkCanvas* canvas) override {
        SkISize size = canvas->getBaseLayerSize();

        SkImageInfo info = SkImageInfo::Make(size, fColorType, fAlphaType, fCS);
        SkBitmap bmp;
        bmp.allocPixels(info);
        bmp.eraseColor(SK_ColorBLACK);

        for (int loop = 0; loop < loops; ++loop) {
            canvas->writePixels(info, bmp.getPixels(), bmp.rowBytes(), 0, 0);
        }
    }

private:
    vx_color_type fColorType;
    vx_alpha_type fAlphaType;
    sk_sp<SkColorSpace> fCS;
    SkString    fName;

    using INHERITED = Benchmark;
};

//////////////////////////////////////////////////////////////////////////////

DEF_BENCH(return new WritePixelsBench(VX_COLOR_TYPE_RGBA_8888, VX_ALPHA_TYPE_PREMULTIPLIED, nullptr);)
DEF_BENCH(return new WritePixelsBench(VX_COLOR_TYPE_RGBA_8888, VX_ALPHA_TYPE_UNPREMULTIPLIED, nullptr);)
DEF_BENCH(return new WritePixelsBench(VX_COLOR_TYPE_RGBA_8888, VX_ALPHA_TYPE_PREMULTIPLIED, SkColorSpace::MakeSRGB());)
DEF_BENCH(return new WritePixelsBench(VX_COLOR_TYPE_RGBA_8888, VX_ALPHA_TYPE_UNPREMULTIPLIED, SkColorSpace::MakeSRGB());)

DEF_BENCH(return new WritePixelsBench(VX_COLOR_TYPE_BGRA_8888, VX_ALPHA_TYPE_PREMULTIPLIED, nullptr);)
DEF_BENCH(return new WritePixelsBench(VX_COLOR_TYPE_BGRA_8888, VX_ALPHA_TYPE_UNPREMULTIPLIED, nullptr);)
DEF_BENCH(return new WritePixelsBench(VX_COLOR_TYPE_BGRA_8888, VX_ALPHA_TYPE_PREMULTIPLIED, SkColorSpace::MakeSRGB());)
DEF_BENCH(return new WritePixelsBench(VX_COLOR_TYPE_BGRA_8888, VX_ALPHA_TYPE_UNPREMULTIPLIED, SkColorSpace::MakeSRGB());)
