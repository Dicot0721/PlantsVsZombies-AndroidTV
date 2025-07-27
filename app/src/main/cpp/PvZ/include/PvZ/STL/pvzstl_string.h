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

#ifndef PVZ_STL_PVZSTL_STRING_H
#define PVZ_STL_PVZSTL_STRING_H

#include "pvzstl_concepts.h"

#include <cassert>

#include <atomic>
#include <stdexcept>
#include <string>

#ifdef PVZ_VERSION
extern uintptr_t gLibBaseOffset;
#endif

namespace pvzstl {

template <__character CharT>
class basic_string;

template <__character CharT>
[[nodiscard]] basic_string<CharT> operator+(const basic_string<CharT> &lhs, const basic_string<CharT> &rhs);

template <__character CharT>
[[nodiscard]] basic_string<CharT> operator+(const CharT *lhs, const basic_string<CharT> &rhs);

template <__character CharT>
[[nodiscard]] basic_string<CharT> operator+(CharT lhs, const basic_string<CharT> &rhs);

template <__character CharT>
[[nodiscard]] basic_string<CharT> operator+(const basic_string<CharT> &lhs, const CharT *rhs);

template <__character CharT>
[[nodiscard]] basic_string<CharT> operator+(const basic_string<CharT> &lhs, CharT rhs);

template <typename Tp, typename CharT>
concept __convertible_to_string_view = std::is_convertible_v<const Tp &, std::basic_string_view<CharT>> && !std::is_convertible_v<const Tp &, const CharT *>;

struct __uninitialized_size_tag {};

/**
 * @class 采用写时复制 (COW) 实现的字符串类模板
 *
 * g++ 在版本 5 前 std::string 的实现 (简化版).<br />
 * 可参考文章: <a href="https://chengxumiaodaren.com/docs/open-source/cpp-string/">C++ string源码剖析：深入理解COW机制</a>.
 */
template <__character CharT>
class basic_string {
public:
    using traits_type = std::char_traits<CharT>;
    using value_type = CharT;
    using size_type = std::uint32_t;

    using __self_view = std::basic_string_view<value_type>;

    static constexpr size_type npos = static_cast<size_type>(-1);

    basic_string()
        : __data_{__rep::empty_rep().__data} {}

    basic_string(const basic_string &other)
        : __data_{other.__data_} {
        if (__rep *p = __get_rep(); p != &__rep::empty_rep()) {
            ++p->__ref_count;
        }
    }

    basic_string(basic_string &&other) noexcept
        : __data_{other.__data_} {
        other.__data_ = __rep::empty_rep().__data;
    }

    basic_string(const value_type *s) {
        assert((s != nullptr) && "basic_string(const CharT *) detected nullptr");
        __init(s, traits_type::length(s));
    }

    basic_string(std::nullptr_t) = delete;

    basic_string(const value_type *s, size_type n) {
        assert((s != nullptr || n == 0) && "basic_string(const CharT *, size_type) detected nullptr");
        __init(s, n);
    }

    basic_string(const basic_string &str, size_type pos, size_type n) {
        const size_type str_sz = str.size();
        if (pos > str_sz) {
            __throw_out_of_range();
        }
        __init(str.c_str() + pos, std::min(n, str_sz - pos));
    }

    basic_string(const basic_string &str, size_type pos) {
        const size_type str_sz = str.size();
        if (pos > str_sz) {
            __throw_out_of_range();
        }
        __init(str.c_str() + pos, str_sz - pos);
    }

    ~basic_string() {
        __reset();
    }

    /* implicit */ operator __self_view() const noexcept {
        return __self_view{c_str(), size()};
    }

    basic_string &operator=(const basic_string &other);
    basic_string &operator=(basic_string &&other) noexcept;

    basic_string &operator=(const value_type *s);

    basic_string &operator=(std::nullptr_t) = delete;

    /**
     * @brief 就地构造 pvzstl::basic_string 对象.
     */
    template <typename... Args>
    basic_string &emplace(Args &&...args) {
        return *new (this) basic_string{std::forward<Args>(args)...};
    }

    [[nodiscard, gnu::always_inline]] size_type size() const noexcept {
        return __get_rep()->__size;
    }

    [[nodiscard, gnu::always_inline]] size_type max_size() const noexcept {
        return __rep::__max_size;
    }

    [[nodiscard, gnu::always_inline]] size_type capacity() const noexcept {
        return __get_rep()->__capacity;
    }

    void reserve(size_type new_cap);

    void clear() noexcept;

    [[nodiscard, gnu::always_inline]] bool empty() const noexcept {
        return size() == 0;
    }

    [[nodiscard, gnu::always_inline]] const value_type &operator[](size_type pos) const noexcept {
        assert((pos <= size()) && "string index out of bounds");
        return *(c_str() + pos);
    }

    [[nodiscard]] const value_type &at(size_type n) const;

    basic_string &operator+=(const basic_string &str) {
        return append(str);
    }

    basic_string &operator+=(const value_type *s) {
        return append(s);
    }

    basic_string &operator+=(value_type c) {
        push_back(c);
        return *this;
    }

    basic_string &append(const basic_string &str) {
        return append(str.c_str(), str.size());
    }

    basic_string &append(const value_type *s, size_type n);

    basic_string &append(const value_type *s) {
        assert((s != nullptr) && "basic_string::append received nullptr");
        return append(s, traits_type::length(s));
    }

    void push_back(value_type c);

    [[nodiscard, gnu::always_inline]] const value_type &front() const noexcept {
        assert(!empty() && "basic_string::front(): string is empty");
        return *c_str();
    }

    [[nodiscard, gnu::always_inline]] const value_type &back() const noexcept {
        assert(!empty() && "basic_string::back(): string is empty");
        return *(c_str() + size() - 1);
    }

    [[nodiscard]] basic_string substr(size_type pos = 0, size_type n = npos) const {
        return basic_string{*this, pos, n};
    }

    [[nodiscard, gnu::always_inline]] const value_type *c_str() const noexcept {
        return __data_;
    }

    [[nodiscard]] size_type find(const basic_string &str, size_type pos = 0) const noexcept {
        return find(__self_view{str}, pos);
    }

    template <__convertible_to_string_view<value_type> Tp>
    [[nodiscard]] size_type find(const Tp &t, size_type pos = 0) const noexcept;

    [[nodiscard]] size_type find(const value_type *s, size_type pos, size_type n) const noexcept {
        return find(__self_view{s, n}, pos);
    }

    [[nodiscard]] size_type find(const value_type *s, size_type pos = 0) const noexcept {
        return find(__self_view{s}, pos);
    }

    [[nodiscard]] size_type find(value_type c, size_type pos = 0) const noexcept {
        return find(__self_view{&c, 1}, pos);
    }

    [[nodiscard]] bool starts_with(__self_view sv) const noexcept {
        return __self_view{c_str(), size()}.starts_with(sv);
    }

    [[nodiscard]] bool starts_with(value_type c) const noexcept {
        return !empty() && (front() == c);
    }

    [[nodiscard]] bool starts_with(const value_type *s) const noexcept {
        return starts_with(__self_view{s});
    }

    [[nodiscard]] bool ends_with(__self_view sv) const noexcept {
        return __self_view{c_str(), size()}.ends_with(sv);
    }

    [[nodiscard]] bool ends_with(value_type c) const noexcept {
        return !empty() && (back() == c);
    }

    [[nodiscard]] bool ends_with(const value_type *s) const noexcept {
        return ends_with(__self_view{s});
    }

    [[nodiscard]] bool contains(__self_view sv) const noexcept {
        return __self_view{c_str(), size()}.contains(sv);
    }

    [[nodiscard]] bool contains(value_type c) const noexcept {
        return __self_view{c_str(), size()}.contains(c);
    }

    [[nodiscard]] bool contains(const value_type *s) const {
        return __self_view{c_str(), size()}.contains(s);
    }

    [[nodiscard, gnu::always_inline]] std::int32_t __ref_count() const noexcept {
        return __get_rep()->__ref_count;
    }

    [[nodiscard, gnu::always_inline]] bool __unique() const noexcept {
        return __ref_count() == 0;
    }

    [[noreturn, gnu::always_inline]] void __throw_length_error() const {
        throw std::length_error{"basic_string"};
    }

    [[noreturn, gnu::always_inline]] void __throw_out_of_range() const {
        throw std::out_of_range{"basic_string"};
    }

protected:
    mutable value_type *__data_;

    struct __rep {
        static constexpr size_type __max_size = 0x3FFF'FFFC;

        size_type __size;                // 字符数
        size_type __capacity;            // 已分配存储空间中可以容纳的字符数
        std::atomic_int32_t __ref_count; // 引用计数 (小于等于 0 时释放内存)
        value_type __data[];             // 作为字符存储的底层数组 (柔性数组成员)

        [[nodiscard]] static __rep *create(size_type cap, size_type old_cap) {
            if (cap > __max_size) {
                throw std::length_error{"basic_string::__rep::create"};
            }
            if ((cap > old_cap) && (cap < 2 * old_cap)) {
                cap = 2 * old_cap;
            }
            const size_type alloc_sz = sizeof(__rep) + sizeof(value_type) * (cap + 1);
            // note: here pass align
            return new (::operator new(alloc_sz)) __rep{.__capacity = cap, .__ref_count = 0};
        }

        static void destroy(__rep *p) noexcept {
            if (p->__ref_count-- <= 0) {
                ::operator delete(p);
            }
        }

        [[nodiscard, gnu::always_inline]] static __rep &empty_rep() noexcept {
#ifdef PVZ_VERSION
            assert(::gLibBaseOffset != 0);
            static constexpr uintptr_t offset = (sizeof(value_type) == sizeof(int8_t)) ? /* string */ 0x71BB54 : /* wstring */ 0x69E45C;
            static uintptr_t empty_rep_storage_addr = ::gLibBaseOffset + offset;
            return *reinterpret_cast<__rep *>(empty_rep_storage_addr);
#else
            alignas(__rep) static std::byte empty_rep_storage[sizeof(__rep) + sizeof(value_type)] = {};
            return *reinterpret_cast<__rep *>(empty_rep_storage);
#endif
        }
    };

    // Construct a string with enough storage to hold `size` characters, but don't initialize the characters.
    // The contents of the string, including the null terminator, must be initialized separately.
    explicit basic_string(__uninitialized_size_tag, size_type size)
        : __data_{__rep::create(size, 0)->__data} {
        __get_rep()->__size = size;
    }

    [[nodiscard, gnu::always_inline]] __rep *__get_rep() const noexcept {
        return reinterpret_cast<__rep *>(__data_) - 1;
    }

    void __init(const value_type *s, size_type sz);

    void __reset() noexcept;

    friend basic_string operator+ <>(const basic_string &, const basic_string &);
    friend basic_string operator+ <>(const value_type *, const basic_string &);
    friend basic_string operator+ <>(value_type, const basic_string &);
    friend basic_string operator+ <>(const basic_string &, const value_type *);
    friend basic_string operator+ <>(const basic_string &, value_type);
};

template <__character CharT>
basic_string<CharT> &basic_string<CharT>::operator=(const basic_string &other) {
    if ((this != std::addressof(other)) && (__data_ != other.__data_)) {
        __reset();
        if ((__data_ = other.__data_) != __rep::empty_rep().__data) {
            ++__get_rep()->__ref_count;
        }
    }
    return *this;
}

template <__character CharT>
basic_string<CharT> &basic_string<CharT>::operator=(basic_string &&other) noexcept {
    if (this != std::addressof(other)) {
        __reset();
        __data_ = std::exchange(other.__data_, __rep::empty_rep().__data);
    }
    return *this;
}

template <__character CharT>
basic_string<CharT> &basic_string<CharT>::operator=(const value_type *s) {
    assert((s != nullptr) && "basic_string::operator= received nullptr");
    const size_type len = traits_type::length(s);
    if (empty() && len == 0) {
        return *this;
    }
    const size_type cap = capacity();
    if (len > cap || !__unique()) {
        __rep *p = __rep::create(len, cap);
        traits_type::copy(p->__data, s, len);
        __reset();
        __data_ = p->__data;
    } else {
        traits_type::copy(__data_, s, len);
    }
    __data_[len] = value_type{0};
    __get_rep()->__size = len;
    return *this;
}

template <__character CharT>
void basic_string<CharT>::clear() noexcept {
    if (empty()) {
        return;
    }
    if (!__unique()) {
        __reset();
        return;
    }
    __data_[0] = value_type{0};
    __get_rep()->__size = 0;
}

template <__character CharT>
void basic_string<CharT>::reserve(size_type new_cap) {
    if (new_cap > max_size()) {
        __throw_length_error();
    }
    const size_type old_cap = capacity();
    if (new_cap <= old_cap) {
        return;
    }
    const size_type sz = size();
    __rep *p = __rep::create(new_cap, old_cap);
    traits_type::copy(p->__data, c_str(), sz + 1);
    p->__size = sz;
    __reset();
    __data_ = p->__data;
}

template <__character CharT>
basic_string<CharT> &basic_string<CharT>::append(const value_type *s, size_type n) {
    assert((s != nullptr || n == 0) && "basic_string::append received nullptr");
    if (n == 0) {
        return *this;
    }
    const size_type old_sz = size();
    const size_type new_sz = old_sz + n;
    const size_type cap = capacity();
    if (new_sz > cap || !__unique()) {
        __rep *p = __rep::create(new_sz, cap);
        traits_type::copy(p->__data, c_str(), old_sz);
        traits_type::copy(p->__data + old_sz, s, n);
        __reset();
        __data_ = p->__data;
    } else {
        traits_type::copy(__data_ + old_sz, s, n);
    }
    __data_[new_sz] = value_type{0};
    __get_rep()->__size = new_sz;
    return *this;
}

template <__character CharT>
void basic_string<CharT>::push_back(value_type c) {
    const size_type sz = size();
    reserve(sz + 1);
    __data_[sz] = c;
    __data_[sz + 1] = value_type{0};
    ++__get_rep()->__size;
}

template <__character CharT>
[[nodiscard]] auto basic_string<CharT>::at(size_type n) const -> const value_type & {
    if (n >= size()) {
        __throw_out_of_range();
    }
    return (*this)[n];
}

template <__character CharT>
template <__convertible_to_string_view<CharT> Tp>
[[nodiscard]] auto basic_string<CharT>::find(const Tp &t, size_type pos) const noexcept -> size_type {
    using SelfViewSizeType = typename __self_view::size_type;
    const SelfViewSizeType xpos = __self_view{c_str(), size()}.find(__self_view{t}, pos);
    if constexpr (std::is_same_v<SelfViewSizeType, size_type>) {
        return xpos;
    } else {
        return (xpos == __self_view::npos) ? npos : static_cast<size_type>(xpos);
    }
}

template <__character CharT>
void basic_string<CharT>::__init(const value_type *s, size_type sz) {
    __data_ = __rep::create(sz, 0)->__data;
    traits_type::copy(__data_, s, sz);
    __data_[sz] = value_type{0};
    __get_rep()->__size = sz;
}

template <__character CharT>
void basic_string<CharT>::__reset() noexcept {
    if (__rep *p = __get_rep(); p != &__rep::empty_rep()) {
        __rep::destroy(p);
        __data_ = __rep::empty_rep().__data;
    }
}

template <__character CharT>
[[nodiscard]] bool operator==(const basic_string<CharT> &lhs, const basic_string<CharT> &rhs) noexcept {
    return std::basic_string_view<CharT>{lhs} == std::basic_string_view<CharT>{rhs};
}

template <__character CharT>
[[nodiscard]] bool operator==(const basic_string<CharT> &lhs, const CharT *rhs) noexcept {
    return std::basic_string_view<CharT>{lhs} == std::basic_string_view<CharT>{rhs};
}

template <__character CharT>
[[nodiscard]] auto operator<=>(const basic_string<CharT> &lhs, const basic_string<CharT> &rhs) noexcept {
    return std::basic_string_view<CharT>{lhs} <=> std::basic_string_view<CharT>{rhs};
}

template <__character CharT>
[[nodiscard]] auto operator<=>(const basic_string<CharT> &lhs, const CharT *rhs) noexcept {
    return std::basic_string_view<CharT>{lhs} <=> std::basic_string_view<CharT>{rhs};
}

template <__character CharT>
[[nodiscard]] basic_string<CharT> operator+(const basic_string<CharT> &lhs, const basic_string<CharT> &rhs) {
    using String = basic_string<CharT>;
    using Traits = typename String::traits_type;
    const auto lhs_sz = lhs.size();
    const auto rhs_sz = rhs.size();
    String r{__uninitialized_size_tag{}, lhs_sz + rhs_sz};
    Traits::copy(r.__data_, lhs.c_str(), lhs_sz);
    Traits::copy(r.__data_ + lhs_sz, rhs.c_str(), rhs_sz);
    r.__data_[lhs_sz + rhs_sz] = CharT{0};
    return r;
}

template <__character CharT>
[[nodiscard]] basic_string<CharT> operator+(const CharT *lhs, const basic_string<CharT> &rhs) {
    using String = basic_string<CharT>;
    using Traits = typename String::traits_type;
    const auto lhs_sz = Traits::length(lhs);
    const auto rhs_sz = rhs.size();
    String r{__uninitialized_size_tag{}, lhs_sz + rhs_sz};
    Traits::copy(r.__data_, lhs, lhs_sz);
    Traits::copy(r.__data_ + lhs_sz, rhs.c_str(), rhs_sz);
    r.__data_[lhs_sz + rhs_sz] = CharT{0};
    return r;
}

template <__character CharT>
[[nodiscard]] basic_string<CharT> operator+(CharT lhs, const basic_string<CharT> &rhs) {
    using String = basic_string<CharT>;
    using Traits = typename String::traits_type;
    const auto rhs_sz = rhs.size();
    String r{__uninitialized_size_tag{}, rhs_sz + 1};
    r.__data_[0] = lhs;
    Traits::copy(r.__data_ + 1, rhs.c_str(), rhs_sz);
    r.__data_[rhs_sz + 1] = CharT{0};
    return r;
}

template <__character CharT>
[[nodiscard]] basic_string<CharT> operator+(const basic_string<CharT> &lhs, const CharT *rhs) {
    using String = basic_string<CharT>;
    using Traits = typename String::traits_type;
    const auto lhs_sz = lhs.size();
    const auto rhs_sz = Traits::length(rhs);
    String r{__uninitialized_size_tag{}, lhs_sz + rhs_sz};
    Traits::copy(r.__data_, lhs.c_str(), lhs_sz);
    Traits::copy(r.__data_ + lhs_sz, rhs, rhs_sz);
    r.__data_[lhs_sz + rhs_sz] = CharT{0};
    return r;
}

template <__character CharT>
[[nodiscard]] basic_string<CharT> operator+(const basic_string<CharT> &lhs, CharT rhs) {
    using String = basic_string<CharT>;
    using Traits = typename String::traits_type;
    const auto lhs_sz = lhs.size();
    String r{__uninitialized_size_tag{}, lhs_sz + 1};
    Traits::copy(r.__data_, lhs.c_str(), lhs_sz);
    r.__data_[lhs_sz] = rhs;
    r.__data_[lhs_sz + 1] = CharT{0};
    return r;
}

template <__character CharT>
[[nodiscard]] basic_string<CharT> operator+(basic_string<CharT> &&lhs, const basic_string<CharT> &rhs) {
    return std::move(lhs.append(rhs));
}

template <__character CharT>
[[nodiscard]] basic_string<CharT> operator+(basic_string<CharT> &&lhs, basic_string<CharT> &&rhs) {
    return std::move(lhs.append(rhs));
}

template <__character CharT>
[[nodiscard]] basic_string<CharT> operator+(basic_string<CharT> &&lhs, const CharT *rhs) {
    return std::move(lhs.append(rhs));
}

template <__character CharT>
[[nodiscard]] basic_string<CharT> operator+(basic_string<CharT> &&lhs, CharT rhs) {
    lhs.push_back(rhs);
    return std::move(lhs);
}

using string = basic_string<char>;

} // namespace pvzstl

#endif // PVZ_STL_PVZSTL_STRING_H
