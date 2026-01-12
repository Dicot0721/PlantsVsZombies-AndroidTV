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

#ifndef PVZ_SEXYAPPFRAMEWORK_MISC_COMMON_H
#define PVZ_SEXYAPPFRAMEWORK_MISC_COMMON_H

#include "PvZ/STL/pvzstl_string.h"
#include "PvZ/Symbols.h"

#include <algorithm>
#include <cstdlib>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

enum PixelFormat {
    kPixelFormat_None = -1,
    kPixelFormat_Automatic,
    kPixelFormat_RGBA8888,
    kPixelFormat_RGBA4444,
    kPixelFormat_RGBA5551,
    kPixelFormat_RGB565,
    kPixelFormat_RGB888,
    kPixelFormat_L8,
    kPixelFormat_A8,
    kPixelFormat_LA88,
    kPixelFormat_RGB_PVRTC2,
    kPixelFormat_RGB_PVRTC4,
    kPixelFormat_RGBA_PVRTC2,
    kPixelFormat_RGBA_PVRTC4
};

namespace Sexy {

inline int Rand(int range) {
    return reinterpret_cast<int (*)(int)>(Sexy_RandIntAddr)(range);
}

inline float Rand(float range) {
    return reinterpret_cast<float (*)(float)>(Sexy_RandFloatAddr)(range);
}

inline void vformat(pvzstl::string &output, const char *fmt, va_list vList) {
    reinterpret_cast<void (*)(pvzstl::string &, const char *, va_list)>(Sexy_vformatAddr)(output, fmt, vList);
}

inline void InGameStrFormat(pvzstl::string &output, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    reinterpret_cast<void (*)(pvzstl::string &, const char *, ...)>(Sexy_StrFormatAddr)(output, fmt, ap);
    va_end(ap);
}

[[gnu::format(printf, 1, 2)]] inline pvzstl::string StrFormat(const char *fmt, ...) {
    pvzstl::string output;
    va_list args;
    va_start(args, fmt);
    vformat(output, fmt, args);
    va_end(args);
    return output;
}

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_MISC_COMMON_H
