#include "PvZ/TodLib/Common/TodList.h"

void TodAllocator::Free(void *theItem, int theItemSize) {
    mTotalItems--;
    *(void **)theItem = mFreeList; // 将原可用区域头存入 [*theItem] 中
    mFreeList = theItem;           // 将 theItem 设为新的可用区域头
}