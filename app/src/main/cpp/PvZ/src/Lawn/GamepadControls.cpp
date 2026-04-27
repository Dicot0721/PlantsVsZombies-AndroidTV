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

#include "PvZ/Lawn/GamepadControls.h"
#include "Homura/Logger.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Board/Challenge.h"
#include "PvZ/Lawn/Board/CursorObject.h"
#include "PvZ/Lawn/Board/Plant.h"
#include "PvZ/Lawn/Board/SeedBank.h"
#include "PvZ/Lawn/Board/ZenGarden.h"
#include "PvZ/Lawn/Board/Zombie.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/Widget/VSSetupAddonWidget.h"
#include "PvZ/NetPlay.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Effect/Attachment.h"
#include "PvZ/TodLib/Effect/Reanimator.h"

using namespace Sexy;

// GamepadControls::GamepadControls(Board *theBoard, int thePlayerIndex1, int thePlayerIndex2) {
// _constructor(theBoard, thePlayerIndex1, thePlayerIndex2);
//}

void GamepadControls::_constructor(Board *theBoard, int thePlayerIndex1, int thePlayerIndex2) {
    old_GamepadControls_GamepadControls(this, theBoard, thePlayerIndex1, thePlayerIndex2);

    if (isKeyboardTwoPlayerMode)
        return;

    GameMode aGameMode = mGameObject->mApp->mGameMode;
    bool isTwoSeedBankMode = (aGameMode == GameMode::GAMEMODE_MP_VS || (aGameMode >= GameMode::GAMEMODE_TWO_PLAYER_COOP_DAY && aGameMode <= GameMode::GAMEMODE_TWO_PLAYER_COOP_ENDLESS));
    if (!keyboardMode && !isTwoSeedBankMode && aGameMode != GameMode::GAMEMODE_CHALLENGE_SLOT_MACHINE) {
        mIsInShopSeedBank = true; // 是否在Shop栏。
    }
}

void GamepadControls_pickUpCobCannon(int gamePad, int cobCannon) {
    // 用于拿起指定的加农炮
    int v8;        // r6
    int v9;        // r1
    int *v10;      // r0
    int v11;       // r3
    int v12;       // r3
    int v13;       // r7
    int v14;       // r6
    int v15;       // r1
    int v16;       // s14
    int v17;       // s15
    uint32_t *v18; // r3
    int v19;       // r3

    v8 = 0;
    v9 = *((uint32_t *)gamePad + 37);
    v10 = (int *)*((uint32_t *)gamePad + 14);
    if (v9)
        v11 = 140;
    else
        v11 = 141;
    v12 = *((uint32_t *)v10 + v11);
    if (*(uint32_t *)(v12 + 152) != -1) {
        if (*(uint8_t *)(v12 + 200)) {
            v19 = *(uint32_t *)(v12 + 196);
            if (v19) {
                if ((unsigned int)(unsigned int)v19 < *((uint32_t *)v10 + 79)) {
                    if (v19 == *(uint32_t *)(*((uint32_t *)v10 + 77) + 352 * (unsigned int)v19 + 348))
                        v8 = *((uint32_t *)v10 + 77) + 352 * (unsigned int)v19;
                    else
                        v8 = 0;
                }
            } else {
                v8 = 0;
            }
        } else {
            v8 = *(uint8_t *)(v12 + 200);
        }
    }
    v13 = cobCannon;
    if (cobCannon != v8 && *(uint32_t *)(cobCannon + 76) == 37 && *((uint32_t *)gamePad + 24) != 8) {
        v14 = *((uint8_t *)gamePad + 200);
        if (!*((uint8_t *)gamePad + 200)) {
            Board *aBoard = reinterpret_cast<Board *>(v10); // 显式类型转换
            aBoard->ClearCursor(v9);
            v15 = *(uint32_t *)(v13 + 348);
            v16 = (int)*((float *)gamePad + 27);
            v17 = (int)*((float *)gamePad + 28);
            v18 = (uint32_t *)(*((uint32_t *)gamePad + 14) + 22528);
            v18[29] = 30;
            v18[30] = v16;
            v18[31] = v17;
            *((uint32_t *)gamePad + 49) = v15;
            *((uint32_t *)gamePad + 55) = v14;
            *((uint8_t *)gamePad + 200) = 1;
        }
    }
}

void GamepadControls::Draw(Sexy::Graphics *g) {
    // 实现在光标内绘制铲子和黄油手套(黄油手套其实就是花园的手套),并在锤僵尸关卡绘制种植预览


    if (mPlayerIndex2 != -1) {
        LawnApp *anApp = mGameObject->mApp;
        bool is2P = mPlayerIndex1 == 1;
        CursorObject *aCursorObject = is2P ? mBoard->mCursorObject[1] : mBoard->mCursorObject[0];


        if (!anApp->IsCoopMode()) {
            requestDrawButterInCursor = false;
        }
        if (!mBoard->mShowShovel) {
            requestDrawShovelInCursor = false;
        }

        if (requestDrawButterInCursor) {
            if (is2P) {
                g->DrawImage(addonImages.butter_glove, mCursorPositionX, mCursorPositionY);
            }
        }

        if (requestDrawShovelInCursor) {
            if (anApp->mGameMode == GameMode::GAMEMODE_MP_VS) {
                if (!mIsZombie) {
                    aCursorObject->mCursorType = CursorType::CURSOR_TYPE_SHOVEL;
                    aCursorObject->mX = mCursorPositionX - 20;
                    aCursorObject->mY = mCursorPositionY - 20;
                    if (aCursorObject->BeginDraw(g)) {
                        aCursorObject->Draw(g);
                        aCursorObject->EndDraw(g);
                    }
                }
            } else if (!is2P) {
                if (anApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_LAST_STAND) {
                    if (mBoard->mChallenge->mChallengeState == ChallengeState::STATECHALLENGE_NORMAL && anApp->mGameScene == GameScenes::SCENE_PLAYING) {
                        aCursorObject->mCursorType = CursorType::CURSOR_TYPE_MONEY_SIGN;
                        aCursorObject->mX = mCursorPositionX;
                        aCursorObject->mY = mCursorPositionY;
                    } else {
                        aCursorObject->mCursorType = CursorType::CURSOR_TYPE_SHOVEL;
                        aCursorObject->mX = mCursorPositionX - 20;
                        aCursorObject->mY = mCursorPositionY - 20;
                    }
                } else {
                    aCursorObject->mCursorType = CursorType::CURSOR_TYPE_SHOVEL;
                    aCursorObject->mX = mCursorPositionX - 20;
                    aCursorObject->mY = mCursorPositionY - 20;
                }
                if (aCursorObject->BeginDraw(g)) {
                    aCursorObject->Draw(g);
                    aCursorObject->EndDraw(g);
                }
            }
        }


        if (anApp->IsWhackAZombieLevel()) {
            int theGridX = mBoard->PixelToGridXKeepOnBoard(mCursorPositionX, mCursorPositionY);
            int theGridY = mBoard->PixelToGridYKeepOnBoard(mCursorPositionX, mCursorPositionY);
            int thePixelX = mBoard->GridToPixelX(theGridX, theGridY);
            int thePixelY = mBoard->GridToPixelY(theGridX, theGridY);
            g->mTransX += thePixelX;
            g->mTransY += thePixelY;
            DrawPreview(g);
            g->mTransX -= thePixelX;
            g->mTransY -= thePixelY;
        }
    }
    if (mIsCobCannonSelected && useNewCobCannon && !keyboardMode) {
        Sexy::Image *cobcannon_1 = Sexy::IMAGE_COBCANNON_TARGET_1;
        Sexy::IMAGE_COBCANNON_TARGET_1 = addonImages.custom_cobcannon;
        old_GamepadControls_Draw(this, g);
        Sexy::IMAGE_COBCANNON_TARGET_1 = cobcannon_1;
        return;
    }
    // 联机光标上绘制双方玩家昵称
    if (gTcpConnected || gTcpClientSocket >= 0) {
        if (mPlayerIndex1 == 0 && gSecondPlayerName[0] != '\0') {
            Image *tmp1 = Sexy::IMAGE_CURSOR_P1_TEXT;
            Sexy::IMAGE_CURSOR_P1_TEXT = IMAGE_BLANK;
            old_GamepadControls_Draw(this, g);
            TodDrawString(g,
                          gTcpConnected ? gSecondPlayerName : mBoard->mApp->mPlayerInfo->mName,
                          mCursorPositionX - 5,
                          mCursorPositionY - 60,
                          Sexy::FONT_DWARVENTODCRAFT18,
                          Color(255, 242, 14, 255),
                          DrawStringJustification::DS_ALIGN_CENTER);
            Sexy::IMAGE_CURSOR_P1_TEXT = tmp1;
            return;
        }
        if (mPlayerIndex1 == 1 && gSecondPlayerName[0] != '\0') {
            Image *tmp = Sexy::IMAGE_CURSOR_P2_TEXT;
            Sexy::IMAGE_CURSOR_P2_TEXT = IMAGE_BLANK;
            old_GamepadControls_Draw(this, g);
            TodDrawString(g,
                          gTcpClientSocket >= 0 ? gSecondPlayerName : mBoard->mApp->mPlayerInfo->mName,
                          mCursorPositionX - 5,
                          mCursorPositionY - 60,
                          Sexy::FONT_DWARVENTODCRAFT18,
                          Color(68, 207, 255, 255),
                          DrawStringJustification::DS_ALIGN_CENTER);
            Sexy::IMAGE_CURSOR_P2_TEXT = tmp;
            return;
        }
    }
    old_GamepadControls_Draw(this, g);
}

void GamepadControls::Update(float a2) {
    LawnApp *anApp = mGameObject->mApp;
    int aGridX = mBoard->PixelToGridXKeepOnBoard(mCursorPositionX, mCursorPositionY);
    int aGridY = mBoard->PixelToGridYKeepOnBoard(mCursorPositionX, mCursorPositionY);
    int aGridCenterPosX = mBoard->GridToPixelX(aGridX, aGridY) + mBoard->GridCellWidth(aGridX, aGridY) / 2;
    int aGridCenterPosY = mBoard->GridToPixelY(aGridX, aGridY) + mBoard->GridCellHeight(aGridX, aGridY) / 2;
    // 键盘双人模式 平滑移动光标
    if (isKeyboardTwoPlayerMode) {
        int aGamepadIndex = mGameObject->mApp->PlayerToGamepadIndex(mPlayerIndex1);
        if (aGamepadIndex == 0) {
            mGamepadVelocityLeftX = gGamepadP1VelX;
            mGamepadVelocityLeftY = gGamepadP1VelY;
            if (gGamepadP1VelX == 0) {
                mCursorPositionX += (aGridCenterPosX - mCursorPositionX) / 10;
            }
            if (gGamepadP1VelY == 0) {
                mCursorPositionY += (aGridCenterPosY - mCursorPositionY) / 10;
            }
        } else if (aGamepadIndex == 1) {
            mGamepadVelocityLeftX = gGamepadP2VelX;
            mGamepadVelocityLeftY = gGamepadP2VelY;
            if (gGamepadP2VelX == 0) {
                mCursorPositionX += (aGridCenterPosX - mCursorPositionX) / 10;
            }
            if (gGamepadP2VelY == 0) {
                mCursorPositionY += (aGridCenterPosY - mCursorPositionY) / 10;
            }
        }
    }

    if (positionAutoFix && !anApp->IsWhackAZombieLevel() && anApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZOMBIQUARIUM && gTcpServerSocket == -1) {
        if (this == mBoard->mGamepadControls[0] && gPlayerIndex != TouchPlayerIndex::TOUCHPLAYER_PLAYER1 && gPlayerIndexSecond != TouchPlayerIndex::TOUCHPLAYER_PLAYER1) {
            mCursorPositionX += (aGridCenterPosX - mCursorPositionX) / 10;
            mCursorPositionY += (aGridCenterPosY - mCursorPositionY) / 10;
        }
        if (this == mBoard->mGamepadControls[1] && gPlayerIndex != TouchPlayerIndex::TOUCHPLAYER_PLAYER2 && gPlayerIndexSecond != TouchPlayerIndex::TOUCHPLAYER_PLAYER2) {
            mCursorPositionX += (aGridCenterPosX - mCursorPositionX) / 10;
            mCursorPositionY += (aGridCenterPosY - mCursorPositionY) / 10;
        }
    }


    old_GamepadControls_Update(this, a2);

    // Reanimation *mCursorReanim = ReanimationTryToGet(gamepadControls->mGameObject->anApp, gamepadControls->mCursorReanimID);
    // LOGD("%d",mCursorReanim);
    // if (mCursorReanim != nullptr) {
    // if ((gamepadControls->mPlayerIndex2 == 0 &&(mIsZombie == TouchPlayerIndex::TOUCHPLAYER_PLAYER1 || gPlayerIndexSecond == TouchPlayerIndex::TOUCHPLAYER_PLAYER1)) ||
    // (gamepadControls->mPlayerIndex2 == 1
    // &&(mIsZombie == TouchPlayerIndex::TOUCHPLAYER_PLAYER2 || gPlayerIndexSecond == TouchPlayerIndex::TOUCHPLAYER_PLAYER2))) {
    // if (!Reanimation_IsAnimPlaying(mCursorReanim, "anim_depressed"))
    // Reanimation_PlayReanim(mCursorReanim, "anim_depressed", a::REANIM_LOOP, 0,12.0);
    // LOGD("456456");
    // } else if (!Reanimation_IsAnimPlaying(mCursorReanim, "anim_bounce")) {
    // Reanimation_PlayReanim(mCursorReanim, "anim_bounce", a::REANIM_LOOP, 0, 12.0);
    // }
    // }

    if (!isKeyboardTwoPlayerMode && !anApp->CanShopLevel() && mGamepadState == 6 && mIsInShopSeedBank) {
        mIsInShopSeedBank = false;
    }
}

void GamepadControls::ButtonDownFireCobcannonTest() {
    // 解除加农炮选取半秒后才能发射的限制
    mBoard->mCobCannonCursorDelayCounter = 0;

    old_GamepadControls_ButtonDownFireCobcannonTest(this);
}

void GamepadControls::InvalidatePreviewReanim() {
    Reanimation *aPreviewReanim4 = mGameObject->mApp->ReanimationTryToGet(mPreviewReanimID4);
    if (aPreviewReanim4 != nullptr) {
        aPreviewReanim4->ReanimationDie();
        mPreviewReanimID4 = ReanimationID::REANIMATIONID_NULL;
    }

    if (mPreviewReanimID3 != 0) {
        mGameObject->mApp->RemoveReanimation(mPreviewReanimID3);
        mPreviewReanimID3 = REANIMATIONID_NULL;
    }
}

FilterEffect GetFilterEffectTypeBySeedType(SeedType mSeedType) {
    if (mSeedType == SeedType::SEED_HYPNOSHROOM || mSeedType == SeedType::SEED_SQUASH || mSeedType == SeedType::SEED_POTATOMINE || mSeedType == SeedType::SEED_GARLIC
        || mSeedType == SeedType::SEED_LILYPAD) {
        return FilterEffect::FILTEREFFECT_LESS_WASHED_OUT;
    }

    return FilterEffect::FILTEREFFECT_WASHED_OUT;
}

void GamepadControls::UpdatePreviewReanim() {
    // 动态预览!!

    // TV后续版本仅在PreviewingSeedType切换时进行一次Reanimation::Update，而TV 1.0.1则是无时无刻进行Reanimation::Update。我们恢复1.0.1的逻辑即可。

    LawnApp *anApp = mGameObject->mApp;
    CursorObject *aCursorObject = mPlayerIndex1 ? mBoard->mCursorObject[1] : mBoard->mCursorObject[0];
    SeedBank *aSeedBank = GetSeedBank();

    if (!dynamicPreview) { // 如果没开启动态预览，则开启砸罐子无尽和锤僵尸关卡的动态预览，并执行旧游戏函数。
        if ((anApp->IsWhackAZombieLevel() || anApp->IsScaryPotterLevel()) && mGamepadState == 7) {
            if (aSeedBank == nullptr || mSelectedSeedIndex < 0 || mSelectedSeedIndex >= 10) {
                old_GamepadControls_UpdatePreviewReanim(this);
                return;
            }
            SeedPacket *seedPacket = &aSeedBank->mSeedPackets[mSelectedSeedIndex];
            aCursorObject->mType = seedPacket->mPacketType;
            aCursorObject->mImitaterType = seedPacket->mImitaterType;
        }
        old_GamepadControls_UpdatePreviewReanim(this);
        return;
    }

    GameMode aGameMode = anApp->mGameMode;
    int aGridX = mBoard->PixelToGridXKeepOnBoard(mCursorPositionX, mCursorPositionY);
    int aGridY = mBoard->PixelToGridYKeepOnBoard(mCursorPositionX, mCursorPositionY);
    if (aSeedBank == nullptr || mSelectedSeedIndex < 0 || mSelectedSeedIndex >= 10) {
        return;
    }
    SeedType aSeedType = aCursorObject->mType;
    bool isImitater = aSeedBank->mSeedPackets[mSelectedSeedIndex].mPacketType == SeedType::SEED_IMITATER;

    if ((anApp->IsWhackAZombieLevel() || aGameMode == GameMode::GAMEMODE_SCARY_POTTER_ENDLESS) && mGamepadState == 7) {
        // 开启砸罐子无尽和锤僵尸关卡的动态预览
        SeedPacket *seedPacket = &aSeedBank->mSeedPackets[mSelectedSeedIndex];
        aCursorObject->mType = seedPacket->mPacketType;
        aCursorObject->mImitaterType = seedPacket->mImitaterType;
    }

    if (mIsZombie) {
        aSeedType = aSeedBank->mSeedPackets[mSelectedSeedIndex].mPacketType;
    }
    if (aSeedType == SeedType::SEED_IMITATER) {
        aSeedType = aCursorObject->mImitaterType;
    }

    bool flagUpdateCanPlant = true;
    bool flagDrawGray = false;
    bool flagUpdateChangeType = false;
    if (mPreviewingSeedType != aSeedType && aSeedType != SeedType::SEED_NONE) {
        Reanimation *aNewPreviewingReanim = nullptr;
        InvalidatePreviewReanim();
        RenderLayer aRenderLayer = mIsZombie ? RENDER_LAYER_ZOMBIE : RENDER_LAYER_PLANT;
        int aRenderOrder = Board::MakeRenderOrder(aRenderLayer, aGridY, 100);
        float theDrawHeightOffset = PlantDrawHeightOffset(mBoard, nullptr, aSeedType, aGridX, aGridY);
        if (mIsZombie) {
            ZombieType aZombieType = Challenge::IZombieSeedTypeToZombieType(aSeedType);
            switch (aZombieType) {
                case ZombieType::ZOMBIE_INVALID:
                    mPreviewingSeedType = aSeedType;
                    return;
                case ZombieType::ZOMBIE_GARGANTUAR:
                case ZombieType::ZOMBIE_REDEYE_GARGANTUAR: // 在对战里可能用得到
                    theDrawHeightOffset += 30.0;
                    break;
                case ZombieType::ZOMBIE_POLEVAULTER:
                    theDrawHeightOffset += 15.0;
                    break;
                default:
                    break;
            }
            ZombieDefinition &theZombieDefinition = GetZombieDefinition(aZombieType);
            Reanimation *zombieReanim = anApp->AddReanimation(-20.0, -35 - theDrawHeightOffset, aRenderOrder + 1, theZombieDefinition.mReanimationType);
            Zombie::SetupReanimLayers(zombieReanim, aZombieType);
            if (aZombieType == ZombieType::ZOMBIE_DOOR || aZombieType == ZombieType::ZOMBIE_TRASHCAN || aZombieType == ZombieType::ZOMBIE_NEWSPAPER || aZombieType == ZombieType::ZOMBIE_LADDER) {
                Zombie::SetupShieldReanims(aZombieType, zombieReanim);
            }
            zombieReanim->mIsAttachment = true;
            if (aZombieType == ZombieType::ZOMBIE_POGO) {
                zombieReanim->PlayReanim("anim_pogo", ReanimLoopType::REANIM_LOOP, 0, 12.0);
            } else if (aZombieType == ZombieType::ZOMBIE_DANCER) {
                zombieReanim->PlayReanim("anim_armraise", ReanimLoopType::REANIM_LOOP, 0, 12.0);
            } else if (aZombieType == ZombieType::ZOMBIE_ZAMBONI) {
                zombieReanim->PlayReanim("anim_drive", ReanimLoopType::REANIM_LOOP, 0, 12.0);
            } else if (aZombieType == ZombieType::ZOMBIE_IMP) {
                zombieReanim->PlayReanim("anim_walk", ReanimLoopType::REANIM_LOOP, 0, 12.0);
            } else {
                if (aZombieType == ZombieType::ZOMBIE_FLAG) {
                    Reanimation *zombieReanimAttachment = anApp->AddReanimation(0, 0, 0, ReanimationType::REANIM_ZOMBIE_FLAGPOLE);
                    zombieReanimAttachment->PlayReanim("Zombie_flag", ReanimLoopType::REANIM_LOOP, 0, 15.0);
                    mPreviewReanimID3 = anApp->ReanimationGetID(zombieReanimAttachment);
                    ReanimatorTrackInstance *TrackInstanceByName = zombieReanim->GetTrackInstanceByName("Zombie_flaghand");
                    AttachReanim(TrackInstanceByName->mAttachmentID, zombieReanimAttachment, 0.0, 0.0);
                    zombieReanim->mFrameBasePose = 0;
                } else if (aZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR) {
                    zombieReanim->SetImageOverride("anim_head1", Sexy::IMAGE_REANIM_ZOMBIE_GARGANTUAR_HEAD_REDEYE);
                } else if (aZombieType == ZombieType::ZOMBIE_PEA_HEAD) {
                    zombieReanim->HideTrackByPrefix("anim_hair", true);
                    zombieReanim->HideTrackByPrefix("anim_head2", true);
                    zombieReanim->SetFramesForLayer("anim_walk2");
                    ReanimatorTrackInstance *aTrackInstance = zombieReanim->GetTrackInstanceByName("anim_head1");
                    aTrackInstance->mImageOverride = IMAGE_BLANK;
                    Reanimation *aPeaHeadReanim = anApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_PEASHOOTER);
                    aPeaHeadReanim->PlayReanim("anim_head_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
                    AttachEffect *aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aPeaHeadReanim, 0.0f, 0.0f);
                    zombieReanim->mFrameBasePose = 0;
                    TodScaleRotateTransformMatrix((SexyMatrix3 &)aAttachEffect->mOffset, 65.0f, -8.0f, 0.2f, -1.0f, 1.0f);
                } else if (aZombieType == ZombieType::ZOMBIE_WALLNUT_HEAD) {
                    zombieReanim->HideTrackByPrefix("anim_hair", true);
                    zombieReanim->HideTrackByPrefix("anim_head", true);
                    zombieReanim->HideTrackByPrefix("Zombie_tie", true);
                    zombieReanim->SetFramesForLayer("anim_walk2");
                    ReanimatorTrackInstance *aTrackInstance = zombieReanim->GetTrackInstanceByName("zombie_body");
                    Reanimation *aWallnutHeadReanim = anApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_WALLNUT);
                    aWallnutHeadReanim->PlayReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
                    AttachEffect *aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aWallnutHeadReanim, 0.0f, 0.0f);
                    zombieReanim->mFrameBasePose = 0;
                    TodScaleRotateTransformMatrix((SexyMatrix3 &)aAttachEffect->mOffset, 50.0f, 0.0f, 0.2f, -0.8f, 0.8f);
                } else if (aZombieType == ZombieType::ZOMBIE_TALLNUT_HEAD) {
                    zombieReanim->HideTrackByPrefix("anim_hair", true);
                    zombieReanim->HideTrackByPrefix("anim_head", true);
                    zombieReanim->HideTrackByPrefix("Zombie_tie", true);
                    zombieReanim->SetFramesForLayer("anim_walk2");
                    ReanimatorTrackInstance *aTrackInstance = zombieReanim->GetTrackInstanceByName("zombie_body");
                    Reanimation *aTallnutHeadReanim = anApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_TALLNUT);
                    aTallnutHeadReanim->PlayReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
                    AttachEffect *aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aTallnutHeadReanim, 0.0f, 0.0f);
                    zombieReanim->mFrameBasePose = 0;
                    TodScaleRotateTransformMatrix((SexyMatrix3 &)aAttachEffect->mOffset, 37.0f, 0.0f, 0.2f, -0.8f, 0.8f);
                } else if (aZombieType == ZombieType::ZOMBIE_JALAPENO_HEAD) {
                    zombieReanim->HideTrackByPrefix("anim_hair", true);
                    zombieReanim->HideTrackByPrefix("anim_head", true);
                    zombieReanim->HideTrackByPrefix("Zombie_tie", true);
                    zombieReanim->SetFramesForLayer("anim_walk2");
                    ReanimatorTrackInstance *aTrackInstance = zombieReanim->GetTrackInstanceByName("zombie_body");
                    Reanimation *aJalapenoHeadReanim = anApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_JALAPENO);
                    aJalapenoHeadReanim->PlayReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
                    AttachEffect *aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aJalapenoHeadReanim, 0.0f, 0.0f);
                    zombieReanim->mFrameBasePose = 0;
                    TodScaleRotateTransformMatrix((SexyMatrix3 &)aAttachEffect->mOffset, 55.0f, -5.0f, 0.2f, -1.0f, 1.0f);
                } else if (aZombieType == ZombieType::ZOMBIE_GATLING_HEAD) {
                    zombieReanim->HideTrackByPrefix("anim_hair", true);
                    zombieReanim->HideTrackByPrefix("anim_head2", true);
                    zombieReanim->SetFramesForLayer("anim_walk2");
                    ReanimatorTrackInstance *aTrackInstance = zombieReanim->GetTrackInstanceByName("anim_head1");
                    aTrackInstance->mImageOverride = IMAGE_BLANK;
                    Reanimation *aGatlingHeadReanim = anApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_GATLINGPEA);
                    aGatlingHeadReanim->PlayReanim("anim_head_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
                    AttachEffect *aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aGatlingHeadReanim, 0.0f, 0.0f);
                    zombieReanim->mFrameBasePose = 0;
                    TodScaleRotateTransformMatrix((SexyMatrix3 &)aAttachEffect->mOffset, 65.0f, -5.0f, 0.2f, -1.0f, 1.0f);
                } else if (aZombieType == ZombieType::ZOMBIE_SQUASH_HEAD) {
                    zombieReanim->HideTrackByPrefix("anim_hair", true);
                    zombieReanim->HideTrackByPrefix("anim_head2", true);
                    zombieReanim->SetFramesForLayer("anim_walk2");
                    ReanimatorTrackInstance *aTrackInstance = zombieReanim->GetTrackInstanceByName("anim_head1");
                    aTrackInstance->mImageOverride = IMAGE_BLANK;
                    Reanimation *aSquashHeadReanim = anApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_SQUASH);
                    aSquashHeadReanim->PlayReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
                    AttachEffect *aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aSquashHeadReanim, 0.0f, 0.0f);
                    zombieReanim->mFrameBasePose = 0;
                    TodScaleRotateTransformMatrix((SexyMatrix3 &)aAttachEffect->mOffset, 55.0f, -15.0f, 0.2f, -0.75f, 0.75f);
                }
                zombieReanim->PlayReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, 12.0);
            }
            aNewPreviewingReanim = zombieReanim;
        } else {
            if (aSeedType >= SeedType::NUM_SEED_TYPES || aSeedType == SeedType::NUM_SEEDS_IN_CHOOSER)
                return;
            Reanimation *plantReanim = anApp->AddReanimation(0.0, theDrawHeightOffset, aRenderOrder + 2, GetPlantDefinition(aSeedType).mReanimationType);
            plantReanim->mIsAttachment = true;
            if (isImitater)
                plantReanim->mFilterEffect = GetFilterEffectTypeBySeedType(aSeedType);
            plantReanim->PlayReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, 12.0);

            // 为豌豆家族加入其stem动画
            if (aSeedType == SeedType::SEED_PEASHOOTER || aSeedType == SeedType::SEED_SNOWPEA || aSeedType == SeedType::SEED_REPEATER || aSeedType == SeedType::SEED_GATLINGPEA
                || aSeedType == SeedType::SEED_LEFTPEATER) {
                Reanimation *plantReanimAttachment = anApp->AddReanimation(0.0, theDrawHeightOffset, aRenderOrder + 3, GetPlantDefinition(aSeedType).mReanimationType);
                plantReanimAttachment->mLoopType = ReanimLoopType::REANIM_LOOP;
                if (isImitater)
                    plantReanimAttachment->mFilterEffect = GetFilterEffectTypeBySeedType(aSeedType);
                plantReanimAttachment->SetFramesForLayer("anim_head_idle");
                const char *trackName = "anim_stem";
                if (plantReanim->TrackExists(trackName) || (trackName = "anim_idle", plantReanim->TrackExists(trackName))) {
                    plantReanimAttachment->AttachToAnotherReanimation(plantReanim, trackName);
                }
            }
            // 为反向射手的两个头、三发射手的三个头加入动画
            if (aSeedType == SeedType::SEED_SPLITPEA) {
                Reanimation *plantReanimAttachment1 = anApp->AddReanimation(0.0, 0.0, aRenderOrder + 3, GetPlantDefinition(aSeedType).mReanimationType);
                plantReanimAttachment1->mAnimRate = plantReanim->mAnimRate;
                plantReanimAttachment1->mLoopType = ReanimLoopType::REANIM_LOOP;
                if (isImitater)
                    plantReanimAttachment1->mFilterEffect = GetFilterEffectTypeBySeedType(aSeedType);
                plantReanimAttachment1->SetFramesForLayer("anim_head_idle");
                plantReanimAttachment1->AttachToAnotherReanimation(plantReanim, "anim_idle");
                Reanimation *plantReanimAttachment2 = anApp->AddReanimation(0.0, 0.0, aRenderOrder + 3, GetPlantDefinition(aSeedType).mReanimationType);
                plantReanimAttachment2->mAnimRate = plantReanim->mAnimRate;
                plantReanimAttachment2->mLoopType = ReanimLoopType::REANIM_LOOP;
                if (isImitater)
                    plantReanimAttachment2->mFilterEffect = GetFilterEffectTypeBySeedType(aSeedType);
                plantReanimAttachment2->SetFramesForLayer("anim_splitpea_idle");
                plantReanimAttachment2->AttachToAnotherReanimation(plantReanim, "anim_idle");
            } else if (aSeedType == SeedType::SEED_THREEPEATER) {
                plantReanim->mAnimRate = RandRangeFloat(15.0, 20.0);
                Reanimation *plantReanimAttachment1 = anApp->AddReanimation(0.0, 0.0, aRenderOrder + 3, GetPlantDefinition(aSeedType).mReanimationType);
                plantReanimAttachment1->mAnimRate = plantReanim->mAnimRate;
                plantReanimAttachment1->mLoopType = ReanimLoopType::REANIM_LOOP;
                if (isImitater)
                    plantReanimAttachment1->mFilterEffect = GetFilterEffectTypeBySeedType(aSeedType);
                plantReanimAttachment1->SetFramesForLayer("anim_head_idle1");
                plantReanimAttachment1->AttachToAnotherReanimation(plantReanim, "anim_head1");
                Reanimation *plantReanimAttachment2 = anApp->AddReanimation(0.0, 0.0, aRenderOrder + 3, GetPlantDefinition(aSeedType).mReanimationType);
                plantReanimAttachment2->mAnimRate = plantReanim->mAnimRate;
                plantReanimAttachment2->mLoopType = ReanimLoopType::REANIM_LOOP;
                if (isImitater)
                    plantReanimAttachment2->mFilterEffect = GetFilterEffectTypeBySeedType(aSeedType);
                plantReanimAttachment2->SetFramesForLayer("anim_head_idle2");
                plantReanimAttachment2->AttachToAnotherReanimation(plantReanim, "anim_head2");
                Reanimation *plantReanimAttachment3 = anApp->AddReanimation(0.0, 0.0, aRenderOrder + 3, GetPlantDefinition(aSeedType).mReanimationType);
                plantReanimAttachment3->mAnimRate = plantReanim->mAnimRate;
                plantReanimAttachment3->mLoopType = ReanimLoopType::REANIM_LOOP;
                if (isImitater)
                    plantReanimAttachment3->mFilterEffect = GetFilterEffectTypeBySeedType(aSeedType);
                plantReanimAttachment3->SetFramesForLayer("anim_head_idle3");
                plantReanimAttachment3->AttachToAnotherReanimation(plantReanim, "anim_head3");
            }
            aNewPreviewingReanim = plantReanim;
        }
        mPreviewingSeedType = aSeedType;
        mPreviewReanimID4 = anApp->ReanimationGetID(aNewPreviewingReanim);
        flagUpdateChangeType = true;
    } else {
        // 如果目标预览植物类型没变化, 则为模仿者上色
        Reanimation *mPreviewReanim4 = anApp->ReanimationTryToGet(mPreviewReanimID4);
        if (mPreviewReanim4 != nullptr) {
            FilterEffect aFilterEffect = isImitater ? GetFilterEffectTypeBySeedType(aSeedType) : FilterEffect::FILTEREFFECT_NONE;
            mPreviewReanim4->mFilterEffect = aFilterEffect;
            if (aSeedType == SeedType::SEED_THREEPEATER || aSeedType == SeedType::SEED_SPLITPEA || aSeedType == SeedType::SEED_PEASHOOTER || aSeedType == SeedType::SEED_SNOWPEA
                || aSeedType == SeedType::SEED_REPEATER || aSeedType == SeedType::SEED_GATLINGPEA || aSeedType == SeedType::SEED_LEFTPEATER) {
                int mTrackCount = mPreviewReanim4->mDefinition->mTrackCount;
                for (int i = 0; i < mTrackCount; i++) {
                    ReanimatorTrackInstance *reanimatorTrackInstance = mPreviewReanim4->mTrackInstances + i;
                    uint16_t mAttachmentID = reanimatorTrackInstance->mAttachmentID;
                    if (mAttachmentID == 0)
                        continue;
                    if (gEffectSystem == nullptr || gEffectSystem->mAttachmentHolder == nullptr)
                        break;
                    Attachment *attachment = gEffectSystem->mAttachmentHolder->mAttachments.DataArrayTryToGet(mAttachmentID);
                    if (attachment == nullptr)
                        continue;
                    int mNumEffects = attachment->mNumEffects;
                    for (int j = 0; j < mNumEffects; ++j) {
                        if (attachment->mEffectArray[j].mEffectType == EffectType::EFFECT_REANIM) {
                            Reanimation *attachReanim = anApp->ReanimationTryToGet(attachment->mEffectArray[j].mEffectID);
                            if (attachReanim != nullptr) {
                                attachReanim->mFilterEffect = aFilterEffect;
                            }
                        }
                    }
                }
            }
        }
    }

    Reanimation *mPreviewReanim4 = anApp->ReanimationTryToGet(mPreviewReanimID4);
    if (mPreviewReanim4 == nullptr)
        return;

    if (aCursorObject->mCursorType != CursorType::CURSOR_TYPE_PLANT_FROM_USABLE_COIN && mGamepadState != 7)
        return;
    if (mSelectedSeedIndex == -1)
        return;

    SeedPacket *seedPacket = &aSeedBank->mSeedPackets[mSelectedSeedIndex];
    if (!seedPacket->mActive) {
        flagUpdateCanPlant = false;
        flagDrawGray = true;
    }
    if (mBoard->CanPlantAt(aGridX, aGridY, aSeedType) != PlantingReason::PLANTING_OK) {
        flagUpdateCanPlant = false;
        flagDrawGray = true;
    }
    if (!mBoard->HasConveyorBeltSeedBank(mPlayerIndex2) && aCursorObject->mCursorType != CursorType::CURSOR_TYPE_PLANT_FROM_USABLE_COIN) {
        if (aGameMode == GameMode::GAMEMODE_MP_VS) {
            if (mIsZombie) {
                if (!mBoard->CanTakeDeathMoney(mBoard->GetCurrentPlantCost(aSeedType, SeedType::SEED_NONE))) {
                    flagUpdateCanPlant = false;
                    flagDrawGray = true;
                }
            } else {
                if (!mBoard->CanTakeSunMoney(mBoard->GetCurrentPlantCost(aSeedType, SeedType::SEED_NONE), 0)) {
                    flagUpdateCanPlant = false;
                    flagDrawGray = true;
                }
            }
        } else {
            if (!mBoard->CanTakeSunMoney(mBoard->GetCurrentPlantCost(aSeedType, SeedType::SEED_NONE), mPlayerIndex2)) {
                flagUpdateCanPlant = false;
                flagDrawGray = true;
            }
        }
    }

    Graphics newGraphics(mPreviewImage);
    newGraphics.ClearRect(0, 0, mPreviewImage->mWidth, mPreviewImage->mHeight);
    newGraphics.Translate(256, 256);
    if (flagUpdateCanPlant || flagUpdateChangeType)
        mPreviewReanim4->Update();
    if (flagDrawGray) {
        newGraphics.SetColorizeImages(true);
        (newGraphics).SetColor(gColorGray);
    }
    mPreviewReanim4->Draw(&newGraphics);
    mPreviewReanim4->DrawRenderGroup(&newGraphics, 2);
    mPreviewReanim4->DrawRenderGroup(&newGraphics, 1);
    mPreviewReanim4->DrawRenderGroup(&newGraphics, 3);
}

void GamepadControls::DrawPreview(Sexy::Graphics *g) {
    // 修复排山倒海、砸罐子无尽、锤僵尸、种子雨不显示植物预览的问题。
    LawnApp *anApp = mGameObject->mApp;
    GameMode mGameMode = anApp->mGameMode;
    if (mGameMode == GameMode::GAMEMODE_CHALLENGE_RAINING_SEEDS) { // 为种子雨添加种植预览
        CursorObject *cursorObject = mPlayerIndex1 ? mBoard->mCursorObject[1] : mBoard->mCursorObject[0];
        if (cursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_USABLE_COIN) {
            mGamepadState = 7;
            old_GamepadControls_DrawPreview(this, g);
            mGamepadState = 1;
            return;
        }
    }

    if (anApp->IsWhackAZombieLevel() || anApp->IsScaryPotterLevel()) {
        if (mGamepadState == 7) {
            SeedBank *seedBank = GetSeedBank();
            SeedPacket *seedPacket = &seedBank->mSeedPackets[mSelectedSeedIndex];
            mSelectedSeedType = seedPacket->mPacketType == SeedType::SEED_IMITATER ? seedPacket->mImitaterType : seedPacket->mPacketType;
            old_GamepadControls_DrawPreview(this, g);
            return;
        }
    }
    if (mGameMode == GameMode::GAMEMODE_CHALLENGE_COLUMN) {
        int mGridX = mBoard->PixelToGridXKeepOnBoard(mCursorPositionX, mCursorPositionY);
        int mGridY = mBoard->PixelToGridYKeepOnBoard(mCursorPositionX, mCursorPositionY);
        if (mSelectedSeedType != SeedType::SEED_NONE) {
            g->SetColorizeImages(true);
            Sexy::Color theColor = {255, 255, 255, 125};
            g->SetColor(theColor);
            g->Translate(-256, -256);
            if (dynamicPreview) { // 修复动态预览时植物错位
                int thePixelY = mBoard->GridToPixelY(mGridX, mGridY);
                for (int i = 0; i != 6; ++i) {
                    if (mBoard->CanPlantAt(mGridX, i, mSelectedSeedType) == PlantingReason::PLANTING_OK) {
                        int theGridPixelY = mBoard->GridToPixelY(mGridX, i);
                        g->DrawImage(mPreviewImage, 0, theGridPixelY - thePixelY);
                    }
                }
            } else {
                for (int i = 0; i != 6; ++i) {
                    if (mBoard->CanPlantAt(mGridX, i, mSelectedSeedType) == PlantingReason::PLANTING_OK) {
                        float offset = PlantDrawHeightOffset(mBoard, nullptr, mSelectedSeedType, mGridX, i);
                        g->DrawImage(mPreviewImage, 0, offset + (i - mGridY) * 85);
                    }
                }
            }
            g->Translate(256, 256);
            g->SetColorizeImages(false);
        }
        return;
    }

    if (mGameMode == GameMode::GAMEMODE_CHALLENGE_BIG_TIME
        && (mSelectedSeedType == SeedType::SEED_SUNFLOWER || mSelectedSeedType == SeedType::SEED_WALLNUT || mSelectedSeedType == SeedType::SEED_MARIGOLD)) {
        // 种大突破关卡 放大植物预览
        g->SetScale(1.5, 1.5, 0, 0);
        g->Translate(-15, -25);
        old_GamepadControls_DrawPreview(this, g);
        g->SetScale(1, 1, 0, 0);
        g->Translate(15, 25);
        return;
    }

    if (mSelectedSeedType == SEED_BEGHOULED_BUTTON_SHUFFLE || mSelectedSeedType == SEED_ZOMBIE_BEGHOULED_BUTTON_SHUFFLE) {
        return;
    }

    if (mGamepadState == 7 && mSelectedSeedType == SeedType::SEED_ZOMBIE_MOUND) {
        g->SetColorizeImages(true);
        Color aColor = Color(255, 255, 255, 125);
        g->SetColor(aColor);

        Image *aImage = Sexy::IMAGE_TOMBSTONES;
        int aCelWidth = aImage->GetCelWidth();
        int aCelCol = 2;
        int aGridX = mBoard->PixelToGridXKeepOnBoard(int(mCursorPositionX), int(mCursorPositionY));
        int aGridY = mBoard->PixelToGridYKeepOnBoard(int(mCursorPositionX), int(mCursorPositionY));
        GridItem *aMound = mBoard->GetMoundAt(aGridX, aGridY);
        if (aMound) {
            if (aMound->mMoundLevel == 0) {
                aCelCol = 0;
            } else if (aMound->mMoundLevel == 1) {
                aCelCol = 3;
            } else if (aMound->mMoundLevel == 2) {
                aCelCol = 4;
            } else if (aMound->mMoundLevel == 3) {
                aCelCol = 1;
            }
        }
        Rect aRect = Rect(aCelWidth * aCelCol, 0, aCelWidth, aImage->GetCelHeight());
        g->DrawImage(aImage, 0, 0, aRect);

        g->SetColorizeImages(false);

        UpdatePreviewReanim();
        return;
    }

    old_GamepadControls_DrawPreview(this, g);
}

void GamepadControls::OnButtonDown(Sexy::GamepadButton theButton, int thePlayerIndex, unsigned int unk) {
    SeedBank *aSeedBank = GetSeedBank();
    SeedPacket *aSeedPacket = &aSeedBank->mSeedPackets[mSelectedSeedIndex];
    SeedType aPacketType = aSeedPacket->mPacketType;
    int aCost = mBoard->GetCurrentPlantCost(aSeedPacket->mPacketType, SeedType::SEED_NONE);
    if (mGameObject->mApp->IsVSMode() && theButton == Sexy::GamepadButton::GAMEPAD_BUTTON_A) {
        if (mIsZombie) {
            int aGridX = mBoard->PixelToGridXKeepOnBoard((int)mCursorPositionX, (int)mCursorPositionY);
            int aGridY = mBoard->PixelToGridYKeepOnBoard((int)mCursorPositionX, (int)mCursorPositionY);

            if (!mBoard->CanTakeDeathMoney(aCost) || !aSeedPacket->CanPickUp() || mBoard->CanPlantAt(aGridX, aGridY, aPacketType) || mBoard->HasLevelAwardDropped())
                return;

            if (aPacketType == SEED_ZOMBIE_BEGHOULED_BUTTON_SHUFFLE) {
                std::vector<SeedType> aPlantSeeds, aZombieSeeds;
                PickShuffleSeeds(mGameObject->mApp, aPlantSeeds, aZombieSeeds, true);
                if (!aZombieSeeds.empty()) {
                    for (int aPacketIndex = 1; aPacketIndex <= aZombieSeeds.size(); ++aPacketIndex) {
                        SeedType aSeedType = aZombieSeeds[aPacketIndex - 1];
                        aSeedBank->mSeedPackets[aPacketIndex].SetPacketType(aSeedType, SeedType::SEED_NONE);
                    }
                }
                mBoard->TakeDeathMoney(aCost);
                aSeedPacket->Deactivate();
                aSeedPacket->WasPlanted(mPlayerIndex2);
                return;
            }

            if (mBoard->HasLevelAwardDropped() || (mBoard->mChallenge->IsMPSuddenDeath() && Challenge::gVSSuddenDeathMode <= 1 && Challenge::IsMPResourceProducer(aSeedPacket->mPacketType))
                || mBoard->mChallenge->ISMPSeedSuddenDeathDisabled(aSeedBank->mIsZombie, aPacketType)) {
                mGameObject->mApp->PlaySample(Sexy::SOUND_BUZZER);
                return;
            }

            if (aPacketType == SeedType::SEED_ZOMBIE_MOUND) {
                GridItem *aGraveStone = mBoard->GetGraveStoneAt(aGridX, aGridY);
                GridItem *aMound = mBoard->GetMoundAt(aGridX, aGridY);

                int aTargetLevel = -1;
                GridItem *aTargetGridItem = nullptr;

                if (aGraveStone) {
                    aTargetLevel = 0;
                    aTargetGridItem = aGraveStone;
                } else if (aMound && aMound->mMoundLevel < 4) {
                    aTargetLevel = aMound->mMoundLevel + 1;
                    aTargetGridItem = aMound;
                }

                if (aTargetLevel >= 0 && mBoard->TakeDeathMoney(aCost)) {
                    mBoard->AddAMound(aGridX, aGridY, aTargetLevel);

                    GridItem *aUpgradeMound = mBoard->GetMoundAt(aGridX, aGridY);
                    if (aUpgradeMound) {
                        aUpgradeMound->mVSGraveStoneHealth = 350 + 70 * aTargetLevel;
                    }

                    if (aTargetGridItem) {
                        aTargetGridItem->GridItemDie();
                    }

                    aSeedPacket->Deactivate();
                    aSeedPacket->WasPlanted(mPlayerIndex1);
                }
            }

            if (aPacketType == SeedType::SEED_ZOMBIE_GRAVESTONE) {
                if (mBoard->CanAddGraveStoneAt(aGridX, aGridY) && mBoard->TakeDeathMoney(aCost)) {
                    GridItem *aGraveStone = mBoard->AddAGraveStone(aGridX, aGridY);
                    aGraveStone->mIsSpecialGrave = false;
                    aGraveStone->mVSGraveStoneHealth = 350;
                    aSeedPacket->Deactivate();
                    aSeedPacket->WasPlanted(mPlayerIndex1);
                }
            }

            ZombieType aZombieType = Challenge::IZombieSeedTypeToZombieType(aPacketType);
            if (aZombieType != ZombieType::ZOMBIE_INVALID && mBoard->TakeDeathMoney(aCost)) {
                if (aZombieType == ZombieType::ZOMBIE_BUNGEE) {
                    Zombie *aBungeeZombie = mBoard->AddZombieInRow(aZombieType, aGridY, 0, false);
                    aBungeeZombie->mTargetCol = aGridX;
                    aBungeeZombie->SetRow(aGridY);
                    aBungeeZombie->mPosX = float(mBoard->GridToPixelX(aGridX, aGridY));
                    aBungeeZombie->mPosY = aBungeeZombie->GetPosYBasedOnRow(aGridY);
                    aBungeeZombie->mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_GRAVE_STONE, aGridY, 7);
                } else if (aZombieType == ZombieType::ZOMBIE_FLAG) {
                    mBoard->DisplayAdviceAgain("[ADVICE_HUGE_WAVE]", MessageStyle::MESSAGE_STYLE_HUGE_WAVE, AdviceType::ADVICE_HUGE_WAVE);
                    mBoard->SpawnZombieWave();
                } else if (Challenge::IsMPZombieTypeAddInRow(aZombieType)) {
                    mBoard->AddZombieInRow(aZombieType, aGridY, Zombie::ZOMBIE_WAVE_VS, true);
                } else {
                    if (mBoard->mPlantRow[aGridY] == PlantRowType::PLANTROW_POOL) { // 如果是水路则放置在出生点
                        mBoard->AddZombieInRow(aZombieType, aGridY, Zombie::ZOMBIE_WAVE_VS, true);
                    } else {
                        Zombie *aZombie = mBoard->AddZombie(aZombieType, Zombie::ZOMBIE_WAVE_VS, false);
                        if (aZombie) {
                            if (mBoard->StageHasRoof()) {
                                Zombie *aBungeeZombie = mBoard->AddZombie(ZombieType::ZOMBIE_BUNGEE, Zombie::ZOMBIE_WAVE_VS, false);
                                aBungeeZombie->BungeeDropZombie(aZombie, aGridX, aGridY);
                            } else {
                                aZombie->RiseFromGrave(aGridX, aGridY);
                            }
                        }
                    }
                }

                aSeedPacket->Deactivate();
                aSeedPacket->WasPlanted(mPlayerIndex2);
                return;
            }
        } else {
            int aGridX = mBoard->PixelToGridXKeepOnBoard((int)mCursorPositionX, (int)mCursorPositionY);
            int aGridY = mBoard->PixelToGridYKeepOnBoard((int)mCursorPositionX, (int)mCursorPositionY);

            if (!mBoard->CanTakeSunMoney(aCost, 0) || !aSeedPacket->CanPickUp() || mBoard->CanPlantAt(aGridX, aGridY, aPacketType) || mBoard->HasLevelAwardDropped())
                return;

            if (aPacketType < SeedType::NUM_SEED_TYPES) {
                old_GamepadControls_OnButtonDown(this, theButton, thePlayerIndex, unk);
                return;
            }

            if (aPacketType == SEED_BEGHOULED_BUTTON_SHUFFLE) {
                std::vector<SeedType> aPlantSeeds, aZombieSeeds;
                PickShuffleSeeds(mGameObject->mApp, aPlantSeeds, aZombieSeeds, false);
                if (!aPlantSeeds.empty()) {
                    for (int aPacketIndex = 1; aPacketIndex <= aPlantSeeds.size(); ++aPacketIndex) {
                        SeedType aSeedType = aPlantSeeds[aPacketIndex - 1];
                        aSeedBank->mSeedPackets[aPacketIndex].SetPacketType(aSeedType, SeedType::SEED_NONE);
                    }
                }
                mBoard->TakeSunMoney(aCost, 0);
                aSeedPacket->Deactivate();
                aSeedPacket->WasPlanted(mPlayerIndex1);
            }
        }
    }

    old_GamepadControls_OnButtonDown(this, theButton, thePlayerIndex, unk);
}

void ZenGardenControls::Update(float a2) {
    old_ZenGardenControls_Update(this, a2);
}
