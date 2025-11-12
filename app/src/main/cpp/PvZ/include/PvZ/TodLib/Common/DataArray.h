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

#include <cstdint>
#include <cstring>

enum {
    DATA_ARRAY_INDEX_MASK = 65535,
    DATA_ARRAY_KEY_MASK = -65536,
    DATA_ARRAY_KEY_SHIFT = 16,
    DATA_ARRAY_MAX_SIZE = 65536,
    DATA_ARRAY_KEY_FIRST = 1,
};

template <typename T>
class DataArray {
public:
    struct DataArrayItem {
        T mItem;
        uint32_t mID;
    };

    DataArrayItem *mBlock;
    uint32_t mMaxUsedCount;
    uint32_t mMaxSize;
    uint32_t mFreeListHead;
    uint32_t mSize;
    uint32_t mNextKey;
    const char *mName;

    DataArray() noexcept {
        mBlock = nullptr;
        mMaxUsedCount = 0U;
        mMaxSize = 0U;
        mFreeListHead = 0U;
        mSize = 0U;
        mNextKey = 1U;
        mName = nullptr;
    }

    ~DataArray() {
        DataArrayDispose();
    }

    void DataArrayInitialize(uint32_t theMaxSize, const char *theName) {
        mBlock = reinterpret_cast<DataArrayItem *>(operator new(sizeof(DataArrayItem) * theMaxSize));
        mMaxSize = theMaxSize;
        mNextKey = 1001U;
        mName = theName;
    }

    void DataArrayDispose() {
        if (mBlock != nullptr) {
            DataArrayFreeAll();
            operator delete(mBlock);
            mBlock = nullptr;
            mMaxUsedCount = 0U;
            mMaxSize = 0U;
            mFreeListHead = 0U;
            mSize = 0U;
            mName = nullptr;
        }
    }

    void DataArrayFree(T *theItem) noexcept {
        auto *aItem = reinterpret_cast<DataArrayItem *>(theItem);
        theItem->~T();
        uint32_t anId = aItem->mID & DATA_ARRAY_INDEX_MASK;
        aItem->mID = mFreeListHead;
        mFreeListHead = anId;
        mSize--;
    }

    void DataArrayFreeAll() noexcept {
        T *aItem = nullptr;
        while (IterateNext(aItem)) {
            DataArrayFree(aItem);
        }
        mFreeListHead = 0U;
        mMaxUsedCount = 0U;
    }

    uint32_t DataArrayGetID(T *theItem) const noexcept {
        auto *aItem = reinterpret_cast<DataArrayItem *>(theItem);
        return aItem->mID;
    }

    bool IterateNext(T *&theItem) const noexcept {
        auto *aItem = reinterpret_cast<DataArrayItem *>(theItem);
        if (aItem == nullptr) {
            aItem = &mBlock[0];
        } else {
            ++aItem;
        }
        DataArrayItem *aLast = &mBlock[mMaxUsedCount];
        while (uintptr_t(aItem) < uintptr_t(aLast)) {
            if (aItem->mID & DATA_ARRAY_KEY_MASK) {
                theItem = reinterpret_cast<T *>(aItem);
                return true;
            }
            ++aItem;
        }
        return false;
    }

    T *DataArrayAlloc() {
        uint32_t aNext = mMaxUsedCount;
        if (mFreeListHead == mMaxUsedCount) {
            mFreeListHead = ++mMaxUsedCount;
        } else {
            aNext = mFreeListHead;
            mFreeListHead = mBlock[mFreeListHead].mID;
        }

        DataArrayItem *aNewItem = &mBlock[aNext];
        std::memset(aNewItem, 0, sizeof(DataArrayItem));
        aNewItem->mID = (mNextKey++ << DATA_ARRAY_KEY_SHIFT) | aNext;
        if (mNextKey == DATA_ARRAY_MAX_SIZE) {
            mNextKey = 1;
        }
        ++mSize;

        new (aNewItem) T();
        return reinterpret_cast<T *>(aNewItem);
    }

    T *DataArrayTryToGet(uint32_t theId) noexcept {
        if (!theId || (theId & DATA_ARRAY_INDEX_MASK) >= mMaxSize) {
            return nullptr;
        }
        DataArrayItem *aBlock = &mBlock[theId & DATA_ARRAY_INDEX_MASK];
        return aBlock->mID == theId ? &aBlock->mItem : nullptr;
    }

    T *DataArrayGet(uint32_t theId) noexcept {
        return &mBlock[uint16_t(theId)].mItem;
    }
};

#endif // PVZ_SEXYAPPFRAMEWORK_TODLIB_COMMON_DATA_ARRAY_H
