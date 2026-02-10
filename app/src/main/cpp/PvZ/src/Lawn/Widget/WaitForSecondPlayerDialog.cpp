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

#include "PvZ/Lawn/Widget/WaitForSecondPlayerDialog.h"
#include "Homura/BitUtils.h"
#include "Homura/Logger.h"
#include "Homura/StringUtils.h"
#include "Homura/TypeUtils.h"
#include "PvZ/Android/Native/BridgeApp.h"
#include "PvZ/Android/Native/NativeApp.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/TodLib/Common/TodStringFile.h"

#include <arpa/inet.h>
#include <asm-generic/fcntl.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <linux/in.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/endian.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace Sexy;

void WaitForSecondPlayerDialog::SetMode(UIMode mode) {
    // 退出旧模式时做必要清理


    if (mUIMode == UIMode::MODE2_WIFI) {
        // 离开 WIFI 模式：停止广播、退出/离开、关闭扫描
        StopUdpBroadcastRoom();
        LeaveRoom();
        ExitRoom();
        CloseUdpScanSocket();
    }
    if (mUIMode == UIMode::MODE3_SERVER) {
        // 这里先只清状态；真正断开服务器连接你后续接入socket再处理
        mServerConnected = false;
    }

    mUIMode = mode;
    if (mUIMode == UIMode::MODE1_INIT) {
        mDialogHeader = TodStringTranslate("[MODE_SELECT_TITLE]");
    } else if (mUIMode == UIMode::MODE2_WIFI) {
        mDialogHeader = TodStringTranslate("[MODE_WIFI_TITLE]");
    } else {
        mDialogHeader = TodStringTranslate("[MODE_SERVER_TITLE]");
    }

    // 进入 WIFI 模式默认开始扫描
    if (mUIMode == UIMode::MODE2_WIFI) {
        mIsCreatingRoom = false;
        mIsJoiningRoom = false;
        InitUdpScanSocket();
    }

    RefreshButtons();
}

void WaitForSecondPlayerDialog::RefreshButtons() {
    switch (mUIMode) {
        case UIMode::MODE1_INIT:
            mLeftButton->SetLabel("[WIFI_VS]");
            mLeftButton->mDisabled = false;

            mRightButton->SetLabel("[SERVER_VS]");
            mRightButton->mDisabled = false;

            mLawnYesButton->SetLabel("[PLAY_OFFLINE]");
            mLawnYesButton->mDisabled = false;

            mLawnNoButton->SetLabel("[BACK]");
            mLawnNoButton->mDisabled = false;
            break;

        case UIMode::MODE2_WIFI: {

            //  如果正在创建房间（Host），左按钮改成“设置房间端口”
            if (mIsCreatingRoom) {
                // left: 设置端口
                mLeftButton->SetLabel("[SET_ROOM_PORT]");
                mLeftButton->mDisabled = false;

                // right: 退出房间
                mRightButton->SetLabel("[EXIT_ROOM_BUTTON]");
                mRightButton->mDisabled = false;

                // Yes: 开始游戏（有人加入才可点）
                mLawnYesButton->SetLabel("[START_GAME]");
                mLawnYesButton->mDisabled = (tcpClientSocket == -1);

                // No: 返回模式选择
                mLawnNoButton->SetLabel("[BACK_TO_MODE_SELECT]");
                mLawnNoButton->mDisabled = false;

                break;
            }

            // left: 加入/离开
            mLeftButton->SetLabel(mIsJoiningRoom ? "[LEAVE_ROOM_BUTTON]" : "[JOIN_ROOM_BUTTON]");
            if (mIsJoiningRoom) {
                mLeftButton->mDisabled = false;
            } else {
                // 扫描模式下：没房间就禁用“加入房间”
                bool inScanMode = (!mIsCreatingRoom && !mIsJoiningRoom);
                if (inScanMode) {
                    mLeftButton->mDisabled = (scanned_server_count == 0);
                } else {
                    // 其他情况（例如创建房间时 left 通常禁用）
                    mLeftButton->mDisabled = true;
                }
            }

            // right: 创建/退出
            mRightButton->SetLabel(mIsCreatingRoom ? "[EXIT_ROOM_BUTTON]" : "[CREATE_ROOM_BUTTON]");
            mRightButton->mDisabled = mIsJoiningRoom;

            // Yes：未创建房间 -> “加入指定IP房间”；创建房间 -> “开始游戏”
            mLawnYesButton->SetLabel("[JOIN_SPECIFIED_IP_ROOM]");
            mLawnYesButton->mDisabled = mIsJoiningRoom;

            mLawnNoButton->SetLabel("[BACK_TO_MODE_SELECT]");
            mLawnNoButton->mDisabled = false;

            break;
        }


        case UIMode::MODE3_SERVER: {
            // left: 加入 / 离开
            mLeftButton->SetLabel(mServerJoined ? "[LEAVE_ROOM_BUTTON]" : "[JOIN_ROOM_BUTTON]");

            // right: 创建 / 退出
            mRightButton->SetLabel(mServerHosting ? "[EXIT_ROOM_BUTTON]" : "[CREATE_ROOM_BUTTON]");

            // ✅ YesButton：host/joined 都显示“开始游戏”
            if (mServerHosting) {
                mLawnYesButton->SetLabel("[START_GAME]");
                mLawnYesButton->mDisabled = (!mServerConnected || mServerConnecting || !mServerHostHasGuest);
            } else if (mServerJoined) {
                mLawnYesButton->SetLabel("[START_GAME]");
                mLawnYesButton->mDisabled = true; // ✅ guest 永远禁用
            } else {
                // ✅ 已连接后显示“更换服务器”，功能仍然是弹输入框重新连接
                mLawnYesButton->SetLabel((mServerConnected || mServerConnecting) ? "[CHANGE_SERVER]" : "[CONNECT_SERVER]");
                mLawnYesButton->mDisabled = false;
            }


            mLawnNoButton->SetLabel("[BACK_TO_MODE_SELECT]");
            mLawnNoButton->mDisabled = false;

            // ✅ 加入按钮：连接后且“房间数量>0”才允许（空闲态）
            bool canJoinIdle = (mServerConnected && !mServerConnecting && !mServerHosting && !mServerJoined && (mServerRoomCount > 0));
            mLeftButton->mDisabled = !canJoinIdle && !mServerJoined; // 离开房间时应可点
            if (mServerJoined) {
                mLeftButton->mDisabled = (!mServerConnected || mServerConnecting);
            }

            // 创建按钮：空闲态可创建；hosting 时可退出
            bool canCreateIdle = (mServerConnected && !mServerConnecting && !mServerJoined);
            mRightButton->mDisabled = !canCreateIdle;

            break;
        }
    }
}

void WaitForSecondPlayerDialog::ShowTextInput(const char *titleKey, const char *hintKey) {
    Native::BridgeApp *bridgeApp = Native::BridgeApp::getSingleton();
    JNIEnv *env = bridgeApp->getJNIEnv();
    jobject view = bridgeApp->mNativeApp->getView();
    jclass viewCls = env->GetObjectClass(view);
    jmethodID mid = env->GetMethodID(viewCls, "showTextInputDialog2", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    jstring jTitle = env->NewStringUTF(TodStringTranslate(titleKey).c_str());
    jstring jHint = env->NewStringUTF(TodStringTranslate(hintKey).c_str());
    jstring jInitial = env->NewStringUTF("");
    env->CallVoidMethod(view, mid, 0, jTitle, jHint, jInitial);
    env->DeleteLocalRef(jTitle);
    env->DeleteLocalRef(jHint);
    env->DeleteLocalRef(jInitial);
    env->DeleteLocalRef(viewCls);
}

void WaitForSecondPlayerDialog::_constructor(LawnApp *theApp) {
    old_WaitForSecondPlayerDialog_WaitForSecondPlayerDialog(this, theApp);

    if (mApp->mGameMode != GAMEMODE_MP_VS) {
        GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 1);
        GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 1);
        return;
    }


    // 解决此Dialog显示时背景僵尸全部聚集、且草丛大块空缺的问题
    if (theApp->mBoard != nullptr) {
        theApp->mBoard->UpdateGame();
        theApp->mBoard->UpdateCoverLayer();
    }

    GameButton *playOfflineButton = MakeButton(1000, this, this, "[PLAY_OFFLINE]");
    mLawnYesButton = playOfflineButton;

    GameButton *backButton = MakeButton(1001, this, this, "[BACK]");
    mLawnNoButton = backButton;

    mLeftButton = MakeButton(1002, this, this, "[JOIN_ROOM_BUTTON]");
    mLeftButton->mDisabled = true;
    AddWidget(mLeftButton);

    mRightButton = MakeButton(1003, this, this, "[CREATE_ROOM_BUTTON]");
    AddWidget(mRightButton);

    this->LawnDialog::Resize(0, 0, 800, 600);

    playOfflineButton->mY -= 20;
    playOfflineButton->mWidth -= 30;
    playOfflineButton->mX += 15;

    backButton->mY -= 20;
    backButton->mWidth -= 30;
    backButton->mX += 15;

    mLeftButton->mX = playOfflineButton->mX;
    mLeftButton->mY = playOfflineButton->mY - 80;
    mLeftButton->mWidth = playOfflineButton->mWidth;
    mLeftButton->mHeight = playOfflineButton->mHeight;

    mRightButton->mX = backButton->mX;
    mRightButton->mY = backButton->mY - 80;
    mRightButton->mWidth = backButton->mWidth;
    mRightButton->mHeight = backButton->mHeight;

    InitUdpScanSocket();
    mIsCreatingRoom = false;
    mIsJoiningRoom = false;

    mSelectedServerIndex = 0;
    mUseManualTarget = false;
    mManualIp[0] = '\0';
    mManualPort = 0;
    mUIMode = UIMode::MODE1_INIT;
    mInputPurpose = InputPurpose::NONE;
    mServerConnected = false;
    mSelectedRoomIndex_Server = 0;


    // ===== MODE3 init =====
    mServerSock = -1;
    mServerConnecting = false;

    mServerHosting = false;
    mServerJoined = false;
    mServerHostHasGuest = false;
    mServerHostedRoomId = 0;
    mServerJoinedRoomId = 0;
    mServerLastQueryTick = 0;
    mServerLastRecvTick = 0;

    mServerIp[0] = '\0';
    mServerPort = 0;

    mServerRoomCount = 0;
    mSrvRecvLen = 0;

    std::memset(mServerRooms, 0, sizeof(mServerRooms));
    std::memset(mSrvRecvBuf, 0, sizeof(mSrvRecvBuf));
    mServerStatusText = TodStringTranslate("[STATUS_NOT_CONNECTED]");


    SetMode(UIMode::MODE1_INIT);
}

void WaitForSecondPlayerDialog::_destructor() {
    old_WaitForSecondPlayerDialog_Delete(this);
}

void WaitForSecondPlayerDialog::Draw(Graphics *g) {
    // 先画原始 Dialog（背景、按钮等）
    old_WaitForSecondPlayerDialog_Draw(this, g);
    if (mUIMode == UIMode::MODE1_INIT) {
        g->DrawString(TodStringTranslate("[LOCAL_VS_DESC]"), 160, 200);
        g->DrawString(TodStringTranslate("[WIFI_VS_DESC]"), 160, 240);
        g->DrawString(TodStringTranslate("[SERVER_VS_DESC]"), 160, 280);
    } else if (mUIMode == UIMode::MODE2_WIFI) {

        // =========================
        // MODE2_WIFI: Host（创建房间）
        // =========================
        if (mIsCreatingRoom) {
            pvzstl::string fmt = TodStringTranslate("[ROOM_CREATED_FMT]");
            pvzstl::string str = StrFormat(fmt.c_str(), mApp->mPlayerInfo->mName);
            g->DrawString(str, 230, 150);

            if (tcpPort != 0) {
                pvzstl::string fmt1 = TodStringTranslate("[PORT_IF_FMT]");
                pvzstl::string str1 = StrFormat(fmt1.c_str(), tcpPort, ifname.c_str());
                g->DrawString(str1, 260, 200);
            }

            pvzstl::string str2 = TodStringTranslate((udpBroadcastSocket >= 0) ? "[ROOM_SCAN_OPEN_OK]" : "[ROOM_SCAN_OPEN_FAIL]");
            g->DrawString(str2, 260, 250);

            // 是否有玩家加入
            pvzstl::string str3 = TodStringTranslate((tcpClientSocket == -1) ? "[WAIT_OTHER_JOIN]" : "[OTHER_JOINED]");
            g->DrawString(str3, 260, 300);

            // （可选）提示开始游戏按钮状态
            pvzstl::string str4 = TodStringTranslate((tcpClientSocket == -1) ? "[WAIT_OTHER_JOIN_TO_START]" : "[CAN_START_CLICK_START]");
            g->DrawString(str4, 220, 360);

            return;
        }

        // =========================
        // MODE2_WIFI: Guest（加入房间）
        // =========================
        if (mIsJoiningRoom) {
            if (mUseManualTarget) {
                // 手动连接
                pvzstl::string str = TodStringTranslate(tcp_connected ? "[JOINED_MANUAL]" : "[JOINING_MANUAL]");
                g->DrawString(str, 280, 150);

                pvzstl::string str1 = StrFormat("IP: %s:%d", mManualIp, mManualPort);
                g->DrawString(str1, 280, 200);

            } else {
                // 扫描列表连接：使用当前选中项
                int idx = mSelectedServerIndex;
                if (idx < 0)
                    idx = 0;
                if (idx >= scanned_server_count)
                    idx = scanned_server_count - 1;

                if (scanned_server_count <= 0) {
                    g->DrawString(TodStringTranslate("[NO_AVAILABLE_ROOMS]"), 280, 150);
                } else {
                    pvzstl::string fmtJoin = TodStringTranslate(tcp_connected ? "[JOINED_ROOM_FMT]" : "[JOINING_ROOM_FMT]");
                    pvzstl::string str = StrFormat(fmtJoin.c_str(), servers[idx].name);
                    g->DrawString(str, 280, 150);

                    g->DrawString(StrFormat("IP: %s:%d", servers[idx].ip, servers[idx].tcp_port), 280, 200);
                }
            }

            return;
        }

        // =========================
        // MODE2_WIFI: 扫描中/列表展示（未创建、未加入）
        // =========================
        if (scanned_server_count <= 0) {
            pvzstl::string str1 = TodStringTranslate((udpScanSocket >= 0) ? "[SCANNING_ROOMS]" : "[SCAN_FAILED]");
            g->DrawString(str1, 320, 200);

            g->DrawString(TodStringTranslate("[SCAN_TIP_MANUAL_JOIN]"), 55, 260);


            return;
        }

        // 列表：高亮 mSelectedServerIndex，支持点击选择（你 MouseDown 已实现）
        int idx = mSelectedServerIndex;
        if (idx < 0)
            idx = 0;
        if (idx >= scanned_server_count)
            idx = scanned_server_count - 1;
        mSelectedServerIndex = idx;

        int yPos = 180;


        Sexy::Color oldColor = g->mColor;

        // （可选）标题
        g->DrawString(TodStringTranslate("[AVAILABLE_ROOMS]"), 230, 140);
        g->SetFont(*Sexy_FONT_DWARVENTODCRAFT18_Addr);
        for (int i = 0; i < scanned_server_count; i++) {
            if (i == mSelectedServerIndex) {
                // 选中高亮（你原本用 leaderboard_selector）
                TodDrawImageScaledF(g, addonImages.leaderboard_selector, 140, yPos - 35, 0.7, 0.7);
                g->SetColor(Color(0, 205, 0, 255));
            } else {
                g->SetColor(oldColor);
            }

            pvzstl::string fmtLine = TodStringTranslate("[ROOM_LINE_FMT]");
            pvzstl::string line = StrFormat(fmtLine.c_str(), servers[i].name, servers[i].ip, servers[i].tcp_port);
            g->DrawString(line, 230, yPos);
            yPos += 50;
        }

        g->SetColor(oldColor);
    } else if (mUIMode == UIMode::MODE3_SERVER) {

        pvzstl::string head = TodStringTranslate(mServerConnected ? "[SERVER_CONNECTED]" : (mServerConnecting ? "[SERVER_CONNECTING]" : "[SERVER_NOT_CONNECTED]"));
        g->DrawString(head, 280, 160);

        pvzstl::string fmtSt = TodStringTranslate("[STATUS_FMT]");
        pvzstl::string st = StrFormat(fmtSt.c_str(), mServerStatusText.c_str());
        g->DrawString(st, 260, 200);

        if (!mServerConnected) {
            g->DrawString(TodStringTranslate("[SERVER_CONNECT_TIP1]"), 150, 240);
            g->DrawString(TodStringTranslate("[SERVER_CONNECT_TIP2]"), 170, 280);

        } else {
            // hosting/joined 提示
            if (mServerHosting) {
                pvzstl::string tail = TodStringTranslate(mServerHostHasGuest ? "[SERVER_HOST_HAS_GUEST]" : "[SERVER_HOST_WAIT_GUEST]");
                pvzstl::string fmtHost = TodStringTranslate("[SERVER_HOST_ROOM_FMT]");
                pvzstl::string s = StrFormat(fmtHost.c_str(), mServerHostedRoomId, tail.c_str());
                g->DrawString(s, 170, 240);
            } else if (mServerJoined) {
                pvzstl::string fmtJoined = TodStringTranslate("[SERVER_JOINED_ROOM_FMT]");
                pvzstl::string s = StrFormat(fmtJoined.c_str(), mServerJoinedRoomId);
                g->DrawString(s, 240, 240);
            } else {
                DrawServerRoomList(g);
            }
        }
    }
}


bool WaitForSecondPlayerDialog::ManualIpConnect() {
    const std::string input = std::move(gInputString);
    gHasInputContent = false;
    gHasInputContent.notify_one();
    LOG_DEBUG("raw input='{}'", input);

    const size_t colonPos = input.find(':');
    if (colonPos == std::string::npos) {
        LOG_ERROR("No colon in input");
        return false;
    }

    // 校验端口
    const std::string portStr = homura::Trim(std::string_view{input}.substr(colonPos + 1));
    const int port = std::atoi(portStr.c_str());
    if (port < 1 || port > 65535) {
        LOG_ERROR("invalid port: '{}'", portStr);
        return false;
    }

    // 校验 IP
    const std::string ip = homura::Trim(std::string_view{input}.substr(0, colonPos));
    in_addr addr{};
    if (inet_pton(AF_INET, ip.c_str(), &addr) != 1) {
        LOG_DEBUG("invalid ip '{}'", ip);
        return false;
    }

    // 保存目标
    mManualIp[ip.copy(mManualIp, INET_ADDRSTRLEN - 1)] = '\0';
    mManualPort = port;
    mUseManualTarget = true;
    LOG_DEBUG("target {}:{}", &mManualIp[0], mManualPort);

    // 切换到 joining 状态，重置连接状态（避免旧状态干扰）
    mIsJoiningRoom = true;
    CloseUdpScanSocket();

    mRightButton->mDisabled = true;
    mLeftButton->SetLabel("[LEAVE_ROOM_BUTTON]");

    mLawnYesButton->mDisabled = true;
    mLawnYesButton->SetLabel("[PLAY_ONLINE]");

    if (tcpServerSocket >= 0) {
        shutdown(tcpServerSocket, SHUT_RDWR);
        close(tcpServerSocket);
        tcpServerSocket = -1;
    }
    tcp_connecting = false;
    tcp_connected = false;

    // 关闭扫描 socket（避免 scan 模式逻辑干扰）
    CloseUdpScanSocket();

    // （可选）如果你希望这里同步更新按钮状态/文字，也可以放在这里

    return true;
}


void WaitForSecondPlayerDialog::Update() {
    // =========================================================
    // 1) 统一处理输入框回填（gInputString）
    //    关键点：
    //    - 只在“真的消费了输入”时才清 mInputPurpose
    //    - 若用途/模式不匹配：兜底清掉输入，避免每帧刷屏
    // =========================================================
    if (gHasInputContent) {
        assert(!gInputString.empty());

        // MODE2：WIFI 手动加入指定 IP
        if (mInputPurpose == InputPurpose::LAN_JOIN_MANUAL && mUIMode == UIMode::MODE2_WIFI) {
            mUseManualTarget = true;
            ManualIpConnect(); // 内部会消费 gInputString
            mInputPurpose = InputPurpose::NONE;
            RefreshButtons(); // 状态变化后立即刷新按钮
        }
        // MODE2：WIFI 房主设置房间端口
        else if (mInputPurpose == InputPurpose::HOST_SET_PORT) {
            // 取走输入并清空
            const std::string input = std::move(gInputString);
            gHasInputContent = false;
            gHasInputContent.notify_one();

            // 允许输入 0（随机端口），范围 0~65535
            const int port = std::atoi(input.c_str());
            if (port < 1 || port > 65535) {
                mApp->LawnMessageBox(Dialogs::DIALOG_MESSAGE, "[PORT_INVALID_TITLE]", "[PORT_INVALID_DESC]", "[DIALOG_BUTTON_OK]", "", 3);
                mInputPurpose = InputPurpose::NONE;
                return;
            }

            // 保存设置
            mApp->mPlayerInfo->mVSRoomPort = port;
            mApp->mPlayerInfo->SaveDetails();

            mInputPurpose = InputPurpose::NONE;
            // ✅ 关键：重建房间，让 tcpPort / 广播端口真正改变
            ExitRoom();   // 会关 tcpClient/tcpListen/udpBroadcast
            CreateRoom(); // 你已改为使用 mVSRoomPort bind

            // CreateRoom() 失败时：回到扫描模式避免卡死
            if (!mIsCreatingRoom) {
                InitUdpScanSocket();
                mIsJoiningRoom = false;
            } else {
                // 创建成功：不需要扫描
                CloseUdpScanSocket();
            }
            RefreshButtons();
        }
        // MODE3：连接服务器 IP:PORT
        else if (mInputPurpose == InputPurpose::SERVER_CONNECT_ADDR && mUIMode == UIMode::MODE3_SERVER) {
            ServerConnectFromInput(); // 内部会消费 gInputString
            mInputPurpose = InputPurpose::NONE;
            RefreshButtons(); // 状态变化后立即刷新按钮
        } else {
            // 兜底：收到输入但用途/模式不匹配
            // 防止 gInputString 永远不空导致每帧重复触发/刷日志
            LOG_WARN("[Input] drop input='{}' purpose={} mode={}", gInputString, int(mInputPurpose), int(mUIMode));
            gInputString.clear();
            gHasInputContent = false;
            gHasInputContent.notify_one();
            mInputPurpose = InputPurpose::NONE; // 这里不强制清 mInputPurpose 也行；清掉更安全
        }
    }

    // =========================================================
    // 2) MODE2：WIFI 才跑 UDP 扫描/广播节拍
    // =========================================================
    if (mUIMode == UIMode::MODE2_WIFI) {
        bool inScanMode = (!mIsCreatingRoom && !mIsJoiningRoom);
        if (inScanMode) {
            // 扫描模式下：没房间就禁用“加入房间”
            mLeftButton->mDisabled = (scanned_server_count == 0);

            // 选中索引修正
            mSelectedServerIndex = std::clamp(mSelectedServerIndex, 0, std::max(0, scanned_server_count - 1));
        }

        // 创建房间时：开始游戏按钮是否可点
        if (mIsCreatingRoom) {
            mLawnYesButton->mDisabled = (tcpClientSocket == -1);
        }

        // UDP 广播/扫描节拍
        lastBroadcastTime++;
        if (lastBroadcastTime >= 100) { // ~1秒
            if (mIsCreatingRoom) {
                UdpBroadcastRoom();
            } else if (!mIsJoiningRoom) {
                ScanUdpBroadcastRoom();
            }
        }

        // TCP accept / connect
        if (tcpListenSocket >= 0) {
            CheckTcpAccept();
        }
        if (mIsJoiningRoom && !tcp_connected) {
            TryTcpConnect();
        }
    }

    // =========================================================
    // 3) MODE3：服务器联机 IO（connect 完成检测 + 收包） + 自动 query
    // =========================================================
    if (mUIMode == UIMode::MODE3_SERVER) {
        // 网络 IO（你实现：包含 connect 完成检测、收包解析等）
        ServerUpdateIO();

        // 自动 Query：仅在“空闲态”每秒一次
        // 空闲态定义：已连接 && 未创建房间 && 未加入房间 && 未进入 relay
        mServerLastQueryTick++;
        if (mServerConnected && !mServerHosting && !mServerJoined) {
            if (mServerLastQueryTick >= 100) { // ~1秒
                mServerLastQueryTick = 0;
                ServerSendQuery();
            }
        } else {
            // 不在空闲态就不刷列表，tick 防溢出
            if (mServerLastQueryTick > 1000000)
                mServerLastQueryTick = 0;
        }
    }

    // =========================================================
    // 4) 每帧根据状态刷新文字/禁用（避免状态变化后没更新）
    // =========================================================
    RefreshButtons();
}


size_t WaitForSecondPlayerDialog::getClientEventSize(EventType type) {
    switch (type) {
        default:
            return sizeof(BaseEvent);
    }
}

void WaitForSecondPlayerDialog::processClientEvent(void *buf, ssize_t bufSize) {
    BaseEvent *event = (BaseEvent *)buf;
    LOG_DEBUG("TYPE:{}", (int)event->type);
    switch (event->type) {
        default:
            break;
    }
}

size_t WaitForSecondPlayerDialog::getServerEventSize(EventType type) {
    switch (type) {
        case EVENT_START_GAME:
        default:
            return sizeof(BaseEvent);
    }
}

void WaitForSecondPlayerDialog::processServerEvent(void *buf, ssize_t bufSize) {
    BaseEvent *event = (BaseEvent *)buf;
    LOG_DEBUG("TYPE:{}", (int)event->type);
    switch (event->type) {
        case EVENT_START_GAME:
            GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 1);
            GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 1);
            break;
        default:
            break;
    }
}


void WaitForSecondPlayerDialog::InitUdpScanSocket() {
    scanned_server_count = 0;
    udpScanSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpScanSocket < 0) {
        LOG_DEBUG("socket ERROR");
        return;
    }

    // 非阻塞
    int flags = fcntl(udpScanSocket, F_GETFL, 0);
    fcntl(udpScanSocket, F_SETFL, flags | O_NONBLOCK);
    // 允许地址重用
    int opt = 1;
    setsockopt(udpScanSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    // 绑定端口
    sockaddr_in recv_addr{
        .sin_family = AF_INET,
        .sin_port = htons(UDP_PORT),
        .sin_addr{.s_addr = INADDR_ANY},
    };

    if (bind(udpScanSocket, (sockaddr *)&recv_addr, sizeof(recv_addr)) < 0) {
        LOG_DEBUG("bind ERROR");
        close(udpScanSocket);
        udpScanSocket = -1;
        return;
    }

    LOG_DEBUG("[UDP Scan] Listening on port {}", UDP_PORT);
}

void WaitForSecondPlayerDialog::CloseUdpScanSocket() {
    if (udpScanSocket >= 0) {
        close(udpScanSocket);
        udpScanSocket = -1;
    }
    // scanned_server_count = 0;
}

bool WaitForSecondPlayerDialog::GetActiveBroadcast(sockaddr_in &out_bcast, std::string *out_ifname) {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
        return false;

    ifconf ifc;
    char buf[1024];
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if (ioctl(fd, SIOCGIFCONF, &ifc) < 0) {
        close(fd);
        return false;
    }

    ifreq *it = (ifreq *)buf;
    ifreq *end = (ifreq *)(buf + ifc.ifc_len);

    bool found_wifi = false;
    bool found_other = false;
    sockaddr_in wifi_bcast{};
    sockaddr_in other_bcast{};
    std::string wifi_if, other_if;

    for (; it < end; ++it) {
        ifreq ifr{};
        std::strncpy(ifr.ifr_name, it->ifr_name, IFNAMSIZ);

        // 跳过回环 / 未启用接口
        if (ioctl(fd, SIOCGIFFLAGS, &ifr) == 0) {
            if ((ifr.ifr_flags & IFF_LOOPBACK) || !(ifr.ifr_flags & IFF_UP))
                continue;
        }

        // 获取广播地址
        if (ioctl(fd, SIOCGIFBRDADDR, &ifr) == 0) {
            sockaddr_in *sin = (sockaddr_in *)&ifr.ifr_broadaddr;
            if (sin->sin_family != AF_INET)
                continue;

            // ✅ Wi-Fi / 热点接口优先（wlan*, ap*, en*）
            if (strncasecmp(ifr.ifr_name, "wlan", 4) == 0 || strncasecmp(ifr.ifr_name, "ap", 2) == 0 || strncasecmp(ifr.ifr_name, "en", 2) == 0) {
                wifi_bcast = *sin;
                wifi_if = ifr.ifr_name;
                found_wifi = true;
                // 不 break，继续扫描，看是否还有更匹配的
                continue;
            }

            // 记录其他接口（例如 ccmni、rmnet 等）
            if (!found_other) {
                other_bcast = *sin;
                other_if = ifr.ifr_name;
                found_other = true;
            }
        }
    }

    close(fd);

    if (found_wifi) {
        out_bcast = wifi_bcast;
        if (out_ifname)
            *out_ifname = wifi_if;
        return true;
    } else if (found_other) {
        out_bcast = other_bcast;
        if (out_ifname)
            *out_ifname = other_if;
        return true;
    }

    return false;
}


void WaitForSecondPlayerDialog::CreateRoom() {
    // 1) 创建TCP监听socket
    tcpListenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcpListenSocket < 0) {
        LOG_DEBUG("TCP socket failed errno={}", errno);
        mApp->LawnMessageBox(Dialogs::DIALOG_MESSAGE, "[CREATE_ROOM_FAIL_TITLE]", "[CREATE_ROOM_FAIL_SOCKET]", "[DIALOG_BUTTON_OK]", "", 3);
        return;
    }

    int flags = fcntl(tcpListenSocket, F_GETFL, 0);
    fcntl(tcpListenSocket, F_SETFL, flags | O_NONBLOCK);

    sockaddr_in addr{
        .sin_family = AF_INET,
        .sin_port = htons(mApp->mPlayerInfo->mVSRoomPort), // 允许0
        .sin_addr{.s_addr = INADDR_ANY},
    };
    int opt = 1;
    setsockopt(tcpListenSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(tcpListenSocket, (sockaddr *)&addr, sizeof(addr)) < 0) {
        LOG_DEBUG("TCP bind failed errno={}", errno);

        // ✅ 清理
        close(tcpListenSocket);
        tcpListenSocket = -1;

        InitUdpScanSocket();

        // ✅ 弹窗（端口占用最常见）
        pvzstl::string strFmt = TodStringTranslate("[CREATE_ROOM_FAIL_BIND]");

        int result = mApp->LawnMessageBox(Dialogs::DIALOG_MESSAGE, "[CREATE_ROOM_FAIL_TITLE]", StrFormat(strFmt.c_str(), mApp->mPlayerInfo->mVSRoomPort).c_str(), "[DIALOG_BUTTON_OK]", "", 3);
        if (result == 1000) {
            mInputPurpose = InputPurpose::HOST_SET_PORT;
            ShowTextInput("[INPUT_TITLE_SET_PORT]", "[HINT_PORT]");
        }
        return;
    }

    if (listen(tcpListenSocket, 1) < 0) {
        LOG_DEBUG("TCP listen failed errno={}", errno);

        // ✅ 清理
        close(tcpListenSocket);
        tcpListenSocket = -1;

        mApp->LawnMessageBox(Dialogs::DIALOG_MESSAGE, "[CREATE_ROOM_FAIL_TITLE]", "[CREATE_ROOM_FAIL_LISTEN]", "[DIALOG_BUTTON_OK]", "", 3);
        return;
    }

    // 获取实际分配的端口号（当 mVSRoomPort=0 时这里会得到随机端口）
    socklen_t addr_len = sizeof(addr);
    getsockname(tcpListenSocket, (sockaddr *)&addr, &addr_len);
    tcpPort = ntohs(addr.sin_port);

    // 2) 创建UDP广播socket
    udpBroadcastSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpBroadcastSocket < 0) {
        mIsCreatingRoom = true;
        LOG_DEBUG("UDP socket failed errno={}", errno);
        return;
    }

    int on = 1;
    setsockopt(udpBroadcastSocket, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
    setsockopt(udpBroadcastSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    // 选择广播地址（失败也给提示，但仍可兜底 255.255.255.255）
    sockaddr_in bcast{};
    if (GetActiveBroadcast(bcast, &ifname)) {
        bcast.sin_port = htons(UDP_PORT);
        broadcast_addr = bcast;

        char ipstr[INET_ADDRSTRLEN]{};
        inet_ntop(AF_INET, &bcast.sin_addr, ipstr, sizeof(ipstr));
        LOG_DEBUG("[UDP] use if={} bcast={}", ifname.c_str(), ipstr);
    } else {
        memset(&broadcast_addr, 0, sizeof(broadcast_addr));
        broadcast_addr.sin_family = AF_INET;
        broadcast_addr.sin_port = htons(UDP_PORT);
        inet_pton(AF_INET, "255.255.255.255", &broadcast_addr.sin_addr);
        LOG_WARN("[UDP] fallback broadcast 255.255.255.255:{}", UDP_PORT);

        // ✅ 可选：提示玩家“没找到网卡广播地址，已用兜底”
        // （如果你觉得太吵可以注释掉）
        // ShowCreateRoomFail("[CREATE_ROOM_WARN_TITLE]", "[CREATE_ROOM_WARN_BCAST_FALLBACK]");
    }

    flags = fcntl(udpBroadcastSocket, F_GETFL, 0);
    fcntl(udpBroadcastSocket, F_SETFL, flags | O_NONBLOCK);

    LOG_DEBUG("[Host] Room created. TCP port={}, UDP port={}", tcpPort, UDP_PORT);

    UdpBroadcastRoom();
    mIsCreatingRoom = true;
}

void WaitForSecondPlayerDialog::ExitRoom() {
    mIsCreatingRoom = false;

    if (tcpClientSocket >= 0) {
        shutdown(tcpClientSocket, SHUT_RDWR); // 关闭读写
        close(tcpClientSocket);
        tcpClientSocket = -1;
    }

    if (tcpListenSocket >= 0) {
        shutdown(tcpListenSocket, SHUT_RDWR);
        close(tcpListenSocket);
        tcpListenSocket = -1;
    }

    if (udpBroadcastSocket >= 0) {
        close(udpBroadcastSocket);
        udpBroadcastSocket = -1;
    }

    // 其他清理操作
}


void WaitForSecondPlayerDialog::JoinRoom() {
    mIsJoiningRoom = true;
}

void WaitForSecondPlayerDialog::LeaveRoom() {
    mIsJoiningRoom = false;
    if (tcpServerSocket >= 0) {
        shutdown(tcpServerSocket, SHUT_RDWR); // 关闭读写
        close(tcpServerSocket);
        tcpServerSocket = -1;
        tcp_connecting = false;
        tcp_connected = false;
    }

    mUseManualTarget = false;
    mManualIp[0] = '\0';
    mManualPort = 0;
}

void WaitForSecondPlayerDialog::UdpBroadcastRoom() {
    lastBroadcastTime = 0;
    if (udpBroadcastSocket < 0)
        return;
    LawnApp *lawnApp = *gLawnApp_Addr;
    if (!lawnApp || !lawnApp->mPlayerInfo || !lawnApp->mPlayerInfo->mName)
        return;

    const char *message = lawnApp->mPlayerInfo->mName;

    if (tcpPort != 0) {
        size_t msg_len = strlen(message) + 1; // 含 '\0'
        size_t total_len = msg_len + sizeof(tcpPort);

        char send_buf[256];
        if (total_len > sizeof(send_buf))
            return; // 防止溢出

        memcpy(send_buf, message, msg_len);
        memcpy(send_buf + msg_len, &tcpPort, sizeof(tcpPort));

        ssize_t sent = sendto(udpBroadcastSocket, send_buf, total_len, 0, (sockaddr *)&broadcast_addr, sizeof(broadcast_addr));

        if (sent > 0)
            LOG_DEBUG("[Send] msg: '{}', num: {}\n", message, tcpPort);
        else if (!(errno == EAGAIN || errno == EWOULDBLOCK))
            LOG_DEBUG("sendto ERROR {}", errno);
    }
}

bool WaitForSecondPlayerDialog::CheckTcpAccept() {
    if (tcpListenSocket < 0)
        return false;
    if (tcpClientSocket >= 0) {
        return true;
    }
    sockaddr_in clientAddr{};
    socklen_t addrlen = sizeof(clientAddr);
    tcpClientSocket = accept(tcpListenSocket, (sockaddr *)&clientAddr, &addrlen);
    if (tcpClientSocket < 0) {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
            return false; // 没有连接
        LOG_DEBUG("accept ERROR");
        return false;
    }
    int one = 1;
    setsockopt(tcpClientSocket, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one)); // 禁用 Nagle 算法
    int on = 1;
    setsockopt(tcpClientSocket, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(on));
    int idle = 30;
    setsockopt(tcpClientSocket, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(idle));
    int intvl = 10;
    setsockopt(tcpClientSocket, IPPROTO_TCP, TCP_KEEPINTVL, &intvl, sizeof(intvl));
    int cnt = 3;
    setsockopt(tcpClientSocket, IPPROTO_TCP, TCP_KEEPCNT, &cnt, sizeof(cnt));

    int flags = fcntl(tcpClientSocket, F_GETFL, 0);
    fcntl(tcpClientSocket, F_SETFL, flags | O_NONBLOCK);

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, ip, sizeof(ip));
    LOG_DEBUG("[TCP] Client connected: {}", ip);
    return true;
}

void WaitForSecondPlayerDialog::ScanUdpBroadcastRoom() {
    lastBroadcastTime = 0;
    sockaddr_in recv_addr{};
    socklen_t addr_len = sizeof(recv_addr);
    char buffer[NAME_LENGTH + sizeof(int)] = {0};

    // 循环读取所有可用包
    while (true) {
        ssize_t n = recvfrom(udpScanSocket, buffer, sizeof(buffer), 0, (sockaddr *)&recv_addr, &addr_len);
        if (n > 0) {
            // 解析消息
            char *msg = buffer;
            size_t msg_len = strnlen(msg, NAME_LENGTH - 1) + 1;

            if (n < (ssize_t)(msg_len + sizeof(int)))
                continue; // 包太短，跳过

            int tcp_port = 0;
            memcpy(&tcp_port, buffer + msg_len, sizeof(tcp_port));

            char server_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &recv_addr.sin_addr, server_ip, sizeof(server_ip));

            time_t now = time(nullptr);
            bool found = false;

            // 更新已存在的server
            for (int i = 0; i < scanned_server_count; i++) {
                if (strcmp(servers[i].ip, server_ip) == 0) {
                    servers[i].tcp_port = tcp_port;
                    strncpy(servers[i].name, msg, NAME_LENGTH);
                    servers[i].last_seen = now;
                    found = true;
                    LOG_DEBUG("[Scan] Update server: {}:{} ({})\n", server_ip, tcp_port, msg);
                    break;
                }
            }

            // 新server
            if (!found && scanned_server_count < MAX_SERVERS) {
                strncpy(servers[scanned_server_count].ip, server_ip, INET_ADDRSTRLEN);
                strncpy(servers[scanned_server_count].name, msg, NAME_LENGTH);
                servers[scanned_server_count].tcp_port = tcp_port;
                servers[scanned_server_count].last_seen = now;
                scanned_server_count++;
                LOG_DEBUG("[Scan] New server: {}:{} ({})\n", server_ip, tcp_port, msg);
            }

        } else if (n < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN)
                break; // 没有更多数据可读
            else
                LOG_DEBUG("recvfrom ERROR");
            break; // 数据错误
        } else {
            break; // 没有数据
        }
    }

    // 检查超时
    time_t current_time = time(nullptr);
    for (int i = 0; i < scanned_server_count;) {
        if (difftime(current_time, servers[i].last_seen) > UDP_TIMEOUT) {

            // 如果选中的是最后一个，而我们要把最后一个删掉
            int last = scanned_server_count - 1;

            // 1) 如果选中项就是被删除的 i：
            //    删除后，当前位置会被 last 覆盖，所以让选中保持在 i（继续指向“被搬过来的那一项”）
            if (mSelectedServerIndex == i) {
                // 选中保持 i，不变
            }
            // 2) 如果选中项是 last，而 last 要被搬到 i：
            //    选中项应该跟着搬到 i（否则你会“莫名丢选中”）
            else if (mSelectedServerIndex == last) {
                mSelectedServerIndex = i;
            }
            // 3) 其他情况不用改

            servers[i] = servers[last];
            scanned_server_count--;

            // 删除后防越界
            if (scanned_server_count <= 0) {
                mSelectedServerIndex = 0;
            } else if (mSelectedServerIndex >= scanned_server_count) {
                mSelectedServerIndex = scanned_server_count - 1;
            }

            continue;
        }
        i++;
    }
}

void WaitForSecondPlayerDialog::TryTcpConnect() {
    if (tcp_connected)
        return;

    // 既不是手动目标，也没有扫描到房间，就没法连
    if (!mUseManualTarget && scanned_server_count == 0)
        return;

    // 统一得到目标 ip/port（用于 connect + 日志）
    char target_ip[INET_ADDRSTRLEN] = {0};
    int target_port = 0;

    if (mUseManualTarget) {
        strncpy(target_ip, mManualIp, INET_ADDRSTRLEN - 1);
        target_port = mManualPort;
    } else {
        int idx = mSelectedServerIndex;
        if (idx < 0)
            idx = 0;
        if (idx >= scanned_server_count)
            idx = scanned_server_count - 1;

        strncpy(target_ip, servers[idx].ip, INET_ADDRSTRLEN - 1);
        target_port = servers[idx].tcp_port;
    }

    // 组装 sockaddr
    sockaddr_in server_addr{
        .sin_family = AF_INET,
        .sin_port = htons(target_port),
    };
    inet_pton(AF_INET, target_ip, &server_addr.sin_addr);

    if (!tcp_connecting) {
        tcpServerSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (tcpServerSocket < 0) {
            LOG_DEBUG("[Client] socket ERROR errno={}", errno);
            return;
        }

        // 非阻塞
        int flags = fcntl(tcpServerSocket, F_GETFL, 0);
        fcntl(tcpServerSocket, F_SETFL, flags | O_NONBLOCK);

        // 发起非阻塞 connect
        int ret = connect(tcpServerSocket, (sockaddr *)&server_addr, sizeof(server_addr));
        if (ret < 0) {
            if (errno == EINPROGRESS) {
                tcp_connecting = true;
                LOG_DEBUG("[Client] Connecting to {}:{} ...", target_ip, target_port);
            } else {
                LOG_DEBUG("[Client] connect ERROR errno={}", errno);
                close(tcpServerSocket);
                tcpServerSocket = -1;
                tcp_connecting = false;
                tcp_connected = false;
            }
        } else {
            // 立即连接成功
            int one = 1;
            setsockopt(tcpServerSocket, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

            int on = 1;
            setsockopt(tcpServerSocket, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(on));
            int idle = 30;
            setsockopt(tcpServerSocket, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(idle));
            int intvl = 10;
            setsockopt(tcpServerSocket, IPPROTO_TCP, TCP_KEEPINTVL, &intvl, sizeof(intvl));
            int cnt = 3;
            setsockopt(tcpServerSocket, IPPROTO_TCP, TCP_KEEPCNT, &cnt, sizeof(cnt));

            tcp_connected = true;
            tcp_connecting = false;
            LOG_DEBUG("[Client] Connected immediately to {}:{}", target_ip, target_port);
        }

    } else {
        // 检查连接是否完成
        fd_set writefds;
        FD_ZERO(&writefds);
        FD_SET(tcpServerSocket, &writefds);

        timeval tv{0, 0};
        int ret = select(tcpServerSocket + 1, nullptr, &writefds, nullptr, &tv);
        if (ret > 0 && FD_ISSET(tcpServerSocket, &writefds)) {
            int err = 0;
            socklen_t len = sizeof(err);
            if (getsockopt(tcpServerSocket, SOL_SOCKET, SO_ERROR, &err, &len) < 0) {
                LOG_DEBUG("[Client] getsockopt ERROR errno={}", errno);
                close(tcpServerSocket);
                tcpServerSocket = -1;
                tcp_connecting = false;
                tcp_connected = false;
                return;
            }

            if (err == 0) {
                int one = 1;
                setsockopt(tcpServerSocket, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

                int on = 1;
                setsockopt(tcpServerSocket, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(on));
                int idle = 30;
                setsockopt(tcpServerSocket, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(idle));
                int intvl = 10;
                setsockopt(tcpServerSocket, IPPROTO_TCP, TCP_KEEPINTVL, &intvl, sizeof(intvl));
                int cnt = 3;
                setsockopt(tcpServerSocket, IPPROTO_TCP, TCP_KEEPCNT, &cnt, sizeof(cnt));

                tcp_connected = true;
                tcp_connecting = false;
                LOG_DEBUG("[Client] Connected to {}:{}", target_ip, target_port);
            } else {
                LOG_DEBUG("[Client] Connect failed to {}:{} err={}", target_ip, target_port, err);
                close(tcpServerSocket);
                tcpServerSocket = -1;
                tcp_connecting = false;
                tcp_connected = false;
            }
        }
        // select==0 表示还在连接中，下次 Update 再检查
    }
}


void WaitForSecondPlayerDialog::StopUdpBroadcastRoom() {
    if (udpBroadcastSocket >= 0) {
        close(udpBroadcastSocket);
        udpBroadcastSocket = -1;
    }
    LOG_DEBUG("[UDP] Broadcast closed\n");
}


void WaitForSecondPlayerDialog_ButtonDepress(Sexy::ButtonListener *listener, int id) {
    auto *dialog = homura::DynamicCast<Sexy::ButtonListener, WaitForSecondPlayerDialog>(listener);
    const UIMode aUIMode = dialog->mUIMode;
    switch (id) {
        case 1000: // YesButton
            switch (aUIMode) {
                case UIMode::MODE1_INIT:
                    // 本地游戏：按两下A
                    dialog->GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 1);
                    dialog->GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 1);
                    break;
                case UIMode::MODE2_WIFI:
                    if (dialog->mIsCreatingRoom) {
                        // 开始游戏（房主）：根据是否有玩家加入决定是否可点（RefreshButtons里已禁用）
                        dialog->GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 1);
                        dialog->GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 1);
                        if (tcpClientSocket >= 0) {
                            BaseEvent event = {EventType::EVENT_START_GAME};
                            sendWithSize(tcpClientSocket, &event, sizeof(BaseEvent), 0);
                        }
                    } else {
                        // 加入指定IP房间：弹输入框
                        dialog->mInputPurpose = InputPurpose::LAN_JOIN_MANUAL;
                        dialog->ShowTextInput("[INPUT_TITLE_JOIN_IP]", "[HINT_IP_PORT]");
                        return;
                    }
                    break;
                case UIMode::MODE3_SERVER:
                    if (dialog->mServerHosting) {
                        // ✅ 开始游戏（只有 host 且有人加入时按钮才会启用）
                        dialog->ServerSendStart();
                        return;
                    }

                    // 未创建房间：连接服务器
                    dialog->mInputPurpose = InputPurpose::SERVER_CONNECT_ADDR;
                    dialog->ShowTextInput("[INPUT_TITLE_CONNECT_SERVER]", "[HINT_IP_PORT]");
                    return;
            }
            break;
        case 1001: // NoButton
            if (aUIMode == UIMode::MODE1_INIT) {
                // 返回：沿用你原来的清理
                dialog->StopUdpBroadcastRoom();
                dialog->LeaveRoom();
                dialog->ExitRoom();
                dialog->CloseUdpScanSocket();
            } else {
                // 模式2/3：返回到模式1
                dialog->SetMode(UIMode::MODE1_INIT);
                dialog->mServerHosting = false;
                dialog->mServerJoined = false;
                if (dialog->mServerSock) {
                    close(dialog->mServerSock);
                    dialog->mServerSock = -1;
                }
                return;
            }
            break;
        case 1002: // leftButton
            switch (aUIMode) {
                case UIMode::MODE1_INIT:
                    dialog->SetMode(UIMode::MODE2_WIFI);
                    break;
                case UIMode::MODE2_WIFI:
                    // ✅ Host（创建房间中）：leftButton 改为“设置房间端口”
                    if (dialog->mIsCreatingRoom) {
                        dialog->mInputPurpose = InputPurpose::HOST_SET_PORT;
                        dialog->ShowTextInput("[INPUT_TITLE_SET_PORT]", "[HINT_PORT]");
                        return;
                    }

                    // ===== 下面保持你原来的 Join/Leave 逻辑 =====
                    // 加入房间 / 离开房间（沿用你原逻辑）
                    if (dialog->mIsJoiningRoom) {
                        dialog->LeaveRoom();
                        dialog->InitUdpScanSocket();
                    } else {
                        dialog->JoinRoom();
                        dialog->CloseUdpScanSocket();
                    }
                    dialog->RefreshButtons();
                    break;
                case UIMode::MODE3_SERVER:
                    if (!dialog->mServerConnected) {
                        return;
                    }
                    if (dialog->mServerJoined) {
                        dialog->ServerSendLeaveRoom(); // LEAVE_ROOM(0x07)
                    } else if (!dialog->mServerHosting) {
                        // 空闲态才能 join
                        dialog->ServerSendJoinSelected(); // JOIN(0x03)
                    }
                    dialog->RefreshButtons();
                    return;
            }
            break;
        case 1003: // rightButton
            switch (aUIMode) {
                case UIMode::MODE1_INIT:
                    dialog->SetMode(UIMode::MODE3_SERVER);
                    break;
                case UIMode::MODE2_WIFI:
                    // 创建房间 / 退出房间（沿用你原逻辑）
                    if (dialog->mIsCreatingRoom) {
                        dialog->ExitRoom();
                        dialog->InitUdpScanSocket();
                    } else {
                        dialog->CreateRoom();
                        dialog->CloseUdpScanSocket();
                    }
                    dialog->RefreshButtons();
                    break;
                case UIMode::MODE3_SERVER:
                    if (!dialog->mServerConnected) {
                        return;
                    }
                    if (dialog->mServerHosting) {
                        dialog->ServerSendExitRoom(); // EXIT_ROOM(0x06)
                    } else if (!dialog->mServerJoined) {
                        dialog->ServerSendCreate(); // CREATE(0x01)
                    }
                    dialog->RefreshButtons();
                    return;
            }
            break;
        default:
            break;
    }
    old_WaitForSecondPlayerDialog_ButtonDepress(listener, id);
}

bool WaitForSecondPlayerDialog::ServerTryReadOneFrame(uint8_t &outType, uint8_t *outPayload, uint16_t &outLen) {
    if (mSrvRecvLen < 3)
        return false;

    uint8_t type = mSrvRecvBuf[0];
    uint16_t len = (uint16_t(mSrvRecvBuf[1]) << 8) | uint16_t(mSrvRecvBuf[2]);
    if (mSrvRecvLen < 3 + (int)len)
        return false;

    outType = type;
    outLen = len;
    if (len > 0 && outPayload) {
        std::memcpy(outPayload, mSrvRecvBuf + 3, len);
    }

    // consume
    int remain = mSrvRecvLen - (3 + (int)len);
    if (remain > 0) {
        std::memmove(mSrvRecvBuf, mSrvRecvBuf + 3 + len, remain);
    }
    mSrvRecvLen = remain;
    return true;
}

void WaitForSecondPlayerDialog::ServerUpdateIO() {
    if (mServerSock < 0)
        return;

    // 1) connect 完成检测
    if (mServerConnecting && !mServerConnected) {
        fd_set wfds;
        FD_ZERO(&wfds);
        FD_SET(mServerSock, &wfds);
        timeval tv{0, 0};
        int r = select(mServerSock + 1, nullptr, &wfds, nullptr, &tv);
        if (r > 0 && FD_ISSET(mServerSock, &wfds)) {
            int err = 0;
            socklen_t elen = sizeof(err);
            getsockopt(mServerSock, SOL_SOCKET, SO_ERROR, &err, &elen);
            if (err == 0) {
                mServerConnecting = false;
                mServerConnected = true;
                mServerStatusText = TodStringTranslate("[STATUS_CONNECTED]");
                ServerSendQuery();
            } else {
                ServerDisconnect("connect error");
                mServerStatusText = TodStringTranslate("[STATUS_CONNECT_FAILED]");
            }
        }
    }

    // 2) 读数据（非阻塞）
    while (true) {
        if (mSrvRecvLen >= (int)sizeof(mSrvRecvBuf)) {
            // buffer full -> drop
            ServerDisconnect("recv overflow");
            return;
        }

        ssize_t n = recv(mServerSock, mSrvRecvBuf + mSrvRecvLen, sizeof(mSrvRecvBuf) - mSrvRecvLen, 0);
        if (n > 0) {
            mSrvRecvLen += (int)n;
        } else if (n == 0) {
            ServerDisconnect("server closed");
            return;
        } else {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                break;
            ServerDisconnect("recv error");
            return;
        }
    }

    // 3) 解析帧并处理
    uint8_t type;
    uint16_t len;
    uint8_t payload[2048];

    while (ServerTryReadOneFrame(type, payload, len)) {
        // 服务器对战：RespType
        switch (type) {
            case 0x81: { // ROOM_CREATED
                if (len >= 4) {
                    int id = homura::ReadBEI32(payload);
                    mServerHosting = true;
                    mServerJoined = false;
                    mServerHostHasGuest = false;
                    mServerHostedRoomId = id;
                    mServerJoinedRoomId = 0;
                    mServerStatusText = TodStringTranslate("[STATUS_ROOM_CREATED]");
                }
                break;
            }
            case 0x82: { // ROOM_LIST
                // payload: [count:1] + count*([roomId:4][flags:1][nameLen:1][nameBytes])
                mServerRoomCount = 0;
                if (len < 1)
                    break;
                int count = payload[0] & 0xFF;
                int off = 1;

                for (int i = 0; i < count && mServerRoomCount < 255; i++) {
                    if (off + 6 > (int)len)
                        break;
                    int id = homura::ReadBEI32(payload + off);
                    off += 4;
                    int flags = payload[off++] & 0xFF;
                    int nameLen = payload[off++] & 0xFF;
                    if (off + nameLen > (int)len)
                        break;

                    ServerRoomItem &it = mServerRooms[mServerRoomCount++];
                    it.roomId = id;
                    it.full = (flags & 1) != 0;
                    it.gaming = (flags & 2) != 0;
                    std::memset(it.name, 0, sizeof(it.name));
                    int cp = nameLen;
                    if (cp > (int)sizeof(it.name) - 1)
                        cp = (int)sizeof(it.name) - 1;
                    std::memcpy(it.name, payload + off, cp);
                    off += nameLen;
                }

                if (mSelectedRoomIndex_Server < 0)
                    mSelectedRoomIndex_Server = 0;
                if (mSelectedRoomIndex_Server >= mServerRoomCount)
                    mSelectedRoomIndex_Server = mServerRoomCount - 1;
                if (mSelectedRoomIndex_Server < 0)
                    mSelectedRoomIndex_Server = 0;
                break;
            }
            case 0x83: { // JOIN_RESULT
                bool ok = (len >= 1 && payload[0] == 1);
                int rid = (len >= 5) ? homura::ReadBEI32(payload + 1) : 0;
                if (ok) {
                    mServerJoined = true;
                    mServerHosting = false;
                    mServerHostedRoomId = 0;
                    mServerJoinedRoomId = rid;
                    mServerHostHasGuest = false;
                    mServerStatusText = TodStringTranslate("[STATUS_JOINED_ROOM]");

                } else {
                    mServerStatusText = TodStringTranslate("[STATUS_JOIN_FAILED]");
                }
                break;
            }
            case 0x84: { // GUEST_JOINED
                if (len >= 4) {
                    int rid = homura::ReadBEI32(payload);
                    if (mServerHosting && rid == mServerHostedRoomId) {
                        mServerHostHasGuest = true;
                        mServerStatusText = TodStringTranslate("[STATUS_GUEST_JOINED]");
                    }
                }
                break;
            }
            case 0x87: { // GUEST_LEFT
                if (len >= 4) {
                    int rid = homura::ReadBEI32(payload);
                    if (mServerHosting && rid == mServerHostedRoomId) {
                        mServerHostHasGuest = false;
                        mServerStatusText = TodStringTranslate("[STATUS_GUEST_LEFT]");
                    }
                }
                break;
            }
            case 0x86: { // ROOM_EXITED
                // 不管 host/guest 哪边退出成功，回到空闲
                mServerHosting = false;
                mServerJoined = false;
                mServerHostHasGuest = false;
                mServerHostedRoomId = 0;
                mServerJoinedRoomId = 0;

                mServerStatusText = TodStringTranslate("[STATUS_ROOM_EXITED]");

                // 退出后拉一次列表
                ServerSendQuery();
                break;
            }
            case 0x85: { // RELAY_BEGIN
                mServerStatusText = TodStringTranslate("[STATUS_BATTLE_BEGIN]");
                LOG_DEBUG("[MODE3] RELAY_BEGIN");

                // ✅ 如果已经交接过，就忽略（理论上不会进来，因为 mServerSock 会被置 -1）
                if (mServerSock < 0) {
                    LOG_DEBUG("[MODE3] RELAY_BEGIN ignored (already handed off)");
                    break;
                }

                // === 交接：把服务器 socket 复用给 MODE2 的全局收发 ===
                // 先清掉 WIFI 的两个 socket，避免 UpdateFrames 同时读两路
                if (tcpClientSocket >= 0) {
                    close(tcpClientSocket);
                    tcpClientSocket = -1;
                }
                if (tcpServerSocket >= 0) {
                    close(tcpServerSocket);
                    tcpServerSocket = -1;
                }
                tcp_connected = false;
                tcp_connecting = false;

                if (mServerHosting) {
                    // 我是房主：后续走 tcpClientSocket（你的 MODE2 host 收包逻辑就是读 tcpClientSocket）
                    tcpClientSocket = mServerSock;
                    // 保持非阻塞没问题：你 UpdateFrames 用 MSG_DONTWAIT，本来就非阻塞友好
                    mServerSock = -1; // ✅ 交接完成：让 WaitDialog 不再管理该 socket
                } else if (mServerJoined) {
                    // 我是房客：后续走 tcpServerSocket + tcp_connected=true
                    tcpServerSocket = mServerSock;
                    tcp_connected = true;
                    tcp_connecting = false;
                    mServerSock = -1; // ✅ 交接完成
                } else {
                    // 理论不该发生
                    LOG_DEBUG("[MODE3] RELAY_BEGIN but role unknown, disconnect");
                    ServerDisconnect("relay role unknown");
                    break;
                }

                // 进入对战：按两下A
                GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 1);
                GameButtonDown(Sexy::GamepadButton::GAMEPAD_BUTTON_A, 1);
                return;
            }


            case 0xFF: { // ERROR
                int ec = (len >= 1) ? (payload[0] & 0xFF) : -1;
                pvzstl::string strFmt = TodStringTranslate("[STATUS_CONNECT_FAIL_ERRNO_FMT]");
                mServerStatusText = StrFormat(strFmt.c_str(), ec);
                break;
            }
            default:
                break;
        }
    }
}

static bool SendAll(int sock, const void *data, size_t len) {
    const uint8_t *p = (const uint8_t *)data;
    size_t off = 0;
    while (off < len) {
        ssize_t n = send(sock, p + off, len - off, 0);
        if (n > 0) {
            off += (size_t)n;
            continue;
        }
        if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
            continue;
        return false;
    }
    return true;
}

bool WaitForSecondPlayerDialog::ServerSendU8(uint8_t b) {
    if (mServerSock < 0)
        return false;
    return SendAll(mServerSock, &b, 1);
}
void WaitForSecondPlayerDialog::ServerSendQuery() {
    // MsgType.QUERY = 0x02
    ServerSendU8(0x02);
}

void WaitForSecondPlayerDialog::ServerSendCreate() {
    if (mServerSock < 0)
        return;
    if (!mApp || !mApp->mPlayerInfo || !mApp->mPlayerInfo->mName)
        return;

    const char *name = mApp->mPlayerInfo->mName;
    int nlen = (int)std::strlen(name);
    if (nlen > 255)
        nlen = 255;

    uint8_t head[2];
    head[0] = 0x01;          // MsgType.CREATE
    head[1] = (uint8_t)nlen; // nameLen

    if (!SendAll(mServerSock, head, 2)) {
        mServerStatusText = TodStringTranslate("[STATUS_SEND_CREATE_FAIL]");
        return;
    }
    if (nlen > 0 && !SendAll(mServerSock, name, (size_t)nlen)) {
        mServerStatusText = TodStringTranslate("[STATUS_SEND_CREATE_FAIL]");
        return;
    }
}


void WaitForSecondPlayerDialog::DrawServerRoomList(Sexy::Graphics *g) {
    if (mServerRoomCount <= 0) {
        g->DrawString(TodStringTranslate("[SERVER_NO_ROOMS_TIP]"), 240, 240);
        return;
    }

    int yPos = 280;
    Sexy::Color oldColor = g->mColor;

    g->DrawString(TodStringTranslate("[SERVER_ROOM_LIST_TITLE]"), 230, 240);
    g->SetFont(*Sexy_FONT_DWARVENTODCRAFT18_Addr);
    int idx = mSelectedRoomIndex_Server;
    if (idx < 0)
        idx = 0;
    if (idx >= mServerRoomCount)
        idx = mServerRoomCount - 1;
    mSelectedRoomIndex_Server = idx;

    for (int i = 0; i < mServerRoomCount; i++) {
        if (i == mSelectedRoomIndex_Server) {
            TodDrawImageScaledF(g, addonImages.leaderboard_selector, 140, yPos - 35, 0.7, 0.7);
            g->SetColor(Sexy::Color(0, 255, 0));
        } else {
            g->SetColor(oldColor);
        }

        const ServerRoomItem &r = mServerRooms[i];
        pvzstl::string tagGaming = TodStringTranslate("[TAG_GAMING]");
        pvzstl::string tagFull = TodStringTranslate("[TAG_FULL]");
        pvzstl::string tag = r.gaming ? tagGaming : (r.full ? tagFull : "");
        pvzstl::string fmt = TodStringTranslate("[SERVER_ROOM_LINE_FMT]");
        pvzstl::string line = StrFormat(fmt.c_str(), r.name, r.roomId, tag.c_str());
        g->DrawString(line, 230, yPos);
        yPos += 45;
    }

    g->SetColor(oldColor);
}

void WaitForSecondPlayerDialog::ServerSelectRoomByMouse(int x, int y) {
    (void)x;

    // 与 DrawServerRoomList 的 yPos 对齐
    const int listY = 280 - 30;
    const int lineH = 45;

    if (mServerRoomCount <= 0)
        return;

    if (y >= listY && y < listY + mServerRoomCount * lineH) {
        int idx = (y - listY) / lineH;
        if (idx >= 0 && idx < mServerRoomCount) {
            if (mSelectedRoomIndex_Server != idx) {
                mSelectedRoomIndex_Server = idx;
                mApp->PlaySample(*Sexy_SOUND_GRAVEBUTTON_Addr);
            }
        }
    }
}

void WaitForSecondPlayerDialog::ServerSendJoinSelected() {
    if (mServerSock < 0)
        return;
    if (mServerRoomCount <= 0)
        return;

    int idx = mSelectedRoomIndex_Server;
    if (idx < 0)
        idx = 0;
    if (idx >= mServerRoomCount)
        idx = mServerRoomCount - 1;
    int roomId = mServerRooms[idx].roomId;

    uint8_t buf[1 + 4];
    buf[0] = 0x03; // JOIN
    homura::WriteBEI32(buf + 1, roomId);

    if (!SendAll(mServerSock, buf, sizeof(buf))) {
        mServerStatusText = TodStringTranslate("[STATUS_SEND_JOIN_FAIL]");
        ServerDisconnect("join send fail");
    }
}


void WaitForSecondPlayerDialog::ServerSendExitRoom() {
    // EXIT_ROOM = 0x06
    if (!ServerSendU8(0x06)) {
        mServerStatusText = TodStringTranslate("[STATUS_SEND_EXIT_FAIL]");
        ServerDisconnect("exit send fail");
    }
}

void WaitForSecondPlayerDialog::ServerSendLeaveRoom() {
    // LEAVE_ROOM = 0x07
    if (!ServerSendU8(0x07)) {
        mServerStatusText = TodStringTranslate("[STATUS_SEND_LEAVE_FAIL]");
        ServerDisconnect("leave send fail");
    }
}

void WaitForSecondPlayerDialog::ServerSendStart() {
    // START = 0x05
    if (!ServerSendU8(0x05)) {
        mServerStatusText = TodStringTranslate("[STATUS_SEND_START_FAIL]");
        ServerDisconnect("start send fail");
    }
}


bool WaitForSecondPlayerDialog::ServerConnectFromInput() {
    const std::string input = std::move(gInputString);
    gHasInputContent = false;
    gHasInputContent.notify_one();
    LOG_DEBUG("input: '{}'", input);

    const size_t colonPos = input.find(':');
    if (colonPos == std::string::npos) {
        mServerStatusText = TodStringTranslate("[STATUS_ADDR_FORMAT_ERROR]");
        return false;
    }

    const std::string portStr = homura::Trim(std::string_view{input}.substr(colonPos + 1));
    const int port = std::atoi(portStr.c_str());
    if (port < 1 || port > 65535) {
        mServerStatusText = TodStringTranslate("[STATUS_PORT_ERROR]");
        return false;
    }

    const std::string ip = homura::Trim(std::string_view{input}.substr(0, colonPos));
    in_addr addr{};
    if (inet_pton(AF_INET, ip.c_str(), &addr) != 1) {
        mServerStatusText = TodStringTranslate("[STATUS_IP_ERROR]");
        return false;
    }

    // 如果之前连着，先断
    if (mServerSock >= 0) {
        ServerDisconnect("reconnect");
    }

    mServerIp[ip.copy(mServerIp, INET_ADDRSTRLEN - 1)] = '\0';
    mServerPort = port;
    LOG_DEBUG("target: {}:{}", &mServerIp[0], mServerPort);

    // 建 socket + 非阻塞 connect
    mServerSock = socket(AF_INET, SOCK_STREAM, 0);
    if (mServerSock < 0) {
        mServerStatusText = TodStringTranslate("[STATUS_SOCKET_FAIL]");
        return false;
    }

    int one = 1;
    setsockopt(mServerSock, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

    int flags = fcntl(mServerSock, F_GETFL, 0);
    fcntl(mServerSock, F_SETFL, flags | O_NONBLOCK);

    sockaddr_in sa{
        .sin_family = AF_INET,
        .sin_port = htons(uint16_t(mServerPort)),
    };
    inet_pton(AF_INET, mServerIp, &sa.sin_addr);

    int ret = connect(mServerSock, (sockaddr *)&sa, sizeof(sa));
    int err = errno; // 立刻保存
    LOG_DEBUG("[MODE3] connect ret={} errno={}", ret, err);
    if (ret == 0) {
        mServerConnecting = false;
        mServerConnected = true;
        mServerStatusText = TodStringTranslate("[STATUS_CONNECTED]");

        // 刚连上先拉一次列表
        mServerRoomCount = 0;
        mSrvRecvLen = 0;
        ServerSendQuery();
        return true;
    }

    if (err == EINPROGRESS) {
        mServerConnecting = true;
        mServerConnected = false;
        mServerStatusText = TodStringTranslate("[STATUS_CONNECTING]");
        return true;
    }

    ServerDisconnect("connect fail");
    pvzstl::string strFmt = TodStringTranslate("[STATUS_CONNECT_FAIL_ERRNO_FMT]");
    mServerStatusText = StrFormat(strFmt.c_str(), std::strerror(err));
    return false;
}

void WaitForSecondPlayerDialog::ServerDisconnect([[maybe_unused]] const char *why) {
    if (mServerSock >= 0) {
        shutdown(mServerSock, SHUT_RDWR);
        close(mServerSock);
        mServerSock = -1;
    }

    mServerConnecting = false;
    mServerConnected = false;

    mServerHosting = false;
    mServerJoined = false;
    mServerHostHasGuest = false;
    mServerHostedRoomId = 0;
    mServerJoinedRoomId = 0;

    mServerRoomCount = 0;
    mSelectedRoomIndex_Server = 0;
    mSrvRecvLen = 0;

    mServerStatusText = TodStringTranslate("[STATUS_NOT_CONNECTED]");
}


void WaitForSecondPlayerDialog::Resize(int theX, int theY, int theWidth, int theHeight) {}


void WaitForSecondPlayerDialog::MouseDown(int x, int y, int theClickCount) {
    // MODE3：点选服务器房间
    if (mUIMode == UIMode::MODE3_SERVER) {
        if (!mServerConnected)
            return;
        if (mServerHosting || mServerJoined)
            return; // 在房间里不让选（你也可以允许）
        ServerSelectRoomByMouse(x, y);
        return;
    }

    // ===== 下面保持你原来的 WIFI 逻辑 =====
    if (mIsCreatingRoom || mIsJoiningRoom)
        return;
    if (scanned_server_count <= 0)
        return;

    [[maybe_unused]] const int listX = 230;
    const int listY = 180 - 30;
    const int lineH = 50;

    if (y >= listY && y < listY + scanned_server_count * lineH) {
        int idx = (y - listY) / lineH;
        if (idx >= 0 && idx < scanned_server_count) {
            if (mSelectedServerIndex != idx) {
                mSelectedServerIndex = idx;
                mApp->PlaySample(*Sexy_SOUND_GRAVEBUTTON_Addr);
            }
        }
    }
}
