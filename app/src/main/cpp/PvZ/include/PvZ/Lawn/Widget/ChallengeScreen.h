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

#ifndef PVZ_LAWN_WIDGET_CHALLENGE_SCREEN_H
#define PVZ_LAWN_WIDGET_CHALLENGE_SCREEN_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/SexyAppFramework/Misc/KeyCodes.h"
#include "PvZ/SexyAppFramework/Widget/Dialog.h"
#include "PvZ/Symbols.h"

#define NUM_CHALLENGE_MODES ((int)GameMode::NUM_GAME_MODES - 1)

struct Curve1DUtil {
    int unk[16];
};

class ChallengeScreen : public Sexy::Widget, public Sexy::ButtonListener {
public:
    Sexy::ButtonWidget *mButtons[94];     // 65 ~ 158
    LawnApp *mApp;                        // 159
    int *mToolTipWidget;                  // 160
    ChallengePage mPageIndex;             // 161
    bool mCheatEnableChallenges;          // 648
    UnlockingState mUnlockState;          // 163
    int mUnlockStateCounter;              // 164
    int mUnlockChallengeIndex;            // 165
    float mLockShakeX;                    // 166
    float mLockShakeY;                    // 167
    Curve1DUtil mUtil;                    // 168 ~ 183
    int *mHelpBarWidget;                  // 184
    int mScreenTopChallengeIndex;         // 185
    int mSelectedChallengeIndex;          // 186
    float mUnkFloat;                      // 187
    GameMode mUnk1[94];                   // 188 ~ 281
    int mUnk2[94];                        // 282 ~ 375
    int mTotalGameInPage;                 // 376
    int mSelectedChallenge;               // 377 其值固定比mSelectedMode小2
    GameMode mSelectedMode;               // 378
    int unk4;                             // 379
    // 大小380个整数

    void SetUnlockChallengeIndex(ChallengePage thePage, bool theIsIZombie = false) {
        reinterpret_cast<void (*)(ChallengeScreen *, ChallengePage, bool)>(ChallengeScreen_SetUnlockChallengeIndexAddr)(this, thePage, theIsIZombie);
    }
    void SetScrollTarget(int theIndex) {
        reinterpret_cast<void (*)(ChallengeScreen *, int)>(ChallengeScreen_SetScrollTargetAddr)(this, theIndex);
    }
    void KeyDown(Sexy::KeyCode theKey) {
        reinterpret_cast<void (*)(ChallengeScreen *, Sexy::KeyCode)>(ChallengeScreen_KeyDownAddr)(this, theKey);
    }

    ChallengeScreen(LawnApp *theApp, ChallengePage thePage);
    void Draw(Sexy::Graphics *graphics);
    void AddedToManager(int *theWidgetManager);
    void Update();
    void RemovedFromManager(int *theWidgetManager);
    void ButtonPress(int theButtonId);
    void ButtonDepress(int theId);
    void UpdateButtons();

    void MouseDown(int x, int y, int theClickCount);
    void MouseUp(int x, int y);
    void MouseDrag(int x, int y);

protected:
    friend void InitHookFunction();

    void _constructor(LawnApp *theApp, ChallengePage thePage);
    void _destructor2();
};

class ChallengeDefinition {
public:
    GameMode mChallengeMode;    // 0
    int mChallengeIconIndex;    // 1
    ChallengePage mPage;        // 2
    int mRow;                   // 3 无用
    int mCol;                   // 4 无用
    const char *mChallengeName; // 5
};
extern ChallengeDefinition gChallengeDefs[NUM_CHALLENGE_MODES];

ChallengeDefinition &GetChallengeDefinition(int theChallengeMode);
/***************************************************************************************************************/


inline void (*old_ChallengeScreen_ChallengeScreen)(ChallengeScreen *challengeScreen, LawnApp *lawnApp, ChallengePage page);

inline void (*old_ChallengeScreen_Draw)(ChallengeScreen *challengeScreen, Sexy::Graphics *graphics);

inline void (*old_ChallengeScreen_AddedToManager)(ChallengeScreen *a, int *a2);

inline void (*old_ChallengeScreen_Update)(ChallengeScreen *a);

inline void (*old_ChallengeScreen_RemovedFromManager)(ChallengeScreen *a, int *a2);

inline void (*old_ChallengeScreen_Delete2)(ChallengeScreen *challengeScreen);

inline ChallengeDefinition &(*old_GetChallengeDefinition)(int index);

inline void (*old_ChallengeScreen_MouseDown)(ChallengeScreen *challengeScreen, int x, int y, int theClickCount);

inline void (*old_ChallengeScreen_MouseDrag)(ChallengeScreen *challengeScreen, int x, int y);

inline void (*old_ChallengeScreen_MouseUp)(ChallengeScreen *challengeScreen, int x, int y);

#endif // PVZ_LAWN_WIDGET_CHALLENGE_SCREEN_H
