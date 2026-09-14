#ifndef VIVIDX_CORE_SIZE_H
#define VIVIDX_CORE_SIZE_H

#include <vividx/common.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct vx_size_t vx_size_t;
typedef struct vx_size_i_t vx_size_i_t;

struct vx_size_i_t {
    int32_t width;
    int32_t height;
};

static vx_size_i_t vx_size_i_make(int32_t w, int32_t h)
{
    vx_size_i_t s = { .width = w, .height = h };
    return s;
}

/** Returns true iff fWidth == 0 && fHeight == 0
    */
static bool vx_size_i_is_zero(vx_size_i_t size)
{
    return 0 == size.width && 0 == size.height;
}

/** Returns true if either width or height are <= 0 */
static bool vx_size_i_is_empty(vx_size_i_t size)
{
    return size.width <= 0 || size.height <= 0;
}

static int32_t vx_size_i_width(vx_size_i_t size)
{
    return size.width;
}

static int32_t vx_size_i_height(vx_size_i_t size)
{
    return size.height;
}

#if 0
    int64_t area() { return SkToS64(fWidth) * SkToS64(fHeight); }

    bool equals(int32_t w, int32_t h) { return fWidth == w && fHeight == h; }
#endif

static inline bool vx_size_i_eq(vx_size_i_t a, vx_size_i_t b)
{
    return a.width == b.width && a.height == b.height;
}

static inline bool vx_size_i_ne(vx_size_i_t a, vx_size_i_t b)
{
    return !vx_size_i_eq(a, b);
}

///////////////////////////////////////////////////////////////////////////////

struct vx_size_t {
    float width;
    float height;
};

static vx_size_t vx_size_make(float w, float h)
{
    vx_size_t size = { .width = w, .height = h };
    return size;
}

#if 0
static vx_size_t vx_size_Make(const SkISize& src) {
    return {SkIntToScalar(src.width()), SkIntToScalar(src.height())};
}
#endif

/** Returns true iff fWidth == 0 && fHeight == 0
    */
static bool vx_size_is_zero(vx_size_t size)
{
    return 0 == size.width && 0 == size.height;
}

/** Returns true if either width or height are <= 0 */
static bool vx_size_is_empty(vx_size_t size)
{
    return size.width <= 0 || size.height <= 0;
}

static float vx_size_width(vx_size_t size)
{
    return size.width;
}
static float vx_size_height(vx_size_t size)
{
    return size.height;
}

#if 0
static vx_size_t vx_size_to_round(vx_size_t size)
{
    return {SkScalarRoundToInt(fWidth), SkScalarRoundToInt(fHeight)};
}

static vx_size_t vx_size_to_ceil(vx_size_t size)
{
    return {SkScalarCeilToInt(fWidth), SkScalarCeilToInt(fHeight)};
}

static vx_size_t vx_size_to_floor(vx_size_t size)
{
    return {SkScalarFloorToInt(fWidth), SkScalarFloorToInt(fHeight)};
}
#endif

static inline bool vx_size_eq(vx_size_t a, vx_size_t b)
{
    return a.width == b.width && a.height == b.height;
}

static inline bool vx_size_ne(vx_size_t a, vx_size_t b)
{
    return !vx_size_eq(a, b);
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* VIVIDX_CORE_SIZE_H */
