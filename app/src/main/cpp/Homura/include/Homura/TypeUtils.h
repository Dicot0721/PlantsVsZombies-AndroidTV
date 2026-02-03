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

#ifndef HOMURA_TYPEUTILS_H
#define HOMURA_TYPEUTILS_H

#include <concepts>

namespace homura {

template <typename Base, std::derived_from<Base> Derived>
Derived *DynamicCast(Base *ptr) noexcept {
    if (ptr == nullptr) {
        return nullptr;
    }
    Derived *const derivedPtr = reinterpret_cast<Derived *>(alignof(Derived));
    Base *const basePtr = derivedPtr;
    return reinterpret_cast<Derived *>(uintptr_t(ptr) - (uintptr_t(basePtr) - uintptr_t(derivedPtr)));
}

} // namespace homura

#endif // HOMURA_TYPEUTILS_H
