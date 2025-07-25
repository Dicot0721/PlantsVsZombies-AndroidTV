#ifndef PVZ_LAWN_COIN_H
#define PVZ_LAWN_COIN_H

#include "GameObject.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Lawn/System/PlayerInfo.h"
#include "PvZ/Symbols.h"

class Coin : public GameObject {
public:
    float mPosX;                   // 13
    float mPosY;                   // 14
    float mPrevPosX;               // 15
    float mPrevPosY;               // 16
    float mVelX;                   // 17
    float mVelY;                   // 18
    float mScale;                  // 19
    bool mDead;                    // 80
    int mFadeCount;                // 21
    float mCollectX;               // 22
    float mCollectY;               // 23
    int mGroundY;                  // 24
    int mCoinAge;                  // 25
    bool mIsBeingCollected;        // 104
    int mCollectedByPlayerIndex;   // 27
    int mDisappearCounter;         // 28
    CoinType mType;                // 29
    CoinMotion mCoinMotion;        // 30
    AttachmentID mAttachmentID[3]; // 31 ~ 33
    float mCollectionDistance;     // 34
    SeedType mUsableSeedType;      // 35
    PottedPlant mPottedPlantSpec;  // 36
    int unknownMember[4];          // 37 ~ 40
    bool mNeedsBouncyArrow;        // 164
    bool mHasBouncyArrow;          // 165
    bool mHitGround;               // 166
    int mTimesDropped;             // 42
    int mPlayerIndex;              // 43
    float unk2;                    // 44
    bool unk3;                     // 180
    int unk4;                      // 46
    bool unk5;                     // 188
    int mAutoCollectAge;           // 48
    bool unk7;                     // 196
    bool unk8;                     // 197
    int unkInt;                    // 50
    // 大小51个整数

    void MouseDown(int x, int y, int theClickCount) {
        reinterpret_cast<void (*)(Coin *, int, int, int)>(Coin_MouseDownAddr)(this, x, y, theClickCount);
    }
    void Collect(int thePlayerIndex) {
        reinterpret_cast<void (*)(Coin *, int)>(Coin_CollectAddr)(this, thePlayerIndex);
    }
    void Die() {
        reinterpret_cast<void (*)(Coin *)>(Coin_DieAddr)(this);
    }
    bool IsLevelAward() {
        return reinterpret_cast<bool (*)(Coin *)>(Coin_IsLevelAwardAddr)(this);
    }
    bool IsPresentWithAdvice() {
        return reinterpret_cast<bool (*)(Coin *)>(Coin_IsPresentWithAdviceAddr)(this);
    }
    bool IsMoney() {
        return reinterpret_cast<bool (*)(Coin *)>(Coin_IsMoneyAddr)(this);
    }
    void PlayGroundSound() {
        reinterpret_cast<void (*)(Coin *)>(Coin_PlayGroundSoundAddr)(this);
    }
    int GetDisappearTime() {
        return reinterpret_cast<int (*)(Coin *)>(Coin_GetDisappearTimeAddr)(this);
    }
    void StartFade() {
        reinterpret_cast<void (*)(Coin *)>(Coin_StartFadeAddr)(this);
    }
    float GetSunScale() {
        return reinterpret_cast<float (*)(Coin *)>(Coin_GetSunScaleAddr)(this);
    }

    void GamepadCursorOver(int thePlayerIndex);
    void Update();
    void UpdateFallForAward();
    void UpdateFall();
    bool MouseHitTest(int theX, int theY, int **theHitResult, int thePlayerIndex);
};

/***************************************************************************************************************/
inline bool enableManualCollect;
inline bool BanDropCoin;


inline void (*old_Coin_GamepadCursorOver)(Coin *coin, int a2);


inline void (*old_Coin_Update)(Coin *coin);

inline void (*old_Coin_UpdateFall)(Coin *coin);

inline bool (*old_Coin_MouseHitTest)(Coin *coin, int a2, int a3, int **hitResult, int a5);

#endif // PVZ_LAWN_COIN_H
