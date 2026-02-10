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

#include "PvZ/Lawn/Widget/HouseChooserDialog.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Symbols.h"

bool HouseChooserDialog::IsHouseAvaliable(HouseType houseType) {
    LawnApp *lawnApp = *gLawnApp_Addr;
    return houseType == 0 || lawnApp->mPlayerInfo->mPurchases[StoreItem::STORE_ITEM_BLUEPRINT_BLING + int(houseType)] > 0;
}

void HouseChooserDialog::MouseDown(int x, int y, int theClickCount) {
    old_HouseChooserDialog_MouseDown(this, x, y, theClickCount);

    int width = (*Sexy_IMAGE_STORE_BLUEPRINT_CLOWN_Addr)->GetWidth();
    int height = (*Sexy_IMAGE_STORE_BLUEPRINT_CLOWN_Addr)->GetHeight();
    int houseIdToSelect = HouseType::BLUEPRINT_INVALID;
    for (int i = 0; i < 5; ++i) {
        Sexy::Rect rect = {75 + 90 * i, 125, width, height};
        if (TRect_Contains(&rect, x, y) && IsHouseAvaliable((HouseType)i)) {
            houseIdToSelect = i;
            break;
        }
    }
    if (houseIdToSelect == HouseType::BLUEPRINT_INVALID)
        return;
    int currentHouseType = mSelectedHouseType;
    if (currentHouseType == houseIdToSelect) {
        GameButtonDown(GamepadButton::GAMEPAD_BUTTON_A, 0, false);
    } else {
        while (mSelectedHouseType != houseIdToSelect) {
            GameButtonDown(GamepadButton::GAMEPAD_BUTTON_LEFT, 0, false);
        }
    }
}

void HouseChooserDialog::KeyDown(Sexy::KeyCode theKey) {
    old_HouseChooserDialog_KeyDown(this, theKey);

    using Sexy::KeyCode;
    switch (theKey) {
        case KeyCode::KEYCODE_LEFT:
            return GameButtonDown(GamepadButton::GAMEPAD_BUTTON_LEFT, 0, false);
        case KeyCode::KEYCODE_UP:
            return GameButtonDown(GamepadButton::GAMEPAD_BUTTON_UP, 0, false);
        case KeyCode::KEYCODE_RIGHT:
            return GameButtonDown(GamepadButton::GAMEPAD_BUTTON_RIGHT, 0, false);
        case KeyCode::KEYCODE_DOWN:
            return GameButtonDown(GamepadButton::GAMEPAD_BUTTON_DOWN, 0, false);
        case KeyCode::KEYCODE_ESCAPE:
        case KeyCode::KEYCODE_GAMEPAD_B:
            return GameButtonDown(GamepadButton::GAMEPAD_BUTTON_B, 0, false);
        case KeyCode::KEYCODE_RETURN:
        case KeyCode::KEYCODE_GAMEPAD_A:
            return GameButtonDown(GamepadButton::GAMEPAD_BUTTON_A, 0, false);
        default:
            break;
    }
}
