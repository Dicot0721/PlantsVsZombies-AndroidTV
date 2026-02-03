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

#ifndef PVZ_SEXYAPPFRAMEWORK_WIDGET_CHECKBOX_H
#define PVZ_SEXYAPPFRAMEWORK_WIDGET_CHECKBOX_H

#include "PvZ/Symbols.h"

#include "Widget.h"

namespace Sexy {

class Checkbox : public Widget {
public:
    int unknownMember[8];   // 64 ~ 71
    int *mListener;         // 72
    Image *mUncheckedImage; // 73
    Image *mCheckedImage;   // 74
    int mId;                // 75
    bool mChecked;          // 304
    Rect mCheckedRect;      // 77 ~ 80
    Rect mUncheckedRect;    // 81 ~ 84
    Color mOutlineColor;    // 85 ~ 88
    Color mBkgColor;        // 89 ~ 92
    Color mCheckColor;      // 93 ~ 96
    // 大小97个整数

    ~Checkbox() {
        _destructor();
    }

protected:
    void _destructor() {
        reinterpret_cast<void (*)(Checkbox *)>(Sexy_Checkbox_DeleteAddr)(this);
    }
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_WIDGET_CHECKBOX_H
