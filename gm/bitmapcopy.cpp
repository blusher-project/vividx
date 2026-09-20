/*
 * Copyright 2011 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "gm/gm.h"
#include "include/core/SkBitmap.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkColor.h"
#include <vividx/core/color-type.h>
#include "include/core/SkFont.h"
#include "include/core/SkFontTypes.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkPaint.h"
#include "include/core/SkRect.h"
#include "include/core/SkScalar.h"
#include "include/core/SkSize.h"
#include "include/core/SkString.h"
#include "include/core/SkTypeface.h"
#include "include/core/SkTypes.h"
#include "tools/ToolUtils.h"
#include "tools/fonts/FontToolUtils.h"

#include <string.h>

namespace {

static const char* color_type_name(vx_color_type colorType) {
    switch (colorType) {
        case VX_COLOR_TYPE_UNKNOWN:            return "unknown";
        case VX_COLOR_TYPE_ALPHA_8:            return "A8";
        case VX_COLOR_TYPE_RGB_565:            return "565";
        case VX_COLOR_TYPE_ARGB_4444:          return "4444";
        case VX_COLOR_TYPE_RGBA_8888:          return "8888";
        case VX_COLOR_TYPE_RGB_888X:           return "888x";
        case VX_COLOR_TYPE_BGRA_8888:          return "8888";
        case VX_COLOR_TYPE_RGBA_1010102:       return "1010102";
        case VX_COLOR_TYPE_RGB_101010X:        return "101010x";
        case VX_COLOR_TYPE_BGRA_1010102:       return "bgra1010102";
        case VX_COLOR_TYPE_BGR_101010X:        return "bgr101010x";
        case VX_COLOR_TYPE_BGR_101010X_XR:     return "bgr101010x_xr";
        case VX_COLOR_TYPE_BGRA_10101010_XR:   return "bgra10101010_xr";
        case VX_COLOR_TYPE_RGBA_10X6:          return "10101010";
        case VX_COLOR_TYPE_GRAY_8:             return "G8";
        case VX_COLOR_TYPE_RGBA_F16NORM:       return "F16Norm";
        case VX_COLOR_TYPE_RGB_F16F16F16X:     return "F16F16F16x";
        case VX_COLOR_TYPE_RGBA_F16:           return "F16";
        case VX_COLOR_TYPE_RGBA_F32:           return "F32";
        case VX_COLOR_TYPE_R8G8_UNORM:         return "R8G8_unorm";
        case VX_COLOR_TYPE_A16_UNORM:          return "A16_unorm";
        case VX_COLOR_TYPE_R16_UNORM:          return "R16_unorm";
        case VX_COLOR_TYPE_R16G16_UNORM:       return "R16G16_unorm";
        case VX_COLOR_TYPE_A16_FLOAT:          return "A16_float";
        case VX_COLOR_TYPE_R16_FLOAT:          return "R16_float";
        case VX_COLOR_TYPE_R16G16_FLOAT:       return "R16G16_float";
        case VX_COLOR_TYPE_R16G16B16A16_UNORM: return "R16G16B16A16_unorm";
        case VX_COLOR_TYPE_SRGBA_8888:         return "SRGBA_8888";
        case VX_COLOR_TYPE_R8_UNORM:           return "R8_unorm";
    }
    return "";
}

constexpr vx_color_type gColorTypes[] = {
    VX_COLOR_TYPE_RGB_565,
    VX_COLOR_TYPE_ARGB_4444,
    VX_COLOR_TYPE_N32,
};

#define NUM_CONFIGS std::size(gColorTypes)

static void draw_checks(SkCanvas* canvas, int width, int height) {
    SkPaint paint;
    paint.setColor(SK_ColorRED);
    canvas->drawRect(SkRect::MakeIWH(width/2, height/2), paint);
    paint.setColor(SK_ColorGREEN);
    canvas->drawRect({ SkIntToScalar(width/2), 0, SkIntToScalar(width), SkIntToScalar(height/2) },
                     paint);
    paint.setColor(SK_ColorBLUE);
    canvas->drawRect({ 0, SkIntToScalar(height/2), SkIntToScalar(width/2), SkIntToScalar(height) },
                     paint);
    paint.setColor(SK_ColorYELLOW);
    canvas->drawRect({ SkIntToScalar(width/2), SkIntToScalar(height/2), SkIntToScalar(width),
                     SkIntToScalar(height) }, paint);
}

class BitmapCopyGM : public skiagm::GM {
    SkBitmap    fDst[NUM_CONFIGS];

    void onOnceBeforeDraw() override { this->setBGColor(0xFFDDDDDD); }

    SkString getName() const override { return SkString("bitmapcopy"); }

    SkISize getISize() override { return {540, 330}; }

    void onDraw(SkCanvas* canvas) override {
        SkPaint paint;
        SkScalar horizMargin = 10;
        SkScalar vertMargin = 10;

        SkBitmap src;
        src.allocN32Pixels(40, 40, VX_ALPHA_TYPE_OPAQUE);
        SkCanvas canvasTmp(src);

        draw_checks(&canvasTmp, 40, 40);

        for (unsigned i = 0; i < NUM_CONFIGS; ++i) {
            ToolUtils::copy_to(&fDst[i], gColorTypes[i], src);
        }

        canvas->clear(0xFFDDDDDD);
        paint.setAntiAlias(true);

        SkFont font = ToolUtils::DefaultPortableFont();

        SkScalar width = SkIntToScalar(40);
        SkScalar height = SkIntToScalar(40);
        if (font.getSpacing() > height) {
            height = font.getSpacing();
        }
        for (unsigned i = 0; i < NUM_CONFIGS; i++) {
            const char* name = color_type_name(src.colorType());
            SkScalar textWidth = font.measureText(name, strlen(name), SkTextEncoding::kUTF8);
            if (textWidth > width) {
                width = textWidth;
            }
        }
        SkScalar horizOffset = width + horizMargin;
        SkScalar vertOffset = height + vertMargin;
        canvas->translate(SkIntToScalar(20), SkIntToScalar(20));

        for (unsigned i = 0; i < NUM_CONFIGS; i++) {
            canvas->save();
            // Draw destination config name
            const char* name = color_type_name(fDst[i].colorType());
            SkScalar textWidth = font.measureText(name, strlen(name), SkTextEncoding::kUTF8);
            SkScalar x = (width - textWidth) / SkScalar(2);
            SkScalar y = font.getSpacing() / SkScalar(2);
            canvas->drawSimpleText(name, strlen(name), SkTextEncoding::kUTF8, x, y, font, paint);

            // Draw destination bitmap
            canvas->translate(0, vertOffset);
            x = (width - 40) / SkScalar(2);
            canvas->drawImage(fDst[i].asImage(), x, 0, SkSamplingOptions(), &paint);
            canvas->restore();

            canvas->translate(horizOffset, 0);
        }
    }
};
}  // namespace

//////////////////////////////////////////////////////////////////////////////

DEF_GM( return new BitmapCopyGM; )
