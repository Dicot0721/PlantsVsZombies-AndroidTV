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

#include "PvZ/Lawn/BaseGamepadControls.h"
#include "PvZ/GlobalVariable.h"

void __BaseGamepadControls::GetGamepadVelocity(float *horizontal, float *vertical) {
    // 如果horizontal或vertical不为零，则会在重型武器中播放小推车移动动画。
    // 所以这里将vertical设置为非零，以播放动画；同时horizontal设置为0，以免小推车移动。
    *horizontal = 0.0f;
    *vertical = gPlayerIndex == TouchPlayerIndex::TOUCHPLAYER_NONE ? 0.0f : 100.0f;
}