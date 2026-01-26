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

#include "PvZ/Lawn/Widget/NewOptionsDialog.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/System/Music.h"
#include "PvZ/Lawn/Widget/VSResultsMenu.h"
#include "PvZ/Lawn/Widget/WaitForSecondPlayerDialog.h"

void NewOptionsDialog::ButtonDepress(int buttonId) {

    if (buttonId == 5 && (tcp_connected || tcpClientSocket >= 0)) {
        mApp->PlaySample(*Sexy_SOUND_GRAVEBUTTON_Addr);

        this->Sexy::Dialog::ButtonDepress(buttonId);

        if (mApp->mBoard) {


            Sexy::Dialog *restartDialog = mApp->DoConfirmRestartDialog();

            if (restartDialog->WaitForResult(false) == 1000) {
                mApp->mMusic->StopAllMusic();
                mApp->mSoundSystem->CancelPausedFoley();
                mApp->KillNewOptionsDialog();

                if (tcp_connected) {
                    // 客户端点击投降
                    BaseEvent event = {EventType::EVENT_CLIENT_BOARD_CONCEDE};
                    sendWithSize(tcpServerSocket, &event, sizeof(BaseEvent), 0);
                    if (mApp->mBoard->mGamepadControls2->mPlayerIndex2 == 1) {
                        mApp->SetBoardResult(7);
                        mApp->mGameScene = SCENE_ZOMBIES_WON;
                    } else {
                        mApp->SetBoardResult(8);
                        mApp->mGameScene = SCENE_PLANTS_WON;
                    }
                }

                if (tcpClientSocket >= 0) {
                    // 主机端点击投降
                    BaseEvent event = {EventType::EVENT_SERVER_BOARD_CONCEDE};
                    sendWithSize(tcpClientSocket, &event, sizeof(BaseEvent), 0);
                    if (mApp->mBoard->mGamepadControls1->mPlayerIndex2 == 1) {
                        mApp->SetBoardResult(7);
                        mApp->mGameScene = SCENE_ZOMBIES_WON;
                    } else {
                        mApp->SetBoardResult(8);
                        mApp->mGameScene = SCENE_PLANTS_WON;
                    }
                }


                mApp->ShowVSResultsScreen();
                mApp->mVSResultsScreen->InitFromBoard(mApp->mBoard);
                mApp->KillBoard();
            }
        }
    }


    return old_NewOptionsDialog_ButtonDepress(this, buttonId);
}