#include <vividx/assert.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void vx_abort_no_print(void)
{
#if defined(VX_DEBUG) && defined(SK_BUILD_FOR_WIN)
    __fastfail(FAST_FAIL_FATAL_APP_EXIT);
#elif defined(__clang__)
    __builtin_trap();
#else
    abort();
#endif
}

#ifdef __cplusplus
}
#endif // __cplusplus
