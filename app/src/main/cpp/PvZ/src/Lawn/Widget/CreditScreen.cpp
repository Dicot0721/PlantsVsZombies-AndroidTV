//
// Created by 28636 on 2025/5/26.
//

#include "PvZ/Lawn/Widget/CreditScreen.h"
#include "PvZ/Misc.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/TodLib/Common/TodStringFile.h"

void CreditScreen_CreditScreen(Sexy::Widget *creditScreen, LawnApp *a2, bool a3) {
    old_CreditScreen_CreditScreen(creditScreen, a2, a3);

    pvzstl::string str = TodStringTranslate("[MAIN_MENU_BUTTON]");
    gCreditScreenBackButton = MakeButton(1000, (Sexy::ButtonListener *)creditScreen + 64, creditScreen, str);
    gCreditScreenBackButton->Resize(725, 0, 170, 50);
    creditScreen->AddWidget((Sexy::Widget*)gCreditScreenBackButton);
}

void CreditScreen_RemovedFromManager(Sexy::Widget *creditScreen, int a2) {
    creditScreen->mFocusedChildWidget = (Sexy::Widget*)gCreditScreenBackButton; // 修复触摸CreditScreen后点击按钮退出就会闪退的BUG,虽然不知道为什么
    creditScreen->RemoveWidget((Sexy::Widget*)gCreditScreenBackButton);
    return old_CreditScreen_RemovedFromManager(creditScreen, a2);
}

void CreditScreen_Delete2(Sexy::Widget *creditScreen) {
    old_CreditScreen_Delete2(creditScreen);

    gCreditScreenBackButton->~GameButton();
    gCreditScreenBackButton = nullptr;
}

void CreditScreen::ButtonDepress(int theId) {
    if (theId == 1000) {
        LawnApp *lawnApp = (LawnApp *)*gLawnApp_Addr;
        lawnApp->mCreditScreen->PauseCredits();
    }
}