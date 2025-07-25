//
// Created by 28636 on 2025/5/24.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_LAWNMOWER_H
#define PLANTSVSZOMBIES_ANDROIDTV_LAWNMOWER_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Symbols.h"

class LawnApp;
class Board;
class Zombie;
class LawnMower {
public:
    LawnApp* mApp;              //+0x0
    Board* mBoard;              //+0x4
    float mPosX;                //+0x8
    float mPosY;                //+0xC
    int mRenderOrder;           //+0x10
    int mRow;                   //+0x14
    int mAnimTicksPerFrame;     //+0x18
    ReanimationID mReanimID;    //+0x1C
    int mChompCounter;          //+0x20
    int mRollingInCounter;      //+0x24
    int mSquishedCounter;       //+0x28
    LawnMowerState mMowerState; //+0x2C
    bool mDead;                 //+0x30
    bool mVisible;              //+0x31
    LawnMowerType mMowerType;   //+0x34
    float mAltitude;            //+0x38
    MowerHeight mMowerHeight;   //+0x3C
    int mLastPortalX;           //+0x40

    void StartMower() {
        reinterpret_cast<void (*)(LawnMower*)>(LawnMower_StartMowerAddr)(this);
    }

    void Update();
};

inline void (*old_LawnMower_Update)(LawnMower* lawnMover);

#endif // PLANTSVSZOMBIES_ANDROIDTV_LAWNMOWER_H
