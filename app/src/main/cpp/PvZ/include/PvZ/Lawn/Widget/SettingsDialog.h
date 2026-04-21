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

#ifndef PVZ_LAWN_WIDGET_SETTINGS_DIALOG_H
#define PVZ_LAWN_WIDGET_SETTINGS_DIALOG_H

#include "LawnDialog.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/SexyAppFramework/Widget/CheckboxListener.h"
#include "PvZ/SexyAppFramework/Widget/SliderListener.h"

namespace Sexy {
class Checkbox;
}

class SettingsDialog : public LawnDialog {
private:
    enum {
        SettingsDialog_HardwareAcceleration = 1024,
        SettingsDialog_HapticFeedback,
    };

public:
    Sexy::SliderListener *mSliderListener;    // 191
    Sexy::CheckboxListener mCheckboxListener; // 192
    LawnApp *mApp;                            // 193
    Sexy::Widget *mMusicSlider;               // 194
    Sexy::Widget *mSoundSlider;               // 195
    GameButton *mBackButton;                  // 196
    GameButton *mSelectDeviceButton;          // 197
    int unk[5];                               // 198 ~ 202
    // 115: 203, 111: 205
    Sexy::Checkbox *mHardwareAccelerationCheckbox = nullptr;
    Sexy::Checkbox *mHapticFeedbackCheckbox = nullptr;

    SettingsDialog(LawnApp *theApp) {
        _constructor(theApp);
    }
    ~SettingsDialog() {
        _destructor();
    };

    void AddedToManager(Sexy::WidgetManager *manager);
    void RemovedFromManager(Sexy::WidgetManager *manager);
    void Draw(Sexy::Graphics *g);
    void CheckboxChecked(int theId, bool isChecked);

protected:
    friend void InitHookFunction();

    void _constructor(LawnApp *theApp);
    void _destructor();
};

inline void (*old_SettingsDialog_AddedToManager)(SettingsDialog *settingsDialog, Sexy::WidgetManager *manager);

inline void (*old_SettingsDialog_RemovedFromManager)(SettingsDialog *settingsDialog, Sexy::WidgetManager *manager);

inline void (*old_SettingsDialog__constructor)(SettingsDialog *settingsDialog, LawnApp *theApp);

inline void (*old_SettingsDialog__destructor)(SettingsDialog *settingsDialog);

inline void (*old_SettingsDialog_Draw)(SettingsDialog *settingsDialog, Sexy::Graphics *graphics);

#endif // PVZ_LAWN_WIDGET_SETTINGS_DIALOG_H
