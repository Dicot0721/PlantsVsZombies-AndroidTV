/*
 * Copyright (C) 2023-2026  PvZ TV Touch Team
 *
 * This file is part of PlantsVsZombies-AndroidTV.
 *
 * PlantsVsZombies-AndroidTV is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * PlantsVsZombies-AndroidTV is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * PlantsVsZombies-AndroidTV.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef PVZ_SEXYAPPFRAMEWORK_MISC_RECT_H
#define PVZ_SEXYAPPFRAMEWORK_MISC_RECT_H

#include "Point.h"

namespace Sexy {

template <class T>
class TRect {
public:
    T mX;
    T mY;
    T mWidth;
    T mHeight;

    constexpr TRect(T theX, T theY, T theWidth, T theHeight)
        : mX{theX}
        , mY{theY}
        , mWidth{theWidth}
        , mHeight{theHeight} {}

    constexpr TRect()
        : mX(0)
        , mY(0)
        , mWidth(0)
        , mHeight(0) {}

    [[nodiscard]] constexpr bool Intersects(const TRect &theTRect) const {
        return (theTRect.mX + theTRect.mWidth > mX)  //
            && (theTRect.mY + theTRect.mHeight > mY) //
            && (theTRect.mX < mX + mWidth)           //
            && (theTRect.mY < mY + mHeight);
    }

    [[nodiscard]] constexpr TRect Intersection(const TRect &theTRect) const {
        T x1 = std::max(mX, theTRect.mX);
        T x2 = std::min(mX + mWidth, theTRect.mX + theTRect.mWidth);
        T y1 = std::max(mY, theTRect.mY);
        T y2 = std::min(mY + mHeight, theTRect.mY + theTRect.mHeight);
        if (x2 < x1 || y2 < y1) {
            return TRect(0, 0, 0, 0);
        } else {
            return TRect(x1, y1, x2 - x1, y2 - y1);
        }
    }

    [[nodiscard]] constexpr TRect Union(const TRect &theTRect) const {
        T x1 = std::min(mX, theTRect.mX);
        T x2 = std::max(mX + mWidth, theTRect.mX + theTRect.mWidth);
        T y1 = std::min(mY, theTRect.mY);
        T y2 = std::max(mY + mHeight, theTRect.mY + theTRect.mHeight);
        return TRect(x1, y1, x2 - x1, y2 - y1);
    }

    [[nodiscard]] constexpr bool Contains(T theX, T theY) const {
        return (theX >= mX) && (theX < mX + mWidth) && (theY >= mY) && (theY < mY + mHeight);
    }

    [[nodiscard]] constexpr bool Contains(const TPoint<T> &thePoint) const {
        return (thePoint.mX >= mX) && (thePoint.mX < mX + mWidth) && (thePoint.mY >= mY) && (thePoint.mY < mY + mHeight);
    }

    constexpr void Offset(T theX, T theY) {
        mX += theX;
        mY += theY;
    }

    constexpr void Offset(const TPoint<T> &thePoint) {
        mX += thePoint.mX;
        mY += thePoint.mY;
    }

    [[nodiscard]] constexpr TRect Inflate(T theX, T theY) {
        mX -= theX;
        mWidth += theX * 2;
        mY -= theY;
        mHeight += theY * 2;
        return *this;
    }

    [[nodiscard]] constexpr bool operator==(const TRect &theRect) const = default;
};

using Rect = Sexy::TRect<int>;
using FRect = Sexy::TRect<double>;

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_MISC_RECT_H
