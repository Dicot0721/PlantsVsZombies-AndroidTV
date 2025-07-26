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

#ifndef PVZ_SEXYAPPFRAMEWORK_EFFECT_FILTER_EFFECT_H
#define PVZ_SEXYAPPFRAMEWORK_EFFECT_FILTER_EFFECT_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Symbols.h"

namespace Sexy {
class Image;
class MemoryImage;
} // namespace Sexy

enum FilterEffect { FILTEREFFECT_NONE = -1, FILTEREFFECT_WASHED_OUT = 0, FILTEREFFECT_LESS_WASHED_OUT = 1, FILTEREFFECT_WHITE = 2, FILTEREFFECT_CUSTOM = 3, NUM_FILTEREFFECT };

Sexy::MemoryImage *FilterEffectCreateImage(Sexy::Image *theImage, FilterEffect theFilterEffect);

Sexy::Image *FilterEffectGetImage(Sexy::Image *theImage, FilterEffect);

FilterEffect GetFilterEffectTypeBySeedType(SeedType mSeedType);

inline void FilterEffectDoWashedOut(Sexy::MemoryImage *theImage) {
    reinterpret_cast<void (*)(Sexy::MemoryImage *)>(FilterEffectDoWashedOutAddr)(theImage);
}

inline void FilterEffectDoLessWashedOut(Sexy::MemoryImage *theImage) {
    reinterpret_cast<void (*)(Sexy::MemoryImage *)>(FilterEffectDoLessWashedOutAddr)(theImage);
}

inline void FilterEffectDoWhite(Sexy::MemoryImage *theImage) {
    reinterpret_cast<void (*)(Sexy::MemoryImage *)>(FilterEffectDoWhiteAddr)(theImage);
}

inline void FilterEffectDoLumSat(Sexy::MemoryImage *theImage, float theLum, float theSat) {
    reinterpret_cast<void (*)(Sexy::MemoryImage *, float, float)>(FilterEffectDoLumSatAddr)(theImage, theLum, theSat);
}

#endif // PVZ_SEXYAPPFRAMEWORK_EFFECT_FILTER_EFFECT_H
