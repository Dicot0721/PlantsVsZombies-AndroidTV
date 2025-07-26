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

#ifndef PVZ_IMAGELIB_IMAGE_LIB_H
#define PVZ_IMAGELIB_IMAGE_LIB_H

#include <string>

namespace ImageLib {

class Image {
public:
    int *vTable;         // 0
    int mWidth;          // 1
    int mHeight;         // 2
    unsigned int *mBits; // 3
    int *mText1;         // 4
    int *mText2;         // 5
    bool unkBool;        // 24
    int unk1[256];       // 7 ~ 262
    int unkInt;          // 263
}; // 264个整数

} // namespace ImageLib


#endif // PVZ_IMAGELIB_IMAGE_LIB_H
