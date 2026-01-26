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

#ifndef PVZ_SEXYAPPFRAMEWORK_EFFECT_TOD_PARTICLE_H
#define PVZ_SEXYAPPFRAMEWORK_EFFECT_TOD_PARTICLE_H

#include "PvZ/SexyAppFramework/Misc/SexyVector.h"
#include "PvZ/TodLib/Common/DataArray.h"
#include "PvZ/TodLib/Common/TodList.h"

// ====================================================================================================
// ★ 【发射器定义】
// ----------------------------------------------------------------------------------------------------
// 粒子发射器的定义数据描述了其各种行为的参数的变化规律和范围。
// ====================================================================================================
class TodEmitterDefinition {};

// ====================================================================================================
// ★ 【粒子系统定义】
// ----------------------------------------------------------------------------------------------------
// 粒子系统的定义数据，是粒子系统中各个粒子发射器的定义数据的集合。
// ====================================================================================================
class TodParticleDefinition {
public:
    TodEmitterDefinition *mEmitterDefs;
    int mEmitterDefCount;
};

// ######################################################################################################################################################
// ############################################################ 以下正式开始粒子系统相关声明 ############################################################
// ######################################################################################################################################################

class TodParticleHolder {
public:
};

class TodParticleEmitter {
public:
};

class TodParticleSystem {
public:
    int *vTable;                             // 0
    int unk1[3];                             // 1 ~ 3
    ParticleEffect mEffectType;              // 4
    TodParticleDefinition *mParticleDef;     // 5
    TodParticleHolder *mParticleHolder;      // 6
    TodList<ParticleEmitterID> mEmitterList; // 7 ~ 10
    bool mDead;                              // 44
    bool mIsAttachment;                      // 45
    int mRenderOrder;                        // 12
    bool mDontUpdate;                        // 52
    bool mActive;                            // 53
    int mParticleId;                         // 14
    // 大小15个整数

    void Draw(Sexy::Graphics *g) {
        reinterpret_cast<void (*)(TodParticleSystem *, Sexy::Graphics *)>(TodParticleSystem_DrawAddr)(this, g);
    }
    void OverrideColor(const char *theEmitterName, const Sexy::Color &theColor) {
        reinterpret_cast<void (*)(TodParticleSystem *, const char *, const Sexy::Color &)>(TodParticleSystem_OverrideColorAddr)(this, theEmitterName, theColor);
    }
    void OverrideImage(const char *theEmitterName, Sexy::Image *theImage) {
        reinterpret_cast<void (*)(TodParticleSystem *, const char *, Sexy::Image *)>(TodParticleSystem_OverrideImageAddr)(this, theEmitterName, theImage);
    }
    void ParticleSystemDie() {
        reinterpret_cast<void (*)(TodParticleSystem *)>(TodParticleSystem_ParticleSystemDieAddr)(this);
    }
    void Update() {
        reinterpret_cast<void (*)(TodParticleSystem *)>(TodParticleSystem_UpdateAddr)(this);
    }

    ~TodParticleSystem() {
        _destructor();
    }

protected:
    void _destructor() {
        reinterpret_cast<void (*)(TodParticleSystem *)>(TodParticleSystem_Delete2Addr)(this);
    }
};

#endif // PVZ_SEXYAPPFRAMEWORK_EFFECT_TOD_PARTICLE_H
