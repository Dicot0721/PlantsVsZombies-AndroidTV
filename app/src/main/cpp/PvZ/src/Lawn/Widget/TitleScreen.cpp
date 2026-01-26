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

#include "PvZ/Lawn/Widget/TitleScreen.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/Symbols.h"

void TitleScreen_Draw(TitleScreen *titleScreen, Sexy::Graphics *graphics) {
    old_TitleScreen_Draw(titleScreen, graphics);
    // LOGD("%d",*Sexy_IMAGE_ESRB_RATING_Addr);
    // if (*Sexy_IMAGE_ESRB_RATING_Addr)
    // DrawImage(graphics,*Sexy_IMAGE_ESRB_RATING_Addr,0,0);
    // LOGD("draw");
    // int *q = nullptr;
    // q[1] = 1;
}

void TitleScreen_Update(TitleScreen *titleScreen) {
    old_TitleScreen_Update(titleScreen);
    // LOGD("Update");
    // int *q = nullptr;
    // q[1] = 1;
}

void TitleScreen_SwitchState(TitleScreen *titleScreen, TitleScreen::TitleState state, int duration) {
    if (state == TitleScreen::PopcapLogo_OR_IntroVideo && jumpLogo) {
        state = TitleScreen::Loading;
        duration = 0;
    }
    titleScreen->mTitleState = state;
    titleScreen->mTitleStateDuration = duration;
    titleScreen->mTitleStateCounter = duration;
}

void TitleScreen::_constructor(LawnApp *theApp) {
    old_TitleScreen_TitleScreen(this, theApp);
}