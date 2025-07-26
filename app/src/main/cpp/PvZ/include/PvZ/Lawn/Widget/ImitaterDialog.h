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

#ifndef PVZ_LAWN_WIDGET_IMITATER_DIALOG_H
#define PVZ_LAWN_WIDGET_IMITATER_DIALOG_H

#include "LawnDialog.h"
#include "PvZ/Lawn/Common/ConstEnums.h"

class ImitaterDialog : public LawnDialog {
public:
    SeedType SeedHitTest(int x, int y) {
        return reinterpret_cast<SeedType (*)(ImitaterDialog *, int, int)>(ImitaterDialog_SeedHitTestAddr)(this, x, y);
    }

    void MouseDown(int x, int y, int theCount);
};


inline void (*old_ImitaterDialog_ImitaterDialog)(ImitaterDialog *instance, int a2);

inline void (*old_ImitaterDialog_ShowToolTip)(ImitaterDialog *instance);

inline bool (*old_ImitaterDialog_KeyDown)(ImitaterDialog *a, int a2);

inline void (*old_ImitaterDialog_MouseDown)(ImitaterDialog *a, int x, int y, int theCount);


void ImitaterDialog_ImitaterDialog(ImitaterDialog *instance, int playerIndex);

void ImitaterDialog_ShowToolTip(ImitaterDialog *instance);

bool ImitaterDialog_KeyDown(ImitaterDialog *a, int a2);

#endif // PVZ_LAWN_WIDGET_IMITATER_DIALOG_H
