//
// Created by 28636 on 2025/5/26.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_WIDGET_H
#define PLANTSVSZOMBIES_ANDROIDTV_WIDGET_H

#include "PvZ/SexyAppFramework/Misc/Common.h"
#include "PvZ/SexyAppFramework/Graphics/Color.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/SexyAppFramework/Misc/KeyCodes.h"

#include "Insets.h"
#include "WidgetContainer.h"

namespace Sexy {

class Widget : public WidgetContainer {
public:
    bool mVisible;               // 116
    bool mMouseVisible;          // 117
    bool mDisabled;              // 118
    bool mHasFocus;              // 119
    bool mIsDown;                // 120
    bool mIsOver;                // 121
    bool mHasTransparencies;     // 122
    int mColorsUnk[2];           // 32 ~ 33
    Insets mMouseInsets;         // 34 ~ 37
    bool mDoFinger;              // 152
    bool mWantsFocus;            // 153
    int unk1[11];                // 38 ~ 48
    Widget *mFocusLinks[4];      // 49 ~ 52 ，上下左右
    int unk2[2];                 // 53 ~ 54
    Widget *mFocusedChildWidget; // 55
    int unk3[4];                 // 56 ~ 59
    int *mAnimatorForState[4];   // 60 ~ 63
}; // 大小64个整数！

} // namespace Sexy


#endif // PLANTSVSZOMBIES_ANDROIDTV_WIDGET_H
