//
// Created by 28636 on 2025/5/26.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_IMAGE_H
#define PLANTSVSZOMBIES_ANDROIDTV_IMAGE_H

#include "../Misc/Common.h"
#include "../Misc/Point.h"
#include "../Misc/Rect.h"

#include "Color.h"

class LawnApp;

namespace Sexy {

class Image {
public:
    int *vTable;                      // 0
    int placeHolder[2];               // 1 ~ 2
    bool unkBool;                     // 12
    int unkMmWidthems1[2];            // 4 ~ 5
    int *stringUnk1;                  // 6
    int *stringUnk2;                  // 7
    int *stringUnk3;                  // 8
    int mWidth;                       // 9
    int mHeight;                      // 10
    int mPitch;                       // 11
    int mStride;                      // 12
    PixelFormat mFormat;              // 13
    int mPixelFormatHint;             // 14
    int mNumRows;                     // 15
    int mNumCols;                     // 16
    int *mAnimInfo;                   // 17
    int unk[6];                       // 18 ~ 23
    int mHorizonWrapMode;             // 24
    int mVertWrapMode;                // 25
    int mTag;                         // 26
    int unkMems3;                     // 27
    // 大小28个整数

    int GetWidth();
    int GetHeight();
    int GetCelWidth();  // returns the width of just 1 cel in a strip of images
    int GetCelHeight(); // like above but for vertical strips

    //    virtual void FillRect(const Rect &theRect, const Color &theColor, int theDrawMode);
//    virtual void DrawRect(const Rect &theRect, const Color &theColor, int theDrawMode);
//    virtual void ClearRect(const Rect &theRect);
//    virtual void Blt(Image *theImage, int theX, int theY, const Rect &theSrcRect, const Color &theColor, int theDrawMode);
//    virtual void StretchBlt(Image *theImage, const Rect &theDestRect, const Rect &theSrcRect, const Rect &theClipRect, const Color &theColor, int theDrawMode, bool fastStretch);
};

}


#endif // PLANTSVSZOMBIES_ANDROIDTV_IMAGE_H
