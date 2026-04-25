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

#include "PvZ/Lawn/System/SaveGame.h"
#include "PvZ/TodLib/Effect/Reanimator.h"

void SaveGameContext::SyncReanimationDef(ReanimatorDefinition *&theDefinition) {
    // 解决大头贴动画的读档问题
    if (mReading) {
        int aReanimType;
        SyncInt(aReanimType);
        if (aReanimType == ReanimationType::REANIM_NONE) {
            theDefinition = nullptr;
        } else if (aReanimType >= 0 && aReanimType < NUM_REANIMS) {
            ReanimatorEnsureDefinitionLoaded(ReanimationType(aReanimType), true);
            theDefinition = &gReanimatorDefArray[aReanimType];
        } else {
            mFailed = true;
        }
    } else {
        int aReanimType = ReanimationType::REANIM_NONE;
        for (int i = 0; i < ReanimationType::NUM_REANIMS; ++i) {
            ReanimatorDefinition *aDef = &gReanimatorDefArray[i];
            if (theDefinition == aDef) {
                aReanimType = i;
                break;
            }
        }
        SyncInt(aReanimType);
    }
}
