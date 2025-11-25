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

#include "PvZ/Lawn/Widget/HelpTextScreen.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Misc.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Common/TodStringFile.h"

namespace {
constexpr int nextPageButtonX = 836;
constexpr int nextPageButtonY = 318;

constexpr int prevPageButtonX = 2540;
constexpr int prevPageButtonY = 318;

GameButton *gHelpTextScreenCloseButton;
} // namespace


void HelpTextScreen_Update(HelpTextScreen *helpTextScreen) {
    if (gHelpTextScreenCloseButton == nullptr) {
        pvzstl::string str = TodStringTranslate("[CLOSE]");
        gHelpTextScreenCloseButton = MakeButton(1000, (Sexy::ButtonListener *)helpTextScreen + 64, helpTextScreen, str);
        helpTextScreen->AddWidget(gHelpTextScreenCloseButton);
    }
    gHelpTextScreenCloseButton->Resize(650 - helpTextScreen->mX, 540 - helpTextScreen->mY, 170, 50);
    old_HelpTextScreen_Update(helpTextScreen);
}

void HelpTextScreen_Draw(HelpTextScreen *helpTextScreen, Sexy::Graphics *g) {
    old_HelpTextScreen_Draw(helpTextScreen, g);
    g->DrawImage(*Sexy_IMAGE_ZEN_NEXTGARDEN_Addr, nextPageButtonX, nextPageButtonY);
    g->DrawImageMirror(*Sexy_IMAGE_ZEN_NEXTGARDEN_Addr, prevPageButtonX, prevPageButtonY, true);
}

void HelpTextScreen_HelpTextScreen(HelpTextScreen *helpTextScreen, int *lawnApp, int pageIndex) {
    old_HelpTextScreen_HelpTextScreen(helpTextScreen, lawnApp, pageIndex);
    // 这个HelpTextScreen是全屏的，但触控事件并不会分发到此处，而是发给子控件。只有内容外侧的点击事件才能收到。

    helpTextScreen->Resize(helpTextScreen->mX, helpTextScreen->mY, 3000, helpTextScreen->mHeight);
}

void HelpTextScreen_AddedToManager(HelpTextScreen *helpTextScreen, int *manager) {
    // 创建按钮
    old_HelpTextScreen_AddedToManager(helpTextScreen, manager);
}

void HelpTextScreen::MouseDown(int x, int y, int theClickCount) {
    // LOGD("D%d %d", x, y);
    // prevPageButtonX = x;
    // prevPageButtonY = y;

    int imageWidth = (*Sexy_IMAGE_ZEN_NEXTGARDEN_Addr)->GetCelWidth();
    int imageHeight = (*Sexy_IMAGE_ZEN_NEXTGARDEN_Addr)->GetHeight();

    Sexy::Rect nextPageRect = {nextPageButtonX, nextPageButtonY, imageWidth, imageHeight};
    if (TRect_Contains(&nextPageRect, x, y)) {
        KeyDown(Sexy::KeyCode::KEYCODE_RIGHT);
        return;
    }

    Sexy::Rect prevPageRect = {prevPageButtonX, prevPageButtonY, imageWidth, imageHeight};
    if (TRect_Contains(&prevPageRect, x, y)) {
        KeyDown(Sexy::KeyCode::KEYCODE_LEFT);
        return;
    }
}

void HelpTextScreen_RemovedFromManager(HelpTextScreen *helpTextScreen, int *widgetManager) {
    // 修复MailScreen的可触控区域不为全屏
    if (gHelpTextScreenCloseButton != nullptr) {
        helpTextScreen->RemoveWidget(gHelpTextScreenCloseButton);
    }
    old_HelpTextScreen_RemovedFromManager(helpTextScreen, widgetManager);
}

void HelpTextScreen_Delete2(HelpTextScreen *helpTextScreen) {
    old_HelpTextScreen_Delete2(helpTextScreen);
    if (gHelpTextScreenCloseButton != nullptr) {
        gHelpTextScreenCloseButton->~GameButton();
        gHelpTextScreenCloseButton = nullptr;
    }
}

void HelpTextScreen_ButtonDepress(HelpTextScreen *helpTextScreen, int id) {
    if (id == 1000) {
        LawnApp *gLawnApp = *gLawnApp_Addr;
        gLawnApp->KillHelpTextScreen();
    } else
        old_HelpTextScreen_ButtonDepress(helpTextScreen, id);
}
