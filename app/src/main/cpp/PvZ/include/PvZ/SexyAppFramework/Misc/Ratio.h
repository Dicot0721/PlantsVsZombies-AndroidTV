//
// Created by 28636 on 2025/5/28.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_RATIO_H
#define PLANTSVSZOMBIES_ANDROIDTV_RATIO_H

#include "PvZ/Symbols.h"

namespace Sexy {

struct Ratio {
    void Set(int theNumerator, int theDenominator) {
        reinterpret_cast<void (*)(Ratio *, int, int)>(Sexy_Ratio_SetAddr)(this, theNumerator, theDenominator);
    }
};

} // namespace Sexy

#endif // PLANTSVSZOMBIES_ANDROIDTV_RATIO_H
