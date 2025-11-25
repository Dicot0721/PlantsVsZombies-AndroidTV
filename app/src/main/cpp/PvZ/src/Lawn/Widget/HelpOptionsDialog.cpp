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

#include "PvZ/Lawn/Widget/HelpOptionsDialog.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/Widget/GameButton.h"
#include "PvZ/MagicAddr.h"
#include "PvZ/Misc.h"

void HelpOptionsDialog_ButtonDepress(HelpOptionsDialog *a, int a2) {
    // 修复在游戏战斗中打开新版暂停菜单时可以切换用户
    if (a2 == 1) {
        if (isMainMenu)
            a->mApp->DoUserDialog();
        else
            a->mApp->LawnMessageBox(Dialogs::DIALOG_MESSAGE, "[DIALOG_WARNING]", "[CHANGE_USER_FORBID]", "[DIALOG_BUTTON_OK]", "", 3);
        return;
    }
    // if( thePlayerIndex == 0){
    // int * lawnApp = (int*)a[HELPOPTIONS_LAWNAPP_OFFSET];
    // GameMode::GameMode mGameMode = (GameMode::GameMode) *(lawnApp + LAWNAPP_GAMEMODE_OFFSET);
    // if (mGameMode >= GameMode::GAMEMODE_MP_VS && mGameMode < GameMode::GAMEMODE_TWO_PLAYER_COOP_ENDLESS) {
    // LawnApp_ShowHelpTextScreen(lawnApp, 2);
    // } else {
    // LawnApp_ShowHelpTextScreen(lawnApp, 0);
    // }
    // return;
    // }
    old_HelpOptionsDialog_ButtonDepress(a, a2);
}

void HelpOptionsDialog_HelpOptionsDialog(HelpOptionsDialog *a, LawnApp *a2) {
    // 在战斗界面去除“切换用户”按钮
    old_HelpOptionsDialog_HelpOptionsDialog(a, a2);
    if (!isMainMenu) {
        GameButton *switchUserButton = a->mSwitchUserButton;
        GameButton *settingsButton = a->mSettingsButton;
        int theX = switchUserButton->mX;
        int theY = switchUserButton->mY;
        int theWidth = switchUserButton->mWidth;
        int theHeight = switchUserButton->mHeight;
        switchUserButton->mDisabled = true;
        switchUserButton->mVisible = false;
        switchUserButton->Resize(0, 0, 0, 0);
        settingsButton->Resize(theX, theY, theWidth, theHeight);
    }
}

void HelpOptionsDialog_Resize(HelpOptionsDialog *a, int a2, int a3, int a4, int a5) {
    // 在战斗界面去除“切换用户”按钮
    old_HelpOptionsDialog_Resize(a, a2, a3, a4, a5);
    if (!isMainMenu) {
        GameButton *switchUserButton = a->mSwitchUserButton;
        GameButton *settingsButton = a->mSettingsButton;
        int theX = switchUserButton->mX;
        int theY = switchUserButton->mY;
        int theWidth = switchUserButton->mWidth;
        int theHeight = switchUserButton->mHeight;
        switchUserButton->mDisabled = true;
        switchUserButton->mVisible = false;
        switchUserButton->Resize(0, 0, 0, 0);
        settingsButton->Resize(theX, theY, theWidth, theHeight);
    }
}
