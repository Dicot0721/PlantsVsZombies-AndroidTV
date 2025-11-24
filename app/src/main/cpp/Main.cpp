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

#include "PvZ/GlobalVariable.h"
#include "PvZ/HookInit.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Lawn/GamepadControls.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/MagicAddr.h"
#include "PvZ/Symbols.h"

#include <jni.h>

#include <cmath>

#include <map>
#include <numbers>
#include <sstream>

static jstring StringToJString(JNIEnv *env, std::string_view sv) {
    return env->NewStringUTF(sv.data());
}

static std::string JStringToString(JNIEnv *env, jstring str) {
    const char *buffer = env->GetStringUTFChars(str, nullptr);
    std::string result(buffer);
    env->ReleaseStringUTFChars(str, buffer);
    return result;
}

/**
 * @brief Homura 模块的初始化函数.
 *
 * Java 层已指定模块加载顺序: 先 libGameMain.so, 后 libHomura.so.
 */
[[gnu::constructor]] static void lib_main() {
    // 获取符号地址
    GetFunctionAddr();

    // 部分安卓4设备无法获取到基址，暂不清楚原因, 但仅影响 Patch 而不影响Hook, 影响不大.
    gLibBaseOffset = ((Board_UpdateAddr != nullptr) && (uintptr_t(Board_UpdateAddr) > BOARD_UPDATE_ADDR_RELATIVE + 1)) ? (uintptr_t(Board_UpdateAddr) - BOARD_UPDATE_ADDR_RELATIVE - 1) : 0;

    // Hook
    CallHook();

    homura::Patcher::UpdateBaseAddrMap("libGameMain.so", gLibBaseOffset);
    ApplyPatches();
}

// jint JNI_OnLoad(JavaVM *vm, void *reserved) {
// return JNI_VERSION_1_6;
// }

extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeDisableShop(JNIEnv *env, jclass clazz) {
    disableShop = true;
}

extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeEnableManualCollect(JNIEnv *env, jclass clazz) {
    enableManualCollect = true;
}

extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeSetHeavyWeaponAngle(JNIEnv *env, jclass clazz, jint i) {
    double radian = i * std::numbers::pi / 180;
    angle1 = float(std::cos(radian));
    angle2 = float(std::sin(radian));
}

extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeEnableNewOptionsDialog(JNIEnv *env, jclass clazz) {
    enableNewOptionsDialog = true;
}

extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeGaoJiPause(JNIEnv *env, jclass clazz, jboolean enable) {
    if (isMainMenu) {
        return;
    }
    requestPause = enable;
    LawnApp *lawnApp = *gLawnApp_Addr;

    // if (lawnApp->mPlayerInfo != nullptr){
    // lawnApp->mPlayerInfo->mChallengeRecords[GameMode::ChallengeButteredPopcorn - 2] = 0;
    // lawnApp->mPlayerInfo->mChallengeRecords[GameMode::ChallengeHeavyWeapon - 2] = 0;
    // }
    if (enable) {
        lawnApp->PlaySample(*Sexy_SOUND_PAUSE_Addr);
    } else {
        lawnApp->PlaySample(*Sexy_SOUND_GRAVEBUTTON_Addr);
    }
}

extern "C" JNIEXPORT jboolean JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeIsGaoJiPaused(JNIEnv *env, jclass clazz) {
    return requestPause;
}

extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeHideCoverLayer(JNIEnv *env, jclass clazz) {
    hideCoverLayer = true;
}

extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeShowCoolDown(JNIEnv *env, jclass clazz) {
    showCoolDown = true;
}

extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeEnableNormalLevelMode(JNIEnv *env, jclass clazz) {
    normalLevel = true;
}

extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeEnableImitater(JNIEnv *env, jclass clazz) {
    imitater = true;
}

extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeSendSecondTouch(JNIEnv *env, jclass clazz, jint x, jint y, jint action) {
    if (tcp_connected || tcpClientSocket >= 0) {
        return;
    }
    Board *aBoard = (*gLawnApp_Addr)->mBoard;
    if (aBoard == nullptr) {
        return;
    }
    switch (action) {
        case 0:
            aBoard->MouseDownSecond(x, y, 0);
            break;
        case 1:
            aBoard->MouseDragSecond(x, y);
            break;
        case 2:
            aBoard->MouseUpSecond(x, y, 0);
            break;
        default:
            break;
    }
}

extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeEnableNewShovel(JNIEnv *env, jclass clazz) {
    useNewShovel = true;
}

extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeDisableTrashBinZombie(JNIEnv *env, jclass clazz) {
    gZombieTrashBinDef.mPickWeight = 0;
}

extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeShowHouse(JNIEnv *env, jclass clazz) {
    showHouse = true;
}

extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeUseNewCobCannon(JNIEnv *env, jclass clazz) {
    useNewCobCannon = true;
}

extern "C" JNIEXPORT void JNICALL Java_com_android_support_Preferences_Changes(JNIEnv *env, jclass clazz, jobject con, jint featNum, jstring featName, jint value, jboolean boolean, jstring str) {
    switch (featNum) {
        case 1:
            infiniteSun = boolean; // 无限阳光
            break;
        case 2:
            seedPacketFastCoolDown = boolean; // 卡片无冷却
            break;
        case 3:
            abilityFastCoolDown = boolean; // 技能无冷却
            break;
        case 4:
            mushroomsNoSleep = boolean; // 蘑菇免唤醒
            break;
        case 5:
            requestPause = boolean; // 高级暂停
            break;
        case 6:
            noFog = boolean; // 清除迷雾
            break;
        case 7:
            BanDropCoin = boolean; // 禁止掉落阳光金币
            break;
        case 8:
            speedUpMode = value; // 设置游戏倍速
            break;
        case 9:
            hypnoAllZombies = boolean; // 魅惑所有僵尸
            break;
        case 10:
            freezeAllZombies = boolean; // 冻结所有僵尸
            break;
        case 11:
            startAllMowers = boolean; // 启动所有小推车
            break;
        case 21:
            showPlantHealth = boolean; // 所有植物显血
            break;
        case 22:
            showNutGarlicSpikeHealth = boolean; // 损伤点类植物显血
            break;
        case 23:
            showZombieBodyHealth = boolean; // 本体显血
            break;
        case 24:
            showHelmAndShieldHealth = boolean; // 防具显血
            break;
        case 25:
            showGargantuarHealth = boolean; // 巨人显血
            break;
        case 26:
            drawDebugText = boolean; // 显示出怪信息
            break;
        case 27:
            drawDebugRects = boolean; // 绘制碰撞箱
            break;
        case 41:
            doCheatCodeDialog = boolean; // 秘籍指令
            break;
        case 42:
            FreePlantAt = boolean; // 自由种植
            break;
        case 43:
            transparentVase = boolean; // 罐子透视
            break;
        case 44:
            zombieBloated = boolean; // 普僵啃坚果必过敏
            break;
        case 45:
            ZombieCanNotWon = boolean; // 无视僵尸进家
            break;
        case 46:
            boardEdgeAdjust = value * 10; // 进家线后退
            break;
        case 47:
            zombieSetScale = value; // 进家线后退
            break;
        case 48:
            maidCheats = value; // 女仆秘籍
            break;
        case 61:
            ColdPeaCanPassFireWood = boolean; // 寒冰豌豆无视火炬
            break;
        case 62:
            projectilePierce = boolean; // 子弹帧伤
            break;
        case 63:
            bulletSpinnerChosenNum = value - 1; // 手动选择子弹类型
            break;
        case 64:
            randomBullet = boolean; // 随机子弹类型
            break;
        case 65:
            isOnlyPeaUseable = boolean; // 仅对普通豌豆生效
            break;
        case 66:
            isOnlyTouchFireWood = boolean; // 豌豆穿过火炬后转变
            break;
        case 67:
            banCobCannon = boolean; // Ban玉米炮弹
            break;
        case 68:
            banStar = boolean; // Ban星星子弹
            break;
        case 81:
            doCheatDialog = boolean; // 关卡跳转器
            break;
        case 82:
            passNowLevel = boolean; // 过了这关
            break;
        case 83:
            daveNoPickSeeds = boolean; // 取消必选卡片
            break;
        case 84:
            endlessLastStand = boolean; // 坚不可摧无尽
            break;
        case 85:
            targetWavesToJump = value; // 目标轮数
            break;
        case 86:
            requestJumpSurvivalStage = boolean; // 跳轮
            break;
        case 87:
            stopSpawning = boolean; // 暂停刷怪
            break;
        case 88:
            banMower = boolean; // 不出小推车
            break;
        case 101:
            PumpkinWithLadder = boolean; // 种下南瓜自动搭梯
            break;
        case 102:
            VSBackGround = value; // 更换场景
            break;
        case 103:
            theBuildPlantType = (SeedType)(value >= 49 ? value + 1 : value - 1); // 植物类型
            break;
        case 104:
            theBuildZombieType = (ZombieType)(value - 1); // 僵尸类型
            break;
        case 105:
            theBuildLadderX = value; // 横坐标
            theBuildPlantX = value;
            BuildZombieX = value;
            break;
        case 106:
            theBuildLadderY = value; // 纵坐标
            theBuildPlantY = value;
            BuildZombieY = value;
            break;
        case 107:
            isImitaterPlant = boolean; // 模仿者植物
            break;
        case 108:
            plantBuild = boolean; // 种植植物
            break;
        case 109:
            zombieBuild = boolean; // 放置僵尸
            break;
        case 110:
            ladderBuild = boolean; // 搭梯子(单格)
            break;
        case 111:
            recoverAllMowers = boolean; // 恢复所有小推车
            break;
        case 112:
            ClearAllPlant = boolean; // 清除所有植物
            break;
        case 113:
            clearAllMowers = boolean; // 清除所有小推车
            break;
        case 114:
            graveBuild = boolean; // 冒墓碑
            break;
        case 115:
            clearAllZombies = boolean; // 清除所有僵尸
            break;
        case 116:
            clearAllGraves = boolean; // 清除所有墓碑
            break;
        case 121:
            formationId = value - 1; // 选择白天泳池阵型
            break;
        case 122:
            layChoseFormation = boolean; // 布置选择阵型
            break;
            // case 123:
            // break; // 复制阵型代码
        case 124:
            customFormation = JStringToString(env, str); // 粘贴阵型代码
            break;
        case 125:
            layPastedFormation = boolean; // 布置粘贴阵型
            break;
        case 141:
            targetSeedBank = value + 1; // 目标卡槽
            break;
        case 142:
            choiceSeedPacketIndex = value; // 卡片位置
            break;
        case 143:
            if (value <= 48) {
                choiceSeedType = (SeedType)(value - 1); // [豌豆射手, 模仿者)
            } else if (value <= 52) {
                choiceSeedType = (SeedType)(value + 1); // [爆炸坚果, NUM_SEED_TYPES)
            } else if (value <= 76) {
                choiceSeedType = (SeedType)(value + 8); // [墓碑, 气球僵尸]
            }
            break; // 卡片类型
        case 144:
            isImitaterSeed = boolean; // 模仿者植物卡片
            break;
        case 145:
            setSeedPacket = boolean; // 更换卡片
        case 200:
            checkZombiesAllowed[ZombieType::ZOMBIE_NORMAL] = boolean; // 普通僵尸
            break;
        case 202:
            checkZombiesAllowed[ZombieType::ZOMBIE_TRAFFIC_CONE] = boolean; // 路障僵尸
            break;
        case 203:
            checkZombiesAllowed[ZombieType::ZOMBIE_POLEVAULTER] = boolean; // 撑杆僵尸
            break;
        case 204:
            checkZombiesAllowed[ZombieType::ZOMBIE_PAIL] = boolean; // 铁桶僵尸
            break;
        case 205:
            checkZombiesAllowed[ZombieType::ZOMBIE_NEWSPAPER] = boolean; // 报纸僵尸
            break;
        case 206:
            checkZombiesAllowed[ZombieType::ZOMBIE_DOOR] = boolean; // 铁网门僵尸
            break;
        case 207:
            checkZombiesAllowed[ZombieType::ZOMBIE_FOOTBALL] = boolean; // 橄榄球僵尸
            break;
        case 208:
            checkZombiesAllowed[ZombieType::ZOMBIE_DANCER] = boolean; // 舞者僵尸
            break;
        case 211:
            checkZombiesAllowed[ZombieType::ZOMBIE_SNORKEL] = boolean; // 潜水僵尸
            break;
        case 212:
            checkZombiesAllowed[ZombieType::ZOMBIE_ZAMBONI] = boolean; // 雪橇车僵尸
            break;
        case 214:
            checkZombiesAllowed[ZombieType::ZOMBIE_DOLPHIN_RIDER] = boolean; // 海豚骑士僵尸
            break;
        case 215:
            checkZombiesAllowed[ZombieType::ZOMBIE_JACK_IN_THE_BOX] = boolean; // 小丑僵尸
            break;
        case 216:
            checkZombiesAllowed[ZombieType::ZOMBIE_BALLOON] = boolean; // 气球僵尸
            break;
        case 217:
            checkZombiesAllowed[ZombieType::ZOMBIE_DIGGER] = boolean; // 矿工僵尸
            break;
        case 218:
            checkZombiesAllowed[ZombieType::ZOMBIE_POGO] = boolean; // 蹦蹦僵尸
            break;
        case 219:
            checkZombiesAllowed[ZombieType::ZOMBIE_YETI] = boolean; // 僵尸雪人
            break;
        case 220:
            checkZombiesAllowed[ZombieType::ZOMBIE_BUNGEE] = boolean; // 飞贼僵尸
            break;
        case 221:
            checkZombiesAllowed[ZombieType::ZOMBIE_LADDER] = boolean; // 梯子僵尸
            break;
        case 222:
            checkZombiesAllowed[ZombieType::ZOMBIE_CATAPULT] = boolean; // 投石车僵尸
            break;
        case 223:
            checkZombiesAllowed[ZombieType::ZOMBIE_GARGANTUAR] = boolean; // 白眼巨人僵尸
            break;
        case 226:
            checkZombiesAllowed[ZombieType::ZOMBIE_TRASHCAN] = boolean; // 垃圾桶僵尸
            break;
        case 227:
            checkZombiesAllowed[ZombieType::ZOMBIE_PEA_HEAD] = boolean; // 豌豆射手僵尸
            break;
        case 228:
            checkZombiesAllowed[ZombieType::ZOMBIE_WALLNUT_HEAD] = boolean; // 坚果僵尸
            break;
        case 229:
            checkZombiesAllowed[ZombieType::ZOMBIE_JALAPENO_HEAD] = boolean; // 火爆辣椒僵尸
            break;
        case 230:
            checkZombiesAllowed[ZombieType::ZOMBIE_GATLING_HEAD] = boolean; // 机枪射手僵尸
            break;
        case 231:
            checkZombiesAllowed[ZombieType::ZOMBIE_SQUASH_HEAD] = boolean; // 窝瓜僵尸
            break;
        case 232:
            checkZombiesAllowed[ZombieType::ZOMBIE_TALLNUT_HEAD] = boolean; // 高坚果僵尸
            break;
        case 233:
            checkZombiesAllowed[ZombieType::ZOMBIE_REDEYE_GARGANTUAR] = boolean; // 红眼巨人僵尸
            break;
        case 234:
            choiceSpawnMode = value; // 刷怪模式
            break;
        case 236:
            buttonSetSpawn = boolean; // 设置出怪
            break;
        default:
            break;
    }
}

extern "C" JNIEXPORT jobjectArray JNICALL Java_com_android_support_CkHomuraMenu_GetFeatureList(JNIEnv *env, jobject thiz) {
    static constexpr const char *features[] = {
        "Collapse_常用功能",
        "1_CollapseAdd_Toggle_无限阳光",
        "42_CollapseAdd_Toggle_自由种植",
        "2_CollapseAdd_Toggle_卡片无冷却",
        "3_CollapseAdd_Toggle_技能无冷却",
        "4_CollapseAdd_Toggle_蘑菇免唤醒",
        "5_CollapseAdd_Toggle_高级暂停",
        "8_CollapseAdd_Spinner_<font color='green'>设置游戏倍速：_关闭,1.2倍,1.5倍,2倍,2.5倍,3倍,5倍,10倍",
        "82_CollapseAdd_OnceCheckBox_直接过关",


        "Collapse_调试功能",
        "CollapseAdd_RichTextView_<font color='green'>显示植物血量:",
        "21_CollapseAdd_Toggle_所有植物显血",
        "22_CollapseAdd_Toggle_损伤点类植物显血",
        "CollapseAdd_RichTextView_<font color='green'>显示僵尸血量:",
        "23_CollapseAdd_Toggle_本体显血",
        "25_CollapseAdd_Toggle_巨人显血",
        "24_CollapseAdd_Toggle_防具显血",
        "CollapseAdd_RichTextView_<font color='green'>显示游戏信息:",
        "26_CollapseAdd_Toggle_显示出怪信息",
        "27_CollapseAdd_Toggle_绘制碰撞箱",


        "Collapse_娱乐功能",
        "6_CollapseAdd_Toggle_清除迷雾",
        "43_CollapseAdd_Toggle_罐子透视",
        "45_CollapseAdd_Toggle_无视僵尸进家",
        "44_CollapseAdd_Toggle_普僵啃坚果必过敏",
        "7_CollapseAdd_Toggle_禁止掉落阳光金币",
        "48_CollapseAdd_Spinner_<font color='green'>女仆秘籍：_关闭,保持前进(舞王一直前进),跳舞(舞王不前进且不会召唤舞伴),召唤舞伴(舞王不前进且一直尝试召唤舞伴)",
        "46_CollapseAdd_Spinner_<font color='green'>进家线后退：_关闭,10,20,30,40,50,60,70,80",
        "47_CollapseAdd_Spinner_<font color='green'>修改僵尸大小：_关闭,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0",
        "CollapseAdd_RichTextView_<font color='yellow'>#血量与大小呈平方关系!",
        "41_CollapseAdd_OnceCheckBox_智慧树指令",


        "Collapse_关卡设置",
        "87_CollapseAdd_Toggle_暂停刷怪",
        "88_CollapseAdd_Toggle_不出小推车",
        "83_CollapseAdd_Toggle_取消必选卡片",
        "84_CollapseAdd_Toggle_坚不可摧无尽",
        "81_CollapseAdd_OnceCheckBox_跳关器",
        "102_CollapseAdd_Spinner_<font color='green'>更换场景：_关闭,白天,黑夜,泳池,雾夜,屋顶,月夜,温室园,蘑菇园,水族馆",
        "CollapseAdd_RichTextView_<font color='green'>无尽模式跳轮:",
        "85_CollapseAdd_InputValue_目标轮数",
        "86_CollapseAdd_OnceCheckBox_跳轮",
        "CollapseAdd_RichTextView_<font color='yellow'>#完成选卡后才能跳轮!",


        "Collapse_卡槽设置",
        "141_CollapseAdd_Spinner_<font color='green'>修改卡槽：_卡槽1,卡槽2",
        "142_CollapseAdd_Spinner_<font color='green'>卡片位置：_第1格,第2格,第3格,第4格,第5格,第6格,第7格,第8格,第9格,第10格",
        // "143_CollapseAdd_Spinner_<font
        // color='green'>全部卡片类型：_关闭,豌豆射手,向日葵,樱桃炸弹,坚果,土豆地雷,寒冰射手,大嘴花,双重射手,小喷菇,阳光菇,大喷菇,咬咬碑,迷惑菇,胆小菇,冰川菇,末日菇,莲叶,窝瓜,三重射手,缠绕水草,火爆辣椒,地刺,火炬树桩,高坚果,水兵菇,路灯花,仙人掌,三叶草,双向射手,星星果,南瓜头,磁力菇,卷心菜投手,花盆,玉米投手,咖啡豆,大蒜,叶子保护伞,金盏花,西瓜投手,机枪射手,双胞向日葵,多嘴小蘑菇,猫尾草,冰西瓜,吸金磁,钢地刺,玉米加农炮,变身茄子,无(可选植物种类总数),爆炸坚果,巨型坚果,幼苗,反向双重射手,无(植物种类总数),刷新阵型(僵尸迷阵),消除炸弹坑(僵尸迷阵),阳光(拉霸),钻石(拉霸),潜水僵尸(僵尸水族馆),奖杯(僵尸水族馆),墓碑,普通僵尸,垃圾桶僵尸,路障僵尸,撑杆僵尸,铁桶僵尸,旗帜僵尸,报纸僵尸,铁网门僵尸,橄榄球僵尸,舞者僵尸,雪橇车僵尸,玩偶匣僵尸,矿工僵尸,蹦蹦僵尸,蹦极僵尸,梯子僵尸,投石车僵尸,巨人僵尸,无,鸭子救生圈僵尸,潜水僵尸,海豚骑士僵尸,小鬼僵尸,气球僵尸",
        "143_CollapseAdd_Spinner_<font "
        "color='green'>卡片类型：_未选择,豌豆射手,向日葵,樱桃炸弹,坚果,土豆地雷,寒冰射手,大嘴花,双重射手,小喷菇,阳光菇,大喷菇,咬咬碑,迷惑菇,胆小菇,冰川菇,末日菇,莲叶,窝瓜,三重射手,缠绕水草,火爆辣椒,"
        "地刺,火炬树桩,高坚果,水兵菇,路灯花,仙人掌,三叶草,双向射手,星星果,南瓜头,磁力菇,卷心菜投手,花盆,玉米投手,咖啡豆,大蒜,叶子保护伞,金盏花,西瓜投手,机枪射手,双胞向日葵,多嘴小蘑菇,猫尾草,冰西瓜,"
        "吸金磁,钢地刺,玉米加农炮,爆炸坚果,巨型坚果,幼苗,反向双重射手,墓碑,普通僵尸,垃圾桶僵尸,路障僵尸,撑杆僵尸,铁桶僵尸,旗帜僵尸,报纸僵尸,铁网门僵尸,橄榄球僵尸,舞者僵尸,雪橇车僵尸,玩偶匣僵尸,"
        "矿工僵尸,蹦蹦僵尸,蹦极僵尸,梯子僵尸,投石车僵尸,巨人僵尸,红眼巨人僵尸,鸭子救生圈僵尸,潜水僵尸,海豚骑士僵尸,小鬼僵尸,气球僵尸",
        "144_CollapseAdd_CheckBox_模仿者植物卡片",
        "145_CollapseAdd_OnceCheckBox_更换卡片",


        "Collapse_子弹设置",
        "62_CollapseAdd_Toggle_子弹帧伤",
        "61_CollapseAdd_Toggle_寒冰豌豆无视火炬",
        "63_CollapseAdd_Spinner_<font color='green'>手动选择子弹类型：_关闭,豌豆,寒冰豌豆,卷心菜,西瓜,蘑菇孢子,冰西瓜,火焰豌豆,星星,尖刺,篮球,玉米粒,玉米炮弹,黄油,自残子弹",
        "64_CollapseAdd_Toggle_随机子弹类型",
        "65_CollapseAdd_Toggle_仅对普通豌豆生效",
        "66_CollapseAdd_Toggle_豌豆穿过火炬后转变",
        "67_CollapseAdd_CheckBox_Ban玉米炮弹",
        "68_CollapseAdd_CheckBox_Ban星星子弹",


        "Collapse_出怪设置",
        "CollapseAdd_RichTextView_<font color='green'>请选择僵尸种类:",
        "200_CollapseAdd_CheckBox_普通僵尸(自然出怪必出)",
        "202_CollapseAdd_CheckBox_路障僵尸",
        "203_CollapseAdd_CheckBox_撑杆僵尸",
        "204_CollapseAdd_CheckBox_铁桶僵尸",
        "205_CollapseAdd_CheckBox_报纸僵尸",
        "206_CollapseAdd_CheckBox_铁网门僵尸",
        "207_CollapseAdd_CheckBox_橄榄球僵尸",
        "208_CollapseAdd_CheckBox_舞者僵尸",
        "211_CollapseAdd_CheckBox_潜水僵尸",
        "212_CollapseAdd_CheckBox_雪橇车僵尸",
        "214_CollapseAdd_CheckBox_海豚骑士僵尸",
        "215_CollapseAdd_CheckBox_玩偶匣僵尸",
        "216_CollapseAdd_CheckBox_气球僵尸",
        "217_CollapseAdd_CheckBox_矿工僵尸",
        "218_CollapseAdd_CheckBox_蹦蹦僵尸",
        "219_CollapseAdd_CheckBox_僵尸雪人",
        "220_CollapseAdd_CheckBox_蹦极僵尸",
        "221_CollapseAdd_CheckBox_梯子僵尸",
        "222_CollapseAdd_CheckBox_投石车僵尸",
        "223_CollapseAdd_CheckBox_白眼巨人僵尸",
        "233_CollapseAdd_CheckBox_红眼巨人僵尸",
        "226_CollapseAdd_CheckBox_垃圾桶僵尸",
        "227_CollapseAdd_CheckBox_豌豆射手僵尸",
        "228_CollapseAdd_CheckBox_坚果僵尸",
        "229_CollapseAdd_CheckBox_火爆辣椒僵尸",
        "230_CollapseAdd_CheckBox_机枪射手僵尸",
        "231_CollapseAdd_CheckBox_窝瓜僵尸",
        "232_CollapseAdd_CheckBox_高坚果僵尸",
        "234_CollapseAdd_Spinner_<font color='green'>请选择刷怪模式：_关闭,自然出怪(由游戏生成出怪列表),极限出怪(均匀填充出怪列表)",
        "236_CollapseAdd_OnceCheckBox_设置出怪",


        "Collapse_场地布置",
        "101_CollapseAdd_Toggle_种下南瓜自动搭梯",
        "105_CollapseAdd_Spinner_<font color='yellow'>放置横坐标：_第1列,第2列,第3列,第4列,第5列,第6列,第7列,第8列,第9列,所有列,僵尸出生点",
        "106_CollapseAdd_Spinner_<font color='yellow'>放置纵坐标：_第1行,第2行,第3行,第4行,第5行,第6行,所有行",
        "103_CollapseAdd_Spinner_<font color='green'>植物类型：_"
        "未选择,豌豆射手,向日葵,樱桃炸弹,坚果,土豆地雷,寒冰射手,大嘴花,双重射手,"
        "小喷菇,阳光菇,大喷菇,咬咬碑,迷惑菇,胆小菇,冰川菇,末日菇,"
        "莲叶,窝瓜,三重射手,缠绕水草,火爆辣椒,"
        "地刺,火炬树桩,高坚果,水兵菇,路灯花,仙人掌,三叶草,双向射手,星星果,南瓜头,磁力菇,"
        "卷心菜投手,花盆,玉米投手,咖啡豆,大蒜,叶子保护伞,金盏花,西瓜投手,"
        "机枪射手,双胞向日葵,多嘴小蘑菇,猫尾草,冰西瓜,吸金磁,钢地刺,玉米加农炮,"
        "爆炸坚果,巨型坚果,幼苗,反向双重射手",
        "107_CollapseAdd_CheckBox_模仿者植物",
        "108_CollapseAdd_OnceCheckBox_种植植物",
        "104_CollapseAdd_Spinner_<font "
        "color='green'>僵尸类型：_"
        "未选择,普通僵尸,旗帜僵尸,路障僵尸,撑杆僵尸,铁桶僵尸,报纸僵尸,铁网门僵尸,橄榄球僵尸,"
        "舞者僵尸,伴舞僵尸,鸭子救生圈僵尸,潜水僵尸,雪橇车僵尸,雪橇车僵尸小队,海豚骑士僵尸,玩偶匣僵尸,气球僵尸,"
        "矿工僵尸,蹦蹦僵尸,僵尸雪人,蹦极僵尸,梯子僵尸,投石车僵尸,白眼巨人僵尸,小鬼僵尸,僵王博士,"
        "垃圾桶僵尸,豌豆射手僵尸,坚果僵尸,火爆辣椒僵尸,机枪射手僵尸,窝瓜僵尸,高坚果僵尸,红眼巨人僵尸",
        "109_CollapseAdd_OnceCheckBox_放置僵尸",
        "CollapseAdd_RichTextView_<font color='green'>其他类型:",
        "114_CollapseAdd_OnceCheckBox_放置墓碑",
        "110_CollapseAdd_OnceCheckBox_放置梯子",
        "111_CollapseAdd_OnceCheckBox_恢复所有小推车",
        "CollapseAdd_RichTextView_<font color='yellow'>清除:",
        "112_CollapseAdd_OnceCheckBox_清除所有植物",
        "115_CollapseAdd_OnceCheckBox_清除所有僵尸",
        "116_CollapseAdd_OnceCheckBox_清除所有墓碑",
        "113_CollapseAdd_OnceCheckBox_清除所有小推车",
        "CollapseAdd_RichTextView_<font color='yellow'>杂项:",
        "9_CollapseAdd_OnceCheckBox_魅惑所有僵尸",
        "10_CollapseAdd_OnceCheckBox_冻结所有僵尸",
        "11_CollapseAdd_OnceCheckBox_启动所有小推车",


        "Collapse_一键布阵",
        "121_CollapseAdd_Spinner_<font "
        "color='green'>选择泳池无尽阵型：_未选择,"
        "[0]电波钟无炮,[1]最简无炮,[2]伪无伤无炮,[3]自然控丑无炮,[4]火焰无炮,[5]分裂火焰无炮,[6]后退无炮,[7]超前置无炮,[8]王子无炮,[9]机械钟无炮,"
        "[10]神之无炮,[11]石英钟无炮,[12]靠天无炮,[13]方块无神无炮,[14]56加速无神无炮,[15]压制一炮,[16]小二炮,[17]火焰二炮,[18]核武二炮,[19]分裂二炮,"
        "[20]方正二炮,[21]经典二炮,[22]冲关三炮,[23]太极四炮,[24]全金属四炮,[25]方块四炮,[26]青四炮,[27]水路无植物四炮,[28]方四炮,[29]神之四炮,"
        "[30]双核底线四炮,[31]经典四炮,[32]火焰四炮,[33]底线四炮,[34]传统四炮,[35]半场无植物五炮,[36]散炸五炮,[37]心五炮,[38]陆路无植物六炮,[39]水路无植物六炮,"
        "[40]青苔六炮,[41]禅房花木深,[42]神之六炮,[43]玉米六炮,[44]空炸六炮,[45]超后置六炮,[46]方六炮,[47]蝶韵,[48]一勺汤圆,[49]间隔无植物七炮,"
        "[50]玉兔茕茕,[51]无保护八炮,[52]树八炮,[53]全对称树八炮,[54]矩形八炮,[55]神之八炮,[56]阴阳八炮,[57]浮萍八炮,[58]后置八炮,[59]饲养海豚,"
        "[60]玉米八炮,[61]经典八炮,[62]花海八炮,[63]C2八炮,[64]分离八炮,[65]全对称八炮,[66]3C八炮,[67]灯台八炮,[68]⑨炮,[69]方块九炮,"
        "[70]C6i九炮,[71]心九炮,[72]轮炸九炮,[73]②炮,[74]六芒星十炮,[75]六边形十炮,[76]方块十炮,[77]斜方十炮,[78]简化十炮,[79]后置十炮,"
        "[80]经典十炮,[81]六线囚尸,[82]斜十炮,[83]魔方十炮,[84]戴夫的小汉堡,[85]鸡尾酒,[86]一勺汤圆十二炮,[87]玉壶春十二炮,[88]半场十二炮,[89]简化十二炮,"
        "[90]经典十二炮,[91]火焰十二炮,[92]冰雨十二炮·改,[93]冰雨十二炮•改改,[94]单紫卡十二炮,[95]神柱十二炮,[96]神之十二炮,[97]水路无植物十二炮,[98]纯白悬空十二炮,[99]后花园十二炮,"
        "[100]玉米十二炮,[101]两路暴狂,[102]九列十二炮,[103]梯曾十二炮,[104]君海十二炮,[105]箜篌引,[106]梅花十三,[107]最后之作,[108]冰心灯,[109]太极十四炮,"
        "[110]真·四炮,[111]神棍十四炮,[112]神之十四炮,[113]穿越十四炮,[114]钻石十五炮,[115]神之十五炮,[116]真·二炮,[117]冰箱灯,[118]炮环十二花,[119]单冰十六炮,"
        "[120]对称十六炮,[121]神之十六炮,[122]裸奔十六炮,[123]双冰十六炮,[124]超前置十六炮,[125]火焰十六炮,[126]经典十六炮,[127]折线十六炮,[128]肺十八炮(极限),[129]纯十八炮,"
        "[130]真·十八炮,[131]冰魄十八炮,[132]尾炸十八炮,[133]经典十八炮,[134]纯二十炮,[135]空炸二十炮,[136]钉耙二十炮,[137]新二十炮,[138]无冰瓜二十炮,[139]绝望之路,"
        "[140]二十一炮,[141]新二十二炮,[142]二十二炮,[143]无冰瓜二十二炮,[144]九列二十二炮,[145]二十四炮,[146]垫材二十四炮,[147]胆守(极限)",
        "122_CollapseAdd_OnceCheckBox_布置已选阵型",
        "CollapseAdd_RichTextView_<font color='green'>阵型导出/导入:",
        "123_CollapseAdd_FormationCopy_复制阵型代码",
        "124_CollapseAdd_InputText_粘贴阵型代码",
        "125_CollapseAdd_OnceCheckBox_布置粘贴阵型",
        "CollapseAdd_RichTextView_<font color='yellow'>#可以暂停游戏后进行布阵",


    };

    int featuresCount = std::size(features);
    jobjectArray ret = env->NewObjectArray(featuresCount, env->FindClass("java/lang/String"), nullptr);
    for (int i = 0; i < featuresCount; ++i) {
        env->SetObjectArrayElement(ret, i, StringToJString(env, features[i]));
    }
    return ret;
}

extern "C" JNIEXPORT jobjectArray JNICALL Java_com_android_support_CkHomuraMenu_SettingsList(JNIEnv *env, jobject thiz) {
    static constexpr const char *features[] = {
        "Category_菜单设置",
        "-1_Toggle_退出时保存设置", //-1 is checked on Preferences.java
        "Category_更多",
        "-6_Button_<font color='green'>返回菜单</font>",
    };

    int featuresCount = std::size(features);
    jobjectArray ret = env->NewObjectArray(featuresCount, env->FindClass("java/lang/String"), nullptr);
    for (int i = 0; i < featuresCount; ++i) {
        env->SetObjectArrayElement(ret, i, StringToJString(env, features[i]));
    }
    return ret;
}

// 生成阵型代码字符串
static std::string generateLineupStr(const std::multimap<int, int> &theMap) {
    std::ostringstream ss1; // 花盆荷叶
    std::ostringstream ss2; // 普通植物
    std::ostringstream ss3; // 南瓜

    auto it = theMap.cbegin();
    auto end = theMap.cend();
    while (it != end) {
        const auto &[key, value] = *it;
        int aSeedType = key & 0x3F;
        std::ostringstream *ssPtr;
        switch (aSeedType) {
            case SeedType::SEED_LILYPAD:
            case SeedType::SEED_FLOWERPOT:
                ssPtr = &ss1;
                break;
            case SeedType::SEED_PUMPKINSHELL:
                ssPtr = &ss3;
                break;
            default:
                ssPtr = &ss2;
                break;
        }
        bool wakeUp = (key >> 6) & 1;
        bool imitaterMorphed = (key >> 7) & 1;
        bool ladder = (key >> 8) & 1;
        int aPlantCol = value & 0x0F;
        int aRow = value >> 4;
        *ssPtr << aSeedType << ' ';
        if (wakeUp) {
            *ssPtr << 'W';
        }
        if (imitaterMorphed) {
            *ssPtr << 'I';
        }
        if (ladder) {
            *ssPtr << 'L';
        }
        if (wakeUp || imitaterMorphed || ladder) {
            *ssPtr << ' ';
        }
        *ssPtr << aPlantCol << ',' << aRow;
        while ((++it != end) && (it->first == key)) {
            int v = it->second;
            int col = v & 0x0F;
            int row = v >> 4;
            *ssPtr << ' ' << col << ',' << row;
        }
        *ssPtr << " ; ";
    }

    ss1 << ss2.view() << ss3.view();
    return std::move(ss1).str();
}

extern "C" JNIEXPORT jstring JNICALL Java_com_android_support_CkHomuraMenu_GetCurrentFormation(JNIEnv *env, jobject thiz) {
    Board *aBoard = (*gLawnApp_Addr)->mBoard;
    if (aBoard == nullptr) {
        return StringToJString(env, "");
    }

    std::multimap<int, int> map;
    for (Plant *aPlant = nullptr; aBoard->IteratePlants(aPlant);) {
        if (aPlant->mDead) {
            continue;
        }
        SeedType aSeedType = aPlant->mSeedType;
        SeedType aImitaterType = aPlant->mImitaterType;
        if (aSeedType == SeedType::SEED_IMITATER) {
            aSeedType = aImitaterType;
        }
        int aPlantCol = aPlant->mPlantCol;
        int aRow = aPlant->mRow;
        bool aIsAsleep = aPlant->mIsAsleep;
        bool canHaveLadder = aSeedType == SeedType::SEED_WALLNUT || aSeedType == SeedType::SEED_TALLNUT || aSeedType == SeedType::SEED_PUMPKINSHELL;
        bool canBeAsleep = Plant::IsNocturnal(aSeedType);
        bool wakeUp = canBeAsleep && !aIsAsleep;
        bool imitaterMorphed = aSeedType == SeedType::SEED_IMITATER || aImitaterType == SeedType::SEED_IMITATER;
        bool ladder = canHaveLadder && (aBoard->GetLadderAt(aPlantCol, aRow) != nullptr);

        int key = aSeedType | (wakeUp << 6) | (imitaterMorphed << 7) | (ladder << 8);
        int value = aPlantCol | (aRow << 4);
        map.emplace(key, value);
    }
    return StringToJString(env, generateLineupStr(map));
}

extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_native1PButtonDown(JNIEnv *env, jclass clazz, jint code) {
    Board *aBoard = (*gLawnApp_Addr)->mBoard;
    if (aBoard) {
        gButtonDownP1 = true;
        gButtonCodeP1 = ButtonCode(code);
    }
}

extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_native2PButtonDown(JNIEnv *env, jclass clazz, jint code) {
    Board *aBoard = (*gLawnApp_Addr)->mBoard;
    if (aBoard) {
        gButtonDownP2 = true;
        gButtonCodeP2 = ButtonCode(code);
    }
}


extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeSwitchTwoPlayerMode(JNIEnv *env, jclass clazz, jboolean is_on) {
    isKeyboardTwoPlayerMode = doKeyboardTwoPlayerDialog = is_on;
    if (is_on) {
        return;
    }
    LawnApp *gLawnApp = *gLawnApp_Addr;
    Board *aBoard = gLawnApp->mBoard;
    if (gLawnApp->IsCoopMode() || gLawnApp->mGameMode == GameMode::GAMEMODE_MP_VS) {
        return;
    }
    gLawnApp->ClearSecondPlayer();
    if (aBoard) {
        aBoard->mGamepadControls2->mPlayerIndex2 = -1;
    }
    gLawnApp->mTwoPlayerState = -1;
}

extern "C" JNIEXPORT jboolean JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeIsInGame(JNIEnv *env, jclass clazz) {
    LawnApp *gLawnApp = *gLawnApp_Addr;
    Board *aBoard = gLawnApp->mBoard;
    auto *aFocusWidget = gLawnApp->mWidgetManager->mFocusWidget;
    if (aBoard && aFocusWidget == reinterpret_cast<Sexy::Widget *>(aBoard)) {
        return true;
    }
    SeedChooserScreen *aSeedChooser = gLawnApp->mSeedChooserScreen;
    if (gLawnApp->IsCoopMode() && aSeedChooser && (aFocusWidget == reinterpret_cast<Sexy::Widget *>(aSeedChooser))) {
        return true;
    }
    if (gLawnApp->IsVSMode() && gLawnApp->mVSSetupScreen && (gLawnApp->mVSSetupScreen->mState == VSSetupMenu::VS_SETUP_SIDES || gLawnApp->mVSSetupScreen->mState == VSSetupMenu::VS_CUSTOM_BATTLE)) {
        return true;
    }

    return false;
}


extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeSendButtonEvent(JNIEnv *env, jclass clazz, jboolean is_key_down, jint button_code) {
    bool aIsPlayer2 = button_code >= 256;
    ButtonCode aButtonCode = ButtonCode(aIsPlayer2 ? button_code - 256 : button_code);
    LawnApp *gLawnApp = *gLawnApp_Addr;
    Board *aBoard = gLawnApp->mBoard;
    auto *aFocusWidget = gLawnApp->mWidgetManager->mFocusWidget;

    if (!aBoard || aFocusWidget != reinterpret_cast<Sexy::Widget *>(aBoard)) {
        SeedChooserScreen *aSeedChooser = gLawnApp->mSeedChooserScreen;
        if (is_key_down && gLawnApp->IsCoopMode() && aSeedChooser && aFocusWidget == reinterpret_cast<Sexy::Widget *>(aSeedChooser)) {
            gButtonDownSeedChooser = true;
            gButtonCode = aButtonCode;
            gGamePlayerIndex = aIsPlayer2 ? 1 : 0;
            return;
        }

        VSSetupMenu *aVSSetup = gLawnApp->mVSSetupScreen;
        if (is_key_down && gLawnApp->IsVSMode() && aVSSetup && (aVSSetup->mState == VSSetupMenu::VS_SETUP_SIDES || aVSSetup->mState == VSSetupMenu::VS_CUSTOM_BATTLE)) {
            gButtonDownVSSetup = true;
            gButtonCode = aButtonCode;
            gGamePlayerIndex = aIsPlayer2 ? 1 : 0;
            return;
        }
        return;
    }

    float &aX = (aIsPlayer2 == true) ? gGamepadP2VelX : gGamepadP1VelX;
    float &aY = (aIsPlayer2 == true) ? gGamepadP2VelY : gGamepadP1VelY;
    if (is_key_down) {
        switch (aButtonCode) {
            case ButtonCode::BUTTONCODE_B:
                gKeyDown = true;
                gGamePlayerIndex = aIsPlayer2 ? 1 : 0;
                break;
            case ButtonCode::BUTTONCODE_UP:
                aY = -400;
                break;
            case ButtonCode::BUTTONCODE_DOWN:
                aY = 400;
                break;
            case ButtonCode::BUTTONCODE_LEFT:
                aX = -400;
                break;
            case ButtonCode::BUTTONCODE_RIGHT:
                aX = 400;
                break;
            default:
                gButtonDown = true;
                gButtonCode = aButtonCode;
                gGamePlayerIndex = aIsPlayer2 ? 1 : 0;
                break;
        }
    } else {
        switch (aButtonCode) {
            case ButtonCode::BUTTONCODE_UP:
            case ButtonCode::BUTTONCODE_DOWN:
                aY = 0;
                break;
            case ButtonCode::BUTTONCODE_LEFT:
            case ButtonCode::BUTTONCODE_RIGHT:
                aX = 0;
                break;
            default:
                break;
        }
    }
}

extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeAutoFixPosition(JNIEnv *env, jclass clazz) {
    positionAutoFix = true;
}

extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeUseXboxMusics(JNIEnv *env, jclass clazz) {
    useXboxMusic = true;
}

extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeSeedBankPin(JNIEnv *env, jclass clazz) {
    seedBankPin = true;
}
extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeDynamicPreview(JNIEnv *env, jclass clazz) {
    dynamicPreview = true;
}
extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeEnableOpenSL(JNIEnv *env, jclass clazz) {
    InitOpenSL();
}

extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeJumpLogo(JNIEnv *env, jclass clazz) {
    jumpLogo = true;
}

extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeMoreZombieSeeds(JNIEnv *env, jclass clazz) {
    gMoreZombieSeeds = true;
}

extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeVSBalanceAdjustment(JNIEnv *env, jclass clazz) {
    gVSBalanceAdjustment = true;
}

extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeIntroVideoCompleted(JNIEnv *env, jclass clazz) {
    TitleScreen *aTitleScreen = (*gLawnApp_Addr)->mTitleScreen;
    if (aTitleScreen != nullptr) {
        aTitleScreen->mVideoCompleted = true;
    }
}

extern "C" JNIEXPORT void JNICALL Java_com_transmension_mobile_EnhanceActivity_nativeHeavyWeaponAccel(JNIEnv *env, jclass clazz) {
    heavyWeaponAccel = true;
}
