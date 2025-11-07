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

#ifndef PVZ_SEXYAPPFRAMEWORK_WIDGET_DIALOG_H
#define PVZ_SEXYAPPFRAMEWORK_WIDGET_DIALOG_H

#include "PvZ/SexyAppFramework/Misc/TextLayout.h"
#include "PvZ/SexyAppFramework/Widget/ButtonListener.h"
#include "PvZ/SexyAppFramework/Widget/Insets.h"
#include "PvZ/SexyAppFramework/Widget/Widget.h"

namespace Sexy {

class ButtonWidget;
class DialogButton;
class Font;
class Image;

class __Dialog : public Sexy::__Widget {
public:
    ButtonListener mButtonListener; // 64
    int *mDialogListener;           // 65
    Image *mComponentImage;         // 66
    DialogButton *mYesButton;       // 67
    DialogButton *mNoButton;        // 68
    int mNumButtons;                // 69
    int *mDialogHeader;             // 70
    int *mDialogFooter;             // 71
    int *mDialogLines;              // 72
    int *mUnkText;                  // 73
    TextLayout mTextLayout;         // 74 ~ 157
    int unk1[3];                    // 158 ~ 160
    int mButtonMode;                // 161
    int *mHeaderFont;               // 162
    int *mLinesFont;                // 163
    int mTextAlign;                 // 164
    int mLineSpacingOffset;         // 165
    int mButtonHeight;              // 166
    Insets mBackgroundInsets;       // 167 ~ 170
    Insets mContentInsets;          // 171 ~ 174
    int mSpaceAfterHeader;          // 175
    bool mDragging;                 // 176
    int mDragMouseX;                // 177
    int mDragMouseY;                // 178
    int mId;                        // 179
    bool mIsModal;                  // 180
    int mResult;                    // 181
    int mButtonHorzSpacing;         // 182
    int mButtonSidePadding;         // 183
    // 大小184个整数

    int WaitForResult(bool unk) {
        return reinterpret_cast<int (*)(__Dialog *, bool)>(Sexy_Dialog_WaitForResultAddr)(this, unk);
    }

protected:
    __Dialog() = default;
    ~__Dialog() = default;
};

class Dialog : public __Dialog {
public:
    Dialog() = delete;
    ~Dialog() = delete;
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_WIDGET_DIALOG_H
