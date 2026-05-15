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

#ifndef PVZ_SEXYAPPFRAMEWORK_MISC_COMMON_H
#define PVZ_SEXYAPPFRAMEWORK_MISC_COMMON_H

#include "Homura/TypeUtils.h"
#include "PvZ/STL/string.h"
#include "PvZ/Symbols.h"

#include <cstdarg>

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

inline int Rand(int range) { // 取 [0, range) 的随机整数
    return reinterpret_cast<int (*)(int)>(Sexy_RandIntAddr)(range);
}

inline float Rand(float range) {
    return reinterpret_cast<float (*)(float)>(Sexy_RandFloatAddr)(range);
}

inline int GetTickCount() {
    return reinterpret_cast<int (*)()>(Sexy_GetTickCountAddr)();
}

inline void vformat(homura::Storage<pvzstl::string> &result, const char *fmt, std::va_list vList) {
    reinterpret_cast<void (*)(homura::Storage<pvzstl::string> &, const char *, std::va_list)>(Sexy_vformatAddr)(result, fmt, vList);
}

[[gnu::format(printf, 1, 2)]] inline pvzstl::string StrFormat(const char *fmt, ...) {
    homura::AutoDestructStorage<pvzstl::string> result;
    std::va_list args;
    va_start(args, fmt);
    vformat(result, fmt, args);
    va_end(args);
    return *std::move(result);
}

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_MISC_COMMON_H
