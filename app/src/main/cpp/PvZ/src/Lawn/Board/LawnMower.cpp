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

#include "PvZ/Lawn/Board/LawnMower.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/LawnApp.h"

void LawnMower::Update() {
    if (!requestPause) {
        old_LawnMower_Update(this);
    }
}

void LawnMower::StartMower() {
    if (mApp->IsVSMode() && mApp->mGameScene == SCENE_PLAYING) {
        if (tcp_connected)
            return;

        if (tcpClientSocket >= 0) {
            U16_Event event = {{EventType::EVENT_SERVER_BOARD_LAWNMOWER_START}, uint16_t(mRow)};
            sendWithSize(tcpClientSocket, &event, sizeof(U16_Event), 0);
        }
    }

    old_LawnMower_StartMower(this);
}
