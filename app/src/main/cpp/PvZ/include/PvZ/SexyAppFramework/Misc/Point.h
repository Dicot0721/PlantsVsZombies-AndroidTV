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

namespace Sexy {

template <class T>
class TPoint {
public:
    T mX;
    T mY;

public:
    constexpr TPoint(T theX, T theY)
        : mX{theX}
        , mY{theY} {}

    constexpr TPoint()
        : mX(0)
        , mY(0) {}

    [[nodiscard]] constexpr bool operator==(const TPoint &p) const = default;

    [[nodiscard]] constexpr TPoint operator+(const TPoint &p) const {
        return TPoint(mX + p.mX, mY + p.mY);
    }

    [[nodiscard]] constexpr TPoint operator-(const TPoint &p) const {
        return TPoint(mX - p.mX, mY - p.mY);
    }

    [[nodiscard]] constexpr TPoint operator*(const TPoint &p) const {
        return TPoint(mX * p.mX, mY * p.mY);
    }

    [[nodiscard]] constexpr TPoint operator/(const TPoint &p) const {
        return TPoint(mX / p.mX, mY / p.mY);
    }

    constexpr TPoint &operator+=(const TPoint &p) {
        mX += p.mX;
        mY += p.mY;
        return *this;
    }

    constexpr TPoint &operator-=(const TPoint &p) {
        mX -= p.mX;
        mY -= p.mY;
        return *this;
    }

    constexpr TPoint &operator*=(const TPoint &p) {
        mX *= p.mX;
        mY *= p.mY;
        return *this;
    }

    constexpr TPoint &operator/=(const TPoint &p) {
        mX /= p.mX;
        mY /= p.mY;
        return *this;
    }

    [[nodiscard]] constexpr TPoint operator*(T s) const {
        return TPoint(mX * s, mY * s);
    }

    [[nodiscard]] constexpr TPoint operator/(T s) const {
        return TPoint(mX / s, mY / s);
    }
};

using Point = TPoint<int>;
using FPoint = TPoint<double>;

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_MISC_POINT_H
