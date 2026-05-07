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
#include "PvZ/NetPlay.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Common/TodStringFile.h"
#include <unistd.h>

using namespace Sexy;

namespace {
SeedChooserScreen *gSeedChooserTouchOwner = nullptr;
constexpr uint32_t kSeedChooserDragSyncIntervalMs = 50;
SeedType gLastDragSyncSeedType[2][2] = {{SeedType::SEED_NONE, SeedType::SEED_NONE}, {SeedType::SEED_NONE, SeedType::SEED_NONE}};
uint32_t gLastDragSyncTickMs[2][2] = {{0, 0}, {0, 0}};

bool IsLocalChooserInputAllowed(SeedChooserScreen *screen) {
    if (!screen->mApp->IsVSMode()) {
        return true;
    }

    VSSetupMenu *vsSetup = screen->mApp->mVSSetupMenu;
    if (vsSetup == nullptr) {
        return true;
    }

    VSSide chooserSide = screen->mIsZombieChooser ? VSSide::VS_SIDE_ZOMBIE : VSSide::VS_SIDE_PLANT;
    VSSide localSide = VSSide::VS_SIDE_NONE;
    if (gTcpConnected) {
        localSide = vsSetup->mSides[1];
    } else if (gTcpClientSocket >= 0) {
        localSide = vsSetup->mSides[0];
    } else {
        return true;
    }

    VSSide controlledSide = localSide;
    if (screen->mBanningPhase) {
        if (localSide == VSSide::VS_SIDE_PLANT) {
            controlledSide = VSSide::VS_SIDE_ZOMBIE;
        } else if (localSide == VSSide::VS_SIDE_ZOMBIE) {
            controlledSide = VSSide::VS_SIDE_PLANT;
        }
    }
    return controlledSide == chooserSide;
}

bool IsExtraSeedsModeEnabled(SeedChooserScreen *screen) {
    if (screen == nullptr || screen->mApp == nullptr || !screen->mApp->IsVSMode()) {
        return false;
    }

    VSSetupMenu *vsSetup = screen->mApp->mVSSetupMenu;
    if (vsSetup == nullptr || vsSetup->mAddonWidget == nullptr) {
        return false;
    }

    return vsSetup->mAddonWidget->mExtraSeedsMode;
}

inline void NormalizeLocalPoint(SeedChooserScreen *screen, int &x, int &y) {
    // Some platforms/reporting paths send global coordinates; normalize to widget-local.
    if (x < 0 || x >= screen->mWidth || y < 0 || y >= screen->mHeight) {
        x -= screen->mX;
        y -= screen->mY;
    }
}
} // namespace


void SeedChooserScreen::_constructor(bool theIsZombieChooser) {
    // 记录当前游戏状态，同时修复在没解锁商店图鉴时依然显示相应按钮的问题、对战选种子界面的按钮问题；
    // 还添加了生存模式保留上次选卡，添加坚果艺术关卡默认选择坚果，添加向日葵艺术关卡默认选择坚果、杨桃、萝卜伞
    mApp = reinterpret_cast<LawnApp *>(Sexy::gSexyAppBase);
    mBoard = mApp->mBoard;
    GameMode mGameMode = mApp->mGameMode;
    if (mBoard->mCutScene->IsSurvivalRepick() && !mApp->IsCoopMode()) {
        GamepadControls *gamePad = mBoard->mGamepadControls[0];
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

        mBanningPhase = mApp->mVSSetupMenu->mAddonWidget->mBanMode;
        mShowExtraSeeds = mApp->mVSSetupMenu->mAddonWidget->mExtraSeedsMode;
        mHas7Packets = mApp->mVSSetupMenu->mAddonWidget->mExtraPacketsMode;
        if (mIsZombieChooser && mShowExtraSeeds) {
            mPageButton = MakeNewButton(SeedChooserScreen::SeedChooserScreen_Page, this, this, "", nullptr, Sexy::IMAGE_ZEN_NEXTGARDEN, Sexy::IMAGE_ZEN_NEXTGARDEN, Sexy::IMAGE_ZEN_NEXTGARDEN);
            mPageButton->Resize(225, 525, 60, 60);
            AddWidget(mPageButton);
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
        gSeedChooserScreenMainMenuButton = MakeButton(104, this, this, "[MENU_BUTTON]");
        gSeedChooserScreenMainMenuButton->Resize(mApp->IsCoopMode() ? 345 : 650, -3, 120, 80);
        AddWidget(gSeedChooserScreenMainMenuButton);
    }
}

void SeedChooserScreen::_destructor() {
    old_SeedChooserScreen__destructor(this);

    delete mPageButton;
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
        mBoard->mSeedBank[1]->mSeedPackets[3].mPacketType = SeedType(mSeedIndex2);
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
        if (theSeedType == mBannedSeed[theSeedType].mSeedType) {
            return true;
        }
        if (theSeedType == SeedType::SEED_INSTANT_COFFEE) {
            if (mBoard->StageIsNight() || mBanningPhase)
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
            if (theSeedType == SeedType::SEED_ZOMBIE_SNORKEL || theSeedType == SeedType::SEED_ZOMBIE_DOLPHIN_RIDER) {
                return true;
            }
        }
        if (mBoard->StageHasRoof()) {
            if (theSeedType == SeedType::SEED_FLOWERPOT) {
                return false;
            }
            if (theSeedType == SeedType::SEED_ZOMBIE_DANCER || theSeedType == SeedType::SEED_ZOMBIE_DIGGER) {
                return true;
            }
        }
        if (mShowExtraSeeds && theSeedType == SeedType::SEED_BLOVER) {
            return false;
        }
    }

    return old_SeedChooserScreen_SeedNotAllowedToPick(this, theSeedType);
}

SeedType SeedChooserScreen::GetZombieSeedType(int theSeedIndex) {
    int aSeedType = theSeedIndex + SEED_ZOMBIE_GRAVESTONE;
    // 解锁更多对战僵尸
    // return aSeedType > SEED_ZOMBIE_GARGANTUAR ? SEED_NONE : SeedType(aSeedType);
    return aSeedType < NUM_ZOMBIE_SEED_IN_CHOOSER ? SeedType(aSeedType) : SEED_NONE;
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
    int selectedIndex = int(&theChosenSeed - mChosenSeeds);
    if (mApp->IsVSMode() && thePlayerIndex >= 0 && thePlayerIndex <= 1) {
        int cursorX = (thePlayerIndex == 0) ? mCursorPositionX1 : mCursorPositionX2;
        int cursorY = (thePlayerIndex == 0) ? mCursorPositionY1 : mCursorPositionY2;
        SeedType cursorSeedType = SeedHitTest(cursorX, cursorY);
        int cursorIndex = (cursorSeedType == SeedType::SEED_NONE) ? -1 : GetSeedPacketIndex(cursorSeedType);
        if (cursorIndex >= 0 && cursorIndex < NUM_SEEDS_IN_CHOOSER) {
            selectedIndex = cursorIndex;
        }
    }

    if (selectedIndex < 0 || selectedIndex >= NUM_SEEDS_IN_CHOOSER) {
        return;
    }

    ChosenSeed &selectedSeed = mChosenSeeds[selectedIndex];
    SeedType selectedSeedType = mIsZombieChooser ? GetZombieSeedType(selectedIndex) : SeedType(selectedIndex);
    if (selectedSeedType == SeedType::SEED_NONE || !HasPacket(selectedSeedType, mIsZombieChooser)) {
        return;
    }

    // Keep local chosen-seed payload coherent with index->type mapping.
    selectedSeed.mSeedType = selectedSeedType;

    if (mApp->IsVSMode()) {
        if (gTcpConnected) {
            // 客户端始终上报点击事件：即使选卡失败，也用于同步光标位置。
            U8x3_Event event = {{EventType::EVENT_CLIENT_SEEDCHOOSER_SELECT_SEED}, {uint8_t(selectedSeedType), uint8_t(mIsZombieChooser), 0}};
            netplay::PutEvent(event);
            return;
        } else if (gTcpClientSocket >= 0) {
            // 主机也广播该点击，远端可据此同步光标，再由主机权威决定是否入槽。
            U8x3_Event event = {{EventType::EVENT_SERVER_SEEDCHOOSER_SELECT_SEED}, {uint8_t(selectedSeedType), uint8_t(mIsZombieChooser), 0}};
            netplay::PutEvent(event);
        }

        if (mSeedsInBank == mSeedBank1->mNumPackets || !CanPickNow()) {
            return;
        }
    }

    ClickedSeedInChooser_Orgin(selectedSeed, thePlayerIndex);
}

void SeedChooserScreen::ClickedSeedInChooser_Orgin(ChosenSeed &theChosenSeed, int thePlayerIndex) {
    int chosenSeedIndex = int(&theChosenSeed - mChosenSeeds);
    if (chosenSeedIndex < 0 || chosenSeedIndex >= NUM_SEEDS_IN_CHOOSER) {
        return;
    }

    SeedType canonicalSeedType = mIsZombieChooser ? GetZombieSeedType(chosenSeedIndex) : SeedType(chosenSeedIndex);
    if (canonicalSeedType == SeedType::SEED_NONE || !HasPacket(canonicalSeedType, mIsZombieChooser)) {
        return;
    }
    theChosenSeed.mSeedType = canonicalSeedType;
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
        mApp->PlaySample(Sexy::SOUND_BUZZER);
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
    if (mApp->IsVSMode()) {
        if (mBanningPhase) { // 如果当前处于禁用阶段
            int x = (aGamepadIndex == 1) ? mCursorPositionX2 : mCursorPositionX1;
            int y = (aGamepadIndex == 1) ? mCursorPositionY2 : mCursorPositionY1;
            SeedType aSeedType = SeedHitTest(x, y);
            if (aSeedType != SEED_NONE && !SeedNotAllowedToPick(aSeedType)) {
                BannedSeed &aBannedSeed = mBannedSeed[aSeedType];
                aBannedSeed.mSeedType = theChosenSeed.mSeedType;

                int aSeedBanned = aBannedSeed.mSeedType;

                mBannedSeed[aSeedBanned].mX = theChosenSeed.mX;
                mBannedSeed[aSeedBanned].mY = theChosenSeed.mY;
                mBannedSeed[aSeedBanned].mSeedState = BannedSeedState::SEED_BANNED; // 将被选卡设为禁用状态

                mSeedsInBanned++; // 已禁用卡片数量 + 1
                if (!mIsZombieChooser) {
                    // 如果已禁用数量与需禁用数量一致，结束禁用阶段
                    if (mSeedsInBanned == mNumBanPackets) {
                        mApp->mSeedChooserScreen->mBanningPhase = false;
                        mApp->mZombieChooserScreen->mBanningPhase = false;
                    }
                }

                mApp->PlayFoley(FoleyType::FOLEY_FLOOP);
                OnPlayerPickedSeed(aGamepadIndex);
            }
            return;
        }
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
    if (mIsZombieChooser && mPageIndex == 1) {
        theChosenSeed.mStartY -= 5 * (SEED_PACKET_HEIGHT + 3);
    }

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
    mApp->PlaySample(Sexy::SOUND_TAP);

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

        // 当植物完成第三次选卡，开启第二轮禁用
        if (!mIsZombieChooser && mSeedsInBanned > 0 && mSeedsIn1PBank == 4) {
            // 需禁用数量增加1，额外卡槽模式则为2
            mApp->mSeedChooserScreen->mNumBanPackets += mHas7Packets ? 2 : 1;
            mApp->mZombieChooserScreen->mNumBanPackets += mHas7Packets ? 2 : 1;
            // 重新开启禁用阶段
            mApp->mSeedChooserScreen->mBanningPhase = true;
            mApp->mZombieChooserScreen->mBanningPhase = true;
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
    if (mApp->IsCoopMode() && theButton == GamepadButton::GAMEPAD_BUTTON_A) {
        if (mChooseState == SeedChooserState::CHOOSE_VIEW_LAWN) {
            old_SeedChooserScreen_GameButtonDown(this, theButton, thePlayerIndex);
            return;
        }

        if (mApp->mSecondPlayerGamepadIndex == -1 && mPlayerIndex != thePlayerIndex)
            return;

        int aSeedIndex = thePlayerIndex ? mSeedIndex2 : mSeedIndex1;
        int aSeedsInBank = mSeedsInBank;
        // 此处将判定条件改为选满8个种子时无法选取模仿者。原版游戏中此处是选满4个则无法选取，导致模仿者选取出现问题。
        if (aSeedIndex == SEED_IMITATER && aSeedsInBank < 8) {
            if (mChosenSeeds[SEED_IMITATER].mSeedState != ChosenSeedState::SEED_IN_BANK) {
                // 先将已选种子数改为0，然后执行旧函数，这样模仿者选取界面就被打开了。
                mSeedsInBank = 0;
                old_SeedChooserScreen_GameButtonDown(this, theButton, thePlayerIndex);
                // 然后再恢复已选种子数即可。
                mSeedsInBank = aSeedsInBank;
                return;
            }
        }
    }

    if (mPageButton) {
        if (theButton == GamepadButton::GAMEPAD_BUTTON_TR) {
            ButtonDepress(SeedChooserScreen::SeedChooserScreen_Page);
            return;
        }
    }

    // VS 模式回退到原始逻辑，避免破坏“选满自动开局”等原行为。
    if (mApp->IsVSMode()) {
        if (theButton == GamepadButton::GAMEPAD_BUTTON_A && IsLocalChooserInputAllowed(this)) {
            int playerIndex = mApp->GamepadToPlayerIndex(thePlayerIndex);
            if (playerIndex < 0 || playerIndex > 1) {
                playerIndex = mPlayerIndex;
            }

            int cursorX = (playerIndex == 0) ? mCursorPositionX1 : mCursorPositionX2;
            int cursorY = (playerIndex == 0) ? mCursorPositionY1 : mCursorPositionY2;
            SeedType aSeedType = SeedHitTest(cursorX, cursorY);
            if (aSeedType != SeedType::SEED_NONE) {
                if (gTcpConnected) {
                    U8x3_Event event = {{EventType::EVENT_CLIENT_SEEDCHOOSER_SELECT_SEED}, {uint8_t(aSeedType), uint8_t(mIsZombieChooser), 1}};
                    netplay::PutEvent(event);
                } else if (gTcpClientSocket >= 0) {
                    U8x3_Event event = {{EventType::EVENT_SERVER_SEEDCHOOSER_SELECT_SEED}, {uint8_t(aSeedType), uint8_t(mIsZombieChooser), 1}};
                    netplay::PutEvent(event);
                }
            }
        }
        old_SeedChooserScreen_GameButtonDown(this, theButton, thePlayerIndex);
        return;
    }

    unsigned int gamepadIndex = thePlayerIndex;
    int chooserPlayerIndex = mPlayerIndex;

    auto passSecondPlayerGate = [&]() -> bool {
        int secondPlayerState = mApp->mSecondPlayerGamepadIndex;
        if (secondPlayerState != -1 || theButton != GamepadButton::GAMEPAD_BUTTON_START) {
            return secondPlayerState == static_cast<int>(gamepadIndex);
        }

        if (!mApp->IsAdventureMode()) {
            return mApp->mSecondPlayerGamepadIndex == static_cast<int>(gamepadIndex);
        }

        mApp->SetSecondPlayer(static_cast<int>(gamepadIndex));
        return true;
    };

    if (chooserPlayerIndex != 0) {
        if (gamepadIndex != static_cast<unsigned int>(chooserPlayerIndex) && !passSecondPlayerGate()) {
            return;
        }
    } else if (gamepadIndex != 0) {
        // IDA uses *(int *)(mApp->unkMem?[slot] + 412) <= 1.
        // Guard against invalid runtime pointer here.
        bool singlePad = false;
        int runtimePtr = mApp->unkMem6[104];
        if (runtimePtr != 0) {
            singlePad = (*reinterpret_cast<int *>(runtimePtr + 412) <= 1);
        } else {
            singlePad = (int(mApp->mGamePad1IsOn) + int(mApp->mGamePad2IsOn) <= 1);
        }

        if (singlePad) {
            gamepadIndex = 0;
            mApp->SwapGamepadId(0, thePlayerIndex);
            chooserPlayerIndex = mPlayerIndex;
            if (gamepadIndex != static_cast<unsigned int>(chooserPlayerIndex) && !passSecondPlayerGate()) {
                return;
            }
        } else if (!passSecondPlayerGate()) {
            return;
        }
    }

    int playerIndex = mApp->GamepadToPlayerIndex(gamepadIndex);
    if (playerIndex < 0 || playerIndex > 1) {
        return;
    }

    if (mSeedsInFlight > 0) {
        for (auto &chosenSeed : mChosenSeeds) {
            LandFlyingSeed(chosenSeed);
        }
    }

    GamepadControls *controls = playerIndex == 0 ? mBoard->mGamepadControls[0] : mBoard->mGamepadControls[1];
    if (controls == nullptr || controls->mPlayerIndex1 == -1) {
        return;
    }

    if (mChooseState == SeedChooserState::CHOOSE_VIEW_LAWN && theButton == GamepadButton::GAMEPAD_BUTTON_A) {
        if (CancelLawnView()) {
            RebuildHelpbar();
        }
        return;
    }

    int &cursorX = (playerIndex == 0) ? mCursorPositionX1 : mCursorPositionX2;
    int &cursorY = (playerIndex == 0) ? mCursorPositionY1 : mCursorPositionY2;
    int &cursorSeed = (playerIndex == 0) ? mSeedIndex1 : mSeedIndex2;

    auto moveCursor = [&](SeedDir dir) {
        int fromSeed = cursorSeed == SeedType::SEED_NONE ? 0 : static_cast<int>(cursorSeed);
        int nextSeed = GetNextSeedInDir(fromSeed, dir);
        cursorSeed = nextSeed;
        GetSeedPositionInChooser(nextSeed, cursorX, cursorY);
    };

    switch (theButton) {
        case GamepadButton::GAMEPAD_BUTTON_UP:
        case GamepadButton::GAMEPAD_BUTTON_DPAD_UP:
            moveCursor(SeedDir::SEED_DIR_UP);
            return;

        case GamepadButton::GAMEPAD_BUTTON_DOWN:
        case GamepadButton::GAMEPAD_BUTTON_DPAD_DOWN:
            moveCursor(SeedDir::SEED_DIR_DOWN);
            return;

        case GamepadButton::GAMEPAD_BUTTON_LEFT:
        case GamepadButton::GAMEPAD_BUTTON_DPAD_LEFT:
            moveCursor(SeedDir::SEED_DIR_LEFT);
            return;

        case GamepadButton::GAMEPAD_BUTTON_RIGHT:
        case GamepadButton::GAMEPAD_BUTTON_DPAD_RIGHT: {
            int currentSeed = (cursorSeed == SEED_NONE) ? 0 : cursorSeed;
            if (!mIsZombieChooser && currentSeed % 8 == 7 && HasPacket(SeedType::SEED_IMITATER, false) && mApp->mGameMode != GameMode::GAMEMODE_MP_VS) {
                cursorSeed = SEED_IMITATER;
                GetSeedPositionInChooser(SEED_IMITATER, cursorX, cursorY);
                return;
            }
            moveCursor(SeedDir::SEED_DIR_RIGHT);
            return;
        }

        case GamepadButton::GAMEPAD_BUTTON_SELECT:
        case GamepadButton::GAMEPAD_BUTTON_THUMBL:
            if (mBoard->mCutScene->IsSurvivalRepick() && mChooseState == SeedChooserState::CHOOSE_NORMAL) {
                ButtonDepress(SeedChooserScreen::SeedChooserScreen_ViewLawn);
            }
            return;

        case GamepadButton::GAMEPAD_BUTTON_START:
            if (mStartButton != nullptr && !mStartButton->mDisabled && mApp->mGameMode != GameMode::GAMEMODE_MP_VS) {
                ButtonDepress(SeedChooserScreen::SeedChooserScreen_Start);
            }
            return;

        case GamepadButton::GAMEPAD_BUTTON_A: {
            if (mApp->mSecondPlayerGamepadIndex == -1 && mPlayerIndex != static_cast<int>(gamepadIndex)) {
                return;
            }

            if (cursorSeed == SEED_IMITATER && mSeedsInBank < mSeedBank1->mNumPackets) {
                if (mChosenSeeds[SEED_IMITATER].mSeedState != ChosenSeedState::SEED_IN_BANK) {
                    int savedSeedsInBank = mSeedsInBank;
                    mSeedsInBank = 0;
                    old_SeedChooserScreen_GameButtonDown(this, theButton, thePlayerIndex);
                    mSeedsInBank = savedSeedsInBank;
                    return;
                }

                ClickedSeedInBank(&mChosenSeeds[SEED_IMITATER], playerIndex);
            }

            SeedType aSeedType = SeedHitTest(cursorX, cursorY);
            if (aSeedType == SeedType::SEED_NONE) {
                return;
            }

            if (SeedNotAllowedToPick(aSeedType)) {
                mApp->PlaySample(Sexy::SOUND_BUZZER);
                return;
            }

            if (SeedNotAllowedDuringTrial(aSeedType)) {
                mApp->PlaySample(Sexy::SOUND_TAP);
                if (mApp->LawnMessageBox(Dialogs::DIALOG_MESSAGE, "[GET_FULL_VERSION_TITLE]", "[GET_FULL_VERSION_BODY]", "[GET_FULL_VERSION_YES_BUTTON]", "[GET_FULL_VERSION_NO_BUTTON]", 1) == 1000) {
                    mApp->DoBackToMain();
                }
                return;
            }

            ChosenSeed &aChosenSeed = mChosenSeeds[GetSeedPacketIndex(aSeedType)];
            if (aChosenSeed.mSeedState == ChosenSeedState::SEED_IN_CHOOSER) {
                ClickedSeedInChooser(aChosenSeed, playerIndex);
                return;
            }

            if (aChosenSeed.mSeedState != ChosenSeedState::SEED_IN_BANK) {
                return;
            }

            if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS) {
                return;
            }

            ClickedSeedInBank(&aChosenSeed, playerIndex);
            return;
        }

        case GamepadButton::GAMEPAD_BUTTON_B: {
            if (mChooseState == SeedChooserState::CHOOSE_VIEW_LAWN) {
                return;
            }

            if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS) {
                mApp->DoConfirmBackToMain(false);
                return;
            }

            int pickedCount = 0;
            for (const auto &seed : mChosenSeeds) {
                if (seed.mSeedState != ChosenSeedState::SEED_IN_BANK || seed.mCrazyDavePicked) {
                    continue;
                }
                if (!mApp->IsCoopMode() || seed.mChosenPlayerIndex == playerIndex) {
                    ++pickedCount;
                }
            }

            if (pickedCount <= 0) {
                mApp->DoConfirmBackToMain(false);
                return;
            }

            int removeBankIndex = pickedCount - 1;
            for (auto &seed : mChosenSeeds) {
                if (seed.mSeedState != ChosenSeedState::SEED_IN_BANK || seed.mCrazyDavePicked) {
                    continue;
                }
                if (mApp->IsCoopMode() && seed.mChosenPlayerIndex != playerIndex) {
                    continue;
                }
                if (seed.mSeedIndexInBank == removeBankIndex) {
                    ClickedSeedInBank(&seed, playerIndex);
                    return;
                }
            }
            return;
        }

        case GamepadButton::GAMEPAD_BUTTON_X:
            if (mApp->CanShowStore() && mApp->mGameMode != GameMode::GAMEMODE_MP_VS) {
                ButtonDepress(SeedChooserScreen::SeedChooserScreen_Store);
            }
            return;

        case GamepadButton::GAMEPAD_BUTTON_Y:
            if (mApp->CanShowAlmanac() && mApp->mGameMode != GameMode::GAMEMODE_MP_VS) {
                ButtonDepress(SeedChooserScreen::SeedChooserScreen_Almanac);
            }
            return;

        default:
            return;
    }
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

        // 禁用阶段种子栏中的卡变灰
        if (mBanningPhase) {
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
    LawnApp *lawnApp = gLawnApp;
    lawnApp->mSeedChooserScreen->mFocusedChildWidget = nullptr; // 修复点击菜单后无法按键选取植物卡片
}

void SeedChooserScreen::ButtonDepress(int theId) {
    if (mApp->IsVSMode() && !IsLocalChooserInputAllowed(this)) {
        return;
    }

    if (mApp->IsVSMode()) {
        if (gTcpConnected) {
            U8_Event event = {{EventType::EVENT_CLIENT_SEEDCHOOSER_BUTTON_DEPRESS}, uint8_t(theId)};
            netplay::PutEvent(event);
        } else if (gTcpClientSocket >= 0) {
            U8_Event event = {{EventType::EVENT_SERVER_SEEDCHOOSER_BUTTON_DEPRESS}, uint8_t(theId)};
            netplay::PutEvent(event);
        }
    }

    ButtonDepress_Origin(theId);
}

void SeedChooserScreen::ButtonDepress_Origin(int theId) {
    if (mSeedsInFlight > 0 || mChooseState == SeedChooserState::CHOOSE_VIEW_LAWN || !mMouseVisible) {
        return;
    }

    if (theId == SeedChooserScreen_Menu) {
        mApp->PlaySample(Sexy::SOUND_PAUSE);
        mApp->DoNewOptions(false, 0);
        return;
    }

    if (theId == SeedChooserScreen::SeedChooserScreen_Page) {
        mPageIndex = (mPageIndex == 0) ? 1 : 0;
        // 翻页后光标移动回第一张卡
        int x, y;
        GetSeedPositionInChooser(0, x, y);
        mCursorPositionX1 = mCursorPositionX2 = x;
        mCursorPositionY1 = mCursorPositionY2 = y;
        mSeedIndex1 = mSeedIndex2 = SeedType(0);
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
    if (mIsZombieChooser && aRow == 3 && !IsExtraSeedsModeEnabled(this)) {
        x = 53 * aCol + 48;
    } else {
        x = 53 * aCol + 22;
    }
    if (!mIsZombieChooser && Has7Rows()) {
        y = 70 * aRow + 123;
    } else {
        y = 73 * aRow + 128;
        //        if (mIsZombieChooser && mPageIndex == 1) {
        //            y = 73 * aRow + 128 - 5 * (SEED_PACKET_HEIGHT + 3);
        //        }
    }
}

int SeedChooserScreen::NumColumns() {
    return mIsZombieChooser ? 5 : 8;
}


void SeedChooserScreen::ShowToolTip(unsigned int thePlayerIndex) {
    old_SeedChooserScreen_ShowToolTip(this, thePlayerIndex);

    ToolTipWidget *aToolTip = (thePlayerIndex == 1) ? mToolTip2 : mToolTip1;
    int aToolTipSeed = (thePlayerIndex == 1) ? mToolTipSeed2 : mToolTipSeed1;

    if (mApp->IsVSMode()) {
        int aGamepadIndex = mApp->PlayerToGamepadIndex(thePlayerIndex);
        int x = (aGamepadIndex == 1) ? mCursorPositionX2 : mCursorPositionX1;
        int y = (aGamepadIndex == 1) ? mCursorPositionY2 : mCursorPositionY1;
        SeedType aSeedType = SeedHitTest(x, y);
        for (auto &aBannedSeed : mBannedSeed) {
            if (aSeedType == aBannedSeed.mSeedType) {
                aToolTip->SetWarningText("本轮已禁用");
            }
        }

        int aSeedX, aSeedY;
        SeedType aZombieSeedType = GetZombieIndexBySeedType(aSeedType);
        GetSeedPositionInChooser(aZombieSeedType, aSeedX, aSeedY);
        if (mSeedsInFlight <= 0 && mIsZombieChooser && mPageIndex == 1) {
            aToolTip->mY = aSeedY - 4 * (SEED_PACKET_HEIGHT + 3);
        }

        if (mIsZombieChooser) {
            if (mChosenSeeds[aSeedType - SEED_ZOMBIE_GRAVESTONE].mSeedState == ChosenSeedState::SEED_IN_BANK && mChosenSeeds[aSeedType - SEED_ZOMBIE_GRAVESTONE].mCrazyDavePicked) {
                aToolTip->SetWarningText(aToolTipSeed == SEED_ZOMBIE_GRAVESTONE ? "[ZOMBIE_BOSS_WANTS]" : "");
            }
            // 对战显示隐藏僵尸卡信息
            if (aSeedType > SeedType::SEED_ZOMBIE_GARGANTUAR && aSeedType < SeedType::NUM_ZOMBIE_SEED_IN_CHOOSER) {
                const char *aTitle = nullptr;
                const char *aLabel = nullptr;
                switch (aSeedType) {
                    case SeedType::SEED_ZOMBIE_PEA_HEAD: // 豌豆射手僵尸
                        aTitle = "[PEA_HEAD_ZOMBIE]";
                        aLabel = "[PEA_HEAD_ZOMBIE_DESCRIPTION_HEADER]";
                        break;
                    case SeedType::SEED_ZOMBIE_JALAPENO_HEAD: // 火爆辣椒僵尸
                        aTitle = "[JALAPENO_HEAD_ZOMBIE]";
                        aLabel = "[JALAPENO_HEAD_ZOMBIE_DESCRIPTION_HEADER]";
                        break;
                    case SeedType::SEED_ZOMBIE_GATLINGPEA_HEAD: // 机枪射手僵尸
                        aTitle = "[GATLING_HEAD_ZOMBIE]";
                        aLabel = "[GATLING_HEAD_ZOMBIE_DESCRIPTION_HEADER]";
                        break;
                    case SeedType::SEED_ZOMBIE_SQUASH_HEAD: // 窝瓜僵尸
                        aTitle = "[SQUASH_HEAD_ZOMBIE]";
                        aLabel = "[SQUASH_HEAD_ZOMBIE_DESCRIPTION_HEADER]";
                        break;
                    case SeedType::SEED_ZOMBIE_TALLNUT_HEAD: // 高坚果僵尸
                        aTitle = "[TALLNUT_HEAD_ZOMBIE]";
                        aLabel = "[TALLNUT_HEAD_ZOMBIE_DESCRIPTION_HEADER]";
                        aToolTip->mX = aSeedX + 2 * (SEED_PACKET_WIDTH + 6);
                        break;
                    case SeedType::SEED_ZOMBIE_GIGA_FOOTBALL:
                        aTitle = "[GIGA_FOOTBALL_ZOMBIE]";
                        aLabel = "[GIGA_FOOTBALL_ZOMBIE_DESCRIPTION_HEADER]";
                        break;
                    case SeedType::SEED_ZOMBIE_MOUND: // 召唤墓碑
                        aTitle = "[ZOMBIE_MOUND]";
                        aLabel = "[ZOMBIE_MOUND_DESCRIPTION]";
                        aToolTip->mX = aSeedX + 9 * (SEED_PACKET_WIDTH + 1);
                        aToolTip->mY = aSeedY - 4 * (SEED_PACKET_HEIGHT + 3);
                        aToolTip->mVisible = true;
                        break;
                    default:
                        return;
                }
                aToolTip->SetTitle(aTitle);
                aToolTip->SetLabel(aLabel);
            }
        } else {
            if (mBanningPhase) {
                if (mChosenSeeds[aSeedType].mSeedState == ChosenSeedState::SEED_IN_CHOOSER) {
                    if (aToolTipSeed == SEED_INSTANT_COFFEE) {
                        aToolTip->SetWarningText("在此阶段不允许");
                    }
                }
            }
        }
    }
}

SeedType SeedChooserScreen::GetZombieIndexBySeedType(SeedType theSeedType) {
    SeedType aSeedType = theSeedType - SEED_ZOMBIE_GRAVESTONE < 0 ? SeedType::SEED_NONE : SeedType(theSeedType - SEED_ZOMBIE_GRAVESTONE);
    //    if (mPageIndex == 1) {
    //        aSeedType = theSeedType - SEED_ZOMBIE_PEA_HEAD < 0 ? SeedType::SEED_NONE : SeedType(theSeedType - SEED_ZOMBIE_PEA_HEAD);
    //    }
    return aSeedType;
}

void SeedChooserScreen::MouseMove(int x, int y) {
    if (mApp->IsVSMode() && !IsLocalChooserInputAllowed(this)) {
        return;
    }
    NormalizeLocalPoint(this, x, y);
    if (x < 0 || x >= mWidth || y < 0 || y >= mHeight) {
        return;
    }

    SeedType aSeedType = SeedHitTest(x, y);
    // 该函数探测不到模仿者位置
    if (aSeedType == SeedType::SEED_NONE) {
        return;
    }

    if (mIsZombieChooser) {
        if ((mPageIndex == 0 && aSeedType > SeedType::SEED_ZOMBIE_BALLOON) || (mPageIndex == 1 && aSeedType >= SeedType::NUM_ZOMBIE_SEED_IN_CHOOSER)) {
            return;
        }
        if (mPageIndex == 1) {
            aSeedType = SeedType(aSeedType - 25);
        }

        SeedType aZombieSeedType = GetZombieIndexBySeedType(aSeedType);
        GetSeedPositionInChooser(aZombieSeedType, mCursorPositionX1, mCursorPositionY1);
        GetSeedPositionInChooser(aZombieSeedType, mCursorPositionX2, mCursorPositionY2);
        mSeedIndex1 = aZombieSeedType;
        mSeedIndex2 = aZombieSeedType;
    } else if (m1PChoosingSeeds) {
        if (mApp->IsVSMode() && aSeedType > SeedType::SEED_MELONPULT)
            return;

        if (mApp->IsVSMode()) {
            GetSeedPositionInChooser(aSeedType, mCursorPositionX1, mCursorPositionY1);
            GetSeedPositionInChooser(aSeedType, mCursorPositionX2, mCursorPositionY2);
        } else {
            GetSeedPositionInChooser(aSeedType, mCursorPositionX1, mCursorPositionY1);
        }
        mSeedIndex1 = aSeedType;
    } else {
        GetSeedPositionInChooser(aSeedType, mCursorPositionX2, mCursorPositionY2);
        mSeedIndex2 = aSeedType;
    }
}

void SeedChooserScreen::MouseDown(int x, int y, int theClickCount) {
    NormalizeLocalPoint(this, x, y);
    if (x < 0 || x >= mWidth || y < 0 || y >= mHeight) {
        return;
    }

    if (mApp->IsVSMode() && !IsLocalChooserInputAllowed(this)) {
        gSeedChooserTouchState = SeedChooserTouchState::SEEDCHOOSER_TOUCHSTATE_NONE;
        gSeedChooserTouchOwner = this;
        return;
    }
    if (gSeedChooserTouchOwner != nullptr && gSeedChooserTouchOwner != this) {
        return;
    }
    if (gSeedChooserTouchOwner == this) {
        gSeedChooserTouchState = SeedChooserTouchState::SEEDCHOOSER_TOUCHSTATE_NONE;
    }

    m1PChoosingSeeds = !mApp->IsCoopMode() || mSeedsIn1PBank < 4;

    bool mViewLawnButtonDisabled = mViewLawnButton == nullptr || !mBoard->mCutScene->IsSurvivalRepick();
    bool mStoreButtonDisabled = mStoreButton == nullptr || mStoreButton->mDisabled;
    bool mStartButtonDisabled = mStartButton == nullptr || mStartButton->mDisabled;
    bool mAlmanacButtonDisabled = mAlmanacButton == nullptr || mAlmanacButton->mDisabled;

    if (!mViewLawnButtonDisabled) { // !mDisabled
        Sexy::Rect mViewLawnButtonRect = {mViewLawnButton->mX, mViewLawnButton->mY, mViewLawnButton->mWidth, 50};
        // LOGD("mStoreButtonRect:%d %d %d %d",mStoreButtonRect[0],mStoreButtonRect[1],mStoreButtonRect[2],mStoreButtonRect[3]);
        if (mViewLawnButtonRect.Contains(x, y)) {
            mApp->PlaySample(Sexy::SOUND_TAP);
            gSeedChooserTouchState = SeedChooserTouchState::ViewLawnButton;
            gSeedChooserTouchOwner = this;
            // GameButtonDown(seedChooserScreen, 8, 0);
            return;
        }
    }

    if (!mStoreButtonDisabled) { // !mDisabled
        Sexy::Rect mStoreButtonRect = {mStoreButton->mX, mStoreButton->mY, mStoreButton->mWidth, 50};
        // LOGD("mStoreButtonRect:%d %d %d %d",mStoreButtonRect[0],mStoreButtonRect[1],mStoreButtonRect[2],mStoreButtonRect[3]);
        if (mStoreButtonRect.Contains(x, y)) {
            mApp->PlaySample(Sexy::SOUND_TAP);
            gSeedChooserTouchState = SeedChooserTouchState::StoreButton;
            gSeedChooserTouchOwner = this;
            // GameButtonDown(seedChooserScreen, 8, 0);
            return;
        }
    }

    if (!mStartButtonDisabled) { // !mDisabled
        Sexy::Rect mStartButtonRect = {mStartButton->mX, mStartButton->mY, mStartButton->mWidth, 50};
        if (mStartButtonRect.Contains(x, y)) {
            mApp->PlaySample(Sexy::SOUND_TAP);
            gSeedChooserTouchState = SeedChooserTouchState::StartButton;
            gSeedChooserTouchOwner = this;

            // SeedChooserScreen_OnStartButton(seedChooserScreen);
            return;
        }
    }

    if (!mAlmanacButtonDisabled) { // !mDisabled
        Sexy::Rect mAlmanacButtonRect = {mAlmanacButton->mX, mAlmanacButton->mY, mAlmanacButton->mWidth, 50};
        if (mAlmanacButtonRect.Contains(x, y)) {
            mApp->PlaySample(Sexy::SOUND_TAP);
            gSeedChooserTouchState = SeedChooserTouchState::AlmanacButton;
            gSeedChooserTouchOwner = this;

            // GameButtonDown(seedChooserScreen, 9, 0);
            return;
        }
    }

    if (HasPacket(SeedType::SEED_IMITATER, false) && !mApp->IsVSMode() && !mIsZombieChooser) {
        int mImitaterPositionX = 0;
        int mImitaterPositionY = 0;
        GetSeedPositionInChooser(SeedType::SEED_IMITATER, mImitaterPositionX, mImitaterPositionY);
        Sexy::Rect mImitaterPositionRect = {mImitaterPositionX, mImitaterPositionY, SEED_PACKET_WIDTH, SEED_PACKET_HEIGHT};
        if (mImitaterPositionRect.Contains(x, y)) {
            if (m1PChoosingSeeds) {
                mCursorPositionX1 = mImitaterPositionX;
                mCursorPositionY1 = mImitaterPositionY;
                mSeedIndex1 = SeedType::SEED_IMITATER;
            } else {
                mCursorPositionX2 = mImitaterPositionX;
                mCursorPositionY2 = mImitaterPositionY;
                mSeedIndex2 = SeedType::SEED_IMITATER;
            }
            GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, !m1PChoosingSeeds);
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
        if ((mPageIndex == 0 && aSeedType > SeedType::SEED_ZOMBIE_BALLOON) || (mPageIndex == 1 && aSeedType >= SeedType::NUM_ZOMBIE_SEED_IN_CHOOSER)) {
            return;
        }
        if (mPageIndex == 1) {
            aSeedType = SeedType(aSeedType - 25);
        }

        SeedType aZombieSeedType = GetZombieIndexBySeedType(aSeedType);
        GetSeedPositionInChooser(aZombieSeedType, mCursorPositionX1, mCursorPositionY1);
        GetSeedPositionInChooser(aZombieSeedType, mCursorPositionX2, mCursorPositionY2);
        mSeedIndex1 = aZombieSeedType;
        mSeedIndex2 = aZombieSeedType;
    } else if (m1PChoosingSeeds) {
        if (mApp->IsVSMode() && aSeedType > SeedType::SEED_MELONPULT)
            return;

        if (mApp->IsVSMode()) {
            GetSeedPositionInChooser(aSeedType, mCursorPositionX1, mCursorPositionY1);
            GetSeedPositionInChooser(aSeedType, mCursorPositionX2, mCursorPositionY2);
        } else {
            GetSeedPositionInChooser(aSeedType, mCursorPositionX1, mCursorPositionY1);
        }
        mSeedIndex1 = aSeedType;
    } else {
        GetSeedPositionInChooser(aSeedType, mCursorPositionX2, mCursorPositionY2);
        mSeedIndex2 = aSeedType;
    }
    gSeedChooserTouchState = SeedChooserTouchState::SeedChooser;
    gSeedChooserTouchOwner = this;
}

void SeedChooserScreen::MouseDrag(int x, int y) {
    if (mApp->IsVSMode() && !IsLocalChooserInputAllowed(this)) {
        return;
    }
    NormalizeLocalPoint(this, x, y);
    if (x < 0 || x >= mWidth || y < 0 || y >= mHeight) {
        return;
    }
    if (gSeedChooserTouchOwner != this) {
        return;
    }

    if (gSeedChooserTouchState == SeedChooserTouchState::SeedChooser) {
        SeedType aSeedType = SeedHitTest(x, y);
        // 该函数探测不到模仿者位置
        if (aSeedType == SeedType::SEED_NONE) {
            return;
        }
        if (mIsZombieChooser) {
            if ((mPageIndex == 0 && aSeedType > SeedType::SEED_ZOMBIE_BALLOON) || (mPageIndex == 1 && aSeedType >= SeedType::NUM_ZOMBIE_SEED_IN_CHOOSER)) {
                return;
            }
            if (mPageIndex == 1) {
                aSeedType = SeedType(aSeedType - 25);
            }

            SeedType aZombieSeedType = GetZombieIndexBySeedType(aSeedType);
            GetSeedPositionInChooser(aZombieSeedType, mCursorPositionX1, mCursorPositionY1);
            GetSeedPositionInChooser(aZombieSeedType, mCursorPositionX2, mCursorPositionY2);
            mSeedIndex1 = aZombieSeedType;
            mSeedIndex2 = aZombieSeedType;
        } else if (m1PChoosingSeeds) {
            if (mApp->IsVSMode() && aSeedType > SeedType::SEED_MELONPULT)
                return;

            if (mApp->IsVSMode()) {
                GetSeedPositionInChooser(aSeedType, mCursorPositionX1, mCursorPositionY1);
                GetSeedPositionInChooser(aSeedType, mCursorPositionX2, mCursorPositionY2);
            } else {
                GetSeedPositionInChooser(aSeedType, mCursorPositionX1, mCursorPositionY1);
            }
            mSeedIndex1 = aSeedType;
        } else {
            GetSeedPositionInChooser(aSeedType, mCursorPositionX2, mCursorPositionY2);
            mSeedIndex2 = aSeedType;
        }
    }

    if (mApp->IsVSMode() && IsLocalChooserInputAllowed(this)) {
        int ownerPlayerIndex = mPlayerIndex;
        if (ownerPlayerIndex < 0 || ownerPlayerIndex > 1) {
            ownerPlayerIndex = 0;
        }
        int chooserIndex = mIsZombieChooser ? 1 : 0;
        int cursorX = (ownerPlayerIndex == 0) ? mCursorPositionX1 : mCursorPositionX2;
        int cursorY = (ownerPlayerIndex == 0) ? mCursorPositionY1 : mCursorPositionY2;
        SeedType hoverSeedType = SeedHitTest(cursorX, cursorY);
        if (hoverSeedType != SeedType::SEED_NONE) {
            uint32_t nowMs = Sexy::GetTickCount();
            bool sameSeed = (gLastDragSyncSeedType[chooserIndex][ownerPlayerIndex] == hoverSeedType);
            uint32_t elapsedMs = nowMs - gLastDragSyncTickMs[chooserIndex][ownerPlayerIndex];
            if (sameSeed && elapsedMs < kSeedChooserDragSyncIntervalMs) {
                return;
            }

            // data2: bit0 = isZombieChooser, bit7 = moveOnly(sync cursor without picking)
            if (gTcpConnected) {
                U8x3_Event event = {{EventType::EVENT_CLIENT_SEEDCHOOSER_SELECT_SEED}, {uint8_t(hoverSeedType), uint8_t(mIsZombieChooser), 1}};
                netplay::PutEvent(event);
            } else if (gTcpClientSocket >= 0) {
                U8x3_Event event = {{EventType::EVENT_SERVER_SEEDCHOOSER_SELECT_SEED}, {uint8_t(hoverSeedType), uint8_t(mIsZombieChooser), 1}};
                netplay::PutEvent(event);
            }
            gLastDragSyncSeedType[chooserIndex][ownerPlayerIndex] = hoverSeedType;
            gLastDragSyncTickMs[chooserIndex][ownerPlayerIndex] = nowMs;
        }
    }
}

void SeedChooserScreen::MouseUp(int x, int y) {
    if (mApp->IsVSMode() && !IsLocalChooserInputAllowed(this)) {
        if (gSeedChooserTouchOwner == this) {
            gSeedChooserTouchState = SeedChooserTouchState::SEEDCHOOSER_TOUCHSTATE_NONE;
            gSeedChooserTouchOwner = nullptr;
        }
        return;
    }
    if (gSeedChooserTouchOwner != this) {
        return;
    }

    switch (gSeedChooserTouchState) {
        case SeedChooserTouchState::ViewLawnButton:
            ButtonDepress(SeedChooserScreen_ViewLawn);
            break;
        case SeedChooserTouchState::SeedChooser:
            if (mApp->IsVSMode()) {
                GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, mPlayerIndex);
            } else if (!mIsZombieChooser && m1PChoosingSeeds && mApp->IsCoopMode()) {
                GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 0);
            } else {
                GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 1);
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
    gSeedChooserTouchOwner = nullptr;
}

int SeedChooserScreen::GetNextSeedInDir(int theNumSeed, SeedDir theMoveDirection) {
    if (mIsZombieChooser) {
        if (!mShowExtraSeeds) {
            // 右下角边缘
            if ((theNumSeed == 14 && theMoveDirection == SeedDir::SEED_DIR_DOWN) || //
                (theNumSeed == 18 && theMoveDirection == SeedDir::SEED_DIR_RIGHT)) {
                return 18;
            }
        } else {
            if ((theNumSeed == 4 && theMoveDirection == SeedDir::SEED_DIR_DOWN) || //
                (theNumSeed == 5 && theMoveDirection == SeedDir::SEED_DIR_RIGHT)) {
                return NUM_ZOMBIE_SEED_IN_CHOOSER - SEED_ZOMBIE_BALLOON - 2;
            }
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
            int aMaxRow = mIsZombieChooser ? (mShowExtraSeeds ? 4 : 3) // 拓展僵尸选卡适配键盘选取
                                           : (Has7Rows() ? 5 : 4);
            if (mPageIndex == 1) {
                aMaxRow = 1;
            }
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
    Image *aBackgroundImage = mIsZombieChooser ? Sexy::IMAGE_SEEDCHOOSER_BACKGROUND2 : Sexy::IMAGE_SEEDCHOOSER_BACKGROUND;
    g->DrawImage(aBackgroundImage, 0, 87);

    // Draw imitater addon for plant chooser
    if (!mIsZombieChooser && HasPacket(SEED_IMITATER, false) && !mApp->IsVSMode()) {
        g->DrawImage(Sexy::IMAGE_SEEDCHOOSER_IMITATERADDON, mImitaterButton->mX - 5, mImitaterButton->mY - 12);
    }

    // Draw title text
    Color aTitleColor;
    const char *aTitleText;
    if (mIsZombieChooser) {
        aTitleColor = Color(0, 255, 0);
        aTitleText = "[CHOOSE_YOUR_ZOMBIES]";
    } else {
        aTitleColor = Color(213, 159, 43);
        aTitleText = "[CHOOSE_YOUR_PLANTS]";
    }

    TodDrawString(g, aTitleText, aBackgroundImage->mWidth / 2, 114, Sexy::FONT_DWARVENTODCRAFT18, aTitleColor, DS_ALIGN_CENTER);

    // Calculate seed count
    int aNumSeeds = 19;
    if (!mIsZombieChooser) {
        if (mApp->IsVSMode() || !Has7Rows())
            aNumSeeds = 40;
        else if (HasPacket(SEED_IMITATER, false))
            aNumSeeds = 49;
        else
            aNumSeeds = 48;
    } else {
        if (mShowExtraSeeds) {
            if (mPageIndex == 0) {
                aNumSeeds = 25;
            } else if (mPageIndex == 1) {
                aNumSeeds = NUM_ZOMBIE_SEED_IN_CHOOSER - SEED_ZOMBIE_GRAVESTONE;
            }
        }
    }

    // Draw seed packet shadows (two passes)
    for (int aPass = 0; aPass < 2; aPass++) {
        bool aDrawShadow = (aPass == 0);

        for (SeedType aSeedShadow = SEED_PEASHOOTER; aSeedShadow < aNumSeeds; aSeedShadow = SeedType(aSeedShadow + 1)) {
            int x, y;
            GetSeedPositionInChooser(aSeedShadow, x, y);
            if (mPageIndex == 1) {
                y -= 5 * (SEED_PACKET_HEIGHT + 3);
            }

            SeedType aDisplaySeedType = aSeedShadow;
            if (mIsZombieChooser) {
                aDisplaySeedType = GetZombieSeedType(aSeedShadow);
                if (mPageIndex == 1 && aDisplaySeedType <= SeedType::SEED_ZOMBIE_BALLOON)
                    continue;
            }

            if (aDisplaySeedType == SEED_IMITATER)
                continue;

            if (aDisplaySeedType == SEED_NONE || !HasPacket(aDisplaySeedType, mIsZombieChooser)) {
                if (aDrawShadow)
                    g->DrawImage(Sexy::IMAGE_SEEDPACKETSILHOUETTE, x, y);
            } else {
                ChosenSeed &aChosenSeed = mChosenSeeds[aSeedShadow];
                if (aChosenSeed.mSeedState != SEED_IN_CHOOSER) {
                    // Determine grayness based on selection state
                    int aGrayness = 55;
                    //                    if (mSeedIndex1 == aSeedShadow || mSeedIndex2 == aSeedShadow)
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
            g->DrawImage(Sexy::IMAGE_SEEDPACKETSILHOUETTE, x, y);
        }
    }

    // Draw coop mode second bank slots
    if (mApp->IsCoopMode() && mSeedBank2) {
        for (int anIndex = 0; anIndex < aNumSeedsInBank; anIndex++) {
            if (FindSeedInBank(anIndex, true) == SEED_NONE) {
                int x, y;
                GetSeedPositionInBank(anIndex, x, y, 1);
                g->DrawImage(Sexy::IMAGE_SEEDPACKETSILHOUETTE, x, y);
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

        if (aDisplaySeedType == SEED_NONE || (!mIsZombieChooser && aSeedType >= aNumSeeds))
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
        } else {
            if (mIsZombieChooser) {
                if (mPageIndex == 0 && aDisplaySeedType > SeedType::SEED_ZOMBIE_BALLOON) {
                    continue;
                } else if (mPageIndex == 1) {
                    if (aDisplaySeedType <= SeedType::SEED_ZOMBIE_BALLOON) {
                        continue;
                    } else {
                        aPosY -= 5 * (SEED_PACKET_HEIGHT + 3);
                    }
                }
            }
        }

        // Determine grayness
        bool aGrayed = false;

        if (!mIsZombieChooser) {
            if (aSeedState == SEED_IN_CHOOSER) {
                if (SeedNotRecommendedToPick(aChosenSeed.mSeedType) || SeedNotAllowedToPick(aChosenSeed.mSeedType)) {
                    aGrayed = true;
                }
            }

            if (SeedNotAllowedDuringTrial(aChosenSeed.mSeedType))
                aGrayed = true;
        }

        // Check if being dragged
        if (mSeedIndex1 == aSeedType && mBoard->mGamepadControls[0]->mPlayerIndex1 != -1 && aSeedState == SEED_IN_CHOOSER) {
            mSeedIndex1 = aSeedType;
        }

        if (mSeedIndex2 == aSeedType && mBoard->mGamepadControls[1]->mPlayerIndex2 != -1 && aSeedState == SEED_IN_CHOOSER) {
            mSeedIndex2 = aSeedType;
        }

        DrawPacket(g, aPosX, aPosY, aChosenSeed.mSeedType, aChosenSeed.mImitaterType, 0.0f, aGrayed ? 115 : 255, &aBaseColor, true, true);
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
        int aPlayerState = (aPlayerIndex ? mBoard->mGamepadControls[1] : mBoard->mGamepadControls[0])->mPlayerIndex2;
        if (aPlayerState != -1 && !unkMems3[3]) {
            if (aPlayerState == mPlayerIndex || !mApp->IsVSMode()) {
                Image *aSelectorImage = (aPlayerState == mApp->mSecondPlayerGamepadIndex) ? Sexy::IMAGE_SEED_SELECTOR_BLUE : Sexy::IMAGE_SEED_SELECTOR;
                if (mBanningPhase) {
                    aSelectorImage = (aSelectorImage == Sexy::IMAGE_SEED_SELECTOR_BLUE) ? Sexy::IMAGE_SEED_SELECTOR : Sexy::IMAGE_SEED_SELECTOR_BLUE;
                }
                g->DrawImage(aSelectorImage, aCursorX - 8, aCursorY - 4, 64, 85);
            }
        }
    }

    // Draw dragging seeds for player 1
    if (mSeedIndex1 != SEED_NONE && ShouldDisplayCursor(0)) {
        int x, y;
        GetSeedPositionInChooser(mSeedIndex1, x, y);
        auto aSeedType = SeedType(mSeedIndex1);
        SeedType aDisplaySeedType = mIsZombieChooser ? GetZombieSeedType(mSeedIndex1) : aSeedType;
        ChosenSeed &aChosenSeed = mChosenSeeds[mSeedIndex1];
        int aGrayness = 255;
        if (mPageIndex == 0 && aChosenSeed.mSeedState != SEED_IN_CHOOSER)
            aGrayness = 55;
        if (mPageIndex == 1 && mChosenSeeds[mSeedIndex1 + 25].mSeedState != SEED_IN_CHOOSER)
            aGrayness = 55;
        if ((!mIsZombieChooser && ((SeedNotRecommendedToPick(aSeedType) || SeedNotAllowedToPick(aSeedType))) && aChosenSeed.mSeedState == SEED_IN_CHOOSER) || SeedNotAllowedDuringTrial(aSeedType))
            aGrayness = 115;

        if (mPageIndex == 1) {
            aDisplaySeedType = SeedType(aDisplaySeedType + 25);
        }
        DrawPacket(g, x, y + 5, aDisplaySeedType, SEED_NONE, 0.0f, aGrayness, &aBaseColor, true, true);
    }

    // Draw dragging seeds for player 2
    if (mSeedIndex2 != SEED_NONE && ShouldDisplayCursor(1)) {
        int x, y;
        GetSeedPositionInChooser(mSeedIndex2, x, y);
        auto aSeedType = SeedType(mSeedIndex2);
        SeedType aDisplaySeedType = mIsZombieChooser ? GetZombieSeedType(mSeedIndex2) : aSeedType;
        ChosenSeed &aChosenSeed = mChosenSeeds[mSeedIndex2];
        int aGrayness = 255;
        if (mPageIndex == 0 && aChosenSeed.mSeedState != SEED_IN_CHOOSER)
            aGrayness = 55;
        if (mPageIndex == 1 && mChosenSeeds[mSeedIndex2 + 25].mSeedState != SEED_IN_CHOOSER)
            aGrayness = 55;
        if ((!mIsZombieChooser && ((SeedNotRecommendedToPick(aSeedType) || SeedNotAllowedToPick(aSeedType))) && aChosenSeed.mSeedState == SEED_IN_CHOOSER) || SeedNotAllowedDuringTrial(aSeedType))
            aGrayness = 115;

        if (mPageIndex == 1) {
            aDisplaySeedType = SeedType(aDisplaySeedType + 25);
        }
        DrawPacket(g, x, y + 5, aDisplaySeedType, SEED_NONE, 0.0f, aGrayness, &aBaseColor, true, true);
    }

    // 绘制对战禁用叉叉
    DrawBanIcon(g);

    // Draw cursor arrows for players
    for (int aPlayerIndex = 0; aPlayerIndex < 2; aPlayerIndex++) {
        if (ShouldDisplayCursor(aPlayerIndex) && (aPlayerIndex ? mBoard->mGamepadControls[1] : mBoard->mGamepadControls[0])->mPlayerIndex2 != -1) {
            Image *aArrowImage = aPlayerIndex ? Sexy::IMAGE_CURSOR_ARROW_P2 : Sexy::IMAGE_CURSOR_ARROW_P1;
            Image *aTextImage = aPlayerIndex ? Sexy::IMAGE_CURSOR_P2_TEXT : Sexy::IMAGE_CURSOR_P1_TEXT;

            if (mApp->IsVSMode() && mBanningPhase) {
                aArrowImage = aPlayerIndex ? Sexy::IMAGE_CURSOR_ARROW_P1 : Sexy::IMAGE_CURSOR_ARROW_P2;
                aTextImage = aPlayerIndex ? Sexy::IMAGE_CURSOR_P1_TEXT : Sexy::IMAGE_CURSOR_P2_TEXT;
            }

            float aBounce = sinf(unkF * 5.0f) * 2.0f;

            // 联机光标上绘制双方玩家昵称
            char *firstPlayerName = mBoard->mApp->mPlayerInfo->mName;
            if (gTcpConnected || gTcpClientSocket >= 0) {
                if (gSecondPlayerName[0] != '\0') {
                    char *name = mPlayerIndex ? (gTcpConnected ? gSecondPlayerName : firstPlayerName) : (gTcpClientSocket >= 0 ? gSecondPlayerName : firstPlayerName);
                    Color color = mPlayerIndex ? Color(68, 207, 255, 255) : Color(255, 242, 14, 255);
                    if (mBanningPhase) {
                        name = mPlayerIndex ? (gTcpClientSocket >= 0 ? firstPlayerName : gSecondPlayerName) : (gTcpConnected ? firstPlayerName : gSecondPlayerName);
                        color = mPlayerIndex ? Color(255, 242, 14, 255) : Color(68, 207, 255, 255);
                    } else {
                        name = mPlayerIndex ? (gTcpConnected ? firstPlayerName : gSecondPlayerName) : (gTcpClientSocket >= 0 ? firstPlayerName : gSecondPlayerName);
                        color = mPlayerIndex ? Color(68, 207, 255, 255) : Color(255, 242, 14, 255);
                    }
                    g->DrawImageF(aArrowImage, float(aCursorX + 25 - aArrowImage->mWidth / 2), float(aCursorY - 8) + aBounce);
                    TodDrawString(g, name, aCursorX + 25 - aArrowImage->mWidth / 2, aCursorY - 10, Sexy::FONT_DWARVENTODCRAFT18, color, DrawStringJustification::DS_ALIGN_CENTER);
                }
            } else {
                g->DrawImageF(aArrowImage, float(aCursorX + 25 - aArrowImage->mWidth / 2), float(aCursorY - 8) + aBounce);
                g->DrawImageF(aTextImage, float(aCursorX + 25 - aTextImage->mWidth / 2), float(aCursorY - 32));
            }
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
    if (!mApp->HasGamepad() && (!mApp->mGamePad1IsOn || !mApp->mGamePad2IsOn)) {
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
        int aTextX = aBackgroundImage->mWidth / 2;
        int aTextY = aBackgroundImage->mHeight - 63;

        Color aTextColor;
        if (aFlashPhase <= 50)
            aTextColor = Color(127, 127, 127, 255);
        else
            aTextColor = Color::White;

        TodDrawString(g, "[HELP_TEXT_2_START]", aTextX, aTextY, Sexy::FONT_DWARVENTODCRAFT24, aTextColor, DS_ALIGN_CENTER);
    }
    //    else {
    //        // Check for disconnected controller warning
    //        int aTwoPlayerState = mApp->mSecondPlayerGamepadIndex;
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
    //                    aWarningText = StrFormat(aWarningText.c_str(), aTwoPlayerState + 1); // 此处的StrFormat在TV中传入2，PSV则是mApp->mSecondPlayerGamepadIndex
    //
    //                    int aTextX = aBackgroundImage->mWidth / 2;
    //                    int aTextY = aBackgroundImage->mHeight - 63;
    //                    Color aWarningColor(255, 0, 0);
    //
    //                    TodDrawString(g, aWarningText, aTextX, aTextY, Sexy::FONT_DWARVENTODCRAFT24, aWarningColor, DS_ALIGN_CENTER);
    //                }
    //            }
    //        }
    //    }

    DeferOverlay(0);
}

void SeedChooserScreen::DrawBanIcon(Sexy::Graphics *g) {
    if (!mApp->IsVSMode())
        return;

    if (mBanningPhase) {
        Graphics aBanGraphics(*g);
        aBanGraphics.mTransX = 0;
        aBanGraphics.mTransY = 0;
        aBanGraphics.SetColor(Color(205, 0, 0, 255));
        aBanGraphics.SetFont(Sexy::FONT_DWARVENTODCRAFT18);
        aBanGraphics.DrawString(TodStringTranslate("[VS_UI_BAN_PHASE_BIG]"), 440, 110);
    }

    for (auto &aBannedSeed : mBannedSeed) {
        if (aBannedSeed.mSeedState == BannedSeedState::SEED_BANNED) {
            int x = aBannedSeed.mX;
            int y = aBannedSeed.mY;
            if (mIsZombieChooser) {
                if (mPageIndex == 0 && aBannedSeed.mSeedType > SeedType::SEED_ZOMBIE_BALLOON) {
                    continue;
                } else if (mPageIndex == 1) {
                    if (aBannedSeed.mSeedType <= SeedType::SEED_ZOMBIE_BALLOON) {
                        continue;
                    }
                    y = aBannedSeed.mY - 5 * (SEED_PACKET_HEIGHT + 3);
                }
            }
            g->DrawImage(IMAGE_MP_TARGETS_X, x + 5, y + 5);
        }
    }
}

SeedType SeedChooserScreen::SeedHitTest(int x, int y) {
    SeedType aSeedType = SeedHitTest_Origin(x, y);
    if (mIsZombieChooser && mPageIndex == 1) {
        if (aSeedType == SeedType::SEED_NONE) {
            return SEED_NONE;
        }
        aSeedType = SeedType(aSeedType + 25);
    }
    return aSeedType;
}

SeedType SeedChooserScreen::SeedHitTest_Origin(int x, int y) {
    return old_SeedChooserScreen_SeedHitTest(this, x, y);
}

void SeedChooserScreen::VSAutoPickResourceGen() {
    SeedType aSeedType;
    int aX, aY;
    if (mSeedsInBank == 0) {
        if (mIsZombieChooser) {
            aSeedType = SeedType::SEED_ZOMBIE_GRAVESTONE;
        } else {
            aSeedType = mBoard->StageIsNight() ? SeedType::SEED_SUNSHROOM : SeedType::SEED_SUNFLOWER;
        }
        int aIndex = GetSeedPacketIndex(aSeedType);
        GetSeedPositionInBank(0, aX, aY, 0);
        mChosenSeeds[aIndex].mX = mChosenSeeds[aIndex].mStartX = mChosenSeeds[aIndex].mEndX = aX;
        mChosenSeeds[aIndex].mY = mChosenSeeds[aIndex].mStartY = mChosenSeeds[aIndex].mEndY = aY;
        mChosenSeeds[aIndex].mChosenPlayerIndex = mIsZombieChooser;
        mChosenSeeds[aIndex].mSeedIndexInBank = 0;
        mChosenSeeds[aIndex].mSeedState = ChosenSeedState::SEED_IN_BANK;
        mChosenSeeds[aIndex].mCrazyDavePicked = true;
        ++mSeedsInBank;
        ++mSeedsIn1PBank;
    }
}
