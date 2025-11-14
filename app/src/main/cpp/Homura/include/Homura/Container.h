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

#ifndef HOMURA_CONTAINER_H
#define HOMURA_CONTAINER_H

#include <optional>
#include <unordered_map>

namespace homura {

template <typename Key, typename T>
std::optional<T> FindInHashMap(const std::unordered_map<Key, T> &map, const Key &key) {
    auto it = map.find(key);
    return (it != map.cend()) ? std::optional<T>{it->second} : std::nullopt;
}

template <typename Key, typename T>
bool FindInHashMap(const std::unordered_map<Key, T> &map, const Key &key, T &output) {
    auto it = map.find(key);
    if (it == map.cend()) {
        return false;
    }
    output = it->second;
    return true;
}

} // namespace homura

#endif // HOMURA_CONTAINER_H
