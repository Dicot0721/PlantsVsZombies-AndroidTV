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

#ifndef PVZ_LAWN_WIDGET_HELP_TEXT_SCREEN_H
#define PVZ_LAWN_WIDGET_HELP_TEXT_SCREEN_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/SexyAppFramework/Widget/Widget.h"

class HelpTextScreen : public Sexy::__Widget {
public:
    void KeyDown(Sexy::KeyCode theKey) {
        reinterpret_cast<void (*)(HelpTextScreen *, Sexy::KeyCode)>(HelpTextScreen_KeyDownAddr)(this, theKey);
    }

    void MouseDown(int x, int y, int theClickCount);
};


inline void (*old_HelpTextScreen_Update)(HelpTextScreen *helpTextScreen);

inline void (*old_HelpTextScreen_Draw)(HelpTextScreen *helpTextScreen, Sexy::Graphics *graphics);

inline void (*old_HelpTextScreen_HelpTextScreen)(HelpTextScreen *helpTextScreen, int *lawnApp, int pageIndex);

inline void (*old_HelpTextScreen_AddedToManager)(HelpTextScreen *helpTextScreen, int *manager);

inline void (*old_HelpTextScreen_MouseDown)(HelpTextScreen *helpTextScreen, int x, int y, int theClickCount);

inline void (*old_HelpTextScreen_RemovedFromManager)(HelpTextScreen *helpTextScreen, int *widgetManager);

inline void (*old_HelpTextScreen_Delete2)(HelpTextScreen *helpTextScreen);

inline void (*old_HelpTextScreen_ButtonDepress)(HelpTextScreen *helpTextScreen, int);


void HelpTextScreen_Update(HelpTextScreen *helpTextScreen);

void HelpTextScreen_Draw(HelpTextScreen *helpTextScreen, Sexy::Graphics *graphics);

void HelpTextScreen_HelpTextScreen(HelpTextScreen *helpTextScreen, int *lawnApp, int pageIndex);

void HelpTextScreen_AddedToManager(HelpTextScreen *helpTextScreen, int *manager);

void HelpTextScreen_MouseDown(HelpTextScreen *helpTextScreen, int x, int y, int theClickCount);

void HelpTextScreen_RemovedFromManager(HelpTextScreen *helpTextScreen, int *widgetManager);

void HelpTextScreen_Delete2(HelpTextScreen *helpTextScreen);

void HelpTextScreen_ButtonDepress(HelpTextScreen *helpTextScreen, int id);

#endif // PVZ_LAWN_WIDGET_HELP_TEXT_SCREEN_H
