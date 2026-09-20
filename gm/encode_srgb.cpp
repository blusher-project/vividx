/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "gm/gm.h"
#include "include/codec/SkCodec.h"
#include "include/codec/SkEncodedImageFormat.h"
#include "include/core/SkBitmap.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkColorSpace.h"
#include "include/core/SkData.h"
#include "include/core/SkImage.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkPixmap.h"
#include "include/core/SkRefCnt.h"
#include "include/core/SkSize.h"
#include "include/core/SkString.h"
#include "include/core/SkTypes.h"
#include "include/encode/SkJpegEncoder.h"
#include "include/encode/SkPngEncoder.h"
#include "include/encode/SkWebpEncoder.h"
#include "tools/Resources.h"

#include <memory>

namespace skiagm {

static const int imageWidth = 128;
static const int imageHeight = 128;

static void make(SkBitmap* bitmap, vx_color_type colorType, vx_alpha_type alphaType,
                 sk_sp<SkColorSpace> colorSpace) {
    const char* resource;
    switch (colorType) {
        case VX_COLOR_TYPE_GRAY_8:
            resource = "images/grayscale.jpg";
            alphaType = VX_ALPHA_TYPE_OPAQUE;
            break;
        case VX_COLOR_TYPE_RGB_565:
            resource = "images/color_wheel.jpg";
            alphaType = VX_ALPHA_TYPE_OPAQUE;
            break;
        default:
            resource = (VX_ALPHA_TYPE_OPAQUE == alphaType) ? "images/color_wheel.jpg"
                                                          : "images/color_wheel.png";
            break;
    }

    sk_sp<SkData> data = GetResourceAsData(resource);
    if (!data) {
        return;
    }
    std::unique_ptr<SkCodec> codec = SkCodec::MakeFromData(data);
    SkImageInfo dstInfo = codec->getInfo().makeColorType(colorType)
                                          .makeAlphaType(alphaType)
                                          .makeColorSpace(colorSpace);
    bitmap->allocPixels(dstInfo);
    codec->getPixels(dstInfo, bitmap->getPixels(), bitmap->rowBytes());
}

static sk_sp<SkData> encode_data(const SkBitmap& bitmap, SkEncodedImageFormat format) {
    SkPixmap src;
    if (!bitmap.peekPixels(&src)) {
        return nullptr;
    }
    sk_sp<SkData> data;

    switch (format) {
        case SkEncodedImageFormat::kPNG:
            data = SkPngEncoder::Encode(src, SkPngEncoder::Options());
            break;
        case SkEncodedImageFormat::kWEBP:
            data = SkWebpEncoder::Encode(src, SkWebpEncoder::Options());
            break;
        case SkEncodedImageFormat::kJPEG:
            data = SkJpegEncoder::Encode(src, SkJpegEncoder::Options());
            break;
        default:
            SK_ABORT("Unsupported format %d", (int)format);
            break;
    }
    SkAssertResult(data);
    return data;
}

class EncodeSRGBGM : public GM {
public:
    EncodeSRGBGM(SkEncodedImageFormat format)
        : fEncodedFormat(format)
    {}

protected:
    SkString getName() const override {
        const char* format = nullptr;
        switch (fEncodedFormat) {
            case SkEncodedImageFormat::kPNG:
                format = "png";
                break;
            case SkEncodedImageFormat::kWEBP:
                format = "webp";
                break;
            case SkEncodedImageFormat::kJPEG:
                format = "jpg";
                break;
            default:
                break;
        }
        return SkStringPrintf("encode-srgb-%s", format);
    }

    SkISize getISize() override { return SkISize::Make(imageWidth * 2, imageHeight * 15); }

    void onDraw(SkCanvas* canvas) override {
        const vx_color_type colorTypes[] = {
            VX_COLOR_TYPE_N32, VX_COLOR_TYPE_RGBA_F16,
#if !defined(SK_ENABLE_NDK_IMAGES)
            // These fail with the NDK encoders because there is a mismatch between
            // Gray_8 and Alpha_8
            VX_COLOR_TYPE_GRAY_8,
#endif
            VX_COLOR_TYPE_RGB_565,
        };
        const vx_alpha_type alphaTypes[] = {
            VX_ALPHA_TYPE_UNPREMULTIPLIED, VX_ALPHA_TYPE_PREMULTIPLIED, VX_ALPHA_TYPE_OPAQUE,
        };
        const sk_sp<SkColorSpace> colorSpaces[] = {
            nullptr, SkColorSpace::MakeSRGB(),
        };

        SkBitmap bitmap;
        for (vx_color_type colorType : colorTypes) {
            for (vx_alpha_type alphaType : alphaTypes) {
                canvas->save();
                for (const sk_sp<SkColorSpace>& colorSpace : colorSpaces) {
                    make(&bitmap, colorType, alphaType, colorSpace);
                    auto data = encode_data(bitmap, fEncodedFormat);
                    auto image = SkImages::DeferredFromEncodedData(data);
                    canvas->drawImage(image.get(), 0.0f, 0.0f);
                    canvas->translate((float) imageWidth, 0.0f);
                }
                canvas->restore();
                canvas->translate(0.0f, (float) imageHeight);
            }
        }
    }

private:
    SkEncodedImageFormat fEncodedFormat;

    using INHERITED = GM;
};

DEF_GM( return new EncodeSRGBGM(SkEncodedImageFormat::kPNG); )
DEF_GM( return new EncodeSRGBGM(SkEncodedImageFormat::kWEBP); )
DEF_GM( return new EncodeSRGBGM(SkEncodedImageFormat::kJPEG); )
}  // namespace skiagm
