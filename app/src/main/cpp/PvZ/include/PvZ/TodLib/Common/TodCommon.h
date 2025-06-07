//
// Created by 28636 on 2025/4/28.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_TODCOMMON_H
#define PLANTSVSZOMBIES_ANDROIDTV_TODCOMMON_H

#include "PvZ/Lawn/Common/LawnCommon.h"
#include "PvZ/SexyAppFramework/Misc/Common.h"
#include "PvZ/SexyAppFramework/Misc/ResourceManager.h"

#include <cstdlib>

struct TodAllocator;
namespace Sexy
{
class Graphics;
class SexyVector2;
class Image;
class Color;
class Font;
};

struct TodWeightedArray
{
    int mItem;
    int mWeight;
};

struct TodWeightedGridArray {
    int mX;
    int mY;
    int mWeight;
};

class TodSmoothArray {
public:
    int mItem;
    float mWeight;
    float mLastPicked;
    float mSecondLastPicked;
};

inline int RandRangeInt(int theMin, int theMax) {
    return reinterpret_cast<int (*)(int, int)>(RandRangeIntAddr)(theMin, theMax);
}

inline float RandRangeFloat(float theMin, float theMax) {
    return reinterpret_cast<float (*)(float, float)>(RandRangeFloatAddr)(theMin, theMax);
}

inline void TodDrawImageCelF(Sexy::Graphics *g, Sexy::Image *theImageStrip, float thePosX, float thePosY, int theCelCol, int theCelRow) {
    reinterpret_cast<void (*)(Sexy::Graphics *, Sexy::Image *, float, float, int, int)>(RandRangeFloatAddr)(g, theImageStrip, thePosX, thePosY, theCelCol, theCelRow);
}

inline void TodScaleRotateTransformMatrix(Sexy::SexyMatrix3 &m, float x, float y, float rad, float theScaleX, float theScaleY) {
    reinterpret_cast<void (*)(Sexy::SexyMatrix3 &, float, float, float, float, float)>(TodScaleRotateTransformMatrixAddr)(m, x, y, rad, theScaleX, theScaleY);
}

inline void TodBltMatrix(
    Sexy::Graphics *g, Sexy::Image *theImage, const Sexy::SexyMatrix3 &theTransform, const Sexy::Rect &theClipRect, const Sexy::Color &theColor, int theDrawMode, const Sexy::Rect &theSrcRect) {
    reinterpret_cast<void (*)(Sexy::Graphics *, Sexy::Image *, const Sexy::SexyMatrix3 &, const Sexy::Rect &, const Sexy::Color &, int, const Sexy::Rect &)>(TodBltMatrixAddr)(
        g, theImage, theTransform, theClipRect, theColor, theDrawMode, theSrcRect);
}

//inline int RandRangeInt(int theMin, int theMax);

//inline float RandRangeFloat(float theMin, float theMax);

//inline void TodDrawImageCelF(Sexy::Graphics *g, Sexy::Image *theImageStrip, float thePosX, float thePosY, int theCelCol, int theCelRow);

inline int (*TodDrawImageCelCenterScaledF)(Sexy::Graphics *a1, Sexy::Image *a2, float a3, float a4, int a5, float a6, float a7);

inline void (*TodDrawImageScaledF)(Sexy::Graphics *g, Sexy::Image *image, float x, float y, float xScaled, float yScaled);

inline void (*TodDrawImageCelScaledF)(Sexy::Graphics *, Sexy::Image *, float, float, int, int, float, float);

inline void (*TodDrawImageCenterScaledF)(Sexy::Graphics *, Sexy::Image *, float, float, float, float);

inline void (*TodDrawString)(Sexy::Graphics *, int *, int, int, Sexy::Font *, Sexy::Color color, DrawStringJustification);

inline void (*TodScaleTransformMatrix)(Sexy::SexyMatrix3 &, float, float, float, float);

inline void (*TodDrawStringMatrix)(Sexy::Graphics *, Sexy::Font *, Sexy::SexyMatrix3 &, int *, Sexy::Color *);

inline int (*TodAnimateCurve)(int theTimeStart, int theTimeEnd, int theTimeAge, int thePositionStart, int thePositionEnd, TodCurves theCurve);

inline float (*TodAnimateCurveFloat)(int theTimeStart, int theTimeEnd, int theTimeAge, float thePositionStart, float thePositionEnd, TodCurves theCurve);

inline float (*TodAnimateCurveFloatTime)(float theTimeStart, float theTimeEnd, float theTimeAge, float thePositionStart, float thePositionEnd, TodCurves theCurve);

inline void (*TodDrawStringWrapped)(Sexy::Graphics *graphics, int *text, Sexy::Rect *theRect, Sexy::Font *theFont, Sexy::Color *theColor, DrawStringJustification justification, bool a7);

inline int (*TodDrawStringWrappedHelper)(Sexy::Graphics *pGraphics, int *pInt, Sexy::Rect *pRect, Sexy::Font *pFont, Sexy::Color color, DrawStringJustification justification, bool i1, bool i2);


inline void SetBit(uint& theNum, int theIdx, bool theValue = true) {
    if (theValue)
        theNum |= 1 << theIdx;
    else
        theNum &= ~(1 << theIdx);
}
inline bool TestBit(uint theNum, int theIdx) {
    return theNum & (1 << theIdx);
}

#endif // PLANTSVSZOMBIES_ANDROIDTV_TODCOMMON_H
