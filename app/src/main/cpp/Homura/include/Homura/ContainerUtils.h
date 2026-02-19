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

#ifndef HOMURA_CONTAINER_H
#define HOMURA_CONTAINER_H

#include <iterator>
#include <optional>

namespace homura::inline container {

template <typename T>
concept IsMapType = (std::is_same_v<typename T::value_type, std::pair<typename T::key_type, typename T::mapped_type>>
                     || std::is_same_v<typename T::value_type, std::pair<const typename T::key_type, typename T::mapped_type>>)
    && requires(const T &map, const typename T::key_type &key) {
           { map.find(key) } -> std::same_as<typename T::const_iterator>;
           { map.end() } -> std::same_as<typename T::const_iterator>;
       };

template <IsMapType T>
[[nodiscard]] auto FindInMap(const T &map, const typename T::key_type &key) -> std::optional<typename T::mapped_type> {
    auto it = map.find(key);
    return (it != map.end()) ? std::optional<typename T::mapped_type>(it->second) : std::nullopt;
}

template <IsMapType T>
bool FindInMap(const T &map, const typename T::key_type &key, std::assignable_from<typename T::mapped_type> auto &&output) {
    auto it = map.find(key);
    if (it == map.end()) {
        return false;
    }
    output = it->second;
    return true;
}

} // namespace homura::inline container

#endif // HOMURA_CONTAINER_H
