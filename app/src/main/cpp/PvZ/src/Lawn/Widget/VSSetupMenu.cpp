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

#include "PvZ/Lawn/Widget/VSSetupMenu.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/SeedBank.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/Widget/WaitForSecondPlayerDialog.h"

using namespace Sexy;

VSSetupWidget::VSSetupWidget() {
    if (gMoreZombieSeeds) {
        game_patches::drawMoreZombieSeeds.Modify();
    }
}

VSSetupWidget::~VSSetupWidget() {
    gVSMorePacketsButton->mMorePacketsButton->mBtnNoDraw = true;
    gVSMorePacketsButton->mMorePacketsButton->mDisabled = true;
    gVSMorePacketsButton->mDrawString = false;
    gVSMorePacketsButton = nullptr;
}

void VSSetupWidget::SetDisable() {
    gVSMorePacketsButton->mMorePacketsButton->mBtnNoDraw = true;
    gVSMorePacketsButton->mMorePacketsButton->mDisabled = true;
    gVSMorePacketsButton->mDrawString = false;
}

void VSSetupWidget::ButtonDepress(this VSSetupWidget &self, int theId) {
    if (theId == 1145) {
        self.CheckboxChecked(1145, self.mIsMorePackets);
        std::swap(gVSMorePacketsButton->mCheckboxImage, gVSMorePacketsButton->mCheckboxImagePress);
        ButtonWidget *aButton = gVSMorePacketsButton->mMorePacketsButton;
        aButton->mButtonImage = gVSMorePacketsButton->mCheckboxImage;
        aButton->mOverImage = gVSMorePacketsButton->mCheckboxImage;
        aButton->mDownImage = gVSMorePacketsButton->mCheckboxImage;
    }
}

void VSSetupWidget::CheckboxChecked(int theId, bool checked) {
    switch (theId) {
        case 1145:
            mIsMorePackets = !checked;
            break;
        default:
            break;
    }
}

void VSSetupMenu::_constructor() {
    old_VSSetupMenu_Constructor(this);

    Image *aCheckbox = *Sexy_IMAGE_OPTIONS_CHECKBOX0_Addr;
    Image *aCheckboxPressed = *Sexy_IMAGE_OPTIONS_CHECKBOX1_Addr;
    pvzstl::string str = StrFormat("");
    // 拓展卡槽
    gVSMorePacketsButton = new VSSetupWidget;
    ButtonWidget *aMorePacketsButton = MakeNewButton(1145, &mButtonListener, this, str, nullptr, aCheckbox, aCheckbox, aCheckbox);
    gVSMorePacketsButton->mMorePacketsButton = aMorePacketsButton;
    gVSMorePacketsButton->mCheckboxImage = aCheckbox;
    gVSMorePacketsButton->mCheckboxImagePress = aCheckboxPressed;
    aMorePacketsButton->Resize(MORE_PACKETS_BUTTON_X, MORE_PACKETS_BUTTON_Y, 175, 50);
    mApp->mBoard->AddWidget(aMorePacketsButton);
    gVSMorePacketsButton->mDrawString = true;

    mApp->mDanceMode = false;
    mApp->mBoard->mDanceMode = false;
}

void VSSetupMenu::_destructor() {
    old_VSSetupMenu_Destructor(this);

    if (gVSMorePacketsButton != nullptr)
        gVSMorePacketsButton->~VSSetupWidget();
}

void VSSetupMenu::Draw(Graphics *g) {
    old_VSSetupMenu_Draw(this, g);
}

void VSSetupMenu::Update() {
    // 记录当前游戏状态
    old_VSSetupMenu_Update(this);
}

void VSSetupMenu::KeyDown(Sexy::KeyCode theKey) {
    // 修复在对战的阵营选取界面无法按返回键退出的BUG。
    if (theKey == Sexy::KeyCode::KEYCODE_ESCAPE) {
        switch (mState) {
            case 1:
            case 2:
                mApp->DoBackToMain();
                return;
            case 3:
                mApp->DoNewOptions(false, 0);
                return;
        }
    }

    old_VSSetupMenu_KeyDown(this, theKey);
}

void VSSetupMenu::OnStateEnter(int theState) {
    if (theState == 0) {
        mInt76 = -1;
        // TODO:修复 WaitForSecondPlayerDialog重复构造
        auto *aWaitDialog = new WaitForSecondPlayerDialog(mApp);
        //        mApp->AddDialog(aWaitDialog); // 生成了两份mLawnNoButton

        int buttonId = ((int (*)(WaitForSecondPlayerDialog *, bool))aWaitDialog->vTable[127])(aWaitDialog, true);
        if (buttonId == 1000) {
            SetSecondPlayerIndex(mApp->mTwoPlayerState);
            GoToState(1);
        } else if (buttonId == 1001) {
            CloseVSSetup(1);
            mApp->KillBoard();
            mApp->ShowGameSelector();
        } else {
            // TODO：联机
        }
        return;
    }

    old_VSSetupMenu_OnStateEnter(this, theState);
}

void VSSetupMenu::ButtonPress(int theId) {
    old_VSSetupMenu_ButtonPress(this, theId);
}

void VSSetupMenu::ButtonDepress(int theId) {
    old_VSSetupMenu_ButtonDepress(this, theId);

    if (!isKeyboardTwoPlayerMode && mState == 1) {
        // 自动分配阵营
        GameButtonDown(ButtonCode::BUTTONCODE_LEFT, 0, 0);
        GameButtonDown(ButtonCode::BUTTONCODE_RIGHT, 1, 0);
        GameButtonDown(ButtonCode::BUTTONCODE_A, 0, 0);
        GameButtonDown(ButtonCode::BUTTONCODE_A, 1, 0);
    }

    mApp->mBoard->PickBackground(); // 修改器修改场地后开局立即更换

    // 对战额外卡槽
    int aNumPackets = mApp->mBoard->GetNumSeedsInBank(false);

    SeedBank *aSeedBank1 = mApp->mBoard->mSeedBank1;
    SeedBank *aSeedBank2 = mApp->mBoard->mSeedBank2;

    aSeedBank1->mNumPackets = aNumPackets;
    aSeedBank2->mNumPackets = aNumPackets;

    switch (theId) {
        case 9: // 快速游戏
            if (aNumPackets == 7) {
                aSeedBank1->mSeedPackets[3].SetPacketType(SeedType::SEED_TORCHWOOD, SeedType::SEED_NONE);
                aSeedBank1->mSeedPackets[4].SetPacketType(SeedType::SEED_POTATOMINE, SeedType::SEED_NONE);
                aSeedBank1->mSeedPackets[5].SetPacketType(SeedType::SEED_SQUASH, SeedType::SEED_NONE);
                aSeedBank1->mSeedPackets[6].SetPacketType(SeedType::SEED_JALAPENO, SeedType::SEED_NONE);
                aSeedBank2->mSeedPackets[4].SetPacketType(SeedType::SEED_ZOMBIE_PAIL, SeedType::SEED_NONE);
                aSeedBank2->mSeedPackets[5].SetPacketType(SeedType::SEED_ZOMBIE_FOOTBALL, SeedType::SEED_NONE);
                aSeedBank2->mSeedPackets[6].SetPacketType(SeedType::SEED_ZOMBIE_FLAG, SeedType::SEED_NONE);
            }
            break;
        case 10: // 自定义战场
            if (mState == 3) {
                gVSMorePacketsButton->SetDisable();
            }
            break;
        case 11: // 随机战场
            if (aNumPackets == 7) {
                mApp->mBoard->mSeedBank1->mNumPackets = 6;
                mApp->mBoard->mSeedBank2->mNumPackets = 6;
                // 开启“额外开槽”后随机选卡会导致界面卡死
                //                std::vector<SeedType> aZombieSeeds, aPlantSeeds, tmpZombieSeeds, tmpPlantSeeds;
                //
                //                PickRandomZombies(aZombieSeeds);
                //                do {
                //                    PickRandomZombies(tmpZombieSeeds);
                //                } while (tmpZombieSeeds[4] == aZombieSeeds[4]);
                //                aZombieSeeds.push_back(tmpZombieSeeds[4]);
                //
                //                PickRandomPlants(aPlantSeeds, aZombieSeeds);
                //                do {
                //                    PickRandomPlants(tmpPlantSeeds, aZombieSeeds);
                //                } while (tmpPlantSeeds[4] == aPlantSeeds[4]);
                //                aPlantSeeds.push_back(tmpPlantSeeds[4]);
                //
                //                aSeedBank2->mSeedPackets[0].SetPacketType(SeedType::SEED_ZOMBIE_GRAVESTONE, SeedType::SEED_NONE);
                //                aSeedBank1->mSeedPackets[0].SetPacketType(SeedType::SEED_SUNFLOWER, SeedType::SEED_NONE);
                //                for (int i = 0; i < aZombieSeeds.size(); ++i) {
                //                    aSeedBank2->mSeedPackets[i + 1].SetPacketType(aZombieSeeds[i], SeedType::SEED_NONE);
                //                }
                //                for (int i = 0; i < aPlantSeeds.size(); ++i) {
                //                    aSeedBank2->mSeedPackets[i + 1].SetPacketType(aPlantSeeds[i], SeedType::SEED_NONE);
                //                }
            }
            break;
        case 1145: // 额外卡槽
            gVSMorePacketsButton->ButtonDepress(1145);
            break;
        default:
            break;
    }

    // 修复“额外卡槽”开启后卡槽位置不正确
    for (int i = 0; i < SEEDBANK_MAX; i++) {
        SeedPacket *aPacket = &aSeedBank1->mSeedPackets[i];
        SeedPacket *aPacket2 = &aSeedBank2->mSeedPackets[i];
        aPacket->mIndex = i;
        aPacket->mX = mApp->mBoard->GetSeedPacketPositionX(i, 0, false);
        aPacket2->mX = mApp->mBoard->GetSeedPacketPositionX(i, 1, true);
    }
}