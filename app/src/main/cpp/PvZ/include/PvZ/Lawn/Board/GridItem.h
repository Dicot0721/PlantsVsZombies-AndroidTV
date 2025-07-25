#ifndef PVZ_LAWN_BOARD_GRID_ITEM_H
#define PVZ_LAWN_BOARD_GRID_ITEM_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/Symbols.h"

class Board;
class Reanimation;

class MotionTrailFrame {
public:
    float mPosX;
    float mPosY;
    float mAnimTime;
};

class GridItem {
public:
    int placeHolder[4];
    LawnApp *mApp;                           // 4
    Board *mBoard;                           // 5
    GridItemType mGridItemType;              // 6
    GridItemState mGridItemState;            // 7
    int mGridX;                              // 8
    int mGridY;                              // 9
    int mGridItemCounter;                    // 10
    int mRenderOrder;                        // 11
    bool mDead;                              // 48
    float mPosX;                             // 13
    float mPosY;                             // 14
    float mGoalX;                            // 15
    float mGoalY;                            // 16
    Reanimation *mGridItemReanimID;          // 17
    int *mGridItemParticleID;                // 18
    ZombieType mZombieType;                  // 19
    SeedType mSeedType;                      // 20
    ScaryPotType mScaryPotType;              // 21
    bool mHighlighted;                       // 88
    int mTransparentCounter;                 // 23
    int mSunCount;                           // 24
    MotionTrailFrame mMotionTrailFrames[12]; // 25 ~ 60
    int mMotionTrailCount;                   // 61
    int unkMems[14];                         // 62 ~ 75
    // 大小76个整数

    void GridItemDie() {
        reinterpret_cast<void (*)(GridItem *)>(GridItem_GridItemDieAddr)(this);
    }

    void DrawScaryPot(Sexy::Graphics *g);
    void Update();
    void UpdateScaryPot();
    void DrawStinky(Sexy::Graphics *g);
    void DrawSquirrel(Sexy::Graphics *g);
    void DrawCrater(Sexy::Graphics *g);
};

/***************************************************************************************************************/
inline bool transparentVase;


inline void (*old_GridItem_Update)(GridItem *a1);

inline void (*old_GridItem_UpdateScaryPot)(GridItem *scaryPot);

inline void (*old_GridItem_DrawStinky)(GridItem *mStinky, Sexy::Graphics *graphics);

#endif // PVZ_LAWN_BOARD_GRID_ITEM_H
