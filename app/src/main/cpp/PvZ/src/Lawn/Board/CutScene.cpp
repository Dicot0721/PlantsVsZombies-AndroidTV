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

#include "PvZ/Lawn/Board/CutScene.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Board/SeedBank.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/Widget/WaitForSecondPlayerDialog.h"
#include "PvZ/SexyAppFramework/Widget/WidgetManager.h"

using namespace Sexy;

void CutScene::ShowShovel() {
    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BUTTERED_POPCORN) {
        // 不绘制锤子铲子黄油按钮
        mBoard->mShowShovel = false;
        mBoard->mShowButter = false;
        mBoard->mShowHammer = false;
        return;
    }

    old_CutScene_ShowShovel(this);
}


void CutScene::Update() {
    if (mPreUpdatingBoard)
        return;
    if (mApp->mGameMode == GameMode::GAMEMODE_ADVENTURE_TWO_PLAYER || mApp->IsCoopMode()) {
        if (mApp->mTwoPlayerState == -1 && !mApp->GetDialog(Dialogs::DIALOG_CONTINUE) && !mApp->GetDialog(Dialogs::DIALOG_WAIT_FOR_SECOND_PLAYER)) {

            auto *aDialog = new WaitForSecondPlayerDialog(mApp);
            mApp->AddDialog(aDialog);
            mApp->mWidgetManager->SetFocus(aDialog);

            int buttonId = aDialog->WaitForResult(true);
            if (buttonId == 1001) {
                mBoard->unknownBool = 1;
            } else {

                SeedBank *seedBank2 = mApp->mBoard->mSeedBank2;
                if (seedBank2) {
                    SeedBank *seedBank = (SeedBank *)operator new(sizeof(SeedBank));
                    seedBank->mNumPackets = seedBank2->mNumPackets;
                    for (int i = 0; i < seedBank2->mNumPackets; ++i) {
                        seedBank->mSeedPackets[i] = seedBank2->mSeedPackets[i];
                    }

                    for (int i = 0; i < 7; ++i) {
                        seedBank->mShopSeedPackets[i] = seedBank2->mShopSeedPackets[i];
                    }

                    // seedBank2->~SeedBank();
                    // mApp->mBoard->mSeedBank2 = nullptr;
                    mApp->SetSecondPlayer(1);
                    mApp->mBoard->mGamepadControls2->mPlayerIndex2 = 1;
                    for (int i = 0; i < seedBank->mNumPackets; ++i) {
                        seedBank->mSeedPackets[i].mSeedBank = seedBank;
                    }
                    // mApp->mBoard->mSeedBank2 = seedBank;
                }
            }
            return;
        }
    }

    old_CutScene_Update(this);
}


void CutScene::UpdateZombiesWonMP() {}

void CutScene::UpdatePlantsWon() {}


void CutScene::AddFlowerPots() {
    // 对战添加初始花盆
    if (mApp->mGameMode == GAMEMODE_MP_VS) {
        if (mBoard->StageHasRoof()) {
            mBoard->AddPlant(0, 1, SeedType::SEED_FLOWERPOT, SeedType::SEED_NONE, 1, false);
            mBoard->AddPlant(0, 3, SeedType::SEED_FLOWERPOT, SeedType::SEED_NONE, 1, false);
            for (int i = 3; i < 4; ++i) {
                for (int j = 0; j < 5; ++j) {
                    mBoard->AddPlant(i, j, SeedType::SEED_FLOWERPOT, SeedType::SEED_NONE, 1, false);
                }
            }
        }
    }
    return old_CutScene_AddFlowerPots(this);
}

void CutScene::PlaceLawnItems() {

    if (mApp->mGameMode == GAMEMODE_MP_VS) {
        if (mPlacedLawnItems)
            return;

        mPlacedLawnItems = 1;

        // ===== TwoPlayerVS 专用：放 MPTarget + 墓碑/植物 =====
        if (mApp->mGameMode == GAMEMODE_MP_VS) {
            const int rows = mBoard->StageHas6Rows() ? 6 : 5;

            // ---------- 1) AddMPTarget: x=8, row=r ----------
            for (int r = 0; r < rows; r++) {
                bool shouldAddTarget = false;

                if (*Challenge_gVSWinModeAddr == 2) {
                    if (rows == 6) {
                        // rows==6: r==1 or r==4
                        shouldAddTarget = (r == 1 || r == 4);
                    } else {
                        // rows == 5: r == 1 or r == 3
                        shouldAddTarget = (r == 1 || r == 3);
                    }
                } else if (*Challenge_gVSWinModeAddr == 3) {
                    if (rows == 6) {
                        // rows==6: add at r==0,2,3,5
                        shouldAddTarget = (r == 0 || r == 2 || r == 3 || r == 5);
                    } else {
                        // rows==5: r==1 or r==4
                        shouldAddTarget = (r == 1 || r == 4);
                    }
                } else {
                    // 其他 winmode：默认 add
                    shouldAddTarget = true;
                }

                if (shouldAddTarget) {
                    mBoard->AddMPTarget(8, r);
                }
            }

            // ---------- 2) AddAGraveStone + AddPlant ----------
            SeedType seedToPlant = (mApp->mVsInitialPlantMode == 1 || mApp->mVsInitialPlantMode == 3) ? SEED_SUNSHROOM : SEED_SUNFLOWER;

            for (int r = 0; r < rows; r++) {
                bool shouldPlace = false;
                if (rows == 5) {
                    if (r == 1 || r == 3) // r==1 or r==3
                    {
                        shouldPlace = true;
                    }
                } else {
                    if (r == 1 || r == 4) {
                        shouldPlace = true;
                    }
                }

                if (shouldPlace) {
                    mBoard->AddAGraveStone(8, r);
                    mBoard->AddPlant(0, r, seedToPlant, (SeedType)-1, -1, true);
                }
            }
        }

        if (this->IsSurvivalRepick()) {
            return;
        }

        mBoard->InitLawnMowers();
        AddFlowerPots();
        mBoard->PlaceRake();
        return;
    }

    return old_CutScene_PlaceLawnItems(this);
}
