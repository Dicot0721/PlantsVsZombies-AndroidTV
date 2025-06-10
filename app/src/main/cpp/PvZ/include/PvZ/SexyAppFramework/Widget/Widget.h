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

//class WidgetManager;

template <bool IS_IN_DERIVED = false>
class __Widget : public __WidgetContainer<true> {
public:
    bool mVisible;                 // 116
    bool mMouseVisible;            // 117
    bool mDisabled;                // 118
    bool mHasFocus;                // 119
    bool mIsDown;                  // 120
    bool mIsOver;                  // 121
    bool mHasTransparencies;       // 122
    int mColorsUnk[2];             // 32 ~ 33
    Insets mMouseInsets;           // 34 ~ 37
    bool mDoFinger;                // 152
    bool mWantsFocus;              // 153
    int unk1[11];                  // 38 ~ 48
    __Widget *mFocusLinks[4];      // 49 ~ 52 ，上下左右
    int unk2[2];                   // 53 ~ 54
    __Widget *mFocusedChildWidget; // 55
    int unk3[4];                   // 56 ~ 59
    int *mAnimatorForState[4];     // 60 ~ 63
    // 大小64个整数！

    __Widget()
        requires(!IS_IN_DERIVED)
    {
        Create();
    }
    void Create() { reinterpret_cast<void (*)(__Widget *)>(Sexy_Widget_WidgetAddr)(this); }

    void Resize(int theX, int theY, int theWidth, int theHeight) { reinterpret_cast<void (*)(__Widget *, int, int, int, int)>(Sexy_Widget_ResizeAddr)(this, theX, theY, theWidth, theHeight); }
    void SetVisible(bool isVisible) { reinterpret_cast<void (*)(__Widget *, bool)>(Sexy_Widget_SetVisibleAddr)(this, isVisible); }
    void Move(int theNewX, int theNewY) { reinterpret_cast<void (*)(__Widget *, int, int)>(Sexy_Widget_MoveAddr)(this, theNewX, theNewY); }
    void DeferOverlay(int thePriority = 0) { reinterpret_cast<void (*)(__Widget *, int)>(Sexy_Widget_DeferOverlayAddr)(this, thePriority); }

    void MarkDirty();
    void AddWidget(__Widget *theWidget);
    void RemoveWidget(__Widget *theWidget);
    __Widget *FindWidget(int theId);

protected:
    __Widget()
        requires IS_IN_DERIVED
    {}
};

using Widget = __Widget<>;

} // namespace Sexy


#endif // PLANTSVSZOMBIES_ANDROIDTV_WIDGET_H
