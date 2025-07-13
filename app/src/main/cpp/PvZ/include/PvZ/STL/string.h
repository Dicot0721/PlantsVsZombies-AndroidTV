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

/**
 * @class 采用写时复制 (COW) 实现的字符串类模板
 *
 * clang 在 C++11 时的 std::string 实现 (简化版).
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
    }

    /**
     * @brief 构造一个未申请内存的 pvzstl::basic_string 对象.
     *
     * 仅在有函数需要就地构造 pvzstl::basic_string 时使用.
     */
    basic_string(std::nullptr_t) {}

    basic_string(const basic_string &other)
        : _data(other._data) {
        if (_data != nullptr) {
            ++get_base_ptr()->ref_count;
        }
    }

    basic_string(basic_string &&other) noexcept
        : _data(other._data) {
        other._data = nullptr;
    }

    ~basic_string() { release(); }

    basic_string &operator=(const basic_string &other) {
        if (this != std::addressof(other)) {
            release();
            if ((_data = other._data) != nullptr) {
                ++get_base_ptr()->ref_count;
            }
        }
        return *this;
    }

    basic_string &operator=(basic_string &&other) noexcept {
        if (this != std::addressof(other)) {
            release();
            _data = std::exchange(other._data, nullptr);
        }
        return *this;
    }

    [[nodiscard]] size_type size() const noexcept { return get_base_ptr()->size; }

    [[nodiscard]] size_type capacity() const noexcept { return get_base_ptr()->capacity; }

    [[nodiscard]] bool empty() const noexcept { return size() == 0; }

    [[nodiscard]] const value_type *c_str() const noexcept { return _data; }

    [[nodiscard]] bool valid() const noexcept { return _data != nullptr; }

protected:
    struct impl {
        size_type size;                // 字符数
        size_type capacity;            // 已分配存储空间中可以容纳的字符数
        std::atomic_int32_t ref_count; // 引用计数 (小于等于 0 时释放内存)
        value_type data[];             // 作为字符存储的底层数组 (柔性数组成员)
    };

    const impl *get_base_ptr() const noexcept { return reinterpret_cast<impl *>(uintptr_t(_data) - sizeof(impl)); }
    impl *get_base_ptr() noexcept { return reinterpret_cast<impl *>(uintptr_t(_data) - sizeof(impl)); }

    void init(const value_type *s, size_type sz) {
        std::size_t alloc_size = sizeof(impl) + sizeof(value_type) * (sz + 1);
        impl *base_ptr = new (::operator new(alloc_size)) impl{.size = sz, .capacity = sz, .ref_count = 0};
        _data = base_ptr->data;
        traits_type::copy(_data, s, sz);
        _data[sz] = value_type(0);
    }

    void release() noexcept {
        if (_data == nullptr) {
            return;
        }
        impl *base_ptr = get_base_ptr();
        if (base_ptr->ref_count > 0) {
            --base_ptr->ref_count;
        } else {
            ::operator delete(base_ptr);
        }
        _data = nullptr;
    }

    value_type *_data = nullptr;
};

using string = basic_string<char>;

} // namespace pvzstl

#endif // PVZ_PVZSTL_STRING_H
