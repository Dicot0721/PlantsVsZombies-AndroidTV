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

#ifndef PVZ_LAWN_BOARD_SEED_PACKET_H
#define PVZ_LAWN_BOARD_SEED_PACKET_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Symbols.h"

#include "GameObject.h"

class HitResult;
class SeedBank;
class SeedPacket : public __GameObject {
public:
    int mRefreshCounter;             // 13
    int mRefreshTime;                // 14
    int mIndex;                      // 15
    int mOffsetY;                    // 16
    SeedType mPacketType;            // 17
    SeedType mImitaterType;          // 18
    int mSlotMachineCountDown;       // 19
    SeedType mSlotMachiningNextSeed; // 20
    float mSlotMachiningPosition;    // 21
    bool mActive;                    // 88
    bool mRefreshing;                // 89
    int mTimesUsed;                  // 23
    SeedBank *mSeedBank;             // 24
    float mLastSelectedTime;         // 25
    int unknownIntMember1;           // 26
    int unknownIntMember2;           // 27
    bool mSelectedBy2P;              // 112
    bool mSelected;                  // 113
    bool mSelectedByBothPlayer;      // 114
    // 大小29个整数

    void DrawBackground(Sexy::Graphics *g) {
        reinterpret_cast<void (*)(SeedPacket *, Sexy::Graphics *)>(SeedPacket_DrawBackgroundAddr)(this, g);
    }
    bool CanPickUp() {
        return reinterpret_cast<bool (*)(SeedPacket *)>(SeedPacket_CanPickUpAddr)(this);
    }
    bool MouseHitTest(int x, int y, HitResult *theHitResult) {
        return reinterpret_cast<bool (*)(SeedPacket *, int, int, HitResult *)>(SeedPacket_MouseHitTestAddr)(this, x, y, theHitResult);
    }
    bool GetPlayerIndex() {
        return reinterpret_cast<bool (*)(SeedPacket *)>(SeedPacket_GetPlayerIndexAddr)(this);
    }
    void DrawMiddle(Sexy::Graphics *g) {
        reinterpret_cast<void (*)(SeedPacket *, Sexy::Graphics *)>(SeedPacket_DrawMiddleAddr)(this, g);
    }
    void Deactivate() {
        reinterpret_cast<void (*)(SeedPacket *)>(SeedPacket_DeactivateAddr)(this);
    }
//    void WasPlanted(int thePlayerIndex) {
//        reinterpret_cast<void (*)(SeedPacket *, int)>(SeedPacket_WasPlantedAddr)(this, thePlayerIndex);
//    }

    void Update();
    void UpdateSelected();
    void DrawOverlay(Sexy::Graphics *g);
    void Draw(Sexy::Graphics *g);
    void FlashIfReady();
    bool BeginDraw(Sexy::Graphics *g);
    void EndDraw(Sexy::Graphics *g);
    void SetPacketType(SeedType theSeedType, SeedType theImitaterType);
    void WasPlanted(int thePlayerIndex);
};

void DrawSeedPacket(Sexy::Graphics *g,
                    float x,
                    float y,
                    SeedType theSeedType,
                    SeedType theImitaterType,
                    float thePercentDark,
                    int theGrayness,
                    bool theDrawCost,
                    bool theUseCurrentCost,
                    bool isZombieSeed,
                    bool isSeedPacketSelected);

/***************************************************************************************************************/
inline bool showCoolDown;


inline void (*old_SeedPacket_Update)(SeedPacket *seedPacket);

inline void (*old_SeedPacket_UpdateSelected)(SeedPacket *seedPacket);

inline void (*old_SeedPacket_DrawOverlay)(SeedPacket *seedPacket, Sexy::Graphics *graphics);

inline void (*old_SeedPacket_Draw)(SeedPacket *seedPacket, Sexy::Graphics *graphics);

inline void (*old_SeedPacket_MouseDown)(SeedPacket *seedPacket, int x, int y, int c, int unk);

inline bool (*old_SeedPacket_BeginDraw)(SeedPacket *, Sexy::Graphics *);

inline void (*old_SeedPacket_EndDraw)(SeedPacket *, Sexy::Graphics *);

inline void (*old_SeedPacket_FlashIfReady)(SeedPacket *seedPacket);

inline void (*old_SeedPacket_SetPacketType)(SeedPacket *, SeedType theSeedType, SeedType theImitaterType);

inline void (*old_SeedPacket_WasPlanted)(SeedPacket *, int player);

#endif // PVZ_LAWN_BOARD_SEED_PACKET_H
