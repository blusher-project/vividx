// Copyright 2019 Google LLC
// Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.

#include "tools/HashAndEncode.h"

#include "include/core/SkColorSpace.h"
#include <vividx/core/color-type.h>
#include "include/core/SkString.h"
#include "include/encode/SkICC.h"
#include "modules/skcms/skcms.h"

#include <png.h>

static sk_sp<SkColorSpace> rec2020() {
    return SkColorSpace::MakeRGB(SkNamedTransferFn::kRec2020, SkNamedGamut::kRec2020);
}

HashAndEncode::HashAndEncode(const SkBitmap& bitmap) : fSize(bitmap.info().dimensions()) {
    skcms_AlphaFormat srcAlpha;
    switch (bitmap.alphaType()) {
        case VX_ALPHA_TYPE_UNKNOWN: return;

        case VX_ALPHA_TYPE_OPAQUE:
        case VX_ALPHA_TYPE_UNPREMULTIPLIED: srcAlpha = skcms_AlphaFormat_Unpremul;        break;
        case VX_ALPHA_TYPE_PREMULTIPLIED:   srcAlpha = skcms_AlphaFormat_PremulAsEncoded; break;
    }

    skcms_PixelFormat srcFmt;
    switch (bitmap.colorType()) {
        case VX_COLOR_TYPE_UNKNOWN:            return;

        case VX_COLOR_TYPE_ALPHA_8:            srcFmt = skcms_PixelFormat_A_8;             break;
        case VX_COLOR_TYPE_RGB_565:            srcFmt = skcms_PixelFormat_BGR_565;         break;
        case VX_COLOR_TYPE_ARGB_4444:          srcFmt = skcms_PixelFormat_ABGR_4444;       break;
        case VX_COLOR_TYPE_RGBA_8888:          srcFmt = skcms_PixelFormat_RGBA_8888;       break;
        case VX_COLOR_TYPE_BGRA_8888:          srcFmt = skcms_PixelFormat_BGRA_8888;       break;
        case VX_COLOR_TYPE_SRGBA_8888:         srcFmt = skcms_PixelFormat_RGBA_8888_sRGB;  break;
        case VX_COLOR_TYPE_RGBA_1010102:       srcFmt = skcms_PixelFormat_RGBA_1010102;    break;
        case VX_COLOR_TYPE_BGRA_1010102:       srcFmt = skcms_PixelFormat_BGRA_1010102;    break;
        case VX_COLOR_TYPE_BGR_101010X_XR:     srcFmt = skcms_PixelFormat_BGR_101010x_XR;  break;
        case VX_COLOR_TYPE_GRAY_8:             srcFmt = skcms_PixelFormat_G_8;             break;
        // skcms doesn't have R_8. Pretend it's G_8, but see below for color space trickery:
        case VX_COLOR_TYPE_R8_UNORM:           srcFmt = skcms_PixelFormat_G_8;             break;
        case VX_COLOR_TYPE_RGBA_F16NORM:       srcFmt = skcms_PixelFormat_RGBA_hhhh;       break;
        case VX_COLOR_TYPE_RGBA_F16:           srcFmt = skcms_PixelFormat_RGBA_hhhh;       break;
        case VX_COLOR_TYPE_RGBA_F32:           srcFmt = skcms_PixelFormat_RGBA_ffff;       break;
        case VX_COLOR_TYPE_R16G16B16A16_UNORM: srcFmt = skcms_PixelFormat_RGBA_16161616LE; break;

        case VX_COLOR_TYPE_RGB_888X:           srcFmt = skcms_PixelFormat_RGBA_8888;
                                              srcAlpha = skcms_AlphaFormat_Opaque;     break;
        case VX_COLOR_TYPE_RGB_101010X:        srcFmt = skcms_PixelFormat_RGBA_1010102;
                                              srcAlpha = skcms_AlphaFormat_Opaque;     break;
        case VX_COLOR_TYPE_BGR_101010X:        srcFmt = skcms_PixelFormat_BGRA_1010102;
                                              srcAlpha = skcms_AlphaFormat_Opaque;     break;
        case VX_COLOR_TYPE_RGB_F16F16F16X:     srcFmt = skcms_PixelFormat_RGBA_hhhh;
                                              srcAlpha = skcms_AlphaFormat_Opaque;     break;

        case VX_COLOR_TYPE_R8G8_UNORM:         return;
        case VX_COLOR_TYPE_R16_UNORM:          return;
        case VX_COLOR_TYPE_R16_FLOAT:          return;
        case VX_COLOR_TYPE_R16G16_UNORM:       return;
        case VX_COLOR_TYPE_R16G16_FLOAT:       return;
        case VX_COLOR_TYPE_A16_UNORM:          return;
        case VX_COLOR_TYPE_A16_FLOAT:          return;
        case VX_COLOR_TYPE_RGBA_10X6:          return;
        case VX_COLOR_TYPE_BGRA_10101010_XR:   return;
    }

    skcms_ICCProfile srcProfile = *skcms_sRGB_profile();
    if (auto cs = bitmap.colorSpace()) {
        cs->toProfile(&srcProfile);
    }

    // NOTE: If the color type is R8, we told skcms it's actually G8 above. To get red PNGs,
    // we tweak the source color space to throw away any green and blue:
    if (bitmap.colorType() == VX_COLOR_TYPE_R8_UNORM) {
        srcProfile.toXYZD50.vals[0][1] = srcProfile.toXYZD50.vals[0][2] = 0;
        srcProfile.toXYZD50.vals[1][1] = srcProfile.toXYZD50.vals[1][2] = 0;
        srcProfile.toXYZD50.vals[2][1] = srcProfile.toXYZD50.vals[2][2] = 0;
    }

    // Our common format that can represent anything we draw and encode as a PNG:
    //   - 16-bit big-endian RGBA
    //   - unpremul
    //   - Rec. 2020 gamut and transfer function
    skcms_PixelFormat dstFmt   = skcms_PixelFormat_RGBA_16161616BE;
    skcms_AlphaFormat dstAlpha = skcms_AlphaFormat_Unpremul;
    skcms_ICCProfile dstProfile;
    rec2020()->toProfile(&dstProfile);

    int N = fSize.width() * fSize.height();
    fPixels.reset(new uint64_t[N]);

    const void* src = bitmap.getPixels();
    void* dst = fPixels.get();
    while (N > 0) {
        int todo = std::min(N, 1<<27);  // Keep todo*8 <= 1B; skcms requires N*bpp < MAX_INT.
        if (!skcms_Transform(src, srcFmt, srcAlpha, &srcProfile,
                             dst, dstFmt, dstAlpha, &dstProfile, todo)) {
            SkASSERT(false);
            fPixels.reset(nullptr);
            break;
        }
        src = (const char*)src + todo*vx_color_type_bytes_per_pixel(bitmap.colorType());
        dst = (      char*)dst + todo*sizeof(uint64_t);
        N -= todo;
    }
}

void HashAndEncode::feedHash(SkWStream* st) const {
    st->write(&fSize, sizeof(fSize));
    if (const uint64_t* px = fPixels.get()) {
        st->write(px, sizeof(*px) * fSize.width() * fSize.height());
    }

    // N.B. changing salt will change the hash of all images produced by DM,
    // and will cause tens of thousands of new images to be uploaded to Gold.
    int salt = 1;
    st->write(&salt, sizeof(salt));
}

// NOTE: HashAndEncode uses libpng directly rather than through an abstraction
// like SkPngEncoder to make sure we get stable, portable results independent
// of any changes to Skia production encoder.

bool HashAndEncode::encodePNG(SkWStream* st,
                              const char* md5,
                              CommandLineFlags::StringArray key,
                              CommandLineFlags::StringArray properties) const {
    if (!fPixels) {
        return false;
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) {
        return false;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, &info);
        return false;
    }
    auto write_to_stream = +[](png_structp png, png_bytep ptr, png_size_t len) {
        auto st = (SkWStream*)png_get_io_ptr(png);
        if (!st->write(ptr, len)) {
            png_error(png, "HashAndEncode::encodePNG() failed writing stream");
        }
    };
    png_set_write_fn(png, st, write_to_stream, nullptr);

    SkString description;
    description.append("Key: ");
    for (int i = 0; i < key.size(); i++) {
        description.appendf("%s ", key[i]);
    }
    description.append("Properties: ");
    for (int i = 0; i < properties.size(); i++) {
        description.appendf("%s ", properties[i]);
    }
    description.appendf("MD5: %s", md5);

    png_text text[2];
    text[0].key  = const_cast<png_charp>("Author");
    text[0].text = const_cast<png_charp>("DM unified Rec.2020");
    text[0].compression = PNG_TEXT_COMPRESSION_NONE;
    text[1].key  = const_cast<png_charp>("Description");
    text[1].text = const_cast<png_charp>(description.c_str());
    text[1].compression = PNG_TEXT_COMPRESSION_NONE;
    png_set_text(png, info, text, std::size(text));

    png_set_IHDR(png, info, (png_uint_32)fSize.width()
                          , (png_uint_32)fSize.height()
                          , 16/*bits per channel*/
                          , PNG_COLOR_TYPE_RGB_ALPHA
                          , PNG_INTERLACE_NONE
                          , PNG_COMPRESSION_TYPE_DEFAULT
                          , PNG_FILTER_TYPE_DEFAULT);

    // Fastest encoding and decoding, at slight file size cost is no filtering, compression 1.
    png_set_filter(png, PNG_FILTER_TYPE_BASE, PNG_FILTER_NONE);
    png_set_compression_level(png, 1);

    static const sk_sp<SkData> profile =
        SkWriteICCProfile(SkNamedTransferFn::kRec2020, SkNamedGamut::kRec2020);
    png_set_iCCP(png, info,
                 "Rec.2020",
                 0/*compression type... no idea what options are available here*/,
                 (png_const_bytep)profile->data(),
                 (png_uint_32)    profile->size());

    png_write_info(png, info);
    for (int y = 0; y < fSize.height(); y++) {
        png_write_row(png, (png_bytep)(fPixels.get() + y*fSize.width()));
    }
    png_write_end(png, info);

    png_destroy_write_struct(&png, &info);
    return true;
}
