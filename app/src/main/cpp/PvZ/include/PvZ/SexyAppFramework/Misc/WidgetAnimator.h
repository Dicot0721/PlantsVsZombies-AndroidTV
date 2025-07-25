//
// Created by 28636 on 2025/5/26.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_WIDGETANIMATOR_H
#define PLANTSVSZOMBIES_ANDROIDTV_WIDGETANIMATOR_H

#include "../Widget/Widget.h"

namespace Sexy {

class WidgetAnimator {
public:
    int *vTable;            // 0
    int unk[2];             // 1 ~ 2
    Widget *mWidget;        // 3
    int mUpdateCounter;     // 4
    bool mHideOnCompletion; // 20
};

} // namespace Sexy

#endif // PLANTSVSZOMBIES_ANDROIDTV_WIDGETANIMATOR_H
