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

#ifndef HOMURA_BITUTILS_H
#define HOMURA_BITUTILS_H

#include <cstdint>
#include <cstring>

#include <bit>

static_assert((std::endian::native == std::endian::big) || (std::endian::native == std::endian::little), "Mixed endian is unsupported");

namespace homura {

template <std::integral T>
[[nodiscard]] T ReadNet(const std::uint8_t *src) {
    T dest;
    std::memcpy(&dest, src, sizeof(T));
    if constexpr (std::endian::native != std::endian::big) {
        dest = std::byteswap(dest);
    }
    return dest;
}

[[nodiscard]] inline std::int16_t ReadNetI16(const std::uint8_t *src) {
    return ReadNet<std::int16_t>(src);
}

[[nodiscard]] inline std::int32_t ReadNetI32(const std::uint8_t *src) {
    return ReadNet<std::int32_t>(src);
}

[[nodiscard]] inline std::int64_t ReadNetI64(const std::uint8_t *src) {
    return ReadNet<std::int64_t>(src);
}

[[nodiscard]] inline std::uint16_t ReadNetU16(const std::uint8_t *src) {
    return ReadNet<std::uint16_t>(src);
}

[[nodiscard]] inline std::uint32_t ReadNetU32(const std::uint8_t *src) {
    return ReadNet<std::uint32_t>(src);
}

[[nodiscard]] inline std::uint64_t ReadNetU64(const std::uint8_t *src) {
    return ReadNet<std::uint64_t>(src);
}

template <std::integral T>
void WriteNet(std::uint8_t *dest, T src) {
    if constexpr (std::endian::native != std::endian::big) {
        src = std::byteswap(src);
    }
    std::memcpy(dest, &src, sizeof(T));
}

inline void WriteNetI16(std::uint8_t *dest, std::int16_t src) {
    WriteNet<std::int16_t>(dest, src);
}

inline void WriteNetI32(std::uint8_t *dest, std::int32_t src) {
    WriteNet<std::int32_t>(dest, src);
}

inline void WriteNetI64(std::uint8_t *dest, std::int64_t src) {
    WriteNet<std::int64_t>(dest, src);
}

inline void WriteNetU16(std::uint8_t *dest, std::uint16_t src) {
    WriteNet<std::uint16_t>(dest, src);
}

inline void WriteNetU32(std::uint8_t *dest, std::uint32_t src) {
    WriteNet<std::uint32_t>(dest, src);
}

inline void WriteNetU64(std::uint8_t *dest, std::uint64_t src) {
    WriteNet<std::uint64_t>(dest, src);
}

} // namespace homura

#endif // HOMURA_BITUTILS_H
