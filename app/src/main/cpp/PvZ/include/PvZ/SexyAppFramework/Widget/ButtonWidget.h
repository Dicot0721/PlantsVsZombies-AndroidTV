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

#ifndef PVZ_SEXYAPPFRAMEWORK_WIDGET_BUTTON_WIDGET_H
#define PVZ_SEXYAPPFRAMEWORK_WIDGET_BUTTON_WIDGET_H

#include "PvZ/SexyAppFramework/Misc/TextLayout.h"
#include "PvZ/Symbols.h"

// #include "ButtonListener.h"
#include "SelectableWidget.h"

namespace Sexy {
class Image;
class ButtonListener;

class ButtonWidget : public SelectableWidget {
public:
    enum {
        COLOR_LABEL,
        COLOR_LABEL_HILITE,
        COLOR_DARK_OUTLINE,
        COLOR_LIGHT_OUTLINE,
        COLOR_MEDIUM_OUTLINE,
        COLOR_BKG,
        NUM_COLORS,
    };

    int *mLabel;            // 72
    int *mStringUnk2;       // 73
    TextLayout mTextLayout; // 74 ~ 157
    int unkMem1[9];         // 158 ~ 166
    Insets mInsets;         // 167 ~ 170
    int unkMems2[2];        // 171 ~ 172
    Image *mButtonImage;    // 173
    Image *mOverImage;      // 174
    Image *mDownImage;      // 175
    Image *mDisabledImage;  // 176
    Rect mNormalRect;       // 177 ~ 180
    Rect mOverRect;         // 181 ~ 184
    Rect mDownRect;         // 185 ~ 188
    Rect mDisabledRect;     // 189 ~ 192
    bool mInverted;         // 772
    bool mBtnNoDraw;        // 773
    bool mFrameNoDraw;      // 774
    int unkMems3[2];        // 194 ~ 195
    double mOverAlpha;      // 197 ~ 198
    int unkMems4[4];        // 198 ~ 201
    bool unkBool3;          // 808
    bool unkBool4;          // 809
    int unkMem2;            // 203
    // 大小204个整数

    void SetFont(Font *theFont) {
        reinterpret_cast<void (*)(ButtonWidget *, Font *)>(Sexy_ButtonWidget_SetFontAddr)(this, theFont);
    }

protected:
    ButtonWidget() = default;
    ~ButtonWidget() = default;

    void _constructor(int theId, ButtonListener *theButtonListener) {
        reinterpret_cast<void (*)(ButtonWidget *, int, ButtonListener *)>(Sexy_ButtonWidget__constructorAddr)(this, theId, theButtonListener);
    }
    void _destructor() {
        reinterpret_cast<void (*)(ButtonWidget *)>(Sexy_ButtonWidget__destructorAddr)(this);
    }
};

class ButtonWidget_ : public ButtonWidget {
public:
    ButtonWidget_(int theId, ButtonListener *theButtonListener) {
        _constructor(theId, theButtonListener);
    }
    ~ButtonWidget_() {
        _destructor();
    }
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_WIDGET_BUTTON_WIDGET_H
