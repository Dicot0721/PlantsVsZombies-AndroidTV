/*
 * Copyright (C) 2023-2025  PvZ TV Touch Team
 *
 * This file is part of PlantsVsZombies-AndroidTV.
 *
 * PlantsVsZombies-AndroidTV is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * PlantsVsZombies-AndroidTV is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * PlantsVsZombies-AndroidTV.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef PVZ_SEXYAPPFRAMEWORK_WIDGET_WIDGET_MANAGER_H
#define PVZ_SEXYAPPFRAMEWORK_WIDGET_WIDGET_MANAGER_H

#include "../Misc/Common.h"
#include "../Misc/KeyCodes.h"

 #include "WidgetContainer.h"

namespace Sexy {

class Widget;

class WidgetManager  : public __WidgetContainer {
public:
    char unkMem[44];
    Widget* mFocusWidget;    // 40

    void SetFocus(Widget* aWidget) {
        reinterpret_cast<void (*)(WidgetManager*, Widget*)>(Sexy_WidgetManager_SetFocusAddr)(this, aWidget);
    }
    Widget* GetWidgetAt(int x, int y, int* theWidgetX, int* theWidgetY) {
        return reinterpret_cast<Widget* (*)(WidgetManager*, int, int, int*, int*)>(Sexy_WidgetManager_GetWidgetAtAddr)(this, x, y, theWidgetX, theWidgetY);
    }

    void MouseDown(int x, int y, int theClickCount);
    void MouseDrag(int x, int y);
    void MouseUp(int x, int y, int theClickCount);
};

} // namespace Sexy

inline void (*old_Sexy_WidgetManager_MouseDown)(Sexy::WidgetManager*, int x, int y, int theClickCount);
inline void (*old_Sexy_WidgetManager_MouseDrag)(Sexy::WidgetManager*, int x, int y);
inline void (*old_Sexy_WidgetManager_MouseUp)(Sexy::WidgetManager*, int x, int y, int theClickCount);


#endif // PVZ_SEXYAPPFRAMEWORK_WIDGET_WIDGET_MANAGER_H
