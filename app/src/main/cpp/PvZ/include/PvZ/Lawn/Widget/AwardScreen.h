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

#ifndef PVZ_LAWN_WIDGET_AWARD_SCREEN_H
#define PVZ_LAWN_WIDGET_AWARD_SCREEN_H

#include "PvZ/SexyAppFramework/Widget/Widget.h"
#include "PvZ/Symbols.h"

namespace Sexy {
class GameButton;
}

class AwardScreen : public Sexy::Widget {
public:
    int unkInt64;                   // 64
    Sexy::GameButton *mStartButton; // 65
    Sexy::GameButton *mMenuButton;  // 66
    LawnApp *mApp;                  // 67
    int mFadeInCounter;             // 68
    AwardType mAwardType;           // 69

    void StartButtonPressed() {
        reinterpret_cast<void (*)(AwardScreen *)>(AwardScreen_StartButtonPressedAddr)(this);
    }

    void MouseDown(int x, int y, int theClickCount);
    void MouseUp(int x, int y, int theClickCount);
};


inline void (*old_AwardScreen_MouseDown)(AwardScreen *awardScreen, int x, int y, int theClickCount);

inline void (*old_AwardScreen_MouseUp)(AwardScreen *awardScreen, int x, int y, int theClickCount);

#endif // PVZ_LAWN_WIDGET_AWARD_SCREEN_H
