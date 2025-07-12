#ifndef PVZ_PVZSTL_STRING_H
#define PVZ_PVZSTL_STRING_H

#include <cassert>

#include <atomic>
#include <string>

namespace pvzstl {

// clang-format off
template <typename T>
concept character = std::disjunction_v<
    std::is_same<T, char>,
    std::is_same<T, wchar_t>,
    std::is_same<T, char8_t>,
    std::is_same<T, char16_t>,
    std::is_same<T, char32_t>
>;
// clang-format on

template <character CharT>
class basic_string;

template <character T>
[[nodiscard]] basic_string<T> basic_string_wrapper(const T *raw_s);

/**
 * @class 字符串
 *
 * 使用写时复制 (COW) 实现的字符串类模板.
 */
template <character CharT>
class basic_string {
public:
    using traits_type = std::char_traits<CharT>;

    using value_type = CharT;
    using size_type = std::uint32_t;
    using pointer = value_type *;
    using const_pointer = const value_type *;

    basic_string() {
        static constexpr value_type empty_s[1] = {0};
        init(empty_s, 0);
    }

    basic_string(const value_type *s) {
        assert((s != nullptr) && "basic_string(const value_type *) detected nullptr");
        init(s, traits_type::length(s));
    };

    basic_string(std::nullptr_t) = delete;

    // 暂时不考虑拷贝
    basic_string(const basic_string &other) = delete;
    basic_string &operator=(const basic_string &other) = delete;

    basic_string(basic_string &&other) noexcept
        : _ptr(other._ptr) {
        other._ptr = nullptr;
    }

    basic_string &operator=(basic_string &&other) noexcept {
        if (this != std::addressof(other)) {
            release();
            _ptr = std::exchange(other._ptr, nullptr);
        }
        return *this;
    }

    ~basic_string() noexcept { release(); }

    [[nodiscard]] const value_type *data() const noexcept { return _ptr->data; }
    [[nodiscard]] value_type *data() noexcept { return _ptr->data; }

    [[nodiscard]] const value_type *c_str() const noexcept { return data(); }

    [[nodiscard]] bool empty() const noexcept { return _ptr->size == 0; }

    [[nodiscard]] size_type size() const noexcept { return _ptr->size; }

    [[nodiscard]] size_type capacity() const noexcept { return _ptr->capacity; }

    template <character T>
    friend basic_string<T> basic_string_wrapper(const T *raw_s) {
        assert(raw_s != nullptr);
        basic_string<T> str;
        str._ptr = reinterpret_cast<impl *>(uintptr_t(raw_s) - sizeof(impl));
        return str;
    }

protected:
    struct impl {
        size_type size;                // 字符串长度 (已用空间大小)
        size_type capacity;            // 字符串容量 (可用空间大小, 不包括终止符)
        std::atomic_int32_t ref_count; // 引用计数 (小于等于 0 时销毁对象)
        value_type data[];             // 字符数组 (柔性数组成员)
    };

    void init(const value_type *s, size_type sz) {
        std::size_t alloc_size = sizeof(impl) + sizeof(value_type) * (sz + 1); // 暂时不考虑用对齐来提高性能
        _ptr = new (::operator new(alloc_size)) impl{.size = sz, .capacity = sz, .ref_count = 0};
        traits_type::copy(_ptr->data, s, sz);
        _ptr->data[sz] = static_cast<value_type>(0);
    }

    void release() noexcept {
        if (_ptr == nullptr) {
            return;
        }
        if (_ptr->ref_count > 0) {
            --_ptr->ref_count;
        } else {
            ::operator delete(_ptr);
        }
        _ptr = nullptr;
    }

    impl *_ptr = nullptr;
};

// template <character CharT>
// basic_string<CharT>::basic_string(const basic_string &other)
//     : _ptr(other._ptr) {
//     if (_ptr != nullptr) {
//         ++_ptr->ref_count;
//     }
// }

// template <character CharT>
// basic_string<CharT> &basic_string<CharT>::operator=(const basic_string &other) {
//     if (this != std::addressof(other)) {
//         release();
//         _ptr = other._ptr;
//         if (_ptr != nullptr) {
//             ++_ptr->ref_count;
//         }
//     }
//     return *this;
// }

using string = basic_string<char>;

[[nodiscard]] inline string string_wrapper(const char *raw_s) {
    return basic_string_wrapper<char>(raw_s);
}

} // namespace pvzstl

#endif // PVZ_PVZSTL_STRING_H
