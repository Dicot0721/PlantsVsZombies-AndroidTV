#ifndef PVZ_SEXYAPPFRAMEWORK_MISC_RATIO_H
#define PVZ_SEXYAPPFRAMEWORK_MISC_RATIO_H

#include "PvZ/Symbols.h"

namespace Sexy {

struct Ratio {
    void Set(int theNumerator, int theDenominator) {
        reinterpret_cast<void (*)(Ratio *, int, int)>(Sexy_Ratio_SetAddr)(this, theNumerator, theDenominator);
    }
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_MISC_RATIO_H
