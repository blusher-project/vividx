#ifndef VIVIDX_CORE_COLOR_TYPE_H
#define VIVIDX_CORE_COLOR_TYPE_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

enum vx_color_type {
    // Unknown or unrepresentable as an SkColorType.
    VX_COLOR_TYPE_UNKNOWN,
    // Single channel data (8-bit) interpreted as an alpha value. RGB are 0.
    //   Bits: [A:7..0]
    VX_COLOR_TYPE_ALPHA_8,
    // Three channel BGR data (5 bits red, 6 bits green, 5 bits blue) packed into a LE 16-bit word.
    // NOTE: The name of this enum value does not match the standard convention for SkColorType.
    //   Bits: [R:15..11 G:10..5 B:4..0]
    VX_COLOR_TYPE_RGB_565,
    // Four channel ABGR data (4 bits per channel) packed into a LE 16-bit word.
    // NOTE: The name of this enum value does not match the standard convention for SkColorType.
    //   Bits: [R:15..12 G:11..8 B:7..4 A:3..0]
    VX_COLOR_TYPE_ARGB_4444,
    // Four channel RGBA data (8 bits per channel) packed into a LE 32-bit word.
    //   Bits: [A:31..24 B:23..16 G:15..8 R:7..0]
    VX_COLOR_TYPE_RGBA_8888,
    // Three channel RGB data (8 bits per channel) packed into a LE 32-bit word. The remaining bits
    // are ignored and alpha is forced to opaque.
    //   Bits: [x:31..24 B:23..16 G:15..8 R:7..0]
    VX_COLOR_TYPE_RGB_888X,
    // Four channel BGRA data (8 bits per channel) packed into a LE 32-bit word. R and B are swapped
    // relative to kRGBA_8888.
    //   Bits: [A:31..24 R:23..16 G:15..8 B:7..0]
    VX_COLOR_TYPE_BGRA_8888,
    // Four channel RGBA data (10 bits per color, 2 bits for alpha) packed into a LE 32-bit word.
    //   Bits: [A:31..30 B:29..20 G:19..10 R:9..0]
    VX_COLOR_TYPE_RGBA_1010102,
    // Four channel BGRA data (10 bits per color, 2 bits for alpha) packed into a LE 32-bit word.
    // R and B are swapped relative to kRGBA_1010102.
    //   Bits: [A:31..30 R:29..20 G:19..10 B:9..0]
    VX_COLOR_TYPE_BGRA_1010102,
    // Three channel RGB data (10 bits per channel) packed into a LE 32-bit word. The remaining bits
    // are ignored and alpha is forced to opaque.
    //   Bits: [x:31..30 B:29..20 G:19..10 R:9..0]
    VX_COLOR_TYPE_RGB_101010X,
    // Three channel BGR data (10 bits per channel) packed into a LE 32-bit word. The remaining bits
    // are ignored and alpha is forced to opaque. R and B are swapped relative to kRGB_101010x.
    //   Bits: [x:31..30 R:29..20 G:19..10 B:9..0]
    VX_COLOR_TYPE_BGR_101010X,
    // Three channel BGR data (10 bits per channel) packed into a LE 32-bit word. The remaining bits
    // are ignored and alpha is forced to opaque. Instead of normalizing [0, 1023] to [0.0, 1.0] the
    // color channels map to an extended range of [-0.752941, 1.25098], compatible with
    // MTLPixelFormatBGR10_XR.
    //   Bits: [x:31..30 R:29..20 G:19..10 B:9..0]
    VX_COLOR_TYPE_BGR_101010X_XR,
    // Four channel BGRA data (10 bits per channel) packed into a LE 64-bit word. Each channel is
    // preceded by 6 bits of padding.  Instead of normalizing [0, 1023] to [0.0, 1.0] the color and
    // alpha channels map to an extended range of [-0.752941, 1.25098], compatible with
    // MTLPixelFormatBGRA10_XR.
    //   Bits: [A:63..54 x:53..48 R:47..38 x:37..32 G:31..22 x:21..16 B:15..6 x:5..0]
    VX_COLOR_TYPE_BGRA_10101010_XR,
    // Four channel RGBA data (10 bits per channel) packed into a LE 64-bit word. Each channel is
    // preceded by 6 bits of padding.
    //   Bits: [A:63..54 x:53..48 B:47..38 x:37..32 G:31..22 x:21..16 R:15..6 x:5..0]
    VX_COLOR_TYPE_RGBA_10X6,
    // Single channel data (8-bit) interpreted as a grayscale value (e.g. replicated to RGB).
    //   Bits: [G:7..0]
    VX_COLOR_TYPE_GRAY_8,
    // Four channel RGBA data (16-bit half-float per channel) packed into a LE 64-bit word. Values
    // are assumed to be in [0.0,1.0] range, unlike kRGBA_F16.
    //   Bits: [A:63..48 B:47..32 G:31..16 R:15..0]
    VX_COLOR_TYPE_RGBA_F16NORM,
    // Four channel RGBA data (16-bit half-float per channel) packed into a LE 64-bit word.
    // This has extended range compared to kRGBA_F16Norm.
    //   Bits: [A:63..48 B:47..32 G:31..16 R:15..0]
    VX_COLOR_TYPE_RGBA_F16,
    // Three channel RGB data (16-bit half-float per channel) packed into a LE 64-bit word. The last
    // 16 bits are ignored and alpha is forced to opaque.
    //   Bits: [x:63..48 B:47..32 G:31..16 R:15..0]
    VX_COLOR_TYPE_RGB_F16F16F16X,
    // Four channel RGBA data (32-bit float per channel) packed into a LE 128-bit word.
    //   Bits: [A:127..96 B:95..64 G:63..32 R:31..0]
    VX_COLOR_TYPE_RGBA_F32,

    // The following 8 colortypes are just for reading from - not for rendering to

    // Two channel RG data (8 bits per channel). Blue is forced to 0, alpha is forced to opaque.
    //   Bits: [G:15..8 R:7..0]
    VX_COLOR_TYPE_R8G8_UNORM,
    // Single channel data (16-bit half-float) interpreted as alpha. RGB are 0.
    //   Bits: [A:15..0]
    VX_COLOR_TYPE_A16_FLOAT,
    // Two channel RG data (16-bit half-float per channel) packed into a LE 32-bit word.
    // Blue is forced to 0, alpha is forced to opaque.
    //   Bits: [G:31..16 R:15..0]
    VX_COLOR_TYPE_R16G16_FLOAT,
    // Single channel data (16 bits) interpreted as alpha. RGB are 0.
    //   Bits: [A:15..0]
    VX_COLOR_TYPE_A16_UNORM,
    // Single channel data (16 bits) interpreted as red. G and B are forced to 0, alpha is forced to
    // opaque.
    //   Bits: [R:15..0]
    VX_COLOR_TYPE_R16_UNORM,
    // Two channel RG data (16 bits per channel) packed into a LE 32-bit word. B is forced to 0,
    // alpha is forced to opaque.
    //   Bits: [G:31..16 R:15..0]
    VX_COLOR_TYPE_R16G16_UNORM,
    // Four channel RGBA data (16 bits per channel) packed into a LE 64-bit word.
    //   Bits: [A:63..48 B:47..32 G:31..16 R:15..0]
    VX_COLOR_TYPE_R16G16B16A16_UNORM,
    // Four channel RGBA data (8 bits per channel) packed into a LE 32-bit word. The RGB values are
    // assumed to be encoded with the sRGB transfer function, which can be decoded automatically
    // by GPU hardware with certain texture formats.
    //   Bits: [A:31..24 B:23..16 G:15..8 R:7..0]
    VX_COLOR_TYPE_SRGBA_8888,
    // Single channel data (8 bits) interpreted as red. G and B are forced to 0, alpha is forced to
    // opaque.
    //    Bits: [R:7..0]
    VX_COLOR_TYPE_R8_UNORM,
};

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* VIVIDX_CORE_COLOR_TYPE_H */
