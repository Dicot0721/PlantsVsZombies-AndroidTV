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

#ifndef PVZ_SEXYAPPFRAMEWORK_MISC_POINT_H
#define PVZ_SEXYAPPFRAMEWORK_MISC_POINT_H

#include "PvZ/SexyAppFramework/Misc/Common.h"

namespace Sexy {

template <class _T>
class TPoint {
public:
    _T mX;
    _T mY;

public:
    TPoint(_T theX, _T theY)
        : mX(theX)
        , mY(theY) {}

    TPoint(const TPoint<_T> &theTPoint)
        : mX(theTPoint.mX)
        , mY(theTPoint.mY) {}

    TPoint()
        : mX(0)
        , mY(0) {}

    inline bool operator==(const TPoint &p) {
        return ((p.mX == mX) && (p.mY == mY));
    }

    inline bool operator!=(const TPoint &p) {
        return ((p.mX != mX) || (p.mY != mY));
    }

    TPoint operator+(const TPoint &p) const {
        return TPoint(mX + p.mX, mY + p.mY);
    }
    TPoint operator-(const TPoint &p) const {
        return TPoint(mX - p.mX, mY - p.mY);
    }
    TPoint operator*(const TPoint &p) const {
        return TPoint(mX * p.mX, mY * p.mY);
    }
    TPoint operator/(const TPoint &p) const {
        return TPoint(mX / p.mX, mY / p.mY);
    }
    TPoint &operator+=(const TPoint &p) {
        mX += p.mX;
        mY += p.mY;
        return *this;
    }
    TPoint &operator-=(const TPoint &p) {
        mX -= p.mX;
        mY -= p.mY;
        return *this;
    }
    TPoint &operator*=(const TPoint &p) {
        mX *= p.mX;
        mY *= p.mY;
        return *this;
    }
    TPoint &operator/=(const TPoint &p) {
        mX /= p.mX;
        mY /= p.mY;
        return *this;
    }
    TPoint operator*(_T s) const {
        return TPoint(mX * s, mY * s);
    }
    TPoint operator/(_T s) const {
        return TPoint(mX / s, mY / s);
    }
};

typedef TPoint<int> Point;
typedef TPoint<double> FPoint;

}; // namespace Sexy


#endif // PVZ_SEXYAPPFRAMEWORK_MISC_POINT_H
