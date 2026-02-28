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

#include "PvZ/SexyAppFramework/Graphics/MemoryImage.h"

using namespace Sexy;

void MemoryImage::ClearRect(const Rect &theRect) {
    int mY;     // r4
    int *mBits; // r7
    int v7;     // r0

    mBits = (int *)(*((int (**)(Sexy::Image *))this->vTable + 13))(this);
    mY = theRect.mY;
    if (mY < theRect.mHeight + mY) {
        do {
            v7 = theRect.mX + (*((int (**)(Sexy::Image *))this->vTable + 6))(this) * mY++;
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