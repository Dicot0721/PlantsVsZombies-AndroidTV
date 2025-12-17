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
#include <unistd.h>


std::vector<char> clientVSResultsMenuRecvBuffer;


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
            int theId = event1->data;
        } break;

        default:
            break;
    }
}

void VSResultsMenu::HandleTcpClientMessage(void *buf, ssize_t bufSize) {

    clientVSResultsMenuRecvBuffer.insert(clientVSResultsMenuRecvBuffer.end(), (char *)buf, (char *)buf + bufSize);
    size_t offset = 0;

    while (clientVSResultsMenuRecvBuffer.size() - offset >= sizeof(BaseEvent)) {
        BaseEvent *base = (BaseEvent *)&clientVSResultsMenuRecvBuffer[offset];
        size_t eventSize = getClientEventSize(base->type);
        if (clientVSResultsMenuRecvBuffer.size() - offset < eventSize)
            break; // 不完整

        processClientEvent((char *)&clientVSResultsMenuRecvBuffer[offset], eventSize);
        offset += eventSize;
    }

    if (offset != 0) {
        clientVSResultsMenuRecvBuffer.erase(clientVSResultsMenuRecvBuffer.begin(), clientVSResultsMenuRecvBuffer.begin() + offset);
    }
}


std::vector<char> serverVSResultsMenuRecvBuffer;


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
            int theId = event1->data;
            mResultsButtonId = theId;
            OnExit();
        } break;

        default:
            break;
    }
}


void VSResultsMenu::HandleTcpServerMessage(void *buf, ssize_t bufSize) {
    serverVSResultsMenuRecvBuffer.insert(serverVSResultsMenuRecvBuffer.end(), (char *)buf, (char *)buf + bufSize);
    size_t offset = 0;

    while (serverVSResultsMenuRecvBuffer.size() - offset >= sizeof(BaseEvent)) {
        BaseEvent *base = (BaseEvent *)&serverVSResultsMenuRecvBuffer[offset];
        size_t eventSize = getServerEventSize(base->type);
        if (serverVSResultsMenuRecvBuffer.size() - offset < eventSize)
            break; // 不完整

        processServerEvent((char *)&serverVSResultsMenuRecvBuffer[offset], eventSize);
        offset += eventSize;
    }

    if (offset != 0) {
        serverVSResultsMenuRecvBuffer.erase(serverVSResultsMenuRecvBuffer.begin(), serverVSResultsMenuRecvBuffer.begin() + offset);
    }
}

void VSResultsMenu::Update() {
    // 记录当前游戏状态
    old_VSResultsMenu_Update(this);


    //    if (tcpClientSocket >= 0) {
    //        char buf[1024];
    //
    //        while (true) {
    //            ssize_t n = recv(tcpClientSocket, buf, sizeof(buf) - 1, MSG_DONTWAIT);
    //            if (n > 0) {
    //                // buf[n] = '\0'; // 确保字符串结束
    //                // LOG_DEBUG("[TCP] 收到来自Client的数据: {}", buf);
    //
    //                HandleTcpClientMessage(buf, n);
    //            } else if (n == 0) {
    //                // 对端关闭连接（收到FIN）
    //                LOG_DEBUG("[TCP] 对方关闭连接");
    //                close(tcpClientSocket);
    //                tcpClientSocket = -1;
    //                break;
    //            } else {
    //                if (errno == EAGAIN || errno == EWOULDBLOCK) {
    //                    // 没有更多数据可读，正常退出
    //                    break;
    //                } else if (errno == EINTR) {
    //                    // 被信号中断，重试
    //                    continue;
    //                } else {
    //                    LOG_DEBUG("[TCP] recv 出错 errno={}", errno);
    //                    close(tcpClientSocket);
    //                    tcpClientSocket = -1;
    //                    break;
    //                }
    //            }
    //        }
    //    }
    //
    //    if (tcp_connected) {
    //        char buf[1024];
    //        while (true) {
    //            ssize_t n = recv(tcpServerSocket, buf, sizeof(buf) - 1, MSG_DONTWAIT);
    //            if (n > 0) {
    //                // buf[n] = '\0'; // 确保字符串结束
    //                // LOG_DEBUG("[TCP] 收到来自Server的数据: {}", buf);
    //                HandleTcpServerMessage(buf, n);
    //
    //            } else if (n == 0) {
    //                // 对端关闭连接（收到FIN）
    //                LOG_DEBUG("[TCP] 对方关闭连接");
    //                close(tcpServerSocket);
    //                tcpServerSocket = -1;
    //                tcp_connecting = false;
    //                tcp_connected = false;
    //                break;
    //            } else {
    //                if (errno == EAGAIN || errno == EWOULDBLOCK) {
    //                    // 没有更多数据可读，正常退出
    //                    break;
    //                } else if (errno == EINTR) {
    //                    // 被信号中断，重试
    //                    continue;
    //                } else {
    //                    LOG_DEBUG("[TCP] recv 出错 errno={}", errno);
    //                    close(tcpServerSocket);
    //                    tcpServerSocket = -1;
    //                    tcp_connecting = false;
    //                    tcp_connected = false;
    //                    break;
    //                }
    //            }
    //        }
    //    }
}

void VSResultsMenu::OnExit() {
    old_VSResultsMenu_OnExit(this);
}

void VSResultsMenu::ButtonDepress(int buttonId) {

    if (mIsFading)
        return;

    if (buttonId == 1) {
        mResultsButtonId = buttonId;
        OnExit();
        return;
    }


    if (tcp_connected) {
        // 客户端点击再来一局
        U8_Event event = {{EventType::EVENT_CLIENT_VSRESULT_BUTTON_DEPRESS}, uint8_t(buttonId)};
        send(tcpServerSocket, &event, sizeof(U8_Event), 0);
        return;
    }

    if (tcpClientSocket >= 0) {
        U8_Event event = {{EventType::EVENT_SERVER_VSRESULT_BUTTON_DEPRESS}, uint8_t(buttonId)};
        send(tcpClientSocket, &event, sizeof(U8_Event), 0);
    }


    mResultsButtonId = buttonId;
    OnExit();


    // if (buttonId == 1) {
    // LawnApp_DoBackToMain((int *) *gLawnApp_Addr);
    // LawnApp_KillVSResultsScreen((int *) *gLawnApp_Addr);
    // } else if (buttonId == 0) {
    // LawnApp_PreNewGame(*gLawnApp_Addr, 76, 0);
    // LawnApp_KillVSResultsScreen((int *) *gLawnApp_Addr);
    // }
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
