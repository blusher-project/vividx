#ifndef VIVIDX_CORE_RECT_H
#define VIVIDX_CORE_RECT_H

#include <stdbool.h>
#include <stdint.h>

#include <vividx/common.h>
#include <vividx/core/point.h>
#include <vividx/core/size.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/// \struct vx_rect_t
///
/// vx_rect_t holds four float coordinates describing the upper and
/// lower bounds of a rectangle. vx_rect_t may be created from outer bounds or
/// from position, width, and height. vx_rect_t describes an area; if its right
/// is less than or equal to its left, or if its bottom is less than or equal to
/// its top, it is considered empty.
typedef struct vx_rect_t vx_rect_t;

/// \struct vx_rect_i
///
/// SkIRect holds four 32-bit integer coordinates describing the upper and
/// lower bounds of a rectangle. SkIRect may be created from outer bounds or
/// from position, width, and height. SkIRect describes an area; if its right
/// is less than or equal to its left, or if its bottom is less than or equal to
/// its top, it is considered empty.
typedef struct vx_rect_i_t vx_rect_i_t;

//!<==================
//!< Integer Rect
//!<==================

struct vx_rect_i_t {
    int32_t left;   //!< smaller x-axis bounds
    int32_t top;    //!< smaller y-axis bounds
    int32_t right;  //!< larger x-axis bounds
    int32_t bottom; //!< larger y-axis bounds
};

/** Returns constructed SkIRect set to (0, 0, 0, 0).
    Many other rectangles are empty; if left is equal to or greater than right,
    or if top is equal to or greater than bottom. Setting all members to zero
    is a convenience, but does not designate a special empty rectangle.

    @return  bounds (0, 0, 0, 0)
*/
[[nodiscard]] static vx_rect_i_t vx_rect_i_make()
{
    vx_rect_i_t rect = { .left = 0, .top = 0, .right = 0, .bottom = 0 };
    return rect;
}

/** Returns constructed SkIRect set to: (x, y, x + w, y + h).
    Does not validate input; w or h may be negative.

    @param x  stored in fLeft
    @param y  stored in fTop
    @param w  added to x and stored in fRight
    @param h  added to y and stored in fBottom
    @return   bounds at (x, y) with width w and height h
*/
[[nodiscard]] static vx_rect_i_t vx_rect_i_make_xywh(int32_t x,
                                                     int32_t y,
                                                     int32_t w,
                                                     int32_t h)
{
    vx_rect_i_t rect = {
        .left = x,
        .top = y,
        .right = vx_int32_sat_add(x, w),
        .bottom = vx_int32_sat_add(y, h),
    };
    return rect;
}

/** Returns constructed SkIRect set to (0, 0, w, h). Does not validate input; w or h
    may be negative.

    @param w  width of constructed SkIRect
    @param h  height of constructed SkIRect
    @return   bounds (0, 0, w, h)
*/
[[nodiscard]] VX_PUBLIC vx_rect_i_t vx_rect_i_make_wh(int32_t w, int32_t h);

/** Returns constructed SkIRect set to (0, 0, size.width(), size.height()).
    Does not validate input; size.width() or size.height() may be negative.

    @param size  values for SkIRect width and height
    @return      bounds (0, 0, size.width(), size.height())
*/
[[nodiscard]] VX_PUBLIC vx_rect_i_t vx_rect_i_make_from_size(vx_size_i_t size);

/** Returns constructed SkIRect set to (l, t, r, b). Does not sort input; SkIRect may
    result in fLeft greater than fRight, or fTop greater than fBottom.

    @param l  integer stored in fLeft
    @param t  integer stored in fTop
    @param r  integer stored in fRight
    @param b  integer stored in fBottom
    @return   bounds (l, t, r, b)
*/
[[nodiscard]] VX_PUBLIC vx_rect_i_t vx_rect_i_make_ltrb(int32_t l,
                                                        int32_t t,
                                                        int32_t r,
                                                        int32_t b);


/** Returns left edge of SkIRect, if sorted.
    Call sort() to reverse fLeft and fRight if needed.

    @return  fLeft
*/
static int32_t vx_rect_i_left(vx_rect_i_t rect)
{
    return rect.left;
}

/** Returns top edge of SkIRect, if sorted. Call isEmpty() to see if SkIRect may be invalid,
    and sort() to reverse fTop and fBottom if needed.

    @return  fTop
*/
static int32_t vx_rect_i_top(vx_rect_i_t rect)
{
    return rect.top;
}

/** Returns right edge of SkIRect, if sorted.
    Call sort() to reverse fLeft and fRight if needed.

    @return  fRight
*/
static int32_t vx_rect_i_right(vx_rect_i_t rect)
{
    return rect.right;
}

/** Returns bottom edge of SkIRect, if sorted. Call isEmpty() to see if SkIRect may be invalid,
    and sort() to reverse fTop and fBottom if needed.

    @return  fBottom
*/
static int32_t vx_rect_i_bottom(vx_rect_i_t rect)
{
    return rect.bottom;
}

/** Returns left edge of SkIRect, if sorted. Call isEmpty() to see if SkIRect may be invalid,
    and sort() to reverse fLeft and fRight if needed.

    @return  fLeft
*/
static int32_t vx_rect_i_x(vx_rect_i_t rect)
{
    return rect.left;
}

/** Returns top edge of SkIRect, if sorted. Call isEmpty() to see if SkIRect may be invalid,
    and sort() to reverse fTop and fBottom if needed.

    @return  fTop
*/
static int32_t vx_rect_i_y(vx_rect_i_t rect)
{
    return rect.top;
}

// Experimental
static vx_point_i_t vx_rect_i_top_left(vx_rect_i_t rect)
{
    return vx_point_i_make(rect.left, rect.top);
}

/** Returns span on the x-axis. This does not check if SkIRect is sorted, or if
    result fits in 32-bit signed integer; result may be negative.

    @return  fRight minus fLeft
*/
static int32_t vx_rect_i_width(vx_rect_i_t rect)
{
    return vx_int32_can_overflow_sub(rect.right, rect.left);
}

/** Returns span on the y-axis. This does not check if SkIRect is sorted, or if
    result fits in 32-bit signed integer; result may be negative.

    @return  fBottom minus fTop
*/
static int32_t vx_rect_i_height(vx_rect_i_t rect)
{
    return vx_int32_can_overflow_sub(rect.bottom, rect.top);
}

/** Returns spans on the x-axis and y-axis. This does not check if SkIRect is sorted,
    or if result fits in 32-bit signed integer; result may be negative.

    @return  SkISize (width, height)
*/
static vx_size_i_t vx_rect_i_size(vx_rect_i_t rect)
{
    return vx_size_i_make(vx_rect_i_width(rect), vx_rect_i_height(rect));
}

/** Returns span on the x-axis. This does not check if SkIRect is sorted, so the
    result may be negative. This is safer than calling width() since width() might
    overflow in its calculation.

    @return  fRight minus fLeft cast to int64_t
*/
static int64_t vx_rect_i_width64(vx_rect_i_t rect)
{
    return (int64_t)rect.right - (int64_t)rect.left;
}

/** Returns span on the y-axis. This does not check if SkIRect is sorted, so the
    result may be negative. This is safer than calling height() since height() might
    overflow in its calculation.

    @return  fBottom minus fTop cast to int64_t
*/
static int64_t vx_rect_i_height64(vx_rect_i_t rect)
{
    return (int64_t)rect.bottom - (int64_t)rect.top;
}

#if 0
/** Returns true if fLeft is equal to or greater than fRight, or if fTop is equal
    to or greater than fBottom. Call sort() to reverse rectangles with negative
    width64() or height64().

    @return  true if width64() or height64() are zero or negative
*/
bool isEmpty64() const { return fRight <= fLeft || fBottom <= fTop; }
#endif

/// Returns true if width() or height() are zero or negative.
///
/// \return true if width() or height() are zero or negative
VX_PUBLIC
bool vx_rect_i_is_empty(vx_rect_i_t rect);

/** Returns true if all members in a: fLeft, fTop, fRight, and fBottom; are
    identical to corresponding members in b.

    @param a  SkIRect to compare
    @param b  SkIRect to compare
    @return   true if members are equal
*/
static bool vx_rect_i_eq(vx_rect_i_t a, vx_rect_i_t b) {
    return a.left == b.left && a.top == b.top &&
            a.right == b.right && a.bottom == b.bottom;
}

/** Returns true if any member in a: fLeft, fTop, fRight, and fBottom; is not
    identical to the corresponding member in b.

    @param a  SkIRect to compare
    @param b  SkIRect to compare
    @return   true if members are not equal
*/
static bool vx_rect_i_ne(vx_rect_i_t a, vx_rect_i_t b) {
    return a.left != b.left || a.top != b.top ||
            a.right != b.right || a.bottom != b.bottom;
}

#if 0
/** Sets SkIRect to (0, 0, 0, 0).

    Many other rectangles are empty; if left is equal to or greater than right,
    or if top is equal to or greater than bottom. Setting all members to zero
    is a convenience, but does not designate a special empty rectangle.
*/
void setEmpty() { memset(this, 0, sizeof(*this)); }

/** Sets SkIRect to (left, top, right, bottom).
    left and right are not sorted; left is not necessarily less than right.
    top and bottom are not sorted; top is not necessarily less than bottom.

    @param left    stored in fLeft
    @param top     stored in fTop
    @param right   stored in fRight
    @param bottom  stored in fBottom
*/
void setLTRB(int32_t left, int32_t top, int32_t right, int32_t bottom) {
    fLeft   = left;
    fTop    = top;
    fRight  = right;
    fBottom = bottom;
}

/** Sets SkIRect to: (x, y, x + width, y + height).
    Does not validate input; width or height may be negative.

    @param x       stored in fLeft
    @param y       stored in fTop
    @param width   added to x and stored in fRight
    @param height  added to y and stored in fBottom
*/
void setXYWH(int32_t x, int32_t y, int32_t width, int32_t height) {
    fLeft   = x;
    fTop    = y;
    fRight  = vx_int32_sat_add(x, width);
    fBottom = vx_int32_sat_add(y, height);
}

void setWH(int32_t width, int32_t height) {
    fLeft   = 0;
    fTop    = 0;
    fRight  = width;
    fBottom = height;
}

void setSize(SkISize size) {
    fLeft = 0;
    fTop = 0;
    fRight = size.width();
    fBottom = size.height();
}
#endif

/** Returns SkIRect offset by (dx, dy).

    If dx is negative, SkIRect returned is moved to the left.
    If dx is positive, SkIRect returned is moved to the right.
    If dy is negative, SkIRect returned is moved upward.
    If dy is positive, SkIRect returned is moved downward.

    \param dx  offset added to fLeft and fRight
    \param dy  offset added to fTop and fBottom
    \return    SkIRect offset by dx and dy, with original width and height
*/
VX_PUBLIC vx_rect_i_t vx_rect_i_offset(vx_rect_i_t rect,
                                       int32_t dx,
                                       int32_t dy);

/** Returns SkIRect offset by (offset.x(), offset.y()).

    If offset.x() is negative, SkIRect returned is moved to the left.
    If offset.x() is positive, SkIRect returned is moved to the right.
    If offset.y() is negative, SkIRect returned is moved upward.
    If offset.y() is positive, SkIRect returned is moved downward.

    @param offset  translation vector
    @return    SkIRect translated by offset, with original width and height
*/
static inline vx_rect_i_t vx_rect_i_offset_from_vector(vx_rect_i_t rect,
                                                       vx_vector_i_t offset)
{
    return vx_rect_i_offset(rect, offset.x, offset.y);
}

/** Returns SkIRect, inset by (dx, dy).

    If dx is negative, SkIRect returned is wider.
    If dx is positive, SkIRect returned is narrower.
    If dy is negative, SkIRect returned is taller.
    If dy is positive, SkIRect returned is shorter.

    @param dx  offset added to fLeft and subtracted from fRight
    @param dy  offset added to fTop and subtracted from fBottom
    @return    SkIRect inset symmetrically left and right, top and bottom
*/
VX_PUBLIC vx_rect_i_t vx_rect_i_inset(vx_rect_i_t r, int32_t dx, int32_t dy);

/** Returns SkIRect, outset by (dx, dy).

    If dx is negative, SkIRect returned is narrower.
    If dx is positive, SkIRect returned is wider.
    If dy is negative, SkIRect returned is shorter.
    If dy is positive, SkIRect returned is taller.

    @param dx  offset subtracted to fLeft and added from fRight
    @param dy  offset subtracted to fTop and added from fBottom
    @return    SkIRect outset symmetrically left and right, top and bottom
*/
VX_PUBLIC vx_rect_i_t vx_rect_i_outset(vx_rect_i_t r, int32_t dx, int32_t dy);

/** Offsets SkIRect so that fLeft equals newX, and fTop equals newY. width and height
    are unchanged.

    @param newX  stored in fLeft, preserving width()
    @param newY  stored in fTop, preserving height()
*/
VX_PUBLIC vx_rect_i_t vx_rect_i_offset_to(vx_rect_i_t r,
                                          int32_t newX,
                                          int32_t newY);

/** Adjusts SkIRect by adding dL to fLeft, dT to fTop, dR to fRight, and dB to fBottom.

    If dL is positive, narrows SkIRect on the left. If negative, widens it on the left.
    If dT is positive, shrinks SkIRect on the top. If negative, lengthens it on the top.
    If dR is positive, narrows SkIRect on the right. If negative, widens it on the right.
    If dB is positive, shrinks SkIRect on the bottom. If negative, lengthens it on the bottom.

    The resulting SkIRect is not checked for validity. Thus, if the resulting SkIRect left is
    greater than right, the SkIRect will be considered empty. Call sort() after this call
    if that is not the desired behavior.

    @param dL  offset added to fLeft
    @param dT  offset added to fTop
    @param dR  offset added to fRight
    @param dB  offset added to fBottom
*/
VX_PUBLIC vx_rect_i_t vx_rect_i_adjusted(vx_rect_i_t r,
                                         int32_t dl,
                                         int32_t dt,
                                         int32_t dr,
                                         int32_t db);

/** Returns true if: fLeft <= x < fRight && fTop <= y < fBottom.
    Returns false if SkIRect is empty.

    Considers input to describe constructed SkIRect: (x, y, x + 1, y + 1) and
    returns true if constructed area is completely enclosed by SkIRect area.

    @param x  test SkIPoint x-coordinate
    @param y  test SkIPoint y-coordinate
    @return   true if (x, y) is inside SkIRect
*/
static bool vx_rect_i_contains_xy(vx_rect_i_t rect, int32_t x, int32_t y)
{
    return x >= rect.left && x < rect.right && y >= rect.top && y < rect.bottom;
}

/** Returns true if SkIRect contains r.
    Returns false if SkIRect is empty or r is empty.

    SkIRect contains r when SkIRect area completely includes r area.

    @param r  SkIRect contained
    @return   true if all sides of SkIRect are outside r
    */
static bool vx_rect_i_contains(vx_rect_i_t lhs, vx_rect_i_t rhs)
{
    return !vx_rect_i_is_empty(rhs) && !vx_rect_i_is_empty(lhs) &&     // check for empties
        lhs.left <= rhs.left && lhs.top <= rhs.top &&
        lhs.right >= rhs.right && lhs.bottom >= rhs.bottom;
}

VX_PUBLIC
bool vx_rect_i_contains_rect(vx_rect_i_t lhs, vx_rect_t rhs);

#if 0
/** Returns true if SkIRect contains construction.
    Asserts if SkIRect is empty or construction is empty, and if SK_DEBUG is defined.

    Return is undefined if SkIRect is empty or construction is empty.

    @param r  SkIRect contained
    @return   true if all sides of SkIRect are outside r
*/
bool containsNoEmptyCheck(const SkIRect& r) const {
    SkASSERT(fLeft < fRight && fTop < fBottom);
    SkASSERT(r.fLeft < r.fRight && r.fTop < r.fBottom);
    return fLeft <= r.fLeft && fTop <= r.fTop && fRight >= r.fRight && fBottom >= r.fBottom;
}
#endif

/** Returns true if a intersects b, and sets SkIRect to intersection.
    Returns false if a does not intersect b, and leaves SkIRect unchanged.

    Returns false if either a or b is empty, leaving SkIRect unchanged.

    @param a  SkIRect to intersect
    @param b  SkIRect to intersect
    @return   true if a and b have area in common
*/
[[nodiscard]]
VX_PUBLIC
vx_rect_i_t vx_rect_i_intersected(vx_rect_i_t a, vx_rect_i_t b);

/// \brief Check if `a` intersects `b`.
///
/// Returns true if a intersects b.
/// Returns false if either a or b is empty, or do not intersect.
///
/// \param a  vx_rect_i_t to intersect
/// \param b  vx_rect_i_t to intersect
/// \return   true if a and b have area in common
VX_PUBLIC
bool vx_rect_i_intersects(vx_rect_i_t a, vx_rect_i_t b);

/// Sets SkIRect to the union of itself and r.
///
/// Has no effect if r is empty. Otherwise, if SkIRect is empty, sets SkIRect to r.
///
/// \param r  expansion SkIRect
///
/// example: https://fiddle.skia.org/c/@IRect_join_2
VX_PUBLIC
vx_rect_i_t vx_rect_i_joined(vx_rect_i_t lhs, vx_rect_i_t rhs);

/** Returns SkIRect with fLeft and fRight swapped if fLeft is greater than fRight; and
    with fTop and fBottom swapped if fTop is greater than fBottom. Result may be empty;
    and width() and height() will be zero or positive.

    \return  sorted vx_rect_i_t
*/
VX_PUBLIC
vx_rect_i_t vx_rect_i_sorted(vx_rect_i_t rect);



//!<==================
//!< Float Rect
//!<==================

struct vx_rect_t {
    float left;     //!< smaller x-axis bounds
    float top;      //!< smaller y-axis bounds
    float right;    //!< larger x-axis bounds
    float bottom;   //!< larger y-axis bounds
};

/// \memberof vx_rect_t
///
/// Returns constructed vx_rect_t set to (0, 0, 0, 0).
/// Many other rectangles are empty; if left is equal to or greater than right,
/// or if top is equal to or greater than bottom. Setting all members to zero
/// is a convenience, but does not designate a special empty rectangle.
///
/// \return  bounds (0, 0, 0, 0)
[[nodiscard]] static vx_rect_t vx_rect_make()
{
    vx_rect_t rect = { .left = 0, .top = 0, .right = 0, .bottom = 0 };
    return rect;
}

/** Returns constructed SkRect set to (x, y, x + w, y + h).
    Does not validate input; w or h may be negative.

    @param x  stored in fLeft
    @param y  stored in fTop
    @param w  added to x and stored in fRight
    @param h  added to y and stored in fBottom
    @return   bounds at (x, y) with width w and height h
*/
[[nodiscard]] static vx_rect_t vx_rect_make_xywh(float x,
                                                 float y,
                                                 float w,
                                                 float h)
{
    vx_rect_t rect = { .left = x, .top = y, .right = x + w, .bottom = y + h};
    return rect;
}

/** Returns constructed SkRect set to (l, t, r, b). Does not sort input; SkRect may
    result in fLeft greater than fRight, or fTop greater than fBottom.

    @param l  float stored in fLeft
    @param t  float stored in fTop
    @param r  float stored in fRight
    @param b  float stored in fBottom
    @return   bounds (l, t, r, b)
*/
[[nodiscard]] static vx_rect_t vx_rect_make_ltrb(float l,
                                                 float t,
                                                 float r,
                                                 float b)
{
    vx_rect_t rect = { .left = l, .top = t, .right = r, .bottom = b };
    return rect;
}

/** Returns constructed SkRect set to float values (0, 0, w, h). Does not
    validate input; w or h may be negative.

    Passing integer values may generate a compiler warning since SkRect cannot
    represent 32-bit integers exactly. Use SkIRect for an exact integer rectangle.

    @param w  float width of constructed SkRect
    @param h  float height of constructed SkRect
    @return   bounds (0, 0, w, h)
*/
[[nodiscard]] static vx_rect_t vx_rect_make_wh(float w, float h) {
    return vx_rect_make_ltrb(0, 0, w, h);
}

#if 0
/** Returns constructed SkRect set to integer values (0, 0, w, h). Does not validate
    input; w or h may be negative.

    Use to avoid a compiler warning that input may lose precision when stored.
    Use SkIRect for an exact integer rectangle.

    @param w  integer width of constructed SkRect
    @param h  integer height of constructed SkRect
    @return   bounds (0, 0, w, h)
*/
[[nodiscard]] static vx_rect_t vx_rect_MakeIWH(int w, int h) {
    return {0, 0, static_cast<float>(w), static_cast<float>(h)};
}
#endif

/** Returns constructed SkRect set to (0, 0, size.width(), size.height()). Does not
    validate input; size.width() or size.height() may be negative.

    @param size  float values for SkRect width and height
    @return      bounds (0, 0, size.width(), size.height())
*/
[[nodiscard]] static vx_rect_t vx_rect_make_from_size(vx_size_t size) {
    vx_rect_t rect = {
        .left = 0, .top = 0,
        .right = size.width, .bottom = size.height
    };
    return rect;
}

#if 0
/** Returns constructed SkIRect set to (0, 0, size.width(), size.height()).
    Does not validate input; size.width() or size.height() may be negative.

    @param size  integer values for SkRect width and height
    @return      bounds (0, 0, size.width(), size.height())
*/
static SkRect Make(const SkISize& size) {
    return MakeIWH(size.width(), size.height());
}
#endif

/** Returns constructed SkIRect set to irect, promoting integers to float.
    Does not validate input; fLeft may be greater than fRight, fTop may be greater
    than fBottom.

    @param irect  integer unsorted bounds
    @return       irect members converted to float
*/
[[nodiscard]] static vx_rect_t vx_rect_make_from_rect_i(vx_rect_i_t rect_i) {
    vx_rect_t rect = {
        .left = (float)(rect_i.left),
        .top = (float)(rect_i.top),
        .right = (float)(rect_i.right),
        .bottom = (float)(rect_i.bottom),
    };
    return rect;
}

/** Returns true if fLeft is equal to or greater than fRight, or if fTop is equal
    to or greater than fBottom. Call sort() to reverse rectangles with negative
    width() or height().

    @return  true if width() or height() are zero or negative
*/
VX_PUBLIC
bool vx_rect_is_empty(vx_rect_t rect);

/** Returns true if fLeft is equal to or less than fRight, or if fTop is equal
    to or less than fBottom. Call sort() to reverse rectangles with negative
    width() or height().

    @return  true if width() or height() are zero or positive
*/
static bool vx_rect_is_sorted(vx_rect_t rect)
{
    return rect.left <= rect.right && rect.top <= rect.bottom;
}

/** Returns true if all values in the rectangle are finite.

    @return  true if no member is infinite or NaN
*/
static bool vx_rect_is_finite(vx_rect_t r)
{
    return vx_float_is_finite_4f(r.left, r.top, r.right, r.bottom);
}

/** Returns left edge of SkRect, if sorted. Call isSorted() to see if SkRect is valid.
    Call sort() to reverse fLeft and fRight if needed.

    @return  fLeft
*/
static float vx_rect_x(vx_rect_t rect)
{
    return rect.left;
}

/** Returns top edge of SkRect, if sorted. Call isEmpty() to see if SkRect may be invalid,
    and sort() to reverse fTop and fBottom if needed.

    @return  fTop
*/
static float vx_rect_y(vx_rect_t rect)
{
    return rect.top;
}

/** Returns left edge of SkRect, if sorted. Call isSorted() to see if SkRect is valid.
    Call sort() to reverse fLeft and fRight if needed.

    @return  fLeft
*/
static float vx_rect_left(vx_rect_t rect)
{
    return rect.left;
}

/** Returns top edge of SkRect, if sorted. Call isEmpty() to see if SkRect may be invalid,
    and sort() to reverse fTop and fBottom if needed.

    @return  fTop
*/
static float vx_rect_top(vx_rect_t rect)
{
    return rect.top;
}

/** Returns right edge of SkRect, if sorted. Call isSorted() to see if SkRect is valid.
    Call sort() to reverse fLeft and fRight if needed.

    @return  fRight
*/
static float vx_rect_right(vx_rect_t rect)
{
    return rect.right;
}

/** Returns bottom edge of SkRect, if sorted. Call isEmpty() to see if SkRect may be invalid,
    and sort() to reverse fTop and fBottom if needed.

    @return  fBottom
*/
static float vx_rect_bottom(vx_rect_t rect)
{
    return rect.bottom;
}

/** Returns span on the x-axis. This does not check if SkRect is sorted, or if
    result fits in 32-bit float; result may be negative or infinity.

    @return  fRight minus fLeft
*/
static float vx_rect_width(vx_rect_t rect)
{
    return rect.right - rect.left;
}

/** Returns span on the y-axis. This does not check if SkRect is sorted, or if
    result fits in 32-bit float; result may be negative or infinity.

    @return  fBottom minus fTop
*/
static float vx_rect_height(vx_rect_t rect)
{
    return rect.bottom - rect.top;
}

/** Returns average of left edge and right edge. Result does not change if SkRect
    is sorted. Result may overflow to infinity if SkRect is far from the origin.

    @return  midpoint on x-axis
*/
static float vx_rect_center_x(vx_rect_t r)
{
    return vx_float_midpoint(r.left, r.right);
}

/** Returns average of top edge and bottom edge. Result does not change if SkRect
    is sorted.

    @return  midpoint on y-axis
*/
static float vx_rect_center_y(vx_rect_t r)
{
    return vx_float_midpoint(r.top, r.bottom);
}

/** Returns the point this->centerX(), this->centerY().
    @return  rectangle center
    */
static vx_point_t vx_rect_center(vx_rect_t r)
{
    return vx_point_make(vx_rect_center_x(r), vx_rect_center_y(r));
}

/** Returns true if all members in a: fLeft, fTop, fRight, and fBottom; are
    equal to the corresponding members in b.

    a and b are not equal if either contain NaN. a and b are equal if members
    contain zeroes with different signs.

    @param a  SkRect to compare
    @param b  SkRect to compare
    @return   true if members are equal
*/
static bool vx_rect_eq(vx_rect_t a, vx_rect_t b) {
    return a.left == b.left &&
            a.top == b.top &&
            a.right == b.right &&
            a.bottom == b.bottom;
}

/** Returns true if any in a: fLeft, fTop, fRight, and fBottom; does not
    equal the corresponding members in b.

    a and b are not equal if either contain NaN. a and b are equal if members
    contain zeroes with different signs.

    @param a  SkRect to compare
    @param b  SkRect to compare
    @return   true if members are not equal
*/
static bool vx_rect_ne(vx_rect_t a, vx_rect_t b) {
    return !vx_rect_eq(a, b);
}

#if 0
    SkPoint TL() const { return {fLeft,  fTop}; }
    SkPoint TR() const { return {fRight, fTop}; }
    SkPoint BL() const { return {fLeft,  fBottom}; }
    SkPoint BR() const { return {fRight, fBottom}; }

    /** Returns four points in quad that enclose SkRect,
     *  respect the specified path-direction.
     */
    std::array<SkPoint, 4> toQuad(SkPathDirection dir = SkPathDirection::kCW) const {
        std::array<SkPoint, 4> storage;
        this->copyToQuad(storage, dir);
        return storage;
    }

    // Same as toQuad(), but copies the 4 points into the specified storage
    // which must be at least a size of 4.
    void copyToQuad(SkSpan<SkPoint> pts, SkPathDirection dir = SkPathDirection::kCW) const {
        SkASSERT(pts.size() >= 4);
        pts[0] = this->TL();
        pts[2] = this->BR();
        if (dir == SkPathDirection::kCW) {
            pts[1] = this->TR();
            pts[3] = this->BL();
        } else {
            pts[1] = this->BL();
            pts[3] = this->TR();
        }
    }

    // DEPRECATED: use std::array or copyToQuad versions
    void toQuad(SkPoint quad[4]) const {
        this->copyToQuad({quad, 4});
    }

    /**
     * Compute the bounds of the span of points.
     * If the span is empty, returns the empty-rect {0, 0, 0, 0.
     * If the span contains non-finite values (inf or nan), returns {}
     */
    static std::optional<SkRect> Bounds(SkSpan<const SkPoint> pts);

    static SkRect BoundsOrEmpty(SkSpan<const SkPoint> pts) {
        if (auto bounds = Bounds(pts)) {
            return bounds.value();
        } else {
            return MakeEmpty();
        }
    }

    /** Sets to bounds of SkPoint array with count entries. If count is zero or smaller,
        or if SkPoint array contains an infinity or NaN, sets to (0, 0, 0, 0).

        Result is either empty or sorted: fLeft is less than or equal to fRight, and
        fTop is less than or equal to fBottom.

        @param pts    SkPoint span
    */
    void setBounds(SkSpan<const SkPoint> pts) {
        (void)this->setBoundsCheck(pts);
    }

    /** Sets to bounds of the span of points, and return true (if all point values were finite).
     *
     * If the span is empty, set the rect to empty() and return true.
     * If any point contains an infinity or NaN, set the rect to empty and return false.
     *
     * @param pts    SkPoint span
     * example: https://fiddle.skia.org/c/@Rect_setBoundsCheck
     */
    bool setBoundsCheck(SkSpan<const SkPoint> pts);

    /** Sets to bounds of the span of points.
     *
     * If the span is empty, set the rect to empty().
     * If any point contains an infinity or NaN, set the rect to NaN.
     *
     * @param pts    SkPoint span
     * example: https://fiddle.skia.org/c/@Rect_setBoundsNoCheck
     */
    void setBoundsNoCheck(SkSpan<const SkPoint> pts);

    /** Sets bounds to the smallest SkRect enclosing SkPoint p0 and p1. The result is
        sorted and may be empty. Does not check to see if values are finite.

        @param p0  corner to include
        @param p1  corner to include
    */
    void set(const SkPoint& p0, const SkPoint& p1) {
        fLeft =   std::min(p0.fX, p1.fX);
        fRight =  std::max(p0.fX, p1.fX);
        fTop =    std::min(p0.fY, p1.fY);
        fBottom = std::max(p0.fY, p1.fY);
    }

    /** Returns SkRect offset by (dx, dy).

        If dx is negative, SkRect returned is moved to the left.
        If dx is positive, SkRect returned is moved to the right.
        If dy is negative, SkRect returned is moved upward.
        If dy is positive, SkRect returned is moved downward.

        @param dx  added to fLeft and fRight
        @param dy  added to fTop and fBottom
        @return    SkRect offset on axes, with original width and height
    */
    SkRect makeOffset(float dx, float dy) const {
        return MakeLTRB(fLeft + dx, fTop + dy, fRight + dx, fBottom + dy);
    }

    /** Returns SkRect offset by v.

        @param v  added to rect
        @return    SkRect offset on axes, with original width and height
    */
    SkRect makeOffset(SkVector v) const { return this->makeOffset(v.x(), v.y()); }

    /** Returns SkRect, inset by (dx, dy).

        If dx is negative, SkRect returned is wider.
        If dx is positive, SkRect returned is narrower.
        If dy is negative, SkRect returned is taller.
        If dy is positive, SkRect returned is shorter.

        @param dx  added to fLeft and subtracted from fRight
        @param dy  added to fTop and subtracted from fBottom
        @return    SkRect inset symmetrically left and right, top and bottom
    */
    SkRect makeInset(float dx, float dy) const {
        return MakeLTRB(fLeft + dx, fTop + dy, fRight - dx, fBottom - dy);
    }

    /** Returns SkRect, outset by (dx, dy).

        If dx is negative, SkRect returned is narrower.
        If dx is positive, SkRect returned is wider.
        If dy is negative, SkRect returned is shorter.
        If dy is positive, SkRect returned is taller.

        @param dx  subtracted to fLeft and added from fRight
        @param dy  subtracted to fTop and added from fBottom
        @return    SkRect outset symmetrically left and right, top and bottom
    */
    SkRect makeOutset(float dx, float dy) const {
        return MakeLTRB(fLeft - dx, fTop - dy, fRight + dx, fBottom + dy);
    }

    /** Offsets SkRect by adding dx to fLeft, fRight; and by adding dy to fTop, fBottom.

        If dx is negative, moves SkRect to the left.
        If dx is positive, moves SkRect to the right.
        If dy is negative, moves SkRect upward.
        If dy is positive, moves SkRect downward.

        @param dx  offset added to fLeft and fRight
        @param dy  offset added to fTop and fBottom
    */
    void offset(float dx, float dy)
    {
        fLeft   += dx;
        fTop    += dy;
        fRight  += dx;
        fBottom += dy;
    }

    /** Offsets SkRect by adding delta.fX to fLeft, fRight; and by adding delta.fY to
        fTop, fBottom.

        If delta.fX is negative, moves SkRect to the left.
        If delta.fX is positive, moves SkRect to the right.
        If delta.fY is negative, moves SkRect upward.
        If delta.fY is positive, moves SkRect downward.

        @param delta  added to SkRect
    */
    void offset(const SkPoint& delta)
    {
        this->offset(delta.fX, delta.fY);
    }

    /** Offsets SkRect so that fLeft equals newX, and fTop equals newY. width and height
        are unchanged.

        @param newX  stored in fLeft, preserving width()
        @param newY  stored in fTop, preserving height()
    */
    void offsetTo(float newX, float newY)
    {
        fRight += newX - fLeft;
        fBottom += newY - fTop;
        fLeft = newX;
        fTop = newY;
    }

    /** Insets SkRect by (dx, dy).

        If dx is positive, makes SkRect narrower.
        If dx is negative, makes SkRect wider.
        If dy is positive, makes SkRect shorter.
        If dy is negative, makes SkRect taller.

        @param dx  added to fLeft and subtracted from fRight
        @param dy  added to fTop and subtracted from fBottom
    */
    void inset(float dx, float dy)
    {
        fLeft   += dx;
        fTop    += dy;
        fRight  -= dx;
        fBottom -= dy;
    }

    /** Outsets SkRect by (dx, dy).

        If dx is positive, makes SkRect wider.
        If dx is negative, makes SkRect narrower.
        If dy is positive, makes SkRect taller.
        If dy is negative, makes SkRect shorter.

        @param dx  subtracted to fLeft and added from fRight
        @param dy  subtracted to fTop and added from fBottom
    */
    void outset(float dx, float dy)
    {
        this->inset(-dx, -dy);
    }
#endif

VX_PUBLIC
vx_rect_t vx_rect_intersected(vx_rect_t a, vx_rect_t b);

VX_PUBLIC
bool vx_rect_intersects(vx_rect_t a, vx_rect_t b);

/** Sets SkRect to the union of itself and r.

    Has no effect if r is empty. Otherwise, if SkRect is empty, sets
    SkRect to r.

    @param r  expansion SkRect

    example: https://fiddle.skia.org/c/@Rect_join_2
*/
VX_PUBLIC
vx_rect_t vx_rect_joined(vx_rect_t r);

#if 0
    /** Sets SkRect to the union of itself and r.

        Asserts if r is empty and SK_DEBUG is defined.
        If SkRect is empty, sets SkRect to r.

        May produce incorrect results if r is empty.

        @param r  expansion SkRect
    */
    void joinNonEmptyArg(const SkRect& r) {
        SkASSERT(!r.isEmpty());
        // if we are empty, just assign
        if (fLeft >= fRight || fTop >= fBottom) {
            *this = r;
        } else {
            this->joinPossiblyEmptyRect(r);
        }
    }

    /** Sets SkRect to the union of itself and the construction.

        May produce incorrect results if SkRect or r is empty.

        @param r  expansion SkRect
    */
    void joinPossiblyEmptyRect(const SkRect& r) {
        fLeft   = std::min(fLeft, r.left());
        fTop    = std::min(fTop, r.top());
        fRight  = std::max(fRight, r.right());
        fBottom = std::max(fBottom, r.bottom());
    }
#endif

/** Returns true if: fLeft <= x < fRight && fTop <= y < fBottom.
    Returns false if SkRect is empty.

    @param x  test SkPoint x-coordinate
    @param y  test SkPoint y-coordinate
    @return   true if (x, y) is inside SkRect
*/
VX_PUBLIC bool vx_rect_contains_xy(vx_rect_t r, float x, float y);

/** Returns true if SkRect contains r.
    Returns false if SkRect is empty or r is empty.

    SkRect contains r when SkRect area completely includes r area.

    @param r  SkRect contained
    @return   true if all sides of SkRect are outside r
*/
VX_PUBLIC bool vx_rect_contains(vx_rect_t lhs, vx_rect_t rhs);

#if 0
    /** Sets SkIRect by adding 0.5 and discarding the fractional portion of SkRect
        members, using (sk_float_round2int(fLeft), sk_float_round2int(fTop),
                        sk_float_round2int(fRight), sk_float_round2int(fBottom)).

        @param dst  storage for SkIRect
    */
    void round(SkIRect* dst) const {
        SkASSERT(dst);
        dst->setLTRB(sk_float_round2int(fLeft),  sk_float_round2int(fTop),
                     sk_float_round2int(fRight), sk_float_round2int(fBottom));
    }

    /** Sets SkIRect by discarding the fractional portion of fLeft and fTop; and rounding
        up fRight and fBottom, using
        (sk_float_floor2int(fLeft), sk_float_floor2int(fTop),
         sk_float_ceil2int(fRight), sk_float_ceil2int(fBottom)).

        @param dst  storage for SkIRect
    */
    void roundOut(SkIRect* dst) const {
        SkASSERT(dst);
        dst->setLTRB(sk_float_floor2int(fLeft), sk_float_floor2int(fTop),
                     sk_float_ceil2int(fRight), sk_float_ceil2int(fBottom));
    }

    /** Sets SkRect by discarding the fractional portion of fLeft and fTop; and rounding
        up fRight and fBottom, using
        (std::floor(fLeft), std::floor(fTop),
         std::ceil(fRight), std::ceil(fBottom)).

        @param dst  storage for SkRect
    */
    void roundOut(SkRect* dst) const {
        dst->setLTRB(std::floor(fLeft), std::floor(fTop),
                     std::ceil(fRight), std::ceil(fBottom));
    }

    /** Sets SkRect by rounding up fLeft and fTop; and discarding the fractional portion
        of fRight and fBottom, using
        (sk_float_ceil2int(fLeft), sk_float_ceil2int(fTop),
         sk_float_floor2int(fRight), sk_float_floor2int(fBottom)).

        @param dst  storage for SkIRect
    */
    void roundIn(SkIRect* dst) const {
        SkASSERT(dst);
        dst->setLTRB(sk_float_ceil2int(fLeft),   sk_float_ceil2int(fTop),
                     sk_float_floor2int(fRight), sk_float_floor2int(fBottom));
    }

    /** Returns SkIRect by adding 0.5 and discarding the fractional portion of SkRect
        members, using (sk_float_round2int(fLeft), sk_float_round2int(fTop),
                        sk_float_round2int(fRight), sk_float_round2int(fBottom)).

        @return  rounded SkIRect
    */
    SkIRect round() const {
        SkIRect ir;
        this->round(&ir);
        return ir;
    }

    /** Sets SkIRect by discarding the fractional portion of fLeft and fTop; and rounding
        up fRight and fBottom, using
        (sk_float_floor2int(fLeft), sk_float_floor2int(fTop),
         sk_float_ceil2int(fRight), sk_float_ceil2int(fBottom)).

        @return  rounded SkIRect
    */
    SkIRect roundOut() const {
        SkIRect ir;
        this->roundOut(&ir);
        return ir;
    }
    /** Sets SkIRect by rounding up fLeft and fTop; and discarding the fractional portion
        of fRight and fBottom, using
        (sk_float_ceil2int(fLeft), sk_float_ceil2int(fTop),
         sk_float_floor2int(fRight), sk_float_floor2int(fBottom)).

        @return  rounded SkIRect
    */
    SkIRect roundIn() const {
        SkIRect ir;
        this->roundIn(&ir);
        return ir;
    }
#endif

/** Returns SkRect with fLeft and fRight swapped if fLeft is greater than fRight; and
    with fTop and fBottom swapped if fTop is greater than fBottom. Result may be empty;
    and width() and height() will be zero or positive.

    @return  sorted SkRect
*/
VX_PUBLIC vx_rect_t vx_rect_sorted(vx_rect_t r);

#if 0
    /** Returns pointer to first float in SkRect, to treat it as an array with four
        entries.

        @return  pointer to fLeft
    */
    const float* asScalars() const { return &fLeft; }

    /** Writes text representation of SkRect to standard output. Set asHex to true to
        generate exact binary representations of floating point numbers.

        @param asHex  true if SkScalar values are written as hexadecimal

        example: https://fiddle.skia.org/c/@Rect_dump
    */
    void dump(bool asHex) const;
    SkString dumpToString(bool asHex) const;

    /** Writes text representation of SkRect to standard output. The representation may be
        directly compiled as C++ code. Floating point values are written
        with limited precision; it may not be possible to reconstruct original SkRect
        from output.
    */
    void dump() const { this->dump(false); }

    /** Writes text representation of SkRect to standard output. The representation may be
        directly compiled as C++ code. Floating point values are written
        in hexadecimal to preserve their exact bit pattern. The output reconstructs the
        original SkRect.

        Use instead of dump() when submitting
    */
    void dumpHex() const { this->dump(true); }
#endif


#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* VIVIDX_CORE_RECT_H */
