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

#include "PvZ/Lawn/Widget/SeedChooserScreen.h"
#include "Homura/Logger.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Board/CutScene.h"
#include "PvZ/Lawn/Board/SeedBank.h"
#include "PvZ/Lawn/GamepadControls.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/Widget/GameButton.h"
#include "PvZ/Lawn/Widget/VSSetupMenu.h"
#include "PvZ/Lawn/Widget/WaitForSecondPlayerDialog.h"
#include "PvZ/Misc.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Common/TodStringFile.h"
#include <unistd.h>

using namespace Sexy;


SeedChooserScreen::SeedChooserScreen(bool theIsZombieChooser) {
    _constructor(theIsZombieChooser);
}


void SeedChooserScreen::_constructor(bool theIsZombieChooser) {
    // 记录当前游戏状态，同时修复在没解锁商店图鉴时依然显示相应按钮的问题、对战选种子界面的按钮问题；
    // 还添加了生存模式保留上次选卡，添加坚果艺术关卡默认选择坚果，添加向日葵艺术关卡默认选择坚果、杨桃、萝卜伞
    mApp = reinterpret_cast<LawnApp *>(*Sexy_gSexyAppBase_Addr);
    mBoard = mApp->mBoard;
    GameMode mGameMode = mApp->mGameMode;
    if (mBoard->mCutScene->IsSurvivalRepick() && !mApp->IsCoopMode()) {
        GamepadControls *gamePad = mBoard->mGamepadControls1;
        SeedBank *mSeedBank = gamePad->GetSeedBank();
        int mNumPackets = mSeedBank->mNumPackets;
        std::vector<SeedType> aSeedArray(mNumPackets);
        SeedType aImitaterType = SeedType::SEED_NONE;
        for (int i = 0; i < mNumPackets; i++) {
            SeedPacket aSeedPacket = mSeedBank->mSeedPackets[i];
            aSeedArray[i] = aSeedPacket.mPacketType;
            if (aSeedPacket.mPacketType == SeedType::SEED_IMITATER && aImitaterType == SeedType::SEED_NONE) {
                aImitaterType = aSeedPacket.mImitaterType;
            }
        }

        old_SeedChooserScreen_SeedChooserScreen(this, theIsZombieChooser);

        // 实现无尽模式保留上次选卡。为什么不直接像WP版那样一一对应地选卡呢？因为玩家有可能通过爆炸坚果修改卡槽选中了多个相同类型的卡片或不在SeedChooser内的卡片，一一对应的话会有BUG
        int theValidChosenSeedNum = 0;
        for (int i = 0; i < mNumPackets; i++) {
            SeedType theSeed = aSeedArray[i];
            if (theSeed >= SeedType::NUM_SEEDS_IN_CHOOSER)
                continue;

            ChosenSeed *theChosenSeed = &(mChosenSeeds[theSeed]);
            if (theChosenSeed->mSeedType == SeedType::SEED_IMITATER) {
                theChosenSeed->mImitaterType = aImitaterType;
            }

            if (theChosenSeed->mSeedState == ChosenSeedState::SEED_IN_BANK)
                continue;

            GetSeedPositionInBank(theValidChosenSeedNum, theChosenSeed->mX, theChosenSeed->mY, 0);
            theChosenSeed->mEndX = theChosenSeed->mX;
            theChosenSeed->mEndY = theChosenSeed->mY;
            theChosenSeed->mStartX = theChosenSeed->mX;
            theChosenSeed->mStartY = theChosenSeed->mY;
            theChosenSeed->mSeedState = ChosenSeedState::SEED_IN_BANK;
            theChosenSeed->mSeedIndexInBank = theValidChosenSeedNum;
            theValidChosenSeedNum++;
        }

        mSeedsInBank = theValidChosenSeedNum;
        mSeedsIn1PBank = theValidChosenSeedNum;
        if (theValidChosenSeedNum == mNumPackets) {
            EnableStartButton(true);
        }

    } else {
        old_SeedChooserScreen_SeedChooserScreen(this, theIsZombieChooser);
    }

    if (mGameMode == GameMode::GAMEMODE_CHALLENGE_ART_CHALLENGE_WALLNUT) {
        ChosenSeed *theChosenSeed = &(mChosenSeeds[SeedType::SEED_WALLNUT]);
        theChosenSeed->mX = mBoard->GetSeedPacketPositionX(0, 0, false);
        theChosenSeed->mY = 8;
        theChosenSeed->mEndX = theChosenSeed->mX;
        theChosenSeed->mEndY = theChosenSeed->mY;
        theChosenSeed->mStartX = theChosenSeed->mX;
        theChosenSeed->mStartY = theChosenSeed->mY;
        theChosenSeed->mSeedState = ChosenSeedState::SEED_IN_BANK;
        theChosenSeed->mSeedIndexInBank = 0;
        mSeedsInBank += 1;
        mSeedsIn1PBank += 1;
    } else if (mGameMode == GameMode::GAMEMODE_CHALLENGE_ART_CHALLENGE_SUNFLOWER) {
        SeedType types[] = {SeedType::SEED_WALLNUT, SeedType::SEED_STARFRUIT, SeedType::SEED_UMBRELLA};
        for (int i = 0; i < std::size(types); ++i) {
            ChosenSeed *theChosenSeed = &(mChosenSeeds[types[i]]);
            GetSeedPositionInBank(i, theChosenSeed->mX, theChosenSeed->mY, 0);
            theChosenSeed->mEndX = theChosenSeed->mX;
            theChosenSeed->mEndY = theChosenSeed->mY;
            theChosenSeed->mStartX = theChosenSeed->mX;
            theChosenSeed->mStartY = theChosenSeed->mY;
            theChosenSeed->mSeedState = ChosenSeedState::SEED_IN_BANK;
            theChosenSeed->mSeedIndexInBank = i;
            mSeedsInBank += 1;
            mSeedsIn1PBank += 1;
        }
    }

    if (mApp->IsVSMode()) {
        // 去除对战中的冗余按钮
        if (mStoreButton) {
            mStoreButton->mDisabled = true;
            mStoreButton->mBtnNoDraw = true;
        }
        if (mAlmanacButton) {
            mAlmanacButton->mDisabled = true;
            mAlmanacButton->mBtnNoDraw = true;
        }
        if (mStartButton) { // 此处仿照PS3版处理，同时去除双方的开始按钮
            mStartButton->mDisabled = true;
            mStartButton->mBtnNoDraw = true;
        }
    } else {
        if (mStoreButton) {
            if (!mApp->CanShowStore()) { // 去除在未解锁商店时商店按钮
                mStoreButton->mDisabled = true;
                mStoreButton->mBtnNoDraw = true;
            }
        }
        if (mAlmanacButton) {
            if (!mApp->CanShowAlmanac()) { // 去除在未解锁图鉴时的图鉴按钮
                mAlmanacButton->mDisabled = true;
                mAlmanacButton->mBtnNoDraw = true;
            }
        }
    }

    if (mApp->mGameMode != GameMode::GAMEMODE_MP_VS && !mIsZombieChooser) {
        pvzstl::string str = TodStringTranslate("[MENU_BUTTON]");
        gSeedChooserScreenMainMenuButton = MakeButton(104, this, this, str);
        gSeedChooserScreenMainMenuButton->Resize(mApp->IsCoopMode() ? 345 : 650, -3, 120, 80);
        AddWidget(gSeedChooserScreenMainMenuButton);
    }
}


void SeedChooserScreen::RebuildHelpbar() {
    // 拓宽Widget大小
    if (mApp->mGameMode != GameMode::GAMEMODE_MP_VS && !mIsZombieChooser) {
        Resize(mX, mY, 800, 600); // 原本(472,521)，改为(800,600)，不然没办法点击模仿者按钮和底栏三按钮。
    } else {
        Resize(mX, mY, mWidth, 600);
    }

    old_SeedChooserScreen_RebuildHelpbar(this);
}


void SeedChooserScreen::Update() {
    // 记录当前1P选卡是否选满
    if (mApp->IsCoopMode()) {
        m1PChoosingSeeds = mSeedsIn1PBank < 4;
    }

    old_SeedChooserScreen_Update(this);
}


void SeedChooserScreen::EnableStartButton(int theIsEnabled) {
    // 双人键盘模式下结盟选满后直接开始
    if (theIsEnabled && mApp->IsCoopMode() && isKeyboardTwoPlayerMode) {
        old_SeedChooserScreen_EnableStartButton(this, theIsEnabled);
        OnStartButton();
        mBoard->mSeedBank[1]->mSeedPackets[3].mPacketType = mSeedType2;
        mBoard->mSeedBank[1]->mSeedPackets[3].mImitaterType = SeedType::SEED_NONE;
        return;
    }

    old_SeedChooserScreen_EnableStartButton(this, theIsEnabled);
}

void SeedChooserScreen::OnStartButton() {
    if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS) {
        // 如果是对战模式，则直接关闭种子选择界面。用于修复对战模式选卡完毕后点击开始按钮导致的闪退
        CloseSeedChooser();
        return;
    }

    old_SeedChooserScreen_OnStartButton(this);
}

bool SeedChooserScreen::SeedNotAllowedToPick(SeedType theSeedType) {
    // 解除更多对战场地中的某些植物不能选取的问题，如泳池对战不能选荷叶，屋顶对战不能选花盆
    if (mApp->IsVSMode()) {
        if (theSeedType == gVSSetupAddonWidget->mBannedSeed[theSeedType].mSeedType) {
            return true;
        }
        if (gVSSetupAddonWidget->mBanMode && theSeedType == SeedType::SEED_INSTANT_COFFEE) {
            return true;
        }
        if (mBoard->StageHasPool()) {
            if (theSeedType == SeedType::SEED_LILYPAD || theSeedType == SeedType::SEED_TANGLEKELP || theSeedType == SeedType::SEED_SEASHROOM) {
                return false;
            }
            if (mBoard->StageIsNight() && theSeedType == SeedType::SEED_PLANTERN) {
                return false;
            }
        } else {
            if (theSeedType == SeedType::SEED_ZOMBIE_DUCKY_TUBE || theSeedType == SeedType::SEED_ZOMBIE_SNORKEL || theSeedType == SeedType::SEED_ZOMBIE_DOLPHIN_RIDER) {
                return true;
            }
        }
        if (mBoard->StageHasRoof() && theSeedType == SeedType::SEED_FLOWERPOT) {
            return false;
        }
        if (gVSSetupAddonWidget && gVSSetupAddonWidget->mExtraSeedsMode && theSeedType == SeedType::SEED_BLOVER) {
            return false;
        }
    }

    return old_SeedChooserScreen_SeedNotAllowedToPick(this, theSeedType);
}

SeedType SeedChooserScreen::GetZombieSeedType(SeedType theSeedType) {
    int aSeedType = theSeedType + SEED_ZOMBIE_GRAVESTONE;
    if (gVSSetupAddonWidget && gVSSetupAddonWidget->mExtraSeedsMode) { // 解锁更多对战僵尸
        return aSeedType >= NUM_ZOMBIE_SEED_IN_CHOOSER ? SEED_NONE : (SeedType)aSeedType;
    } else {
        return aSeedType > SEED_ZOMBIE_GARGANTUAR ? SEED_NONE : (SeedType)aSeedType;
    }
}

ZombieType SeedChooserScreen::GetZombieType(ZombieType theZombieType) {
    return theZombieType >= NUM_ZOMBIE_TYPES ? ZOMBIE_INVALID : theZombieType;
}

int SeedChooserScreen::GetSeedPacketIndex(int theSeedIndex) {
    if (mIsZombieChooser)
        return theSeedIndex - SEED_ZOMBIE_GRAVESTONE;
    else
        return theSeedIndex;
}

void SeedChooserScreen::OnPlayerPickedSeed(int thePlayerIndex) {
    VSSetupMenu *aVSSetupScreen = mApp->mVSSetupMenu;
    if (aVSSetupScreen)
        aVSSetupScreen->OnPlayerPickedSeed(thePlayerIndex);
}

SeedType SeedChooserScreen::FindSeedInBank(int theIndexInBank, int thePlayerIndex) {
    for (SeedType aSeedType = SEED_PEASHOOTER; aSeedType < NUM_SEEDS_IN_CHOOSER; aSeedType = (SeedType)(aSeedType + 1)) {
        SeedType aZombieSeedType;
        if (mIsZombieChooser) {
            aZombieSeedType = GetZombieSeedType(aSeedType);
            aSeedType = aZombieSeedType;
        }
        if (HasPacket(aSeedType, mIsZombieChooser) && mChosenSeeds->mSeedState == SEED_IN_BANK && mChosenSeeds->mSeedIndexInBank == theIndexInBank
            && mChosenSeeds->mChosenPlayerIndex == thePlayerIndex) {
            return mChosenSeeds->mSeedType;
        }
    }
    return SEED_NONE;
}

void SeedChooserScreen::ClickedSeedInChooser(ChosenSeed &theChosenSeed, int thePlayerIndex) {
    // 实现1P结盟选卡选满后自动转换为2P选卡
    if (mApp->IsCoopMode())
        thePlayerIndex = !m1PChoosingSeeds;

    int aGamepadIndex = mApp->PlayerToGamepadIndex(thePlayerIndex);

    // 检查是否允许选择种子
    bool canPickSeed = true;

    // 合作模式检查
    if (mApp->IsCoopMode()) {
        if (mSeedsInBank > 8) {
            canPickSeed = false;
        }
    }
    // 非合作模式检查
    else if (mSeedsInBank == mSeedBank1->mNumPackets) {
        canPickSeed = false;
    }

    // VS模式检查
    if (mApp->IsVSMode() && !CanPickNow()) {
        mApp->PlaySample(*SOUND_BUZZER);
        canPickSeed = false;
    }

    // 检查玩家种子栏容量
    if (mApp->IsCoopMode()) {
        int *aNumSeedsInBank = (&mSeedsIn1PBank + thePlayerIndex);
        if (*aNumSeedsInBank > 3) {
            canPickSeed = false;
        }
    }

    if (!canPickSeed) {
        return;
    }

    // 禁选模式（BP）
    if (mApp->IsVSMode() && gVSSetupAddonWidget->mBanMode) {
        int x = (aGamepadIndex == 1) ? mCursorPositionX2 : mCursorPositionX1;
        int y = (aGamepadIndex == 1) ? mCursorPositionY2 : mCursorPositionY1;
        SeedType aSeedType = SeedHitTest(x, y);
        if (aSeedType != SEED_NONE && !SeedNotAllowedToPick(aSeedType)) {
            BannedSeed &aBannedSeed = gVSSetupAddonWidget->mBannedSeed[aSeedType];
            aBannedSeed.mSeedType = theChosenSeed.mSeedType;

            int aSeedBanned = aBannedSeed.mSeedType;

            gVSSetupAddonWidget->mBannedSeed[aSeedBanned].mX = theChosenSeed.mX;
            gVSSetupAddonWidget->mBannedSeed[aSeedBanned].mY = theChosenSeed.mY;
            gVSSetupAddonWidget->mBannedSeed[aSeedBanned].mSeedState = BannedSeedState::SEED_BANNED;
            if (mIsZombieChooser)
                gVSSetupAddonWidget->mBannedSeed[aSeedBanned].mChosenPlayerIndex = 1;

            gVSSetupAddonWidget->mSeedsInBothBanned++;
            if (gVSSetupAddonWidget->mSeedsInBothBanned == gVSSetupAddonWidget->mNumBanPackets) {
                gVSSetupAddonWidget->mBanMode = false;
                mBoard->SwitchGamepadControls();
            }

            mApp->PlaySample(*SOUND_TAP);
            OnPlayerPickedSeed(aGamepadIndex);
        }
        return;
    }

    // 确定种子栏
    int aSeedsInBank;
    if (mApp->IsCoopMode() && thePlayerIndex == 1) {
        aSeedsInBank = mSeedsIn2PBank;
    } else {
        aSeedsInBank = mSeedsIn1PBank;
    }

    // 设置种子动画参数
    theChosenSeed.mStartX = theChosenSeed.mX;
    theChosenSeed.mStartY = theChosenSeed.mY;
    theChosenSeed.mTimeStartMotion = mSeedChooserAge;
    theChosenSeed.mTimeEndMotion = mSeedChooserAge + 25;

    // 确定实际玩家索引
    int aActualPlayerIndex;
    if (mApp->IsAdventureMode()) {
        aActualPlayerIndex = 0;
        theChosenSeed.mChosenPlayerIndex = 0;
    } else {
        if (mApp->IsVSMode()) {
            VSSetupMenu *aVSSetupScreen = mApp->mVSSetupMenu;
            aActualPlayerIndex = (thePlayerIndex == 1) ? aVSSetupScreen->mSides[1] : aVSSetupScreen->mSides[0];
            theChosenSeed.mChosenPlayerIndex = aActualPlayerIndex;
        } else {
            aActualPlayerIndex = thePlayerIndex;
            theChosenSeed.mChosenPlayerIndex = thePlayerIndex;
        }
    }

    // 获取种子在种子栏中的位置
    GetSeedPositionInBank(aSeedsInBank, theChosenSeed.mEndX, theChosenSeed.mEndY, aActualPlayerIndex);

    // 更新种子状态和计数
    theChosenSeed.mSeedIndexInBank = aSeedsInBank;
    theChosenSeed.mSeedState = SEED_FLYING_TO_BANK;

    mSeedsInFlight++;
    mSeedsInBank++;

    if (mApp->IsCoopMode() && thePlayerIndex == 1) {
        mSeedsIn2PBank++;
    } else {
        mSeedsIn1PBank++;
    }

    // 播放音效并更新UI
    RemoveToolTip(thePlayerIndex);
    mApp->PlaySample(*SOUND_TAP);

    // 检查是否启用开始按钮
    if (!mApp->IsCoopMode() && mSeedsInBank == mSeedBank1->mNumPackets) {
        EnableStartButton(true);
    }

    if (mApp->IsCoopMode() && mSeedsInBank == (mSeedBank2->mNumPackets + mSeedBank1->mNumPackets)) {
        EnableStartButton(true);
    }

    // VS模式特殊处理
    if (mApp->IsVSMode()) {
        OnPlayerPickedSeed(aGamepadIndex);

        if (gVSSetupAddonWidget && gVSSetupAddonWidget->mSeedsInBothBanned > 0 && mSeedsIn1PBank == 4 && !mIsZombieChooser) {
            gVSSetupAddonWidget->mBanMode = true;
            gVSSetupAddonWidget->mSeedsInBothBanned = 0;
            gVSSetupAddonWidget->mNumBanPackets = 2;
            mBoard->SwitchGamepadControls();
        }
    }
}

void SeedChooserScreen::CrazyDavePickSeeds() {
    if (daveNoPickSeeds) {
        return;
    }

    old_SeedChooserScreen_CrazyDavePickSeeds(this);
}

void SeedChooserScreen::ClickedSeedInBank(ChosenSeed *theChosenSeed, unsigned int thePlayerIndex) {
    // 解决结盟1P选够4个种子之后，无法点击种子栏内的已选种子来退选的问题
    if (mApp->IsCoopMode()) {
        thePlayerIndex = theChosenSeed->mChosenPlayerIndex;
    }

    old_SeedChooserScreen_ClickedSeedInBank(this, theChosenSeed, thePlayerIndex);
}

void SeedChooserScreen::OnKeyDown(KeyCode theKey, unsigned int thePlayerIndex) {
    old_SeedChooserScreen_OnKeyDown(this, theKey, thePlayerIndex);
}

void SeedChooserScreen::GameButtonDown(GamepadButton theButton, unsigned int thePlayerIndex) {
    // 修复结盟2P无法选择模仿者
    if (mApp->IsCoopMode() && theButton == GamepadButton::BUTTONCODE_A) {
        if (mChooseState == SeedChooserState::CHOOSE_VIEW_LAWN) {
            old_SeedChooserScreen_GameButtonDown(this, theButton, thePlayerIndex);
            return;
        }

        if (mApp->mTwoPlayerState == -1 && mPlayerIndex != thePlayerIndex)
            return;

        SeedType aSeedType = thePlayerIndex ? mSeedType2 : mSeedType1;
        int aSeedsInBank = mSeedsInBank;
        // 此处将判定条件改为选满8个种子时无法选取模仿者。原版游戏中此处是选满4个则无法选取，导致模仿者选取出现问题。
        if (aSeedType == SeedType::SEED_IMITATER && aSeedsInBank < 8) {
            if (mChosenSeeds[SeedType::SEED_IMITATER].mSeedState != ChosenSeedState::SEED_IN_BANK) {
                // 先将已选种子数改为0，然后执行旧函数，这样模仿者选取界面就被打开了。
                mSeedsInBank = 0;
                old_SeedChooserScreen_GameButtonDown(this, theButton, thePlayerIndex);
                // 然后再恢复已选种子数即可。
                mSeedsInBank = aSeedsInBank;
                return;
            }
        }
    }

    if (tcpClientSocket >= 0 && mApp->IsVSMode()) {
        U8U8_Event event = {{EventType::EVENT_SEEDCHOOSER_SELECT_SEED}, uint8_t(mIsZombieChooser ? mSeedType2 : mSeedType1), mIsZombieChooser};
        sendWithSize(tcpClientSocket, &event, sizeof(U8U8_Event), 0);
    }

    old_SeedChooserScreen_GameButtonDown(this, theButton, thePlayerIndex);
}

void SeedChooserScreen::DrawPacket(
    Sexy::Graphics *g, int x, int y, SeedType theSeedType, SeedType theImitaterType, float thePercentDark, int theGrayness, Color *theColor, bool theDrawCost, bool theUseCurrentCost) {
    // 修复SeedChooser里的卡片亮度不正确。
    // 已选的卡片grayness为55，不推荐的卡片grayness为115。theColor则固定为{255,255,255,255}。

    // int aConvertedGrayness = ((theColor->mRed + theColor->mGreen + theColor->mBlue) / 3 + theGrayness) / 2;
    // 此算法用于在对战模式将非选卡的一方的卡片整体变暗。但这种算法下，55亮度会变成155亮度，115亮度会变成185亮度，严重影响非对战模式的选卡体验。所以需要修复。

    int aConvertedGrayness = (mApp->IsVSMode()) ? ((theColor->mRed + theColor->mGreen + theColor->mBlue) / 3 + theGrayness) / 2 : theGrayness;
    if (mApp->IsVSMode()) {
        if (mIsZombieChooser && SeedNotAllowedToPick(theSeedType)) {
            if (CanPickNow())
                aConvertedGrayness = 115;
            else
                aConvertedGrayness = 55;
        }

        if (gVSSetupAddonWidget && gVSSetupAddonWidget->mBanMode) {
            for (int i = 0; i < NUM_SEEDS_IN_CHOOSER; i++) {
                if (mChosenSeeds[i].mSeedType == theSeedType && mChosenSeeds[i].mSeedState == ChosenSeedState::SEED_IN_BANK) {
                    aConvertedGrayness = 115;
                }
            }
        }
    }

    DrawSeedPacket(g, x, y, theSeedType, theImitaterType, thePercentDark, aConvertedGrayness, theDrawCost, false, mIsZombieChooser, theUseCurrentCost);
}

void SeedChooserScreen::ButtonPress(int theId) {
    LawnApp *lawnApp = *gLawnApp_Addr;
    lawnApp->mSeedChooserScreen->mFocusedChildWidget = nullptr; // 修复点击菜单后无法按键选取植物卡片
}

void SeedChooserScreen::ButtonDepress(int theId) {

    if (mSeedsInFlight > 0 || mChooseState == SeedChooserState::CHOOSE_VIEW_LAWN || !mMouseVisible) {
        return;
    }

    if (theId == SeedChooserScreen_Menu) {
        mApp->PlaySample(*Sexy_SOUND_PAUSE_Addr);
        mApp->DoNewOptions(false, 0);
        return;
    }

    old_SeedChooserScreen_ButtonDepress(this, theId);
}

void SeedChooserScreen::GetSeedPositionInBank(int theIndex, int &x, int &y, int thePlayerIndex) {
    // 修复对战选卡时的错位
    if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS) {
        if (!mIsZombieChooser) {
            x = mBoard->mSeedBank[0]->mX - mX + mBoard->GetSeedPacketPositionX(theIndex, 0, mIsZombieChooser);
            y = mBoard->mSeedBank[0]->mY - mY + 8;
        } else {
            x = mBoard->mSeedBank[1]->mX - mX + mBoard->GetSeedPacketPositionX(theIndex, 0, mIsZombieChooser);
            y = mBoard->mSeedBank[1]->mY - mY + 8;
        }
        return;
    }

    old_SeedChooserScreen_GetSeedPositionInBank(this, theIndex, x, y, thePlayerIndex);
}

void SeedChooserScreen::GetSeedPositionInChooser(int theIndex, int &x, int &y) {
    if (!mIsZombieChooser && theIndex == SeedType::SEED_IMITATER) {
        x = mImitaterButton->mX;
        y = mImitaterButton->mY;
        return;
    }
    int aRow = theIndex / NumColumns();
    int aCol = theIndex % NumColumns();
    bool isExtraSeedsMode = gVSSetupAddonWidget && gVSSetupAddonWidget->mExtraSeedsMode;
    if (mIsZombieChooser && aRow == 3 && !isExtraSeedsMode) {
        x = 53 * aCol + 48;
    } else {
        x = 53 * aCol + 22;
    }
    if (!mIsZombieChooser && Has7Rows()) {
        y = 70 * aRow + 123;
    } else {
        y = 73 * aRow + 128;
    }
}

int SeedChooserScreen::NumColumns() {
    return mIsZombieChooser ? 5 : 8;
}


void SeedChooserScreen::ShowToolTip(unsigned int thePlayerIndex) {
    old_SeedChooserScreen_ShowToolTip(this, thePlayerIndex);

    bool aIsPlayer2 = thePlayerIndex == 1;
    ToolTipWidget *aTolTip = aIsPlayer2 ? mToolTip2 : mToolTip1;

    if (mApp->IsVSMode()) {
        int aGamepadIndex = mApp->PlayerToGamepadIndex(thePlayerIndex);
        int x = (aGamepadIndex == 1) ? mCursorPositionX2 : mCursorPositionX1;
        int y = (aGamepadIndex == 1) ? mCursorPositionY2 : mCursorPositionY1;
        SeedType aSeedType = SeedHitTest(x, y);
        for (int i = 0; i < NUM_ZOMBIE_SEED_TYPES; ++i) {
            if (gVSSetupAddonWidget && aSeedType == gVSSetupAddonWidget->mBannedSeed[i].mSeedType) {
                aTolTip->SetWarningText("本轮已禁用");
            }
        }

        if (mIsZombieChooser) {
            if (mChosenSeeds[aSeedType - SeedType::SEED_ZOMBIE_GRAVESTONE].mSeedState == ChosenSeedState::SEED_IN_BANK && mChosenSeeds[aSeedType - SeedType::SEED_ZOMBIE_GRAVESTONE].mCrazyDavePicked) {
                bool seedIsGrave = mToolTipSeed1 == SeedType::SEED_ZOMBIE_GRAVESTONE || mToolTipSeed2 == SeedType::SEED_ZOMBIE_GRAVESTONE;
                pvzstl::string str = seedIsGrave ? TodStringTranslate("[ZOMBIE_BOSS_WANTS]") : "";
                aTolTip->SetWarningText(str);
            }
            // 对战显示隐藏僵尸卡信息
            if (aSeedType > SeedType::SEED_ZOMBIE_GARGANTUAR && aSeedType < SeedType::NUM_ZOMBIE_SEED_IN_CHOOSER) {
                pvzstl::string aTitle, aLabel;
                switch (aSeedType) {
                        // case SeedType::SEED_ZOMBIE_REDEYE_GARGANTUAR: // 红眼巨人僵尸
                        // aTitle = TodStringTranslate("[REDEYE_GARGANTUAR_ZOMBIE]");
                        // aLabel = TodStringTranslate("[REDEYE_GARGANTUAR_ZOMBIE_DESCRIPTION_HEADER]");
                        // break;
                    case SeedType::SEED_ZOMBIE_REDEYE_GARGANTUAR: // 红眼巨人僵尸
                        aTitle = TodStringTranslate("[REDEYE_GARGANTUAR_ZOMBIE]");
                        aLabel = TodStringTranslate("[REDEYE_GARGANTUAR_ZOMBIE_DESCRIPTION_HEADER]");
                        break;
                    case SeedType::SEED_ZOMBIE_PEA_HEAD: // 豌豆射手僵尸
                        aTitle = TodStringTranslate("[PEA_HEAD_ZOMBIE]");
                        aLabel = TodStringTranslate("[PEA_HEAD_ZOMBIE_DESCRIPTION_HEADER]");
                        break;
                    case SeedType::SEED_ZOMBIE_WALLNUT_HEAD: // 坚果僵尸
                        aTitle = TodStringTranslate("[WALLNUT_HEAD_ZOMBIE]");
                        aLabel = TodStringTranslate("[WALLNUT_HEAD_ZOMBIE_DESCRIPTION_HEADER]");
                        break;
                    case SeedType::SEED_ZOMBIE_JALAPENO_HEAD: // 火爆辣椒僵尸
                        aTitle = TodStringTranslate("[JALAPENO_HEAD_ZOMBIE]");
                        aLabel = TodStringTranslate("[JALAPENO_HEAD_ZOMBIE_DESCRIPTION_HEADER]");
                        break;
                    case SeedType::SEED_ZOMBIE_GATLINGPEA_HEAD: // 机枪射手僵尸
                        aTitle = TodStringTranslate("[GATLING_HEAD_ZOMBIE]");
                        aLabel = TodStringTranslate("[GATLING_HEAD_ZOMBIE_DESCRIPTION_HEADER]");
                        break;
                    case SeedType::SEED_ZOMBIE_SQUASH_HEAD: // 窝瓜僵尸
                        aTitle = TodStringTranslate("[SQUASH_HEAD_ZOMBIE]");
                        aLabel = TodStringTranslate("[SQUASH_HEAD_ZOMBIE_DESCRIPTION_HEADER]");
                        break;
                    case SeedType::SEED_ZOMBIE_TALLNUT_HEAD: // 高坚果僵尸
                    {
                        aTitle = TodStringTranslate("[TALLNUT_HEAD_ZOMBIE]");
                        aLabel = TodStringTranslate("[TALLNUT_HEAD_ZOMBIE_DESCRIPTION_HEADER]");
                        int aSeedX, aSeedY;
                        SeedType aZombieSeedType = GetZombieIndexBySeedType(aSeedType);
                        GetSeedPositionInChooser(aZombieSeedType, aSeedX, aSeedY);
                        aTolTip->mX = aSeedX + 2 * (SEED_PACKET_WIDTH + 6);
                    } break;
                    default:
                        return;
                }
                aTolTip->SetTitle(aTitle);
                aTolTip->SetLabel(aLabel);
            }
        } else {
            if (gVSSetupAddonWidget && gVSSetupAddonWidget->mBanMode) {
                if (mChosenSeeds[aSeedType].mSeedState == ChosenSeedState::SEED_IN_CHOOSER) {
                    bool aCanNotBanned = mToolTipSeed1 == SeedType::SEED_INSTANT_COFFEE || mToolTipSeed2 == SeedType::SEED_INSTANT_COFFEE;
                    pvzstl::string str = aCanNotBanned ? "此阶段不允许" : "";
                    aTolTip->SetWarningText(str);
                }
            }
        }
    }
}

SeedType SeedChooserScreen::GetZombieIndexBySeedType(SeedType theSeedType) {
    return theSeedType - SEED_ZOMBIE_GRAVESTONE < 0 ? SeedType::SEED_NONE : (SeedType)(theSeedType - SEED_ZOMBIE_GRAVESTONE);
}

void SeedChooserScreen::MouseMove(int x, int y) {
    SeedType aSeedType = SeedHitTest(x, y);
    // 该函数探测不到模仿者位置
    if (aSeedType == SeedType::SEED_NONE) {
        return;
    }

    if (mIsZombieChooser) {
        if (aSeedType >= SeedType::NUM_ZOMBIE_SEED_IN_CHOOSER)
            return;

        SeedType aZombieSeedType = GetZombieIndexBySeedType(aSeedType);
        GetSeedPositionInChooser(aZombieSeedType, mCursorPositionX1, mCursorPositionX1);
        GetSeedPositionInChooser(aZombieSeedType, mCursorPositionX2, mCursorPositionY2);
        mSeedType2 = aZombieSeedType;
    } else if (m1PChoosingSeeds) {
        if (mApp->IsVSMode() && aSeedType > SeedType::SEED_MELONPULT)
            return;

        if (mApp->IsVSMode()) {
            GetSeedPositionInChooser(aSeedType, mCursorPositionX1, mCursorPositionX1);
            GetSeedPositionInChooser(aSeedType, mCursorPositionX2, mCursorPositionY2);
        } else {
            GetSeedPositionInChooser(aSeedType, mCursorPositionX1, mCursorPositionY1);
        }
        mSeedType1 = aSeedType;
    } else {
        GetSeedPositionInChooser(aSeedType, mCursorPositionX2, mCursorPositionY2);
        mSeedType2 = aSeedType;
    }
}

void SeedChooserScreen::MouseDown(int x, int y, int theClickCount) {
    m1PChoosingSeeds = !mApp->IsCoopMode() || mSeedsIn1PBank < 4;

    bool mViewLawnButtonDisabled = mViewLawnButton == nullptr || !mBoard->mCutScene->IsSurvivalRepick();
    bool mStoreButtonDisabled = mStoreButton == nullptr || mStoreButton->mDisabled;
    bool mStartButtonDisabled = mStartButton == nullptr || mStartButton->mDisabled;
    bool mAlmanacButtonDisabled = mAlmanacButton == nullptr || mAlmanacButton->mDisabled;

    if (!mViewLawnButtonDisabled) { // !mDisabled
        Sexy::Rect mViewLawnButtonRect = {mViewLawnButton->mX, mViewLawnButton->mY, mViewLawnButton->mWidth, 50};
        // LOGD("mStoreButtonRect:%d %d %d %d",mStoreButtonRect[0],mStoreButtonRect[1],mStoreButtonRect[2],mStoreButtonRect[3]);
        if (TRect_Contains(&mViewLawnButtonRect, x, y)) {
            mApp->PlaySample(*Sexy_SOUND_TAP_Addr);
            gSeedChooserTouchState = SeedChooserTouchState::ViewLawnButton;
            // GameButtonDown(seedChooserScreen, 8, 0);
            return;
        }
    }

    if (!mStoreButtonDisabled) { // !mDisabled
        Sexy::Rect mStoreButtonRect = {mStoreButton->mX, mStoreButton->mY, mStoreButton->mWidth, 50};
        // LOGD("mStoreButtonRect:%d %d %d %d",mStoreButtonRect[0],mStoreButtonRect[1],mStoreButtonRect[2],mStoreButtonRect[3]);
        if (TRect_Contains(&mStoreButtonRect, x, y)) {
            mApp->PlaySample(*Sexy_SOUND_TAP_Addr);
            gSeedChooserTouchState = SeedChooserTouchState::StoreButton;
            // GameButtonDown(seedChooserScreen, 8, 0);
            return;
        }
    }

    if (!mStartButtonDisabled) { // !mDisabled
        Sexy::Rect mStartButtonRect = {mStartButton->mX, mStartButton->mY, mStartButton->mWidth, 50};
        if (TRect_Contains(&mStartButtonRect, x, y)) {
            mApp->PlaySample(*Sexy_SOUND_TAP_Addr);
            gSeedChooserTouchState = SeedChooserTouchState::StartButton;

            // SeedChooserScreen_OnStartButton(seedChooserScreen);
            return;
        }
    }

    if (!mAlmanacButtonDisabled) { // !mDisabled
        Sexy::Rect mAlmanacButtonRect = {mAlmanacButton->mX, mAlmanacButton->mY, mAlmanacButton->mWidth, 50};
        if (TRect_Contains(&mAlmanacButtonRect, x, y)) {
            mApp->PlaySample(*Sexy_SOUND_TAP_Addr);
            gSeedChooserTouchState = SeedChooserTouchState::AlmanacButton;

            // GameButtonDown(seedChooserScreen, 9, 0);
            return;
        }
    }

    if (HasPacket(SeedType::SEED_IMITATER, false) && !mApp->IsVSMode() && !mIsZombieChooser) {
        int mImitaterPositionX = 0;
        int mImitaterPositionY = 0;
        GetSeedPositionInChooser(SeedType::SEED_IMITATER, mImitaterPositionX, mImitaterPositionY);
        Sexy::Rect mImitaterPositionRect = {mImitaterPositionX, mImitaterPositionY, SEED_PACKET_WIDTH, SEED_PACKET_HEIGHT};
        if (TRect_Contains(&mImitaterPositionRect, x, y)) {
            if (m1PChoosingSeeds) {
                mCursorPositionX1 = mImitaterPositionX;
                mCursorPositionY1 = mImitaterPositionY;
                mSeedType1 = SeedType::SEED_IMITATER;
            } else {
                mCursorPositionX2 = mImitaterPositionX;
                mCursorPositionY2 = mImitaterPositionY;
                mSeedType2 = SeedType::SEED_IMITATER;
            }
            GameButtonDown(BUTTONCODE_A, !m1PChoosingSeeds);
            return;
        }
    }
    SeedType aSeedType = SeedHitTest(x, y);
    // 该函数探测不到模仿者位置

    if (aSeedType == SeedType::SEED_NONE) {
        return;
    }

    if (!mIsZombieChooser && (mChosenSeeds[aSeedType].mSeedState == ChosenSeedState::SEED_FLYING_TO_BANK || mChosenSeeds[aSeedType].mSeedState == ChosenSeedState::SEED_FLYING_TO_CHOOSER)) {
        return;
    }

    if (mIsZombieChooser) {
        if (aSeedType >= SeedType::NUM_ZOMBIE_SEED_IN_CHOOSER)
            return;

        SeedType aZombieSeedType = GetZombieIndexBySeedType(aSeedType);
        GetSeedPositionInChooser(aZombieSeedType, mCursorPositionX1, mCursorPositionY1);
        GetSeedPositionInChooser(aZombieSeedType, mCursorPositionX2, mCursorPositionY2);
        mSeedType2 = aZombieSeedType;
    } else if (m1PChoosingSeeds) {
        if (mApp->IsVSMode() && aSeedType > SeedType::SEED_MELONPULT)
            return;

        if (mApp->IsVSMode()) {
            GetSeedPositionInChooser(aSeedType, mCursorPositionX1, mCursorPositionY1);
            GetSeedPositionInChooser(aSeedType, mCursorPositionX2, mCursorPositionY2);
        } else {
            GetSeedPositionInChooser(aSeedType, mCursorPositionX1, mCursorPositionY1);
        }
        mSeedType1 = aSeedType;
    } else {
        GetSeedPositionInChooser(aSeedType, mCursorPositionX2, mCursorPositionY2);
        mSeedType2 = aSeedType;
    }
    gSeedChooserTouchState = SeedChooserTouchState::SeedChooser;
}

void SeedChooserScreen::MouseDrag(int x, int y) {
    if (gSeedChooserTouchState == SeedChooserTouchState::SeedChooser) {
        SeedType aSeedType = SeedHitTest(x, y);
        // 该函数探测不到模仿者位置
        if (aSeedType == SeedType::SEED_NONE) {
            return;
        }
        if (mIsZombieChooser) {
            if (aSeedType >= SeedType::NUM_ZOMBIE_SEED_IN_CHOOSER)
                return;

            SeedType aZombieSeedType = GetZombieIndexBySeedType(aSeedType);
            GetSeedPositionInChooser(aZombieSeedType, mCursorPositionX1, mCursorPositionY1);
            GetSeedPositionInChooser(aZombieSeedType, mCursorPositionX2, mCursorPositionY2);
            mSeedType2 = aZombieSeedType;
        } else if (m1PChoosingSeeds) {
            if (mApp->IsVSMode() && aSeedType > SeedType::SEED_MELONPULT)
                return;

            if (mApp->IsVSMode()) {
                GetSeedPositionInChooser(aSeedType, mCursorPositionX1, mCursorPositionY1);
                GetSeedPositionInChooser(aSeedType, mCursorPositionX2, mCursorPositionY2);
            } else {
                GetSeedPositionInChooser(aSeedType, mCursorPositionX1, mCursorPositionY1);
            }
            mSeedType1 = aSeedType;
        } else {
            GetSeedPositionInChooser(aSeedType, mCursorPositionX2, mCursorPositionY2);
            mSeedType2 = aSeedType;
        }
    }
}

void SeedChooserScreen::MouseUp(int x, int y) {

    if (tcp_connected && mApp->mGameMode == GAMEMODE_MP_VS) {
        U8U8_Event event = {{EventType::EVENT_SEEDCHOOSER_SELECT_SEED}, uint8_t(mIsZombieChooser ? mSeedType2 : mSeedType1), mIsZombieChooser};
        sendWithSize(tcpServerSocket, &event, sizeof(U8U8_Event), 0);
        return;
    }

    switch (gSeedChooserTouchState) {
        case SeedChooserTouchState::ViewLawnButton:
            ButtonDepress(SeedChooserScreen_ViewLawn);
            break;
        case SeedChooserTouchState::SeedChooser:
            if (!mIsZombieChooser && m1PChoosingSeeds && mApp->IsCoopMode()) {
                GameButtonDown(BUTTONCODE_A, 0);
            } else {
                GameButtonDown(BUTTONCODE_A, 1);
            }
            break;
        case SeedChooserTouchState::StoreButton:
            ButtonDepress(SeedChooserScreen_Store);
            break;
        case SeedChooserTouchState::StartButton:
            ButtonDepress(SeedChooserScreen_Start);
            break;
        case SeedChooserTouchState::AlmanacButton:
            ButtonDepress(SeedChooserScreen_Almanac);
            break;
        default:
            break;
    }
    gSeedChooserTouchState = SeedChooserTouchState::SEEDCHOOSER_TOUCHSTATE_NONE;
}

int SeedChooserScreen::GetNextSeedInDir(int theNumSeed, SeedDir theMoveDirection) {
    bool isExtraSeedsMode = gVSSetupAddonWidget && gVSSetupAddonWidget->mExtraSeedsMode;
    if (mIsZombieChooser && !isExtraSeedsMode) {
        // 右下角边缘
        if ((theNumSeed == 14 && theMoveDirection == SeedDir::SEED_DIR_DOWN) || //
            (theNumSeed == 18 && theMoveDirection == SeedDir::SEED_DIR_RIGHT)) {
            return 18;
        }
    }

    const int aNumCol = NumColumns();
    int aRow;
    int aCol;
    if (theNumSeed == SeedType::SEED_IMITATER) {
        aCol = 8;
        aRow = 5;
    } else {
        aRow = theNumSeed / aNumCol;
        aCol = theNumSeed % aNumCol;
    }

    switch (theMoveDirection) {
        case SeedDir::SEED_DIR_UP:
            if (aRow > 0) {
                --aRow;
            }
            break;
        case SeedDir::SEED_DIR_DOWN: {
            int aMaxRow = mIsZombieChooser ? (isExtraSeedsMode ? 5 : 3) // 拓展僵尸选卡适配键盘选取
                                           : (Has7Rows() ? 5 : 4);
            if (aRow < aMaxRow) {
                ++aRow;
            }
        } break;
        case SeedDir::SEED_DIR_LEFT:
            if (aCol > 0) {
                --aCol;
            }
            break;
        case SeedDir::SEED_DIR_RIGHT:
            if (aCol < aNumCol - 1) {
                ++aCol;
            }
            break;
        default:
            break;
    }
    int aNextSeed = aCol + aNumCol * aRow;
    return aNextSeed;
}

void SeedChooserScreen::Draw(Graphics *g) {
    // Early returns for dialogs
    if (mApp->GetDialog(DIALOG_STORE) || mApp->GetDialog(DIALOG_ALMANAC))
        return;

    g->SetLinearBlend(true);

    if (!mBoard->ChooseSeedsOnCurrentLevel() || (mBoard->mCutScene && mBoard->mCutScene->IsBeforePreloading()))
        return;

    // Setup base color
    Color aBaseColor(255, 255, 255);

    // Handle two-player mode dimming
    if (mApp->IsVSMode() && !CanPickNow()) {
        float aDimAmount = TodAnimateCurveFloat(0, 25, mDimCounter, 1.0f, 0.45f, CURVE_EASE_IN_OUT);
        g->SetColorizeImages(true);
        aBaseColor = Color((int)(aDimAmount * 255.0f), (int)(aDimAmount * 255.0f), (int)(aDimAmount * 255.0f));
        g->SetColor(aBaseColor);
    }

    // Draw background
    Image *aBackgroundImage = mIsZombieChooser ? *Sexy::IMAGE_SEEDCHOOSER_BACKGROUND2 : *Sexy::IMAGE_SEEDCHOOSER_BACKGROUND;
    g->DrawImage(aBackgroundImage, 0, 87);

    // Draw imitater addon for plant chooser
    if (!mIsZombieChooser && HasPacket(SEED_IMITATER, false) && !mApp->IsVSMode()) {
        g->DrawImage(*Sexy::IMAGE_SEEDCHOOSER_IMITATERADDON, mImitaterButton->mX - 5, mImitaterButton->mY - 12);
    }

    // Draw title text
    Color aTitleColor;
    pvzstl::string aTitleText;
    if (mIsZombieChooser) {
        aTitleColor = Color(0, 255, 0);
        aTitleText = "[CHOOSE_YOUR_ZOMBIES]";
    } else {
        aTitleColor = Color(213, 159, 43);
        aTitleText = "[CHOOSE_YOUR_PLANTS]";
    }

    pvzstl::string aTitleString = TodStringTranslate(aTitleText.c_str());
    TodDrawString(g, aTitleString, aBackgroundImage->mWidth / 2, 114, *Sexy::FONT_DWARVENTODCRAFT18, aTitleColor, DS_ALIGN_CENTER);

    bool isExtraSeedsMode = gVSSetupAddonWidget && gVSSetupAddonWidget->mExtraSeedsMode;

    // Calculate seed count
    int aNumSeeds = isExtraSeedsMode ? NUM_ZOMBIE_SEED_IN_CHOOSER - SEED_ZOMBIE_GRAVESTONE : 19;
    if (!mIsZombieChooser) {
        if (mApp->IsVSMode() || !Has7Rows())
            aNumSeeds = 40;
        else if (HasPacket(SEED_IMITATER, false))
            aNumSeeds = 49;
        else
            aNumSeeds = 48;
    }

    // Draw seed packet shadows (two passes)
    for (int aPass = 0; aPass < 2; aPass++) {
        bool aDrawShadow = (aPass == 0);

        for (SeedType aSeedShadow = SEED_PEASHOOTER; aSeedShadow < aNumSeeds; aSeedShadow = SeedType(aSeedShadow + 1)) {
            int x, y;
            GetSeedPositionInChooser(aSeedShadow, x, y);

            SeedType aDisplaySeedType = aSeedShadow;
            if (mIsZombieChooser)
                aDisplaySeedType = GetZombieSeedType(aSeedShadow);

            if (aDisplaySeedType == SEED_IMITATER)
                continue;

            if (aDisplaySeedType == SEED_NONE || !HasPacket(aDisplaySeedType, mIsZombieChooser)) {
                if (aDrawShadow)
                    g->DrawImage(*Sexy::IMAGE_SEEDPACKETSILHOUETTE, x, y);
            } else {
                ChosenSeed &aChosenSeed = mChosenSeeds[aSeedShadow];
                if (aChosenSeed.mSeedState != SEED_IN_CHOOSER) {
                    // Determine grayness based on selection state
                    int aGrayness = 55;
                    //                    if (mSeedType1 == aSeedShadow || mSeedType2 == aSeedShadow)
                    //                        aGrayness = 55;
                    //                    else
                    //                        aGrayness = 255;

                    DrawPacket(g, x, y, aDisplaySeedType, SEED_NONE, 0.0f, aGrayness, &aBaseColor, true, true);
                }
            }
        }
    }

    // Draw empty seed bank slots
    int aNumSeedsInBank = mSeedBank1->mNumPackets;
    for (int anIndex = 0; anIndex < aNumSeedsInBank; anIndex++) {
        if (FindSeedInBank(anIndex, false) == SEED_NONE) {
            int x, y;
            GetSeedPositionInBank(anIndex, x, y, 0);
            g->DrawImage(*Sexy::IMAGE_SEEDPACKETSILHOUETTE, x, y);
        }
    }

    // Draw coop mode second bank slots
    if (mApp->IsCoopMode() && mSeedBank2) {
        for (int anIndex = 0; anIndex < aNumSeedsInBank; anIndex++) {
            if (FindSeedInBank(anIndex, true) == SEED_NONE) {
                int x, y;
                GetSeedPositionInBank(anIndex, x, y, 1);
                g->DrawImage(*Sexy::IMAGE_SEEDPACKETSILHOUETTE, x, y);
            }
        }
    }

    // Draw seeds in chooser and bank
    for (SeedType aSeedType = SEED_PEASHOOTER; aSeedType < NUM_SEEDS_IN_CHOOSER; aSeedType = SeedType(aSeedType + 1)) {
        SeedType aDisplaySeedType = aSeedType;
        if (mIsZombieChooser)
            aDisplaySeedType = GetZombieSeedType(aSeedType);

        if (!HasPacket(aDisplaySeedType, mIsZombieChooser))
            continue;

        if (aDisplaySeedType == SEED_NONE || aSeedType >= aNumSeeds)
            continue;

        ChosenSeed &aChosenSeed = mChosenSeeds[aSeedType];
        ChosenSeedState aSeedState = aChosenSeed.mSeedState;

        if (aSeedState == SEED_FLYING_TO_BANK || aSeedState == SEED_FLYING_TO_CHOOSER || aSeedState == SEED_PACKET_HIDDEN)
            continue;

        if (aSeedState != SEED_IN_CHOOSER && !mBoard->mCutScene->mSeedChoosing)
            continue;

        // Calculate position
        int aPosX = aChosenSeed.mX;
        int aPosY = aChosenSeed.mY;

        if (aSeedState == SEED_IN_BANK) {
            GetSeedPositionInBank(aChosenSeed.mSeedIndexInBank, aPosX, aPosY, aChosenSeed.mChosenPlayerIndex);
            aChosenSeed.mX = aPosX;
            aChosenSeed.mY = aPosY;
        }

        // Determine grayness
        int aGrayness = 255;
        bool aIsGrayed = false;

        if (!mIsZombieChooser) {
            if (aSeedState == SEED_IN_CHOOSER) {
                if (SeedNotRecommendedToPick(aChosenSeed.mSeedType) || SeedNotAllowedToPick(aChosenSeed.mSeedType)) {
                    aIsGrayed = true;
                }
            }

            if (SeedNotAllowedDuringTrial(aChosenSeed.mSeedType))
                aIsGrayed = true;
        }

        if (aIsGrayed)
            aGrayness = 115;

        // Check if being dragged
        if (mSeedType1 == aSeedType && mBoard->mGamepadControls1->mPlayerIndex1 != -1 && aSeedState == SEED_IN_CHOOSER) {
            mSeedType1 = aSeedType;
            if (aIsGrayed)
                aGrayness = 115;
            else
                aGrayness = 255;
        }

        if (mSeedType2 == aSeedType && mBoard->mGamepadControls2->mPlayerIndex2 != -1 && aSeedState == SEED_IN_CHOOSER) {
            mSeedType2 = aSeedType;
            if (aIsGrayed)
                aGrayness = 115;
            else
                aGrayness = 255;
        }

        DrawPacket(g, aPosX, aPosY, aChosenSeed.mSeedType, aChosenSeed.mImitaterType, 0.0f, aGrayness, &aBaseColor, true, true);
    }

    // Draw imitater button
    if (!mIsZombieChooser && !mApp->IsVSMode()) {
        g->Translate(mImitaterButton->mX, mImitaterButton->mY);
        mImitaterButton->Draw(g);
        g->Translate(-mImitaterButton->mX, -mImitaterButton->mY);
    }

    int aGamepadIndex = mApp->PlayerToGamepadIndex(mPlayerIndex);
    int aCursorX = aGamepadIndex ? mCursorPositionX2 : mCursorPositionX1;
    int aCursorY = aGamepadIndex ? mCursorPositionY2 : mCursorPositionY1;

    // Draw cursor selectors for two players
    for (int aPlayerIndex = 0; aPlayerIndex < 2; aPlayerIndex++) {
        int aPlayerState = (aPlayerIndex ? mBoard->mGamepadControls2 : mBoard->mGamepadControls1)->mPlayerIndex2;
        if (aPlayerState != -1 && !unkMems3[3]) {
            if (aPlayerState == mPlayerIndex || !mApp->IsVSMode()) {
                Image *aSelectorImage = (aPlayerState == mApp->mTwoPlayerState) ? *Sexy::IMAGE_SEED_SELECTOR_BLUE : *Sexy::IMAGE_SEED_SELECTOR;

                g->DrawImage(aSelectorImage, aCursorX - 8, aCursorY - 4, 64, 85);
            }
        }
    }

    // Draw dragging seeds for player 1
    if (mSeedType1 != SEED_NONE && ShouldDisplayCursor(0)) {
        int x, y;
        GetSeedPositionInChooser(mSeedType1, x, y);
        SeedType aDisplaySeedType = mIsZombieChooser ? GetZombieSeedType(mSeedType1) : mSeedType1;
        ChosenSeed &aChosenSeed = mChosenSeeds[mSeedType1];
        int aGrayness = 255;
        if (aChosenSeed.mSeedState != SEED_IN_CHOOSER)
            aGrayness = 55;
        if (((SeedNotRecommendedToPick(mSeedType1) || SeedNotAllowedToPick(mSeedType1)) && aChosenSeed.mSeedState == SEED_IN_CHOOSER) || SeedNotAllowedDuringTrial(mSeedType1))
            aGrayness = 115;

        DrawPacket(g, x, y + 5, aDisplaySeedType, SEED_NONE, 0.0f, aGrayness, &aBaseColor, true, true);
    }

    // Draw dragging seeds for player 2
    if (mSeedType2 != SEED_NONE && ShouldDisplayCursor(1)) {
        int x, y;
        GetSeedPositionInChooser(mSeedType2, x, y);
        SeedType aDisplaySeedType = mIsZombieChooser ? GetZombieSeedType(mSeedType2) : mSeedType2;
        ChosenSeed &aChosenSeed = mChosenSeeds[mSeedType2];
        int aGrayness = 255;
        if (aChosenSeed.mSeedState != SEED_IN_CHOOSER)
            aGrayness = 55;
        if (((SeedNotRecommendedToPick(mSeedType2) || SeedNotAllowedToPick(mSeedType2)) && aChosenSeed.mSeedState == SEED_IN_CHOOSER) || SeedNotAllowedDuringTrial(mSeedType2))
            aGrayness = 115;
        DrawPacket(g, x, y + 5, aDisplaySeedType, SEED_NONE, 0.0f, aGrayness, &aBaseColor, true, true);
    }

    // Draw cursor arrows for players
    for (int aPlayerIndex = 0; aPlayerIndex < 2; aPlayerIndex++) {
        if (ShouldDisplayCursor(aPlayerIndex) && (aPlayerIndex ? mBoard->mGamepadControls2 : mBoard->mGamepadControls1)->mPlayerIndex2 != -1) {
            Image *aArrowImage = aPlayerIndex ? *Sexy::IMAGE_CURSOR_ARROW_P2 : *Sexy::IMAGE_CURSOR_ARROW_P1;
            Image *aTextImage = aPlayerIndex ? *Sexy::IMAGE_CURSOR_P2_TEXT : *Sexy::IMAGE_CURSOR_P1_TEXT;

            float aBounce = sinf(unkF * 5.0f) * 2.0f;

            g->DrawImageF(aArrowImage, (float)(aCursorX + 25 - aArrowImage->mWidth / 2), (float)(aCursorY - 8) + aBounce);
            g->DrawImageF(aTextImage, (float)(aCursorX + 25 - aTextImage->mWidth / 2), (float)(aCursorY - 32));
        }
    }

    // Draw flying seed packets
    for (SeedType aSeedType = SEED_PEASHOOTER; aSeedType < NUM_SEEDS_IN_CHOOSER; aSeedType = SeedType(aSeedType + 1)) {
        SeedType aDisplaySeedType = aSeedType;
        if (mIsZombieChooser)
            aDisplaySeedType = GetZombieSeedType(aSeedType);

        if (!HasPacket(aDisplaySeedType, mIsZombieChooser))
            continue;

        ChosenSeed &aChosenSeed = mChosenSeeds[aSeedType];
        ChosenSeedState aSeedState = aChosenSeed.mSeedState;

        if (aSeedState == SEED_FLYING_TO_BANK || aSeedState == SEED_FLYING_TO_CHOOSER) {
            DrawPacket(g, aChosenSeed.mX, aChosenSeed.mY, aChosenSeed.mSeedType, aChosenSeed.mImitaterType, 0.0f, 255, &aBaseColor, true, true);
        }
    }

    // Draw UI widgets
    GamepadApp *aGamepadApp = reinterpret_cast<GamepadApp *>(mApp);
    bool aHasGamepad = aGamepadApp->HasGamepad();
    if (!aHasGamepad && (!mApp->mGamePad1IsOn || !mApp->mGamePad2IsOn)) {
        // Draw button widgets
        for (size_t i = 0; i < 4; i++) {
            GameButton *aButton[4] = {mViewLawnButton, mStoreButton, mStartButton, mAlmanacButton};
            if (aButton[i] && aButton[i]->mVisible) {
                g->Translate(aButton[i]->mX, aButton[i]->mY);
                aButton[i]->Draw(g);
                g->Translate(-aButton[i]->mX, -aButton[i]->mY);
            }
        }
    } else if (mShowHelpText && !mApp->IsVSMode()) {
        // Draw help text with flashing effect
        int aFlashPhase = mSeedChooserAge % 100;
        pvzstl::string aHelpText = TodStringTranslate("[HELP_TEXT_2_START]");
        int aTextX = aBackgroundImage->mWidth / 2;
        int aTextY = aBackgroundImage->mHeight - 63;

        Color aTextColor;
        if (aFlashPhase <= 50)
            aTextColor = Color(127, 127, 127, 255);
        else
            aTextColor = Color::White;

        TodDrawString(g, aHelpText, aTextX, aTextY, *Sexy::FONT_DWARVENTODCRAFT24, aTextColor, DS_ALIGN_CENTER);
    }
    //    else {
    //        // Check for disconnected controller warning
    //        int aTwoPlayerState = mApp->mTwoPlayerState;
    //        if (aTwoPlayerState != -1 && aTwoPlayerState == mPlayerIndex) {
    //            // if (mBoard->mGamepadControls[aTwoPlayerState] &&
    //            //     !mBoard->mGamepadControls[aTwoPlayerState]->mControllerConnected)
    //            // { // 这是AI给出的结果，很显然还原是对的，但是实际上没有这个成员 | 故此还原TV伪C的判断
    //
    //            if (!*(bool *)(mApp->unkMem6[aTwoPlayerState + 135] + 412)) {
    //
    //                // Warninig警告: 不得简化sDisconnectTimer这个变量，更不能删除static字样！
    //                static int sDisconnectTimer = 0;
    //                sDisconnectTimer++;
    //                /* 这一段看的我很迷糊，首先是mGamepadControls的判断，TV的伪C是判断!mApp->Unk6[aTwoPlayerState + 139], PSV又是调用函数判断成立
    //                        经过我的分析，PSV调用的是一个判断控制器的状态的函数，有以下返回值: 0已连接，1167控制器未连接，TV很有可能函数已经被阉割了，不过我有空看看1.0.1的ida
    //
    //                   还有sDisconnectTimer这个变量，在PSV与TV中都是全局变量，在进行这一步时会进行X++。
    //                        在这里我就不声明为全局变量了，声明为一个函数内静态变量(相当于全局变量但是只有此作用域可使用)*/
    //
    //                int aSeconds = (sDisconnectTimer / 60) % 60;
    //                if (aSeconds > 30) {
    //                    pvzstl::string aWarningText = TodStringTranslate("[RECONNECT_SECOND_CONTROLLER_FMT]");
    //                    aWarningText = StrFormat(aWarningText.c_str(), aTwoPlayerState + 1); // 此处的StrFormat在TV中传入2，PSV则是mApp->mTwoPlayerState
    //
    //                    int aTextX = aBackgroundImage->mWidth / 2;
    //                    int aTextY = aBackgroundImage->mHeight - 63;
    //                    Color aWarningColor(255, 0, 0);
    //
    //                    TodDrawString(g, aWarningText, aTextX, aTextY, *Sexy::FONT_DWARVENTODCRAFT24, aWarningColor, DS_ALIGN_CENTER);
    //                }
    //            }
    //        }
    //    }

    this->Widget::DeferOverlay(0);

    // 绘制禁用叉叉
    DrawBanIcon(g);
}

void SeedChooserScreen::DrawBanIcon(Sexy::Graphics *g) {
    if (!gVSSetupAddonWidget)
        return;

    if (gVSSetupAddonWidget->mBanMode) {
        Graphics aBanGraphics(*g);
        aBanGraphics.mTransX = 0;
        aBanGraphics.mTransY = 0;
        aBanGraphics.SetColor(Color(205, 0, 0, 255));
        aBanGraphics.SetFont(*Sexy_FONT_DWARVENTODCRAFT18_Addr);
        aBanGraphics.DrawString(TodStringTranslate("[VS_UI_BAN_PHASE_BIG]"), 440, 110);
    }

    for (int i = 0; i < NUM_ZOMBIE_SEED_TYPES; i++) {
        if (gVSSetupAddonWidget->mBannedSeed[i].mSeedState == BannedSeedState::SEED_BANNED) {
            if ((mIsZombieChooser && gVSSetupAddonWidget->mBannedSeed[i].mChosenPlayerIndex == 1) || (!mIsZombieChooser && gVSSetupAddonWidget->mBannedSeed[i].mChosenPlayerIndex == 0)) {
                int x = gVSSetupAddonWidget->mBannedSeed[i].mX;
                int y = gVSSetupAddonWidget->mBannedSeed[i].mY;
                g->DrawImage(*IMAGE_MP_TARGETS_X, x + 5, y + 5);
            }
        }
    }
}

SeedType SeedChooserScreen::SeedHitTest(int x, int y) {
    return old_SeedChooserScreen_SeedHitTest(this, x, y);
}