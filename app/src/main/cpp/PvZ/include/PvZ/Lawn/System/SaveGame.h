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

#ifndef PVZ_LAWN_SYSTEM_SAVE_GAME_H
#define PVZ_LAWN_SYSTEM_SAVE_GAME_H

#include "PvZ/Symbols.h"

class SaveGameContext {
public:
    void SyncInt(int &theInt) {
        reinterpret_cast<void (*)(SaveGameContext *, int &)>(SaveGameContext_SyncIntAddr)(this, theInt);
    }

    void SyncReanimationDef(ReanimatorDefinition *&theDefinition);
};


#endif // PVZ_LAWN_SYSTEM_SAVE_GAME_H
