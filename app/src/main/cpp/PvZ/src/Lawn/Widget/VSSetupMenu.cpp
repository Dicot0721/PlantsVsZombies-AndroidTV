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
#include "PvZ/Lawn/Board/CutScene.h"
#include "PvZ/Lawn/Board/SeedBank.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/Widget/SeedChooserScreen.h"
#include "PvZ/Lawn/Widget/WaitForSecondPlayerDialog.h"
#include "PvZ/TodLib/Common/TodStringFile.h"
#include <unistd.h>

using namespace Sexy;

void VSSetupMenu::_constructor() {
    old_VSSetupMenu_Constructor(this);
    msNextFirstPick = VS_FIRST_PICK_ZOMBIE;

    // 拓展卡槽,禁选模式
    gVSSetupAddonWidget = new VSSetupAddonWidget(this);

    //    gVSSelectBgDayButton = MakeNewButton(9000,&mButtonListener, this, "", nullptr, aCheckbox, aCheckbox, aCheckbox);
    //    gVSSelectBgNightButton = MakeNewButton(9001,&mButtonListener, this, "", nullptr, aCheckbox, aCheckbox, aCheckbox);
    //    gVSSelectBgPoolButton = MakeNewButton(9002,&mButtonListener, this, "", nullptr, aCheckbox, aCheckbox, aCheckbox);
    //    gVSSelectBgPoolNightButton = MakeNewButton(9003,&mButtonListener, this, "", nullptr, aCheckbox, aCheckbox, aCheckbox);
    //    gVSSelectBgRoofButton = MakeNewButton(9004,&mButtonListener, this, "", nullptr, aCheckbox, aCheckbox, aCheckbox);
    //    gVSSelectBgRoofNightButton = MakeNewButton(9005,&mButtonListener, this, "", nullptr, aCheckbox, aCheckbox, aCheckbox);
    //
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
        TodDrawString(g, "[VS_PICK_SIDES]", 350, 110, *Sexy::FONT_DWARVENTODCRAFT18, Color::White, DrawStringJustification::DS_ALIGN_LEFT);
    } else if (mState == VSSetupState::VS_SETUP_STATE_SELECT_BATTLE) {
        TodDrawString(g, "[VS_PICK_BATTLES]", 350, 110, *Sexy::FONT_DWARVENTODCRAFT18, Color::White, DrawStringJustification::DS_ALIGN_LEFT);
    }

    if (drawTipArrowAlphaCounter > 200) {
        int aAlpha = TodAnimateCurve(0, 100, drawTipArrowAlphaCounter % 100, 50, 255, TodCurves::CURVE_BOUNCE);
        Color theColor = {255, 255, 255, aAlpha};
        g->SetColorizeImages(true);
        g->SetColor(theColor);

        if (!tcp_connected && mSides[0] == -1) {
            Sexy::Widget *theController1Widget = FindWidget(7);
            g->DrawImage(*Sexy_IMAGE_ZEN_NEXTGARDEN_Addr, theController1Widget->mX + 160, theController1Widget->mY + 40);
            g->DrawImageMirror(*Sexy_IMAGE_ZEN_NEXTGARDEN_Addr, theController1Widget->mX - 50, theController1Widget->mY + 40, true);
        }


        if (tcpClientSocket < 0 && mSides[1] == -1) {
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
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy::FONT_HOUSEOFTERROR28, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                case VSSetupMenu_Custom_Battle: {
                    pvzstl::string fmt = TodStringTranslate("[VS_TIP_REMIND_HOST_FMT]");
                    pvzstl::string opt = TodStringTranslate("[VS_OPT_CUSTOM_ARENA]");
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy::FONT_HOUSEOFTERROR28, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                case VSSetupMenu_Random_Battle: {
                    pvzstl::string fmt = TodStringTranslate("[VS_TIP_REMIND_HOST_FMT]");
                    pvzstl::string opt = TodStringTranslate("[VS_OPT_RANDOM_ARENA]");
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy::FONT_HOUSEOFTERROR28, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                case VSSetupAddonWidget::VSSetupAddonWidget_ExtraPackets: {
                    pvzstl::string fmt = TodStringTranslate("[VS_TIP_REMIND_HOST_FMT]");
                    pvzstl::string opt = TodStringTranslate((gVSSetupAddonWidget && !gVSSetupAddonWidget->mExtraPacketsMode) ? "[VS_OPT_ENABLE_EXTRA_SLOTS]" : "[VS_OPT_DISABLE_EXTRA_SLOTS]");
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy::FONT_HOUSEOFTERROR28, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                case VSSetupAddonWidget::VSSetupAddonWidget_ExtraSeeds: {
                    pvzstl::string fmt = TodStringTranslate("[VS_TIP_REMIND_HOST_FMT]");
                    pvzstl::string opt = TodStringTranslate((gVSSetupAddonWidget && !gVSSetupAddonWidget->mExtraSeedsMode) ? "[VS_OPT_ENABLE_EXTRA_SEEDS]" : "[VS_OPT_DISABLE_EXTRA_SEEDS]");
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy::FONT_HOUSEOFTERROR28, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                case VSSetupAddonWidget::VSSetupAddonWidget_BanMode: {
                    pvzstl::string fmt = TodStringTranslate("[VS_TIP_REMIND_HOST_FMT]");
                    pvzstl::string opt = TodStringTranslate((gVSSetupAddonWidget && !gVSSetupAddonWidget->mBanMode) ? "[VS_OPT_ENABLE_BAN_MODE]" : "[VS_OPT_DISABLE_BAN_MODE]");
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy::FONT_HOUSEOFTERROR28, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                case VSSetupAddonWidget::VSSetupAddonWidget_BalancePatch: {
                    pvzstl::string fmt = TodStringTranslate("[VS_TIP_REMIND_HOST_FMT]");
                    pvzstl::string opt = TodStringTranslate((gVSSetupAddonWidget && !gVSSetupAddonWidget->mBalancePatchMode) ? "[VS_OPT_ENABLE_BALANCE_PATCH]" : "[VS_OPT_DISABLE_BALANCE_PATCH]");
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy::FONT_HOUSEOFTERROR28, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
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
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy::FONT_HOUSEOFTERROR28, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                case VSSetupMenu_Custom_Battle: {
                    pvzstl::string fmt = TodStringTranslate("[VS_TIP_OPPONENT_WANTS_PLAY_FMT]");
                    pvzstl::string opt = TodStringTranslate("[VS_OPT_CUSTOM_ARENA]");
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy::FONT_HOUSEOFTERROR28, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                case VSSetupMenu_Random_Battle: {
                    pvzstl::string fmt = TodStringTranslate("[VS_TIP_OPPONENT_WANTS_PLAY_FMT]");
                    pvzstl::string opt = TodStringTranslate("[VS_OPT_RANDOM_ARENA]");
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy::FONT_HOUSEOFTERROR28, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                case VSSetupAddonWidget::VSSetupAddonWidget_ExtraPackets: {
                    pvzstl::string fmt = TodStringTranslate("[VS_TIP_OPPONENT_WANTS_GET_FMT]");
                    pvzstl::string opt = TodStringTranslate((gVSSetupAddonWidget && !gVSSetupAddonWidget->mExtraPacketsMode) ? "[VS_OPT_ENABLE_EXTRA_SLOTS]" : "[VS_OPT_DISABLE_EXTRA_SLOTS]");
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy::FONT_HOUSEOFTERROR28, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                case VSSetupAddonWidget::VSSetupAddonWidget_ExtraSeeds: {
                    pvzstl::string fmt = TodStringTranslate("[VS_TIP_OPPONENT_WANTS_GET_FMT]");
                    pvzstl::string opt = TodStringTranslate((gVSSetupAddonWidget && !gVSSetupAddonWidget->mExtraSeedsMode) ? "[VS_OPT_ENABLE_EXTRA_SEEDS]" : "[VS_OPT_DISABLE_EXTRA_SEEDS]");
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy::FONT_HOUSEOFTERROR28, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                case VSSetupAddonWidget::VSSetupAddonWidget_BanMode: {
                    pvzstl::string fmt = TodStringTranslate("[VS_TIP_OPPONENT_WANTS_GET_FMT]");
                    pvzstl::string opt = TodStringTranslate((gVSSetupAddonWidget && !gVSSetupAddonWidget->mBanMode) ? "[VS_OPT_ENABLE_BAN_MODE]" : "[VS_OPT_DISABLE_BAN_MODE]");
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy::FONT_HOUSEOFTERROR28, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                case VSSetupAddonWidget::VSSetupAddonWidget_BalancePatch: {
                    pvzstl::string fmt = TodStringTranslate("[VS_TIP_OPPONENT_WANTS_GET_FMT]");
                    pvzstl::string opt = TodStringTranslate((gVSSetupAddonWidget && !gVSSetupAddonWidget->mBalancePatchMode) ? "[VS_OPT_ENABLE_BALANCE_PATCH]" : "[VS_OPT_DISABLE_BALANCE_PATCH]");
                    TodDrawString(g, StrFormat(fmt.c_str(), opt.c_str()), 140, 620, *Sexy::FONT_HOUSEOFTERROR28, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
                    break;
                }
                default:
                    break;
            }
        }
    }


    if (gVSSetupAddonWidget) {
        if (gVSSetupAddonWidget->mDrawString) {
            g->SetFont(*Sexy_FONT_DWARVENTODCRAFT18_Addr);
            g->SetColor(Color(0, 205, 0, 255));
            g->DrawString(TodStringTranslate("[VS_UI_EXTRA_SLOTS]"), VS_ADDON_BUTTON_X + 40, VS_BUTTON_EXTRA_PACKETS_Y + 25);
            g->DrawString(TodStringTranslate("[VS_UI_EXTRA_SEEDS]"), VS_ADDON_BUTTON_X + 40, VS_BUTTON_EXTRA_SEEDS_Y + 25);
            g->DrawString(TodStringTranslate("[VS_UI_BAN_MODE]"), VS_ADDON_BUTTON_X + 40, VS_BUTTON_BAN_MODE_Y + 25);
            g->DrawString(TodStringTranslate("[VS_UI_BALANCE_PATCH]"), VS_ADDON_BUTTON_X + 40, VS_BUTTON_BALANCE_PATCH_Y + 25);

            if (gVSSetupAddonWidget->mBanMode) {
                g->SetColor(Color(205, 0, 0, 255));
                g->DrawString(TodStringTranslate("[VS_UI_BAN_PHASE_BIG]"), 200, 45);
            }
        }
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
            sendWithSize(tcpClientSocket, &event, sizeof(U16_Event), 0);
        }
    } else if (touchingOnWhichController == 2) {
        if (tcpClientSocket >= 0)
            return;
        Sexy::Widget *theController2Widget = FindWidget(8);
        theController2Widget->Move(theController2Widget->mX + x - touchDownX, theController2Widget->mY);
        if (tcpServerSocket >= 0) {
            U16_Event event = {{EventType::EVENT_VSSETUPMENU_MOVE_CONTROLLER}, uint16_t(theController2Widget->mX)};
            sendWithSize(tcpServerSocket, &event, sizeof(U16_Event), 0);
        }
    }
    touchDownX = x;
}

void VSSetupMenu::MouseUp(int x, int y, int theCount) {

    if (touchingOnWhichController == 1) {
        if (tcp_connected)
            return;
        Sexy::Widget *theController1Widget = FindWidget(7);
        int newController1Position = theController1Widget->mX > 400 ? 1 : theController1Widget->mX > 250 ? -1 : 0;
        if (newController1Position == mSides[0]) {
            GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 0, 0);
        }
        mSides[0] = newController1Position;
        if (tcpClientSocket >= 0) {
            U8_Event event = {{EventType::EVENT_VSSETUPMENU_SET_CONTROLLER}, mSides[0] == -1 ? uint8_t(2) : uint8_t(mSides[0])};
            sendWithSize(tcpClientSocket, &event, sizeof(U8_Event), 0);
        }
        is1PControllerMoving = false;
    } else if (touchingOnWhichController == 2) {
        if (tcpClientSocket >= 0)
            return;
        Sexy::Widget *theController2Widget = FindWidget(8);
        int newController2Position = theController2Widget->mX > 400 ? 1 : theController2Widget->mX > 250 ? -1 : 0;

        if (newController2Position == mSides[1]) {
            GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 1, 0);
        }
        mSides[1] = newController2Position;
        if (tcpServerSocket >= 0) {
            U8_Event event = {{EventType::EVENT_VSSETUPMENU_SET_CONTROLLER}, mSides[1] == -1 ? uint8_t(2) : uint8_t(mSides[1])};
            sendWithSize(tcpServerSocket, &event, sizeof(U8_Event), 0);
        }
        is2PControllerMoving = false;
    }
    touchingOnWhichController = 0;
    if (mSides[0] != -1 && mSides[1] != -1 && mSides[0] != mSides[1]) {
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
    // 原一共要选 5 个，此处改为 (卡槽数 - 1) 个
    for (int num_possible = 0; num_possible < mApp->mBoard->GetNumSeedsInBank(false) - 1; ++num_possible) {
        // ------------------------------------------------------------
        // 1) 选择使用哪个 pool 组（原 v5），规则与 IDA 一致
        // num_possible: 0,1 -> group 0
        //               2,3 -> group 1
        //               4   -> group 2
        //        5（额外卡槽） -> group 2
        // ------------------------------------------------------------
        int pool = 0;
        if (num_possible == 2 || num_possible == 3)
            pool = 1;
        else if (num_possible == 4 || num_possible == 5)
            pool = 2;

        // zombies 的 pool 从 msRandomPools[48] 开始，每组 8 个
        const int poolBase = 48 + 8 * pool;

        // ------------------------------------------------------------
        // 2) 统计该池有效元素个数 validCount（<=8，遇到 -1 截断）
        //    并保留 IDA 里那次“无用但可能有副作用”的 HasSeedType 调用
        // ------------------------------------------------------------
        int validCount = 0;
        for (int i = 0; i < 8; ++i) {
            const SeedType aSeedType = msRandomPools[poolBase + i];
            if (aSeedType == SEED_NONE)
                break;

            ++validCount;
        }

        // ------------------------------------------------------------
        // 3) 从该池中随机挑一个：
        //    (1) 不能与 theZombieSeeds 重复
        //    (2) mApp->HasSeedType(seed, 1) 必须为真
        // ------------------------------------------------------------
        SeedType aSeedType = SEED_NONE;
        while (true) {
            // 挑到一个不重复的候选
            do {
                const int idx = Sexy::Rand(validCount);
                aSeedType = msRandomPools[poolBase + idx];
            } while (std::find(theZombieSeeds.begin(), theZombieSeeds.end(), aSeedType) != theZombieSeeds.end());

            // 校验可用
            if (mApp->HasSeedType(aSeedType, 1))
                break;
        }

        theZombieSeeds.push_back(aSeedType);
    }
}

void VSSetupMenu::PickRandomPlants(std::vector<SeedType> &thePlantSeeds, const std::vector<SeedType> &theZombieSeeds) {
    // ------------------------------------------------------------
    // 1) 有概率先塞一个固定种子 SEED_INSTANT_COFFEE
    // ------------------------------------------------------------
    int alreadyPicked = 0;
    if ((mApp->mPlayerInfo->mLevel > 20 || mApp->HasFinishedAdventure()) && Sexy::Rand(5) == 1) {
        thePlantSeeds.push_back(SEED_INSTANT_COFFEE);
        alreadyPicked = 1;
    }

    // IDA：v26 = 3 * v5（v5 是 0/1）
    const int poolGroupOffset = 3 * alreadyPicked; // 0 或 3

    // ------------------------------------------------------------
    // 2) 从随机池中继续挑，直到总共 (卡槽数 - 1) 个 plant seed
    // ------------------------------------------------------------

    for (int num_possible = alreadyPicked; num_possible < mApp->mBoard->GetNumSeedsInBank(true) - 1; ++num_possible) {
        // 原 v6：根据 num_possible 选择池子变体
        int pool = 0;
        if (num_possible == 2 || num_possible == 3)
            pool = 1;
        else if (num_possible == 4 || num_possible == 5)
            pool = 2;

        // 池子基址：8 个一组
        const int poolBase = 8 * (poolGroupOffset + pool);

        // 统计有效元素数：最多 8 个，遇到 -1 截断
        int validCount = 0;
        for (int i = 0; i < 8; ++i) {
            const SeedType aSeedType = msRandomPools[poolBase + i];
            if (aSeedType == SeedType::SEED_NONE)
                break;

            ++validCount;
        }

        // 从该池里随机挑一个：
        // 1) 不与 thePlantSeeds 重复
        // 2) mApp->HasSeedType 为真
        SeedType aSeedType = SEED_NONE;
        while (true) {
            // 先挑一个不重复的
            do {
                const int idx = Sexy::Rand(validCount);
                aSeedType = (SeedType)msRandomPools[poolBase + idx];
            } while (std::find(thePlantSeeds.begin(), thePlantSeeds.end(), aSeedType) != thePlantSeeds.end());

            // 再确认可用
            if (mApp->HasSeedType(aSeedType, 0))
                break;
        }

        thePlantSeeds.push_back(aSeedType);
    }

    // ------------------------------------------------------------
    // 3) 如果 zombieSeeds 里包含蹦蹦僵尸，则做 “坚果 -> 高坚果” 替换（前提拥有坚果）
    // ------------------------------------------------------------
    const bool zombieHasPogo = (std::find(theZombieSeeds.begin(), theZombieSeeds.end(), SEED_ZOMBIE_POGO) != theZombieSeeds.end());

    if (zombieHasPogo && mApp->HasSeedType(SEED_WALLNUT, 0)) {
        auto it = std::find(thePlantSeeds.begin(), thePlantSeeds.end(), SEED_WALLNUT);
        if (it != thePlantSeeds.end()) {
            *it = SEED_TALLNUT;
        }
    }

    if (tcpClientSocket >= 0) {
        U16x12_Event event;
        event.type = EventType::EVENT_VSSETUPMENU_RANDOM_PICK;
        for (int i = 0; i < thePlantSeeds.size(); ++i) {
            event.data[i] = thePlantSeeds[i];
        }

        for (int i = 0; i < theZombieSeeds.size(); ++i) {
            event.data[i + 6] = theZombieSeeds[i];
        }

        sendWithSize(tcpClientSocket, &event, sizeof(U16x12_Event), 0);
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
            int realData = event1->data == 2 ? -1 : event1->data;
            if (mSides[1] == realData) {
                GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 1, 0);
            }
            mSides[1] = realData;
            is2PControllerMoving = false;
            if (mSides[0] != -1 && mSides[1] != -1 && mSides[0] != mSides[1]) {
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
            return sizeof(U8U8U8U8_Event);
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
                mApp->mBoard->mSeedBank[0]->mSeedPackets[i + 1].SetPacketType((SeedType)event1->data[i], SeedType::SEED_NONE);
            }

            mApp->mBoard->mSeedBank[1]->mSeedPackets[0].SetPacketType(SeedType::SEED_ZOMBIE_GRAVESTONE, SeedType::SEED_NONE);
            for (int i = 0; i < mApp->mBoard->GetNumSeedsInBank(true) - 1; ++i) {
                mApp->mBoard->mSeedBank[1]->mSeedPackets[i + 1].SetPacketType((SeedType)event1->data[i + 6], SeedType::SEED_NONE);
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
            int realData = event1->data == 2 ? -1 : event1->data;
            if (mSides[0] == realData) {
                GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 0, 0);
            }
            mSides[0] = realData;
            is1PControllerMoving = false;
            if (mSides[0] != -1 && mSides[1] != -1 && mSides[0] != mSides[1]) {
                GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 0, 0);
                GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 1, 0);
            }
        } break;
        case EVENT_SERVER_VSSETUP_ADDON_BUTTON_INIT: {
            U8U8U8U8_Event *eventButtonInit = static_cast<U8U8U8U8_Event *>(event);
            gVSSetupAddonWidget->mExtraPacketsMode = eventButtonInit->data1;
            gVSSetupAddonWidget->mExtraSeedsMode = eventButtonInit->data2;
            gVSSetupAddonWidget->mBanMode = eventButtonInit->data3;
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
            U8U8U8U8_Event event = {{EventType::EVENT_SERVER_VSSETUP_ADDON_BUTTON_INIT},
                                    uint8_t(gVSSetupAddonWidget->mExtraPacketsMode),
                                    uint8_t(gVSSetupAddonWidget->mExtraSeedsMode),
                                    uint8_t(gVSSetupAddonWidget->mBanMode),
                                    uint8_t(gVSSetupAddonWidget->mBalancePatchMode)};
            sendWithSize(tcpClientSocket, &event, sizeof(U8U8U8U8_Event), 0);
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
    } else if (tcpClientSocket >= 0) {
        U8_Event event = {{EventType::EVENT_VSSETUPMENU_ENTER_STATE}, uint8_t(theState)};
        sendWithSize(tcpClientSocket, &event, sizeof(U8_Event), 0);
    }

    old_VSSetupMenu_OnStateEnter(this, theState);

    if (mState == VS_SETUP_STATE_CUSTOM_BATTLE) {
        mNextFirstPick = msNextFirstPick;
    }
}

void VSSetupMenu::ButtonPress(int theId) {
    old_VSSetupMenu_ButtonPress(this, theId);
}

void VSSetupMenu::ButtonDepress(int theId) {
    if (!isKeyboardTwoPlayerMode && mState == VS_SETUP_STATE_SIDES) {
        // 自动分配阵营
        // GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_DPAD_LEFT, 0, 0);
        // GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_DPAD_RIGHT, 1, 0);
        if (mSides[0] != -1 && mSides[1] != -1 && mSides[0] != mSides[1]) {
            GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 0, 0);
            GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 1, 0);
        }
        //        else {
        //            return;
        // // 自动分配阵营
        // mSides[0] = 0;
        // mSides[1] = 1;
        // GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 0, 0);
        // GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 1, 0);
        //        }
    }

    if (gVSSetupRequestState == theId) {
        gVSSetupRequestState = 0;
    }

    if (tcp_connected) {
        U8_Event event = {{EventType::EVENT_CLIENT_VSSETUPMENU_BUTTON_DEPRESS}, uint8_t(theId)};
        sendWithSize(tcpServerSocket, &event, sizeof(U8_Event), 0);
        gVSSetupRequestState = theId;
        return;
    }

    if (tcpClientSocket >= 0) {
        U8_Event event = {{EventType::EVENT_SERVER_VSSETUPMENU_BUTTON_DEPRESS}, uint8_t(theId)};
        sendWithSize(tcpClientSocket, &event, sizeof(U8_Event), 0);
    }

    old_VSSetupMenu_ButtonDepress(this, theId);

    // 对战额外卡槽
    int aNumPackets = mApp->mBoard->GetNumSeedsInBank(false);

    SeedBank *aSeedBank1 = mApp->mBoard->mSeedBank[0];
    SeedBank *aSeedBank2 = mApp->mBoard->mSeedBank[1];

    aSeedBank1->mNumPackets = aNumPackets;
    aSeedBank2->mNumPackets = aNumPackets;

    switch (theId) {
        case VSSetupMenu_Quick_Play:
            if (mState == VSSetupState::VS_SETUP_STATE_SELECT_BATTLE && aNumPackets == 7) {
                aSeedBank1->mSeedPackets[3].SetPacketType(SeedType::SEED_TORCHWOOD, SeedType::SEED_NONE);
                aSeedBank1->mSeedPackets[4].SetPacketType(SeedType::SEED_POTATOMINE, SeedType::SEED_NONE);
                aSeedBank1->mSeedPackets[5].SetPacketType(SeedType::SEED_SQUASH, SeedType::SEED_NONE);
                aSeedBank1->mSeedPackets[6].SetPacketType(SeedType::SEED_JALAPENO, SeedType::SEED_NONE);
                aSeedBank2->mSeedPackets[4].SetPacketType(SeedType::SEED_ZOMBIE_PAIL, SeedType::SEED_NONE);
                aSeedBank2->mSeedPackets[5].SetPacketType(SeedType::SEED_ZOMBIE_FOOTBALL, SeedType::SEED_NONE);
                aSeedBank2->mSeedPackets[6].SetPacketType(SeedType::SEED_ZOMBIE_FLAG, SeedType::SEED_NONE);
            }
            break;
        case VSSetupMenu_Custom_Battle:
            if (mState == VS_SETUP_STATE_CUSTOM_BATTLE) {
                gVSSetupAddonWidget->SetDisable();
                PickBackgroundImmediately();
            }
            if (gVSSetupAddonWidget && gVSSetupAddonWidget->mBanMode) { // 禁选模式下交换双方控制权
                mApp->mBoard->SwitchGamepadControls();
            }
            break;
        case VSSetupMenu_Random_Battle:
            if (aNumPackets == 7) {
                //                mApp->mBoard->mSeedBank[0]->mNumPackets = 6;
                //                mApp->mBoard->mSeedBank[1]->mNumPackets = 6;
                // 开启“额外开槽”后随机选卡会导致界面卡死
                // std::vector<SeedType> aZombieSeeds, aPlantSeeds, tmpZombieSeeds, tmpPlantSeeds;
                //
                // PickRandomZombies(aZombieSeeds);
                // do {
                // PickRandomZombies(tmpZombieSeeds);
                // } while (tmpZombieSeeds[4] == aZombieSeeds[4]);
                // aZombieSeeds.push_back(tmpZombieSeeds[4]);
                //
                // PickRandomPlants(aPlantSeeds, aZombieSeeds);
                // do {
                // PickRandomPlants(tmpPlantSeeds, aZombieSeeds);
                // } while (tmpPlantSeeds[4] == aPlantSeeds[4]);
                // aPlantSeeds.push_back(tmpPlantSeeds[4]);
                //
                // aSeedBank2->mSeedPackets[0].SetPacketType(SeedType::SEED_ZOMBIE_GRAVESTONE, SeedType::SEED_NONE);
                // aSeedBank1->mSeedPackets[0].SetPacketType(SeedType::SEED_SUNFLOWER, SeedType::SEED_NONE);
                // for (int i = 0; i < aZombieSeeds.size(); ++i) {
                // aSeedBank2->mSeedPackets[i + 1].SetPacketType(aZombieSeeds[i], SeedType::SEED_NONE);
                // }
                // for (int i = 0; i < aPlantSeeds.size(); ++i) {
                // aSeedBank2->mSeedPackets[i + 1].SetPacketType(aPlantSeeds[i], SeedType::SEED_NONE);
                // }
            }
            break;
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
        SeedPacket *aPacket = &aSeedBank1->mSeedPackets[i];
        SeedPacket *aPacket2 = &aSeedBank2->mSeedPackets[i];
        aPacket->mIndex = i;
        aPacket->mX = mApp->mBoard->GetSeedPacketPositionX(i, 0, false);
        aPacket2->mX = mApp->mBoard->GetSeedPacketPositionX(i, 1, true);
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
            sendWithSize(tcpClientSocket, &event, sizeof(U8_Event), 0);
        }
    }
}

void VSSetupMenu::CloseVSSetup(bool a2) {

    PickBackgroundImmediately();

    old_VSSetupMenu_CloseVSSetup(this, a2);
}