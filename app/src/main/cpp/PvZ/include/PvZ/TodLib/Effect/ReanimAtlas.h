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

#ifndef PVZ_SEXYAPPFRAMEWORK_EFFECT_REANIM_ATLAS_H
#define PVZ_SEXYAPPFRAMEWORK_EFFECT_REANIM_ATLAS_H

#include "PvZ/Symbols.h"

class ReanimatorDefinition;
namespace Sexy {
class MemoryImage;
} // namespace Sexy

class ReanimAtlasImage {
public:
    int mX;
    int mY;
    int mWidth;
    int mHeight;
    Sexy::Image *mOriginalImage;

    ReanimAtlasImage()
        : mX(0)
        , mY(0)
        , mWidth(0)
        , mHeight(0)
        , mOriginalImage(nullptr) {}
};

class ReanimAtlas {
public:
    ReanimAtlasImage *GetEncodedReanimAtlas(Sexy::Image *theImage) {
        return reinterpret_cast<ReanimAtlasImage *(*)(ReanimAtlas *, Sexy::Image *)>(ReanimAtlas_GetEncodedReanimAtlasAddr)(this, theImage);
    }
};

#endif // PVZ_SEXYAPPFRAMEWORK_EFFECT_REANIM_ATLAS_H
