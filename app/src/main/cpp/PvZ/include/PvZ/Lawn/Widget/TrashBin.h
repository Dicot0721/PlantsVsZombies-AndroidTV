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

#ifndef PVZ_LAWN_WIDGET_TRASH_BIN_H
#define PVZ_LAWN_WIDGET_TRASH_BIN_H

#include "PvZ/SexyAppFramework/Widget/Widget.h"
#include "PvZ/Symbols.h"

constexpr int zombiePileHeight = 70;
constexpr int plantPileHeight = 100;

class TrashBin : public Sexy::Widget {
public:
    enum TrashPileType { PLANT_PILE = 0, ZOMBIE_PILE = 1 };

    TrashPileType mTrashPileType; // 64
    float mTrashHeight;           // 65
    Sexy::Image *mPileImage[10];  // 66 ~ 75
    bool mMirrorPile[10];         // 76 ~ 78
    int mRandomPileWeight[6];     // 79 ~ 84
    int mPileNum;                 // 85
    // 大小86个整数

    TrashBin(TrashPileType theTrashPileType, float theHeight);

    void Draw(Sexy::Graphics *g);
    Sexy::Image *GetZombieTrashPiece(int theLevel);
    Sexy::Image *GetPlantTrashPiece(int theLevel);

protected:
    friend void InitHookFunction();

    void _constructor(TrashPileType theTrashPileType, float theHeight);

    void _destructor() {
        reinterpret_cast<void (*)(TrashBin *)>(TrashBin_DeleteAddr)(this);
    }
};

inline void (*old_TrashBin_TrashBin)(TrashBin *trashBin, TrashBin::TrashPileType theTrashPileType, float height);

#endif // PVZ_LAWN_WIDGET_TRASH_BIN_H
