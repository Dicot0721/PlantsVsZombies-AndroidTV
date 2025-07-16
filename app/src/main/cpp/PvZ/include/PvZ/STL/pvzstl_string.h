#ifndef PVZ_STL_PVZSTL_STRING_H
#define PVZ_STL_PVZSTL_STRING_H

#ifdef PVZ_VERSION
#include "PvZ/Symbols.h" // gLibBaseOffset
#endif

#include "pvzstl_concepts.h"

#include <cassert>

#include <atomic>
#include <stdexcept>
#include <string>

namespace pvzstl {

template <character CharT>
class basic_string;

template <character CharT>
[[nodiscard]] basic_string<CharT> operator+(const basic_string<CharT> &lhs, const basic_string<CharT> &rhs);

template <character CharT>
[[nodiscard]] basic_string<CharT> operator+(const CharT *lhs, const basic_string<CharT> &rhs);

template <character CharT>
[[nodiscard]] basic_string<CharT> operator+(const basic_string<CharT> &lhs, const CharT *rhs);

/**
 * @class 采用写时复制 (COW) 实现的字符串类模板
 *
 * clang 在 C++11 时的 std::string 实现 (简化版).<br />
 * 可参考文章: <a href="https://chengxumiaodaren.com/docs/open-source/cpp-string/">C++ string源码剖析：深入理解COW机制</a>.
 */
template <character CharT>
class basic_string {
public:
    using traits_type = std::char_traits<CharT>;
    using value_type = CharT;
    using size_type = std::uint32_t;

    using __self_view = std::basic_string_view<value_type>;

    static constexpr size_type npos = -1;

    basic_string() = default;

    basic_string(const basic_string &other)
        : _data{other._data} {
        if (Rep *p = get_rep(); p != &Rep::empty_rep()) {
            ++p->ref_count;
        }
    }

    basic_string(basic_string &&other) noexcept
        : _data{other._data} {
        other._data = Rep::empty_rep().data;
    }

    basic_string(const value_type *s) {
        assert((s != nullptr) && "basic_string(const CharT *) detected nullptr");
        size_type len = traits_type::length(s);
        _data = Rep::create(s, len, len)->data;
    }

    basic_string(std::nullptr_t) = delete;

    basic_string(const value_type *s, size_type n) {
        assert((s != nullptr || n == 0) && "basic_string(const CharT*, n) detected nullptr");
        _data = Rep::create(s, n, n)->data;
    }

    basic_string(const basic_string &str, size_type pos, size_type n) {
        size_type str_sz = str.size();
        if (pos > str_sz) {
            __throw_out_of_range();
        }
        size_type len = std::min(n, str_sz - pos);
        _data = Rep::create(str._data + pos, len, len)->data;
    }

    basic_string(const basic_string &str, size_type pos) {
        size_type str_sz = str.size();
        if (pos > str_sz) {
            __throw_out_of_range();
        }
        size_type len = str_sz - pos;
        _data = Rep::create(str._data + pos, len, len)->data;
    }

    ~basic_string() { reset(); }

    /* implicit */ operator __self_view() const noexcept { return __self_view{_data, size()}; }

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

    [[nodiscard]] size_type size() const noexcept { return get_rep()->size; }

    [[nodiscard]] size_type capacity() const noexcept { return get_rep()->capacity; }

    [[nodiscard]] bool empty() const noexcept { return size() == 0; }

    [[nodiscard]] const value_type &operator[](size_type pos) const noexcept {
        assert((pos <= size()) && "string index out of bounds");
        return *(_data + pos);
    }

    basic_string &operator+=(const basic_string &str) { return append(str); }

    basic_string &operator+=(const value_type *s) { return append(s); }

    basic_string &append(const basic_string &str) { return append(str._data, str.size()); }

    basic_string &append(const value_type *s, size_type n);

    basic_string &append(const value_type *s) {
        assert((s != nullptr) && "basic_string::append received nullptr");
        return append(s, traits_type::length(s));
    }

    [[nodiscard]] const value_type &front() const noexcept {
        assert(!empty() && "basic_string::front(): string is empty");
        return *_data;
    }

    [[nodiscard]] const value_type &back() const noexcept {
        assert(!empty() && "basic_string::back(): string is empty");
        return *(_data + size() - 1);
    }

    [[nodiscard]] basic_string substr(size_type pos = 0, size_type n = npos) const { return basic_string{*this, pos, n}; }

    [[nodiscard]] const value_type *c_str() const noexcept { return _data; }

    [[nodiscard]] bool starts_with(__self_view sv) const noexcept { return __self_view{_data, size()}.starts_with(sv); }

    [[nodiscard]] bool starts_with(value_type c) const noexcept { return !empty() && (front() == c); }

    [[nodiscard]] bool starts_with(const value_type *s) const noexcept { return starts_with(__self_view{s}); }

    [[nodiscard]] bool ends_with(__self_view sv) const noexcept { return __self_view{_data, size()}.ends_with(sv); }

    [[nodiscard]] bool ends_with(value_type c) const noexcept { return !empty() && (back() == c); }

    [[nodiscard]] bool ends_with(const value_type *s) const noexcept { return ends_with(__self_view{s}); }

    [[nodiscard]] bool contains(__self_view sv) const noexcept { return __self_view{_data, size()}.contains(sv); }

    [[nodiscard]] bool contains(value_type c) const noexcept { return __self_view{_data, size()}.contains(c); }

    [[nodiscard]] bool contains(const value_type *s) const { return __self_view{_data, size()}.contains(s); }

    [[nodiscard, gnu::always_inline]] std::int32_t __ref_count() const noexcept { return get_rep()->ref_count; }

    [[noreturn]] void __throw_out_of_range() const { throw std::out_of_range{"basic_string"}; }

protected:
    friend basic_string operator+ <>(const basic_string &, const basic_string &);
    friend basic_string operator+ <>(const value_type *, const basic_string &);
    friend basic_string operator+ <>(const basic_string &, const value_type *);

    struct Rep {
        static constexpr size_type _MAX_SIZE = 0x3FFF'FFFC;

        size_type size;                // 字符数
        size_type capacity;            // 已分配存储空间中可以容纳的字符数
        std::atomic_int32_t ref_count; // 引用计数 (小于等于 0 时释放内存)
        value_type data[];             // 作为字符存储的底层数组 (柔性数组成员)

        [[nodiscard, gnu::always_inline]] static Rep &empty_rep() noexcept {
#ifdef PVZ_VERSION
            static constexpr uintptr_t offset = (sizeof(value_type) == sizeof(int8_t)) ? /* char */ 0x71BB54 : /* wchar_t */ 0x69E45C;
            return *reinterpret_cast<Rep *>(gLibBaseOffset + offset);
#else
            alignas(Rep) static std::byte empty_rep_storage[sizeof(Rep) + sizeof(value_type)] = {};
            return *reinterpret_cast<Rep *>(empty_rep_storage);
#endif
        }

        [[nodiscard]] static Rep *create(const value_type *s, size_type sz, size_type cap) {
            assert((s != nullptr || sz == 0) && (sz <= cap) && (cap < _MAX_SIZE));
            std::size_t alloc_sz = sizeof(Rep) + sizeof(value_type) * (cap + 1);
            Rep *p = new (::operator new(alloc_sz)) Rep{.size = sz, .capacity = cap, .ref_count = 0};
            if (sz > 0) {
                traits_type::copy(p->data, s, sz);
            }
            p->data[sz] = value_type{0};
            return p;
        }

        static void destroy(Rep *p) noexcept {
            assert(p != nullptr);
            if (p->ref_count-- <= 0) {
                ::operator delete(p);
            }
        }
    };

    [[nodiscard, gnu::always_inline]] Rep *get_rep() const noexcept { return reinterpret_cast<Rep *>(_data) - 1; }

    void reset() noexcept {
        if (Rep *p = get_rep(); p != &Rep::empty_rep()) {
            Rep::destroy(p);
            _data = Rep::empty_rep().data;
        }
    }

    mutable value_type *_data = Rep::empty_rep().data;
};

template <character CharT>
basic_string<CharT> &basic_string<CharT>::operator=(const basic_string &other) {
    if ((this != std::addressof(other)) && (_data != other._data)) {
        reset();
        if ((_data = other._data) != Rep::empty_rep().data) {
            ++get_rep()->ref_count;
        }
    }
    return *this;
}

template <character CharT>
basic_string<CharT> &basic_string<CharT>::operator=(basic_string &&other) noexcept {
    if (this != std::addressof(other)) {
        reset();
        _data = std::exchange(other._data, Rep::empty_rep().data);
    }
    return *this;
}

template <character CharT>
basic_string<CharT> &basic_string<CharT>::operator=(const value_type *s) {
    assert((s != nullptr) && "basic_string::assign received nullptr");
    size_type len = traits_type::length(s);
    if ((_data == Rep::empty_rep().data) && len == 0) {
        return *this;
    }
    size_type cap = capacity();
    if (len > cap || __ref_count() > 0) {
        Rep *p = Rep::create(s, len, std::max(len, cap));
        reset();
        _data = p->data;
    } else {
        traits_type::move(_data, s, len);
        _data[len] = value_type{0};
        get_rep()->size = len;
    }
    return *this;
}

template <character CharT>
basic_string<CharT> &basic_string<CharT>::append(const value_type *s, size_type n) {
    assert((s != nullptr || n == 0) && "basic_string::append received nullptr");
    if (n == 0) {
        return *this;
    }
    size_type old_sz = size();
    size_type new_sz = old_sz + n;
    size_type cap = capacity();
    if (new_sz > cap || __ref_count() > 0) {
        Rep *p = Rep::create(_data, old_sz, std::max(new_sz, cap));
        traits_type::copy(p->data + old_sz, s, n);
        reset();
        _data = p->data;
    } else {
        traits_type::copy(_data + old_sz, s, n);
    }
    _data[new_sz] = value_type{0};
    get_rep()->size = new_sz;
    return *this;
}

template <character CharT>
[[nodiscard]] bool operator==(const basic_string<CharT> &lhs, const basic_string<CharT> &rhs) noexcept {
    return std::basic_string_view<CharT>{lhs} == std::basic_string_view<CharT>{rhs};
}

template <character CharT>
[[nodiscard]] bool operator==(const basic_string<CharT> &lhs, const CharT *rhs) noexcept {
    return std::basic_string_view<CharT>{lhs} == std::basic_string_view<CharT>{rhs};
}

template <character CharT>
[[nodiscard]] auto operator<=>(const basic_string<CharT> &lhs, const basic_string<CharT> &rhs) noexcept {
    return std::basic_string_view<CharT>{lhs} <=> std::basic_string_view<CharT>{rhs};
}

template <character CharT>
[[nodiscard]] auto operator<=>(const basic_string<CharT> &lhs, const CharT *rhs) noexcept {
    return std::basic_string_view<CharT>{lhs} <=> std::basic_string_view<CharT>{rhs};
}

template <character CharT>
[[nodiscard]] basic_string<CharT> operator+(const basic_string<CharT> &lhs, const basic_string<CharT> &rhs) {
    using Traits = std::char_traits<CharT>;
    using String = basic_string<CharT>;
    using SizeType = typename String::size_type;

    SizeType lhs_sz = lhs.size();
    SizeType rhs_sz = rhs.size();
    SizeType sz = lhs_sz + rhs_sz;
    String ret;

    ret._data = String::Rep::create(lhs.c_str(), lhs_sz, sz)->data;
    Traits::copy(ret._data + lhs_sz, rhs.c_str(), rhs_sz);
    ret._data[sz] = CharT{0};
    ret.get_rep()->size = sz;
    return ret;
}

template <character CharT>
[[nodiscard]] basic_string<CharT> operator+(const CharT *lhs, const basic_string<CharT> &rhs) {
    using Traits = std::char_traits<CharT>;
    using String = basic_string<CharT>;
    using SizeType = typename String::size_type;

    SizeType lhs_sz = Traits::length(lhs);
    SizeType rhs_sz = rhs.size();
    SizeType sz = lhs_sz + rhs_sz;
    String ret;

    ret._data = String::Rep::create(lhs, lhs_sz, sz)->data;
    Traits::copy(ret._data + lhs_sz, rhs.c_str(), rhs_sz);
    ret._data[sz] = CharT{0};
    ret.get_rep()->size = sz;
    return ret;
}

template <character CharT>
[[nodiscard]] basic_string<CharT> operator+(const basic_string<CharT> &lhs, const CharT *rhs) {
    using Traits = std::char_traits<CharT>;
    using String = basic_string<CharT>;
    using SizeType = typename String::size_type;

    SizeType lhs_sz = lhs.size();
    SizeType rhs_sz = Traits::length(rhs);
    SizeType sz = lhs_sz + rhs_sz;
    String ret;

    ret._data = String::Rep::create(lhs.c_str(), lhs_sz, sz)->data;
    Traits::copy(ret._data + lhs_sz, rhs, rhs_sz);
    ret._data[sz] = CharT{0};
    ret.get_rep()->size = sz;
    return ret;
}

template <character CharT>
[[nodiscard]] basic_string<CharT> operator+(basic_string<CharT> &&lhs, const basic_string<CharT> &rhs) {
    return std::move(lhs.append(rhs));
}

template <character CharT>
[[nodiscard]] basic_string<CharT> operator+(basic_string<CharT> &&lhs, basic_string<CharT> &&rhs) {
    return std::move(lhs.append(rhs));
}

template <character CharT>
[[nodiscard]] basic_string<CharT> operator+(basic_string<CharT> &&lhs, const CharT *rhs) {
    return std::move(lhs.append(rhs));
}

using string = basic_string<char>;

} // namespace pvzstl

#endif // PVZ_STL_PVZSTL_STRING_H
