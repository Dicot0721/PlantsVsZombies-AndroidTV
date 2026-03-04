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

#include <dlfcn.h>

#include <cassert>

SharedLibLoader::SharedLibLoader(const char *filename)
    : handle_{dlopen(filename, RTLD_NOW | RTLD_NOLOAD)} {
    if (!IsOpen()) {
        LOG_ERROR("Failed to load shared library '{}': {}", filename, dlerror());
    }
}

SharedLibLoader::~SharedLibLoader() {
    if (IsOpen()) {
        dlclose(handle_);
    }
}

SharedLibLoader &SharedLibLoader::operator=(SharedLibLoader &&other) noexcept {
    std::swap(handle_, other.handle_);
    return *this;
}

void SharedLibLoader::GetSymbolImpl(const char *name, void *&output) const {
    void *ptr = dlsym(handle_, name);
    assert((ptr != nullptr) && "failed to get symbol");
    output = ptr;
}
