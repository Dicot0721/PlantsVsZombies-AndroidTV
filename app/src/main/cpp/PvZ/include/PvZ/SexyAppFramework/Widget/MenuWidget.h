//
// Created by 28636 on 2025/5/26.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_MENUWIDGET_H
#define PLANTSVSZOMBIES_ANDROIDTV_MENUWIDGET_H

#include "ButtonListener.h"
#include "Widget.h"

namespace Sexy {

class MenuWidget : public Widget {
public:
    ButtonListener mButtonListener; // 64
    bool mIsFading;                 // 260
    bool unkBool2;                  // 261
    int *mMenuParser;               // 66
    int *mSourceFileName;           // 67
    int unkInt1;                    // 68
    Image *mImage;                  // 69
    // 大小70个整数

    void Draw(Graphics *g) {
        reinterpret_cast<void (*)(MenuWidget *, Graphics *)>(Sexy_MenuWidget_DrawAddr)(this, g);
    }
};

} // namespace Sexy

#endif // PLANTSVSZOMBIES_ANDROIDTV_MENUWIDGET_H
