/*
 * Copyright (C) 2023-2025  PvZ TV Touch Team
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

#ifndef PVZ_SEXYAPPFRAMEWORK_WIDGET_WIDGET_CONTAINER_H
#define PVZ_SEXYAPPFRAMEWORK_WIDGET_WIDGET_CONTAINER_H

#include "PvZ/SexyAppFramework/Misc/Common.h"
#include "PvZ/SexyAppFramework/Misc/Flags.h"
#include "PvZ/SexyAppFramework/Misc/Rect.h"
#include "PvZ/Symbols.h"

namespace Sexy {

class Graphics;
class WidgetManager;
class Widget;

class __WidgetContainer {
public:
    int *vTable;                   // 0
    int mWidgetsUnk[5];            // 1 ~ 5
    WidgetManager *mWidgetManager; // 6
    __WidgetContainer *mParent;    // 7
    bool mUpdateIteratorModified;  // 32
    int *mUpdateIterator;          // 9
    int mLastWMUpdateCount;        // 10
    int mUpdateCnt;                // 11
    int unkMember;                 // 12
    int mX;                        // 13
    int mY;                        // 14
    int mWidth;                    // 15
    int mHeight;                   // 16
    bool mHasAlpha;                // 68
    bool mClip;                    // 69
    FlagsMod mWidgetFlagsMod;      // 18 ~ 19
    int mPriority;                 // 20
    int mZOrder;                   // 21
    int unk[6];                    // 22 ~ 27
    int mWidgetId;                 // 28
    // 大小未知，目前认为是29个整数。反正Widget是64个整数，足够了。

    void SetFocus(Widget *theWidget) {
        reinterpret_cast<void (*)(__WidgetContainer *, Widget *)>(Sexy_WidgetContainer_SetFocusAddr)(this, theWidget);
    }
    void MarkDirty() {
        reinterpret_cast<void (*)(__WidgetContainer *)>(Sexy_WidgetContainer_MarkDirtyAddr)(this);
    }

protected:
    __WidgetContainer() = default;
    ~__WidgetContainer() = default;
};

class WidgetContainer : public __WidgetContainer {
public:
    WidgetContainer() = delete;
    ~WidgetContainer() = delete;
};

} // namespace Sexy


#endif // PVZ_SEXYAPPFRAMEWORK_WIDGET_WIDGET_CONTAINER_H
