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
#ifndef PLANTSVSZOMBIES_ANDROIDTV_OPENINGENCOUNTER_H
#define PLANTSVSZOMBIES_ANDROIDTV_OPENINGENCOUNTER_H

enum EncounterType {
    ENCOUNTER_NONE = -1,
    ENCOUNTER_SUN_RAIN,
    ENCOUNTER_LITTER_TROUBLE,
    ENCOUNTER_FREE_SHUFFLE,
    NUM_ENCOUNTER,
};

class OpeningEncounter {
public:
    EncounterType mType = ENCOUNTER_NONE;
    int mCounter = 0;
    bool mDoEffect = true;

    OpeningEncounter() = default;

    void OpeningEncounterInitialize(EncounterType theType);
    void Update();
    void UpdateSunRain();
};

inline OpeningEncounter *gOpeningEncounter;

#endif // PLANTSVSZOMBIES_ANDROIDTV_OPENINGENCOUNTER_H
