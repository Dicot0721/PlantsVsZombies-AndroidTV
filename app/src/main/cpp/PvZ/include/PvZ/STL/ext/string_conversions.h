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

#ifndef PVZ_STL_EXT_STRING_CONVERSIONS_H
#define PVZ_STL_EXT_STRING_CONVERSIONS_H

/**
 * @file string_conversions.h
 * https://gcc.gnu.org/onlinedocs/gcc-4.9.4/libstdc++/api/a01277.html#aa2909347e3b9614ab08f9493159eadac
 */

#include <cstdarg>
#include <cstdio>
#include <cwchar>

#include <limits>

namespace pvzstl {

// Helper for the to_string / to_wstring functions.
template <typename String, std::size_t N, typename CharT = typename String::value_type>
[[nodiscard]] String _to_xstring(int (*convf)(CharT *, std::size_t, const CharT *, std::va_list), const CharT *fmt, ...) {
    CharT buf[sizeof(CharT) * N];
    std::va_list args;
    va_start(args, fmt);
    const int len = convf(buf, N, fmt, args);
    va_end(args);
    return String(buf, len);
}

} // namespace pvzstl

#endif // PVZ_STL_EXT_STRING_CONVERSIONS_H
