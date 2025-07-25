#ifndef PVZ_SEXYAPPFRAMEWORK_MISC_PROFILE_MGR_H
#define PVZ_SEXYAPPFRAMEWORK_MISC_PROFILE_MGR_H

#include "PvZ/Symbols.h"

#include "PvZ/Lawn/System/PlayerInfo.h"

namespace Sexy {

class ProfileMgr {
public:
    int *vTable;                // 0
    int *mProfileEventListener; // 1
};

class DefaultProfileMgr : public ProfileMgr {
public:
    int *mProfileMap;       // 2
    int *mProfileMapEnd;    // 3
    int mNumProfiles;       // 7
    int mNextProfileUseSeq; // 9

    DefaultPlayerInfo *GetAnyProfile() {
        return reinterpret_cast<DefaultPlayerInfo *(*)(DefaultProfileMgr *)>(Sexy_DefaultProfileMgr_GetAnyProfileAddr)(this);
    }
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_MISC_PROFILE_MGR_H
