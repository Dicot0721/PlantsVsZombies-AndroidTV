#include "PvZ/Lawn/Board/Zombie.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Board/Challenge.h"
#include "PvZ/Lawn/Board/GridItem.h"
#include "PvZ/Lawn/Board/Plant.h"
#include "PvZ/Lawn/Board/Projectile.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/MagicAddr.h"
#include "PvZ/Misc.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Common/TodCommon.h"
#include "PvZ/TodLib/Effect/Reanimator.h"
#include "PvZ/TodLib/Effect/Attachment.h"

using namespace Sexy;
using namespace std;

ZombieDefinition gZombieDefs[NUM_ZOMBIE_TYPES] = {
    {ZOMBIE_NORMAL, REANIM_ZOMBIE, 1, 1, 1, 4000, _S("ZOMBIE")},
    {ZOMBIE_FLAG, REANIM_ZOMBIE, 1, 1, 1, 0, _S("FLAG_ZOMBIE")},
    {ZOMBIE_TRAFFIC_CONE, REANIM_ZOMBIE, 2, 3, 1, 4000, _S("CONEHEAD_ZOMBIE")},
    {ZOMBIE_POLEVAULTER, REANIM_POLEVAULTER, 2, 6, 5, 2000, _S("POLE_VAULTING_ZOMBIE")},
    {ZOMBIE_PAIL, REANIM_ZOMBIE, 4, 8, 1, 3000, _S("BUCKETHEAD_ZOMBIE")},
    {ZOMBIE_NEWSPAPER, REANIM_ZOMBIE_NEWSPAPER, 2, 11, 1, 1000, _S("NEWSPAPER_ZOMBIE")},
    {ZOMBIE_DOOR, REANIM_ZOMBIE, 4, 13, 5, 3500, _S("SCREEN_DOOR_ZOMBIE")},
    {ZOMBIE_FOOTBALL, REANIM_ZOMBIE_FOOTBALL, 7, 16, 5, 2000, _S("FOOTBALL_ZOMBIE")},
    {ZOMBIE_DANCER, REANIM_DANCER, 5, 18, 5, 1000, _S("DANCING_ZOMBIE")},
    {ZOMBIE_BACKUP_DANCER, REANIM_BACKUP_DANCER, 1, 18, 1, 0, _S("BACKUP_DANCER")},
    {ZOMBIE_DUCKY_TUBE, REANIM_ZOMBIE, 1, 21, 5, 0, _S("DUCKY_TUBE_ZOMBIE")},
    {ZOMBIE_SNORKEL, REANIM_SNORKEL, 3, 23, 10, 2000, _S("SNORKEL_ZOMBIE")},
    {ZOMBIE_ZAMBONI, REANIM_ZOMBIE_ZAMBONI, 7, 26, 10, 2000, _S("ZOMBONI")},
    {ZOMBIE_BOBSLED, REANIM_BOBSLED, 3, 26, 10, 2000, _S("ZOMBIE_BOBSLED_TEAM")},
    {ZOMBIE_DOLPHIN_RIDER, REANIM_ZOMBIE_DOLPHINRIDER, 3, 28, 10, 1500, _S("DOLPHIN_RIDER_ZOMBIE")},
    {ZOMBIE_JACK_IN_THE_BOX, REANIM_JACKINTHEBOX, 3, 31, 10, 1000, _S("JACK_IN_THE_BOX_ZOMBIE")},
    {ZOMBIE_BALLOON, REANIM_BALLOON, 2, 33, 10, 2000, _S("BALLOON_ZOMBIE")},
    {ZOMBIE_DIGGER, REANIM_DIGGER, 4, 36, 10, 1000, _S("DIGGER_ZOMBIE")},
    {ZOMBIE_POGO, REANIM_POGO, 4, 38, 10, 1000, _S("POGO_ZOMBIE")},
    {ZOMBIE_YETI, REANIM_YETI, 4, 40, 1, 1, _S("ZOMBIE_YETI")},
    {ZOMBIE_BUNGEE, REANIM_BUNGEE, 3, 41, 10, 1000, _S("BUNGEE_ZOMBIE")},
    {ZOMBIE_LADDER, REANIM_LADDER, 4, 43, 10, 1000, _S("LADDER_ZOMBIE")},
    {ZOMBIE_CATAPULT, REANIM_CATAPULT, 5, 46, 10, 1500, _S("CATAPULT_ZOMBIE")},
    {ZOMBIE_GARGANTUAR, REANIM_GARGANTUAR, 10, 48, 15, 1500, _S("GARGANTUAR")},
    {ZOMBIE_IMP, REANIM_IMP, 10, 48, 1, 0, _S("IMP")},
    {ZOMBIE_BOSS, REANIM_BOSS, 10, 50, 1, 0, _S("BOSS")},
    {ZOMBIE_TRASH_BIN, REANIM_ZOMBIE, 1, 99, 1, 4000, _S("TRASHCAN_ZOMBIE")},
    {ZOMBIE_PEA_HEAD, REANIM_ZOMBIE, 1, 99, 1, 4000, _S("ZOMBIE")},
    {ZOMBIE_WALLNUT_HEAD, REANIM_ZOMBIE, 4, 99, 1, 3000, _S("ZOMBIE")},
    {ZOMBIE_JALAPENO_HEAD, REANIM_ZOMBIE, 3, 99, 10, 1000, _S("ZOMBIE")},
    {ZOMBIE_GATLING_HEAD, REANIM_ZOMBIE, 3, 99, 10, 2000, _S("ZOMBIE")},
    {ZOMBIE_SQUASH_HEAD, REANIM_ZOMBIE, 3, 99, 10, 2000, _S("ZOMBIE")},
    {ZOMBIE_TALLNUT_HEAD, REANIM_ZOMBIE, 4, 99, 10, 2000, _S("ZOMBIE")},
    {ZOMBIE_REDEYE_GARGANTUAR, REANIM_GARGANTUAR, 10, 48, 15, 6000, _S("REDEYED_GARGANTUAR")},
};

ZombieDefinition &GetZombieDefinition(ZombieType theZombieType) {
    if (theZombieType == ZOMBIE_TRASH_BIN) {
        return gZombieTrashBinDef;
    }

    return gZombieDefs[theZombieType];
}

void Zombie::ZombieInitialize(int theRow, ZombieType theType, bool theVariant, Zombie *theParentZombie, int theFromWave, bool isVisible) {
    old_Zombie_ZombieInitialize(this, theRow, theType, theVariant, theParentZombie, theFromWave, isVisible);

    if (zombieSetScale != 0 && mZombieType != ZombieType::ZOMBIE_BOSS) {
        mScaleZombie = 0.2 * zombieSetScale;
        UpdateAnimSpeed();
        float theRatio = mScaleZombie * mScaleZombie;
        mBodyHealth *= theRatio;
        mHelmHealth *= theRatio;
        mShieldHealth *= theRatio;
        mFlyingHealth *= theRatio;
        mBodyMaxHealth = mBodyHealth;
        mHelmMaxHealth = mHelmHealth;
        mShieldMaxHealth = mShieldHealth;
        mFlyingMaxHealth = mFlyingHealth;
    }

    if (IsZombatarZombie(theType) && theFromWave != -3) {
        SetZombatarReanim();
    }
}

bool Zombie::IsOnBoard() {
    if (mFromWave == Zombie::ZOMBIE_WAVE_CUTSCENE || mFromWave == Zombie::ZOMBIE_WAVE_UI) {
        return false;
    }

    return true;
}

void Zombie::Update() {
    if (zombieBloated) {
        // 如果开启了“普僵必噎死”
        mBloated = mZombieType == ZombieType::ZOMBIE_NORMAL && !mInPool;
    }

    if (requestPause) {
        // 如果开了高级暂停
        return;
    }

    if (mZombieType == ZombieType::ZOMBIE_FLAG && mBossFireBallReanimID != 0) {
        Reanimation *reanimation = mApp->ReanimationTryToGet(mBossFireBallReanimID);
        if (reanimation != nullptr)
            reanimation->Update();
    }

    return old_Zombie_Update(this);
}

void Zombie::UpdateActions() {
    old_Zombie_UpdateActions(this);

    // UpdateZombieType类函数在此处添加
}

void Zombie::SquishAllInSquare(int theX, int theY, ZombieAttackType theAttackType) {
    return old_Zombie_SquishAllInSquare(this, theX, theY, theAttackType);
}

void Zombie::UpdateZombieGargantuar() {
    // 修复巨人不索敌敌方僵尸的BUG
    if (mZombiePhase == ZombiePhase::PHASE_GARGANTUAR_SMASHING) {
        Reanimation *aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->ShouldTriggerTimedEvent(0.64f)) {
            Zombie *aZombie = FindZombieTarget();
            if (aZombie) {
                aZombie->TakeDamage(1500, 0U);
            }

            Plant *aPlant = FindPlantTarget(ZombieAttackType::ATTACKTYPE_CHEW);
            if (aPlant) {
                if (aPlant->mSeedType == SeedType::SEED_SPIKEROCK) {
                    TakeDamage(20, 32U);
                    aPlant->SpikeRockTakeDamage();
                    if (aPlant->mPlantHealth <= 0) {
                        SquishAllInSquare(aPlant->mPlantCol, aPlant->mRow, ZombieAttackType::ATTACKTYPE_CHEW);
                    }
                } else {
                    SquishAllInSquare(aPlant->mPlantCol, aPlant->mRow, ZombieAttackType::ATTACKTYPE_CHEW);
                }
            }

            if (mApp->IsScaryPotterLevel()) {
                int aGridX = mBoard->PixelToGridX(mPosX, mPosY);
                GridItem *aScaryPot = mBoard->GetScaryPotAt(aGridX, mRow);
                if (aScaryPot) {
                    mBoard->mChallenge->ScaryPotterOpenPot(aScaryPot);
                }
            }

            if (mApp->IsIZombieLevel()) {
                GridItem *aBrain = mBoard->mChallenge->IZombieGetBrainTarget(this);
                if (aBrain) {
                    mBoard->mChallenge->IZombieSquishBrain(aBrain);
                }
            }

            mApp->PlayFoley(FoleyType::FOLEY_THUMP);
            mBoard->ShakeBoard(0, 3);
        }

        if (aBodyReanim->mLoopCount > 0) {
            mZombiePhase = ZombiePhase::PHASE_ZOMBIE_NORMAL;
            StartWalkAnim(20);
        }

        return;
    }

    float aThrowingDistance = mPosX - 360.0f;
    if (mZombiePhase == ZombiePhase::PHASE_GARGANTUAR_THROWING) {
        Reanimation *aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->ShouldTriggerTimedEvent(0.74f)) {
            mHasObject = false;
            ReanimShowPrefix("Zombie_imp", RENDER_GROUP_HIDDEN);
            ReanimShowTrack("Zombie_gargantuar_whiterope", RENDER_GROUP_HIDDEN);
            mApp->PlayFoley(FoleyType::FOLEY_SWING);

            Zombie *aZombieImp = mBoard->AddZombie(ZombieType::ZOMBIE_IMP, mFromWave, false);
            if (aZombieImp == nullptr)
                return;

            float aMinThrowDistance = 40.0f;
            if (mBoard->StageHasRoof()) {
                aThrowingDistance -= 180.0f;
                aMinThrowDistance = -140.0f;
            }
            if (aThrowingDistance < aMinThrowDistance) {
                aThrowingDistance = aMinThrowDistance;
            } else if (aThrowingDistance > 140.0f) {
                aThrowingDistance -= RandRangeFloat(0.0f, 100.0f);
            }

            aZombieImp->mPosX = mPosX - 133.0f;
            aZombieImp->mPosY = GetPosYBasedOnRow(mRow);
            aZombieImp->SetRow(mRow);
            aZombieImp->mVariant = false;
            aZombieImp->mAltitude = 88.0f;
            aZombieImp->mRenderOrder = mRenderOrder + 1;
            aZombieImp->mZombiePhase = ZombiePhase::PHASE_IMP_GETTING_THROWN;
            // 修复魅惑巨人不投掷魅惑小鬼的BUG
            aZombieImp->mScaleZombie = mScaleZombie;
            aZombieImp->mBodyHealth *= mScaleZombie * mScaleZombie;
            aZombieImp->mBodyMaxHealth *= mScaleZombie * mScaleZombie;

            if (mMindControlled) {
                aZombieImp->mPosX = mPosX + mWidth;
                aZombieImp->StartMindControlled();
                aZombieImp->mVelX = -3.0f;
            } else {
                aZombieImp->mVelX = 3.0f;
            }
            aZombieImp->mChilledCounter = mChilledCounter;
            aZombieImp->mVelZ = 0.5f * (aThrowingDistance / aZombieImp->mVelX) * THOWN_ZOMBIE_GRAVITY;
            aZombieImp->PlayZombieReanim("anim_thrown", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 18.0f);
            aZombieImp->UpdateReanim();
            mApp->PlayFoley(FoleyType::FOLEY_IMP);
        }

        if (aBodyReanim->mLoopCount > 0) {
            mZombiePhase = ZombiePhase::PHASE_ZOMBIE_NORMAL;
            StartWalkAnim(20);
        }

        return;
    }

    if (IsImmobilizied() || !mHasHead)
        return;

    if (mHasObject && mBodyHealth < mBodyMaxHealth / 2 && aThrowingDistance > 40.0f) {
        mZombiePhase = ZombiePhase::PHASE_GARGANTUAR_THROWING;
        PlayZombieReanim("anim_throw", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 24.0f);
        return;
    }

    bool doSmash = false;
    if (FindPlantTarget(ZombieAttackType::ATTACKTYPE_CHEW)) {
        doSmash = true;
    } else if (mApp->IsScaryPotterLevel()) {
        int aGridX = mBoard->PixelToGridX(mPosX, mPosY);
        if (mBoard->GetScaryPotAt(aGridX, mRow)) {
            doSmash = true;
        }
    } else if (mApp->IsIZombieLevel()) {
        if (mBoard->mChallenge->IZombieGetBrainTarget(this)) {
            doSmash = true;
        }
    } else if (FindZombieTarget()) {
        doSmash = true;
    }

    if (doSmash) {
        mZombiePhase = ZombiePhase::PHASE_GARGANTUAR_SMASHING;
        mApp->PlayFoley(FoleyType::FOLEY_LOW_GROAN);
        PlayZombieReanim("anim_smash", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 16.0f);
    }
}

void Zombie::UpdateZombiePeaHead() {
    // 用于修复豌豆僵尸被魅惑后依然向左发射会伤害植物的子弹的BUG、啃食时不发射子弹的BUG
    // 游戏原版逻辑是判断是否hasHead 且 是否isEating。这里去除对吃植物的判断

    if (!mHasHead)
        return;

    if (mPhaseCounter == 35) {
        Reanimation *aHeadReanim = mApp->ReanimationGet(mSpecialHeadReanimID);
        aHeadReanim->PlayReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 35.0f);
    } else if (mPhaseCounter <= 0) {
        Reanimation *aHeadReanim = mApp->ReanimationGet(mSpecialHeadReanimID);
        aHeadReanim->PlayReanim("anim_head_idle", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 15.0f);
        mApp->PlayFoley(FoleyType::FOLEY_THROW);

        Reanimation *aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        int index = aBodyReanim->FindTrackIndexById(*ReanimTrackId_anim_head1_Addr);
        ReanimatorTransform aTransForm = ReanimatorTransform();
        aBodyReanim->GetCurrentTransform(index, &aTransForm);

        float aOriginX = mPosX + aTransForm.mTransX - 9.0f;
        float aOriginY = mPosY + aTransForm.mTransY + 6.0f - mAltitude;

        if (mMindControlled) // 魅惑修复
        {
            aOriginX += 90.0f * mScaleZombie;
            Projectile *aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder, mRow, ProjectileType::PROJECTILE_PEA);
            aProjectile->mDamageRangeFlags = 1;
        } else {
            Projectile *aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder, mRow, ProjectileType::PROJECTILE_ZOMBIE_PEA);
            aProjectile->mMotionType = ProjectileMotion::MOTION_BACKWARDS;
        }

        mPhaseCounter = 150;
    }
}

void Zombie::UpdateZombieGatlingHead() {
    // 用于修复加特林僵尸被魅惑后依然向左发射会伤害植物的子弹的BUG、啃食时不发射子弹的BUG
    // 游戏原版逻辑是判断是否hasHead 且 是否isEating。这里去除对吃植物的判断

    if (!mHasHead)
        return;

    if (mPhaseCounter == 100) {
        Reanimation *mSpecialHeadReanim = mApp->ReanimationGet(mSpecialHeadReanimID);
        mSpecialHeadReanim->PlayReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 35.0f);
    } else if (mPhaseCounter == 18 || mPhaseCounter == 35 || mPhaseCounter == 51 || mPhaseCounter == 68) {
        mApp->PlayFoley(FoleyType::FOLEY_THROW);

        Reanimation *aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        int aTrackIndex = aBodyReanim->FindTrackIndexById(*ReanimTrackId_anim_head1_Addr);
        ReanimatorTransform aTransForm = ReanimatorTransform();
        aBodyReanim->GetCurrentTransform(aTrackIndex, &aTransForm);

        float aOriginX = mPosX + aTransForm.mTransX - 9.0f;
        float aOriginY = mPosY + aTransForm.mTransY + 6.0f - mAltitude;

        if (mMindControlled) // 魅惑修复
        {
            aOriginX += 90.0f * mScaleZombie;
            Projectile *aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder, mRow, ProjectileType::PROJECTILE_PEA);
            aProjectile->mDamageRangeFlags = 1;
        } else {
            Projectile *aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder, mRow, ProjectileType::PROJECTILE_ZOMBIE_PEA);
            aProjectile->mMotionType = ProjectileMotion::MOTION_BACKWARDS;
        }
    } else if (mPhaseCounter <= 0) {
        Reanimation *aHeadReanim = mApp->ReanimationGet(mSpecialHeadReanimID);
        aHeadReanim->PlayReanim("anim_head_idle", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 15.0f);
        mPhaseCounter = 150;
    }
}

void Zombie::BossDestroyIceballInRow(int theRow) {
    if (theRow != mFireballRow)
        return;

    Reanimation *aFireBallReanim = mApp->ReanimationTryToGet(mBossFireBallReanimID);
    if (aFireBallReanim && !mIsFireBall) {
        mApp->AddTodParticle(mPosX + 80.0, mAnimCounter + 80.0, 400000, ParticleEffect::PARTICLE_ICEBALL_DEATH);

        aFireBallReanim->ReanimationDie();
        mBossFireBallReanimID = ReanimationID::REANIMATIONID_NULL;
        mBoard->RemoveParticleByType(ParticleEffect::PARTICLE_ICEBALL_TRAIL);
    }
}

void Zombie::BurnRow(int theRow) {
    // 辣椒僵尸被魅惑后的爆炸函数

    Zombie *aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie)) {
        if ((aZombie->mZombieType == ZombieType::ZOMBIE_BOSS || aZombie->mRow == theRow) && aZombie->EffectedByDamage(127)) {
            aZombie->RemoveColdEffects();
            aZombie->ApplyBurn();
        }
    }

    GridItem *aGridItem = nullptr;
    while (mBoard->IterateGridItems(aGridItem)) {
        if (aGridItem->mGridY == theRow && aGridItem->mGridItemType == GridItemType::GRIDITEM_LADDER) {
            aGridItem->GridItemDie();
        }
    }

    Zombie *aBossZombie = mBoard->GetBossZombie();
    if (aBossZombie && aBossZombie->mFireballRow == theRow) {
        aBossZombie->BossDestroyIceballInRow(theRow);
    }
}

void Zombie::UpdateZombieJalapenoHead() {
    // 修复辣椒僵尸被魅惑后爆炸依然伤害植物的BUG

    if (!mHasHead)
        return;

    if (mPhaseCounter == 0) {
        mApp->PlayFoley(FoleyType::FOLEY_JALAPENO_IGNITE);
        mApp->PlayFoley(FoleyType::FOLEY_JUICY);
        mBoard->DoFwoosh(mRow);
        mBoard->ShakeBoard(3, -4);

        if (mMindControlled) // 魅惑修复
        {
            BurnRow(mRow);
        } else {
            Plant *aPlant = nullptr;
            while (mBoard->IteratePlants(aPlant)) {
                // Rect aPlantRect = aPlant->GetPlantRect();
                if (aPlant->mRow == mRow && !aPlant->NotOnGround()) {
                    mBoard->mPlantsEaten++;
                    aPlant->Die();
                }
            }
        }
    }
}

// void Zombie::UpdateZombieSquashHead()
//{
//     if (mHasHead && mIsEating && mZombiePhase == ZombiePhase::PHASE_SQUASH_PRE_LAUNCH)
//     {
//         StopEating();
//         PlayZombieReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 20, 12.0f);
//         mHasHead = false;
//
//         Reanimation* aHeadReanim = mApp->ReanimationGet(mSpecialHeadReanimID);
//         aHeadReanim->PlayReanim("anim_jumpup", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 24.0f);
//         aHeadReanim->mRenderOrder = mRenderOrder + 1;
//         aHeadReanim->SetPosition(mPosX + 6.0f, mPosY - 21.0f);
//
//         Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
//         ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName("anim_head1");
//         AttachmentDetach(aTrackInstance->mAttachmentID);
//         aHeadReanim->OverrideScale(0.75f, 0.75f);
//         aHeadReanim->mOverlayMatrix.m10 = 0.0f;
//
//         mZombiePhase = ZombiePhase::PHASE_SQUASH_RISING;
//         mPhaseCounter = 95;
//     }
//
//     if (mZombiePhase == ZombiePhase::PHASE_SQUASH_RISING)
//     {
//         int aDestX = mBoard->GridToPixelX(mBoard->PixelToGridXKeepOnBoard(mX, mY), mRow);
//
//         if (mMindControlled)
//         {
//             Zombie* aZombie = FindZombieTarget();
//             if (aZombie)
//             {
//                 aDestX = aZombie->ZombieTargetLeadX(0.0f);
//             }
//             else
//             {
//                 aDestX += 90.0f * mScaleZombie;
//             }
//         }
//
//         int aPosX = TodAnimateCurve(50, 20, mPhaseCounter, 0, aDestX - mPosX, TodCurves::CURVE_EASE_IN_OUT);
//         int aPosY = TodAnimateCurve(50, 20, mPhaseCounter, 0, -20, TodCurves::CURVE_EASE_IN_OUT);
//
//         Reanimation* aHeadReanim = mApp->ReanimationGet(mSpecialHeadReanimID);
//         aHeadReanim->SetPosition(mPosX + aPosX + 6.0f, mPosY + aPosY - 21.0f);
//
//         if (mPhaseCounter == 0)
//         {
//             aHeadReanim = mApp->ReanimationGet(mSpecialHeadReanimID);
//             aHeadReanim->PlayReanim("anim_jumpdown", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 60.0f);
//             mZombiePhase = ZombiePhase::PHASE_SQUASH_FALLING;
//             mPhaseCounter = 10;
//         }
//     }
//
//     if (mZombiePhase == ZombiePhase::PHASE_SQUASH_FALLING)
//     {
//         int aPosY = TodAnimateCurve(10, 0, mPhaseCounter, -20, 74, TodCurves::CURVE_LINEAR);
//         int aDestX = mBoard->GridToPixelX(mBoard->PixelToGridXKeepOnBoard(mX, mY), mRow);
//
//         if (mMindControlled)
//         {
//             Zombie* aZombie = FindZombieTarget();
//             if (aZombie)
//             {
//                 aDestX = aZombie->ZombieTargetLeadX(0.0f);
//             }
//             else
//             {
//                 aDestX += 90.0f * mScaleZombie;
//             }
//         }
//
//         Reanimation* aHeadReanim = mApp->ReanimationGet(mSpecialHeadReanimID);
//         aHeadReanim->SetPosition(mPosX + 6.0f + aDestX - mPosX, mPosY - 21.0f + aPosY);
//
//         if (mPhaseCounter == 2)
//         {
//             if (mMindControlled)  // 魅惑修复
//             {
//                 Rect aAttackRect(aDestX - 73, mPosY + 4, 65, 90);  // 具体数值未实测，待定
//
//                 Zombie* aZombie = nullptr;
//                 while (mBoard->IterateZombies(aZombie))
//                 {
//                     if ((aZombie->mRow == mRow || aZombie->mZombieType == ZombieType::ZOMBIE_BOSS) && aZombie->EffectedByDamage(13U))
//                     {
//                         Rect aZombieRect = aZombie->GetZombieRect();
//                         if (GetRectOverlap(aAttackRect, aZombieRect) > (aZombie->mZombieType == ZombieType::ZOMBIE_FOOTBALL ? -20 : 0))
//                         {
//                             aZombie->TakeDamage(1800, 18U);
//                         }
//                     }
//                 }
//             }
//             else
//             {
//                 SquishAllInSquare(mBoard->PixelToGridXKeepOnBoard(mX, mY), mRow, ZombieAttackType::ATTACKTYPE_CHEW);
//             }
//         }
//
//         if (mPhaseCounter == 0)
//         {
//             mZombiePhase = ZombiePhase::PHASE_SQUASH_DONE_FALLING;
//             mPhaseCounter = 100;
//
//             mBoard->ShakeBoard(1, 4);
//             mApp->PlayFoley(FoleyType::FOLEY_THUMP);
//         }
//     }
//
//     if (mZombiePhase == ZombiePhase::PHASE_SQUASH_DONE_FALLING && mPhaseCounter == 0)
//     {
//         Reanimation* aHeadReanim = mApp->ReanimationGet(mSpecialHeadReanimID);
//         aHeadReanim->ReanimationDie();
//         mSpecialHeadReanimID = ReanimationID::REANIMATIONID_NULL;
//
//         TakeDamage(1800, 9U);
//     }
// }

void Zombie::Draw(Sexy::Graphics *g) {
    // 根据玩家的“僵尸显血”功能是否开启，决定是否在游戏的原始old_Zombie_Draw函数执行完后额外绘制血量文本。
    old_Zombie_Draw(this, g);
    int drawHeightOffset = 0;
    int holder[1];
    if (showZombieBodyHealth || (showGargantuarHealth && (mZombieType == ZombieType::ZOMBIE_GARGANTUAR || mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR))) { // 如果玩家开了"僵尸显血"
        Sexy_StrFormat((int*)holder, "%d/%d", mBodyHealth, mBodyMaxHealth);
        g->SetColor(white);
        g->SetFont(*Sexy_FONT_DWARVENTODCRAFT18_Addr);
        if (mZombieType == ZombieType::ZOMBIE_BOSS) {
            // 如果是僵王,将血量绘制到僵王头顶。从而修复图鉴中僵王血量绘制位置不正确。
            // 此处仅在图鉴中生效,实战中僵王绘制不走Zombie_Draw()，走Zombie_DrawBossPart()
            g->mTransX = 780.0f;
            g->mTransY = 240.0f;
        }
        g->DrawString((SexyString&)holder, 0, drawHeightOffset);
        Sexy_String_Delete(holder);
        g->SetFont(nullptr);
        drawHeightOffset += 20;
    }
    if (showHelmAndShieldHealth) {
        if (mHelmHealth > 0) { // 如果有头盔，绘制头盔血量
            Sexy_StrFormat(holder, "%d/%d", mHelmHealth, mHelmMaxHealth);
            g->SetColor(yellow);
            g->SetFont(*Sexy_FONT_DWARVENTODCRAFT18_Addr);
            g->DrawString((SexyString&)holder, 0, drawHeightOffset);
            Sexy_String_Delete(holder);
            g->SetFont(nullptr);
            drawHeightOffset += 20;
        }
        if (mShieldHealth > 0) { // 如果有盾牌，绘制盾牌血量
            Sexy_StrFormat((int*)holder, "%d/%d", mShieldHealth, mShieldMaxHealth);
            g->SetColor(blue);
            g->SetFont(*Sexy_FONT_DWARVENTODCRAFT18_Addr);
            g->DrawString((SexyString&)holder, 0, drawHeightOffset);
            Sexy_String_Delete((int*)holder);
            g->SetFont(nullptr);
        }
    }
}

void Zombie::DrawBossPart(Sexy::Graphics *g, int theBossPart) {
    // 根据玩家的“僵尸显血”功能是否开启，决定是否在游戏的原始old_Zombie_DrawBossPart函数执行完后额外绘制血量文本。
    old_Zombie_DrawBossPart(this, g, theBossPart);
    if (theBossPart == 3) {
        // 每次绘制Boss都会调用四次本函数，且theBossPart从0到3依次增加，代表绘制Boss的不同Part。
        // 我们只在theBossPart==3时(绘制最后一个部分时)绘制一次血量，免去每次都绘制。
        if (showZombieBodyHealth) { // 如果玩家开了"僵尸显血"
            int holder[1];
            Sexy_StrFormat((int*)holder, "%d/%d", mBodyHealth, mBodyMaxHealth);
            g->SetColor(white);
            g->SetFont(*Sexy_FONT_DWARVENTODCRAFT18_Addr);
            float tmpTransX = g->mTransX;
            float tmpTransY = g->mTransY;
            g->mTransX = 800.0f;
            g->mTransY = 200.0f;
            g->DrawString((SexyString&)holder, 0, 0);
            Sexy_String_Delete((int*)holder);
            g->mTransX = tmpTransX;
            g->mTransY = tmpTransY;
            g->SetFont(nullptr);
        }
    }
}

int Zombie::GetDancerFrame() {
    if (mFromWave == -3 || IsImmobilizied())
        return 0;

    // 女仆秘籍
    switch (maidCheats) {
        case 1:
            return 11; // 保持前进 (DancerDancingLeft)
        case 2:
            return 18; // 跳舞 (DancerRaiseLeft1)
        case 3:
            return 12; // 召唤舞伴 (DancerWalkToRaise)
        default:
            break;
    }

    // 修复女仆秘籍问题、修复舞王和舞者的跳舞时间不吃高级暂停也不吃倍速
    // 关键就是用mBoard.mMainCounter代替mApp.mAppCounter做计时
    int num1 = mZombiePhase == ZombiePhase::PHASE_DANCER_DANCING_IN ? 10 : 20;
    int num2 = mZombiePhase == ZombiePhase::PHASE_DANCER_DANCING_IN ? 110 : 460;
    return mBoard->mMainCounter % num2 / num1;
    // return *(uint32_t *) (lawnApp + 2368) % num2 / num1;
}

bool Zombie::ZombieTypeCanGoInPool(ZombieType theZombieType) {
    // 修复泳池对战的僵尸们走水路时不攻击植物
    LawnApp *lawnApp = (LawnApp *)*gLawnApp_Addr;
    if (lawnApp->mGameMode == GameMode::GAMEMODE_MP_VS && (VSBackGround == 3 || VSBackGround == 4 || VSBackGround == 9)) {
        return true;
    }

    return old_ZombieTypeCanGoInPool(theZombieType);
}

Rect Zombie::GetZombieRect() {
    Rect aZombieRect = mZombieRect;
    if (IsWalkingBackwards()) {
        aZombieRect.mX = mWidth - aZombieRect.mX - aZombieRect.mWidth;
    }

    ZombieDrawPosition aDrawPos;
    GetDrawPos(aDrawPos);
    aZombieRect.Offset(mX, mY + aDrawPos.mBodyY);
    if (aDrawPos.mClipHeight > CLIP_HEIGHT_LIMIT) {
        aZombieRect.mHeight -= aDrawPos.mClipHeight;
    }

    return aZombieRect;
}

void Zombie::RiseFromGrave(int theGridX, int theGridY) {
    // 修复对战切换场地为泳池后的闪退BUG。但是仅仅是修复闪退，泳池对战还谈不上能玩的程度
    if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS) {

        if (mBoard->mPlantRow[theGridY] == PlantRowType::PLANTROW_POOL) {
            //                if (old_ZombieTypeCanGoInPool(mZombieType)) {
            DieNoLoot();
            mBoard->AddZombieInRow(mZombieType, theGridY, mBoard->mCurrentWave, 1);
            return;
            //                }
        }

        BackgroundType tmp = mBoard->mBackground;
        mBoard->mBackground = BackgroundType::BACKGROUND_1_DAY;
        old_Zombie_RiseFromGrave(this, theGridX, theGridY);
        mBoard->mBackground = tmp;
        return;
    }
    old_Zombie_RiseFromGrave(this, theGridX, theGridY);
}

void Zombie::CheckForBoardEdge() {
    // 修复僵尸正常进家、支持调整僵尸进家线
    if (IsWalkingBackwards() && mPosX > 850.0f) {
        DieNoLoot();
        return;
    }
    int boardEdge;
    if (mZombieType == ZombieType::ZOMBIE_POLEVAULTER || mZombieType == ZombieType::ZOMBIE_GARGANTUAR || mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR) {
        // 如果是撑杆、巨人、红眼巨人
        boardEdge = -100;
    } else if (mZombieType == ZombieType::ZOMBIE_FOOTBALL || mZombieType == ZombieType::ZOMBIE_ZAMBONI || mZombieType == ZombieType::ZOMBIE_CATAPULT) {
        // 如果是橄榄球、冰车、篮球
        boardEdge = -125;
    } else if (mZombieType == ZombieType::ZOMBIE_DANCER || mZombieType == ZombieType::ZOMBIE_BACKUP_DANCER || mZombieType == ZombieType::ZOMBIE_SNORKEL) {
        // 如果是舞王、伴舞、潜水
        boardEdge = -80;
    } else {
        // 如果是除上述僵尸外的僵尸
        boardEdge = -50;
    }
    boardEdge -= boardEdgeAdjust; // 支持任意调整进家线
    if (mX <= boardEdge && mHasHead) {
        if (mApp->IsIZombieLevel()) {
            DieNoLoot();
        } else {
            mBoard->ZombiesWon(this);
        }
    }
    if (mX <= boardEdge + 70 && !mHasHead) {
        TakeDamage(1800, 9u);
    }
}

void Zombie::StopEating() {
    return old_Zombie_StopEating(this);
}

void Zombie::EatPlant(Plant *thePlant) {
    // 修复正向出土的矿工不上梯子
    if (mZombiePhase == ZombiePhase::PHASE_DANCER_DANCING_IN) {
        mPhaseCounter = 1;
        return;
    }
    if (mYuckyFace) {
        return;
    }
    int aPlantCol = thePlant->mPlantCol;
    int aRow = thePlant->mRow;
    if (mBoard->GetLadderAt(aPlantCol, aRow) != nullptr && mZombieType == ZombieType::ZOMBIE_DIGGER && mZombiePhase == ZombiePhase::PHASE_DIGGER_WALKING_WITHOUT_AXE && !IsWalkingBackwards()) {
        StopEating();
        if (mZombieHeight == ZombieHeight::HEIGHT_ZOMBIE_NORMAL && mUseLadderCol != aPlantCol) {
            mZombieHeight = ZombieHeight::HEIGHT_UP_LADDER;
            mUseLadderCol = aPlantCol;
        }
        return;
    }
    return old_Zombie_EatPlant(this, thePlant);
}

void Zombie::DetachShield() {
    // 修复梯子僵尸断手后掉梯子时手会恢复的BUG。
    old_Zombie_DetachShield(this);
    if (mShieldType == ShieldType::SHIELDTYPE_LADDER && !mHasArm) {
        ReanimShowPrefix("Zombie_outerarm", -1);
    }
}

void Zombie::BossSpawnAttack() {
    // 修复泳池僵王为六路放僵尸时闪退
    RemoveColdEffects();
    mZombiePhase = ZombiePhase::PHASE_BOSS_SPAWNING;
    if (mBossMode == 0) {
        mSummonCounter = RandRangeInt(450, 550);
    } else if (mBossMode == 1) {
        mSummonCounter = RandRangeInt(350, 450);
    } else if (mBossMode == 2) {
        mSummonCounter = RandRangeInt(150, 250);
    }
    mTargetRow = mBoard->PickRowForNewZombie(ZombieType::ZOMBIE_NORMAL);
    switch (mTargetRow) {
        case 0:
            PlayZombieReanim("anim_spawn_1", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0);
            break;
        case 1:
            PlayZombieReanim("anim_spawn_2", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0);
            break;
        case 2:
            PlayZombieReanim("anim_spawn_3", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0);
            break;
        case 3:
            PlayZombieReanim("anim_spawn_4", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0);
            break;
        default:
            PlayZombieReanim("anim_spawn_5", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0);
            break;
    }

    mApp->PlayFoley(FoleyType::FOLEY_HYDRAULIC_SHORT);
}

bool Zombie::IsBouncingPogo() {
    return mZombiePhase >= ZombiePhase::PHASE_POGO_BOUNCING && mZombiePhase <= ZombiePhase::PHASE_POGO_FORWARD_BOUNCE_7;
}

bool Zombie::IsFlying() {
    return mZombiePhase == ZombiePhase::PHASE_BALLOON_FLYING || mZombiePhase == ZombiePhase::PHASE_BALLOON_POPPING;
}

int Zombie::GetBobsledPosition() {
    return old_Zombie_GetBobsledPosition(this);
}

bool Zombie::IsBobsledTeamWithSled() {
    return GetBobsledPosition() != -1;
}

bool Zombie::IsDeadOrDying() {
    return mDead || mZombiePhase == ZombiePhase::PHASE_ZOMBIE_DYING || mZombiePhase == ZombiePhase::PHASE_ZOMBIE_BURNED || mZombiePhase == ZombiePhase::PHASE_ZOMBIE_MOWERED;
}

bool Zombie::CanBeChilled() {
    if (mZombieType == ZombieType::ZOMBIE_ZAMBONI || IsBobsledTeamWithSled())
        return false;

    if (IsDeadOrDying())
        return false;

    if (mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING || mZombiePhase == ZombiePhase::PHASE_DIGGER_RISING || mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING_PAUSE_WITHOUT_AXE
        || mZombiePhase == ZombiePhase::PHASE_DIGGER_RISE_WITHOUT_AXE || mZombiePhase == ZombiePhase::PHASE_RISING_FROM_GRAVE || mZombiePhase == ZombiePhase::PHASE_DANCER_RISING)
        return false;

    if (mMindControlled)
        return false;

    return mZombieType != ZombieType::ZOMBIE_BOSS || mZombiePhase == ZombiePhase::PHASE_BOSS_HEAD_IDLE_BEFORE_SPIT || mZombiePhase == ZombiePhase::PHASE_BOSS_HEAD_IDLE_AFTER_SPIT
        || mZombiePhase == ZombiePhase::PHASE_BOSS_HEAD_SPIT;
}

bool Zombie::CanBeFrozen() {
    if (!CanBeChilled())
        return false;

    if (mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT || mZombiePhase == ZombiePhase::PHASE_DOLPHIN_INTO_POOL || mZombiePhase == ZombiePhase::PHASE_DOLPHIN_IN_JUMP
        || mZombiePhase == ZombiePhase::PHASE_SNORKEL_INTO_POOL || IsFlying() || mZombiePhase == ZombiePhase::PHASE_IMP_GETTING_THROWN || mZombiePhase == ZombiePhase::PHASE_IMP_LANDING
        || mZombiePhase == ZombiePhase::PHASE_BOBSLED_CRASHING || mZombiePhase == ZombiePhase::PHASE_JACK_IN_THE_BOX_POPPING || mZombiePhase == ZombiePhase::PHASE_SQUASH_RISING
        || mZombiePhase == ZombiePhase::PHASE_SQUASH_FALLING || mZombiePhase == ZombiePhase::PHASE_SQUASH_DONE_FALLING || IsBouncingPogo())
        return false;

    return mZombieType != ZombieType::ZOMBIE_BUNGEE || mZombiePhase == ZombiePhase::PHASE_BUNGEE_AT_BOTTOM;
}

bool Zombie::EffectedByDamage(unsigned int theDamageRangeFlags) {
    if (!TestBit(theDamageRangeFlags, (int)DamageRangeFlags::DAMAGES_DYING) && IsDeadOrDying()) {
        return false;
    }

    if (TestBit(theDamageRangeFlags, (int)DamageRangeFlags::DAMAGES_ONLY_MINDCONTROLLED)) {
        if (!mMindControlled) {
            return false;
        }
    } else if (mMindControlled) {
        return false;
    }

    if (mZombieType == ZombieType::ZOMBIE_BUNGEE && mZombiePhase != ZombiePhase::PHASE_BUNGEE_AT_BOTTOM && mZombiePhase != ZombiePhase::PHASE_BUNGEE_GRABBING) {
        return false; // 蹦极僵尸只有在停留时才会受到攻击
    }

    if (mZombieHeight == ZombieHeight::HEIGHT_GETTING_BUNGEE_DROPPED) {
        return false; // 被空投的过程中不会受到攻击
    }

    if (mZombieType == ZombieType::ZOMBIE_BOSS) {
        Reanimation *aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (mZombiePhase == ZombiePhase::PHASE_BOSS_HEAD_ENTER && aBodyReanim->mAnimTime < 0.5f) {
            return false;
        }
        if (mZombiePhase == ZombiePhase::PHASE_BOSS_HEAD_LEAVE && aBodyReanim->mAnimTime > 0.5f) {
            return false;
        }

        if (mZombiePhase != ZombiePhase::PHASE_BOSS_HEAD_IDLE_BEFORE_SPIT && mZombiePhase != ZombiePhase::PHASE_BOSS_HEAD_IDLE_AFTER_SPIT && mZombiePhase != ZombiePhase::PHASE_BOSS_HEAD_SPIT) {
            return false; // 僵王博士只有在低头状态下才会受到攻击
        }
    }

    if (mZombieType == ZombieType::ZOMBIE_BOBSLED && GetBobsledPosition() > 0) {
        return false; // 存在雪橇时，只有领头僵尸会受到攻击
    }

    if (mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT || mZombiePhase == ZombiePhase::PHASE_IMP_GETTING_THROWN || mZombiePhase == ZombiePhase::PHASE_DIGGER_RISING
        || mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING_PAUSE_WITHOUT_AXE || mZombiePhase == ZombiePhase::PHASE_DIGGER_RISE_WITHOUT_AXE || mZombiePhase == ZombiePhase::PHASE_DOLPHIN_INTO_POOL
        || mZombiePhase == ZombiePhase::PHASE_DOLPHIN_IN_JUMP || mZombiePhase == ZombiePhase::PHASE_SNORKEL_INTO_POOL || mZombiePhase == ZombiePhase::PHASE_BALLOON_POPPING
        || mZombiePhase == ZombiePhase::PHASE_RISING_FROM_GRAVE || mZombiePhase == ZombiePhase::PHASE_BOBSLED_CRASHING || mZombiePhase == ZombiePhase::PHASE_DANCER_RISING) {
        return TestBit(theDamageRangeFlags, (int)DamageRangeFlags::DAMAGES_OFF_GROUND);
    }

    if (mZombieType != ZombieType::ZOMBIE_BOBSLED && GetZombieRect().mX > WIDE_BOARD_WIDTH) {
        return false; // 除雪橇僵尸小队外，场外的僵尸不会受到攻击
    }

    bool submerged = mZombieType == ZombieType::ZOMBIE_SNORKEL && mInPool && !mIsEating;
    if (TestBit(theDamageRangeFlags, (int)DamageRangeFlags::DAMAGES_SUBMERGED) && submerged) {
        return true;
    }

    bool underground = mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING;
    if (TestBit(theDamageRangeFlags, (int)DamageRangeFlags::DAMAGES_UNDERGROUND) && underground) {
        return true;
    }

    if (TestBit(theDamageRangeFlags, (int)DamageRangeFlags::DAMAGES_FLYING) && IsFlying()) {
        return true;
    }

    return TestBit(theDamageRangeFlags, (int)DamageRangeFlags::DAMAGES_GROUND) && !IsFlying() && !submerged && !underground;
}

void Zombie::AddButter() {
    if (CanBeFrozen() && mZombieType != ZombieType::ZOMBIE_BOSS) {
        // Ban冰车 跳跳 僵王 飞翔的气球 跳跃的撑杆 即将跳水的潜水 等等
        if (mButteredCounter <= 100) {
            if (mButteredCounter == 0) {
                mApp->PlayFoley(FoleyType::FOLEY_BUTTER);
            }
            ApplyButter();
        }
    }
}

bool Zombie::IsWalkingBackwards() {
    if (mMindControlled)
        return true;

    if (mZombieHeight == ZombieHeight::HEIGHT_ZOMBIQUARIUM) {
        if (mVelZ < 1.5707964f || mVelZ > 4.712389f) {
            return true;
        }
    }

    if (mZombieType == ZombieType::ZOMBIE_DIGGER) {
        if (mZombiePhase == ZombiePhase::PHASE_DIGGER_RISING || mZombiePhase == ZombiePhase::PHASE_DIGGER_STUNNED || mZombiePhase == ZombiePhase::PHASE_DIGGER_WALKING) {
            return true;
        } else if (mZombiePhase == ZombiePhase::PHASE_ZOMBIE_DYING || mZombiePhase == ZombiePhase::PHASE_ZOMBIE_BURNED || mZombiePhase == ZombiePhase::PHASE_ZOMBIE_MOWERED) {
            return mHasObject;
        }

        return false;
    }

    return mZombieType == ZombieType::ZOMBIE_YETI && !mHasObject;
}

void Zombie::SetZombatarReanim() {
    PlayerInfo *aPlayerInfo = mApp->mPlayerInfo;
    if (!aPlayerInfo->mZombatarEnabled)
        return;
    Reanimation *aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    ReanimatorTrackInstance *aHeadTrackInstance = aBodyReanim->GetTrackInstanceByName("anim_head1");
    aHeadTrackInstance->mImageOverride = *IMAGE_BLANK;
    Reanimation *aZombatarHeadReanim = mApp->AddReanimation(0, 0, 0, ReanimationType::REANIM_ZOMBATAR_HEAD);
    Reanimation_SetZombatarHats(aZombatarHeadReanim, aPlayerInfo->mZombatarHat, aPlayerInfo->mZombatarHatColor);
    Reanimation_SetZombatarHair(aZombatarHeadReanim, aPlayerInfo->mZombatarHair, aPlayerInfo->mZombatarHairColor);
    Reanimation_SetZombatarFHair(aZombatarHeadReanim, aPlayerInfo->mZombatarFacialHair, aPlayerInfo->mZombatarFacialHairColor);
    Reanimation_SetZombatarAccessories(aZombatarHeadReanim, aPlayerInfo->mZombatarAccessory, aPlayerInfo->mZombatarAccessoryColor);
    Reanimation_SetZombatarEyeWear(aZombatarHeadReanim, aPlayerInfo->mZombatarEyeWear, aPlayerInfo->mZombatarEyeWearColor);
    Reanimation_SetZombatarTidBits(aZombatarHeadReanim, aPlayerInfo->mZombatarTidBit, aPlayerInfo->mZombatarTidBitColor);
    aZombatarHeadReanim->PlayReanim("anim_head_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0);
    aZombatarHeadReanim->AssignRenderGroupToTrack("anim_hair", -1);
    mBossFireBallReanimID = mApp->ReanimationGetID(aZombatarHeadReanim);
    AttachEffect *attachEffect = AttachReanim(aHeadTrackInstance->mAttachmentID, aZombatarHeadReanim, 0.0f, 0.0f);
    TodScaleRotateTransformMatrix((SexyMatrix3 &)attachEffect->mOffset, -20.0, -1.0, 0.2, 1.0, 1.0);
    ReanimShowPrefix("anim_hair", -1);
    ReanimShowPrefix("anim_head2", -1);
}

bool Zombie::IsZombatarZombie(ZombieType theType) {
    // return type == ZombieType::ZOMBIE_FLAG || type == ZombieType::ZOMBIE_NORMAL || type == ZombieType::ZOMBIE_TRAFFIC_CONE || type == ZombieType::ZOMBIE_DOOR || type == ZombieType::ZOMBIE_TRASH_BIN
    // || type == ZombieType::ZOMBIE_PAIL || type
    // == ZombieType::ZOMBIE_DUCKY_TUBE;
    return theType == ZombieType::ZOMBIE_FLAG;
}

void Zombie::DieNoLoot() {
    if (mZombieType == ZombieType::ZOMBIE_GARGANTUAR && mBoard != nullptr && mApp->mGameScene == GameScenes::SCENE_PLAYING) {
        mBoard->GrantAchievement(AchievementId::ACHIEVEMENT_GARG, true);
    }

    if (IsZombatarZombie(mZombieType)) {
        // 大头贴
        mApp->RemoveReanimation(mBossFireBallReanimID);
    }
    old_Zombie_DieNoLoot(this);
}

void Zombie::DrawBungeeCord(Sexy::Graphics *g, int theOffsetX, int theOffsetY) {
    // 修复在Boss关的蹦极绳子不绑在Boss手上
    int aCordCelHeight = (*Sexy_IMAGE_BUNGEECORD_Addr)->GetCelHeight() * mScaleZombie;
    float aPosX = 0.0f;
    float aPosY = 0.0f;
    GetTrackPosition("Zombie_bungi_body", aPosX, aPosY);
    bool aSetClip = false;
    if (IsOnBoard() && mApp->IsFinalBossLevel()) {
        Zombie *aBossZombie = mBoard->GetBossZombie();
        int aClipAmount = 55;
        if (aBossZombie->mZombiePhase == ZombiePhase::PHASE_BOSS_BUNGEES_LEAVE) {
            Reanimation *reanimation = mApp->ReanimationGet(aBossZombie->mBodyReanimID);
            aClipAmount = TodAnimateCurveFloatTime(0.0f, 0.2f, reanimation->mAnimTime, 55.0f, 0.0f, TodCurves::CURVE_LINEAR);
        }
        if (mTargetCol >= aBossZombie->mTargetCol) { // ">" ------ > ">="，修复第一根手指蹦极不绑在手上
            if (mTargetCol > aBossZombie->mTargetCol) {
                aClipAmount += 60; // 55 ---- > 115，修复第2、3根手指蹦极不绑在手上
            }
            g->SetClipRect(-g->mTransX, aClipAmount - g->mTransY, 800, 600);
            aSetClip = true;
        }
    }

    for (float y = aPosY - aCordCelHeight; y > -60 - aCordCelHeight; y -= aCordCelHeight) {
        float thePosX = theOffsetX + 61.0f - 4.0f / mScaleZombie;
        float thePosY = y - mPosY;
        TodDrawImageScaledF(g, *Sexy_IMAGE_BUNGEECORD_Addr, thePosX, thePosY, mScaleZombie, mScaleZombie);
    }
    if (aSetClip) {
        g->ClearClipRect();
    }
}

void Zombie::GetDrawPos(ZombieDrawPosition &theDrawPos) {
    theDrawPos.mImageOffsetX = mPosX - mX;
    theDrawPos.mImageOffsetY = mPosY - mY;

    if (mIsEating) {
        theDrawPos.mHeadX = 47;
        theDrawPos.mHeadY = 4;
    } else {
        switch (mFrame) {
            case 0:
                theDrawPos.mHeadX = 50;
                theDrawPos.mHeadY = 2;
                break;
            case 1:
                theDrawPos.mHeadX = 49;
                theDrawPos.mHeadY = 1;
                break;
            case 2:
                theDrawPos.mHeadX = 49;
                theDrawPos.mHeadY = 2;
                break;
            case 3:
                theDrawPos.mHeadX = 48;
                theDrawPos.mHeadY = 4;
                break;
            case 4:
                theDrawPos.mHeadX = 48;
                theDrawPos.mHeadY = 5;
                break;
            case 5:
                theDrawPos.mHeadX = 48;
                theDrawPos.mHeadY = 4;
                break;
            case 6:
                theDrawPos.mHeadX = 48;
                theDrawPos.mHeadY = 2;
                break;
            case 7:
                theDrawPos.mHeadX = 49;
                theDrawPos.mHeadY = 1;
                break;
            case 8:
                theDrawPos.mHeadX = 49;
                theDrawPos.mHeadY = 2;
                break;
            case 9:
                theDrawPos.mHeadX = 50;
                theDrawPos.mHeadY = 4;
                break;
            case 10:
                theDrawPos.mHeadX = 50;
                theDrawPos.mHeadY = 5;
                break;
            default:
                theDrawPos.mHeadX = 50;
                theDrawPos.mHeadY = 4;
                break;
        }
    }

    theDrawPos.mArmY = theDrawPos.mHeadY / 2;

    switch (mZombieType) {
        case ZombieType::ZOMBIE_FOOTBALL:
            theDrawPos.mImageOffsetY -= 16.0f;
            break;
        case ZombieType::ZOMBIE_YETI:
            theDrawPos.mImageOffsetY -= 20.0f;
            break;
        case ZombieType::ZOMBIE_CATAPULT:
            theDrawPos.mImageOffsetX -= 25.0f;
            theDrawPos.mImageOffsetY -= 18.0f;
            break;
        case ZombieType::ZOMBIE_POGO:
            theDrawPos.mImageOffsetY += 16.0f;
            break;
        case ZombieType::ZOMBIE_BALLOON:
            theDrawPos.mImageOffsetY += 17.0f;
            break;
        case ZombieType::ZOMBIE_POLEVAULTER:
            theDrawPos.mImageOffsetX -= 6.0f;
            theDrawPos.mImageOffsetY -= 11.0f;
            break;
        case ZombieType::ZOMBIE_ZAMBONI:
            theDrawPos.mImageOffsetX += 68.0f;
            theDrawPos.mImageOffsetY -= 23.0f;
            break;
        case ZombieType::ZOMBIE_GARGANTUAR:
        case ZombieType::ZOMBIE_REDEYE_GARGANTUAR:
            theDrawPos.mImageOffsetY -= 8.0f;
            break;
        case ZombieType::ZOMBIE_BOBSLED:
            theDrawPos.mImageOffsetY -= 12.0f;
            break;
    }

    if (mZombiePhase == ZombiePhase::PHASE_RISING_FROM_GRAVE) {
        theDrawPos.mBodyY = -mAltitude;

        if (mInPool) {
            theDrawPos.mClipHeight = theDrawPos.mBodyY;
        } else {
            float aHeightLimit = min(mPhaseCounter, 40);
            theDrawPos.mClipHeight = theDrawPos.mBodyY + aHeightLimit;
        }

        if (IsOnHighGround()) {
            theDrawPos.mBodyY -= HIGH_GROUND_HEIGHT;
        }

        return;
    }

    if (mZombieType == ZombieType::ZOMBIE_DOLPHIN_RIDER) {
        theDrawPos.mBodyY = -mAltitude;
        theDrawPos.mClipHeight = CLIP_HEIGHT_OFF;

        if (mZombiePhase == ZombiePhase::PHASE_DOLPHIN_INTO_POOL) {
            Reanimation *aBodyReanim = mApp->ReanimationGet(mBodyReanimID);

            if (aBodyReanim->mAnimTime >= 0.56f && aBodyReanim->mAnimTime <= 0.65f) // 跳上海豚的起跳过程
            {
                theDrawPos.mClipHeight = 0.0f;
            } else if (aBodyReanim->mAnimTime >= 0.75f) // 跳上海豚的下落过程
            {
                theDrawPos.mClipHeight = -mAltitude - 10.0f;
            }
        } else if (mZombiePhase == ZombiePhase::PHASE_DOLPHIN_RIDING) {
            theDrawPos.mImageOffsetX += 70.0f; // 额外 70 像素的横坐标偏移用于弥补跳上海豚后的 mPosX -= 70.0f

            if (mZombieHeight == ZombieHeight::HEIGHT_DRAGGED_UNDER) {
                theDrawPos.mClipHeight = -mAltitude - 15.0f;
            } else {
                theDrawPos.mClipHeight = -mAltitude - 10.0f;
            }
        } else if (mZombiePhase == ZombiePhase::PHASE_DOLPHIN_IN_JUMP) {
            theDrawPos.mImageOffsetX += 70.0f + mAltitude;

            Reanimation *aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
            if (aBodyReanim->mAnimTime <= 0.06f) // 起跳出水之前
            {
                theDrawPos.mClipHeight = -mAltitude - 10.0f;
            } else if (aBodyReanim->mAnimTime >= 0.5f && aBodyReanim->mAnimTime <= 0.76f) // 起跳过程中（脱离水面后至重新入水前）
            {
                theDrawPos.mClipHeight = -13.0f;
            }
        } else if (mZombiePhase == ZombiePhase::PHASE_DOLPHIN_WALKING_IN_POOL || mZombiePhase == ZombiePhase::PHASE_ZOMBIE_DYING) {
            theDrawPos.mImageOffsetY += 50.0f; // 额外 50 像素的横坐标偏移用于弥补跳跃过程中前进的距离

            if (mZombiePhase == ZombiePhase::PHASE_ZOMBIE_DYING) {
                theDrawPos.mClipHeight = -mAltitude + 44.0f;
            } else if (mZombieHeight == ZombieHeight::HEIGHT_DRAGGED_UNDER) {
                theDrawPos.mClipHeight = -mAltitude + 36.0f;
            }
        } else if (mZombiePhase == ZombiePhase::PHASE_DOLPHIN_WALKING && mZombieHeight == ZombieHeight::HEIGHT_OUT_OF_POOL) {
            theDrawPos.mClipHeight = -mAltitude;
        } else if (mZombiePhase == ZombiePhase::PHASE_DOLPHIN_WALKING_WITHOUT_DOLPHIN && mZombieHeight == ZombieHeight::HEIGHT_OUT_OF_POOL) {
            theDrawPos.mClipHeight = -mAltitude;
        }
    } else if (mZombieType == ZombieType::ZOMBIE_SNORKEL) {
        theDrawPos.mBodyY = -mAltitude;
        theDrawPos.mClipHeight = CLIP_HEIGHT_OFF;

        if (mZombiePhase == ZombiePhase::PHASE_SNORKEL_INTO_POOL) {
            Reanimation *aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
            if (aBodyReanim->mAnimTime >= 0.8f) // 入水后
            {
                theDrawPos.mClipHeight = -10.0f;
            }
        } else if (mInPool) {
            theDrawPos.mClipHeight = -mAltitude - 5.0f;
            theDrawPos.mClipHeight += 20.0f - 20.0f * mScaleZombie;
        }
    } else if (mInPool) {
        theDrawPos.mBodyY = -mAltitude;
        theDrawPos.mClipHeight = -mAltitude - 7.0f;
        theDrawPos.mClipHeight += 10.0f - 10.0f * mScaleZombie;

        if (mIsEating) {
            theDrawPos.mClipHeight += 7.0f;
        }
    } else if (mZombiePhase == ZombiePhase::PHASE_DANCER_RISING) {
        theDrawPos.mBodyY = -mAltitude;
        theDrawPos.mClipHeight = -mAltitude;

        if (IsOnHighGround()) {
            theDrawPos.mBodyY -= HIGH_GROUND_HEIGHT;
        }
    } else if (mZombiePhase == ZombiePhase::PHASE_DIGGER_RISING || mZombiePhase == ZombiePhase::PHASE_DIGGER_RISE_WITHOUT_AXE) {
        theDrawPos.mBodyY = -mAltitude;

        if (mPhaseCounter > 20) {
            theDrawPos.mClipHeight = -mAltitude;
        } else {
            theDrawPos.mClipHeight = CLIP_HEIGHT_OFF;
        }
    } else if (mZombieType == ZombieType::ZOMBIE_BUNGEE) {
        theDrawPos.mBodyY = -mAltitude;
        theDrawPos.mImageOffsetX -= 18.0f;

        if (IsOnHighGround()) {
            theDrawPos.mBodyY -= HIGH_GROUND_HEIGHT;
        }

        theDrawPos.mClipHeight = CLIP_HEIGHT_OFF;
    } else {
        theDrawPos.mBodyY = -mAltitude;
        theDrawPos.mClipHeight = CLIP_HEIGHT_OFF;
    }
}

bool Zombie::IsOnHighGround() {
    return IsOnBoard() && mBoard->mGridSquareType[mBoard->PixelToGridXKeepOnBoard(mX + 75, mY)][mRow] == GridSquareType::GRIDSQUARE_HIGH_GROUND;
}

bool Zombie::IsTangleKelpTarget() {
    // 修复水草拉僵尸有概率失效

    if (!mBoard->StageHasPool()) {
        return false;
    }
    if (mZombieHeight == ZombieHeight::HEIGHT_DRAGGED_UNDER) {
        return true;
    }
    Plant *aPlant = nullptr;
    while (mBoard->IteratePlants(aPlant)) {
        if (!aPlant->mDead && aPlant->mSeedType == SeedType::SEED_TANGLEKELP && aPlant->mTargetZombieID == mZombieID) {
            return true;
        }
    }
    return false;
}

void Zombie::DrawReanim(Sexy::Graphics *graphics, ZombieDrawPosition *theZombieDrawPosition, int theBaseRenderGroup) {
    // 大头贴专门Draw一下
    old_Zombie_DrawReanim(this, graphics, theZombieDrawPosition, theBaseRenderGroup);
    if (IsZombatarZombie(mZombieType)) {
        Reanimation *reanimation = mApp->ReanimationTryToGet(mBossFireBallReanimID);
        if (reanimation != nullptr) {
            reanimation->Draw(graphics);
        }
    }
}


void Zombie::DropHead(unsigned int theDamageFlags) {
    // 负责 大头贴掉头
    // TODO: 大头贴僵尸掉头时掉饰品(掉 hat 和 eyeWear)
    old_Zombie_DropHead(this, theDamageFlags);
    if (IsZombatarZombie(mZombieType)) {
        Reanimation *reanimation = mApp->ReanimationTryToGet(mBossFireBallReanimID);
        if (reanimation != nullptr) {
            int index[2] = {Reanimation_GetZombatarHatTrackIndex(reanimation), Reanimation_GetZombatarEyeWearTrackIndex(reanimation)};
            for (int i = 0; i < 2; ++i) {
                if (index[i] == -1)
                    continue;
                ReanimatorTrackInstance *reanimatorTrackInstance = reanimation->mTrackInstances + index[i];
                ReanimatorTrack *reanimatorTrack = reanimation->mDefinition->mTracks + index[i];
                SexyTransform2D aSexyTransform2D;
                reanimation->GetTrackMatrix(index[i], aSexyTransform2D);
                float aPosX = mPosX + aSexyTransform2D.m[0][2];
                float aPosY = mPosY + aSexyTransform2D.m[1][2];
                TodParticleSystem *todParticleSystem = mApp->AddTodParticle(aPosX, aPosY, mRenderOrder + 1, ParticleEffect::PARTICLE_ZOMBIE_HEAD);
                TodParticleSystem_OverrideColor(todParticleSystem, nullptr, &reanimatorTrackInstance->mTrackColor);
                TodParticleSystem_OverrideImage(todParticleSystem, nullptr, reanimatorTrack->mTransforms[0].mImage);
            }
            mApp->RemoveReanimation(mBossFireBallReanimID);
            mBossFireBallReanimID = ReanimationID::REANIMATIONID_NULL;
        }
    }
}

int Zombie::GetHelmDamageIndex() {
    if (mHelmHealth < mHelmMaxHealth / 3) {
        return 2;
    }

    if (mHelmHealth < mHelmMaxHealth * 2 / 3) {
        return 1;
    }

    return 0;
}

int Zombie::GetBodyDamageIndex() {
    if (mZombieType == ZombieType::ZOMBIE_BOSS) {
        if (mBodyHealth < mBodyMaxHealth / 2) {
            return 2;
        }

        if (mBodyHealth < mBodyMaxHealth * 4 / 5) {
            return 1;
        }

        return 0;
    } else {
        if (mBodyHealth < mBodyMaxHealth / 3) {
            return 2;
        }

        if (mBodyHealth < mBodyMaxHealth * 2 / 3) {
            return 1;
        }

        return 0;
    }
}

int Zombie::GetShieldDamageIndex() {
    if (mShieldHealth < mShieldMaxHealth / 3) {
        return 2;
    }

    if (mShieldHealth < mShieldMaxHealth * 2 / 3) {
        return 1;
    }

    return 0;
}

bool Zombie::IsFireResistant() {
    return mZombieType == ZombieType::ZOMBIE_CATAPULT || mZombieType == ZombieType::ZOMBIE_ZAMBONI || mShieldType == ShieldType::SHIELDTYPE_DOOR || mShieldType == ShieldType::SHIELDTYPE_LADDER
        || mShieldType == ShieldType::SHIELDTYPE_TRASH_BIN;
}

Plant *Zombie::FindPlantTarget(ZombieAttackType theAttackType) {
    return old_Zombie_FindPlantTarget(this, theAttackType);
}

Zombie *Zombie::FindZombieTarget() {
    return old_Zombie_FindZombieTarget(this);
}

void Zombie::TakeDamage(int theDamage, unsigned int theDamageFlags) {
    return old_Zombie_TakeDamage(this, theDamage, theDamageFlags);
}

void Zombie::PlayZombieReanim(const char *theTrackName, ReanimLoopType theLoopType, int theBlendTime, float theAnimRate) {
    return old_Zombie_PlayZombieReanim(this, theTrackName, theLoopType, theBlendTime, theAnimRate);
}

void Zombie::StartWalkAnim(int theBlendTime) {
    return old_Zombie_StartWalkAnim(this, theBlendTime);
}

void Zombie::ReanimShowPrefix(const char *theTrackPrefix, int theRenderGroup) {
    return old_Zombie_ReanimShowPrefix(this, theTrackPrefix, theRenderGroup);
}

void Zombie::ReanimShowTrack(const char *theTrackName, int theRenderGroup) {
    return old_Zombie_ReanimShowTrack(this, theTrackName, theRenderGroup);
}

float Zombie::GetPosYBasedOnRow(int theRow) {
    return old_Zombie_GetPosYBasedOnRow(this, theRow);
}

void Zombie::SetRow(int theRow) {
    return old_Zombie_SetRow(this, theRow);
}

void Zombie::StartMindControlled() {
    return old_Zombie_StartMindControlled(this);
}

void Zombie::UpdateReanim() {
    return old_Zombie_UpdateReanim(this);
}

bool Zombie::IsImmobilizied() {
    return mIceTrapCounter > 0 || mButteredCounter > 0;
}

void Zombie::SetupLostArmReanim() {
    switch (mZombieType) {
        case ZombieType::ZOMBIE_FOOTBALL:
            ReanimShowPrefix("Zombie_football_leftarm_lower", -1);
            ReanimShowPrefix("Zombie_football_leftarm_hand", -1);
            break;
        case ZombieType::ZOMBIE_NEWSPAPER:
            ReanimShowTrack("Zombie_paper_hands", -1);
            ReanimShowTrack("Zombie_paper_leftarm_lower", -1);
            break;
        case ZombieType::ZOMBIE_POLEVAULTER:
            ReanimShowTrack("Zombie_polevaulter_outerarm_lower", -1);
            ReanimShowTrack("Zombie_outerarm_hand", -1);
            break;
        case ZombieType::ZOMBIE_DANCER:
            ReanimShowPrefix("Zombie_disco_outerarm_lower", -1);
            ReanimShowPrefix("Zombie_disco_outerhand_point", -1);
            ReanimShowPrefix("Zombie_disco_outerhand", -1);
            ReanimShowPrefix("Zombie_disco_outerarm_upper", -1);
            break;
        case ZombieType::ZOMBIE_BACKUP_DANCER:
            ReanimShowPrefix("Zombie_disco_outerarm_lower", -1);
            ReanimShowPrefix("Zombie_disco_outerhand", -1);
            break;
        default:
            ReanimShowPrefix("Zombie_outerarm_lower", -1);
            ReanimShowPrefix("Zombie_outerarm_hand", -1);
            break;
    }
    Reanimation *reanimation = mApp->ReanimationTryToGet(mBodyReanimID);
    if (reanimation != nullptr) {
        switch (mZombieType) {
            case ZombieType::ZOMBIE_FOOTBALL:
                reanimation->SetImageOverride("zombie_football_leftarm_hand", *Sexy_IMAGE_REANIM_ZOMBIE_FOOTBALL_LEFTARM_UPPER2_Addr);
                break;
            case ZombieType::ZOMBIE_NEWSPAPER:
                reanimation->SetImageOverride("Zombie_paper_leftarm_lower", *Sexy_IMAGE_REANIM_ZOMBIE_PAPER_LEFTARM_UPPER2_Addr);
                break;
            case ZombieType::ZOMBIE_POLEVAULTER:
                reanimation->SetImageOverride("Zombie_polevaulter_outerarm_lower", *Sexy_IMAGE_REANIM_ZOMBIE_POLEVAULTER_OUTERARM_UPPER2_Addr);
                break;
            case ZombieType::ZOMBIE_BALLOON:
                reanimation->SetImageOverride("zombie_outerarm_lower", *Sexy_IMAGE_REANIM_ZOMBIE_BALLOON_OUTERARM_UPPER2_Addr);
                break;
            case ZombieType::ZOMBIE_IMP:
                reanimation->SetImageOverride("Zombie_imp_outerarm_upper", *Sexy_IMAGE_REANIM_ZOMBIE_IMP_ARM1_BONE_Addr);
                break;
            case ZombieType::ZOMBIE_DIGGER:
                reanimation->SetImageOverride("Zombie_digger_outerarm_upper", *Sexy_IMAGE_REANIM_ZOMBIE_DIGGER_OUTERARM_UPPER2_Addr);
                break;
            case ZombieType::ZOMBIE_BOBSLED:
                reanimation->SetImageOverride("Zombie_dolphinrider_outerarm_upper", *Sexy_IMAGE_REANIM_ZOMBIE_BOBSLED_OUTERARM_UPPER2_Addr);
                break;
            case ZombieType::ZOMBIE_JACK_IN_THE_BOX:
                reanimation->SetImageOverride("Zombie_jackbox_outerarm_lower", *Sexy_IMAGE_REANIM_ZOMBIE_JACKBOX_OUTERARM_LOWER2_Addr);
                break;
            case ZombieType::ZOMBIE_SNORKEL:
                reanimation->SetImageOverride("Zombie_snorkle_outerarm_upper", *Sexy_IMAGE_REANIM_ZOMBIE_SNORKLE_OUTERARM_UPPER2_Addr);
                break;
            case ZombieType::ZOMBIE_DOLPHIN_RIDER:
                reanimation->SetImageOverride("Zombie_dolphinrider_outerarm_upper", *Sexy_IMAGE_REANIM_ZOMBIE_DOLPHINRIDER_OUTERARM_UPPER2_Addr);
                break;
            case ZombieType::ZOMBIE_POGO:
                reanimation->SetImageOverride("Zombie_outerarm_upper", *Sexy_IMAGE_REANIM_ZOMBIE_POGO_OUTERARM_UPPER2_Addr);
                reanimation->SetImageOverride("Zombie_pogo_stickhands", *Sexy_IMAGE_REANIM_ZOMBIE_POGO_STICKHANDS2_Addr);
                reanimation->SetImageOverride("Zombie_pogo_stick", *Sexy_IMAGE_REANIM_ZOMBIE_POGO_STICKDAMAGE2_Addr);
                reanimation->SetImageOverride("Zombie_pogo_stick2", *Sexy_IMAGE_REANIM_ZOMBIE_POGO_STICK2DAMAGE2_Addr);
                break;
            case ZombieType::ZOMBIE_FLAG: {
                reanimation->SetImageOverride("Zombie_outerarm_upper", *Sexy_IMAGE_REANIM_ZOMBIE_OUTERARM_UPPER2_Addr);
                Reanimation *reanimation2 = mApp->ReanimationTryToGet(mSpecialHeadReanimID);
                if (reanimation2 != nullptr) {
                    reanimation2->SetImageOverride("Zombie_flag", *Sexy_IMAGE_REANIM_ZOMBIE_FLAG3_Addr);
                }
                break;
            }
            case ZombieType::ZOMBIE_DANCER:
                reanimation->SetImageOverride("Zombie_disco_outerarm_upper", *Sexy_IMAGE_REANIM_ZOMBIE_DISCO_OUTERARM_UPPER2_Addr);
                break;
            case ZombieType::ZOMBIE_BACKUP_DANCER:
                reanimation->SetImageOverride( "Zombie_disco_outerarm_upper", *Sexy_IMAGE_REANIM_ZOMBIE_BACKUP_OUTERARM_UPPER2_Addr);
                break;
            case ZombieType::ZOMBIE_LADDER:
                reanimation->SetImageOverride("Zombie_ladder_outerarm_upper", *Sexy_IMAGE_REANIM_ZOMBIE_LADDER_OUTERARM_UPPER2_Addr);
                break;
            case ZombieType::ZOMBIE_YETI:
                reanimation->SetImageOverride("Zombie_yeti_outerarm_upper", *Sexy_IMAGE_REANIM_ZOMBIE_YETI_OUTERARM_UPPER2_Addr);
                break;
            default:
                reanimation->SetImageOverride("Zombie_outerarm_upper", *Sexy_IMAGE_REANIM_ZOMBIE_OUTERARM_UPPER2_Addr);
                break;
        }
    }
}

void Zombie::PickRandomSpeed() {
    if (mZombiePhase == ZombiePhase::PHASE_DOLPHIN_WALKING_IN_POOL) {
        mVelX = 0.3f;
    } else if (mZombiePhase == ZombiePhase::PHASE_DIGGER_WALKING) // 矿工行走
    {
        if (mApp->IsIZombieLevel()) {
            mVelX = 0.23f; // IZ模式
        } else {
            mVelX = 0.12f; // 一般模式
        }
    } else if (mZombieType == ZombieType::ZOMBIE_IMP && mApp->IsIZombieLevel()) // IZ小鬼
    {
        mVelX = 0.9f;
    } else if (mZombiePhase == ZombiePhase::PHASE_YETI_RUNNING) {
        mVelX = 0.8f;
    } else if (mZombieType == ZombieType::ZOMBIE_YETI) {
        mVelX = 0.4f;
    } else if (mZombieType == ZombieType::ZOMBIE_DANCER || mZombieType == ZombieType::ZOMBIE_BACKUP_DANCER || mZombieType == ZombieType::ZOMBIE_POGO || mZombieType == ZombieType::ZOMBIE_FLAG) {
        mVelX = 0.45f;
    } else if (mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING || mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT || mZombieType == ZombieType::ZOMBIE_FOOTBALL
               || mZombieType == ZombieType::ZOMBIE_SNORKEL || mZombieType == ZombieType::ZOMBIE_JACK_IN_THE_BOX) {
        mVelX = RandRangeFloat(0.66f, 0.68f);
    } else if (mZombiePhase == ZombiePhase::PHASE_LADDER_CARRYING || mZombieType == ZombieType::ZOMBIE_SQUASH_HEAD) {
        mVelX = RandRangeFloat(0.79f, 0.81f);
    } else if (mZombiePhase == ZombiePhase::PHASE_NEWSPAPER_MAD || mZombiePhase == ZombiePhase::PHASE_DOLPHIN_WALKING || mZombiePhase == ZombiePhase::PHASE_DOLPHIN_WALKING_WITHOUT_DOLPHIN) {
        mVelX = RandRangeFloat(0.89f, 0.91f);
    } else {
        mVelX = RandRangeFloat(0.23f, 0.32f); // 普僵
        if (mVelX < 0.3f) {
            mAnimTicksPerFrame = 12;
        } else {
            mAnimTicksPerFrame = 15;
        }
    }

    UpdateAnimSpeed();
}

