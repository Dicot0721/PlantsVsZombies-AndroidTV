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

#ifndef PVZ_COMMON_GAME_CONSTANTS_H
#define PVZ_COMMON_GAME_CONSTANTS_H

#include "ConstEnums.h"

// ============================================================
// ■ 常数
// ============================================================
inline constexpr int BOARD_WIDTH = 800;
inline constexpr int BOARD_HEIGHT = 600;
inline constexpr int WIDE_BOARD_WIDTH = 800;
inline constexpr int BOARD_OFFSET = 220;
inline constexpr int BOARD_EDGE = -100;
inline constexpr int BOARD_IMAGE_WIDTH_OFFSET = 1180;
inline constexpr int BOARD_ICE_START = 800;
inline constexpr int LAWN_XMIN = 40;
inline constexpr int LAWN_YMIN = 80;
inline constexpr int HIGH_GROUND_HEIGHT = 30;

inline constexpr int SEEDBANK_MAX = 10;
inline constexpr int SEED_BANK_OFFSET_X = 0;
inline constexpr int SEED_BANK_OFFSET_X_END = 10;
inline constexpr int SEED_CHOOSER_OFFSET_Y = 516;
inline constexpr int SEED_PACKET_WIDTH = 50;
inline constexpr int SEED_PACKET_HEIGHT = 70;
inline constexpr int IMITATER_DIALOG_WIDTH = 500;
inline constexpr int IMITATER_DIALOG_HEIGHT = 600;

// ============================================================
// ■ 关卡相关
// ============================================================
inline constexpr int ADVENTURE_AREAS = 5;
inline constexpr int LEVELS_PER_AREA = 10;
inline constexpr int NUM_LEVELS = ADVENTURE_AREAS * LEVELS_PER_AREA;
inline constexpr int FINAL_LEVEL = NUM_LEVELS;
inline constexpr int FLAG_RAISE_TIME = 100;
inline constexpr int LAST_STAND_FLAGS = 5;
inline constexpr int ZOMBIE_COUNTDOWN_FIRST_WAVE = 1800;
inline constexpr int ZOMBIE_COUNTDOWN = 2500;
inline constexpr int ZOMBIE_COUNTDOWN_RANGE = 600;
inline constexpr int ZOMBIE_COUNTDOWN_BEFORE_FLAG = 4500;
inline constexpr int ZOMBIE_COUNTDOWN_BEFORE_REPICK = 5499;
inline constexpr int ZOMBIE_COUNTDOWN_MIN = 400;
inline constexpr int FOG_BLOW_RETURN_TIME = 2000;
inline constexpr int SUN_COUNTDOWN = 425;
inline constexpr int SUN_COUNTDOWN_RANGE = 275;
inline constexpr int SUN_COUNTDOWN_MAX = 950;
inline constexpr int SURVIVAL_NORMAL_FLAGS = 5;
inline constexpr int SURVIVAL_HARD_FLAGS = 10;

// ============================================================
// ■ 商店相关
// ============================================================
inline constexpr int STORESCREEN_ITEMOFFSET_1_X = 422;
inline constexpr int STORESCREEN_ITEMOFFSET_1_Y = 206;
inline constexpr int STORESCREEN_ITEMOFFSET_2_X = 372;
inline constexpr int STORESCREEN_ITEMOFFSET_2_Y = 310;
inline constexpr int STORESCREEN_ITEMSIZE = 74;
inline constexpr int STORESCREEN_COINBANK_X = 650;
inline constexpr int STORESCREEN_COINBANK_Y = 559;
inline constexpr int STORESCREEN_PAGESTRING_X = 470;
inline constexpr int STORESCREEN_PAGESTRING_Y = 500;

#endif // PVZ_COMMON_GAME_CONSTANTS_H
