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

#ifndef PVZ_LAWN_BOARD_SEED_BANK_H
#define PVZ_LAWN_BOARD_SEED_BANK_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Symbols.h"

#include "SeedPacket.h"

struct ShopSeedPacket {
    int unknown[21];
}; // 大小21个整数


class HitResult;
class SeedBank : public __GameObject {
public:
    int mNumPackets;                    // 13
    int mCutSceneDarken;                // 14
    int mConveyorBeltCounter;           // 15
    bool mIsZombie;                     // 64
    int *mToolTipWidget;                // 17
    SeedPacket mSeedPackets[10];        // 18 ~ 307
    ShopSeedPacket mShopSeedPackets[7]; // 308 ~ 354
    int unknownMembers[100];            // 355 ~ 454
    // 大小455个整数

    int GetNumSeedsOnConveyorBelt() {
        return reinterpret_cast<int (*)(SeedBank *)>(SeedBank_GetNumSeedsOnConveyorBeltAddr)(this);
    }

    SeedBank(bool thePlayerIndex);
    ~SeedBank() {
        _destructor();
    }
    void Draw(Sexy::Graphics *g);
    bool MouseHitTest(int x, int y, HitResult *theHitResult);
    void UpdateWidth();
    void Move(int x, int y);
    bool ContainsPoint(int x, int y);
    bool BeginDraw(Sexy::Graphics *g);
    void EndDraw(Sexy::Graphics *g);
    void AddSeed(int a2, int a3);

protected:
    friend void InitHookFunction();

    void _constructor(bool thePlayerIndex);
    void _destructor() {
        reinterpret_cast<void (*)(SeedBank *)>(SeedBank_DeleteAddr)(this);
    }
};

/***************************************************************************************************************/


inline void (*old_SeedBank_Draw)(SeedBank *seedBank, Sexy::Graphics *graphics);

inline void (*old_SeedBank_SeedBank)(SeedBank *seedBank, bool playerIndex);

inline void (*old_SeedBank_UpdateWidth)(SeedBank *seedBank);

inline bool (*old_SeedBank_BeginDraw)(SeedBank *seedBank, Sexy::Graphics *graphics);

inline void (*old_SeedBank_EndDraw)(SeedBank *seedBank, Sexy::Graphics *graphics);

inline void (*old_SeedBank_AddSeed)(SeedBank *seedBank, int a2, int a3);

#endif // PVZ_LAWN_BOARD_SEED_BANK_H
