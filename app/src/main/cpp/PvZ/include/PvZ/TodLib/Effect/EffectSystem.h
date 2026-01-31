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

#ifndef PVZ_SEXYAPPFRAMEWORK_EFFECT_EFFECT_SYSTEM_H
#define PVZ_SEXYAPPFRAMEWORK_EFFECT_EFFECT_SYSTEM_H

#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/SexyAppFramework/Graphics/Image.h"
#include "PvZ/SexyAppFramework/Misc/TriVertex.h"
#include "PvZ/Symbols.h"

constexpr int MAX_TRIANGLES = 256;

class TodTriangleGroup {
public:
    Sexy::Image *mImage;
    Sexy::TriVertex mVertArray[MAX_TRIANGLES][3];
    int mTriangleCount;
    int mDrawMode;

    TodTriangleGroup() = delete;
};

class Reanimation;
class TodParticleHolder;
class TrailHolder;
class ReanimationHolder;
class AttachmentHolder;

class EffectSystem {
public:
    TodParticleHolder *mParticleHolder;
    TrailHolder *mTrailHolder;
    ReanimationHolder *mReanimationHolder;
    AttachmentHolder *mAttachmentHolder;

    void EffectSystemInitialize() {
        reinterpret_cast<void (*)(EffectSystem *)>(EffectSystem_EffectSystemInitializeAddr)(this);
    }
};

#endif // PVZ_SEXYAPPFRAMEWORK_EFFECT_EFFECT_SYSTEM_H
