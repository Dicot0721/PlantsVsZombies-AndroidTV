//
// Created by 28636 on 2025/5/27.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_PROFILEMGR_H
#define PLANTSVSZOMBIES_ANDROIDTV_PROFILEMGR_H

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

    DefaultPlayerInfo *GetAnyProfile() { return reinterpret_cast<DefaultPlayerInfo *(*)(DefaultProfileMgr *)>(Sexy_DefaultProfileMgr_GetAnyProfileAddr)(this); }
};

} // namespace Sexy

#endif // PLANTSVSZOMBIES_ANDROIDTV_PROFILEMGR_H
