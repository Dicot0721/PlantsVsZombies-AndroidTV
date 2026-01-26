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

#ifndef PVZ_SEXYAPPFRAMEWORK_WIDGET_WIDGET_H
#define PVZ_SEXYAPPFRAMEWORK_WIDGET_WIDGET_H

#include "PvZ/SexyAppFramework/Graphics/Color.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/SexyAppFramework/Misc/Common.h"
#include "PvZ/SexyAppFramework/Misc/KeyCodes.h"

#include "Insets.h"
#include "WidgetContainer.h"

namespace Sexy {

class Widget;

class __Widget : public __WidgetContainer {
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

    void _constructor() {
        reinterpret_cast<void (*)(__Widget *)>(Sexy_Widget___ConstructorAddr)(this);
    }
    void _destructor() {
        reinterpret_cast<void (*)(__Widget *)>(Sexy_Widget___DestructorAddr)(this);
    }

    void Resize(int theX, int theY, int theWidth, int theHeight) {
        reinterpret_cast<void (*)(__Widget *, int, int, int, int)>(Sexy_Widget_ResizeAddr)(this, theX, theY, theWidth, theHeight);
    }
    void SetVisible(bool isVisible) {
        reinterpret_cast<void (*)(__Widget *, bool)>(Sexy_Widget_SetVisibleAddr)(this, isVisible);
    }
    void Move(int theNewX, int theNewY) {
        reinterpret_cast<void (*)(__Widget *, int, int)>(Sexy_Widget_MoveAddr)(this, theNewX, theNewY);
    }
    void DeferOverlay(int thePriority = 0) {
        reinterpret_cast<void (*)(__Widget *, int)>(Sexy_Widget_DeferOverlayAddr)(this, thePriority);
    }

    void MarkDirty();
    void AddWidget(__Widget *theWidget);
    void RemoveWidget(__Widget *theWidget);
    Widget *FindWidget(int theId);

protected:
    __Widget() = default;
    ~__Widget() = default;
};

class Widget : public __Widget {
public:
    Widget() {
        __Widget::_constructor();
    }
    ~Widget() {
        __Widget::_destructor();
    }
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_WIDGET_WIDGET_H
