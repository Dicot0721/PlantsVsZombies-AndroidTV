#include "PvZ/Lawn/Widget/MailScreen.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Misc.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/SexyAppFramework/Misc/KeyCodes.h"
#include "PvZ/SexyAppFramework/Widget/GameButton.h"
#include "PvZ/Symbols.h"

namespace {
Sexy::GameButton *gMailScreenCloseButton;
Sexy::GameButton *gMailScreenReadButton;
Sexy::GameButton *gMailScreenSwitchButton;
} // namespace

MailScreen::MailScreen(LawnApp *theApp) {
    Create(theApp);
}

void MailScreen::Create(LawnApp *theApp) {
    // 修复MailScreen的可触控区域不为全屏。
    old_MailScreen_MailScreen(this, theApp);

    int holder2[1];
    TodStringTranslate(holder2, "[MARK_MESSAGE_READ]");
    gMailScreenReadButton = MakeButton(1002, &mButtonListener, this, (SexyString &)holder2);
    gMailScreenReadButton->Resize(-150, 450, 170, 80);
    Sexy_String_Delete(holder2);
    AddWidget((Sexy::Widget*)gMailScreenReadButton);

    int holder1[1];
    TodStringTranslate(holder1, "[GO_TO_READ_MAIL]");
    gMailScreenSwitchButton = MakeButton(1001, &mButtonListener, this, (SexyString &)holder1);
    gMailScreenSwitchButton->Resize(-150, 520, 170, 80);
    Sexy_String_Delete(holder1);
    AddWidget((Sexy::Widget*)gMailScreenSwitchButton);


    int holder[1];
    TodStringTranslate(holder, "[CLOSE]");
    gMailScreenCloseButton = MakeButton(1000, &mButtonListener, this, (SexyString &)holder);
    gMailScreenCloseButton->Resize(800, 520, 170, 80);
    Sexy_String_Delete(holder);
    AddWidget((Sexy::Widget*)gMailScreenCloseButton);

    Resize(0, 0, 800, 600);
}

void MailScreen::AddedToManager(int *theWidgetManager) {
    old_MailScreen_AddedToManager(this, theWidgetManager);
    //    Sexy_Widget_Resize(mailScreen, -240, -60, 1280, 720);
}

void MailScreen::RemovedFromManager(int *widgetManager) {
    // 修复MailScreen的可触控区域不为全屏
    RemoveWidget((Sexy::Widget*)gMailScreenCloseButton);
    RemoveWidget((Sexy::Widget*)gMailScreenReadButton);
    RemoveWidget((Sexy::Widget*)gMailScreenSwitchButton);

    old_MailScreen_RemovedFromManager(this, widgetManager);
}

void MailScreen::Delete2() {
    old_MailScreen_Delete2(this);

    gMailScreenCloseButton->Destroy();
    gMailScreenCloseButton = nullptr;
    gMailScreenReadButton->Destroy();
    gMailScreenReadButton = nullptr;
    gMailScreenSwitchButton->Destroy();;
    gMailScreenSwitchButton = nullptr;
}

void MailScreen::ButtonPress(int theId) {
    old_MailScreen_ButtonPress(this, theId);
}

void MailScreen::ButtonDepress(int theId) {
    mApp = (LawnApp *)*gLawnApp_Addr;
    MailScreen *aRealMailScreen = (MailScreen *)mApp->GetDialog(Dialogs::DIALOG_MAIL);
    if (theId == 1002) {
        aRealMailScreen->KeyDown(Sexy::KEYCODE_ACCEPT, 0, 0);
    } else if (theId == 1001) {
        aRealMailScreen->KeyDown(307, 0, 0);
        bool isAtInBox = aRealMailScreen->mPage == 0;
        gMailScreenReadButton->mDisabled = !isAtInBox;
        gMailScreenReadButton->mBtnNoDraw = !isAtInBox;
        int holder[1];
        TodStringTranslate(holder, isAtInBox ? "[GO_TO_READ_MAIL]" : "[GO_TO_INBOX]");
        gMailScreenSwitchButton->SetLabel((SexyString&)holder);
        Sexy_String_Delete(holder);
    } else
        old_MailScreen_ButtonDepress(this, theId);
}


namespace {
constexpr int mMailTrigger = 20;
int mMailTouchDownX;
int mMailTouchDownY;
} // namespace

void MailScreen::MouseDown(int x, int y, int theClickCount) {
    mMailTouchDownX = x;
    mMailTouchDownY = y;
}

void MailScreen::MouseDrag(int x, int y) {}

void MailScreen::MouseUp(int x, int y) {
    if (mMailTouchDownX - x > mMailTrigger) {
        KeyDown(39, 0, 0);
    } else if (x - mMailTouchDownX > mMailTrigger) {
        KeyDown(37, 0, 0);
    } else if (mMailTouchDownX < 400) {
        KeyDown(37, 0, 0);
    } else {
        KeyDown(39, 0, 0);
    }
}
