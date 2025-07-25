//
// Created by 28636 on 2025/5/26.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_CURSOROBJECT_H
#define PLANTSVSZOMBIES_ANDROIDTV_CURSOROBJECT_H

#include "GameObject.h"

#include "PvZ/Symbols.h"

class Coin;
class Plant;

class CursorObject : public GameObject {
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

class CursorPreview : public GameObject {
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

#endif // PLANTSVSZOMBIES_ANDROIDTV_CURSOROBJECT_H
