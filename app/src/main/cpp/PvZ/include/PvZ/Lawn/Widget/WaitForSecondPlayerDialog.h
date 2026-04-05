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

#ifndef PVZ_LAWN_WIDGET_WAIT_FOR_SECOND_PLAYER_DIALOG_H
#define PVZ_LAWN_WIDGET_WAIT_FOR_SECOND_PLAYER_DIALOG_H

#include "PvZ/Lawn/Widget/LawnDialog.h"
#include "PvZ/NetPlay.h"
#include "PvZ/SexyAppFramework/Misc/GamepadButtons.h"
#include "PvZ/Symbols.h"

enum class UIMode {
    MODE1_INIT = 1,  // 初始
    MODE2_WIFI = 2,  // WIFI 联机
    MODE3_SERVER = 3 // 服务器联机
};

enum class InputPurpose {
    NONE = 0,
    LAN_JOIN_MANUAL,     // 模式2：加入指定IP房间
    HOST_SET_PORT,       // 模式2：设置房间端口
    SERVER_CONNECT_ADDR, // 模式3：连接服务器 IP:PORT
};

class GameButton;

struct ServerRoomItem {
    int roomId;
    char name[128];
    bool full;
    bool gaming;
};

class WaitForSecondPlayerDialog : public LawnDialog {
public:
    enum {
        WaitForSecondPlayerDialog_Enter = 1000,
        WaitForSecondPlayerDialog_Back = 1001,
        WaitForSecondPlayerDialog_Left = 1002,
        WaitForSecondPlayerDialog_Right = 1003,
    };

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

    // MODE3 drawing + selection
    void DrawServerRoomList(Sexy::Graphics *g);
    void ServerSelectRoomByMouse(int x, int y);
    // 统一切模式
    void SetMode(UIMode mode);
    void RefreshButtons();

    // 弹输入框（可复用一个函数，用不同 title）
    void ShowTextInput(const char *title, const char *hint);

    void GameButtonDown(Sexy::GamepadButton theButton, unsigned int thePlayerIndex) {
        reinterpret_cast<void (*)(WaitForSecondPlayerDialog *, Sexy::GamepadButton, unsigned int)>(WaitForSecondPlayerDialog_GameButtonDownAddr)(this, theButton, thePlayerIndex);
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

    void MouseDown(int x, int y, int theClickCount);
    void ButtonDepress_Thunk(this Sexy::ButtonListener &self, int theId);

    void processServerEvent(void *buf, ssize_t bufSize);
    void processClientEvent(void *buf, ssize_t bufSize);

protected:
    friend void InitHookFunction();

    void _constructor(LawnApp *theApp);
    void _destructor();

    bool GetActiveBroadcast(sockaddr_in &out_bcast, std::string *out_ifname);
    bool ServerSendU8(uint8_t b);
};

inline void (*old_WaitForSecondPlayerDialog_WaitForSecondPlayerDialog)(WaitForSecondPlayerDialog *a, LawnApp *theApp);

inline void (*old_WaitForSecondPlayerDialog_Draw)(WaitForSecondPlayerDialog *dialog, Sexy::Graphics *graphics);

inline void (*old_WaitForSecondPlayerDialog_ButtonDepress)(Sexy::ButtonListener &listener, int id);

inline void (*old_WaitForSecondPlayerDialog_Delete)(WaitForSecondPlayerDialog *dialog);

#endif // PVZ_LAWN_WIDGET_WAIT_FOR_SECOND_PLAYER_DIALOG_H
