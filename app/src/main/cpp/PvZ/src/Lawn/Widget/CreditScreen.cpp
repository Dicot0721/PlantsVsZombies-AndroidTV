//
// Created by 28636 on 2025/5/26.
//

#include "PvZ/Lawn/Widget/CreditScreen.h"
#include "PvZ/Misc.h"
#include "PvZ/Lawn/LawnApp.h"

void CreditScreen_CreditScreen(Sexy::Widget *creditScreen, LawnApp *a2, bool a3) {
    old_CreditScreen_CreditScreen(creditScreen, a2, a3);
    int holder[1];
    TodStringTranslate(holder, "[MAIN_MENU_BUTTON]");
    gCreditScreenBackButton = MakeButton(1000, (Sexy::ButtonListener *)creditScreen + 64, creditScreen, (SexyString &)holder);
    gCreditScreenBackButton->Resize(725, 0, 170, 50);
    Sexy_String_Delete(holder);
    creditScreen->AddWidget((Sexy::Widget*)gCreditScreenBackButton);
}

void CreditScreen_RemovedFromManager(Sexy::Widget *creditScreen, int a2) {
    creditScreen->mFocusedChildWidget = (Sexy::Widget*)gCreditScreenBackButton; // 修复触摸CreditScreen后点击按钮退出就会闪退的BUG,虽然不知道为什么
    creditScreen->RemoveWidget((Sexy::Widget*)gCreditScreenBackButton);
    return old_CreditScreen_RemovedFromManager(creditScreen, a2);
}

void CreditScreen_Delete2(Sexy::Widget *creditScreen) {
    old_CreditScreen_Delete2(creditScreen);

    gCreditScreenBackButton->Destroy();
    gCreditScreenBackButton = nullptr;
}

void CreditScreen_ButtonDepress(Sexy::Widget *creditScreen, int id) {
    if (id == 1000) {
        LawnApp *lawnApp = (LawnApp *)*gLawnApp_Addr;
        CreditScreen_PauseCredits(lawnApp->mCreditScreen);
    }
}