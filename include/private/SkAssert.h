/*
 * Copyright 2022 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkAssert_DEFINED
#define SkAssert_DEFINED

#include <vividx/common.h>
// #include <vividx/assert.h> // IWYU pragma: keep
#include "include/private/SkLog.h"

#include <cstddef>
#include <limits>

#include <vividx/assert.h>

[[noreturn]] SK_API inline void sk_print_index_out_of_bounds(size_t i, size_t size) {
    VX_ABORT("Index (%zu) out of bounds for size %zu.", i, size);
}

template <typename T> SK_API inline T sk_collection_check_bounds(T i, T size) {
    if (0 <= i && i < size) SK_LIKELY {
        return i;
    }

    SK_UNLIKELY {
        #if defined(SK_DEBUG)
            sk_print_index_out_of_bounds(static_cast<size_t>(i), static_cast<size_t>(size));
        #else
            SkUNREACHABLE;
        #endif
    }
}

[[noreturn]] SK_API inline void sk_print_length_too_big(size_t i, size_t size) {
    VX_ABORT("Length (%zu) is too big for size %zu.", i, size);
}

template <typename T> SK_API inline T sk_collection_check_length(T i, T size) {
    if (0 <= i && i <= size) SK_LIKELY {
        return i;
    }

    SK_UNLIKELY {
        #if defined(SK_DEBUG)
            sk_print_length_too_big(static_cast<size_t>(i), static_cast<size_t>(size));
        #else
            SkUNREACHABLE;
        #endif
    }
}

SK_API inline void sk_collection_not_empty(bool empty) {
    if (empty) SK_UNLIKELY {
        #if defined(SK_DEBUG)
            VX_ABORT("Collection is empty.");
        #else
            SkUNREACHABLE;
        #endif
    }
}

[[noreturn]] SK_API inline void sk_print_size_too_big(size_t size, size_t maxSize) {
    VX_ABORT("Size (%zu) can't be represented in bytes. Max size is %zu.", size, maxSize);
}

template <typename T>
SK_ALWAYS_INLINE size_t check_size_bytes_too_big(size_t size) {
    const size_t kMaxSize = std::numeric_limits<size_t>::max() / sizeof(T);
    if (size > kMaxSize) {
        #if defined(SK_DEBUG)
            sk_print_size_too_big(size, kMaxSize);
        #else
            SkUNREACHABLE;
        #endif
    }
    return size;
}

#endif  // SkAssert_DEFINED
