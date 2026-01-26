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

#ifndef PVZ_SEXYAPPFRAMEWORK_MISC_SEXY_VECTOR_H
#define PVZ_SEXYAPPFRAMEWORK_MISC_SEXY_VECTOR_H

#include <math.h>

namespace Sexy {
class SexyVector2 {
public:
    float x, y;

    SexyVector2()
        : x(0)
        , y(0) {}
    SexyVector2(float theX, float theY)
        : x(theX)
        , y(theY) {}

    float Dot(const SexyVector2 &v) const {
        return x * v.x + y * v.y;
    }
    SexyVector2 operator+(const SexyVector2 &v) const {
        return SexyVector2(x + v.x, y + v.y);
    }
    SexyVector2 operator-(const SexyVector2 &v) const {
        return SexyVector2(x - v.x, y - v.y);
    }
    SexyVector2 operator-() const {
        return SexyVector2(-x, -y);
    }
    SexyVector2 operator*(float t) const {
        return SexyVector2(t * x, t * y);
    }
    SexyVector2 operator/(float t) const {
        return SexyVector2(x / t, y / t);
    }
    void operator+=(const SexyVector2 &v) {
        x += v.x;
        y += v.y;
    }
    void operator-=(const SexyVector2 &v) {
        x -= v.x;
        y -= v.y;
    }
    void operator*=(float t) {
        x *= t;
        y *= t;
    }
    void operator/=(float t) {
        x /= t;
        y /= t;
    }

    bool operator==(const SexyVector2 &v) {
        return x == v.x && y == v.y;
    }
    bool operator!=(const SexyVector2 &v) {
        return x != v.x || y != v.y;
    }

    float Magnitude() const {
        return sqrtf(x * x + y * y);
    }
    float MagnitudeSquared() const {
        return x * x + y * y;
    }

    SexyVector2 Normalize() const {
        float aMag = Magnitude();
        return aMag != 0 ? (*this) / aMag : *this;
    }
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_MISC_SEXY_VECTOR_H
