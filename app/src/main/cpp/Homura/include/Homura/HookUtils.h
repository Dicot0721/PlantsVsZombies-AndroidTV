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

#ifndef HOMURA_HOOKUTILS_H
#define HOMURA_HOOKUTILS_H

#include <string_view>
#include <type_traits>

namespace homura {

namespace details {
    void HookFunctionImpl(void *symbol, void *newFunc, void **oldFuncAddr);
    bool HookVirtualFuncImpl(void *vTableSymbol, size_t index, void *newFunc, void **oldFuncAddr);
    bool HookPltFunctionImpl(std::string_view libName, uintptr_t offset, void *newFunc, void **oldFuncAddr);
} // namespace details

/**
 * @brief 替换全局函数/静态成员函数.
 *
 * @tparam R 目标函数的返回值类型.
 * @tparam Args 目标函数的参数类型.
 *
 * @param [in] symbol 通过 dlsym 函数获取的函数符号地址.
 * @param [in] newFunc 用于替换的新函数.
 * @param [out] oldFuncAddr 一个函数指针的地址, 用于保留旧函数. (可传入 nullptr 字面量, 代表不保留)
 */
template <typename R, typename... Args>
void HookFunction(void *symbol, R (*newFunc)(Args...), std::add_pointer_t<decltype(newFunc)> oldFuncAddr) {
    details::HookFunctionImpl(symbol, reinterpret_cast<void *>(newFunc), reinterpret_cast<void **>(oldFuncAddr));
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
 */
template <typename R, typename T, typename... Args>
void HookFunction(void *symbol, R (T::*newFunc)(Args...), std::type_identity_t<R (**)(T *, Args...)> oldFuncAddr) {
    details::HookFunctionImpl(symbol, *reinterpret_cast<void **>(&newFunc), reinterpret_cast<void **>(oldFuncAddr));
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
template <typename R, typename... Args>
bool HookVirtualFunc(void *vTableSymbol, size_t index, R (*newFunc)(Args...), std::add_pointer_t<decltype(newFunc)> oldFuncAddr) {
    return details::HookVirtualFuncImpl(vTableSymbol, index, reinterpret_cast<void *>(newFunc), reinterpret_cast<void **>(oldFuncAddr));
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
template <typename R, typename T, typename... Args>
bool HookVirtualFunc(void *vTableSymbol, size_t index, R (T::*newFunc)(Args...), std::type_identity_t<R (**)(T *, Args...)> oldFuncAddr) {
    return details::HookVirtualFuncImpl(vTableSymbol, index, *reinterpret_cast<void **>(&newFunc), reinterpret_cast<void **>(oldFuncAddr));
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
template <typename R, typename... Args>
bool HookPltFunction(std::string_view libName, uintptr_t offset, R (*newFunc)(Args...), std::add_pointer_t<decltype(newFunc)> oldFuncAddr) {
    return details::HookPltFunctionImpl(libName, offset, reinterpret_cast<void *>(newFunc), reinterpret_cast<void **>(oldFuncAddr));
}

} // namespace homura

#endif // HOMURA_HOOKUTILS_H
