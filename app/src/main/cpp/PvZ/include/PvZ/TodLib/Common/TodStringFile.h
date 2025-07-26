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

#ifndef PVZ_SEXYAPPFRAMEWORK_TODLIB_COMMON_TOD_STRING_FILE_H
#define PVZ_SEXYAPPFRAMEWORK_TODLIB_COMMON_TOD_STRING_FILE_H

#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/Symbols.h"

inline void TodDrawStringWrapped(
    Sexy::Graphics *g, const pvzstl::string &theText, const Sexy::Rect &theRect, Sexy::Font *theFont, const Sexy::Color &theColor, DrawStringJustification theJustification, bool drawString) {
    reinterpret_cast<void (*)(Sexy::Graphics *, const pvzstl::string &, const Sexy::Rect &, Sexy::Font *, const Sexy::Color &, DrawStringJustification, bool)>(TodDrawStringWrappedAddr)(
        g, theText, theRect, theFont, theColor, theJustification, drawString);
}

inline int TodDrawStringWrappedHelper(
    Sexy::Graphics *g, const pvzstl::string &theText, const Sexy::Rect &theRect, Sexy::Font *theFont, const Sexy::Color &theColor, DrawStringJustification theJustification, bool drawString, bool i2) {
    return reinterpret_cast<int (*)(Sexy::Graphics *, const pvzstl::string &, const Sexy::Rect &, Sexy::Font *, const Sexy::Color &, DrawStringJustification, bool, bool)>(
        TodDrawStringWrappedHelperAddr)(g, theText, theRect, theFont, theColor, theJustification, drawString, i2);
}

inline pvzstl::string TodStringTranslate(const char *theString) {
    pvzstl::string str;
    reinterpret_cast<void (*)(pvzstl::string &, const char *)>(TodStringTranslateAddr)(str, theString);
    return str;
}

inline void TodStringListLoad(const char *theFileName) {
    reinterpret_cast<void (*)(const char *)>(TodStringListLoadAddr)(theFileName);
}

inline pvzstl::string TodStringListFind(const pvzstl::string &theName) {
    pvzstl::string result;
    reinterpret_cast<void (*)(pvzstl::string &, const pvzstl::string &)>(TodStringListFindAddr)(result, theName);
    return result;
}

// inline void TodStringListLoad(const char* theFileName) {
//     reinterpret_cast<void (*)(const char*)>(TodStringListLoadAddr)(theFileName);
// }

#endif // PVZ_SEXYAPPFRAMEWORK_TODLIB_COMMON_TOD_STRING_FILE_H
