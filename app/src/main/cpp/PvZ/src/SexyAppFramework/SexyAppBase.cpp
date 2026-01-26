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

#include "PvZ/SexyAppFramework/SexyAppBase.h"
#include "Homura/Logger.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/System/PlayerInfo.h"

void Sexy::__SexyAppBase::_constructor() {
    old_Sexy_SexyAppBase_SexyAppBase(this);
    // LOGD("%d",*((uint32_t *)appBase + 464));
    // *((uint32_t *)appBase + 464) = 0; ;
    // *((uint32_t *)appBase + 380) = 100;

    // Sexy_Ratio_Set((int *)appBase + 399, 16, 10);
    // Sexy_Ratio_Set((int *)appBase + 395, 4, 3);
    // Sexy_Ratio_Set((int *)appBase + 397, 4, 3);
    // *((uint32_t *)appBase + 403) = 600;
    // *((uint32_t *)appBase + 402) = 800;
}


bool Sexy::SexyAppBase::UpdateApp() {
    return old_Sexy_SexyAppBase_UpdateApp(this);
}