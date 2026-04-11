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

#include "PvZ/Lawn/Board/CutScene.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Board/SeedBank.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/Widget/ChallengeScreen.h"
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
        if (mApp->mSecondPlayerGamepadIndex == -1 && !mApp->GetDialog(Dialogs::DIALOG_CONTINUE) && !mApp->GetDialog(Dialogs::DIALOG_WAIT_FOR_SECOND_PLAYER)) {

            auto *aDialog = new WaitForSecondPlayerDialog(mApp);
            mApp->AddDialog(aDialog);
            mApp->mWidgetManager->SetFocus(aDialog);

            int buttonId = aDialog->WaitForResult(true);
            if (buttonId == 1001) {
                mBoard->unknownBool = true;
            } else {

                SeedBank *seedBank2 = mApp->mBoard->mSeedBank[1];
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
                    // mApp->mBoard->mSeedBank[1] = nullptr;
                    mApp->SetSecondPlayer(1);
                    mApp->mBoard->mGamepadControls2->mPlayerIndex2 = 1;
                    for (int i = 0; i < seedBank->mNumPackets; ++i) {
                        seedBank->mSeedPackets[i].mSeedBank = seedBank;
                    }
                    // mApp->mBoard->mSeedBank[1] = seedBank;
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
    old_CutScene_AddFlowerPots(this);
}

void CutScene::PlaceLawnItems() {
    if (mPlacedLawnItems) {
        return;
    }
    mPlacedLawnItems = true;

    if (!IsSurvivalRepick()) {
        mBoard->InitLawnMowers();
        AddFlowerPots();
        mBoard->PlaceRake();
    }

    if (mApp->IsVSMode()) {
        int aNumRows = mBoard->StageHas6Rows() ? 6 : 5;
        SeedType aSunPlantType = mBoard->StageIsNight() ? SeedType::SEED_SUNSHROOM : SeedType::SEED_SUNFLOWER;
        for (int aRow = 0; aRow < aNumRows; ++aRow) {
            mBoard->AddMPTarget(8, aRow);
            if ((aRow == 1 || aRow == aNumRows - 2) || mBoard->StageIsNight()) { // 黑夜种满一列
                mBoard->AddAGraveStone(8, aRow);
                Plant *aPlant = mBoard->AddPlant(0, aRow, aSunPlantType, SeedType::SEED_NONE, -1, true);
                if (aSunPlantType == SeedType::SEED_SUNSHROOM) {
                    aPlant->mStateCountdown = 0;
                }
            }
            if (mBoard->StageHasPool() && (aRow == 2 || aRow == 3)) {
                mBoard->AddPlant(0, aRow, SeedType::SEED_LILYPAD, SeedType::SEED_NONE, -1, true);
            }
        }
    }
}

void CutScene::LoadUpsellChallengeScreen() {
    ClearUpsellBoard();
    mUpsellChallengeScreen = new ChallengeScreen(mApp, ChallengePage::CHALLENGE_PAGE_CHALLENGE);
}