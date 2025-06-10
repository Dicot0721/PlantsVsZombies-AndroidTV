//
// Created by 28636 on 2025/5/26.
//

#include "PvZ/SexyAppFramework/Graphics/MemoryImage.h"

using namespace Sexy;

void MemoryImage::ClearRect(const Rect& theRect) {
    int mY;     // r4
    int *mBits; // r7
    int v7;     // r0

    mBits = (int *)(*((int (**)(Sexy::Image *))this->vTable + 13))(this);
    mY = theRect.mY;
    if (mY < theRect.mHeight + mY) {
        do {
            v7 = theRect.mX + (*((int (**)(Sexy::Image *))this->vTable + 6))(this)*mY++;
            memset(&mBits[v7], 0, 4 * theRect.mWidth);
        } while (theRect.mHeight + theRect.mY > mY);
    }

    (*((void (**)(Sexy::Image *))this->vTable + 47))(this);
}

void MemoryImage::PushTransform(const SexyMatrix3 &theTransform, bool concatenate) {
    old_Sexy_MemoryImage_PushTransform(this, theTransform, concatenate);
}

void MemoryImage::PopTransform() {
    old_Sexy_MemoryImage_PopTransform(this);
}