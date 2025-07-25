#ifndef PVZ_MAGIC_ADDR_H
#define PVZ_MAGIC_ADDR_H

/**
 * @file 此文件记录着所有 v1.1.1 和 v1.1.5 中不同的内容.
 */

#include "Homura/Patcher.h"

#if PVZ_VERSION == 111
constexpr int SexyAppBaseSize = 553;
constexpr int SexyAppBasePartSize = 527;
constexpr int LAWNAPP_PLAYSAMPLE_OFFSET = 680;
constexpr uintptr_t BOARD_UPDATE_ADDR_RELATIVE = 0x1669E4;
constexpr uintptr_t ZOMBIE_ISTANGLEKELPTARGET_ADDR_RELATIVE = 0x1AA288;
constexpr uintptr_t ZOMBIE_ISTANGLEKELPTARGET2_ADDR_RELATIVE = 0x1ACCE8;
constexpr uintptr_t J_AUDIOWRITE_ADDR_RELATIVE = 0x524E28;
#elif PVZ_VERSION == 115
constexpr int SexyAppBaseSize = 552;
constexpr int SexyAppBasePartSize = 526;
constexpr int LAWNAPP_PLAYSAMPLE_OFFSET = 676;
constexpr uintptr_t BOARD_UPDATE_ADDR_RELATIVE = 0x164A88;
constexpr uintptr_t ZOMBIE_ISTANGLEKELPTARGET_ADDR_RELATIVE = 0x1AAD98;  // 符号重复，无法dlsym获取地址，只好常量封装
constexpr uintptr_t ZOMBIE_ISTANGLEKELPTARGET2_ADDR_RELATIVE = 0x1A8338; // 符号重复，无法dlsym获取地址，只好常量封装
constexpr uintptr_t J_AUDIOWRITE_ADDR_RELATIVE = 0x12E6C4;
constexpr uintptr_t AGVideoOpenOffset = 0x69394C;
constexpr uintptr_t AGVideoShowOffset = 0x69393C;
constexpr uintptr_t AGVideoEnableOffset = 0x693940;
constexpr uintptr_t AGVideoIsPlayingOffset = 0x693944;
constexpr uintptr_t AGVideoPlayOffset = 0x693950;
#endif // PVZ_VERSION

inline bool enableNewOptionsDialog = false;

namespace game_patches {
inline homura::Patcher whackAZombieNormalSpeed;     // 锤僵尸关卡的僵尸速度恢复为原速
inline homura::Patcher repairShopA;                 // 破解商店
inline homura::Patcher repairShopB;                 // 破解商店
inline homura::Patcher autoPickupSeedPacketDisable; // 禁止光标自动拾取植物卡片
} // namespace game_patches

inline void ApplyPatches() {
    static constexpr char libGameMain[] = "libGameMain.so";

#if PVZ_VERSION == 111
    game_patches::whackAZombieNormalSpeed = homura::Patcher::CreateWithStr(libGameMain, 0x183448, true, "4F F0 01 00");
    game_patches::repairShopA = homura::Patcher::CreateWithStr(libGameMain, 0x1C3B06, true, "05 E0");
    game_patches::repairShopB = homura::Patcher::CreateWithStr(libGameMain, 0x1C3C6C, true, "06 E0");
    game_patches::autoPickupSeedPacketDisable = homura::Patcher::CreateWithStr(libGameMain, 0x1C6068, true, "16");
#elif PVZ_VERSION == 115
    game_patches::whackAZombieNormalSpeed = homura::Patcher::CreateWithStr(libGameMain, 0x1814F0, true, "4F F0 01 00");
    game_patches::repairShopA = homura::Patcher::CreateWithStr(libGameMain, 0x1C1BB6, true, "05 E0");
    game_patches::repairShopB = homura::Patcher::CreateWithStr(libGameMain, 0x1C1D1C, true, "06 E0");
    game_patches::autoPickupSeedPacketDisable = homura::Patcher::CreateWithStr(libGameMain, 0x1C4114, true, "16");
#endif // PVZ_VERSION

    game_patches::whackAZombieNormalSpeed.Modify();
    game_patches::repairShopA.Modify();
    game_patches::repairShopB.Modify();

    // 解锁僵尸方隐藏卡片(同时解锁绘制和点选)
    // homura::Patcher::CreateWithStr(libGameMain, 0x2B63AE, true, "5B").Modify();
    // homura::Patcher::CreateWithStr(libGameMain, 0x14FC7E, true, "1E").Modify();
    // homura::Patcher::CreateWithStr(libGameMain, 0x14D376, true, "FF").Modify(); // 让僵尸选卡栏的第四行左对齐

    // 解锁植物方紫卡(仅解锁绘制，点选需要在Mouse相关函数处理)
    // homura::Patcher::CreateWithStr(libGameMain, 0x150576, true, "30").Modify();
}

#endif // PVZ_MAGIC_ADDR_H
