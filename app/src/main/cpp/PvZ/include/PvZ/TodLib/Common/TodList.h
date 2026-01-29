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

#ifndef PVZ_SEXYAPPFRAMEWORK_TODLIB_COMMON_TOD_LIST_H
#define PVZ_SEXYAPPFRAMEWORK_TODLIB_COMMON_TOD_LIST_H

#include <cassert>

constexpr int MAX_GLOBAL_ALLOCATORS = 128;

class TodAllocator {
public:
    void *mFreeList;
    void *mBlockList;
    int mGrowCount;
    int mTotalItems;
    int mItemSize;

    void Initialize(int theGrowCount, int theItemSize) noexcept;
    void Dispose() noexcept;
    void FreeAll() noexcept;
    void *Alloc(int theItemSize);
    void *Calloc(int theItemSize);
    void Free(void *theItem, int theItemSize) noexcept;
    void Grow();
    bool IsPointerFromAllocator(void *theItem) const noexcept;
    bool IsPointerOnFreeList(void *theItem) const noexcept;
};

template <typename T>
struct TodListNode {
public:
    T mValue;
    TodListNode *mNext;
    TodListNode *mPrev;
};

template <typename T>
class TodList {
public:
    TodListNode<T> *mHead = nullptr;
    TodListNode<T> *mTail = nullptr;
    int mSize = 0;
    TodAllocator *mpAllocator = nullptr;

    ~TodList() {
        RemoveAll();
    }

    TodListNode<T> *GetHead() noexcept {
        return mHead;
    }

    TodListNode<T> *GetTail() noexcept {
        return mTail;
    }

    T RemoveHead() {
        auto *aHead = mHead;
        auto *aSecNode = aHead->mNext;
        mHead = aSecNode;
        if (aSecNode) {
            aSecNode->mPrev = nullptr;
        } else {
            mTail = nullptr;
        }
        T aVal = aHead->mValue;
        --mSize;
        mpAllocator->Free(aHead, sizeof(TodListNode<T>));
        return aVal;
    }

    TodListNode<T> *RemoveAt(TodListNode<T> *theNode) noexcept {
        if (theNode->mPrev != nullptr) {
            theNode->mPrev->mNext = theNode->mNext;
        } else {
            mHead = theNode->mNext;
        }

        if (theNode->mNext != nullptr) {
            theNode->mNext->mPrev = theNode->mPrev;
        } else {
            mTail = theNode->mPrev;
        }

        --mSize;
        mpAllocator->Free(theNode, sizeof(TodListNode<T>));
        return theNode->mNext;
    }

    TodListNode<T> *Find(const T &theItem) {
        for (auto *aNode = mHead; aNode != nullptr; aNode = aNode->mNext) {
            if (aNode->mValue == theItem) {
                return aNode;
            }
        }
        return nullptr;
    }

    void RemoveAll() noexcept {
        for (auto *aNode = mHead; aNode != nullptr;) {
            auto *temp = aNode;
            aNode = aNode->mNext;
            mpAllocator->Free(temp, sizeof(TodListNode<T>));
        }
        mSize = 0;
        mHead = nullptr;
        mTail = nullptr;
    }

    void SetAllocator(TodAllocator *theAllocator) noexcept {
        mpAllocator = theAllocator;
    }
};

#endif // PVZ_SEXYAPPFRAMEWORK_TODLIB_COMMON_TOD_LIST_H
