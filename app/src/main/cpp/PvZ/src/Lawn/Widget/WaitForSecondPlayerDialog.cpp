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
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/LawnApp.h"
#include <arpa/inet.h>
#include <asm-generic/fcntl.h>
#include <fcntl.h>
#include <linux/in.h>
#include <sys/endian.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>

using namespace Sexy;

void WaitForSecondPlayerDialog::_constructor(LawnApp *theApp) {
    old_WaitForSecondPlayerDialog_WaitForSecondPlayerDialog(this, theApp);

    //    GameButtonDown(ButtonCode::BUTTONCODE_A, 1);
    //    GameButtonDown(ButtonCode::BUTTONCODE_A, 1);

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
            pvzstl::string str1 = StrFormat("网口:%s PORT:%d",ifname.c_str(), tcpPort);
            g->DrawString(str1, 260, 200);
        }

        pvzstl::string str2 = udpBroadcastSocket >= 0 ? StrFormat("房间可被查找") : StrFormat("房间不可查找") ;
        g->DrawString(str2, 260, 250);

        pvzstl::string str3 = tcpClientSocket == -1 ? StrFormat("对方未加入...") : StrFormat("对方已加入！");
        g->DrawString(str3, 260, 300);

    } else if (mIsJoiningRoom) {
        pvzstl::string str = tcp_connected ? StrFormat("已加入至: %s的房间", servers[0].name) : StrFormat("正在加入: %s的房间", servers[0].name);
        g->DrawString(str, 280, 150);
        pvzstl::string str1 = StrFormat("IP: %s:%d", servers[0].ip, servers[0].tcp_port);
        g->DrawString(str1, 280, 200);
    } else if (scanned_server_count == 0) {
        pvzstl::string str1 = udpScanSocket >= 0 ? StrFormat("查找房间中...") : StrFormat("无法查找房间");
        g->DrawString(str1, 340, 200);
    } else {
        int yPos = 180;
        for (int i = 0; i < scanned_server_count; i++) {
            pvzstl::string str = StrFormat("%s的房间 %s:%d", servers[i].name, servers[i].ip, servers[i].tcp_port);
            g->DrawString(str, 230, yPos);
            yPos += 30;
        }
    }
}


void WaitForSecondPlayerDialog::Update() {
    //    mJoinButton->mDisabled = server_count == 0;

    if (!mIsCreatingRoom && !mIsJoiningRoom) {
        mJoinButton->mDisabled = scanned_server_count == 0;
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

    if (tcpClientSocket >= 0) {
        char buf[1024];

        while (true) {
            ssize_t n = recv(tcpClientSocket, buf, sizeof(buf), MSG_DONTWAIT);
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

    if (mIsJoiningRoom) {
        if (tcp_connected) {
                char buf[1024];
                while (true) {
                    ssize_t n = recv(tcpServerSocket, buf, sizeof(buf), MSG_DONTWAIT);
                    if (n > 0) {
//                        buf[n] = '\0'; // 确保字符串结束
//                        LOG_DEBUG("[TCP] 收到来自Server的数据: %s", buf);
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
        } else {
            TryTcpConnect();
        }
    }
}

void WaitForSecondPlayerDialog::HandleTcpClientMessage(void* buf,ssize_t bufSize) {

}

void WaitForSecondPlayerDialog::HandleTcpServerMessage(void* buf,ssize_t bufSize) {
    BaseEvent* event = (BaseEvent *)buf;
    switch (event->type) {
        case EVENT_START_GAME:
            GameButtonDown(ButtonCode::BUTTONCODE_A, 1);
            GameButtonDown(ButtonCode::BUTTONCODE_A, 1);
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
    //    scanned_server_count = 0;
}

bool WaitForSecondPlayerDialog::GetActiveBroadcast(sockaddr_in& out_bcast, std::string* out_ifname) {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) return false;

    struct ifconf ifc;
    char buf[1024];
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if (ioctl(fd, SIOCGIFCONF, &ifc) < 0) {
        close(fd);
        return false;
    }

    struct ifreq* it = (struct ifreq*)buf;
    struct ifreq* end = (struct ifreq*)(buf + ifc.ifc_len);

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
            struct sockaddr_in* sin = (struct sockaddr_in*)&ifr.ifr_broadaddr;
            if (sin->sin_family != AF_INET)
                continue;

            // ✅ Wi-Fi / 热点接口优先（wlan*, ap*, en*）
            if (strncasecmp(ifr.ifr_name, "wlan", 4) == 0 ||
                strncasecmp(ifr.ifr_name, "ap", 2) == 0 ||
                strncasecmp(ifr.ifr_name, "en", 2) == 0) {
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
        if (out_ifname) *out_ifname = wifi_if;
        return true;
    } else if (found_other) {
        out_bcast = other_bcast;
        if (out_ifname) *out_ifname = other_if;
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

    //    LOGD("TCP server listening on port %d...\n", tcpPort);

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
}

void WaitForSecondPlayerDialog::UdpBroadcastRoom() {
    lastBroadcastTime = 0;
    if (udpBroadcastSocket < 0)
        return;
    LawnApp *lawnApp = reinterpret_cast<LawnApp *>(*gLawnApp_Addr);
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
            LOG_DEBUG("sendto ERROR {}",errno);
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
    setsockopt(tcpClientSocket, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));   // 禁用 Nagle 算法
    int on = 1;  setsockopt(tcpClientSocket, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(on));
    int idle = 30;     setsockopt(tcpClientSocket, IPPROTO_TCP, TCP_KEEPIDLE,  &idle, sizeof(idle));
    int intvl = 10;    setsockopt(tcpClientSocket, IPPROTO_TCP, TCP_KEEPINTVL, &intvl, sizeof(intvl));
    int cnt = 3;       setsockopt(tcpClientSocket, IPPROTO_TCP, TCP_KEEPCNT,   &cnt,  sizeof(cnt));

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
            LOG_DEBUG("[Scan] Server timeout:{}:{} ({})\n", servers[i].ip, servers[i].tcp_port, servers[i].name);

            // 删除该服务端，将最后一个移到当前位置
            servers[i] = servers[scanned_server_count - 1];
            scanned_server_count--;
            continue;
        }
        i++;
    }
}


void WaitForSecondPlayerDialog::TryTcpConnect() {
    if (tcp_connected)
        return; // 已连接成功
    if (scanned_server_count == 0)
        return; // 没有可连接的服务器

    server_info *target = &servers[0]; // 第一个服务端

    if (!tcp_connecting) {
        // 第一次尝试连接
        tcpServerSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (tcpServerSocket < 0) {
            LOG_DEBUG("[Client] socket ERROR");
            return;
        }

        // 设置非阻塞
        int flags = fcntl(tcpServerSocket, F_GETFL, 0);
        fcntl(tcpServerSocket, F_SETFL, flags | O_NONBLOCK);

        // 目标地址
        sockaddr_in server_addr{};
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(target->tcp_port);
        inet_pton(AF_INET, target->ip, &server_addr.sin_addr);

        // 发起非阻塞connect
        int ret = connect(tcpServerSocket, (sockaddr *)&server_addr, sizeof(server_addr));
        if (ret < 0) {
            if (errno == EINPROGRESS) {
                tcp_connecting = true; // 正在连接
                LOG_DEBUG("[Client] Connecting to {}:{} ...\n", target->ip, target->tcp_port);
            } else {
                LOG_DEBUG("[Client] connect ERROR");
                close(tcpServerSocket);
                tcpServerSocket = -1;
                tcp_connecting = false;
                tcp_connected = false;
            }
        } else {
            // connect立即成功（本地可能直接成功）
            int one = 1;
            setsockopt(tcpServerSocket, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));   // 禁用 Nagle 算法
            int on = 1;  setsockopt(tcpClientSocket, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(on));
            int idle = 30;     setsockopt(tcpClientSocket, IPPROTO_TCP, TCP_KEEPIDLE,  &idle, sizeof(idle));
            int intvl = 10;    setsockopt(tcpClientSocket, IPPROTO_TCP, TCP_KEEPINTVL, &intvl, sizeof(intvl));
            int cnt = 3;       setsockopt(tcpClientSocket, IPPROTO_TCP, TCP_KEEPCNT,   &cnt,  sizeof(cnt));
            tcp_connected = true;
            tcp_connecting = false;
            LOG_DEBUG("[Client] Connected immediately to {}:{}\n", target->ip, target->tcp_port);
        }

    } else {
        // 检查连接是否完成
        fd_set writefds;
        FD_ZERO(&writefds);
        FD_SET(tcpServerSocket, &writefds);

        timeval tv{0, 0}; // 非阻塞
        int ret = select(tcpServerSocket + 1, nullptr, &writefds, nullptr, &tv);
        if (ret > 0 && FD_ISSET(tcpServerSocket, &writefds)) {
            // 检查错误
            int err = 0;
            socklen_t len = sizeof(err);
            if (getsockopt(tcpServerSocket, SOL_SOCKET, SO_ERROR, &err, &len) < 0) {
                LOG_DEBUG("[Client] getsockopt ERROR");
                close(tcpServerSocket);
                tcpServerSocket = -1;
                tcp_connecting = false;
                tcp_connected = false;
                return;
            }

            if (err == 0) {
                int one = 1;
                setsockopt(tcpServerSocket, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));   // 禁用 Nagle 算法
                int on = 1;  setsockopt(tcpClientSocket, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(on));
                int idle = 30;     setsockopt(tcpClientSocket, IPPROTO_TCP, TCP_KEEPIDLE,  &idle, sizeof(idle));
                int intvl = 10;    setsockopt(tcpClientSocket, IPPROTO_TCP, TCP_KEEPINTVL, &intvl, sizeof(intvl));
                int cnt = 3;       setsockopt(tcpClientSocket, IPPROTO_TCP, TCP_KEEPCNT,   &cnt,  sizeof(cnt));
                tcp_connected = true;
                tcp_connecting = false;
                LOG_DEBUG("[Client] Connected to {}:{}\n", target->ip, target->tcp_port);
            } else {
                LOG_DEBUG("[Client] Connect failed, err={}", err);
                close(tcpServerSocket);
                tcpServerSocket = -1;
                tcp_connecting = false;
                tcp_connected = false;
            }
        }
        // 如果select返回0，说明连接还在进行中，下次Update再检查
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
    auto *dialog = reinterpret_cast<WaitForSecondPlayerDialog *>((uint32_t(listener) - offsetof(WaitForSecondPlayerDialog, mButtonListener)));
    if (id == 1000) {
        // 2P手柄按两下A
        dialog->GameButtonDown(ButtonCode::BUTTONCODE_A, 1);
        dialog->GameButtonDown(ButtonCode::BUTTONCODE_A, 1);

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
    }

    old_WaitForSecondPlayerDialog_ButtonDepress(listener, id);
}

void WaitForSecondPlayerDialog::Resize(int theX, int theY, int theWidth, int theHeight) {}