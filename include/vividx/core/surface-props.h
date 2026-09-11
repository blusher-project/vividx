#ifndef VIVIDX_CORE_SURFACE_PROPS_H
#define VIVIDX_CORE_SURFACE_PROPS_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/// Description of how the LCD strips are arranged for each pixel.
/// If this is unknown, or the pixels are meant to be "portable" and/or
/// transformed before showing (e.g. rotated, scaled) then use
/// `VX_PIXEL_GEOMETRY_UNKNOWN`.
enum vx_pixel_geometry {
    VX_PIXEL_GEOMETRY_UNKNOWN,
    VX_PIXEL_GEOMETRY_RGB_H,
    VX_PIXEL_GEOMETRY_BGR_H,
    VX_PIXEL_GEOMETRY_RGB_V,
    VX_PIXEL_GEOMETRY_BGR_V,
};

/// \struct vx_surface_props_t
///
/// Describes properties and constraints of a given `vx_surface_t`.
/// The rendering engine can parse these during drawing, and can sometimes
/// optimize its performance (e.g. disabling an expensive feature).
typedef struct vx_surface_props_t vx_surface_props_t;

enum vx_surface_props_flags {
    VX_SURFACE_PROPS_FLAG_DEFAULT = 0,
    VX_SURFACE_PROPS_FLAG_USE_DEVICE_INDEPENDENT_FONTS = 1 << 0,
    // Use internal MSAA to render to non-MSAA GPU surfaces.
    VX_SURFACE_PROPS_FLAG_DYNAMIC_MSAA = 1 << 1,
    // If set, all rendering will have dithering enabled
    // Currently this only impacts GPU backends
    VX_SURFACE_PROPS_FLAG_ALWAYS_DITHER = 1 << 2,
    // The surface will preserve transparent draws (instead of skipping them).
    VX_SURFACE_PROPS_FLAG_PRESERVES_TRANSPARENT_DRAWS = 1 << 3,
};

#if 0
// Returns true iff geo is a known geometry and is RGB.
static inline bool SkPixelGeometryIsRGB(SkPixelGeometry geo) {
    return kRGB_H_SkPixelGeometry == geo || kRGB_V_SkPixelGeometry == geo;
}

// Returns true iff geo is a known geometry and is BGR.
static inline bool SkPixelGeometryIsBGR(SkPixelGeometry geo) {
    return kBGR_H_SkPixelGeometry == geo || kBGR_V_SkPixelGeometry == geo;
}

// Returns true iff geo is a known geometry and is horizontal.
static inline bool SkPixelGeometryIsH(SkPixelGeometry geo) {
    return kRGB_H_SkPixelGeometry == geo || kBGR_H_SkPixelGeometry == geo;
}

// Returns true iff geo is a known geometry and is vertical.
static inline bool SkPixelGeometryIsV(SkPixelGeometry geo) {
    return kRGB_V_SkPixelGeometry == geo || kBGR_V_SkPixelGeometry == geo;
}


class SK_API SkSurfaceProps {
public:
    enum Flags {
        kDefault_Flag = 0,
        kUseDeviceIndependentFonts_Flag = 1 << 0,
        // Use internal MSAA to render to non-MSAA GPU surfaces.
        kDynamicMSAA_Flag = 1 << 1,
        // If set, all rendering will have dithering enabled
        // Currently this only impacts GPU backends
        kAlwaysDither_Flag = 1 << 2,
        // The surface will preserve transparent draws (instead of skipping them).
        kPreservesTransparentDraws_Flag = 1 << 3,
    };

    /** No flags, unknown pixel geometry, platform-default contrast/gamma. */
    SkSurfaceProps();
    /** TODO(kschmi): Remove this constructor and replace with the one below. **/
    SkSurfaceProps(uint32_t flags, SkPixelGeometry);
    /** Specified pixel geometry, text contrast, and gamma **/
    SkSurfaceProps(uint32_t flags, SkPixelGeometry, SkScalar textContrast, SkScalar textGamma);

    SkSurfaceProps(const SkSurfaceProps&) = default;
    SkSurfaceProps& operator=(const SkSurfaceProps&) = default;

    SkSurfaceProps cloneWithPixelGeometry(SkPixelGeometry newPixelGeometry) const {
        return SkSurfaceProps(fFlags, newPixelGeometry, fTextContrast, fTextGamma);
    }

    static constexpr SkScalar kMaxContrastInclusive = 1;
    static constexpr SkScalar kMinContrastInclusive = 0;
    static constexpr SkScalar kMaxGammaExclusive = 4;
    static constexpr SkScalar kMinGammaInclusive = 0;

    uint32_t flags() const { return fFlags; }
    SkPixelGeometry pixelGeometry() const { return fPixelGeometry; }
    SkScalar textContrast() const { return fTextContrast; }
    SkScalar textGamma() const { return fTextGamma; }

    bool isUseDeviceIndependentFonts() const {
        return SkToBool(fFlags & kUseDeviceIndependentFonts_Flag);
    }

    bool isAlwaysDither() const {
        return SkToBool(fFlags & kAlwaysDither_Flag);
    }

    bool preservesTransparentDraws() const {
        return SkToBool(fFlags & kPreservesTransparentDraws_Flag);
    }

    bool operator==(const SkSurfaceProps& that) const {
        return fFlags == that.fFlags && fPixelGeometry == that.fPixelGeometry &&
        fTextContrast == that.fTextContrast && fTextGamma == that.fTextGamma;
    }

    bool operator!=(const SkSurfaceProps& that) const {
        return !(*this == that);
    }
#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* VIVIDX_CORE_SURFACE_PROPS_H */
