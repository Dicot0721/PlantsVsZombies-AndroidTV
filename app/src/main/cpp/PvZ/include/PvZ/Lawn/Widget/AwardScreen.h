#ifndef PVZ_LAWN_WIDGET_AWARD_SCREEN_H
#define PVZ_LAWN_WIDGET_AWARD_SCREEN_H

#include "PvZ/SexyAppFramework/Widget/Widget.h"
#include "PvZ/Symbols.h"

namespace Sexy {
class GameButton;
}

class AwardScreen : public Sexy::Widget {
public:
    int unkInt64;                   // 64
    Sexy::GameButton *mStartButton; // 65
    Sexy::GameButton *mMenuButton;  // 66
    LawnApp *mApp;                  // 67
    int mFadeInCounter;             // 68
    AwardType mAwardType;           // 69

    void StartButtonPressed() {
        reinterpret_cast<void (*)(AwardScreen *)>(AwardScreen_StartButtonPressedAddr)(this);
    }

    void MouseDown(int x, int y, int theClickCount);
    void MouseUp(int x, int y, int theClickCount);
};


inline void (*old_AwardScreen_MouseDown)(AwardScreen *awardScreen, int x, int y, int theClickCount);

inline void (*old_AwardScreen_MouseUp)(AwardScreen *awardScreen, int x, int y, int theClickCount);

#endif // PVZ_LAWN_WIDGET_AWARD_SCREEN_H
