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

#ifndef HOMURA_SHAREDLIBUTILS_H
#define HOMURA_SHAREDLIBUTILS_H

namespace homura {

class SharedLibLoader {
public:
    SharedLibLoader(const SharedLibLoader &) = delete;

    SharedLibLoader(SharedLibLoader &&other) noexcept
        : handle_{other.handle_} {
        other.handle_ = nullptr;
    }

    explicit SharedLibLoader(const char *filename);
    ~SharedLibLoader();

    SharedLibLoader &operator=(const SharedLibLoader &) = delete;

    SharedLibLoader &operator=(SharedLibLoader &&other) noexcept;

    [[nodiscard]] bool IsOpen() const noexcept {
        return handle_ != nullptr;
    }

    bool GetSymbol(const char *name, auto *&output) const {
        return GetSymbolImpl(name, reinterpret_cast<void *&>(output));
    }

    template <typename T, typename R, typename... Args>
    bool GetSymbol(const char *name, R (T::*&output)(Args...)) const {
        return GetSymbolImpl(name, reinterpret_cast<void *&>(output));
    }

protected:
    void *handle_ = nullptr;

    bool GetSymbolImpl(const char *name, void *&output) const;
};

} // namespace homura

#endif // HOMURA_SHAREDLIBUTILS_H
