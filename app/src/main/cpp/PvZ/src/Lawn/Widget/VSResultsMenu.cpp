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
#include "Homura/Logger.h"
#include "PvZ/Lawn/Widget/WaitForSecondPlayerDialog.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/TodLib/Common/TodCommon.h"

#include <unistd.h>

using namespace Sexy;


void VSResultsMenu::_constructor() {
    old_VSResultsMenu_Constructor(this);
    gVSResultRequestState = -1;
}

size_t VSResultsMenu::getClientEventSize(EventType type) {
    switch (type) {
        case EVENT_CLIENT_VSRESULT_BUTTON_DEPRESS:
            return sizeof(U8_Event);
        default:
            return sizeof(BaseEvent);
    }
}

void VSResultsMenu::processClientEvent(void *buf, ssize_t bufSize) {
    BaseEvent *event = (BaseEvent *)buf;
    LOG_DEBUG("TYPE:{}", (int)event->type);
    switch (event->type) {
        case EVENT_CLIENT_VSRESULT_BUTTON_DEPRESS: {
            U8_Event *event1 = (U8_Event *)event;
            int anId = event1->data;
            gVSResultRequestState = anId;
        } break;

        default:
            break;
    }
}


size_t VSResultsMenu::getServerEventSize(EventType type) {
    switch (type) {
        case EVENT_SERVER_VSRESULT_BUTTON_DEPRESS:
            return sizeof(U8_Event);
        default:
            return sizeof(BaseEvent);
    }
}

void VSResultsMenu::processServerEvent(void *buf, ssize_t bufSize) {
    BaseEvent *event = (BaseEvent *)buf;
    LOG_DEBUG("TYPE:{}", (int)event->type);
    switch (event->type) {
        case EVENT_SERVER_VSRESULT_BUTTON_DEPRESS: {
            U8_Event *event1 = (U8_Event *)event;
            int anId = event1->data;
            mResultsButtonId = anId;
            OnExit();
        } break;

        default:
            break;
    }
}

void VSResultsMenu::Update() {
    // 记录当前游戏状态
    old_VSResultsMenu_Update(this);
}

void VSResultsMenu::OnExit() {
    old_VSResultsMenu_OnExit(this);
}

void VSResultsMenu::ButtonDepress(int theId) {
    if (mIsFading)
        return;

    if (theId == VSResultsMenu::VSResultsMenu_Quit_VS) {
        mResultsButtonId = theId;
        OnExit();
        return;
    }

    if (tcp_connected) {
        // 客户端点击再来一局
        U8_Event event = {{EventType::EVENT_CLIENT_VSRESULT_BUTTON_DEPRESS}, uint8_t(theId)};
        sendWithSize(tcpServerSocket, &event, sizeof(U8_Event), 0);
        gVSResultRequestState = theId;
        return;
    }

    if (tcpClientSocket >= 0) {
        U8_Event event = {{EventType::EVENT_SERVER_VSRESULT_BUTTON_DEPRESS}, uint8_t(theId)};
        sendWithSize(tcpClientSocket, &event, sizeof(U8_Event), 0);
    }

    mResultsButtonId = theId;
    OnExit();

    // if (theId == 1) {
    // LawnApp_DoBackToMain((int *) *gLawnApp_Addr);
    // LawnApp_KillVSResultsScreen((int *) *gLawnApp_Addr);
    // } else if (theId == 0) {
    // LawnApp_PreNewGame(*gLawnApp_Addr, 76, 0);
    // LawnApp_KillVSResultsScreen((int *) *gLawnApp_Addr);
    // }
}

void VSResultsMenu::Draw(Graphics *g) {
    old_VSResultsMenu_Draw(this, g);

    if (tcp_connected) {
        switch (gVSResultRequestState) {
            case VSResultsMenu::VSResultsMenu_Play_Again:
                TodDrawString(g, "已提醒房主再来一次", 400, -20, *Sexy::FONT_HOUSEOFTERROR28, Color(0, 205, 0, 255), DrawStringJustification::DS_ALIGN_CENTER);
                break;
            default:
                break;
        }
    }

    if (tcpClientSocket >= 0) {
        switch (gVSResultRequestState) {
            case VSResultsMenu::VSResultsMenu_Play_Again:
                TodDrawString(g, "对方请求再来一次", 400, -20, *Sexy::FONT_HOUSEOFTERROR28, Color(0, 205, 0, 255), DrawStringJustification::DS_ALIGN_CENTER);
                break;
            default:
                break;
        }
    }
}

void VSResultsMenu::DrawInfoBox(Sexy::Graphics *a2, int a3) {
    // Sexy::Image* tmp = *Sexy_IMAGE_NO_GAMERPIC_Addr;
    // if (addonImages.gamerpic == nullptr && addonImages.zombatar_portrait != nullptr) {
    // int width = tmp->mWidth;
    // int height = tmp->mHeight;
    // addonImages.gamerpic = (Sexy::Image*) operator new(sizeof(Sexy::Image));
    // Sexy_MemoryImage_MemoryImage(addonImages.gamerpic);
    // Sexy_MemoryImage_Create(addonImages.gamerpic,width,height);
    // Sexy::Graphics graphics;
    // Sexy_Graphics_Graphics2(&graphics,addonImages.gamerpic);
    // TodDrawImageScaledF(&graphics,addonImages.zombatar_portrait,0,0,width / addonImages.zombatar_portrait->mWidth,height / addonImages.zombatar_portrait->mHeight);
    // Sexy_Graphics_Delete2(&graphics);
    // *Sexy_IMAGE_NO_GAMERPIC_Addr = addonImages.gamerpic;
    // }
    old_VSResultsMenu_DrawInfoBox(this, a2, a3);
    // *Sexy_IMAGE_NO_GAMERPIC_Addr = tmp;
}
