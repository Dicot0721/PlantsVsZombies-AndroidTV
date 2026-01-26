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

#ifndef PVZ_SEXYAPPFRAMEWORK_GAMEPAD_APP_H
#define PVZ_SEXYAPPFRAMEWORK_GAMEPAD_APP_H

#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Symbols.h"
#include "SexyCommonApp.h"

namespace Sexy {
class __GamepadApp : public __SexyCommonApp {};

class GamepadApp : public __GamepadApp {
public:
    bool HasGamepad() {
        return reinterpret_cast<bool (*)(__GamepadApp *)>(Sexy_GamepadApp_HasGamepadAddr)(this);
    }
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_GAMEPAD_APP_H
