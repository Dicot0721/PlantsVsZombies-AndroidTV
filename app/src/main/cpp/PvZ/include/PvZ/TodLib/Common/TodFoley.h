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

#ifndef PVZ_SEXYAPPFRAMEWORK_TODLIB_COMMON_TOD_FOLEY_H
#define PVZ_SEXYAPPFRAMEWORK_TODLIB_COMMON_TOD_FOLEY_H

#include "PvZ/Lawn/Common/Resources.h"

constexpr int MAX_FOLEY_TYPES = 110;
constexpr int MAX_FOLEY_INSTANCES = 8;

// ######################################################################################################################################################
// ############################################################# 以下为拟音音效定义相关内容 #############################################################
// ######################################################################################################################################################

enum FoleyFlags {
    FOLEYFLAGS_LOOP,              // 循环播放
    FOLEYFLAGS_ONE_AT_A_TIME,     // 禁止叠加播放：指定该种类的音效同时至多存在 1 个实例，重复播放时仅增加引用计数并刷新开始时间
    FOLEYFLAGS_MUTE_ON_PAUSE,     // 暂停时静默
    FOLEYFLAGS_USES_MUSIC_VOLUME, // 使用音乐音量，指定该种类的音效使用与背景音乐相同的音量
    FOLEYFLAGS_DONT_REPEAT        // 禁止变式重复，指定该种类的音效每次实际播放的变式不得与上一次相同
};

enum FoleyType {
    FOLEY_SUN,
    FOLEY_SPLAT = 1,
    FOLEY_LAWNMOWER,
    FOLEY_THROW = 3,
    FOLEY_SPAWN_SUN,
    FOLEY_CHOMP,
    FOLEY_CHOMP_SOFT,
    FOLEY_PLANT = 7,
    FOLEY_USE_SHOVEL = 8,
    FOLEY_DROP,
    FOLEY_BLEEP,
    FOLEY_GROAN,
    FOLEY_BRAINS = 12,
    FOLEY_SUKHBIR = 13,
    FOLEY_JACKINTHEBOX = 14,
    FOLEY_ART_CHALLENGE = 15,
    FOLEY_ZAMBONI = 16,
    FOLEY_THUNDER = 17,
    FOLEY_FROZEN = 18,
    FOLEY_ZOMBIESPLASH = 19,
    FOLEY_BOWLINGIMPACT = 20,
    FOLEY_SQUISH = 21,
    FOLEY_TIRE_POP = 22,
    FOLEY_EXPLOSION = 23,
    FOLEY_SLURP = 24,
    FOLEY_LIMBS_POP = 25,
    FOLEY_POGO_ZOMBIE = 26,
    FOLEY_SNOW_PEA_SPARKLES = 27,
    FOLEY_ZOMBIE_FALLING = 28,
    FOLEY_PUFF = 29,
    FOLEY_FUME = 30,
    FOLEY_COIN = 31,
    FOLEY_KERNEL_SPLAT = 32,
    FOLEY_DIGGER = 33,
    FOLEY_JACK_SURPRISE = 34,
    FOLEY_VASE_BREAKING = 35,
    FOLEY_POOL_CLEANER = 36,
    FOLEY_BASKETBALL = 37,
    FOLEY_IGNITE = 38,
    FOLEY_FIREPEA = 39,
    FOLEY_THUMP = 40,
    FOLEY_SQUASH_HMM = 41,
    FOLEY_MAGNETSHROOM = 42,
    FOLEY_BUTTER = 43,
    FOLEY_BUNGEE_SCREAM = 44,
    FOLEY_BOSS_EXPLOSION_SMALL = 45,
    FOLEY_SHIELD_HIT = 46,
    FOLEY_SWING = 47,
    FOLEY_BONK = 48,
    FOLEY_RAIN = 49,
    FOLEY_DOLPHIN_BEFORE_JUMPING = 50,
    FOLEY_DOLPHIN_APPEARS = 51,
    FOLEY_PLANT_WATER = 52,
    FOLEY_ZOMBIE_ENTERING_WATER = 53,
    FOLEY_GRAVEBUSTERCHOMP = 54,
    FOLEY_CHERRYBOMB = 55,
    FOLEY_JALAPENO_IGNITE = 56,
    FOLEY_REVERSE_EXPLOSION = 57,
    FOLEY_PLASTIC_HIT = 58,
    FOLEY_WINMUSIC = 59,
    FOLEY_BALLOONINFLATE = 60,
    FOLEY_BIGCHOMP = 61,
    FOLEY_MELONIMPACT = 62,
    FOLEY_PLANTGROW = 63,
    FOLEY_SHOOP = 64,
    FOLEY_JUICY = 65,
    FOLEY_NEWSPAPER_RARRGH = 66,
    FOLEY_NEWSPAPER_RIP = 67,
    FOLEY_FLOOP = 68,
    FOLEY_COFFEE = 69,
    FOLEY_LOW_GROAN = 70,
    FOLEY_PRIZE = 71,
    FOLEY_YUCK = 72,
    FOLEY_UMBRELLA = 73,
    FOLEY_GRASSSTEP = 74,
    FOLEY_SHOVEL = 75,
    FOLEY_COB_LAUNCH = 76,
    FOLEY_WATERING = 77,
    FOLEY_POLEVAULT = 78,
    FOLEY_GRAVESTONE_RUMBLE = 79,
    FOLEY_DIRT_RISE = 80,
    FOLEY_FERTILIZER = 81,
    FOLEY_PORTAL = 82,
    FOLEY_WAKEUP = 83,
    FOLEY_BUGSPRAY = 84,
    FOLEY_SCREAM = 85,
    FOLEY_PAPER = 86,
    FOLEY_MONEYFALLS = 87,
    FOLEY_IMP = 88,
    FOLEY_HYDRAULIC_SHORT = 89,
    FOLEY_HYDRAULIC = 90,
    FOLEY_GARGANTUDEATH = 91,
    FOLEY_CERAMIC = 92,
    FOLEY_BOSS_BOULDER_ATTACK = 93,
    FOLEY_CHIME = 94,
    FOLEY_CRAZY_DAVE_SHORT = 95,
    FOLEY_CRAZY_DAVE_LONG = 96,
    FOLEY_CRAZY_DAVE_EXTRA_LONG = 97,
    FOLEY_CRAZY_DAVE_CRAZY = 98,
    FOLEY_PHONOGRAPH = 99,
    FOLEY_DANCER = 100,
    FOLEY_FINAL_FANFARE = 101,
    FOLEY_CRAZY_DAVE_SCREAM = 102,
    FOLEY_CRAZY_DAVE_SCREAM_2 = 103,
    NUM_FOLEY = 104,
    FOLEY_MENU_LEFT = 105,
    FOLEY_MENU_CENTRE = 106,
    FOLEY_MENU_RIGHT = 107,
    FOLEY_THRILLER
};

class FoleyParams {
public:
    FoleyType mFoleyType;
    float mPitchRange;
    int *mSfxID[10];
    unsigned int mFoleyFlags;
};

FoleyParams *LookupFoley(FoleyType theFoleyType);

extern FoleyParams gNewLawnFoleyParamArray[];

inline FoleyParams *(*old_LookupFoley)(FoleyType type);

// ######################################################################################################################################################
// ############################################################ 以下正式开始拟音音效相关声明 ############################################################
// ######################################################################################################################################################

class FoleyInstance {
public:
    int *mInstance;
    int mRefCount;
    bool _paused;
    int mStartTime;
    int mPauseOffset;
};

struct FoleyTypeData {
    FoleyInstance mFoleyInstances[MAX_FOLEY_INSTANCES];
    int mLastVariationPlayed;
};

class TodFoley {
public:
    FoleyTypeData mTypeData[MAX_FOLEY_TYPES];

    bool IsFoleyPlaying(FoleyType theFoleyType) {
        return reinterpret_cast<bool (*)(TodFoley *, FoleyType)>(TodFoley_IsFoleyPlayingAddr)(this, theFoleyType);
    }
    void StopFoley(FoleyType theFoleyType) {
        reinterpret_cast<void (*)(TodFoley *, FoleyType)>(TodFoley_StopFoleyAddr)(this, theFoleyType);
    }
    void CancelPausedFoley() {
        reinterpret_cast<void (*)(TodFoley *)>(TodFoley_CancelPausedFoleyAddr)(this);
    }
};

inline FoleyInstance *SoundSystemFindInstance(TodFoley *theSoundSystem, FoleyType theFoleyType) {
    return reinterpret_cast<FoleyInstance *(*)(TodFoley *, FoleyType)>(SoundSystemFindInstanceAddr)(theSoundSystem, theFoleyType);
}

#endif // PVZ_SEXYAPPFRAMEWORK_TODLIB_COMMON_TOD_FOLEY_H
