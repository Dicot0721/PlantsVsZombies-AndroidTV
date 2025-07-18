#ifndef PVZ_LAWN_CONFIRM_BACK_TO_MAIN_DIALOG_H
#define PVZ_LAWN_CONFIRM_BACK_TO_MAIN_DIALOG_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Lawn/Widget/LawnDialog.h"

class ConfirmBackToMainDialog : public LawnDialog {
public:
    Sexy::GameButton *mRestartButton; // 191
    // 115: 192, 111: 194

    void Create(bool theBool) { reinterpret_cast<void (*)(ConfirmBackToMainDialog *, bool)>(ConfirmBackToMainDialog_ConfirmBackToMainDialogAddr)(this, theBool); }

    ConfirmBackToMainDialog(bool theBool) { Create(theBool); }
};


inline void (*old_ConfirmBackToMainDialog_MouseDrag)(ConfirmBackToMainDialog *confirmBackToMainDialog, int x, int y);

inline void (*old_ConfirmBackToMainDialog_AddedToManager)(ConfirmBackToMainDialog *confirmBackToMainDialog, int a2);

inline void (*old_ConfirmBackToMainDialog_ButtonDepress)(ConfirmBackToMainDialog *a, int a2);


void ConfirmBackToMainDialog_MouseDrag(ConfirmBackToMainDialog *confirmBackToMainDialog, int x, int y);

void ConfirmBackToMainDialog_AddedToManager(ConfirmBackToMainDialog *confirmBackToMainDialog, int a2);

void ConfirmBackToMainDialog_ButtonDepress(ConfirmBackToMainDialog *a, int a2);

#endif // PVZ_LAWN_CONFIRM_BACK_TO_MAIN_DIALOG_H
