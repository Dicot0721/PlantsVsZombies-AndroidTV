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

#include "PvZ/Lawn/Board/SeedBank.h"
#include "Homura/Logger.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Board/CutScene.h"
#include "PvZ/Lawn/Board/SeedPacket.h"
#include "PvZ/Lawn/GamepadControls.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/Widget/VSSetupMenu.h"
#include "PvZ/MagicAddr.h"
#include "PvZ/Misc.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/Symbols.h"

using namespace Sexy;

SeedBank::SeedBank(bool thePlayerIndex) {
    _constructor(thePlayerIndex);
}

void SeedBank::_constructor(bool thePlayerIndex) {
    old_SeedBank_SeedBank(this, thePlayerIndex);
}

void SeedBank::Draw(Sexy::Graphics *g) {
    // 在对战模式取消0.9倍缩放
    if (mApp->mGameMode != GameMode::GAMEMODE_MP_VS) {
        old_SeedBank_Draw(this, g);
        return;
    }
    g->PushState();
    // if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS) {
    // g->mTransX = 0;
    // g->mTransY = 0;
    // int transform[12];
    // Sexy_SexyTransform2D_SexyTransform2D(transform);
    // Sexy_SexyTransform2D_Scale(transform, 0.9, 0.9);
    // int theOffsetX = *(uint8_t *) (*(uint32_t *) (*((uint32_t *) seedBank + 4) + 740) + 96) ? 240 : 80;
    // float theTranslateX = seedBank->mX + theOffsetX;
    // float theTranslateY = seedBank->mY + 60;
    // Sexy_SexyTransform2D_Translate(transform, theTranslateX, theTranslateY);
    ////        Sexy_Graphics_PushTransform(g,transform, true);
    ////        Sexy_Image_PushTransform(g->mDestImage,transform,true);
    // (*(void (**)(uint32_t, int *, bool))(**((uint32_t **)g + 1) + 140))(*((uint32_t *)g + 1),transform,true);
    // }

    if (gVSSetupAddonWidget && gVSSetupAddonWidget->mBanMode) {
        g->SetColorizeImages(true);
        g->SetColor(Color(155, 155, 155));
    }

    if (mApp->mGameScene != GameScenes::SCENE_PLAYING) {
        g->mTransX = g->mTransX - mBoard->mX;
        g->mTransY = g->mTransY - mBoard->mY;
    }
    CutScene *cutScene = mBoard->mCutScene;
    if (cutScene != nullptr && cutScene->IsBeforePreloading()) {
        return;
    }
    if (mApp->IsSlotMachineLevel()) {
        g->DrawImage(*Sexy_IMAGE_SUNBANK_Addr, 0, 0);
    } else if (mBoard->HasConveyorBeltSeedBank(mIsZombie)) {
        if (mApp->IsCoopMode()) {
            g->DrawImage(*Sexy_IMAGE_CONVEYORBELT_BACKDROP_COOP_Addr, 0, 0);
            g->DrawImageCel(*Sexy_IMAGE_CONVEYORBELT_COOP_Addr, 7, 63, 0, mConveyorBeltCounter / 4 % 6);
            g->SetClipRect(7, 0, 313, 600);
        } else {
            g->DrawImage(*Sexy_IMAGE_CONVEYORBELT_BACKDROP_Addr, 83, 0);
            g->DrawImageCel(*Sexy_IMAGE_CONVEYORBELT_Addr, 90, 63, 0, mConveyorBeltCounter / 4 % 6);
            g->SetClipRect(90, 0, 501, 600);
        }
    } else if (mApp->IsCoopMode()) {
        g->DrawImage(*Sexy_IMAGE_SEEDBANK_COOP_Addr, 0, 0);
    } else if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS && mIsZombie) {
        int theSeedBankExtraWidth = mBoard->GetSeedBankExtraWidth();
        g->DrawImage(*Sexy_IMAGE_ZOMBIE_SEEDBANK_Addr, theSeedBankExtraWidth, 0);
    } else {
        Sexy::Image *seedBankImage = *Sexy_IMAGE_SEEDBANK_Addr;
        int theSeedBankExtraWidth = mBoard->GetSeedBankExtraWidth();
        int theRect[4];
        theRect[0] = seedBankImage->mWidth - 12 - theSeedBankExtraWidth;
        theRect[1] = 0;
        theRect[2] = theSeedBankExtraWidth + 12;
        theRect[3] = seedBankImage->mHeight;
        g->DrawImage(seedBankImage, 0, 0);
        g->DrawImage(seedBankImage, seedBankImage->mWidth - 12, 0, (Rect &)theRect);
    }
    SeedPacket *seedPacket1 = nullptr;
    SeedPacket *seedPacket2 = nullptr;
    if (mNumPackets > 0) {
        for (int i = 0; i < mNumPackets; ++i) {
            SeedPacket *seedPacket = &(mSeedPackets[i]);
            if (seedPacket->mPacketType == SeedType::SEED_NONE) {
                continue;
            }
            if (!mApp->IsSlotMachineLevel()) {
                if (mApp->IsCoopMode() || mApp->mGameMode == GameMode::GAMEMODE_MP_VS) {
                    bool rightSideSeedbank = mBoard->mSeedBankRight == this;
                    GamepadControls *gamepadControls = rightSideSeedbank ? mBoard->mGamepadControls2 : mBoard->mGamepadControls1;
                    if (gamepadControls->mPlayerIndex2 != -1 && i == gamepadControls->mSelectedSeedIndex) {
                        if (rightSideSeedbank) {
                            seedPacket2 = seedPacket;
                        } else {
                            seedPacket1 = seedPacket;
                        }
                        continue;
                    }
                } else {
                    GamepadControls *gamepadControls1 = mBoard->mGamepadControls1;
                    if (gamepadControls1->mPlayerIndex2 != -1 && i == gamepadControls1->mSelectedSeedIndex) {
                        seedPacket1 = seedPacket;
                        continue;
                    }
                    GamepadControls *gamepadControls2 = mBoard->mGamepadControls2;
                    if (gamepadControls2->mPlayerIndex2 != -1 && i == gamepadControls2->mSelectedSeedIndex) {
                        seedPacket2 = seedPacket;
                        continue;
                    }
                }
            }
            if (seedPacket->BeginDraw(g)) {
                seedPacket->DrawBackground(g);
                seedPacket->EndDraw(g);
            }
        }
        for (int i = 0; i < mNumPackets; ++i) {
            SeedPacket *seedPacket = &(mSeedPackets[i]);
            if (seedPacket->mPacketType == SeedType::SEED_NONE) {
                continue;
            }
            if (seedPacket == seedPacket1 || seedPacket == seedPacket2) {
                continue;
            }
            if (seedPacket->BeginDraw(g)) {
                seedPacket->DrawMiddle(g);
                seedPacket->EndDraw(g);
            }
        }

        for (int i = 0; i < mNumPackets; ++i) {
            SeedPacket *seedPacket = &(mSeedPackets[i]);
            if (seedPacket->mPacketType == SeedType::SEED_NONE) {
                continue;
            }
            if (seedPacket == seedPacket1 || seedPacket == seedPacket2) {
                continue;
            }
            if (seedPacket->BeginDraw(g)) {
                seedPacket->DrawOverlay(g);
                seedPacket->EndDraw(g);
            }
        }
    }
    g->mClipRect.mX -= 20;
    g->mClipRect.mY -= 20;
    g->mClipRect.mWidth += 20;
    g->mClipRect.mHeight += 30;
    if (seedPacket2 != nullptr && seedPacket2->BeginDraw(g)) {
        seedPacket2->Draw(g);
        seedPacket2->EndDraw(g);
    }
    if (seedPacket1 != nullptr && seedPacket1->BeginDraw(g)) {
        seedPacket1->Draw(g);
        seedPacket1->EndDraw(g);
    }
    g->ClearClipRect();
    if (mApp->IsSlotMachineLevel()) {
        if (mY > -(*(int (**)(Sexy::Image *))(*Sexy_IMAGE_SEEDBANK_Addr + 20))(*Sexy_IMAGE_SEEDBANK_Addr))
            g->DrawImage(*Sexy_IMAGE_SLOTMACHINE_OVERLAY_Addr, 189, -2);
    }
    if (!mBoard->HasConveyorBeltSeedBank(0)) {
        int theMoney;
        if (mIsZombie) {
            theMoney = mBoard->mDeathMoney & ~mBoard->mDeathMoney >> 31; // mDeathMoney
        } else {
            theMoney = mBoard->mSunMoney1 & ~mBoard->mSunMoney1 >> 31; // mSunMoney1
        }

        if (mApp->IsCoopMode() && mBoard->mSeedBankRight == this) {
            theMoney = mBoard->mSunMoney2 & ~mBoard->mSunMoney2 >> 31; // mSunMoney2
        }
        Color theColor = {0, 0, 0, 255};
        pvzstl::string str = StrFormat("%d", theMoney);
        int mOutOfMoneyCounter = mBoard->mOutOfMoneyCounter;
        if (mOutOfMoneyCounter > 0 && mOutOfMoneyCounter % 20 <= 9) {
            theColor.mRed = 255;
            theColor.mGreen = 0;
            theColor.mBlue = 0;
            theColor.mAlpha = 255;
        }
        TodDrawString(g, str, mIsZombie ? (408 + mBoard->GetSeedBankExtraWidth()) : 38, 78, *Sexy_FONT_CONTINUUMBOLD14_Addr, theColor, DrawStringJustification::DS_ALIGN_CENTER);

        if (mApp->IsTwinSunbankMode()) {
            pvzstl::string str1 = StrFormat("%d", mBoard->mSunMoney2 & ~mBoard->mSunMoney2 >> 31);
            TodDrawString(g, str1, 644, 49, *Sexy_FONT_CONTINUUMBOLD14_Addr, theColor, DrawStringJustification::DS_ALIGN_CENTER);
        }
    }
    if (mApp->mGameScene != GameScenes::SCENE_PLAYING) {
        g->mTransX = g->mTransX + mBoard->mX;
        g->mTransY = g->mTransY + mBoard->mY;
    }
    // if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS) {
    ////        Sexy_Graphics_PopTransform(g);
    ////        Sexy_Image_PopTransform(g->mDestImage);
    // (*(void (**)(uint32_t))(**((uint32_t **)g + 1) + 144))(*((uint32_t *)g + 1));
    // }
    g->PopState();
}

bool SeedBank::MouseHitTest(int x, int y, HitResult *theHitResult) {
    int relativeX = x - mX;
    int relativeY = y - mY;
    if (mWidth - 4 <= relativeX || mNumPackets <= 0) {
        theHitResult->mObject = nullptr;
        theHitResult->mObjectType = GameObjectType::OBJECT_TYPE_NONE;
        return false;
    }

    for (int i = 0; i < mNumPackets; i++) {
        if (mSeedPackets[i].MouseHitTest(relativeX, relativeY, theHitResult)) {
            return true;
        }
    }

    theHitResult->mObject = nullptr;
    theHitResult->mObjectType = GameObjectType::OBJECT_TYPE_NONE;
    return false;
}


void SeedBank::UpdateWidth() {
    old_SeedBank_UpdateWidth(this);
}

bool SeedBank::ContainsPoint(int theX, int theY) {
    return theX >= mX && theX < mX + mWidth && theY >= mY && theY < mY + mHeight;
}

void SeedBank::Move(int x, int y) {
    // 在对战模式 错开双方的Bank
    mX = x;
    mY = y;
    if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS) {
        int theSeedBankExtraWidth = mBoard->GetSeedBankExtraWidth();
        if (mIsZombie) {
            mX += (5 - theSeedBankExtraWidth / 2);
        } else {
            mX -= (45 + theSeedBankExtraWidth / 2);
        }
    }
}

bool SeedBank::BeginDraw(Sexy::Graphics *g) {
    return old_SeedBank_BeginDraw(this, g);
}


void SeedBank::EndDraw(Sexy::Graphics *g) {
    old_SeedBank_EndDraw(this, g);
}

void SeedBank::AddSeed(int a2, int a3) {
    old_SeedBank_AddSeed(this, a2, a3);
}
