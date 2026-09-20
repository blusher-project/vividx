/*
 * Copyright 2021 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include <vividx/core/alpha-type.h>
#include <vividx/core/color-type.h>
#include "include/core/SkImageInfo.h"
#include "include/core/SkRefCnt.h"
#include "include/core/SkSurface.h"
#include "include/core/SkTypes.h"
#include "include/gpu/GpuTypes.h"
#include "include/gpu/ganesh/GrDirectContext.h"
#include "include/gpu/ganesh/SkSurfaceGanesh.h"
#include "tests/CtsEnforcement.h"
#include "tests/Test.h"

struct GrContextOptions;

DEF_GANESH_TEST_FOR_ALL_CONTEXTS(skbug12214, r, contextInfo, CtsEnforcement::kApiLevel_T) {
    auto imageInfo = SkImageInfo::Make(/*width=*/32, /*height=*/32, VX_COLOR_TYPE_RGBA_8888,
                                       VX_ALPHA_TYPE_PREMULTIPLIED);
    sk_sp<SkSurface> surface1 =
            SkSurfaces::RenderTarget(contextInfo.directContext(), skgpu::Budgeted::kNo, imageInfo);
    sk_sp<SkSurface> surface2 = SkSurfaces::Raster(imageInfo);

    // The test succeeds if this draw does not crash. (See skbug.com/40043311)
    surface1->draw(surface2->getCanvas(), /*x=*/0, /*y=*/0);
}
