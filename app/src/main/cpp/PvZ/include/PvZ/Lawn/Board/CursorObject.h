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

#ifndef PVZ_LAWN_BOARD_CURSOR_OBJECT_H
#define PVZ_LAWN_BOARD_CURSOR_OBJECT_H

#include "GameObject.h"

#include "PvZ/Symbols.h"

class Coin;
class Plant;

class CursorObject : public __GameObject {
public:
    int mSeedBankIndex;        // 13
    SeedType mType;            // 14
    SeedType mImitaterType;    // 15
    CursorType mCursorType;    // 16
    Coin *mCoinID;             // 17
    Plant *mGlovePlantID;      // 18
    Plant *mDuplicatorPlantID; // 19
    Plant *mCobCannonPlantID;  // 20
    int mHammerDownCounter;    // 21
    int mReanimCursorID;       // 22
    // 大小23个整数

    void Draw(Sexy::Graphics *g) {
        reinterpret_cast<void (*)(CursorObject *, Sexy::Graphics *)>(CursorObject_DrawAddr)(this, g);
    }
    void Update() {
        reinterpret_cast<void (*)(CursorObject *)>(CursorObject_UpdateAddr)(this);
    }

    bool BeginDraw(Sexy::Graphics *g);
    void EndDraw(Sexy::Graphics *g);
};

class CursorPreview : public __GameObject {
public:
    int mGridX;      // 13
    int mGridY;      // 14
    int playerIndex; // 15
    // 大小16个整数

    void Update() {
        reinterpret_cast<void (*)(CursorPreview *)>(CursorPreview_UpdateAddr)(this);
    }
};

inline bool (*old_CursorObject_BeginDraw)(CursorObject *cursorObject, Sexy::Graphics *graphics);

inline void (*old_CursorObject_EndDraw)(CursorObject *cursorObject, Sexy::Graphics *graphics);

#endif // PVZ_LAWN_BOARD_CURSOR_OBJECT_H
