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

#include "PvZ/Lawn/Widget/VSResultsMenu.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"

void VSResultsMenu_Update(int *a) {
    // 记录当前游戏状态
    return old_VSResultsMenu_Update(a);
}

void VSResultsMenu_OnExit(int *a) {
    return old_VSResultsMenu_OnExit(a);
}

void VSResultsMenu_ButtonDepress(int *vsResultsScreen, int buttonId) {
    if (!*((uint8_t *)vsResultsScreen + 260)) {
        vsResultsScreen[75] = buttonId;
        VSResultsMenu_OnExit(vsResultsScreen);
    }
    //    if (buttonId == 1) {
    //        LawnApp_DoBackToMain((int *) *gLawnApp_Addr);
    //        LawnApp_KillVSResultsScreen((int *) *gLawnApp_Addr);
    //    } else if (buttonId == 0) {
    //        LawnApp_PreNewGame(*gLawnApp_Addr, 76, 0);
    //        LawnApp_KillVSResultsScreen((int *) *gLawnApp_Addr);
    //    }
}

void VSResultsMenu_DrawInfoBox(int *a, Sexy::Graphics *a2, int a3) {
    //    Sexy::Image* tmp = *Sexy_IMAGE_NO_GAMERPIC_Addr;
    //    if (addonImages.gamerpic == nullptr && addonImages.zombatar_portrait != nullptr) {
    //        int width = tmp->mWidth;
    //        int height = tmp->mHeight;
    //        addonImages.gamerpic = (Sexy::Image*) operator new(sizeof(Sexy::Image));
    //        Sexy_MemoryImage_MemoryImage(addonImages.gamerpic);
    //        Sexy_MemoryImage_Create(addonImages.gamerpic,width,height);
    //        Sexy::Graphics graphics;
    //        Sexy_Graphics_Graphics2(&graphics,addonImages.gamerpic);
    //        TodDrawImageScaledF(&graphics,addonImages.zombatar_portrait,0,0,width / addonImages.zombatar_portrait->mWidth,height / addonImages.zombatar_portrait->mHeight);
    //        Sexy_Graphics_Delete2(&graphics);
    //        *Sexy_IMAGE_NO_GAMERPIC_Addr = addonImages.gamerpic;
    //    }
    old_VSResultsMenu_DrawInfoBox(a, a2, a3);
    //    *Sexy_IMAGE_NO_GAMERPIC_Addr = tmp;
}
