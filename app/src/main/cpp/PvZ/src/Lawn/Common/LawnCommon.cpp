#include "PvZ/Lawn/Common/LawnCommon.h"
#include "stdlib.h"

// 判断在 [theNumber - theRange, theNumber + theRange] 区间内是否存在 theMod 的整数倍数
bool ModInRange(int theNumber, int theMod, int theRange) {
    theRange = abs(theRange);
    for (int i = theNumber - theRange; i <= theNumber + theRange; i++)
        if (i % theMod == 0)
            return true;
    return false;
}

// 判断点 (x1, y1) 是否位于点 (x2, y2) 周围的 (theRangeX, theRangeY) 范围内
bool GridInRange(int x1, int y1, int x2, int y2, int theRangeX, int theRangeY) {
    return x1 >= x2 - theRangeX && x1 <= x2 + theRangeX && y1 >= y2 - theRangeY && y1 <= y2 + theRangeY;
}