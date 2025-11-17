/*
 * Copyright (C) 2023-2025  PvZ TV Touch Team
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

#ifndef PVZ_COMMON_GAME_CONSTANTS_H
#define PVZ_COMMON_GAME_CONSTANTS_H

#include "ConstEnums.h"
constexpr const double PI = 3.141592653589793;

// ============================================================
// ■ 常数
// ============================================================
const int BOARD_WIDTH = 800;
const int BOARD_HEIGHT = 600;
const int WIDE_BOARD_WIDTH = 800;
const int BOARD_OFFSET = 220;
const int BOARD_EDGE = -100;
const int BOARD_IMAGE_WIDTH_OFFSET = 1180;
const int BOARD_ICE_START = 800;
const int LAWN_XMIN = 40;
const int LAWN_YMIN = 80;
const int HIGH_GROUND_HEIGHT = 30;

const int SEEDBANK_MAX = 10;
const int SEED_BANK_OFFSET_X = 0;
const int SEED_BANK_OFFSET_X_END = 10;
const int SEED_CHOOSER_OFFSET_Y = 516;
const int SEED_PACKET_WIDTH = 50;
const int SEED_PACKET_HEIGHT = 70;
const int IMITATER_DIALOG_WIDTH = 500;
const int IMITATER_DIALOG_HEIGHT = 600;

// ============================================================
// ■ 关卡相关
// ============================================================
const int ADVENTURE_AREAS = 5;
const int LEVELS_PER_AREA = 10;
const int NUM_LEVELS = ADVENTURE_AREAS * LEVELS_PER_AREA;
const int FINAL_LEVEL = NUM_LEVELS;
const int FLAG_RAISE_TIME = 100;
const int LAST_STAND_FLAGS = 5;
const int ZOMBIE_COUNTDOWN_FIRST_WAVE = 1800;
const int ZOMBIE_COUNTDOWN = 2500;
const int ZOMBIE_COUNTDOWN_RANGE = 600;
const int ZOMBIE_COUNTDOWN_BEFORE_FLAG = 4500;
const int ZOMBIE_COUNTDOWN_BEFORE_REPICK = 5499;
const int ZOMBIE_COUNTDOWN_MIN = 400;
const int FOG_BLOW_RETURN_TIME = 2000;
const int SUN_COUNTDOWN = 425;
const int SUN_COUNTDOWN_RANGE = 275;
const int SUN_COUNTDOWN_MAX = 950;
const int SURVIVAL_NORMAL_FLAGS = 5;
const int SURVIVAL_HARD_FLAGS = 10;

// ============================================================
// ■ 商店相关
// ============================================================
const int STORESCREEN_ITEMOFFSET_1_X = 422;
const int STORESCREEN_ITEMOFFSET_1_Y = 206;
const int STORESCREEN_ITEMOFFSET_2_X = 372;
const int STORESCREEN_ITEMOFFSET_2_Y = 310;
const int STORESCREEN_ITEMSIZE = 74;
const int STORESCREEN_COINBANK_X = 650;
const int STORESCREEN_COINBANK_Y = 559;
const int STORESCREEN_PAGESTRING_X = 470;
const int STORESCREEN_PAGESTRING_Y = 500;

#endif // PVZ_COMMON_GAME_CONSTANTS_H
