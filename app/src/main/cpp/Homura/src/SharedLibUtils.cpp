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

#include "Homura/SharedLibUtils.h"
#include "Homura/Logger.h"
#include "Homura/StringUtils.h"

#include <dlfcn.h>

#include <cassert>

#include <map>
#include <unordered_set>

homura::SharedLibLoader::SharedLibLoader(const char *filename) {
    assert((filename != nullptr) && (*filename != '\0') && !IsBlank(filename));
    handle_ = dlopen(filename, RTLD_NOW | RTLD_NOLOAD);
    if (!IsOpen()) [[unlikely]] {
        LOG_ERROR("Failed to load shared library '{}': {}", filename, dlerror());
    }
}

homura::SharedLibLoader::~SharedLibLoader() {
    if (IsOpen()) {
        dlclose(handle_);
    }
}

auto homura::SharedLibLoader::operator=(SharedLibLoader &&other) noexcept -> SharedLibLoader & {
    std::swap(handle_, other.handle_);
    return *this;
}

bool homura::SharedLibLoader::GetSymbolImpl(const char *name, void *&output) const {
    assert((name != nullptr) && (*name != '\0') && !IsBlank(name));
#ifdef PVZ_DEBUG
    // Using 'string_view' is usually safe, because we usually call this function with a string literal
    static std::map<void *, std::unordered_set<std::string_view>> symbolMap;
    auto &symbolSet = symbolMap[handle_];
    if (std::string_view view{name}; !symbolSet.contains(view)) {
        symbolSet.emplace(view);
    } else {
        LOG_WARN("Rebinding symbol {:?}", view);
    }
#endif
    dlerror(); // clear the error
    if ((output = dlsym(handle_, name)) != nullptr) {
        return true;
    }
    if (const char *msg = dlerror()) {
        LOG_ERROR("Failed to get symbol {:?}: {}", name, msg);
        return false;
    }
    LOG_WARN("The value of symbol {:?} is NULL", name);
    return true;
}
