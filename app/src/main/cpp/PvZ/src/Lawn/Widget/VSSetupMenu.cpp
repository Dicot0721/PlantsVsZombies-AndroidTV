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

#include "PvZ/Lawn/Widget/VSSetupMenu.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/LawnApp.h"

void VSSetupMenu::Update() {
    // 记录当前游戏状态
    if (mState == 1) {
        // 自动分配阵营
        GameButtonDown(18, 0, 0);
        GameButtonDown(19, 1, 0);
        GameButtonDown(6, 0, 0);
        GameButtonDown(6, 1, 0);
    }

    return old_VSSetupMenu_Update(this);
}

void VSSetupMenu::KeyDown(Sexy::KeyCode theKey) {
    // 修复在对战的阵营选取界面无法按返回键退出的BUG。
    if (theKey == Sexy::KeyCode::KEYCODE_ESCAPE) {
        switch (mState) {
            case 1:
            case 2:
                mAppBase->DoBackToMain();
                return;
            case 3:
                mAppBase->DoNewOptions(false, 0);
                return;
        }
    }

    return old_VSSetupMenu_KeyDown(this, theKey);
}
