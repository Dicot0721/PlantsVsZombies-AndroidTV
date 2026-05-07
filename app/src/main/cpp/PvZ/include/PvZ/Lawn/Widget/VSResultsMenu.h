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

#ifndef PVZ_LAWN_WIDGET_VS_RESULTS_MENU_H
#define PVZ_LAWN_WIDGET_VS_RESULTS_MENU_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/NetPlay.h"
#include "PvZ/SexyAppFramework/Widget/MenuWidget.h"

#include <cstddef>

class VSResultsMenu : public Sexy::MenuWidget {
public:
    enum {
        VSResultsMenu_Play_Again = 0,
        VSResultsMenu_Quit_VS = 1,
    };

    static int (&msPlayerRecords)[2][5];

    int unk[3];                          // 70 ~ 72
    int mSparkleCounter;                 // 73
    int mSmokeCounter;                   // 74
    int mResultsButtonId;                // 75
    int unk2[2];                         // 76 ~ 77
    int mSides[2];                       // 78 ~ 79
    float unk3[2];                       // 80 ~ 81
    ParticleSystemID mSparkleParticleID; // 82
    ParticleSystemID mSmokeParticleID;   // 83
    int mUpdateCounter;                  // 84


    void Update();
    void OnExit();
    void ButtonDepress(int theId);
    void Draw(Sexy::Graphics *g);
    void DrawInfoBox(Sexy::Graphics *a2, int a3);

    void processClientEvent(const BaseEvent *event);
    void processServerEvent(const BaseEvent *event);

    VSResultsMenu() {
        _constructor();
    }
    ~VSResultsMenu() = delete;

    void InitFromBoard(class Board *board) {
        reinterpret_cast<void (*)(VSResultsMenu *, Board *)>(VSResultsMenu_InitFromBoardAddr)(this, board);
    }
    int *GetPlayerRecord(unsigned int playerIndex) {
        return reinterpret_cast<int *(*)(VSResultsMenu *, unsigned int)>(VSResultsMenu_GetPlayerRecordAddr)(this, playerIndex);
    }

protected:
    friend void InitHookFunction();

    void _constructor();
}; // 85个整数

inline int gVSResultRequestState = -1;


inline void (*old_VSResultsMenu_Update)(VSResultsMenu *a);

inline void (*old_VSResultsMenu_Draw)(VSResultsMenu *, Sexy::Graphics *);

inline void (*old_VSResultsMenu_DrawInfoBox)(VSResultsMenu *a, Sexy::Graphics *a2, int a3);

inline void (*old_VSResultsMenu_Constructor)(VSResultsMenu *);

#endif // PVZ_LAWN_WIDGET_VS_RESULTS_MENU_H
