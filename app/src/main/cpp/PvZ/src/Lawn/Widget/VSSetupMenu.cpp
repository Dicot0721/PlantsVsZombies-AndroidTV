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

#include "PvZ/Lawn/Widget/VSSetupMenu.h"
#include "Homura/Logger.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/SeedBank.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/Widget/SeedChooserScreen.h"
#include "PvZ/Lawn/Widget/WaitForSecondPlayerDialog.h"
#include <unistd.h>

using namespace Sexy;

VSSetupWidget::VSSetupWidget() {
    if (gMoreZombieSeeds) {
        game_patches::drawMoreZombieSeeds.Modify();
    }
}

VSSetupWidget::~VSSetupWidget() {
    gVSMorePacketsButton->mMorePacketsButton->mBtnNoDraw = true;
    gVSMorePacketsButton->mMorePacketsButton->mDisabled = true;
    gVSMorePacketsButton->mDrawString = false;
    gVSMorePacketsButton = nullptr;
}

void VSSetupWidget::SetDisable() {
    gVSMorePacketsButton->mMorePacketsButton->mBtnNoDraw = true;
    gVSMorePacketsButton->mMorePacketsButton->mDisabled = true;
    gVSMorePacketsButton->mDrawString = false;
}

void VSSetupWidget::ButtonDepress(this VSSetupWidget &self, int theId) {
    if (theId == 1145) {
        self.CheckboxChecked(1145, self.mIsMorePackets);
        std::swap(gVSMorePacketsButton->mCheckboxImage, gVSMorePacketsButton->mCheckboxImagePress);
        ButtonWidget *aButton = gVSMorePacketsButton->mMorePacketsButton;
        aButton->mButtonImage = gVSMorePacketsButton->mCheckboxImage;
        aButton->mOverImage = gVSMorePacketsButton->mCheckboxImage;
        aButton->mDownImage = gVSMorePacketsButton->mCheckboxImage;
    }
}

void VSSetupWidget::CheckboxChecked(int theId, bool checked) {
    switch (theId) {
        case 1145:
            mIsMorePackets = !checked;
            break;
        default:
            break;
    }
}

void VSSetupMenu::_constructor() {
    old_VSSetupMenu_Constructor(this);


    Image *aCheckbox = *Sexy_IMAGE_OPTIONS_CHECKBOX0_Addr;
    Image *aCheckboxPressed = *Sexy_IMAGE_OPTIONS_CHECKBOX1_Addr;
    // 拓展卡槽
    gVSMorePacketsButton = new VSSetupWidget;
    ButtonWidget *aMorePacketsButton = MakeNewButton(1145, &mButtonListener, this, "", nullptr, aCheckbox, aCheckbox, aCheckbox);
    gVSMorePacketsButton->mMorePacketsButton = aMorePacketsButton;
    gVSMorePacketsButton->mCheckboxImage = aCheckbox;
    gVSMorePacketsButton->mCheckboxImagePress = aCheckboxPressed;
    aMorePacketsButton->Resize(MORE_PACKETS_BUTTON_X, MORE_PACKETS_BUTTON_Y, 175, 50);
    mApp->mBoard->AddWidget(aMorePacketsButton);
    gVSMorePacketsButton->mDrawString = true;

    mApp->mDanceMode = false;
    mApp->mBoard->mDanceMode = false;

    is1PControllerMoving = false;
    is2PControllerMoving = false;
    touchingOnWhichController = 0;
}

void VSSetupMenu::_destructor() {
    old_VSSetupMenu_Destructor(this);

    if (gVSMorePacketsButton != nullptr)
        gVSMorePacketsButton->~VSSetupWidget();
}

void VSSetupMenu::Draw(Graphics *g) {
    old_VSSetupMenu_Draw(this, g);
}


void VSSetupMenu::AddedToManager(Sexy::WidgetManager *a2) {
    old_VSSetupMenu_AddedToManager(this, a2);
    // 缩小Widget，使得触控可传递给VSSetupMenu自身
    for (int i = 0; i < 9; ++i) {
        Sexy::Widget *theWidget = FindWidget(i);
        if (theWidget) {
            theWidget->Resize(theWidget->mX, theWidget->mY, 0, 0);
        }
    }
}

void VSSetupMenu::MouseDown(int x, int y, int theCount) {
    if (mState == VS_SETUP_SIDES) {

        Sexy::Widget *theController1Widget = FindWidget(7);
        Sexy::Widget *theController2Widget = FindWidget(8);
        if (x > theController1Widget->mX && x < theController1Widget->mX + 170 && y > theController1Widget->mY && y < theController1Widget->mY + 122) {
            is1PControllerMoving = true;
            touchingOnWhichController = 1;
        } else if (x > theController2Widget->mX && x < theController2Widget->mX + 170 && y > theController2Widget->mY && y < theController2Widget->mY + 122) {
            is2PControllerMoving = true;
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
            SimpleShortEvent event = {{EventType::EVENT_VSSETUPMENU_MOVE_CONTROLLER}, (short)theController1Widget->mX};
            send(tcpClientSocket, &event, sizeof(SimpleShortEvent), 0);
        }
    } else if (touchingOnWhichController == 2) {
        if (tcpClientSocket >= 0)
            return;
        Sexy::Widget *theController2Widget = FindWidget(8);
        theController2Widget->Move(theController2Widget->mX + x - touchDownX, theController2Widget->mY);
        if (tcpServerSocket >= 0) {
            SimpleShortEvent event = {{EventType::EVENT_VSSETUPMENU_MOVE_CONTROLLER}, (short)theController2Widget->mX};
            send(tcpServerSocket, &event, sizeof(SimpleShortEvent), 0);
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
        if (newController1Position == mController1Position) {
            GameButtonDown(ButtonCode::BUTTONCODE_A, 0, 0);
        }
        mController1Position = newController1Position;
        if (tcpClientSocket >= 0) {
            SimpleShortEvent event = {{EventType::EVENT_VSSETUPMENU_SET_CONTROLLER}, (short)mController1Position};
            send(tcpClientSocket, &event, sizeof(SimpleShortEvent), 0);
        }
        is1PControllerMoving = false;
    } else if (touchingOnWhichController == 2) {
        if (tcpClientSocket >= 0)
            return;
        Sexy::Widget *theController2Widget = FindWidget(8);
        int newController2Position = theController2Widget->mX > 400 ? 1 : theController2Widget->mX > 250 ? -1 : 0;

        if (newController2Position == mController2Position) {
            GameButtonDown(ButtonCode::BUTTONCODE_A, 1, 0);
        }
        mController2Position = newController2Position;
        if (tcpServerSocket >= 0) {
            SimpleShortEvent event = {{EventType::EVENT_VSSETUPMENU_SET_CONTROLLER}, (short)mController2Position};
            send(tcpServerSocket, &event, sizeof(SimpleShortEvent), 0);
        }
        is2PControllerMoving = false;
    }
    touchingOnWhichController = 0;
    if (mController1Position != -1 && mController2Position != -1 && mController1Position != mController2Position) {
        GameButtonDown(ButtonCode::BUTTONCODE_A, 0, 0);
        GameButtonDown(ButtonCode::BUTTONCODE_A, 1, 0);
    }
}

void VSSetupMenu::Update() {

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

    if (mState == VS_SETUP_CONTROLLERS) {
        return;
    }
    if (mState == VS_SETUP_SIDES && !tcp_connected && tcpClientSocket == -1 && !isKeyboardTwoPlayerMode) {
        // 本地游戏
        // 自动分配阵营
        mController1Position = 0;
        mController2Position = 1;
        GameButtonDown(ButtonCode::BUTTONCODE_A, 0, 0);
        GameButtonDown(ButtonCode::BUTTONCODE_A, 1, 0);
        return;
    }

    if (tcpClientSocket >= 0) {
        char buf[1024];

        while (true) {
            ssize_t n = recv(tcpClientSocket, buf, sizeof(buf) - 1, MSG_DONTWAIT);
            if (n > 0) {
                //                buf[n] = '\0'; // 确保字符串结束
                //                LOG_DEBUG("[TCP] 收到来自Client的数据: {}", buf);

                HandleTcpClientMessage(buf, n);
            } else if (n == 0) {
                // 对端关闭连接（收到FIN）
                LOG_DEBUG("[TCP] 对方关闭连接");
                close(tcpClientSocket);
                tcpClientSocket = -1;
                break;
            } else {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    // 没有更多数据可读，正常退出
                    break;
                } else if (errno == EINTR) {
                    // 被信号中断，重试
                    continue;
                } else {
                    LOG_DEBUG("[TCP] recv 出错 errno={}", errno);
                    close(tcpClientSocket);
                    tcpClientSocket = -1;
                    break;
                }
            }
        }
    }

    if (tcp_connected) {
        char buf[1024];
        while (true) {
            ssize_t n = recv(tcpServerSocket, buf, sizeof(buf) - 1, MSG_DONTWAIT);
            if (n > 0) {
                //                buf[n] = '\0'; // 确保字符串结束
                //                LOG_DEBUG("[TCP] 收到来自Server的数据: {}", buf);
                HandleTcpServerMessage(buf, n);

            } else if (n == 0) {
                // 对端关闭连接（收到FIN）
                LOG_DEBUG("[TCP] 对方关闭连接");
                close(tcpServerSocket);
                tcpServerSocket = -1;
                tcp_connecting = false;
                tcp_connected = false;
                break;
            } else {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    // 没有更多数据可读，正常退出
                    break;
                } else if (errno == EINTR) {
                    // 被信号中断，重试
                    continue;
                } else {
                    LOG_DEBUG("[TCP] recv 出错 errno={}", errno);
                    close(tcpServerSocket);
                    tcpServerSocket = -1;
                    tcp_connecting = false;
                    tcp_connected = false;
                    break;
                }
            }
        }
    }
}

void VSSetupMenu::PickRandomZombies(std::vector<SeedType> &theZombieSeeds) {
    old_VSSetupMenu_PickRandomZombies(this, theZombieSeeds);
}

void VSSetupMenu::PickRandomPlants(std::vector<SeedType> &thePlantSeeds, std::vector<SeedType> const &theZombieSeeds) {
    old_VSSetupMenu_PickRandomPlants(this, thePlantSeeds, theZombieSeeds);
    //    for (int i = 0; i < thePlantSeeds.size(); ++i) {
    //        SeedType type = thePlantSeeds[i];
    //        LOG_DEBUG("1{} {}", i, (int)type);
    //    }
    //
    //    for (int i = 0; i < theZombieSeeds.size(); ++i) {
    //        SeedType type = theZombieSeeds[i];
    //        LOG_DEBUG("2{} {}", i, (int)type);
    //    }

    if (tcpClientSocket >= 0) {
        TenShortDataEvent event;
        event.type = EventType::EVENT_VSSETUPMENU_RANDOM_PICK;
        for (int i = 0; i < thePlantSeeds.size(); ++i) {
            event.data[i] = thePlantSeeds[i];
        }

        for (int i = 0; i < theZombieSeeds.size(); ++i) {
            event.data[i + 5] = theZombieSeeds[i];
        }

        send(tcpClientSocket, &event, sizeof(TenShortDataEvent), 0);
    }
}


std::vector<char> clientVSSetupMenuRecvBuffer;


size_t VSSetupMenu::getClientEventSize(EventType type) {
    switch (type) {
        case EVENT_SEEDCHOOSER_SELECT_SEED:
            return sizeof (TwoCharDataEvent);
        case EVENT_VSSETUPMENU_MOVE_CONTROLLER:
            return sizeof (SimpleShortEvent);
        case EVENT_VSSETUPMENU_SET_CONTROLLER:
            return sizeof(SimpleShortEvent);
        default:
            return sizeof(BaseEvent);
    }
}

void VSSetupMenu::processClientEvent(void *buf, ssize_t bufSize) {
    BaseEvent *event = (BaseEvent *)buf;
    LOG_DEBUG("TYPE:{}", (int)event->type);
    switch (event->type) {
        case EVENT_SEEDCHOOSER_SELECT_SEED: {
            TwoCharDataEvent *event1 = (TwoCharDataEvent *)event;
            SeedType theSeedType = (SeedType)event1->data1;
            bool mIsZombieChooser = event1->data2;
            LOG_DEBUG("theSeedType={}", event1->data1);
            LOG_DEBUG("mIsZombieChooser={}", mIsZombieChooser);
            SeedChooserScreen *screen = (mIsZombieChooser ? mApp->mZombieChooserScreen : mApp->mSeedChooserScreen);
            screen->GetSeedPositionInChooser(theSeedType, screen->mCursorPositionX1, screen->mCursorPositionY1);
            screen->GetSeedPositionInChooser(theSeedType, screen->mCursorPositionX2, screen->mCursorPositionY2);
            (mIsZombieChooser ? screen->mSeedType2 : screen->mSeedType1) = theSeedType;
            screen->GameButtonDown(ButtonCode::BUTTONCODE_A, screen->mPlayerIndex);
        } break;
        case EVENT_VSSETUPMENU_MOVE_CONTROLLER: {
            SimpleShortEvent *event1 = (SimpleShortEvent *)event;
            Sexy::Widget *theController2Widget = FindWidget(8);
            theController2Widget->Move(event1->data, theController2Widget->mY);
            is2PControllerMoving = true;
        } break;
        case EVENT_VSSETUPMENU_SET_CONTROLLER: {
            SimpleShortEvent *event1 = (SimpleShortEvent *)event;
            if (mController2Position == event1->data) {
                GameButtonDown(ButtonCode::BUTTONCODE_A, 1, 0);
            }
            mController2Position = event1->data;
            is2PControllerMoving = false;
            if (mController1Position != -1 && mController2Position != -1 && mController1Position != mController2Position) {
                GameButtonDown(ButtonCode::BUTTONCODE_A, 0, 0);
                GameButtonDown(ButtonCode::BUTTONCODE_A, 1, 0);
            }
        } break;
        default:
            break;
    }
}

void VSSetupMenu::HandleTcpClientMessage(void *buf, ssize_t bufSize) {

    clientVSSetupMenuRecvBuffer.insert(clientVSSetupMenuRecvBuffer.end(), (char *)buf, (char *)buf + bufSize);
    size_t offset = 0;

    while (clientVSSetupMenuRecvBuffer.size() - offset >= sizeof(BaseEvent)) {
        BaseEvent *base = (BaseEvent *)&clientVSSetupMenuRecvBuffer[offset];
        size_t eventSize = getClientEventSize(base->type);
        if (clientVSSetupMenuRecvBuffer.size() - offset < eventSize)
            break; // 不完整

        processClientEvent((char *)&clientVSSetupMenuRecvBuffer[offset], eventSize);
        offset += eventSize;
    }

    if (offset != 0) {
        clientVSSetupMenuRecvBuffer.erase(clientVSSetupMenuRecvBuffer.begin(), clientVSSetupMenuRecvBuffer.begin() + offset);
    }
}



std::vector<char> serverVSSetupMenuRecvBuffer;


size_t VSSetupMenu::getServerEventSize(EventType type) {
    switch (type) {
        case EVENT_VSSETUPMENU_BUTTON_DEPRESS:
            return sizeof(SimpleEvent);
        case EVENT_VSSETUPMENU_ENTER_STATE:
            return sizeof(SimpleEvent);
        case EVENT_SEEDCHOOSER_SELECT_SEED:
            return sizeof(TwoCharDataEvent);
        case EVENT_VSSETUPMENU_RANDOM_PICK:
            return sizeof(TenShortDataEvent);
        case EVENT_VSSETUPMENU_MOVE_CONTROLLER:
            return sizeof(SimpleShortEvent);
        case EVENT_VSSETUPMENU_SET_CONTROLLER:
            return sizeof(SimpleShortEvent);

        default:
            return sizeof(BaseEvent);
    }
}

void VSSetupMenu::processServerEvent(void *buf, ssize_t bufSize) {
    BaseEvent *event = (BaseEvent *)buf;
    LOG_DEBUG("TYPE:{}", (int)event->type);
    switch (event->type) {
        case EVENT_VSSETUPMENU_BUTTON_DEPRESS: {
            SimpleEvent *event1 = (SimpleEvent *)event;
            int theId = event1->data;
            LOG_DEBUG("theId={}", theId);
            if (theId == 11 && mState == VS_SELECT_BATTLE) { // 随机战场
                break;
            }
            tcp_connected = false;
            ButtonDepress(theId);
            tcp_connected = true;
        } break;
        case EVENT_VSSETUPMENU_ENTER_STATE: {
            SimpleEvent *event1 = (SimpleEvent *)event;
            int theState = event1->data;
            LOG_DEBUG("theState={}", theState);
            //                GoToState(theState);
        } break;
        case EVENT_SEEDCHOOSER_SELECT_SEED: {
            TwoCharDataEvent *event1 = (TwoCharDataEvent *)event;
            SeedType theSeedType = (SeedType)event1->data1;
            bool mIsZombieChooser = event1->data2;
            LOG_DEBUG("theSeedType={}", event1->data1);
            LOG_DEBUG("mIsZombieChooser={}", mIsZombieChooser);
            SeedChooserScreen *screen = (mIsZombieChooser ? mApp->mZombieChooserScreen : mApp->mSeedChooserScreen);
            screen->GetSeedPositionInChooser(theSeedType, screen->mCursorPositionX1, screen->mCursorPositionY1);
            screen->GetSeedPositionInChooser(theSeedType, screen->mCursorPositionX2, screen->mCursorPositionY2);
            (mIsZombieChooser ? screen->mSeedType2 : screen->mSeedType1) = theSeedType;
            screen->GameButtonDown(ButtonCode::BUTTONCODE_A, screen->mPlayerIndex);
        } break;
        case EVENT_VSSETUPMENU_RANDOM_PICK: {
            TenShortDataEvent *event1 = (TenShortDataEvent *)event;
            tcp_connected = false;
            ButtonDepress(11);
            tcp_connected = true;

            mApp->mBoard->mSeedBank1->mSeedPackets[0].SetPacketType(SeedType::SEED_SUNFLOWER, SeedType::SEED_NONE);
            for (int i = 0; i < 5; ++i) {
                mApp->mBoard->mSeedBank1->mSeedPackets[i + 1].SetPacketType((SeedType)event1->data[i], SeedType::SEED_NONE);
            }

            mApp->mBoard->mSeedBank2->mSeedPackets[0].SetPacketType(SeedType::SEED_ZOMBIE_GRAVESTONE, SeedType::SEED_NONE);
            for (int i = 0; i < 5; ++i) {
                mApp->mBoard->mSeedBank2->mSeedPackets[i + 1].SetPacketType((SeedType)event1->data[i + 5], SeedType::SEED_NONE);
            }
        } break;
        case EVENT_VSSETUPMENU_MOVE_CONTROLLER: {
            SimpleShortEvent *event1 = (SimpleShortEvent *)event;
            Sexy::Widget *theController1Widget = FindWidget(7);
            theController1Widget->Move(event1->data, theController1Widget->mY);
            is1PControllerMoving = true;
        } break;
        case EVENT_VSSETUPMENU_SET_CONTROLLER: {
            SimpleShortEvent *event1 = (SimpleShortEvent *)event;
            if (mController1Position == event1->data) {
                GameButtonDown(ButtonCode::BUTTONCODE_A, 0, 0);
            }
            mController1Position = event1->data;
            is1PControllerMoving = false;
            if (mController1Position != -1 && mController2Position != -1 && mController1Position != mController2Position) {
                GameButtonDown(ButtonCode::BUTTONCODE_A, 0, 0);
                GameButtonDown(ButtonCode::BUTTONCODE_A, 1, 0);
            }
        } break;
        default:
            break;
    }
}





void VSSetupMenu::HandleTcpServerMessage(void *buf, ssize_t bufSize) {
    serverVSSetupMenuRecvBuffer.insert(serverVSSetupMenuRecvBuffer.end(), (char*)buf, (char*)buf + bufSize);
    size_t offset = 0;

    while (serverVSSetupMenuRecvBuffer.size() - offset >= sizeof(BaseEvent)) {
        BaseEvent* base = (BaseEvent*)&serverVSSetupMenuRecvBuffer[offset];
        size_t eventSize = getServerEventSize(base->type);
        if (serverVSSetupMenuRecvBuffer.size() - offset < eventSize)
            break; // 不完整

        processServerEvent((char *)&serverVSSetupMenuRecvBuffer[offset], eventSize);
        offset += eventSize;
    }

    if (offset != 0) {
        serverVSSetupMenuRecvBuffer.erase(serverVSSetupMenuRecvBuffer.begin(), serverVSSetupMenuRecvBuffer.begin() + offset);
    }
}

void VSSetupMenu::KeyDown(Sexy::KeyCode theKey) {
    // 修复在对战的阵营选取界面无法按返回键退出的BUG。
    if (theKey == Sexy::KeyCode::KEYCODE_ESCAPE) {
        switch (mState) {
            case VS_SETUP_SIDES:
            case VS_SELECT_BATTLE:
                mApp->DoBackToMain();
                return;
            case VS_CUSTOM_BATTLE: // 自定义战场
                mApp->DoNewOptions(false, 0);
                return;
        }
    }

    old_VSSetupMenu_KeyDown(this, theKey);
}

void VSSetupMenu::OnStateEnter(int theState) {
    if (theState == 0) {
        mInt76 = -1;
        // TODO:修复 WaitForSecondPlayerDialog重复构造
        auto *aWaitDialog = new WaitForSecondPlayerDialog(mApp);
        mApp->AddDialog(aWaitDialog); // 生成了两份mLawnNoButton

        int buttonId = aWaitDialog->WaitForResult(true);
        if (buttonId == 1000) {
            SetSecondPlayerIndex(mApp->mTwoPlayerState);
            GoToState(1);
        } else if (buttonId == 1001) {
            CloseVSSetup(1);
            mApp->KillBoard();
            mApp->ShowGameSelector();
        }
        return;
    } else if (tcpClientSocket >= 0) {
        SimpleEvent event = {[EventType::EVENT_VSSETUPMENU_ENTER_STATE], (unsigned char)theState};
        send(tcpClientSocket, &event, sizeof(SimpleEvent), 0);
    }

    old_VSSetupMenu_OnStateEnter(this, theState);
}

void VSSetupMenu::ButtonPress(int theId) {
    old_VSSetupMenu_ButtonPress(this, theId);
}

void VSSetupMenu::ButtonDepress(int theId) {

    if (tcp_connected) {
        return;
    }

    if (!isKeyboardTwoPlayerMode && mState == VS_SETUP_SIDES) {
        // 自动分配阵营
        //        GameButtonDown(ButtonCode::BUTTONCODE_LEFT, 0, 0);
        //        GameButtonDown(ButtonCode::BUTTONCODE_RIGHT, 1, 0);
        if (mController1Position != -1 && mController2Position != -1 && mController1Position != mController2Position) {
            GameButtonDown(ButtonCode::BUTTONCODE_A, 0, 0);
            GameButtonDown(ButtonCode::BUTTONCODE_A, 1, 0);
        } else {
            return;
            //            // 自动分配阵营
            //            mController1Position = 0;
            //            mController2Position = 1;
            //            GameButtonDown(ButtonCode::BUTTONCODE_A, 0, 0);
            //            GameButtonDown(ButtonCode::BUTTONCODE_A, 1, 0);
        }
    }

    old_VSSetupMenu_ButtonDepress(this, theId);

    if (tcpClientSocket >= 0) {
        SimpleEvent event = {{EventType::EVENT_VSSETUPMENU_BUTTON_DEPRESS}, (unsigned char)theId};
        send(tcpClientSocket, &event, sizeof(SimpleEvent), 0);
    }


    mApp->mBoard->PickBackground(); // 修改器修改场地后开局立即更换

    // 对战额外卡槽
    int aNumPackets = mApp->mBoard->GetNumSeedsInBank(false);

    SeedBank *aSeedBank1 = mApp->mBoard->mSeedBank1;
    SeedBank *aSeedBank2 = mApp->mBoard->mSeedBank2;

    aSeedBank1->mNumPackets = aNumPackets;
    aSeedBank2->mNumPackets = aNumPackets;

    switch (theId) {
        case 9: // 快速游戏
            if (aNumPackets == 7) {
                aSeedBank1->mSeedPackets[3].SetPacketType(SeedType::SEED_TORCHWOOD, SeedType::SEED_NONE);
                aSeedBank1->mSeedPackets[4].SetPacketType(SeedType::SEED_POTATOMINE, SeedType::SEED_NONE);
                aSeedBank1->mSeedPackets[5].SetPacketType(SeedType::SEED_SQUASH, SeedType::SEED_NONE);
                aSeedBank1->mSeedPackets[6].SetPacketType(SeedType::SEED_JALAPENO, SeedType::SEED_NONE);
                aSeedBank2->mSeedPackets[4].SetPacketType(SeedType::SEED_ZOMBIE_PAIL, SeedType::SEED_NONE);
                aSeedBank2->mSeedPackets[5].SetPacketType(SeedType::SEED_ZOMBIE_FOOTBALL, SeedType::SEED_NONE);
                aSeedBank2->mSeedPackets[6].SetPacketType(SeedType::SEED_ZOMBIE_FLAG, SeedType::SEED_NONE);
            }
            break;
        case 10: // 自定义战场
            if (mState == VS_CUSTOM_BATTLE) {
                gVSMorePacketsButton->SetDisable();
            }
            break;
        case 11: // 随机战场
            if (aNumPackets == 7) {
                mApp->mBoard->mSeedBank1->mNumPackets = 6;
                mApp->mBoard->mSeedBank2->mNumPackets = 6;
                // 开启“额外开槽”后随机选卡会导致界面卡死
                //                std::vector<SeedType> aZombieSeeds, aPlantSeeds, tmpZombieSeeds, tmpPlantSeeds;
                //
                //                PickRandomZombies(aZombieSeeds);
                //                do {
                //                    PickRandomZombies(tmpZombieSeeds);
                //                } while (tmpZombieSeeds[4] == aZombieSeeds[4]);
                //                aZombieSeeds.push_back(tmpZombieSeeds[4]);
                //
                //                PickRandomPlants(aPlantSeeds, aZombieSeeds);
                //                do {
                //                    PickRandomPlants(tmpPlantSeeds, aZombieSeeds);
                //                } while (tmpPlantSeeds[4] == aPlantSeeds[4]);
                //                aPlantSeeds.push_back(tmpPlantSeeds[4]);
                //
                //                aSeedBank2->mSeedPackets[0].SetPacketType(SeedType::SEED_ZOMBIE_GRAVESTONE, SeedType::SEED_NONE);
                //                aSeedBank1->mSeedPackets[0].SetPacketType(SeedType::SEED_SUNFLOWER, SeedType::SEED_NONE);
                //                for (int i = 0; i < aZombieSeeds.size(); ++i) {
                //                    aSeedBank2->mSeedPackets[i + 1].SetPacketType(aZombieSeeds[i], SeedType::SEED_NONE);
                //                }
                //                for (int i = 0; i < aPlantSeeds.size(); ++i) {
                //                    aSeedBank2->mSeedPackets[i + 1].SetPacketType(aPlantSeeds[i], SeedType::SEED_NONE);
                //                }
            }
            break;
        case 1145: // 额外卡槽
            gVSMorePacketsButton->ButtonDepress(1145);
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