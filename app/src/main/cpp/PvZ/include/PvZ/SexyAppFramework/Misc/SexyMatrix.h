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

#ifndef PVZ_SEXYAPPFRAMEWORK_MISC_SEXY_MATRIX_H
#define PVZ_SEXYAPPFRAMEWORK_MISC_SEXY_MATRIX_H

#include "SexyVector.h"

namespace Sexy {

class SexyMatrix3 {
public:
    union {
        float m[3][3];
        struct {
            float m00, m01, m02;
            float m10, m11, m12;
            float m20, m21, m22;
        };
    };

    constexpr SexyMatrix3() = default;

    constexpr void ZeroMatrix() noexcept {
        // clang-format off
        m00 = m01 = m02 =
        m10 = m11 = m12 =
        m20 = m21 = m22 = 0;
        // clang-format on
    }

    constexpr void LoadIdentity() noexcept {
        m01 = m02 = m10 = m12 = m20 = m21 = 0;
        m00 = m11 = m22 = 1;
    }

    [[nodiscard]] constexpr SexyVector2 operator*(const SexyVector2 &theVec) const {
        return {
            m00 * theVec.x + m01 * theVec.y + m02, //
            m10 * theVec.x + m11 * theVec.y + m12  //
        };
    }

    [[nodiscard]] constexpr SexyVector3 operator*(const SexyVector3 &theVec) const {
        return {
            m00 * theVec.x + m01 * theVec.y + m02 * theVec.z, //
            m10 * theVec.x + m11 * theVec.y + m12 * theVec.z, //
            m20 * theVec.x + m21 * theVec.y + m22 * theVec.z  //
        };
    }

    [[nodiscard]] constexpr SexyMatrix3 operator*(const SexyMatrix3 &theMat) const {
        SexyMatrix3 aResult;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                float x = 0;
                for (int k = 0; k < 3; ++k) {
                    x += m[i][k] * theMat.m[k][j];
                }
                aResult.m[i][j] = x;
            }
        }
        return aResult;
    }

    constexpr SexyMatrix3 &operator*=(const SexyMatrix3 &theMat) {
        return *this = *this * theMat;
    }
};

class SexyTransform2D : public SexyMatrix3 {
public:
    constexpr SexyTransform2D() noexcept {
        LoadIdentity();
    }

    explicit constexpr SexyTransform2D(bool loadIdentity) noexcept
        : SexyMatrix3{} {
        if (loadIdentity) {
            LoadIdentity();
        }
    }

    constexpr SexyTransform2D(const SexyMatrix3 &theMatrix) noexcept
        : SexyMatrix3{theMatrix} {}

    constexpr SexyTransform2D &operator=(const SexyMatrix3 &theMat) noexcept {
        SexyMatrix3::operator=(theMat);
        return *this;
    }

    constexpr void Translate(float tx, float ty) {
        SexyMatrix3 aMat;
        aMat.LoadIdentity();
        aMat.m02 = tx;
        aMat.m12 = ty;
        aMat.m22 = 1;

        *this = aMat * (*this);
    }

    void RotateRad(float rot);
    void RotateDeg(float rot);

    constexpr void Scale(float sx, float sy) {
        SexyMatrix3 aMat;
        aMat.LoadIdentity();
        aMat.m00 = sx;
        aMat.m11 = sy;

        *this = aMat * (*this);
    }
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_MISC_SEXY_MATRIX_H
