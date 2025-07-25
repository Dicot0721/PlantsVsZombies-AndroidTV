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
