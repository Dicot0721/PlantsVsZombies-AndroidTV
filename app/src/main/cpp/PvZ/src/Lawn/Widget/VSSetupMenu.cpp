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
    pvzstl::string str = StrFormat("");
    // 拓展卡槽
    gVSMorePacketsButton = new VSSetupWidget;
    ButtonWidget *aMorePacketsButton = MakeNewButton(1145, &mButtonListener, this, str, nullptr, aCheckbox, aCheckbox, aCheckbox);
    gVSMorePacketsButton->mMorePacketsButton = aMorePacketsButton;
    gVSMorePacketsButton->mCheckboxImage = aCheckbox;
    gVSMorePacketsButton->mCheckboxImagePress = aCheckboxPressed;
    aMorePacketsButton->Resize(MORE_PACKETS_BUTTON_X, MORE_PACKETS_BUTTON_Y, 175, 50);
    mApp->mBoard->AddWidget(aMorePacketsButton);
    gVSMorePacketsButton->mDrawString = true;

    mApp->mDanceMode = false;
    mApp->mBoard->mDanceMode = false;
}

void VSSetupMenu::_destructor() {
    old_VSSetupMenu_Destructor(this);

    if (gVSMorePacketsButton != nullptr)
        gVSMorePacketsButton->~VSSetupWidget();
}

void VSSetupMenu::Draw(Graphics *g) {
    old_VSSetupMenu_Draw(this, g);
}

void VSSetupMenu::Update() {
    // 记录当前游戏状态
    old_VSSetupMenu_Update(this);
    if (mState == 0)
        return;
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

void VSSetupMenu::PickRandomZombies(std::vector<SeedType, std::allocator<SeedType>> &theZombieSeeds) {
    old_VSSetupMenu_PickRandomZombies(this, theZombieSeeds);
}

void VSSetupMenu::PickRandomPlants(std::vector<SeedType, std::allocator<SeedType>> &thePlantSeeds, std::vector<SeedType, std::allocator<SeedType>> const &theZombieSeeds) {
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
        BaseEvent event = {EventType::EVENT_VSSETUPMENU_RANDOM_PICK};
        unsigned char data[10];
        for (int i = 0; i < thePlantSeeds.size(); ++i) {
            data[i] = thePlantSeeds[i];
        }

        for (int i = 0; i < theZombieSeeds.size(); ++i) {
            data[i + 5] = theZombieSeeds[i];
        }

        unsigned char buffer[sizeof(BaseEvent) + sizeof(data)];
        std::memcpy(buffer, &event, sizeof(BaseEvent));               // 拷贝事件头
        std::memcpy(buffer + sizeof(BaseEvent), data, sizeof(data));  // 紧跟 payload

        ssize_t n = send(tcpClientSocket, &buffer, sizeof(BaseEvent) + sizeof(data), 0);
        LOG_DEBUG("send {}", n);
    }
}

void VSSetupMenu::HandleTcpClientMessage(void *buf, ssize_t bufSize) {
    BaseEvent *event = (BaseEvent *)buf;
    switch (event->type) {
        case EVENT_SEEDCHOOSER_SELECT_SEED: {
            TwoCharDataEvent *event1 = (TwoCharDataEvent *)buf;
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
        default:
            break;
    }
}

void VSSetupMenu::HandleTcpServerMessage(void *buf, ssize_t bufSize) {

    int handledBufSize = 0;

    while(bufSize - handledBufSize > sizeof (BaseEvent)) {
        BaseEvent *event = (BaseEvent *)((unsigned char*)buf + handledBufSize);
        switch (event->type) {
            case EVENT_VSSETUPMENU_BUTTON_DEPRESS: {
                handledBufSize += sizeof (SimpleEvent);
                SimpleEvent *event1 = (SimpleEvent *)event;
                int theId = event1->data;
                LOG_DEBUG("theId={}", theId);
                if (theId == 11 && mState == 2) { // 随机战场
                    break;
                }
                tcp_connected = false;
                ButtonDepress(theId);
                tcp_connected = true;
            } break;
            case EVENT_VSSETUPMENU_ENTER_STATE: {
                handledBufSize += sizeof (SimpleEvent);
                SimpleEvent *event1 = (SimpleEvent *)event;
                int theState = event1->data;
                LOG_DEBUG("theState={}", theState);
//                GoToState(theState);
            } break;
            case EVENT_SEEDCHOOSER_SELECT_SEED: {
                handledBufSize += sizeof (TwoCharDataEvent);
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
                handledBufSize += sizeof (BaseEvent) + 10;
                unsigned char* data = (unsigned char*)event + sizeof(BaseEvent);
                tcp_connected = false;
                ButtonDepress(11);
                tcp_connected = true;

                mApp->mBoard->mSeedBank1->mSeedPackets[0].SetPacketType(SeedType::SEED_SUNFLOWER,SeedType::SEED_NONE);
                for (int i = 0; i < 5; ++i) {
                    mApp->mBoard->mSeedBank1->mSeedPackets[i+1].SetPacketType((SeedType)data[i],SeedType::SEED_NONE);
                }

                mApp->mBoard->mSeedBank2->mSeedPackets[0].SetPacketType(SeedType::SEED_ZOMBIE_GRAVESTONE,SeedType::SEED_NONE);
                for (int i = 0; i < 5; ++i) {
                    mApp->mBoard->mSeedBank2->mSeedPackets[i+1].SetPacketType((SeedType)data[i+5],SeedType::SEED_NONE);
                }
            } break;
            default:
                handledBufSize += sizeof (BaseEvent);
                break;
        }
    }
}

void VSSetupMenu::KeyDown(Sexy::KeyCode theKey) {
    // 修复在对战的阵营选取界面无法按返回键退出的BUG。
    if (theKey == Sexy::KeyCode::KEYCODE_ESCAPE) {
        switch (mState) {
            case 1:
            case 2:
                mApp->DoBackToMain();
                return;
            case 3: // 自定义战场
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

        int buttonId = ((int (*)(WaitForSecondPlayerDialog *, bool))aWaitDialog->vTable[127])(aWaitDialog, true);
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
        SimpleEvent event = {EventType::EVENT_VSSETUPMENU_ENTER_STATE, (unsigned char)theState};
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


    old_VSSetupMenu_ButtonDepress(this, theId);

    if (tcpClientSocket >= 0) {
        SimpleEvent event = {EventType::EVENT_VSSETUPMENU_BUTTON_DEPRESS, (unsigned char)theId};
        send(tcpClientSocket, &event, sizeof(SimpleEvent), 0);
    }

    if (!isKeyboardTwoPlayerMode && mState == 1) {
        // 自动分配阵营
        GameButtonDown(ButtonCode::BUTTONCODE_LEFT, 0, 0);
        GameButtonDown(ButtonCode::BUTTONCODE_RIGHT, 1, 0);
        GameButtonDown(ButtonCode::BUTTONCODE_A, 0, 0);
        GameButtonDown(ButtonCode::BUTTONCODE_A, 1, 0);
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
            if (mState == 3) {
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