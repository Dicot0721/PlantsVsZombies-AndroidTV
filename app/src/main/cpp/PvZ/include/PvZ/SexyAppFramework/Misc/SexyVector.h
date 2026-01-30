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

#include <cmath>

namespace Sexy {

class SexyVector2 {
public:
    float x, y;

    constexpr SexyVector2() noexcept
        : x{0}
        , y{0} {}

    constexpr SexyVector2(float theX, float theY) noexcept
        : x{theX}
        , y{theY} {}

    [[nodiscard]] constexpr float Dot(const SexyVector2 &v) const {
        return (x * v.x) + (y * v.y);
    }

    [[nodiscard]] constexpr SexyVector2 operator+(const SexyVector2 &v) const {
        return {x + v.x, y + v.y};
    }

    [[nodiscard]] constexpr SexyVector2 operator-(const SexyVector2 &v) const {
        return {x - v.x, y - v.y};
    }

    [[nodiscard]] constexpr SexyVector2 operator-() const {
        return {-x, -y};
    }

    [[nodiscard]] constexpr SexyVector2 operator*(float t) const {
        return {t * x, t * y};
    }

    [[nodiscard]] constexpr SexyVector2 operator/(float t) const {
        return {x / t, y / t};
    }

    constexpr SexyVector2 &operator+=(const SexyVector2 &v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    constexpr SexyVector2 &operator-=(const SexyVector2 &v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    constexpr SexyVector2 &operator*=(float t) {
        x *= t;
        y *= t;
        return *this;
    }

    constexpr SexyVector2 &operator/=(float t) {
        x /= t;
        y /= t;
        return *this;
    }

    [[nodiscard]] constexpr bool operator==(const SexyVector2 &v) const = default;

    [[nodiscard]] float Magnitude() const {
        return std::sqrtf((x * x) + (y * y));
    }

    [[nodiscard]] constexpr float MagnitudeSquared() const {
        return (x * x) + (y * y);
    }

    [[nodiscard]] SexyVector2 Normalize() const {
        float aMag = Magnitude();
        return (aMag != 0) ? *this / aMag : *this;
    }

    [[nodiscard]] constexpr SexyVector2 Perp() const {
        return {-y, x};
    }
};

class SexyVector3 {
public:
    float x, y, z;

    constexpr SexyVector3() noexcept
        : x{0}
        , y{0}
        , z{0} {}

    constexpr SexyVector3(float theX, float theY, float theZ) noexcept
        : x{theX}
        , y{theY}
        , z{theZ} {}

    [[nodiscard]] constexpr float Dot(const SexyVector3 &v) const {
        return (x * v.x) + (y * v.y) + (z * v.z);
    }

    [[nodiscard]] constexpr SexyVector3 Cross(const SexyVector3 &v) const {
        return {(y * v.z) - (z * v.y), (z * v.x) - (x * v.z), (x * v.y) - (y * v.x)};
    }

    [[nodiscard]] constexpr SexyVector3 operator+(const SexyVector3 &v) const {
        return {x + v.x, y + v.y, z + v.z};
    }

    [[nodiscard]] constexpr SexyVector3 operator-(const SexyVector3 &v) const {
        return {x - v.x, y - v.y, z - v.z};
    }

    [[nodiscard]] constexpr SexyVector3 operator*(float t) const {
        return {t * x, t * y, t * z};
    }

    [[nodiscard]] constexpr SexyVector3 operator/(float t) const {
        return {x / t, y / t, z / t};
    }

    [[nodiscard]] float Magnitude() const {
        return std::sqrtf((x * x) + (y * y) + (z * z));
    }

    [[nodiscard]] SexyVector3 Normalize() const {
        float aMag = Magnitude();
        return (aMag != 0) ? *this / aMag : *this;
    }
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_MISC_SEXY_VECTOR_H
