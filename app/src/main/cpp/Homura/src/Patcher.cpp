#include "Homura/Patcher.h"
#include "Homura/Logger.h"

#include <sys/mman.h>

#include <cassert>
#include <cstdint>
#include <cstring>

#include <fstream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace homura {

bool SetProtection(uintptr_t address, size_t length, int prot) {
    long pageSize = sysconf(_SC_PAGESIZE);
    uintptr_t alignedAddr = address & ~uintptr_t(pageSize - 1); // 向下对齐内存页
    return mprotect(reinterpret_cast<void *>(alignedAddr), length, prot) == 0;
}

Patcher::Patcher(std::string libName, uintptr_t offset, bool isUseMap, std::vector<uint8_t> patchCode)
    : _libName(std::move(libName))
    , _patchCode(std::move(patchCode))
    , _originCode(_patchCode.size()) {
    if (uintptr_t baseAddr = GetBaseAddress(_libName, isUseMap); baseAddr != 0) {
        _address = baseAddr + offset;
    }
    if (_address != 0 && !_patchCode.empty()) {
        std::memcpy(_originCode.data(), reinterpret_cast<void *>(_address), _patchCode.size());
    }
}

Patcher Patcher::CreateWithStr(std::string libName, uintptr_t offset, bool isUseMap, std::string patchCodeStr) {
    std::vector<uint8_t> patchCode;
    std::istringstream stream{std::move(patchCodeStr)};
    bool isTooBig = false;

    // 'byte' cannot be uint8_t (aka: unsigned char)
    for (uint16_t byte; !stream.eof() && (stream >> std::hex >> byte);) {
        if (byte > std::numeric_limits<uint8_t>::max()) {
            isTooBig = true;
            break;
        }
        patchCode.push_back(byte);
    }

    if (stream.fail() || isTooBig) {
        assert(false && "failed to convert patch code string into bytes");
        return {};
    }
    return {std::move(libName), offset, isUseMap, std::move(patchCode)};
}

uintptr_t Patcher::GetBaseAddress(const std::string &libName, bool isUseMap) {
    if (isUseMap) {
        auto it = _baseAddrMap.find(libName);
        if (it != _baseAddrMap.end() && it->second != 0) {
            return it->second;
        }
    }

    static constexpr char mapsPath[] = "/proc/self/maps";
    std::ifstream mapsFile{mapsPath};
    if (!mapsFile.is_open()) {
        LOG_ERROR("Failed to open [{}].", mapsPath);
        return 0;
    }
    for (std::string line; std::getline(mapsFile, line);) {
        if (line.find(libName) != std::string::npos) {
            uintptr_t baseAddr = std::stoul(line, nullptr, 16);
            if (isUseMap) {
                _baseAddrMap[libName] = baseAddr;
            }
            return baseAddr;
        }
    }
    return 0;
}

void Patcher::UpdateBaseAddrMap(const std::string &libName, uintptr_t baseAddr) {
    _baseAddrMap[libName] = baseAddr;
}

bool Patcher::WriteMemory(uintptr_t address, const std::vector<uint8_t> &buffer) {
    if (address == 0 || buffer.empty()) {
        return false;
    }
    if (!SetProtection(address, buffer.size(), PROT_READ | PROT_WRITE | PROT_EXEC)) {
        return false;
    }
    std::memcpy(reinterpret_cast<void *>(address), buffer.data(), buffer.size());
    SetProtection(address, buffer.size(), PROT_READ | PROT_EXEC);
    return std::memcmp(reinterpret_cast<void *>(address), buffer.data(), buffer.size()) == 0;
}

void Patcher::Modify() {
    if (!_hasModified && WriteMemory(_address, _patchCode)) {
        LOG_DEBUG("Modified address at 0x{:X} successfully.", _address);
        _hasModified = true;
    } else {
        LOG_WARN("Failed to modify address at 0x{:X}.", _address);
    }
}

void Patcher::Restore() {
    if (_hasModified && WriteMemory(_address, _originCode)) {
        LOG_DEBUG("Restored address at 0x{:X} successfully.", _address);
        _hasModified = false;
    } else {
        LOG_WARN("Failed to restore address at 0x{:X}.", _address);
    }
}

} // namespace homura
