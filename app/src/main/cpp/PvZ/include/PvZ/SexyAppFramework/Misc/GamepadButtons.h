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

#ifndef PVZ_SEXYAPPFRAMEWORK_MISC_GAMEPADBUTTONS_H
#define PVZ_SEXYAPPFRAMEWORK_MISC_GAMEPADBUTTONS_H

namespace Sexy {

enum GamepadButton {
    GAMEPAD_BUTTON_NONE = -1,
    GAMEPAD_BUTTON_UP = 0,
    GAMEPAD_BUTTON_DOWN = 1,
    GAMEPAD_BUTTON_LEFT = 2,
    GAMEPAD_BUTTON_RIGHT = 3,
    GAMEPAD_BUTTON_SELECT = 4, // GAMEPAD_BUTTON_Z
    GAMEPAD_BUTTON_START = 5,  // GAMEPAD_BUTTON_C
    GAMEPAD_BUTTON_A = 6,
    GAMEPAD_BUTTON_B = 7,
    GAMEPAD_BUTTON_X = 8,
    GAMEPAD_BUTTON_Y = 9,
    GAMEPAD_BUTTON_TL = 10,
    GAMEPAD_BUTTON_TR = 11,
    GAMEPAD_BUTTON_TL2 = 12,
    GAMEPAD_BUTTON_TR2 = 13,
    GAMEPAD_BUTTON_THUMBL = 14,
    GAMEPAD_BUTTON_THUMBR = 15,
    GAMEPAD_BUTTON_DPAD_UP = 16,
    GAMEPAD_BUTTON_DPAD_DOWN = 17,
    GAMEPAD_BUTTON_DPAD_LEFT = 18,
    GAMEPAD_BUTTON_DPAD_RIGHT = 19,
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_MISC_GAMEPADBUTTONS_H
