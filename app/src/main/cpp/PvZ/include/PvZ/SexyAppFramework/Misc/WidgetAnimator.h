#ifndef PVZ_SEXYAPPFRAMEWORK_MISC_WIDGET_ANIMATOR_H
#define PVZ_SEXYAPPFRAMEWORK_MISC_WIDGET_ANIMATOR_H

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

#endif // PVZ_SEXYAPPFRAMEWORK_MISC_WIDGET_ANIMATOR_H
