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

#ifndef HOMURA_PATCHER_H
#define HOMURA_PATCHER_H

#include <cstdint>

#include <sys/mman.h>

#include <map>
#include <string>
#include <vector>

namespace homura {

/**
 * @brief 修改从地址 address 开始, 长度为 length 字节的内存的访问保护.
 *
 * @param [in] address 目标内存地址(不要求对齐).
 * @param [in] length 目标内存区域大小(不要求对齐).
 * @param [in] prot 新的内存保护标志.
 * @return 修改是否成功.
 */
bool SetProtection(uintptr_t address, size_t length, int prot);

class Patcher {
public:
    Patcher() = default;

    Patcher(std::string libName, uintptr_t offset, bool isUseMap, std::vector<uint8_t> patchCode);

    Patcher(const Patcher &) = delete;
    Patcher &operator=(const Patcher &) = delete;

    Patcher(Patcher &&) = default;
    Patcher &operator=(Patcher &&) = default;

    [[nodiscard]] static Patcher CreateWithStr(std::string libName, uintptr_t offset, bool isUseMap, std::string patchCodeStr);

    /**
     * @brief 获取动态库的加载地址与偏移 offset 相加得到的地址.
     *
     * @param [in] isUseMap 是否使用已存在的加载地址.
     */
    [[nodiscard]] static uintptr_t GetBaseAddress(const std::string &libName, bool isUseMap);

    static void UpdateBaseAddrMap(const std::string &libName, uintptr_t baseAddr);

    [[nodiscard]] bool HasModified() const noexcept {
        return _hasModified;
    }

    void Modify();

    void Restore();

protected:
    static bool WriteMemory(uintptr_t address, const std::vector<uint8_t> &buffer);

    static inline std::map<std::string, uintptr_t> _baseAddrMap;

    std::string _libName;
    uintptr_t _address = 0;
    std::vector<uint8_t> _patchCode;
    std::vector<uint8_t> _originCode;
    bool _hasModified = false;
};

} // namespace homura

#endif // HOMURA_PATCHER_H
