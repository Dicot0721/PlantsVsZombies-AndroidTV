//
// Created by 28636 on 2025/5/26.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_SELECTABLEWIDGET_H
#define PLANTSVSZOMBIES_ANDROIDTV_SELECTABLEWIDGET_H

#include "Widget.h"

namespace Sexy {

class SelectableWidget : public Widget {
public:
    Color mUnkColor;  // 64 ~ 67
    float unkMems[4]; // 68 ~ 71
}; // 大小72个整数

} // namespace Sexy


#endif // PLANTSVSZOMBIES_ANDROIDTV_SELECTABLEWIDGET_H
