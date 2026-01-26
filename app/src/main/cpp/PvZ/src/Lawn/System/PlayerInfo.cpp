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

#include "PvZ/Lawn/System/PlayerInfo.h"
#include "PvZ/Lawn/LawnApp.h"

void LawnPlayerInfo::AddCoins(int theAmount) {
    // 用于 购物愉快 成就
    int aNumCoin = mCoins + theAmount;
    if (aNumCoin > 99999) {
        aNumCoin = 99999;
    }
    if (aNumCoin <= 0) {
        aNumCoin = 0;
    }
    mCoins = aNumCoin;
    if (theAmount < 0) {
        mUsedCoins -= theAmount;
        if (mUsedCoins >= 2500) {
            LawnApp *gLawnApp = *gLawnApp_Addr;
            gLawnApp->GrantAchievement(AchievementId::ACHIEVEMENT_SHOP);
        }
    }
}