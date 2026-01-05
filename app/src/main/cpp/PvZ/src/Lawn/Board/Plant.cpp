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

#include "PvZ/Lawn/Board/Plant.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Board/Challenge.h"
#include "PvZ/Lawn/Board/CutScene.h"
#include "PvZ/Lawn/Board/GridItem.h"
#include "PvZ/Lawn/GamepadControls.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/System/ReanimationLawn.h"
#include "PvZ/Lawn/Widget/WaitForSecondPlayerDialog.h"
#include "PvZ/MagicAddr.h"
#include "PvZ/Misc.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/SexyAppFramework/Misc/SexyVector.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Effect/Reanimator.h"
#include "PvZ/TodLib/Effect/TodParticle.h"

#include <algorithm>

using namespace Sexy;

PlantDefinition gPlantDefs[SeedType::NUM_SEED_TYPES] = {
    {SeedType::SEED_PEASHOOTER, nullptr, ReanimationType::REANIM_PEASHOOTER, 0, 100, 750, PlantSubClass::SUBCLASS_SHOOTER, 150, "PEASHOOTER"},
    {SeedType::SEED_SUNFLOWER, nullptr, ReanimationType::REANIM_SUNFLOWER, 1, 50, 750, PlantSubClass::SUBCLASS_NORMAL, 2500, "SUNFLOWER"},
    {SeedType::SEED_CHERRYBOMB, nullptr, ReanimationType::REANIM_CHERRYBOMB, 3, 150, 5000, PlantSubClass::SUBCLASS_NORMAL, 0, "CHERRY_BOMB"},
    {SeedType::SEED_WALLNUT, nullptr, ReanimationType::REANIM_WALLNUT, 2, 50, 3000, PlantSubClass::SUBCLASS_NORMAL, 0, "WALL_NUT"},
    {SeedType::SEED_POTATOMINE, nullptr, ReanimationType::REANIM_POTATOMINE, 37, 25, 3000, PlantSubClass::SUBCLASS_NORMAL, 0, "POTATO_MINE"},
    {SeedType::SEED_SNOWPEA, nullptr, ReanimationType::REANIM_SNOWPEA, 4, 175, 750, PlantSubClass::SUBCLASS_SHOOTER, 150, "SNOW_PEA"},
    {SeedType::SEED_CHOMPER, nullptr, ReanimationType::REANIM_CHOMPER, 31, 150, 750, PlantSubClass::SUBCLASS_NORMAL, 0, "CHOMPER"},
    {SeedType::SEED_REPEATER, nullptr, ReanimationType::REANIM_REPEATER, 5, 200, 750, PlantSubClass::SUBCLASS_SHOOTER, 150, "REPEATER"},
    {SeedType::SEED_PUFFSHROOM, nullptr, ReanimationType::REANIM_PUFFSHROOM, 6, 0, 750, PlantSubClass::SUBCLASS_SHOOTER, 150, "PUFF_SHROOM"},
    {SeedType::SEED_SUNSHROOM, nullptr, ReanimationType::REANIM_SUNSHROOM, 7, 25, 750, PlantSubClass::SUBCLASS_NORMAL, 2500, "SUN_SHROOM"},
    {SeedType::SEED_FUMESHROOM, nullptr, ReanimationType::REANIM_FUMESHROOM, 9, 75, 750, PlantSubClass::SUBCLASS_SHOOTER, 150, "FUME_SHROOM"},
    {SeedType::SEED_GRAVEBUSTER, nullptr, ReanimationType::REANIM_GRAVE_BUSTER, 40, 75, 750, PlantSubClass::SUBCLASS_NORMAL, 0, "GRAVE_BUSTER"},
    {SeedType::SEED_HYPNOSHROOM, nullptr, ReanimationType::REANIM_HYPNOSHROOM, 10, 75, 3000, PlantSubClass::SUBCLASS_NORMAL, 0, "HYPNO_SHROOM"},
    {SeedType::SEED_SCAREDYSHROOM, nullptr, ReanimationType::REANIM_SCRAREYSHROOM, 33, 25, 750, PlantSubClass::SUBCLASS_SHOOTER, 150, "SCAREDY_SHROOM"},
    {SeedType::SEED_ICESHROOM, nullptr, ReanimationType::REANIM_ICESHROOM, 36, 75, 5000, PlantSubClass::SUBCLASS_NORMAL, 0, "ICE_SHROOM"},
    {SeedType::SEED_DOOMSHROOM, nullptr, ReanimationType::REANIM_DOOMSHROOM, 20, 125, 5000, PlantSubClass::SUBCLASS_NORMAL, 0, "DOOM_SHROOM"},
    {SeedType::SEED_LILYPAD, nullptr, ReanimationType::REANIM_LILYPAD, 19, 25, 750, PlantSubClass::SUBCLASS_NORMAL, 0, "LILY_PAD"},
    {SeedType::SEED_SQUASH, nullptr, ReanimationType::REANIM_SQUASH, 21, 50, 3000, PlantSubClass::SUBCLASS_NORMAL, 0, "SQUASH"},
    {SeedType::SEED_THREEPEATER, nullptr, ReanimationType::REANIM_THREEPEATER, 12, 325, 750, PlantSubClass::SUBCLASS_SHOOTER, 150, "THREEPEATER"},
    {SeedType::SEED_TANGLEKELP, nullptr, ReanimationType::REANIM_TANGLEKELP, 17, 25, 3000, PlantSubClass::SUBCLASS_NORMAL, 0, "TANGLE_KELP"},
    {SeedType::SEED_JALAPENO, nullptr, ReanimationType::REANIM_JALAPENO, 11, 125, 5000, PlantSubClass::SUBCLASS_NORMAL, 0, "JALAPENO"},
    {SeedType::SEED_SPIKEWEED, nullptr, ReanimationType::REANIM_SPIKEWEED, 22, 100, 750, PlantSubClass::SUBCLASS_NORMAL, 0, "SPIKEWEED"},
    {SeedType::SEED_TORCHWOOD, nullptr, ReanimationType::REANIM_TORCHWOOD, 29, 175, 750, PlantSubClass::SUBCLASS_NORMAL, 0, "TORCHWOOD"},
    {SeedType::SEED_TALLNUT, nullptr, ReanimationType::REANIM_TALLNUT, 28, 125, 3000, PlantSubClass::SUBCLASS_NORMAL, 0, "TALL_NUT"},
    {SeedType::SEED_SEASHROOM, nullptr, ReanimationType::REANIM_SEASHROOM, 39, 0, 3000, PlantSubClass::SUBCLASS_SHOOTER, 150, "SEA_SHROOM"},
    {SeedType::SEED_PLANTERN, nullptr, ReanimationType::REANIM_PLANTERN, 38, 25, 3000, PlantSubClass::SUBCLASS_NORMAL, 2500, "PLANTERN"},
    {SeedType::SEED_CACTUS, nullptr, ReanimationType::REANIM_CACTUS, 15, 125, 750, PlantSubClass::SUBCLASS_SHOOTER, 150, "CACTUS"},
    {SeedType::SEED_BLOVER, nullptr, ReanimationType::REANIM_BLOVER, 18, 100, 750, PlantSubClass::SUBCLASS_NORMAL, 0, "BLOVER"},
    {SeedType::SEED_SPLITPEA, nullptr, ReanimationType::REANIM_SPLITPEA, 32, 125, 750, PlantSubClass::SUBCLASS_SHOOTER, 150, "SPLIT_PEA"},
    {SeedType::SEED_STARFRUIT, nullptr, ReanimationType::REANIM_STARFRUIT, 30, 125, 750, PlantSubClass::SUBCLASS_SHOOTER, 150, "STARFRUIT"},
    {SeedType::SEED_PUMPKINSHELL, nullptr, ReanimationType::REANIM_PUMPKIN, 25, 125, 3000, PlantSubClass::SUBCLASS_NORMAL, 0, "PUMPKIN"},
    {SeedType::SEED_MAGNETSHROOM, nullptr, ReanimationType::REANIM_MAGNETSHROOM, 35, 100, 750, PlantSubClass::SUBCLASS_NORMAL, 0, "MAGNET_SHROOM"},
    {SeedType::SEED_CABBAGEPULT, nullptr, ReanimationType::REANIM_CABBAGEPULT, 13, 100, 750, PlantSubClass::SUBCLASS_SHOOTER, 300, "CABBAGE_PULT"},
    {SeedType::SEED_FLOWERPOT, nullptr, ReanimationType::REANIM_FLOWER_POT, 33, 25, 750, PlantSubClass::SUBCLASS_NORMAL, 0, "FLOWER_POT"},
    {SeedType::SEED_KERNELPULT, nullptr, ReanimationType::REANIM_KERNELPULT, 13, 100, 750, PlantSubClass::SUBCLASS_SHOOTER, 300, "KERNEL_PULT"},
    {SeedType::SEED_INSTANT_COFFEE, nullptr, ReanimationType::REANIM_COFFEEBEAN, 33, 75, 750, PlantSubClass::SUBCLASS_NORMAL, 0, "COFFEE_BEAN"},
    {SeedType::SEED_GARLIC, nullptr, ReanimationType::REANIM_GARLIC, 8, 50, 750, PlantSubClass::SUBCLASS_NORMAL, 0, "GARLIC"},
    {SeedType::SEED_UMBRELLA, nullptr, ReanimationType::REANIM_UMBRELLALEAF, 23, 100, 750, PlantSubClass::SUBCLASS_NORMAL, 0, "UMBRELLA_LEAF"},
    {SeedType::SEED_MARIGOLD, nullptr, ReanimationType::REANIM_MARIGOLD, 24, 50, 3000, PlantSubClass::SUBCLASS_NORMAL, 2500, "MARIGOLD"},
    {SeedType::SEED_MELONPULT, nullptr, ReanimationType::REANIM_MELONPULT, 14, 300, 750, PlantSubClass::SUBCLASS_SHOOTER, 300, "MELON_PULT"},
    {SeedType::SEED_GATLINGPEA, nullptr, ReanimationType::REANIM_GATLINGPEA, 5, 250, 5000, PlantSubClass::SUBCLASS_SHOOTER, 150, "GATLING_PEA"},
    {SeedType::SEED_TWINSUNFLOWER, nullptr, ReanimationType::REANIM_TWIN_SUNFLOWER, 1, 150, 5000, PlantSubClass::SUBCLASS_NORMAL, 2500, "TWIN_SUNFLOWER"},
    {SeedType::SEED_GLOOMSHROOM, nullptr, ReanimationType::REANIM_GLOOMSHROOM, 27, 150, 5000, PlantSubClass::SUBCLASS_SHOOTER, 200, "GLOOM_SHROOM"},
    {SeedType::SEED_CATTAIL, nullptr, ReanimationType::REANIM_CATTAIL, 27, 225, 5000, PlantSubClass::SUBCLASS_SHOOTER, 150, "CATTAIL"},
    {SeedType::SEED_WINTERMELON, nullptr, ReanimationType::REANIM_WINTER_MELON, 27, 200, 5000, PlantSubClass::SUBCLASS_SHOOTER, 300, "WINTER_MELON"},
    {SeedType::SEED_GOLD_MAGNET, nullptr, ReanimationType::REANIM_GOLD_MAGNET, 27, 50, 5000, PlantSubClass::SUBCLASS_NORMAL, 0, "GOLD_MAGNET"},
    {SeedType::SEED_SPIKEROCK, nullptr, ReanimationType::REANIM_SPIKEROCK, 27, 125, 5000, PlantSubClass::SUBCLASS_NORMAL, 0, "SPIKEROCK"},
    {SeedType::SEED_COBCANNON, nullptr, ReanimationType::REANIM_COBCANNON, 16, 500, 5000, PlantSubClass::SUBCLASS_NORMAL, 600, "COB_CANNON"},
    {SeedType::SEED_IMITATER, nullptr, ReanimationType::REANIM_IMITATER, 33, 0, 750, PlantSubClass::SUBCLASS_NORMAL, 0, "IMITATER"},
    {SeedType::NUM_SEEDS_IN_CHOOSER, nullptr, ReanimationType::REANIM_NONE, 0, 0, 0, PlantSubClass::SUBCLASS_NORMAL, 0, "NUM_SEEDS_IN_CHOOSER"},
    {SeedType::SEED_EXPLODE_O_NUT, nullptr, ReanimationType::REANIM_WALLNUT, 2, 0, 3000, PlantSubClass::SUBCLASS_NORMAL, 0, "EXPLODE_O_NUT"},
    {SeedType::SEED_GIANT_WALLNUT, nullptr, ReanimationType::REANIM_WALLNUT, 2, 0, 3000, PlantSubClass::SUBCLASS_NORMAL, 0, "GIANT_WALLNUT"},
    {SeedType::SEED_SPROUT, nullptr, ReanimationType::REANIM_ZENGARDEN_SPROUT, 33, 0, 3000, PlantSubClass::SUBCLASS_NORMAL, 0, "SPROUT"},
    {SeedType::SEED_LEFTPEATER, nullptr, ReanimationType::REANIM_REPEATER, 5, 200, 750, PlantSubClass::SUBCLASS_SHOOTER, 150, "REPEATER"}};

void Plant::PlantInitialize(int theGridX, int theGridY, SeedType theSeedType, SeedType theImitaterType, int a6) {
    // 在初始化植物后更新一次动画，以解决开场前存在的植物只绘制阴影而不绘制植物本体的问题
    old_Plant_PlantInitialize(this, theGridX, theGridY, theSeedType, theImitaterType, a6);
    UpdateReanim();

    if (mApp->IsVSMode()) {
        //        if (mLaunchRate > 0) {
        //            if (MakesSun())
        //                mLaunchCounter = RandRangeInt(300, mLaunchRate / 2);
        //            else
        //                mLaunchCounter = RandRangeInt(0, mLaunchRate);
        //        } else
        //            mLaunchCounter = 0;

        if (tcpClientSocket >= 0) {
            U16U16_Event event = {{EventType::EVENT_SERVER_BOARD_PLANT_LAUNCHCOUNTER}, uint16_t(mBoard->mPlants.DataArrayGetID(this)), uint16_t(mLaunchCounter)};
            send(tcpClientSocket, &event, sizeof(U16U16_Event), 0);
        }
    }

    // 在对战模式修改指定植物的血量
    if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS) {
        switch (theSeedType) {
            case SeedType::SEED_SUNFLOWER:
            case SeedType::SEED_PEASHOOTER:
                mPlantMaxHealth = 300;
                break;
            default:
                break;
        }
        mPlantHealth = mPlantMaxHealth;
    }
}

void Plant::SetSleeping(bool theIsAsleep) {
    if (mushroomsNoSleep) {
        // 如果开启"蘑菇免唤醒"
        theIsAsleep = false;
    }

    old_Plant_SetSleeping(this, theIsAsleep);
}

int Plant::GetDamageRangeFlags(PlantWeapon thePlantWeapon) {
    switch (mSeedType) {
        case SeedType::SEED_CACTUS:
            return thePlantWeapon == PlantWeapon::WEAPON_SECONDARY ? 1 : 2;
        case SeedType::SEED_CHERRYBOMB:
        case SeedType::SEED_JALAPENO:
        case SeedType::SEED_COBCANNON:
        case SeedType::SEED_DOOMSHROOM:
            return 127;
        case SeedType::SEED_MELONPULT:
        case SeedType::SEED_CABBAGEPULT:
        case SeedType::SEED_KERNELPULT:
        case SeedType::SEED_WINTERMELON:
            return 13;
        case SeedType::SEED_POTATOMINE:
            return 77;
        case SeedType::SEED_SQUASH:
            return 13;
        case SeedType::SEED_PUFFSHROOM:
        case SeedType::SEED_SEASHROOM:
        case SeedType::SEED_FUMESHROOM:
        case SeedType::SEED_GLOOMSHROOM:
        case SeedType::SEED_CHOMPER:
            return 9;
        case SeedType::SEED_CATTAIL:
            return 11;
        case SeedType::SEED_TANGLEKELP:
            return 5;
        case SeedType::SEED_GIANT_WALLNUT:
            return 17;
        default:
            return 1;
    }
}

void Plant::PlayBodyReanim(const char *theTrackName, ReanimLoopType theLoopType, int theBlendTime, float theAnimRate) {
    old_Plant_PlayBodyReanim(this, theTrackName, theLoopType, theBlendTime, theAnimRate);
}

void Plant::SpikeweedAttack() {
    if (mState != PlantState::STATE_SPIKEWEED_ATTACKING) {
        PlayBodyReanim("anim_attack", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 18.0f);
        mApp->PlaySample(*Sexy_SOUND_THROW_Addr);

        mState = PlantState::STATE_SPIKEWEED_ATTACKING;
        mStateCountdown = 100;
    }
}

void Plant::SpikeRockTakeDamage() {
    Reanimation *aBodyReanim = mApp->ReanimationGet(mBodyReanimID);

    SpikeweedAttack();

    mPlantHealth -= 50;
    if (mPlantHealth <= 300) {
        aBodyReanim->AssignRenderGroupToTrack("bigspike3", RENDER_GROUP_HIDDEN);
    }
    if (mPlantHealth <= 150) {
        aBodyReanim->AssignRenderGroupToTrack("bigspike2", RENDER_GROUP_HIDDEN);
    }
    if (mPlantHealth <= 0) {
        mApp->PlayFoley(FoleyType::FOLEY_SQUISH);
        Die();
    }
}

bool Plant::IsSpiky() {
    return mSeedType == SeedType::SEED_SPIKEWEED || mSeedType == SeedType::SEED_SPIKEROCK;
}

void Plant::UpdateReanimColor() {
    // 修复玩家选中但不拿起(gameState为1就是选中但不拿起，为7就是选中且拿起)某个紫卡植物时，相应的可升级绿卡植物也会闪烁的BUG。
    if (mBoard == nullptr) {
        old_Plant_UpdateReanimColor(this);
        return;
    }
    SeedType aSeedType = mSeedType;
    if (!Plant::IsUpgrade(mSeedType)) {
        old_Plant_UpdateReanimColor(this);
        return;
    }
    if (mSeedType == SeedType::SEED_EXPLODE_O_NUT) {
        old_Plant_UpdateReanimColor(this);
        return;
    }
    GamepadControls *gamePad = mBoard->mGamepadControls1;
    if (gamePad->mGamepadState != 7) {
        mSeedType = SeedType::SEED_PEASHOOTER;
        old_Plant_UpdateReanimColor(this);
        mSeedType = aSeedType;
        return;
    }

    old_Plant_UpdateReanimColor(this);
}

bool Plant::IsOnBoard() {
    if (!mIsOnBoard)
        return false;

    return true;
}

bool Plant::IsInPlay() {
    return IsOnBoard() && mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN && mApp->mGameMode != GameMode::GAMEMODE_TREE_OF_WISDOM;
}

void Plant::Update() {
    // 用于修复植物受击闪光、生产发光、铲子下方植物发光，同时实现技能无冷却

    if (abilityFastCoolDown && mSeedType != SeedType::SEED_SPIKEWEED && mSeedType != SeedType::SEED_SPIKEROCK) { // 修复地刺和地刺王开启技能无冷却后不攻击敌人
        if (mStateCountdown > 10) {
            mStateCountdown = 10;
        }
    }

    int mHighLightCounter = mEatenFlashCountdown;
    int cancelHighLightLimit = 999 - (speedUpMode > 0 ? 10 : 0); // 铲子的发光计数是1000。这段代码用于在铲子移走之后的1ms内取消植物发光
    if (mHighLightCounter >= 900 && mHighLightCounter <= cancelHighLightLimit) {
        mBeghouledFlashCountdown = 0;
        mEatenFlashCountdown = 0;
    } else if (mHighLightCounter > 0) {
        mBeghouledFlashCountdown = mHighLightCounter > 25 ? 25 : mHighLightCounter;
    }

    GameScenes mGameScene = mApp->mGameScene;

    if ((!IsOnBoard() || mGameScene != GameScenes::SCENE_LEVEL_INTRO || !mApp->IsWallnutBowlingLevel()) && (!IsOnBoard() || mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
        && (!IsOnBoard() || !mBoard->mCutScene->ShouldRunUpsellBoard()) && IsOnBoard() && mGameScene != GameScenes::SCENE_PLAYING) {
        return;
    }

    if (requestPause) {
        // 如果开了高级暂停
        UpdateReanimColor();
        if (mHighLightCounter == 1000) {
            mBeghouledFlashCountdown = 0;
            mEatenFlashCountdown = 0;
        }
        return;
    }
    // 为了不影响改so，这里不是完全重写，而是执行旧函数
    // Plant_UpdateAbilities(plant);
    // Plant_Animate(plant);
    // if (plant->mPlantHealth < 0)
    // Plant_Die(plant);
    // Plant_UpdateReanim(plant);

    old_Plant_Update(this);
}

bool Plant::NotOnGround() {
    if (mSeedType == SeedType::SEED_SQUASH) {
        if (mState == PlantState::STATE_SQUASH_RISING || mState == PlantState::STATE_SQUASH_FALLING || mState == PlantState::STATE_SQUASH_DONE_FALLING)
            return true;
    }

    return mSquished || mOnBungeeState == PlantOnBungeeState::RISING_WITH_BUNGEE || mDead;
}

void Plant::Draw(Sexy::Graphics *g) {
    // 根据玩家的“植物显血”功能是否开启，决定是否在游戏的原始old_Plant_Draw函数执行完后额外绘制血量文本。

    g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
    int theCelRow = 0;
    float aOffsetX = 0.0f;
    float aOffsetY = PlantDrawHeightOffset(mBoard, 0, mSeedType, mPlantCol, mRow);
    if (IsFlying(mSeedType) && mSquished) {
        aOffsetY += 30.0f;
    }
    int theCelCol = mFrame;
    Image *aImage = GetImage(mSeedType);
    if (mSquished) {
        if (mSeedType == SeedType::SEED_FLOWERPOT) {
            aOffsetY -= 15.0f;
        }
        if (mSeedType == SeedType::SEED_INSTANT_COFFEE) {
            aOffsetY -= 20.0f;
        }
        float ratioSquished = 0.5f;
        g->SetScale(1.0f, ratioSquished, 0.0f, 0.0f);
        g->SetColorizeImages(true);
        Color color = {255, 255, 255, (int)(255.0f * std::min(1.0f, mDisappearCountdown / 100.0f))};
        g->SetColor(color);
        Plant::DrawSeedType(g, mSeedType, mImitaterType, DrawVariation::VARIATION_NORMAL, aOffsetX, aOffsetY + 85.0f * (1 - ratioSquished));
        g->SetScale(1.0f, 1.0f, 0.0f, 0.0f);
        g->SetColorizeImages(false);
        return;
    }

    bool aDrawPumpkinBack = false;
    Plant *aPumpkin = nullptr;

    if (IsOnBoard()) {
        aPumpkin = mBoard->GetPumpkinAt(mPlantCol, mRow);
        if (aPumpkin != nullptr) {
            Plant *aPlantInPumpkin = mBoard->GetTopPlantAt(mPlantCol, mRow, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
            if (aPlantInPumpkin != nullptr && aPlantInPumpkin->mRenderOrder > aPumpkin->mRenderOrder) {
                aPlantInPumpkin = nullptr;
            }
            if (aPlantInPumpkin != nullptr && aPlantInPumpkin->mOnBungeeState == PlantOnBungeeState::GETTING_GRABBED_BY_BUNGEE) {
                aPlantInPumpkin = nullptr;
            }
            if (aPlantInPumpkin == this) {
                aDrawPumpkinBack = true;
            }
            if (aPlantInPumpkin == nullptr && mSeedType == SeedType::SEED_PUMPKINSHELL) {
                aDrawPumpkinBack = true;
            }
        } else if (mSeedType == SeedType::SEED_PUMPKINSHELL) {
            aDrawPumpkinBack = true;
            aPumpkin = this;
        }
    } else if (mSeedType == SeedType::SEED_PUMPKINSHELL) {
        aDrawPumpkinBack = true;
        aPumpkin = this;
    }

    DrawShadow(g, aOffsetX, aOffsetY);

    if (IsFlying(mSeedType)) {
        int num3;
        if (IsOnBoard()) {
            num3 = mBoard->mMainCounter;
        } else {
            num3 = mApp->mAppCounter;
        }
        float num4 = (num3 + mRow * 97 + mPlantCol * 61) * 0.03f;
        float num5 = sin(num4) * 2.0f;
        aOffsetY += num5;
    }

    if (aDrawPumpkinBack) {
        Reanimation *reanimation = mApp->ReanimationGet(aPumpkin->mBodyReanimID);
        Graphics aPumpkinGraphics(*g);
        aPumpkinGraphics.mTransX += aPumpkin->mX - mX;
        aPumpkinGraphics.mTransY += aPumpkin->mY - mY;
        reanimation->DrawRenderGroup(&aPumpkinGraphics, 1);
    }

    aOffsetX += mShakeOffsetX;
    aOffsetY += mShakeOffsetY;
    if (IsInPlay() && mApp->IsIZombieLevel()) {
        mBoard->mChallenge->IZombieDrawPlant(g, this);
    } else if (mBodyReanimID != 0) {
        Reanimation *reanimation2 = mApp->ReanimationTryToGet(mBodyReanimID);
        if (reanimation2 != nullptr) {
            // if (plant->mGloveGrabbed)
            // {
            // SetColorizeImages(g,true);
            // Color color = {150, 255, 150, 255};
            // SetColor(g,&color);
            // }
            reanimation2->DrawRenderGroup(g, 0);
            // if (plant->mGloveGrabbed)
            // {
            // SetColorizeImages(g,false);
            // }
        }
    } else {
        SeedType seedType = SeedType::SEED_NONE;
        SeedType seedType2 = SeedType::SEED_NONE;
        if (mBoard != nullptr) {
            seedType = mBoard->GetSeedTypeInCursor(0);
            seedType2 = mBoard->GetSeedTypeInCursor(1);
        }
        if ((IsPartOfUpgradableTo(seedType) && mBoard->CanPlantAt(mPlantCol, mRow, seedType) == PlantingReason::PLANTING_OK)
            || (IsPartOfUpgradableTo(seedType2) && mBoard->CanPlantAt(mPlantCol, mRow, seedType2) == PlantingReason::PLANTING_OK)) {
            g->SetColorizeImages(true);
            Color color = GetFlashingColor(mBoard->mMainCounter, 90);
            g->SetColor(color);
        } else if ((seedType == SeedType::SEED_COBCANNON && mBoard->CanPlantAt(mPlantCol - 1, mRow, seedType) == PlantingReason::PLANTING_OK)
                   || (seedType2 == SeedType::SEED_COBCANNON && mBoard->CanPlantAt(mPlantCol - 1, mRow, seedType2) == PlantingReason::PLANTING_OK)) {
            g->SetColorizeImages(true);
            Color color = GetFlashingColor(mBoard->mMainCounter, 90);
            g->SetColor(color);
        } else if (mBoard != nullptr && mBoard->mTutorialState == TutorialState::TUTORIAL_SHOVEL_DIG) {
            g->SetColorizeImages(true);
            Color color = GetFlashingColor(mBoard->mMainCounter, 90);
            g->SetColor(color);
        }
        if (aImage != nullptr) {
            TodDrawImageCelF(g, aImage, aOffsetX, aOffsetY, theCelCol, theCelRow);
        }
        // if (mSeedType == a::Sprout)
        // {
        // if (plant->mGloveGrabbed)
        // {
        // SetColorizeImages(g,true);
        // Color color ={150, 255, 150, 255};
        // SetColor(g, &color);
        // }
        // TodDrawImageCelF(g, AtlasResources.IMAGE_CACHED_MARIGOLD, Constants.ZenGarden_Marigold_Sprout_Offset.X, Constants.ZenGarden_Marigold_Sprout_Offset.Y, 0, 0);
        // if (plant->mGloveGrabbed)
        // {
        // SetColorizeImages(g,false);
        // }
        // }
        g->SetColorizeImages(false);
        if (mHighlighted) {
            g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
            g->SetColorizeImages(true);
            Color color = {255, 255, 255, 196};
            g->SetColor(color);
            if (aImage != nullptr) {
                TodDrawImageCelF(g, aImage, aOffsetX, aOffsetY, theCelCol, theCelRow);
            }
            g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
            g->SetColorizeImages(false);
        } else if (mEatenFlashCountdown > 0) {
            g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
            g->SetColorizeImages(true);
            int theAlpha = std::clamp(mEatenFlashCountdown * 3, 0, 255);
            Color color = {255, 255, 255, theAlpha};
            g->SetColor(color);
            if (aImage != nullptr) {
                TodDrawImageCelF(g, aImage, aOffsetX, aOffsetY, theCelCol, theCelRow);
            }
            g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
            g->SetColorizeImages(false);
        }
    }
    if (mSeedType == SeedType::SEED_MAGNETSHROOM && !DrawMagnetItemsOnTop()) {
        DrawMagnetItems(g);
    }


    if (showPlantHealth
        || (showNutGarlicSpikeHealth
            && (mSeedType == SeedType::SEED_WALLNUT || mSeedType == SeedType::SEED_TALLNUT || mSeedType == SeedType::SEED_PUMPKINSHELL || mSeedType == SeedType::SEED_GARLIC
                || mSeedType == SeedType::SEED_SPIKEROCK))) { // 如果玩家开了 植物显血
        pvzstl::string str = StrFormat("%d/%d", mPlantHealth, mPlantMaxHealth);
        g->SetFont(*Sexy_FONT_DWARVENTODCRAFT12_Addr);
        if (mSeedType == SeedType::SEED_PUMPKINSHELL) {
            g->SetColor(yellow);
            g->DrawString(str, 0, 52);
        } else if (mSeedType == SeedType::SEED_FLOWERPOT) {
            g->SetColor(brown);
            g->DrawString(str, 0, 93);
        } else if (mSeedType == SeedType::SEED_LILYPAD) {
            g->SetColor(green);
            g->DrawString(str, 0, 100);
        } else if (mSeedType == SeedType::SEED_COBCANNON) {
            g->SetColor(white);
            g->DrawString(str, 40, 34);
        } else {
            g->SetColor(white);
            g->DrawString(str, 0, 34);
        }
        g->SetFont(nullptr);
    }
}

void Plant::DrawSeedType(Sexy::Graphics *g, SeedType theSeedType, SeedType theImitaterType, DrawVariation theDrawVariation, float thePosX, float thePosY) {
    // 用于绘制卡槽内的模仿者SeedPacket变白效果、模仿者变身后的植物被压扁的白色效果、模仿者变身前被压扁后绘制模仿者自己而非变身后的植物。
    int v38 = ((int *)g)[9];
    int v10 = ((int *)g)[8];
    int v11 = ((int *)g)[4];
    int v39 = ((int *)g)[10];
    int v12 = ((int *)g)[5];
    int v13 = ((int *)g)[6];
    int v40 = ((int *)g)[11];
    int v14 = ((int *)g)[7];
    int v15 = ((int *)g)[2];
    int v16 = ((int *)g)[3];
    Color theColor = g->GetColor();
    int v18 = theColor[1];
    int v19 = theColor[2];
    int v20 = theColor[3];
    Color color;
    color.mRed = theColor.mRed;
    color.mGreen = v18;
    color.mBlue = v19;
    color.mAlpha = v20;
    bool ColorizeImages = g->GetColorizeImages();
    SeedType theSeedType2 = theSeedType;

    if ((theSeedType == theImitaterType && theImitaterType != SeedType::SEED_NONE) ||         // seedPacket中的灰色模仿者卡片在冷却完成后
        (theImitaterType == SeedType::SEED_IMITATER && theSeedType != SeedType::SEED_NONE)) { // 模仿者变身之后的植物被压扁
        switch (theSeedType2) {
            case SeedType::SEED_POTATOMINE:
            case SeedType::SEED_HYPNOSHROOM:
            case SeedType::SEED_LILYPAD:
            case SeedType::SEED_SQUASH:
            case SeedType::SEED_GARLIC:
                theDrawVariation = DrawVariation::VARIATION_IMITATER_LESS;
                break;
            case SeedType::SEED_IMITATER:
                theDrawVariation = DrawVariation::VARIATION_NORMAL;
                break;
            default:
                theDrawVariation = DrawVariation::VARIATION_IMITATER;
                break;
        }
    }
    LawnApp *lawnApp = *gLawnApp_Addr;
    float v24, v25;
    if (lawnApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BIG_TIME
        && (theSeedType2 == SeedType::SEED_SUNFLOWER || theSeedType2 == SeedType::SEED_WALLNUT || theSeedType2 == SeedType::SEED_MARIGOLD)) {
        v24 = -40.0;
        v25 = -20.0;
        g->mScaleX = g->mScaleX * 1.5;
        g->mScaleY = g->mScaleY * 1.5;
    } else {
        v24 = 0.0;
        v25 = 0.0;
    }
    if (theSeedType2 == SeedType::SEED_LEFTPEATER) {
        v25 = v25 + g->mScaleX * 80.0;
        g->mScaleX = -g->mScaleX;
    }
    if (Challenge::IsZombieSeedType(theSeedType2)) {
        ZombieType theZombieType = Challenge::IZombieSeedTypeToZombieType(theSeedType2);
        if (theZombieType != ZombieType::ZOMBIE_INVALID) {
            lawnApp->mReanimatorCache->DrawCachedZombie(g, thePosX, thePosY, theZombieType);
        }
        return;
    } else {
        PlantDefinition aPlantDef = GetPlantDefinition(theSeedType2);
        if (theSeedType2 == SeedType::SEED_GIANT_WALLNUT) {
            g->mScaleX = g->mScaleX * 1.4;
            g->mScaleY = g->mScaleY * 1.4;
            TodDrawImageScaledF(g, *Sexy_IMAGE_REANIM_WALLNUT_BODY_Addr, thePosX - 53.0, thePosY - 56.0, g->mScaleX, g->mScaleY);
        } else if (aPlantDef.mReanimationType == -1) {
            int v29;
            if (theSeedType2 == SeedType::SEED_KERNELPULT)
                v29 = 2;
            else
                v29 = theSeedType2 == SeedType::SEED_TWINSUNFLOWER;

            Image *aImage = GetImage(theSeedType2);
            int v31 = aImage->mNumCols;
            int v32;
            if (v31 > 2)
                v32 = 2;
            else
                v32 = v31 - 1;
            TodDrawImageCelScaledF(g, aImage, v25 + thePosX, v24 + thePosY, v32, v29, g->mScaleX, g->mScaleY);
        } else {
            lawnApp->mReanimatorCache->DrawCachedPlant(g, v25 + thePosX, v24 + thePosY, theSeedType2, theDrawVariation);
        }
    }
    ((int *)g)[8] = v10;
    ((int *)g)[4] = v11;
    ((int *)g)[9] = v38;
    ((int *)g)[5] = v12;
    ((int *)g)[6] = v13;
    ((int *)g)[10] = v39;
    ((int *)g)[7] = v14;
    ((int *)g)[2] = v15;
    ((int *)g)[11] = v40;
    ((int *)g)[3] = v16;
    g->SetColor(color);
    g->SetColorizeImages(ColorizeImages);
}

void Plant::KillAllPlantsNearDoom() {
    Plant *aPlant = nullptr;
    while (mBoard->IteratePlants(aPlant)) {
        if (aPlant->mRow == mRow && aPlant->mPlantCol == mPlantCol) {
            aPlant->Die();
        }
    }
}

void Plant::DoSpecial() {
    // 试图修复辣椒爆炸后反而在本行的末尾处产生冰道。失败。

    if (mApp->IsVSMode() && mApp->mGameScene == SCENE_PLAYING) {
        if (tcp_connected)
            return;

        if (tcpClientSocket >= 0) {
            U16_Event event = {{EventType::EVENT_SERVER_BOARD_PLANT_DO_SPECIAL}, uint16_t(mBoard->mPlants.DataArrayGetID(this))};
            send(tcpClientSocket, &event, sizeof(U16_Event), 0);
        }
    }

    int aPosX = mX + mWidth / 2;
    int aPosY = mY + mHeight / 2;
    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);

    switch (mSeedType) {
        case SeedType::SEED_BLOVER: {
            if (mState != PlantState::STATE_DOINGSPECIAL) {
                mState = PlantState::STATE_DOINGSPECIAL;
                BlowAwayFliers(mX, mRow);
            }
            break;
        }
        case SeedType::SEED_CHERRYBOMB: {
            mApp->PlayFoley(FoleyType::FOLEY_CHERRYBOMB);
            mApp->PlayFoley(FoleyType::FOLEY_JUICY);

            if (mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, 115, 1, true, aDamageRangeFlags) >= 10)
                if (!!mApp->IsLittleTroubleLevel())
                    mBoard->GrantAchievement(AchievementId::ACHIEVEMENT_EXPLODONATOR, true);

            mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_POWIE);
            mBoard->ShakeBoard(3, -4);

            Die();
            break;
        }
        case SeedType::SEED_DOOMSHROOM: {
            mApp->PlaySample(*SOUND_DOOMSHROOM);

            mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, 250, 3, true, aDamageRangeFlags);
            KillAllPlantsNearDoom();

            mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_DOOM);
            mBoard->AddACrater(mPlantCol, mRow)->mGridItemCounter = 18000;
            mBoard->ShakeBoard(3, -4);

            Die();
            break;
        }
        case SeedType::SEED_JALAPENO: {
            mApp->PlayFoley(FoleyType::FOLEY_JALAPENO_IGNITE);
            mApp->PlayFoley(FoleyType::FOLEY_JUICY);

            mBoard->DoFwoosh(mRow);
            mBoard->ShakeBoard(3, -4);

            BurnRow(mRow);
            mBoard->mIceTimer[mRow] = 20;

            Die();
            break;
        }
        case SeedType::SEED_UMBRELLA: {
            if (mState != PlantState::STATE_UMBRELLA_TRIGGERED && mState != PlantState::STATE_UMBRELLA_REFLECTING) {
                mState = PlantState::STATE_UMBRELLA_TRIGGERED;
                mStateCountdown = 5;

                PlayBodyReanim("anim_block", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 22.0f);
            }

            break;
        }
        case SeedType::SEED_ICESHROOM: {
            mApp->PlayFoley(FoleyType::FOLEY_FROZEN);
            IceZombies();
            mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_ICE_TRAP);

            Die();
            break;
        }
        case SeedType::SEED_POTATOMINE: {
            aPosX = mX + mWidth / 2 - 20;
            aPosY = mY + mHeight / 2;

            mApp->PlaySample(*SOUND_POTATO_MINE);
            mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, 60, 0, false, aDamageRangeFlags);

            int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mRow, 0);
            mApp->AddTodParticle(aPosX + 20.0f, aPosY, aRenderPosition, ParticleEffect::PARTICLE_POTATO_MINE);
            mBoard->ShakeBoard(3, -4);

            Die();
            break;
        }
        case SeedType::SEED_INSTANT_COFFEE: {
            Plant *aPlant = mBoard->GetTopPlantAt(mPlantCol, mRow, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
            if (aPlant && aPlant->mIsAsleep) {
                aPlant->mWakeUpCounter = 100;
            }

            mState = PlantState::STATE_DOINGSPECIAL;
            PlayBodyReanim("anim_crumble", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 22.0f);
            mApp->PlayFoley(FoleyType::FOLEY_COFFEE);

            break;
        }
    }
}

// void Plant_CobCannonFire(Plant *plant, int x, int y) {
// LOGD("fire:%d %d",x,y);
// old_Plant_CobCannonFire(plant,x,y);
// }

void Plant::Fire(Zombie *theTargetZombie, int theRow, PlantWeapon thePlantWeapon, GridItem *theTargetGridItem) {
    if (mApp->mGameMode == GAMEMODE_MP_VS) {
        if (tcp_connected)
            return;

        if (tcpClientSocket >= 0) {
            U16U16Buf32Buf32_Event event;

            event.type = EventType::EVENT_SERVER_BOARD_PLANT_FIRE;
            event.data1 = uint16_t(mBoard->mPlants.DataArrayGetID(this));
            event.data2 = theTargetZombie == nullptr ? uint16_t(ZOMBIEID_NULL) : uint16_t(mBoard->mZombies.DataArrayGetID(theTargetZombie));
            event.data3.u16x2.u16_1 = uint16_t(theRow);
            event.data3.u16x2.u16_2 = uint16_t(thePlantWeapon);
            // 如果同时传入有效的 theTargetZombie 和 theTargetGridItem 会导致投手弹道计算错误
            if (theTargetZombie) { // 存在僵尸目标时传入空的场地物 ID
                event.data4.u16x2.u16_1 = uint16_t(GRIDITEMID_NULL);
            } else {
                event.data4.u16x2.u16_1 = theTargetGridItem == nullptr ? uint16_t(GRIDITEMID_NULL) : uint16_t(mBoard->mGridItems.DataArrayGetID(theTargetGridItem));
            }
            send(tcpClientSocket, &event, sizeof(U16U16Buf32Buf32_Event), 0);
        }
    }

    if (mSeedType == SeedType::SEED_FUMESHROOM) {
        DoRowAreaDamage(20, 2U);
        mApp->PlayFoley(FoleyType::FOLEY_FUME);
        return;
    }
    if (mSeedType == SeedType::SEED_GLOOMSHROOM) {
        DoRowAreaDamage(20, 2U);
        return;
    }
    if (mSeedType == SeedType::SEED_STARFRUIT) {
        StarFruitFire();
        return;
    }

    ProjectileType aProjectileType;
    switch (mSeedType) {
        case SeedType::SEED_PEASHOOTER:
        case SeedType::SEED_REPEATER:
        case SeedType::SEED_THREEPEATER:
        case SeedType::SEED_SPLITPEA:
        case SeedType::SEED_GATLINGPEA:
        case SeedType::SEED_LEFTPEATER:
            aProjectileType = ProjectileType::PROJECTILE_PEA;
            break;
        case SeedType::SEED_SNOWPEA:
            aProjectileType = ProjectileType::PROJECTILE_SNOWPEA;
            break;
        case SeedType::SEED_PUFFSHROOM:
        case SeedType::SEED_SCAREDYSHROOM:
        case SeedType::SEED_SEASHROOM:
            aProjectileType = ProjectileType::PROJECTILE_PUFF;
            break;
        case SeedType::SEED_CACTUS:
        case SeedType::SEED_CATTAIL:
            aProjectileType = ProjectileType::PROJECTILE_SPIKE;
            break;
        case SeedType::SEED_CABBAGEPULT:
            aProjectileType = ProjectileType::PROJECTILE_CABBAGE;
            break;
        case SeedType::SEED_KERNELPULT:
            aProjectileType = ProjectileType::PROJECTILE_KERNEL;
            break;
        case SeedType::SEED_MELONPULT:
            aProjectileType = ProjectileType::PROJECTILE_MELON;
            break;
        case SeedType::SEED_WINTERMELON:
            aProjectileType = ProjectileType::PROJECTILE_WINTERMELON;
            break;
        case SeedType::SEED_COBCANNON:
            aProjectileType = ProjectileType::PROJECTILE_COBBIG;
            break;
        default:
            break;
    }
    if (mSeedType == SeedType::SEED_KERNELPULT && thePlantWeapon == PlantWeapon::WEAPON_SECONDARY) {
        aProjectileType = ProjectileType::PROJECTILE_BUTTER;
    }

    mApp->PlayFoley(FoleyType::FOLEY_THROW);
    if (mSeedType == SeedType::SEED_SNOWPEA || mSeedType == SeedType::SEED_WINTERMELON) {
        mApp->PlayFoley(FoleyType::FOLEY_SNOW_PEA_SPARKLES);
    } else if (mSeedType == SeedType::SEED_PUFFSHROOM || mSeedType == SeedType::SEED_SCAREDYSHROOM || mSeedType == SeedType::SEED_SEASHROOM) {
        mApp->PlayFoley(FoleyType::FOLEY_PUFF);
    }

    int aOriginX, aOriginY;
    if (mSeedType == SeedType::SEED_PUFFSHROOM) {
        aOriginX = mX + 40;
        aOriginY = mY + 40;
    } else if (mSeedType == SeedType::SEED_SEASHROOM) {
        aOriginX = mX + 45;
        aOriginY = mY + 63;
    } else if (mSeedType == SeedType::SEED_CABBAGEPULT) {
        aOriginX = mX + 5;
        aOriginY = mY - 12;
    } else if (mSeedType == SeedType::SEED_MELONPULT || mSeedType == SeedType::SEED_WINTERMELON) {
        aOriginX = mX + 25;
        aOriginY = mY - 46;
    } else if (mSeedType == SeedType::SEED_CATTAIL) {
        aOriginX = mX + 20;
        aOriginY = mY - 3;
    } else if (mSeedType == SeedType::SEED_KERNELPULT && thePlantWeapon == PlantWeapon::WEAPON_PRIMARY) {
        aOriginX = mX + 19;
        aOriginY = mY - 37;
    } else if (mSeedType == SeedType::SEED_KERNELPULT && thePlantWeapon == PlantWeapon::WEAPON_SECONDARY) {
        aOriginX = mX + 12;
        aOriginY = mY - 56;
    } else if (mSeedType == SeedType::SEED_PEASHOOTER || mSeedType == SeedType::SEED_SNOWPEA || mSeedType == SeedType::SEED_REPEATER) {
        int aOffsetX, aOffsetY;
        GetPeaHeadOffset(aOffsetX, aOffsetY);
        aOriginX = mX + aOffsetX + 24;
        aOriginY = mY + aOffsetY - 33;
    } else if (mSeedType == SeedType::SEED_LEFTPEATER) {
        int aOffsetX, aOffsetY;
        GetPeaHeadOffset(aOffsetX, aOffsetY);
        aOriginX = mX + aOffsetX + 27;
        aOriginY = mY + aOffsetY - 33;
    } else if (mSeedType == SeedType::SEED_GATLINGPEA) {
        int aOffsetX, aOffsetY;
        GetPeaHeadOffset(aOffsetX, aOffsetY);
        aOriginX = mX + aOffsetX + 34;
        aOriginY = mY + aOffsetY - 33;
    } else if (mSeedType == SeedType::SEED_SPLITPEA) {
        int aOffsetX, aOffsetY;
        GetPeaHeadOffset(aOffsetX, aOffsetY);
        aOriginY = mY + aOffsetY - 33;

        if (thePlantWeapon == PlantWeapon::WEAPON_SECONDARY) {
            aOriginX = mX + aOffsetX - 64;
        } else {
            aOriginX = mX + aOffsetX + 24;
        }
    } else if (mSeedType == SeedType::SEED_THREEPEATER) {
        aOriginX = mX + 45;
        aOriginY = mY + 10;
    } else if (mSeedType == SeedType::SEED_SCAREDYSHROOM) {
        aOriginX = mX + 29;
        aOriginY = mY + 21;
    } else if (mSeedType == SeedType::SEED_CACTUS) {
        if (thePlantWeapon == PlantWeapon::WEAPON_PRIMARY) {
            aOriginX = mX + 93;
            aOriginY = mY - 50;
        } else {
            aOriginX = mX + 70;
            aOriginY = mY + 23;
        }
    } else if (mSeedType == SeedType::SEED_COBCANNON) {
        aOriginX = mX - 44;
        aOriginY = mY - 184;
    } else {
        aOriginX = mX + 10;
        aOriginY = mY + 5;
    }
    if (mBoard->GetFlowerPotAt(mPlantCol, mRow)) {
        aOriginY -= 5;
    }

    if (mSeedType == SeedType::SEED_SNOWPEA) {
        int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_LAWN_MOWER, mRow, 1);
        mApp->AddTodParticle(aOriginX + 8, aOriginY + 13, aRenderPosition, ParticleEffect::PARTICLE_SNOWPEA_PUFF);
    } else if (mSeedType == SeedType::SEED_PUFFSHROOM) {
        int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_LAWN_MOWER, mRow, 1);
        mApp->AddTodParticle(aOriginX + 18, aOriginY + 13, aRenderPosition, ParticleEffect::PARTICLE_PUFFSHROOM_MUZZLE);
    } else if (mSeedType == SeedType::SEED_SCAREDYSHROOM) {
        int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_LAWN_MOWER, mRow, 1);
        mApp->AddTodParticle(aOriginX + 27, aOriginY + 13, aRenderPosition, ParticleEffect::PARTICLE_PUFFSHROOM_MUZZLE);
    }

    Projectile *aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder - 1, theRow, aProjectileType);
    aProjectile->mDamageRangeFlags = GetDamageRangeFlags(thePlantWeapon);

    if (mSeedType == SeedType::SEED_CABBAGEPULT || mSeedType == SeedType::SEED_KERNELPULT || mSeedType == SeedType::SEED_MELONPULT || mSeedType == SeedType::SEED_WINTERMELON) {
        float aRangeX, aRangeY;
        if (theTargetZombie) {
            Rect aZombieRect = theTargetZombie->GetZombieRect();
            aRangeX = theTargetZombie->ZombieTargetLeadX(50.0f) - aOriginX - 30.0f;
            aRangeY = aZombieRect.mY - aOriginY;

            if (theTargetZombie->mZombiePhase == ZombiePhase::PHASE_DOLPHIN_RIDING) {
                aRangeX -= 60.0f;
            }
            if (theTargetZombie->mZombieType == ZombieType::ZOMBIE_POGO && theTargetZombie->mHasObject) {
                aRangeX -= 60.0f;
            }
            if (theTargetZombie->mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING_IN_POOL) {
                aRangeX -= 40.0f;
            }
            if (theTargetZombie->mZombieType == ZombieType::ZOMBIE_BOSS) {
                aRangeY = mBoard->GridToPixelY(8, mRow) - aOriginY;
            }
        } else if (theTargetGridItem) {
            aRangeX = mBoard->GridToPixelX(theTargetGridItem->mGridX, theTargetGridItem->mGridY) - aOriginX;
            aRangeY = (float(mBoard->GridToPixelY(theTargetGridItem->mGridX, theTargetGridItem->mGridY)) - aOriginY) * 0.0083333 - 7.0;
        } else {
            aRangeX = 700.0f - aOriginX;
            aRangeY = 0.0f;
        }
        if (aRangeX < 40.0f) {
            aRangeX = 40.0f;
        }

        aProjectile->mMotionType = ProjectileMotion::MOTION_LOBBED;
        aProjectile->mVelX = aRangeX / 120.0f;
        aProjectile->mVelY = 0.0f;
        aProjectile->mVelZ = aRangeY / 120.0f - 7.0f;
        aProjectile->mAccZ = 0.115f;
    } else if (mSeedType == SeedType::SEED_THREEPEATER) {
        if (theRow < mRow) {
            aProjectile->mMotionType = ProjectileMotion::MOTION_THREEPEATER;
            aProjectile->mVelY = -3.0f;
            aProjectile->mShadowY += 80.0f;
        } else if (theRow > mRow) {
            aProjectile->mMotionType = ProjectileMotion::MOTION_THREEPEATER;
            aProjectile->mVelY = 3.0f;
            aProjectile->mShadowY -= 80.0f;
        }
    } else if (mSeedType == SeedType::SEED_PUFFSHROOM || mSeedType == SeedType::SEED_SEASHROOM) {
        aProjectile->mMotionType = ProjectileMotion::MOTION_PUFF;
    } else if (mSeedType == SeedType::SEED_SPLITPEA && thePlantWeapon == PlantWeapon::WEAPON_SECONDARY) {
        aProjectile->mMotionType = ProjectileMotion::MOTION_BACKWARDS;
    } else if (mSeedType == SeedType::SEED_LEFTPEATER) {
        aProjectile->mMotionType = ProjectileMotion::MOTION_BACKWARDS;
    } else if (mSeedType == SeedType::SEED_CATTAIL) {
        aProjectile->mVelX = 2.0f;
        aProjectile->mMotionType = ProjectileMotion::MOTION_HOMING;
        aProjectile->mTargetZombieID = mBoard->ZombieGetID(theTargetZombie);
    } else if (mSeedType == SeedType::SEED_COBCANNON) {
        aProjectile->mVelX = 0.001f;
        aProjectile->mDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
        aProjectile->mMotionType = ProjectileMotion::MOTION_LOBBED;
        aProjectile->mVelY = 0.0f;
        aProjectile->mAccZ = 0.0f;
        aProjectile->mVelZ = -8.0f;
        aProjectile->mCobTargetX = mTargetX - 40;
        aProjectile->mCobTargetRow = mBoard->PixelToGridYKeepOnBoard(mTargetX, mTargetY);
    }
}

Zombie *Plant::FindTargetZombie(int theRow, PlantWeapon thePlantWeapon) {
    int aDamageRangeFlags = GetDamageRangeFlags(thePlantWeapon);
    Rect aAttackRect = GetPlantAttackRect(thePlantWeapon);
    int aHighestWeight = 0;
    Zombie *aBestZombie = nullptr;

    Zombie *aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie)) {
        int aRowDeviation = aZombie->mRow - theRow;
        if (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS) {
            aRowDeviation = 0;
        }

        if (!aZombie->mHasHead || aZombie->IsTangleKelpTarget()) {
            if (mSeedType == SeedType::SEED_POTATOMINE || mSeedType == SeedType::SEED_CHOMPER || mSeedType == SeedType::SEED_TANGLEKELP) {
                continue;
            }
        }

        bool needPortalCheck = false;
        if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_PORTAL_COMBAT) {
            if (mSeedType == SeedType::SEED_PEASHOOTER || mSeedType == SeedType::SEED_CACTUS || mSeedType == SeedType::SEED_REPEATER) {
                needPortalCheck = true;
            }
        }

        if (mSeedType != SeedType::SEED_CATTAIL) {
            if (mSeedType == SeedType::SEED_GLOOMSHROOM) {
                if (aRowDeviation < -1 || aRowDeviation > 1) {
                    continue;
                }
            } else if (needPortalCheck) {
                if (!mBoard->mChallenge->CanTargetZombieWithPortals(this, aZombie)) {
                    continue;
                }
            } else if (aRowDeviation) {
                continue;
            }
        }

        if (aZombie->EffectedByDamage(aDamageRangeFlags)) {
            int aExtraRange = 0;

            if (mSeedType == SeedType::SEED_CHOMPER) {
                if (aZombie->mZombiePhase == ZombiePhase::PHASE_DIGGER_WALKING) {
                    aAttackRect.mX += 20;
                    aAttackRect.mWidth -= 20;
                }

                if (aZombie->mZombiePhase == ZombiePhase::PHASE_POGO_BOUNCING || (aZombie->mZombieType == ZombieType::ZOMBIE_BUNGEE && aZombie->mTargetCol == mPlantCol)) {
                    continue;
                }

                if (aZombie->mIsEating || mState == PlantState::STATE_CHOMPER_BITING) {
                    aExtraRange = 60;
                }
            }

            if (mSeedType == SeedType::SEED_POTATOMINE) {
                if ((aZombie->mZombieType == ZombieType::ZOMBIE_POGO && aZombie->mHasObject) || aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT
                    || aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT) {
                    continue;
                }

                if (aZombie->mZombieType == ZombieType::ZOMBIE_POLEVAULTER) {
                    aAttackRect.mX += 40;
                    aAttackRect.mWidth -= 40; // 原版经典土豆地雷 Bug 及“四撑杆引雷”的原理
                }

                if (aZombie->mZombieType == ZombieType::ZOMBIE_BUNGEE && aZombie->mTargetCol != mPlantCol) {
                    continue;
                }

                if (aZombie->mIsEating) {
                    aExtraRange = 30;
                }
            }

            if ((mSeedType == SeedType::SEED_EXPLODE_O_NUT && aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT) || (mSeedType == SeedType::SEED_TANGLEKELP && !aZombie->mInPool)) {
                continue;
            }

            Rect aZombieRect = aZombie->GetZombieRect();
            if (!needPortalCheck && GetRectOverlap(aAttackRect, aZombieRect) < -aExtraRange) {
                continue;
            }

            ////////////////////

            int aWeight = -aZombieRect.mX;
            if (mSeedType == SeedType::SEED_CATTAIL) {
                aWeight = -Distance2D(mX + 40.0f, mY + 40.0f, aZombieRect.mX + aZombieRect.mWidth / 2, aZombieRect.mY + aZombieRect.mHeight / 2);
                if (aZombie->IsFlying()) {
                    aWeight += 10000; // 优先攻击飞行单位
                }
            }

            if (aBestZombie == nullptr || aWeight > aHighestWeight) {
                aHighestWeight = aWeight;
                aBestZombie = aZombie;
            }
        }
    }

    return aBestZombie;
}

GridItem *Plant::FindTargetGridItem(PlantWeapon thePlantWeapon) {
    // 对战模式专用，植物索敌僵尸墓碑和靶子僵尸。
    // 原版函数BUG：植物还会索敌梯子和毁灭菇弹坑，故重写以修复BUG。
    int aLastGridX = 0;
    GridItem *aBestGridItem = nullptr;

    GridItem *aGridItem = nullptr;
    if (mApp->IsVSMode()) { // 如果是对战模式
        int aRow = mStartRow;
        while (mBoard->IterateGridItems(aGridItem)) { // 遍历场上的所有GridItem
            int aGridX = aGridItem->mGridX;
            int aGridY = aGridItem->mGridY;
            GridItemType aGridItemType = aGridItem->mGridItemType;

            if (aGridItemType != GridItemType::GRIDITEM_GRAVESTONE && aGridItemType != GridItemType::GRIDITEM_VS_TARGET_ZOMBIE) {
                // 修复植物们攻击核坑和梯子
                continue;
            }

            if (mSeedType == SeedType::SEED_THREEPEATER ? abs(aGridY - aRow) > 1 : aGridY != aRow) {
                // 如果是三线射手，则索敌三行; 反之，索敌一行
                // 注释掉此行，就会发现投手能够命中三格内的靶子了，但会导致很多其他BUG。尚不清楚原因。
                continue;
            }

            if (aBestGridItem == nullptr || aGridX < aLastGridX) {
                if (mSeedType == SeedType::SEED_FUMESHROOM && aGridX - mPlantCol > 3) {
                    // 如果是大喷菇，则索敌三格以内的靶子或墓碑
                    continue;
                }
                if (mSeedType == SeedType::SEED_PUFFSHROOM || mSeedType == SeedType::SEED_SEASHROOM) {
                    if (gVSBalanceAdjustment && aGridX - mPlantCol > 2) {
                        continue;
                    }
                    // 如果是小喷菇或水兵菇，则索敌三格以内的墓碑
                    if (aGridX - mPlantCol > 3) {
                        continue;
                    }
                    // 不主动攻击靶子
                    if (aGridItemType == GridItemType::GRIDITEM_VS_TARGET_ZOMBIE) {
                        continue;
                    }
                }

                aBestGridItem = aGridItem;
                aLastGridX = aGridX;
            }
        }
    }

    return aBestGridItem;
}

void Plant::Die() {
    if (mApp->IsVSMode() && mApp->mGameScene == SCENE_PLAYING) {
        if (tcp_connected)
            return;

        if (tcpClientSocket >= 0) {
            U16_Event event = {{EventType::EVENT_SERVER_BOARD_PLANT_DIE}, uint16_t(mBoard->mPlants.DataArrayGetID(this))};
            send(tcpClientSocket, &event, sizeof(U16_Event), 0);
        }
    }

    if (IsOnBoard() && mSeedType == SeedType::SEED_TANGLEKELP) {
        Zombie *aZombie = mBoard->ZombieTryToGet(mTargetZombieID);
        if (aZombie) {
            aZombie->DieWithLoot();
        }
    }

    mDead = true;
    RemoveEffects();

    if (!Plant::IsFlying(mSeedType) && IsOnBoard()) {
        GridItem *aLadder = mBoard->GetLadderAt(mPlantCol, mRow);
        if (aLadder) {
            aLadder->GridItemDie();
        }
    }

    if (IsOnBoard()) {
        Plant *aTopPlant = mBoard->GetTopPlantAt(mPlantCol, mRow, PlantPriority::TOPPLANT_BUNGEE_ORDER);
        Plant *aFlowerPot = mBoard->GetFlowerPotAt(mPlantCol, mRow);
        if (aFlowerPot && aTopPlant == aFlowerPot) {
            Reanimation *aPotReanim = mApp->ReanimationGet(aFlowerPot->mBodyReanimID);
            aPotReanim->mAnimRate = RandRangeFloat(10.0f, 15.0f);
        }
    }
}

PlantDefinition &GetPlantDefinition(SeedType theSeedType) {
    return gPlantDefs[theSeedType];
}

int Plant::GetCost(SeedType theSeedType, SeedType theImitaterType) {
    LawnApp *gLawnApp = *gLawnApp_Addr;
    if (gLawnApp->mGameMode == GameMode::GAMEMODE_MP_VS) {
        if (theSeedType == SeedType::SEED_IMITATER && theImitaterType != SeedType::SEED_NONE) {
            theSeedType = theImitaterType;
        }
        if (gVSBalanceAdjustment) {
            return GetCostAdjusted(theSeedType);
        } else {
            switch (theSeedType) {
                case SeedType::SEED_CHERRYBOMB:
                case SeedType::SEED_SNOWPEA:
                case SeedType::SEED_REPEATER:
                case SeedType::SEED_ZOMBIE_FOOTBALL:
                case SeedType::SEED_ZOMBIE_DANCER:
                case SeedType::SEED_ZOMBIE_DIGGER:
                case SeedType::SEED_ZOMBIE_LADDER:
                    return 150;
                case SeedType::SEED_SQUASH:
                case SeedType::SEED_GARLIC:
                case SeedType::SEED_ZOMBIE_TRAFFIC_CONE:
                    return 75;
                case SeedType::SEED_THREEPEATER:
                case SeedType::SEED_ZOMBIE_CATAPULT:
                    return 200;
                case SeedType::SEED_JALAPENO:
                case SeedType::SEED_TORCHWOOD:
                case SeedType::SEED_ZOMBIE_BUNGEE:
                case SeedType::SEED_ZOMBIE_SNORKEL:
                case SeedType::SEED_ZOMBIE_DOLPHIN_RIDER:
                case SeedType::SEED_ZOMBIE_JALAPENO_HEAD:
                case SeedType::SEED_ZOMBIE_GATLINGPEA_HEAD:
                case SeedType::SEED_ZOMBIE_TALLNUT_HEAD:
                    return 125;
                case SeedType::SEED_CACTUS:
                case SeedType::SEED_CABBAGEPULT:
                case SeedType::SEED_KERNELPULT:
                case SeedType::SEED_ZOMBIE_POLEVAULTER:
                case SeedType::SEED_ZOMBIE_PAIL:
                case SeedType::SEED_ZOMBIE_SCREEN_DOOR:
                case SeedType::SEED_ZOMBIE_JACK_IN_THE_BOX:
                case SeedType::SEED_ZOMBIE_BALLOON:
                case SeedType::SEED_ZOMBIE_WALLNUT_HEAD:
                    return 100;
                case SeedType::SEED_STARFRUIT:
                case SeedType::SEED_ZOMBONI:
                    return 175;
                case SeedType::SEED_INSTANT_COFFEE:
                case SeedType::SEED_ZOMBIE_NORMAL:
                case SeedType::SEED_ZOMBIE_DUCKY_TUBE:
                case SeedType::SEED_ZOMBIE_IMP:
                    return 25;
                case SeedType::SEED_MELONPULT:
                case SeedType::SEED_ZOMBIE_FLAG:
                    return 300;
                case SeedType::SEED_ZOMBIE_GRAVESTONE:
                case SeedType::SEED_ZOMBIE_TRASHCAN:
                case SeedType::SEED_ZOMBIE_NEWSPAPER:
                case SeedType::SEED_ZOMBIE_YETI:
                case SeedType::SEED_ZOMBIE_PEA_HEAD:
                case SeedType::SEED_ZOMBIE_SQUASH_HEAD:
                    return 50;
                case SeedType::SEED_ZOMBIE_POGO:
                    return 225;
                case SeedType::SEED_ZOMBIE_GARGANTUAR:
                    return 250;
                default:
                    return GetPlantDefinition(theSeedType).mSeedCost;
            }
        }
    }

    return old_Plant_GetCost(theSeedType, theImitaterType);
}

int Plant::GetRefreshTime(SeedType theSeedType, SeedType theImitaterType) {
    if (seedPacketFastCoolDown) {
        return 0;
    }

    LawnApp *gLawnApp = *gLawnApp_Addr;
    if (gLawnApp->mGameMode == GameMode::GAMEMODE_MP_VS) {
        if (theSeedType == SeedType::SEED_IMITATER && theImitaterType != SeedType::SEED_NONE) {
            theSeedType = theImitaterType;
        }
        int aRefreshTime;
        if (gVSBalanceAdjustment) {
            aRefreshTime = GetRefreshTimeAdjusted(theSeedType);
        } else {
            if (Challenge::IsMPSeedType(theSeedType)) {
                switch (theSeedType) {
                    case SeedType::SEED_ZOMBIE_TRASHCAN:
                    case SeedType::SEED_ZOMBIE_TRAFFIC_CONE:
                    case SeedType::SEED_ZOMBIE_POLEVAULTER:
                    case SeedType::SEED_ZOMBIE_PAIL:
                    case SeedType::SEED_ZOMBIE_FLAG:
                    case SeedType::SEED_ZOMBIE_FOOTBALL:
                    case SeedType::SEED_ZOMBIE_DANCER:
                    case SeedType::SEED_ZOMBIE_JACK_IN_THE_BOX:
                    case SeedType::SEED_ZOMBIE_DIGGER:
                    case SeedType::SEED_ZOMBIE_BUNGEE:
                    case SeedType::SEED_ZOMBIE_LADDER:
                    case SeedType::SEED_ZOMBIE_IMP:
                    case SeedType::SEED_ZOMBIE_BALLOON:
                    case SeedType::SEED_ZOMBIE_WALLNUT_HEAD:
                    case SeedType::SEED_ZOMBIE_JALAPENO_HEAD:
                    case SeedType::SEED_ZOMBIE_GATLINGPEA_HEAD:
                    case SeedType::SEED_ZOMBIE_TALLNUT_HEAD:
                        aRefreshTime = 3000;
                        break;
                    case SeedType::SEED_ZOMBIE_NEWSPAPER:
                    case SeedType::SEED_ZOMBIE_SCREEN_DOOR:
                    case SeedType::SEED_ZOMBIE_YETI:
                    case SeedType::SEED_ZOMBIE_SQUASH_HEAD:
                        aRefreshTime = 1500;
                        break;
                    case SeedType::SEED_ZOMBONI:
                    case SeedType::SEED_ZOMBIE_POGO:
                    case SeedType::SEED_ZOMBIE_CATAPULT:
                    case SeedType::SEED_ZOMBIE_GARGANTUAR:
                        aRefreshTime = 6000;
                        break;
                    default:
                        aRefreshTime = 750;
                        break;
                }
            } else {
                switch (theSeedType) {
                    case SeedType::SEED_CHERRYBOMB:
                    case SeedType::SEED_ICESHROOM:
                    case SeedType::SEED_DOOMSHROOM:
                    case SeedType::SEED_JALAPENO:
                        aRefreshTime = 6000;
                        break;
                    case SeedType::SEED_GRAVEBUSTER:
                    case SeedType::SEED_SQUASH:
                        aRefreshTime = 3000;
                        break;
                    case SeedType::SEED_THREEPEATER:
                    case SeedType::SEED_STARFRUIT:
                    case SeedType::SEED_MELONPULT:
                        aRefreshTime = 1500;
                        break;
                    default:
                        aRefreshTime = GetPlantDefinition(theSeedType).mRefreshTime;
                        break;
                }
            }
        }
        if (gLawnApp->mBoard->mChallenge->IsMPSuddenDeath() && *Challenge_gVSSuddenDeathMode_Addr == 1) {
            // sd不减冷却的卡片
            switch (theSeedType) { // 此处用switch-case替换旧的if-else，方便后续增删
                // 墓碑和向日葵，sd用不到
                case SeedType::SEED_ZOMBIE_GRAVESTONE:
                case SeedType::SEED_SUNFLOWER:
                // 默认五个不减cd的
                case SeedType::SEED_TALLNUT:
                case SeedType::SEED_WALLNUT:
                case SeedType::SEED_PUMPKINSHELL:
                case SeedType::SEED_ZOMBIE_TRASHCAN:
                case SeedType::SEED_ZOMBIE_SCREEN_DOOR:
                    return aRefreshTime;
                // 平衡调整后不减cd
                case SeedType::SEED_POTATOMINE:
                case SeedType::SEED_SQUASH:
                case SeedType::SEED_CHERRYBOMB:
                case SeedType::SEED_JALAPENO:
                case SeedType::SEED_DOOMSHROOM:
                case SeedType::SEED_ICESHROOM:
                    if (gVSBalanceAdjustment)
                        return aRefreshTime;
                default:
                    return aRefreshTime / 3;
            }
        }
        return aRefreshTime;
    }
    return old_Plant_GetRefreshTime(theSeedType, theImitaterType);
}

int Plant::GetCostAdjusted(SeedType theSeedType) {
    switch (theSeedType) {
        case SeedType::SEED_SUNSHROOM:
            return 0;
        case SeedType::SEED_INSTANT_COFFEE:
        case SeedType::SEED_ZOMBIE_NORMAL:
        case SeedType::SEED_ZOMBIE_DUCKY_TUBE:
        case SeedType::SEED_ZOMBIE_IMP:
            return 25;
        case SeedType::SEED_GRAVEBUSTER:  // 75 -> 50
        case SeedType::SEED_HYPNOSHROOM:  // 75 -> 50
        case SeedType::SEED_ICESHROOM:    // 75 -> 50
        case SeedType::SEED_BLOVER:       // 100->50
        case SeedType::SEED_PUMPKINSHELL: // 125 -> 50
        case SeedType::SEED_ZOMBIE_GRAVESTONE:
        case SeedType::SEED_ZOMBIE_TRASHCAN:
        case SeedType::SEED_ZOMBIE_TRAFFIC_CONE: // 75 -> 50
        case SeedType::SEED_ZOMBIE_NEWSPAPER:
        case SeedType::SEED_ZOMBIE_YETI:
        case SeedType::SEED_ZOMBIE_PEA_HEAD:
        case SeedType::SEED_ZOMBIE_SQUASH_HEAD:
            return 50;
        case SeedType::SEED_PEASHOOTER: // 100 -> 75
        case SeedType::SEED_KERNELPULT: // 100 -> 75
        case SeedType::SEED_SQUASH:
        case SeedType::SEED_GARLIC:
        case SeedType::SEED_ZOMBIE_POLEVAULTER:     // 100 -> 75
        case SeedType::SEED_ZOMBIE_JACK_IN_THE_BOX: // 100 -> 75
        case SeedType::SEED_ZOMBIE_SNORKEL:
            return 75;
        case SeedType::SEED_TALLNUT: // 125 -> 100
        case SeedType::SEED_CACTUS:
        case SeedType::SEED_SPLITPEA: // 125 -> 100
        case SeedType::SEED_ZOMBIE_PAIL:
        case SeedType::SEED_ZOMBIE_SCREEN_DOOR:
        case SeedType::SEED_ZOMBIE_DOLPHIN_RIDER:
        case SeedType::SEED_ZOMBIE_BALLOON:
        case SeedType::SEED_ZOMBIE_WALLNUT_HEAD:
            return 100;
        case SeedType::SEED_SNOWPEA: // 150 -> 125
        case SeedType::SEED_CHOMPER: // 150 -> 125
        case SeedType::SEED_JALAPENO:
        case SeedType::SEED_TORCHWOOD:
        case SeedType::SEED_ZOMBIE_DIGGER: // 150 -> 125
        case SeedType::SEED_ZOMBIE_BUNGEE:
        case SeedType::SEED_ZOMBIE_LADDER: // 150 -> 125
        case SeedType::SEED_ZOMBIE_JALAPENO_HEAD:
        case SeedType::SEED_ZOMBIE_GATLINGPEA_HEAD:
        case SeedType::SEED_ZOMBIE_TALLNUT_HEAD:
            return 125;
        case SeedType::SEED_REPEATER:
        case SeedType::SEED_ZOMBIE_FOOTBALL:
        case SeedType::SEED_ZOMBIE_DANCER:
        case SeedType::SEED_ZOMBONI:
        case SeedType::SEED_ZOMBIE_CATAPULT: // 200 -> 150
            return 150;
        case SeedType::SEED_DOOMSHROOM: // 125 -> 175
        case SeedType::SEED_STARFRUIT:
        case SeedType::SEED_ZOMBIE_POGO: // 225 -> 175
            return 175;
        case SeedType::SEED_MELONPULT:         // 300 -> 200
        case SeedType::SEED_ZOMBIE_FLAG:       // 300 -> 200
        case SeedType::SEED_ZOMBIE_GARGANTUAR: // 250 -> 200
            return 200;
        case SeedType::SEED_THREEPEATER: // 200 -> 225
            return 225;
            // return 250;
        default:
            return GetPlantDefinition(theSeedType).mSeedCost;
    }

    return GetPlantDefinition(theSeedType).mSeedCost;
}

int Plant::GetRefreshTimeAdjusted(SeedType theSeedType) {
    if (Challenge::IsMPSeedType(theSeedType)) {
        switch (theSeedType) {
            case SeedType::SEED_ZOMBONI:
            case SeedType::SEED_ZOMBIE_POGO:
            case SeedType::SEED_ZOMBIE_CATAPULT:
            case SeedType::SEED_ZOMBIE_GARGANTUAR:
                return 6000;
            case SeedType::SEED_ZOMBIE_TRASHCAN:
            case SeedType::SEED_ZOMBIE_TRAFFIC_CONE:
            case SeedType::SEED_ZOMBIE_PAIL:
            case SeedType::SEED_ZOMBIE_FLAG:
            case SeedType::SEED_ZOMBIE_FOOTBALL:
            case SeedType::SEED_ZOMBIE_DANCER:
            case SeedType::SEED_ZOMBIE_DIGGER:
            case SeedType::SEED_ZOMBIE_BUNGEE:
            case SeedType::SEED_ZOMBIE_LADDER:
            case SeedType::SEED_ZOMBIE_IMP:
            case SeedType::SEED_ZOMBIE_BALLOON:
            case SeedType::SEED_ZOMBIE_WALLNUT_HEAD:
            case SeedType::SEED_ZOMBIE_JALAPENO_HEAD:
            case SeedType::SEED_ZOMBIE_GATLINGPEA_HEAD:
            case SeedType::SEED_ZOMBIE_TALLNUT_HEAD:
                return 3000;
            case SeedType::SEED_ZOMBIE_POLEVAULTER: // 30 -> 15
            case SeedType::SEED_ZOMBIE_NEWSPAPER:
            case SeedType::SEED_ZOMBIE_SCREEN_DOOR:
            case SeedType::SEED_ZOMBIE_JACK_IN_THE_BOX: // 30 -> 15
            case SeedType::SEED_ZOMBIE_YETI:
            case SeedType::SEED_ZOMBIE_SQUASH_HEAD:
                return 1500;
            default:
                return 750;
        }
    } else {
        switch (theSeedType) {
            case SeedType::SEED_CHERRYBOMB:
            case SeedType::SEED_ICESHROOM:
            case SeedType::SEED_DOOMSHROOM:
            case SeedType::SEED_JALAPENO:
            case SeedType::SEED_PUMPKINSHELL: // 30 -> 60
            case SeedType::SEED_TALLNUT:      // 30 -> 60
                return 6000;
            case SeedType::SEED_GRAVEBUSTER:
            case SeedType::SEED_SQUASH:
                return 3000;
            case SeedType::SEED_PEASHOOTER: // 7.5 -> 15
            case SeedType::SEED_SNOWPEA:    // 7.5 -> 15
            case SeedType::SEED_REPEATER:   // 7.5 -> 15
            case SeedType::SEED_THREEPEATER:
            case SeedType::SEED_SPLITPEA: // 7.5 -> 15
            case SeedType::SEED_STARFRUIT:
            case SeedType::SEED_KERNELPULT: // 7.5 -> 15
            case SeedType::SEED_MELONPULT:
                return 1500;
            default:
                return GetPlantDefinition(theSeedType).mRefreshTime;
        }
    }
}

bool Plant::IsNocturnal(SeedType theSeedType) {
    return theSeedType == SeedType::SEED_PUFFSHROOM || theSeedType == SeedType::SEED_SEASHROOM || theSeedType == SeedType::SEED_SUNSHROOM || theSeedType == SeedType::SEED_FUMESHROOM
        || theSeedType == SeedType::SEED_HYPNOSHROOM || theSeedType == SeedType::SEED_DOOMSHROOM || theSeedType == SeedType::SEED_ICESHROOM || theSeedType == SeedType::SEED_MAGNETSHROOM
        || theSeedType == SeedType::SEED_SCAREDYSHROOM || theSeedType == SeedType::SEED_GLOOMSHROOM;
}

bool Plant::IsAquatic(SeedType theSeedType) {
    return theSeedType == SeedType::SEED_LILYPAD || theSeedType == SeedType::SEED_TANGLEKELP || theSeedType == SeedType::SEED_SEASHROOM || theSeedType == SeedType::SEED_CATTAIL;
}

bool Plant::IsFlying(SeedType theSeedType) {
    return theSeedType == SeedType::SEED_INSTANT_COFFEE;
}

bool Plant::IsUpgrade(SeedType theSeedType) {
    // 修复机枪射手在SeedBank光标移动到shop栏后变为绿卡。
    if (theSeedType == SeedType::SEED_GATLINGPEA) {
        LawnApp *lawnApp = *gLawnApp_Addr;
        Board *board = lawnApp->mBoard;
        if (board == nullptr) {
            return old_Plant_IsUpgrade(theSeedType); // 等价于直接return true;但方便改版修改所以返回旧函数
        }
        if (lawnApp->mSeedChooserScreen != nullptr) {
            return true;
        }
        GamepadControls *gamePad = board->mGamepadControls1;
        return !(gamePad->mGamepadState == 7 && gamePad->mIsInShopSeedBank);
    }
    return old_Plant_IsUpgrade(theSeedType);
}

Rect Plant::GetPlantRect() {
    Rect aRect;
    if (mSeedType == SeedType::SEED_TALLNUT) {
        aRect = Rect(mX + 10, mY, mWidth, mHeight);
    } else if (mSeedType == SeedType::SEED_PUMPKINSHELL) {
        aRect = Rect(mX, mY, mWidth - 20, mHeight);
    } else if (mSeedType == SeedType::SEED_COBCANNON) {
        aRect = Rect(mX, mY, 140, 80);
    } else {
        aRect = Rect(mX + 10, mY, mWidth - 20, mHeight);
    }

    return aRect;
}

Rect Plant::GetPlantAttackRect(PlantWeapon thePlantWeapon) {
    Rect aRect;
    if (mApp->IsWallnutBowlingLevel()) {
        aRect = Rect(mX, mY, mWidth - 20, mHeight);
    } else if (thePlantWeapon == PlantWeapon::WEAPON_SECONDARY && mSeedType == SeedType::SEED_SPLITPEA) {
        aRect = Rect(0, mY, mX + 16, mHeight);
    } else
        switch (mSeedType) {
            case SeedType::SEED_LEFTPEATER:
                aRect = Rect(0, mY, mX, mHeight);
                break;
            case SeedType::SEED_SQUASH:
                aRect = Rect(mX + 20, mY, mWidth - 35, mHeight);
                break;
            case SeedType::SEED_CHOMPER:
                aRect = Rect(mX + 80, mY, 40, mHeight);
                break;
            case SeedType::SEED_SPIKEWEED:
            case SeedType::SEED_SPIKEROCK:
                aRect = Rect(mX + 20, mY, mWidth - 50, mHeight);
                break;
            case SeedType::SEED_POTATOMINE:
                aRect = Rect(mX, mY, mWidth - 25, mHeight);
                break;
            case SeedType::SEED_TORCHWOOD:
                aRect = Rect(mX + 50, mY, 30, mHeight);
                break;
            case SeedType::SEED_PUFFSHROOM:
            case SeedType::SEED_SEASHROOM:
                aRect = Rect(mX + 60, mY, 230, mHeight);
                break;
            case SeedType::SEED_FUMESHROOM:
                aRect = Rect(mX + 60, mY, 340, mHeight);
                break;
            case SeedType::SEED_GLOOMSHROOM:
                aRect = Rect(mX - 80, mY - 80, 240, 240);
                break;
            case SeedType::SEED_TANGLEKELP:
                aRect = Rect(mX, mY, mWidth, mHeight);
                break;
            case SeedType::SEED_CATTAIL:
                aRect = Rect(-BOARD_WIDTH, -BOARD_HEIGHT, BOARD_WIDTH * 2, BOARD_HEIGHT * 2);
                break;
            default:
                aRect = Rect(mX + 60, mY, BOARD_WIDTH, mHeight);
                break;
        }

    return aRect;
}

Image *Plant::GetImage(SeedType theSeedType) {
    Image **aImages = GetPlantDefinition(theSeedType).mPlantImage;
    return aImages ? aImages[0] : nullptr;
}

void Plant::SetImitaterFilterEffect() {
    FilterEffect aFilterEffect = GetFilterEffectTypeBySeedType(mSeedType);
    Reanimation *mBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (mBodyReanim != nullptr)
        mBodyReanim->mFilterEffect = aFilterEffect;
    Reanimation *mHeadReanim = mApp->ReanimationTryToGet(mHeadReanimID);
    if (mHeadReanim != nullptr)
        mHeadReanim->mFilterEffect = aFilterEffect;
    Reanimation *mHeadReanim2 = mApp->ReanimationTryToGet(mHeadReanimID2);
    if (mHeadReanim2 != nullptr)
        mHeadReanim2->mFilterEffect = aFilterEffect;
    Reanimation *mHeadReanim3 = mApp->ReanimationTryToGet(mHeadReanimID3);
    if (mHeadReanim3 != nullptr)
        mHeadReanim3->mFilterEffect = aFilterEffect;
}

bool Plant::DrawMagnetItemsOnTop() {
    if (mSeedType == SeedType::SEED_GOLD_MAGNET) {
        for (int i = 0; i < MAX_MAGNET_ITEMS; i++) {
            if (mMagnetItems[i].mItemType != MagnetItemType::MAGNET_ITEM_NONE) {
                return true;
            }
        }

        return false;
    }

    if (mSeedType == SeedType::SEED_MAGNETSHROOM) {
        for (int i = 0; i < MAX_MAGNET_ITEMS; i++) {
            MagnetItem *aMagnetItem = &mMagnetItems[i];
            if (aMagnetItem->mItemType != MagnetItemType::MAGNET_ITEM_NONE) {
                SexyVector2 aVectorToPlant(mX + aMagnetItem->mDestOffsetX - aMagnetItem->mPosX, mY + aMagnetItem->mDestOffsetY - aMagnetItem->mPosY);
                if (aVectorToPlant.Magnitude() > 20.0f) {
                    return true;
                }
            }
        }

        return false;
    }

    return false;
}

void Plant::BurnRow(int theRow) {
    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);

    Zombie *aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie)) {
        if ((aZombie->mZombieType == ZombieType::ZOMBIE_BOSS || aZombie->mRow == theRow) && aZombie->EffectedByDamage(aDamageRangeFlags)) {
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
        // 注：原版中将 Zombie::BossDestroyIceballInRow(int) 函数改为了 Zombie::BossDestroyIceball()，冰球是否位于目标行的判断则移动至此处进行
        aBossZombie->BossDestroyIceballInRow(theRow);
    }
}

bool Plant::MakesSun() {
    return mSeedType == SeedType::SEED_SUNFLOWER || mSeedType == SeedType::SEED_TWINSUNFLOWER || mSeedType == SeedType::SEED_SUNSHROOM;
}

void Plant::UpdateProductionPlant() {
    if (mApp->mGameMode == GAMEMODE_MP_VS && (tcp_connected || tcpClientSocket >= 0)) {
        if (!IsInPlay()) {
            return;
        }
        if (mApp->IsIZombieLevel() || mApp->mGameMode == GameMode::GAMEMODE_UPSELL || mApp->mGameMode == GameMode::GAMEMODE_INTRO) {
            return;
        }
        if (mBoard->HasLevelAwardDropped()) {
            return;
        }
        if (mSeedType == SeedType::SEED_MARIGOLD && mBoard->mCurrentWave == mBoard->mNumWaves) {
            if (mState != PlantState::STATE_MARIGOLD_ENDING) {
                mState = PlantState::STATE_MARIGOLD_ENDING;
                mStateCountdown = 6000;
            } else if (mStateCountdown <= 0) {
                return;
            }
        }
        if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_LAST_STAND && mBoard->mChallenge->mChallengeState != ChallengeState::STATECHALLENGE_LAST_STAND_ONSLAUGHT) {
            return;
        }
        // mLaunchCounter -= 3;
        mLaunchCounter--;
        if (mLaunchCounter <= 100) {
            int num = TodAnimateCurve(100, 0, mLaunchCounter, 0, 100, TodCurves::CURVE_LINEAR);
            mEatenFlashCountdown = mEatenFlashCountdown > num ? mEatenFlashCountdown : num;
        }
        if (mLaunchCounter <= 0)
        // 生产
        {
            if (tcp_connected) {
                return;
            }
            mLaunchCounter = RandRangeInt(mLaunchRate - 150, mLaunchRate);
            if (tcpClientSocket >= 0) {
                U16U16_Event event = {{EventType::EVENT_SERVER_BOARD_PLANT_LAUNCHCOUNTER}, uint16_t(mBoard->mPlants.DataArrayGetID(this)), uint16_t(mLaunchCounter)};
                send(tcpClientSocket, &event, sizeof(U16U16_Event), 0);
            }
            mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
            if (mSeedType == SeedType::SEED_SUNSHROOM) {
                if (mState == PlantState::STATE_SUNSHROOM_SMALL) {
                    mBoard->AddCoin(mX, mY, CoinType::COIN_SMALLSUN, CoinMotion::COIN_MOTION_FROM_PLANT);
                } else {
                    mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
                }
            } else if (mSeedType == SeedType::SEED_SUNFLOWER) {
                mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
            } else if (mSeedType == SeedType::SEED_TWINSUNFLOWER) {
                mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
                mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
            } else if (mSeedType == SeedType::SEED_MARIGOLD) {
                int num2 = Sexy::Rand(100);
                CoinType theCoinType = CoinType::COIN_SILVER;
                if (num2 < 10) {
                    theCoinType = CoinType::COIN_GOLD;
                }
                mBoard->AddCoin(mX, mY, theCoinType, CoinMotion::COIN_MOTION_COIN);
            }
            if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BIG_TIME) {
                if (mSeedType == SeedType::SEED_SUNFLOWER) {
                    mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
                    return;
                }
                if (mSeedType == SeedType::SEED_MARIGOLD) {
                    mBoard->AddCoin(mX, mY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
                }
            }
        }
        return;
    }

    old_Plant_UpdateProductionPlant(this);
}

void Plant::UpdateShooting() {
    if (NotOnGround() || mShootingCounter == 0)
        return;

    mShootingCounter--;

    if (mSeedType == SeedType::SEED_FUMESHROOM && mShootingCounter == 15) {
        int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mRow, 0);
        AddAttachedParticle(mX + 85, mY + 31, aRenderPosition, ParticleEffect::PARTICLE_FUMECLOUD);
    }

    if (mSeedType == SeedType::SEED_GLOOMSHROOM) {
        if (mShootingCounter == 136 || mShootingCounter == 108 || mShootingCounter == 80 || mShootingCounter == 52) {
            int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mRow, 0);
            AddAttachedParticle(mX + 40, mY + 40, aRenderPosition, ParticleEffect::PARTICLE_GLOOMCLOUD);
        }
        if (mShootingCounter == 126 || mShootingCounter == 98 || mShootingCounter == 70 || mShootingCounter == 42) {
            Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY, nullptr);
        }
    } else if (mSeedType == SeedType::SEED_GATLINGPEA) {
        if (mShootingCounter == 18 || mShootingCounter == 35 || mShootingCounter == 51 || mShootingCounter == 68) {
            Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY, nullptr);
        }
    } else if (mSeedType == SeedType::SEED_CATTAIL) {
        if (mShootingCounter == 19) {
            Zombie *aZombie = FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY);
            if (aZombie) {
                Fire(aZombie, mRow, PlantWeapon::WEAPON_PRIMARY, nullptr);
            }
        }
    } else if (mShootingCounter == 1) {
        if (mSeedType == SeedType::SEED_THREEPEATER) {
            int rowAbove = mRow - 1;
            int rowBelow = mRow + 1;
            Reanimation *aHeadReanim2 = mApp->ReanimationGet(mHeadReanimID2);
            Reanimation *aHeadReanim3 = mApp->ReanimationGet(mHeadReanimID3);
            Reanimation *aHeadReanim1 = mApp->ReanimationGet(mHeadReanimID);

            if (aHeadReanim1->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD) {
                Fire(nullptr, rowBelow, PlantWeapon::WEAPON_PRIMARY, nullptr);
            }
            if (aHeadReanim2->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD) {
                Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY, nullptr);
            }
            if (aHeadReanim3->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD) {
                Fire(nullptr, rowAbove, PlantWeapon::WEAPON_PRIMARY, nullptr);
            }
        } else if (mSeedType == SeedType::SEED_SPLITPEA) {
            Reanimation *aHeadBackReanim = mApp->ReanimationTryToGet(mHeadReanimID2);
            Reanimation *aHeadFrontReanim = mApp->ReanimationTryToGet(mHeadReanimID);
            if (aHeadFrontReanim->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD) {
                Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY, nullptr);
            }
            if (aHeadBackReanim->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD) {
                Fire(nullptr, mRow, PlantWeapon::WEAPON_SECONDARY, nullptr);
            }
        } else if (mState == PlantState::STATE_CACTUS_LOW) {
            Fire(nullptr, mRow, PlantWeapon::WEAPON_SECONDARY, nullptr);
        } else if (mSeedType == SeedType::SEED_CABBAGEPULT || mSeedType == SeedType::SEED_KERNELPULT || mSeedType == SeedType::SEED_MELONPULT || mSeedType == SeedType::SEED_WINTERMELON) {
            PlantWeapon aPlantWeapon = PlantWeapon::WEAPON_PRIMARY;
            if (mState == PlantState::STATE_KERNELPULT_BUTTER) {
                Reanimation *aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
                aBodyReanim->AssignRenderGroupToPrefix("Cornpult_butter", RENDER_GROUP_HIDDEN);
                aBodyReanim->AssignRenderGroupToPrefix("Cornpult_kernal", RENDER_GROUP_NORMAL);
                mState = PlantState::STATE_NOTREADY;
                aPlantWeapon = PlantWeapon::WEAPON_SECONDARY;
            }

            Zombie *aZombie = FindTargetZombie(mRow, aPlantWeapon);
            GridItem *aGridItem = FindTargetGridItem(aPlantWeapon);
            Fire(aZombie, mRow, aPlantWeapon, aGridItem);
        } else {
            Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY, nullptr);
        }

        return;
    }

    if (mShootingCounter != 0)
        return;

    Reanimation *aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    Reanimation *aHeadReanim = mApp->ReanimationTryToGet(mHeadReanimID);
    if (mSeedType == SeedType::SEED_THREEPEATER) {
        Reanimation *aHeadReanim2 = mApp->ReanimationGet(mHeadReanimID2);
        Reanimation *aHeadReanim3 = mApp->ReanimationGet(mHeadReanimID3);

        if (aHeadReanim2->mLoopCount > 0) {
            if (aHeadReanim->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD) {
                aHeadReanim->StartBlend(20);
                aHeadReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
                aHeadReanim->SetFramesForLayer("anim_head_idle1");
                aHeadReanim->mAnimRate = aBodyReanim->mAnimRate;
                aHeadReanim->mAnimTime = aBodyReanim->mAnimTime;
            }

            aHeadReanim2->StartBlend(20);
            aHeadReanim2->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim2->SetFramesForLayer("anim_head_idle2");
            aHeadReanim2->mAnimRate = aBodyReanim->mAnimRate;
            aHeadReanim2->mAnimTime = aBodyReanim->mAnimTime;

            if (aHeadReanim3->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD) {
                aHeadReanim3->StartBlend(20);
                aHeadReanim3->mLoopType = ReanimLoopType::REANIM_LOOP;
                aHeadReanim3->SetFramesForLayer("anim_head_idle3");
                aHeadReanim3->mAnimRate = aBodyReanim->mAnimRate;
                aHeadReanim3->mAnimTime = aBodyReanim->mAnimTime;
            }

            return;
        }
    } else if (mSeedType == SeedType::SEED_SPLITPEA) {
        Reanimation *aHeadReanim2 = mApp->ReanimationGet(mHeadReanimID2);

        if (aHeadReanim->mLoopCount > 0) {
            aHeadReanim->StartBlend(20);
            aHeadReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim->SetFramesForLayer("anim_head_idle");
            aHeadReanim->mAnimRate = aBodyReanim->mAnimRate;
            aHeadReanim->mAnimTime = aBodyReanim->mAnimTime;
        }

        if (aHeadReanim2->mLoopCount > 0) {
            aHeadReanim2->StartBlend(20);
            aHeadReanim2->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim2->SetFramesForLayer("anim_splitpea_idle");
            aHeadReanim2->mAnimRate = aBodyReanim->mAnimRate;
            aHeadReanim2->mAnimTime = aBodyReanim->mAnimTime;
        }

        return;
    } else if (mState == PlantState::STATE_CACTUS_HIGH) {
        if (aBodyReanim->mLoopCount > 0) {
            PlayBodyReanim("anim_idlehigh", ReanimLoopType::REANIM_LOOP, 20, 0.0f);

            aBodyReanim->mAnimRate = aBodyReanim->mDefinition->mFPS;
            if (mApp->IsIZombieLevel()) {
                aBodyReanim->mAnimRate = 0.0f;
            }

            return;
        }
    } else if (aHeadReanim) {
        if (aHeadReanim->mLoopCount > 0) {
            aHeadReanim->StartBlend(20);
            aHeadReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim->SetFramesForLayer("anim_head_idle");
            aHeadReanim->mAnimRate = aBodyReanim->mAnimRate;
            aHeadReanim->mAnimTime = aBodyReanim->mAnimTime;
            return;
        }
    } else if (mSeedType == SeedType::SEED_COBCANNON) {
        if (aBodyReanim->mLoopCount > 0) {
            mState = PlantState::STATE_COBCANNON_ARMING;
            mStateCountdown = 3000;
            PlayBodyReanim("anim_unarmed_idle", ReanimLoopType::REANIM_LOOP, 20, aBodyReanim->mDefinition->mFPS);
            return;
        }
    } else if (aBodyReanim && aBodyReanim->mLoopCount > 0) {
        PlayIdleAnim(aBodyReanim->mDefinition->mFPS);
        return;
    }

    mShootingCounter = 1;
}

void Plant::UpdateShooter() {
    mLaunchCounter--;
    if (mLaunchCounter <= 0) {
        if (!(mApp->IsVSMode() && tcp_connected)) {
            mLaunchCounter = mLaunchRate - Sexy::Rand(15);
        }
        if (tcpClientSocket >= 0) {
            U16U16_Event event = {{EventType::EVENT_SERVER_BOARD_PLANT_LAUNCHCOUNTER}, uint16_t(mBoard->mPlants.DataArrayGetID(this)), uint16_t(mLaunchCounter)};
            send(tcpClientSocket, &event, sizeof(U16U16_Event), 0);
        }

        if (mSeedType == SeedType::SEED_THREEPEATER) {
            LaunchThreepeater();
        } else if (mSeedType == SeedType::SEED_STARFRUIT) {
            LaunchStarFruit();
        } else if (mSeedType == SeedType::SEED_SPLITPEA) {
            FindTargetAndFire(mRow, PlantWeapon::WEAPON_SECONDARY);
            Reanimation *aHeadReanim = mApp->ReanimationGet(mHeadReanimID);
            Reanimation *aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
            aHeadReanim->StartBlend(20);
            aHeadReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim->SetFramesForLayer("anim_head_idle");
            aHeadReanim->SetAnimRate(aBodyReanim->mAnimRate);
            aHeadReanim->mAnimTime = aBodyReanim->mAnimTime;
        } else if (mSeedType == SeedType::SEED_CACTUS) {
            if (mState == PlantState::STATE_CACTUS_HIGH) {
                FindTargetAndFire(mRow, PlantWeapon::WEAPON_PRIMARY);
            } else if (mState == PlantState::STATE_CACTUS_LOW) {
                FindTargetAndFire(mRow, PlantWeapon::WEAPON_SECONDARY);
            }
        } else {
            FindTargetAndFire(mRow, PlantWeapon::WEAPON_PRIMARY);
        }
    }

    if (mLaunchCounter == 50 && mSeedType == SeedType::SEED_CATTAIL) {
        FindTargetAndFire(mRow, PlantWeapon::WEAPON_PRIMARY);
    }
    if (mLaunchCounter == 25) {
        if (mSeedType == SeedType::SEED_REPEATER || mSeedType == SeedType::SEED_LEFTPEATER) {
            FindTargetAndFire(mRow, PlantWeapon::WEAPON_PRIMARY);
        } else if (mSeedType == SeedType::SEED_SPLITPEA) {
            FindTargetAndFire(mRow, PlantWeapon::WEAPON_PRIMARY);
            FindTargetAndFire(mRow, PlantWeapon::WEAPON_SECONDARY);
        }
    }
}

void Plant::PlayIdleAnim(float theRate) {
    Reanimation *aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim) {
        PlayBodyReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 20, theRate);
        if (mApp->IsIZombieLevel()) {
            aBodyReanim->mAnimRate = 0.0f;
        }
    }
}

void Plant::IceZombies() {
    Zombie *aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie)) {
        aZombie->HitIceTrap();
    }

    mBoard->mIceTrapCounter = 300;
    TodParticleSystem *aPoolSparklyParticle = mApp->ParticleTryToGet(mBoard->mPoolSparklyParticleID);
    if (aPoolSparklyParticle) {
        aPoolSparklyParticle->mDontUpdate = false;
    }

    Zombie *aBossZombie = nullptr;
    while (mBoard->IterateZombies(aBossZombie)) {
        if (aBossZombie->mZombieType == ZOMBIE_BOSS) {
            aBossZombie->BossDestroyFireball();
        }
    }
}

bool Plant::IsDisposable(SeedType theSeedType) {
    return theSeedType == SeedType::SEED_CHERRYBOMB || theSeedType == SeedType::SEED_JALAPENO || theSeedType == SeedType::SEED_HYPNOSHROOM || theSeedType == SeedType::SEED_ICESHROOM
        || theSeedType == SeedType::SEED_ICESHROOM || theSeedType == SeedType::SEED_ICESHROOM;
}