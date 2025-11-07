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

#include "PvZ/Lawn/Widget/SettingsDialog.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/Widget/GameButton.h"
#include "PvZ/Misc.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Common/TodStringFile.h"

using namespace Sexy;

namespace {
Sexy::Checkbox *g3DAccleratedCheckbox;
Sexy::Checkbox *gVibrateCheckbox;
} // namespace


void SettingsDialog_AddedToManager(SettingsDialog *settingsDialog, int *manager) {
    old_SettingsDialog_AddedToManager(settingsDialog, manager);
    LawnApp *lawnApp = (LawnApp *)*gLawnApp_Addr;
    Sexy::__Widget *mSoundSlider = settingsDialog->mSoundSlider;
    Sexy::__Widget *mBackButton = settingsDialog->mBackButton;


    g3DAccleratedCheckbox = MakeNewCheckbox(1024, &settingsDialog->mCheckboxListener, settingsDialog, lawnApp->Is3DAccelerated());
    gVibrateCheckbox = MakeNewCheckbox(1025, &settingsDialog->mCheckboxListener, settingsDialog, !lawnApp->mPlayerInfo->mIsVibrateClosed);

    g3DAccleratedCheckbox->Resize(80, 260, 300, 50);
    gVibrateCheckbox->Resize(80, 320, 300, 50);

    settingsDialog->AddWidget(g3DAccleratedCheckbox);
    settingsDialog->AddWidget(gVibrateCheckbox);

    mSoundSlider->mFocusLinks[1] = g3DAccleratedCheckbox;
    g3DAccleratedCheckbox->mFocusLinks[1] = gVibrateCheckbox;
    gVibrateCheckbox->mFocusLinks[1] = mBackButton;

    mBackButton->mFocusLinks[0] = gVibrateCheckbox;
    gVibrateCheckbox->mFocusLinks[0] = g3DAccleratedCheckbox;
    g3DAccleratedCheckbox->mFocusLinks[0] = mSoundSlider;
}

void SettingsDialog_RemovedFromManager(SettingsDialog *settingsDialog, int *manager) {
    old_SettingsDialog_RemovedFromManager(settingsDialog, manager);
    settingsDialog->RemoveWidget(g3DAccleratedCheckbox);
    settingsDialog->RemoveWidget(gVibrateCheckbox);
}

void SettingsDialog_Delete2(SettingsDialog *settingsDialog) {
    old_SettingsDialog_Delete2(settingsDialog);
    // Sexy_Checkbox_Delete(g3DAccleratedCheckbox); // 在安卓4.2上，这么Delete会闪退
    (*((void (**)(Sexy::__Widget *))g3DAccleratedCheckbox->vTable + 1))(g3DAccleratedCheckbox); // Delete() ，用这种方式Delete在安卓4.2上就不会闪退，虽然我也不知道为什么会这样
    g3DAccleratedCheckbox = nullptr;
    (*((void (**)(Sexy::__Widget *))gVibrateCheckbox->vTable + 1))(gVibrateCheckbox); // Delete() ，用这种方式Delete在安卓4.2上就不会闪退，虽然我也不知道为什么会这样
    gVibrateCheckbox = nullptr;
}

void SettingsDialog_Draw(SettingsDialog *settingsDialog, Sexy::Graphics *g) {
    old_SettingsDialog_Draw(settingsDialog, g);

    Color color = {107, 110, 145, 255};
    if (settingsDialog->mFocusedChildWidget == g3DAccleratedCheckbox) {
        color.mRed = 0;
        color.mGreen = 255;
        color.mBlue = 0;
        color.mAlpha = 255;
    }
    pvzstl::string str = TodStringTranslate("[OPTIONS_3D_ACCELERATION]");
    g->SetFont(*Sexy_FONT_DWARVENTODCRAFT18_Addr);
    g->SetColor(color);
    g->DrawString(str, g3DAccleratedCheckbox->mX + 80, g3DAccleratedCheckbox->mY + 20);

    Color color1 = {107, 110, 145, 255};
    if (settingsDialog->mFocusedChildWidget == gVibrateCheckbox) {
        color1.mRed = 0;
        color1.mGreen = 255;
        color1.mBlue = 0;
        color1.mAlpha = 255;
    }
    pvzstl::string str1 = TodStringTranslate("[OPTIONS_VIBRATE]");
    g->SetFont(*Sexy_FONT_DWARVENTODCRAFT18_Addr);
    g->SetColor(color1);
    g->DrawString(str1, gVibrateCheckbox->mX + 80, gVibrateCheckbox->mY + 20);
}

void SettingsDialog_CheckboxChecked(SettingsDialog *settingsDialog, int id, bool isChecked) {
    switch (id) {
        case 1024: {
            LawnApp *lawnApp = (LawnApp *)*gLawnApp_Addr;
            lawnApp->Set3DAccelerated(isChecked);
        } break;
        case 1025: {
            LawnApp *lawnApp = (LawnApp *)*gLawnApp_Addr;
            lawnApp->mPlayerInfo->mIsVibrateClosed = !isChecked;
        } break;
    }
}
