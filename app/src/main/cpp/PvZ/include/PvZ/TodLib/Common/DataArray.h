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

#ifndef PVZ_SEXYAPPFRAMEWORK_TODLIB_COMMON_DATA_ARRAY_H
#define PVZ_SEXYAPPFRAMEWORK_TODLIB_COMMON_DATA_ARRAY_H

#include <cassert>
#include <cstdint>
#include <cstring>

enum : uint32_t {
    DATA_ARRAY_INDEX_MASK = 0x0000'FFFF,
    DATA_ARRAY_KEY_MASK = 0xFFFF'0000,
    DATA_ARRAY_KEY_SHIFT = 16,
    DATA_ARRAY_MAX_SIZE = DATA_ARRAY_INDEX_MASK + 1,
    DATA_ARRAY_KEY_FIRST = 1,
};

template <typename T>
class DataArray {
public:
    struct DataArrayItem {
        T mItem;
        uint32_t mID;
    };

    DataArrayItem *mBlock = nullptr;
    uint32_t mMaxUsedCount = 0u;
    uint32_t mMaxSize = 0u;
    uint32_t mFreeListHead = 0u;
    uint32_t mSize = 0u;
    uint32_t mNextKey = 1u;
    const char *mName = nullptr;

    ~DataArray() {
        DataArrayDispose();
    }

    void DataArrayInitialize(uint32_t theMaxSize, const char *theName) {
        assert(mBlock == nullptr);
        mBlock = reinterpret_cast<DataArrayItem *>(::operator new(sizeof(DataArrayItem) * theMaxSize));
        mMaxSize = theMaxSize;
        mNextKey = 1001u;
        mName = theName;
    }

    void DataArrayDispose() noexcept {
        if (mBlock != nullptr) {
            DataArrayFreeAll();
            ::operator delete(mBlock);
            mBlock = nullptr;
            mMaxUsedCount = 0u;
            mMaxSize = 0u;
            mFreeListHead = 0u;
            mSize = 0u;
            mName = nullptr;
        }
    }

    void DataArrayFree(T *theItem) noexcept {
        auto *aItem = reinterpret_cast<DataArrayItem *>(theItem);
        assert(DataArrayGet(aItem->mID) == theItem);
        theItem->~T();
        uint32_t anId = aItem->mID & DATA_ARRAY_INDEX_MASK;
        aItem->mID = mFreeListHead;
        mFreeListHead = anId;
        --mSize;
    }

    void DataArrayFreeAll() noexcept {
        for (T *aItem = nullptr; IterateNext(aItem);) {
            DataArrayFree(aItem);
        }
        mFreeListHead = 0u;
        mMaxUsedCount = 0u;
    }

    uint32_t DataArrayGetID(const T *theItem) /* const */ noexcept {
        auto *aItem = reinterpret_cast<const DataArrayItem *>(theItem);
        assert(DataArrayGet(aItem->mID) == theItem);
        return aItem->mID;
    }

    bool IterateNext(T *&theItem) const noexcept {
        auto *aItem = reinterpret_cast<DataArrayItem *>(theItem);
        if (aItem == nullptr) {
            aItem = &mBlock[0];
        } else {
            ++aItem;
        }
        const DataArrayItem *aLast = &mBlock[mMaxUsedCount];
        for (; aItem < aLast; ++aItem) {
            if (aItem->mID & DATA_ARRAY_KEY_MASK) {
                theItem = reinterpret_cast<T *>(aItem);
                return true;
            }
        }
        return false;
    }

    T *DataArrayAlloc() {
        assert(mSize < mMaxSize);
        assert(mFreeListHead <= mMaxUsedCount);
        uint32_t aNext = mMaxUsedCount;
        if (mFreeListHead == mMaxUsedCount) {
            mFreeListHead = ++mMaxUsedCount;
        } else {
            aNext = mFreeListHead;
            mFreeListHead = mBlock[aNext].mID;
        }

        DataArrayItem *aNewItem = &mBlock[aNext];
        std::memset(aNewItem, 0, sizeof(DataArrayItem));
        aNewItem->mID = (mNextKey++ << DATA_ARRAY_KEY_SHIFT) | aNext;
        if (mNextKey == DATA_ARRAY_MAX_SIZE) {
            mNextKey = 1;
        }
        ++mSize;

        return new (aNewItem) T();
    }

    T *DataArrayTryToGet(uint32_t theId) noexcept {
        if (theId == 0 || (theId & DATA_ARRAY_INDEX_MASK) >= mMaxSize) {
            return nullptr;
        }
        DataArrayItem *aBlock = &mBlock[theId & DATA_ARRAY_INDEX_MASK];
        return (aBlock->mID == theId) ? &aBlock->mItem : nullptr;
    }

    T *DataArrayGet(uint32_t theId) noexcept {
        // assert(DataArrayTryToGet(theId) != nullptr); // useless in net play
        return &mBlock[uint16_t(theId)].mItem;
    }
};

#endif // PVZ_SEXYAPPFRAMEWORK_TODLIB_COMMON_DATA_ARRAY_H
