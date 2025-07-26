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

#ifndef PVZ_SEXYAPPFRAMEWORK_TODLIB_COMMON_TOD_LIST_H
#define PVZ_SEXYAPPFRAMEWORK_TODLIB_COMMON_TOD_LIST_H

constexpr int MAX_GLOBAL_ALLOCATORS = 128;

struct TodAllocator {
    void* mFreeList;
    void* mBlockList;
    int mGrowCount;
    int mTotalItems;
    int mItemSize;
};
extern int gNumGlobalAllocators;
extern TodAllocator gGlobalAllocators[MAX_GLOBAL_ALLOCATORS];

template <typename T>
class TodListNode {
public:
    T mValue;
    TodListNode<T>* mNext;
    TodListNode<T>* mPrev;
};

template <typename T>
class TodList {
public:
    TodListNode<T>* mHead;
    TodListNode<T>* mTail;
    int mSize;
    TodAllocator* mpAllocator;
};

#endif // PVZ_SEXYAPPFRAMEWORK_TODLIB_COMMON_TOD_LIST_H
