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

#ifndef PVZ_LAWN_BOARD_GRID_ITEM_H
#define PVZ_LAWN_BOARD_GRID_ITEM_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/Symbols.h"

class Board;
class Reanimation;

class MotionTrailFrame {
public:
    float mPosX;
    float mPosY;
    float mAnimTime;
};

class GridItem {
public:
    int placeHolder[4];
    LawnApp *mApp;                           // 4
    Board *mBoard;                           // 5
    GridItemType mGridItemType;              // 6
    GridItemState mGridItemState;            // 7
    int mGridX;                              // 8
    int mGridY;                              // 9
    int mGridItemCounter;                    // 10
    int mRenderOrder;                        // 11
    bool mDead;                              // 48
    float mPosX;                             // 13
    float mPosY;                             // 14
    float mGoalX;                            // 15
    float mGoalY;                            // 16
    ReanimationID mGridItemReanimID;         // 17
    ParticleSystemID mGridItemParticleID;    // 18
    ZombieType mZombieType;                  // 19
    SeedType mSeedType;                      // 20
    ScaryPotType mScaryPotType;              // 21
    bool mHighlighted;                       // 88
    int mTransparentCounter;                 // 23
    int mSunCount;                           // 24
    MotionTrailFrame mMotionTrailFrames[12]; // 25 ~ 60
    int mMotionTrailCount;                   // 61
    bool unkBool;                            // 62 * 4
    bool unkBool1;                           // 62 * 4 + 1
    int unkMems[3];                          // 63 ~ 65
    int mLaunchCounter;                      // 66
    int mLaunchRate;                         // 67
    int mJustGotShotCounter;                 // 68
    int mVSGraveStoneHealth;                 // 69
    int mVSTargetZombieHealth;               // 70
    int unkMems2[4];                         // 71 ~ 74
    // 大小75个整数

    void GridItemDie() {
        reinterpret_cast<void (*)(GridItem *)>(GridItem_GridItemDieAddr)(this);
    }
    void UpdateBurialMound() {
        reinterpret_cast<void (*)(GridItem *)>(GridItem_UpdateBurialMoundAddr)(this);
    }

    void DrawScaryPot(Sexy::Graphics *g);
    void Update();
    void UpdateScaryPot();
    void DrawStinky(Sexy::Graphics *g);
    void DrawSquirrel(Sexy::Graphics *g);
    void DrawCrater(Sexy::Graphics *g);
};

/***************************************************************************************************************/
inline bool transparentVase;


inline void (*old_GridItem_Update)(GridItem *a1);

inline void (*old_GridItem_UpdateScaryPot)(GridItem *scaryPot);

inline void (*old_GridItem_DrawStinky)(GridItem *mStinky, Sexy::Graphics *graphics);

#endif // PVZ_LAWN_BOARD_GRID_ITEM_H
