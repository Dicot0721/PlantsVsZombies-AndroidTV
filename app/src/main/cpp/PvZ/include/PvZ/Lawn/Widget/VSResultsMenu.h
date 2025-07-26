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

#ifndef PVZ_LAWN_WIDGET_VS_RESULTS_MENU_H
#define PVZ_LAWN_WIDGET_VS_RESULTS_MENU_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/SexyAppFramework/Widget/Widget.h"

class VSResultsMenu {
public:
};

inline void (*old_VSResultsMenu_Update)(int *a);

inline void (*old_VSResultsMenu_OnExit)(int *a);

inline void (*old_VSResultsMenu_DrawInfoBox)(int *a, Sexy::Graphics *a2, int a3);


void VSResultsMenu_Update(int *a);

void VSResultsMenu_OnExit(int *a);

void VSResultsMenu_ButtonDepress(int *vsResultsScreen, int buttonId);

void VSResultsMenu_DrawInfoBox(int *a, Sexy::Graphics *a2, int a3);

#endif // PVZ_LAWN_WIDGET_VS_RESULTS_MENU_H
