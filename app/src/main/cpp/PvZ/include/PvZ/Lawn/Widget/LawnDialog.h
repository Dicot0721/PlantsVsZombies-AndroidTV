#ifndef PVZ_LAWN_WIDGET_LAWN_DIALOG_H
#define PVZ_LAWN_WIDGET_LAWN_DIALOG_H

#include "PvZ/SexyAppFramework/Widget/Dialog.h"
// #include "PvZ/SexyAppFramework/Widget/GameButton.h"

namespace Sexy {
class GameButton;
}

class LawnDialog : public Sexy::Dialog {
public:
    LawnApp *mApp; // 184
#if PVZ_VERSION == 111
    int mIsZombie;
#endif
    int mButtonDelay;                 // 185
    Sexy::Widget *mReanimationWidget; // 186
    bool mDrawStandardBack;           // 748
    Sexy::GameButton *mLawnYesButton; // 188
    Sexy::GameButton *mLawnNoButton;  // 189
    bool mTallBottom;                 // 760
    bool mVerticalCenterText;         // 761
    bool unkBool;                     // 762
#if PVZ_VERSION == 111
    int unk2;
#endif
}; // 115: 191, 111: 193

#endif // PVZ_LAWN_WIDGET_LAWN_DIALOG_H
