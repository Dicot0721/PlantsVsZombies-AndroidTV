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

#ifndef PVZ_LAWN_SYSTEM_POOL_EFFECT_H
#define PVZ_LAWN_SYSTEM_POOL_EFFECT_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/SexyAppFramework/Graphics/Image.h"

constexpr int CAUSTIC_IMAGE_WIDTH = 128;
constexpr int CAUSTIC_IMAGE_HEIGHT = 64;

namespace Sexy {
class MemoryImage;
class Graphics;
}; // namespace Sexy

struct CausticEffect {
    bool mTextureAlpha[256][256];
};

class LawnApp;
class PoolEffect {
public:
    CausticEffect *mCausticGrayscaleImage; // 0
    Sexy::MemoryImage *mCausticImage;      // 1
    LawnApp *mApp;                         // 2
    int mPoolCounter;                      // 3
    // 大小4个整数

    unsigned int BilinearLookupFixedPoint(unsigned int u, unsigned int v) {
        return reinterpret_cast<unsigned int (*)(PoolEffect *, unsigned int, unsigned int)>(PoolEffect_BilinearLookupFixedPointAddr)(this, u, v);
    }

    void UpdateWaterEffect();
    void PoolEffectDraw(Sexy::Graphics *g, bool theIsNight);
};

#endif // PVZ_LAWN_SYSTEM_POOL_EFFECT_H
