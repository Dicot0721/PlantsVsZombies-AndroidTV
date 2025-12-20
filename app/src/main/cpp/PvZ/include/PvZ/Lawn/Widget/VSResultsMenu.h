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

#ifndef PVZ_LAWN_WIDGET_VS_RESULTS_MENU_H
#define PVZ_LAWN_WIDGET_VS_RESULTS_MENU_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/SexyAppFramework/Widget/MenuWidget.h"
#include "PvZ/SexyAppFramework/Widget/Widget.h"
#include "WaitForSecondPlayerDialog.h"

#include <cstddef>

class VSResultsMenu : public Sexy::MenuWidget {
public:
    int unk[5];           // 70 ~ 74
    int mResultsButtonId; // 75
    int unk2[9];          // 76 ~ 84


    void Update();
    void OnExit();
    void ButtonDepress(int buttonId);
    void DrawInfoBox(Sexy::Graphics *a2, int a3);

    static size_t getClientEventSize(EventType type);
    void processClientEvent(void *buf, ssize_t bufSize);
    static size_t getServerEventSize(EventType type);
    void processServerEvent(void *buf, ssize_t bufSize);
    void HandleTcpServerMessage(void *buf, ssize_t bufSize);
    void HandleTcpClientMessage(void *buf, ssize_t bufSize);
    void InitFromBoard(Board *board) {
        reinterpret_cast<void (*)(VSResultsMenu *, Board *)>(VSResultsMenu_InitFromBoardAddr)(this, board);
    }
}; // 85个整数

inline void (*old_VSResultsMenu_Update)(VSResultsMenu *a);

inline void (*old_VSResultsMenu_OnExit)(VSResultsMenu *a);

inline void (*old_VSResultsMenu_DrawInfoBox)(VSResultsMenu *a, Sexy::Graphics *a2, int a3);


#endif // PVZ_LAWN_WIDGET_VS_RESULTS_MENU_H
