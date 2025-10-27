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

#include "PvZ/Lawn/Widget/WaitForSecondPlayerDialog.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/LawnApp.h"

using namespace Sexy;

void WaitForSecondPlayerDialog::_constructor(LawnApp *theApp) {
    old_WaitForSecondPlayerDialog_WaitForSecondPlayerDialog(this, theApp);

    GameButtonDown(ButtonCode::BUTTONCODE_A, 1);
    GameButtonDown(ButtonCode::BUTTONCODE_A, 1);

    // 解决此Dialog显示时背景僵尸全部聚集、且草丛大块空缺的问题
    if (theApp->mBoard != nullptr) {
        theApp->mBoard->UpdateGame();
        theApp->mBoard->UpdateCoverLayer();
    }

    pvzstl::string str = StrFormat("[PLAY_OFFLINE]");
    GameButton *playOfflineButton = MakeButton(1000, &mButtonListener, this, str);
    mLawnYesButton = playOfflineButton;

    pvzstl::string str2 = StrFormat("[BACK]");
    GameButton *backButton = MakeButton(1001, &mButtonListener, this, str2);
    mLawnNoButton = backButton;

    pvzstl::string str4 = StrFormat("[JOIN_ROOM_BUTTON]");
    GameButton *joinButton = MakeButton(1002, &mButtonListener, this, str4);
    joinButton->mDisabled = true;
    mJoinButton = joinButton;
    AddWidget(joinButton);

    pvzstl::string str3 = StrFormat("[CREATE_ROOM_BUTTON]");
    GameButton *createButton = MakeButton(1003, &mButtonListener, this, str3);
    mCreateButton = createButton;
    AddWidget(createButton);

    this->LawnDialog::Resize(0, 0, 800, 600);

    playOfflineButton->mY -= 20;
    playOfflineButton->mWidth -= 30;
    playOfflineButton->mX += 15;

    backButton->mY -= 20;
    backButton->mWidth -= 30;
    backButton->mX += 15;

    joinButton->mX = playOfflineButton->mX;
    joinButton->mY = playOfflineButton->mY - 80;
    joinButton->mWidth = playOfflineButton->mWidth;
    joinButton->mHeight = playOfflineButton->mHeight;

    createButton->mX = backButton->mX;
    createButton->mY = backButton->mY - 80;
    createButton->mWidth = backButton->mWidth;
    createButton->mHeight = backButton->mHeight;

    mIsCreatingRoom = false;
}

void WaitForSecondPlayerDialog::_destructor() {
    old_WaitForSecondPlayerDialog_Delete(this);
}

void WaitForSecondPlayerDialog::Update() {
    //    mJoinButton->mDisabled = server_count == 0;
}

void WaitForSecondPlayerDialog::Draw(Graphics *g) {
    old_WaitForSecondPlayerDialog_Draw(this, g);

    // TODO:将设备信息文字绘制出来

    if (mIsCreatingRoom) {
        pvzstl::string str = StrFormat("您创建的房间名称:%s的房间", mApp->mPlayerInfo->mName);
        g->DrawString(str, 200, 150);
    } else {
        //        int yPos = 200;
        //        for (int i = 0; i < server_count; i++) {
        //            pvzstl::string str = StrFormat("%s的房间 %s", servers[i].name, servers[i].ip);
        //            g->DrawString(str, 280, yPos);
        //            yPos += 30;
        //        }
    }
}

void WaitForSecondPlayerDialog_ButtonDepress(Sexy::ButtonListener *listener, int id) {
    auto *dialog = reinterpret_cast<WaitForSecondPlayerDialog *>((uint32_t(listener) - offsetof(WaitForSecondPlayerDialog, mButtonListener)));
    if (id == 1000) {
        // 2P手柄按两下A
        dialog->GameButtonDown(ButtonCode::BUTTONCODE_A, 1);
        dialog->GameButtonDown(ButtonCode::BUTTONCODE_A, 1);
    }

    old_WaitForSecondPlayerDialog_ButtonDepress(listener, id);
}

void WaitForSecondPlayerDialog::Resize(int theX, int theY, int theWidth, int theHeight) {
}