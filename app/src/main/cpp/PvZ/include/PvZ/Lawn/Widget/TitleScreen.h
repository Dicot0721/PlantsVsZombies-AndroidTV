#ifndef PVZ_LAWN_TITLE_SCREEN_H
#define PVZ_LAWN_TITLE_SCREEN_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/SexyAppFramework/Widget/ButtonListener.h"
#include "PvZ/SexyAppFramework/Widget/Widget.h"

class TitleScreen : public Sexy::Widget {

public:
    enum TitleState {
        WaitingForFirstDraw = 0,
        PopcapLogo_OR_IntroVideo = 1,
        ESRBLogo = 2, // 在游戏中被弃用
        GuideLogo = 3,
        Loading = 4
    };

public:
    Sexy::ButtonListener mButtonListener; // 64
    Sexy::Image *mPopcapLogo;             // 65
    Sexy::Image *mGuide;                  // 66
    Sexy::Widget *mStartButton;           // 67
    float mCurBarWidth;                   // 68
    float mTotalBarWidth;                 // 69
    float mBarVel;                        // 70
    float mBarStartProgress;              // 71
    bool mRegisterClicked;                // 288
    bool mLoadingThreadComplete;          // 289
    int mTitleAge;                        // 73
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
}; // 大小85个整数

inline void (*old_TitleScreen_Draw)(TitleScreen *titleScreen, Sexy::Graphics *a2);

inline void (*old_TitleScreen_Update)(TitleScreen *titleScreen);


void TitleScreen_Draw(TitleScreen *titleScreen, Sexy::Graphics *graphics);

void TitleScreen_Update(TitleScreen *titleScreen);

void TitleScreen_SwitchState(TitleScreen *titleScreen, TitleScreen::TitleState state, int duration);

#endif // PVZ_LAWN_TITLE_SCREEN_H
