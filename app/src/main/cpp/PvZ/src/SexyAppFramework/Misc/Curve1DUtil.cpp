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

#include "PvZ/SexyAppFramework/Misc/Curve1DUtil.h"

using namespace Sexy;

Curve1DUtil::Curve1DUtil() {
    for (int i = 0; i < 8; ++i) {
        unk[i] = 0;
    }
    unkF[0] = 0.0f;
    unkF[1] = 0.5f;
    unkF[2] = 0.75f;
    unkF[3] = 1.0f;
    unkF[4] = 0.5f;
    unkF[5] = 0.75f;
    unkF[6] = 1.0f;
    unkF[7] = 1.2f;
};