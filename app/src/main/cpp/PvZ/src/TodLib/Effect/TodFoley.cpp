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

#include "PvZ/TodLib/Common/TodFoley.h"
#include "PvZ/GlobalVariable.h"

FoleyParams gNewLawnFoleyParamArray[] = {
    {FoleyType::FOLEY_THRILLER, 0.0f, {&addonSounds.thriller}, 6U},
};

FoleyParams *LookupFoley(FoleyType theFoleyType) {
    // 新增三个Foley，用于主界面白噪音
    if (theFoleyType == FoleyType::FOLEY_MENU_LEFT) {
        return &gMenuLeftFoley;
    } else if (theFoleyType == FoleyType::FOLEY_MENU_CENTRE) {
        return &gMenuCenterFoley;
    } else if (theFoleyType == FoleyType::FOLEY_MENU_RIGHT) {
        return &gMenuRightFoley;
    } else if (theFoleyType > FoleyType::FOLEY_MENU_RIGHT) {
        return &gNewLawnFoleyParamArray[theFoleyType - NUM_FOLEY - 4];
    } else
        return old_LookupFoley(theFoleyType);
}