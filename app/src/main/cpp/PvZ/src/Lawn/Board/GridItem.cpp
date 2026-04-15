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

#include "PvZ/Lawn/Board/GridItem.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Board/Challenge.h"
#include "PvZ/Lawn/Board/CursorObject.h"
#include "PvZ/Lawn/Board/SeedPacket.h"
#include "PvZ/Lawn/Board/ZenGarden.h"
#include "PvZ/Lawn/GamepadControls.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/System/ReanimationLawn.h"
#include "PvZ/NetPlay.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Effect/Reanimator.h"
#include "PvZ/TodLib/Effect/TodParticle.h"

#include <numbers>

using namespace Sexy;

void GridItem::_constructor() {
    old_GridItem_GridItem(this);
}

void GridItem::GridItemDie() {
    if (mApp->IsVSMode() && mApp->mGameScene == SCENE_PLAYING) {
        if (gTcpConnected)
            return;

        if (gTcpClientSocket >= 0) {
            U16_Event event = {{EventType::EVENT_SERVER_BOARD_GRIDITEM_DIE}, uint16_t(mBoard->mGridItems.DataArrayGetID(this))};
            netplay::PutEvent(event);
        }
    }

    mDead = true;

    struct CleanupHelper {
        GridItem *thiz;
        ~CleanupHelper() {
            Reanimation *aGridItemReanim = thiz->mApp->ReanimationTryToGet(thiz->mGridItemReanimID);
            if (aGridItemReanim) {
                aGridItemReanim->ReanimationDie();
                thiz->mGridItemReanimID = ReanimationID::REANIMATIONID_NULL;
            }

            TodParticleSystem *aGridItemParticle = thiz->mApp->ParticleTryToGet(thiz->mGridItemParticleID);
            if (aGridItemParticle) {
                aGridItemParticle->ParticleSystemDie();
                thiz->mGridItemParticleID = ParticleSystemID::PARTICLESYSTEMID_NULL;
            }
        }
    } cleanupHelper{this};

    if (mApp->mGameMode != GameMode::GAMEMODE_MP_VS) {
        return;
    }

    if (mGridItemType == GridItemType::GRIDITEM_GRAVESTONE) {
        if ((Challenge::gVSResourseDropMode == 2 || Challenge::gVSResourseDropMode == 3) && Challenge::gVSResourceDropCount > 0) {
            for (int i = 0; i < Challenge::gVSResourceDropCount; ++i) {
                int x = mBoard->GridToPixelX(mGridX, mGridY);
                int y = mBoard->GridToPixelY(mGridX, mGridY);
                mBoard->AddCoin(x, y, CoinType::COIN_VS_ZOMBIE_BRAIN, CoinMotion::COIN_MOTION_FROM_GRAVE_STONE);
            }
        }
        return;
    }

    int targetsRemain = mBoard->GetMPTargetCount();
    if (mApp->mGameScene == GameScenes::SCENE_PLAYING && !mBoard->mLevelAwardSpawned) {
        if (Challenge::gVSWinMode == 1) {
            if (targetsRemain <= (mBoard->StageHas6Rows() ? 3 : 2)) {
                mBoard->FreezeEffectsForCutscene(true);
                mBoard->PlantsWon(this);
                mGridItemReanimID = ReanimationID::REANIMATIONID_NULL;
                return;
            }
        }

        if (targetsRemain == 0) {
            mBoard->FreezeEffectsForCutscene(true);
            mBoard->PlantsWon(this);
            mGridItemReanimID = ReanimationID::REANIMATIONID_NULL;
            return;
        }
    }

    if (mGridItemType == GridItemType::GRIDITEM_MP_TARGET_ZOMBIE) {
        bool shouldDrop = (Challenge::gVSResourseDropMode == 1 || Challenge::gVSResourseDropMode == 3);
        if (shouldDrop && Challenge::gVSResourceDropCount > 0) {
            for (int i = 0; i < Challenge::gVSResourceDropCount; ++i) {
                int x = mBoard->GridToPixelX(mGridX, mGridY);
                int y = mBoard->GridToPixelY(mGridX, mGridY);
                mBoard->AddCoin(x, y, CoinType::COIN_VS_ZOMBIE_BRAIN, CoinMotion::COIN_MOTION_FROM_GRAVE_STONE);
            }
        }
        return;
    }
}

void GridItem::DrawGridItem(Graphics *g) {
    if (mGridItemType == GridItemType::GRIDITEM_MP_BURIAL_MOUND) {
        DrawGraveStone(g);
        // 召唤墓碑暂无受损动画，故用显血来表示
        int x = mBoard->GridToPixelX(mGridX, mGridY);
        int y = mBoard->GridToPixelY(mGridX, mGridY);
        Graphics aHealthG = Graphics(*g);
        pvzstl::string str = StrFormat("%d/%d", mVSGraveStoneHealth, 350);
        aHealthG.SetFont(*Sexy_FONT_DWARVENTODCRAFT18_Addr);
        aHealthG.SetColor(Color::White);
        aHealthG.DrawString(str, x, y + 34);
        aHealthG.SetFont(nullptr);
    }

    old_GridItem_DrawGridItem(this, g);

    //    Reanimation *aGridItemReanim = mApp->ReanimationTryToGet(mGridItemReanimID);
    //
    //    switch (mGridItemType) {
    //        case GridItemType::GRIDITEM_GRAVESTONE:
    //            DrawGraveStone(g);
    //            if (aGridItemReanim) {
    //                aGridItemReanim->Draw(g);
    //            }
    //            return;
    //        case GridItemType::GRIDITEM_CRATER:
    //            DrawCrater(g);
    //            break;
    //        case GridItemType::GRIDITEM_LADDER:
    //            DrawLadder(g);
    //            break;
    //        case GridItemType::GRIDITEM_BRAIN:
    //            g->DrawImageF(*IMAGE_BRAIN, mPosX, mPosY);
    //            break;
    //        case GridItemType::GRIDITEM_SCARY_POT:
    //            DrawScaryPot(g);
    //            break;
    //        case GridItemType::GRIDITEM_SQUIRREL:
    //            DrawSquirrel(g);
    //            break;
    //        case GridItemType::GRIDITEM_STINKY:
    //            DrawStinky(g);
    //            break;
    //        case GridItemType::GRIDITEM_IZOMBIE_BRAIN:
    //            DrawIZombieBrain(g);
    //            break;
    //        case GridItemType::GRIDITEM_MP_TARGET_ZOMBIE:
    //            DrawMPTarget(g);
    //            break;
    //        default:
    //            break;
    //    }
    //
    //    if (aGridItemReanim) {
    //        aGridItemReanim->Draw(g);
    //    }
    //
    //    TodParticleSystem *aGridItemParticle = mApp->ParticleTryToGet(mGridItemParticleID);
    //    if (aGridItemParticle) {
    //        aGridItemParticle->Draw(g);
    //    }
}

void GridItem::DrawScaryPot(Sexy::Graphics *g) {
    // 修复路灯花照透罐子

    int aImageCol = mGridItemState - GridItemState::GRIDITEM_STATE_SCARY_POT_QUESTION;

    int aXPos = mBoard->GridToPixelX(mGridX, mGridY) - 5;
    int aYPos = mBoard->GridToPixelY(mGridX, mGridY) - 15;
    TodDrawImageCelCenterScaledF(g, *Sexy_IMAGE_PLANTSHADOW2_Addr, aXPos - 5.0, aYPos + 72.0, 0, 1.3, 1.3);

    if (mTransparentCounter > 0) { // 如果罐子要被照透(透明度不为0)
        g->DrawImageCel(*Sexy_IMAGE_SCARY_POT_Addr, aXPos, aYPos, aImageCol, 0);

        Graphics *aInsideGraphics = new Graphics(*g);
        if (mScaryPotType == ScaryPotType::SCARYPOT_SEED) {
            aInsideGraphics->mScaleX = 0.7f;
            aInsideGraphics->mScaleY = 0.7f;
            DrawSeedPacket(aInsideGraphics, aXPos + 23, aYPos + 33, mSeedType, SeedType::SEED_NONE, 0.0, 255, false, false, false, true);
        } else if (mScaryPotType == ScaryPotType::SCARYPOT_ZOMBIE) {
            aInsideGraphics->mScaleX = 0.4f;
            aInsideGraphics->mScaleY = 0.4f;
            float theOffsetX = 6.0;
            float theOffsetY = 19.0;
            if (mZombieType == ZombieType::ZOMBIE_FOOTBALL) {
                theOffsetX = 1.0;
                theOffsetY = 16.0;
            } else if (mZombieType == ZombieType::ZOMBIE_GARGANTUAR) {
                theOffsetX = 15.0;
                theOffsetY = 26.0;
                aInsideGraphics->mScaleX = 0.3f;
                aInsideGraphics->mScaleY = 0.3f;
            }
            mApp->mReanimatorCache->DrawCachedZombie(aInsideGraphics, theOffsetX + aXPos, theOffsetY + aYPos, mZombieType);
        } else if (mScaryPotType == ScaryPotType::SCARYPOT_SUN) {
            int aSuns = mBoard->mChallenge->ScaryPotterCountSunInPot(this);

            Reanimation aReanim{};
            aReanim.ReanimationInitializeType(0.0, 0.0, ReanimationType::REANIM_SUN);
            aReanim.OverrideScale(0.5f, 0.5f);
            aReanim.Update();                                                              // 一次Update是必要的，否则绘制出来是Empty
            aReanim.mFrameStart = (mBoard->mMainCounter / 10) % (aReanim.mFrameCount - 1); // 这行代码可让阳光动起来

            for (int i = 0; i < aSuns; i++) {
                float aOffsetX = 42.0f;
                float aOffsetY = 62.0f;
                switch (i) {
                    case 1:
                        aOffsetX += 3.0f;
                        aOffsetY -= 20.0f;
                        break;
                    case 2:
                        aOffsetX -= 6.0f;
                        aOffsetY -= 10.0f;
                        break;
                    case 3:
                        aOffsetX += 6.0f;
                        aOffsetY -= 5.0f;
                        break;
                    case 4:
                        aOffsetX += 5.0f; // aOffsetY -= 15.0f;          break;
                }

                aReanim.SetPosition(aXPos + aOffsetX, aYPos + aOffsetY);
                aReanim.Draw(g);
            }
        }

        int aAlpha = TodAnimateCurve(0, 50, mTransparentCounter, 255, 58, TodCurves::CURVE_LINEAR);
        g->SetColorizeImages(true);
        Color aColor = {255, 255, 255, aAlpha};
        g->SetColor(aColor);
        delete aInsideGraphics;
    }

    g->DrawImageCel(*Sexy_IMAGE_SCARY_POT_Addr, aXPos, aYPos, aImageCol, 1);
    if (mHighlighted) {
        g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
        g->SetColorizeImages(true);
        if (mTransparentCounter == 0) {
            Color aColor = {255, 255, 255, 196};
            g->SetColor(aColor);
        }
        g->DrawImageCel(*Sexy_IMAGE_SCARY_POT_Addr, aXPos, aYPos, aImageCol, 1);
        g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
    }

    return g->SetColorizeImages(false);
}

void GridItem::Update() {
    if (requestPause) {
        return; // 高级暂停
    }

    //    Reanimation *aGridItemReanim = mApp->ReanimationTryToGet(mGridItemReanimID);
    //    if (aGridItemReanim) {
    //        aGridItemReanim->Update();
    //    }
    //
    //    TodParticleSystem *aGridItemParticle = mApp->ParticleTryToGet(mGridItemParticleID);
    //    if (aGridItemParticle) {
    //        aGridItemParticle->Update();
    //    }
    //
    //    if (mGridItemType == GridItemType::GRIDITEM_PORTAL_CIRCLE || mGridItemType == GridItemType::GRIDITEM_PORTAL_SQUARE) {
    //        UpdatePortal();
    //    }
    //    if (mGridItemType == GridItemType::GRIDITEM_SCARY_POT) {
    //        UpdateScaryPot();
    //    }
    //    if (mGridItemType == GridItemType::GRIDITEM_RAKE) {
    //        UpdateRake();
    //    }
    //    if (mGridItemType == GridItemType::GRIDITEM_IZOMBIE_BRAIN) {
    //        UpdateBrain();
    //    }
    if (mGridItemType == GridItemType::GRIDITEM_MP_BURIAL_MOUND) {
        UpdateBurialMound();
    }

    if ((mGridItemType == GridItemType::GRIDITEM_GRAVESTONE || mGridItemType == GridItemType::GRIDITEM_MP_BURIAL_MOUND) && mApp->IsVSMode() && mApp->mGameScene == SCENE_PLAYING) {
        Reanimation *aGridItemReanim = mApp->ReanimationTryToGet(mGridItemReanimID);
        if (aGridItemReanim) {
            aGridItemReanim->Update();
        }

        TodParticleSystem *aGridItemParticle = mApp->ParticleTryToGet(mGridItemParticleID);
        if (aGridItemParticle) {
            aGridItemParticle->Update();
        }

        if (mGraveJustGotShotCounter > 0) {
            mGraveJustGotShotCounter--;
        }

        mLaunchCounter--;

        if (mLaunchCounter <= 100) {
            int aFlashCountdown = TodAnimateCurve(100, 0, mLaunchCounter, 0, 100, TodCurves::CURVE_LINEAR);
            mGraveJustGotShotCounter = std::max(mGraveJustGotShotCounter, aFlashCountdown);
        }

        if (aGridItemReanim) {
            if (mGraveJustGotShotCounter <= 0) {
                aGridItemReanim->mEnableExtraAdditiveDraw = false;
            } else {
                int aGrayness = std::min(mGraveJustGotShotCounter * 3, 255);
                int aGrayness2 = (aGrayness == 255) ? 127 : (aGrayness / 2);
                aGridItemReanim->mExtraAdditiveColor = Color(aGrayness, aGrayness2, aGrayness, 255);
                aGridItemReanim->mEnableExtraAdditiveDraw = true;
            }
        }

        if (mLaunchCounter <= 0) { // 生产
            if (gTcpConnected) {
                return;
            }
            mLaunchCounter = RandRangeInt(mLaunchRate - 150, mLaunchRate);
            if (gTcpClientSocket >= 0) {
                U16U16_Event event = {{EventType::EVENT_SERVER_BOARD_GRIDITEM_LAUNCHCOUNTER}, uint16_t(mBoard->mGridItems.DataArrayGetID(this)), uint16_t(mLaunchCounter)};
                netplay::PutEvent(event);
            }
            mBoard->AddCoin(mBoard->GridToPixelX(mGridX, mGridY), mBoard->GridToPixelY(mGridX, mGridY), CoinType::COIN_VS_ZOMBIE_BRAIN, CoinMotion::COIN_MOTION_FROM_GRAVE_STONE);
        }

        // 屋顶墓碑落地时播放砸地音效
        if (mBoard->StageHasRoof() && mGridItemCounter == 50) {
            mApp->PlayFoley(FoleyType::FOLEY_THUMP);
        }

        return;
    }

    old_GridItem_Update(this);

    //    if (mGridItemType == GridItemType::GRIDITEM_MP_TARGET_ZOMBIE) {
    //        if (mTargetJustGotShotCounter <= 0) {
    //            // 受击结束，尝试播放 idle 动画
    //            if (aGridItemReanim && !aGridItemReanim->IsAnimPlaying("anim_idle")) {
    //                // 如果正在播放受击动画，确保它播放完毕(到最后一帧)才切换
    //                if (!aGridItemReanim->IsAnimPlaying("anim_hit") || aGridItemReanim->mAnimTime == 1.0f) {
    //                    aGridItemReanim->PlayReanim("anim_idle", REANIM_PLAY_ONCE, 0.0f, 24.0f);
    //                }
    //            }
    //        } else {
    //            mTargetJustGotShotCounter--;
    //            if (mTargetJustGotShotCounter == 0) {
    //                GridItemDie();
    //            }
    //        }
    //    }
}

void GridItem::UpdateScaryPot() {
    old_GridItem_UpdateScaryPot(this);

    if (transparentVase) { // 如果玩家开启“罐子透视”
        if (mTransparentCounter < 50) {
            // 透明度如果小于50，则为透明度加2
            mTransparentCounter += 2;
        }
    }
}

void GridItem::UpdateBurialMound() {
    if (mApp->mGameScene != SCENE_PLAYING)
        return;

    ++mGridItemCounter;

    if (mSummonCounter > 0) {
        --mSummonCounter;

        if (gTcpConnected) {
            return;
        }

        if (mSummonCounter <= 0 && mSummonIndex >= 0) {
            for (int aSummonIndex = 0; aSummonIndex <= mSummonIndex; ++aSummonIndex) {
                ZombieType aZombieType = mBoard->PickGraveRisingZombieTypeMP(mMoundLevel);
                Zombie *aZombie = mBoard->AddZombie(aZombieType, Zombie::ZOMBIE_WAVE_VS, false);

                if (aZombie) {
                    int aGridX = mGridX;
                    int aGridY = mGridY;

                    // 调整Y坐标
                    if (aSummonIndex == 1) {
                        --aGridY;
                    } else if (aSummonIndex == 2) {
                        ++aGridY;
                    }

                    // 限制Y坐标范围
                    if (aGridY > 0) {
                        if (aGridY >= 4) {
                            aGridY = 4;
                        }
                    } else {
                        aGridY = 0;
                    }

                    // 限制X坐标范围
                    if (aGridX > 0) {
                        if (aGridX >= 8) {
                            aGridX = 8;
                        }
                    } else {
                        aGridX = 0;
                    }

                    aZombie->RiseFromGrave(aGridX, aGridY);
                }
            }

            mSummonCounter = RandRangeInt(mLaunchRate - 150, mLaunchRate);

            if (gTcpClientSocket >= 0) {
                U16U16_Event event = {{EventType::EVENT_SERVER_BOARD_GRIDITEM_LAUNCHCOUNTER}, uint16_t(mBoard->mGridItems.DataArrayGetID(this)), uint16_t(mLaunchCounter)};
                netplay::PutEvent(event);
            }
        }
    }
}

int GridItem::GetMoundUpgradeCost() {
    int aCost = Plant::GetCost(SeedType::SEED_ZOMBIE_MOUND, SeedType::SEED_NONE);
    return mMoundLevel < 4 ? aCost + 50 * (mMoundLevel + 1) : aCost;
}

void GridItem::DrawStinky(Sexy::Graphics *g) {
    // 在玩家选取巧克力时，高亮显示光标下方且没喂巧克力的Stinky。
    // 从而修复Stinky无法在醒着时喂巧克力、修复Stinky在喂过巧克力后还能继续喂巧克力。
    // 因为游戏通过Stinky是否高亮来判断是否能喂Stinky。这个机制是为鼠标操作而生，但渡维不加改动地将其用于按键操作，导致无法在Stinky醒着时喂它。
    GamepadControls *aGamePad = mBoard->mGamepadControls1;
    int aCursorX = aGamePad->mCursorPositionX;
    int aCursorY = aGamePad->mCursorPositionY;
    int aCursorGridX = mBoard->PixelToGridX(aCursorX, aCursorY);
    int aCursorGridY = mBoard->PixelToGridY(aCursorX, aCursorY);
    int aStinkyGridX = mBoard->PixelToGridX(mPosX, mPosY);
    int aStinkyGridY = mBoard->PixelToGridY(mPosX, mPosY);
    if (aStinkyGridX != aCursorGridX || aStinkyGridY != aCursorGridY) {
        // 如果Stinky不在光标位置处，则取消高亮。
        mHighlighted = false;
        old_GridItem_DrawStinky(this, g);
        return;
    }
    // 如果Stinky在光标位置处
    CursorObject *aCursorObject = mBoard->mCursorObject1;
    CursorType aCursorType = aCursorObject->mCursorType;
    if (aCursorType == CursorType::CURSOR_TYPE_CHOCOLATE) {
        // 如果光标类型为巧克力
        bool isStinkyHighOnChocolate = mApp->mZenGarden->IsStinkyHighOnChocolate();
        mHighlighted = !isStinkyHighOnChocolate; // 为没喂巧克力的Stinky加入高亮效果
    }

    old_GridItem_DrawStinky(this, g);
}

void GridItem::DrawSquirrel(Sexy::Graphics *g) {
    // 绘制松鼠
    float aXPos = mBoard->GridToPixelX(mGridX, mGridY);
    float aYPos = mBoard->GridToPixelY(mGridX, mGridY);
    switch (mGridItemState) {
        case GridItemState::GRIDITEM_STATE_SQUIRREL_PEEKING:
            aYPos += TodAnimateCurve(50, 0, mGridItemCounter, 0, -40.0f, TodCurves::CURVE_BOUNCE_SLOW_MIDDLE);
            break;
        case GridItemState::GRIDITEM_STATE_SQUIRREL_RUNNING_UP:
            aYPos += TodAnimateCurve(50, 0, mGridItemCounter, 100, 0.0f, TodCurves::CURVE_EASE_IN);
            break;
        case GridItemState::GRIDITEM_STATE_SQUIRREL_RUNNING_DOWN:
            aYPos += TodAnimateCurve(50, 0, mGridItemCounter, -100, 0.0f, TodCurves::CURVE_EASE_IN);
            break;
        case GridItemState::GRIDITEM_STATE_SQUIRREL_RUNNING_LEFT:
            aXPos += TodAnimateCurve(50, 0, mGridItemCounter, 80, 0.0f, TodCurves::CURVE_EASE_IN);
            break;
        case GridItemState::GRIDITEM_STATE_SQUIRREL_RUNNING_RIGHT:
            aXPos += TodAnimateCurve(50, 0, mGridItemCounter, -80, 0.0f, TodCurves::CURVE_EASE_IN);
            break;
        default:
            break;
    }

    g->DrawImage(addonImages.squirrel, aXPos, aYPos);
}

void GridItem::DrawCrater(Sexy::Graphics *g) {
    // 绘制屋顶月夜弹坑
    float aXPos = mBoard->GridToPixelX(mGridX, mGridY) - 8.0f;
    float aYPos = mBoard->GridToPixelY(mGridX, mGridY) + 40.0f;
    if (mGridItemCounter < 25) {
        int anAlpha = TodAnimateCurve(25, 0, mGridItemCounter, 255, 0, TodCurves::CURVE_LINEAR);
        g->SetColor(Color(255, 255, 255, anAlpha));
        g->SetColorizeImages(true);
    }

    bool fading = mGridItemCounter < 9000;
    Sexy::Image *aImage = *Sexy_IMAGE_CRATER_Addr;
    int aCelCol = 0;

    if (mBoard->IsPoolSquare(mGridX, mGridY)) {
        if (mBoard->StageIsNight()) {
            aImage = *Sexy_IMAGE_CRATER_WATER_NIGHT_Addr;
        } else {
            aImage = *Sexy_IMAGE_CRATER_WATER_DAY_Addr;
        }

        if (fading) {
            aCelCol = 1;
        }

        float aPos = mGridY * std::numbers::pi + mGridX * std::numbers::pi * 0.25f;
        float aTime = mBoard->mMainCounter * std::numbers::pi * 2.0f / 200.0f;
        aYPos += sin(aPos + aTime) * 2.0f;
    } else if (mBoard->StageHasRoof()) {
        if (mGridX < 5) {
            if (mBoard->StageIsNight()) {
                aImage = addonImages.crater_night_roof_left;
            } else {
                aImage = *Sexy_IMAGE_CRATER_ROOF_LEFT_Addr;
            }
            aXPos += 16.0f;
            aYPos += -16.0f;
        } else {
            if (mBoard->StageIsNight()) {
                aImage = addonImages.crater_night_roof_center;
            } else {
                aImage = *Sexy_IMAGE_CRATER_ROOF_CENTER_Addr;
            }
            aXPos += 18.0f;
            aYPos += -9.0f;
        }

        if (fading) {
            aCelCol = 1;
        }
    } else if (mBoard->StageIsNight()) {
        aCelCol = 1;
        if (fading) {
            aImage = *Sexy_IMAGE_CRATER_FADING_Addr;
        }
    } else if (fading) {
        aImage = *Sexy_IMAGE_CRATER_FADING_Addr;
    }

    TodDrawImageCelF(g, aImage, aXPos, aYPos, aCelCol, 0);
    g->SetColorizeImages(false);
}

void GridItem::DrawGraveStone(Graphics *g) {
    if (mGridItemCounter <= 0)
        return;

    int aHeightPosition = TodAnimateCurve(0, 100, mGridItemCounter, 1000, 0, TodCurves::CURVE_EASE_IN_OUT);
    int aGridCelLook = mBoard->mGridCelLook[mGridX][mGridY];
    int aGridCelOffsetX = mBoard->mGridCelOffset[mGridX][mGridY][0];
    int aGridCelOffsetY = mBoard->mGridCelOffset[mGridX][mGridY][1];
    int aCelWidth = (*IMAGE_TOMBSTONES)->GetCelWidth();
    int aCelHeight = (*IMAGE_TOMBSTONES)->GetCelHeight();
    int aGraveCol = aGridCelLook % 5;
    int aGraveRow;
    if (mGridY == 0) {
        aGraveRow = 1; // 第一列固定用低矮墓碑贴图
    } else if (mGridItemState == GridItemState::GRIDITEM_STATE_GRAVESTONE_SPECIAL) {
        aGraveRow = 0;
    } else {
        aGraveRow = 2 + aGridCelLook % 2;
    }

    if (mGridItemType == GridItemType::GRIDITEM_MP_BURIAL_MOUND) {
        switch (mMoundLevel) {
            case 0:
                aGraveCol = 2;
                break;
            case 1:
                aGraveCol = 0;
                break;
            case 2:
                aGraveCol = 3;
                break;
            case 3:
                aGraveCol = 4;
                break;
            case 4:
                aGraveCol = 1;
                break;
            default:
                break;
        }
    }

    int aVisibleHeight = TodAnimateCurve(0, 1000, aHeightPosition, aCelHeight, 0, TodCurves::CURVE_EASE_IN_OUT);       // 墓碑主体在当前帧应显示的高度
    int aExtraBottomClip = TodAnimateCurve(0, 50, aHeightPosition, 0, 14, TodCurves::CURVE_EASE_IN_OUT);               // 为模拟墓碑从地面“长出”的效果，从底部额外裁剪的高度
    int aVisibleHeightDirt = TodAnimateCurve(500, 1000, aHeightPosition, aCelHeight, 0, TodCurves::CURVE_EASE_IN_OUT); // 墓碑下方泥土部分应显示的高度
    int aExtraTopClip = 0; // 当墓碑被“咬咬碑”（GraveBuster）啃食时，从顶部额外裁剪的高度，以表现被吃掉的效果
    Plant *aPlant = mBoard->GetTopPlantAt(mGridX, mGridY, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
    if (aPlant && aPlant->mState == PlantState::STATE_GRAVEBUSTER_EATING) {
        aExtraTopClip = TodAnimateCurveFloat(400, 0, aPlant->mStateCountdown, 10.0f, 40.0f, TodCurves::CURVE_LINEAR);
    }

    Rect aSrcRect(aCelWidth * aGraveCol, aCelHeight * aGraveRow + aExtraTopClip, aCelWidth, aVisibleHeight - aExtraBottomClip - aExtraTopClip);
    Rect aSrcRectDirt(aCelWidth * aGraveCol, aCelHeight * aGraveRow, aCelWidth, aVisibleHeightDirt);
    int x = mBoard->GridToPixelX(mGridX, mGridY) + aGridCelOffsetX - 4;
    int y = mBoard->GridToPixelY(mGridX, mGridY) + aCelHeight + aGridCelOffsetY - 9;

    if (mBoard->StageHasRoof()) {
        aHeightPosition = TodAnimateCurve(
            0, 100, mGridItemCounter, 0, 1000, TodCurves::CURVE_EASE_IN_OUT); // 修改动画曲线：将“从土里长出来”的进度（1000->0对应从下到上）改为“从天上坠落”的进度（0->1000对应从上到下）
        aVisibleHeight = TodAnimateCurve(0, 1000, aHeightPosition, 0, aCelHeight, TodCurves::CURVE_EASE_IN_OUT);                   // 调整泥土显示逻辑：泥土应随着墓碑下落而逐渐显示（从下往上）
        aExtraBottomClip = 0;                                                                                                      // 移除底部裁剪（因为是从上往下落，不需要模拟从土里长出的底部裁剪）
        aSrcRect = Rect(aCelWidth * aGraveCol, aCelHeight * aGraveRow + aExtraTopClip, aCelWidth, aVisibleHeight - aExtraTopClip); // 修改源矩形：由于是坠落动画，应从顶部开始显示逐渐增加的高度
        aSrcRectDirt = Rect(aCelWidth * aGraveCol, aCelHeight * aGraveRow + (aCelHeight - aVisibleHeightDirt), aCelWidth, aVisibleHeightDirt); // 修改泥土源矩形：泥土应从底部开始向上显示
        int startYOffset = -200;                                                                                                               // 起始位置在屏幕上方200像素
        int endYOffset = aCelHeight + aGridCelOffsetY - 9;                                                                                     // 最终位置
        int currentYOffset = TodAnimateCurve(0, 1000, aHeightPosition, startYOffset, endYOffset, TodCurves::CURVE_EASE_IN_OUT);
        y = mBoard->GridToPixelY(mGridX, mGridY) + currentYOffset;
    }

    if (mApp->IsVSMode() && mGridItemType == GridItemType::GRIDITEM_GRAVESTONE) {
        Reanimation *aReanim = mApp->ReanimationTryToGet(mGridItemReanimID);
        if (aReanim) {
            g->SetClipRect(x, y - aVisibleHeight + aExtraTopClip, 86, aVisibleHeight - aExtraBottomClip - aExtraTopClip);
            aReanim->SetPosition(x, y - aVisibleHeight + aExtraTopClip);
            aReanim->DrawRenderGroup(g, 0);
            g->ClearClipRect();

            bool isPlantRowPool = mBoard->mPlantRow[mGridY] == PlantRowType::PLANTROW_POOL;
            Image *bottomImage = isPlantRowPool ? addonImages.zombie_duckytube_inwater : *IMAGE_VS_STONE_DIRT; // 泳池绘制鸭子救生圈，草坪绘制泥土
            int offsetX = 0, offsetY = 0;
            if (isPlantRowPool) {
                offsetX = -20;
                offsetY = 40;
            }

            Rect aRectDirt(0, 0, bottomImage->mWidth, TodAnimateCurve(500, 1000, aHeightPosition, bottomImage->mHeight, 0, TodCurves::CURVE_EASE_IN_OUT));
            if (!mBoard->StageHasRoof()) { // 屋顶不绘制墓碑底部贴图
                g->DrawImage(bottomImage, x + offsetX, y - aVisibleHeightDirt + offsetY, aRectDirt);
            }
            aReanim->DrawRenderGroup(g, 1);
            g->mClipRect = Rect(g->mClipRect.mX, g->mClipRect.mY, g->mClipRect.mWidth, g->mClipRect.mHeight);
        }
        g->DrawString(StrFormat("%d", mVSGraveStoneHealth), x, y);
    } else {
        // Mound 受击和生产时变色
        if (mGridItemType == GridItemType::GRIDITEM_MP_BURIAL_MOUND) {
            Graphics aMoundG = Graphics(*g);
            if (mGraveJustGotShotCounter <= 0) {
                aMoundG.SetColorizeImages(false);
            } else {
                int aGrayness = std::min(mGraveJustGotShotCounter * 3, 255);
                int aGrayness2 = (aGrayness == 255) ? 127 : (aGrayness / 2);
                aMoundG.SetColor(Color(aGrayness, aGrayness2, aGrayness, 255));
                aMoundG.SetColorizeImages(true);
            }

            aMoundG.DrawImage(*IMAGE_TOMBSTONES, x, y - aVisibleHeight + aExtraTopClip, aSrcRect);
            aMoundG.DrawImage(*IMAGE_TOMBSTONE_MOUNDS, x, y - aVisibleHeightDirt, aSrcRectDirt);
            return;
        }

        g->DrawImage(*IMAGE_TOMBSTONES, x, y - aVisibleHeight + aExtraTopClip, aSrcRect);
        g->DrawImage(*IMAGE_TOMBSTONE_MOUNDS, x, y - aVisibleHeightDirt, aSrcRectDirt);
    }
}

void GridItem::AddGraveStoneParticles() {
    if (mBoard->StageHasRoof())
        return;

    int aXOffset = mBoard->mGridCelOffset[mGridX][mGridY][0];
    int aYOffset = mBoard->mGridCelOffset[mGridX][mGridY][1];
    int aXPos = mBoard->GridToPixelX(mGridX, mGridY) + 14 + aXOffset;
    int aYPos = mBoard->GridToPixelY(mGridX, mGridY) + 78 + aYOffset;
    if (mBoard->mPlantRow[mGridY] == PlantRowType::PLANTROW_POOL) { // 水路墓碑
        // 播放出水动画和音效
        float aX = mBoard->GridToPixelX(mGridX, mGridY);
        float aY = mBoard->GridToPixelY(mGridX, mGridY);
        float aOffsetY = 40.0f;

        mApp->AddReanimation(aX, aY + aOffsetY, mRenderOrder + 1, ReanimationType::REANIM_SPLASH);
        mApp->AddTodParticle(aX + 37.0f, aY + aOffsetY + 42.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_PLANTING_POOL);

        mApp->PlayFoley(FoleyType::FOLEY_PLANT_WATER);

        // 绘制附加的海草粒子特效
        Reanimation *aReanim = mApp->ReanimationTryToGet(mGridItemReanimID);
        TodParticleSystem *aParticle = mApp->AddTodParticle(0.0f, 0.0f, 0, ParticleEffect::PARTICLE_ZOMBIE_SEAWEED);
        if (aParticle) {
            aReanim->AttachParticleToTrack("Stone", aParticle, 30.0f, 60.0f);
        }
        TodParticleSystem *aParticle2 = mApp->AddTodParticle(0.0f, 0.0f, 0, ParticleEffect::PARTICLE_ZOMBIE_SEAWEED);
        if (aParticle2) {
            aReanim->AttachParticleToTrack("eye glow left", aParticle2, 5.0f, 5.0f);
        }
        TodParticleSystem *aParticle3 = mApp->AddTodParticle(0.0f, 0.0f, 0, ParticleEffect::PARTICLE_ZOMBIE_SEAWEED);
        if (aParticle3) {
            aReanim->AttachParticleToTrack("eye glow right", aParticle3, 77.0f, 20.0f);
        }
    } else {
        mApp->AddTodParticle(aXPos, aYPos, mRenderOrder + 1, ParticleEffect::PARTICLE_GRAVE_STONE_RISE);
        mApp->PlayFoley(FoleyType::FOLEY_DIRT_RISE);
    }
}

void GridItem::DrawMPTarget(Graphics *g) {
    //    Reanimation *reanim = mApp->ReanimationTryToGet(mGridItemReanimID);
    //    pvzstl::string fmt = StrFormat("%d %d %d", mGridItemReanimID, (int)reanim,mRenderOrder);
    //    g->SetFont(*Sexy_FONT_CONTINUUMBOLD14OUTLINE_Addr);
    //    g->DrawString(fmt,0,50 * mGridY);
    //    TodDrawString(g,fmt,0,0,*Sexy_FONT_CONTINUUMBOLD14OUTLINE_Addr,Color(0,0,255,255),DS_ALIGN_LEFT);
    old_GridItem_DrawMPTarget(this, g);
}

void GridItem::TakeDamgae(int theDamage, unsigned int theDamageFlags) {
    //    if (!mApp->IsVSMode())
    //        return;
    //
    //    if (mGridItemType == GridItemType::GRIDITEM_MP_TARGET_ZOMBIE) {
    //        if (mDead || mVSTargetZombieHealth <= 0)
    //            return;
    //
    //        mVSTargetZombieHealth -= theDamage;
    //
    //        if (mVSTargetZombieHealth > 0) {
    //            Reanimation* reanim = mApp->ReanimationGet(mGridItemReanimID);
    //            if (reanim) {
    //                reanim->PlayReanim("anim_hit", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, mDead ? 1 : 0, 24.0f);
    //            }
    //            return;
    //        }
    //
    //        mVSTargetZombieHealth = 0;
    //
    //        if (mBoard->GetMPTargetCount() > 3) {
    //            Reanimation* reanim = mApp->ReanimationGet(mGridItemReanimID);
    //            if (reanim) {
    //                reanim->AssignRenderGroupToTrack("target", -1);
    //                reanim->PlayReanim("anim_death2", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, mDead ? 1 : 0, 12.0f);
    //            }
    //            mTargetJustGotShotCounter = 300;
    //            return;
    //        }
    //
    //        GridItemDie();
    //        return;
    //    }
    //
    //    if (mGridItemType == GridItemType::GRIDITEM_GRAVESTONE) {
    //        if (mDead || mVSGraveStoneHealth <= 0)
    //            return;
    //
    //        mVSGraveStoneHealth -= theDamage;
    //
    //        if (mVSGraveStoneHealth <= 0) {
    //            GridItemDie();
    //            return;
    //        }
    //
    //        mGraveJustGotShotCounter = std::max(mGraveJustGotShotCounter, 25);
    //
    //        float maxHealth = mIsSpecialGrave ? 1000.0f : 350.0f;
    //
    //        // 计算当前和上一帧的健康比例（0~6段）
    //        float currentRatio = static_cast<float>(mVSGraveStoneHealth) / maxHealth;
    //        float previousRatio = static_cast<float>(mVSGraveStoneHealth) / maxHealth;
    //
    //        int prevBreakStage = static_cast<int>(6.0f - (currentRatio * 6.0f));
    //        int newBreakStage = static_cast<int>(6.0f - (previousRatio * 6.0f));
    //
    //        // 限制阶段在0~6之间
    //        prevBreakStage = std::clamp(prevBreakStage, 0, 6);
    //        newBreakStage = std::clamp(newBreakStage, 0, 6);
    //
    //        // 若阶段变化，播放破碎动画
    //        if (newBreakStage != prevBreakStage) {
    //            Reanimation* reanim = mApp->ReanimationTryToGet(mGridItemReanimID);
    //            if (reanim) {
    //                pvzstl::string animName = StrFormat("anim_break0%d", newBreakStage);
    ////                snprintf(animName, sizeof(animName), "anim_break%d", newBreakStage);
    //                reanim->PlayReanim(animName.c_str(), ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 4, 10.0f);
    //            }
    //        }
    //    }

    old_GridItem_TakeDamage(this, theDamage, theDamageFlags);

    if (mGridItemType == GridItemType::GRIDITEM_MP_BURIAL_MOUND) {
        if (mDead || mVSGraveStoneHealth <= 0)
            return;

        mVSGraveStoneHealth -= theDamage;

        if (mVSGraveStoneHealth <= 0) {
            GridItemDie();
            return;
        }

        mGraveJustGotShotCounter = std::max(mGraveJustGotShotCounter, 25);
    }
}