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

#include "PvZ/Lawn/Board/Coin.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Board/Challenge.h"
#include "PvZ/Lawn/Common/GameConstants.h"
#include "PvZ/Lawn/GamepadControls.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/MagicAddr.h"
#include "PvZ/Misc.h"
#include "PvZ/TodLib/Common/TodStringFile.h"
#include "PvZ/TodLib/Effect/Attachment.h"

#include <cmath>

using namespace Sexy;

void Coin::CoinInitialize(int theX, int theY, CoinType theCoinType, CoinMotion theCoinMotion) {
    old_Coin_CoinInitialize(this, theX, theY, theCoinType, theCoinMotion);
}

void Coin::GamepadCursorOver(int thePlayerIndex) {
    //*((uint32_t *)a + 29) == 16 则意味着是砸罐子种子雨老虎机中的植物卡片

    if (!keyboardMode && mType == CoinType::COIN_USABLE_SEED_PACKET) {
        return;
    }


    old_Coin_GamepadCursorOver(this, thePlayerIndex);

    //    if (mBoard && !mBoard->mPaused && mApp->mGameScene == SCENE_PLAYING && !mDead) {
    //        if (mType == COIN_USABLE_SEED_PACKET) {
    //            Collect(thePlayerIndex);
    //        } else if (mType != COIN_COOP_DOUBLE_SUN && !mIsBeingCollected && mCoinMotion != COIN_MOTION_FROM_NEAR_CURSOR) {
    //            if (!IsSun() || mScale >= GetSunScale()) {
    //                mPlayerIndex2 = thePlayerIndex;
    //                mCoinMotion = COIN_MOTION_FROM_NEAR_CURSOR;
    //                unk2 = 0.0;
    //                if (IsSun())
    //                    mScale = GetSunScale();
    //                if (mApp->IsFirstTimeAdventureMode()) {
    //                    if (mBoard->mLevel == 1) {
    //                        pvzstl::string str = TodStringTranslate("[ADVICE_CLICKED_ON_SUN]");
    //                        mBoard->DisplayAdvice(str, MESSAGE_STYLE_TUTORIAL_LEVEL1_STAY, ADVICE_CLICKED_ON_SUN);
    //                    }
    //                }
    //            }
    //        }
    //    }
}

void Coin::Update() {
    if (BanDropCoin && (mType <= CoinType::COIN_LARGESUN || mType == CoinType::COIN_COOP_DOUBLE_SUN || mType == CoinType::COIN_VS_ZOMBIE_BRAIN)) {
        // 开启了"禁止掉落阳光金币"时
        Die();
        return;
    }

    if (mType == CoinType::COIN_VS_PLANT_TROPHY || mType == CoinType::COIN_VS_ZOMBIE_TROPHY) {
        old_Coin_Update(this);
        return;
    }

    if (enableManualCollect) {
        // 如果开了手动拾取，则重置Coin的存在时间计数器为0，从而不会触发自动拾取。
        GameMode aGameMode = mApp->mGameMode;
        // 在重型武器中、花园中依然自动收集；在关卡结束后依然自动收集。
        if (aGameMode != GameMode::GAMEMODE_CHALLENGE_HEAVY_WEAPON && aGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN && aGameMode != GameMode::GAMEMODE_TREE_OF_WISDOM
            && mApp->mGameScene == GameScenes::SCENE_PLAYING && mBoard->mBoardFadeOutCounter <= 0) {
            mCoinAge = 0;
        }
    } else if (mType == CoinType::COIN_VS_ZOMBIE_BRAIN) {
        // 如果没有关闭自动拾取，则为对战模式的僵尸方阳光也加入自动拾取。
        if (mCoinAge > 79 && !mIsBeingCollected) {
            Collect(0);
        }
    }
    if (mApp->IsCoopMode() && (mType == CoinType::COIN_SUN || mType == CoinType::COIN_COOP_DOUBLE_SUN || mType == CoinType::COIN_SMALLSUN || mType == CoinType::COIN_LARGESUN)) {
        // 在结盟模式关闭阳光自动拾取。
        mCoinAge = 0;
    }

    if (IsLevelAward()) {
        // 为关卡结束后的奖励掉落物关闭自动拾取。
        mCoinAge = 0;
    }

    old_Coin_Update(this);
}

void Coin::UpdateFallForAward() {
    // 试图去除关卡掉落物在关卡结束后的自动收集。

    if (unk3)
        return;

    if (mCoinMotion == CoinMotion::COIN_MOTION_FROM_PRESENT) {
        mPosX += mVelX;
        mPosY += mVelY;
        mVelX *= 0.95f;
        mVelY *= 0.95f;
        if (mCoinAge >= 80) {
            Collect(0);
        }
    } else if (mCoinMotion == CoinMotion::COIN_MOTION_FROM_FROM_VS_WON) {
        float v30 = mVelY + 0.2;
        float v34 = v30 + mPosY;
        mPosY = v34;
        mVelY = v30 * 0.95;
        mVelX *= 0.95f;
        mPosX += mVelX;
        if (v34 >= mGroundY) {
            if (sqrtf(mVelY * mVelY + mVelX * mVelX) > 0.5) {
                mApp->PlayFoley(FoleyType::FOLEY_GRASSSTEP);
                mVelY *= -1;
            } else {
                mPosY = mGroundY;
                mVelY = 0.0;
            }
        }
        if (mCoinAge > 199) {
            Collect(0);
        }
    } else if (mCoinMotion == CoinMotion::COIN_MOTION_FROM_NEAR_CURSOR) {
        if (mPlayerIndex >= 0) {
            GamepadControls *gamepadControls = mBoard->GetGamepadControlsByPlayerIndex(mPlayerIndex);
            float v55 = gamepadControls->mCursorPositionX;
            float v56 = gamepadControls->mCursorPositionY;
            float v40 = (float)mWidth / 2;
            v56 = v56 - ((float)mHeight / 2);
            float v41 = v56 - mPosY;
            v55 = v55 - v40;
            float v42 = v55 - mPosX;
            if ((v41 * v41 + v42 * v42) < 1225.0) {
                Collect(mPlayerIndex);
                return;
            }
            float v43 = v41 * 400.0;
            float v44 = v42 * 400.0;
            float v45 = sqrtf(v43 * v43 + v44 * v44);
            float v46 = unk2 + ((6.4 / (v45 / 100.0)) * (v45 / 100.0));
            if (v46 > 600.0)
                v46 = 600.0;
            unk2 = v46;
            if (v45 != 0.0) {
                v43 = v43 / v45;
                v44 = v44 / v45;
            }
            mPosX += (unk2 * v44) * 0.016;
            mPosY += (unk2 * v43) * 0.016;
        }
    } else if (mPosY + mVelY < mGroundY) {
        mPosY += mVelY;
        if (mCoinMotion == CoinMotion::COIN_MOTION_FROM_PLANT || mCoinMotion == CoinMotion::COIN_MOTION_FROM_FROM_GRAVE) {
            mVelY += 0.09;
        } else if (mCoinMotion == CoinMotion::COIN_MOTION_COIN || mCoinMotion == CoinMotion::COIN_MOTION_FROM_BOSS) {
            mVelY += 0.15;
        }

        mPosX += mVelX;
        if (mPosX > BOARD_WIDTH - mWidth && mCoinMotion != CoinMotion::COIN_MOTION_FROM_BOSS) {
            mPosX = BOARD_WIDTH - mWidth;
            // mVelX = -0.4f - RandRangeFloat(0.0f, 0.4f);
        } else if (mPosX < 0.0f) {
            mPosX = 0.0f;
            // mVelX = 0.4f + RandRangeFloat(0.0f, 0.4f);
        }
    } else {
        if (mNeedsBouncyArrow && !mHasBouncyArrow) {
            float aParticleOffsetX = mWidth / 2;
            float aParticleOffsetY = mHeight / 2 - 60;
            if (mType == CoinType::COIN_TROPHY) {
                aParticleOffsetX += 2.0;
            } else if (mType == CoinType::COIN_VS_PLANT_TROPHY || mType == CoinType::COIN_VS_ZOMBIE_TROPHY) {
                aParticleOffsetY -= 20.0;
                int aRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_UI_TOP, mRow, mHasBouncyArrow);
                TodParticleSystem *aParticle = mApp->AddTodParticle(mPosX, mPosY, aRenderOrder, ParticleEffect::PARTICLE_TROPHY_SPARKLE);
                AttachParticle(*(mAttachmentID + 2), aParticle, 0, 0.0);
            } else if (mType == CoinType::COIN_AWARD_MONEY_BAG || mType == CoinType::COIN_AWARD_BAG_DIAMOND) {
                aParticleOffsetY -= 2.0;
                aParticleOffsetX += 2.0;
            } else if (mType == CoinType::COIN_AWARD_PRESENT || IsPresentWithAdvice()) {
                aParticleOffsetY -= 20.0;
            } else if (mType == CoinType::COIN_AWARD_SILVER_SUNFLOWER || mType == CoinType::COIN_AWARD_GOLD_SUNFLOWER) {
                aParticleOffsetX -= 6.0f;
                aParticleOffsetY -= 40.0f;
            } else if (IsMoney()) {
                aParticleOffsetX += 12.0f;
                aParticleOffsetY += 21.0f;
            }

            ParticleEffect aEffect;
            if (mType == CoinType::COIN_FINAL_SEED_PACKET) {
                aEffect = ParticleEffect::PARTICLE_SEED_PACKET;
            } else if (IsMoney()) {
                aEffect = ParticleEffect::PARTICLE_COIN_PICKUP_ARROW;
            } else {
                aEffect = ParticleEffect::PARTICLE_AWARD_PICKUP_ARROW;
            }

            TodParticleSystem *aParticle = mApp->AddTodParticle(mPosX + aParticleOffsetX, mPosY + aParticleOffsetY, 0, aEffect);
            AttachParticle(*mAttachmentID, aParticle, aParticleOffsetX, aParticleOffsetY);
            mHasBouncyArrow = true;
        }

        if (!mHitGround) {
            mHitGround = true;
            PlayGroundSound();
        }

        mPosY = mGroundY;
        mPosX = std::round(mPosX);


        if (mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_LAST_STAND || mBoard == nullptr || mBoard->mChallenge->mChallengeState == ChallengeState::STATECHALLENGE_LAST_STAND_ONSLAUGHT) {
            if (!IsLevelAward() && !IsPresentWithAdvice()) {
                ++mDisappearCounter;
                if (mDisappearCounter >= GetDisappearTime()) {
                    StartFade();
                }
            }
        }
    }

    if (mCoinMotion == CoinMotion::COIN_MOTION_FROM_PLANT || mCoinMotion == CoinMotion::COIN_MOTION_FROM_FROM_GRAVE) {
        float aFinalScale = GetSunScale();
        if (mScale < aFinalScale) {
            mScale += 0.02f;
        } else {
            mScale = aFinalScale;
        }
    }
}

void Coin::UpdateFall() {
    // 去除关卡掉落物在关卡结束后的自动收集。
    if ((mType >= CoinType::COIN_AWARD_MONEY_BAG && mType <= CoinType::COIN_AWARD_GOLD_SUNFLOWER) || mType == CoinType::COIN_FINAL_SEED_PACKET) {
        UpdateFallForAward();
        return;
    }

    old_Coin_UpdateFall(this);
}

bool Coin::MouseHitTest(int theX, int theY, int **theHitResult, int thePlayerIndex) {
    // 去除在玩家按A键时的阳光金币检测，以防止玩家种植、铲除、发射加农炮时的操作被阳光金币遮挡。
    if (mType <= CoinType::COIN_LARGESUN || mType == CoinType::COIN_COOP_DOUBLE_SUN || mType == CoinType::COIN_VS_ZOMBIE_BRAIN) {
        return false;
    }

    return old_Coin_MouseHitTest(this, theX, theY, theHitResult, thePlayerIndex);
}

bool Coin::IsSun() {
    return mType == CoinType::COIN_SUN || mType == CoinType::COIN_SMALLSUN || mType == CoinType::COIN_LARGESUN;
}

void Coin::Draw(Graphics *g) {
    old_Coin_Draw(this, g);
}

Color Coin::GetColor() {
    if ((IsSun() || IsMoney()) && mIsBeingCollected) {
        float aAlpha = ClampFloat(mCollectionDistance * 0.035f, 0.35f, 1.0f) * 255.0f;
        return Color(255, 255, 255, aAlpha);
    }

    if (mFadeCount > 0) {
        int aAlpha = TodAnimateCurve(15, 0, mFadeCount, 255, 0, TodCurves::CURVE_LINEAR);
        return Color(255, 255, 255, aAlpha);
    }

    return Color::White;
}