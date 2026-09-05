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

#include <cstddef>
#include <cstdint>
#include <ctime>

#include "PvZ/STL/string.h"
#include <concepts>
#include <netinet/in.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

inline constexpr uint32_t NETPLAY_VERSION = 3196;

// Network events only transmit the lower 16 bits of DataArray ID; slot/index 0 is a valid object ID,
// so we cannot use the game's internal PLANTID_NULL / ZOMBIEID_NULL / GRIDITEMID_NULL (value 0) as network null.
// DataArray capacity is far less than UINT16_MAX, so 0xFFFF is safely reserved as protocol-specific null.
inline constexpr uint16_t NETPLAY_PLANT_ID_NULL = UINT16_MAX;
inline constexpr uint16_t NETPLAY_ZOMBIE_ID_NULL = UINT16_MAX;
inline constexpr uint16_t NETPLAY_GRIDITEM_ID_NULL = UINT16_MAX;

enum EventType : uint8_t {
    EVENT_NULL,

    EVENT_PING, // Both sides send PING
    EVENT_PONG, // Both sides reply PONG
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
    EVENT_SERVER_VSSETUPMENU_SYNC_VS_MODE,
    EVENT_VSSETUPMENU_ENTER_STATE,
    EVENT_VSSETUPMENU_RANDOM_PICK,
    EVENT_SERVER_VSSETUPMENU_MOVE_CONTROLLER,
    EVENT_CLIENT_VSSETUPMENU_MOVE_CONTROLLER,
    EVENT_CLIENT_VSSETUPMENU_REQUEST_SIDE,
    EVENT_SERVER_VSSETUPMENU_SET_SIDE,
    EVENT_SERVER_VSSETUP_ADDON_BUTTON_INIT,
    EVENT_SERVER_VSSETUP_GLOBALBP_SYNC,
    EVENT_SERVER_VSSETUP_ADDON_CHECKBOX_CHECKED,
    EVENT_CLIENT_VSSETUP_ADDON_CHECKBOX_CHECKED,
    EVENT_CLIENT_VSSETUP_SEND_NAME_STATE,
    EVENT_SERVER_ENCOUNTER_PICK,

    EVENT_SERVER_SEEDCHOOSER_SELECT_SEED,
    EVENT_CLIENT_SEEDCHOOSER_SELECT_SEED,
    EVENT_SERVER_SEEDCHOOSER_BAN_SEED,
    EVENT_CLIENT_SEEDCHOOSER_BAN_SEED,
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
    EVENT_SERVER_BOARD_GRIDITEM_ADDPOLE,
    EVENT_SERVER_BOARD_GRIDITEM_TAEGETZOMBIE_DIE,
    EVENT_SERVER_BOARD_GRIDITEM_TAKE_DAMAGE,

    EVENT_SERVER_BOARD_PLANT_LAUNCHCOUNTER,                // Sync production plants like Sunflower, Sun-shroom glow
    EVENT_SERVER_BOARD_PLANT_SHOOTER_LAUNCH,               // Play Starfruit, Threepeater firing animation
    EVENT_SERVER_BOARD_PLANT_FINDTARGETANDFIRE,            // Play other plants' firing animation
    EVENT_SERVER_BOARD_PLANT_KERNELPLUT_FINDTARGETANDFIRE, // Kernel-pult
    EVENT_SERVER_BOARD_PLANT_PINGPONG_ANIMATION,           // Seems unused, skip syncing
    EVENT_SERVER_BOARD_PLANT_OTHER_ANIMATION,              // Sync sway animation, firing animation frame rate and progress
    EVENT_SERVER_BOARD_PLANT_FIRE,                         // Shoot projectile
    EVENT_SERVER_BOARD_PLANT_ADD,
    EVENT_SERVER_BOARD_PLANT_DIE,
    EVENT_SERVER_BOARD_PLANT_DO_SPECIAL, // Sync plant special ability trigger
    EVENT_SERVER_BOARD_PLANT_ICE_A_ZOMBIE,
    EVENT_SERVER_BOARD_PLANT_CHOMPER_BIT,
    EVENT_SERVER_BOARD_PLANT_MAGNETSHROOM_ATTACK,
    EVENT_SERVER_BOARD_PLANT_MAGNETSHROOM_ATTACK_LADDER,
    EVENT_SERVER_BOARD_PLANT_SQUASH_STATE,
    EVENT_SERVER_BOARD_PLANT_ICEBERG_LETTUCE_LAUNCH,
    EVENT_SERVER_BOARD_PLANT_WIN, // Plant side wins by killing 3 targets; already synced via parent GridItemDie, no need to sync

    EVENT_SERVER_BOARD_ZOMBIE_DIE,
    EVENT_SERVER_BOARD_ZOMBIE_MIND_CONTROLLED,
    EVENT_SERVER_BOARD_ZOMBIE_CONVERT_TO_IMP,
    EVENT_SERVER_BOARD_ZOMBIE_ADD,                   // Sync triggered by AddZombieInRow
    EVENT_SERVER_BOARD_ZOMBIE_BOBSELD_ADD,           // Separately sync Bobsled team
    EVENT_SERVER_BOARD_ZOMBIE_DOGWALKER_ADD,         // Separately sync Dogwalker zombie and zombie dog
    EVENT_SERVER_BOARD_ZOMBIE_BUNGEE_SET_STEAL_GRID, // Bungee zombie sets target position after AddZombieInRow, sync separately
    EVENT_SERVER_BOARD_ZOMBIE_BUNGEE_LIFT_TARGET,
    EVENT_SERVER_BOARD_ZOMBIE_BUNGEE_DROP_ZOMBIE,
    EVENT_SERVER_BOARD_ZOMBIE_BUNGEE_HIT_UMBRELLA,
    EVENT_SERVER_BOARD_ZOMBIE_ADD_BY_CHEAT, // Cheat menu places zombie, then sets position after AddZombieInRow; this event syncs the extra position
    EVENT_SERVER_BOARD_ZOMBIE_RIZE_FORM_GRAVE,
    EVENT_SERVER_BOARD_ZOMBIE_SUMMON_BACKUP_DANCERS,
    EVENT_SERVER_BOARD_ZOMBIE_RAISE_DEAD,
    EVENT_SERVER_BOARD_ZOMBIE_PICK_SPEED,
    EVENT_SERVER_BOARD_ZOMBIE_ICE_TRAP,
    EVENT_SERVER_BOARD_ZOMBIE_POLEVAULTER_IN_VAULT,   // Pole vaulter starts jumping
    EVENT_SERVER_BOARD_ZOMBIE_POLEVAULTER_POST_VAULT, // Pole vaulter lands
    EVENT_SERVER_BOARD_ZOMBIE_GARGANTUAR_START_SMASH, // Start playing ground smash animation
    EVENT_SERVER_BOARD_ZOMBIE_GARGANTUAR_START_THROW, // Start playing imp throw animation
    EVENT_SERVER_BOARD_ZOMBIE_GIGA_GARGANTUAR_START_LIGHTNING,
    EVENT_SERVER_BOARD_ZOMBIE_CATAPLUT_LAUNCHIING,
    EVENT_SERVER_BOARD_ZOMBIE_CATAPLUT_FIRE,
    EVENT_SERVER_BOARD_ZOMBIE_LADDER_START_PLACING,
    EVENT_SERVER_BOARD_ZOMBIE_LADDER_PLACED,
    EVENT_SERVER_BOARD_ZOMBIE_START_EATING,
    EVENT_SERVER_BOARD_ZOMBIE_BOBSLED_PICK_SPEED,
    EVENT_SERVER_BOARD_ZOMBIE_IMP_THROWN,
    EVENT_SERVER_BOARD_ZOMBIE_IMP_KICKED,
    EVENT_SERVER_BOARD_ZOMBIE_IMP_POP,
    EVENT_SERVER_BOARD_ZOMBIE_HUGE_WAVE, // Sync "A huge wave of zombies" notification
    EVENT_SERVER_BOARD_ZOMBIE_SET_ROW,   // Sync zombie row change
    EVENT_SERVER_BOARD_ZOMBIE_PHASE_COUNTER,
    EVENT_SERVER_BOARD_ZOMBIE_DO_SPECIAL, // Sync zombie special ability trigger
    EVENT_SERVER_BOARD_ZOMBIE_EXPLORER_BURN_PLANT,
    EVENT_SERVER_BOARD_ZOMBIE_SQUISH_ALL_IN_SQUARE,
    EVENT_SERVER_BOARD_ZOMBIE_TAKE_DAMAGE,
    EVENT_SERVER_BOARD_ZOMBIE_DROP_HEAD,
    EVENT_SERVER_BOARD_ZOMBIE_WIN, // Zombie side wins by entering house
    EVENT_SERVER_BOARD_ZOMBIE_MOW_DOWN,
    EVENT_SERVER_BOARD_ZOMBIE_TELEPORTATION_SHOOT,
    EVENT_SERVER_BOARD_ZOMBIE_TELEPORT,
    EVENT_SERVER_BOARD_PLANT_TELEPORT,
    EVENT_SERVER_BOARD_ZOMBIE_SUN_BEAN_SUN, // Sync remaining droppable sun after zombie eats Sun Bean

    EVENT_SERVER_BOARD_LAWNMOWER_START,

    EVENT_SERVER_BOARD_PLAY_SOUND,    // Play sound effect
    EVENT_SERVER_BOARD_PLAY_SOUND_SR, // Sound sync for spectator/replay parsing only

    EVENT_SERVER_BOARD_TAKE_SUNMONEY,
    EVENT_SERVER_BOARD_TAKE_DEATHMONEY,

    EVENT_SERVER_BOARD_SEEDPACKET_WASPLANTED,
    EVENT_SERVER_BOARD_START_LEVEL,
    EVENT_SERVER_BOARD_SYNC_ID,

    EVENT_SERVER_BOARD_SHUFFLE_RANDOM_PICK,
    EVENT_SERVER_BOARD_SHUFFLE_RANDOM_PICK_NEXT,

    // Local replay only. Never send this event through netplay::PutEvent.
    EVENT_LOCAL_BOARD_ACTION,

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

struct U16U16I16I16_Event : BaseEvent {
    uint16_t data1;
    uint16_t data2;
    int16_t data3;
    int16_t data4;
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

struct VSSetupGlobalBpSyncEvent : BaseEvent {
    static constexpr int kMaxSeedsPerPlayer = 30; // Extra slot mode consumes 6 cards per game, BO5 max 30 cards
    int8_t mode;
    uint8_t count[2];
    uint8_t seeds[2][kMaxSeedsPerPlayer];
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

struct U16x4_Event : BaseEvent {
    uint16_t data[4];
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

struct U8x5U16UNI32x2_Event : BaseEvent {
    uint8_t data1[5];
    uint16_t data2;
    Union32Bit data3[2];
};

struct U16x5UNI32x5_Event : BaseEvent {
    uint16_t data1;
    Union32Bit data2;
    uint16_t data3[4];
    Union32Bit data4[4];
};

struct U16UNI32U8x16U16x15UNI32x15_Event : BaseEvent {
    uint16_t data1;
    Union32Bit data2;
    uint8_t data3[16];
    uint16_t data4[15];
    Union32Bit data5[15];
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

// Both sides need
inline constexpr int UDP_PORT = 8888;

// Server side needs
inline int gUdpBroadcastSocket = -1;
inline int gTcpListenSocket = -1;
inline int gTcpClientSocket = -1;
inline int gTcpPort = 0;
inline int gLastBroadcastTime = 0;
inline sockaddr_in gBroadcastAddr;
inline std::string gIfname;

// Generate six-digit player code from the last two octets of local IPv4 filtered by netplay module.
pvzstl::string GetLocalIpPlayerCode();

inline char gSecondPlayerName[32];
inline char gServerHostName[32];
inline char gReplayHostName[32];
inline char gReplayGuestName[32];
inline bool gMetricsHostSendNameAllowed = true;

// Client side needs
inline constexpr int MAX_SERVERS = 3;
inline constexpr int UDP_TIMEOUT = 3; // Timeout is 3 seconds
inline constexpr int NAME_LENGTH = 256;

// Global variables for storing discovered server IPs and timestamps
struct ServerInfo {
    char ip[INET_ADDRSTRLEN];
    int tcpPort;
    char name[NAME_LENGTH];
    time_t lastSeen; // Records the last time a broadcast was received
} inline gServers[MAX_SERVERS];

inline int gScannedServerCount = 0; // Number of discovered servers
inline int gUdpScanSocket = -1;

// Client TCP socket
inline int gTcpServerSocket = -1;
inline bool gTcpConnecting = false; // Attempting to connect
inline bool gTcpConnected = false;
inline std::string gMetricsServerIp;
inline int gMetricsServerPort = 0;
inline int gMetricsRoomId = 0;
inline int gMetricsVsBackground = -1;
inline int gMetricsBattleType = -1; // 9 quick, 10 custom, 11 random
inline bool gMetricsShuffleMode = false;
inline bool gMetricsExtraPacket = false;
inline bool gMetricsExtendedSeeds = false;
inline bool gMetricsBanMode = false;
inline bool gMetricsBalancePatch = false;
inline int gMetricsMowerLoss = 0;
inline int gMetricsTargetLoss = 0;
inline int gMetricsGraveLoss = 0;
inline int gMetricsSunflowerLoss = 0;
inline std::unordered_map<int, int> gMetricsPlantUseCount;
inline std::unordered_map<int, int> gMetricsZombieUseCount;

// TODO: Improve server connection detection
inline bool gIsConnectedToServer = false;
inline bool gIsServerModeNetplay = false;

inline bool IsOnlineModeActive() noexcept {
    return gTcpConnected || gTcpClientSocket >= 0;
    //    return gTcpConnecting || gTcpConnected || gTcpClientSocket >= 0 || gTcpServerSocket >= 0 || gTcpListenSocket >= 0;
}

inline bool IsOnlineServerModeActive() noexcept {
    //    return false; // For testing
    return IsOnlineModeActive() && gIsServerModeNetplay;
}

namespace netplay {
struct SettleEvent {
    int seq;
    char side;      // 'P' or 'Z'
    char eventType; // 'K' (PICK) or 'B' (BAN)
    int seedType;
};

namespace detail {
    void PutEventData(const std::byte *data, std::size_t n);
} // namespace detail

template <typename T>
    requires std::derived_from<std::remove_cvref_t<T>, BaseEvent>
void PutEvent(T &&event) {
    using EventType = std::remove_cvref_t<T>;
    static_assert(std::is_trivially_copyable_v<EventType>, "Event must be trivially copyable");
    static_assert(std::in_range<decltype(BaseEvent::size)>(sizeof(EventType)), "'BaseEvent::size' is too small");
    event.BaseEvent::size = static_cast<uint8_t>(sizeof(EventType));
    detail::PutEventData(reinterpret_cast<const std::byte *>(&event), sizeof(EventType));
}

bool FlushSendBuffer(int socket);
void ClearSendBuffer() noexcept;

std::size_t ParseEventSize(const std::byte *data);

/**
 * @param [out] dest Event write buffer (must be aligned)
 * @param [in] src Event read buffer
 *
 * @return dest cast to BaseEvent * result
 */
BaseEvent *GetEvent(std::byte *dest, const std::byte *src);

void MetricsSetEndpoint(const std::string &ip, int roomPort);
void MetricsSetRoomId(int roomId);
void MetricsResetSettlementEvents();
void MetricsRecordSeedEvent(bool zombieSide, bool banEvent, int seedType);
void MetricsSetBattleType(int battleType);
void MetricsSetShuffleMode(bool shuffleMode);
void MetricsSetAddonFlags(bool extraPacket, bool extendedSeeds, bool banMode, bool balancePatch);
void MetricsSetVsBackground(int background);
void MetricsRecordPlantUsed(int seedType);
void MetricsRecordZombieUsed(int zombieType);
void MetricsRecordMowerLoss();
void MetricsRecordTargetLoss();
void MetricsRecordGraveLoss();
void MetricsRecordSunflowerLoss();
bool MetricsSendSettlement(bool plantWin, int mainCounter);

} // namespace netplay

#endif // PVZ_NETPLAY_H
