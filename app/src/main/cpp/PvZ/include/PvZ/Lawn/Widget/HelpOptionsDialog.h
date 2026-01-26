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

#ifndef PVZ_LAWN_WIDGET_HELP_OPTIONS_DIALOG_H
#define PVZ_LAWN_WIDGET_HELP_OPTIONS_DIALOG_H

#include "LawnDialog.h"
#include "PvZ/Lawn/Common/ConstEnums.h"

class HelpOptionsDialog : public __LawnDialog {
public:
    LawnApp *mApp;                 // 191
    GameButton *mHowToPlayButton;  // 192
    GameButton *mSwitchUserButton; // 193
    GameButton *mSettingsButton;   // 194
    GameButton *mCreditsButton;    // 195
    GameButton *mBackButton;       // 196
    GameButton *mUnkButton;        // 197
}; // 115: 198, 111: 200

inline void (*old_HelpOptionsDialog_ButtonDepress)(HelpOptionsDialog *a, int a2);

inline void (*old_HelpOptionsDialog_HelpOptionsDialog)(HelpOptionsDialog *a, LawnApp *a2);

inline void (*old_HelpOptionsDialog_Resize)(HelpOptionsDialog *a, int a2, int a3, int a4, int a5);


void HelpOptionsDialog_ButtonDepress(HelpOptionsDialog *a, int a2);

void HelpOptionsDialog_HelpOptionsDialog(HelpOptionsDialog *a, LawnApp *a2);

void HelpOptionsDialog_Resize(HelpOptionsDialog *a, int a2, int a3, int a4, int a5);

#endif // PVZ_LAWN_WIDGET_HELP_OPTIONS_DIALOG_H
