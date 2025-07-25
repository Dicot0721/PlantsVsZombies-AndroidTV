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

    ButtonWidget(int theId, ButtonListener *theButtonListener) {
        reinterpret_cast<void (*)(ButtonWidget *, int, ButtonListener *)>(Sexy_ButtonWidget_ButtonWidgetAddr)(this, theId, theButtonListener);
    }
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_WIDGET_BUTTON_WIDGET_H
