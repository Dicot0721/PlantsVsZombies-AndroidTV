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

#ifndef PVZ_LAWN_BOARD_TOOL_TIP_WIDGET_H
#define PVZ_LAWN_BOARD_TOOL_TIP_WIDGET_H

#include "PvZ/STL/pvzstl_string.h"
#include "PvZ/Symbols.h"

namespace Sexy {
class Graphics;
}

class ToolTipWidget {
public:
    pvzstl::string mTitle;       // 0
    pvzstl::string mLabel;       // 1
    pvzstl::string mWarningText; // 2
    int mX;                      // 3
    int mY;                      // 4
    int mWidth;                  // 5
    int mHeight;                 // 6

    void SetWarningText(const pvzstl::string &theWarningText) {
        reinterpret_cast<void (*)(ToolTipWidget *, const pvzstl::string &)>(ToolTipWidget_SetWarningTextAddr)(this, theWarningText);
    }
    void SetTitle(const pvzstl::string &theTitle) {
        reinterpret_cast<void (*)(ToolTipWidget *, const pvzstl::string &)>(ToolTipWidget_SetTitleAddr)(this, theTitle);
    }
    void SetLabel(const pvzstl::string &theLabel) {
        reinterpret_cast<void (*)(ToolTipWidget *, const pvzstl::string &)>(ToolTipWidget_SetLabelAddr)(this, theLabel);
    }
    void Draw(Sexy::Graphics *g) {
        reinterpret_cast<void (*)(ToolTipWidget *, Sexy::Graphics *)>(ToolTipWidget_DrawAddr)(this, g);
    }
};

#endif // PVZ_LAWN_BOARD_TOOL_TIP_WIDGET_H
