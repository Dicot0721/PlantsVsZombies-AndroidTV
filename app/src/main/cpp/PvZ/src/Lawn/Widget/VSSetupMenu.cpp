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

#include "PvZ/Lawn/Widget/VSSetupMenu.h"
#include "Homura/Logger.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Challenge.h"
#include "PvZ/Lawn/Board/CutScene.h"
#include "PvZ/Lawn/Board/SeedBank.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/Widget/SeedChooserScreen.h"
#include "PvZ/Lawn/Widget/WaitForSecondPlayerDialog.h"
#include "PvZ/TodLib/Common/TodStringFile.h"

#include <unistd.h>

#include <cassert>

using namespace Sexy;

void VSSetupMenu::_constructor() {
    old_VSSetupMenu_Constructor(this);
    msNextFirstPick = VSPickTurn::VS_PICK_TURN_ZOMBIE;

    // 拓展卡槽,禁选模式 etc.
    gVSSetupAddonWidget = new VSSetupAddonWidget(this);

    is1PControllerMoving = false;
    is2PControllerMoving = false;
    touchingOnWhichController = 0;
    drawTipArrowAlphaCounter = 0;
    gVSSetupRequestState = 0;
}

void VSSetupMenu::_destructor() {
    if (gVSSetupAddonWidget)
        gVSSetupAddonWidget->~VSSetupAddonWidget();

    old_VSSetupMenu_Destructor(this);
}

void VSSetupMenu::Draw(Graphics *g) {
    // 在这里绘制会被DrawOverlay遮挡，去DrawOverlay绘制即可
    old_VSSetupMenu_Draw(this, g);
}

void VSSetupMenu::DrawOverlay(Graphics *g) {
    old_VSSetupMenu_DrawOverlay(this, g);

    if (mState == VSSetupState::VS_SETUP_STATE_SIDES) {
        TodDrawString(g, "[VS_PICK_SIDES]", 350, 110, *Sexy_FONT_DWARVENTODCRAFT18_Addr, Color::White, DrawStringJustification::DS_ALIGN_LEFT);
    } else if (mState == VSSetupState::VS_SETUP_STATE_SELECT_BATTLE) {
        TodDrawString(g, "[VS_PICK_BATTLES]", 350, 110, *Sexy_FONT_DWARVENTODCRAFT18_Addr, Color::White, DrawStringJustification::DS_ALIGN_LEFT);
    }

    if (drawTipArrowAlphaCounter > 200) {
        int aAlpha = TodAnimateCurve(0, 100, drawTipArrowAlphaCounter % 100, 50, 255, TodCurves::CURVE_BOUNCE);
        Color theColor = {255, 255, 255, aAlpha};
        g->SetColorizeImages(true);
        g->SetColor(theColor);

        if (!tcp_connected && mSides[0] == VSSide::VS_SIDE_NONE) {
            Sexy::Widget *theController1Widget = FindWidget(7);
            g->DrawImage(*Sexy_IMAGE_ZEN_NEXTGARDEN_Addr, theController1Widget->mX + 160, theController1Widget->mY + 40);
            g->DrawImageMirror(*Sexy_IMAGE_ZEN_NEXTGARDEN_Addr, theController1Widget->mX - 50, theController1Widget->mY + 40, true);
        }


        if (tcpClientSocket < 0 && mSides[1] == VSSide::VS_SIDE_NONE) {
            Sexy::Widget *theController2Widget = FindWidget(8);
            g->DrawImage(*Sexy_IMAGE_ZEN_NEXTGARDEN_Addr, theController2Widget->mX + 160, theController2Widget->mY + 40);
            g->DrawImageMirror(*Sexy_IMAGE_ZEN_NEXTGARDEN_Addr, theController2Widget->mX - 50, theController2Widget->mY + 40, true);
        }

        g->SetColorizeImages(false);
    }

    if (gVSSetupRequestState != 0) {

        // ======================
        // 我是 guest：已提醒房主...
        // (tcp_connected == true 代表我作为 client 连接到 host)
        // ======================
        if (tcp_connected) {
            switch (gVSSetupRequestState) {
                case VSSetupMenu_Quick_Play: {
                    pvzstl::string fmt = TodStringTranslate("[VS_TIP_REMIND_HOST_FMT]");
                    pvzstl::string opt = TodStringTranslate("[VS_OPT_QUICK_GAME]");
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy_FONT_HOUSEOFTERROR28_Addr, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                case VSSetupMenu_Custom_Battle: {
                    pvzstl::string fmt = TodStringTranslate("[VS_TIP_REMIND_HOST_FMT]");
                    pvzstl::string opt = TodStringTranslate("[VS_OPT_CUSTOM_ARENA]");
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy_FONT_HOUSEOFTERROR28_Addr, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                case VSSetupMenu_Random_Battle: {
                    pvzstl::string fmt = TodStringTranslate("[VS_TIP_REMIND_HOST_FMT]");
                    pvzstl::string opt = TodStringTranslate("[VS_OPT_RANDOM_ARENA]");
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy_FONT_HOUSEOFTERROR28_Addr, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                case VSSetupAddonWidget::VSSetupAddonWidget_ExtraPackets: {
                    pvzstl::string fmt = TodStringTranslate("[VS_TIP_REMIND_HOST_FMT]");
                    pvzstl::string opt = TodStringTranslate((gVSSetupAddonWidget && !gVSSetupAddonWidget->mExtraPacketsMode) ? "[VS_OPT_ENABLE_EXTRA_SLOTS]" : "[VS_OPT_DISABLE_EXTRA_SLOTS]");
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy_FONT_HOUSEOFTERROR28_Addr, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                case VSSetupAddonWidget::VSSetupAddonWidget_ExtraSeeds: {
                    pvzstl::string fmt = TodStringTranslate("[VS_TIP_REMIND_HOST_FMT]");
                    pvzstl::string opt = TodStringTranslate((gVSSetupAddonWidget && !gVSSetupAddonWidget->mExtraSeedsMode) ? "[VS_OPT_ENABLE_EXTRA_SEEDS]" : "[VS_OPT_DISABLE_EXTRA_SEEDS]");
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy_FONT_HOUSEOFTERROR28_Addr, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                case VSSetupAddonWidget::VSSetupAddonWidget_BanMode: {
                    pvzstl::string fmt = TodStringTranslate("[VS_TIP_REMIND_HOST_FMT]");
                    pvzstl::string opt = TodStringTranslate((gVSSetupAddonWidget && !gVSSetupAddonWidget->mBanMode) ? "[VS_OPT_ENABLE_BAN_MODE]" : "[VS_OPT_DISABLE_BAN_MODE]");
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy_FONT_HOUSEOFTERROR28_Addr, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                case VSSetupAddonWidget::VSSetupAddonWidget_BalancePatch: {
                    pvzstl::string fmt = TodStringTranslate("[VS_TIP_REMIND_HOST_FMT]");
                    pvzstl::string opt = TodStringTranslate((gVSSetupAddonWidget && !gVSSetupAddonWidget->mBalancePatchMode) ? "[VS_OPT_ENABLE_BALANCE_PATCH]" : "[VS_OPT_DISABLE_BALANCE_PATCH]");
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy_FONT_HOUSEOFTERROR28_Addr, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                default:
                    break;
            }
        }

        // ======================
        // 我是 host：对方想玩/想要...
        // (tcpClientSocket >= 0 表示我作为 host 收到了 client 连接)
        // ======================
        if (tcpClientSocket >= 0) {
            switch (gVSSetupRequestState) {
                case VSSetupMenu_Quick_Play: {
                    pvzstl::string fmt = TodStringTranslate("[VS_TIP_OPPONENT_WANTS_PLAY_FMT]");
                    pvzstl::string opt = TodStringTranslate("[VS_OPT_QUICK_GAME]");
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy_FONT_HOUSEOFTERROR28_Addr, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                case VSSetupMenu_Custom_Battle: {
                    pvzstl::string fmt = TodStringTranslate("[VS_TIP_OPPONENT_WANTS_PLAY_FMT]");
                    pvzstl::string opt = TodStringTranslate("[VS_OPT_CUSTOM_ARENA]");
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy_FONT_HOUSEOFTERROR28_Addr, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                case VSSetupMenu_Random_Battle: {
                    pvzstl::string fmt = TodStringTranslate("[VS_TIP_OPPONENT_WANTS_PLAY_FMT]");
                    pvzstl::string opt = TodStringTranslate("[VS_OPT_RANDOM_ARENA]");
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy_FONT_HOUSEOFTERROR28_Addr, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                case VSSetupAddonWidget::VSSetupAddonWidget_ExtraPackets: {
                    pvzstl::string fmt = TodStringTranslate("[VS_TIP_OPPONENT_WANTS_GET_FMT]");
                    pvzstl::string opt = TodStringTranslate((gVSSetupAddonWidget && !gVSSetupAddonWidget->mExtraPacketsMode) ? "[VS_OPT_ENABLE_EXTRA_SLOTS]" : "[VS_OPT_DISABLE_EXTRA_SLOTS]");
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy_FONT_HOUSEOFTERROR28_Addr, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                case VSSetupAddonWidget::VSSetupAddonWidget_ExtraSeeds: {
                    pvzstl::string fmt = TodStringTranslate("[VS_TIP_OPPONENT_WANTS_GET_FMT]");
                    pvzstl::string opt = TodStringTranslate((gVSSetupAddonWidget && !gVSSetupAddonWidget->mExtraSeedsMode) ? "[VS_OPT_ENABLE_EXTRA_SEEDS]" : "[VS_OPT_DISABLE_EXTRA_SEEDS]");
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy_FONT_HOUSEOFTERROR28_Addr, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                case VSSetupAddonWidget::VSSetupAddonWidget_BanMode: {
                    pvzstl::string fmt = TodStringTranslate("[VS_TIP_OPPONENT_WANTS_GET_FMT]");
                    pvzstl::string opt = TodStringTranslate((gVSSetupAddonWidget && !gVSSetupAddonWidget->mBanMode) ? "[VS_OPT_ENABLE_BAN_MODE]" : "[VS_OPT_DISABLE_BAN_MODE]");
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy_FONT_HOUSEOFTERROR28_Addr, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                case VSSetupAddonWidget::VSSetupAddonWidget_BalancePatch: {
                    pvzstl::string fmt = TodStringTranslate("[VS_TIP_OPPONENT_WANTS_GET_FMT]");
                    pvzstl::string opt = TodStringTranslate((gVSSetupAddonWidget && !gVSSetupAddonWidget->mBalancePatchMode) ? "[VS_OPT_ENABLE_BALANCE_PATCH]" : "[VS_OPT_DISABLE_BALANCE_PATCH]");
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy_FONT_HOUSEOFTERROR28_Addr, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                default:
                    break;
            }
        }
    }

    if (gVSSetupAddonWidget) {
        gVSSetupAddonWidget->Draw(g);
    }
}


void VSSetupMenu::AddedToManager(Sexy::WidgetManager *a2) {
    old_VSSetupMenu_AddedToManager(this, a2);
    // 缩小Widget，使得触控可传递给VSSetupMenu自身
    for (int i = 0; i < 9; ++i) {
        Sexy::Widget *aWidget = FindWidget(i);
        if (aWidget) {
            aWidget->Resize(aWidget->mX, aWidget->mY, 0, 0);
        }
    }
}

void VSSetupMenu::MouseDown(int x, int y, int theCount) {
    if (mState == VS_SETUP_STATE_SIDES) {
        Sexy::Widget *theController1Widget = FindWidget(7);
        Sexy::Widget *theController2Widget = FindWidget(8);
        if (x > theController1Widget->mX && x < theController1Widget->mX + 170 && y > theController1Widget->mY && y < theController1Widget->mY + 122) {
            if (tcp_connected) {
                return;
            }
            is1PControllerMoving = true;
            drawTipArrowAlphaCounter = 0;
            touchingOnWhichController = 1;
        } else if (x > theController2Widget->mX && x < theController2Widget->mX + 170 && y > theController2Widget->mY && y < theController2Widget->mY + 122) {
            if (tcpClientSocket >= 0) {
                return;
            }
            is2PControllerMoving = true;
            drawTipArrowAlphaCounter = 0;
            touchingOnWhichController = 2;
        }
        touchDownX = x;
    }
}

void VSSetupMenu::MouseDrag(int x, int y) {
    if (touchingOnWhichController == 1) {
        if (tcp_connected)
            return;
        Sexy::Widget *theController1Widget = FindWidget(7);
        theController1Widget->Move(theController1Widget->mX + x - touchDownX, theController1Widget->mY);
        if (tcpClientSocket >= 0) {
            U16_Event event = {{EventType::EVENT_VSSETUPMENU_MOVE_CONTROLLER}, uint16_t(theController1Widget->mX)};
            SendEvent(tcpClientSocket, event);
        }
    } else if (touchingOnWhichController == 2) {
        if (tcpClientSocket >= 0)
            return;
        Sexy::Widget *theController2Widget = FindWidget(8);
        theController2Widget->Move(theController2Widget->mX + x - touchDownX, theController2Widget->mY);
        if (tcpServerSocket >= 0) {
            U16_Event event = {{EventType::EVENT_VSSETUPMENU_MOVE_CONTROLLER}, uint16_t(theController2Widget->mX)};
            SendEvent(tcpServerSocket, event);
        }
    }
    touchDownX = x;
}

void VSSetupMenu::MouseUp(int x, int y, int theCount) {

    if (touchingOnWhichController == 1) {
        if (tcp_connected)
            return;
        Sexy::Widget *aControllerWidgetP1 = FindWidget(7);
        VSSide aSideP1 = aControllerWidgetP1->mX > 400 ? VS_SIDE_ZOMBIE : aControllerWidgetP1->mX > 250 ? VS_SIDE_NONE : VS_SIDE_PLANT;
        if (aSideP1 == mSides[0]) {
            GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 0, 0);
        }
        mSides[0] = aSideP1;
        if (tcpClientSocket >= 0) {
            U8_Event event = {{EventType::EVENT_VSSETUPMENU_SET_CONTROLLER}, mSides[0] == -1 ? uint8_t(2) : uint8_t(mSides[0])};
            SendEvent(tcpClientSocket, event);
        }
        is1PControllerMoving = false;
    } else if (touchingOnWhichController == 2) {
        if (tcpClientSocket >= 0)
            return;
        Sexy::Widget *aControllerWidgetP2 = FindWidget(8);
        VSSide aSideP2 = aControllerWidgetP2->mX > 400 ? VS_SIDE_ZOMBIE : aControllerWidgetP2->mX > 250 ? VS_SIDE_NONE : VS_SIDE_PLANT;

        if (aSideP2 == mSides[1]) {
            GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 1, 0);
        }
        mSides[1] = aSideP2;
        if (tcpServerSocket >= 0) {
            U8_Event event = {{EventType::EVENT_VSSETUPMENU_SET_CONTROLLER}, mSides[1] == VS_SIDE_NONE ? uint8_t(2) : uint8_t(mSides[1])};
            SendEvent(tcpServerSocket, event);
        }
        is2PControllerMoving = false;
    }
    touchingOnWhichController = 0;
    if (mSides[0] != VS_SIDE_NONE && mSides[1] != VS_SIDE_NONE && mSides[0] != mSides[1]) {
        GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 0, 0);
        GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 1, 0);
    }
}

void VSSetupMenu::Update() {
    if (gVSSetupAddonWidget)
        gVSSetupAddonWidget->Update();

    drawTipArrowAlphaCounter++;

    if (is1PControllerMoving || is2PControllerMoving) {
        Sexy::Widget *theController1Widget = FindWidget(7);
        Sexy::Widget *theController2Widget = FindWidget(8);
        int Controller1X = theController1Widget->mX;
        int Controller2X = theController2Widget->mX;
        old_VSSetupMenu_Update(this);
        if (is1PControllerMoving)
            theController1Widget->Move(Controller1X, theController1Widget->mY);
        if (is2PControllerMoving)
            theController2Widget->Move(Controller2X, theController2Widget->mY);
    } else {
        old_VSSetupMenu_Update(this);
    }

    if (mState == VS_SETUP_STATE_CONTROLLERS) {
        return;
    }
    if (mState == VS_SETUP_STATE_SIDES && !tcp_connected && tcpClientSocket == -1 && !isKeyboardTwoPlayerMode) {
        // 本地游戏
        // 自动分配阵营
        //        mSides[0] = 0;
        //        mSides[1] = 1;
        //        GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 0, 0);
        //        GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 1, 0);
        return;
    }
}

void VSSetupMenu::PickRandomZombies(std::vector<SeedType> &theZombieSeeds) {
    assert(theZombieSeeds.empty());

    // 原本选 5 个, 扩展为 (卡槽数 - 1) 个
    const int numSeedsInBank = mApp->mBoard->GetNumSeedsInBank(true) - 1;
    theZombieSeeds.reserve(numSeedsInBank);

    for (int seedIdx = 0; seedIdx < numSeedsInBank; ++seedIdx) {
        // 0 1 -> 0
        // 2 3 -> 1
        // 4 5 -> 2
        int poolIdx = (seedIdx <= 5) ? seedIdx / 2 : 0;
        poolIdx += 6; // 僵尸卡池从池 6 开始

        // 统计该池有效元素个数
        int validCount = 0;
        while (validCount < 8 && msRandomPools[poolIdx][validCount] != SEED_NONE) {
            ++validCount;
        }

        SeedType aSeedType = SEED_NONE;
        do {
            do {
                aSeedType = msRandomPools[poolIdx][Sexy::Rand(validCount)];
            } while (std::ranges::contains(theZombieSeeds, aSeedType)); // 重复则重选
        } while (!mApp->HasSeedType(aSeedType, 1)); // 未获得则重选
        theZombieSeeds.push_back(aSeedType);
    }
}

void VSSetupMenu::PickRandomPlants(std::vector<SeedType> &thePlantSeeds, const std::vector<SeedType> &theZombieSeeds) {
    assert(thePlantSeeds.empty() && (theZombieSeeds.size() == mApp->mBoard->GetNumSeedsInBank(true) - 1));

    // 原本选 5 个, 扩展为 (卡槽数 - 1) 个
    const int numSeedsInBank = mApp->mBoard->GetNumSeedsInBank(false) - 1;
    thePlantSeeds.reserve(numSeedsInBank);

    int seedIdx = 0;
    int poolOffset = 0;

    // 是否为含蘑菇卡组
    if (mApp->mPlayerInfo->mLevel > 20 || /* 二周目 */ mApp->HasFinishedAdventure()) {
        bool isStageNight = mApp->mBoard->StageIsNight();
        bool flag = Sexy::Rand(5) == 1;
        if (!isStageNight && /* 1/5 */ flag) {
            thePlantSeeds.push_back(SEED_INSTANT_COFFEE);
            ++seedIdx;
            poolOffset = 3;
        } else if (isStageNight && /* 4/5 */ !flag) {
            poolOffset = 3;
        }
    }

    for (; seedIdx < numSeedsInBank; ++seedIdx) {
        // 0 1 -> 0
        // 2 3 -> 1
        // 4 5 -> 2
        int poolIdx = (seedIdx <= 5) ? seedIdx / 2 : 0;
        poolIdx += poolOffset; // 含蘑菇卡池从池 3 开始

        // 统计该池有效元素个数
        int validCount = 0;
        while (validCount < 8 && msRandomPools[poolIdx][validCount] != SEED_NONE) {
            ++validCount;
        }

        SeedType aSeedType = SEED_NONE;
        do {
            do {
                aSeedType = msRandomPools[poolIdx][Sexy::Rand(validCount)];
            } while (std::ranges::contains(thePlantSeeds, aSeedType)); // 重复则重选
        } while (!mApp->HasSeedType(aSeedType, 0)); // 未获得则重选
        thePlantSeeds.push_back(aSeedType);
    }

    // 原代码疑点: 前面已检查过 HasSeedType()
    if (std::ranges::contains(theZombieSeeds, SEED_ZOMBIE_POGO) /* && mApp->HasSeedType(SEED_WALLNUT, 0) */) {
        auto it = std::ranges::find(thePlantSeeds, SEED_WALLNUT);
        if (it != thePlantSeeds.end()) {
            *it = SEED_TALLNUT;
        }
    }

    if (tcpClientSocket >= 0) {
        U16x12_Event event;
        event.type = EventType::EVENT_VSSETUPMENU_RANDOM_PICK;
        std::ranges::copy(thePlantSeeds, event.data);
        std::ranges::copy(theZombieSeeds, event.data + 6);
        SendEvent(tcpClientSocket, event);
    }
}

size_t VSSetupMenu::getClientEventSize(EventType type) {
    switch (type) {
        case EVENT_CLIENT_VSSETUPMENU_BUTTON_DEPRESS:
            return sizeof(U8_Event);
        case EVENT_SEEDCHOOSER_SELECT_SEED:
            return sizeof(U8U8_Event);
        case EVENT_VSSETUPMENU_MOVE_CONTROLLER:
            return sizeof(U16_Event);
        case EVENT_VSSETUPMENU_SET_CONTROLLER:
            return sizeof(U8_Event);
        default:
            return sizeof(BaseEvent);
    }
}

void VSSetupMenu::processClientEvent(void *buf, ssize_t bufSize) {
    BaseEvent *event = (BaseEvent *)buf;
    LOG_DEBUG("TYPE:{}", (int)event->type);
    switch (event->type) {
        case EVENT_CLIENT_VSSETUPMENU_BUTTON_DEPRESS: {
            U8_Event *eventButtonDepress = (U8_Event *)event;
            gVSSetupRequestState = eventButtonDepress->data;
        } break;
        case EVENT_SEEDCHOOSER_SELECT_SEED: {
            U8U8_Event *event1 = (U8U8_Event *)event;
            SeedType theSeedType = (SeedType)event1->data1;
            bool mIsZombieChooser = event1->data2;
            LOG_DEBUG("theSeedType={}", event1->data1);
            LOG_DEBUG("mIsZombieChooser={}", mIsZombieChooser);
            SeedChooserScreen *screen = (mIsZombieChooser ? mApp->mZombieChooserScreen : mApp->mSeedChooserScreen);
            screen->GetSeedPositionInChooser(theSeedType, screen->mCursorPositionX1, screen->mCursorPositionY1);
            screen->GetSeedPositionInChooser(theSeedType, screen->mCursorPositionX2, screen->mCursorPositionY2);
            (mIsZombieChooser ? screen->mSeedType2 : screen->mSeedType1) = theSeedType;
            screen->GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, screen->mPlayerIndex);
        } break;
        case EVENT_SERVER_VSSETUPMENU_PICKBACKGROUND: {
            U8_Event *event1 = (U8_Event *)event;
            int tmp = VSBackGround;
            VSBackGround = event1->data;
            tcp_connected = false;
            PickBackgroundImmediately();
            tcp_connected = true;
            VSBackGround = tmp;
        } break;
        case EVENT_VSSETUPMENU_MOVE_CONTROLLER: {
            U16_Event *event1 = (U16_Event *)event;
            Sexy::Widget *theController2Widget = FindWidget(8);
            theController2Widget->Move(event1->data, theController2Widget->mY);
            is2PControllerMoving = true;
        } break;
        case EVENT_VSSETUPMENU_SET_CONTROLLER: {
            U8_Event *event1 = (U8_Event *)event;
            VSSide aSide = event1->data == 2 ? VS_SIDE_NONE : VSSide(event1->data);
            if (mSides[1] == aSide) {
                GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 1, 0);
            }
            mSides[1] = aSide;
            is2PControllerMoving = false;
            if (mSides[0] != VS_SIDE_NONE && mSides[1] != VS_SIDE_NONE && mSides[0] != mSides[1]) {
                GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 0, 0);
                GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 1, 0);
            }
        } break;
        default:
            break;
    }
}

size_t VSSetupMenu::getServerEventSize(EventType type) {
    switch (type) {
        case EVENT_SERVER_VSSETUPMENU_BUTTON_DEPRESS:
        case EVENT_VSSETUPMENU_ENTER_STATE:
        case EVENT_SERVER_VSSETUPMENU_PICKBACKGROUND:
            return sizeof(U8_Event);
        case EVENT_SEEDCHOOSER_SELECT_SEED:
            return sizeof(U8U8_Event);
        case EVENT_SERVER_VSSETUP_ADDON_BUTTON_INIT:
            return sizeof(B1x8_Event);
        case EVENT_VSSETUPMENU_RANDOM_PICK:
            return sizeof(U16x12_Event);
        case EVENT_VSSETUPMENU_MOVE_CONTROLLER:
            return sizeof(U16_Event);
        case EVENT_VSSETUPMENU_SET_CONTROLLER:
            return sizeof(U8_Event);
        default:
            return sizeof(BaseEvent);
    }
}

void VSSetupMenu::processServerEvent(void *buf, ssize_t bufSize) {
    BaseEvent *event = (BaseEvent *)buf;
    LOG_DEBUG("TYPE:{}", (int)event->type);
    switch (event->type) {
        case EVENT_SERVER_VSSETUPMENU_BUTTON_DEPRESS: {
            U8_Event *event1 = (U8_Event *)event;
            int theId = event1->data;
            LOG_DEBUG("theId={}", theId);
            if (theId == VSSetupMenu_Random_Battle && mState == VS_SETUP_STATE_SELECT_BATTLE) { // 随机战场
                break;
            }
            tcp_connected = false;
            ButtonDepress(theId);
            tcp_connected = true;
        } break;
        case EVENT_VSSETUPMENU_ENTER_STATE: {
            [[maybe_unused]] int aState = static_cast<U8_Event *>(event)->data;
            LOG_DEBUG("theState={}", aState);
            // GoToState(aState);
        } break;
        case EVENT_SEEDCHOOSER_SELECT_SEED: {
            U8U8_Event *event1 = (U8U8_Event *)event;
            SeedType theSeedType = (SeedType)event1->data1;
            bool mIsZombieChooser = event1->data2;
            LOG_DEBUG("theSeedType={}", event1->data1);
            LOG_DEBUG("mIsZombieChooser={}", mIsZombieChooser);
            SeedChooserScreen *screen = (mIsZombieChooser ? mApp->mZombieChooserScreen : mApp->mSeedChooserScreen);
            screen->GetSeedPositionInChooser(theSeedType, screen->mCursorPositionX1, screen->mCursorPositionY1);
            screen->GetSeedPositionInChooser(theSeedType, screen->mCursorPositionX2, screen->mCursorPositionY2);
            (mIsZombieChooser ? screen->mSeedType2 : screen->mSeedType1) = theSeedType;
            screen->GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, screen->mPlayerIndex);
        } break;
        case EVENT_VSSETUPMENU_RANDOM_PICK: {
            U16x12_Event *event1 = (U16x12_Event *)event;
            tcp_connected = false;
            ButtonDepress(VSSetupMenu_Random_Battle);
            tcp_connected = true;

            mApp->mBoard->mSeedBank[0]->mSeedPackets[0].SetPacketType(SeedType::SEED_SUNFLOWER, SeedType::SEED_NONE);
            for (int i = 0; i < mApp->mBoard->GetNumSeedsInBank(false) - 1; ++i) {
                mApp->mBoard->mSeedBank[0]->mSeedPackets[i + 1].SetPacketType(SeedType(event1->data[i]), SeedType::SEED_NONE);
            }

            mApp->mBoard->mSeedBank[1]->mSeedPackets[0].SetPacketType(SeedType::SEED_ZOMBIE_GRAVESTONE, SeedType::SEED_NONE);
            for (int i = 0; i < mApp->mBoard->GetNumSeedsInBank(true) - 1; ++i) {
                mApp->mBoard->mSeedBank[1]->mSeedPackets[i + 1].SetPacketType(SeedType(event1->data[i + 6]), SeedType::SEED_NONE);
            }
        } break;
        case EVENT_VSSETUPMENU_MOVE_CONTROLLER: {
            U16_Event *event1 = (U16_Event *)event;
            Sexy::Widget *theController1Widget = FindWidget(7);
            theController1Widget->Move(event1->data, theController1Widget->mY);
            is1PControllerMoving = true;
        } break;
        case EVENT_VSSETUPMENU_SET_CONTROLLER: {
            U8_Event *event1 = (U8_Event *)event;
            VSSide aSide = event1->data == 2 ? VS_SIDE_NONE : VSSide(event1->data);
            if (mSides[0] == aSide) {
                GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 0, 0);
            }
            mSides[0] = aSide;
            is1PControllerMoving = false;
            if (mSides[0] != VS_SIDE_NONE && mSides[1] != VS_SIDE_NONE && mSides[0] != mSides[1]) {
                GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 0, 0);
                GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 1, 0);
            }
        } break;
        case EVENT_SERVER_VSSETUP_ADDON_BUTTON_INIT: {
            B1x8_Event *eventButtonInit = static_cast<B1x8_Event *>(event);
            if (gVSSetupAddonWidget) {
                gVSSetupAddonWidget->mExtraPacketsMode = eventButtonInit->data1;
                gVSSetupAddonWidget->mExtraSeedsMode = eventButtonInit->data2;
                gVSSetupAddonWidget->mBanMode = eventButtonInit->data3;
                gVSSetupAddonWidget->mBalancePatchMode = eventButtonInit->data4;
            }
            mApp->mPlayerInfo->mVSExtraPacketsMode = eventButtonInit->data1;
            mApp->mPlayerInfo->mVSExtraSeedsMode = eventButtonInit->data2;
            mApp->mPlayerInfo->mVSBanMode = eventButtonInit->data3;
            mApp->mPlayerInfo->mVSBalancePatchMode = eventButtonInit->data4;
        } break;
        default:
            break;
    }
}


void VSSetupMenu::KeyDown(Sexy::KeyCode theKey) {
    // 修复在对战的阵营选取界面无法按返回键退出的BUG。
    if (theKey == Sexy::KeyCode::KEYCODE_ESCAPE) {
        switch (mState) {
            case VS_SETUP_STATE_CONTROLLERS:
                break;
            case VS_SETUP_STATE_SIDES:
            case VS_SETUP_STATE_SELECT_BATTLE:
                mApp->DoBackToMain();
                return;
            case VS_SETUP_STATE_CUSTOM_BATTLE: // 自定义战场
                mApp->DoNewOptions(false, 0);
                return;
        }
    }

    old_VSSetupMenu_KeyDown(this, theKey);
}

void VSSetupMenu::OnStateEnter(VSSetupState theState) {
    if (theState == VSSetupState::VS_SETUP_STATE_SIDES) {
        drawTipArrowAlphaCounter = 0;

        if (tcpClientSocket >= 0) {
            B1x8_Event event = {
                {EventType::EVENT_SERVER_VSSETUP_ADDON_BUTTON_INIT},
                gVSSetupAddonWidget->mExtraPacketsMode,
                gVSSetupAddonWidget->mExtraSeedsMode,
                gVSSetupAddonWidget->mBanMode,
                gVSSetupAddonWidget->mBalancePatchMode,
            };
            SendEvent(tcpClientSocket, event);
        }
    }
    if (theState == VSSetupState::VS_SETUP_STATE_CONTROLLERS) {

        if (tcp_connected || tcpClientSocket >= 0) {
            SetSecondPlayerIndex(mApp->mTwoPlayerState);
            GoToState(VSSetupState::VS_SETUP_STATE_SIDES);
            return;
        }
        mControllerIndex[1] = -1;
        auto *aWaitDialog = new WaitForSecondPlayerDialog(mApp);
        mApp->AddDialog(aWaitDialog);

        int aButtonId = aWaitDialog->WaitForResult(true);
        if (aButtonId == 1000) {
            SetSecondPlayerIndex(mApp->mTwoPlayerState);
            GoToState(VSSetupState::VS_SETUP_STATE_SIDES);
        } else if (aButtonId == 1001) {
            CloseVSSetup(true);
            mApp->KillBoard();
            mApp->ShowGameSelector();
        }
        return;
    } else if (theState == VSSetupState::VS_SETUP_STATE_SELECT_BATTLE) {
        gGamepad1ToPlayerIndex = mSides[0];

        if (gIsVSShuffleMode) {
            ButtonDepress(VSSetupMenu_Random_Battle);
        }
    } else if (tcpClientSocket >= 0) {
        U8_Event event = {{EventType::EVENT_VSSETUPMENU_ENTER_STATE}, uint8_t(theState)};
        SendEvent(tcpClientSocket, event);
    }

    old_VSSetupMenu_OnStateEnter(this, theState);

    if (mState == VS_SETUP_STATE_CUSTOM_BATTLE) {
        mSeedPickTurn = msNextFirstPick; // 选卡先手方
    }
}

void VSSetupMenu::ButtonPress(int theId) {
    old_VSSetupMenu_ButtonPress(this, theId);
}

void VSSetupMenu::ButtonDepress(int theId) {
    if (gVSSetupRequestState == theId) {
        gVSSetupRequestState = 0;
    }

    if (tcp_connected) {
        U8_Event event = {{EventType::EVENT_CLIENT_VSSETUPMENU_BUTTON_DEPRESS}, uint8_t(theId)};
        SendEvent(tcpServerSocket, event);
        gVSSetupRequestState = theId;
        return;
    }

    if (tcpClientSocket >= 0) {
        U8_Event event = {{EventType::EVENT_SERVER_VSSETUPMENU_BUTTON_DEPRESS}, uint8_t(theId)};
        SendEvent(tcpClientSocket, event);
    }

    int aNumPackets = mApp->mBoard->GetNumSeedsInBank(false);

    SeedBank *aPlantBank = mApp->mBoard->mSeedBank[0];
    SeedBank *aZombieBank = mApp->mBoard->mSeedBank[1];

    aPlantBank->mNumPackets = aZombieBank->mNumPackets = aNumPackets;

    SeedType aSunPlantType = mApp->mBoard->StageIsNight() ? SeedType::SEED_SUNSHROOM : SeedType::SEED_SUNFLOWER;

    if (mState == VSSetupState::VS_SETUP_STATE_SELECT_BATTLE) {
        switch (theId) {
            case VSSetupMenu_Quick_Play: {
                for (int aPlayerIndex = 0; aPlayerIndex < 2; ++aPlayerIndex) {
                    for (int aPacketIndex = 0; aPacketIndex < 6; ++aPacketIndex) {
                        SeedType aSeedType = msQuickPlayDecks[aPlayerIndex][aPacketIndex];
                        mApp->mBoard->mSeedBank[aPlayerIndex]->mSeedPackets[aPacketIndex].SetPacketType(aSeedType, SeedType::SEED_NONE);
                    }
                }

                mApp->mBoard->mSeedBank[0]->mSeedPackets[0].SetPacketType(aSunPlantType, SeedType::SEED_NONE);

                mSetupMode = VSSetupMode::VS_SETUP_MODE_QUICK_PLAY;
                CloseVSSetup(false);

                if (mApp->mPlayerInfo->mVSExtraPacketsMode) { // 额外卡槽
                    aPlantBank->mSeedPackets[6].SetPacketType(SeedType::SEED_TORCHWOOD, SeedType::SEED_NONE);
                    aZombieBank->mSeedPackets[6].SetPacketType(SeedType::SEED_ZOMBIE_PAIL, SeedType::SEED_NONE);
                }
            } break;

            case VSSetupMenu_Custom_Battle: {
                mApp->ShowSeedChooserScreen();
                mApp->ShowZombieChooserScreen();

                for (int aPlayerIndex = 0; aPlayerIndex < 2; ++aPlayerIndex) {
                    VSSide aSides = mSides[aPlayerIndex];
                    int aControllerIdx = mControllerIndex[aPlayerIndex];

                    if (aSides == VSSide::VS_SIDE_ZOMBIE) {
                        mApp->mZombieChooserScreen->mPlayerIndex = aControllerIdx;
                    } else if (aSides == VSSide::VS_SIDE_PLANT) {
                        mApp->mSeedChooserScreen->mPlayerIndex = aControllerIdx;
                    }
                }

                mSetupMode = VSSetupMode::VS_SETUP_MODE_CUSTOM_BATTLE;
                GoToState(VSSetupState::VS_SETUP_STATE_CUSTOM_BATTLE);

                if (mState == VS_SETUP_STATE_CUSTOM_BATTLE) {
                    gVSSetupAddonWidget->SetDisable(gVSSetupAddonWidget->mExtraPacketsButton);
                    gVSSetupAddonWidget->SetDisable(gVSSetupAddonWidget->mExtraSeedsButton);
                    gVSSetupAddonWidget->SetDisable(gVSSetupAddonWidget->mBanModeButton);
                    gVSSetupAddonWidget->SetDisable(gVSSetupAddonWidget->mBalancePatchButton);
                    gVSSetupAddonWidget->mDrawString = false;
                    PickBackgroundImmediately();
                }
                if (mApp->mPlayerInfo->mVSBanMode) { // 禁选模式下交换双方控制权
                    mApp->mBoard->SwitchGamepadControls();
                }
            } break;

            case VSSetupMenu_Random_Battle: {
                if (gIsVSShuffleMode) {
                    gFreeForFristShuffle[0] = true;
                    gFreeForFristShuffle[1] = true;
                    aPlantBank->mNumPackets = aZombieBank->mNumPackets = 7;
                    aPlantBank->mSeedPackets[6].SetPacketType(SEED_BEGHOULED_BUTTON_SHUFFLE, SeedType::SEED_NONE);
                    aZombieBank->mSeedPackets[6].SetPacketType(SEED_ZOMBIE_BEGHOULED_BUTTON_SHUFFLE, SeedType::SEED_NONE);
                }

                std::vector<SeedType> aZombieSeeds;
                PickRandomZombies(aZombieSeeds);

                mApp->mBoard->mSeedBank[1]->mSeedPackets[0].SetPacketType(SeedType::SEED_ZOMBIE_GRAVESTONE, SeedType::SEED_NONE);

                if (!aZombieSeeds.empty()) {
                    for (int aPacketIndex = 1; aPacketIndex <= aZombieSeeds.size(); ++aPacketIndex) {
                        SeedType aSeedType = aZombieSeeds[aPacketIndex - 1];
                        mApp->mBoard->mSeedBank[1]->mSeedPackets[aPacketIndex].SetPacketType(aSeedType, SeedType::SEED_NONE);
                    }
                }

                std::vector<SeedType> aPlantSeeds;
                PickRandomPlants(aPlantSeeds, aZombieSeeds);

                mApp->mBoard->mSeedBank[0]->mSeedPackets[0].SetPacketType(aSunPlantType, SeedType::SEED_NONE);

                if (!aPlantSeeds.empty()) {
                    for (int aPacketIndex = 1; aPacketIndex <= aPlantSeeds.size(); ++aPacketIndex) {
                        SeedType aSeedType = aPlantSeeds[aPacketIndex - 1];
                        mApp->mBoard->mSeedBank[0]->mSeedPackets[aPacketIndex].SetPacketType(aSeedType, SeedType::SEED_NONE);
                    }
                }

                mSetupMode = VSSetupMode::VS_SETUP_MODE_RANDOM_BATTLE;
                CloseVSSetup(false);
            } break;

            default:
                break;
        }
    }

    switch (theId) {
        case VSSetupAddonWidget::VSSetupAddonWidget_ExtraPackets: // 额外卡槽
        case VSSetupAddonWidget::VSSetupAddonWidget_ExtraSeeds:   // 拓展选卡
        case VSSetupAddonWidget::VSSetupAddonWidget_BanMode:      // 禁选模式
        case VSSetupAddonWidget::VSSetupAddonWidget_BalancePatch: // 平衡调整
            gVSSetupAddonWidget->ButtonDepress(theId);
            break;
        default:
            break;
    }

    // 修复“额外卡槽”开启后卡槽位置不正确
    for (int i = 0; i < SEEDBANK_MAX; i++) {
        SeedPacket *aPlantPacket = &aPlantBank->mSeedPackets[i];
        SeedPacket *aZombiePacket = &aZombieBank->mSeedPackets[i];
        aPlantPacket->mIndex = i;
        aPlantPacket->mX = mApp->mBoard->GetSeedPacketPositionX(i, 0, false);
        aZombiePacket->mX = mApp->mBoard->GetSeedPacketPositionX(i, 1, true);
    }
}

void VSSetupMenu::PickBackgroundImmediately() {
    // 如果修改器里开启了更换场地
    if (VSBackGround != 0 && VSBackGround != mApp->mBoard->mBackground + 1) {

        if (tcp_connected) {
            // 客户端
            return;
        }

        for (int i = 0; i < 6; ++i) {
            mApp->RemoveReanimation(mApp->mBoard->mCoverLayerAnimIDs[i]);
        }
        mApp->mBoard->PickBackground(); // 立即更换
        mApp->mBoard->RemoveAllMowers();
        mApp->mBoard->RemoveAllPlants();
        mApp->mBoard->RemoveAllGridItems();
        mApp->mBoard->mCutScene->mPlacedLawnItems = false;
        mApp->mBoard->mCutScene->PlaceLawnItems();


        if (tcpClientSocket >= 0) {
            U8_Event event = {{EventType::EVENT_SERVER_VSSETUPMENU_PICKBACKGROUND}, uint8_t(VSBackGround)};
            SendEvent(tcpClientSocket, event);
        }
    }
}

void VSSetupMenu::CloseVSSetup(bool a2) {
    PickBackgroundImmediately();

    old_VSSetupMenu_CloseVSSetup(this, a2);
}