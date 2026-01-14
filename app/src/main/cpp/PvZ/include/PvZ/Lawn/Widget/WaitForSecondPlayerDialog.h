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

#include <cstddef>
#include <cstdint>

enum EventType : uint8_t {
    EVENT_NULL,

    EVENT_CLIENT_PING,
    EVENT_SERVER_PONG,

    EVENT_START_GAME,

    EVENT_SERVER_VSSETUPMENU_BUTTON_DEPRESS,
    EVENT_CLIENT_VSSETUPMENU_BUTTON_DEPRESS,
    EVENT_SERVER_VSSETUPMENU_PICKBACKGROUND,
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

    EVENT_SERVER_BOARD_GAMEPAD_PICKUP_SHOVEL,
    EVENT_SERVER_BOARD_GAMEPAD_USE_SHOVEL,

    EVENT_CLIENT_BOARD_PAUSE,
    EVENT_SERVER_BOARD_PAUSE,

    EVENT_CLIENT_BOARD_CONCEDE,
    EVENT_SERVER_BOARD_CONCEDE,

    EVENT_SERVER_BOARD_COIN_ADD,

    EVENT_SERVER_BOARD_GRIDITEM_DIE,
    EVENT_SERVER_BOARD_GRIDITEM_LAUNCHCOUNTER,
    EVENT_SERVER_BOARD_GRIDITEM_ADDGRAVE,

    EVENT_SERVER_BOARD_PLANT_LAUNCHCOUNTER,                // 同步生产植物如向日葵、阳光菇的生产发光
    EVENT_SERVER_BOARD_PLANT_SHOOTER_LAUNCH,               // 播放杨桃、三线射手的开火动画
    EVENT_SERVER_BOARD_PLANT_FINDTARGETANDFIRE,            // 播放其他植物的开火动画
    EVENT_SERVER_BOARD_PLANT_KERNELPLUT_FINDTARGETANDFIRE, // 黄油投手
    EVENT_SERVER_BOARD_PLANT_PINGPONG_ANIMATION,           // 似乎无用，先不同步
    EVENT_SERVER_BOARD_PLANT_OTHER_ANIMATION,              // 同步摇摆动画、开火动画的帧率和播放进度
    EVENT_SERVER_BOARD_PLANT_FIRE,                         // 射出子弹
    EVENT_SERVER_BOARD_PLANT_ADD,
    EVENT_SERVER_BOARD_PLANT_DIE,
    EVENT_SERVER_BOARD_PLANT_DO_SPECIAL, // 同步一次性植物的爆炸

    EVENT_SERVER_BOARD_ZOMBIE_DIE,
    EVENT_SERVER_BOARD_ZOMBIE_MIND_CONTROLLED,
    EVENT_SERVER_BOARD_ZOMBIE_ADD,          // AddZombieInRow触发的同步
    EVENT_SERVER_BOARD_ZOMBIE_BUNGEE_ADD,   // 蹦极僵尸在AddZombieInRow之后还会设置靶标位置，所以单独同步
    EVENT_SERVER_BOARD_ZOMBIE_ADD_BY_CHEAT, // 修改器放置僵尸会在执行AddZombieInRow后额外设置僵尸的位置，本事件就是追加同步僵尸位置
    EVENT_SERVER_BOARD_ZOMBIE_RIZE_FORM_GRAVE,
    EVENT_SERVER_BOARD_ZOMBIE_SUMMON_BACKUP_DANCERS,
    EVENT_SERVER_BOARD_ZOMBIE_PICK_SPEED,
    EVENT_SERVER_BOARD_ZOMBIE_ICE_TRAP,
    EVENT_SERVER_BOARD_ZOMBIE_POLEVAULTER_VAULT,      // 撑杆僵尸开始跳跃。暂时不同步此事件，因为同步后的撑杆落地会概率向前瞬移一段距离
    EVENT_SERVER_BOARD_ZOMBIE_GARGANTUAR_START_SMASH, // 开始播放砸地动画
    EVENT_SERVER_BOARD_ZOMBIE_GARGANTUAR_START_THROW, // 开始播放扔小鬼动画
    EVENT_SERVER_BOARD_ZOMBIE_IMP_THROW,
    EVENT_SERVER_BOARD_ZOMBIE_HUGE_WAVE,    // 同步"一大波僵尸"提示
    EVENT_SERVER_BOARD_ZOMBIE_YUCKY_SETROW, // 同步吃大蒜换行

    EVENT_SERVER_BOARD_LAWNMOWER_START,

    EVENT_SERVER_BOARD_TAKE_SUNMONEY,
    EVENT_SERVER_BOARD_TAKE_DEATHMONEY,

    EVENT_SERVER_BOARD_SEEDPACKET_WASPLANTED,
    EVENT_SERVER_BOARD_START_LEVEL,


    EVENT_CLIENT_VSRESULT_BUTTON_DEPRESS,
    EVENT_SERVER_VSRESULT_BUTTON_DEPRESS,
};

enum class UIMode {
    MODE1_INIT = 1,  // 初始
    MODE2_WIFI = 2,  // WIFI 联机
    MODE3_SERVER = 3 // 服务器联机
};

enum class InputPurpose {
    NONE = 0,
    LAN_JOIN_MANUAL,    // 模式2：加入指定IP房间
    HOST_SET_PORT,       // ✅ 模式2：设置房间端口
    SERVER_CONNECT_ADDR, // 模式3：连接服务器 IP:PORT


};
struct ServerRoomItem {
    int roomId;
    char name[128];
    bool full;
    bool gaming;
};
class WaitForSecondPlayerDialog : public __LawnDialog {
public:
    bool m2PJoined;
    // 115：192，111：194。自roomName1起的成员为我新增的成员，我Hook了构造函数调用方，为构造时分配了更多内存，因此可以为WaitForSecondPlayerDialog任意地新增成员。

    int *roomName1;
    int *roomName2;
    int *roomName3;
    int *roomName4;
    int *roomName5;
    int *roomName6;

    GameButton *mLeftButton;
    GameButton *mRightButton;
    bool mIsCreatingRoom;
    bool mIsJoiningRoom;

    int mSelectedServerIndex;

    // 手动直连 UI
    GameButton *mDirectConnectButton;

    // 手动直连目标
    bool mUseManualTarget;
    char mManualIp[INET_ADDRSTRLEN];
    int mManualPort;

    UIMode mUIMode;
    InputPurpose mInputPurpose;

    // 模式3：服务器连接状态（你后面会接入你给的Java协议）
    bool mServerConnected;

    // （可选）模式3：服务器房间列表选择
    int mSelectedRoomIndex_Server;

    int mServerSock;          // TCP socket to server
    bool mServerConnecting;   // non-blocking connect in progress
    bool mServerHosting;      // created a room
    bool mServerJoined;       // joined a room as guest
    bool mServerHostHasGuest; // server pushed guest joined/left
    int mServerHostedRoomId;  // created room id
    int mServerJoinedRoomId;  // joined room id (optional)
    int mServerLastQueryTick; // frame tick for auto query
    int mServerLastRecvTick;  // for debug/timeout if needed

    char mServerIp[INET_ADDRSTRLEN];
    int mServerPort;

    ServerRoomItem mServerRooms[255];
    int mServerRoomCount;

    // recv buffer for framed protocol
    uint8_t mSrvRecvBuf[8192];
    int mSrvRecvLen;

    // helper UI text
    pvzstl::string mServerStatusText;

    // MODE3 actions
    bool ServerConnectFromInput(); // consume gInputString
    void ServerDisconnect(const char *why);
    void ServerUpdateIO(); // read frames (nonblocking)
    void ServerSendQuery();
    void ServerSendCreate();
    void ServerSendJoinSelected();
    void ServerSendExitRoom();
    void ServerSendLeaveRoom();
    void ServerSendStart(); // host start (optional)


    bool ServerTryReadOneFrame(uint8_t &outType, uint8_t *outPayload, uint16_t &outLen);
    static uint32_t ReadBE32(const uint8_t *p);

    // MODE3 drawing + selection
    void DrawServerRoomList(Sexy::Graphics *g);
    void ServerSelectRoomByMouse(int x, int y);
    // 统一切模式
    void SetMode(UIMode mode);
    void RefreshButtons();

    // 弹输入框（可复用一个函数，用不同 title）
    void ShowTextInput(const char *title, const char *hint);

    void GameButtonDown(GamepadButton theButton, unsigned int thePlayerIndex) {
        reinterpret_cast<void (*)(WaitForSecondPlayerDialog *, GamepadButton, unsigned int)>(WaitForSecondPlayerDialog_GameButtonDownAddr)(this, theButton, thePlayerIndex);
    }

    WaitForSecondPlayerDialog(LawnApp *theApp) {
        _constructor(theApp);
    }
    ~WaitForSecondPlayerDialog() {
        _destructor();
    }

    static size_t getServerEventSize(EventType type);
    static size_t getClientEventSize(EventType type);

    void Update();
    void Draw(Sexy::Graphics *g);
    void Resize(int theX, int theY, int theWidth, int theHeight);
    void MouseDown(int x, int y, int count);
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
    bool ManualIpConnect();

    void processServerEvent(void *buf, ssize_t bufSize);
    void processClientEvent(void *buf, ssize_t bufSize);

protected:
    friend void InitHookFunction();

    void _constructor(LawnApp *theApp);
    void _destructor();

    bool GetActiveBroadcast(sockaddr_in &out_bcast, std::string *out_ifname);
    bool ServerSendU8(uint8_t b);
};

void WaitForSecondPlayerDialog_ButtonDepress(Sexy::ButtonListener *listener, int id);

inline void (*old_WaitForSecondPlayerDialog_WaitForSecondPlayerDialog)(WaitForSecondPlayerDialog *a, LawnApp *theApp);

inline void (*old_WaitForSecondPlayerDialog_Draw)(WaitForSecondPlayerDialog *dialog, Sexy::Graphics *graphics);

inline void (*old_WaitForSecondPlayerDialog_ButtonDepress)(Sexy::ButtonListener *listener, int id);

inline void (*old_WaitForSecondPlayerDialog_Delete)(WaitForSecondPlayerDialog *dialog);

// 双方都需要
constexpr int UDP_PORT = 8888;


// 主机端需要
inline int udpBroadcastSocket = -1;
inline int tcpListenSocket = -1;
inline int tcpClientSocket = -1;
inline int tcpPort = 0;
inline int lastBroadcastTime = 0;
inline sockaddr_in broadcast_addr;
inline std::string ifname;


// 客户端需要
constexpr int MAX_SERVERS = 3;
constexpr int UDP_TIMEOUT = 3; // 超时时间为3秒
constexpr int NAME_LENGTH = 256;

// 全局变量，用于保存发现的服务端IP和时间戳
struct server_info {
    char ip[INET_ADDRSTRLEN];
    int tcp_port;
    char name[NAME_LENGTH];
    time_t last_seen; // 记录最后一次收到广播的时间
};


union Buffer32Bit {
    struct {
        uint8_t u8_1;
        uint8_t u8_2;
        uint8_t u8_3;
        uint8_t u8_4;
    } u8x4;

    struct {
        uint16_t u16_1;
        uint16_t u16_2;
    } u16x2;

    struct {
        int16_t i16_1;
        int16_t i16_2;
    } i16x2;

    uint32_t u32;
    int32_t i32;
    float f32;
};

class BaseEvent {
public:
    EventType type;
    uint8_t size;
};

class U8_Event : public BaseEvent {
public:
    uint8_t data;
};

class U16_Event : public BaseEvent {
public:
    uint16_t data;
};

class U8U8_Event : public BaseEvent {
public:
    uint8_t data1;
    uint8_t data2;
};

class U8U8U16_Event : public BaseEvent {
public:
    uint8_t data1;
    uint8_t data2;
    uint16_t data3;
};

class U16U16_Event : public BaseEvent {
public:
    uint16_t data1;
    uint16_t data2;
};

class I16I16_Event : public BaseEvent {
public:
    int16_t data1;
    int16_t data2;
};

class U8U8U16U16_Event : public BaseEvent {
public:
    uint8_t data1;
    uint8_t data2;
    uint16_t data3;
    uint16_t data4;
};

class U8U8I16I16_Event : public BaseEvent {
public:
    uint8_t data1;
    uint8_t data2;
    int16_t data3;
    int16_t data4;
};

class U8U8U8U8_Event : public BaseEvent {
public:
    uint8_t data1;
    uint8_t data2;
    uint8_t data3;
    uint8_t data4;
};

class U16Buf32_Event : public BaseEvent {
public:
    uint16_t data1;
    Buffer32Bit data2;
};

class U16Buf32Buf32_Event : public BaseEvent {
public:
    uint16_t data1;
    Buffer32Bit data2;
    Buffer32Bit data3;
};

class U16x9_Event : public BaseEvent {
public:
    uint16_t data[9];
};


class U16x12_Event : public BaseEvent {
public:
    uint16_t data[12];
};


class U16U16U16Buf32Buf32_Event : public BaseEvent {
public:
    uint16_t data1;
    uint16_t data2;
    uint16_t data3;
    Buffer32Bit data4;
    Buffer32Bit data5;
};

class U8x4U16Buf32x2_Event : public BaseEvent {
public:
    uint8_t data1[4];
    uint16_t data2;
    Buffer32Bit data3[2];
};

class U16x4U16_Event : public BaseEvent {
public:
    uint16_t data1[4];
    uint16_t data2;
};


inline server_info servers[MAX_SERVERS];
inline int scanned_server_count = 0; // 已发现的服务端数量
inline int udpScanSocket = -1;


// 客户端TCP socket
inline int tcpServerSocket = -1;
inline bool tcp_connecting = false; // 正在尝试连接
inline bool tcp_connected = false;

static inline ssize_t sendWithSize(int socket, BaseEvent *event, size_t len, int flags) {

    //    如果后续出现了大于255byte的数据包，记得改这里
    //    assert(len <= 255);

    event->size = len;
    return send(socket, event, len, flags);
}

#endif // PVZ_LAWN_WIDGET_WAIT_FOR_SECOND_PLAYER_DIALOG_H
