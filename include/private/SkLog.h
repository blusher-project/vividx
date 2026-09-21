/*
 * Copyright 2026 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkLog_DEFINED
#define SkLog_DEFINED

#include <cstdarg>

#include <vividx/common.h>
#include <vividx/common.h>
// #include "include/private/SkLoadUserConfig.h" // IWYU pragma: keep
// #include "include/private/SkLogPriority.h"

#include <vividx/assert.h>

#if !defined(SkLog)
// Implemented per platform.
void SkLogVAList(enum vx_log_priority priority, const char format[], va_list args) VX_PRINTF_LIKE(2, 0);
void SK_SPI SkLog(enum vx_log_priority priority, const char format[], ...) VX_PRINTF_LIKE(2, 3);
#endif

#if !defined(SKIA_LOWEST_ACTIVE_LOG_PRIORITY)
#ifdef SK_DEBUG
    #define SKIA_LOWEST_ACTIVE_LOG_PRIORITY VX_LOG_PRIORITY_DEBUG
#else
    #define SKIA_LOWEST_ACTIVE_LOG_PRIORITY VX_LOG_PRIORITY_INFO
#endif
#endif

#define SKIA_LOG(priority, fmt, ...)                                           \
    do {                                                                       \
        if constexpr (priority <= SKIA_LOWEST_ACTIVE_LOG_PRIORITY) {           \
            SkLog(priority, "[skia] " fmt "\n", ##__VA_ARGS__);                \
        }                                                                      \
    } while (0)

// TODO: SKIA_LOG_E defined void. Fix this.
// #define SKIA_LOG_E(fmt, ...) SKIA_LOG(VX_LOG_PRIORITY_ERROR, "** ERROR ** " fmt, ##__VA_ARGS__)
#define SKIA_LOG_W(fmt, ...) SKIA_LOG(VX_LOG_PRIORITY_WARNING, "WARNING - " fmt, ##__VA_ARGS__)
#define SKIA_LOG_I(fmt, ...) SKIA_LOG(VX_LOG_PRIORITY_INFO, fmt, ##__VA_ARGS__)
#define SKIA_LOG_D(fmt, ...) SKIA_LOG(VX_LOG_PRIORITY_DEBUG, fmt, ##__VA_ARGS__)

#endif // SkLog_DEFINED
