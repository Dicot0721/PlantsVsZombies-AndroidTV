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
#include "PvZ/Lawn/Widget/WaitForSecondPlayerDialog.h"
#include "Homura/Logger.h"
#include "PvZ/Android/Native/BridgeApp.h"
#include "PvZ/Android/Native/NativeApp.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/LawnApp.h"
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

void WaitForSecondPlayerDialog::_constructor(LawnApp *theApp) {
    old_WaitForSecondPlayerDialog_WaitForSecondPlayerDialog(this, theApp);

    // GameButtonDown(GamepadButton::BUTTONCODE_A, 1);
    // GameButtonDown(GamepadButton::BUTTONCODE_A, 1);

    // 解决此Dialog显示时背景僵尸全部聚集、且草丛大块空缺的问题
    if (theApp->mBoard != nullptr) {
        theApp->mBoard->UpdateGame();
        theApp->mBoard->UpdateCoverLayer();
    }

    GameButton *playOfflineButton = MakeButton(1000, &mButtonListener, this, "[PLAY_OFFLINE]");
    mLawnYesButton = playOfflineButton;

    GameButton *backButton = MakeButton(1001, &mButtonListener, this, "[BACK]");
    mLawnNoButton = backButton;

    mJoinButton = MakeButton(1002, &mButtonListener, this, "[JOIN_ROOM_BUTTON]");
    mJoinButton->mDisabled = true;
    AddWidget(mJoinButton);

    mCreateButton = MakeButton(1003, &mButtonListener, this, "[CREATE_ROOM_BUTTON]");
    AddWidget(mCreateButton);

    this->LawnDialog::Resize(0, 0, 800, 600);

    playOfflineButton->mY -= 20;
    playOfflineButton->mWidth -= 30;
    playOfflineButton->mX += 15;

    backButton->mY -= 20;
    backButton->mWidth -= 30;
    backButton->mX += 15;

    mJoinButton->mX = playOfflineButton->mX;
    mJoinButton->mY = playOfflineButton->mY - 80;
    mJoinButton->mWidth = playOfflineButton->mWidth;
    mJoinButton->mHeight = playOfflineButton->mHeight;

    mCreateButton->mX = backButton->mX;
    mCreateButton->mY = backButton->mY - 80;
    mCreateButton->mWidth = backButton->mWidth;
    mCreateButton->mHeight = backButton->mHeight;

    InitUdpScanSocket();
    mIsCreatingRoom = false;
    mIsJoiningRoom = false;

    mSelectedServerIndex = 0;
    mUseManualTarget = false;
    mManualIp[0] = '\0';
    mManualPort = 0;

    // 连接按钮
    mDirectConnectButton = MakeButton(1004, &mButtonListener, this, "[CONNECT_IP]");
    mDirectConnectButton->mX = 300;
    mDirectConnectButton->mY = 350;
    mDirectConnectButton->mWidth = 180;
    mDirectConnectButton->mHeight = 40;
    AddWidget(mDirectConnectButton);

    if (tcp_connected || tcpClientSocket >= 0) {
        GameButtonDown(GamepadButton::BUTTONCODE_A, 1);
        GameButtonDown(GamepadButton::BUTTONCODE_A, 1);
    }
}

void WaitForSecondPlayerDialog::_destructor() {
    old_WaitForSecondPlayerDialog_Delete(this);
}
void WaitForSecondPlayerDialog::Draw(Graphics *g) {
    old_WaitForSecondPlayerDialog_Draw(this, g);

    if (mIsCreatingRoom) {
        pvzstl::string str = StrFormat("您的房间名称: %s的房间", mApp->mPlayerInfo->mName);
        g->DrawString(str, 230, 150);

        if (tcpPort != 0) {
            pvzstl::string str1 = StrFormat("网口:%s PORT:%d", ifname.c_str(), tcpPort);
            g->DrawString(str1, 260, 200);
        }

        pvzstl::string str2 = udpBroadcastSocket >= 0 ? StrFormat("房间可被查找") : StrFormat("房间不可查找");
        g->DrawString(str2, 260, 250);

        pvzstl::string str3 = tcpClientSocket == -1 ? StrFormat("对方未加入...") : StrFormat("对方已加入！");
        g->DrawString(str3, 260, 300);
        return;
    }

    // ===== joining 状态：支持手动 & 选中房间 =====
    if (mIsJoiningRoom) {
        if (mUseManualTarget) {
            // 手动连接
            pvzstl::string str = tcp_connected ? StrFormat("已加入至: 手动连接") : StrFormat("正在加入: 手动连接");
            g->DrawString(str, 280, 150);

            pvzstl::string str1 = StrFormat("IP: %s:%d", mManualIp, mManualPort);
            g->DrawString(str1, 280, 200);

        } else {
            // 扫描列表连接：用当前选中项，而不是 servers[0]
            int idx = mSelectedServerIndex;
            if (idx < 0)
                idx = 0;
            if (idx >= scanned_server_count)
                idx = scanned_server_count - 1;

            if (scanned_server_count <= 0) {
                pvzstl::string str1 = StrFormat("正在加入: (无可用房间)");
                g->DrawString(str1, 280, 150);
            } else {
                pvzstl::string str = tcp_connected ? StrFormat("已加入至: %s的房间", servers[idx].name) : StrFormat("正在加入: %s的房间", servers[idx].name);
                g->DrawString(str, 280, 150);

                pvzstl::string str1 = StrFormat("IP: %s:%d", servers[idx].ip, servers[idx].tcp_port);
                g->DrawString(str1, 280, 200);
            }
        }
        return;
    }

    // ===== 非 joining/creating：显示扫描状态或列表 =====
    if (scanned_server_count == 0) {
        pvzstl::string str1 = udpScanSocket >= 0 ? StrFormat("查找房间中...") : StrFormat("无法查找房间");
        g->DrawString(str1, 340, 200);
        return;
    }

    // 列表：高亮 mSelectedServerIndex（绿色），支持选择其他房间
    int idx = mSelectedServerIndex;
    if (idx < 0)
        idx = 0;
    if (idx >= scanned_server_count)
        idx = scanned_server_count - 1;
    mSelectedServerIndex = idx;

    int yPos = 180;
    Sexy::Color oldColor = g->mColor;

    for (int i = 0; i < scanned_server_count; i++) {
        if (i == mSelectedServerIndex) {
            g->SetColor(Sexy::Color(0, 255, 0)); // 绿色高亮
        } else {
            g->SetColor(oldColor); // 恢复默认
        }

        pvzstl::string line = StrFormat("%s的房间 %s:%d", servers[i].name, servers[i].ip, servers[i].tcp_port);
        g->DrawString(line, 230, yPos);
        yPos += 50; // 你想更稀疏就改成 50
    }

    g->SetColor(oldColor);
}


bool WaitForSecondPlayerDialog::ManualIpConnect() {
    // 1) 没有新输入就直接返回
    if (gInputString.empty())
        return false;

    // 2) 取走输入并清空（避免重复触发）
    std::string input = gInputString;
    gInputString.clear();

    LOG_DEBUG("[ManualIpConnect] raw input='{}'", input.c_str());

    // ====== 内联 Trim ======
    auto Trim = [](const std::string &s) -> std::string {
        const char *ws = " \t\r\n";
        size_t a = s.find_first_not_of(ws);
        if (a == std::string::npos)
            return "";
        size_t b = s.find_last_not_of(ws);
        return s.substr(a, b - a + 1);
    };

    // ====== 解析 IP / IP:PORT（PORT 可缺省） ======
    std::string t = Trim(input);
    if (t.empty()) {
        LOG_DEBUG("[ManualIpConnect] empty input");
        return false;
    }

    std::string ip = t;
    int port = 0;

    size_t pos = t.find(':');
    if (pos != std::string::npos) {
        ip = Trim(t.substr(0, pos));
        std::string p = Trim(t.substr(pos + 1));
        if (!p.empty()) {
            port = atoi(p.c_str());
            if (port <= 0 || port > 65535) {
                LOG_DEBUG("[ManualIpConnect] invalid port in '{}'", t.c_str());
                return false;
            }
        } else {
            LOG_DEBUG("[ManualIpConnect] port is null");
            return false;
        }
    }

    // 校验 IP
    in_addr addr{};
    if (inet_pton(AF_INET, ip.c_str(), &addr) != 1) {
        LOG_DEBUG("[ManualIpConnect] invalid ip '{}'", ip.c_str());
        return false;
    }

    // 3) 保存为手动目标
    std::memset(mManualIp, 0, sizeof(mManualIp));
    std::strncpy(mManualIp, ip.c_str(), INET_ADDRSTRLEN - 1);
    mManualPort = port;
    mUseManualTarget = true;

    LOG_DEBUG("[ManualIpConnect] target {}:{}", mManualIp, mManualPort);

    // 4) 切换到 joining 状态，重置连接状态（避免旧状态干扰）
    mIsJoiningRoom = true;
    CloseUdpScanSocket();

    mCreateButton->mDisabled = true;
    mJoinButton->SetLabel("[LEAVE_ROOM_BUTTON]");

    mLawnYesButton->mDisabled = true;
    mLawnYesButton->SetLabel("[PLAY_ONLINE]");

    if (tcpServerSocket >= 0) {
        shutdown(tcpServerSocket, SHUT_RDWR);
        close(tcpServerSocket);
        tcpServerSocket = -1;
    }
    tcp_connecting = false;
    tcp_connected = false;

    // 5) 关闭扫描 socket（避免 scan 模式逻辑干扰）
    CloseUdpScanSocket();

    // （可选）如果你希望这里同步更新按钮状态/文字，也可以放在这里

    return true;
}

void WaitForSecondPlayerDialog::Update() {
    // mJoinButton->mDisabled = server_count == 0;

    bool inScanMode = (!mIsCreatingRoom && !mIsJoiningRoom);
    if (inScanMode) {
        mJoinButton->mDisabled = scanned_server_count == 0;
        // 扫描列表从 0 -> >0 时，默认选中第一个
        if (scanned_server_count > 0 && mSelectedServerIndex < 0) {
            mSelectedServerIndex = 0;
        }
        // 越界修正
        if (mSelectedServerIndex >= scanned_server_count) {
            mSelectedServerIndex = scanned_server_count - 1;
        }
        if (mSelectedServerIndex < 0)
            mSelectedServerIndex = 0;

        if (!gInputString.empty()) {
            mUseManualTarget = true;
            ManualIpConnect();
        }
    }

    if (mDirectConnectButton) {
        mDirectConnectButton->mVisible = inScanMode;
        mDirectConnectButton->mDisabled = !inScanMode;
    }

    if (mIsCreatingRoom) {
        mLawnYesButton->mDisabled = tcpClientSocket == -1;
    }

    lastBroadcastTime++;
    if (lastBroadcastTime >= 100) {
        if (mIsCreatingRoom) {
            UdpBroadcastRoom(); // 每秒广播一次
        } else if (!mIsJoiningRoom) {
            ScanUdpBroadcastRoom();
        }
    }

    if (tcpListenSocket >= 0) {
        CheckTcpAccept();
    }

    if (mIsJoiningRoom && !tcp_connected) {
        TryTcpConnect();
    }
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
            return sizeof(BaseEvent);
        default:
            return sizeof(BaseEvent);
    }
}

void WaitForSecondPlayerDialog::processServerEvent(void *buf, ssize_t bufSize) {
    BaseEvent *event = (BaseEvent *)buf;
    LOG_DEBUG("TYPE:{}", (int)event->type);
    switch (event->type) {
        case EVENT_START_GAME:
            GameButtonDown(GamepadButton::BUTTONCODE_A, 1);
            GameButtonDown(GamepadButton::BUTTONCODE_A, 1);
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
    sockaddr_in recv_addr{};
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = htons(UDP_PORT);
    recv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(udpScanSocket, (struct sockaddr *)&recv_addr, sizeof(recv_addr)) < 0) {
        LOG_DEBUG("bind ERROR");
        close(udpScanSocket);
        udpScanSocket = -1;
        return;
    }

    LOG_DEBUG("[UDP Scan] Listening on port %d\n", UDP_PORT);
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

    struct ifconf ifc;
    char buf[1024];
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if (ioctl(fd, SIOCGIFCONF, &ifc) < 0) {
        close(fd);
        return false;
    }

    struct ifreq *it = (struct ifreq *)buf;
    struct ifreq *end = (struct ifreq *)(buf + ifc.ifc_len);

    bool found_wifi = false;
    bool found_other = false;
    sockaddr_in wifi_bcast{};
    sockaddr_in other_bcast{};
    std::string wifi_if, other_if;

    for (; it < end; ++it) {
        struct ifreq ifr;
        std::memset(&ifr, 0, sizeof(ifr));
        std::strncpy(ifr.ifr_name, it->ifr_name, IFNAMSIZ);

        // 跳过回环 / 未启用接口
        if (ioctl(fd, SIOCGIFFLAGS, &ifr) == 0) {
            if ((ifr.ifr_flags & IFF_LOOPBACK) || !(ifr.ifr_flags & IFF_UP))
                continue;
        }

        // 获取广播地址
        if (ioctl(fd, SIOCGIFBRDADDR, &ifr) == 0) {
            struct sockaddr_in *sin = (struct sockaddr_in *)&ifr.ifr_broadaddr;
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
    // 1. 创建TCP监听socket
    tcpListenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcpListenSocket < 0) {
        LOG_DEBUG("TCP socket failed");
        return;
    }
    int flags = fcntl(tcpListenSocket, F_GETFL, 0);
    fcntl(tcpListenSocket, F_SETFL, flags | O_NONBLOCK);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(0);

    int opt = 1;
    setsockopt(tcpListenSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));

    if (bind(tcpListenSocket, (sockaddr *)&addr, sizeof(addr)) < 0) {
        LOG_DEBUG("TCP bind failed");
        return;
    }

    if (listen(tcpListenSocket, 1) < 0) {
        LOG_DEBUG("TCP listen failed");
        return;
    }

    // 获取实际分配的端口号
    socklen_t addr_len = sizeof(addr);
    getsockname(tcpListenSocket, (struct sockaddr *)&addr, &addr_len);
    tcpPort = ntohs(addr.sin_port);

    // LOGD("TCP server listening on port %d...\n", tcpPort);

    // 2. 创建UDP广播socket
    udpBroadcastSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpBroadcastSocket < 0) {
        LOG_DEBUG("UDP socket failed");
        return;
    }


    int on = 1;
    setsockopt(udpBroadcastSocket, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
    setsockopt(udpBroadcastSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    // 选择广播地址
    sockaddr_in bcast{};

    if (GetActiveBroadcast(bcast, &ifname)) {
        bcast.sin_port = htons(UDP_PORT);
        broadcast_addr = bcast;
        // 可选：打印一下，便于诊断
        char ipstr[INET_ADDRSTRLEN]{};
        inet_ntop(AF_INET, &bcast.sin_addr, ipstr, sizeof(ipstr));
        LOG_DEBUG("[UDP] use if={}", ifname.c_str());
    } else {
        // 兜底：尽量别用全局广播，尝试常见热点网段；再不行再用 255.255.255.255
        memset(&broadcast_addr, 0, sizeof(broadcast_addr));
        broadcast_addr.sin_family = AF_INET;
        broadcast_addr.sin_port = htons(UDP_PORT);
        inet_pton(AF_INET, "255.255.255.255", &broadcast_addr.sin_addr); // Android 热点常见
        LOG_WARN("[UDP] fallback broadcast 255.255.255.255:{}", UDP_PORT);
    }


    flags = fcntl(udpBroadcastSocket, F_GETFL, 0);
    fcntl(udpBroadcastSocket, F_SETFL, flags | O_NONBLOCK);

    LOG_DEBUG("[Host] Room created. TCP port={}, UDP port={}\n", tcpPort, UDP_PORT);
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

        ssize_t sent = sendto(udpBroadcastSocket, send_buf, total_len, 0, (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr));

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
        ssize_t n = recvfrom(udpScanSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&recv_addr, &addr_len);
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

            time_t now = time(NULL);
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
    time_t current_time = time(NULL);
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
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(target_port);
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


void WaitForSecondPlayerDialog::ShowIpInputDialog() {

    LOG_DEBUG("[ShowIpInputDialog] enter");

    Native::BridgeApp *bridgeApp = Native::BridgeApp::getSingleton();
    if (!bridgeApp) {
        LOG_DEBUG("[ShowIpInputDialog] BridgeApp is null");
        return;
    }

    if (!bridgeApp->mNativeApp) {
        LOG_DEBUG("[ShowIpInputDialog] mNativeApp is null");
        return;
    }

    JNIEnv *env = bridgeApp->getJNIEnv();
    if (!env) {
        LOG_DEBUG("[ShowIpInputDialog] JNIEnv is null");
        return;
    }

    jobject view = bridgeApp->mNativeApp->getView();
    if (!view) {
        LOG_DEBUG("[ShowIpInputDialog] NativeView is null");
        return;
    }

    LOG_DEBUG("[ShowIpInputDialog] NativeView obtained");

    // view 是 com.transmension.mobile.NativeView
    jclass viewCls = env->GetObjectClass(view);
    if (!viewCls) {
        LOG_DEBUG("[ShowIpInputDialog] GetObjectClass(view) failed");
        return;
    }

    LOG_DEBUG("[ShowIpInputDialog] NativeView class obtained");

    // 方法签名：void showTextInputDialog2(int mode, String title, String hint, String initial)
    jmethodID mid = env->GetMethodID(viewCls, "showTextInputDialog2", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if (!mid) {
        LOG_DEBUG("[ShowIpInputDialog] GetMethodID(showTextInputDialog2) failed");
        env->DeleteLocalRef(viewCls);
        if (env->ExceptionCheck()) {
            env->ExceptionDescribe();
            env->ExceptionClear();
        }
        return;
    }

    LOG_DEBUG("[ShowIpInputDialog] Method showTextInputDialog2 found");

    jstring jTitle = env->NewStringUTF("加入此IP的房间:");
    jstring jHint = env->NewStringUTF("IP:PORT");
    jstring jInitial = env->NewStringUTF("");

    if (!jTitle || !jHint || !jInitial) {
        LOG_DEBUG("[ShowIpInputDialog] Failed to create jstring");
        env->DeleteLocalRef(viewCls);
        return;
    }

    LOG_DEBUG("[ShowIpInputDialog] Calling showTextInputDialog2");

    env->CallVoidMethod(view, mid, 0, jTitle, jHint, jInitial);

    if (env->ExceptionCheck()) {
        LOG_DEBUG("[ShowIpInputDialog] Java exception occurred during CallVoidMethod");
        env->ExceptionDescribe(); // 调试期建议保留
        env->ExceptionClear();
    } else {
        LOG_DEBUG("[ShowIpInputDialog] showTextInputDialog2 call succeeded");
    }

    env->DeleteLocalRef(jTitle);
    env->DeleteLocalRef(jHint);
    env->DeleteLocalRef(jInitial);
    env->DeleteLocalRef(viewCls);

    LOG_DEBUG("[ShowIpInputDialog] exit");
}


void WaitForSecondPlayerDialog_ButtonDepress(Sexy::ButtonListener *listener, int id) {
    auto *dialog = reinterpret_cast<WaitForSecondPlayerDialog *>((uint32_t(listener) - offsetof(WaitForSecondPlayerDialog, mButtonListener)));
    if (id == 1000) {
        // 2P手柄按两下A
        dialog->GameButtonDown(GamepadButton::BUTTONCODE_A, 1);
        dialog->GameButtonDown(GamepadButton::BUTTONCODE_A, 1);

        if (tcpClientSocket >= 0) {
            BaseEvent event = {EventType::EVENT_START_GAME};
            send(tcpClientSocket, &event, sizeof(BaseEvent), 0);
        }
    } else if (id == 1001) {
        dialog->StopUdpBroadcastRoom();
        dialog->LeaveRoom();
        dialog->ExitRoom();
        dialog->CloseUdpScanSocket();

    } else if (id == 1002) {


        if (dialog->mIsJoiningRoom) {
            // 启动扫描线程
            dialog->LeaveRoom();
            dialog->InitUdpScanSocket();

            dialog->mCreateButton->mDisabled = false;
            dialog->mJoinButton->SetLabel("[JOIN_ROOM_BUTTON]");

            dialog->mLawnYesButton->mDisabled = false;
            dialog->mLawnYesButton->SetLabel("[PLAY_OFFLINE]");
        } else {
            dialog->JoinRoom();
            dialog->CloseUdpScanSocket();

            dialog->mCreateButton->mDisabled = true;
            dialog->mJoinButton->SetLabel("[LEAVE_ROOM_BUTTON]");

            dialog->mLawnYesButton->mDisabled = true;
            dialog->mLawnYesButton->SetLabel("[PLAY_ONLINE]");
        }
    } else if (id == 1003) {

        if (dialog->mIsCreatingRoom) {
            // 启动扫描线程
            dialog->ExitRoom();
            dialog->InitUdpScanSocket();

            dialog->mJoinButton->mDisabled = true;
            dialog->mCreateButton->SetLabel("[CREATE_ROOM_BUTTON]");

            dialog->mLawnYesButton->mDisabled = false;
            dialog->mLawnYesButton->SetLabel("[PLAY_OFFLINE]");
        } else {
            dialog->CreateRoom();
            dialog->CloseUdpScanSocket();

            dialog->mJoinButton->mDisabled = true;
            dialog->mCreateButton->SetLabel("[EXIT_ROOM_BUTTON]");

            dialog->mLawnYesButton->mDisabled = true;
            dialog->mLawnYesButton->SetLabel("[PLAY_ONLINE]");
        }
    } else if (id == 1004) {
        dialog->ShowIpInputDialog();
    }

    old_WaitForSecondPlayerDialog_ButtonDepress(listener, id);
}

void WaitForSecondPlayerDialog::Resize(int theX, int theY, int theWidth, int theHeight) {}


void WaitForSecondPlayerDialog::MouseDown(int x, int y, int clicks) {
    if (mIsCreatingRoom || mIsJoiningRoom)
        return;
    if (scanned_server_count <= 0)
        return;

    // 列表绘制起点和行高要与你 Draw 保持一致
    const int listX = 230;
    const int listY = 180 - 30;
    const int lineH = 50;

    // 简单判定：只用 y 命中即可
    if (y >= listY && y < listY + scanned_server_count * lineH) {
        int idx = (y - listY) / lineH;
        if (idx >= 0 && idx < scanned_server_count) {
            mSelectedServerIndex = idx;
        }
    }
}
