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

#include "PvZ/Lawn/Board/SeedPacket.h"
#include "Homura/Logger.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Board/Challenge.h"
#include "PvZ/Lawn/Board/SeedBank.h"
#include "PvZ/Lawn/GamepadControls.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/Widget/WaitForSecondPlayerDialog.h"
#include "PvZ/MagicAddr.h"
#include "PvZ/Misc.h"
#include "PvZ/SexyAppFramework/Graphics/Font.h"
#include "PvZ/SexyAppFramework/Misc/SexyMatrix.h"
#include "PvZ/Symbols.h"

using namespace Sexy;

void SeedPacket::Update() {
    if (mRefreshing && seedPacketFastCoolDown) {
        // 正在刷新的种子立即冷却完毕
        mActive = true;
        mRefreshing = false;
    }

    if (requestPause) {

        if (mApp->IsIZombieLevel()) {
            // 在IZ模式不暂停刷新种子卡片
            old_SeedPacket_Update(this);
            return;
        }

        if (mApp->mGameScene == GameScenes::SCENE_PLAYING && mPacketType != SeedType::SEED_NONE) {
            mRefreshCounter--;
        }
        if (mSlotMachineCountDown > 0) {
            mSlotMachineCountDown++;
        }
    }

    for (int i = 0; i < SEEDBANK_MAX; ++i) {
        SeedPacket *aSeedPacket = &mSeedBank->mSeedPackets[i];
        SeedType aPacketType = aSeedPacket->mPacketType;
        if (aPacketType == SeedType::SEED_ZOMBIE_BACKUP_DANCER2) {
            if (!mBoard->GetLiveZombieByType(ZombieType::ZOMBIE_JACKSON)) {
                aSeedPacket->SetPacketType(SeedType::SEED_ZOMBIE_JACKSON, SeedType::SEED_NONE);
                aSeedPacket->Deactivate();
                aSeedPacket->WasPlanted(1);
            }
        }
        if (aPacketType == SeedType::SEED_ZOMBIE_SUPER_FAN_IMP) {
            if (!mBoard->GetLiveZombieByType(ZombieType::ZOMBIE_GIGA_FOOTBALL)) {
                aSeedPacket->SetPacketType(SeedType::SEED_ZOMBIE_GIGA_FOOTBALL, SeedType::SEED_NONE);
                aSeedPacket->Deactivate();
                aSeedPacket->WasPlanted(1);
            }
        }
    }

    old_SeedPacket_Update(this);
}

void SeedPacket::UpdateSelected() {
    if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS || mApp->IsCoopMode()) {
        // 如果是双人模式关卡(对战或结盟)，则使用下面的逻辑来更新当前选中的卡片。用于修复1P和2P的卡片选择框同时出现在两个人各自的植物栏里(也就是植物栏一共出现四个选择框)的问题。

        GamepadControls* theGamepad = GetPlayerIndex() ? mBoard->mGamepadControls2 : mBoard->mGamepadControls1;

        mSelected = theGamepad->mSelectedSeedIndex == mIndex;
        mSelectedBy2P = theGamepad->mPlayerIndex1 == 1 && theGamepad->mSelectedSeedIndex == mIndex;


        return;
    }

    old_SeedPacket_UpdateSelected(this);
}

void SeedPacket::DrawOverlay(Sexy::Graphics *g) {
    // 绘制卡片冷却进度倒计时
    old_SeedPacket_DrawOverlay(this, g);

    if (mRefreshing && showCoolDown) {
        // 如果玩家开启了“显示冷却倒计时”，则绘制倒计时
        int coolDownRemaining = mRefreshTime - mRefreshCounter;
        pvzstl::string str = StrFormat("%1.1f", coolDownRemaining / 100.0f);
        g->SetColor(GetPlayerIndex() ? yellow : blue);
        g->SetFont(*Sexy_FONT_DWARVENTODCRAFT18_Addr);
        g->DrawString(str, coolDownRemaining < 1000 ? 10 : 0, 39);
        g->SetFont(nullptr);
    }
}

void SeedPacket::Draw(Sexy::Graphics *g) {
    // 绘制卡片冷却进度倒计时
    old_SeedPacket_Draw(this, g);
}

void SeedPacket_MouseDown(SeedPacket *seedPacket, int x, int y, int c, int unk) {
    //    LOGD("SeedPacket_MouseDown:%d %d %d %d", x, y, c, unk);
    old_SeedPacket_MouseDown(seedPacket, x, y, c, unk);
}

bool SeedPacket::BeginDraw(Sexy::Graphics *g) {
    return old_SeedPacket_BeginDraw(this, g);
}

void SeedPacket::EndDraw(Sexy::Graphics *g) {
    old_SeedPacket_EndDraw(this, g);
}

void SeedPacket::FlashIfReady() {
    // 去除对战模式下的闪光效果的缩放

    if (!CanPickUp()) {
        return;
    }
    if (mApp->mEasyPlantingCheat) {
        return;
    }

    int playerIndex = 0;
    if (mSeedBank != nullptr) {
        playerIndex = mSeedBank->mIsZombie;
    }
    if (!mBoard->HasConveyorBeltSeedBank(playerIndex) && mSeedBank != nullptr) {
        mApp->AddTodParticle(mSeedBank->mX + mX, mSeedBank->mY + mY, 100000 + 2, ParticleEffect::PARTICLE_SEED_PACKET_FLASH);
    }
    TutorialState tutorialState = mBoard->mTutorialState;
    if (tutorialState == TutorialState::TUTORIAL_LEVEL_1_REFRESH_PEASHOOTER) {
        mBoard->SetTutorialState(TutorialState::TUTORIAL_LEVEL_1_PICK_UP_PEASHOOTER);
        return;
    }
    if (tutorialState == TutorialState::TUTORIAL_LEVEL_2_REFRESH_SUNFLOWER && mPacketType == SeedType::SEED_SUNFLOWER) {
        mBoard->SetTutorialState(TutorialState::TUTORIAL_LEVEL_2_PICK_UP_SUNFLOWER);
        return;
    }
    if (tutorialState == TutorialState::TUTORIAL_MORESUN_REFRESH_SUNFLOWER && mPacketType == SeedType::SEED_SUNFLOWER) {
        mBoard->SetTutorialState(TutorialState::TUTORIAL_MORESUN_PICK_UP_SUNFLOWER);
        return;
    }
}

void SeedPacket::SetPacketType(SeedType theSeedType, SeedType theImitaterType) {
    old_SeedPacket_SetPacketType(this, theSeedType, theImitaterType);

    int aRefreshTime = Plant::GetRefreshTime(theSeedType, theImitaterType);
    // 此处修改对战开局的初始冷却
    if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS) {
        switch (theSeedType) {
            case SEED_SUNSHROOM:
                if (gVSBalanceAdjustment) { // 清除阳光菇的初始冷却
                    mRefreshTime = 0;
                    mRefreshing = false;
                    mActive = true;
                }
                break;
            case SEED_ZOMBIE_SUNFLOWER_HEAD:
                mRefreshTime = 1000;
                mRefreshing = true;
                mActive = false;
                break;
            case SEED_ZOMBIE_SUPER_FAN_IMP:
                mRefreshTime = 500;
                mRefreshing = true;
                mActive = false;
                break;
            case SEED_ZOMBIE_BACKUP_DANCER2:
                mRefreshTime = 1000;
                mRefreshing = true;
                mActive = false;
                break;
            default:
                break;
        }
    }
}

void DrawSeedPacket(Sexy::Graphics *g,
                    float x,
                    float y,
                    SeedType theSeedType,
                    SeedType theImitaterType,
                    float thePercentDark,
                    int theGrayness,
                    bool theDrawCost,
                    bool theUseCurrentCost,
                    bool isZombieSeed,
                    bool isSeedPacketSelected) {
    // 修复选中紫卡、模仿者卡时卡片背景变为普通卡片背景

    SeedType realSeedType = theImitaterType != SeedType::SEED_NONE && theSeedType == SeedType::SEED_IMITATER ? theImitaterType : theSeedType;
    if (theGrayness != 255) {
        Color theColor = {theGrayness, theGrayness, theGrayness, 255};
        g->SetColor(theColor);
        g->SetColorizeImages(true);
    } else if (thePercentDark > 0.0f) {
        Color theColor = {128, 128, 128, 255};
        g->SetColor(theColor);
        g->SetColorizeImages(true);
    }
    int celToDraw;
    if (theSeedType == SeedType::SEED_IMITATER) {
        celToDraw = 0;
    } else if (Plant::IsUpgrade(realSeedType) || Zombie::IsUpgrade(theSeedType)) {
        celToDraw = 1;
    } else if (theSeedType == SeedType::SEED_BEGHOULED_BUTTON_CRATER) {
        celToDraw = 3;
    } else if (theSeedType == SeedType::SEED_BEGHOULED_BUTTON_SHUFFLE) {
        celToDraw = 4;
    } else if (theSeedType == SeedType::SEED_SLOT_MACHINE_SUN) {
        celToDraw = 5;
    } else if (theSeedType == SeedType::SEED_SLOT_MACHINE_DIAMOND) {
        celToDraw = 6;
    } else if (theSeedType == SeedType::SEED_ZOMBIQUARIUM_SNORKLE) {
        celToDraw = 7;
    } else if (theSeedType == SeedType::SEED_ZOMBIQUARIUM_TROPHY) {
        celToDraw = 8;
    } else {
        celToDraw = 2;
    }

    if (isSeedPacketSelected) {
        if (g->mScaleX > 1.0f && theSeedType < SeedType::NUM_SEEDS_IN_CHOOSER) {
            // 紫卡背景BUG就是在这里修复的
            if (celToDraw == 2) {
                TodDrawImageCelScaledF(g, *Sexy_IMAGE_SEEDPACKET_LARGER_Addr, x, y, 0, 0, g->mScaleX * 0.5f, g->mScaleY * 0.5f);
            } else {
                TodDrawImageCelScaledF(g, *Sexy_IMAGE_SEEDS_Addr, x, y, celToDraw, 0, g->mScaleX, g->mScaleY);
            }
        } else if (isZombieSeed) {
            float heightOffset = g->mScaleX > 1.2 ? -1.5f : 0.0f;
            if (Zombie::IsUpgrade(theSeedType)) {
                TodDrawImageCelScaledF(g, *Sexy_IMAGE_SEEDS_Addr, x, y + heightOffset, celToDraw, 0, g->mScaleX, g->mScaleY);
            } else {
                TodDrawImageScaledF(g, *Sexy_IMAGE_ZOMBIE_SEEDPACKET_Addr, x, y + heightOffset, g->mScaleX, g->mScaleY);
            }
        } else {
            TodDrawImageCelScaledF(g, *Sexy_IMAGE_SEEDS_Addr, x, y, celToDraw, 0, g->mScaleX, g->mScaleY);
        }
    }
    bool isPlant = theSeedType < SeedType::SEED_BEGHOULED_BUTTON_SHUFFLE || theSeedType > SeedType::SEED_ZOMBIQUARIUM_TROPHY;
    float offsetY, offsetX, theDrawScale;
    switch (realSeedType) {
        case SeedType::SEED_TALLNUT:
            offsetY = 22.0;
            offsetX = 12.0;
            theDrawScale = 0.3;
            break;
        case SeedType::SEED_INSTANT_COFFEE:
            offsetY = 9.0;
            offsetX = 0.0;
            theDrawScale = 0.55;
            break;
        case SeedType::SEED_COBCANNON:
            offsetY = 22.0;
            offsetX = 6.0;
            theDrawScale = 0.26;
            break;
        case SeedType::SEED_CACTUS:
            offsetY = 13.0;
            offsetX = 9.0;
            theDrawScale = 0.5;
            break;
        case SeedType::SEED_MAGNETSHROOM:
            offsetY = 12.0;
            offsetX = 5.0;
            theDrawScale = 0.5;
            break;
        case SeedType::SEED_TWINSUNFLOWER:
        case SeedType::SEED_GLOOMSHROOM:
            offsetY = 14.0;
            offsetX = 7.0;
            theDrawScale = 0.45;
            break;
        case SeedType::SEED_CATTAIL:
            offsetY = 13.0;
            offsetX = 8.0;
            theDrawScale = 0.45;
            break;
        case SeedType::SEED_UMBRELLA:
            offsetY = 10.0;
            offsetX = 5.0;
            theDrawScale = 0.5;
            break;
        case SeedType::SEED_KERNELPULT:
            offsetY = 14.0;
            offsetX = 13.0;
            theDrawScale = 0.4;
            break;
        case SeedType::SEED_CABBAGEPULT:
            offsetY = 14.0;
            offsetX = 15.0;
            theDrawScale = 0.4;
            break;
        case SeedType::SEED_GRAVEBUSTER:
            offsetY = 15.0;
            offsetX = 10.0;
            theDrawScale = 0.4;
            break;
        case SeedType::SEED_SPLITPEA:
            offsetY = 12.0;
            offsetX = 12.0;
            theDrawScale = 0.45;
            break;
        case SeedType::SEED_BLOVER:
            offsetY = 17.0;
            offsetX = 8.0;
            theDrawScale = 0.4;
            break;
        case SeedType::SEED_STARFRUIT:
            offsetY = 8.0;
            offsetX = 6.0;
            theDrawScale = 0.5;
            break;
        case SeedType::SEED_THREEPEATER:
            offsetY = 10.0;
            offsetX = 5.0;
            theDrawScale = 0.5;
            break;
        case SeedType::SEED_GATLINGPEA:
            offsetY = 8.0;
            offsetX = 2.0;
            theDrawScale = 0.5;
            break;
        case SeedType::SEED_ZOMBIE_POLEVAULTER:
            offsetY = -12.0;
            offsetX = -8.0;
            theDrawScale = 0.35;
            break;
        case SeedType::SEED_MELONPULT:
        case SeedType::SEED_WINTERMELON:
            offsetY = 19.0;
            offsetX = 18.0;
            theDrawScale = 0.35;
            break;
        case SeedType::SEED_POTATOMINE:
        case SeedType::SEED_FUMESHROOM:
        case SeedType::SEED_TANGLEKELP:
        case SeedType::SEED_PUMPKINSHELL:
        case SeedType::SEED_CHOMPER:
        case SeedType::SEED_DOOMSHROOM:
        case SeedType::SEED_SQUASH:
        case SeedType::SEED_HYPNOSHROOM:
        case SeedType::SEED_SPIKEWEED:
        case SeedType::SEED_SPIKEROCK:
        case SeedType::SEED_PLANTERN:
        case SeedType::SEED_TORCHWOOD:
            offsetY = 12.0;
            offsetX = 8.0;
            theDrawScale = 0.4;
            break;
        case SeedType::SEED_ZOMBIE_NORMAL:
        case SeedType::SEED_ZOMBIE_NEWSPAPER:
        case SeedType::SEED_ZOMBIE_FLAG:
        case SeedType::SEED_ZOMBIE_TRAFFIC_CONE:
        case SeedType::SEED_ZOMBIE_PAIL:
        case SeedType::SEED_ZOMBIE_DANCER:
        case SeedType::SEED_ZOMBIE_JACKSON:
        case SeedType::SEED_ZOMBIE_BACKUP_DANCER2:
            offsetY = -7.0;
            offsetX = -3.0;
            theDrawScale = 0.35;
            break;
        case SeedType::SEED_ZOMBIE_LADDER:
        case SeedType::SEED_ZOMBIE_DIGGER:
        case SeedType::SEED_ZOMBIE_SCREEN_DOOR:
        case SeedType::SEED_ZOMBIE_TRASHCAN:
        case SeedType::SEED_ZOMBIE_POGO:
        case SeedType::SEED_ZOMBIE_JACK_IN_THE_BOX:
        case SeedType::SEED_ZOMBIE_DUCKY_TUBE:
            offsetY = -10.0;
            offsetX = -3.0;
            theDrawScale = 0.35;
            break;
        case SeedType::SEED_ZOMBIE_DOLPHIN_RIDER:
            offsetY = -12.0;
            offsetX = -3.0;
            theDrawScale = 0.35;
            break;
        case SeedType::SEED_ZOMBIE_SNORKEL:
            offsetY = -8.0;
            offsetX = -3.0;
            theDrawScale = 0.32;
            break;
        case SeedType::SEED_ZOMBIE_BUNGEE:
            offsetY = -1.0;
            offsetX = 1.0;
            theDrawScale = 0.3;
            break;
        case SeedType::SEED_ZOMBIE_FOOTBALL:
        case SeedType::SEED_ZOMBIE_GIGA_FOOTBALL:
            offsetY = -9.0;
            offsetX = -7.0;
            theDrawScale = 0.33;
            break;
        case SeedType::SEED_ZOMBIE_BALLOON:
            offsetY = -5.0;
            offsetX = -3.0;
            theDrawScale = 0.35;
            break;
        case SeedType::SEED_ZOMBIE_IMP:
        case SeedType::SEED_ZOMBIE_SUPER_FAN_IMP:
            offsetY = -17.0;
            offsetX = -12.0;
            theDrawScale = 0.4;
            break;
        case SeedType::SEED_ZOMBONI:
            offsetY = 3.0;
            offsetX = -5.0;
            theDrawScale = 0.23;
            break;
        case SeedType::SEED_ZOMBIE_CATAPULT:
            offsetY = 3.0;
            offsetX = 1.0;
            theDrawScale = 0.23;
            break;
        case SeedType::SEED_ZOMBIE_GARGANTUAR:
        case SeedType::SEED_ZOMBIE_REDEYE_GARGANTUAR:
            offsetY = 3.0;
            offsetX = 4.0;
            theDrawScale = 0.23;
            break;
        case SeedType::SEED_ZOMBIE_YETI:
            offsetY = -7.0;
            offsetX = 1.0;
            theDrawScale = 0.32;
            break;
        case SeedType::SEED_ZOMBIE_BOSS:
            offsetY = 5.0;
            offsetX = 1.0;
            theDrawScale = 0.18;
            break;
        case SeedType::SEED_ZOMBIE_PEA_HEAD:
        case SeedType::SEED_ZOMBIE_WALLNUT_HEAD:
        case SeedType::SEED_ZOMBIE_JALAPENO_HEAD:
        case SeedType::SEED_ZOMBIE_GATLINGPEA_HEAD:
        case SeedType::SEED_ZOMBIE_SQUASH_HEAD:
        case SeedType::SEED_ZOMBIE_TALLNUT_HEAD:
        case SeedType::SEED_ZOMBIE_SUNFLOWER_HEAD:
        case SeedType::SEED_ZOMBIE_TORCHWOOD_HEAD:
        case SeedType::SEED_ZOMBIE_EXPLODE_O_NUT_HEAD:
            offsetY = -7.0;
            offsetX = -3.0;
            theDrawScale = 0.35;
            break;
        case SeedType::SEED_ZOMBIE_BOBSLED:
            offsetY = -10.0;
            offsetX = -3.0;
            theDrawScale = 0.35;
            break;
        default:
            offsetY = 8.0;
            offsetX = 5.0;
            theDrawScale = 0.5;
            break;
    }
    LawnApp *lawnApp = (LawnApp *)*gLawnApp_Addr;
    float v28, v29;
    if (lawnApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BIG_TIME) {
        if (realSeedType == SeedType::SEED_SUNFLOWER || realSeedType == SeedType::SEED_WALLNUT || realSeedType == SeedType::SEED_MARIGOLD) {
            offsetY = 34.0;
            offsetX = 16.0;
        }
    }
    v28 = offsetX * g->mScaleX;
    v29 = (offsetY + 1.0f) * g->mScaleY;
    if (realSeedType == SeedType::SEED_GIANT_WALLNUT) {
        v29 = 59.0;
        theDrawScale = theDrawScale * 0.75;
        v28 = 52.0;
    }

    if (isPlant && isSeedPacketSelected)
        DrawSeedType(g, x, y, realSeedType, theImitaterType, v28, v29, theDrawScale);

    if (thePercentDark > 0.0) {
        float coolDownHeight = thePercentDark * 68.0 + 2.5;
        Graphics aPlantG(*g);
        Color theColor = {64, 64, 64, 255};
        aPlantG.SetColor(theColor);
        aPlantG.SetColorizeImages(true);
        aPlantG.ClipRect(x, y, g->mScaleX * 50.0f, coolDownHeight * g->mScaleY);
        if (isSeedPacketSelected) {
            if (Challenge::IsMPSeedType(theSeedType)) {
                if (Zombie::IsUpgrade(theSeedType)) {
                    TodDrawImageCelScaledF(&aPlantG, *Sexy_IMAGE_SEEDS_Addr, x, y, celToDraw, 0, g->mScaleX, g->mScaleY);
                } else {
                    TodDrawImageScaledF(&aPlantG, *Sexy_IMAGE_ZOMBIE_SEEDPACKET_Addr, x, y, g->mScaleX, g->mScaleY);
                }
            } else {
                TodDrawImageCelScaledF(&aPlantG, *Sexy_IMAGE_SEEDS_Addr, x, y, celToDraw, 0, g->mScaleX, g->mScaleY);
            }
        }
        if (isPlant && isSeedPacketSelected)
            DrawSeedType(&aPlantG, x, y, theSeedType, theImitaterType, v28, v29, theDrawScale);
    }
    if (theDrawCost) {
        pvzstl::string str;
        Board *board = lawnApp->mBoard;
        if (board != nullptr && board->PlantUsesAcceleratedPricing(realSeedType)) {
            if (theUseCurrentCost) {
                int CurrentPlantCost = board->GetCurrentPlantCost(theSeedType, theImitaterType);
                str = StrFormat("%d", CurrentPlantCost);
            } else {
                int Cost = Plant::GetCost(theSeedType, theImitaterType);
                str = StrFormat("%d+", Cost);
            }
        } else {
            int Cost = Plant::GetCost(theSeedType, theImitaterType);
            str = StrFormat("%d", Cost);
        }
        Sexy::Font *font = *Sexy_FONT_BRIANNETOD12_Addr;
        int width = 31 - (*((int (**)(Sexy::Font *, int *))font->vTable + 8))(font, (int *)&str);
        ;                                                                      // 33  ----- >  31，微调一下文字位置，左移2个像素点
        int height = 48 + (*((int (**)(Sexy::Font *))font->vTable + 2))(font); // 50  ---- >  48, 微调一下文字位置，上移2个像素点
        Color theColor = {0, 0, 0, 255};
        g->PushState();
        if (g->mScaleX == 1.0 && g->mScaleY == 1.0) {
            TodDrawString(g, str, width + x, height + y, font, theColor, DrawStringJustification::DS_ALIGN_LEFT);
        } else {
            SexyMatrix3 aMatrix;
            TodScaleTransformMatrix(aMatrix, x + g->mTransX + width * g->mScaleX, y + g->mTransY + height * g->mScaleY - 1.0, g->mScaleX, g->mScaleY);
            if (g->mScaleX > 1.8) {
                g->SetLinearBlend(false);
            }
            TodDrawStringMatrix(g, font, aMatrix, str, theColor);
            g->SetLinearBlend(true);
        }
        g->PopState();
    }
    g->SetColorizeImages(false);
}


void SeedPacket::WasPlanted(int player) {
    old_SeedPacket_WasPlanted(this,player);
    if (tcpClientSocket >= 0) {
        TwoCharDataEvent event = {EventType::EVENT_SERVER_BOARD_SEEDPACKET_WASPLANTED, (unsigned char)mIndex, mSeedBank == mBoard->mSeedBank1};
        send(tcpClientSocket,&event,sizeof (TwoCharDataEvent),0);
    }
}