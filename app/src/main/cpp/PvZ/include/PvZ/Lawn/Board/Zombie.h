#ifndef PVZ_TV_1_1_5_ZOMBIE_H
#define PVZ_TV_1_1_5_ZOMBIE_H

#include "GameObject.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Lawn/Common/GameConstants.h"
#include "PvZ/Symbols.h"

constexpr int MAX_ZOMBIE_FOLLOWERS = 4;
constexpr int NUM_BOBSLED_FOLLOWERS = 3;
constexpr int NUM_BACKUP_DANCERS = 4;
constexpr int NUM_BOSS_BUNGEES = 3;

constexpr const int ZOMBIE_START_RANDOM_OFFSET = 40;
constexpr const int BUNGEE_ZOMBIE_HEIGHT = 3000;
constexpr const int RENDER_GROUP_SHIELD = 1;
constexpr const int RENDER_GROUP_ARMS = 2;
constexpr const int RENDER_GROUP_OVER_SHIELD = 3;
constexpr const int RENDER_GROUP_BOSS_BACK_LEG = 4;
constexpr const int RENDER_GROUP_BOSS_FRONT_LEG = 5;
constexpr const int RENDER_GROUP_BOSS_BACK_ARM = 6;
constexpr const int RENDER_GROUP_BOSS_FIREBALL_ADDITIVE = 7;
constexpr const int RENDER_GROUP_BOSS_FIREBALL_TOP = 8;
constexpr const int ZOMBIE_LIMP_SPEED_FACTOR = 2;
constexpr const int POGO_BOUNCE_TIME = 80;
constexpr const int DOLPHIN_JUMP_TIME = 120;
constexpr const int JackInTheBoxZombieRadius = 115;
constexpr const int JackInTheBoxPlantRadius = 90;
constexpr const int BOBSLED_CRASH_TIME = 150;
constexpr const int ZOMBIE_BACKUP_DANCER_RISE_HEIGHT = -200;
constexpr const int BOSS_FLASH_HEALTH_FRACTION = 10;
constexpr const int TICKS_BETWEEN_EATS = 4;
constexpr const int DAMAGE_PER_EAT = TICKS_BETWEEN_EATS;
constexpr const float THOWN_ZOMBIE_GRAVITY = 0.05f;
constexpr const float CHILLED_SPEED_FACTOR = 0.4f;
constexpr const float CLIP_HEIGHT_LIMIT = -100.0f;
constexpr const float CLIP_HEIGHT_OFF = -200.0f;
// const Color ZOMBIE_MINDCONTROLLED_COLOR = Color(128, 0, 192, 255);

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

class Plant;
class Reanimation;
class TodParticleSystem;
class Zombie : public GameObject {
public:
    enum {
        ZOMBIE_WAVE_DEBUG = -1,
        ZOMBIE_WAVE_CUTSCENE = -2,
        ZOMBIE_WAVE_UI = -3,
        ZOMBIE_WAVE_WINNER = -4,
    };

    ZombieType mZombieType;                          // 13
    ZombiePhase mZombiePhase;                        // 14
    float mPosX;                                     // 15
    float mPosY;                                     // 16
    float mVelX;                                     // 17
    int mAnimCounter;                                // 18
    int mGroanCounter;                               // 19
    int mAnimTicksPerFrame;                          // 20
    int mAnimFrames;                                 // 21
    int mFrame;                                      // 22
    int mPrevFrame;                                  // 23
    bool mVariant;                                   // 96
    bool mIsEating;                                  // 97
    int mJustGotShotCounter;                         // 25
    int mShieldJustGotShotCounter;                   // 26
    int mShieldRecoilCounter;                        // 27
    int mZombieAge;                                  // 28
    ZombieHeight mZombieHeight;                      // 29
    int mPhaseCounter;                               // 30
    int mFromWave;                                   // 31
    bool mDroppedLoot;                               // 128
    int mZombieFade;                                 // 33
    bool mFlatTires;                                 // 136
    int mUseLadderCol;                               // 35
    int mTargetCol;                                  // 36
    float mAltitude;                                 // 37
    bool mHitUmbrella;                               // 152
    Sexy::Rect mZombieRect;                          // 39 ~ 42
    Sexy::Rect mZombieAttackRect;                    // 43 ~ 46
    int mChilledCounter;                             // 47
    int mButteredCounter;                            // 48
    int mIceTrapCounter;                             // 49
    bool mMindControlled;                            // 200
    bool mBlowingAway;                               // 201
    bool mHasHead;                                   // 202
    bool mHasArm;                                    // 203
    bool mHasObject;                                 // 204
    bool mInPool;                                    // 205
    bool mOnHighGround;                              // 206
    bool mYuckyFace;                                 // 207
    int mYuckyFaceCounter;                           // 52
    HelmType mHelmType;                              // 53
    int mBodyHealth;                                 // 54
    int mBodyMaxHealth;                              // 55
    int mHelmHealth;                                 // 56
    int mHelmMaxHealth;                              // 57
    ShieldType mShieldType;                          // 58
    int mShieldHealth;                               // 59
    int mShieldMaxHealth;                            // 60
    int mFlyingHealth;                               // 61
    int mFlyingMaxHealth;                            // 62
    bool mDead;                                      // 252
    ZombieID mRelatedZombieID;                        // 64
    ZombieID mFollowerZombieID[MAX_ZOMBIE_FOLLOWERS]; // 65 ~ 68
    bool mPlayingSong;                               // 276
    int mParticleOffsetX;                            // 70
    int mParticleOffsetY;                            // 71
    int *mAttachmentID;                              // 72
    int mSummonCounter;                              // 73
    ReanimationID mBodyReanimID;                     // 74
    float mScaleZombie;                              // 75
    float mVelZ;                                     // 76
    float mOrginalAnimRate;                          // 77
    Plant *mTargetPlantID;                           // 78
    int mBossMode;                                   // 79
    int mTargetRow;                                  // 80
    int mBossBungeeCounter;                          // 81
    int mBossStompCounter;                           // 82
    int mBossHeadCounter;                            // 83
    ReanimationID mBossFireBallReanimID;             // 84
    ReanimationID mSpecialHeadReanimID;              // 85
    int mFireballRow;                                // 86
    bool mIsFireBall;                                // 348
    int mMoweredReanimID;                            // 88
    int mLastPortalX;                                // 89
    bool mBloated;                                   // 360
    int unkMems[6];                                  // 91 ~ 96
    int mZombieID;                                   // 97
    // 大小98个整数

    void RemoveColdEffects() { reinterpret_cast<void (*)(Zombie *)>(Zombie_RemoveColdEffectsAddr)(this); }
    void ApplyButter() { reinterpret_cast<void (*)(Zombie *)>(Zombie_ApplyButterAddr)(this); }
    void ApplyBurn() { reinterpret_cast<void (*)(Zombie *)>(Zombie_ApplyBurnAddr)(this); }
    TodParticleSystem *AddAttachedParticle(int thePosX, int thePosY, ParticleEffect theEffect) {
        return reinterpret_cast<TodParticleSystem *(*)(Zombie *, int, int, ParticleEffect)>(Zombie_AddAttachedParticleAddr)(this, thePosX, thePosY, theEffect);
    }
    void StartEating() { reinterpret_cast<void (*)(Zombie *)>(Zombie_StartEatingAddr)(this); }
    void DropArm(unsigned int theDamageFlags) { reinterpret_cast<void (*)(Zombie *, unsigned int)>(Zombie_DropArmAddr)(this, theDamageFlags); }
    static void SetupReanimLayers(Reanimation *aReanim, ZombieType theZombieType) { reinterpret_cast<void (*)(Reanimation *, ZombieType)>(Zombie_SetupReanimLayersAddr)(aReanim, theZombieType); }
    static void SetupShieldReanims(ZombieType theZombieType, Reanimation *aReanim) { reinterpret_cast<void (*)(ZombieType, Reanimation *)>(Zombie_SetupShieldReanimsAddr)(theZombieType, aReanim); }
    void UpdatePlaying() { reinterpret_cast<void (*)(Zombie *)>(Zombie_UpdatePlayingAddr)(this); }
    int TakeHelmDamage(int theDamage, unsigned int theDamageFlags) { return reinterpret_cast<int (*)(Zombie *, int, unsigned int)>(Zombie_TakeHelmDamageAddr)(this, theDamage, theDamageFlags); }
    int TakeFlyingDamage(int theDamage, unsigned int theDamageFlags) { return reinterpret_cast<int (*)(Zombie *, int, unsigned int)>(Zombie_TakeFlyingDamageAddr)(this, theDamage, theDamageFlags); }
    int TakeShieldDamage(int theDamage, unsigned int theDamageFlags) { return reinterpret_cast<int (*)(Zombie *, int, unsigned int)>(Zombie_TakeShieldDamageAddr)(this, theDamage, theDamageFlags); }
    void TakeBodyDamage(int theDamage, unsigned int theDamageFlags) { reinterpret_cast<void (*)(Zombie *, int, unsigned int)>(Zombie_TakeBodyDamageAddr)(this, theDamage, theDamageFlags); }
    void UpdateDamageStates(unsigned int theDamageFlags) { reinterpret_cast<void (*)(Zombie *, unsigned int)>(Zombie_UpdateDamageStatesAddr)(this, theDamageFlags); }
    void GetTrackPosition(const char *theTrackName, float &thePosX, float &thePosY) {
        reinterpret_cast<void (*)(Zombie *, const char *, float &, float &)>(Zombie_GetTrackPositionAddr)(this, theTrackName, thePosX, thePosY);
    }
    void UpdateAnimSpeed() { reinterpret_cast<void (*)(Zombie *)>(Zombie_UpdateAnimSpeedAddr)(this); }
    void HitIceTrap() { reinterpret_cast<void (*)(Zombie *)>(Zombie_HitIceTrapAddr)(this); }
    void Create() { reinterpret_cast<void (*)(Zombie *)>(Zombie_ZombieAddr)(this); };
    void UpdateZombiePosition() { reinterpret_cast<void (*)(Zombie *)>(Zombie_UpdateZombiePositionAddr)(this); };
    void LoadReanim(ReanimationType theReanimationType) { reinterpret_cast<void (*)(Zombie *, ReanimationType)>(Zombie_LoadReanimAddr)(this, theReanimationType); }
    void LoadPlainZombieReanim() { reinterpret_cast<void (*)(Zombie *)>(Zombie_LoadPlainZombieReanimAddr)(this); }
    void AttachShield() { reinterpret_cast<void (*)(Zombie *)>(Zombie_AttachShieldAddr)(this); }

    Zombie() { Create(); }
    void ZombieInitialize(int theRow, ZombieType theType, bool theVariant, Zombie *theParentZombie, int theFromWave, bool theIsVisible);
    void Draw(Sexy::Graphics *g);
    void DieNoLoot();
    void Update();
    void UpdateActions();
    void UpdateZombieGargantuar();
    void UpdateZombiePeaHead();
    void UpdateZombieGatlingHead();
    void BurnRow(int theRow);
    void UpdateZombieJalapenoHead();
    //    void UpdateZombieSquashHead();
    void BossDestroyIceballInRow(int theRow);
    int GetDancerFrame();
    void RiseFromGrave(int theGridX, int theGridY);
    void EatPlant(Plant *thePlant);
    void DetachShield();
    void CheckForBoardEdge();
    void DrawBossPart(Sexy::Graphics *g, int theBossPart);
    static bool ZombieTypeCanGoInPool(ZombieType theZombieType);
    void BossSpawnAttack();
    void DrawBungeeCord(Sexy::Graphics *graphics, int theOffsetX, int theOffsetY);
    bool IsTangleKelpTarget();
    void DrawReanim(Sexy::Graphics *graphics, ZombieDrawPosition *theZombieDrawPosition, int theBaseRenderGroup);
    void DropHead(unsigned int theDamageFlags);
    Plant *FindPlantTarget(ZombieAttackType theAttackType);
    Zombie *FindZombieTarget();
    void TakeDamage(int theDamage, unsigned int theDamageFlags);
    void PlayZombieReanim(const char *theTrackName, ReanimLoopType theLoopType, int theBlendTime, float theAnimRate);
    void StartWalkAnim(int theBlendTime);
    void ReanimShowPrefix(const char *theTrackPrefix, int theRenderGroup);
    void ReanimShowTrack(const char *theTrackName, int theRenderGroup);
    float GetPosYBasedOnRow(int theRow);
    void SetRow(int theRow);
    void StartMindControlled();
    void UpdateReanim();
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
    void StopEating();
    Sexy::Rect GetZombieRect();
    void GetDrawPos(ZombieDrawPosition &theDrawPos);
    bool IsOnHighGround();
    bool IsOnBoard();
    bool EffectedByDamage(unsigned int theDamageRangeFlags);
    int GetHelmDamageIndex();
    int GetBodyDamageIndex();
    int GetShieldDamageIndex();
    bool IsFireResistant();
    void PickRandomSpeed();

};

class ZombieDefinition {
public:
    ZombieType mZombieType;
    ReanimationType mReanimationType;
    int mZombieValue;
    int mStartingLevel;
    int mFirstAllowedWave;
    int mPickWeight;
    const SexyChar *mZombieName;
};
extern ZombieDefinition gZombieDefs[NUM_ZOMBIE_TYPES];

ZombieDefinition &GetZombieDefinition(ZombieType theZombieType);
/***************************************************************************************************************/

inline bool zombieBloated;
inline bool showZombieBodyHealth;
inline bool showGargantuarHealth;
inline bool showHelmAndShieldHealth;
inline int maidCheats; // 女仆秘籍
inline int boardEdgeAdjust;
inline int zombieSetScale;
inline ZombieDefinition gZombieTrashBinDef = {ZombieType::ZOMBIE_TRASH_BIN, ReanimationType::REANIM_ZOMBIE, 1, 99, 1, 4000, "TRASHCAN_ZOMBIE"};


inline void (*old_Zombie_Update)(Zombie *a1);

inline void (*old_Zombie_UpdateActions)(Zombie*);

inline void (*old_Zombie_Draw)(Zombie *zombie, Sexy::Graphics *graphics);

inline void (*old_Zombie_DrawBossPart)(Zombie *a1, Sexy::Graphics *graphics, int theBossPart);

inline bool (*old_ZombieTypeCanGoInPool)(ZombieType zombieType);

inline void (*old_Zombie_RiseFromGrave)(Zombie *zombie, int gridX, int gridY);

inline void (*old_Zombie_EatPlant)(Zombie *a, Plant *a2);

inline void (*old_Zombie_DetachShield)(Zombie *zombie);

inline void (*old_Zombie_ZombieInitialize)(Zombie *zombie, int theRow, ZombieType theType, bool theVariant, Zombie *theParentZombie, int theFromWave, bool isVisible);

inline void (*old_Zombie_DieNoLoot)(Zombie *);

inline void (*old_Zombie_DrawReanim)(Zombie *zombie, Sexy::Graphics *graphics, ZombieDrawPosition *zombieDrawPosition, int theBaseRenderGroup);

inline void (*old_Zombie_DropHead)(Zombie *zombie, unsigned int a2);

inline Plant *(*old_Zombie_FindPlantTarget)(Zombie *, ZombieAttackType);

inline Zombie *(*old_Zombie_FindZombieTarget)(Zombie *);

inline void (*old_Zombie_TakeDamage)(Zombie *, int theDamage, unsigned int theDamageFlags);

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

inline void (*old_Zombie_StopEating)(Zombie *);
#endif // PVZ_TV_1_1_5_ZOMBIE_H
