/*
 * Copyright 2017 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "include/codec/SkAndroidCodec.h"
#include <vividx/core/alpha-type.h>
#include "include/core/SkBitmap.h"
#include "include/core/SkColor.h"
#include "include/core/SkColorSpace.h"
#include <vividx/core/color-type.h>
#include "include/core/SkData.h"
#include "include/core/SkDataTable.h"
#include "include/core/SkImageInfo.h"
#include "include/encode/SkPngEncoder.h"
#include "tests/Test.h"

#include <memory>
#include <utility>

DEF_TEST(Codec_recommendedF16, r) {
    // Encode an F16 bitmap. SkPngEncoder will encode this to a true-color PNG
    // with a bit depth of 16. SkAndroidCodec should always recommend F16 for
    // such a PNG.
    SkBitmap bm;
    bm.allocPixels(SkImageInfo::Make(10, 10, VX_COLOR_TYPE_RGBA_F16,
            VX_ALPHA_TYPE_PREMULTIPLIED, SkColorSpace::MakeSRGB()));
    // What is drawn is not important.
    bm.eraseColor(SK_ColorBLUE);

    auto data = SkPngEncoder::Encode(bm.pixmap(), {});
    REPORTER_ASSERT(r, data != nullptr);
    auto androidCodec = SkAndroidCodec::MakeFromData(std::move(data));
    if (!androidCodec) {
        ERRORF(r, "Failed to create SkAndroidCodec");
        return;
    }

    REPORTER_ASSERT(r, androidCodec->computeOutputColorType(VX_COLOR_TYPE_N32)
            == VX_COLOR_TYPE_RGBA_F16);
}
