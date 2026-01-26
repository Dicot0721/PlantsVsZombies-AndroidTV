/*
 * Copyright (C) 2023-2026  PvZ TV Touch Team
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


    void Load() { // vTable + 4 * 3
        return reinterpret_cast<void (*)(DefaultProfileMgr *)>(Sexy_DefaultProfileMgr_LoadAddr)(this);
    }

    void Save() { // vTable + 4 * 4
        return reinterpret_cast<void (*)(DefaultProfileMgr *)>(Sexy_DefaultProfileMgr_SaveAddr)(this);
    }

    DefaultPlayerInfo *GetProfile(pvzstl::string &name) { // vTable + 4 * 8
        return reinterpret_cast<DefaultPlayerInfo *(*)(DefaultProfileMgr *, pvzstl::string &)>(Sexy_DefaultProfileMgr_GetProfileAddr)(this, name);
    }
    DefaultPlayerInfo *AddProfile(pvzstl::string &name) { // vTable + 4 * 9
        return reinterpret_cast<DefaultPlayerInfo *(*)(DefaultProfileMgr *, pvzstl::string &)>(Sexy_DefaultProfileMgr_AddProfileAddr)(this, name);
    }
    DefaultPlayerInfo *GetAnyProfile() { // vTable + 4 * 10
        return reinterpret_cast<DefaultPlayerInfo *(*)(DefaultProfileMgr *)>(Sexy_DefaultProfileMgr_GetAnyProfileAddr)(this);
    }
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_MISC_PROFILE_MGR_H
