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

#include "PvZ/Symbols.h"

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

    SexyMatrix3() = default; // so 中存在对应的空函数
};

class SexyTransform2D : public SexyMatrix3 {
public:
    SexyTransform2D() {
        reinterpret_cast<void (*)(SexyTransform2D *)>(Sexy_SexyTransform2D_SexyTransform2DAddr)(this);
    }

    void Scale(float sx, float sy) {
        reinterpret_cast<void (*)(SexyTransform2D *, float, float)>(Sexy_SexyTransform2D_ScaleAddr)(this, sx, sy);
    }
    void Translate(float tx, float ty) {
        reinterpret_cast<void (*)(SexyTransform2D *, float, float)>(Sexy_SexyTransform2D_TranslateAddr)(this, tx, ty);
    }
    void RotateRad(float rot) {
        reinterpret_cast<void (*)(SexyTransform2D *, float)>(Sexy_SexyTransform2D_RotateRadAddr)(this, rot);
    }
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_MISC_SEXY_MATRIX_H
