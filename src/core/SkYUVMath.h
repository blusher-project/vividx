/*
 * Copyright 2019 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkYUVMath_DEFINED
#define SkYUVMath_DEFINED

#include <vividx/core/image-info.h>

void SkColorMatrix_RGB2YUV(vx_yuv_color_space, float m[20]);
void SkColorMatrix_YUV2RGB(vx_yuv_color_space, float m[20]);

// Used to create the pre-compiled tables in SkYUVMath.cpp
void SkColorMatrix_DumpYUVMatrixTables();

#endif
