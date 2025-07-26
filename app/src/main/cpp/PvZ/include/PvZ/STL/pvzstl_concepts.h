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

#ifndef PVZ_STL_PVZSTL_CONCEPTS_H
#define PVZ_STL_PVZSTL_CONCEPTS_H

#include <type_traits>

namespace pvzstl {

// clang-format off
template <typename T>
concept __character = std::disjunction_v<
    std::is_same<T, char>,
    std::is_same<T, wchar_t>,
    std::is_same<T, char8_t>,
    std::is_same<T, char16_t>,
    std::is_same<T, char32_t>
>;
// clang-format on

} // namespace pvzstl

#endif // PVZ_STL_PVZSTL_CONCEPTS_H
