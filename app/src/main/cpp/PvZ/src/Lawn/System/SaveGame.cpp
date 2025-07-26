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

#include "PvZ/Lawn/System/SaveGame.h"
#include "PvZ/TodLib/Effect/Reanimator.h"

void SaveGameContext::SyncReanimationDef(ReanimatorDefinition *&theDefinition) {
    // 解决大头贴动画的读档问题
    if (*((uint8_t *)this + 29)) {
        int reanimationType;
        SyncInt(*&reanimationType);
        if (reanimationType == -1) {
            theDefinition = nullptr;
        } else if (reanimationType <= ReanimationType::REANIM_ZOMBATAR_HEAD) {
            ReanimatorEnsureDefinitionLoaded((ReanimationType)reanimationType, true);
            ReanimatorDefinition *v6 = *gReanimatorDefArray_Addr; // r3
            theDefinition = v6 + reanimationType;
        } else {
            *((uint8_t *)this + 28) = true;
        }
    } else {
        int v3 = 0;
        int reanimationType = -1;
        ReanimatorDefinition *v5 = theDefinition;             // r1
        ReanimatorDefinition *v6 = *gReanimatorDefArray_Addr; // r3
        while (v5 != v6++) {
            if (++v3 == ReanimationType::REANIM_ZOMBATAR_HEAD + 1)
                goto LABEL_7;
        }
        reanimationType = v3;
    LABEL_7:
        SyncInt(reanimationType);
    }
}