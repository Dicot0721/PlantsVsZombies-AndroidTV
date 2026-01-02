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

template <typename SV, typename CharT>
concept _convertible_to_string_view = std::is_convertible_v<const SV &, std::basic_string_view<CharT>> && !std::is_convertible_v<const SV &, const CharT *>;

/**
 * @class 采用写时复制 (COW) 实现的字符串类模板
 *
 * g++ 在版本 5 前 std::string 的实现 (简化版).
 * 参考资料: https://gcc.gnu.org/onlinedocs/gcc-4.9.4/libstdc++/api/a00745.html
 */
template <typename CharT>
class basic_string {
public:
    using traits_type = std::char_traits<CharT>;
    using value_type = CharT;
    using size_type = /* std::size_t */ std::uint32_t;
    using difference_type = std::ptrdiff_t;
    using pointer = CharT *;
    using const_pointer = const CharT *;
    using reference = CharT &;
    using const_reference = const CharT &;

#ifdef __cpp_lib_ranges_as_const
    using const_iterator = std::basic_const_iterator<const_pointer>;
#else
    using const_iterator = const_pointer;
#endif
    using iterator = const_iterator;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using reverse_iterator = const_reverse_iterator;

    using _self_view = std::basic_string_view<CharT>;

#ifdef PVZ_VERSION
    static_assert(std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t> || std::is_same_v<CharT, char32_t>);
#else
    static_assert(!std::is_array_v<CharT>, "Character type of basic_string must not be an array");
    static_assert(std::is_standard_layout_v<CharT>, "Character type of basic_string must be standard-layout");
    static_assert(std::is_trivial_v<CharT>, "Character type of basic_string must be trivial");
    static_assert(std::is_same_v<CharT, typename traits_type::char_type>, "traits_type::char_type must be the same type as CharT");
#endif

    static constexpr size_type npos = static_cast<size_type>(-1);

    basic_string() noexcept // strengthened
        : _dataplus{_rep::_empty_rep()._data} {}

    basic_string(const basic_string &other)
        : _dataplus{other._get_rep()->_grab()} {}

    basic_string(basic_string &&other) noexcept
        : _dataplus{other._dataplus} {
        other._dataplus = _rep::_empty_rep()._data;
    }

    basic_string(const basic_string &str, size_type pos, size_type n)
        : _dataplus{_construct(str.c_str() + str._check_range(pos, "basic_string"), std::min(n, str.size() - pos))} {}

    basic_string(const basic_string &str, size_type pos)
        : _dataplus{_construct(str.c_str() + str._check_range(pos, "basic_string"), str.size() - pos)} {}

    basic_string(basic_string &&str, size_type pos, size_type n)
        : basic_string(std::move(str.assign(str, pos, n))) {}

    basic_string(basic_string &&str, size_type pos)
        : basic_string(std::move(str.assign(str, pos))) {}

    basic_string(const CharT *s, size_type n)
        : _dataplus{_construct(s, n)} {}

    basic_string(const CharT *s)
        : _dataplus{_construct(s, ((s != nullptr) ? traits_type::length(s) : npos))} {}

    basic_string(std::nullptr_t) = delete;

    basic_string(size_type n, CharT c)
        : _dataplus{_construct(n, c)} {}

    basic_string(std::initializer_list<CharT> il)
        : _dataplus{_construct(il.begin(), il.size())} {}

    ~basic_string() {
        _get_rep()->_dispose();
    }

    /* implicit */ operator _self_view() const noexcept {
        return _self_view{c_str(), size()};
    }

    basic_string &operator=(const basic_string &other) {
        if (_get_rep() != other._get_rep()) {
            _get_rep()->_dispose();
            _dataplus = other._get_rep()->_grab();
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

    basic_string &operator=(CharT c) {
        return assign(1, c);
    }

    basic_string &operator=(std::initializer_list<CharT> il) {
        return assign(il);
    }

    basic_string &assign(const basic_string &str, size_type pos, size_type n = npos) {
        str._check_range(pos, "basic_string::assign");
        return assign(str.c_str() + pos, std::min(n, str.size() - pos));
    }

    basic_string &assign(const basic_string &str) {
        return *this = str;
    }

    basic_string &assign(basic_string &&str) noexcept {
        return *this = std::move(str);
    }

    basic_string &assign(const CharT *s, size_type n) {
        assert((s != nullptr || n == 0) && "basic_string::assign received nullptr");
        _check_length(0, n, "basic_string::assign");
        if (_disjunct(s) || _get_rep()->_is_shared()) {
            return _replace_safe(0, size(), s, n);
        }
        const size_type pos = static_cast<size_type>(s - c_str());
        if (pos >= n) {
            traits_type::copy(_dataplus, s, n);
        } else if (pos > 0) {
            traits_type::move(_dataplus, s, n);
        }
        _get_rep()->_set_size_and_sharable(n);
        return *this;
    }

    basic_string &assign(const CharT *s) {
        assert((s != nullptr) && "basic_string::assign received nullptr");
        return assign(s, traits_type::length(s));
    }

    basic_string &assign(size_type n, CharT c) {
        return _replace_aux(0, size(), n, c);
    }

    basic_string &assign(std::initializer_list<CharT> il) {
        return assign(il.begin(), il.size());
    }

    [[nodiscard]] const CharT &at(size_type pos) const {
        if (pos >= size()) {
            throw std::out_of_range{"basic_string::at"};
        }
        return c_str()[pos];
    }

    [[nodiscard]] CharT &at(size_type pos) {
        if (pos >= size()) {
            throw std::out_of_range{"basic_string::at"};
        }
        _leak();
        return _dataplus[pos];
    }

    // 不提供非 const 重载以满足 noexcept 要求,
    // 需要修改请使用 `at()`.
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
        return _dataplus;
    }

    [[nodiscard]] const CharT *c_str() const noexcept {
        return _dataplus;
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
        return _get_rep()->_size;
    }

    [[nodiscard]] size_type length() const noexcept {
        return _get_rep()->_size;
    }

    [[nodiscard]] constexpr size_type max_size() const noexcept {
        return _rep::_max_size();
    }

    void reserve(size_type new_cap) {
        const size_type old_cap = capacity();
        if ((new_cap <= old_cap) && !_get_rep()->_is_shared()) {
            return;
        }
        CharT *tmp = _get_rep()->_clone(std::max(new_cap, old_cap) - size());
        _get_rep()->_dispose();
        _dataplus = tmp;
    }

    [[nodiscard]] size_type capacity() const noexcept {
        return _get_rep()->_capacity;
    }

    void shrink_to_fit() {
        if (capacity() > size() || _get_rep()->_is_shared()) {
            CharT *tmp = _get_rep()->_clone();
            _get_rep()->_dispose();
            _dataplus = tmp;
        }
    }

    void clear() noexcept {
        if (_get_rep()->_is_shared()) {
            _get_rep()->_dispose();
            _dataplus = _rep::_empty_rep()._data;
        } else {
            _get_rep()->_set_size_and_sharable(0);
        }
    }

    basic_string &insert(size_type pos1, const basic_string &str, size_type pos2, size_type n = npos) {
        str._check_range(pos2, "basic_string::insert");
        return insert(pos1, str.c_str() + pos2, std::min(n, str.size() - pos2));
    }

    basic_string &insert(size_type pos, const basic_string &str) {
        return insert(pos, str.c_str(), str.size());
    }

    basic_string &insert(size_type pos, const CharT *s, size_type n) {
        assert((s != nullptr || n == 0) && "basic_string::insert received nullptr");
        _check_range(pos, "basic_string::insert");
        _check_length(0, n, "basic_string::insert");
        if (_disjunct(s) || _get_rep()->_is_shared()) {
            return _replace_safe(pos, 0, s, n);
        }
        const size_type off = static_cast<size_type>(s - c_str());
        _mutate(pos, 0, n);
        s = c_str() + off;
        CharT *p = _dataplus + pos;
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

    basic_string &insert(size_type pos, size_type n, CharT c) {
        return _replace_aux(_check_range(pos, "basic_string::insert"), 0, n, c);
    }

    basic_string &erase(size_type pos = 0, size_type n = npos) {
        _mutate(_check_range(pos, "basic_string::erase"), std::min(n, size() - pos), 0);
        return *this;
    }

    void push_back(CharT c) {
        const size_type len = size() + 1;
        reserve(len);
        _dataplus[size()] = c;
        _get_rep()->_set_size_and_sharable(len);
    }

    void pop_back() {
        assert(!empty() && "basic_string::pop_back(): string is already empty");
        erase(size() - 1, 1);
    }

    basic_string &append(const basic_string &str, size_type pos, size_type n = npos) {
        str._check_range(pos, "basic_string::append");
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
        _check_length(0, n, "basic_string::append");
        const size_type len = n + size();
        if (_disjunct(s)) {
            reserve(len);
        } else {
            const size_type off = static_cast<size_type>(s - c_str());
            reserve(len);
            s = c_str() + off;
        }
        traits_type::copy(_dataplus + size(), s, n);
        _get_rep()->_set_size_and_sharable(len);
        return *this;
    }

    basic_string &append(const CharT *s) {
        assert((s != nullptr) && "basic_string::append received nullptr");
        return append(s, traits_type::length(s));
    }

    basic_string &append(size_type n, CharT c) {
        if (n > 0) {
            _check_length(0, n, "basic_string::append");
            const size_type len = n + size();
            reserve(len);
            traits_type::assign(_dataplus + size(), n, c);
            _get_rep()->_set_size_and_sharable(len);
        }
        return *this;
    }

    basic_string &append(std::initializer_list<CharT> il) {
        return append(il.begin(), il.size());
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

    basic_string &operator+=(std::initializer_list<CharT> il) {
        return append(il);
    }

    basic_string &replace(size_type pos1, size_type n1, const basic_string &str, size_type pos2, size_type n2 = npos) {
        str._check_range(pos2, "basic_string::replace");
        return replace(pos1, n1, str.c_str() + pos2, std::min(n2, str.size() - pos2));
    }

    basic_string &replace(size_type pos, size_type n, const basic_string &str) {
        return replace(pos, n, str.c_str(), str.size());
    }

    basic_string &replace(size_type pos, size_type n1, const CharT *s, size_type n2) {
        assert((s != nullptr || n2 == 0) && "basic_string::replace received nullptr");
        _check_range(pos, "basic_string::replace");
        n1 = std::min(n1, size() - pos);
        _check_length(n1, n2, "basic_string::replace");
        bool left;
        if (_disjunct(s) || _get_rep()->_is_shared()) {
            return _replace_safe(pos, n1, s, n2);
        } else if ((left = (s + n2 <= c_str() + pos)) || (c_str() + pos + n1 <= s)) {
            size_type off = static_cast<size_type>(s - c_str());
            if (!left) {
                off += n2 - n1;
            }
            _mutate(pos, n1, n2);
            traits_type::copy(_dataplus + pos, c_str() + off, n2);
            return *this;
        } else {
            const basic_string tmp(s, n2);
            return _replace_safe(pos, n1, tmp.c_str(), n2);
        }
    }

    basic_string &replace(size_type pos, size_type n, const CharT *s) {
        assert((s != nullptr) && "basic_string::replace received nullptr");
        return replace(pos, n, s, traits_type::length(s));
    }

    basic_string &replace(size_type pos, size_type n1, size_type n2, CharT c) {
        return _replace_aux(_check_range(pos, "basic_string::replace"), std::min(n1, size() - pos), n2, c);
    }

    size_type copy(CharT *dest, size_type n, size_type pos = 0) const {
        _check_range(pos, "basic_string::copy");
        n = std::min(n, size() - pos);
        assert((dest != nullptr || n == 0) && "basic_string::copy received nullptr");
        if (n > 0) {
            traits_type::copy(dest, c_str() + pos, n);
        }
        return n;
    }

    void resize(size_type n, CharT c) {
        const size_type sz = size();
        _check_length(sz, n, "basic_string::resize");
        if (n > sz) {
            append(n - sz, c);
        } else if (n < sz) {
            erase(n);
        }
    }

    void resize(size_type n) {
        resize(n, CharT{});
    }

    void swap(basic_string &other) noexcept /* strengthened */ {
        if (_get_rep()->_is_leaked()) {
            _get_rep()->_set_sharable();
        }
        if (other._get_rep()->_is_leaked()) {
            other._get_rep()->_set_sharable();
        }
        std::swap(_dataplus, other._dataplus);
    }

    [[nodiscard]] size_type find(const basic_string &str, size_type pos = 0) const noexcept {
        return find(_self_view{str}, pos);
    }

    template <_convertible_to_string_view<CharT> SV>
    [[nodiscard]] size_type find(const SV &t, size_type pos = 0) const noexcept(std::is_nothrow_convertible_v<const SV &, _self_view>) {
        const _self_view sv = t;
        return _from_view_pos(_self_view{*this}.find(sv, pos));
    }

    [[nodiscard]] size_type find(const CharT *s, size_type pos, size_type n) const {
        assert((s != nullptr || n == 0) && "basic_string::find received nullptr");
        return find(_self_view{s, n}, pos);
    }

    [[nodiscard]] size_type find(const CharT *s, size_type pos = 0) const {
        assert((s != nullptr) && "basic_string::find received nullptr");
        return find(_self_view{s}, pos);
    }

    [[nodiscard]] size_type find(CharT c, size_type pos = 0) const noexcept {
        return _from_view_pos(_self_view{*this}.find(c, pos));
    }

    [[nodiscard]] size_type rfind(const basic_string &str, size_type pos = npos) const noexcept {
        return rfind(_self_view{str}, pos);
    }

    template <_convertible_to_string_view<CharT> SV>
    [[nodiscard]] size_type rfind(const SV &t, size_type pos = npos) const noexcept(std::is_nothrow_convertible_v<const SV &, _self_view>) {
        const _self_view sv = t;
        return _from_view_pos(_self_view{*this}.rfind(sv, pos));
    }

    [[nodiscard]] size_type rfind(const CharT *s, size_type pos, size_type n) const {
        assert((s != nullptr || n == 0) && "basic_string::rfind received nullptr");
        return rfind(_self_view{s, n}, pos);
    }

    [[nodiscard]] size_type rfind(const CharT *s, size_type pos = npos) const {
        assert((s != nullptr) && "basic_string::rfind received nullptr");
        return rfind(_self_view{s}, pos);
    }

    [[nodiscard]] size_type rfind(CharT c, size_type pos = npos) const noexcept {
        return _from_view_pos(_self_view{*this}.rfind(c, pos));
    }

    [[nodiscard]] size_type find_first_of(const basic_string &str, size_type pos = 0) const noexcept {
        return find_first_of(_self_view{str}, pos);
    }

    template <_convertible_to_string_view<CharT> SV>
    [[nodiscard]] size_type find_first_of(const SV &t, size_type pos = 0) const noexcept(std::is_nothrow_convertible_v<const SV &, _self_view>) {
        const _self_view sv = t;
        return _from_view_pos(_self_view{*this}.find_first_of(sv, pos));
    }

    [[nodiscard]] size_type find_first_of(const CharT *s, size_type pos, size_type n) const {
        assert((s != nullptr || n == 0) && "basic_string::find_first_of received nullptr");
        return find_first_of(_self_view{s, n}, pos);
    }

    [[nodiscard]] size_type find_first_of(const CharT *s, size_type pos = 0) const {
        assert((s != nullptr) && "basic_string::find_first_of received nullptr");
        return find_first_of(_self_view{s}, pos);
    }

    [[nodiscard]] size_type find_first_of(CharT c, size_type pos = 0) const noexcept {
        return find(c, pos);
    }

    [[nodiscard]] size_type find_first_not_of(const basic_string &str, size_type pos = 0) const noexcept {
        return find_first_not_of(_self_view{str}, pos);
    }

    template <_convertible_to_string_view<CharT> SV>
    [[nodiscard]] size_type find_first_not_of(const SV &t, size_type pos = 0) const noexcept(std::is_nothrow_convertible_v<const SV &, _self_view>) {
        const _self_view sv = t;
        return _from_view_pos(_self_view{*this}.find_first_not_of(sv, pos));
    }

    [[nodiscard]] size_type find_first_not_of(const CharT *s, size_type pos, size_type n) const {
        assert((s != nullptr || n == 0) && "basic_string::find_first_not_of received nullptr");
        return find_first_not_of(_self_view{s, n}, pos);
    }

    [[nodiscard]] size_type find_first_not_of(const CharT *s, size_type pos = 0) const {
        assert((s != nullptr) && "basic_string::find_first_not_of received nullptr");
        return find_first_not_of(_self_view{s}, pos);
    }

    [[nodiscard]] size_type find_first_not_of(CharT c, size_type pos = 0) const noexcept {
        return _from_view_pos(_self_view{*this}.find_first_not_of(c, pos));
    }

    [[nodiscard]] size_type find_last_of(const basic_string &str, size_type pos = npos) const noexcept {
        return find_last_of(_self_view{str}, pos);
    }

    template <_convertible_to_string_view<CharT> SV>
    [[nodiscard]] size_type find_last_of(const SV &t, size_type pos = npos) const noexcept(std::is_nothrow_convertible_v<const SV &, _self_view>) {
        const _self_view sv = t;
        return _from_view_pos(_self_view{*this}.find_last_of(sv, pos));
    }

    [[nodiscard]] size_type find_last_of(const CharT *s, size_type pos, size_type n) const {
        assert((s != nullptr || n == 0) && "basic_string::find_last_of received nullptr");
        return find_last_of(_self_view{s, n}, pos);
    }

    [[nodiscard]] size_type find_last_of(const CharT *s, size_type pos = npos) const {
        assert((s != nullptr) && "basic_string::find_last_of received nullptr");
        return find_last_of(_self_view{s}, pos);
    }

    [[nodiscard]] size_type find_last_of(CharT c, size_type pos = npos) const noexcept {
        return rfind(c, pos);
    }

    [[nodiscard]] size_type find_last_not_of(const basic_string &str, size_type pos = npos) const noexcept {
        return find_last_not_of(_self_view{str}, pos);
    }

    template <_convertible_to_string_view<CharT> SV>
    [[nodiscard]] size_type find_last_not_of(const SV &t, size_type pos = npos) const noexcept(std::is_nothrow_convertible_v<const SV &, _self_view>) {
        const _self_view sv = t;
        return _from_view_pos(_self_view{*this}.find_last_not_of(sv, pos));
    }

    [[nodiscard]] size_type find_last_not_of(const CharT *s, size_type pos, size_type n) const {
        assert((s != nullptr || n == 0) && "basic_string::find_last_not_of received nullptr");
        return find_last_not_of(_self_view{s, n}, pos);
    }

    [[nodiscard]] size_type find_last_not_of(const CharT *s, size_type pos = npos) const {
        assert((s != nullptr) && "basic_string::find_last_not_of received nullptr");
        return find_last_not_of(_self_view{s}, pos);
    }

    [[nodiscard]] size_type find_last_not_of(CharT c, size_type pos = npos) const noexcept {
        return _from_view_pos(_self_view{*this}.find_last_not_of(c, pos));
    }

    [[nodiscard]] bool starts_with(_self_view sv) const noexcept {
        return _self_view{*this}.starts_with(sv);
    }

    [[nodiscard]] bool starts_with(const CharT *s) const {
        assert((s != nullptr) && "basic_string::starts_with received nullptr");
        return starts_with(_self_view{s});
    }

    [[nodiscard]] bool starts_with(CharT c) const noexcept {
        return !empty() && traits_type::eq(front(), c);
    }

    [[nodiscard]] bool ends_with(_self_view sv) const noexcept {
        return _self_view{*this}.ends_with(sv);
    }

    [[nodiscard]] bool ends_with(const CharT *s) const {
        assert((s != nullptr) && "basic_string::ends_with received nullptr");
        return ends_with(_self_view{s});
    }

    [[nodiscard]] bool ends_with(CharT c) const noexcept {
        return !empty() && traits_type::eq(back(), c);
    }

    [[nodiscard]] bool contains(_self_view sv) const noexcept {
        return _self_view{*this}.contains(sv);
    }

    [[nodiscard]] bool contains(const CharT *s) const {
        assert((s != nullptr) && "basic_string::contains received nullptr");
        return _self_view{*this}.contains(s);
    }

    [[nodiscard]] bool contains(CharT c) const noexcept {
        return _self_view{*this}.contains(c);
    }

    [[nodiscard]] basic_string substr(size_type pos = 0, size_type n = npos) const & {
        return basic_string(*this, _check_range(pos, "basic_string::substr"), n);
    }

    [[nodiscard]] basic_string substr(size_type pos = 0, size_type n = npos) && {
        return basic_string(std::move(*this), _check_range(pos, "basic_string::substr"), n);
    }

protected:
    struct _rep {
        size_type _size;                // 字符数
        size_type _capacity;            // 已分配存储空间中可以容纳的字符数
        std::atomic_int32_t _ref_count; // 引用计数 (小于等于 0 时释放内存)
        CharT _data[];                  // 作为字符存储的底层数组 (柔性数组成员)

        // C++ 标准中未明确在静态成员变量的初始化器中自身是否为完整类型 (`sizeof` 运算符需要完整类型),
        // 故将 `_max_size` 定义为静态成员函数.
        [[nodiscard]] static consteval size_type _max_size() noexcept {
            // npos = (m + 1) * sizeof(CharT) + sizeof(_rep)
            // max_size = m / 4
            return (((npos - sizeof(_rep)) / sizeof(CharT)) - 1) / 4;
        }

        [[nodiscard]] static _rep &_empty_rep() noexcept {
#ifdef PVZ_VERSION
            assert(::gLibBaseOffset != 0);
            constexpr uintptr_t offset = std::is_same_v<CharT, char> ? /* string */ 0x71BB54 : /* basic_string<int> */ 0x69E45C;
            return *reinterpret_cast<_rep *>(::gLibBaseOffset + offset);
#else
            alignas(_rep) static std::byte empty_rep_storage[sizeof(_rep) + sizeof(CharT)] = {};
            return *reinterpret_cast<_rep *>(&empty_rep_storage);
#endif
        }

        [[nodiscard]] static _rep *_create(size_type cap, size_type old_cap) {
            if (cap > _max_size()) {
                throw std::length_error{"basic_string::_rep::_create"};
            }
            constexpr size_type pagesize = 4096;
            constexpr size_type malloc_header_size = 4 * sizeof(void *);
            if ((cap > old_cap) && (cap < 2 * old_cap)) {
                cap = 2 * old_cap;
            }
            size_type size = (cap + 1) * sizeof(CharT) + sizeof(_rep);
            const size_type adj_size = size + malloc_header_size;
            if (adj_size > pagesize && cap > old_cap) {
                const size_type extra = pagesize - adj_size % pagesize;
                cap += extra / sizeof(CharT);
                if (cap > _max_size()) {
                    cap = _max_size();
                }
                size = (cap + 1) * sizeof(CharT) + sizeof(_rep);
            }
            void *place = ::operator new(size);
            _rep *p = ::new (place) _rep;
            p->_capacity = cap;
            return p;
        }

        [[nodiscard]] CharT *_ref_copy() noexcept {
            if (this != &_empty_rep()) {
                ++_ref_count;
            }
            return _data;
        }

        [[nodiscard]] CharT *_clone(size_type res = 0) const {
            _rep *r = _create(_size + res, _capacity);
            if (_size > 0) {
                traits_type::copy(r->_data, _data, _size);
            }
            r->_set_size(_size);
            return r->_data;
        }

        [[nodiscard]] CharT *_grab() {
            return !_is_leaked() ? _ref_copy() : _clone();
        }

        void _dispose() noexcept {
            if ((this != &_empty_rep()) && (_ref_count-- <= 0)) {
                ::operator delete(this);
            }
        }

        // 调用过不清楚是否修改自身数据的成员函数, 如 `operator[]`, `begin()`
        [[nodiscard]] bool _is_leaked() const noexcept {
            return _ref_count < 0;
        }

        [[nodiscard]] bool _is_shared() const noexcept {
            return _ref_count > 0;
        }

        void _set_leaked() noexcept {
            _ref_count = -1;
        }

        void _set_sharable() noexcept {
            _ref_count = 0;
        }

        void _set_size(size_type sz) noexcept {
            _size = sz;
            _data[sz] = CharT{};
        }

        void _set_size_and_sharable(size_type sz) noexcept {
            if (this != &_empty_rep()) {
                _set_sharable();
                _set_size(sz);
            }
        }
    };

    mutable CharT *_dataplus;

    [[nodiscard]] static CharT *_construct(const CharT *s, size_type n) {
        if (n == 0) {
            return _rep::_empty_rep()._data;
        }
        // NB: Not required, but considered best practice.
        if (s == nullptr) {
            throw std::logic_error{"basic_string::_construct null not valid"};
        }
        _rep *r = _rep::_create(n, 0);
        traits_type::copy(r->_data, s, n);
        r->_set_size(n);
        return r->_data;
    }

    [[nodiscard]] static CharT *_construct(size_type n, CharT c) {
        if (n == 0) {
            return _rep::_empty_rep()._data;
        }
        _rep *r = _rep::_create(n, 0);
        traits_type::assign(r->_data, n, c);
        r->_set_size(n);
        return r->_data;
    }

    [[nodiscard]] static constexpr size_type _from_view_pos(_self_view::size_type pos) noexcept {
        if constexpr (npos == _self_view::npos) {
            return pos;
        } else {
            return (pos == _self_view::npos) ? npos : static_cast<size_type>(pos);
        }
    }

    [[nodiscard]] _rep *_get_rep() const noexcept {
        return reinterpret_cast<_rep *>(_dataplus) - 1;
    }

    size_type _check_range(size_type pos, const char *msg) const {
        if (pos > size()) {
            throw std::out_of_range{msg};
        }
        return pos;
    }

    void _check_length(size_type n1, size_type n2, const char *msg) const {
        if (max_size() - (size() - n1) < n2) {
            throw std::length_error{msg};
        }
    }

    [[nodiscard]] bool _disjunct(const CharT *s) const noexcept {
        return (s < c_str()) || (c_str() + size() < s);
    }

    // for use in begin() & non-const op[]
    void _leak() {
        if (_get_rep()->_is_leaked() || _get_rep() == &_rep::_empty_rep()) {
            return;
        }
        if (_get_rep()->_is_shared()) {
            reserve(capacity());
        }
        _get_rep()->_set_leaked();
    }

    // 清空范围 [ `begin() + pos`, `begin() + pos + len1` ) 中的字符,
    // 并在原位置预留大小为 `len2` 的空间.
    void _mutate(size_type pos, size_type len1, size_type len2) {
        const size_type cap = capacity();
        const size_type old_sz = size();
        const size_type new_sz = old_sz + len2 - len1;
        const size_type how_much = old_sz - pos - len1;
        if (new_sz > cap || _get_rep()->_is_shared()) {
            _rep *r = _rep::_create(new_sz, cap);
            if (pos > 0) {
                traits_type::copy(r->_data, c_str(), pos);
            }
            if (how_much > 0) {
                traits_type::copy((r->_data + pos + len2), (c_str() + pos + len1), how_much);
            }
            _get_rep()->_dispose();
            _dataplus = r->_data;
        } else if ((how_much > 0) && (len1 != len2)) {
            traits_type::move((_dataplus + pos + len2), (c_str() + pos + len1), how_much);
        }
        _get_rep()->_set_size_and_sharable(new_sz);
    }

    basic_string &_replace_aux(size_type pos, size_type n1, size_type n2, CharT c) {
        _check_length(n1, n2, "basic_string::_replace_aux");
        _mutate(pos, n1, n2);
        if (n2 > 0) {
            traits_type::assign(_dataplus + pos, n2, c);
        }
        return *this;
    }

    basic_string &_replace_safe(size_type pos, size_type n1, const CharT *s, size_type n2) {
        _mutate(pos, n1, n2);
        if (n2 > 0) {
            traits_type::copy(_dataplus + pos, s, n2);
        }
        return *this;
    }
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
    basic_string<CharT> r;
    r.reserve(lhs.size() + rhs.size());
    r.append(lhs);
    r.append(rhs);
    return r;
}

template <typename CharT>
[[nodiscard]] basic_string<CharT> operator+(const CharT *lhs, const basic_string<CharT> &rhs) {
    assert((lhs != nullptr) && "operator+(const CharT *, const basic_string &) received nullptr");
    const auto len = basic_string<CharT>::traits_type::length(lhs);
    basic_string<CharT> r;
    r.reserve(len + rhs.size());
    r.append(lhs, len);
    r.append(rhs);
    return r;
}

template <typename CharT>
[[nodiscard]] basic_string<CharT> operator+(CharT lhs, const basic_string<CharT> &rhs) {
    basic_string<CharT> r;
    r.reserve(1 + rhs.size());
    r.push_back(lhs);
    r.append(rhs);
    return r;
}

template <typename CharT>
[[nodiscard]] basic_string<CharT> operator+(const basic_string<CharT> &lhs, const CharT *rhs) {
    assert((rhs != nullptr) && "operator+(const basic_string &, const CharT *) received nullptr");
    const auto len = basic_string<CharT>::traits_type::length(rhs);
    basic_string<CharT> r;
    r.reserve(lhs.size() + len);
    r.append(lhs);
    r.append(rhs, len);
    return r;
}

template <typename CharT>
[[nodiscard]] basic_string<CharT> operator+(const basic_string<CharT> &lhs, CharT rhs) {
    basic_string<CharT> r;
    r.reserve(lhs.size() + 1);
    r.append(lhs);
    r.push_back(rhs);
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
[[nodiscard]] basic_string<CharT> operator+(CharT lhs, basic_string<CharT> &&rhs) {
    return std::move(rhs.insert(0, 1, lhs));
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
using wstring = basic_string<wchar_t>; // `basic_string<int>` in PvZ
#ifndef PVZ_VERSION
using u8string = basic_string<char8_t>;
using u16string = basic_string<char16_t>;
#endif
using u32string = basic_string<char32_t>; // `basic_string<int>` in PvZ

} // namespace pvzstl


namespace std {

template <typename CharT>
struct hash<pvzstl::basic_string<CharT>> {
    [[nodiscard]] size_t operator()(const pvzstl::basic_string<CharT> &val) const noexcept {
        using StringView = basic_string_view<CharT>;
        return hash<StringView>{}(StringView{val});
    }
};

} // namespace std

#endif // PVZ_STL_PVZSTL_STRING_H
