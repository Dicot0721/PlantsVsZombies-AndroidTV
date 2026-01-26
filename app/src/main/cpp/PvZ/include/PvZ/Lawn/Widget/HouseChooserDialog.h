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

#ifndef PVZ_LAWN_WIDGET_HOUSE_CHOOSER_DIALOG_H
#define PVZ_LAWN_WIDGET_HOUSE_CHOOSER_DIALOG_H

#include "LawnDialog.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Lawn/GamepadControls.h"
#include "PvZ/SexyAppFramework/Misc/KeyCodes.h"

enum HouseType {
    BLUEPRINT_INVALID = -1,
    BLUEPRINT_BLING = 0,
    BLUEPRINT_SCARY = 1,
    BLUEPRINT_TRAILER = 2,
    BLUEPRINT_FUTURE = 3,
    BLUEPRINT_CLOWN = 4,
};

class HouseChooserDialog : public __LawnDialog {
public:
    HouseType mSelectedHouseType; // 191
    int unk[2];                   // 192 ~ 193
    // 115: 194, 111: 196

    void GameButtonDown(GamepadButton theButton, int thePlayerIndex, bool a4) {
        reinterpret_cast<void (*)(HouseChooserDialog *, GamepadButton, int, bool)>(HouseChooserDialog_GameButtonDownAddr)(this, theButton, thePlayerIndex, a4);
    }

    static bool IsHouseAvaliable(HouseType houseType);

    void MouseDown(int x, int y, int theClickCount);
    void KeyDown(Sexy::KeyCode theKey);
};


inline void (*old_HouseChooserDialog_MouseDown)(HouseChooserDialog *a, int x, int y, int theClickCount);

inline void (*old_HouseChooserDialog_KeyDown)(HouseChooserDialog *a, Sexy::KeyCode theKey);

#endif // PVZ_LAWN_WIDGET_HOUSE_CHOOSER_DIALOG_H
