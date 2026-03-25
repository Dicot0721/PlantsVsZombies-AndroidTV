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

#ifndef PVZ_FORMATION_H
#define PVZ_FORMATION_H

#include <string>

class Board;

namespace formation {

std::string_view GetBuiltinFormationStr(std::size_t theIndex) noexcept;

std::string GenerateFormationStr(Board *theBoard);

void ApplyFormation(Board *theBoard, std::string_view theFormationStr);

} // namespace formation

#endif // PVZ_FORMATION_H
