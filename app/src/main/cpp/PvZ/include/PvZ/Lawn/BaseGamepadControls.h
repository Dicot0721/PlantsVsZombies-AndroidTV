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

#ifndef PVZ_LAWN_BASE_GAMEPAD_CONTROLS_H
#define PVZ_LAWN_BASE_GAMEPAD_CONTROLS_H

#include "PvZ/Lawn/Board/GameObject.h"

class SeedBank;

class BaseGamepadControls {
public:
    int *mVtable;                    // 0
    GameObject mGameObject;          // 1 ~ 13
    Board *mBoard;                   // 14
    int unknown_always_1;            // 15
    float mUnkFirstHighlightRelated; // 16
    int mShovelRelated1;             // 17
    float mGamepadAccLeftX;
    float mGamepadAccLeftY;
    float mGridCenterPositionX;    // 20
    float mGridCenterPositionY;    // 21
    float mRightPositionX;         // 22
    float mRightPositionY;         // 23
    int mGamepadState;             // 24
    int unk1;                      // 25
    float mDigIndicatorPercentage; // 26 , 每按下一次铲除键就加2.3
    float mCursorPositionX;        // 27
    float mCursorPositionY;        // 28
    float mGamepadVelocityLeftX;   // 29
    float mGamepadVelocityLeftY;   // 30
    float mGamepadVelocityRightX;  // 31
    float mGamepadVelocityRightY;  // 32
    int unk2[4];                   // 33 ~ 36
    int mPlayerIndex1;             // 37，P1/P2
    int mPlayerIndex2;             // 38，与mPlayerIndex完全相等，没有区别
    float mCursorPositionYJitter;  // 39
    float mUpdateAdd0Dot016;       // 40
    int unkMems[2];                // 41 ~ 42
    // 大小43个整数

    void GetGamepadVelocity(float *horizontal, float *vertical);

    BaseGamepadControls() = delete;
    ~BaseGamepadControls() = delete;
};

#endif // PVZ_LAWN_BASE_GAMEPAD_CONTROLS_H
