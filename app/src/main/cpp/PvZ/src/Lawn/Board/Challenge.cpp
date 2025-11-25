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

#include "PvZ/Lawn/Board/Challenge.h"
#include "Homura/Logger.h"
#include "PvZ/Android/IntroVideo.h"
#include "PvZ/Android/Native/NativeApp.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Board/GridItem.h"
#include "PvZ/Lawn/Board/Plant.h"
#include "PvZ/Lawn/Board/SeedBank.h"
#include "PvZ/Lawn/Board/SeedPacket.h"
#include "PvZ/Lawn/Board/Zombie.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/Widget/GameButton.h"
#include "PvZ/Misc.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Common/TodStringFile.h"
#include "PvZ/TodLib/Effect/Reanimator.h"

#include <cstddef>

using namespace Sexy;

SeedType gArtChallengeWallnut[MAX_GRID_SIZE_Y][MAX_GRID_SIZE_X] = {
    {SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE, SEED_WALLNUT, SEED_WALLNUT, SEED_WALLNUT, SEED_NONE, SEED_NONE},
    {SEED_NONE, SEED_NONE, SEED_NONE, SEED_WALLNUT, SEED_NONE, SEED_NONE, SEED_NONE, SEED_WALLNUT, SEED_NONE},
    {SEED_NONE, SEED_NONE, SEED_NONE, SEED_WALLNUT, SEED_NONE, SEED_NONE, SEED_NONE, SEED_WALLNUT, SEED_NONE},
    {SEED_NONE, SEED_NONE, SEED_NONE, SEED_WALLNUT, SEED_NONE, SEED_NONE, SEED_NONE, SEED_WALLNUT, SEED_NONE},
    {SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE, SEED_WALLNUT, SEED_WALLNUT, SEED_WALLNUT, SEED_NONE, SEED_NONE},
    {SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE},
};

SeedType gArtChallengeSunFlower[MAX_GRID_SIZE_Y][MAX_GRID_SIZE_X] = {
    {SEED_NONE, SEED_NONE, SEED_STARFRUIT, SEED_STARFRUIT, SEED_STARFRUIT, SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE},
    {SEED_NONE, SEED_STARFRUIT, SEED_WALLNUT, SEED_WALLNUT, SEED_WALLNUT, SEED_STARFRUIT, SEED_NONE, SEED_NONE, SEED_NONE},
    {SEED_NONE, SEED_NONE, SEED_STARFRUIT, SEED_STARFRUIT, SEED_STARFRUIT, SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE},
    {SEED_NONE, SEED_NONE, SEED_NONE, SEED_UMBRELLA, SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE},
    {SEED_NONE, SEED_NONE, SEED_UMBRELLA, SEED_UMBRELLA, SEED_UMBRELLA, SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE},
    {SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE},
};

SeedType gArtChallengeStarFruit[MAX_GRID_SIZE_Y][MAX_GRID_SIZE_X] = {
    {SEED_NONE, SEED_NONE, SEED_NONE, SEED_STARFRUIT, SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE},
    {SEED_NONE, SEED_NONE, SEED_NONE, SEED_STARFRUIT, SEED_STARFRUIT, SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE},
    {SEED_NONE, SEED_STARFRUIT, SEED_STARFRUIT, SEED_STARFRUIT, SEED_STARFRUIT, SEED_STARFRUIT, SEED_STARFRUIT, SEED_NONE, SEED_NONE},
    {SEED_NONE, SEED_NONE, SEED_NONE, SEED_STARFRUIT, SEED_STARFRUIT, SEED_STARFRUIT, SEED_NONE, SEED_NONE, SEED_NONE},
    {SEED_NONE, SEED_NONE, SEED_NONE, SEED_STARFRUIT, SEED_NONE, SEED_NONE, SEED_STARFRUIT, SEED_NONE, SEED_NONE},
    {SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE, SEED_NONE},
};

Challenge::Challenge() {
    _constructor();
}

void Challenge::_constructor() {
    if (requestJumpSurvivalStage) {
        // 如果玩家按了无尽跳关
        if (mSurvivalStage > 0 || mApp->mGameScene == GameScenes::SCENE_PLAYING) {
            // 需要玩家至少已完成选种子，才能跳关。否则有BUG
            mSurvivalStage = targetWavesToJump;
        }
        requestJumpSurvivalStage = false;
    }

    old_Challenge_Challenge(this);
}

void Challenge::Update() {
    if (requestJumpSurvivalStage) {
        // 如果玩家按了无尽跳关
        if (mSurvivalStage > 0 || mApp->mGameScene == GameScenes::SCENE_PLAYING) {
            // 需要玩家至少已完成选种子，才能跳关。否则有BUG
            mSurvivalStage = targetWavesToJump;
        }
        requestJumpSurvivalStage = false;
    }

    GameMode gameMode = mApp->mGameMode;
    if (gameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN || gameMode == GameMode::GAMEMODE_TREE_OF_WISDOM) {
        TutorialState mTutorialState = mBoard->mTutorialState;
        if (mTutorialState == TutorialState::TUTORIAL_ZEN_GARDEN_PICKUP_WATER || mTutorialState == TutorialState::TUTORIAL_ZEN_GARDEN_WATER_PLANT
            || mTutorialState == TutorialState::TUTORIAL_ZEN_GARDEN_KEEP_WATERING) {
            gBoardMenuButton->mBtnNoDraw = true;
            gBoardStoreButton->mBtnNoDraw = true;
            gBoardStoreButton->mDisabled = true;
            gBoardMenuButton->mDisabled = true;
        } else if (mApp->mCrazyDaveState != CrazyDaveState::CRAZY_DAVE_OFF) {
            gBoardStoreButton->mBtnNoDraw = true;
            gBoardMenuButton->mBtnNoDraw = true;
            gBoardStoreButton->mDisabled = true;
            gBoardMenuButton->mDisabled = true;
        } else if (mTutorialState == TutorialState::TUTORIAL_ZEN_GARDEN_VISIT_STORE) {
            gBoardStoreButton->mBtnNoDraw = false;
            gBoardMenuButton->mBtnNoDraw = true;
            gBoardStoreButton->mDisabled = false;
            gBoardMenuButton->mDisabled = true;
        } else {
            gBoardStoreButton->mBtnNoDraw = false;
            gBoardMenuButton->mBtnNoDraw = false;
            gBoardStoreButton->mDisabled = false;
            gBoardMenuButton->mDisabled = false;
        }
    }

    if (requestPause) {
        return;
    }

    old_Challenge_Update(this);
}

void Challenge::HeavyWeaponFire(float a2, float a3) {
    // 设定重型武器子弹的发射角度
    if (a2 == 0 && a3 == 1) {
        a2 = angle1;
        a3 = angle2;
    }
    old_Challenge_HeavyWeaponFire(this, a2, a3);
}

void Challenge::HeavyWeaponReanimUpdate() {
    Reanimation *heavyWeaponReanim = mApp->ReanimationTryToGet(mReanimHeavyWeaponID2);
    if (heavyWeaponReanim == nullptr)
        return;

    SexyTransform2D sexyTransform2D{};
    sexyTransform2D.Translate(-129.55, -71.45);
    sexyTransform2D.RotateRad(mHeavyWeaponAngle);
    sexyTransform2D.Translate(mHeavyWeaponX, mHeavyWeaponY);
    sexyTransform2D.Translate(129.55, 71.45);
    sexyTransform2D.Translate(0.0, -20.0);
    heavyWeaponReanim->mOverlayMatrix = sexyTransform2D;
}

void Challenge::HeavyWeaponUpdate() {
    // 设定重型武器动画的发射角度
    old_Challenge_HeavyWeaponUpdate(this);

    if (angle1 != 0) {
        mHeavyWeaponAngle = acosf(angle1) - 1.5708f;
        HeavyWeaponReanimUpdate();
    }
}

void Challenge::IZombieDrawPlant(Sexy::Graphics *g, Plant *thePlant) {
    // 参照PC内测版源代码，在IZ模式绘制植物的函数开始前额外绘制纸板效果。

    Reanimation *mBodyReanim = mApp->ReanimationTryToGet(thePlant->mBodyReanimID);
    if (mBodyReanim != nullptr) {
        IZombieSetPlantFilterEffect(thePlant, FilterEffect::FILTEREFFECT_WHITE);
        float aOffsetX = g->mTransX;
        float aOffsetY = g->mTransY;
        Color theColor;
        g->SetColorizeImages(true);

        g->mTransX = aOffsetX + 4.0;
        g->mTransY = aOffsetY + 4.0;
        theColor.mRed = 122;
        theColor.mGreen = 86;
        theColor.mBlue = 58;
        theColor.mAlpha = 255;
        g->SetColor(theColor);
        mBodyReanim->DrawRenderGroup(g, 0);

        g->mTransX = aOffsetX + 2.0;
        g->mTransY = aOffsetY + 2.0;
        theColor.mRed = 171;
        theColor.mGreen = 135;
        theColor.mBlue = 107;
        theColor.mAlpha = 255;
        g->SetColor(theColor);
        mBodyReanim->DrawRenderGroup(g, 0);

        g->mTransX = aOffsetX - 2.0;
        g->mTransY = aOffsetY - 2.0;
        theColor.mRed = 171;
        theColor.mGreen = 135;
        theColor.mBlue = 107;
        theColor.mAlpha = 255;
        g->SetColor(theColor);
        mBodyReanim->DrawRenderGroup(g, 0);

        g->mTransX = aOffsetX;
        g->mTransY = aOffsetY;
        theColor.mRed = 255;
        theColor.mGreen = 201;
        theColor.mBlue = 160;
        theColor.mAlpha = 255;
        g->SetColor(theColor);
        IZombieSetPlantFilterEffect(thePlant, FilterEffect::FILTEREFFECT_NONE);
        mBodyReanim->DrawRenderGroup(g, 0);

        IZombieSetPlantFilterEffect(thePlant, FilterEffect::FILTEREFFECT_NONE);
        g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
        g->SetColorizeImages(false);
    }
}

bool Challenge::IZombieEatBrain(Zombie *theZombie) {
    // 修复IZ脑子血量太高
    GridItem *aBrain = IZombieGetBrainTarget(theZombie);
    if (aBrain == nullptr)
        return false;

    theZombie->StartEating();
    // int mHealth = aBrain->mGridItemCounter - 1;
    int mHealth = aBrain->mGridItemCounter - 2; // 一次吃掉脑子的两滴血
    aBrain->mGridItemCounter = mHealth;
    if (mHealth <= 0) {
        mApp->PlaySample(*Sexy_SOUND_GULP_Addr);
        aBrain->GridItemDie();
        IZombieScoreBrain(aBrain);
    }
    return true;
}

void Challenge::DrawArtChallenge(Sexy::Graphics *g) {
    // 绘制坚果的两只大眼睛
    g->SetColorizeImages(true);
    Color theColor = {255, 255, 255, 100};
    g->SetColor(theColor);

    for (int theGridY = 0; theGridY < 6; theGridY++) {
        for (int theGridX = 0; theGridX < 9; theGridX++) {
            SeedType ArtChallengeSeed = GetArtChallengeSeed(theGridX, theGridY);
            if (ArtChallengeSeed != SeedType::SEED_NONE && mBoard->GetTopPlantAt(theGridX, theGridY, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION) == nullptr) {
                int x = mBoard->GridToPixelX(theGridX, theGridY);
                int y = mBoard->GridToPixelY(theGridX, theGridY);
                Plant::DrawSeedType(g, ArtChallengeSeed, SeedType::SEED_NONE, DrawVariation::VARIATION_NORMAL, x, y);
            }
        }
    }

    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ART_CHALLENGE_WALLNUT) {
        Color theColor = {255, 255, 255, 255};
        g->SetColor(theColor);
        int x1 = mBoard->GridToPixelX(4, 1);
        int y1 = mBoard->GridToPixelY(4, 1);
        g->DrawImage(addonImages.googlyeye, x1, y1);
        int x2 = mBoard->GridToPixelX(6, 1);
        int y2 = mBoard->GridToPixelY(6, 1);
        g->DrawImage(addonImages.googlyeye, x2, y2);
    }

    g->SetColorizeImages(false);
}

PlantingReason Challenge::CanPlantAt(int theGridX, int theGridY, SeedType theSeedType) {
    // 修复IZ多个蹦极可放置在同一格子内
    GameMode aGameMode = mApp->mGameMode;
    if (mApp->IsWallnutBowlingLevel()) {
        if (theGridX > 2) {
            return PlantingReason::PLANTING_NOT_PASSED_LINE;
        }
    } else if (mApp->IsIZombieLevel()) {
        int num = 6;
        if (aGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_1 || aGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_2 || aGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_3
            || aGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_4 || aGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_5) {
            num = 4;
        }
        if (aGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_6 || aGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_7 || aGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_8
            || aGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_ENDLESS) {
            num = 5;
        }
        if (theSeedType == SeedType::SEED_ZOMBIE_BUNGEE) {
            if (theGridX < num) {
                Zombie *aZombie = nullptr;
                while (mBoard->IterateZombies(aZombie)) {
                    if (aZombie->mZombieType == ZombieType::ZOMBIE_BUNGEE) {
                        int aGridX = mBoard->PixelToGridX(aZombie->mX, aZombie->mY);
                        if (aGridX == theGridX && aZombie->mRow == theGridY) {
                            return PlantingReason::PLANTING_NOT_HERE;
                        }
                    }
                }
                return PlantingReason::PLANTING_OK;
            }
            return PlantingReason::PLANTING_NOT_HERE;
        } else if (IsZombieSeedType(theSeedType)) {
            if (theGridX >= num) {
                return PlantingReason::PLANTING_OK;
            }
            return PlantingReason::PLANTING_NOT_HERE;
        }
    } else if (mApp->IsArtChallenge()) {
        SeedType artChallengeSeed = GetArtChallengeSeed(theGridX, theGridY);
        if (artChallengeSeed != SeedType::SEED_NONE && artChallengeSeed != theSeedType && theSeedType != SeedType::SEED_LILYPAD && theSeedType != SeedType::SEED_PUMPKINSHELL) {
            return PlantingReason::PLANTING_NOT_ON_ART;
        }
        if (aGameMode == GameMode::GAMEMODE_CHALLENGE_ART_CHALLENGE_WALLNUT) {
            if (theGridX == 4 && theGridY == 1) {
                return PlantingReason::PLANTING_NOT_HERE;
            }
            if (theGridX == 6 && theGridY == 1) {
                return PlantingReason::PLANTING_NOT_HERE;
            }
        }
    } else if (mApp->IsFinalBossLevel() && theGridX >= 8) {
        return PlantingReason::PLANTING_NOT_HERE;
    } else if (aGameMode == GameMode::GAMEMODE_MP_VS_HIDE || aGameMode == GameMode::GAMEMODE_MP_VS) {
        if (IsMPSeedType(theSeedType)) {
            if (theGridX > 5 || theSeedType == SeedType::SEED_ZOMBIE_BUNGEE)
                return PlantingReason::PLANTING_OK;
            return PlantingReason::PLANTING_NOT_PASSED_LINE_VS;
        }
        if (theSeedType == SeedType::SEED_GRAVEBUSTER) {
            if (mBoard->GetGridItemAt(GridItemType::GRIDITEM_GRAVESTONE, theGridX, theGridY) == nullptr)
                return PlantingReason::PLANTING_ONLY_ON_GRAVES;
        } else {
            if (theGridX <= 5)
                return PlantingReason::PLANTING_OK;
            return PlantingReason::PLANTING_NOT_PASSED_LINE_VS;
        }
    }
    return PlantingReason::PLANTING_OK;
}

void Challenge::InitLevel() {
    old_Challenge_InitLevel(this);

    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BUTTERED_POPCORN) {
        mBoard->NewPlant(0, 0, SeedType::SEED_COBCANNON, SeedType::SEED_NONE, -1);
        mBoard->NewPlant(0, 1, SeedType::SEED_COBCANNON, SeedType::SEED_NONE, -1);
        mBoard->NewPlant(0, 4, SeedType::SEED_COBCANNON, SeedType::SEED_NONE, -1);
        mBoard->NewPlant(0, 5, SeedType::SEED_COBCANNON, SeedType::SEED_NONE, -1);
    }

    // 为结盟僵王的2P传送带补充开局的4个固定植物
    if (mApp->mGameMode == GAMEMODE_TWO_PLAYER_COOP_BOSS) {
        mBoard->mSeedBank2->AddSeed(SeedType::SEED_CABBAGEPULT, false);
        mBoard->mSeedBank2->AddSeed(SeedType::SEED_JALAPENO, false);
        mBoard->mSeedBank2->AddSeed(SeedType::SEED_CABBAGEPULT, false);
        mBoard->mSeedBank2->AddSeed(SeedType::SEED_ICESHROOM, false);
        mConveyorBeltCounter2 = 1000;
    }
}

void Challenge::StartLevel() {
    old_Challenge_StartLevel(this);

    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_HEAVY_WEAPON && heavyWeaponAccel) {
        Native::BridgeApp *bridgeApp = Native::BridgeApp::getSingleton();
        JNIEnv *env = bridgeApp->getJNIEnv();
        jobject activity = bridgeApp->mNativeApp->getActivity();
        jclass cls = env->GetObjectClass(activity);
        jmethodID methodID = env->GetMethodID(cls, "startOrientationListener", "()V");
        env->CallVoidMethod(activity, methodID);
        env->DeleteLocalRef(cls);
    }
}

void Challenge::InitZombieWaves() {
    old_Challenge_InitZombieWaves(this);

    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BUTTERED_POPCORN) {
        ZombieType zombieList[] = {
            ZombieType::ZOMBIE_NORMAL,
            ZombieType::ZOMBIE_TRAFFIC_CONE,
            ZombieType::ZOMBIE_PAIL,
            ZombieType::ZOMBIE_DOOR,
            ZombieType::ZOMBIE_FOOTBALL,
            ZombieType::ZOMBIE_NEWSPAPER,
            ZombieType::ZOMBIE_JACK_IN_THE_BOX,
            ZombieType::ZOMBIE_POLEVAULTER,
            ZombieType::ZOMBIE_DOLPHIN_RIDER,
            ZombieType::ZOMBIE_LADDER,
            ZombieType::ZOMBIE_GARGANTUAR,
        };
        InitZombieWavesFromList(zombieList, std::size(zombieList));
    }
}

void Challenge::TreeOfWisdomFertilize() {
    old_Challenge_TreeOfWisdomFertilize(this);

    // 检查智慧树成就
    PlayerInfo *playerInfo = mApp->mPlayerInfo;
    if (playerInfo->mChallengeRecords[GameMode::GAMEMODE_TREE_OF_WISDOM - 2] >= 99) {
        mBoard->GrantAchievement(AchievementId::ACHIEVEMENT_TREE, true);
    }
}

void Challenge::LastStandUpdate() {
    if (mBoard->mNextSurvivalStageCounter == 0 && mChallengeState == ChallengeState::STATECHALLENGE_NORMAL && gBoardStoreButton->mBtnNoDraw) {
        gBoardStoreButton->mBtnNoDraw = false;
        gBoardStoreButton->mDisabled = false;
        pvzstl::string str = TodStringTranslate(mSurvivalStage == 0 ? "[START_ONSLAUGHT]" : "[CONTINUE_ONSLAUGHT]");
        gBoardStoreButton->SetLabel(str);
        gBoardStoreButton->Resize(325, 555, 170, 120);
    }

    if (mChallengeState == ChallengeState::STATECHALLENGE_LAST_STAND_ONSLAUGHT && mApp->mGameScene == GameScenes::SCENE_PLAYING) {
        mChallengeStateCounter++;
    }

    if (mChallengeState == ChallengeState::STATECHALLENGE_NORMAL && !gBoardStoreButton->mBtnNoDraw) {
        gBoardStoreButton->Resize(325, 555, 170, 120);
        gBoardStoreButton->mBtnNoDraw = false;
        gBoardStoreButton->mDisabled = false;
    }
}

int Challenge::IsMPSeedType(SeedType theSeedType) {
    return theSeedType >= SEED_ZOMBIE_GRAVESTONE && theSeedType < NUM_ZOMBIE_SEED_TYPES;
}

int Challenge::IsZombieSeedType(SeedType theSeedType) {
    return theSeedType >= SEED_ZOMBIE_GRAVESTONE && theSeedType < NUM_ZOMBIE_SEED_TYPES;
}

void Challenge::IZombieSetPlantFilterEffect(Plant *thePlant, FilterEffect theFilterEffect) {
    Reanimation *aBodyReanim = mApp->ReanimationTryToGet(thePlant->mBodyReanimID);
    Reanimation *aHeadReanim = mApp->ReanimationTryToGet(thePlant->mHeadReanimID);
    Reanimation *aHeadReanim2 = mApp->ReanimationTryToGet(thePlant->mHeadReanimID2);
    Reanimation *aHeadReanim3 = mApp->ReanimationTryToGet(thePlant->mHeadReanimID3);
    if (aBodyReanim)
        aBodyReanim->mFilterEffect = theFilterEffect;
    if (aHeadReanim)
        aHeadReanim->mFilterEffect = theFilterEffect;
    if (aHeadReanim2)
        aHeadReanim2->mFilterEffect = theFilterEffect;
    if (aHeadReanim3)
        aHeadReanim3->mFilterEffect = theFilterEffect;
}

ZombieType Challenge::IZombieSeedTypeToZombieType(SeedType theSeedType) {
    // 此处可修改VS和IZ中的僵尸类型
    switch (theSeedType) {
        case SEED_ZOMBIE_NORMAL:
            return ZOMBIE_NORMAL;
        case SEED_ZOMBIE_TRASHCAN:
            return ZOMBIE_TRASHCAN;
        case SEED_ZOMBIE_TRAFFIC_CONE:
            return ZOMBIE_TRAFFIC_CONE;
        case SEED_ZOMBIE_POLEVAULTER:
            return ZOMBIE_POLEVAULTER;
        case SEED_ZOMBIE_PAIL:
            return ZOMBIE_PAIL;
        case SEED_ZOMBIE_FLAG:
            return ZOMBIE_FLAG;
        case SEED_ZOMBIE_NEWSPAPER:
            return ZOMBIE_NEWSPAPER;
        case SEED_ZOMBIE_SCREEN_DOOR:
            return ZOMBIE_DOOR;
        case SEED_ZOMBIE_FOOTBALL:
            return ZOMBIE_FOOTBALL;
        case SEED_ZOMBIE_DANCER:
            return ZOMBIE_DANCER;
        case SEED_ZOMBONI:
            return ZOMBIE_ZAMBONI;
        case SEED_ZOMBIE_JACK_IN_THE_BOX:
            return ZOMBIE_JACK_IN_THE_BOX;
        case SEED_ZOMBIE_DIGGER:
            return ZOMBIE_DIGGER;
        case SEED_ZOMBIE_POGO:
            return ZOMBIE_POGO;
        case SEED_ZOMBIE_BUNGEE:
            return ZOMBIE_BUNGEE;
        case SEED_ZOMBIE_LADDER:
            return ZOMBIE_LADDER;
        case SEED_ZOMBIE_CATAPULT:
            return ZOMBIE_CATAPULT;
        case SEED_ZOMBIE_GARGANTUAR:
            return ZOMBIE_GARGANTUAR;
        case SEED_ZOMBIE_YETI:
            return ZOMBIE_YETI;
        case SEED_ZOMBIE_DUCKY_TUBE:
            return ZOMBIE_DUCKY_TUBE;
        case SEED_ZOMBIE_SNORKEL:
            return ZOMBIE_SNORKEL;
        case SEED_ZOMBIE_DOLPHIN_RIDER:
            return ZOMBIE_DOLPHIN_RIDER;
        case SEED_ZOMBIE_IMP:
            return ZOMBIE_IMP;
        case SEED_ZOMBIE_BALLOON:
            return ZOMBIE_BALLOON;
        case SEED_ZOMBIE_PEA_HEAD:
            return ZOMBIE_PEA_HEAD; // 豌豆射手僵尸
        case SEED_ZOMBIE_WALLNUT_HEAD:
            return ZOMBIE_WALLNUT_HEAD; // 坚果僵尸
        case SEED_ZOMBIE_JALAPENO_HEAD:
            return ZOMBIE_JALAPENO_HEAD; // 火爆辣椒僵尸
        case SEED_ZOMBIE_GATLINGPEA_HEAD:
            return ZOMBIE_GATLING_HEAD; // 机枪射手僵尸
        case SEED_ZOMBIE_SQUASH_HEAD:
            return ZOMBIE_SQUASH_HEAD; // 窝瓜僵尸
        case SEED_ZOMBIE_TALLNUT_HEAD:
            return ZOMBIE_TALLNUT_HEAD; // 高坚果僵尸
        case SEED_ZOMBIE_SUNFLOWER_HEAD:
            return ZOMBIE_SUNFLOWER_HEAD;
        case SEED_ZOMBIE_TORCHWOOD_HEAD:
            return ZOMBIE_TORCHWOOD_HEAD;
        case SEED_ZOMBIE_EXPLODE_O_NUT_HEAD:
            return ZOMBIE_EXPLODE_O_NUT_HEAD;
        case SEED_ZOMBIE_JACKSON:
            return ZOMBIE_JACKSON;
        case SEED_ZOMBIE_BACKUP_DANCER2:
            return ZOMBIE_BACKUP_DANCER2;
        case SEED_ZOMBIE_GIGA_FOOTBALL:
            return ZOMBIE_GIGA_FOOTBALL;
        case SEED_ZOMBIE_SUPER_FAN_IMP:
            return ZOMBIE_SUPER_FAN_IMP;
        case SEED_ZOMBIE_BOBSLED:
            return ZOMBIE_BOBSLED; // 雪橇车小队
        default:
            return ZOMBIE_INVALID;
    }
}

void Challenge::IZombiePlaceZombie(ZombieType theZombieType, int theGridX, int theGridY) {
    Zombie *aZombie = mBoard->AddZombieInRow(theZombieType, theGridY, 0, 0);
    if (theZombieType == ZOMBIE_BUNGEE) {
        aZombie->mTargetCol = theGridX;
        aZombie->SetRow(theGridX);
        aZombie->mPosX = mBoard->GridToPixelX(theGridX, theGridY);
        aZombie->mPosY = aZombie->GetPosYBasedOnRow(theGridY);
        aZombie->mRenderOrder = Board::MakeRenderOrder(RENDER_LAYER_GRAVE_STONE, theGridY, 7);
    } else {
        aZombie->mPosX = mBoard->GridToPixelX(theGridX, theGridY) - 30.0f;
    }
}

void Challenge::DrawHeavyWeapon(Sexy::Graphics *g) {
    // 修复僵尸进家后重型武器关卡长草露馅
    g->DrawImage(*Sexy_IMAGE_HEAVY_WEAPON_OVERLAY_Addr, -73, 559);
}

bool Challenge::UpdateZombieSpawning() {
    if (stopSpawning) {
        return true;
    }

    return old_Challenge_UpdateZombieSpawning(this);
}

void Challenge::HeavyWeaponPacketClicked(SeedPacket *theSeedPacket) {
    // 修复疯狂点击毁灭菇导致GridItem数量超出上限而闪退
    if (theSeedPacket->mPacketType == SeedType::SEED_DOOMSHROOM) {
        GridItem *aGridItem = nullptr;
        while (mBoard->IterateGridItems(aGridItem)) {
            if (aGridItem->mGridItemType == GridItemType::GRIDITEM_CRATER) {
                aGridItem->GridItemDie();
            }
        }
    }

    old_Challenge_HeavyWeaponPacketClicked(this, theSeedPacket);
}

void Challenge::_destructor() {
    old_Challenge_Delete(this);

    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_HEAVY_WEAPON && heavyWeaponAccel) {
        Native::BridgeApp *bridgeApp = Native::BridgeApp::getSingleton();
        JNIEnv *env = bridgeApp->getJNIEnv();
        jobject activity = bridgeApp->mNativeApp->getActivity();
        jclass cls = env->GetObjectClass(activity);
        jmethodID methodID = env->GetMethodID(cls, "stopOrientationListener", "()V");
        env->CallVoidMethod(activity, methodID);
        env->DeleteLocalRef(cls);
    }
}

void Challenge::ScaryPotterOpenPot(GridItem *theScaryPot) {
    old_Challenge_ScaryPotterOpenPot(this, theScaryPot);
}

void Challenge::UpdateConveyorBelt(int playerIndex) {
    old_Challenge_UpdateConveyorBelt(this, playerIndex);
}

GridItem *Challenge::IZombieGetBrainTarget(Zombie *theZombie) {
    return old_Challenge_IZombieGetBrainTarget(this, theZombie);
    // if (theZombie->mZombieType == ZOMBIE_BUNGEE || theZombie->IsWalkingBackwards())
    // return nullptr;
    //
    // Rect aZombieRect = theZombie->GetZombieAttackRect();
    // if (theZombie->mZombiePhase == PHASE_POLEVAULTER_PRE_VAULT)
    // {
    // aZombieRect = Rect(50 + theZombie->mX, 0, 20, 115);
    // }
    // if (theZombie->mZombieType == ZOMBIE_BALLOON)
    // {
    // aZombieRect.mX += 25;
    // }
    //
    // if (aZombieRect.mX > 20)
    // return nullptr;
    //
    // GridItem* aBrain = mBoard->GetGridItemAt(GRIDITEM_IZOMBIE_BRAIN, 0, theZombie->mRow);
    // return (aBrain && aBrain->mGridItemState != GRIDITEM_STATE_BRAIN_SQUISHED) ? aBrain : nullptr;
}

void Challenge::IZombieSquishBrain(GridItem *theBrain) {
    old_Challenge_IZombieSquishBrain(this, theBrain);
}

int Challenge::ScaryPotterCountSunInPot(GridItem *theScaryPot) {
    return theScaryPot->mSunCount;
}

SeedType Challenge::GetArtChallengeSeed(int theGridX, int theGridY) {
    if (theGridY < 6) {

        GameMode aGameMode = mApp->mGameMode;
        if (aGameMode == GAMEMODE_CHALLENGE_ART_CHALLENGE_WALLNUT)
            return gArtChallengeWallnut[theGridY][theGridX];
        if (aGameMode == GAMEMODE_CHALLENGE_ART_CHALLENGE_SUNFLOWER)
            return gArtChallengeSunFlower[theGridY][theGridX];
        if (aGameMode == GAMEMODE_CHALLENGE_SEEING_STARS)
            return gArtChallengeStarFruit[theGridY][theGridX];
    }
    return SEED_NONE;
}

void Challenge::InitZombieWavesFromList(ZombieType *theZombieList, int theListLength) {
    for (int i = 0; i < theListLength; i++) {
        mBoard->mZombieAllowed[(int)theZombieList[i]] = true;
    }
}

void Challenge::IZombieSetupPlant(Plant *thePlant) {
    Reanimation *aBodyReanim = mApp->ReanimationTryToGet(thePlant->mBodyReanimID);
    Reanimation *aHeadReanim = mApp->ReanimationTryToGet(thePlant->mHeadReanimID);
    Reanimation *aHeadReanim2 = mApp->ReanimationTryToGet(thePlant->mHeadReanimID2);
    Reanimation *aHeadReanim3 = mApp->ReanimationTryToGet(thePlant->mHeadReanimID3);
    if (aBodyReanim)
        aBodyReanim->mAnimRate = 0;
    if (aHeadReanim)
        aHeadReanim->mAnimRate = 0;
    if (aHeadReanim2)
        aHeadReanim2->mAnimRate = 0;
    if (aHeadReanim3)
        aHeadReanim3->mAnimRate = 0;

    if (thePlant->mSeedType == SEED_POTATOMINE) {
        thePlant->PlayBodyReanim("anim_armed", REANIM_LOOP, 0, 0);
        thePlant->mState = STATE_POTATO_ARMED;
    }

    thePlant->mBlinkCountdown = 0;
    thePlant->UpdateReanim();
}