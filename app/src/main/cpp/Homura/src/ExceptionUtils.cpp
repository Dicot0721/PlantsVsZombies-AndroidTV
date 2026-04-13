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

#include "Homura/ExceptionUtils.h"
#include "Homura/Logger.h"

#include <csignal>

#include <exception>

[[noreturn]] static void ExceptionHandler() {
    try {
        if (auto eptr = std::current_exception()) {
            std::rethrow_exception(eptr);
        } else {
            LOG_FATAL("Unhandled 'std::terminate()' call");
        }
    } catch (const std::exception &e) {
        LOG_FATAL("Unhandled exception: {}", e.what());
    } catch (...) {
        LOG_FATAL("Unknown exception caught");
    }
    std::abort();
}

void homura::RegisterExceptionHandler() noexcept {
    std::set_terminate(&ExceptionHandler);
}

[[noreturn]] static void AccessViolationHandler(int sig, siginfo_t *info, void *uct) {
    LOG_FATAL("Unhandled access to 0x{:X}", reinterpret_cast<std::uintptr_t>(info->si_addr));
    std::abort();
}

void homura::RegisterAccessViolationHandler() {
    struct sigaction sa{
        .sa_sigaction = &AccessViolationHandler,
        .sa_flags = SA_SIGINFO,
    };
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGSEGV, &sa, nullptr) == -1) [[unlikely]] {
        LOG_ERROR("Failed to register an exception handler");
    }
}
