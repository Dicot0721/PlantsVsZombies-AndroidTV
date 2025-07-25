//
// Created by 28636 on 2025/5/26.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_LAWNDIALOG_H
#define PLANTSVSZOMBIES_ANDROIDTV_LAWNDIALOG_H

#include "PvZ/SexyAppFramework/Widget/Dialog.h"
// #include "PvZ/SexyAppFramework/Widget/GameButton.h"

namespace Sexy {
class GameButton;
}

class LawnDialog : public Sexy::Dialog {
public:
    LawnApp *mApp; // 184
#ifdef VERSION_111
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
#ifdef VERSION_111
    int unk2;
#endif
}; // 115: 191, 111: 193


#endif // PLANTSVSZOMBIES_ANDROIDTV_LAWNDIALOG_H
