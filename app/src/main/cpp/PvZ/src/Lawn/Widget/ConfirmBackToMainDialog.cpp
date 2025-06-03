#include "PvZ/Lawn/Widget/ConfirmBackToMainDialog.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Misc.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/SexyAppFramework/Widget/GameButton.h"

void ConfirmBackToMainDialog_MouseDrag(ConfirmBackToMainDialog *confirmBackToMainDialog, int x, int y) {
    //  修复拖动此Dialog后按钮错位。为三个Button做Resize即可。
    old_ConfirmBackToMainDialog_MouseDrag(confirmBackToMainDialog, x, y);
    int theButtonXStart = (confirmBackToMainDialog->mWidth - 509) / 2;
    int mLawnYesButtonY = confirmBackToMainDialog->mLawnYesButton->mY;
    confirmBackToMainDialog->mLawnYesButton->Resize(theButtonXStart, mLawnYesButtonY, 163, 46);
    confirmBackToMainDialog->mRestartButton->Resize(theButtonXStart + 173, mLawnYesButtonY, 163, 46);
    confirmBackToMainDialog->mLawnNoButton->Resize(theButtonXStart + 346, mLawnYesButtonY, 163, 46);
}

void ConfirmBackToMainDialog_AddedToManager(ConfirmBackToMainDialog *confirmBackToMainDialog, int a2) {
    old_ConfirmBackToMainDialog_AddedToManager(confirmBackToMainDialog, a2);
}

void ConfirmBackToMainDialog_ButtonDepress(ConfirmBackToMainDialog *a, int a2) {
    // 修复重新开始时不关闭NEWOPTIONS
    old_ConfirmBackToMainDialog_ButtonDepress(a, a2);
    if (a2 == 1) {
        a->mApp->KillDialog(Dialogs::DIALOG_NEWOPTIONS);
    }
}
