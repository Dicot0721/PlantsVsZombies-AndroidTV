#ifndef PVZ_SEXYAPPFRAMEWORK_SOUND_AUDIERE_SOUND_MANAGER_H
#define PVZ_SEXYAPPFRAMEWORK_SOUND_AUDIERE_SOUND_MANAGER_H

#include "PvZ/STL/pvzstl_string.h"
#include "PvZ/Symbols.h"

namespace Sexy {

class AudiereSoundManager {
public:
    int LoadSound(const pvzstl::string &theFileName) {
        return reinterpret_cast<int (*)(AudiereSoundManager *, const pvzstl::string &)>(Sexy_AudiereSoundManager_LoadSoundAddr)(this, theFileName);
    }
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_SOUND_AUDIERE_SOUND_MANAGER_H
