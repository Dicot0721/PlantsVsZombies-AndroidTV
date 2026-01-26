/*
 * Copyright (C) 2023-2026  PvZ TV Touch Team
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

#ifndef PVZ_SEXYAPPFRAMEWORK_WIDGET_MENU_WIDGET_H
#define PVZ_SEXYAPPFRAMEWORK_WIDGET_MENU_WIDGET_H

#include "ButtonListener.h"
#include "Widget.h"

namespace Sexy {

class MenuWidget : public __Widget {
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

#endif // PVZ_SEXYAPPFRAMEWORK_WIDGET_MENU_WIDGET_H
