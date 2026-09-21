/*
 * Copyright 2014 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#include "src/core/SkConvertPixels.h"

#include <vividx/core/color-type.h>
#include "include/core/SkImageInfo.h"
#include "include/core/SkSize.h"
#include "include/private/SkAssert.h"
#include "include/private/SkTPin.h"
#include "include/private/SkTemplates.h"
#include "src/core/SkColorData.h"
#include "src/core/SkColorSpaceXformSteps.h"
#include "src/core/SkImageInfoPriv.h"
#include "src/core/SkRasterPipeline.h"
#include "src/core/SkRasterPipelineOpContexts.h"
#include "src/core/SkRectMemcpy.h"
#include "src/core/SkSwizzlePriv.h"

#include <cstdint>
#include <cstring>
#include <initializer_list>

static bool rect_memcpy(const SkImageInfo& dstInfo,       void* dstPixels, size_t dstRB,
                        const SkImageInfo& srcInfo, const void* srcPixels, size_t srcRB,
                        const SkColorSpaceXformSteps& steps) {
    // We can copy the pixels when no color type, alpha type, or color space changes.
    if (dstInfo.colorType() != srcInfo.colorType()) {
        return false;
    }
    if (dstInfo.colorType() != VX_COLOR_TYPE_ALPHA_8
            && steps.fFlags.mask() != 0b00000) {
        return false;
    }

    if (srcPixels != dstPixels) {
        SkRectMemcpy(dstPixels, dstRB,
                     srcPixels, srcRB, dstInfo.minRowBytes(), dstInfo.height());
    }
    return true;
}

static bool swizzle_or_premul(const SkImageInfo& dstInfo,       void* dstPixels, size_t dstRB,
                              const SkImageInfo& srcInfo, const void* srcPixels, size_t srcRB,
                              const SkColorSpaceXformSteps& steps) {
    auto is_8888 = [](vx_color_type ct) {
        return ct == VX_COLOR_TYPE_RGBA_8888 || ct == VX_COLOR_TYPE_BGRA_8888;
    };
    if (!is_8888(dstInfo.colorType()) ||
        !is_8888(srcInfo.colorType()) ||
        steps.fFlags.linearize         ||
        steps.fFlags.gamut_transform   ||
#if !defined(SK_ARM_HAS_NEON)
        steps.fFlags.unpremul          ||
#endif
        steps.fFlags.encode) {
        return false;
    }

    const bool swapRB = dstInfo.colorType() != srcInfo.colorType();

    void (*fn)(uint32_t*, const uint32_t*, int) = nullptr;

    if (steps.fFlags.premul) {
        fn = swapRB ? SkOpts::RGBA_to_bgrA
                    : SkOpts::RGBA_to_rgbA;
    } else if (steps.fFlags.unpremul) {
        fn = swapRB ? SkOpts::rgbA_to_BGRA
                    : SkOpts::rgbA_to_RGBA;
    } else {
        // If we're not swizzling, we ought to have used rect_memcpy().
        SkASSERT(swapRB);
        fn = SkOpts::RGBA_to_BGRA;
    }

    for (int y = 0; y < dstInfo.height(); y++) {
        fn((uint32_t*)dstPixels, (const uint32_t*)srcPixels, dstInfo.width());
        dstPixels = SkTAddOffset<void>(dstPixels, dstRB);
        srcPixels = SkTAddOffset<const void>(srcPixels, srcRB);
    }
    return true;
}

static bool convert_to_alpha8(const SkImageInfo& dstInfo,       void* vdst, size_t dstRB,
                              const SkImageInfo& srcInfo, const void*  src, size_t srcRB,
                              const SkColorSpaceXformSteps&) {
    if (dstInfo.colorType() != VX_COLOR_TYPE_ALPHA_8) {
        return false;
    }
    auto dst = (uint8_t*)vdst;

    switch (srcInfo.colorType()) {
        case VX_COLOR_TYPE_UNKNOWN:
        case VX_COLOR_TYPE_ALPHA_8: {
            // Unknown should never happen.
            // Alpha8 should have been handled by rect_memcpy().
            SkASSERT(false);
            return false;
        }

        case VX_COLOR_TYPE_A16_UNORM: {
            auto src16 = (const uint16_t*) src;
            for (int y = 0; y < srcInfo.height(); y++) {
                for (int x = 0; x < srcInfo.width(); x++) {
                    dst[x] = src16[x] >> 8;
                }
                dst = SkTAddOffset<uint8_t>(dst, dstRB);
                src16 = SkTAddOffset<const uint16_t>(src16, srcRB);
            }
            return true;
        }

        case VX_COLOR_TYPE_GRAY_8:
        case VX_COLOR_TYPE_RGB_565:
        case VX_COLOR_TYPE_R8G8_UNORM:
        case VX_COLOR_TYPE_R16_UNORM:
        case VX_COLOR_TYPE_R16_FLOAT:
        case VX_COLOR_TYPE_R16G16_UNORM:
        case VX_COLOR_TYPE_R16G16_FLOAT:
        case VX_COLOR_TYPE_RGB_888X:
        case VX_COLOR_TYPE_RGB_101010X:
        case VX_COLOR_TYPE_BGR_101010X:
        case VX_COLOR_TYPE_BGR_101010X_XR:
        case VX_COLOR_TYPE_RGB_F16F16F16X:
        case VX_COLOR_TYPE_R8_UNORM: {
            for (int y = 0; y < srcInfo.height(); ++y) {
               memset(dst, 0xFF, srcInfo.width());
               dst = SkTAddOffset<uint8_t>(dst, dstRB);
            }
            return true;
        }

        case VX_COLOR_TYPE_ARGB_4444: {
            auto src16 = (const uint16_t*) src;
            for (int y = 0; y < srcInfo.height(); y++) {
                for (int x = 0; x < srcInfo.width(); x++) {
                    dst[x] = SkPacked4444ToA32(src16[x]);
                }
                dst = SkTAddOffset<uint8_t>(dst, dstRB);
                src16 = SkTAddOffset<const uint16_t>(src16, srcRB);
            }
            return true;
        }

        case VX_COLOR_TYPE_BGRA_8888:
        case VX_COLOR_TYPE_RGBA_8888:
        case VX_COLOR_TYPE_SRGBA_8888: {
            auto src32 = (const uint32_t*) src;
            for (int y = 0; y < srcInfo.height(); y++) {
                for (int x = 0; x < srcInfo.width(); x++) {
                    dst[x] = src32[x] >> 24;
                }
                dst = SkTAddOffset<uint8_t>(dst, dstRB);
                src32 = SkTAddOffset<const uint32_t>(src32, srcRB);
            }
            return true;
        }

        case VX_COLOR_TYPE_RGBA_1010102:
        case VX_COLOR_TYPE_BGRA_1010102: {
            auto src32 = (const uint32_t*) src;
            for (int y = 0; y < srcInfo.height(); y++) {
                for (int x = 0; x < srcInfo.width(); x++) {
                    dst[x] = (src32[x] >> 30) * 0x55;
                }
                dst = SkTAddOffset<uint8_t>(dst, dstRB);
                src32 = SkTAddOffset<const uint32_t>(src32, srcRB);
            }
            return true;
        }

        case VX_COLOR_TYPE_RGBA_F16NORM:
        case VX_COLOR_TYPE_RGBA_F16: {
            auto src64 = (const uint64_t*) src;
            for (int y = 0; y < srcInfo.height(); y++) {
                for (int x = 0; x < srcInfo.width(); x++) {
                    dst[x] = (uint8_t) (255.0f * vx_uint16_to_float(src64[x] >> 48));
                }
                dst = SkTAddOffset<uint8_t>(dst, dstRB);
                src64 = SkTAddOffset<const uint64_t>(src64, srcRB);
            }
            return true;
        }

        case VX_COLOR_TYPE_RGBA_F32: {
            auto rgba = (const float*)src;
            for (int y = 0; y < srcInfo.height(); y++) {
                for (int x = 0; x < srcInfo.width(); x++) {
                    dst[x] = (uint8_t)(255.0f * rgba[4*x+3]);
                }
                dst  = SkTAddOffset<uint8_t>(dst, dstRB);
                rgba = SkTAddOffset<const float>(rgba, srcRB);
            }
            return true;
        }

        case VX_COLOR_TYPE_A16_FLOAT: {
            auto srcF16 = (const uint16_t*) src;
            for (int y = 0; y < srcInfo.height(); y++) {
                for (int x = 0; x < srcInfo.width(); x++) {
                    dst[x] = (uint8_t) (255.0f * vx_uint16_to_float(srcF16[x]));
                }
                dst = SkTAddOffset<uint8_t>(dst, dstRB);
                srcF16 = SkTAddOffset<const uint16_t>(srcF16, srcRB);
            }
            return true;
        }

        case VX_COLOR_TYPE_BGRA_10101010_XR: {
            auto src64 = (const uint64_t*) src;
            for (int y = 0; y < srcInfo.height(); y++) {
                for (int x = 0; x < srcInfo.width(); x++) {
                    static constexpr int64_t kZero = 384;
                    static constexpr int64_t kRange = 510;
                    static constexpr int64_t kMaxU8 = 0xff;
                    static constexpr int64_t kMinU8 = 0x00;
                    static constexpr int64_t kDivisor = kRange / kMaxU8;
                    int64_t raw_alpha = src64[x] >> 54;
                    // f(384) = 0
                    // f(894) = 255
                    int64_t alpha =
                            SkTPin((raw_alpha - kZero) / kDivisor, kMinU8, kMaxU8);
                    dst[x] = static_cast<uint8_t>(alpha);
                }
                dst = SkTAddOffset<uint8_t>(dst, dstRB);
                src64 = SkTAddOffset<const uint64_t>(src64, srcRB);
            }
            return true;
        }
        case VX_COLOR_TYPE_RGBA_10X6:
        case VX_COLOR_TYPE_R16G16B16A16_UNORM: {
            auto src64 = (const uint64_t*) src;
            for (int y = 0; y < srcInfo.height(); y++) {
                for (int x = 0; x < srcInfo.width(); x++) {
                    dst[x] = (src64[x] >> 48) >> 8;
                }
                dst = SkTAddOffset<uint8_t>(dst, dstRB);
                src64 = SkTAddOffset<const uint64_t>(src64, srcRB);
            }
            return true;
        }
    }
    return false;
}

// Default: Use the pipeline.
static void convert_with_pipeline(const SkImageInfo& dstInfo, void* dstRow, int dstStride,
                                  const SkImageInfo& srcInfo, const void* srcRow, int srcStride,
                                  const SkColorSpaceXformSteps& steps) {
    SkRasterPipelineContexts::MemoryCtx src = {const_cast<void*>(srcRow), srcStride},
                                        dst = {dstRow, dstStride};

    SkRasterPipeline_<256> pipeline;
    pipeline.appendLoad(srcInfo.colorType(), &src);
    steps.apply(&pipeline);
    pipeline.appendStore(dstInfo.colorType(), &dst);
    pipeline.run(0,0, srcInfo.width(), srcInfo.height());
}

bool SkConvertPixels(const SkImageInfo& dstInfo,       void* dstPixels, size_t dstRB,
                     const SkImageInfo& srcInfo, const void* srcPixels, size_t srcRB) {
    SkASSERT(dstInfo.dimensions() == srcInfo.dimensions());
    SkASSERT(SkImageInfoValidConversion(dstInfo, srcInfo));

    int srcStride = (int)(srcRB / srcInfo.bytesPerPixel());
    int dstStride = (int)(dstRB / dstInfo.bytesPerPixel());
    if ((size_t)srcStride * srcInfo.bytesPerPixel() != srcRB ||
        (size_t)dstStride * dstInfo.bytesPerPixel() != dstRB) {
        return false;
    }

    if (srcPixels == dstPixels &&
        srcInfo.bytesPerPixel() != dstInfo.bytesPerPixel()) {
        // In-place conversions are not supported for different pixel widths.
        return false;
    }

    SkColorSpaceXformSteps steps{srcInfo.colorSpace(), srcInfo.alphaType(),
                                 dstInfo.colorSpace(), dstInfo.alphaType()};

    for (auto fn : {rect_memcpy, swizzle_or_premul, convert_to_alpha8}) {
        if (fn(dstInfo, dstPixels, dstRB, srcInfo, srcPixels, srcRB, steps)) {
            return true;
        }
    }
    convert_with_pipeline(dstInfo, dstPixels, dstStride, srcInfo, srcPixels, srcStride, steps);
    return true;
}
