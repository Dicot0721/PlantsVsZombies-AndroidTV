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

#include "PvZ/Lawn/Widget/MailScreen.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/Widget/GameButton.h"
#include "PvZ/Misc.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/SexyAppFramework/Misc/KeyCodes.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Common/TodStringFile.h"

namespace {
GameButton *gMailScreenCloseButton;
GameButton *gMailScreenReadButton;
GameButton *gMailScreenSwitchButton;
} // namespace

MailScreen::MailScreen(LawnApp *theApp) {
    _constructor(theApp);
}

void MailScreen::_constructor(LawnApp *theApp) {
    // 修复MailScreen的可触控区域不为全屏。
    old_MailScreen_MailScreen(this, theApp);

    pvzstl::string str2 = TodStringTranslate("[MARK_MESSAGE_READ]");
    gMailScreenReadButton = MakeButton(1002, &mButtonListener, this, str2);
    gMailScreenReadButton->Resize(-150, 450, 170, 80);
    AddWidget(gMailScreenReadButton);

    pvzstl::string str1 = TodStringTranslate("[GO_TO_READ_MAIL]");
    gMailScreenSwitchButton = MakeButton(1001, &mButtonListener, this, str1);
    gMailScreenSwitchButton->Resize(-150, 520, 170, 80);
    AddWidget(gMailScreenSwitchButton);


    pvzstl::string str = TodStringTranslate("[CLOSE]");
    gMailScreenCloseButton = MakeButton(1000, &mButtonListener, this, str);
    gMailScreenCloseButton->Resize(800, 520, 170, 80);
    AddWidget(gMailScreenCloseButton);

    Resize(0, 0, 800, 600);
}

void MailScreen::AddedToManager(int *theWidgetManager) {
    old_MailScreen_AddedToManager(this, theWidgetManager);
    // Sexy_Widget_Resize(mailScreen, -240, -60, 1280, 720);
}

void MailScreen::RemovedFromManager(int *widgetManager) {
    // 修复MailScreen的可触控区域不为全屏
    RemoveWidget(gMailScreenCloseButton);
    RemoveWidget(gMailScreenReadButton);
    RemoveWidget(gMailScreenSwitchButton);

    old_MailScreen_RemovedFromManager(this, widgetManager);
}

void MailScreen::__Destructor2() {
    old_MailScreen_Delete2(this);

    gMailScreenCloseButton->~GameButton();
    gMailScreenCloseButton = nullptr;
    gMailScreenReadButton->~GameButton();
    gMailScreenReadButton = nullptr;
    gMailScreenSwitchButton->~GameButton();
    gMailScreenSwitchButton = nullptr;
}

void MailScreen::ButtonPress(int theId) {
    old_MailScreen_ButtonPress(this, theId);
}

void MailScreen::ButtonDepress(int theId) {
    mApp = *gLawnApp_Addr;
    MailScreen *aRealMailScreen = (MailScreen *)mApp->GetDialog(Dialogs::DIALOG_MAIL);
    if (theId == 1002) {
        aRealMailScreen->KeyDown(Sexy::KEYCODE_RETURN, 0, 0);
    } else if (theId == 1001) {
        aRealMailScreen->KeyDown(307, 0, 0);
        bool isAtInBox = aRealMailScreen->mPage == 0;
        gMailScreenReadButton->mDisabled = !isAtInBox;
        gMailScreenReadButton->mBtnNoDraw = !isAtInBox;
        pvzstl::string str = TodStringTranslate(isAtInBox ? "[GO_TO_READ_MAIL]" : "[GO_TO_INBOX]");
        gMailScreenSwitchButton->SetLabel(str);
    } else
        old_MailScreen_ButtonDepress(this, theId);
}


namespace {
constexpr int MAIL_TRIGGER_DISTANCE = 20;
int mMailTouchDownX;
int mMailTouchDownY;
} // namespace

void MailScreen::MouseDown(int x, int y, int theClickCount) {
    mMailTouchDownX = x;
    mMailTouchDownY = y;
}

void MailScreen::MouseDrag(int x, int y) {}

void MailScreen::MouseUp(int x, int y) {
    const int distance = x - mMailTouchDownX;
    if (distance > MAIL_TRIGGER_DISTANCE) {
        KeyDown(37, 0, 0);
    } else if (-distance > MAIL_TRIGGER_DISTANCE) {
        KeyDown(39, 0, 0);
    } else if (mMailTouchDownX < 400) {
        KeyDown(37, 0, 0);
    } else {
        KeyDown(39, 0, 0);
    }
}
