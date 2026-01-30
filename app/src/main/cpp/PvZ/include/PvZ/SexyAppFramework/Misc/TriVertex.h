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

#ifndef PVZ_SEXYAPPFRAMEWORK_MISC_TRI_VERTEX_H
#define PVZ_SEXYAPPFRAMEWORK_MISC_TRI_VERTEX_H

#include <cstdint>

namespace Sexy {

class TriVertex {
public:
    float x, y, u, v;
    uint32_t color = 0;

    constexpr TriVertex() = default;

    constexpr TriVertex(float theX, float theY) noexcept
        : x{theX}
        , y{theY} {}

    constexpr TriVertex(float theX, float theY, float theU, float theV) noexcept
        : x{theX}
        , y{theY}
        , u{theU}
        , v{theV} {}

    constexpr TriVertex(float theX, float theY, float theU, float theV, uint32_t theColor) noexcept
        : x{theX}
        , y{theY}
        , u{theU}
        , v{theV}
        , color{theColor} {}
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_MISC_TRI_VERTEX_H
