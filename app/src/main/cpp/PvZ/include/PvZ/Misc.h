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

#ifndef PVZ_MISC_H
#define PVZ_MISC_H

#include "PvZ/Lawn/Board/MessageWidget.h"
#include "PvZ/Lawn/System/PlayerInfo.h"
#include "PvZ/Lawn/System/ReanimationLawn.h"
#include "PvZ/Lawn/Widget/AchievementsWidget.h"
#include "PvZ/Lawn/Widget/ZombatarWidget.h"
#include "PvZ/SexyAppFramework/Widget/Checkbox.h"
#include "PvZ/TodLib/Common/TodFoley.h"

inline void DrawCheckboxText(Sexy::Graphics *g, const pvzstl::string &theText1, Sexy::Checkbox *theCheckBox, const char *theText2) {
    reinterpret_cast<void (*)(Sexy::Graphics *, const pvzstl::string &, Sexy::Checkbox *, const char *)>(DrawCheckboxTextAddr)(g, theText1, theCheckBox, theText2);
}


inline int (*old_SexyDialog_AddedToManager)(void *instance, void *instance1);

inline int (*old_SexyDialog_RemovedFromManager)(void *instance, void *instance1);

inline void (*old_HelpBarWidget_HelpBarWidget)(Sexy::__Widget *a);

inline void (*old_Sexy_ExtractLoadingSoundsResources)(int *a, int *theManager);


int randomInt(int a, int b);

void DrawSeedType(Sexy::Graphics *g, float x, float y, SeedType theSeedType, SeedType theImitaterType, float xOffset, float yOffset, float scale);

int SexyDialog_AddedToManager(void *instance, void *instance1);

int SexyDialog_RemovedFromManager(void *instance, void *instance1);

void Sexy_ExtractLoadingSoundsResources(int *a, int *theManager);

void WaitForSecondPlayerDialog_WaitForSecondPlayerDialog(int *a, int *a2);

void HelpBarWidget_HelpBarWidget(Sexy::__Widget *a);

#endif // PVZ_MISC_H
