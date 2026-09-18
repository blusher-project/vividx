#ifndef VIVIDX_ASSERT_H
#define VIVIDX_ASSERT_H

#include <vividx/common.h>

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
#define SKIA_LOG_E(args, ...) void(args)
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
#define SkASSERT_RELEASE(cond) \
    static_cast<void>( __builtin_expect(static_cast<bool>(cond), 1) \
        ? static_cast<void>(0) \
        : []{ VX_ABORT("check(%s)", #cond); }() )

#define SkASSERTF_RELEASE(cond, fmt, ...)                                  \
    static_cast<void>( __builtin_expect(static_cast<bool>(cond), 1)        \
        ? static_cast<void>(0)                                             \
        : [&]{ VX_ABORT("assertf(%s): " fmt, #cond, ##__VA_ARGS__); }() )
#else
#define SkASSERT_RELEASE(cond) \
    static_cast<void>( (cond) ? static_cast<void>(0) : []{ VX_ABORT("check(%s)", #cond); }() )

#define SkASSERTF_RELEASE(cond, fmt, ...)                                   \
    static_cast<void>( (cond)                                               \
        ? static_cast<void>(0)                                              \
        : [&]{ VX_ABORT("assertf(%s): " fmt, #cond, ##__VA_ARGS__); }() )
#endif

#define VX_DEBUG SK_DEBUG
#if defined(VX_DEBUG)
    #define SkASSERT(cond)            SkASSERT_RELEASE(cond)
    #define SkASSERTF(cond, fmt, ...) SkASSERTF_RELEASE(cond, fmt, ##__VA_ARGS__)
    #define SkDEBUGFAIL(message)      VX_ABORT("%s", message)
    #define SkDEBUGFAILF(fmt, ...)    VX_ABORT(fmt, ##__VA_ARGS__)
    #define SkAssertResult(cond)      SkASSERT(cond)
#else
    #define SkASSERT(cond)            static_cast<void>(0)
    #define SkASSERTF(cond, fmt, ...) static_cast<void>(0)
    #define SkDEBUGFAIL(message)
    #define SkDEBUGFAILF(fmt, ...)

    // unlike SkASSERT, this macro executes its condition in the non-debug build.
    // The if is present so that this can be used with functions marked [[nodiscard]].
    #define SkAssertResult(cond)         if (cond) {} do {} while(false)
#endif

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
