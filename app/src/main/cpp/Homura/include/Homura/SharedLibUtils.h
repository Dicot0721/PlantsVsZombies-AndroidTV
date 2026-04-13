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

    /**
     * Note: 即使未发生错误, 输出仍可能为空指针. (此时找到的符号的值本身就是 NULL)
     *
     * @retval false 仅在发生错误时返回 false.
     */
    bool GetSymbol(const char *name, auto *&output) const {
        return GetSymbolImpl(name, reinterpret_cast<void *&>(output));
    }

protected:
    bool GetSymbolImpl(const char *name, void *&output) const;

    void *handle_ = nullptr;
};

} // namespace homura

#endif // HOMURA_SHAREDLIBUTILS_H
