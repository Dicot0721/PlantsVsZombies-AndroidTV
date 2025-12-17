//
// Created by Administrator on 2025/12/17.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_NEWOPTIONSDIALOG_H
#define PLANTSVSZOMBIES_ANDROIDTV_NEWOPTIONSDIALOG_H


#include "LawnDialog.h"
class NewOptionsDialog : public __LawnDialog {

public:
    void ButtonDepress(int buttonId);
};
inline void (*old_NewOptionsDialog_ButtonDepress)(NewOptionsDialog *a1, int a2);

#endif // PLANTSVSZOMBIES_ANDROIDTV_NEWOPTIONSDIALOG_H
