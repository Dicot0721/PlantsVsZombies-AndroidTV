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

#ifndef PVZ_LAWN_WIDGET_SETTINGS_DIALOG_H
#define PVZ_LAWN_WIDGET_SETTINGS_DIALOG_H

#include "LawnDialog.h"
#include "PvZ/Lawn/Common/ConstEnums.h"

namespace Sexy {
class CheckboxListener;
}

class SettingsDialog : public LawnDialog {
public:
    int *mSliderListener;                      // 191
    Sexy::CheckboxListener *mCheckboxListener; // 192
    LawnApp *mApp;                             // 193
    Sexy::Widget *mMusicSlider;                // 194
    Sexy::Widget *mSoundSlider;                // 195
    Sexy::GameButton *mBackButton;             // 196
    Sexy::GameButton *mSelectDeviceButton;     // 197
    int unk[5];                                // 198 ~ 202
}; // 115: 203, 111: 205

inline void (*old_SettingsDialog_AddedToManager)(SettingsDialog *settingsDialog, int *manager);

inline void (*old_SettingsDialog_RemovedFromManager)(SettingsDialog *settingsDialog, int *manager);

inline void (*old_SettingsDialog_Delete2)(SettingsDialog *settingsDialog);

inline void (*old_SettingsDialog_Draw)(SettingsDialog *settingsDialog, Sexy::Graphics *graphics);


void SettingsDialog_AddedToManager(SettingsDialog *settingsDialog, int *manager);

void SettingsDialog_RemovedFromManager(SettingsDialog *settingsDialog, int *manager);

void SettingsDialog_Delete2(SettingsDialog *settingsDialog);

void SettingsDialog_Draw(SettingsDialog *settingsDialog, Sexy::Graphics *g);

void SettingsDialog_CheckboxChecked(SettingsDialog *settingsDialog, int id, bool isChecked);

#endif // PVZ_LAWN_WIDGET_SETTINGS_DIALOG_H
