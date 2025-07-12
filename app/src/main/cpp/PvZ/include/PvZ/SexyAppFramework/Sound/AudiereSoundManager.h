//
// Created by 28636 on 2025/7/12.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_AUDIERESOUNDMANAGER_H
#define PLANTSVSZOMBIES_ANDROIDTV_AUDIERESOUNDMANAGER_H

#include "PvZ/Symbols.h"
#include "PvZ/STL/string.h"

namespace Sexy {

class AudiereSoundManager {
public:
    int LoadSound(pvzstl::string::pointer *theFileName) {
        return reinterpret_cast<int (*)(AudiereSoundManager *, pvzstl::string::pointer *)>(Sexy_AudiereSoundManager_LoadSoundAddr)(this, theFileName);
    }
};

} // namespace Sexy

#endif // PLANTSVSZOMBIES_ANDROIDTV_AUDIERESOUNDMANAGER_H
