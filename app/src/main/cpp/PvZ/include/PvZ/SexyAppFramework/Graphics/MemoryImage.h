/*
 * Copyright (C) 2023-2025  PvZ TV Touch Team
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

#ifndef PVZ_SEXYAPPFRAMEWORK_GRAPHICS_MEMORY_IMAGE_H
#define PVZ_SEXYAPPFRAMEWORK_GRAPHICS_MEMORY_IMAGE_H

#include "Image.h"

namespace Sexy {

const ulong MEMORYCHECK_ID = 0x4BEEFADE;

class SexyMatrix3;

class MemoryImage : public __Image {
public:
    LawnApp *mApp;         // 28
    int unkMems4;          // 29
    ulong *mBits;          // 30
    int mBitsChangedCount; // 31
    int unk1[4];           // 32 ~ 35
    bool mUnkModeRelated;  // 144
    bool mHasTrans;        // 145
    bool mHasAlpha;        // 146
    bool mUnkBool;         // 147
    bool mIsVolatile;      // 148
    int unk2[20];          // 38 ~ 57
    // 大小58个整数

    MemoryImage() {
        _constructor();
    }
    ~MemoryImage() {
        _destructor();
    }

    ulong *GetBits() {
        return reinterpret_cast<ulong *(*)(MemoryImage *)>(Sexy_MemoryImage_GetBitsAddr)(this);
    }
    void Create(int theWidth, int theHeight) {
        reinterpret_cast<void (*)(MemoryImage *, int, int)>(Sexy_MemoryImage_CreateAddr)(this, theWidth, theHeight);
    }
    void SetImageMode(bool hasTrans, bool hasAlpha) {
        reinterpret_cast<void (*)(MemoryImage *, bool, bool)>(Sexy_MemoryImage_SetImageModeAddr)(this, hasTrans, hasAlpha);
    }
    void WriteToPng(pvzstl::string theString) {
        reinterpret_cast<void (*)(MemoryImage *, pvzstl::string &)>(Sexy_MemoryImage_WriteToPngAddr)(this, theString);
    }
    void WriteToJPEG(int *theString) {
        reinterpret_cast<void (*)(MemoryImage *, int *)>(Sexy_MemoryImage_WriteToJPEGAddr)(this, theString);
    }
    void BitsChanged() {
        reinterpret_cast<void (*)(MemoryImage *)>(Sexy_MemoryImage_BitsChangedAddr)(this);
    }

    void ClearRect(const Rect &theRect);
    void PushTransform(const SexyMatrix3 &theTransform, bool concatenate);
    void PopTransform();

protected:
    void _constructor() {
        reinterpret_cast<void (*)(MemoryImage *)>(Sexy_MemoryImage_MemoryImageAddr)(this);
    }
    void _destructor() {
        reinterpret_cast<void (*)(MemoryImage *)>(Sexy_MemoryImage_DeleteAddr)(this);
    }
};

} // namespace Sexy

inline void (*old_Sexy_MemoryImage_PushTransform)(Sexy::MemoryImage *image, const Sexy::SexyMatrix3 &theTransform, bool concatenate);

inline void (*old_Sexy_MemoryImage_PopTransform)(Sexy::MemoryImage *image);


#endif // PVZ_SEXYAPPFRAMEWORK_GRAPHICS_MEMORY_IMAGE_H
