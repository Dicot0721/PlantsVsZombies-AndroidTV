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
