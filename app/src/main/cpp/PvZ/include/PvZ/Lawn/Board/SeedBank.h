#ifndef PVZ_LAWN_BOARD_SEED_BANK_H
#define PVZ_LAWN_BOARD_SEED_BANK_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Symbols.h"

#include "SeedPacket.h"

struct ShopSeedPacket {
    int unknown[21];
}; // 大小21个整数


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
    // 大小455个整数

    int GetNumSeedsOnConveyorBelt() {
        return reinterpret_cast<int (*)(SeedBank *)>(SeedBank_GetNumSeedsOnConveyorBeltAddr)(this);
    }

    SeedBank(bool thePlayerIndex);
    void Draw(Sexy::Graphics *g);
    bool MouseHitTest(int x, int y, HitResult *theHitResult);
    void UpdateWidth();
    void Move(int x, int y);
    bool ContainsPoint(int x, int y);
    bool BeginDraw(Sexy::Graphics *g);
    void EndDraw(Sexy::Graphics *g);

protected:
    void __Constructor(bool thePlayerIndex);
};

/***************************************************************************************************************/


inline void (*old_SeedBank_Draw)(SeedBank *seedBank, Sexy::Graphics *graphics);

inline void (*old_SeedBank_SeedBank)(SeedBank *seedBank, bool playerIndex);

inline void (*old_SeedBank_UpdateWidth)(SeedBank *seedBank);

inline bool (*old_SeedBank_BeginDraw)(SeedBank *seedBank, Sexy::Graphics *graphics);

inline void (*old_SeedBank_EndDraw)(SeedBank *seedBank, Sexy::Graphics *graphics);


#endif // PVZ_LAWN_BOARD_SEED_BANK_H
