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

#ifndef PVZ_LAWN_BOARD_ZOMBIE_H
#define PVZ_LAWN_BOARD_ZOMBIE_H

#include "GameObject.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Lawn/Common/GameConstants.h"
#include "PvZ/Symbols.h"

constexpr int MAX_ZOMBIE_FOLLOWERS = 4;
constexpr int NUM_BOBSLED_FOLLOWERS = 3;
constexpr int NUM_BACKUP_DANCERS = 4;
constexpr int NUM_BOSS_BUNGEES = 3;

constexpr int MAX_DEAD_FOLLOWERS = 15;
constexpr int ProductorZombieLaunchRate = 2500;

constexpr int ZOMBIE_START_RANDOM_OFFSET = 40;
constexpr int BUNGEE_ZOMBIE_HEIGHT = 3000;
constexpr int RENDER_GROUP_SHIELD = 1;
constexpr int RENDER_GROUP_ARMS = 2;
constexpr int RENDER_GROUP_OVER_SHIELD = 3;
constexpr int RENDER_GROUP_BOSS_BACK_LEG = 4;
constexpr int RENDER_GROUP_BOSS_FRONT_LEG = 5;
constexpr int RENDER_GROUP_BOSS_BACK_ARM = 6;
constexpr int RENDER_GROUP_BOSS_FIREBALL_ADDITIVE = 7;
constexpr int RENDER_GROUP_BOSS_FIREBALL_TOP = 8;
constexpr int ZOMBIE_LIMP_SPEED_FACTOR = 2;
constexpr int POGO_BOUNCE_TIME = 80;
constexpr int DOLPHIN_JUMP_TIME = 120;
constexpr int JackInTheBoxZombieRadius = 115;
constexpr int JackInTheBoxPlantRadius = 90;
constexpr int BOBSLED_CRASH_TIME = 150;
constexpr int ZOMBIE_BACKUP_DANCER_RISE_HEIGHT = -200;
constexpr int BOSS_FLASH_HEALTH_FRACTION = 10;
constexpr int TICKS_BETWEEN_EATS = 8;
constexpr int DAMAGE_PER_EAT = TICKS_BETWEEN_EATS;
constexpr float THOWN_ZOMBIE_GRAVITY = 0.05f;
constexpr float CHILLED_SPEED_FACTOR = 0.4f;
constexpr float CLIP_HEIGHT_LIMIT = -100.0f;
constexpr float CLIP_HEIGHT_OFF = -200.0f;
inline const Sexy::Color ZOMBIE_MINDCONTROLLED_COLOR = Sexy::Color(128, 0, 192, 255);

enum ZombieAttackType {
    ATTACKTYPE_CHEW,
    ATTACKTYPE_DRIVE_OVER,
    ATTACKTYPE_VAULT,
    ATTACKTYPE_LADDER,
};

enum ZombieParts {
    PART_BODY,
    PART_HEAD,
    PART_HEAD_EATING,
    PART_TONGUE,
    PART_ARM,
    PART_HAIR,
    PART_HEAD_YUCKY,
    PART_ARM_PICKAXE,
    PART_ARM_POLEVAULT,
    PART_ARM_LEASH,
    PART_ARM_FLAG,
    PART_POGO,
    PART_DIGGER
};

class ZombieDrawPosition {
public:
    int mHeadX;
    int mHeadY;
    int mArmY;
    float mBodyY;
    float mImageOffsetX;
    float mImageOffsetY;
    float mClipHeight;
};

class Coin;
class Plant;
class Reanimation;
class TodParticleSystem;
class Zombie : public __GameObject {
public:
    enum {
        ZOMBIE_WAVE_DEBUG = -1,
        ZOMBIE_WAVE_CUTSCENE = -2,
        ZOMBIE_WAVE_UI = -3,
        ZOMBIE_WAVE_WINNER = -4,
    };

    ZombieType mZombieType;                           // 13
    ZombiePhase mZombiePhase;                         // 14
    float mPosX;                                      // 15
    float mPosY;                                      // 16
    float mVelX;                                      // 17
    int mAnimCounter;                                 // 18
    int mGroanCounter;                                // 19
    int mAnimTicksPerFrame;                           // 20
    int mAnimFrames;                                  // 21
    int mFrame;                                       // 22
    int mPrevFrame;                                   // 23
    bool mVariant;                                    // 96
    bool mIsEating;                                   // 97
    short mSquashHeadCol;                             // 新增成员，用于窝瓜僵尸修复
    int mJustGotShotCounter;                          // 25
    int mShieldJustGotShotCounter;                    // 26
    int mShieldRecoilCounter;                         // 27
    int mZombieAge;                                   // 28
    ZombieHeight mZombieHeight;                       // 29
    int mPhaseCounter;                                // 30
    int mFromWave;                                    // 31
    bool mDroppedLoot;                                // 128
    int mZombieFade;                                  // 33
    bool mFlatTires;                                  // 136
    int mUseLadderCol;                                // 35
    int mTargetCol;                                   // 36
    float mAltitude;                                  // 37
    bool mHitUmbrella;                                // 152
    Sexy::Rect mZombieRect;                           // 39 ~ 42
    Sexy::Rect mZombieAttackRect;                     // 43 ~ 46
    int mChilledCounter;                              // 47
    int mButteredCounter;                             // 48
    int mIceTrapCounter;                              // 49
    bool mMindControlled;                             // 200
    bool mBlowingAway;                                // 201
    bool mHasHead;                                    // 202
    bool mHasArm;                                     // 203
    bool mHasObject;                                  // 204
    bool mInPool;                                     // 205
    bool mOnHighGround;                               // 206
    bool mYuckyFace;                                  // 207
    int mYuckyFaceCounter;                            // 52
    HelmType mHelmType;                               // 53
    int mBodyHealth;                                  // 54
    int mBodyMaxHealth;                               // 55
    int mHelmHealth;                                  // 56
    int mHelmMaxHealth;                               // 57
    ShieldType mShieldType;                           // 58
    int mShieldHealth;                                // 59
    int mShieldMaxHealth;                             // 60
    int mFlyingHealth;                                // 61
    int mFlyingMaxHealth;                             // 62
    bool mDead;                                       // 252
    ZombieID mRelatedZombieID;                        // 64
    ZombieID mFollowerZombieID[MAX_ZOMBIE_FOLLOWERS]; // 65 ~ 68
    bool mPlayingSong;                                // 276
    int mParticleOffsetX;                             // 70
    int mParticleOffsetY;                             // 71
    AttachmentID mAttachmentID;                       // 72
    int mSummonCounter;                               // 73
    ReanimationID mBodyReanimID;                      // 74
    float mScaleZombie;                               // 75
    float mVelZ;                                      // 76
    float mOrginalAnimRate;                           // 77
    Plant *mTargetPlantID;                            // 78
    int mBossMode;                                    // 79
    int mTargetRow;                                   // 80
    int mBossBungeeCounter;                           // 81
    int mBossStompCounter;                            // 82
    int mBossHeadCounter;                             // 83
    ReanimationID mBossFireBallReanimID;              // 84
    ReanimationID mSpecialHeadReanimID;               // 85
    int mFireballRow;                                 // 86
    bool mIsFireBall;                                 // 348
    ReanimationID mMoweredReanimID;                   // 88
    int mLastPortalX;                                 // 89
    bool mBloated;                                    // 360
    int unkMems[6];                                   // 91 ~ 96
    // 大小96个整数

    void RemoveColdEffects() {
        reinterpret_cast<void (*)(Zombie *)>(Zombie_RemoveColdEffectsAddr)(this);
    }
    void ApplyButter() {
        reinterpret_cast<void (*)(Zombie *)>(Zombie_ApplyButterAddr)(this);
    }
    TodParticleSystem *AddAttachedParticle(int thePosX, int thePosY, ParticleEffect theEffect) {
        return reinterpret_cast<TodParticleSystem *(*)(Zombie *, int, int, ParticleEffect)>(Zombie_AddAttachedParticleAddr)(this, thePosX, thePosY, theEffect);
    }
    void StartEating() {
        reinterpret_cast<void (*)(Zombie *)>(Zombie_StartEatingAddr)(this);
    }
    static void SetupReanimLayers(Reanimation *aReanim, ZombieType theZombieType) {
        reinterpret_cast<void (*)(Reanimation *, ZombieType)>(Zombie_SetupReanimLayersAddr)(aReanim, theZombieType);
    }
    static void SetupShieldReanims(ZombieType theZombieType, Reanimation *aReanim) {
        reinterpret_cast<void (*)(ZombieType, Reanimation *)>(Zombie_SetupShieldReanimsAddr)(theZombieType, aReanim);
    }
    void UpdatePlaying() {
        reinterpret_cast<void (*)(Zombie *)>(Zombie_UpdatePlayingAddr)(this);
    }
    int TakeFlyingDamage(int theDamage, unsigned int theDamageFlags) {
        return reinterpret_cast<int (*)(Zombie *, int, unsigned int)>(Zombie_TakeFlyingDamageAddr)(this, theDamage, theDamageFlags);
    }
    int TakeShieldDamage(int theDamage, unsigned int theDamageFlags) {
        return reinterpret_cast<int (*)(Zombie *, int, unsigned int)>(Zombie_TakeShieldDamageAddr)(this, theDamage, theDamageFlags);
    }
    void TakeBodyDamage(int theDamage, unsigned int theDamageFlags) {
        reinterpret_cast<void (*)(Zombie *, int, unsigned int)>(Zombie_TakeBodyDamageAddr)(this, theDamage, theDamageFlags);
    }
    void GetTrackPosition(const char *theTrackName, float &thePosX, float &thePosY) {
        reinterpret_cast<void (*)(Zombie *, const char *, float &, float &)>(Zombie_GetTrackPositionAddr)(this, theTrackName, thePosX, thePosY);
    }
    void UpdateAnimSpeed() {
        reinterpret_cast<void (*)(Zombie *)>(Zombie_UpdateAnimSpeedAddr)(this);
    }
    void UpdateZombiePosition() {
        reinterpret_cast<void (*)(Zombie *)>(Zombie_UpdateZombiePositionAddr)(this);
    };
    void LoadReanim(ReanimationType theReanimationType) {
        reinterpret_cast<void (*)(Zombie *, ReanimationType)>(Zombie_LoadReanimAddr)(this, theReanimationType);
    }
    void LoadPlainZombieReanim() {
        reinterpret_cast<void (*)(Zombie *)>(Zombie_LoadPlainZombieReanimAddr)(this);
    }
    void AttachShield() {
        reinterpret_cast<void (*)(Zombie *)>(Zombie_AttachShieldAddr)(this);
    }
    void ReanimIgnoreClipRect(const char *theTrackName, bool theIgnoreClipRect) {
        reinterpret_cast<void (*)(Zombie *, const char *, bool)>(Zombie_ReanimIgnoreClipRectAddr)(this, theTrackName, theIgnoreClipRect);
    }
    void SetAnimRate(float theAnimRate) {
        reinterpret_cast<void (*)(Zombie *, float)>(Zombie_SetAnimRateAddr)(this, theAnimRate);
    }
    void DrawDancerReanim(Sexy::Graphics *g, ZombieDrawPosition &theDrawPos) {
        reinterpret_cast<void (*)(Zombie *, Sexy::Graphics *, ZombieDrawPosition &)>(Zombie_DrawDancerReanimAddr)(this, g, theDrawPos);
    }
    void PlayDeathAnim(unsigned int theDamageFlags) {
        reinterpret_cast<void (*)(Zombie *, unsigned int)>(Zombie_PlayDeathAnimAddr)(this, theDamageFlags);
    }
    void DrawBobsledReanim(Sexy::Graphics *g, ZombieDrawPosition &theDrawPos, bool theBeforeZombie) {
        reinterpret_cast<void (*)(Zombie *, Sexy::Graphics *, ZombieDrawPosition &, bool)>(Zombie_DrawBobsledReanimAddr)(this, g, theDrawPos, theBeforeZombie);
    }
    void DrawBungeeReanim(Sexy::Graphics *g, ZombieDrawPosition &theDrawPos) {
        reinterpret_cast<void (*)(Zombie *, Sexy::Graphics *, ZombieDrawPosition &)>(Zombie_DrawBungeeReanimAddr)(this, g, theDrawPos);
    }
    void EatZombie(Zombie *theZombie) {
        reinterpret_cast<void (*)(Zombie *, Zombie *)>(Zombie_EatZombieAddr)(this, theZombie);
    }
    void RemoveIceTrap() {
        reinterpret_cast<void (*)(Zombie *)>(Zombie_RemoveIceTrapAddr)(this);
    }
    void BungeeDropPlant() {
        reinterpret_cast<void (*)(Zombie *)>(Zombie_BungeeDropPlantAddr)(this);
    }
    void BalloonPropellerHatSpin(bool theSpinning) {
        reinterpret_cast<void (*)(Zombie *, bool)>(Zombie_BalloonPropellerHatSpinAddr)(this, theSpinning);
    }
    void BobsledBurn() {
        reinterpret_cast<void (*)(Zombie *)>(Zombie_BobsledBurnAddr)(this);
    }
    bool HasYuckyFaceImage() {
        return reinterpret_cast<bool (*)(Zombie *)>(Zombie_HasYuckyFaceImageAddr)(this);
    }
    void ShowYuckyFace(bool show) {
        reinterpret_cast<void (*)(Zombie *, bool)>(Zombie_ShowYuckyFaceAddr)(this, show);
    }

    Zombie() {
        _constructor();
    }
    void ZombieInitialize(int theRow, ZombieType theType, bool theVariant, Zombie *theParentZombie, int theFromWave, bool theIsVisible);
    void CheckIfPreyCaught();
    void Draw(Sexy::Graphics *g);
    void DieWithLoot();
    void DieNoLoot();
    void StopZombieSound();
    void Update();
    void UpdateActions();
    void LandFlyer(unsigned int theDamageFlags);
    void UpdateZombieFlyer();
    void UpdateYeti();
    void UpdateZombieImp();
    void UpdateZombieJackInTheBox();
    void UpdateZombieGargantuar();
    Zombie *ThrowAZombieImp(float theOffsetDistance);
    void UpdateZombiePeaHead();
    void UpdateZombieGatlingHead();
    void BurnRow(int theRow);
    void UpdateZombieJalapenoHead();
    void UpdateZombieSquashHead();
    void UpdateZombieDancer();
    void UpdateZombieRiseFromGrave();
    void UpdateDamageStates(unsigned int theDamageFlags);
    void BossDestroyIceballInRow(int theRow);
    void BossDestroyFireball();
    int GetDancerFrame();
    void RiseFromGrave(int theGridX, int theGridY);
    void EatPlant(Plant *thePlant);
    void DetachShield();
    void CheckForBoardEdge();
    void DrawBossPart(Sexy::Graphics *g, int theBossPart);
    static bool IsZombotany(ZombieType theZombieType);
    static bool ZombieTypeCanGoInPool(ZombieType theZombieType);
    void BossSpawnAttack();
    void DrawBungeeCord(Sexy::Graphics *graphics, int theOffsetX, int theOffsetY);
    bool IsTangleKelpTarget();
    void DrawReanim(Sexy::Graphics *g, ZombieDrawPosition &theDrawPos, int theBaseRenderGroup);
    bool CanLoseBodyParts();
    void SetupReanimForLostHead();
    void DropHead(unsigned int theDamageFlags);
    void SetupReanimForLostArm(unsigned int theDamageFlags);
    void DropArm(unsigned int theDamageFlags);
    Sexy::Rect GetZombieAttackRect();
    Plant *FindPlantTarget(ZombieAttackType theAttackType);
    bool CanTargetPlant(Plant *thePlant, ZombieAttackType theAttackType);
    Zombie *FindZombieTarget();
    void TakeDamage(int theDamage, unsigned int theDamageFlags);
    int TakeHelmDamage(int theDamage, unsigned int theDamageFlags);
    void PlayZombieReanim(const char *theTrackName, ReanimLoopType theLoopType, int theBlendTime, float theAnimRate);
    void StartWalkAnim(int theBlendTime);
    void ReanimShowPrefix(const char *theTrackPrefix, int theRenderGroup);
    void ReanimShowTrack(const char *theTrackName, int theRenderGroup);
    float GetPosYBasedOnRow(int theRow);
    void SetRow(int theRow);
    void StartMindControlled();
    void UpdateReanim();
    void UpdateReanimColor();
    void SetupLostArmReanim();
    void SetZombatarReanim();
    bool IsImmobilizied();
    bool IsBouncingPogo();
    bool IsFlying();
    int GetBobsledPosition();
    bool IsBobsledTeamWithSled();
    bool IsDeadOrDying();
    bool CanBeChilled();
    bool CanBeFrozen();
    void AddButter();
    static bool IsZombatarZombie(ZombieType theType);
    void SquishAllInSquare(int theX, int theY, ZombieAttackType theAttackType);
    bool IsWalkingBackwards();
    void SetupDoorArms(Reanimation *aReanim, bool theShow);
    void ShowDoorArms(bool theShow);
    void StopEating();
    Sexy::Rect GetZombieRect();
    void GetDrawPos(ZombieDrawPosition &theDrawPos);
    bool IsOnHighGround();
    void DropLoot();
    bool IsOnBoard();
    bool EffectedByDamage(unsigned int theDamageRangeFlags);
    int GetHelmDamageIndex();
    int GetBodyDamageIndex();
    int GetShieldDamageIndex();
    bool IsFireResistant();
    void PickRandomSpeed();
    void ApplySyncedSpeed(float theVelX, short theAnimTicks);
    float ZombieTargetLeadX(float theTime);
    void ApplyBurn();
    void ApplyChill(bool theIsIceTrap);
    void HitIceTrap();
    bool ZombieNotWalking();
    bool IsMovingAtChilledSpeed();
    void UpdateZombieWalking();
    ZombiePhase GetDancerPhase();
    ZombieID SummonBackupDancer(int theRow, int thePosX);
    void SummonBackupDancers();
    bool NeedsMoreBackupDancers();
    void UpdateYuckyFace();
    void UpdateZombiePolevaulter();
    void DoSpecial();

protected:
    void _constructor() {
        reinterpret_cast<void (*)(Zombie *)>(Zombie_ZombieAddr)(this);
    };
};

class ZombieDefinition {
public:
    ZombieType mZombieType;
    ReanimationType mReanimationType;
    int mZombieValue;
    int mStartingLevel;
    int mFirstAllowedWave;
    int mPickWeight;
    const char *mZombieName;
};
extern ZombieDefinition gZombieDefs[NUM_ZOMBIE_TYPES];
inline ZombieDefinition gZombieTrashBinDef = {ZombieType::ZOMBIE_TRASHCAN, ReanimationType::REANIM_ZOMBIE, 1, 99, 1, 4000, "TRASHCAN_ZOMBIE"};

ZombieDefinition &GetZombieDefinition(ZombieType theZombieType);
/***************************************************************************************************************/

inline bool zombieBloated;
inline bool showZombieBodyHealth;
inline bool showGargantuarHealth;
inline bool showHelmAndShieldHealth;
inline int maidCheats; // 女仆秘籍
inline int boardEdgeAdjust;
inline int zombieSetScale;


inline void (*old_Zombie_Update)(Zombie *a1);

inline void (*old_Zombie_UpdateActions)(Zombie *);

inline void (*old_Zombie_Draw)(Zombie *zombie, Sexy::Graphics *graphics);

inline void (*old_Zombie_DrawBossPart)(Zombie *a1, Sexy::Graphics *graphics, int theBossPart);

inline bool (*old_ZombieTypeCanGoInPool)(ZombieType zombieType);

inline void (*old_Zombie_RiseFromGrave)(Zombie *zombie, int gridX, int gridY);

inline void (*old_Zombie_EatPlant)(Zombie *a, Plant *a2);

inline void (*old_Zombie_DetachShield)(Zombie *zombie);

inline void (*old_Zombie_ZombieInitialize)(Zombie *zombie, int theRow, ZombieType theType, bool theVariant, Zombie *theParentZombie, int theFromWave, bool isVisible);

inline void (*old_Zombie_DieNoLoot)(Zombie *);

inline void (*old_Zombie_DrawReanim)(Zombie *zombie, Sexy::Graphics *graphics, ZombieDrawPosition &zombieDrawPosition, int theBaseRenderGroup);

inline void (*old_Zombie_DropHead)(Zombie *zombie, unsigned int a2);

inline void (*old_Zombie_DropArm)(Zombie *, unsigned int);

inline void (*old_Zombie_TakeDamage)(Zombie *, int theDamage, unsigned int theDamageFlags);

inline int (*old_Zombie_TakeHelmDamage)(Zombie *, int theDamage, unsigned int theDamageFlags);

inline void (*old_Zombie_PlayZombieReanim)(Zombie *, const char *theTrackName, ReanimLoopType theLoopType, int theBlendTime, float theAnimRate);

inline void (*old_Zombie_StartWalkAnim)(Zombie *, int theBlendTime);

inline void (*old_Zombie_ReanimShowPrefix)(Zombie *, const char *, int);

inline void (*old_Zombie_ReanimShowTrack)(Zombie *, const char *, int);

inline float (*old_Zombie_GetPosYBasedOnRow)(Zombie *, int theRow);

inline void (*old_Zombie_SetRow)(Zombie *, int theRow);

inline void (*old_Zombie_StartMindControlled)(Zombie *);

inline void (*old_Zombie_UpdateReanim)(Zombie *);

inline int (*old_Zombie_GetBobsledPosition)(Zombie *);

inline void (*old_Zombie_SquishAllInSquare)(Zombie *, int theX, int theY, ZombieAttackType theAttackType);

inline void (*old_Zombie_UpdateZombieWalking)(Zombie *);

inline void (*old_Zombie_DropLoot)(Zombie *);

inline void (*old_Zombie_ApplyBurn)(Zombie *);

inline void (*old_Zombie_UpdateYuckyFace)(Zombie *);

#endif // PVZ_LAWN_BOARD_ZOMBIE_H
