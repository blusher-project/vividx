/*
 * Copyright 2018 The Android Open Source Project
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef skcpu_GlyphRunPainter_DEFINED
#define skcpu_GlyphRunPainter_DEFINED

#include "include/core/SkSurfaceProps.h"

#include <cstdint>

class SkCanvas;
class SkColorSpace;
class SkMatrix;
class SkPaint;
#include <vividx/core/color-type.h>
enum class SkScalerContextFlags : uint32_t;
namespace sktext {
class GlyphRunList;
}

namespace skcpu {

class BitmapDevicePainter;

class GlyphRunListPainter {
public:
    GlyphRunListPainter(const SkSurfaceProps& props, vx_color_type colorType, SkColorSpace* cs);

    void drawForBitmapDevice(
            SkCanvas* canvas, const BitmapDevicePainter* bitmapDevice,
            const sktext::GlyphRunList& glyphRunList, const SkPaint& paint,
            const SkMatrix& drawMatrix);
private:
    // The props as on the actual device.
    const SkSurfaceProps fDeviceProps;

    // The props for when the bitmap device can't draw LCD text.
    const SkSurfaceProps fBitmapFallbackProps;
    const vx_color_type fColorType;
    const SkScalerContextFlags fScalerContextFlags;
};

}  // namespace skcpu

#endif  // skcpu_GlyphRunPainter_DEFINED
