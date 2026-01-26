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

#include "PvZ/Lawn/Board/ZenGarden.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/SexyAppFramework/Misc/Common.h"

using namespace Sexy;

static SpecialGridPlacement gGreenhouseGridPlacement[] = {
    {73, 73, 0, 0},  {155, 71, 1, 0},  {239, 68, 2, 0},  {321, 73, 3, 0},  {406, 71, 4, 0},  {484, 67, 5, 0},  {566, 70, 6, 0},  {648, 72, 7, 0},
    {67, 168, 0, 1}, {150, 165, 1, 1}, {232, 170, 2, 1}, {314, 175, 3, 1}, {416, 173, 4, 1}, {497, 170, 5, 1}, {578, 164, 6, 1}, {660, 168, 7, 1},
    {41, 268, 0, 2}, {130, 266, 1, 2}, {219, 260, 2, 2}, {310, 266, 3, 2}, {416, 267, 4, 2}, {504, 261, 5, 2}, {594, 265, 6, 2}, {684, 269, 7, 2},
    {37, 371, 0, 3}, {124, 369, 1, 3}, {211, 368, 2, 3}, {302, 369, 3, 3}, {425, 375, 4, 3}, {512, 368, 5, 3}, {602, 365, 6, 3}, {691, 368, 7, 3},
};

static SpecialGridPlacement gMushroomGridPlacement[] = {{110, 441, 0, 0}, {237, 360, 1, 0}, {298, 458, 2, 0}, {355, 296, 3, 0}, {387, 203, 4, 0}, {460, 385, 5, 0}, {486, 478, 6, 0}, {552, 283, 7, 0}};

static SpecialGridPlacement gAquariumGridPlacement[] = {{113, 185, 0, 0}, {306, 120, 1, 0}, {356, 270, 2, 0}, {622, 120, 3, 0}, {669, 270, 4, 0}, {122, 355, 5, 0}, {365, 458, 6, 0}, {504, 417, 7, 0}};

void ZenGarden::DrawBackdrop(Graphics *g) {
    old_ZenGarden_DrawBackdrop(this, g);
}

SpecialGridPlacement *ZenGarden::GetSpecialGridPlacements(int &theCount) {
    if (mBoard->mBackground == BackgroundType::BACKGROUND_MUSHROOM_GARDEN) {
        theCount = std::size(gMushroomGridPlacement);
        return gMushroomGridPlacement;
    }
    if (mBoard->mBackground == BackgroundType::BACKGROUND_ZOMBIQUARIUM) {
        theCount = std::size(gAquariumGridPlacement);
        return gAquariumGridPlacement;
    }
    if (mBoard->mBackground == BackgroundType::BACKGROUND_GREENHOUSE) {
        theCount = std::size(gGreenhouseGridPlacement);
        return gGreenhouseGridPlacement;
    }

    return nullptr;
}

int ZenGarden::GridToPixelX(int theGridX, int theGridY) {
    int aCount;
    SpecialGridPlacement *aSpecialGrids = GetSpecialGridPlacements(aCount);
    for (int i = 0; i < aCount; i++) {
        SpecialGridPlacement &aGrid = aSpecialGrids[i];
        if (theGridX == aGrid.mGridX && theGridY == aGrid.mGridY) {
            return aGrid.mPixelX;
        }
    }
    return -1;
}

int ZenGarden::GridToPixelY(int theGridX, int theGridY) {
    int aCount;
    SpecialGridPlacement *aSpecialGrids = GetSpecialGridPlacements(aCount);
    for (int i = 0; i < aCount; i++) {
        SpecialGridPlacement &aGrid = aSpecialGrids[i];
        if (theGridX == aGrid.mGridX && theGridY == aGrid.mGridY) {
            return aGrid.mPixelY;
        }
    }
    return -1;
}

GridItem *ZenGarden::GetStinky() {
    GridItem *aGridItem = nullptr;
    while (mBoard->IterateGridItems(aGridItem)) {
        if (aGridItem->mGridItemType == GridItemType::GRIDITEM_STINKY) {
            return aGridItem;
        }
    }
    return nullptr;
}