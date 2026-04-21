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

#include "PvZ/Lawn/Widget/SettingsDialog.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Lawn/Common/LawnCommon.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/Widget/GameButton.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/SexyAppFramework/Widget/Checkbox.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Common/TodStringFile.h"

using namespace Sexy;

void SettingsDialog::_constructor(LawnApp *theApp) {
    old_SettingsDialog__constructor(this, theApp);

    mHardwareAccelerationCheckbox = MakeNewCheckbox(SettingsDialog::SettingsDialog_HardwareAcceleration, &mCheckboxListener, this, theApp->Is3DAccelerated());
    mHapticFeedbackCheckbox = MakeNewCheckbox(SettingsDialog::SettingsDialog_HapticFeedback, &mCheckboxListener, this, !theApp->mPlayerInfo->mIsHapticFeedbackClosed);

    mHardwareAccelerationCheckbox->Resize(80, 260, 300, 50);
    mHapticFeedbackCheckbox->Resize(80, 320, 300, 50);

    mSoundSlider->mFocusLinks[1] = mHardwareAccelerationCheckbox;
    mHardwareAccelerationCheckbox->mFocusLinks[1] = mHapticFeedbackCheckbox;
    mHapticFeedbackCheckbox->mFocusLinks[1] = mBackButton;

    mBackButton->mFocusLinks[0] = mHapticFeedbackCheckbox;
    mHapticFeedbackCheckbox->mFocusLinks[0] = mHardwareAccelerationCheckbox;
    mHardwareAccelerationCheckbox->mFocusLinks[0] = mSoundSlider;
}

void SettingsDialog::AddedToManager(WidgetManager *theWidgetManager) {
    old_SettingsDialog_AddedToManager(this, theWidgetManager);

    AddWidget(mHardwareAccelerationCheckbox);
    AddWidget(mHapticFeedbackCheckbox);
}

void SettingsDialog::RemovedFromManager(WidgetManager *theWidgetManager) {
    old_SettingsDialog_RemovedFromManager(this, theWidgetManager);

    RemoveWidget(mHardwareAccelerationCheckbox);
    RemoveWidget(mHapticFeedbackCheckbox);
}

void SettingsDialog::_destructor() {
    old_SettingsDialog__destructor(this);

    delete mHardwareAccelerationCheckbox;
    delete mHapticFeedbackCheckbox;
}

void SettingsDialog::Draw(Sexy::Graphics *g) {
    old_SettingsDialog_Draw(this, g);

    if (mHardwareAccelerationCheckbox) {
        const Color color = (mFocusedChildWidget == mHardwareAccelerationCheckbox) ? Color(0, 255, 0) : Color(107, 110, 145);
        g->SetFont(*Sexy_FONT_DWARVENTODCRAFT18_Addr);
        g->SetColor(color);
        g->DrawString(TodStringTranslate("[OPTIONS_3D_ACCELERATION]"), mHardwareAccelerationCheckbox->mX + 80, mHardwareAccelerationCheckbox->mY + 20);
    }

    if (mHapticFeedbackCheckbox) {
        const Color color = (mFocusedChildWidget == mHapticFeedbackCheckbox) ? Color(0, 255, 0) : Color(107, 110, 145);
        g->SetFont(*Sexy_FONT_DWARVENTODCRAFT18_Addr);
        g->SetColor(color);
        g->DrawString(TodStringTranslate("[OPTIONS_HAPTIC_FEEDBACK]"), mHapticFeedbackCheckbox->mX + 80, mHapticFeedbackCheckbox->mY + 20);
    }
}

void SettingsDialog::CheckboxChecked(int theId, bool isChecked) {
    LawnApp *lawnApp = *gLawnApp_Addr;

    switch (theId) {
        case SettingsDialog::SettingsDialog_HardwareAcceleration:
            lawnApp->Set3DAccelerated(isChecked);
            break;
        case SettingsDialog::SettingsDialog_HapticFeedback:
            lawnApp->mPlayerInfo->mIsHapticFeedbackClosed = !isChecked;
            break;
        default:
            break;
    }
}
