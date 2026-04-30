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

#ifndef PVZ_NETPLAY_H
#define PVZ_NETPLAY_H

#include <netinet/in.h>

#include <cstddef>
#include <cstdint>

#include <concepts>
#include <string>
#include <utility>

inline constexpr uint32_t NETPLAY_VERSION = 3160;

enum EventType : uint8_t {
    EVENT_NULL,

    EVENT_CLIENT_PING,
    EVENT_SERVER_PONG,
    /************************************************************/
    EVENT_SERVER_WAITFORSECONDPALYER_VERSION_CHECK,

    EVENT_CLIENT_WAITFORSECONDPALYER_PLAYER_NAME,
    EVENT_SERVER_WAITFORSECONDPALYER_PLAYER_NAME,

    EVENT_WAITFORSECONDPALYER_START_GAME,
    NUM_EVENT_WAITFORSECONDPALYER,
    /************************************************************/
    EVENT_SERVER_CHALLENGESCREEN_SELECT_MODE,
    EVENT_SERVER_CHALLENGESCREEN_BUTTON_DEPRESS,
    EVENT_CLIENT_CHALLENGESCREEN_SELECT_MODE,
    NUM_EVENT_CHALLENGESCREEN,
    /************************************************************/
    EVENT_SERVER_VSSETUPMENU_BUTTON_DEPRESS,
    EVENT_CLIENT_VSSETUPMENU_BUTTON_DEPRESS,
    EVENT_SERVER_VSSETUPMENU_PICKBACKGROUND,
    EVENT_VSSETUPMENU_ENTER_STATE,
    EVENT_VSSETUPMENU_RANDOM_PICK,
    EVENT_VSSETUPMENU_MOVE_CONTROLLER,
    EVENT_VSSETUPMENU_SET_CONTROLLER,
    EVENT_SERVER_VSSETUP_ADDON_BUTTON_INIT,
    EVENT_SERVER_VSSETUP_ADDON_CHECKBOX_CHECKED,
    EVENT_CLIENT_VSSETUP_ADDON_CHECKBOX_CHECKED,
    EVENT_SERVER_ENCOUNTER_PICK,

    EVENT_SERVER_SEEDCHOOSER_SELECT_SEED,
    EVENT_CLIENT_SEEDCHOOSER_SELECT_SEED,
    EVENT_SERVER_SEEDCHOOSER_BUTTON_DEPRESS,
    EVENT_CLIENT_SEEDCHOOSER_BUTTON_DEPRESS,
    NUM_EVENT_VSSETUPMENU,
    /************************************************************/
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
    EVENT_SERVER_BOARD_GRIDITEM_SUMMONCOUNTER,
    EVENT_SERVER_BOARD_GRIDITEM_ADDGRAVE,
    EVENT_SERVER_BOARD_GRIDITEM_ADDLADDER,
    EVENT_SERVER_BOARD_GRIDITEM_ADDCRATER,
    EVENT_SERVER_BOARD_GRIDITEM_ADDMOUND,

    EVENT_SERVER_BOARD_PLANT_LAUNCHCOUNTER,                // 同步生产植物如向日葵、阳光菇的生产发光
    EVENT_SERVER_BOARD_PLANT_SHOOTER_LAUNCH,               // 播放杨桃、三线射手的开火动画
    EVENT_SERVER_BOARD_PLANT_FINDTARGETANDFIRE,            // 播放其他植物的开火动画
    EVENT_SERVER_BOARD_PLANT_KERNELPLUT_FINDTARGETANDFIRE, // 黄油投手
    EVENT_SERVER_BOARD_PLANT_PINGPONG_ANIMATION,           // 似乎无用，先不同步
    EVENT_SERVER_BOARD_PLANT_OTHER_ANIMATION,              // 同步摇摆动画、开火动画的帧率和播放进度
    EVENT_SERVER_BOARD_PLANT_FIRE,                         // 射出子弹
    EVENT_SERVER_BOARD_PLANT_ADD,
    EVENT_SERVER_BOARD_PLANT_DIE,
    EVENT_SERVER_BOARD_PLANT_EATEN,      // 播放植物被吃掉的音效
    EVENT_SERVER_BOARD_PLANT_DO_SPECIAL, // 同步植物触发特性
    EVENT_SERVER_BOARD_PLANT_CHOMPER_BIT,
    EVENT_SERVER_BOARD_PLANT_MAGNETSHROOM_ATTACK,
    EVENT_SERVER_BOARD_PLANT_MAGNETSHROOM_ATTACK_LADDER,
    EVENT_SERVER_BOARD_PLANT_WIN, // 植物方通过杀够3只靶子胜利，目前版本由于已同步上级GridItemDie，故不需要同步

    EVENT_SERVER_BOARD_ZOMBIE_DIE,
    EVENT_SERVER_BOARD_ZOMBIE_MIND_CONTROLLED,
    EVENT_SERVER_BOARD_ZOMBIE_ADD,          // AddZombieInRow触发的同步
    EVENT_SERVER_BOARD_ZOMBIE_BOBSELD_ADD,  // 单独同步雪橇小队
    EVENT_SERVER_BOARD_ZOMBIE_BUNGEE_STEAL, // 蹦极僵尸在AddZombieInRow之后还会设置靶标位置，所以单独同步
    EVENT_SERVER_BOARD_ZOMBIE_BUNGEE_DROP_ZOMBIE,
    EVENT_SERVER_BOARD_ZOMBIE_BUNGEE_HIT_UMBRELLA,
    EVENT_SERVER_BOARD_ZOMBIE_ADD_BY_CHEAT, // 修改器放置僵尸会在执行AddZombieInRow后额外设置僵尸的位置，本事件就是追加同步僵尸位置
    EVENT_SERVER_BOARD_ZOMBIE_RIZE_FORM_GRAVE,
    EVENT_SERVER_BOARD_ZOMBIE_SUMMON_BACKUP_DANCERS,
    EVENT_SERVER_BOARD_ZOMBIE_PICK_SPEED,
    EVENT_SERVER_BOARD_ZOMBIE_ICE_TRAP,
    EVENT_SERVER_BOARD_ZOMBIE_POLEVAULTER_IN_VAULT,   // 撑杆僵尸开始跳跃
    EVENT_SERVER_BOARD_ZOMBIE_POLEVAULTER_POST_VAULT, // 撑杆僵尸落地
    EVENT_SERVER_BOARD_ZOMBIE_GARGANTUAR_START_SMASH, // 开始播放砸地动画
    EVENT_SERVER_BOARD_ZOMBIE_GARGANTUAR_START_THROW, // 开始播放扔小鬼动画
    EVENT_SERVER_BOARD_ZOMBIE_CATAPLUT_LAUNCHIING,
    EVENT_SERVER_BOARD_ZOMBIE_CATAPLUT_FIRE,
    EVENT_SERVER_BOARD_ZOMBIE_LADDER_START_PLACING,
    EVENT_SERVER_BOARD_ZOMBIE_LADDER_PLACED,
    EVENT_SERVER_BOARD_ZOMBIE_IMP_THROW,
    EVENT_SERVER_BOARD_ZOMBIE_HUGE_WAVE,    // 同步"一大波僵尸"提示
    EVENT_SERVER_BOARD_ZOMBIE_YUCKY_SETROW, // 同步吃大蒜换行
    EVENT_SERVER_BOARD_ZOMBIE_PHASE_COUNTER,
    EVENT_SERVER_BOARD_ZOMBIE_DO_SPECIAL, // 同步僵尸触发特性
    EVENT_SERVER_BOARD_ZOMBIE_TAKE_DAMAGE,
    EVENT_SERVER_BOARD_ZOMBIE_WIN, // 僵尸方通过进家胜利

    EVENT_SERVER_BOARD_LAWNMOWER_START,

    EVENT_SERVER_BOARD_TAKE_SUNMONEY,
    EVENT_SERVER_BOARD_TAKE_DEATHMONEY,

    EVENT_SERVER_BOARD_SEEDPACKET_WASPLANTED,
    EVENT_SERVER_BOARD_START_LEVEL,
    EVENT_SERVER_BOARD_SYNC_ID,

    EVENT_SERVER_BOARD_SHUFFLE_RANDOM_PICK,
    EVENT_SERVER_BOARD_SHUFFLE_RANDOM_PICK_NEXT,

    NUM_EVENT_BOARD,
    /************************************************************/
    EVENT_CLIENT_VSRESULT_BUTTON_DEPRESS,
    EVENT_SERVER_VSRESULT_BUTTON_DEPRESS,
    NUM_EVENT_VSRESULT
};

struct BaseEvent {
    EventType type;
    uint8_t size;
};

union Union32Bit {
    struct {
        int8_t i8_1;
        int8_t i8_2;
        int8_t i8_3;
        int8_t i8_4;
    } i8x4;

    struct {
        uint8_t u8_1;
        uint8_t u8_2;
        uint8_t u8_3;
        uint8_t u8_4;
    } u8x4;

    struct {
        int16_t i16_1;
        int16_t i16_2;
    } i16x2;

    struct {
        uint16_t u16_1;
        uint16_t u16_2;
    } u16x2;

    uint32_t u32;
    int32_t i32;
    float f32;
};
static_assert(sizeof(Union32Bit) == 4);

struct U8_Event : BaseEvent {
    uint8_t data;
};

struct U16_Event : BaseEvent {
    uint16_t data;
};

struct I16_Event : BaseEvent {
    int16_t data;
};

struct U8U8_Event : BaseEvent {
    uint8_t data1;
    uint8_t data2;
};

struct U8x3_Event : BaseEvent {
    uint8_t data[3];
};

struct U8U8U16_Event : BaseEvent {
    uint8_t data1;
    uint8_t data2;
    uint16_t data3;
};

struct U16U16_Event : BaseEvent {
    uint16_t data1;
    uint16_t data2;
};

struct I16I16_Event : BaseEvent {
    int16_t data1;
    int16_t data2;
};

struct U8U8U16U16_Event : BaseEvent {
    uint8_t data1;
    uint8_t data2;
    uint16_t data3;
    uint16_t data4;
};

struct U8x3U16x3_Event : BaseEvent {
    uint8_t data1[3];
    uint16_t data2[3];
};

struct U8U8I16I16_Event : BaseEvent {
    uint8_t data1;
    uint8_t data2;
    int16_t data3;
    int16_t data4;
};

struct B1x8_Event : BaseEvent {
    uint8_t data1 : 1;
    uint8_t data2 : 1;
    uint8_t data3 : 1;
    uint8_t data4 : 1;
    uint8_t data5 : 1;
    uint8_t data6 : 1;
    uint8_t data7 : 1;
    uint8_t data8 : 1;
};

struct U16UNI32_Event : BaseEvent {
    uint16_t data1;
    Union32Bit data2;
};

struct U16UNI32UNI32_Event : BaseEvent {
    uint16_t data1;
    Union32Bit data2;
    Union32Bit data3;
};

struct U16U16U8_Event : BaseEvent {
    uint16_t data1;
    uint16_t data2;
    uint8_t data3;
};

struct U16x6_Event : BaseEvent {
    uint16_t data[6];
};

struct U16x9_Event : BaseEvent {
    uint16_t data[9];
};


struct U16x12_Event : BaseEvent {
    uint16_t data[12];
};


struct U16U16U16UNI32UNI32_Event : BaseEvent {
    uint16_t data1;
    uint16_t data2;
    uint16_t data3;
    Union32Bit data4;
    Union32Bit data5;
};

struct U8x4U16UNI32x2_Event : BaseEvent {
    uint8_t data1[4];
    uint16_t data2;
    Union32Bit data3[2];
};

struct U16x5UNI32x5_Event : BaseEvent {
    uint16_t data1;
    Union32Bit data2;
    uint16_t data3[4];
    Union32Bit data4[4];
};

struct CHARx32_Event : BaseEvent {
    char chars[32];
};

struct U8x2U16x4UNI32x8_Event : BaseEvent {
    uint8_t data1[2];
    uint16_t data2[4];
    Union32Bit data3[4];
    Union32Bit data4[4];
};

// 双方都需要
inline constexpr int UDP_PORT = 8888;

// 主机端需要
inline int gUdpBroadcastSocket = -1;
inline int gTcpListenSocket = -1;
inline int gTcpClientSocket = -1;
inline int gTcpPort = 0;
inline int gLastBroadcastTime = 0;
inline sockaddr_in gBroadcastAddr;
inline std::string gIfname;

inline char gSecondPlayerName[32];

// 客户端需要
inline constexpr int MAX_SERVERS = 3;
inline constexpr int UDP_TIMEOUT = 3; // 超时时间为3秒
inline constexpr int NAME_LENGTH = 256;

// 全局变量，用于保存发现的服务端IP和时间戳
struct ServerInfo {
    char ip[INET_ADDRSTRLEN];
    int tcpPort;
    char name[NAME_LENGTH];
    time_t lastSeen; // 记录最后一次收到广播的时间
} inline gServers[MAX_SERVERS];

inline int gScannedServerCount = 0; // 已发现的服务端数量
inline int gUdpScanSocket = -1;

// 客户端TCP socket
inline int gTcpServerSocket = -1;
inline bool gTcpConnecting = false; // 正在尝试连接
inline bool gTcpConnected = false;

namespace netplay {

namespace details {
    void PutEventData(const std::byte *src, std::size_t n);
} // namespace details

template <typename T>
    requires(!std::is_const_v<std::remove_reference_t<T>> &&    //
             !std::is_volatile_v<std::remove_reference_t<T>> && //
             std::derived_from<std::remove_reference_t<T>, BaseEvent>)
ssize_t PutEvent(T &&event) {
    static_assert(std::is_trivially_copyable_v<std::remove_reference_t<T>>, "Event must be trivially copyable");
    static_assert(std::in_range<decltype(event.BaseEvent::size)>(sizeof(T)), "BaseEvent::size too small");
    event.BaseEvent::size = sizeof(T);
    details::PutEventData(reinterpret_cast<std::byte *>(&event), sizeof(T));
    return sizeof(T);
}

bool FlushSendBuffer(int socket);
void ClearSendBuffer() noexcept;

} // namespace netplay

#endif // PVZ_NETPLAY_H
