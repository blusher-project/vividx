/*
 * Copyright 2025 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "src/gpu/graphite/TextureFormat.h"

#include "include/core/SkColor.h"
#include <vividx/core/color-type.h>
#include "src/core/SkImageInfoPriv.h"

namespace skgpu::graphite {

using TF = TextureFormat; // for brevity in this file

const char* TextureFormatName(TextureFormat format) {
    switch (format) {
        case TF::kUnsupported:    return "Unsupported";
        case TF::kR8:             return "R8";
        case TF::kR16:            return "R16";
        case TF::kR16F:           return "R16F";
        case TF::kR32F:           return "R32F";
        case TF::kA8:             return "A8";
        case TF::kRG8:            return "RG8";
        case TF::kRG16:           return "RG16";
        case TF::kRG16F:          return "RG16F";
        case TF::kRG32F:          return "RG32F";
        case TF::kRGB8:           return "RGB8";
        case TF::kBGR8:           return "BGR8";
        case TF::kB5_G6_R5:       return "B5_G6_R5";
        case TF::kR5_G6_B5:       return "R5_G6_B5";
        case TF::kRGB16:          return "RGB16";
        case TF::kRGB16F:         return "RGB16F";
        case TF::kRGB32F:         return "RGB32F";
        case TF::kRGB8_sRGB:      return "RGB8_sRGB";
        case TF::kBGR10_XR:       return "BGR10_XR";
        case TF::kRGBA8:          return "RGBA8";
        case TF::kRGBA16:         return "RBGA16";
        case TF::kRGBA16F:        return "RGBA16F";
        case TF::kRGBA32F:        return "RGBA32F";
        case TF::kRGB10_A2:       return "RGB10_A2";
        case TF::kRGBA10x6:       return "RGBA10x6";
        case TF::kRGBA8_sRGB:     return "RGBA8_sRGB";
        case TF::kBGRA8:          return "BGRA8";
        case TF::kBGR10_A2:       return "BGR10_A2";
        case TF::kBGRA8_sRGB:     return "BGRA8_sRGB";
        case TF::kABGR4:          return "ABGR4";
        case TF::kARGB4:          return "ARGB4";
        case TF::kBGRA10x6_XR:    return "BGRA10x6_XR";
        case TF::kRGB8_ETC2:      return "RGB8_ETC2";
        case TF::kRGB8_ETC2_sRGB: return "RGB8_ETC2_sRGB";
        case TF::kRGB8_BC1:       return "RGB8_BC1";
        case TF::kRGBA8_BC1:      return "RGBA8_BC1";
        case TF::kRGBA8_BC1_sRGB: return "RGBA8_BC1_sRGB";
        case TF::kYUV8_P2_420:    return "YUV8_P2_420";
        case TF::kYUV8_P3_420:    return "YUV8_P3_420";
        case TF::kYUV10x6_P2_420: return "YUV10x6_P2_420";
        case TF::kYUV8_P2_422:    return "YUV8_P2_422";
        case TF::kYUV8_P3_422:    return "YUV8_P3_422";
        case TF::kYUV10x6_P2_422: return "YUV10x6_P2_422";
        case TF::kYUV8_P2_444:    return "YUV8_P2_444";
        case TF::kYUV8_P3_444:    return "YUV8_P3_444";
        case TF::kYUV10x6_P2_444: return "YUV10x6_P2_444";
        case TF::kExternal:       return "External";
        case TF::kS8:             return "S8";
        case TF::kD16:            return "D16";
        case TF::kD32F:           return "D32F";
        case TF::kD24_S8:         return "D24_S8";
        case TF::kD32F_S8:        return "D32F_S8";
    }
    SkUNREACHABLE;
}

SkTextureCompressionType TextureFormatCompressionType(TextureFormat format) {
    switch (format) {
        case TF::kRGB8_ETC2:      [[fallthrough]];
        case TF::kRGB8_ETC2_sRGB: return SkTextureCompressionType::kETC2_RGB8_UNORM;
        case TF::kRGB8_BC1:       return SkTextureCompressionType::kBC1_RGB8_UNORM;
        case TF::kRGBA8_BC1:      [[fallthrough]];
        case TF::kRGBA8_BC1_sRGB: return SkTextureCompressionType::kBC1_RGBA8_UNORM;
        default:                  return SkTextureCompressionType::kNone;
    }
}

TextureFormat CompressionTypeToTextureFormat(SkTextureCompressionType type) {
    switch (type) {
        case SkTextureCompressionType::kBC1_RGB8_UNORM:  return TF::kRGB8_BC1;
        case SkTextureCompressionType::kBC1_RGBA8_UNORM: return TF::kRGBA8_BC1;
        case SkTextureCompressionType::kETC2_RGB8_UNORM: return TF::kRGB8_ETC2;
        default:                                         return TF::kUnsupported;
    }
}

int TextureFormatBytesPerBlock(TextureFormat format) {
    switch (format) {
        case TF::kUnsupported: return 0;
        case TF::kR8:          return 1;
        case TF::kR16:         return 2;
        case TF::kR16F:        return 2;
        case TF::kR32F:        return 4;
        case TF::kA8:          return 1;
        case TF::kRG8:         return 2;
        case TF::kRG16:        return 4;
        case TF::kRG16F:       return 4;
        case TF::kRG32F:       return 8;
        case TF::kRGB8:        return 3;
        case TF::kBGR8:        return 3;
        case TF::kB5_G6_R5:    return 2;
        case TF::kR5_G6_B5:    return 2;
        case TF::kRGB16:       return 6;
        case TF::kRGB16F:      return 6;
        case TF::kRGB32F:      return 12;
        case TF::kRGB8_sRGB:   return 3;
        case TF::kBGR10_XR:    return 4;
        case TF::kRGBA8:       return 4;
        case TF::kRGBA16:      return 8;
        case TF::kRGBA16F:     return 8;
        case TF::kRGBA32F:     return 16;
        case TF::kRGB10_A2:    return 4;
        case TF::kRGBA10x6:    return 8;
        case TF::kRGBA8_sRGB:  return 4;
        case TF::kBGRA8:       return 4;
        case TF::kBGR10_A2:    return 4;
        case TF::kBGRA8_sRGB:  return 4;
        case TF::kABGR4:       return 2;
        case TF::kARGB4:       return 2;
        case TF::kBGRA10x6_XR: return 8;
        case TF::kS8:          return 1;
        case TF::kD16:         return 2;
        case TF::kD32F:        return 4;
        case TF::kD24_S8:      return 4;
        case TF::kD32F_S8:     return 5; // Assuming it's multiplanar

        // NOTE: For compressed formats, the block size refers to an actual compressed block of
        // multiple texels, whereas with other formats the block size represents a single pixel.
        case TF::kRGB8_ETC2:
        case TF::kRGB8_ETC2_sRGB:
        case TF::kRGB8_BC1:
        case TF::kRGBA8_BC1:
        case TF::kRGBA8_BC1_sRGB:
            return 8;
        // NOTE: We don't actually know the size of external formats, so this is an arbitrary value.
        // We will see external formats only in wrapped SkImages, so this won't impact Skia's
        // internal budgeting.
        case TF::kExternal:
            return 4;
        // TODO(b/401016699): We are just over estimating this value to be used in gpu size
        // calculations even though the actually size is probably less. We should instead treat
        // planar formats similar to compressed textures that go through their own special query for
        // calculating size.
        case TF::kYUV8_P2_420:
        case TF::kYUV8_P3_420:
        case TF::kYUV8_P2_422:
        case TF::kYUV8_P3_422:
        case TF::kYUV8_P2_444:
        case TF::kYUV8_P3_444:
            return 3;
        case TF::kYUV10x6_P2_420:
        case TF::kYUV10x6_P2_422:
        case TF::kYUV10x6_P2_444:
            return 6;
    }
    SkUNREACHABLE;
}

uint32_t TextureFormatChannelMask(TextureFormat format) {
    switch (format) {
        case TF::kA8:             return VX_COLOR_CHANNEL_FLAG_ALPHA;

        case TF::kR8:             [[fallthrough]];
        case TF::kR16:
        case TF::kR16F:
        case TF::kR32F:           return VX_COLOR_CHANNEL_FLAG_RED;

        case TF::kRG8:            [[fallthrough]];
        case TF::kRG16:
        case TF::kRG16F:
        case TF::kRG32F:          return VX_COLOR_CHANNEL_FLAGS_RG;

        case TF::kRGB8:           [[fallthrough]];
        case TF::kBGR8:
        case TF::kB5_G6_R5:
        case TF::kR5_G6_B5:
        case TF::kRGB16:
        case TF::kRGB16F:
        case TF::kRGB32F:
        case TF::kRGB8_sRGB:
        case TF::kBGR10_XR:
        case TF::kRGB8_ETC2:
        case TF::kRGB8_ETC2_sRGB:
        case TF::kRGB8_BC1:
        case TF::kYUV8_P2_420:
        case TF::kYUV8_P3_420:
        case TF::kYUV10x6_P2_420:
        case TF::kYUV8_P2_422:
        case TF::kYUV8_P3_422:
        case TF::kYUV10x6_P2_422:
        case TF::kYUV8_P2_444:
        case TF::kYUV8_P3_444:
        case TF::kYUV10x6_P2_444: return VX_COLOR_CHANNEL_FLAGS_RGB;

        case TF::kRGBA8:          [[fallthrough]];
        case TF::kRGBA16:
        case TF::kRGBA16F:
        case TF::kRGBA32F:
        case TF::kRGB10_A2:
        case TF::kRGBA10x6:
        case TF::kRGBA8_sRGB:
        case TF::kBGRA8:
        case TF::kBGR10_A2:
        case TF::kBGRA8_sRGB:
        case TF::kABGR4:
        case TF::kARGB4:
        case TF::kBGRA10x6_XR:
        case TF::kRGBA8_BC1:
        case TF::kRGBA8_BC1_sRGB:
        case TF::kExternal:       return VX_COLOR_CHANNEL_FLAGS_RGBA;

        case TF::kS8:             [[fallthrough]];
        case TF::kD16:
        case TF::kD32F:
        case TF::kD24_S8:
        case TF::kD32F_S8:
        case TF::kUnsupported:    return 0;
    }
    SkUNREACHABLE;
}

bool TextureFormatAutoClamps(TextureFormat format) {
    // Floating point formats, extended range formats, and non-normalized integer formats do not
    // auto-clamp. Everything behaves like an unsigned normalized number.
    return !(TextureFormatIsFloatingPoint(format) ||
             format == TF::kBGR10_XR ||
             format == TF::kBGRA10x6_XR ||
             format == TF::kS8);
}

bool TextureFormatIsFloatingPoint(TextureFormat format) {
    switch (format) {
        // Floating point formats
        case TF::kR16F:           [[fallthrough]];
        case TF::kR32F:
        case TF::kRG16F:
        case TF::kRG32F:
        case TF::kRGB16F:
        case TF::kRGB32F:
        case TF::kRGBA16F:
        case TF::kRGBA32F:
        case TF::kD32F:
        case TF::kD32F_S8:        return true;

        // Everything else is unorm, unorm-srgb, fixed point, or integral
        case TF::kUnsupported:    [[fallthrough]];
        case TF::kR8:
        case TF::kR16:
        case TF::kA8:
        case TF::kRG8:
        case TF::kRG16:
        case TF::kRGB8:
        case TF::kBGR8:
        case TF::kB5_G6_R5:
        case TF::kR5_G6_B5:
        case TF::kRGB16:
        case TF::kRGB8_sRGB:
        case TF::kBGR10_XR:
        case TF::kRGBA8:
        case TF::kRGBA16:
        case TF::kRGB10_A2:
        case TF::kRGBA10x6:
        case TF::kRGBA8_sRGB:
        case TF::kBGRA8:
        case TF::kBGR10_A2:
        case TF::kBGRA8_sRGB:
        case TF::kABGR4:
        case TF::kARGB4:
        case TF::kBGRA10x6_XR:
        case TF::kRGB8_ETC2:
        case TF::kRGB8_ETC2_sRGB:
        case TF::kRGB8_BC1:
        case TF::kRGBA8_BC1:
        case TF::kRGBA8_BC1_sRGB:
        case TF::kYUV8_P2_420:
        case TF::kYUV8_P3_420:
        case TF::kYUV10x6_P2_420:
        case TF::kYUV8_P2_422:
        case TF::kYUV8_P3_422:
        case TF::kYUV10x6_P2_422:
        case TF::kYUV8_P2_444:
        case TF::kYUV8_P3_444:
        case TF::kYUV10x6_P2_444:
        case TF::kExternal:
        case TF::kS8:
        case TF::kD16:
        case TF::kD24_S8:          return false;
    }
    SkUNREACHABLE;
}

bool TextureFormatIsDepthOrStencil(TextureFormat format) {
    switch (format) {
        case TF::kS8:      [[fallthrough]];
        case TF::kD16:
        case TF::kD32F:
        case TF::kD24_S8:
        case TF::kD32F_S8:
            return true;
        default:
            return false;
    }
}

bool TextureFormatHasDepth(TextureFormat format) {
    switch (format) {
        case TF::kD16:     [[fallthrough]];
        case TF::kD32F:
        case TF::kD24_S8:
        case TF::kD32F_S8:
            return true;
        default:
            return false;
    }
}

bool TextureFormatHasStencil(TextureFormat format) {
    switch (format) {
        case TF::kS8:      [[fallthrough]];
        case TF::kD24_S8:
        case TF::kD32F_S8:
            return true;
        default:
            return false;
    }
}

bool TextureFormatIsMultiplanar(TextureFormat format) {
    switch (format) {
        case TF::kYUV8_P2_420:    [[fallthrough]];
        case TF::kYUV8_P3_420:
        case TF::kYUV10x6_P2_420:
        case TF::kYUV8_P2_422:
        case TF::kYUV8_P3_422:
        case TF::kYUV10x6_P2_422:
        case TF::kYUV8_P2_444:
        case TF::kYUV8_P3_444:
        case TF::kYUV10x6_P2_444:
            return true;
        default:
            return false;
    }
}

// Supporting implementation details for TextureFormat and SkColorType conversions
// ------------------------------------------------------------------------------------------------

<<<<<<< HEAD
Swizzle ReadSwizzleForColorType(vx_color_type ct, TextureFormat format) {
    // TODO(b/390473370): When data transfers can apply an RG swizzle outside of the
    // SkColorType representation, we should instead apply the swizzle on upload and
    // preserve the expected order for any GPU use.
    if (ct == VX_COLOR_TYPE_ARGB_4444 && format == TextureFormat::kARGB4) {
        return Swizzle::BGRA();
    }

    uint32_t colorChannels = vx_color_type_channel_flags(ct);
=======
Swizzle ReadSwizzleForColorType(SkColorType ct, TextureFormat format) {
    uint32_t colorChannels = SkColorTypeChannelFlags(ct);
>>>>>>> skia/chrome/m155
    uint32_t formatChannels = TextureFormatChannelMask(format);

    // Read swizzles only have to handle a few semantics around the sampled values, as any sort of
    // channel ordering for RGB vs BGR is handled by hardware. All we have to handle is mapping to
    // "gray", red-vs-alpha, and forcing to opaque.
    if (SkColorTypeIsAlphaOnly(ct)) {
        // If the format isn't just an alpha channel (e.g. TF::kA8), we need to adjust
        if (formatChannels != VX_COLOR_CHANNEL_FLAG_ALPHA) {
            // If the format has an alpha channel, mask every other channel to 0
            if (formatChannels & VX_COLOR_CHANNEL_FLAG_ALPHA) {
                return Swizzle("000a");
            } else {
                // Otherwise move the red channel to alpha
                SkASSERT(formatChannels & VX_COLOR_CHANNEL_FLAG_RED);
                return Swizzle("000r");
            }
        } else {
            // otherwise leave as "rgba" and let hardware do the right thing
            return Swizzle::RGBA();
        }
    } else {
        // First map gray to rrra; if this is just gray and not gray+alpha, it will also be forced
        // to opaque below and become rrr1.
        Swizzle swizzle;
        if (colorChannels & VX_COLOR_CHANNEL_FLAG_GRAY) {
            SkASSERT(formatChannels & VX_COLOR_CHANNEL_FLAG_RED);
            swizzle = Swizzle::RRRA();
        } else {
            swizzle = Swizzle::RGBA();
        }

        // Last, force the alpha to opaque if the color type masks it off but is present in the
        // texture format.
        if (!(colorChannels & VX_COLOR_CHANNEL_FLAG_ALPHA) &&
             (formatChannels & VX_COLOR_CHANNEL_FLAG_ALPHA)) {
            swizzle = Swizzle::Concat(swizzle, Swizzle::RGB1());
        }

        return swizzle;
    }
}

std::optional<skgpu::Swizzle> WriteSwizzleForColorType(vx_color_type ct, TextureFormat format) {
    // D/S, compressed, external, and multiplanar formats aren't renderable with a color type.
    // Format support would mean we never really try to get here in practice, but keep consistent.
    if (format == TextureFormat::kExternal ||
        TextureFormatIsDepthOrStencil(format) ||
        TextureFormatIsMultiplanar(format) ||
        TextureFormatCompressionType(format) != SkTextureCompressionType::kNone) {
        return std::nullopt;
    }

    uint32_t colorChannels = vx_color_type_channel_flags(ct);
    uint32_t formatChannels = TextureFormatChannelMask(format);

    // Write swizzles only have to handle a few semantics around the sampled values, as any sort of
    // channel ordering for RGB vs BGR is handled by hardware. This reduces to just handling red
    // vs alpha. The other cases for read swizzles do not apply:
    //   - We disallow gray since computing luminance is beyond a swizzle.
    //   - We disallow forcing to opaque since in all cases where we'd do that we have no guarantee
    //     of what the dst pixel's alpha was. In the future, we could support forced-opaque
    //     rendering by always using shader-based blending or by guaranteeing a one-time initialize
    //     draw that forced any alpha channel to 1 (b/489785214).
    if (SkColorTypeIsAlphaOnly(ct)) {
        // If the format isn't just an alpha channel (e.g. TF::kA8), we need to adjust
        if (formatChannels != VX_COLOR_CHANNEL_FLAG_ALPHA) {
            // If the format has an alpha channel, mask every other channel to 0
            if (formatChannels & VX_COLOR_CHANNEL_FLAG_ALPHA) {
                return Swizzle("000a");
            } else {
                // Otherwise move the alpha channel to red
                SkASSERT(formatChannels & VX_COLOR_CHANNEL_FLAG_RED);
                return Swizzle("a000");
            }
        } else {
            // otherwise leave as "rgba" and let hardware do the right thing
            return Swizzle::RGBA();
        }
    } else {
        if (((colorChannels & formatChannels) != formatChannels) ||
            (colorChannels & VX_COLOR_CHANNEL_FLAG_GRAY)) {
            return std::nullopt;
        }
        return Swizzle::RGBA();
    }
}

SkSpan<const TextureFormat> PreferredTextureFormats(vx_color_type ct) {
    #define N(...) std::size({__VA_ARGS__})
    #define CASE(C, ...) case C: { \
            static const std::array<TextureFormat, N(__VA_ARGS__)> kFormats{{__VA_ARGS__}}; \
            return SkSpan(kFormats); }

    switch (ct) {
        case VX_COLOR_TYPE_UNKNOWN:  return {};
        // NOTE: Not all backends support all TextureFormats. Some of the more advanced formats
        // may not be supported at all and have no viable fallback. For color types that have
        // equivalent texture formats differing only in RGB vs. BGR swizzle, we allow both
        // format variations to maximize color types that have some format. For alpha-only color
        // types, we only match to red-channel formats as they have the broadest support.

        //   SkColorType                    | TextureFormat(s)...
        CASE(VX_COLOR_TYPE_ALPHA_8,            TF::kR8)
        // NOTE: kRGB_565_SkColorType is misnamed and natively matches B5_G6_R5
        CASE(VX_COLOR_TYPE_RGB_565,            TF::kB5_G6_R5,   TF::kR5_G6_B5)
        // NOTE: kARGB_4444_SkColorType is misnamed and natively matches ABGR4
        CASE(VX_COLOR_TYPE_ARGB_4444,          TF::kABGR4,      TF::kARGB4)
        CASE(VX_COLOR_TYPE_RGBA_8888,          TF::kRGBA8,      TF::kBGRA8)
        CASE(VX_COLOR_TYPE_RGB_888X,           TF::kRGB8,       TF::kRGBA8,      TF::kBGRA8)
        CASE(VX_COLOR_TYPE_BGRA_8888,          TF::kBGRA8,      TF::kRGBA8)
        CASE(VX_COLOR_TYPE_RGBA_1010102,       TF::kRGB10_A2,   TF::kBGR10_A2)
        CASE(VX_COLOR_TYPE_BGRA_1010102,       TF::kBGR10_A2,   TF::kRGB10_A2)
        CASE(VX_COLOR_TYPE_RGB_101010X,        TF::kRGB10_A2,   TF::kBGR10_A2)
        CASE(VX_COLOR_TYPE_BGR_101010X,        TF::kBGR10_A2,   TF::kRGB10_A2)
        CASE(VX_COLOR_TYPE_BGR_101010X_XR,     TF::kBGR10_XR)
        CASE(VX_COLOR_TYPE_BGRA_10101010_XR,   TF::kBGRA10x6_XR)
        CASE(VX_COLOR_TYPE_RGBA_10X6,          TF::kRGBA10x6)
        CASE(VX_COLOR_TYPE_GRAY_8,             TF::kR8)
        CASE(VX_COLOR_TYPE_RGBA_F16NORM,       TF::kRGBA16F)
        CASE(VX_COLOR_TYPE_RGBA_F16,           TF::kRGBA16F)
        CASE(VX_COLOR_TYPE_RGB_F16F16F16X,     TF::kRGBA16F)
        CASE(VX_COLOR_TYPE_RGBA_F32,           TF::kRGBA32F)
        CASE(VX_COLOR_TYPE_R8G8_UNORM,         TF::kRG8)
        CASE(VX_COLOR_TYPE_A16_FLOAT,          TF::kR16F)
        CASE(VX_COLOR_TYPE_R16_FLOAT,          TF::kR16F)
        CASE(VX_COLOR_TYPE_R16G16_FLOAT,       TF::kRG16F)
        CASE(VX_COLOR_TYPE_A16_UNORM,          TF::kR16)
        CASE(VX_COLOR_TYPE_R16_UNORM,          TF::kR16)
        CASE(VX_COLOR_TYPE_R16G16_UNORM,       TF::kRG16)
        CASE(VX_COLOR_TYPE_R16G16B16A16_UNORM, TF::kRGBA16)
        CASE(VX_COLOR_TYPE_SRGBA_8888,         TF::kRGBA8_sRGB,
                                              TF::kBGRA8_sRGB)
        CASE(VX_COLOR_TYPE_R8_UNORM,           TF::kR8)
    }

    SkUNREACHABLE;
    #undef CASE
    #undef N
}

std::pair<vx_color_type, SkEnumBitMask<FormatXferOp>>
TextureFormatColorTypeInfo(TextureFormat format) {
    #define CASE(TF, CT, Ops) case TF: return {CT, Ops};
    using X = FormatXferOp;

    switch (format) {
        //   TextureFormat      | SkColorType                    | FormatXferOp(s)
        CASE(TF::kUnsupported,    VX_COLOR_TYPE_UNKNOWN,            X::kDisabled)

        CASE(TF::kR8,             VX_COLOR_TYPE_R8_UNORM,           X::kIdentity)
        CASE(TF::kR16,            VX_COLOR_TYPE_R16_UNORM,          X::kIdentity)
        CASE(TF::kR16F,           VX_COLOR_TYPE_R16_FLOAT,          X::kIdentity)
        CASE(TF::kR32F,           VX_COLOR_TYPE_R16_FLOAT,          X::kDisabled)
        CASE(TF::kA8,             VX_COLOR_TYPE_ALPHA_8,            X::kIdentity)
        CASE(TF::kRG8,            VX_COLOR_TYPE_R8G8_UNORM,         X::kIdentity)
        CASE(TF::kRG16,           VX_COLOR_TYPE_R16G16_UNORM,       X::kIdentity)
        CASE(TF::kRG16F,          VX_COLOR_TYPE_R16G16_FLOAT,       X::kIdentity)
        CASE(TF::kRG32F,          VX_COLOR_TYPE_R16G16_FLOAT,       X::kDisabled)
        CASE(TF::kRGB8,           VX_COLOR_TYPE_RGB_888X,           X::kDropAlpha)
        CASE(TF::kBGR8,           VX_COLOR_TYPE_RGB_888X,           X::kSwapRB | X::kDropAlpha)
        // NOTE: kRGB_565_SkColorType is misnamed and natively matches TextureFormat::kB5_G6_R5
        CASE(TF::kB5_G6_R5,       VX_COLOR_TYPE_RGB_565,            X::kIdentity)
        CASE(TF::kR5_G6_B5,       VX_COLOR_TYPE_RGB_565,            X::kSwapRB)
        CASE(TF::kRGB16,          VX_COLOR_TYPE_R16G16B16A16_UNORM, X::kDropAlpha)
        CASE(TF::kRGB16F,         VX_COLOR_TYPE_RGB_F16F16F16X,     X::kDropAlpha)
        CASE(TF::kRGB32F,         VX_COLOR_TYPE_RGBA_F32,           X::kDropAlpha)
        CASE(TF::kRGB8_sRGB,      VX_COLOR_TYPE_SRGBA_8888,         X::kDropAlpha)
        CASE(TF::kBGR10_XR,       VX_COLOR_TYPE_BGR_101010X_XR,     X::kIdentity)
        CASE(TF::kRGBA8,          VX_COLOR_TYPE_RGBA_8888,          X::kIdentity)
        CASE(TF::kRGBA16,         VX_COLOR_TYPE_R16G16B16A16_UNORM, X::kIdentity)
        CASE(TF::kRGBA16F,        VX_COLOR_TYPE_RGBA_F16,           X::kIdentity)
        CASE(TF::kRGBA32F,        VX_COLOR_TYPE_RGBA_F32,           X::kIdentity)
        CASE(TF::kRGB10_A2,       VX_COLOR_TYPE_RGBA_1010102,       X::kIdentity)
        CASE(TF::kRGBA10x6,       VX_COLOR_TYPE_RGBA_10X6,          X::kIdentity)
        CASE(TF::kRGBA8_sRGB,     VX_COLOR_TYPE_SRGBA_8888,         X::kIdentity)
        CASE(TF::kBGRA8,          VX_COLOR_TYPE_BGRA_8888,          X::kIdentity)
        CASE(TF::kBGR10_A2,       VX_COLOR_TYPE_BGRA_1010102,       X::kIdentity)
        CASE(TF::kBGRA8_sRGB,     VX_COLOR_TYPE_SRGBA_8888,         X::kSwapRB)
        // NOTE: kARGB_4444_SkColorType is misnamed and natively matches TextureFormat::kABGR4
        CASE(TF::kABGR4,          VX_COLOR_TYPE_ARGB_4444,          X::kIdentity)
        CASE(TF::kARGB4,          VX_COLOR_TYPE_ARGB_4444,          X::kSwapRB)
        CASE(TF::kBGRA10x6_XR,    VX_COLOR_TYPE_BGRA_10101010_XR,   X::kIdentity)

        // Compressed, multi-planar, and external formats can't exactly describe their data as
        // an SkColorType (although transfers with specialized data could be allowed).
        CASE(TF::kRGB8_ETC2,      VX_COLOR_TYPE_RGB_888X,           X::kDisabled)
        CASE(TF::kRGB8_ETC2_sRGB, VX_COLOR_TYPE_SRGBA_8888,         X::kDisabled)
        CASE(TF::kRGB8_BC1,       VX_COLOR_TYPE_RGB_888X,           X::kDisabled)
        CASE(TF::kRGBA8_BC1,      VX_COLOR_TYPE_RGBA_8888,          X::kDisabled)
        CASE(TF::kRGBA8_BC1_sRGB, VX_COLOR_TYPE_SRGBA_8888,         X::kDisabled)
        CASE(TF::kYUV8_P2_420,    VX_COLOR_TYPE_RGB_888X,           X::kDisabled)
        CASE(TF::kYUV8_P3_420,    VX_COLOR_TYPE_RGB_888X,           X::kDisabled)
        CASE(TF::kYUV10x6_P2_420, VX_COLOR_TYPE_RGBA_10X6,          X::kDisabled)
        CASE(TF::kYUV8_P2_422,    VX_COLOR_TYPE_RGB_888X,           X::kDisabled)
        CASE(TF::kYUV8_P3_422,    VX_COLOR_TYPE_RGB_888X,           X::kDisabled)
        CASE(TF::kYUV10x6_P2_422, VX_COLOR_TYPE_RGBA_10X6,          X::kDisabled)
        CASE(TF::kYUV8_P2_444,    VX_COLOR_TYPE_RGB_888X,           X::kDisabled)
        CASE(TF::kYUV8_P3_444,    VX_COLOR_TYPE_RGB_888X,           X::kDisabled)
        CASE(TF::kYUV10x6_P2_444, VX_COLOR_TYPE_RGBA_10X6,          X::kDisabled)
        CASE(TF::kExternal,       VX_COLOR_TYPE_RGBA_8888,          X::kDisabled)

        // Non color texture formats can't be used with SkColorType
        CASE(TF::kS8,             VX_COLOR_TYPE_UNKNOWN,            X::kDisabled)
        CASE(TF::kD16,            VX_COLOR_TYPE_UNKNOWN,            X::kDisabled)
        CASE(TF::kD32F,           VX_COLOR_TYPE_UNKNOWN,            X::kDisabled)
        CASE(TF::kD24_S8,         VX_COLOR_TYPE_UNKNOWN,            X::kDisabled)
        CASE(TF::kD32F_S8,        VX_COLOR_TYPE_UNKNOWN,            X::kDisabled)
    }

    SkUNREACHABLE;
    #undef CASE
}

bool AreColorTypeAndFormatCompatible(vx_color_type targetColorType, TextureFormat format) {
    // If the format maps to the color type, they are compatible
    auto [baseColorType, _] = TextureFormatColorTypeInfo(format);
    if (baseColorType != VX_COLOR_TYPE_UNKNOWN && baseColorType == targetColorType) {
        return true; // shortcut
    }

    // If the color type could map to the format, they are compatible
    for (TextureFormat preferred : PreferredTextureFormats(targetColorType)) {
        if (preferred == format) {
            return true;
        }
    }

    // Also allow kRGB_888x if kRGBA_8888 is compatible since RGBx is just a swizzle. This is almost
    // always handled by the combination of base color type and preferred formats, but for external
    // and compressed formats those two functions aren't quite descriptive enough.
    if (targetColorType == VX_COLOR_TYPE_RGB_888X &&
        AreColorTypeAndFormatCompatible(VX_COLOR_TYPE_RGBA_8888, format)) {
        return true;
    }

    // Otherwise consider them incompatible
    return false;
}

} // namespace skgpu::graphite
