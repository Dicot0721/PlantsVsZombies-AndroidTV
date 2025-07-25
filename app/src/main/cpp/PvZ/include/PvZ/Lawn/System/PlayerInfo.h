//
// Created by 28636 on 2025/5/27.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_PLAYERINFO_H
#define PLANTSVSZOMBIES_ANDROIDTV_PLAYERINFO_H

#include "../Common/ConstEnums.h"
#include "PvZ/Lawn/Widget/LeaderboardsWidget.h"

class PottedPlant {
public:
    enum FacingDirection // Prefix: FACING
    {
        FACING_RIGHT,
        FACING_LEFT
    };

    SeedType mSeedType : 7;
    FacingDirection mFacing : 1;

    GardenType mWhichZenGarden : 2;
    int mY : 2;
    DrawVariation mDrawVariation : 4;

    int mX : 3;
    PottedPlantNeed mPlantNeed : 3;
    PottedPlantAge mPlantAge : 2;

    int mTimesFed : 3;
    int mFeedingsPerGrow : 3;
    int : 2;

    int mLastWateredTime;       // 1
    int mLastNeedFulfilledTime; // 2
    int mLastFertilizedTime;    // 3
    int mLastChocolateTime;     // 4
};

class PlayerInfo {
public:
    int *vTable;                // 0
    int unk1;                   // 1
    int *mProFileMgr;           // 2
    int unk2;                   // 3
    char *mName;                // 4
    int mUseSeq;                // 5
    int mId;                    // 6
    int mProfileId;             // 7
    int mFlags;                 // 8
    int mLevel;                 // 9
    int mCoins;                 // 10
    int mChallengeRecords[100]; // 11 ~ 110 ， 但末尾6个完全不会用到，可以成为我的自己存数据的空间。
    int mPurchases[36];         // 111 ~ 146 ，本应该是mPurchases[80]，111 ~ 190，但仅用到了前36个。
    bool mAchievements[12];     // 147 ~ 149, 从mPurchases[80]分出来的
    bool mUnused[145];          // 150 ~
    bool mIsVibrateClosed;
    bool mZombatarEnabled;
    unsigned char mZombatarHat;
    unsigned char mZombatarHatColor;
    unsigned char mZombatarHair;
    unsigned char mZombatarHairColor;
    unsigned char mZombatarFacialHair;
    unsigned char mZombatarFacialHairColor;
    unsigned char mZombatarAccessory;
    unsigned char mZombatarAccessoryColor;
    unsigned char mZombatarEyeWear;
    unsigned char mZombatarEyeWearColor;
    unsigned char mZombatarTidBit;
    unsigned char mZombatarTidBitColor;
    bool mIs3DAcceleratedClosed;   // ~ 189, 从mPurchases[80]分出来的
    int mUsedCoins;                // 190, 从mPurchases[80]分出来的
    int unkMem4[2];                // 191 ~ 192
    int mLastStinkyChocolateTime;  // 193
    int mStinkyPosX;               // 194
    int mStinkyPosY;               // 195
    int mNumPottedPlants;          // 196
    int unk4;                      // 197
    PottedPlant mPottedPlants[50]; // 198 ~ 447
    double mMusicVolume;           // 448 ~ 449
    double mSoundVolume;           // 450 ~ 451
    int unkMems6[5];               // 452 ~ 456
    bool mHelpTextSeen[6];         // 1828 ~ 1833
    int unk5;                      // 459
    bool unkBool1;                 // 1840
    bool unkBool2;                 // 1841
    bool mPassedShopSeedTutorial;  // 1842
    bool mMailMessageRead[32];     // 1843 ~ 1874 ，紧密存放，可以存放32x8个bool
    bool mMailMessageSeen[32];     // 1875 ~ 1906 ，紧密存放，可以存放32x8个bool
    GameStats mGameStats;          // 477 ~ 514
    int unk6;                      // 515
    // 大小516个整数
};

// 大小未知，故成员全部放在基类PlayerInfo
class DefaultPlayerInfo : public PlayerInfo {
public:
};

class LawnPlayerInfo : public PlayerInfo {
public:
    int GetFlag(int theFlag) {
        return reinterpret_cast<int (*)(LawnPlayerInfo *, int)>(LawnPlayerInfo_GetFlagAddr)(this, theFlag);
    }

    void AddCoins(int theAmount);
};


#endif // PLANTSVSZOMBIES_ANDROIDTV_PLAYERINFO_H
