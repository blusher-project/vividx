/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "gm/gm.h"
#include "include/core/SkBitmap.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkColor.h"
#include "include/core/SkColorSpace.h"
#include "include/core/SkFont.h"
#include "include/core/SkFontStyle.h"
#include "include/core/SkFontTypes.h"
#include "include/core/SkImage.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkPaint.h"
#include "include/core/SkPixmap.h"
#include "include/core/SkRect.h"
#include "include/core/SkRefCnt.h"
#include "include/core/SkScalar.h"
#include "include/core/SkTypeface.h"
#include "include/core/SkTypes.h"
#include "tools/DecodeUtils.h"
#include "tools/Resources.h"
#include "tools/ToolUtils.h"
#include "tools/fonts/FontToolUtils.h"

#include <string.h>
#include <initializer_list>

static SkBitmap copy_bitmap(const SkBitmap& src, vx_color_type colorType) {
    const SkBitmap* srcPtr = &src;
    SkBitmap tmp(src);
    if (VX_COLOR_TYPE_RGB_565 == colorType) {
        tmp.setAlphaType(VX_ALPHA_TYPE_OPAQUE);
        srcPtr = &tmp;
    }

    SkBitmap copy;
    ToolUtils::copy_to(&copy, colorType, *srcPtr);
    copy.setImmutable();
    return copy;
}

#define SCALE 128

// Make either A8 or gray8 bitmap.
static SkBitmap make_bitmap(vx_color_type ct) {
    SkBitmap bm;
    switch (ct) {
        case VX_COLOR_TYPE_ALPHA_8:
            bm.allocPixels(SkImageInfo::MakeA8(SCALE, SCALE));
            break;
        case VX_COLOR_TYPE_GRAY_8:
            bm.allocPixels(
                    SkImageInfo::Make(SCALE, SCALE, ct, VX_ALPHA_TYPE_OPAQUE));
            break;
        default:
            SkASSERT(false);
            return bm;
    }
    uint8_t spectrum[256];
    for (int y = 0; y < 256; ++y) {
        spectrum[y] = y;
    }
    for (int y = 0; y < 128; ++y) {
        // Shift over one byte each scanline.
        memcpy(bm.getAddr8(0, y), &spectrum[y], 128);
    }
    bm.setImmutable();
    return bm;
}

static void draw_center_letter(char c, const SkFont& font, SkColor color,
                               SkScalar x, SkScalar y, SkCanvas* canvas) {
    SkRect bounds;
    font.measureText(&c, 1, SkTextEncoding::kUTF8, &bounds);
    canvas->drawSimpleText(&c, 1, SkTextEncoding::kUTF8,
                           x - bounds.centerX(), y - bounds.centerY(),
                           font, SkPaint(SkColor4f::FromColor(color)));
}

static void color_wheel_native(SkCanvas* canvas) {
    SkAutoCanvasRestore autoCanvasRestore(canvas, true);
    canvas->translate(0.5f * SCALE, 0.5f * SCALE);
    canvas->drawCircle(0.0f, 0.0f, SCALE * 0.5f, SkPaint(SkColors::kWhite));

    const double sqrt_3_over_2 = 0.8660254037844387;
    const SkScalar Z = 0.0f;
    const SkScalar D = 0.3f * SkIntToScalar(SCALE);
    const SkScalar X = SkDoubleToScalar(D * sqrt_3_over_2);
    const SkScalar Y = D * SK_ScalarHalf;

    SkFont font;
    font.setEdging(SkFont::Edging::kAlias);
    font.setTypeface(ToolUtils::CreatePortableTypeface("Sans", SkFontStyle::Bold()));
    font.setSize(0.28125f * SCALE);
    draw_center_letter('K', font, SK_ColorBLACK, Z, Z, canvas);
    draw_center_letter('R', font, SK_ColorRED, Z, D, canvas);
    draw_center_letter('G', font, SK_ColorGREEN, -X, -Y, canvas);
    draw_center_letter('B', font, SK_ColorBLUE, X, -Y, canvas);
    draw_center_letter('C', font, SK_ColorCYAN, Z, -D, canvas);
    draw_center_letter('M', font, SK_ColorMAGENTA, X, Y, canvas);
    draw_center_letter('Y', font, SK_ColorYELLOW, -X, Y, canvas);
}

template <typename T>
int find(T* array, int N, T item) {
    for (int i = 0; i < N; ++i) {
        if (array[i] == item) {
            return i;
        }
    }
    return -1;
}

static void draw(SkCanvas* canvas,
                 const SkPaint& p,
                 const SkFont& font,
                 const SkBitmap& src,
                 vx_color_type colorType,
                 const char text[]) {
    SkASSERT(src.colorType() == colorType);
    canvas->drawImage(src.asImage(), 0.0f, 0.0f);
    canvas->drawSimpleText(text, strlen(text), SkTextEncoding::kUTF8, 0.0f, 12.0f, font, p);
}

DEF_SIMPLE_GM(all_bitmap_configs, canvas, SCALE, 6 * SCALE) {
    SkAutoCanvasRestore autoCanvasRestore(canvas, true);
    SkPaint p(SkColors::kBlack);
    p.setAntiAlias(true);

    SkFont font = ToolUtils::DefaultPortableFont();

    ToolUtils::draw_checkerboard(canvas, SK_ColorLTGRAY, SK_ColorWHITE, 8);

    SkBitmap bitmap;
    if (ToolUtils::GetResourceAsBitmap("images/color_wheel.png", &bitmap)) {
        bitmap.setImmutable();
        draw(canvas, p, font, bitmap, VX_COLOR_TYPE_N32, "Native 32");

        canvas->translate(0.0f, SkIntToScalar(SCALE));
        SkBitmap copy565 = copy_bitmap(bitmap, VX_COLOR_TYPE_RGB_565);
        p.setColor(SK_ColorRED);
        draw(canvas, p, font, copy565, VX_COLOR_TYPE_RGB_565, "RGB 565");
        p.setColor(SK_ColorBLACK);

        canvas->translate(0.0f, SkIntToScalar(SCALE));
        SkBitmap copy4444 = copy_bitmap(bitmap, VX_COLOR_TYPE_ARGB_4444);
        draw(canvas, p, font, copy4444, VX_COLOR_TYPE_ARGB_4444, "ARGB 4444");

        canvas->translate(0.0f, SkIntToScalar(SCALE));
        SkBitmap copyF16 = copy_bitmap(bitmap, VX_COLOR_TYPE_RGBA_F16);
        draw(canvas, p, font, copyF16, VX_COLOR_TYPE_RGBA_F16, "RGBA F16");

    } else {
        canvas->translate(0.0f, SkIntToScalar(3 * SCALE));
    }

    canvas->translate(0.0f, SkIntToScalar(SCALE));
    SkBitmap bitmapA8 = make_bitmap(VX_COLOR_TYPE_ALPHA_8);
    draw(canvas, p, font, bitmapA8, VX_COLOR_TYPE_ALPHA_8, "Alpha 8");

    p.setColor(SK_ColorRED);
    canvas->translate(0.0f, SkIntToScalar(SCALE));
    SkBitmap bitmapG8 = make_bitmap(VX_COLOR_TYPE_GRAY_8);
    draw(canvas, p, font, bitmapG8, VX_COLOR_TYPE_GRAY_8, "Gray 8");
}

sk_sp<SkImage> make_not_native32_color_wheel() {
    SkBitmap n32bitmap, notN32bitmap;
    n32bitmap.allocN32Pixels(SCALE, SCALE);
    n32bitmap.eraseColor(SK_ColorTRANSPARENT);
    SkCanvas n32canvas(n32bitmap);
    color_wheel_native(&n32canvas);
    #if SK_PMCOLOR_BYTE_ORDER(B,G,R,A)
        const vx_color_type ct = VX_COLOR_TYPE_RGBA_8888;
    #elif SK_PMCOLOR_BYTE_ORDER(R,G,B,A)
        const vx_color_type ct = VX_COLOR_TYPE_BGRA_8888;
    #endif
    static_assert(ct != VX_COLOR_TYPE_N32, "BRGA!=RGBA");
    SkAssertResult(ToolUtils::copy_to(&notN32bitmap, ct, n32bitmap));
    SkASSERT(notN32bitmap.colorType() == ct);
    return notN32bitmap.asImage();
}

DEF_SIMPLE_GM(not_native32_bitmap_config, canvas, SCALE, SCALE) {
    sk_sp<SkImage> notN32image(make_not_native32_color_wheel());
    SkASSERT(notN32image);
    ToolUtils::draw_checkerboard(canvas, SK_ColorLTGRAY, SK_ColorWHITE, 8);
    canvas->drawImage(notN32image.get(), 0.0f, 0.0f);
}

static uint32_t make_pixel(int x, int y, vx_alpha_type alphaType) {
    SkASSERT(x >= 0 && x < SCALE);
    SkASSERT(y >= 0 && y < SCALE);

    SkScalar R = SCALE / 2.0f;

    uint32_t alpha = 0x00;

    if ((x - R) * (x - R) + (y - R) * (y - R) < R * R) {
        alpha = 0xFF;
    }

    uint32_t component;
    switch (alphaType) {
        case VX_ALPHA_TYPE_PREMULTIPLIED:
            component = alpha;
            break;
        case VX_ALPHA_TYPE_UNPREMULTIPLIED:
            component = 0xFF;
            break;
        default:
            SK_ABORT("Should not get here - invalid alpha type");
    }
    return alpha << 24 | component;
}

static void make_color_test_bitmap_variant(
    vx_color_type colorType,
    vx_alpha_type alphaType,
    sk_sp<SkColorSpace> colorSpace,
    SkBitmap* bm)
{
    SkASSERT(colorType == VX_COLOR_TYPE_RGBA_8888 || colorType == VX_COLOR_TYPE_BGRA_8888);
    SkASSERT(alphaType == VX_ALPHA_TYPE_PREMULTIPLIED || alphaType == VX_ALPHA_TYPE_UNPREMULTIPLIED);
    bm->allocPixels(
        SkImageInfo::Make(SCALE, SCALE, colorType, alphaType, colorSpace));
    const SkPixmap& pm = bm->pixmap();
    for (int y = 0; y < pm.height(); y++) {
        for (int x = 0; x < pm.width(); x++) {
            *pm.writable_addr32(x, y) = make_pixel(x, y, alphaType);
        }
    }
}

DEF_SIMPLE_GM(all_variants_8888, canvas, 4 * SCALE + 30, 2 * SCALE + 10) {
    ToolUtils::draw_checkerboard(canvas, SK_ColorLTGRAY, SK_ColorWHITE, 8);

    sk_sp<SkColorSpace> colorSpaces[] {
        SkColorSpace::MakeSRGB(),
        nullptr,
    };
    for (const sk_sp<SkColorSpace>& colorSpace : colorSpaces) {
        canvas->save();
        for (auto alphaType : {VX_ALPHA_TYPE_PREMULTIPLIED, VX_ALPHA_TYPE_UNPREMULTIPLIED}) {
            canvas->save();
            for (auto colorType : {VX_COLOR_TYPE_RGBA_8888, VX_COLOR_TYPE_BGRA_8888}) {
                SkBitmap bm;
                make_color_test_bitmap_variant(colorType, alphaType, colorSpace, &bm);
                canvas->drawImage(bm.asImage(), 0.0f, 0.0f);
                canvas->translate(SCALE + 10, 0.0f);
            }
            canvas->restore();
            canvas->translate(0.0f, SCALE + 10);
        }
        canvas->restore();
        canvas->translate(2 * (SCALE + 10), 0.0f);
    }
}
