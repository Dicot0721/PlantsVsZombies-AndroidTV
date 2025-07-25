#ifndef PVZ_SEXYAPPFRAMEWORK_EFFECT_REANIM_ATLAS_H
#define PVZ_SEXYAPPFRAMEWORK_EFFECT_REANIM_ATLAS_H

#include "PvZ/Symbols.h"

class ReanimatorDefinition;
namespace Sexy {
class MemoryImage;
}; // namespace Sexy

class ReanimAtlasImage {
public:
    int mX;
    int mY;
    int mWidth;
    int mHeight;
    Sexy::Image* mOriginalImage;

    ReanimAtlasImage()
        : mX(0)
        , mY(0)
        , mWidth(0)
        , mHeight(0)
        , mOriginalImage(nullptr) {}
};

class ReanimAtlas {
public:
    ReanimAtlasImage* GetEncodedReanimAtlas(Sexy::Image* theImage) {
        return reinterpret_cast<ReanimAtlasImage* (*)(ReanimAtlas*, Sexy::Image*)>(ReanimAtlas_GetEncodedReanimAtlasAddr)(this, theImage);
    }
};

#endif // PVZ_SEXYAPPFRAMEWORK_EFFECT_REANIM_ATLAS_H
