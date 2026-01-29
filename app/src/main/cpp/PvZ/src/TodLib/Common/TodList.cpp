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

#include "PvZ/TodLib/Common/TodList.h"

#include <cstdint>
#include <cstring>

void TodAllocator::Initialize(int theGrowCount, int theItemSize) noexcept {
    assert(theItemSize >= sizeof(void *));
    mFreeList = nullptr;
    mBlockList = nullptr;
    mGrowCount = theGrowCount;
    mTotalItems = 0;
    mItemSize = theItemSize;
}

void TodAllocator::Dispose() noexcept {
    FreeAll();
}

void TodAllocator::Grow() {
    assert(mGrowCount > 0);
    assert(mItemSize >= sizeof(void *));
    void *aBlock = ::operator new(mGrowCount * mItemSize + 4);
    *reinterpret_cast<void **>(aBlock) = mBlockList;
    mBlockList = aBlock;

    void *aFreeList = mFreeList;
    void *aItem = reinterpret_cast<void *>(uintptr_t(aBlock) + 4);
    for (int i = 0; i < mGrowCount; ++i) {
        *reinterpret_cast<void **>(aItem) = aFreeList;
        aFreeList = aItem;
        aItem = reinterpret_cast<void *>(uintptr_t(aItem) + mItemSize);
    }
    mFreeList = aFreeList;
}

bool TodAllocator::IsPointerFromAllocator(void *theItem) const noexcept {
    size_t aBlockSize = mGrowCount * mItemSize;
    for (void *aPtr = mBlockList; aPtr != nullptr; aPtr = *reinterpret_cast<void **>(aPtr)) {
        // 区块的首个四字节为额外申请的、用于存储指向下一区块的指针的区域
        uintptr_t aItemPtr = uintptr_t(theItem);
        // 判断 theItem 是否位于当前区块内且指向某一项的区域的起始地址
        uintptr_t aBlockPtr = uintptr_t(aPtr) + sizeof(void *);
        if ((aItemPtr >= aBlockPtr) && (aItemPtr < aBlockPtr + aBlockSize) && ((aItemPtr - aBlockPtr) % mItemSize == 0)) {
            return true;
        }
    }
    return false;
}

bool TodAllocator::IsPointerOnFreeList(void *theItem) const noexcept {
    for (void *aPtr = mFreeList; aPtr != nullptr; aPtr = *reinterpret_cast<void **>(aPtr)) {
        if (theItem == aPtr) {
            return true;
        }
    }
    return false;
}

void *TodAllocator::Alloc(int theItemSize) {
    ++mTotalItems;
    if (mFreeList == nullptr) {
        Grow();
    }
    void *anItem = reinterpret_cast<void *>(mFreeList);
    mFreeList = *reinterpret_cast<void **>(anItem);
    return anItem;
}

void *TodAllocator::Calloc(int theItemSize) {
    void *anItem = Alloc(theItemSize);
    std::memset(anItem, 0, theItemSize);
    return anItem;
}

void TodAllocator::Free(void *theItem, int theItemSize) noexcept {
    --mTotalItems;
    assert(IsPointerFromAllocator(theItem));
    assert(!IsPointerOnFreeList(theItem));
    *reinterpret_cast<void **>(theItem) = mFreeList; // 将原可用区域头存入 [*theItem] 中
    mFreeList = theItem;                             // 将 theItem 设为新的可用区域头
}

void TodAllocator::FreeAll() noexcept {
    for (void *aBlock = mBlockList; aBlock != nullptr;) {
        void *aNext = *reinterpret_cast<void **>(aBlock);
        ::operator delete(aBlock);
        aBlock = aNext;
    }
    mBlockList = nullptr;
    mFreeList = nullptr;
    mTotalItems = 0;
}
