//
// Created by 28636 on 2025/5/26.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_DIALOGBUTTON_H
#define PLANTSVSZOMBIES_ANDROIDTV_DIALOGBUTTON_H

#include "ButtonWidget.h"

class LawnApp;

namespace Sexy {

class DialogButton : public ButtonWidget {
public:
    Image *mImage;                   // 204
    int mTranslateX, mTranslateY;    // 205 ~ 206
    int mTextOffsetX, mTextOffsetY;  // 207 ~ 208
    LawnApp *mApp;                   // 209
}; // 大小210个整数

}


#endif // PLANTSVSZOMBIES_ANDROIDTV_DIALOGBUTTON_H
