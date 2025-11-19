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

#include "PvZ/Lawn/Widget/CreditScreen.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Misc.h"
#include "PvZ/TodLib/Common/TodStringFile.h"

void CreditScreen_CreditScreen(Sexy::__Widget *creditScreen, LawnApp *a2, bool a3) {
    old_CreditScreen_CreditScreen(creditScreen, a2, a3);

    pvzstl::string str = TodStringTranslate("[MAIN_MENU_BUTTON]");
    gCreditScreenBackButton = MakeButton(1000, (Sexy::ButtonListener *)creditScreen + 64, creditScreen, str);
    gCreditScreenBackButton->Resize(725, 0, 170, 50);
    creditScreen->AddWidget(gCreditScreenBackButton);
}

void CreditScreen_RemovedFromManager(Sexy::__Widget *creditScreen, int a2) {
    creditScreen->mFocusedChildWidget = gCreditScreenBackButton; // 修复触摸CreditScreen后点击按钮退出就会闪退的BUG,虽然不知道为什么
    creditScreen->RemoveWidget(gCreditScreenBackButton);
    return old_CreditScreen_RemovedFromManager(creditScreen, a2);
}

void CreditScreen_Delete2(Sexy::__Widget *creditScreen) {
    old_CreditScreen_Delete2(creditScreen);

    gCreditScreenBackButton->~GameButton();
    gCreditScreenBackButton = nullptr;
}

void CreditScreen::ButtonDepress(int theId) {
    if (theId == 1000) {
        LawnApp *lawnApp = *gLawnApp_Addr;
        lawnApp->mCreditScreen->PauseCredits();
    }
}