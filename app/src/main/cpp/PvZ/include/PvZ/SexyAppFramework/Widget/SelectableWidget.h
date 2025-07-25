#ifndef PVZ_SEXYAPPFRAMEWORK_WIDGET_SELECTABLE_WIDGET_H
#define PVZ_SEXYAPPFRAMEWORK_WIDGET_SELECTABLE_WIDGET_H

#include "Widget.h"

namespace Sexy {

class SelectableWidget : public __Widget {
public:
    Color mUnkColor;  // 64 ~ 67
    float unkMems[4]; // 68 ~ 71
}; // 大小72个整数

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_WIDGET_SELECTABLE_WIDGET_H
