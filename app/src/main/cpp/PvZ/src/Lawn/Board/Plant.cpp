#include "PvZ/Lawn/Board/Plant.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Board/Challenge.h"
#include "PvZ/Lawn/Board/CutScene.h"
#include "PvZ/Lawn/Board/GridItem.h"
#include "PvZ/Lawn/GamepadControls.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/System/ReanimationLawn.h"
#include "PvZ/MagicAddr.h"
#include "PvZ/Misc.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/SexyAppFramework/Misc/SexyVector.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Effect/Reanimator.h"

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

    // 在对战模式修改指定植物的血量
    if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS) {
        switch (theSeedType) {
            case SeedType::SEED_SUNFLOWER:
                mPlantMaxHealth = 300;
                break;
            case SeedType::SEED_PEASHOOTER:
                mPlantMaxHealth = 300;
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

    return old_Plant_SetSleeping(this, theIsAsleep);
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
    return old_Plant_PlayBodyReanim(this, theTrackName, theLoopType, theBlendTime, theAnimRate);
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
    //     Plant_UpdateAbilities(plant);
    //     Plant_Animate(plant);
    //     if (plant->mPlantHealth < 0)
    //         Plant_Die(plant);
    //     Plant_UpdateReanim(plant);

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
            //            if (plant->mGloveGrabbed)
            //            {
            //                SetColorizeImages(g,true);
            //                Color color = {150, 255, 150, 255};
            //                SetColor(g,&color);
            //            }
            reanimation2->DrawRenderGroup(g, 0);
            //            if (plant->mGloveGrabbed)
            //            {
            //                SetColorizeImages(g,false);
            //            }
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
        //        if (mSeedType == a::Sprout)
        //        {
        //            if (plant->mGloveGrabbed)
        //            {
        //                SetColorizeImages(g,true);
        //                Color color ={150, 255, 150, 255};
        //                SetColor(g, &color);
        //            }
        //            TodDrawImageCelF(g, AtlasResources.IMAGE_CACHED_MARIGOLD, Constants.ZenGarden_Marigold_Sprout_Offset.X, Constants.ZenGarden_Marigold_Sprout_Offset.Y, 0, 0);
        //            if (plant->mGloveGrabbed)
        //            {
        //                SetColorizeImages(g,false);
        //            }
        //        }
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
    LawnApp *lawnApp = (LawnApp *)*gLawnApp_Addr;
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

void Plant::DoSpecial() {
    // 试图修复辣椒爆炸后反而在本行的末尾处产生冰道。失败。

    if (mSeedType == SeedType::SEED_CHERRYBOMB) {
        // 用于成就
        int num1 = mBoard->GetLiveZombiesCount();
        old_Plant_DoSpecial(this);
        int num2 = mBoard->GetLiveZombiesCount();
        if (num1 - num2 >= 10 && !mApp->IsLittleTroubleLevel()) {
            mBoard->GrantAchievement(AchievementId::ACHIEVEMENT_EXPLODONATOR, true);
        }
        return;
    }
    return old_Plant_DoSpecial(this);
}

// void Plant_CobCannonFire(Plant *plant, int x, int y) {
//     LOGD("fire:%d %d",x,y);
//     old_Plant_CobCannonFire(plant,x,y);
// }

GridItem *Plant::FindTargetGridItem(PlantWeapon thePlantWeapon) {
    // 对战模式专用，植物索敌僵尸墓碑和靶子僵尸。
    // 原版函数BUG：植物还会索敌梯子和毁灭菇弹坑，故重写以修复BUG。
    GridItem *aGridItem = nullptr;
    GridItem *aTargetGridItem = nullptr;
    int aLastGridX = 0;
    if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS) { // 如果是对战模式(关卡ID为76)
        int mRow = mStartRow;
        while (mBoard->IterateGridItems(aGridItem)) { // 遍历场上的所有GridItem

            GridItemType mGridItemType = aGridItem->mGridItemType;
            if (mGridItemType != GridItemType::GRIDITEM_GRAVESTONE && mGridItemType != GridItemType::GRIDITEM_VS_TARGET_ZOMBIE) {
                // 修复植物们攻击核坑和梯子
                continue;
            }
            int mGridX = aGridItem->mGridX;
            int mGridY = aGridItem->mGridY;
            if (mSeedType == SeedType::SEED_THREEPEATER ? abs(mGridY - mRow) > 1 : mGridY != mRow) {
                // 如果是三线射手，则索敌三行; 反之，索敌一行
                // 注释掉此行，就会发现投手能够命中三格内的靶子了，但会导致很多其他BUG。尚不清楚原因。
                continue;
            }


            if (aTargetGridItem == nullptr || mGridX < aLastGridX) {
                if (mSeedType == SeedType::SEED_FUMESHROOM && mGridX - mPlantCol > 3) {
                    // 如果是大喷菇，则索敌三格以内的靶子或墓碑
                    continue;
                }
                if (mSeedType == SeedType::SEED_PUFFSHROOM || mSeedType == SeedType::SEED_SEASHROOM) {
                    // 如果是小喷菇或水兵菇，则索敌三格以内的墓碑
                    if (mGridX - mPlantCol > 3) {
                        continue;
                    }
                    // 不主动攻击靶子
                    if (mGridItemType == GridItemType::GRIDITEM_VS_TARGET_ZOMBIE) {
                        continue;
                    }
                }
                aTargetGridItem = aGridItem;
                aLastGridX = mGridX;
            }
        }
    }
    return aTargetGridItem;
}

void Plant::Die() {
    //    //某植物死亡触发辣椒效果（以双发为例）
    //    if (mSeedType == SeedType::SEED_REPEATER) {
    //        // 方法一：直接调用辣椒DoSpecial相关函数
    //        mApp->PlayFoley(FoleyType::FOLEY_JALAPENO_IGNITE); // 播放音效
    //        mApp->PlayFoley(FoleyType::FOLEY_JUICY);
    //
    //        mBoard->DoFwoosh(mRow); // 生成火焰动画
    //        mBoard->ShakeBoard(3, -4); // 屏幕震动
    //
    //        BurnRow(mRow); // 点燃本行的僵尸，移除梯子、冰球
    //        mBoard->mIceTimer[mRow] = 20; // 移除冰道
    ///*******************************************************************************************************************************************/
    //        // 方法二：召唤辣椒并瞬爆
    //        Plant *aPlant = mBoard->AddPlant(mPlantCol, mRow, SeedType::SEED_JALAPENO, SeedType::SEED_NONE, 0, true); // 生成辣椒
    //        aPlant->mDoSpecialCountdown = 1; // 辣椒爆炸倒计时
    //    }

    old_Plant_Die(this);
}

PlantDefinition &GetPlantDefinition(SeedType theSeedType) {
    return gPlantDefs[theSeedType];
}

int Plant::GetCost(SeedType theSeedType, SeedType theImitaterType) {
    LawnApp *lawnApp = (LawnApp *)*gLawnApp_Addr;
    if (lawnApp->mGameMode == GameMode::GAMEMODE_MP_VS) {
        if (theSeedType == SeedType::SEED_IMITATER && theImitaterType != SeedType::SEED_NONE) {
            theSeedType = theImitaterType;
        }
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
                return 125;
            case SeedType::SEED_CACTUS:
            case SeedType::SEED_CABBAGEPULT:
            case SeedType::SEED_KERNELPULT:
            case SeedType::SEED_ZOMBIE_POLEVAULTER:
            case SeedType::SEED_ZOMBIE_PAIL:
            case SeedType::SEED_ZOMBIE_SCREEN_DOOR:
            case SeedType::SEED_ZOMBIE_JACK_IN_THE_BOX:
            case SeedType::SEED_ZOMBIE_DUCKY_TUBE:
                return 100;
            case SeedType::SEED_STARFRUIT:
            case SeedType::SEED_ZOMBONI:
                return 175;
            case SeedType::SEED_INSTANT_COFFEE:
            case SeedType::SEED_ZOMBIE_NORMAL:
                return 25;
            case SeedType::SEED_MELONPULT:
            case SeedType::SEED_ZOMBIE_FLAG:
            case SeedType::SEED_ZOMBIE_UNKNOWN:
            case SeedType::SEED_ZOMBIE_BALLOON:
                return 300;
            case SeedType::SEED_ZOMBIE_TOMBSTONE:
            case SeedType::SEED_ZOMBIE_TRASH_BIN:
            case SeedType::SEED_ZOMBIE_NEWSPAPER:
            case SeedType::SEED_ZOMBIE_IMP:
                return 50;
            case SeedType::SEED_ZOMBIE_POGO:
                return 225;
            case SeedType::SEED_ZOMBIE_GARGANTUAR:
            case SeedType::SEED_ZOMBIE_DOLPHIN_RIDER:
                return 250;
            default:
                return GetPlantDefinition(theSeedType).mSeedCost;
        }
    }
    return old_Plant_GetCost(theSeedType, theImitaterType);
}

int Plant::GetRefreshTime(SeedType theSeedType, SeedType theImitaterType) {
    if (seedPacketFastCoolDown) {
        return 0;
    }
    LawnApp *lawnApp = (LawnApp *)*gLawnApp_Addr;
    if (lawnApp->mGameMode == GameMode::GAMEMODE_MP_VS) {
        int refreshTime;
        if (theSeedType == SeedType::SEED_IMITATER && theImitaterType != SeedType::SEED_NONE) {
            theSeedType = theImitaterType;
        }
        if (Challenge::IsMPSeedType(theSeedType)) {
            switch (theSeedType) {
                case SeedType::SEED_ZOMBIE_TRASH_BIN:
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
                case SeedType::SEED_ZOMBIE_BALLOON:
                    refreshTime = 3000;
                    break;
                case SeedType::SEED_ZOMBIE_NEWSPAPER:
                case SeedType::SEED_ZOMBIE_SCREEN_DOOR:
                    refreshTime = 1500;
                    break;
                case SeedType::SEED_ZOMBONI:
                case SeedType::SEED_ZOMBIE_POGO:
                case SeedType::SEED_ZOMBIE_CATAPULT:
                case SeedType::SEED_ZOMBIE_GARGANTUAR:
                case SeedType::SEED_ZOMBIE_UNKNOWN:
                    refreshTime = 6000;
                    break;
                default:
                    refreshTime = 750;
                    break;
            }
        } else {
            switch (theSeedType) {
                case SeedType::SEED_CHERRYBOMB:
                case SeedType::SEED_ICESHROOM:
                case SeedType::SEED_DOOMSHROOM:
                case SeedType::SEED_JALAPENO:
                    refreshTime = 6000;
                    break;
                case SeedType::SEED_GRAVEBUSTER:
                case SeedType::SEED_SQUASH:
                    refreshTime = 3000;
                    break;
                case SeedType::SEED_THREEPEATER:
                case SeedType::SEED_STARFRUIT:
                case SeedType::SEED_MELONPULT:
                    refreshTime = 1500;
                    break;
                default:
                    refreshTime = GetPlantDefinition(theSeedType).mRefreshTime;
                    break;
            }
        }
        if (lawnApp->mBoard->mChallenge->IsMPSuddenDeath() && *Challenge_gVSSuddenDeathMode_Addr == 1) {
            if (theSeedType == SeedType::SEED_ZOMBIE_TOMBSTONE || theSeedType == SeedType::SEED_SUNFLOWER || theSeedType == SeedType::SEED_TALLNUT || theSeedType == SeedType::SEED_WALLNUT
                || theSeedType == SeedType::SEED_ZOMBIE_SCREEN_DOOR || theSeedType == SeedType::SEED_PUMPKINSHELL || theSeedType == SeedType::SEED_ZOMBIE_TRASH_BIN) {
                return refreshTime;
            }
            refreshTime /= 3;
        }
        return refreshTime;
    }
    return old_Plant_GetRefreshTime(theSeedType, theImitaterType);
}

bool Plant::IsNocturnal(SeedType theSeedtype) {
    return theSeedtype == SeedType::SEED_PUFFSHROOM || theSeedtype == SeedType::SEED_SEASHROOM || theSeedtype == SeedType::SEED_SUNSHROOM || theSeedtype == SeedType::SEED_FUMESHROOM
        || theSeedtype == SeedType::SEED_HYPNOSHROOM || theSeedtype == SeedType::SEED_DOOMSHROOM || theSeedtype == SeedType::SEED_ICESHROOM || theSeedtype == SeedType::SEED_MAGNETSHROOM
        || theSeedtype == SeedType::SEED_SCAREDYSHROOM || theSeedtype == SeedType::SEED_GLOOMSHROOM;
}

bool Plant::IsAquatic(SeedType theSeedType) {
    return theSeedType == SeedType::SEED_LILYPAD || theSeedType == SeedType::SEED_TANGLEKELP || theSeedType == SeedType::SEED_SEASHROOM || theSeedType == SeedType::SEED_CATTAIL;
}

bool Plant::IsFlying(SeedType theSeedtype) {
    return theSeedtype == SeedType::SEED_INSTANT_COFFEE;
}

bool Plant::IsUpgrade(SeedType theSeedType) {
    // 修复机枪射手在SeedBank光标移动到shop栏后变为绿卡。
    if (theSeedType == SeedType::SEED_GATLINGPEA) {
        LawnApp *lawnApp = (LawnApp *)*gLawnApp_Addr;
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