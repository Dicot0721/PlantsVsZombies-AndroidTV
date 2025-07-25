#include "PvZ/SexyAppFramework/Graphics/Image.h"

using namespace Sexy;

int __Image::GetWidth() {
    return mWidth;
}

int __Image::GetHeight() {
    return mHeight;
}

int __Image::GetCelHeight() {
    return mHeight / mNumRows;
}

int __Image::GetCelWidth() {
    return mWidth / mNumCols;
}

void __Image::PushTransform(const SexyMatrix3 &theTransform, bool concatenate) {
    old_Sexy_Image_PushTransform(this, theTransform, concatenate);
}

void __Image::PopTransform() {
    old_Sexy_Image_PopTransform(this);
}

void GLImage::PushTransform(const SexyMatrix3 &theTransform, bool concatenate) {
    old_Sexy_GLImage_PushTransform(this, theTransform, concatenate);
}

void GLImage::PopTransform() {
    old_Sexy_GLImage_PopTransform(this);
}