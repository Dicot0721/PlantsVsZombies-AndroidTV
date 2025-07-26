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

#ifndef PVZ_SEXYAPPFRAMEWORK_WIDGET_SCROLLBAR_WIDGET_H
#define PVZ_SEXYAPPFRAMEWORK_WIDGET_SCROLLBAR_WIDGET_H

#include "PvZ/Symbols.h"

#include "Widget.h"

namespace Sexy {

class ScrollbarWidget : public __Widget {
public:
    int unkMem;             // 64
    Widget *mUpButton;      // 65
    Widget *mDownButton;    // 66
    bool mInvisIfNoScroll;  // 268
    int mId;                // 68
    double mValue;          // 69 ~ 70
    double mMaxValue;       // 71 ~ 72
    double mPageSize;       // 73 ~ 74
    bool mHorizontal;       // 300
    int mButtonLength;      // 76
    bool mPressedOnThumb;   // 77
    int mMouseDownThumbPos; // 78
    int mMouseDownX;        // 79
    int mMouseDownY;        // 80
    int mUpdateMode;        // 81
    int mUpdateAcc;         // 82
    int mButtonAcc;         // 83
    int mLastMouseX;        // 84
    int mLastMouseY;        // 85
    int *mScrollListener;   // 86
    Image *mThumbImage;     // 87
    Image *mBarImage;       // 88
    Image *mPagingImage;    // 89
    // 大小90个整数

    void SetMaxValue(double theValue) {
        reinterpret_cast<void (*)(Sexy::ScrollbarWidget *, double)>(Sexy_ScrollbarWidget_SetMaxValueAddr)(this, theValue);
    }
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_WIDGET_SCROLLBAR_WIDGET_H
