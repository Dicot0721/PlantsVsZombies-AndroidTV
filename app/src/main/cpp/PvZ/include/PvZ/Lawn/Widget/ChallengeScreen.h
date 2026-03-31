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

#include "HelpBarWidget.h"
#include "PvZ/Lawn/Board/ToolTipWidget.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/SexyAppFramework/Misc/Curve1DUtil.h"
#include "PvZ/SexyAppFramework/Misc/KeyCodes.h"
#include "PvZ/SexyAppFramework/Widget/Dialog.h"
#include "PvZ/Symbols.h"
#include "WaitForSecondPlayerDialog.h"

constexpr int NUM_CHALLENGE_MODES(int(GameMode::NUM_GAME_MODES - 1));
constexpr int GAMEMODE_MP_VS_DAY = 68;
constexpr int GAMEMODE_MP_VS_NIGHT = 69;
constexpr int GAMEMODE_MP_VS_POOL_DAY = 70;
constexpr int GAMEMODE_MP_VS_POOL_NIGHT = 71;
constexpr int GAMEMODE_MP_VS_ROOF = 72;
constexpr int GAMEMODE_MP_VS_SHUFFLE_MODE = 73;
constexpr int NUM_VS_MODES(GAMEMODE_MP_VS_SHUFFLE_MODE - GAMEMODE_MP_VS_DAY + 1);

class NewLawnButton;
class ChallengeScreen : public Sexy::Widget, public Sexy::ButtonListener {
private:
    enum {
        ChallengeScreen_Back = 100,
        ChallengeScreen_Mode = 200,
        ChallengeScreen_Page = 300,
    };

public:
    Sexy::ButtonWidget *mChallengeButtons[NUM_CHALLENGE_MODES]; // 65 ~ 158
    LawnApp *mApp;                                              // 159
    ToolTipWidget *mToolTip;                                    // 160
    ChallengePage mPageIndex;                                   // 161
    bool mCheatEnableChallenges;                                // 648
    UnlockingState mUnlockState;                                // 163
    int mUnlockStateCounter;                                    // 164
    int mUnlockChallengeIndex;                                  // 165
    float mLockShakeX;                                          // 166
    float mLockShakeY;                                          // 167
    Sexy::Curve1DUtil mUtil;                                    // 168 ~ 183
    HelpBarWidget *mHelpBarWidget;                              // 184
    int mScreenTopChallengeIndex;                               // 185
    int mSelectedChallengeIndex;                                // 186
    float mUnkFloat;                                            // 187
    GameMode mUnk1[NUM_CHALLENGE_MODES];                        // 188 ~ 281
    int mUnk2[NUM_CHALLENGE_MODES];                             // 282 ~ 375
    int mTotalGameInPage;                                       // 376
    int mSelectedChallenge;                                     // 377 其值固定比mSelectedMode小2
    GameMode mSelectedMode;                                     // 378
    int unk4;                                                   // 379
    // 大小380个整数, 以下是新增成员!
    NewLawnButton *mBackButton = nullptr;
    WaitForSecondPlayerDialog *mConnectDialog = nullptr;

    ChallengeScreen(LawnApp *theApp, ChallengePage thePage) {
        _constructor(theApp, thePage);
    }

    ~ChallengeScreen() = delete;

    void SetUnlockChallengeIndex(ChallengePage thePage, bool theIsIZombie = false) {
        reinterpret_cast<void (*)(ChallengeScreen *, ChallengePage, bool)>(ChallengeScreen_SetUnlockChallengeIndexAddr)(this, thePage, theIsIZombie);
    }
    void SetScrollTarget(int theIndex) {
        reinterpret_cast<void (*)(ChallengeScreen *, int)>(ChallengeScreen_SetScrollTargetAddr)(this, theIndex);
    }
    void DrawButton(Sexy::Graphics *g, int theChallengeIndex, int theChallengeMode) {
        reinterpret_cast<void (*)(ChallengeScreen *, Sexy::Graphics *, int, int)>(ChallengeScreen_DrawButtonAddr)(this, g, theChallengeIndex, theChallengeMode);
    }
    int MoreTrophiesNeeded(int theChallengeIndex) {
        return reinterpret_cast<int (*)(ChallengeScreen *, int)>(ChallengeScreen_MoreTrophiesNeededAddr)(this, theChallengeIndex);
    }

    void Draw(Sexy::Graphics *g);
    void AddedToManager(Sexy::WidgetManager *theWidgetManager);
    void Update();
    void RemovedFromManager(Sexy::WidgetManager *theWidgetManager);
    void ButtonPress(int theButtonId);
    void ButtonDepress(int theId);
    void UpdateButtons();
    void MouseDown(int x, int y, int theClickCount);
    void MouseUp(int x, int y);
    void MouseDrag(int x, int y);
    void KeyDown(Sexy::KeyCode theKey);
    static size_t getClientEventSize(EventType type);
    static size_t getServerEventSize(EventType type);
    void processClientEvent(void *buf, ssize_t bufSize);
    void processServerEvent(void *buf, ssize_t bufSize);

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
extern ChallengeDefinition gChallengeDefs[200];
extern ChallengeDefinition gVSChallengeDefs[NUM_VS_MODES];

ChallengeDefinition &GetChallengeDefinition(int theChallengeMode);
/***************************************************************************************************************/
inline int gChallengeScreenRequestState = 0;

inline void (*old_ChallengeScreen_ChallengeScreen)(ChallengeScreen *challengeScreen, LawnApp *lawnApp, ChallengePage page);

inline void (*old_ChallengeScreen_KeyDown)(ChallengeScreen *challengeScreen, Sexy::KeyCode code);

inline void (*old_ChallengeScreen_Draw)(ChallengeScreen *challengeScreen, Sexy::Graphics *graphics);

inline void (*old_ChallengeScreen_AddedToManager)(ChallengeScreen *a, Sexy::WidgetManager *a2);

inline void (*old_ChallengeScreen_Update)(ChallengeScreen *a);

inline void (*old_ChallengeScreen_RemovedFromManager)(ChallengeScreen *a, Sexy::WidgetManager *a2);

inline void (*old_ChallengeScreen_Delete2)(ChallengeScreen *challengeScreen);

inline void (*old_ChallengeScreen_MouseDown)(ChallengeScreen *challengeScreen, int x, int y, int theClickCount);

inline void (*old_ChallengeScreen_MouseDrag)(ChallengeScreen *challengeScreen, int x, int y);

inline void (*old_ChallengeScreen_MouseUp)(ChallengeScreen *challengeScreen, int x, int y);

#endif // PVZ_LAWN_WIDGET_CHALLENGE_SCREEN_H
