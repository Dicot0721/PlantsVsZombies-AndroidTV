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
    float x;
    float y;
    uint32_t offset8;
    float offsetC;
    uint32_t color = 0; // ARGB (0 = use color specified in function call)
    uint32_t offset14;
    float u;
    float v;

    constexpr TriVertex() = default;
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_MISC_TRI_VERTEX_H
