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

#include "PvZ/Lawn/Board/CutScene.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Lawn/LawnApp.h"

void CutScene::ShowShovel() {
    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BUTTERED_POPCORN) {
        // 不绘制锤子铲子黄油按钮
        mBoard->mShowShovel = false;
        mBoard->mShowButter = false;
        mBoard->mShowHammer = false;
        return;
    }

    return old_CutScene_ShowShovel(this);
}


void CutScene::Update() {
    return old_CutScene_Update(this);
}
