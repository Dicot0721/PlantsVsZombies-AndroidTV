//
// Created by 28636 on 2025/5/26.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_DIALOG_H
#define PLANTSVSZOMBIES_ANDROIDTV_DIALOG_H

#include "PvZ/SexyAppFramework/Misc/TextLayout.h"

#include "ButtonListener.h"
#include "Widget.h"

namespace Sexy {

class DialogListener;
class ButtonWidget;
class DialogButton;
class Font;

class Dialog : public Widget {
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
}; // 大小184个整数

} // namespace Sexy

#endif // PLANTSVSZOMBIES_ANDROIDTV_DIALOG_H
