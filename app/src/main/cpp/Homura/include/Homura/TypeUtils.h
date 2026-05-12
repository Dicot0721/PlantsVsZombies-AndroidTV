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

#ifndef HOMURA_TYPEUTILS_H
#define HOMURA_TYPEUTILS_H

#include <concepts>
#include <memory>

namespace homura {

/**
 * @brief 用作未初始化存储的类模板
 *
 * 对象使用 placement new 创建, 并使用显式析构函数调用销毁.
 */
template <typename T>
    requires(requires { sizeof(T); } && !std::is_reference_v<T>)
class Storage {
public:
    using ElementType = T;

    Storage() = default;

    Storage(const Storage &) = delete;
    Storage(Storage &&) = delete;
    Storage &operator=(const Storage &) = delete;
    Storage &operator=(Storage &&) = delete;

    [[nodiscard]] const T *operator->() const noexcept {
        return reinterpret_cast<const T *>(&data_);
    }

    [[nodiscard]] T *operator->() noexcept {
        return reinterpret_cast<T *>(&data_);
    }

    [[nodiscard]] const T &operator*() const & noexcept {
        return reinterpret_cast<const T &>(data_);
    }

    [[nodiscard]] T &operator*() & noexcept {
        return reinterpret_cast<T &>(data_);
    }

    [[nodiscard]] const T &&operator*() const && noexcept {
        return reinterpret_cast<const T &&>(data_);
    }

    [[nodiscard]] T &&operator*() && noexcept {
        return reinterpret_cast<T &&>(data_);
    }

    template <typename... Args>
        requires std::is_constructible_v<T, Args &&...>
    T &Construct(Args &&...args) {
        return *std::construct_at(operator->(), std::forward<Args>(args)...);
    }

    template <typename U, typename... Args>
        requires std::is_constructible_v<T, std::initializer_list<U> &, Args &&...>
    T &Construct(std::initializer_list<U> ilist, Args &&...args) {
        return *std::construct_at(operator->(), ilist, std::forward<Args>(args)...);
    }

    void Destruct()
        requires std::is_destructible_v<T>
    {
        std::destroy_at(operator->());
    }

protected:
    alignas(T) unsigned char data_[sizeof(T)];
};

/**
 * @brief 用作未初始化存储的类模板
 *
 * 对象使用 placement new 创建, 但不需要使用显式析构函数调用销毁.
 */
template <std::destructible T>
class AutoDestructStorage : public Storage<T> {
public:
    constexpr AutoDestructStorage() noexcept
        : Storage<T>{} {};

    ~AutoDestructStorage() {
        this->Destruct();
    }
};

} // namespace homura

#endif // HOMURA_TYPEUTILS_H
