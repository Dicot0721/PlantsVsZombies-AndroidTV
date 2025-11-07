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

#ifndef PVZ_SEXYAPPFRAMEWORK_TODLIB_COMMON_DATA_ARRAY_H
#define PVZ_SEXYAPPFRAMEWORK_TODLIB_COMMON_DATA_ARRAY_H

#include <string.h>

#include "TodCommon.h"
#include "TodDebug.h"

enum { DATA_ARRAY_INDEX_MASK = 65535, DATA_ARRAY_KEY_MASK = -65536, DATA_ARRAY_KEY_SHIFT = 16, DATA_ARRAY_MAX_SIZE = 65536, DATA_ARRAY_KEY_FIRST = 1 };

template <typename T>
class DataArray {
public:
    // class DataArrayItem {
    // public:
    // T mItem;
    // unsigned int mID; // 存疑，真的有这个mID么
    // };

public:
    T* mBlock;
    unsigned int mMaxUsedCount;
    unsigned int mMaxSize;
    unsigned int mFreeListHead;
    unsigned int mSize;
    unsigned int mNextKey;
    const char* mName;

public:
    T* DataArrayGet(unsigned int theId) {
        // return mBlock + (short)theId;
        return &mBlock[(short)theId];
    }
};

#endif // PVZ_SEXYAPPFRAMEWORK_TODLIB_COMMON_DATA_ARRAY_H
