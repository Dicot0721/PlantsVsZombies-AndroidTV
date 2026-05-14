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

#include "PvZ/Lawn/Widget/VSResultsMenu.h"
#include "Homura/Logger.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/NetPlay.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/TodLib/Common/TodCommon.h"
#include "PvZ/TodLib/Common/TodStringFile.h"

#include <unistd.h>

using namespace Sexy;

class ImageWidgetLike : public Sexy::Widget {
public:
    Sexy::Image *mImage;
};


void VSResultsMenu::_constructor() {
    old_VSResultsMenu_Constructor(this);
    gVSResultRequestState = -1;
    gNetDelayNow = 0; // 清除旧的延时数据

    mCheckboxController = nullptr;
}

void VSResultsMenu::processClientEvent(const BaseEvent *event) {
    LOG_DEBUG("TYPE:{}", (int)event->type);
    switch (event->type) {
        case EVENT_CLIENT_VSRESULT_BUTTON_DEPRESS: {
            auto *event1 = static_cast<const U8_Event *>(event);
            int anId = event1->data;
            gVSResultRequestState = anId;
        } break;
        default:
            break;
    }
}


void VSResultsMenu::processServerEvent(const BaseEvent *event) {
    LOG_DEBUG("TYPE:{}", (int)event->type);
    switch (event->type) {
        case EVENT_SERVER_VSRESULT_BUTTON_DEPRESS: {
            auto *event1 = static_cast<const U8_Event *>(event);
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
    if (mResultsButtonId == VSResultsMenu_Quit_VS) {
        gLawnApp->ShowMainMenuScreen();
        gLawnApp->KillVSResultsScreen();
    } else if (mResultsButtonId == VSResultsMenu_Play_Again) {
        //        gLawnApp->PreNewGame(GameMode::GAMEMODE_MP_VS, false);
        // 改为返回关卡选择页面
        gLawnApp->ShowChallengeScreen(ChallengePage::CHALLENGE_PAGE_VS);
        gLawnApp->KillVSResultsScreen();
    }
}

void VSResultsMenu::ButtonDepress(int theId) {
    if (mIsFading)
        return;

    if (gIsServerModeSpectator && (gTcpConnected || gTcpServerSocket >= 0 || gTcpClientSocket >= 0)) {
        if (gTcpServerSocket >= 0) {
            shutdown(gTcpServerSocket, SHUT_RDWR);
            close(gTcpServerSocket);
            gTcpServerSocket = -1;
        }
        if (gTcpClientSocket >= 0) {
            shutdown(gTcpClientSocket, SHUT_RDWR);
            close(gTcpClientSocket);
            gTcpClientSocket = -1;
        }
        gTcpConnected = false;
        gTcpConnecting = false;
        gIsServerModeNetplay = false;
        gServerModeTransport = ServerModeTransport::NONE;
        gIsServerModeSpectator = false;
        gSecondPlayerName[0] = '\0';
        gServerHostName[0] = '\0';
        netplay::ClearSendBuffer();

        mResultsButtonId = theId;
        OnExit();
        return;
    }

    if (theId == VSResultsMenu::VSResultsMenu_Quit_VS) {
        mResultsButtonId = theId;
        OnExit();
        return;
    }

    if (gTcpConnected) {
        // 客户端点击再来一局
        U8_Event event = {{EventType::EVENT_CLIENT_VSRESULT_BUTTON_DEPRESS}, uint8_t(theId)};
        netplay::PutEvent(event);
        gVSResultRequestState = theId;
        return;
    }

    if (gTcpClientSocket >= 0) {
        U8_Event event = {{EventType::EVENT_SERVER_VSRESULT_BUTTON_DEPRESS}, uint8_t(theId)};
        netplay::PutEvent(event);
    }

    mResultsButtonId = theId;
    OnExit();
}

void VSResultsMenu::Draw(Graphics *g) {
    old_VSResultsMenu_Draw(this, g);
    if (gIsServerModeNetplay && gServerModeTransport == ServerModeTransport::RELAY && mCheckboxController != nullptr) {
        mCheckboxController->DrawCheckboxLabel(g);
    }

    if (gTcpConnected) {
        switch (gVSResultRequestState) {
            case VSResultsMenu::VSResultsMenu_Play_Again:
                TodDrawString(g, "[VS_RESULT_REMIND_HOST_PLAY_AGAIN]", 400, -20, Sexy::FONT_HOUSEOFTERROR28, Color(0, 205, 0, 255), DrawStringJustification::DS_ALIGN_CENTER);
                break;
            default:
                break;
        }
    }

    if (gTcpClientSocket >= 0) {
        switch (gVSResultRequestState) {
            case VSResultsMenu::VSResultsMenu_Play_Again:
                TodDrawString(g, "[VS_RESULT_OPPONENT_REQUEST_PLAY_AGAIN]", 400, -20, Sexy::FONT_HOUSEOFTERROR28, Color(0, 205, 0, 255), DrawStringJustification::DS_ALIGN_CENTER);
                break;
            default:
                break;
        }
    }
}


void VSResultsMenu::DrawInfoBox(Sexy::Graphics *a2, int a3) {
    Sexy::Widget *slotWidget = FindWidget(a3 + 2);
    if (slotWidget == nullptr) {
        return;
    }

    auto *infoWidget = reinterpret_cast<ImageWidgetLike *>(slotWidget);
    a2->PushState();
    a2->Translate(slotWidget->mX, slotWidget->mY);

    int sideInSlot = mSides[a3];
    if (sideInSlot < 0 || sideInSlot > 1) {
        a2->PopState();
        return;
    }
    int this76Side = unk2[sideInSlot];
    int *playerRecord = GetPlayerRecord((unsigned int)this76Side);
    DefaultProfileMgr *profileMgr = gLawnApp->mProfileMgr;
    PlayerInfo *profileObj = gLawnApp->mPlayerInfo;
    if (profileMgr != nullptr) {
        profileObj = profileMgr->GetProfile(profileObj->mName, this76Side);
    }
    if (playerRecord == nullptr || profileObj == nullptr) {
        a2->PopState();
        return;
    }
    bool isZombieSlot = (sideInSlot != 0);

    infoWidget->mImage = isZombieSlot ? Sexy::IMAGE_VS_INFO_BOX_ZOMBIES : Sexy::IMAGE_VS_INFO_BOX_PLANTS;
    a2->DrawImage(Sexy::IMAGE_NO_GAMERPIC, 31, 52);
    a2->DrawImage(isZombieSlot ? Sexy::IMAGE_VS_INFO_BOX_ZOMBIES_OVERLAY : Sexy::IMAGE_VS_INFO_BOX_PLANTS_OVERLAY, 0, 0);
    pvzstl::string playerFmt = TodStringTranslate("[PLAYER_FMT]");
    pvzstl::string playerLabel = StrFormat(playerFmt.c_str(), a3 + 1);
    if ((gTcpConnected || gTcpClientSocket >= 0) && gSecondPlayerName[0] != '\0') {
        const char *hostName = (gServerHostName[0] != '\0') ? gServerHostName : gLawnApp->mPlayerInfo->mName;
        const char *guestName = gSecondPlayerName;
        playerLabel = (this76Side == 0) ? hostName : guestName;
    }

    a2->SetColor(Sexy::Color::White);
    a2->SetFont(Sexy::FONT_DWARVENTODCRAFT18);
    a2->DrawString(playerLabel, 42, 44);
    int winStreak = playerRecord[3];
    profileObj->mWinStreak = winStreak;
    if (winStreak > 1) {
        pvzstl::string streakFmt = TodStringTranslate("[WIN_STREAK_FMT]");
        a2->DrawString(StrFormat(streakFmt.c_str(), winStreak), 263, 78);
    }
    float trophyX = unk3[0];
    float trophyY = unk3[1];
    if (mSmokeCounter > 49) {
        trophyY = (float)TodAnimateCurve(50, 60, mSmokeCounter, 82, 74, TodCurves::CURVE_EASE_IN_OUT);
    } else {
        trophyY = (float)TodAnimateCurve(0, 50, mSmokeCounter, -60 - slotWidget->mY - Sexy::IMAGE_MP_TROPHY_BASE->mHeight - Sexy::IMAGE_MP_PLANT_TROPHY->mHeight, 82, TodCurves::CURVE_LINEAR);
    }

    int winnerSide = playerRecord[0];
    if (winnerSide != -1) {
        if (TodParticleSystem *smoke = gLawnApp->ParticleTryToGet(mSmokeParticleID)) {
            smoke->Draw(a2);
        }

        Sexy::Image *topTrophy = (winnerSide != 0) ? Sexy::IMAGE_MP_ZOMBIE_TROPHY : Sexy::IMAGE_MP_PLANT_TROPHY;
        a2->DrawImage(Sexy::IMAGE_MP_TROPHY_BASE, (int)(trophyX - Sexy::IMAGE_MP_TROPHY_BASE->mWidth / 2.0f), (int)trophyY);
        a2->DrawImage(topTrophy, (int)(trophyX + 2.0f - topTrophy->mWidth / 2.0f), (int)(trophyY - 46.0f));

        if (TodParticleSystem *sparkle = gLawnApp->ParticleTryToGet(mSparkleParticleID)) {
            sparkle->SystemMove(trophyX, trophyY);
            sparkle->Draw(a2);
        }
    }
    float plantTrophyX = (winnerSide == -1) ? 117.0f : 192.0f;
    int plantWins = playerRecord[1];
    if (plantWins > 0) {
        float step = 196.0f / (float)plantWins;
        if (step > 52.0f) {
            step = 52.0f;
        }
        for (int i = 0; i < plantWins; i++) {
            a2->DrawImage(Sexy::IMAGE_MP_PLANT_TROPHY, (int)plantTrophyX, 82, 40, 40);
            plantTrophyX += step;
        }
    }
    float zombieTrophyX = (winnerSide == -1) ? 117.0f : 192.0f;
    int zombieWins = playerRecord[2];
    if (zombieWins > 0) {
        float step = 196.0f / (float)zombieWins;
        if (step > 52.0f) {
            step = 52.0f;
        }
        for (int i = 0; i < zombieWins; i++) {
            a2->DrawImage(Sexy::IMAGE_MP_ZOMBIE_TROPHY, (int)zombieTrophyX, 124, 40, 40);
            zombieTrophyX += step;
        }
    }
    a2->PopState();
}
