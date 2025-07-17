//
// Created by 28636 on 2025/7/17.
//

#include "PvZ/Lawn/Widget/WaitForSecondPlayerDialog.h"

void WaitForSecondPlayerDialog::Create(int *a2) {
    // 自动跳过等待2P对话框
    old_WaitForSecondPlayerDialog_WaitForSecondPlayerDialog(this, a2);

    GameButtonDown(ButtonCode::BUTTONCODE_A, 1);
    GameButtonDown(ButtonCode::BUTTONCODE_A, 1);
}