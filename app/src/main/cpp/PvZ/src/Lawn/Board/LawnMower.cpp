//
// Created by 28636 on 2025/5/24.
//

#include "PvZ/Lawn/Board/LawnMower.h"
#include "PvZ/GlobalVariable.h"

void LawnMower::Update() {
    if (requestPause) {
        return;
    }

    old_LawnMower_Update(this);
}