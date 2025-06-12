#ifndef PVZ_PVZSTL_STRING_H
#define PVZ_PVZSTL_STRING_H

#include "PvZ/STL/cstdlib.h"

#include <cassert>
#include <cstdint>
#include <cstring>

#include <atomic>

namespace pvzstl {

/**
 * @class 字符串
 *
 * 使用写时复制 (COW) 实现的字符串类.
 */
template <typename CharT>
class basic_string {
public:
    basic_string()
        : basic_string(&CharT{'\0'}) {}

    basic_string(const CharT *s, bool is_from_other = false) {
        assert((s != nullptr) && "construction from null");
        if (is_from_other && (s != nullptr)) {
            _ptr = reinterpret_cast<impl *>(uintptr_t(s) - 12);
            return;
        }

        std::size_t length = (s != nullptr) ? std::strlen(s) : 0;
        _ptr = reinterpret_cast<impl *>(pvzstl::malloc(sizeof(impl) + length + 1)); // +1 is for '\0'
        if (_ptr == nullptr) {
            return;
        }
        _ptr->_size = length;
        _ptr->_capacity = length + 1;
        _ptr->_ref_count = 0;
        if (length > 0) {
            std::memcpy(_ptr->_data, s, length);
        }
        _ptr->_data[length] = CharT{'\0'};
    };

    basic_string(const basic_string &other)
        : _ptr(other._ptr) {
        ++_ptr->_ref_count;
    }

    basic_string(basic_string &&other) noexcept
        : _ptr(other._ptr) {
        other._ptr = nullptr;
    }

    basic_string &operator=(const basic_string &other) {
        if (this != &other) {
            reset();
            _ptr = other._ptr;
            ++_ptr->_ref_count;
        }
        return *this;
    }

    basic_string &operator=(basic_string &&other) noexcept {
        if (this != &other) {
            reset();
            _ptr = std::exchange(other._ptr, nullptr);
        }
        return *this;
    }

    ~basic_string() noexcept { reset(); }

    [[nodiscard]] const CharT *data() const noexcept { return _ptr != nullptr ? _ptr->_data : ""; }
    [[nodiscard]] CharT *data() noexcept { return static_cast<char *>(_ptr != nullptr ? _ptr->_data : ""); }

    [[nodiscard]] const CharT *c_str() const noexcept { return _ptr != nullptr ? _ptr->_data : ""; }

    [[nodiscard]] bool empty() const noexcept { return _ptr == nullptr || _ptr->_size == 0; }

    [[nodiscard]] std::size_t size() const noexcept { return _ptr != nullptr ? _ptr->_size : 0; }

    [[nodiscard]] std::size_t capacity() const noexcept { return _ptr != nullptr ? _ptr->_capacity : 0; }

protected:
    struct impl {
        uint32_t _size;                 // 字符串长度
        uint32_t _capacity;             // 字符串可用空间大小
        std::atomic_int32_t _ref_count; // 引用计数 (为 0 时销毁对象)
        CharT _data[];                  // 字符数组 (弹性数组)
    };

    void reset() noexcept {
        if (_ptr == nullptr) {
            return;
        }
        if (_ptr->_ref_count > 0) {
            --_ptr->_ref_count;
            return;
        }
        pvzstl::free(_ptr);
        _ptr = nullptr;
    }

    impl *_ptr = nullptr;
};

using string = basic_string<char>;

} // namespace pvzstl

#endif // PVZ_PVZSTL_STRING_H
