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

#include <cassert>

#include <atomic>
#include <stdexcept>
#include <string>
#include <type_traits>

#ifdef PVZ_VERSION
extern uintptr_t gLibBaseOffset;
#endif

namespace pvzstl {

template <typename CharT>
class basic_string;

template <typename CharT>
[[nodiscard]] basic_string<CharT> operator+(const basic_string<CharT> &lhs, const basic_string<CharT> &rhs);

template <typename CharT>
[[nodiscard]] basic_string<CharT> operator+(const CharT *lhs, const basic_string<CharT> &rhs);

template <typename CharT>
[[nodiscard]] basic_string<CharT> operator+(CharT lhs, const basic_string<CharT> &rhs);

template <typename CharT>
[[nodiscard]] basic_string<CharT> operator+(const basic_string<CharT> &lhs, const CharT *rhs);

template <typename CharT>
[[nodiscard]] basic_string<CharT> operator+(const basic_string<CharT> &lhs, CharT rhs);

template <typename Tp, typename CharT>
concept __convertible_to_string_view = std::is_convertible_v<const Tp &, std::basic_string_view<CharT>> && !std::is_convertible_v<const Tp &, const CharT *>;

struct __uninitialized_size_tag {};

/**
 * @class 采用写时复制 (COW) 实现的字符串类模板
 *
 * g++ 在版本 5 前 std::string 的实现 (简化版).
 * 参考资料: https://gcc.gnu.org/onlinedocs/gcc-4.9.4/libstdc++/api/a00745.html
 */
template <typename CharT>
class basic_string {
public:
    static_assert(std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t> || std::is_same_v<CharT, char32_t>);

    using traits_type = std::char_traits<CharT>;
    using value_type = CharT;
    using size_type = std::uint32_t;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using reference = value_type &;
    using const_reference = const value_type &;

#ifdef __cpp_lib_ranges_as_const
    using const_iterator = std::basic_const_iterator<const_pointer>;
#else
    using const_iterator = const_pointer;
#endif
    using iterator = const_iterator;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using reverse_iterator = const_reverse_iterator;

    using __self_view = std::basic_string_view<CharT>;

    static constexpr size_type npos = static_cast<size_type>(-1);

    basic_string() noexcept
        : __data_{__rep::__empty_rep().__data} {}

    basic_string(const basic_string &other) noexcept
        : __data_{other.__get_rep()->__ref_copy()} {}

    basic_string(basic_string &&other) noexcept
        : __data_{other.__data_} {
        other.__data_ = __rep::__empty_rep().__data;
    }

    basic_string(const basic_string &str, size_type pos, size_type n) {
        str.__check_range(pos, "basic_string");
        __init(str.c_str() + pos, std::min(n, str.size() - pos));
    }

    basic_string(const basic_string &str, size_type pos) {
        str.__check_range(pos, "basic_string");
        __init(str.c_str() + pos, str.size() - pos);
    }

    basic_string(basic_string &&str, size_type pos, size_type n)
        : basic_string{std::move(str.assign(str, pos, n))} {}

    basic_string(basic_string &&str, size_type pos)
        : basic_string{std::move(str.assign(str, pos))} {}

    basic_string(const CharT *s, size_type n) {
        assert((s != nullptr || n == 0) && "basic_string(const CharT *, n) detected nullptr");
        __init(s, n);
    }

    basic_string(const CharT *s) {
        assert((s != nullptr) && "basic_string(const CharT *) detected nullptr");
        __init(s, traits_type::length(s));
    }

    basic_string(std::nullptr_t) = delete;

    ~basic_string() noexcept {
        __get_rep()->__dispose();
    }

    /* implicit */ operator __self_view() const noexcept {
        return __self_view{c_str(), size()};
    }

    basic_string &operator=(const basic_string &other) noexcept {
        if (__get_rep() != other.__get_rep()) {
            __get_rep()->__dispose();
            __data_ = other.__get_rep()->__ref_copy();
        }
        return *this;
    }

    basic_string &operator=(basic_string &&other) noexcept {
        swap(other);
        return *this;
    }

    basic_string &operator=(const CharT *s) {
        return assign(s);
    }

    basic_string &operator=(std::nullptr_t) = delete;

    basic_string &assign(const basic_string &str, size_type pos, size_type n = npos) {
        str.__check_range(pos, "basic_string::assign");
        return assign(str.c_str() + pos, std::min(n, str.size() - pos));
    }

    basic_string &assign(const basic_string &str) {
        return *this = str;
    }

    basic_string &assign(basic_string &&str) {
        return *this = std::move(str);
    }

    basic_string &assign(const CharT *s, size_type n) {
        assert((s != nullptr || n == 0) && "basic_string::assign received nullptr");
        __check_length(0, n, "basic_string::assign");
        if (__disjunct(s) || __get_rep()->__is_shared()) {
            return __replace_safe(0, size(), s, n);
        }
        const size_type pos = s - c_str();
        if (pos >= n) {
            traits_type::copy(__data_, s, n);
        } else if (pos > 0) {
            traits_type::move(__data_, s, n);
        }
        __get_rep()->__set_size(n);
        return *this;
    }

    basic_string &assign(const CharT *s) {
        assert((s != nullptr) && "basic_string::assign received nullptr");
        return assign(s, traits_type::length(s));
    }

    [[nodiscard]] const CharT &at(size_type pos) const {
        if (pos >= size()) {
            throw std::out_of_range{"basic_string::at"};
        }
        return c_str()[pos];
    }

    [[nodiscard]] const CharT &operator[](size_type pos) const noexcept {
        assert((pos <= size()) && "string index out of bounds");
        return c_str()[pos];
    }

    [[nodiscard]] const CharT &front() const noexcept {
        assert(!empty() && "basic_string::front(): string is empty");
        return *c_str();
    }

    [[nodiscard]] const CharT &back() const noexcept {
        assert(!empty() && "basic_string::back(): string is empty");
        return *(c_str() + size() - 1);
    }

    [[nodiscard]] const CharT *data() const noexcept {
        return __data_;
    }

    [[nodiscard]] const CharT *c_str() const noexcept {
        return __data_;
    }

    [[nodiscard]] const_iterator begin() const noexcept {
        return cbegin();
    }

    [[nodiscard]] const_iterator end() const noexcept {
        return cend();
    }

    [[nodiscard]] const_iterator cbegin() const noexcept {
        return const_iterator{c_str()};
    }

    [[nodiscard]] const_iterator cend() const noexcept {
        return const_iterator{c_str() + size()};
    }

    [[nodiscard]] const_reverse_iterator rbegin() const noexcept {
        return crbegin();
    }

    [[nodiscard]] const_reverse_iterator rend() const noexcept {
        return crend();
    }

    [[nodiscard]] const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator{cend()};
    }

    [[nodiscard]] const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator{cbegin()};
    }

    [[nodiscard]] bool empty() const noexcept {
        return size() == 0;
    }

    [[nodiscard]] size_type size() const noexcept {
        return __get_rep()->__size;
    }

    [[nodiscard]] size_type max_size() const noexcept {
        return __rep::__max_size;
    }

    void reserve(size_type new_cap) {
        const size_type old_cap = capacity();
        if ((new_cap <= old_cap) && !__get_rep()->__is_shared()) {
            return;
        }
        const size_type sz = size();
        CharT *tmp = __get_rep()->__clone(std::max(new_cap, sz) - sz);
        __get_rep()->__dispose();
        __data_ = tmp;
    }

    [[nodiscard]] size_type capacity() const noexcept {
        return __get_rep()->__capacity;
    }

    void clear() noexcept {
        if (__get_rep()->__is_shared()) {
            __get_rep()->__dispose();
            __data_ = __rep::__empty_rep().__data;
        } else {
            __get_rep()->__set_size(0);
        }
    }

    basic_string &insert(size_type pos1, const basic_string &str, size_type pos2, size_type n = npos) {
        str.__check_range(pos2, "basic_string::insert");
        return insert(pos1, str.c_str() + pos2, std::min(n, str.size() - pos2));
    }

    basic_string &insert(size_type pos, const basic_string &str) {
        return insert(pos, str, 0, str.size());
    }

    basic_string &insert(size_type pos, const CharT *s, size_type n) {
        assert((s != nullptr || n == 0) && "basic_string::insert received nullptr");
        __check_range(pos, "basic_string::insert");
        __check_length(0, n, "basic_string::insert");
        if (__disjunct(s) || __get_rep()->__is_shared()) {
            return __replace_safe(pos, 0, s, n);
        }
        const size_type off = s - c_str();
        __mutate(pos, 0, n);
        s = c_str() + off;
        CharT *p = __data_ + pos;
        if (s + n <= p) {
            traits_type::copy(p, s, n);
        } else if (s >= p) {
            traits_type::copy(p, s + n, n);
        } else {
            const size_type nleft = p - s;
            traits_type::copy(p, s, nleft);
            traits_type::copy(p + nleft, p + n, n - nleft);
        }
        return *this;
    }

    basic_string &insert(size_type pos, const CharT *s) {
        assert((s != nullptr) && "basic_string::insert received nullptr");
        return insert(pos, s, traits_type::length(s));
    }

    basic_string &erase(size_type pos = 0, size_type n = npos) {
        __mutate(__check_range(pos, "basic_string::erase"), std::min(n, size() - pos), 0);
        return *this;
    }

    void push_back(CharT c) {
        const size_type len = size() + 1;
        reserve(len);
        __data_[size()] = c;
        __get_rep()->__set_size(len);
    }

    void pop_back() {
        assert(!empty() && "basic_string::pop_back(): string is already empty");
        erase(size() - 1, 1);
    }

    basic_string &append(const basic_string &str, size_type pos, size_type n = npos) {
        str.__check_range(pos, "basic_string::append");
        return append(str.c_str() + pos, std::min(n, str.size() - pos));
    }

    basic_string &append(const basic_string &str) {
        return append(str.c_str(), str.size());
    }

    basic_string &append(const CharT *s, size_type n) {
        assert((s != nullptr || n == 0) && "basic_string::append received nullptr");
        if (n == 0) {
            return *this;
        }
        __check_length(0, n, "basic_string::append");
        const size_type len = n + size();
        if (len > capacity() || __get_rep()->__is_shared()) {
            if (__disjunct(s)) {
                reserve(len);
            } else {
                const size_type off = s - c_str();
                reserve(len);
                s = c_str() + off;
            }
        }
        traits_type::copy(__data_ + size(), s, n);
        __get_rep()->__set_size(len);
        return *this;
    }

    basic_string &append(const CharT *s) {
        assert((s != nullptr) && "basic_string::append received nullptr");
        return append(s, traits_type::length(s));
    }

    basic_string &operator+=(const basic_string &str) {
        return append(str);
    }

    basic_string &operator+=(const CharT *s) {
        return append(s);
    }

    basic_string &operator+=(CharT c) {
        push_back(c);
        return *this;
    }

    basic_string &replace(size_type pos1, size_type n1, const basic_string &str, size_type pos2, size_type n2 = npos) {
        str.__check_range(pos2, "basic_string::replace");
        return replace(pos1, n1, str.c_str() + pos2, std::min(n2, str.size() - pos2));
    }

    basic_string &replace(size_type pos, size_type n, const basic_string &str) {
        return replace(pos, n, str.c_str(), str.size());
    }

    basic_string &replace(size_type pos, size_type n1, const CharT *s, size_type n2) {
        assert((s != nullptr || n2 == 0) && "basic_string::replace received nullptr");
        __check_range(pos, "basic_string::replace");
        n1 = std::min(n1, size() - pos);
        __check_length(n1, n2, "basic_string::replace");
        bool left;
        if (__disjunct(s) || __get_rep()->__is_shared()) {
            return __replace_safe(pos, n1, s, n2);
        } else if ((left = (s + n2 <= c_str() + pos)) || (c_str() + pos + n1 <= s)) {
            size_type off = s - c_str();
            if (!left) {
                off += n2 - n1;
            }
            __mutate(pos, n1, n2);
            traits_type::copy(__data_ + pos, c_str() + off, n2);
            return *this;
        } else {
            const basic_string tmp{s, n2};
            return __replace_safe(pos, n1, tmp.c_str(), n2);
        }
    }

    basic_string &replace(size_type pos, size_type n, const CharT *s) {
        assert((s != nullptr) && "basic_string::replace received nullptr");
        return replace(pos, n, s, traits_type::length(s));
    }

    size_type copy(CharT *dest, size_type n, size_type pos = 0) const {
        __check_range(pos, "basic_string::copy");
        n = std::min(n, size() - pos);
        assert((dest != nullptr || n == 0) && "basic_string::copy received nullptr");
        if (n > 0) {
            traits_type::copy(dest, c_str() + pos, n);
        }
        return n;
    }

    void swap(basic_string &other) noexcept {
        std::swap(__data_, other.__data_);
    }

    [[nodiscard]] size_type find(const basic_string &str, size_type pos = 0) const noexcept {
        return find(__self_view{str}, pos);
    }

    template <__convertible_to_string_view<CharT> StringViewLike>
    [[nodiscard]] size_type find(const StringViewLike &t, size_type pos = 0) const noexcept(std::is_nothrow_convertible_v<const StringViewLike &, __self_view>) {
        const auto xpos = __self_view{c_str(), size()}.find(__self_view{t}, pos);
        if constexpr (std::is_same_v<size_type, typename __self_view::size_type>) {
            return xpos;
        } else {
            return (xpos != __self_view::npos) ? static_cast<size_type>(xpos) : npos;
        }
    }

    [[nodiscard]] size_type find(const CharT *s, size_type pos, size_type n) const {
        assert((s != nullptr || n == 0) && "basic_string::find received nullptr");
        return find(__self_view{s, n}, pos);
    }

    [[nodiscard]] size_type find(const CharT *s, size_type pos = 0) const {
        assert((s != nullptr) && "basic_string::find received nullptr");
        return find(__self_view{s}, pos);
    }

    [[nodiscard]] size_type find(CharT c, size_type pos = 0) const noexcept {
        const size_type sz = size();
        if (pos >= sz) {
            return npos;
        }
        const size_type n = sz - pos;
        const CharT *p = traits_type::find(c_str() + pos, n, c);
        return (p != nullptr) ? (p - c_str()) : npos;
    }

    [[nodiscard]] bool starts_with(__self_view sv) const noexcept {
        return __self_view{c_str(), size()}.starts_with(sv);
    }

    [[nodiscard]] bool starts_with(CharT c) const noexcept {
        return !empty() && (front() == c);
    }

    [[nodiscard]] bool starts_with(const CharT *s) const {
        assert((s != nullptr) && "basic_string::starts_with received nullptr");
        return starts_with(__self_view{s});
    }

    [[nodiscard]] bool ends_with(__self_view sv) const noexcept {
        return __self_view{c_str(), size()}.ends_with(sv);
    }

    [[nodiscard]] bool ends_with(CharT c) const noexcept {
        return !empty() && (back() == c);
    }

    [[nodiscard]] bool ends_with(const CharT *s) const {
        assert((s != nullptr) && "basic_string::ends_with received nullptr");
        return ends_with(__self_view{s});
    }

    [[nodiscard]] bool contains(__self_view sv) const noexcept {
        return __self_view{c_str(), size()}.contains(sv);
    }

    [[nodiscard]] bool contains(CharT c) const noexcept {
        return __self_view{c_str(), size()}.contains(c);
    }

    [[nodiscard]] bool contains(const CharT *s) const {
        assert((s != nullptr) && "basic_string::contains received nullptr");
        return __self_view{c_str(), size()}.contains(s);
    }

    [[nodiscard]] basic_string substr(size_type pos = 0, size_type n = npos) const & {
        return basic_string{*this, __check_range(pos, "basic_string::substr"), n};
    }

    [[nodiscard]] basic_string substr(size_type pos = 0, size_type n = npos) && {
        return basic_string{std::move(*this), __check_range(pos, "basic_string::substr"), n};
    }

    /**
     * @brief 就地构造 pvzstl::basic_string 对象.
     */
    template <typename... Args>
        requires std::constructible_from<basic_string, Args...>
    basic_string &emplace(Args &&...args) {
        return *::new (this) basic_string{std::forward<Args>(args)...};
    }

protected:
    mutable CharT *__data_;

    struct __rep {
        static constexpr size_type __max_size = 0x3FFF'FFFC;

        size_type __size;                // 字符数
        size_type __capacity;            // 已分配存储空间中可以容纳的字符数
        std::atomic_int32_t __ref_count; // 引用计数 (小于等于 0 时释放内存)
        CharT __data[];                  // 作为字符存储的底层数组 (柔性数组成员)

        [[nodiscard]] static __rep &__empty_rep() noexcept {
#ifdef PVZ_VERSION
            assert(::gLibBaseOffset != 0);
            static constexpr uintptr_t offset = (sizeof(CharT) == sizeof(int8_t)) ? /* string */ 0x71BB54 : /* wstring */ 0x69E45C;
            static uintptr_t __empty_rep_storage_addr = ::gLibBaseOffset + offset;
            return *reinterpret_cast<__rep *>(__empty_rep_storage_addr);
#else
            alignas(__rep) static std::byte __empty_rep_storage[sizeof(__rep) + sizeof(CharT)] = {};
            return *reinterpret_cast<__rep *>(__empty_rep_storage);
#endif
        }

        [[nodiscard]] static __rep *__create(size_type cap, size_type old_cap) {
            if (cap > __max_size) {
                throw std::length_error{"basic_string::__rep::__create"};
            }
            if ((cap > old_cap) && (cap < 2 * old_cap)) {
                cap = 2 * old_cap;
            }
            const size_type alloc_sz = sizeof(__rep) + sizeof(CharT) * (cap + 1);
            // NB: here pass align
            return ::new (::operator new(alloc_sz)) __rep{.__capacity = cap, .__ref_count = 0};
        }

        [[nodiscard]] CharT *__ref_copy() noexcept {
            if (this != &__empty_rep()) {
                ++__ref_count;
            }
            return __data;
        }

        [[nodiscard]] CharT *__clone(size_type res = 0) const {
            __rep *r = __create(__size + res, __capacity);
            if (__size > 0) {
                traits_type::copy(r->__data, __data, __size);
            }
            r->__set_size(__size);
            return r->__data;
        }

        void __dispose() noexcept {
            if ((this != &__empty_rep()) && (__ref_count-- <= 0)) {
                ::operator delete(this);
            }
        }

        [[nodiscard]] bool __is_shared() const noexcept {
            return __ref_count > 0;
        }

        void __set_size(size_type sz) noexcept {
            if (this != &__empty_rep()) {
                __size = sz;
                __data[sz] = CharT{0};
            }
        }
    };

    // Construct a string with enough storage to hold `size` characters, but don't initialize the characters.
    // The contents of the string, including the null terminator, must be initialized separately.
    explicit basic_string(__uninitialized_size_tag, size_type size)
        : __data_{__rep::__create(size, 0)->__data} {
        __get_rep()->__size = size;
    }

    [[nodiscard]] __rep *__get_rep() const noexcept {
        return reinterpret_cast<__rep *>(__data_) - 1;
    }

    size_type __check_range(size_type pos, const char *msg) const {
        if (pos > size()) {
            throw std::out_of_range{msg};
        }
        return pos;
    }

    void __check_length(size_type n1, size_type n2, const char *msg) const {
        if (max_size() - (size() - n1) < n2) {
            throw std::length_error{msg};
        }
    }

    [[nodiscard]] bool __disjunct(const CharT *s) const noexcept {
        return (s < c_str()) || (c_str() + size() < s);
    }

    void __init(const CharT *s, size_type sz) {
        __data_ = __rep::__create(sz, 0)->__data;
        traits_type::copy(__data_, s, sz);
        __get_rep()->__set_size(sz);
    }

    // 清空范围 [ `begin() + pos`, `begin() + pos + len1` ) 中的字符,
    // 并在原位置预留大小为 `len2` 的空间.
    void __mutate(size_type pos, size_type len1, size_type len2) {
        const size_type old_sz = size();
        const size_type new_sz = old_sz + len2 - len1;
        const size_type how_much = old_sz - pos - len1;
        if (new_sz > capacity() || __get_rep()->__is_shared()) {
            __rep *r = __rep::__create(new_sz, capacity());
            if (pos > 0) {
                traits_type::copy(r->__data, c_str(), pos);
            }
            if (how_much > 0) {
                traits_type::copy((r->__data + pos + len2), (c_str() + pos + len1), how_much);
            }
            __get_rep()->__dispose();
            __data_ = r->__data;
        } else if ((how_much > 0) && (len1 != len2)) {
            traits_type::move((__data_ + pos + len2), (c_str() + pos + len1), how_much);
        }
        __get_rep()->__set_size(new_sz);
    }

    basic_string &__replace_safe(size_type pos, size_type n1, const CharT *s, size_type n2) {
        __mutate(pos, n1, n2);
        if (n2 > 0) {
            traits_type::copy(__data_ + pos, s, n2);
        }
        return *this;
    }

    friend basic_string operator+ <>(const basic_string &, const basic_string &);
    friend basic_string operator+ <>(const CharT *, const basic_string &);
    friend basic_string operator+ <>(CharT, const basic_string &);
    friend basic_string operator+ <>(const basic_string &, const CharT *);
    friend basic_string operator+ <>(const basic_string &, CharT);
};

template <typename CharT>
[[nodiscard]] bool operator==(const basic_string<CharT> &lhs, const basic_string<CharT> &rhs) noexcept {
    return std::basic_string_view<CharT>{lhs} == std::basic_string_view<CharT>{rhs};
}

template <typename CharT>
[[nodiscard]] bool operator==(const basic_string<CharT> &lhs, const CharT *rhs) {
    return std::basic_string_view<CharT>{lhs} == std::basic_string_view<CharT>{rhs};
}

template <typename CharT>
[[nodiscard]] auto operator<=>(const basic_string<CharT> &lhs, const basic_string<CharT> &rhs) noexcept {
    return std::basic_string_view<CharT>{lhs} <=> std::basic_string_view<CharT>{rhs};
}

template <typename CharT>
[[nodiscard]] auto operator<=>(const basic_string<CharT> &lhs, const CharT *rhs) {
    return std::basic_string_view<CharT>{lhs} <=> std::basic_string_view<CharT>{rhs};
}

template <typename CharT>
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

template <typename CharT>
[[nodiscard]] basic_string<CharT> operator+(const CharT *lhs, const basic_string<CharT> &rhs) {
    assert((lhs != nullptr) && "operator+(const CharT *, const basic_string &) received nullptr");
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

template <typename CharT>
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

template <typename CharT>
[[nodiscard]] basic_string<CharT> operator+(const basic_string<CharT> &lhs, const CharT *rhs) {
    assert((rhs != nullptr) && "operator+(const basic_string &, const CharT *) received nullptr");
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

template <typename CharT>
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

template <typename CharT>
[[nodiscard]] basic_string<CharT> operator+(basic_string<CharT> &&lhs, const basic_string<CharT> &rhs) {
    return std::move(lhs.append(rhs));
}

template <typename CharT>
[[nodiscard]] basic_string<CharT> operator+(const basic_string<CharT> &lhs, basic_string<CharT> &&rhs) {
    return std::move(rhs.insert(0, lhs));
}

template <typename CharT>
[[nodiscard]] basic_string<CharT> operator+(basic_string<CharT> &&lhs, basic_string<CharT> &&rhs) {
    return std::move(lhs.append(rhs));
}

template <typename CharT>
[[nodiscard]] basic_string<CharT> operator+(const CharT *lhs, basic_string<CharT> &&rhs) {
    assert((lhs != nullptr) && "operator+(const CharT *, basic_string &&) received nullptr");
    return std::move(rhs.insert(0, lhs));
}

template <typename CharT>
[[nodiscard]] basic_string<CharT> operator+(basic_string<CharT> &&lhs, const CharT *rhs) {
    assert((rhs != nullptr) && "operator+(basic_string &&, const CharT *) received nullptr");
    return std::move(lhs.append(rhs));
}

template <typename CharT>
[[nodiscard]] basic_string<CharT> operator+(basic_string<CharT> &&lhs, CharT rhs) {
    lhs.push_back(rhs);
    return std::move(lhs);
}

using string = basic_string<char>;

/* `basic_string<int>` in PvZ */
// using wstring = basic_string<wchar_t>;
// using u32string = basic_string<char32_t>;

} // namespace pvzstl

#endif // PVZ_STL_PVZSTL_STRING_H
