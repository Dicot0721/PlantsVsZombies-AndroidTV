/*
 * Copyright (C) 2023-2025  PvZ TV Touch Team
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

#ifndef HOMURA_LOGGER_H
#define HOMURA_LOGGER_H

#include <android/log.h>

#include <format>
#include <source_location>

/**
 * @file 简单的日志工具. (接口已封装为宏)
 *
 * 调试时可在命令行工具中输入命令 adb logcat -s pvztv 查看日志.
 * 也可以输入 adb logcat *:S pvztv:D/I/W/E 控制输出级别.
 */

#ifdef PVZ_DEBUG
#define LOG_DEBUG(...) homura::Log<ANDROID_LOG_DEBUG>(std::source_location::current(), __VA_ARGS__)
#define LOG_INFO(...) homura::Log<ANDROID_LOG_INFO>(std::source_location::current(), __VA_ARGS__)
#define LOG_WARN(...) homura::Log<ANDROID_LOG_WARN>(std::source_location::current(), __VA_ARGS__)
#define LOG_ERROR(...) homura::Log<ANDROID_LOG_ERROR>(std::source_location::current(), __VA_ARGS__)
#else
#define LOG_DEBUG(...) ((void)0)
#define LOG_INFO(...) ((void)0)
#define LOG_WARN(...) ((void)0)
#define LOG_ERROR(...) ((void)0)
#endif // PVZ_DEBUG

namespace homura {

constexpr android_LogPriority PVZ_LOG_LEVEL = ANDROID_LOG_DEBUG;

constexpr const char *PVZ_LOG_TAG = "pvztv";

template <android_LogPriority LEVEL, typename... Args>
void Log(std::source_location location, std::format_string<Args...> format, Args &&...args) {
    if constexpr (LEVEL >= PVZ_LOG_LEVEL) {
        std::string message = std::vformat(format.get(), std::make_format_args(args...));
        __android_log_print(LEVEL, PVZ_LOG_TAG, "[%s] %s", location.function_name(), message.c_str());
    }
}

} // namespace homura

#endif // HOMURA_LOGGER_H
