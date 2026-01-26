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

#ifndef PVZ_LAWN_BOARD_CUT_SCENE_H
#define PVZ_LAWN_BOARD_CUT_SCENE_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/SexyAppFramework/Misc/KeyCodes.h"
#include "PvZ/Symbols.h"
#include "Zombie.h"

class LawnApp;
class Board;
class ChallengeScreen;

class CutScene {
public:
    LawnApp *mApp;                           // 0
    Board *mBoard;                           // 1
    int mCutsceneTime;                       // 2
    int mSodTime;                            // 3
    int mGraveStoneTime;                     // 4
    int mReadySetPlantTime;                  // 5
    int mFogTime;                            // 6
    int mBossTime;                           // 7
    int mCrazyDaveTime;                      // 8
    int mLawnMowerTime;                      // 9
    int mCrazyDaveDialogStart;               // 10
    bool mSeedChoosing;                      // 44
    ReanimationID mZombiesWonReanimID;       // 12
    bool mPreloaded;                         // 52
    bool mPlacedZombies;                     // 53
    bool mPlacedLawnItems;                   // 54
    int mCrazyDaveCountDown;                 // 14【推销戴夫倒计时】
    int mCrazyDaveLastTalkIndex;             // 15【戴夫推销的话的编号】
    bool mUpsellHideBoard;                   // 64【推销隐藏关卡界面】
    ChallengeScreen *mUpsellChallengeScreen; // 17【推销插入的小游戏界面】
    bool mPreUpdatingBoard;                  // 72【场景布置阶段的关卡预更新】
    ReanimationID mPlantsWonReanim;
    Zombie *mZombieWonZombie;
    int unk[9]; // 21 ~ 29
    // 大小30个整数

    bool IsSurvivalRepick() {
        return reinterpret_cast<bool (*)(CutScene *)>(CutScene_IsSurvivalRepickAddr)(this);
    }
    void OnKeyDown(Sexy::KeyCode theKey, unsigned int a3) {
        reinterpret_cast<bool (*)(CutScene *, Sexy::KeyCode, unsigned int)>(CutScene_OnKeyDownAddr)(this, theKey, a3);
    }
    bool IsBeforePreloading() {
        return reinterpret_cast<bool (*)(CutScene *)>(CutScene_IsBeforePreloadingAddr)(this);
    }
    bool ShouldRunUpsellBoard() {
        return reinterpret_cast<bool (*)(CutScene *)>(CutScene_ShouldRunUpsellBoardAddr)(this);
    }

    void MouseDown(int x, int y) {
        reinterpret_cast<bool (*)(CutScene *)>(CutScene_MouseDownAddr)(this);
    }

    void ShowShovel();
    void Update();
    void UpdateZombiesWonMP();
    void UpdatePlantsWon();
    void PlaceLawnItems();
    void AddFlowerPots();
};


inline void (*old_CutScene_ShowShovel)(CutScene *cutScene);

inline void (*old_CutScene_Update)(CutScene *instance);

inline void (*old_CutScene_PlaceLawnItems)(CutScene *cutScene);

inline void (*old_CutScene_AddFlowerPots)(CutScene *cutScene);

#endif // PVZ_LAWN_BOARD_CUT_SCENE_H
