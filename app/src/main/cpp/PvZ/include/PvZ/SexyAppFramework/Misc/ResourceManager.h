/*
 * Copyright (C) 2023-2025  PvZ TV Touch Team
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

#ifndef PVZ_SEXYAPPFRAMEWORK_MISC_RESOURCE_MANAGER_H
#define PVZ_SEXYAPPFRAMEWORK_MISC_RESOURCE_MANAGER_H

#include "PvZ/STL/pvzstl_string.h"

namespace Sexy {

class ResourceManager {
protected:
    enum ResType {
        ResType_Image,
        ResType_Sound,
        ResType_Font,
        ResType_Music,
        ResType_Reanim,
        ResType_Particle,
        ResType_Trail,
    };

    struct BaseRes {
        int *vTable;       // 0
        ResType mType;     // 1
        int *mResourceRef; // 2
        int unk[8];        // 3 ~ 10
        char *mPath;       // 11
        int unk2[13];      // 12 ~ 24
    }; // 大小25个整数

    struct SoundRes : public BaseRes {
        int mSoundId;   // 25
        double mVolume; // 26 ~ 27
        int mPanning;   // 28
        int unk3;       // 29
    }; // 大小30个整数

public:
    int GetSoundThrow(const pvzstl::string &result, const pvzstl::string &theId) {
        return reinterpret_cast<int (*)(const pvzstl::string &, const pvzstl::string &)>(Sexy_ResourceManager_GetSoundThrowAddr)(result, theId);
    }
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_MISC_RESOURCE_MANAGER_H
