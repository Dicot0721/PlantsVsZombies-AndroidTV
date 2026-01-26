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