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

#ifndef PVZ_LAWN_BOARD_CHALLENGE_H
#define PVZ_LAWN_BOARD_CHALLENGE_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Effect/FilterEffect.h"

constexpr int BEGHOULED_MAX_GRIDSIZEX = 8;
constexpr int BEGHOULED_MAX_GRIDSIZEY = 5;
constexpr int ART_CHALLEGE_SIZE_X = 8;
constexpr int MAX_PICK_GRID_SIZE = 50;

constexpr int BEGHOULED_WINNING_SCORE = 75;
constexpr int SLOT_MACHINE_WINNING_SCORE = 2000;
constexpr int ZOMBIQUARIUM_WINNING_SCORE = 1000;
constexpr int I_ZOMBIE_WINNING_SCORE = 5;
constexpr int MAX_PORTALS = 4;
constexpr int MAX_SQUIRRELS = 7;
constexpr int MAX_SCARY_POTS = 54;
constexpr int STORM_FLASH_TIME = 150;

class LawnApp;
class Board;
class Plant;
class Zombie;
class GridItem;
class SeedPacket;
class HitResult;

class Challenge {
public:
    int *vTable;                          // 0
    int unk1[3];                          // 1 ~ 3
    LawnApp *mApp;                        // 4
    Board *mBoard;                        // 5
    int *mHelpBarWidget;                  // 6
    bool mBeghouledMouseCapture;          // 28
    int mBeghouledMouseDownX;             // 8
    int mBeghouledMouseDownY;             // 9
    bool mBeghouledEated[9][6];           // 40 ~  93
    bool mBeghouledPurcasedUpgrade[3];    // 94 ~ 96
    int mBeghouledMatchesThisMove;        // 25
    ChallengeState mChallengeState;       // 26
    int mChallengeStateCounter;           // 27
    int mConveyorBeltCounter1;            // 28
    int mConveyorBeltCounter2;            // 29
    int mChallengeScore;                  // 30
    bool mShowBowlingLine;                // 124
    SeedType mLastConveyorSeedType;       // 32
    int mSurvivalStage;                   // 33
    int mSlotMachineRollCount;            // 34
    int mReanimChallenge;                 // 35
    int mReanimCloud[6];                  // 36 ~ 41
    int mCloudCounter[6];                 // 42 ~ 47
    int mChallengeGridX;                  // 48
    int mChallengeGridY;                  // 49
    int mScaryPotterPots;                 // 50
    int mRainCounter;                     // 51
    int mTreeOfWisdomTalkIndex;           // 52
    int unk3[14];                         // 53 ~ 66
    float mHeavyWeaponX;                  // 67
    float mHeavyWeaponY;                  // 68
    float mHeavyWeaponAngle;              // 69
    int unkHeavyWeaponWithInitValue1000;  // 70
    int mHeavyWeaponCatTailAttackCounter; // 71
    int mHeavyWeaponCatTailLevel;         // 72
    int mHeavyWeaponPeaLevel;             // 73
    int mHeavyWeaponSnowPeaCounter;       // 74
    int mHeavyWeaponTorchWoodCounter;     // 75
    int unkHeavyWeaponWithInitValue10;    // 76
    ReanimationID mReanimHeavyWeaponID1;  // 77
    ReanimationID mReanimHeavyWeaponID2;  // 78
    ReanimationID mReanimHeavyWeaponID3;  // 79
    // 大小80个整数

    bool MouseDown(int x, int y, int theClickCount, HitResult *theHitResult, int thePlayerIndex) {
        return reinterpret_cast<bool (*)(Challenge *, int, int, int, HitResult *, int)>(Challenge_MouseDownAddr)(this, x, y, theClickCount, theHitResult, thePlayerIndex);
    }
    void TreeOfWisdomOpenStore() {
        reinterpret_cast<void (*)(Challenge *)>(Challenge_TreeOfWisdomOpenStoreAddr)(this);
    }
    bool IsMPSuddenDeath() {
        return reinterpret_cast<bool (*)(Challenge *)>(Challenge_IsMPSuddenDeathAddr)(this);
    };
    void IZombieScoreBrain(GridItem *theBrain) {
        reinterpret_cast<void (*)(Challenge *, GridItem *)>(Challenge_IZombieScoreBrainAddr)(this, theBrain);
    }
    void GraveDangerSpawnGraveAt(int theGridX, int theGridY) {
        reinterpret_cast<void (*)(Challenge *, int, int)>(Challenge_GraveDangerSpawnGraveAtAddr)(this, theGridX, theGridY);
    }
    void PlantAdded(Plant *thePlant) {
        reinterpret_cast<void (*)(Challenge *, Plant *)>(Challenge_PlantAddedAddr)(this, thePlant);
    }
    void DrawWeather(Sexy::Graphics *g) {
        reinterpret_cast<void (*)(Challenge *, Sexy::Graphics *)>(Challenge_DrawWeatherAddr)(this, g);
    }
    void DrawRain(Sexy::Graphics *g) {
        reinterpret_cast<void (*)(Challenge *, Sexy::Graphics *)>(Challenge_DrawRainAddr)(this, g);
    }
    void DrawStormNight(Sexy::Graphics *g) {
        reinterpret_cast<void (*)(Challenge *, Sexy::Graphics *)>(Challenge_DrawStormNightAddr)(this, g);
    }
    void UpdateStormyNight() {
        reinterpret_cast<void (*)(Challenge *)>(Challenge_UpdateStormyNightAddr)(this);
    }
    int CanTargetZombieWithPortals(Plant *thePlant, Zombie *theZombie) {
        return reinterpret_cast<int (*)(Challenge *, Plant *, Zombie *)>(Challenge_CanTargetZombieWithPortalsAddr)(this, thePlant, theZombie);
    }
    void ScaryPotterJackExplode(int thePosX, int thePosY) {
        reinterpret_cast<void (*)(Challenge *, int, int)>(Challenge_ScaryPotterJackExplodeAddr)(this, thePosX, thePosY);
    }

    Challenge();
    void Update();
    void HeavyWeaponFire(float a2, float a3);
    void IZombieDrawPlant(Sexy::Graphics *g, Plant *thePlant);
    void HeavyWeaponUpdate();
    bool IZombieEatBrain(Zombie *theZombie);
    void DrawArtChallenge(Sexy::Graphics *g);
    PlantingReason CanPlantAt(int theGridX, int theGridY, SeedType theSeedType);
    void InitLevel();
    void InitZombieWaves();
    void TreeOfWisdomFertilize();
    void LastStandUpdate();
    void DrawHeavyWeapon(Sexy::Graphics *g);
    bool UpdateZombieSpawning();
    void HeavyWeaponPacketClicked(SeedPacket *theSeedPacket);
    static int IsMPSeedType(SeedType theSeedType);
    static int IsZombieSeedType(SeedType theSeedType);
    void IZombieSetPlantFilterEffect(Plant *thePlant, FilterEffect theFilterEffect);
    static ZombieType IZombieSeedTypeToZombieType(SeedType theSeedType);
    void IZombiePlaceZombie(ZombieType theZombieType, int theGridX, int theGridY);
    void StartLevel();
    void ScaryPotterOpenPot(GridItem *theScaryPot);
    GridItem *IZombieGetBrainTarget(Zombie *theZombie);
    void IZombieSquishBrain(GridItem *theBrain);
    int ScaryPotterCountSunInPot(GridItem *theScaryPot);
    SeedType GetArtChallengeSeed(int theGridX, int theGridY);
    void InitZombieWavesFromList(ZombieType *theZombieList, int theListLength);
    void IZombieSetupPlant(Plant *thePlant);
    void HeavyWeaponReanimUpdate();
    void MouseDownWhackAZombie(int theX, int theY, int thePlayerIndex);

protected:
    friend void InitHookFunction();

    void _constructor();
    void _destructor();
    void UpdateConveyorBelt(int playerIndex);
};

/***************************************************************************************************************/
inline int targetWavesToJump = 1;
inline bool requestJumpSurvivalStage;
inline bool stopSpawning; // 暂停刷怪


inline void (*old_Challenge_Update)(Challenge *a);

inline void (*old_Challenge_Challenge)(Challenge *a);

inline void (*old_Challenge_HeavyWeaponFire)(Challenge *challenge, float a2, float a3);

inline void (*old_Challenge_HeavyWeaponUpdate)(Challenge *challenge);

inline void (*old_Challenge_InitLevel)(Challenge *challenge);

inline void (*old_Challenge_InitZombieWaves)(Challenge *a);

inline void (*old_Challenge_TreeOfWisdomFertilize)(Challenge *);

inline ZombieType (*old_Challenge_IZombieSeedTypeToZombieType)(SeedType type);

inline bool (*old_Challenge_UpdateZombieSpawning)(Challenge *);

inline void (*old_Challenge_HeavyWeaponPacketClicked)(Challenge *challenge, SeedPacket *seedPacket);

inline void (*old_Challenge_StartLevel)(Challenge *challenge);

inline void (*old_Challenge_Delete)(Challenge *challenge);

inline void (*old_Challenge_ScaryPotterOpenPot)(Challenge *, GridItem *theScaryPot);

inline GridItem *(*old_Challenge_IZombieGetBrainTarget)(Challenge *, Zombie *theZombie);

inline void (*old_Challenge_IZombieSquishBrain)(Challenge *, GridItem *theBrain);

inline void (*old_Challenge_UpdateConveyorBelt)(Challenge *, int);


#endif // PVZ_LAWN_BOARD_CHALLENGE_H
