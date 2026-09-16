/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "src/codec/SkSampler.h"

#include "include/codec/SkCodec.h"
#include "include/core/SkColorType.h"
#include "include/core/SkImageInfo.h"
#include "include/private/SkTemplates.h"
#include "src/codec/SkCodecPriv.h"
#include "src/core/SkMemset.h"

#include <cstdint>
#include <cstring>

void SkSampler::Fill(const SkImageInfo& info, void* dst, size_t rowBytes,
                     SkCodec::ZeroInitialized zeroInit) {
    SkASSERT(dst != nullptr);

    if (SkCodec::kYes_ZeroInitialized == zeroInit) {
        return;
    }

    const int width = info.width();
    const int numRows = info.height();

    // Use the proper memset routine to fill the remaining bytes
    switch (info.colorType()) {
        case VX_COLOR_TYPE_RGBA_8888:
        case VX_COLOR_TYPE_BGRA_8888: {
            uint32_t* dstRow = (uint32_t*) dst;
            for (int row = 0; row < numRows; row++) {
                SkOpts::memset32(dstRow, 0, width);
                dstRow = SkTAddOffset<uint32_t>(dstRow, rowBytes);
            }
            break;
        }
        case VX_COLOR_TYPE_RGB_565: {
            uint16_t* dstRow = (uint16_t*) dst;
            for (int row = 0; row < numRows; row++) {
                SkOpts::memset16(dstRow, 0, width);
                dstRow = SkTAddOffset<uint16_t>(dstRow, rowBytes);
            }
            break;
        }
        case VX_COLOR_TYPE_GRAY_8: {
            uint8_t* dstRow = (uint8_t*) dst;
            for (int row = 0; row < numRows; row++) {
                memset(dstRow, 0, width);
                dstRow = SkTAddOffset<uint8_t>(dstRow, rowBytes);
            }
            break;
        }
        case VX_COLOR_TYPE_RGBA_F16: {
            uint64_t* dstRow = (uint64_t*) dst;
            for (int row = 0; row < numRows; row++) {
                SkOpts::memset64(dstRow, 0, width);
                dstRow = SkTAddOffset<uint64_t>(dstRow, rowBytes);
            }
            break;
        }
        default:
            SkCodecPrintf("Error: Unsupported dst color type for fill().  Doing nothing.\n");
            SkASSERT(false);
            break;
    }
}
