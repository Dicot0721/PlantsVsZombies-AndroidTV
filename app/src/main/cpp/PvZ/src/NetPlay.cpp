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

namespace {
std::vector<uint8_t> sendBuffer;
}

void netplay::details::PutEventData(const std::uint8_t *src, std::size_t n) {
    sendBuffer.append_range(std::views::counted(src, n));
}

bool netplay::FlushSendBuffer(int socket) {
    if (sendBuffer.empty()) {
        return true;
    }

    const auto *data = sendBuffer.data();
    const auto size = sendBuffer.size();
    size_t sent = 0;
    while (sent < size) {
        ssize_t ret = send(socket, data + sent, size - sent, 0);
        if (ret < 0) {
            LOG_ERROR("Failed to send event: {}", std::strerror(errno));
            break;
        }
        sent += ret;
    }

    sendBuffer.clear();
    return sent < size;
}

void netplay::ClearSendBuffer() noexcept {
    sendBuffer.clear();
}
