//
// Created by 28636 on 2025/5/26.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_CREDITSCREEN_H
#define PLANTSVSZOMBIES_ANDROIDTV_CREDITSCREEN_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/SexyAppFramework/Widget/Widget.h"
// #include "../../SexyAppFramework/PerfTimer.h"
#include "PvZ/SexyAppFramework/Misc/PerfTimer.h"
#include "PvZ/SexyAppFramework/Widget/ButtonListener.h"
#include "PvZ/SexyAppFramework/Widget/GameButton.h"

class CreditScreen : public Sexy::Widget {
public:
    enum CreditsPhase {
        CREDITS_MAIN1 = 0,
        CREDITS_MAIN2 = 1,
        CREDITS_MAIN3 = 2,
        CREDITS_END = 3,
    };

public:
    Sexy::ButtonListener mButtonListener; // 64
    LawnApp *mApp;                        // 65
    CreditsPhase mCreditsPhase;           // 66
    int mCreditsPhaseCounter;             // 67
    int mCreditsReanimID;                 // 68
    int mFogParticleID;                   // 69
    int mBlinkCountdown;                  // 70
    Sexy::Widget *mOverlayWidget;         // 71
    bool mDrawBrain;                      // 288
    float mBrainPosX;                     // 73
    float mBrainPosY;                     // 74
    int mUpdateCount;                     // 75
    int mDrawCount;                       // 76
    int unkInt1;                          // 77
    Sexy::PerfTimer mTimerSinceStart;     // 78 ~ 82
    int unkInt2;                          // 83
    bool mDontSync;                       // 336
    bool mCreditsPaused;                  // 337
    int unkInt3[3];                       // 85 ~ 87
    bool mPreloaded;                      // 352
    int unkInt4;                          // 89
    double mScrollPositionY1;             // 90 ~ 91
    double mScrollPositionY2;             // 92 ~ 93
    bool mIsFromMainMenu;                 // 376,即94
    // 大小95个整数

    void PauseCredits() {
        reinterpret_cast<void (*)(CreditScreen *)>(CreditScreen_PauseCreditsAddr)(this);
    }

    void ButtonDepress(int theId);
};

static Sexy::GameButton *gCreditScreenBackButton;

inline void (*old_CreditScreen_CreditScreen)(Sexy::Widget *a, LawnApp *a2, bool a3);

inline void (*old_CreditScreen_RemovedFromManager)(Sexy::Widget *creditScreen, int a2);

inline void (*old_CreditScreen_Delete2)(Sexy::Widget *creditScreen);

void CreditScreen_CreditScreen(Sexy::Widget *creditScreen, LawnApp *a2, bool a3);

void CreditScreen_RemovedFromManager(Sexy::Widget *creditScreen, int a2);

void CreditScreen_Delete2(Sexy::Widget *creditScreen);
#endif // PLANTSVSZOMBIES_ANDROIDTV_CREDITSCREEN_H
