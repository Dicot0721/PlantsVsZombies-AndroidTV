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

#ifndef PVZ_LAWN_WIDGET_WAIT_FOR_SECOND_PLAYER_DIALOG_H
#define PVZ_LAWN_WIDGET_WAIT_FOR_SECOND_PLAYER_DIALOG_H

#include "PvZ/Lawn/GamepadControls.h"
#include "PvZ/Symbols.h"

class WaitForSecondPlayerDialog {
public:
    void GameButtonDown(ButtonCode theButton, unsigned int thePlayerIndex) {
        reinterpret_cast<void (*)(WaitForSecondPlayerDialog *, ButtonCode, unsigned int)>(WaitForSecondPlayerDialog_GameButtonDownAddr)(this, theButton, thePlayerIndex);
    }

    WaitForSecondPlayerDialog(int *a2) {
        __Constructor(a2);
    }

protected:
    friend void InitHookFunction();

    void __Constructor(int *);
};

inline void (*old_WaitForSecondPlayerDialog_WaitForSecondPlayerDialog)(WaitForSecondPlayerDialog *a, int *a2);

#endif // PVZ_LAWN_WIDGET_WAIT_FOR_SECOND_PLAYER_DIALOG_H
