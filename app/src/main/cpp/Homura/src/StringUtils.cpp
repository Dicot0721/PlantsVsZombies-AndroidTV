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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations" // warning: 'xxx' is deprecated

#include "Homura/StringUtils.h"

#include <cctype>

#include <codecvt>
#include <locale>

namespace homura::inline string {

[[nodiscard]] std::string ToLower(std::string_view sv) {
    return {
        std::from_range,
        sv | std::views::transform([](unsigned char c) { return std::tolower(c); }),
    };
}

[[nodiscard]] std::string ToUpper(std::string_view sv) {
    return {
        std::from_range,
        sv | std::views::transform([](unsigned char c) { return std::toupper(c); }),
    };
}

[[nodiscard]] std::wstring StringToWString(std::string_view sv) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(sv.data());
}

[[nodiscard]] std::string WStringToString(std::wstring_view sv) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(sv.data());
}

} // namespace homura::inline string

#pragma clang diagnostic pop
