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

#ifndef PVZ_LAWN_WIDGET_LAWN_DIALOG_H
#define PVZ_LAWN_WIDGET_LAWN_DIALOG_H

#include "PvZ/SexyAppFramework/Widget/Dialog.h"
// #include "PvZ/SexyAppFramework/Widget/GameButton.h"

namespace Sexy {
class GameButton;
}

class LawnDialog : public Sexy::Dialog {
public:
    LawnApp *mApp; // 184
#if PVZ_VERSION == 111
    int mIsZombie;
#endif
    int mButtonDelay;                 // 185
    Sexy::Widget *mReanimationWidget; // 186
    bool mDrawStandardBack;           // 748
    Sexy::GameButton *mLawnYesButton; // 188
    Sexy::GameButton *mLawnNoButton;  // 189
    bool mTallBottom;                 // 760
    bool mVerticalCenterText;         // 761
    bool unkBool;                     // 762
#if PVZ_VERSION == 111
    int unk2;
#endif
}; // 115: 191, 111: 193

#endif // PVZ_LAWN_WIDGET_LAWN_DIALOG_H
