/*
 * Copyright (C) 2023-2026  PvZ TV Touch Team
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

#include "PvZ/Lawn/Widget/ImitaterDialog.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/Widget/SeedChooserScreen.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/SexyAppFramework/Misc/KeyCodes.h"

void ImitaterDialog_ImitaterDialog(ImitaterDialog *instance, int playerIndex) {
    // 记录当前游戏状态
    old_ImitaterDialog_ImitaterDialog(instance, playerIndex);
}

void ImitaterDialog_ShowToolTip(ImitaterDialog *instance) {
    // 在触控模式下不显示ToolTip
    if (!keyboardMode)
        return;
    old_ImitaterDialog_ShowToolTip(instance);
}

bool ImitaterDialog_KeyDown(ImitaterDialog *a, int a2) {
    // 修复不选择模仿者卡片并退出时的闪退
    if (a2 == Sexy::KEYCODE_ESCAPE || a2 == Sexy::KEYCODE_GAMEPAD_B) {
        LawnApp *lawnApp = a->mApp;
        SeedChooserScreen *seedChooserScreen = lawnApp->mSeedChooserScreen;
        seedChooserScreen->RemoveWidget(a);
        (*(void (**)(LawnApp *, Sexy::Widget *))(*(uint32_t *)lawnApp + 188))(lawnApp, a);
        seedChooserScreen->mImitaterDialogOpened = 0;
        return true;
    }
    return old_ImitaterDialog_KeyDown(a, a2);
}

void ImitaterDialog::MouseDown(int x, int y, int theCount) {
    SeedType aSeedType = SeedHitTest(x, y);
    if (aSeedType != SeedType::SEED_NONE) {
        SeedChooserScreen *aSeedChooser = mApp->mSeedChooserScreen;
        if (!aSeedChooser->SeedNotAllowedToPick(aSeedType)) {
            ChosenSeed &aImitater = aSeedChooser->mChosenSeeds[SeedType::SEED_IMITATER];
            aImitater.mImitaterType = aSeedType;
            aImitater.mSeedState = ChosenSeedState::SEED_IN_CHOOSER;
            aSeedChooser->GetSeedPositionInChooser(SeedType::SEED_IMITATER, aImitater.mX, aImitater.mY);
            // SeedChooserScreen_ClickedSeedInChooser(*(uint32_t *) (a[184] + 2244), aSeedChooser + 835, -1);//模仿者选种子闪退，就是因为这里参数为-1而不是0或者1
            // int mIsZombie = a[194];
            aSeedChooser->ClickedSeedInChooser(aImitater, m1PChoosingSeeds ? 0 : 1);
            aSeedChooser->UpdateImitaterButton();
            // (*(void (**)(int, int)) (*(uint32_t *) a[184] + 428))(a[184], a[179]);
            aSeedChooser->RemoveWidget(this);
            (*(void (**)(LawnApp *, Sexy::Widget *))(*(uint32_t *)mApp + 188))(mApp, this);
            aSeedChooser->mImitaterDialogOpened = 0;
        }
    }
}
