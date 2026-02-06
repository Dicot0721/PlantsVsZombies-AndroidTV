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

template <template <typename...> typename Map, typename Key, typename T>
concept IsMap = std::is_same_v<std::iter_value_t<typename Map<Key, T>::const_iterator>, std::pair<const Key, T>> //
    && requires(const Map<Key, T> &map, const Key &key) {
           { map.find(key) } -> std::same_as<typename Map<Key, T>::const_iterator>;
           { map.end() } -> std::same_as<typename Map<Key, T>::const_iterator>;
       };

template <template <typename...> typename Map, typename Key, typename T>
    requires IsMap<Map, Key, T>
[[nodiscard]] std::optional<T> FindInMap(const Map<Key, T> &map, std::type_identity_t<const Key &> key) {
    auto it = map.find(key);
    return (it != map.end()) ? std::optional<T>(it->second) : std::nullopt;
}

template <template <typename...> typename Map, typename Key, typename T>
    requires IsMap<Map, Key, T>
bool FindInMap(const Map<Key, T> &map, std::type_identity_t<const Key &> key, T &output) {
    auto it = map.find(key);
    if (it == map.end()) {
        return false;
    }
    output = it->second;
    return true;
}

} // namespace homura::inline container

#endif // HOMURA_CONTAINER_H
