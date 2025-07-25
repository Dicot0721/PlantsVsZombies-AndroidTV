#ifndef PVZ_SEXYAPPFRAMEWORK_WIDGET_DIALOG_BUTTON_H
#define PVZ_SEXYAPPFRAMEWORK_WIDGET_DIALOG_BUTTON_H

#include "ButtonWidget.h"

class LawnApp;

namespace Sexy {

class DialogButton : public ButtonWidget {
public:
    Image *mImage;                  // 204
    int mTranslateX, mTranslateY;   // 205 ~ 206
    int mTextOffsetX, mTextOffsetY; // 207 ~ 208
    LawnApp *mApp;                  // 209
}; // 大小210个整数

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_WIDGET_DIALOG_BUTTON_H
