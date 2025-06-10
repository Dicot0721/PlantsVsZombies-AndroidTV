//
// Created by 28636 on 2025/5/27.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_FILTEREFFECT_H
#define PLANTSVSZOMBIES_ANDROIDTV_FILTEREFFECT_H

#include "PvZ/Lawn/Common/ConstEnums.h"

namespace Sexy
{
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

Sexy::Image *FilterEffectGetImage(Sexy::Image *image, FilterEffect);

FilterEffect GetFilterEffectTypeBySeedType(SeedType mSeedType);

inline void (*FilterEffectDoWashedOut)(Sexy::Image *);

inline void (*FilterEffectDoLessWashedOut)(Sexy::Image *);

inline void (*FilterEffectDoWhite)(Sexy::Image *);

inline void (*FilterEffectDoLumSat)(Sexy::Image *theImage, float aLum, float aSat);
#endif // PLANTSVSZOMBIES_ANDROIDTV_FILTEREFFECT_H
