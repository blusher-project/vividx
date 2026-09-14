/*
 * Copyright 2006 The Android Open Source Project
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "include/core/SkRect.h"

#include <stdint.h>

#include "include/core/SkM44.h"
#include "include/private/SkDebug.h"
#include "include/private/SkTPin.h"
#include "src/core/SkRectPriv.h"
#include "vividx/core/rect.h"

class SkMatrix;

//!<==================
//!< SkIRect
//!<==================

SkIRect::SkIRect()
{
    fLeft = 0;
    fTop = 0;
    fRight = 0;
    fBottom = 0;
}

SkIRect::SkIRect(vx_rect_i_t rect)
{
    fLeft   = rect.left;
    fTop    = rect.top;
    fRight  = rect.right;
    fBottom = rect.bottom;
}

SkIRect SkIRect::MakeEmpty() {
    return vx_rect_i_make();
}

SkIRect SkIRect::MakeWH(int32_t w, int32_t h) {
    return vx_rect_i_make_wh(w, h);
}

SkIRect SkIRect::MakeSize(const SkISize& size)
{
    SkIRect iRect = SkIRect::MakeEmpty();
    iRect.fLeft = 0;
    iRect.fTop = 0,
    iRect.fRight = size.fWidth;
    iRect.fBottom = size.fHeight;
    return iRect;
}

SkIRect SkIRect::MakePtSize(SkIPoint pt, SkISize size)
{
    return MakeXYWH(pt.x(), pt.y(), size.width(), size.height());
}

SkIRect SkIRect::MakeLTRB(int32_t l, int32_t t, int32_t r, int32_t b)
{
    return vx_rect_i_make_ltrb(l, t, r, b);
}

SkIRect SkIRect::MakeXYWH(int32_t x, int32_t y, int32_t w, int32_t h)
{
    return vx_rect_i_make_xywh(x, y, w, h);
}

int32_t SkIRect::left() const { return fLeft; }

int32_t SkIRect::top() const { return fTop; }

int32_t SkIRect::right() const { return fRight; }

int32_t SkIRect::bottom() const { return fBottom; }

int32_t SkIRect::x() const { return fLeft; }

int32_t SkIRect::y() const { return fTop; }

// Experimental
SkIPoint SkIRect::topLeft() const { return {fLeft, fTop}; }

int32_t SkIRect::width() const
{
    return vx_rect_i_width(*this);
}

int32_t SkIRect::height() const
{
    return vx_rect_i_height(*this);
}

SkISize SkIRect::size() const { return SkISize::Make(this->width(), this->height()); }

int64_t SkIRect::width64() const { return (int64_t)fRight - (int64_t)fLeft; }

int64_t SkIRect::height64() const { return (int64_t)fBottom - (int64_t)fTop; }

bool SkIRect::isEmpty64() const { return fRight <= fLeft || fBottom <= fTop; }

bool SkIRect::isEmpty() const
{
    return vx_rect_i_is_empty(*this);
}

bool operator==(const SkIRect& a, const SkIRect& b)
{
    return vx_rect_i_eq(a, b);
}

bool operator!=(const SkIRect& a, const SkIRect& b)
{
    return vx_rect_i_ne(a, b);
}

void SkIRect::setEmpty() { memset(this, 0, sizeof(*this)); }

void SkIRect::setLTRB(int32_t left, int32_t top, int32_t right, int32_t bottom) {
    fLeft   = left;
    fTop    = top;
    fRight  = right;
    fBottom = bottom;
}

void SkIRect::setXYWH(int32_t x, int32_t y, int32_t width, int32_t height) {
    fLeft   = x;
    fTop    = y;
    fRight  = Sk32_sat_add(x, width);
    fBottom = Sk32_sat_add(y, height);
}

void SkIRect::setWH(int32_t width, int32_t height) {
    fLeft   = 0;
    fTop    = 0;
    fRight  = width;
    fBottom = height;
}

void SkIRect::setSize(SkISize size) {
    fLeft = 0;
    fTop = 0;
    fRight = size.width();
    fBottom = size.height();
}

SkIRect SkIRect::makeOffset(int32_t dx, int32_t dy) const
{
    return vx_rect_i_offset(*this, dx, dy);
}

SkIRect SkIRect::makeOffset(SkIVector offset) const
{
    return this->makeOffset(offset.x(), offset.y());
}

SkIRect SkIRect::makeInset(int32_t dx, int32_t dy) const
{
    return vx_rect_i_inset(*this, dx, dy);
}

SkIRect SkIRect::makeOutset(int32_t dx, int32_t dy) const
{
    return vx_rect_i_outset(*this, dx, dy);
}

void SkIRect::offset(int32_t dx, int32_t dy)
{
    vx_rect_i_t r = vx_rect_i_offset(*this, dx, dy);
    *this = r;
}

void SkIRect::offset(const SkIPoint& delta) {
    this->offset(delta.fX, delta.fY);
}

void SkIRect::offsetTo(int32_t newX, int32_t newY)
{
    vx_rect_i_t r = vx_rect_i_offset_to(*this, newX, newY);
    *this = r;
}

void SkIRect::inset(int32_t dx, int32_t dy)
{
    *this = vx_rect_i_inset(*this, dx, dy);
}

void SkIRect::outset(int32_t dx, int32_t dy)
{
    this->inset(-dx, -dy);
}

void SkIRect::adjust(int32_t dL, int32_t dT, int32_t dR, int32_t dB)
{
    *this = vx_rect_i_adjusted(*this, dL, dT, dR, dB);
}

bool SkIRect::contains(int32_t x, int32_t y) const
{
    return vx_rect_i_contains_xy(*this, x, y);
}

bool SkIRect::contains(const SkIRect& r) const
{
    return vx_rect_i_contains(*this, r);
}

bool SkIRect::containsNoEmptyCheck(const SkIRect& r) const
{
    SkASSERT(fLeft < fRight && fTop < fBottom);
    SkASSERT(r.fLeft < r.fRight && r.fTop < r.fBottom);
    return fLeft <= r.fLeft && fTop <= r.fTop && fRight >= r.fRight && fBottom >= r.fBottom;
}

bool SkIRect::intersect(const SkIRect& r)
{
    return this->intersect(*this, r);
}

bool SkIRect::intersect(const SkIRect& a, const SkIRect& b)
{
    SkIRect tmp = vx_rect_i_intersected(a, b);
    if (tmp.isEmpty()) {
        return false;
    }
    *this = tmp;
    return true;
}

bool SkIRect::Intersects(const SkIRect& a, const SkIRect& b)
{
    return SkIRect::MakeEmpty().intersect(a, b);
}

void SkIRect::join(const SkIRect& r)
{
    *this = vx_rect_i_joined(*this, r);
}

void SkIRect::sort()
{
    *this = vx_rect_i_sorted(*this);
}

SkIRect SkIRect::makeSorted() const
{
    return vx_rect_i_sorted(*this);
}

const int32_t* SkIRect::asInt32s() const { return &fLeft; }

SkIRect::operator vx_rect_i_t() const
{
    return vx_rect_i_make_ltrb(fLeft, fTop, fRight, fBottom);
}


//!<==================
//!< SkRect
//!<==================

SkRect::SkRect()
{
    fLeft = 0;
    fTop = 0;
    fRight = 0;
    fBottom = 0;
}

SkRect::SkRect(vx_rect_t r)
    : fLeft(r.left), fTop(r.top), fRight(r.right), fBottom(r.bottom)
{
}

SkRect SkRect::MakeEmpty() {
    return vx_rect_make_ltrb(0, 0, 0, 0);
}

SkRect SkRect::MakeWH(float w, float h) {
    return vx_rect_make_wh(w, h);
}

SkRect SkRect::MakeIWH(int w, int h) {
    return SkRect::MakeLTRB(0, 0, static_cast<float>(w), static_cast<float>(h));
}

SkRect SkRect::MakeSize(const SkSize& size) {
    return SkRect::MakeLTRB(0, 0, size.fWidth, size.fHeight);
}

SkRect SkRect::MakeLTRB(float l, float t, float r, float b) {
    return vx_rect_make_ltrb(l, t, r, b);
}

SkRect SkRect::MakeXYWH(float x, float y, float w, float h) {
    return vx_rect_make_xywh(x, y, w, h);
}

SkRect SkRect::Make(const SkISize& size) {
    return MakeIWH(size.width(), size.height());
}

SkRect SkRect::Make(const SkIRect& irect) {
    return SkRect::MakeLTRB(
        static_cast<float>(irect.fLeft), static_cast<float>(irect.fTop),
        static_cast<float>(irect.fRight), static_cast<float>(irect.fBottom)
    );
}

bool SkRect::isEmpty() const {
    // We write it as the NOT of a non-empty rect, so we will return true if any values
    // are NaN.
    return !(fLeft < fRight && fTop < fBottom);
}

bool SkRect::isSorted() const { return fLeft <= fRight && fTop <= fBottom; }

bool SkRect::isFinite() const {
    return SkIsFinite(fLeft, fTop, fRight, fBottom);
}

float SkRect::x() const { return fLeft; }

float SkRect::y() const { return fTop; }

float SkRect::left() const { return fLeft; }

float SkRect::top() const { return fTop; }

float SkRect::right() const { return fRight; }

float SkRect::bottom() const { return fBottom; }

float SkRect::width() const { return fRight - fLeft; }

float SkRect::height() const { return fBottom - fTop; }

float SkRect::centerX() const {
    return vx_float_midpoint(fLeft, fRight);
}

float SkRect::centerY() const {
    return vx_float_midpoint(fTop, fBottom);
}

SkPoint SkRect::center() const { return {this->centerX(), this->centerY()}; }

bool operator==(const SkRect& a, const SkRect& b) {
    return a.fLeft == b.fLeft &&
            a.fTop == b.fTop &&
            a.fRight == b.fRight &&
            a.fBottom == b.fBottom;
}

bool operator!=(const SkRect& a, const SkRect& b) {
    return !(a == b);
}

SkPoint SkRect::TL() const { return {fLeft,  fTop}; }
SkPoint SkRect::TR() const { return {fRight, fTop}; }
SkPoint SkRect::BL() const { return {fLeft,  fBottom}; }
SkPoint SkRect::BR() const { return {fRight, fBottom}; }

std::array<SkPoint, 4> SkRect::toQuad(SkPathDirection dir) const {
    std::array<SkPoint, 4> storage;
    this->copyToQuad(storage, dir);
    return storage;
}

// Same as toQuad(), but copies the 4 points into the specified storage
// which must be at least a size of 4.
void SkRect::copyToQuad(SkSpan<SkPoint> pts, SkPathDirection dir) const {
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
void SkRect::toQuad(SkPoint quad[4]) const {
    this->copyToQuad({quad, 4});
}

void SkRect::setEmpty() { *this = MakeEmpty(); }

void SkRect::set(const SkIRect& src) {
    fLeft   = src.fLeft;
    fTop    = src.fTop;
    fRight  = src.fRight;
    fBottom = src.fBottom;
}

void SkRect::setLTRB(float left, float top, float right, float bottom) {
    fLeft   = left;
    fTop    = top;
    fRight  = right;
    fBottom = bottom;
}

SkRect SkRect::BoundsOrEmpty(SkSpan<const SkPoint> pts) {
    if (auto bounds = Bounds(pts)) {
        return bounds.value();
    } else {
        return MakeEmpty();
    }
}

void SkRect::setBounds(SkSpan<const SkPoint> pts) {
    (void)this->setBoundsCheck(pts);
}

void SkRect::set(const SkPoint& p0, const SkPoint& p1) {
    fLeft =   std::min(p0.fX, p1.fX);
    fRight =  std::max(p0.fX, p1.fX);
    fTop =    std::min(p0.fY, p1.fY);
    fBottom = std::max(p0.fY, p1.fY);
}

void SkRect::setXYWH(float x, float y, float width, float height) {
    fLeft = x;
    fTop = y;
    fRight = x + width;
    fBottom = y + height;
}

void SkRect::setWH(float width, float height) {
    fLeft = 0;
    fTop = 0;
    fRight = width;
    fBottom = height;
}

void SkRect::setIWH(int32_t width, int32_t height) {
    this->setWH(width, height);
}

SkRect SkRect::makeOffset(float dx, float dy) const {
    return MakeLTRB(fLeft + dx, fTop + dy, fRight + dx, fBottom + dy);
}

SkRect SkRect::makeOffset(SkVector v) const { return this->makeOffset(v.x(), v.y()); }

SkRect SkRect::makeInset(float dx, float dy) const {
    return MakeLTRB(fLeft + dx, fTop + dy, fRight - dx, fBottom - dy);
}

SkRect SkRect::makeOutset(float dx, float dy) const {
    return MakeLTRB(fLeft - dx, fTop - dy, fRight + dx, fBottom + dy);
}

void SkRect::offset(float dx, float dy) {
    fLeft   += dx;
    fTop    += dy;
    fRight  += dx;
    fBottom += dy;
}

void SkRect::offset(const SkPoint& delta) {
    this->offset(delta.fX, delta.fY);
}

void SkRect::offsetTo(float newX, float newY) {
    fRight += newX - fLeft;
    fBottom += newY - fTop;
    fLeft = newX;
    fTop = newY;
}

void SkRect::inset(float dx, float dy)  {
    fLeft   += dx;
    fTop    += dy;
    fRight  -= dx;
    fBottom -= dy;
}

void SkRect::outset(float dx, float dy)  { this->inset(-dx, -dy); }

bool SkRect::Intersects(float al, float at, float ar, float ab,
                        float bl, float bt, float br, float bb) {
    float L = std::max(al, bl);
    float R = std::min(ar, br);
    float T = std::max(at, bt);
    float B = std::min(ab, bb);
    return L < R && T < B;
}

bool SkRect::intersects(const SkRect& r) const {
    return Intersects(fLeft, fTop, fRight, fBottom,
                        r.fLeft, r.fTop, r.fRight, r.fBottom);
}

bool SkRect::Intersects(const SkRect& a, const SkRect& b) {
    return Intersects(a.fLeft, a.fTop, a.fRight, a.fBottom,
                        b.fLeft, b.fTop, b.fRight, b.fBottom);
}

void SkRect::joinNonEmptyArg(const SkRect& r) {
    SkASSERT(!r.isEmpty());
    // if we are empty, just assign
    if (fLeft >= fRight || fTop >= fBottom) {
        *this = r;
    } else {
        this->joinPossiblyEmptyRect(r);
    }
}

void SkRect::joinPossiblyEmptyRect(const SkRect& r) {
    fLeft   = std::min(fLeft, r.left());
    fTop    = std::min(fTop, r.top());
    fRight  = std::max(fRight, r.right());
    fBottom = std::max(fBottom, r.bottom());
}

bool SkRect::contains(float x, float y) const {
    return x >= fLeft && x < fRight && y >= fTop && y < fBottom;
}

bool SkRect::contains(const SkRect& r) const {
    // todo: can we eliminate the this->isEmpty check?
    return  !r.isEmpty() && !this->isEmpty() &&
            fLeft <= r.fLeft && fTop <= r.fTop &&
            fRight >= r.fRight && fBottom >= r.fBottom;
}

bool SkRect::contains(const SkIRect& r) const {
    // todo: can we eliminate the this->isEmpty check?
    return  !r.isEmpty() && !this->isEmpty() &&
            fLeft <= r.fLeft && fTop <= r.fTop &&
            fRight >= r.fRight && fBottom >= r.fBottom;
}

void SkRect::round(SkIRect* dst) const {
    SkASSERT(dst);
    dst->setLTRB(sk_float_round2int(fLeft),  sk_float_round2int(fTop),
                    sk_float_round2int(fRight), sk_float_round2int(fBottom));
}

void SkRect::roundOut(SkIRect* dst) const {
    SkASSERT(dst);
    dst->setLTRB(sk_float_floor2int(fLeft), sk_float_floor2int(fTop),
                    sk_float_ceil2int(fRight), sk_float_ceil2int(fBottom));
}

void SkRect::roundOut(SkRect* dst) const {
    dst->setLTRB(std::floor(fLeft), std::floor(fTop),
                    std::ceil(fRight), std::ceil(fBottom));
}

void SkRect::roundIn(SkIRect* dst) const {
    SkASSERT(dst);
    dst->setLTRB(sk_float_ceil2int(fLeft),   sk_float_ceil2int(fTop),
                    sk_float_floor2int(fRight), sk_float_floor2int(fBottom));
}

SkIRect SkRect::round() const {
    SkIRect ir = SkIRect::MakeEmpty();
    this->round(&ir);
    return ir;
}

SkIRect SkRect::roundOut() const {
    SkIRect ir = SkIRect::MakeEmpty();
    this->roundOut(&ir);
    return ir;
}

SkIRect SkRect::roundIn() const {
    SkIRect ir = SkIRect::MakeEmpty();
    this->roundIn(&ir);
    return ir;
}

void SkRect::sort() {
    using std::swap;
    if (fLeft > fRight) {
        swap(fLeft, fRight);
    }

    if (fTop > fBottom) {
        swap(fTop, fBottom);
    }
}

SkRect SkRect::makeSorted() const {
    return MakeLTRB(std::min(fLeft, fRight), std::min(fTop, fBottom),
                    std::max(fLeft, fRight), std::max(fTop, fBottom));
}

const float* SkRect::asScalars() const { return &fLeft; }

void SkRect::dump() const { this->dump(false); }

void SkRect::dumpHex() const { this->dump(true); }


std::optional<SkRect> SkRect::Bounds(SkSpan<const SkPoint> points)
{
    if (points.empty()) {
        return SkRect::MakeEmpty();
    }

    /*
     *  Both of these variants compute the same numerics.
     *
     *  But, the "simple" one (no explicit skvx) runs faster (most of the time) on 64bit
     *  machines, and the tricky skvx version runs faster (most of the time) on 32bit machines.
     *
     *  Hence the if/else
     */

    if constexpr (sizeof(void*) == 8) {
        float L = points[0].fX, T = points[0].fY, R = points[0].fX, B = points[0].fY;
        float nx = 0, ny = 0;
        for (auto p : points) {
            L = std::fminf(p.fX, L);
            T = std::fminf(p.fY, T);
            R = std::fmaxf(p.fX, R);
            B = std::fmaxf(p.fY, B);

            // we do this to look for infinities or nans
            nx *= p.fX;
            ny *= p.fY;
        }

        // if this is true, all our values were finite
        if (nx == 0 && ny == 0) {
            return {SkRect::MakeLTRB(L, T, R, B)};
            // return {{L, T, R, B}};
        }
    } else {
        auto count = points.size();
        auto pts = points.data();

        skvx::float4 min, max;
        if (count & 1) {
            min = max = skvx::float2::Load(pts).xyxy();
            pts   += 1;
            count -= 1;
        } else {
            min = max = skvx::float4::Load(pts);
            pts   += 2;
            count -= 2;
        }

        skvx::float4 accum = min * 0;
        while (count) {
            skvx::float4 xy = skvx::float4::Load(pts);
            accum = accum * xy;
            min = skvx::min(min, xy);
            max = skvx::max(max, xy);
            pts   += 2;
            count -= 2;
        }

        const bool all_finite = all(accum * 0 == 0);
        if (all_finite) {
            return MakeLTRB(std::min(min[0], min[2]), std::min(min[1], min[3]),
                            std::max(max[0], max[2]), std::max(max[1], max[3]));
        }
    }

    /*
     *  If we got here, we were not empty, and at least one of the span values was
     *  either an Infinity or NaN -- so we return failure (no finite bounds)
     */
    return {};
}

bool SkRect::setBoundsCheck(SkSpan<const SkPoint> pts) {
    if (auto bounds = Bounds(pts)) {
        *this = bounds.value();
        return true;
    } else {
        *this = MakeEmpty();
        return false;
    }
}

void SkRect::setBoundsNoCheck(SkSpan<const SkPoint> pts) {
    if (auto bounds = Bounds(pts)) {
        *this = bounds.value();
    } else {
        this->setLTRB(SK_FloatNaN, SK_FloatNaN, SK_FloatNaN, SK_FloatNaN);
    }
}

#define CHECK_INTERSECT(al, at, ar, ab, bl, bt, br, bb) \
    float L = std::max(al, bl);                         \
    float R = std::min(ar, br);                         \
    float T = std::max(at, bt);                         \
    float B = std::min(ab, bb);                         \
    do { if (!(L < R && T < B)) return false; } while (0)
    // do the !(opposite) check so we return false if either arg is NaN

bool SkRect::intersect(const SkRect& r) {
    CHECK_INTERSECT(r.fLeft, r.fTop, r.fRight, r.fBottom, fLeft, fTop, fRight, fBottom);
    this->setLTRB(L, T, R, B);
    return true;
}

bool SkRect::intersect(const SkRect& a, const SkRect& b) {
    CHECK_INTERSECT(a.fLeft, a.fTop, a.fRight, a.fBottom, b.fLeft, b.fTop, b.fRight, b.fBottom);
    this->setLTRB(L, T, R, B);
    return true;
}

void SkRect::join(const SkRect& r) {
    if (r.isEmpty()) {
        return;
    }

    if (this->isEmpty()) {
        *this = r;
    } else {
        fLeft   = std::min(fLeft, r.fLeft);
        fTop    = std::min(fTop, r.fTop);
        fRight  = std::max(fRight, r.fRight);
        fBottom = std::max(fBottom, r.fBottom);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////

#include "include/core/SkString.h"
#include "src/core/SkStringUtils.h"

static const char* set_scalar(SkString* storage, float value, SkScalarAsStringType asType) {
    storage->reset();
    SkAppendScalar(storage, value, asType);
    return storage->c_str();
}

SkString SkRect::dumpToString(bool asHex) const {
    SkScalarAsStringType asType = asHex ? kHex_SkScalarAsStringType : kDec_SkScalarAsStringType;

    SkString line;
    if (asHex) {
        SkString tmp;
        line.printf( "SkRect::MakeLTRB(%s, /* %f */\n", set_scalar(&tmp, fLeft, asType), fLeft);
        line.appendf("                 %s, /* %f */\n", set_scalar(&tmp, fTop, asType), fTop);
        line.appendf("                 %s, /* %f */\n", set_scalar(&tmp, fRight, asType), fRight);
        line.appendf("                 %s  /* %f */);", set_scalar(&tmp, fBottom, asType), fBottom);
    } else {
        SkString strL, strT, strR, strB;
        SkAppendScalarDec(&strL, fLeft);
        SkAppendScalarDec(&strT, fTop);
        SkAppendScalarDec(&strR, fRight);
        SkAppendScalarDec(&strB, fBottom);
        line.printf("SkRect::MakeLTRB(%s, %s, %s, %s);",
                    strL.c_str(), strT.c_str(), strR.c_str(), strB.c_str());
    }
    return line;
}

void SkRect::dump(bool asHex) const {
    SkDebugf("%s\n", this->dumpToString(asHex).c_str());
}

SkRect::operator vx_rect_t() const
{
    return vx_rect_make_ltrb(fLeft, fTop, fRight, fBottom);
}

////////////////////////////////////////////////////////////////////////////////////////////////

//!<==================
//!< SkRectPriv
//!<==================

template<typename R>
static bool subtract(const R& a, const R& b, R* out) {
    if (a.isEmpty() || b.isEmpty() || !R::Intersects(a, b)) {
        // Either already empty, or subtracting the empty rect, or there's no intersection, so
        // in all cases the answer is A.
        *out = a;
        return true;
    }

    // 4 rectangles to consider. If the edge in A is contained in B, the resulting difference can
    // be represented exactly as a rectangle. Otherwise the difference is the largest subrectangle
    // that is disjoint from B:
    // 1. Left part of A:   (A.left,  A.top,    B.left,  A.bottom)
    // 2. Right part of A:  (B.right, A.top,    A.right, A.bottom)
    // 3. Top part of A:    (A.left,  A.top,    A.right, B.top)
    // 4. Bottom part of A: (A.left,  B.bottom, A.right, A.bottom)
    //
    // Depending on how B intersects A, there will be 1 to 4 positive areas:
    //  - 4 occur when A contains B
    //  - 3 occur when B intersects a single edge
    //  - 2 occur when B intersects at a corner, or spans two opposing edges
    //  - 1 occurs when B spans two opposing edges and contains a 3rd, resulting in an exact rect
    //  - 0 occurs when B contains A, resulting in the empty rect
    //
    // Compute the relative areas of the 4 rects described above. Since each subrectangle shares
    // either the width or height of A, we only have to divide by the other dimension, which avoids
    // overflow on int32 types, and even if the float relative areas overflow to infinity, the
    // comparisons work out correctly and (one of) the infinitely large subrects will be chosen.
    float aHeight = (float) a.height();
    float aWidth = (float) a.width();
    float leftArea = 0.f, rightArea = 0.f, topArea = 0.f, bottomArea = 0.f;
    int positiveCount = 0;
    if (b.fLeft > a.fLeft) {
        leftArea = (b.fLeft - a.fLeft) / aWidth;
        positiveCount++;
    }
    if (a.fRight > b.fRight) {
        rightArea = (a.fRight - b.fRight) / aWidth;
        positiveCount++;
    }
    if (b.fTop > a.fTop) {
        topArea = (b.fTop - a.fTop) / aHeight;
        positiveCount++;
    }
    if (a.fBottom > b.fBottom) {
        bottomArea = (a.fBottom - b.fBottom) / aHeight;
        positiveCount++;
    }

    if (positiveCount == 0) {
        SkASSERT(b.contains(a));
        *out = R::MakeEmpty();
        return true;
    }

    *out = a;
    if (leftArea > rightArea && leftArea > topArea && leftArea > bottomArea) {
        // Left chunk of A, so the new right edge is B's left edge
        out->fRight = b.fLeft;
    } else if (rightArea > topArea && rightArea > bottomArea) {
        // Right chunk of A, so the new left edge is B's right edge
        out->fLeft = b.fRight;
    } else if (topArea > bottomArea) {
        // Top chunk of A, so the new bottom edge is B's top edge
        out->fBottom = b.fTop;
    } else {
        // Bottom chunk of A, so the new top edge is B's bottom edge
        SkASSERT(bottomArea > 0.f);
        out->fTop = b.fBottom;
    }

    // If we have 1 valid area, the disjoint shape is representable as a rectangle.
    SkASSERT(!R::Intersects(*out, b));
    return positiveCount == 1;
}

bool SkRectPriv::Subtract(const SkRect& a, const SkRect& b, SkRect* out) {
    return subtract<SkRect>(a, b, out);
}

bool SkRectPriv::Subtract(const SkIRect& a, const SkIRect& b, SkIRect* out) {
    return subtract<SkIRect>(a, b, out);
}


bool SkRectPriv::QuadContainsRect(const SkMatrix& m,
                                  const SkIRect& a,
                                  const SkIRect& b,
                                  float tol) {
    return QuadContainsRect(SkM44(m), SkRect::Make(a), SkRect::Make(b), tol);
}

bool SkRectPriv::QuadContainsRect(const SkM44& m, const SkRect& a, const SkRect& b, float tol) {
    return all(QuadContainsRectMask(m, a, b, tol));
}

skvx::int4 SkRectPriv::QuadContainsRectMask(const SkM44& m,
                                            const SkRect& a,
                                            const SkRect& b,
                                            float tol) {
    SkDEBUGCODE(SkM44 inverse;)
    SkASSERT(m.invert(&inverse));
    // With empty rectangles, the calculated edges could give surprising results. If 'a' were not
    // sorted, its normals would point outside the sorted rectangle, so lots of potential rects
    // would be seen as "contained". If 'a' is all 0s, its edge equations are also (0,0,0) so every
    // point has a distance of 0, and would be interpreted as inside.
    if (a.isEmpty()) {
        return skvx::int4(0); // all "false"
    }
    // However, 'b' is only used to define its 4 corners to check against the transformed edges.
    // This is valid regardless of b's emptiness or sortedness.

    // Calculate the 4 homogenous coordinates of 'a' transformed by 'm' where Z=0 and W=1.
    auto ax = skvx::float4{a.fLeft, a.fRight, a.fRight, a.fLeft};
    auto ay = skvx::float4{a.fTop, a.fTop, a.fBottom, a.fBottom};

    auto max = m.rc(0,0)*ax + m.rc(0,1)*ay + m.rc(0,3);
    auto may = m.rc(1,0)*ax + m.rc(1,1)*ay + m.rc(1,3);
    auto maw = m.rc(3,0)*ax + m.rc(3,1)*ay + m.rc(3,3);

    if (all(maw < 0.f)) {
        // If all points of A are mapped to w < 0, then the edge equations end up representing the
        // convex hull of projected points when A should in fact be considered empty.
        return skvx::int4(0); // all "false"
    }

    // Cross product of adjacent vertices provides homogenous lines for the 4 sides of the quad
    auto lA = may*skvx::shuffle<1,2,3,0>(maw) - maw*skvx::shuffle<1,2,3,0>(may);
    auto lB = maw*skvx::shuffle<1,2,3,0>(max) - max*skvx::shuffle<1,2,3,0>(maw);
    auto lC = max*skvx::shuffle<1,2,3,0>(may) - may*skvx::shuffle<1,2,3,0>(max);

    // Before transforming, the corners of 'a' were in CW order, but afterwards they may become CCW,
    // so the sign corrects the direction of the edge normals to point inwards.
    float sign = (lA[0]*lB[1] - lB[0]*lA[1]) < 0 ? -1.f : 1.f;

    // Calculate distance from 'b' to each edge. Since 'b' has presumably been transformed by 'm'
    // *and* projected, this assumes W = 1.
    SkRect bInset = b.makeInset(tol, tol);
    auto d0 = sign * (lA*bInset.fLeft  + lB*bInset.fTop    + lC);
    auto d1 = sign * (lA*bInset.fRight + lB*bInset.fTop    + lC);
    auto d2 = sign * (lA*bInset.fRight + lB*bInset.fBottom + lC);
    auto d3 = sign * (lA*bInset.fLeft  + lB*bInset.fBottom + lC);

    // 'b' is contained in the mapped rectangle if all distances are >= 0
    return (d0 >= 0.f) & (d1 >= 0.f) & (d2 >= 0.f) & (d3 >= 0.f);
}

SkIRect SkRectPriv::ClosestDisjointEdge(const SkIRect& src, const SkIRect& dst) {
    if (src.isEmpty() || dst.isEmpty()) {
        return SkIRect::MakeEmpty();
    }

    int l = src.fLeft;
    int r = src.fRight;
    if (r <= dst.fLeft) {
        // Select right column of pixels in crop
        l = r - 1;
    } else if (l >= dst.fRight) {
        // Left column of 'crop'
        r = l + 1;
    } else {
        // Regular intersection along X axis.
        l = SkTPin(l, dst.fLeft, dst.fRight);
        r = SkTPin(r, dst.fLeft, dst.fRight);
    }

    int t = src.fTop;
    int b = src.fBottom;
    if (b <= dst.fTop) {
        // Select bottom row of pixels in crop
        t = b - 1;
    } else if (t >= dst.fBottom) {
        // Top row of 'crop'
        b = t + 1;
    } else {
        t = SkTPin(t, dst.fTop, dst.fBottom);
        b = SkTPin(b, dst.fTop, dst.fBottom);
    }

    return SkIRect::MakeLTRB(l,t,r,b);
}
