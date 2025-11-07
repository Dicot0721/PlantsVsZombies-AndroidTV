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

#ifndef PVZ_LAWN_BOARD_BOARD_H
#define PVZ_LAWN_BOARD_BOARD_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Lawn/Widget/AchievementsWidget.h"
#include "PvZ/SexyAppFramework/Misc/KeyCodes.h"
#include "PvZ/SexyAppFramework/Widget/ButtonListener.h"
#include "PvZ/SexyAppFramework/Widget/Widget.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Common/DataArray.h"

#include "Coin.h"
#include "GridItem.h"
#include "LawnMower.h"
#include "Plant.h"
#include "Projectile.h"
#include "PvZ/Lawn/Widget/WaitForSecondPlayerDialog.h"
#include "Zombie.h"

constexpr int MAX_GRID_SIZE_X = 9;
constexpr int MAX_GRID_SIZE_Y = 6;
constexpr int MAX_ZOMBIES_IN_WAVE = 50;
constexpr int MAX_ZOMBIE_WAVES = 100;
constexpr int MAX_GRAVE_STONES = MAX_GRID_SIZE_X * MAX_GRID_SIZE_Y;
constexpr int MAX_POOL_GRID_SIZE = 10;
constexpr int MAX_RENDER_ITEMS = 2048;
constexpr int PROGRESS_METER_COUNTER = 150;

class LawnApp;
class CursorObject;
class CursorPreview;
class GameButton;
class CustomMessageWidget;
class SeedBank;
class CutScene;
class Challenge;
class Reanimation;
class TodParticleSystem;
class GamepadControls;

class HitResult {
public:
    void *mObject;
    GameObjectType mObjectType;
};

class RenderItem {
public:
    RenderObjectType mRenderObjectType;
    int mZPos;
    union {
        GameObject *mGameObject;
        Plant *mPlant;
        Zombie *mZombie;
        Coin *mCoin;
        Projectile *mProjectile;
        CursorPreview *mCursorPreview;
        TodParticleSystem *mParticleSytem;
        Reanimation *mReanimation;
        GridItem *mGridItem;
        LawnMower *mMower;
        BossPart mBossPart;
        int mBoardGridY;
    };
};

struct ZombiePicker {
    int mZombieCount;
    int mZombiePoints;
    int mZombieTypeCount[ZombieType::NUM_ZOMBIE_TYPES];
    int mAllWavesZombieTypeCount[ZombieType::NUM_ZOMBIE_TYPES];
};

/*inline*/ void ZombiePickerInitForWave(ZombiePicker *theZombiePicker);
/*inline*/ void ZombiePickerInit(ZombiePicker *theZombiePicker);

struct PlantsOnLawn {
    Plant *mUnderPlant;
    Plant *mPumpkinPlant;
    Plant *mFlyingPlant;
    Plant *mNormalPlant;
};

struct BungeeDropGrid {
    TodWeightedGridArray mGridArray[MAX_GRID_SIZE_X * MAX_GRID_SIZE_Y];
    int mGridArrayCount;
};

struct PlantRbTree {
    int unk[6];
};

enum TouchState {
    TOUCHSTATE_NONE = 0,
    TOUCHSTATE_SEED_BANK = 1,
    TOUCHSTATE_SHOVEL_RECT = 2,
    TOUCHSTATE_BUTTER_RECT = 3,
    TOUCHSTATE_BOARD = 4,
    TOUCHSTATE_BOARD_MOVED_FROM_SEED_BANK = 5,
    TOUCHSTATE_BOARD_MOVED_FROM_SHOVEL_RECT = 6,
    TOUCHSTATE_BOARD_MOVED_FROM_BUTTER_RECT = 7,
    TOUCHSTATE_VALID_COBCONON = 8,
    TOUCHSTATE_USEFUL_SEED_PACKET = 9,
    TOUCHSTATE_UNUSED = 10,
    TOUCHSTATE_HEAVY_WEAPON = 11,
    TOUCHSTATE_PICKING_SOMETHING = 12,
    TOUCHSTATE_ZEN_GARDEN_TOOLS = 13,
    TOUCHSTATE_BOARD_MOVED_FROM_ZEN_GARDEN_TOOLS = 14,
    TOUCHSTATE_VALID_COBCONON_SECOND = 15,
};

enum TouchPlayerIndex {
    TOUCHPLAYER_NONE = -1,
    TOUCHPLAYER_PLAYER1 = 0,
    TOUCHPLAYER_PLAYER2 = 1,
};

class Board : public Sexy::__Widget {
public:
    Sexy::ButtonListener mButtonListener;         // 64
    int unknownMembers1[4];                       // 65 ~ 68
    LawnApp *mApp;                                // 69
    DataArray<Zombie> mZombiesList;               // 70 ~ 76
    DataArray<Plant> mPlantsList;                 // 77 ~ 83
    DataArray<Projectile> mProjectilesList;       // 84 ~ 90
    DataArray<Coin> mCoinsList;                   // 91 ~ 97
    DataArray<LawnMower> mLawnMowersList;         // 98 ~ 104
    DataArray<GridItem> mGridItemsList;           // 105 ~ 111
    PlantRbTree mTangleKelpTree;                  // 112 ~ 117
    PlantRbTree mFlowerPotTree;                   // 118 ~ 123
    PlantRbTree mPumpkinTree;                     // 124 ~ 129
    CustomMessageWidget *mAdvice;                 // 130
    SeedBank *mSeedBank1;                         // 131
    SeedBank *mSeedBank2;                         // 132
    int unknownMembers3[7];                       // 133 ~ 139
    GamepadControls *mGamepadControls1;           // 140
    GamepadControls *mGamepadControls2;           // 141
    CursorObject *mCursorObject1;                 // 142
    CursorObject *mCursorObject2;                 // 143
    CursorPreview *mCursorPreview1;               // 144
    CursorPreview *mCursorPreview2;               // 145
    int *mToolTipWidget;                          // 146
    int *mToolTipFont;                            // 147
    CutScene *mCutScene;                          // 148
    Challenge *mChallenge;                        // 149
    bool unknownBool;                             // 600
    bool mPaused;                                 // 601
    GridSquareType mGridSquareType[9][6];         // 151 ~ 204
    int mGridCelLook[9][6];                       // 205 ~ 258
    int mGridCelOffset[9][6][2];                  // 259 ~ 366
    int mGridCelFog[9][7];                        // 367 ~ 429
    bool mEnableGraveStones;                      // 1720
    int mSpecialGraveStoneX;                      // 431
    int mSpecialGraveStoneY;                      // 432
    float mFogOffset;                             // 433
    int mOffsetMoved;                             // 434
    ReanimationID mCoverLayerAnimIDs[7];          // 435 ~ 441
    int mFogBlownCountDown;                       // 442
    PlantRowType mPlantRow[6];                    // 443 ~ 448
    int mWaveRowGotLawnMowered[6];                // 449 ~ 454
    int mBonusLawnMowersRemaining;                // 455
    int mIceMinX[6];                              // 456 ~ 461
    int mIceTimer[6];                             // 462 ~ 467
    ParticleSystemID mIceParticleID[6];           // 468 ~ 473
    TodSmoothArray mRowPickingArray[6];           // 474 ~ 497
    ZombieType mZombiesInWave[100][50];           // 498 ~ 5497
    bool mZombieAllowed[100];                     // 5498 ~ 5522
    int mSunCountDown;                            // 5523
    int mNumSunsFallen;                           // 5524
    int mShakeCounter;                            // 5525
    int mShakeAmountX;                            // 5526
    int mShakeAmountY;                            // 5527
    BackgroundType mBackground;                   // 5528
    int mLevel;                                   // 5529
    int mSodPosition;                             // 5530
    int mPrevMouseX;                              // 5531
    int mPrevMouseY;                              // 5532
    int mSunMoney1;                               // 5533
    int mSunMoney2;                               // 5534
    int mDeathMoney;                              // 5535
    int mNumWaves;                                // 5536
    int mMainCounter;                             // 5537
    int mEffectCounter;                           // 5538
    int mDrawCount;                               // 5539
    int mRiseFromGraveCounter;                    // 5540
    int mOutOfMoneyCounter;                       // 5541
    int mCurrentWave;                             // 5542
    int mTotalSpawnedWaves;                       // 5543
    TutorialState mTutorialState;                 // 5544
    int *mTutorialParticleID;                     // 5545
    int mTutorialTimer;                           // 5546
    int mLastBungeeWave;                          // 5547
    int mZombieHealthToNextWave;                  // 5548
    int mZombieHealthWaveStart;                   // 5549
    int mZombieCountDown;                         // 5550
    int mZombieCountDownStart;                    // 5551
    int mHugeWaveCountDown;                       // 5552
    bool mHelpDisplayed[72];                      // 5553 ~ 5570
    AdviceType mHelpIndex;                        // 5571
    bool mFinalBossKilled;                        // 22288
    bool mShowShovel;                             // 22289
    bool mShowButter;                             // 22290
    bool mShowHammer;                             // 22291
    int mCoinBankFadeCount;                       // 5573
    DebugTextMode mDebugTextMode;                 // 5574
    bool mLevelComplete;                          // 22300
    bool mNewWallNutAndSunFlowerAndChomperOnly;   // 在对齐间隙插入成员，22301
    char mNewPeaShooterCount;                     // 在对齐间隙插入成员，22302
    int mBoardFadeOutCounter;                     // 5576
    int mNextSurvivalStageCounter;                // 5577
    int mScoreNextMowerCounter;                   // 5578
    bool mLevelAwardSpawned;                      // 22316
    int mProgressMeterWidth;                      // 5580
    int mFlagRaiseCounter;                        // 5581
    int mIceTrapCounter;                          // 5582
    int mBoardRandSeed;                           // 5583
    ParticleSystemID mPoolSparklyParticleID;      // 5584
    ReanimationID mFwooshID[MAX_GRID_SIZE_Y][12]; // 5585 ~ 5656
    int mFwooshCountDown;                         // 5657
    int mTimeStopCounter;                         // 5658
    bool mDroppedFirstCoin;                       // 22636
    int mFinalWaveSoundCounter;                   // 5660
    int mCobCannonCursorDelayCounter;             // 5661
    int mCobCannonMouseX;                         // 5662
    int mCobCannonMouseY;                         // 5663
    bool mKilledYeti;                             // 22656
    bool mMustacheMode;                           // 22657
    bool mSuperMowerMode;                         // 22658
    bool mFutureMode;                             // 22659
    bool mPinataMode;                             // 22660
    bool mDanceMode;                              // 22661
    bool mDaisyMode;                              // 22662
    bool mSukhbirMode;                            // 22663
    int mPrevBoardResult;                         // 5666
    int mTriggeredLawnMowers;                     // 5667
    int mPlayTimeActiveLevel;                     // 5668
    int mPlayTimeInactiveLevel;                   // 5669
    int mMaxSunPlants;                            // 5670
    int mStartDrawTime;                           // 5671
    int mIntervalDrawTime;                        // 5672
    int mIntervalDrawCountStart;                  // 5673
    float mMinFPS;                                // 5674
    int mPreloadTime;                             // 5675
    int mGameID;                                  // 5676
    int mGravesCleared;                           // 5677
    int mPlantsEaten;                             // 5678
    int mPlantsShoveled;                          // 5679
    int mCoinsCollected;                          // 5680
    int mDiamondsCollected;                       // 5681
    int mPottedPlantsCollected;                   // 5682
    int mChocolateCollected;                      // 5683
    bool mPeaShooterUsed;                         // 22736
    bool mCatapultPlantsUsed;                     // 22737
    int mCollectedCoinStreak;                     // 5685
    int mUnkIntSecondPlayer1;                     // 5686
    bool mUnkBoolSecondPlayer;                    // 22748
    int mUnkIntSecondPlayer2;                     // 5688
    int *mStringSecondPlayer;                     // 5689
    int unknownMembers[8];                        // 5690 ~ 5697

    Projectile *AddProjectile(int theX, int theY, int theRenderOrder, int theRow, ProjectileType theProjectileType) {
        return reinterpret_cast<Projectile *(*)(Board *, int, int, int, int, ProjectileType)>(Board_AddProjectileAddr)(this, theX, theY, theRenderOrder, theRow, theProjectileType);
    }
    int PixelToGridX(int theX, int theY) {
        return reinterpret_cast<int (*)(Board *, int, int)>(Board_PixelToGridXAddr)(this, theX, theY);
    }
    int PixelToGridY(int theX, int theY) {
        return reinterpret_cast<int (*)(Board *, int, int)>(Board_PixelToGridYAddr)(this, theX, theY);
    }
    GridItem *GetLadderAt(int theGridX, int theGridY);
    GridItem *GetScaryPotAt(int theGridX, int theGridY);
    GridItem *GetGridItemAt(GridItemType theGridItemType, int theGridX, int theGridY);
    void Move(int theX, int theY) {
        reinterpret_cast<void (*)(Board *, int, int)>(Board_MoveAddr)(this, theX, theY);
    } // 整体移动整个草坪，包括种子栏和铲子按钮等等。
    void DoFwoosh(int theRow) {
        reinterpret_cast<void (*)(Board *, int)>(Board_DoFwooshAddr)(this, theRow);
    }
    bool IteratePlants(Plant *&thePlant) {
        return reinterpret_cast<bool (*)(Board *, Plant *&)>(Board_IteratePlantsAddr)(this, thePlant);
    }
    bool IterateZombies(Zombie *&theZombie) {
        return reinterpret_cast<bool (*)(Board *, Zombie *&)>(Board_IterateZombiesAddr)(this, theZombie);
    }
    bool IterateProjectiles(Projectile *&theProjectile) {
        return reinterpret_cast<bool (*)(Board *, Projectile *&)>(Board_IterateProjectilesAddr)(this, theProjectile);
    }
    bool IterateCoins(Coin *&theCoin) {
        return reinterpret_cast<bool (*)(Board *, Coin *&)>(Board_IterateCoinsAddr)(this, theCoin);
    }
    bool IterateLawnMowers(LawnMower *&theLawnMower) {
        return reinterpret_cast<bool (*)(Board *, LawnMower *&)>(Board_IterateLawnMowersAddr)(this, theLawnMower);
    }
    bool IterateParticles(TodParticleSystem *&theParticle) {
        return reinterpret_cast<bool (*)(Board *, TodParticleSystem *&)>(Board_IterateParticlesAddr)(this, theParticle);
    }
    bool IterateGridItems(GridItem *&theGridItem) {
        return reinterpret_cast<bool (*)(Board *, GridItem *&)>(Board_IterateGridItemsAddr)(this, theGridItem);
    }
    Plant *GetTopPlantAt(int theGridX, int theGridY, PlantPriority thePriority) {
        return reinterpret_cast<Plant *(*)(Board *, int, int, PlantPriority)>(Board_GetTopPlantAtAddr)(this, theGridX, theGridY, thePriority);
    }
    bool ProgressMeterHasFlags() {
        return reinterpret_cast<bool (*)(Board *)>(Board_ProgressMeterHasFlagsAddr)(this);
    }
    bool IsSurvivalStageWithRepick() {
        return reinterpret_cast<bool (*)(Board *)>(Board_IsSurvivalStageWithRepickAddr)(this);
    }
    void PickUpTool(GameObjectType theObjectType, int thePlayerIndex) {
        reinterpret_cast<void (*)(Board *, GameObjectType, int)>(Board_PickUpToolAddr)(this, theObjectType, thePlayerIndex);
    }
    int GameAxisMove(int theButton, int thePlayerIndex, int theIsLongPress) {
        return reinterpret_cast<int (*)(Board *, int, int, int)>(Board_GameAxisMoveAddr)(this, theButton, thePlayerIndex, theIsLongPress);
    }
    int InitCoverLayer() {
        return reinterpret_cast<int (*)(Board *)>(Board_InitCoverLayerAddr)(this);
    }
    void LoadBackgroundImages() {
        reinterpret_cast<void (*)(Board *)>(Board_LoadBackgroundImagesAddr)(this);
    }
    bool HasConveyorBeltSeedBank(int thePlayerIndex) {
        return reinterpret_cast<bool (*)(Board *, int)>(Board_HasConveyorBeltSeedBankAddr)(this, thePlayerIndex);
    }
    Zombie *ZombieHitTest(int theMouseX, int theMouseY, int thePlayerIndex) {
        return reinterpret_cast<Zombie *(*)(Board *, int, int, int)>(Board_ZombieHitTestAddr)(this, theMouseX, theMouseY, thePlayerIndex);
    }
    void ClearAdviceImmediately() {
        reinterpret_cast<void (*)(Board *)>(Board_ClearAdviceImmediatelyAddr)(this);
    }
    void DisplayAdvice(const pvzstl::string &theAdvice, MessageStyle theMessageStyle, AdviceType theHelpIndex) {
        reinterpret_cast<void (*)(Board *, const pvzstl::string &, MessageStyle, AdviceType)>(Board_DisplayAdviceAddr)(this, theAdvice, theMessageStyle, theHelpIndex);
    }
    void DisplayAdviceAgain(const pvzstl::string &theAdvice, MessageStyle theMessageStyle, AdviceType theHelpIndex) {
        reinterpret_cast<void (*)(Board *, const pvzstl::string &, MessageStyle, AdviceType)>(Board_DisplayAdviceAgainAddr)(this, theAdvice, theMessageStyle, theHelpIndex);
    }

    Plant *NewPlant(int theGridX, int theGridY, SeedType theSeedType, SeedType theImitaterType, int thePlayerIndex) {
        return reinterpret_cast<Plant *(*)(Board *, int, int, SeedType, SeedType, int)>(Board_NewPlantAddr)(this, theGridX, theGridY, theSeedType, theImitaterType, thePlayerIndex);
    }
    bool CanUseGameObject(GameObjectType theGameObject) {
        return reinterpret_cast<int (*)(Board *, GameObjectType)>(Board_CanUseGameObjectAddr)(this, theGameObject);
    }
    Plant *ToolHitTest(int theX, int theY) {
        return reinterpret_cast<Plant *(*)(Board *, int, int)>(Board_ToolHitTestAddr)(this, theX, theY);
    }
    void RefreshSeedPacketFromCursor(int thePlayerIndex) {
        reinterpret_cast<void (*)(Board *, int)>(Board_RefreshSeedPacketFromCursorAddr)(this, thePlayerIndex);
    }
    Plant *GetPlantsOnLawn(int theGridX, int theGridY, PlantsOnLawn *thePlantOnLawn) { // 检查加农炮用
        return reinterpret_cast<Plant *(*)(Board *, int, int, PlantsOnLawn *)>(Board_GetPlantsOnLawnAddr)(this, theGridX, theGridY, thePlantOnLawn);
    }
    GridItem *AddALadder(int theGridX, int theGridY) {
        return reinterpret_cast<GridItem *(*)(Board *, int, int)>(Board_AddALadderAddr)(this, theGridX, theGridY);
    }
    void ClearCursor(int thePlayerIndex) {
        reinterpret_cast<void (*)(Board *, int)>(Board_ClearCursorAddr)(this, thePlayerIndex);
    }
    void MouseDownWithTool(int x, int y, int theClickCount, CursorType theCursorType, int thePlayerIndex) {
        reinterpret_cast<void (*)(Board *, int, int, int, CursorType, int)>(Board_MouseDownWithToolAddr)(this, x, y, theClickCount, theCursorType, thePlayerIndex);
    }
    void SetTutorialState(TutorialState theTutorialState) {
        reinterpret_cast<void (*)(Board *, TutorialState)>(Board_SetTutorialStateAddr)(this, theTutorialState);
    }
    Sexy::Rect GetButterButtonRect() {
        return reinterpret_cast<Sexy::Rect (*)(Board *)>(Board_GetButterButtonRectAddr)(this);
    }
    Zombie *ZombieTryToGet(ZombieID theZombieID) {
        return reinterpret_cast<Zombie *(*)(Board *, ZombieID)>(Board_ZombieTryToGetAddr)(this, theZombieID);
    }
    void MouseDownWithPlant(int x, int y, int theClickCount) {
        reinterpret_cast<void (*)(Board *, int, int, int)>(Board_MouseDownWithPlantAddr)(this, x, y, theClickCount);
    }
    bool CanInteractWithBoardButtons() {
        return reinterpret_cast<bool (*)(Board *)>(Board_CanInteractWithBoardButtonsAddr)(this);
    }
    // Coin *AddCoin(int theX, int theY, CoinType theCoinType, CoinMotion theCoinMotion) {
    // return reinterpret_cast<Coin *(*)(Board *, int, int, CoinType, CoinMotion)>(Board_AddCoinAddr)(this, theX, theY, theCoinType, theCoinMotion);
    // }
    bool CanTakeSunMoney(int theAmount, int thePlayerIndex) {
        return reinterpret_cast<bool (*)(Board *, int, int)>(Board_CanTakeSunMoneyAddr)(this, theAmount, thePlayerIndex);
    }
    Sexy::Rect GetZenButtonRect(GameObjectType theObjectType) {
        return reinterpret_cast<Sexy::Rect (*)(Board *, GameObjectType)>(Board_GetZenButtonRectAddr)(this, theObjectType);
    }
    int PickRowForNewZombie(ZombieType theZombieType) {
        return reinterpret_cast<int (*)(Board *, ZombieType)>(Board_PickRowForNewZombieAddr)(this, theZombieType);
    }
    ZombieType GetIntroducedZombieType() {
        return reinterpret_cast<ZombieType (*)(Board *)>(Board_GetIntroducedZombieTypeAddr)(this);
    }
    ZombieType PickZombieType(int theZombiePoints, int theWaveIndex, ZombiePicker *theZombiePicker) {
        return reinterpret_cast<ZombieType (*)(Board *, int, int, ZombiePicker *)>(Board_PickZombieTypeAddr)(this, theZombiePoints, theWaveIndex, theZombiePicker);
    }
    bool HasLevelAwardDropped() {
        return reinterpret_cast<bool (*)(Board *)>(Board_HasLevelAwardDroppedAddr)(this);
    }
    bool CanDropLoot() {
        return reinterpret_cast<bool (*)(Board *)>(Board_CanDropLootAddr)(this);
    }
    void SpawnZombiesFromGraves() {
        reinterpret_cast<void (*)(Board *)>(Board_SpawnZombiesFromGravesAddr)(this);
    }
    void ClearAdvice(AdviceType theHelpIndex) {
        reinterpret_cast<void (*)(Board *)>(Board_ClearAdviceAddr)(this);
    }
    void NextWaveComing() {
        reinterpret_cast<void (*)(Board *)>(Board_NextWaveComingAddr)(this);
    }
    int GridCellWidth(int theGridX, int theGridY) {
        return reinterpret_cast<int (*)(Board *, int, int)>(Board_GridCellWidthAddr)(this, theGridX, theGridY);
    }
    int GridCellHeight(int theGridX, int theGridY) {
        return reinterpret_cast<int (*)(Board *, int, int)>(Board_GridCellHeightAddr)(this, theGridX, theGridY);
    }
    SeedType GetSeedTypeInCursor(int thePlayerIndex) {
        return reinterpret_cast<SeedType (*)(Board *, int)>(Board_GetSeedTypeInCursorAddr)(this, thePlayerIndex);
    }
    void TryToSaveGame() {
        reinterpret_cast<void (*)(Board *)>(Board_TryToSaveGameAddr)(this);
    }
    bool CanTakeDeathMoney(int theAmount) {
        return reinterpret_cast<bool (*)(Board *, int)>(Board_CanTakeDeathMoneyAddr)(this, theAmount);
    }
    // int TakeDeathMoney(int theAmount) {
    // return reinterpret_cast<int (*)(Board *, int)>(Board_TakeDeathMoneyAddr)(this, theAmount);
    // }
    void RemoveAllMowers() {
        reinterpret_cast<void (*)(Board *)>(Board_RemoveAllMowersAddr)(this);
    }
    void ResetLawnMowers() {
        reinterpret_cast<void (*)(Board *)>(Board_ResetLawnMowersAddr)(this);
    }
    ZombieID ZombieGetID(Zombie *theZombie) {
        return reinterpret_cast<ZombieID (*)(Board *, Zombie *)>(Board_ZombieGetIDAddr)(this, theZombie);
    }
    void SetDanceMode(bool theEnableDance) {
        reinterpret_cast<void (*)(Board *, bool)>(Board_SetDanceModeAddr)(this, theEnableDance);
    }
    bool ChooseSeedsOnCurrentLevel() {
        return reinterpret_cast<bool (*)(Board *)>(Board_ChooseSeedsOnCurrentLevelAddr)(this);
    }
    bool RowCanHaveZombies(int theRow) {
        return reinterpret_cast<bool (*)(Board *, int)>(Board_RowCanHaveZombiesAddr)(this, theRow);
    }
    void UpdateCoverLayer() {
        reinterpret_cast<void (*)(Board *)>(Board_UpdateCoverLayerAddr)(this);
    }

    Board(LawnApp *theApp);
    void InitLevel();
    void SetGrids();
    void StartLevel();
    void Update();
    void RemovedFromManager(Sexy::WidgetManager *theManager);
    void UpdateButtons();
    int GetNumSeedsInBank(bool thePlayerIndex);
    void RemoveParticleByType(ParticleEffect theEffectType);
    void FadeOutLevel();
    Plant *AddPlant(int theGridX, int theGridY, SeedType theSeedType, SeedType theImitaterType, int thePlayerIndex, bool theIsDoEffect);
    void AddSunMoney(int theAmount, int thePlayerIndex);
    void AddDeathMoney(int theAmount);
    PlantingReason CanPlantAt(int theGridX, int theGridY, SeedType theSeedType);
    bool PlantingRequirementsMet(SeedType theSeedType);
    Plant *GetFlowerPotAt(int theGridX, int theGridY);
    Plant *GetPumpkinAt(int theGridX, int theGridY);
    void ZombiesWon(Zombie *theZombie);
    void UpdateSunSpawning();
    void UpdateZombieSpawning();
    void PickBackground();
    void DrawCoverLayer(Sexy::Graphics *g, int theRow);
    void UpdateGame();
    void UpdateGameObjects();
    bool IsFlagWave(int theWaveNumber);
    void SpawnZombieWave();
    void DrawProgressMeter(Sexy::Graphics *g, int theX, int theY);
    int GetNumWavesPerFlag();
    bool IsLevelDataLoaded();
    bool NeedSaveGame();
    void UpdateFwoosh();
    void UpdateFog();
    void DrawFog(Sexy::Graphics *g);
    void UpdateIce();
    void DrawBackdrop(Sexy::Graphics *g);
    bool RowCanHaveZombieType(int theRow, ZombieType theZombieType);
    void DrawDebugText(Sexy::Graphics *g);
    void DrawDebugObjectRects(Sexy::Graphics *g);
    void DrawFadeOut(Sexy::Graphics *g);
    int GetCurrentPlantCost(SeedType theSeedType, SeedType theImitaterType);
    void Pause(bool thePause);
    void AddSecondPlayer(int a2);
    bool IsLastStandFinalStage();
    bool MouseHitTest(int x, int y, HitResult *theHitResult, bool thePlayerIndex);
    void DrawShovel(Sexy::Graphics *g);
    bool StageIsNight();
    bool StageHasPool();
    bool StageHasRoof();
    bool StageHas6Rows();
    bool ZombieIsAddInRow(ZombieType theZombieType);
    Zombie *AddZombieInRow(ZombieType theZombieType, int theRow, int theFromWave, bool theIsRustle);
    Zombie *AddZombie(ZombieType theZombieType, int theFromWave, bool theIsRustle);
    void DoPlantingEffects(int theGridX, int theGridY, Plant *thePlant);
    void InitLawnMowers();
    void PickZombieWaves();
    void DrawUITop(Sexy::Graphics *g);
    // void GetShovelButtonRect(Rect *rect);
    void UpdateLevelEndSequence();
    void UpdateGridItems();
    void ShakeBoard(int theShakeAmountX, int theShakeAmountY);
    void DrawZenButtons(Sexy::Graphics *g);
    void DrawGameObjects(Sexy::Graphics *g);
    void SpeedUpUpdate();
    void DrawShovelButton(Sexy::Graphics *g, LawnApp *theApp);
    void ShovelDown();
    int PixelToGridXKeepOnBoard(int theX, int theY);
    int PixelToGridYKeepOnBoard(int theX, int theY);
    int GridToPixelX(int theGridX, int theGridY);
    int GridToPixelY(int theGridX, int theGridY);
    static int MakeRenderOrder(RenderLayer theRenderLayer, int theRow, int theLayerOffset);
    int GetLiveGargantuarCount();
    int GetLiveZombiesCount();
    Zombie *GetLiveZombieByType(ZombieType theZombieType);
    void FixReanimErrorAfterLoad();
    void DoPlantingAchievementCheck(SeedType theSeedType);
    bool GrantAchievement(AchievementId theAchievementId, bool theIsShow);
    int CountPlantByType(SeedType theSeedType);
    void ParseFormationSegment(char *theSegment);
    void LoadFormation(char *theFormation);
    bool ZenGardenItemNumIsZero(CursorType theCursorType);
    int GetSeedBankExtraWidth();
    Sexy::Rect GetShovelButtonRect();
    bool PlantUsesAcceleratedPricing(SeedType theSeedType);
    bool IsPlantInCursor();
    void RemoveAllPlants();
    void RemoveAllZombies();
    bool IsValidCobCannonSpotHelper(int theGridX, int theGridY);
    bool IsPoolSquare(int theGridX, int theGridY);
    void PutZombieInWave(ZombieType theZombieType, int theWaveNumber, ZombiePicker *theZombiePicker);
    int TotalZombiesHealthInWave(int theWaveIndex);
    void KillAllZombiesInRadius(int theRow, int theX, int theY, int theRadius, int theRowRange, bool theBurn, int theDamageRangeFlags);
    void KillAllPlantsInRadius(int theX, int theY, int theRadius);
    void RemoveCutsceneZombies();
    int CountZombiesOnScreen();
    float GetPosYBasedOnRow(float thePosX, int theRow);
    Zombie *GetBossZombie();
    GamepadControls *GetGamepadControlsByPlayerIndex(int thePlayerIndex);
    void DrawHammerButton(Sexy::Graphics *g, LawnApp *theApp);
    void DrawButterButton(Sexy::Graphics *g, LawnApp *theApp);
    void Draw(Sexy::Graphics *g);
    int GetSeedPacketPositionX(int thePacketIndex, int theSeedBankIndex, bool thePlayerIndex);

    void MouseMove(int x, int y);
    void MouseDown(int x, int y, int theClickCount);
    void MouseDownSecond(int x, int y, int theClickCount);
    void MouseUp(int x, int y, int theClickCount);
    void MouseUpSecond(int x, int y, int theClickCount);
    void MouseDrag(int x, int y);
    void MouseDragSecond(int x, int y);
    void ButtonDepress(int theId);
    void KeyDown(Sexy::KeyCode theKey);
    Coin *AddCoin(int theX, int theY, CoinType theCoinType, CoinMotion theCoinMotion);
    bool TakeDeathMoney(int theAmount);
    GridItem *AddAGraveStone(int gridX, int gridY);
    bool TakeSunMoney(int theAmount, int thePlayer);

protected:
    friend void InitHookFunction();

    void _constructor(LawnApp *theApp);
    void HandleTcpClientMessage(void *buf, ssize_t bufSize);
    void HandleTcpServerMessage(void *buf, ssize_t bufSize);
    void __MouseDown(int x, int y, int theClickCount);
    void __MouseDrag(int x, int y);
    void __MouseUp(int x, int y, int theClickCount);
    void PauseFromSecondPlayer(bool thePause);

    size_t getClientEventSize(EventType type);
    void processClientEvent(void *buf, ssize_t bufSize);
    size_t getServerEventSize(EventType type);
    void processServerEvent(void *buf, ssize_t bufSize);
};

int GetRectOverlap(const Sexy::Rect &rect1, const Sexy::Rect &rect2);
bool GetCircleRectOverlap(int theCircleX, int theCircleY, int theRadius, const Sexy::Rect &theRect);
/***************************************************************************************************************/
inline int theBuildLadderX;
inline int theBuildPlantX;
inline int BuildZombieX;
inline int theBuildLadderY;
inline int theBuildPlantY;
inline int BuildZombieY;
inline bool ladderBuild;
inline bool graveBuild;
inline bool plantBuild;
inline bool zombieBuild;
inline SeedType theBuildPlantType = SeedType::SEED_NONE;
inline ZombieType theBuildZombieType = ZombieType::ZOMBIE_INVALID;
inline bool isImitaterPlant;
inline int theBuildZombieCount = 1;
inline int theZombieBuildRow;
inline int theZombieBuildi;
inline bool uniformBuildZombie;
inline int theUniformZombieBuildRow;
inline bool passNowLevel;
inline std::string customFormation;
inline int formationId = -1;
inline bool ClearAllPlant;
inline bool clearAllZombies;
inline bool hypnoAllZombies;
inline bool freezeAllZombies;
inline bool clearAllGraves;
inline bool clearAllMowers;
inline bool recoverAllMowers;
inline bool startAllMowers;
inline bool banMower;
inline bool layChoseFormation;
inline bool layPastedFormation;
inline bool noFog;
inline bool checkZombiesAllowed[ZombieType::NUM_ZOMBIE_TYPES]; // 僵尸选中情况
inline int choiceSpawnMode;                                    // 刷怪模式
inline bool buttonSetSpawn;                                    // 设置出怪
inline int targetSeedBank = 1;
inline int choiceSeedPacketIndex;
inline SeedType choiceSeedType = SeedType::SEED_NONE;
inline bool isImitaterSeed;
inline bool setSeedPacket;
inline Sexy::Rect mTouchVSShovelRect = {0, 85, 56, 56};
inline bool hideCoverLayer;
inline bool infiniteSun; // 无限阳光
inline bool drawDebugText;
inline bool drawDebugRects;
inline bool FreePlantAt;
inline bool ZombieCanNotWon;
inline bool PumpkinWithLadder; // AddPlant
inline bool endlessLastStand;

// isLongPress的数值为：首次按下为0，后续按下为1
// playerIndex为0或者1，代表玩家1或者2
// buttonCode和GameButton通用
// 上 0
// 下 1
// 左 2
// 右 3


inline void (*old_FixBoardAfterLoad)(Board *board);

inline int (*old_LawnSaveGame)(Board *a1, int *a2);

inline int (*old_LawnLoadGame)(Board *a1, int *a2);

inline void (*old_Board_UpdateGame)(Board *board);

inline void (*old_Board_UpdateGameObjects)(Board *board);

inline void (*old_Board_DrawDebugText)(Board *board, Sexy::Graphics *graphics);

inline void (*old_Board_DrawDebugObjectRects)(Board *board, Sexy::Graphics *graphics);

inline int (*old_Board_GetCurrentPlantCost)(Board *board, SeedType a2, SeedType a3);

inline PlantingReason (*old_Board_CanPlantAt)(Board *board, int theGridX, int theGridY, SeedType seedType);

inline bool (*old_Board_PlantingRequirementsMet)(Board *board, SeedType theSeedType);

inline void (*old_BoardZombiesWon)(Board *board, Zombie *zombie);

inline Plant *(*old_Board_AddPlant)(Board *board, int x, int y, SeedType seedType, SeedType theImitaterType, int playerIndex, bool doPlantEffect);

inline void (*old_Board_KeyDown)(Board *board, Sexy::KeyCode theKey);

inline void (*old_Board_UpdateSunSpawning)(Board *board);

inline void (*old_Board_UpdateZombieSpawning)(Board *board);

inline void (*old_Board_UpdateIce)(Board *board);

inline void (*old_Board_PickBackground)(Board *board);

inline bool (*old_Board_StageHasPool)(Board *board);

inline void (*old_Board_UpdateFwoosh)(Board *board);

inline void (*old_Board_DrawFog)(Board *board, Sexy::Graphics *g);

inline Zombie *(*old_Board_AddZombieInRow)(Board *board, ZombieType theZombieType, int theRow, int theFromWave, bool playAnim);

inline void (*old_Board_Update)(Board *board);

inline bool (*old_Board_IsFlagWave)(Board *board, int currentWave);

inline void (*old_Board_SpawnZombieWave)(Board *board);

inline void (*old_Board_DrawProgressMeter)(Board *board, Sexy::Graphics *graphics, int a3, int a4);

inline bool (*old_Board_IsLevelDataLoaded)(Board *board);

inline bool (*old_Board_NeedSaveGame)(Board *board);

inline void (*old_Board_DrawBackdrop)(Board *board, Sexy::Graphics *graphics);

inline void (*old_Board_Pause)(Board *board, bool a2);

inline void (*old_Board_InitLawnMowers)(Board *board);

inline void (*old_Board_PickZombieWaves)(Board *board);

inline void (*old_Board_RemovedFromManager)(Board *board, Sexy::WidgetManager *manager);

inline void (*old_Board_InitLevel)(Board *board);

inline void (*old_Board_ButtonDepress)(Board *board, int id);

inline void (*old_Board_Board)(Board *board, LawnApp *mApp);

inline void (*old_Board_MouseUp)(Board *board, int a2, int a3, int a4);

inline void (*old_Board_MouseDrag)(Board *board, int x, int y);

inline void (*old_Board_MouseDown)(Board *board, int x, int y, int theClickCount);

inline void (*old_Board_MouseMove)(Board *board, int x, int y);

inline bool (*old_Board_MouseHitTest)(Board *board, int x, int y, HitResult *hitResult, bool posScaled);

inline void (*old_Board_FadeOutLevel)(Board *board);

inline void (*old_Board_AddSunMoney)(Board *board, int theAmount, int playerIndex);

inline void (*old_Board_AddDeathMoney)(Board *board, int theAmount);

inline void (*old_Board_UpdateLevelEndSequence)(Board *board);

inline void (*old_Board_UpdateGridItems)(Board *board);

inline void (*old_Board_StartLevel)(Board *board);

inline void (*old_Board_DrawUITop)(Board *board, Sexy::Graphics *graphics);

inline bool (*old_Board_RowCanHaveZombieType)(Board *board, int theRow, ZombieType theZombieType);

inline void (*old_Board_ShakeBoard)(Board *board, int theShakeAmountX, int theShakeAmountY);

inline void (*old_Board_UpdateFog)(Board *board);

inline Sexy::Rect (*old_Board_GetShovelButtonRect)(Board *board);

inline void (*old_Board_DrawZenButtons)(Board *board, Sexy::Graphics *a2);

inline void (*old_Board_DrawGameObjects)(Board *, Sexy::Graphics *);

inline int (*old_Board_GetNumSeedsInBank)(Board *, bool);

inline void (*old_Board_Draw)(Board *, Sexy::Graphics *g);

inline Coin *(*old_Board_AddCoin)(Board *board, int theX, int theY, CoinType theCoinType, CoinMotion theCoinMotion);

void FixBoardAfterLoad(Board *board);

void SyncReanimation(Board *a1, Reanimation *a2, int *theSaveGameContext);

int LawnSaveGame(Board *board, int *a2);

int LawnLoadGame(Board *board, int *a2);

const char *GetNameByAchievementId(AchievementId theAchievementId);

Sexy::Image *GetIconByAchievementId(AchievementId theAchievementId);

bool TRect_Contains(Sexy::Rect *rect, int x, int y);

bool Board_KeyUp(Board *board, int keyCode);

void Board_DrawStartButton(Board *board, Sexy::Graphics *graphics, LawnApp *lawnApp);

inline GridItem *(*old_Board_AddAGraveStone)(Board *board, int gridX, int gridY);

inline bool (*old_Board_TakeSunMoney)(Board *board, int amount, int player);

inline bool (*old_Board_TakeDeathMoney)(Board *board, int amount);

#endif // PVZ_LAWN_BOARD_BOARD_H
