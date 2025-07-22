//
// Created by 28636 on 2025/5/27.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_FILTEREFFECT_H
#define PLANTSVSZOMBIES_ANDROIDTV_FILTEREFFECT_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Symbols.h"

namespace Sexy {
class Image;
class MemoryImage;
}

enum FilterEffect {
    FILTEREFFECT_NONE = -1,
    FILTEREFFECT_WASHED_OUT = 0,
    FILTEREFFECT_LESS_WASHED_OUT = 1,
    FILTEREFFECT_WHITE = 2,
    FILTEREFFECT_CUSTOM = 3,
    NUM_FILTEREFFECT
};

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
#endif // PLANTSVSZOMBIES_ANDROIDTV_FILTEREFFECT_H
