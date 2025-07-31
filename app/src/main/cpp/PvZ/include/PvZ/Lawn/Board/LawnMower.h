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

#ifndef PVZ_LAWN_BOARD_LAWN_MOWER_H
#define PVZ_LAWN_BOARD_LAWN_MOWER_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Symbols.h"

class LawnApp;
class Board;
class Zombie;
class LawnMower {
public:
    LawnApp* mApp;              //+0x0
    Board* mBoard;              //+0x4
    float mPosX;                //+0x8
    float mPosY;                //+0xC
    int mRenderOrder;           //+0x10
    int mRow;                   //+0x14
    int mAnimTicksPerFrame;     //+0x18
    ReanimationID mReanimID;    //+0x1C
    int mChompCounter;          //+0x20
    int mRollingInCounter;      //+0x24
    int mSquishedCounter;       //+0x28
    LawnMowerState mMowerState; //+0x2C
    bool mDead;                 //+0x30
    bool mVisible;              //+0x31
    LawnMowerType mMowerType;   //+0x34
    float mAltitude;            //+0x38
    MowerHeight mMowerHeight;   //+0x3C
    int mLastPortalX;           //+0x40

    void StartMower() {
        reinterpret_cast<void (*)(LawnMower*)>(LawnMower_StartMowerAddr)(this);
    }

    void Update();
};

inline void (*old_LawnMower_Update)(LawnMower* lawnMover);

#endif // PVZ_LAWN_BOARD_LAWN_MOWER_H
