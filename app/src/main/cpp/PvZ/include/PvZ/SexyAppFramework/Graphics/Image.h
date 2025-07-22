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

class SexyMatrix3;

class __Image {
public:
    int *vTable;           // 0
    int placeHolder[2];    // 1 ~ 2
    bool unkBool;          // 12
    int unkMmWidthems1[2]; // 4 ~ 5
    int *stringUnk1;       // 6
    int *stringUnk2;       // 7
    int *stringUnk3;       // 8
    int mWidth;            // 9
    int mHeight;           // 10
    int mPitch;            // 11
    int mStride;           // 12
    PixelFormat mFormat;   // 13
    int mPixelFormatHint;  // 14
    int mNumRows;          // 15
    int mNumCols;          // 16
    int *mAnimInfo;        // 17
    int unk[6];            // 18 ~ 23
    int mHorizonWrapMode;  // 24
    int mVertWrapMode;     // 25
    int mTag;              // 26
    int unkMems3;          // 27
    // 大小28个整数

    __Image() = default;
    ~__Image() = default;

    int GetWidth();
    int GetHeight();
    int GetCelWidth();  // returns the width of just 1 cel in a strip of images
    int GetCelHeight(); // like above but for vertical strips
    void PushTransform(const SexyMatrix3 &theTransform, bool concatenate);
    void PopTransform();
};

class Image : public __Image {
public:
    Image() = delete;
    ~Image() = delete;
};

class GLImage : public __Image {
public:
    void PushTransform(const SexyMatrix3 &theTransform, bool concatenate);
    void PopTransform();
};

} // namespace Sexy

inline void (*old_Sexy_Image_PushTransform)(Sexy::__Image *image, const Sexy::SexyMatrix3 &theTransform, bool concatenate);

inline void (*old_Sexy_Image_PopTransform)(Sexy::__Image *image);

inline void (*old_Sexy_GLImage_PushTransform)(Sexy::GLImage *image, const Sexy::SexyMatrix3 &theTransform, bool concatenate);

inline void (*old_Sexy_GLImage_PopTransform)(Sexy::GLImage *image);

#endif // PLANTSVSZOMBIES_ANDROIDTV_IMAGE_H
