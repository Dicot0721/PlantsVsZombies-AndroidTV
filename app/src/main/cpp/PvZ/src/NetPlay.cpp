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

#include "PvZ/NetPlay.h"
#include "Homura/Logger.h"

#include <sys/socket.h>

#include <cstring>

#include <ranges>
#include <vector>

static std::vector<std::byte> sendBuffer;

void netplay::details::PutEventData(const std::byte *data, std::size_t n) {
    sendBuffer.append_range(std::views::counted(data, n));
}

bool netplay::FlushSendBuffer(int socket) {
    if (sendBuffer.empty()) {
        return true;
    }

    const auto *p = sendBuffer.data();
    const auto *end = sendBuffer.data() + sendBuffer.size();
    while (p < end) {
        ssize_t ret = send(socket, p, end - p, 0);
        if (ret < 0) {
            LOG_ERROR("Failed to send event: {}", std::strerror(errno));
            break;
        }
        p += ret;
    }

    sendBuffer.clear();
    return p >= end;
}

void netplay::ClearSendBuffer() noexcept {
    sendBuffer.clear();
}

std::size_t netplay::ParseEventSize(const std::byte *data) {
    decltype(BaseEvent::size) sz;
    std::memcpy(&sz, data + offsetof(BaseEvent, size), sizeof(sz));
    return sz;
}

BaseEvent *netplay::GetEvent(std::byte *dest, const std::byte *src) {
    std::memcpy(dest, src, ParseEventSize(src));
    return reinterpret_cast<BaseEvent *>(dest);
}
