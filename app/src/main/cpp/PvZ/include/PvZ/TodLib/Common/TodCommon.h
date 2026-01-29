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

#ifndef PVZ_SEXYAPPFRAMEWORK_TODLIB_COMMON_TOD_COMMON_H
#define PVZ_SEXYAPPFRAMEWORK_TODLIB_COMMON_TOD_COMMON_H

#include "PvZ/Lawn/Common/LawnCommon.h"
#include "PvZ/SexyAppFramework/Graphics/MemoryImage.h"
#include "PvZ/SexyAppFramework/Misc/Common.h"
#include "PvZ/SexyAppFramework/Misc/ResourceManager.h"

#include <cstdlib>

class TodAllocator;

namespace Sexy {
class Graphics;
class SexyVector2;
class Color;
class Font;
class Image;
} // namespace Sexy

struct TodWeightedArray {
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

inline int ClampInt(int theNum, int theMin, int theMax) {
    return theNum <= theMin ? theMin : theNum >= theMax ? theMax : theNum;
}

inline float ClampFloat(float theNum, float theMin, float theMax) {
    return theNum <= theMin ? theMin : theNum >= theMax ? theMax : theNum;
}

inline float Distance2D(float x1, float y1, float x2, float y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

inline void TodDrawImageCelF(Sexy::Graphics *g, Sexy::Image *theImageStrip, float thePosX, float thePosY, int theCelCol, int theCelRow) {
    reinterpret_cast<void (*)(Sexy::Graphics *, Sexy::Image *, float, float, int, int)>(TodDrawImageCelFAddr)(g, theImageStrip, thePosX, thePosY, theCelCol, theCelRow);
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

    for (int i = -1; i <= 1; i++) // 依次循环上方、当前、下方的一行
    {
        if (i == 0) // 排除当前行
        {
            continue;
        }

        for (int j = -1; j <= 1; j++) // 依次循环左方、当前、右方的一列
        {
            if ((x != 0 || j != -1) && (x != theImage->mWidth - 1 || j != 1) && (y != 0 || i != -1) && (y != theImage->mHeight - 1 || i != 1)) {
                unsigned long aPixel = *(thePixel + i * theImage->mWidth + j);
                if (aPixel & 0xFF000000UL) // 如果不是透明像素
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
    aRed = std::min(aRed, 255);
    aGreen /= aBitsCount;
    aGreen = std::min(aGreen, 255);
    aBlue /= aBitsCount;
    aBlue = std::min(aBlue, 255);
    return (aRed << 16) | (aGreen << 8) | (aBlue);
}

inline void FixPixelsOnAlphaEdgeForBlending(Sexy::Image *theImage) {
    Sexy::MemoryImage *aImage = (Sexy::MemoryImage *)theImage;
    unsigned long *aBitsPtr = aImage->mBits;
    if (aImage->mBits == nullptr)
        return;

    for (int y = 0; y < theImage->mHeight; y++) {
        for (int x = 0; x < theImage->mWidth; x++) {
            if ((*aBitsPtr & 0xFF000000UL) == 0) // 如果像素的不透明度为 0
            {
                *aBitsPtr = AverageNearByPixels(aImage, aBitsPtr, x, y); // 计算该点周围非透明像素的平均颜色
            }

            aBitsPtr++;
        }
    }
    //++theImage->mBitsChangedCount;
    aImage->BitsChanged();
}

inline void TodDrawImageCelCenterScaledF(Sexy::Graphics *g, Sexy::Image *theImageStrip, float thePosX, float thePosY, int theCelCol, float theScaleX, float theScaleY) {
    reinterpret_cast<void *(*)(Sexy::Graphics *, Sexy::Image *, float, float, int, float, float)>(TodDrawImageCelCenterScaledFAddr)(
        g, theImageStrip, thePosX, thePosY, theCelCol, theScaleX, theScaleY);
}
inline void TodDrawImageScaledF(Sexy::Graphics *g, Sexy::Image *theImage, float thePosX, float thePosY, float theScaleX, float theScaleY) {
    reinterpret_cast<void *(*)(Sexy::Graphics *, Sexy::Image *, float, float, float, float)>(TodDrawImageScaledFAddr)(g, theImage, thePosX, thePosY, theScaleX, theScaleY);
}

inline void TodDrawImageCelScaledF(Sexy::Graphics *g, Sexy::Image *theImageStrip, float thePosX, float thePosY, int theCelCol, int theCelRow, float theScaleX, float theScaleY) {
    reinterpret_cast<void *(*)(Sexy::Graphics *, Sexy::Image *, float, float, int, int, float, float)>(TodDrawImageCelScaledFAddr)(
        g, theImageStrip, thePosX, thePosY, theCelCol, theCelRow, theScaleX, theScaleY);
}

inline void TodDrawImageCenterScaledF(Sexy::Graphics *g, Sexy::Image *theImage, float thePosX, float thePosY, float theScaleX, float theScaleY) {
    reinterpret_cast<void *(*)(Sexy::Graphics *, Sexy::Image *, float, float, float, float)>(TodDrawImageCenterScaledFAddr)(g, theImage, thePosX, thePosY, theScaleX, theScaleY);
}

inline void TodScaleTransformMatrix(Sexy::SexyMatrix3 &m, float x, float y, float theScaleX, float theScaleY) {
    reinterpret_cast<void *(*)(Sexy::SexyMatrix3 &, float, float, float, float)>(TodScaleTransformMatrixAddr)(m, x, y, theScaleX, theScaleY);
}

inline void TodDrawString(Sexy::Graphics *g, const pvzstl::string &theText, int thePosX, int thePosY, Sexy::Font *theFont, const Sexy::Color theColor, DrawStringJustification theJustification) {
    reinterpret_cast<void *(*)(Sexy::Graphics *, const pvzstl::string &, int, int, Sexy::Font *, const Sexy::Color, DrawStringJustification)>(TodDrawStringAddr)(
        g, theText, thePosX, thePosY, theFont, theColor, theJustification);
}

inline void TodDrawStringMatrix(Sexy::Graphics *g, const Sexy::Font *theFont, const Sexy::SexyMatrix3 &theMatrix, const pvzstl::string &theString, const Sexy::Color &theColor) {
    reinterpret_cast<void *(*)(Sexy::Graphics *, const Sexy::Font *, const Sexy::SexyMatrix3 &, const pvzstl::string &, const Sexy::Color &)>(TodDrawStringMatrixAddr)(
        g, theFont, theMatrix, theString, theColor);
}

inline int TodAnimateCurve(int theTimeStart, int theTimeEnd, int theTimeAge, int thePositionStart, int thePositionEnd, TodCurves theCurve) {
    return reinterpret_cast<int (*)(int, int, int, int, int, TodCurves)>(TodAnimateCurveAddr)(theTimeStart, theTimeEnd, theTimeAge, thePositionStart, thePositionEnd, theCurve);
}

inline float TodAnimateCurveFloat(int theTimeStart, int theTimeEnd, int theTimeAge, float thePositionStart, float thePositionEnd, TodCurves theCurve) {
    return reinterpret_cast<float (*)(int, int, int, float, float, TodCurves)>(TodAnimateCurveFloatAddr)(theTimeStart, theTimeEnd, theTimeAge, thePositionStart, thePositionEnd, theCurve);
}

inline float TodAnimateCurveFloatTime(float theTimeStart, float theTimeEnd, float theTimeAge, float thePositionStart, float thePositionEnd, TodCurves theCurve) {
    return reinterpret_cast<float (*)(float, float, float, float, float, TodCurves)>(TodAnimateCurveFloatTimeAddr)(theTimeStart, theTimeEnd, theTimeAge, thePositionStart, thePositionEnd, theCurve);
}

inline bool TodLoadResources(const pvzstl::string &theGroup) {
    return reinterpret_cast<bool (*)(const pvzstl::string &)>(TodLoadResourcesAddr)(theGroup);
}

inline pvzstl::string TodReplaceString(const pvzstl::string &theText, const char *theStringToFind, const pvzstl::string &theStringToSubstitute) {
    pvzstl::string result;
    reinterpret_cast<void (*)(pvzstl::string &, const pvzstl::string &, const char *, const pvzstl::string &)>(TodReplaceStringAddr)(result, theText, theStringToFind, theStringToSubstitute);
    return result;
}

inline pvzstl::string TodReplaceNumberString(const pvzstl::string &theText, const char *theStringToFind, int theNumber) {
    pvzstl::string result;
    reinterpret_cast<void (*)(pvzstl::string &, const pvzstl::string &, const char *, int)>(TodReplaceNumberStringAddr)(result, theText, theStringToFind, theNumber);
    return result;
}

inline TodAllocator *FindGlobalAllocator(int theSize) {
    return reinterpret_cast<TodAllocator *(*)(int)>(FindGlobalAllocatorAddr)(theSize);
}

// inline unsigned long AverageNearByPixels(Sexy::MemoryImage *theImage, unsigned long *thePixel, int x, int y);
//
// inline void FixPixelsOnAlphaEdgeForBlending(Sexy::Image *theImage);

inline Sexy::Color GetFlashingColor(int theCounter, int theFlashTime) {
    int aTimeAge = theCounter % theFlashTime;
    int aTimeInf = theFlashTime / 2;
    int aGrayness = std::clamp(55 + 200 * abs(aTimeInf - aTimeAge) / aTimeInf, 0, 255);
    return Sexy::Color(aGrayness, aGrayness, aGrayness, 255);
}

inline Sexy::Color ColorAdd(const Sexy::Color &theColor1, const Sexy::Color &theColor2) {
    int r = theColor1.mRed + theColor2.mRed;
    int g = theColor1.mGreen + theColor2.mGreen;
    int b = theColor1.mBlue + theColor2.mBlue;
    int a = theColor1.mAlpha + theColor2.mAlpha;

    return Sexy::Color(ClampInt(r, 0, 255), ClampInt(g, 0, 255), ClampInt(b, 0, 255), ClampInt(a, 0, 255)); // 线性减淡
}

inline bool FloatApproxEqual(float theFloatVal1, float theFloatVal2) {
    return fabs(theFloatVal1 - theFloatVal2) < FLT_EPSILON;
}

inline void SetBit(unsigned int &theNum, int theIdx, bool theValue = true) {
    if (theValue)
        theNum |= 1 << theIdx;
    else
        theNum &= ~(1 << theIdx);
}
inline bool TestBit(unsigned int theNum, int theIdx) {
    return theNum & (1 << theIdx);
}

#endif // PVZ_SEXYAPPFRAMEWORK_TODLIB_COMMON_TOD_COMMON_H
