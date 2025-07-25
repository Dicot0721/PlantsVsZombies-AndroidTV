//
// Created by 28636 on 2025/7/17.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_WAITFORSECONDPLAYERDIALOG_H
#define PLANTSVSZOMBIES_ANDROIDTV_WAITFORSECONDPLAYERDIALOG_H

#include "PvZ/Lawn/GamepadControls.h"
#include "PvZ/Symbols.h"

class WaitForSecondPlayerDialog {
public:
    void GameButtonDown(ButtonCode theButton, unsigned int thePlayerIndex) {
        reinterpret_cast<void (*)(WaitForSecondPlayerDialog *, ButtonCode, unsigned int)>(WaitForSecondPlayerDialog_GameButtonDownAddr)(this, theButton, thePlayerIndex);
    }

    WaitForSecondPlayerDialog(int *a2) {
        __Constructor(a2);
    }

protected:
    friend void InitHookFunction();

    void __Constructor(int *);
};

inline void (*old_WaitForSecondPlayerDialog_WaitForSecondPlayerDialog)(WaitForSecondPlayerDialog *a, int *a2);

#endif // PLANTSVSZOMBIES_ANDROIDTV_WAITFORSECONDPLAYERDIALOG_H
