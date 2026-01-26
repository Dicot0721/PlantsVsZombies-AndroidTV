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

#ifndef HOMURA_STRUCT_WRAPPER_H
#define HOMURA_STRUCT_WRAPPER_H

#include <cstdint>

#include <concepts>
#include <mdspan>
#include <span>

namespace homura {

/**
 * @brief 通过对象地址和非静态成员变量的偏移量, 获取该成员变量的地址.
 *
 * @tparam T 成员变量的类型.
 *
 * @param thiz 对象指针 (任意类型).
 * @param offset 成员变量在类内的偏移量.
 *
 * @return 返回一个指针, 其值为成员变量的地址.
 */
template <typename T>
[[nodiscard]] T *GetMemAddr(void *thiz, uintptr_t offset) {
    return reinterpret_cast<T *>(uintptr_t(thiz) + offset);
}

/**
 * @brief 通过对象地址和成员变量的偏移量, 获取该成员变量的引用. (可使用多级偏移)
 *
 * @tparam T 成员变量的类型.
 *
 * @param thiz 对象指针 (任意类型).
 * @param first 第一级偏移量.
 * @param others 余下的各级偏移量.
 *
 * @return 返回成员变量的引用.
 *
 * @par 示例:
 * @code
 * Challenge *aChallenge = homura::GetMemberRef&lt;Challenge *&gt;(gLawnApp, 0x8A0, 0x254);
 * @endcode
 */
template <typename T>
[[nodiscard]] T &GetMemberRef(void *thiz, uintptr_t first, std::integral auto... others) {
    if constexpr (sizeof...(others) > 0) {
        return GetMemberRef<T>(GetMemberRef<void *>(thiz, first), others...);
    } else {
        return *GetMemAddr<T>(thiz, first);
    }
}

/**
 * @brief 通过对象地址和数组类型成员变量的偏移量, 获取该成员变量的一维视图. (可使用多级偏移)
 *
 * @tparam T  std::span 对象所包含元素的类型.
 * @tparam N  std::span 对象所包含的元素的数量.
 *
 * @param thiz 对象指针 (任意类型).
 * @param first 第一级偏移量.
 * @param others 余下的各级偏移量.
 *
 * @return 返回一个 std::span 对象, 可用于遍历或修改该成员变量中的元素.
 *
 * @par 示例:
 * @code
 * auto aZombieAllowed = homura::GetMemberSpan&lt;bool, 100&gt(aBoard, 0x55E8);
 * aZombieAllowed[(int)ZombieType::ZOMBIE_NORMAL] = false; // 禁止出普通僵尸
 * @endcode
 */
template <typename T, size_t N>
[[nodiscard]] std::span<T, N> GetMemberSpan(void *thiz, uintptr_t first, std::integral auto... others) {
    if constexpr (sizeof...(others) > 0) {
        return GetMemberSpan<T, N>(GetMemberRef<void *>(thiz, first), others...);
    } else {
        return std::span<T, N>(GetMemAddr<T>(thiz, first), N);
    }
}

/**
 * @brief 通过对象地址和数组成员变量的偏移量, 获取该成员变量的多维视图. (可使用多级偏移)
 *
 * @tparam T  std::mdspan 对象所包含元素的类型.
 * @tparam Dims  std::mdspan 对象各个维度的大小.
 *
 * @param thiz 对象指针 (任意类型).
 * @param first 第一级偏移量.
 * @param others 余下的各级偏移量.
 *
 * @return 返回一个 std::mdspan 对象, 可用于遍历或修改该成员变量中的元素.
 *
 * @par 示例:
 * @code
 * auto mZombiesInWave = homura::GetMemberMdspan&lt;ZombieType, 100, 50&gt(aBoard, 0x7C8);
 *
 * // 将前 20 波中每波的第一只僵尸设置为旗帜僵尸
 * for (int wave = 0; wave &lt; 20; ++wave) {
 *     mZombiesInWave[wave, 0] = ZombieType::ZOMBIE_FLAG;
 * }
 * @endcode
 */
template <typename T, size_t... DIMS>
[[nodiscard]] auto GetMemberMdspan(void *thiz, uintptr_t first, std::integral auto... others) -> std::mdspan<T, std::extents<size_t, DIMS...>> {
    if constexpr (sizeof...(others) > 0) {
        return GetMemberMdspan<T, DIMS...>(GetMemberRef<void *>(thiz, first), others...);
    } else {
        return std::mdspan<T, std::extents<size_t, DIMS...>>(GetMemAddr<T>(thiz, first));
    }
}

/**
 * @brief 通过对象地址和虚函数的索引调用虚函数.
 *
 * @tparam I 虚函数在虚函数表中的索引. (注意不是偏移量)
 * @tparam R 虚函数的返回值类型.
 * @tparam T 对象所属的类型.
 * @tparam Args 虚函数的各参数类型.
 *
 * @param thiz 对象指针.
 * @param args 虚函数的参数列表.
 */
template <typename T, size_t I, typename R, typename... Args>
R CallVirtualFunc(T *thiz, std::type_identity_t<Args &&>... args) {
    using FuncType = R (T::*)(Args...);
    FuncType *vtable = *reinterpret_cast<FuncType **>(thiz);
    FuncType vfunc = vtable[I];
    return (thiz->*vfunc)(std::forward<Args>(args)...);
}

} // namespace homura

#endif // HOMURA_STRUCT_WRAPPER_H
