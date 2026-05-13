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

#ifndef PVZ_LAWN_BOARD_SEED_BANK_H
#define PVZ_LAWN_BOARD_SEED_BANK_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Symbols.h"

#include "SeedPacket.h"

class SeedBank;

struct ShopSeedPacket : GameObject {
    int mStoreItemType;         // 13 (offset 52)
    int mPacketOffsetX;         // 14 (offset 56)
    SeedBank *mSeedBank;        // 15 (offset 60)
    float mSelectAnimCounter;   // 16 (offset 64)
    float mSelectArrowAnimTime; // 17 (offset 68)
    float mSelectScale;         // 18 (offset 72)
    SeedType mSeedType;         // 19 (offset 76)
    bool mPlayerIndex;
    bool mIsSelected;
    bool mShowBothPlayerCursors;

    void Update() {
        reinterpret_cast<void (*)(ShopSeedPacket *)>(ShopSeedPacket_UpdateAddr)(this);
    }
}; // size: 21 ints


class HitResult;
class SeedBank : public GameObject {
public:
    int mNumPackets;                    // 13
    int mCutSceneDarken;                // 14
    int mConveyorBeltCounter;           // 15
    bool mIsZombie;                     // 64
    int *mToolTipWidget;                // 17
    SeedPacket mSeedPackets[10];        // 18 ~ 307
    ShopSeedPacket mShopSeedPackets[7]; // 308 ~ 354
    int unknownMembers[100];            // 355 ~ 454
    // size: 455 ints

    int GetNumSeedsOnConveyorBelt() {
        return reinterpret_cast<int (*)(SeedBank *)>(SeedBank_GetNumSeedsOnConveyorBeltAddr)(this);
    }

    SeedBank(bool thePlayerIndex) {
        _constructor(thePlayerIndex);
    }
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
