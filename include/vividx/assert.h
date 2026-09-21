/*
 * Copyright 2022 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * 2026, Aspen Schneider.
 */

#ifndef VIVIDX_ASSERT_H
#define VIVIDX_ASSERT_H

#include <vividx/common.h>

//!<=============================
//!< Features
//!<-----------------------------
//!< Copied from SkFeatures.h
//!<=============================

#if !defined(SK_BUILD_FOR_ANDROID) && !defined(SK_BUILD_FOR_IOS) && !defined(SK_BUILD_FOR_WIN) && \
    !defined(SK_BUILD_FOR_UNIX) && !defined(SK_BUILD_FOR_MAC)

    #ifdef __APPLE__
        #include <TargetConditionals.h>
    #endif

    #if defined(_WIN32) || defined(__SYMBIAN32__)
        #define SK_BUILD_FOR_WIN
    #elif defined(ANDROID) || defined(__ANDROID__)
        #define SK_BUILD_FOR_ANDROID
    #elif defined(__EMSCRIPTEN__)
        // WASM toolchains expose a Unix-like compilation environment, but it is
        // not Unix (e.g. posix signals are not supported).
        #define SK_BUILD_FOR_WASM
    #elif defined(linux) || defined(__linux) || defined(__FreeBSD__) || \
          defined(__OpenBSD__) || defined(__sun) || defined(__NetBSD__) || \
          defined(__DragonFly__) || defined(__Fuchsia__) || \
          defined(__GLIBC__) || defined(__GNU__) || defined(__unix__)
        #define SK_BUILD_FOR_UNIX
    #elif TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
        #define SK_BUILD_FOR_IOS
    #else
        #define SK_BUILD_FOR_MAC
    #endif
#endif // end SK_BUILD_FOR_*


#if defined(SK_BUILD_FOR_WIN) && !defined(__clang__)
    #if !defined(SK_RESTRICT)
        #define SK_RESTRICT __restrict
    #endif
#endif

#if !defined(SK_RESTRICT)
    #define SK_RESTRICT __restrict__
#endif

#if !defined(SK_CPU_BENDIAN) && !defined(SK_CPU_LENDIAN)
    #if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
        #define SK_CPU_BENDIAN
    #elif defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
        #define SK_CPU_LENDIAN
    #elif defined(__sparc) || defined(__sparc__) || \
      defined(_POWER) || defined(__powerpc__) || \
      defined(__ppc__) || defined(__hppa) || \
      defined(__PPC__) || defined(__PPC64__) || \
      defined(_MIPSEB) || defined(__ARMEB__) || \
      defined(__s390__) || \
      (defined(__sh__) && defined(__BIG_ENDIAN__)) || \
      (defined(__ia64) && defined(__BIG_ENDIAN__))
         #define SK_CPU_BENDIAN
    #else
        #define SK_CPU_LENDIAN
    #endif
#endif

#if defined(__i386) || defined(_M_IX86) ||  defined(__x86_64__) || defined(_M_X64)
  #define SK_CPU_X86 1
#endif

#if defined(__loongarch__) || defined (__loongarch64)
  #define SK_CPU_LOONGARCH 1
#endif

#if defined(__powerpc__) || defined (__powerpc64__)
  #define SK_CPU_PPC 1
#endif

/**
 *  SK_CPU_X64_LEVEL
 *
 *  If defined, SK_CPU_X64_LEVEL should be set to the highest supported level.
 *  On non-Intel, non-AMD CPUs this should be undefined.
 */
#define SK_CPU_X64_LEVEL_SSE1     10
#define SK_CPU_X64_LEVEL_SSE2     20
#define SK_CPU_X64_LEVEL_SSE3     30
#define SK_CPU_X64_LEVEL_SSSE3    31
#define SK_CPU_X64_LEVEL_SSE41    41
#define SK_CPU_X64_LEVEL_SSE42    42
#define SK_CPU_X64_LEVEL_AVX      51
#define SK_CPU_X64_LEVEL_AVX2     52
#define SK_CPU_X64_LEVEL_ML4      60

/**
 *  SK_CPU_LSX_LEVEL
 *
 *  If defined, SK_CPU_LSX_LEVEL should be set to the highest supported level.
 *  On non-loongarch CPU this should be undefined.
 */
#define SK_CPU_LSX_LEVEL_LSX      10
#define SK_CPU_LSX_LEVEL_LASX     20

// TODO(kjlubick) clean up these checks

// Are we in GCC/Clang?
#ifndef SK_CPU_X64_LEVEL
    // These checks must be done in descending order to ensure we set the highest
    // available SSE level.
    #if defined(__AVX512F__) && defined(__AVX512DQ__) && defined(__AVX512CD__) && \
        defined(__AVX512BW__) && defined(__AVX512VL__)
        #define SK_CPU_X64_LEVEL    SK_CPU_X64_LEVEL_ML4
    #elif defined(__AVX2__)
        #define SK_CPU_X64_LEVEL    SK_CPU_X64_LEVEL_AVX2
    #elif defined(__AVX__)
        #define SK_CPU_X64_LEVEL    SK_CPU_X64_LEVEL_AVX
    #elif defined(__SSE4_2__)
        #define SK_CPU_X64_LEVEL    SK_CPU_X64_LEVEL_SSE42
    #elif defined(__SSE4_1__)
        #define SK_CPU_X64_LEVEL    SK_CPU_X64_LEVEL_SSE41
    #elif defined(__SSSE3__)
        #define SK_CPU_X64_LEVEL    SK_CPU_X64_LEVEL_SSSE3
    #elif defined(__SSE3__)
        #define SK_CPU_X64_LEVEL    SK_CPU_X64_LEVEL_SSE3
    #elif defined(__SSE2__)
        #define SK_CPU_X64_LEVEL    SK_CPU_X64_LEVEL_SSE2
    #endif
#endif

#ifndef SK_CPU_LSX_LEVEL
    #if defined(__loongarch_asx)
        #define SK_CPU_LSX_LEVEL    SK_CPU_LSX_LEVEL_LASX
    #elif defined(__loongarch_sx)
        #define SK_CPU_LSX_LEVEL    SK_CPU_LSX_LEVEL_LSX
    #endif
#endif

// Are we in VisualStudio?
#ifndef SK_CPU_X64_LEVEL
    // These checks must be done in descending order to ensure we set the highest
    // available SSE level. 64-bit intel guarantees at least SSE2 support.
    #if defined(__AVX512F__) && defined(__AVX512DQ__) && defined(__AVX512CD__) && \
        defined(__AVX512BW__) && defined(__AVX512VL__)
        #define SK_CPU_X64_LEVEL        SK_CPU_X64_LEVEL_ML4
    #elif defined(__AVX2__)
        #define SK_CPU_X64_LEVEL        SK_CPU_X64_LEVEL_AVX2
    #elif defined(__AVX__)
        #define SK_CPU_X64_LEVEL        SK_CPU_X64_LEVEL_AVX
    #elif defined(_M_X64) || defined(_M_AMD64)
        #define SK_CPU_X64_LEVEL        SK_CPU_X64_LEVEL_SSE2
    #elif defined(_M_IX86_FP)
        #if _M_IX86_FP >= 2
            #define SK_CPU_X64_LEVEL    SK_CPU_X64_LEVEL_SSE2
        #elif _M_IX86_FP == 1
            #define SK_CPU_X64_LEVEL    SK_CPU_X64_LEVEL_SSE1
        #endif
    #endif
#endif

// ARM defines
#if defined(__arm__) && (!defined(__APPLE__) || !TARGET_IPHONE_SIMULATOR)
    #define SK_CPU_ARM32
#elif defined(__aarch64__)
    #define SK_CPU_ARM64
#endif

// All 64-bit ARM chips have NEON.  Many 32-bit ARM chips do too.
#if !defined(SK_ARM_HAS_NEON) && defined(__ARM_NEON)
    #define SK_ARM_HAS_NEON
#endif


//!<===================================
//!< Load User Config
//!<-----------------------------------
//!< Copied from SkLoadUserConfig.h
//!<===================================

#ifndef SK_USER_CONFIG_WAS_LOADED

/**
 * SKIA_LOWEST_ACTIVE_LOG_PRIORITY can be defined to one of these values (in
 * SkUserConfig.h) to control Skia's logging behavior.
 *
 * For example:
 * ```
 * #define SKIA_LOWEST_ACTIVE_LOG_PRIORITY VX_LOG_PRIORITY_WARNING
 * ```
 * Would cause Skia to log warnings, non-fatal errors, and fatal errors.
 * However, debug logs would be omitted.
 */
enum vx_log_priority {
    VX_LOG_PRIORITY_ERROR = 0,
    VX_LOG_PRIORITY_WARNING = 1,
    VX_LOG_PRIORITY_INFO = 2,
    VX_LOG_PRIORITY_DEBUG = 3,
};

// Compat.
#define SkLogPriority       vx_log_priority

// Allows embedders that want to disable macros that take arguments to just
// define that symbol to be one of these
#define SK_NOTHING_ARG1(arg1)
#define SK_NOTHING_ARG2(arg1, arg2)
#define SK_NOTHING_ARG3(arg1, arg2, arg3)

// IWYU pragma: begin_exports
// Note: SK_USER_CONFIG_HEADER will not work with Bazel builds and some C++ compilers.
#if defined(SK_USER_CONFIG_HEADER)
    #include SK_USER_CONFIG_HEADER
#elif defined(SK_USE_BAZEL_CONFIG_HEADER)
    // The Bazel config file is presumed to be in the root directory of its Bazel Workspace.
    // This is achieved in Skia by having a nested WORKSPACE in include/config and a cc_library
    // defined in that folder. As a result, we do not try to include SkUserConfig.h from the
    // top of Skia because Bazel sandboxing will move it to a different location.
    #include "SkUserConfig.h"  // NO_G3_REWRITE
#else
    // All definitions in "include/config/SkUserConfig.h" are commented out.
    // #include "include/config/SkUserConfig.h"
#endif
// IWYU pragma: end_exports

// Checks to make sure the SkUserConfig options do not conflict.
#if !defined(VX_DEBUG) && !defined(VX_RELEASE)
    #ifdef NDEBUG
        #define VX_RELEASE
    #else
        #define VX_DEBUG
    #endif
#endif

#if defined(VX_DEBUG) && defined(VX_RELEASE)
#  error "cannot define both SK_DEBUG and SK_RELEASE"
#elif !defined(VX_DEBUG) && !defined(VX_RELEASE)
#  error "must define either SK_DEBUG or SK_RELEASE"
#endif

#if defined(SK_CPU_LENDIAN) && defined(SK_CPU_BENDIAN)
#  error "cannot define both SK_CPU_LENDIAN and SK_CPU_BENDIAN"
#elif !defined(SK_CPU_LENDIAN) && !defined(SK_CPU_BENDIAN)
#  error "must define either SK_CPU_LENDIAN or SK_CPU_BENDIAN"
#endif

#if defined(SK_CPU_BENDIAN) && !defined(I_ACKNOWLEDGE_SKIA_DOES_NOT_SUPPORT_BIG_ENDIAN)
    #error "The Skia team is not endian-savvy enough to support big-endian CPUs."
    #error "If you still want to use Skia,"
    #error "please define I_ACKNOWLEDGE_SKIA_DOES_NOT_SUPPORT_BIG_ENDIAN."
#endif

#define SK_USER_CONFIG_WAS_LOADED
#endif // SK_USER_CONFIG_WAS_LOADED


//!<=============================
//!< Assert
//!<-----------------------------
//!< Copied from SkAssert.h
//!<=============================

#if !defined(SkUNREACHABLE)
#  if defined(_MSC_VER) && !defined(__clang__)
#    include <intrin.h>
#    define FAST_FAIL_INVALID_ARG                 5
// See https://developercommunity.visualstudio.com/content/problem/1128631/code-flow-doesnt-see-noreturn-with-extern-c.html
// for why this is wrapped. Hopefully removable after msvc++ 19.27 is no longer supported.
[[noreturn]] static inline void sk_fast_fail() { __fastfail(FAST_FAIL_INVALID_ARG); }
#    define SkUNREACHABLE sk_fast_fail()
#  else
#    define SkUNREACHABLE __builtin_trap()
#  endif
#endif

#if defined(__clang__) && defined(__has_attribute)
    #if __has_attribute(likely)
        #define VX_LIKELY [[likely]]
        #define VX_UNLIKELY [[unlikely]]
    #else
        #define VX_LIKELY
        #define VX_UNLIKELY
    #endif
#else
    #define VX_LIKELY
    #define VX_UNLIKELY
#endif

// c++23 will give us [[assume]] -- until then we're stuck with various other options:
#if defined(__clang__)
    #define VX_ASSUME(cond) __builtin_assume(cond)
#elif defined(__GNUC__)
    #if __GNUC__ >= 13
        #define VX_ASSUME(cond) __attribute__((assume(cond)))
    #else
        // NOTE: This implementation could actually evaluate `cond`, which is not desirable.
        #define VX_ASSUME(cond) ((cond) ? (void)0 : __builtin_unreachable())
    #endif
#elif defined(_MSC_VER)
    #define VX_ASSUME(cond) __assume(cond)
#else
    #define VX_ASSUME(cond) ((void)0)
#endif

#ifndef SKIA_LOG_E
/// TODO: Fix no-op.
#define SKIA_LOG_E(args, ...)
#endif

#if !defined(VX_ABORT)
#  if defined(SK_BUILD_FOR_WIN)
     // This style lets Visual Studio follow errors back to the source file.
#    define SK_DUMP_LINE_FORMAT "%s(%d)"
#  else
#    define SK_DUMP_LINE_FORMAT "%s:%d"
#  endif
#  define VX_ABORT(message, ...) \
    do { \
        SKIA_LOG_E(SK_DUMP_LINE_FORMAT ": fatal error: \"" message "\"", \
                 __FILE__, __LINE__, ##__VA_ARGS__); \
        vx_abort_no_print(); \
    } while (false)
#endif

// SkASSERT, SkASSERTF and SkASSERT_RELEASE can be used as standalone assertion expressions, e.g.
//    uint32_t foo(int x) {
//        SkASSERT(x > 4);
//        return x - 4;
//    }
// and are also written to be compatible with constexpr functions:
//    constexpr uint32_t foo(int x) {
//        return SkASSERT(x > 4),
//               x - 4;
//    }
#if defined(__clang__)
#define VX_ASSERT_RELEASE(cond) \
    static_cast<void>( __builtin_expect(static_cast<bool>(cond), 1) \
        ? static_cast<void>(0) \
        : []{ VX_ABORT("check(%s)", #cond); }() )

#define VX_ASSERTF_RELEASE(cond, fmt, ...)                                  \
    static_cast<void>( __builtin_expect(static_cast<bool>(cond), 1)        \
        ? static_cast<void>(0)                                             \
        : [&]{ VX_ABORT("assertf(%s): " fmt, #cond, ##__VA_ARGS__); }() )
#else
#define VX_ASSERT_RELEASE(cond) \
    static_cast<void>( (cond) ? static_cast<void>(0) : []{ VX_ABORT("check(%s)", #cond); }() )

#define VX_ASSERTF_RELEASE(cond, fmt, ...)                                   \
    static_cast<void>( (cond)                                               \
        ? static_cast<void>(0)                                              \
        : [&]{ VX_ABORT("assertf(%s): " fmt, #cond, ##__VA_ARGS__); }() )
#endif

#if defined(VX_DEBUG)
    #define VX_ASSERT(cond)            VX_ASSERT_RELEASE(cond)
    #define VX_ASSERTF(cond, fmt, ...) VX_ASSERTF_RELEASE(cond, fmt, ##__VA_ARGS__)
    #define VX_DEBUGFAIL(message)      VX_ABORT("%s", message)
    #define VX_DEBUGFAILF(fmt, ...)    VX_ABORT(fmt, ##__VA_ARGS__)
    #define VXAssertResult(cond)      VX_ASSERT(cond)
#else
    #define VX_ASSERT(cond)             ((void)0)
    #define VX_ASSERTF(cond, fmt, ...)  ((void)0)
    #define VX_DEBUGFAIL(message)
    #define VX_DEBUGFAILF(fmt, ...)

    // unlike SkASSERT, this macro executes its condition in the non-debug build.
    // The if is present so that this can be used with functions marked [[nodiscard]].
    #define VXAssertResult(cond)         if (cond) {} do {} while(false)
#endif
#define SK_DEBUG VX_DEBUG

#define SkASSERT                        VX_ASSERT
#define SkASSERTF                       VX_ASSERTF
#define SkASSERT_RELEASE                VX_ASSERT_RELEASE
#define SkASSERTF_RELEASE               VX_ASSERTF_RELEASE
#define SK_UNLIKELY                     VX_UNLIKELY
#define SK_LIKELY                       VX_LIKELY
#define SkAssertResult                  VXAssertResult
#define SK_ABORT                        VX_ABORT
#define SkDEBUGFAIL                     VX_DEBUGFAIL
#define SkDEBUGFAILF                    VX_DEBUGFAILF
#define SK_ASSUME                       VX_ASSUME


//!<=============================
//!< Debug
//!<-----------------------------
//!< Copied from SkDebug.h
//!<=============================

#if !defined(vx_debugf)
    void VX_SPI vx_debugf(const char format[], ...) VX_PRINTF_LIKE(1, 2);
#endif

#if defined(VX_DEBUG)
    #define VX_DEBUGCODE(...)  __VA_ARGS__
    #define VX_DEBUGF(...)     vx_debugf(__VA_ARGS__)
#else
    #define VX_DEBUGCODE(...)
    #define VX_DEBUGF(...)
#endif

#define SkDEBUGCODE             VX_DEBUGCODE
#define SkDebugf                vx_debugf
#define SkDEBUGF                VX_DEBUGF

#ifdef __cplusplus
extern "C" {
#endif // __cpluspluss

/** Called internally if we hit an unrecoverable error.
    The platform implementation must not return, but should either throw
    an exception or otherwise exit.
*/
[[noreturn]] VX_PUBLIC void vx_abort_no_print(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* VIVIDX_ASSERT_H */
