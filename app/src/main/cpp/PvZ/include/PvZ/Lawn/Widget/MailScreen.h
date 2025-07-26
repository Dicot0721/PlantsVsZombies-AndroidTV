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

#ifndef PVZ_LAWN_WIDGET_MAIL_SCREEN_H
#define PVZ_LAWN_WIDGET_MAIL_SCREEN_H

#include "LawnDialog.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Symbols.h"

class MailScreen : public LawnDialog {
public:
    int unk[3];   // 191 ~ 193
    int mPage;    // 194
    int unk2[12]; // 195 ~ 206
    // 115: 207, 111: 209

    void KeyDown(int theKey, int a2, int a3) {
        reinterpret_cast<void (*)(MailScreen *, int, int, int)>(MailScreen_KeyDownAddr)(this, theKey, a2, a3);
    }

    MailScreen(LawnApp *theApp);
    void AddedToManager(int *theWidgetManager);
    void RemovedFromManager(int *widgetManager);

    void MouseDown(int x, int y, int theClickCount);
    void MouseUp(int x, int y);
    void MouseDrag(int x, int y);
    void ButtonPress(int theId);
    void ButtonDepress(int theId);

protected:
    friend void InitHookFunction();

    void __Constructor(LawnApp *theApp);
    void __Destructor2();
};

class MailBox {
public:
    int GetNumUnseenMessages() {
        return reinterpret_cast<int (*)(MailBox *)>(Mailbox_GetNumUnseenMessagesAddr)(this);
    }
};

// inline void (*MailScreen_KeyDown)(MailScreen *mailScreen, int keyCode, int, int);


inline void (*old_MailScreen_MailScreen)(MailScreen *mailScreen, LawnApp *lawnApp);

inline void (*old_MailScreen_AddedToManager)(MailScreen *mailScreen, int *widgetManager);

inline void (*old_MailScreen_RemovedFromManager)(MailScreen *mailScreen, int *widgetManager);

inline void (*old_MailScreen_Delete2)(MailScreen *mailScreen);

inline void (*old_MailScreen_ButtonPress)(MailScreen *mailScreen, int id);

inline void (*old_MailScreen_ButtonDepress)(MailScreen *mailScreen, int id);

inline void (*old_MailScreen_MouseDown)(MailScreen *mailScreen, int x, int y, int theClickCount);

inline void (*old_MailScreen_MouseDrag)(MailScreen *mailScreen, int x, int y);

inline void (*old_MailScreen_MouseUp)(MailScreen *mailScreen, int x, int y);

#endif // PVZ_LAWN_WIDGET_MAIL_SCREEN_H
