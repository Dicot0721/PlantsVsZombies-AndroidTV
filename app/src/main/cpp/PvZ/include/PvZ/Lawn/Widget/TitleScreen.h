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

#ifndef PVZ_LAWN_WIDGET_TITLE_SCREEN_H
#define PVZ_LAWN_WIDGET_TITLE_SCREEN_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/SexyAppFramework/Widget/ButtonListener.h"
#include "PvZ/SexyAppFramework/Widget/Widget.h"

class TitleScreen : public Sexy::Widget, public Sexy::ButtonListener {
public:
    enum TitleState {
        WaitingForFirstDraw = 0,
        PopcapLogo_OR_IntroVideo = 1,
        ESRBLogo = 2, // 在游戏中被弃用
        GuideLogo = 3,
        Loading = 4
    };

    Sexy::Image *mPopcapLogo;    // 65
    Sexy::Image *mGuide;         // 66
    Sexy::Widget *mStartButton;  // 67
    float mCurBarWidth;          // 68
    float mTotalBarWidth;        // 69
    float mBarVel;               // 70
    float mBarStartProgress;     // 71
    bool mRegisterClicked;       // 288
    bool mLoadingThreadComplete; // 289
    int mTitleAge;               // 73
    bool mNeedRegister;
    bool mNeedShowRegisterBox;
    bool mNeedToInit;
    float mPrevLoadingPercent;  // 75
    TitleState mTitleState;     // 76
    int mTitleStateCounter;     // 77
    int mTitleStateDuration;    // 78
    int mIntroVideoAge;         // 79
    bool mVideoCompleted;       // 80 * 4 = 320
    bool mLoaderScreenIsLoaded; // 321
    bool unkBool2;              // 322
    bool mIsPlayingIntroVideo;  // 323
    int unk2[2];
    bool mNeedPlayLogo; // 4 * 83
    LawnApp *mApp;      // 84

    TitleScreen(LawnApp *theApp) {
        _constructor(theApp);
    }

protected:
    friend void InitHookFunction();

    void _constructor(LawnApp *theApp);

}; // 大小85个整数

inline void (*old_TitleScreen_Draw)(TitleScreen *titleScreen, Sexy::Graphics *a2);

inline void (*old_TitleScreen_Update)(TitleScreen *titleScreen);

inline void (*old_TitleScreen_TitleScreen)(TitleScreen *titleScreen, LawnApp *);


void TitleScreen_Draw(TitleScreen *titleScreen, Sexy::Graphics *graphics);

void TitleScreen_Update(TitleScreen *titleScreen);

void TitleScreen_SwitchState(TitleScreen *titleScreen, TitleScreen::TitleState state, int duration);

#endif // PVZ_LAWN_WIDGET_TITLE_SCREEN_H
