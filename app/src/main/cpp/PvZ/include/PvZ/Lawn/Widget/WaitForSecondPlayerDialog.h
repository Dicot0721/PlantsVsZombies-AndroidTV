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

#ifndef PVZ_LAWN_WIDGET_WAIT_FOR_SECOND_PLAYER_DIALOG_H
#define PVZ_LAWN_WIDGET_WAIT_FOR_SECOND_PLAYER_DIALOG_H

#include "PvZ/Lawn/GamepadControls.h"
#include "PvZ/Lawn/Widget/GameButton.h"
#include "PvZ/Lawn/Widget/LawnDialog.h"
#include "PvZ/Symbols.h"
#include <linux/in.h>
#include <netinet/in.h>

class WaitForSecondPlayerDialog : public __LawnDialog {
public:
    bool m2PJoined;

    int *roomName1;
    int *roomName2;
    int *roomName3;
    int *roomName4;
    int *roomName5;
    int *roomName6;

    GameButton *mJoinButton;
    GameButton *mCreateButton;
    bool mIsCreatingRoom;
    bool mIsJoiningRoom;
    // 115：192，111：194。自roomName1起的成员为我新增的成员，我Hook了构造函数调用方，为构造时分配了更多内存，因此可以为WaitForSecondPlayerDialog任意地新增成员。

    void GameButtonDown(ButtonCode theButton, unsigned int thePlayerIndex) {
        reinterpret_cast<void (*)(WaitForSecondPlayerDialog *, ButtonCode, unsigned int)>(WaitForSecondPlayerDialog_GameButtonDownAddr)(this, theButton, thePlayerIndex);
    }

    WaitForSecondPlayerDialog(LawnApp *theApp) {
        _constructor(theApp);
    }
    ~WaitForSecondPlayerDialog() {
        _destructor();
    }
    void Update();
    void Draw(Sexy::Graphics *g);
    void Resize(int theX, int theY, int theWidth, int theHeight);
    void CreateRoom();
    void JoinRoom();
    void UdpBroadcastRoom();
    bool CheckTcpAccept();
    void ScanUdpBroadcastRoom();
    void TryTcpConnect();
    void StopUdpBroadcastRoom();
    void InitUdpScanSocket();
    void CloseUdpScanSocket();
    void LeaveRoom();
    void ExitRoom();

protected:
    friend void InitHookFunction();

    void _constructor(LawnApp *theApp);
    void _destructor();

    void HandleTcpClientMessage(void *buf, ssize_t);
    void HandleTcpServerMessage(void *buf, ssize_t);
    bool GetActiveBroadcast(sockaddr_in &out_bcast, std::string *out_ifname);
    int ScoreIfname(const char *ifname);
};

void WaitForSecondPlayerDialog_ButtonDepress(Sexy::ButtonListener *listener, int id);

inline void (*old_WaitForSecondPlayerDialog_WaitForSecondPlayerDialog)(WaitForSecondPlayerDialog *a, LawnApp *theApp);

inline void (*old_WaitForSecondPlayerDialog_Draw)(WaitForSecondPlayerDialog *dialog, Sexy::Graphics *graphics);

inline void (*old_WaitForSecondPlayerDialog_ButtonDepress)(Sexy::ButtonListener *listener, int id);

inline void (*old_WaitForSecondPlayerDialog_Delete)(WaitForSecondPlayerDialog *dialog);

// 双方都需要
#define UDP_PORT 8888


// 主机端需要
inline int udpBroadcastSocket = -1;
inline int tcpListenSocket = -1;
inline int tcpClientSocket = -1;
inline int tcpPort = 0;
inline int lastBroadcastTime = 0;
inline struct sockaddr_in broadcast_addr;
inline std::string ifname;


// 客户端需要
#define MAX_SERVERS 3
#define UDP_TIMEOUT 3 // 超时时间为3秒
#define NAME_LENGTH 256

// 全局变量，用于保存发现的服务端IP和时间戳
typedef struct {
    char ip[INET_ADDRSTRLEN];
    int tcp_port;
    char name[NAME_LENGTH];
    time_t last_seen; // 记录最后一次收到广播的时间
} server_info;


enum EventType : unsigned char {
    EVENT_NULL,

    EVENT_START_GAME,

    EVENT_VSSETUPMENU_BUTTON_DEPRESS,
    EVENT_VSSETUPMENU_ENTER_STATE,
    EVENT_VSSETUPMENU_RANDOM_PICK,
    EVENT_VSSETUPMENU_MOVE_CONTROLLER,
    EVENT_VSSETUPMENU_SET_CONTROLLER,

    EVENT_SEEDCHOOSER_SELECT_SEED,

    EVENT_CLIENT_BOARD_TOUCH_DOWN,
    EVENT_CLIENT_BOARD_TOUCH_DRAG,
    EVENT_CLIENT_BOARD_TOUCH_UP,
    EVENT_BOARD_TOUCH_DOWN_REPLY,
    EVENT_BOARD_TOUCH_DRAG_REPLY,
    EVENT_BOARD_TOUCH_UP_REPLY,
    EVENT_SERVER_BOARD_TOUCH_DOWN,
    EVENT_SERVER_BOARD_TOUCH_DRAG,
    EVENT_SERVER_BOARD_TOUCH_UP,


    EVENT_CLIENT_BOARD_TOUCH_CLEAR_CURSOR,
    EVENT_SERVER_BOARD_TOUCH_CLEAR_CURSOR,

    EVENT_CLIENT_BOARD_GAMEPAD_SET_STATE,
    EVENT_SERVER_BOARD_GAMEPAD_SET_STATE,

    EVENT_CLIENT_BOARD_PAUSE,
    EVENT_SERVER_BOARD_PAUSE,

    EVENT_SERVER_BOARD_COIN_ADD,


    EVENT_SERVER_BOARD_GRIDITEM_LAUNCHCOUNTER,
    EVENT_SERVER_BOARD_GRIDITEM_ADDGRAVE,

    EVENT_SERVER_BOARD_PLANT_LAUNCHCOUNTER,
    EVENT_SERVER_BOARD_PLANT_ANIMATION, // 效果不佳，后续再想办法
    EVENT_SERVER_BOARD_PLANT_FIRE,
    EVENT_SERVER_BOARD_PLANT_ADD,
    EVENT_SERVER_BOARD_PLANT_DIE,


    EVENT_SERVER_BOARD_ZOMBIE_ADD,
    EVENT_SERVER_BOARD_ZOMBIE_RIZE_FORM_GRAVE,
    EVENT_SERVER_BOARD_ZOMBIE_PICK_SPEED,

    EVENT_SERVER_BOARD_TAKE_SUNMONEY,
    EVENT_SERVER_BOARD_TAKE_DEATHMONEY,

    EVENT_SERVER_BOARD_SEEDPACKET_WASPLANTED,
    EVENT_SERVER_BOARD_START_LEVEL,
};

struct TwoShort {
    short s1;
    short s2;
};

struct FourChar {
    unsigned char c1;
    unsigned char c2;
    unsigned char c3;
    unsigned char c4;
};

union IntFloatShortChar {
    int i;
    float f;
    TwoShort s;
    FourChar c;
};


class BaseEvent {
public:
    EventType type;
};

class SimpleEvent : public BaseEvent {
public:
    unsigned char data;
};

class SimpleShortEvent : public BaseEvent {
public:
    short data;
};

class TwoCharDataEvent : public BaseEvent {
public:
    unsigned char data1;
    unsigned char data2;
};

class TwoCharOneShortDataEvent : public BaseEvent {
public:
    unsigned char data1;
    unsigned char data2;
    short data3;
};


class TwoShortDataEvent : public BaseEvent {
public:
    short data1;
    short data2;
};

class TwoCharTwoShortDataEvent : public BaseEvent {
public:
    unsigned char data1;
    unsigned char data2;
    short data3;
    short data4;
};

class FourCharDataEvent : public BaseEvent {
public:
    unsigned char data1;
    unsigned char data2;
    unsigned char data3;
    unsigned char data4;
};

class FourShortDataEvent : public BaseEvent {
public:
    short data1;
    short data2;
    short data3;
    short data4;
};

class NineShortDataEvent : public BaseEvent {
public:
    short data[9];
};


class TenShortDataEvent : public BaseEvent {
public:
    short data[10];
};


class TwoShortTwoIntDataEvent : public BaseEvent {
public:
    short data1;
    short data2;
    IntFloatShortChar data3;
    IntFloatShortChar data4;
};

class FourCharOneShortTwoIntDataEvent : public BaseEvent {
public:
    unsigned char data1[4];
    short data2;
    IntFloatShortChar data3[2];
};


inline server_info servers[MAX_SERVERS];
inline int scanned_server_count = 0; // 已发现的服务端数量
inline int udpScanSocket = -1;


// 客户端TCP socket
inline int tcpServerSocket = -1;
inline bool tcp_connecting = false; // 正在尝试连接
inline bool tcp_connected = false;


#endif // PVZ_LAWN_WIDGET_WAIT_FOR_SECOND_PLAYER_DIALOG_H
