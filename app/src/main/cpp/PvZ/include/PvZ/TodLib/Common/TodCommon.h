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
class Color;
class Font;

template <bool>
class __Image;

using Image = __Image<false>;
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

inline unsigned long AverageNearByPixels(Sexy::MemoryImage *theImage, unsigned long *thePixel, int x, int y) {
    int aRed = 0;
    int aGreen = 0;
    int aBlue = 0;
    int aBitsCount = 0;

    for (int i = -1; i <= 1; i++)  // 依次循环上方、当前、下方的一行
    {
        if (i == 0)  // 排除当前行
        {
            continue;
        }

        for (int j = -1; j <= 1; j++)  // 依次循环左方、当前、右方的一列
        {
            if ((x != 0 || j != -1) && (x != theImage->mWidth - 1 || j != 1) && (y != 0 || i != -1) && (y != theImage->mHeight - 1 || i != 1))
            {
                unsigned long aPixel = *(thePixel + i * theImage->mWidth + j);
                if (aPixel & 0xFF000000UL)  // 如果不是透明像素
                {
                    aRed += (aPixel >> 16) & 0x000000FFUL;
                    aGreen += (aPixel >> 8) & 0x000000FFUL;
                    aBlue += aPixel & 0x000000FFUL;
                    aBitsCount++;
                }
            }
        }
    }

    if (aBitsCount == 0)
        return 0;

    aRed /= aBitsCount;
    aRed = min(aRed, 255);
    aGreen /= aBitsCount;
    aGreen = min(aGreen, 255);
    aBlue /= aBitsCount;
    aBlue = min(aBlue, 255);
    return (aRed << 16) | (aGreen << 8) | (aBlue);
}

inline void FixPixelsOnAlphaEdgeForBlending(Sexy::Image *theImage) {
    Sexy::MemoryImage* aImage = (Sexy::MemoryImage*)theImage;
    unsigned long* aBitsPtr = aImage->mBits;
    if (aImage->mBits == nullptr)
        return;

    for (int y = 0; y < theImage->mHeight; y++)
    {
        for (int x = 0; x < theImage->mWidth; x++)
        {
            if ((*aBitsPtr & 0xFF000000UL) == 0)  // 如果像素的不透明度为 0
            {
                *aBitsPtr = AverageNearByPixels(aImage, aBitsPtr, x, y);  // 计算该点周围非透明像素的平均颜色
            }

            aBitsPtr++;
        }
    }
    //++theImage->mBitsChangedCount;
    aImage->BitsChanged();
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

//inline unsigned long AverageNearByPixels(Sexy::MemoryImage *theImage, unsigned long *thePixel, int x, int y);
//
//inline void FixPixelsOnAlphaEdgeForBlending(Sexy::Image *theImage);

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
