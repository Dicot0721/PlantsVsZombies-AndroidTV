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

#ifndef PVZ_LAWN_WIDGET_CONFIRM_BACK_TO_MAIN_DIALOG_H
#define PVZ_LAWN_WIDGET_CONFIRM_BACK_TO_MAIN_DIALOG_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Lawn/Widget/LawnDialog.h"

class ConfirmBackToMainDialog : public __LawnDialog {
public:
    GameButton *mRestartButton; // 191
    // 115: 192, 111: 194

    ConfirmBackToMainDialog(bool theBool) {
        _constructor(theBool);
    }

protected:
    void _constructor(bool theBool) {
        reinterpret_cast<void (*)(ConfirmBackToMainDialog *, bool)>(ConfirmBackToMainDialog_ConfirmBackToMainDialogAddr)(this, theBool);
    }
};


inline void (*old_ConfirmBackToMainDialog_MouseDrag)(ConfirmBackToMainDialog *confirmBackToMainDialog, int x, int y);

inline void (*old_ConfirmBackToMainDialog_AddedToManager)(ConfirmBackToMainDialog *confirmBackToMainDialog, int a2);

inline void (*old_ConfirmBackToMainDialog_ButtonDepress)(ConfirmBackToMainDialog *a, int a2);


void ConfirmBackToMainDialog_MouseDrag(ConfirmBackToMainDialog *confirmBackToMainDialog, int x, int y);

void ConfirmBackToMainDialog_AddedToManager(ConfirmBackToMainDialog *confirmBackToMainDialog, int a2);

void ConfirmBackToMainDialog_ButtonDepress(ConfirmBackToMainDialog *a, int a2);

#endif // PVZ_LAWN_WIDGET_CONFIRM_BACK_TO_MAIN_DIALOG_H
