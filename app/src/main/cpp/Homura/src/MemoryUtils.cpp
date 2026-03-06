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

#include "Homura/MemoryUtils.h"
#include "Homura/Logger.h"

#include <cassert>
#include <cerrno>
#include <cstring>

#include <fstream>
#include <sstream>
#include <unordered_map>
#include <utility>

bool homura::SetProtection(uintptr_t address, size_t length, int prot) {
    const long pageSize = sysconf(_SC_PAGESIZE);
    const uintptr_t alignedAddr = address & ~(pageSize - 1); // 向下对齐内存页
    return mprotect(reinterpret_cast<void *>(alignedAddr), length, prot) == 0;
}

bool homura::WriteMemory(uintptr_t address, const std::vector<uint8_t> &buffer) {
    if (address == 0 || buffer.empty()) [[unlikely]] {
        LOG_ERROR("Invalid argument(s)");
        return false;
    }
    if (!SetProtection(address, buffer.size(), PROT_READ | PROT_WRITE | PROT_EXEC)) [[unlikely]] {
        LOG_ERROR("Failed to set protection: {}", std::strerror(errno));
        return false;
    }
    void *ptr = reinterpret_cast<void *>(address);
    std::memcpy(ptr, buffer.data(), buffer.size());
    SetProtection(address, buffer.size(), PROT_READ | PROT_EXEC);
    return std::memcmp(ptr, buffer.data(), buffer.size()) == 0;
}

uintptr_t homura::GetLibBaseAddr(const std::string &libName) {
    static std::unordered_map<std::string, uintptr_t> baseAddrMap;
    if (auto it = baseAddrMap.find(libName); it != baseAddrMap.end()) {
        return it->second;
    }

    constexpr auto &mapsPath = "/proc/self/maps";
    std::ifstream mapsFile{mapsPath};
    if (!mapsFile.is_open()) {
        LOG_ERROR("Failed to open {:?}", mapsPath);
        return 0;
    }
    for (std::string line; std::getline(mapsFile, line);) {
        if (line.contains(libName)) {
            // On unix-like OS, `sizeof(long) == sizeof(void *)` is always true
            const uintptr_t baseAddr = std::stoul(line, nullptr, 16);
            baseAddrMap.emplace(libName, baseAddr);
            return baseAddr;
        }
    }
    return 0;
}

homura::Patcher::Patcher(std::string libName, uintptr_t offset, std::vector<uint8_t> patchBytes)
    : libName_(std::move(libName))
    , patchBytes_(std::move(patchBytes))
    , originBytes_(patchBytes_.size()) {
    if (uintptr_t baseAddr = GetLibBaseAddr(libName_)) {
        address_ = baseAddr + offset;
    } else {
        LOG_ERROR("Failed to get base address of {:?}", libName);
    }
    if (address_ != 0 && !patchBytes_.empty()) {
        std::memcpy(originBytes_.data(), reinterpret_cast<void *>(address_), patchBytes_.size());
    }
}

[[nodiscard]] auto homura::Patcher::CreateWithStr(std::string libName, uintptr_t offset, std::string patchBytesStr) -> Patcher {
    std::vector<uint8_t> patchCode;
    std::istringstream stream{std::move(patchBytesStr)};
    bool isOutOfRange = false;

    patchCode.reserve((patchBytesStr.size() + 1) / 3);

    // 'byte' cannot be uint8_t (aka: unsigned char)
    for (int16_t byte; !stream.eof() && (stream >> std::hex >> byte);) {
        if (!std::in_range<uint8_t>(byte)) [[unlikely]] {
            isOutOfRange = true;
            break;
        }
        patchCode.push_back(byte);
    }

    if (stream.fail() || isOutOfRange) [[unlikely]] {
        assert(false && "failed to convert patch code string into bytes");
        return {};
    }
    return {std::move(libName), offset, std::move(patchCode)};
}

bool homura::Patcher::Modify() {
    if (hasModified_) [[unlikely]] {
        LOG_WARN("Repeatedly modify address at 0x{:X}", address_);
    }
    if (WriteMemory(address_, patchBytes_)) {
        hasModified_ = true;
        return true;
    }
    LOG_ERROR("Failed to modify address at 0x{:X}.", address_);
    return false;
}

bool homura::Patcher::Restore() {
    if (!hasModified_) [[unlikely]] {
        LOG_WARN("Restoring address at 0x{:X} before modify", address_);
    }
    if (WriteMemory(address_, originBytes_)) {
        hasModified_ = false;
        return true;
    }
    LOG_ERROR("Failed to restore address at 0x{:X}.", address_);
    return false;
}
