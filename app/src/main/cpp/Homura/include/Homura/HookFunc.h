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

#ifndef HOMURA_HOOK_FUNC_H
#define HOMURA_HOOK_FUNC_H

#include "Homura/Logger.h"
#include "Homura/Patcher.h"
#include "SubstrateHook.h"

#include <type_traits>

namespace homura {

/**
 * @brief 将类型 T 约束为 指向函数指针的指针类型 或 空指针类型.
 */
template <typename T, typename R, typename... Args>
concept FuncPtrAddr = std::is_same_v<T, R (**)(Args...)> || std::is_null_pointer_v<T>;

/**
 * @brief 替换全局函数/静态成员函数.
 *
 * @tparam R 目标函数的返回值类型.
 * @tparam Args 目标函数的参数类型.
 *
 * @param [in] symbol 通过 dlsym 函数获取的函数符号地址.
 * @param [in] newFunc 用于替换的新函数.
 * @param [out] oldFuncAddr 一个函数指针的地址, 用于保留旧函数. (可传入 nullptr 字面量, 代表不保留)
 *
 * @return 是否成功替换.
 */
template <typename R, typename... Args, FuncPtrAddr<R, Args...> OldFuncAddr>
bool HookFunction(void *symbol, R (*newFunc)(Args...), OldFuncAddr oldFuncAddr) {
    if (symbol == nullptr || newFunc == nullptr || (!std::is_null_pointer_v<OldFuncAddr> && oldFuncAddr == nullptr)) {
        LOG_ERROR("Is nullptr: symbol: {}, newFunc: {}, oldFunc: {}", symbol == nullptr, newFunc == nullptr, oldFuncAddr == nullptr);
        return false;
    }

    if constexpr (!std::is_null_pointer_v<OldFuncAddr>) {
        MSHookFunction(symbol, reinterpret_cast<void *>(newFunc), reinterpret_cast<void **>(oldFuncAddr));
    } else {
        MSHookFunction(symbol, reinterpret_cast<void *>(newFunc), nullptr);
    }
    return true;
}

/**
 * @brief 替换非静态成员函数.
 *
 * @tparam R 目标函数的返回值类型.
 * @tparam T 目标函数所属类的类型.
 * @tparam Args 目标函数的参数类型.
 *
 * @param [in] symbol 通过 dlsym 函数获取的函数符号地址.
 * @param [in] newFunc 用于替换的新函数.
 * @param [out] oldFuncAddr 一个函数指针的地址, 用于保留旧函数. (可传入 nullptr 字面量, 代表不保留)
 *
 * @return 是否成功替换.
 */
template <typename R, typename T, typename... Args, FuncPtrAddr<R, T *, Args...> OldFuncAddr>
bool HookFunction(void *symbol, R (T::*newFunc)(Args...), OldFuncAddr oldFuncAddr) {
    if (symbol == nullptr || newFunc == nullptr || (!std::is_null_pointer_v<OldFuncAddr> && oldFuncAddr == nullptr)) {
        LOG_ERROR("Is nullptr: symbol: {}, newFunc: {}, oldFunc: {}", symbol == nullptr, newFunc == nullptr, oldFuncAddr == nullptr);
        return false;
    }

    if constexpr (!std::is_null_pointer_v<OldFuncAddr>) {
        MSHookFunction(symbol, *reinterpret_cast<void **>(&newFunc), reinterpret_cast<void **>(oldFuncAddr));
    } else {
        MSHookFunction(symbol, *reinterpret_cast<void **>(&newFunc), nullptr);
    }
    return true;
}

/**
 * @brief 替换虚函数.
 *
 * @todo Remove this.
 *
 * @tparam R 目标函数的返回值类型.
 * @tparam Args 目标函数的参数类型.
 *
 * @param [in] vTableSymbol 通过 dlsym 函数获取的虚函数表符号地址.
 * @param [in] index 目标函数在虚函数表中的索引.
 * @param [in] newFunc 用于替换的新函数.
 * @param [out] oldFuncAddr 一个函数指针的地址, 用于保留旧函数. (可传入 nullptr 字面量, 代表不保留)
 *
 * @return 是否成功替换.
 */
template <typename R, typename... Args, FuncPtrAddr<R, Args...> OldFuncAddr>
bool HookVirtualFunc(void *vTableSymbol, size_t index, R (*newFunc)(Args...), OldFuncAddr oldFuncAddr) {
    if (vTableSymbol == nullptr || newFunc == nullptr || (!std::is_null_pointer_v<OldFuncAddr> && oldFuncAddr == nullptr)) {
        LOG_ERROR("Is nullptr: vTableSymbol: {}, newFunc: {}, oldFunc: {}", vTableSymbol == nullptr, newFunc == nullptr, oldFuncAddr == nullptr);
        return false;
    }
    auto **funcPtrAddr = reinterpret_cast<R (**)(Args...)>(vTableSymbol) + index;
    if (!SetProtection(reinterpret_cast<uintptr_t>(funcPtrAddr), sizeof(void *), PROT_READ | PROT_WRITE)) {
        return false;
    }

    if constexpr (!std::is_null_pointer_v<OldFuncAddr>) {
        *oldFuncAddr = *funcPtrAddr;
    }
    *funcPtrAddr = newFunc;

    SetProtection(reinterpret_cast<uintptr_t>(funcPtrAddr), sizeof(void *), PROT_READ);
    return true;
}

/**
 * @brief 替换虚函数.
 *
 * @tparam R 目标函数的返回值类型.
 * @tparam Args 目标函数的参数类型.
 *
 * @param [in] vTableSymbol 通过 dlsym 函数获取的虚函数表符号地址.
 * @param [in] index 目标函数在虚函数表中的索引.
 * @param [in] newFunc 用于替换的新函数.
 * @param [out] oldFuncAddr 一个函数指针的地址, 用于保留旧函数. (可传入 nullptr 字面量, 代表不保留)
 *
 * @return 是否成功替换.
 */
template <typename R, typename T, typename... Args, FuncPtrAddr<R, T *, Args...> OldFuncAddr>
bool HookVirtualFunc(void *vTableSymbol, size_t index, R (T::*newFunc)(Args...), OldFuncAddr oldFuncAddr) {
    if (vTableSymbol == nullptr || newFunc == nullptr || (!std::is_null_pointer_v<OldFuncAddr> && oldFuncAddr == nullptr)) {
        LOG_ERROR("Is nullptr: vTableSymbol: {}, newFunc: {}, oldFunc: {}", vTableSymbol == nullptr, newFunc == nullptr, oldFuncAddr == nullptr);
        return false;
    }
    auto **funcPtrAddr = reinterpret_cast<R (**)(T *, Args...)>(vTableSymbol) + index;
    if (!SetProtection(reinterpret_cast<uintptr_t>(funcPtrAddr), sizeof(void *), PROT_READ | PROT_WRITE)) {
        return false;
    }

    if constexpr (!std::is_null_pointer_v<OldFuncAddr>) {
        *oldFuncAddr = *funcPtrAddr;
    }
    *funcPtrAddr = *reinterpret_cast<decltype(funcPtrAddr)>(&newFunc);

    SetProtection(reinterpret_cast<uintptr_t>(funcPtrAddr), sizeof(void *), PROT_READ);
    return true;
}

/**
 * @brief 替换 PLT 段函数.
 *
 * @tparam R 目标函数的返回值类型.
 * @tparam Args 目标函数的参数类型.
 *
 * @param [in] libName 目标函数所属模块的名称.
 * @param [in] offset 目标函数在所属模块中的偏移量.
 * @param [in] newFunc 用于替换的新函数.
 * @param [out] oldFuncAddr 一个函数指针的地址, 用于保留旧函数. (可传入 nullptr 字面量, 代表不保留)
 *
 * @return 是否成功替换.
 */
template <typename R, typename... Args, FuncPtrAddr<R, Args...> OldFuncAddr>
bool HookPltFunction(const std::string &libName, uintptr_t offset, R (*newFunc)(Args...), OldFuncAddr oldFuncAddr) {
    if (newFunc == nullptr || (!std::is_null_pointer_v<OldFuncAddr> && oldFuncAddr == nullptr)) {
        LOG_ERROR("Is nullptr: newFunc: {}, oldFunc: {}", newFunc == nullptr, oldFuncAddr == nullptr);
        return false;
    }
    uintptr_t baseAddr = Patcher::GetBaseAddress(libName, false);
    if (baseAddr == 0) {
        LOG_ERROR("Failed to get base address.");
        return false;
    }
    uintptr_t funcPtrAddr = baseAddr + offset;
    if (!SetProtection(funcPtrAddr, sizeof(void *), PROT_READ | PROT_WRITE)) {
        return false;
    }

    if constexpr (!std::is_null_pointer_v<OldFuncAddr>) {
        *oldFuncAddr = *reinterpret_cast<R (**)(Args...)>(funcPtrAddr);
    }
    *reinterpret_cast<R (**)(Args...)>(funcPtrAddr) = newFunc;

    SetProtection(funcPtrAddr, sizeof(void *), PROT_READ);
    return true;
}

} // namespace homura

#endif // HOMURA_HOOK_FUNC_H
