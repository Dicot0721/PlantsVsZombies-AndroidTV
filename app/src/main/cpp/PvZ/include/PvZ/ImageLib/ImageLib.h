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

#ifndef PVZ_IMAGELIB_IMAGE_LIB_H
#define PVZ_IMAGELIB_IMAGE_LIB_H

#include <string>

namespace ImageLib {

class Image {
public:
    void **vTable;       // Offset 0: Virtual table pointer
    int mWidth;          // Offset 8: Image width
    int mHeight;         // Offset 12: Image height
    unsigned int *mBits; // Offset 16: Pixel data (ARGB)
    int *mText1;         // Offset 24: Text/string pointer 1
    int *mText2;         // Offset 32: Text/string pointer 2
    bool unkBool;        // Offset 40: Unknown boolean flag
    // Padding: 3 bytes (auto-alignment)
    int unk1[256];       // Offset 44: Unknown array of 256 integers
    int unkInt;          // Offset 1068: Unknown integer
};
// Total size: ~1072 bytes (64-bit) or ~1060 bytes (32-bit)

} // namespace ImageLib


#endif // PVZ_IMAGELIB_IMAGE_LIB_H
