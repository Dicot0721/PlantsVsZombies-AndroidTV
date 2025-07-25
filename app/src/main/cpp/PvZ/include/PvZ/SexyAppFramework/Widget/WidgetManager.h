#ifndef PVZ_SEXYAPPFRAMEWORK_WIDGET_WIDGET_MANAGER_H
#define PVZ_SEXYAPPFRAMEWORK_WIDGET_WIDGET_MANAGER_H

#include "../Misc/Common.h"
#include "../Misc/KeyCodes.h"

// #include "WidgetContainer.h"

namespace Sexy {

template <bool>
class __Widget;

using Widget = __Widget<false>;

class WidgetManager {
public:
    void SetFocus(Widget* aWidget) {
        reinterpret_cast<void (*)(WidgetManager*, Widget*)>(Sexy_WidgetManager_SetFocusAddr)(this, aWidget);
    }
    Widget* GetWidgetAt(int x, int y, int* theWidgetX, int* theWidgetY) {
        return reinterpret_cast<Widget* (*)(WidgetManager*, int, int, int*, int*)>(Sexy_WidgetManager_GetWidgetAtAddr)(this, x, y, theWidgetX, theWidgetY);
    }
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_WIDGET_WIDGET_MANAGER_H
