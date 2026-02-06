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

#ifndef HOMURA_STRINGUTILS_H
#define HOMURA_STRINGUTILS_H

#include <ranges>
#include <string>
#include <vector>

namespace homura::inline string {

/**
 * @warning 字符串含非 ASCII 字符时可能会出现意外
 */
constexpr std::string Replace(std::string_view sv, std::string_view old, std::string_view new_) {
    std::string result(sv);
    for (size_t pos = 0; (pos = result.find(old, pos)) != std::string::npos;) {
        result.replace(pos, old.length(), new_);
        pos += new_.length();
    }
    return result;
}

/**
 * @warning 字符串含非 ASCII 字符时可能会出现意外
 */
constexpr std::vector<std::string> Split(std::string_view sv, char delim) {
    std::vector<std::string> result;
    result.reserve(std::ranges::count(sv, delim) + 1);

    size_t start = 0;
    for (size_t pos; (pos = sv.find(delim, start)) != std::string_view::npos;) {
        result.emplace_back(sv.substr(start, pos - start));
        start = pos + 1;
    }
    result.emplace_back(sv.substr(start));
    return result;
}

/**
 * @warning 字符串含非 ASCII 字符时可能会出现意外
 */
constexpr std::vector<std::string> Split(std::string_view sv, std::string_view sep) {
    std::vector<std::string> result;
    auto view = std::views::split(sv, sep);
    result.reserve(std::ranges::distance(view));
    for (const auto &word : view) {
        result.emplace_back(std::from_range, word);
    }
    return result;
}

constexpr bool IsSpace(char c) noexcept {
    using namespace std::string_view_literals;
    // Whitespace characters as classified by the classic "C" locale
    return " \f\n\r\t\v"sv.contains(c);
}

/**
 * @warning 字符串含非 ASCII 字符时可能会出现意外
 */
constexpr std::string Trim(std::string_view sv) {
    auto view = sv                        //
        | std::views::drop_while(IsSpace) //
        | std::views::reverse             //
        | std::views::drop_while(IsSpace) //
        | std::views::reverse;
    return {std::from_range, view};
}

/**
 * @warning 字符串含非 ASCII 字符时可能会出现意外
 */
constexpr std::string TrimLeft(std::string_view sv) {
    return {std::from_range, std::views::drop_while(sv, IsSpace)};
}

/**
 * @warning 字符串含非 ASCII 字符时可能会出现意外
 */
constexpr std::string TrimRight(std::string_view sv) {
    auto view = sv                        //
        | std::views::reverse             //
        | std::views::drop_while(IsSpace) //
        | std::views::reverse;
    return {std::from_range, view};
}

} // namespace homura::inline string

#endif // HOMURA_STRINGUTILS_H
