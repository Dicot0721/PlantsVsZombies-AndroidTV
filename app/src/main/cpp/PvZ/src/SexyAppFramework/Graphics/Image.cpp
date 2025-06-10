//
// Created by 28636 on 2025/5/26.
//

#include "PvZ/SexyAppFramework/Graphics/Image.h"

using namespace Sexy;

template<>
int Image::GetWidth()
{
    return mWidth;
}

template<>
int	Image::GetHeight()
{
    return mHeight;
}

template<>
int Image::GetCelHeight()
{
    return mHeight / mNumRows;
}

template<>
int Image::GetCelWidth()
{
    return mWidth / mNumCols;
}

template<>
void Image::PushTransform(const SexyMatrix3 &theTransform, bool concatenate) {
    old_Sexy_Image_PushTransform(this, theTransform, concatenate);
}

template<>
void Image::PopTransform() {
    old_Sexy_Image_PopTransform(this);
}

void GLImage::PushTransform(const SexyMatrix3 &theTransform, bool concatenate) {
    old_Sexy_GLImage_PushTransform(this, theTransform, concatenate);
}

void GLImage::PopTransform() {
    old_Sexy_GLImage_PopTransform(this);
}