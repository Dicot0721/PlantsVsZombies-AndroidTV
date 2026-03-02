#ifndef PVZ_STL_PVZSTL_VECTOR_H
#define PVZ_STL_PVZSTL_VECTOR_H

#include <algorithm>
#include <atomic>
#include <cassert>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace pvzstl {
template <typename _Iterator>
inline typename std::iterator_traits<_Iterator>::iterator_category __iterator_category(_Iterator) {
    return typename std::iterator_traits<_Iterator>::iterator_category();
}

template <typename _ForwardIterator, typename _Alloc>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last, const _Alloc &__alloc) noexcept {
    for (; __first != __last; ++__first)
        std::allocator_traits<_Alloc>::destroy(__alloc, std::addressof(*__first));
}

// 简化分配器特质
template <typename _Alloc>
struct __alloc_traits {
    template <typename _Tp>
    struct rebind {
        typedef typename _Alloc::template rebind<_Tp>::other other;
    };

    template <typename _Tp>
    static _Tp *allocate(_Alloc &__a, size_t __n) {
        return __a.allocate(__n);
    }

    template <typename _Tp>
    static void deallocate(_Alloc &__a, _Tp *__p, size_t __n) {
        __a.deallocate(__p, __n);
    }
};

// 针对 std::allocator 的特化
template <typename _Tp>
struct __alloc_traits<std::allocator<_Tp>> {
    template <typename _Up>
    struct rebind {
        typedef std::allocator<_Up> other;
    };

    template <typename _Up>
    static _Up *allocate(std::allocator<_Up> &__a, size_t __n) {
        return __a.allocate(__n);
    }

    template <typename _Up>
    static void deallocate(std::allocator<_Up> &__a, _Up *__p, size_t __n) {
        __a.deallocate(__p, __n);
    }
};

template <typename _Tp, typename _Alloc = std::allocator<_Tp>>
struct _Vector_base {
    // 重新绑定分配器类型
    typedef typename __alloc_traits<_Alloc>::template rebind<_Tp>::other _Tp_alloc_type;
    typedef typename std::allocator_traits<_Tp_alloc_type>::pointer pointer;

    // 必须与旧版 STL 内存布局完全一致
    struct _Vector_impl : public _Tp_alloc_type {
        pointer _M_start;
        pointer _M_finish;
        pointer _M_end_of_storage;

        _Vector_impl() noexcept
            : _Tp_alloc_type()
            , _M_start(0)
            , _M_finish(0)
            , _M_end_of_storage(0) {}

        _Vector_impl(const _Tp_alloc_type &__a) noexcept
            : _Tp_alloc_type(__a)
            , _M_start(0)
            , _M_finish(0)
            , _M_end_of_storage(0) {}

        _Vector_impl(_Vector_impl &&__x) noexcept
            : _Tp_alloc_type(std::move(__x))
            , _M_start(__x._M_start)
            , _M_finish(__x._M_finish)
            , _M_end_of_storage(__x._M_end_of_storage) {
            __x._M_start = __x._M_finish = __x._M_end_of_storage = 0;
        }

        void swap(_Vector_impl &__x) noexcept {
            std::swap(_M_start, __x._M_start);
            std::swap(_M_finish, __x._M_finish);
            std::swap(_M_end_of_storage, __x._M_end_of_storage);
        }
    };

    _Vector_impl _M_impl;

    _Vector_base()
        : _M_impl() {}

    _Vector_base(const _Tp_alloc_type &__a)
        : _M_impl(__a) {}

    _Vector_base(size_t __n)
        : _M_impl() {
        _M_create_storage(__n);
    }

    _Vector_base(size_t __n, const _Tp_alloc_type &__a)
        : _M_impl(__a) {
        _M_create_storage(__n);
    }

    ~_Vector_base() {
        if (_M_impl._M_start) {
            _M_deallocate(_M_impl._M_start, _M_impl._M_end_of_storage - _M_impl._M_start);
        }
    }

    _Vector_base(_Vector_base &&__x) noexcept
        : _M_impl(std::move(__x._M_impl)) {}

    _Vector_base &operator=(_Vector_base &&__x) noexcept {
        if (this != &__x) {
            this->~_Vector_base();
            _M_impl = std::move(__x._M_impl);
        }
        return *this;
    }

protected:
    pointer _M_allocate(size_t __n) {
        typedef __alloc_traits<_Tp_alloc_type> _Tr;
        return __n != 0 ? _Tr::allocate(_M_impl, __n) : 0;
    }

    void _M_deallocate(pointer __p, size_t __n) {
        if (__p) {
            typedef __alloc_traits<_Tp_alloc_type> _Tr;
            _Tr::deallocate(_M_impl, __p, __n);
        }
    }

    void _M_create_storage(size_t __n) {
        _M_impl._M_start = _M_allocate(__n);
        _M_impl._M_finish = _M_impl._M_start;
        _M_impl._M_end_of_storage = _M_impl._M_start + __n;
    }
};

// 主 vector 类模板
template <typename _Tp, typename _Alloc = std::allocator<_Tp>>
class vector : protected _Vector_base<_Tp, _Alloc> {
    typedef _Vector_base<_Tp, _Alloc> _Base;
    typedef typename _Base::_Tp_alloc_type _Tp_alloc_type;
    typedef std::allocator_traits<_Tp_alloc_type> _Alloc_traits;

public:
    // 类型定义，必须与标准库一致
    typedef _Tp value_type;
    typedef _Alloc allocator_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef typename _Alloc_traits::pointer pointer;
    typedef typename _Alloc_traits::const_pointer const_pointer;

    // 迭代器
    typedef pointer iterator;
    typedef const_pointer const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    using _Base::_M_impl;

    // 构造函数
    vector() noexcept(noexcept(_Alloc()))
        : _Base() {}

    explicit vector(const _Alloc &__a) noexcept
        : _Base(__a) {}

    explicit vector(size_type __n, const _Alloc &__a = _Alloc())
        : _Base(__n, __a) {
        _M_default_initialize(__n);
    }

    vector(size_type __n, const _Tp &__value, const _Alloc &__a = _Alloc())
        : _Base(__n, __a) {
        _M_fill_initialize(__n, __value);
    }

    template <typename _InputIterator, typename = typename std::enable_if<!std::is_integral<_InputIterator>::value>::type>
    vector(_InputIterator __first, _InputIterator __last, const _Alloc &__a = _Alloc())
        : _Base(__a) {
        _M_range_initialize(__first, __last, __iterator_category(__first));
    }

    vector(const vector &__x)
        : _Base(__x.size(), _Alloc_traits::select_on_container_copy_construction(__x._M_get_Tp_allocator())) {
        _M_range_initialize(__x.begin(), __x.end(), std::random_access_iterator_tag());
    }

    vector(vector &&__x) noexcept
        : _Base(std::move(__x)) {}

    vector(std::initializer_list<_Tp> __l, const _Alloc &__a = _Alloc())
        : _Base(__a) {
        _M_range_initialize(__l.begin(), __l.end(), std::random_access_iterator_tag());
    }

    ~vector() {
        _M_clear();
    }

    vector &operator=(const vector &__x) {
        if (this != &__x) {
            const size_type __xlen = __x.size();
            if (__xlen > capacity()) {
                pointer __tmp = _M_allocate_and_copy(__xlen, __x.begin(), __x.end());
                _M_clear();
                _M_impl._M_start = __tmp;
                _M_impl._M_end_of_storage = _M_impl._M_start + __xlen;
            } else if (size() >= __xlen) {
                std::copy(__x.begin(), __x.end(), begin());
                _M_erase_at_end(_M_impl._M_start + __xlen);
            } else {
                std::copy(__x.begin(), __x.begin() + size(), _M_impl._M_start);
                _M_range_initialize(__x.begin() + size(), __x.end(), std::random_access_iterator_tag());
            }
            _M_impl._M_finish = _M_impl._M_start + __xlen;
        }
        return *this;
    }

    vector &operator=(vector &&__x) noexcept {
        if (this != &__x) {
            clear();
            _M_impl.swap(__x._M_impl);
        }
        return *this;
    }

    vector &operator=(std::initializer_list<_Tp> __l) {
        assign(__l.begin(), __l.end());
        return *this;
    }

    // 赋值
    void assign(size_type __n, const _Tp &__val) {
        _M_fill_assign(__n, __val);
    }

    template <typename _InputIterator, typename = typename std::enable_if<!std::is_integral<_InputIterator>::value>::type>
    void assign(_InputIterator __first, _InputIterator __last) {
        _M_assign_dispatch(__first, __last, __iterator_category(__first));
    }

    void assign(std::initializer_list<_Tp> __l) {
        assign(__l.begin(), __l.end());
    }

    // 迭代器
    iterator begin() noexcept {
        return iterator(_M_impl._M_start);
    }
    const_iterator begin() const noexcept {
        return const_iterator(_M_impl._M_start);
    }
    iterator end() noexcept {
        return iterator(_M_impl._M_finish);
    }
    const_iterator end() const noexcept {
        return const_iterator(_M_impl._M_finish);
    }
    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }
    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }
    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }
    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    // 容量
    size_type size() const noexcept {
        return size_type(_M_impl._M_finish - _M_impl._M_start);
    }
    size_type max_size() const noexcept {
        return _Alloc_traits::max_size(_M_get_Tp_allocator());
    }
    void resize(size_type __new_size) {
        resize(__new_size, _Tp());
    }

    void resize(size_type __new_size, const _Tp &__x) {
        if (__new_size > size()) {
            _M_default_append(__new_size - size());
        } else if (__new_size < size()) {
            _M_erase_at_end(_M_impl._M_start + __new_size);
        }
    }

    size_type capacity() const noexcept {
        return size_type(_M_impl._M_end_of_storage - _M_impl._M_start);
    }
    bool empty() const noexcept {
        return _M_impl._M_start == _M_impl._M_finish;
    }

    void reserve(size_type __n) {
        if (__n > max_size()) {
            throw std::length_error("vector::reserve");
        }
        if (capacity() < __n) {
            _M_reallocate(__n);
        }
    }

    void shrink_to_fit() {
        if (capacity() != size()) {
            _M_shrink_to_fit();
        }
    }

    // 元素访问
    reference operator[](size_type __n) noexcept {
        return _M_impl._M_start[__n];
    }

    const_reference operator[](size_type __n) const noexcept {
        return _M_impl._M_start[__n];
    }

    reference at(size_type __n) {
        if (__n >= size()) {
            throw std::out_of_range("vector::at");
        }
        return _M_impl._M_start[__n];
    }

    const_reference at(size_type __n) const {
        if (__n >= size()) {
            throw std::out_of_range("vector::at");
        }
        return _M_impl._M_start[__n];
    }

    reference front() noexcept {
        return *_M_impl._M_start;
    }
    const_reference front() const noexcept {
        return *_M_impl._M_start;
    }
    reference back() noexcept {
        return *(_M_impl._M_finish - 1);
    }
    const_reference back() const noexcept {
        return *(_M_impl._M_finish - 1);
    }

    pointer data() noexcept {
        return _M_impl._M_start;
    }
    const_pointer data() const noexcept {
        return _M_impl._M_start;
    }

    // 修改器
    template <typename... _Args>
    void emplace_back(_Args &&...__args) {
        if (_M_impl._M_finish != _M_impl._M_end_of_storage) {
            _Alloc_traits::construct(_M_impl, _M_impl._M_finish, std::forward<_Args>(__args)...);
            ++_M_impl._M_finish;
        } else {
            _M_emplace_back_aux(std::forward<_Args>(__args)...);
        }
    }

    void push_back(const _Tp &__x) {
        if (_M_impl._M_finish != _M_impl._M_end_of_storage) {
            _Alloc_traits::construct(_M_impl, _M_impl._M_finish, __x);
            ++_M_impl._M_finish;
        } else {
            _M_push_back_slow_case(__x);
        }
    }

    void push_back(_Tp &&__x) {
        emplace_back(std::move(__x));
    }

    void pop_back() noexcept {
        --_M_impl._M_finish;
        _Alloc_traits::destroy(_M_impl, _M_impl._M_finish);
    }

    template <typename... _Args>
    iterator emplace(const_iterator __position, _Args &&...__args) {
        return _M_emplace_aux(__position, std::forward<_Args>(__args)...);
    }

    iterator insert(const_iterator __position, const _Tp &__x) {
        return _M_insert_aux(__position, __x);
    }

    iterator insert(const_iterator __position, _Tp &&__x) {
        return _M_insert_aux(__position, std::move(__x));
    }

    iterator insert(const_iterator __position, size_type __n, const _Tp &__x) {
        return _M_fill_insert(__position, __n, __x);
    }

    template <typename _InputIterator, typename = typename std::enable_if<!std::is_integral<_InputIterator>::value>::type>
    iterator insert(const_iterator __position, _InputIterator __first, _InputIterator __last) {
        return _M_range_insert(__position, __first, __last, __iterator_category(__first));
    }

    iterator insert(const_iterator __position, std::initializer_list<_Tp> __l) {
        return _M_range_insert(__position, __l.begin(), __l.end(), std::random_access_iterator_tag());
    }

    iterator erase(const_iterator __position) {
        return _M_erase(__position);
    }

    iterator erase(const_iterator __first, const_iterator __last) {
        return _M_erase(__first, __last);
    }

    void clear() noexcept {
        _M_erase_at_end(_M_impl._M_start);
    }

    void swap(vector &__x) noexcept {
        _M_impl.swap(__x._M_impl);
    }

    allocator_type get_allocator() const noexcept {
        return allocator_type(_M_get_Tp_allocator());
    }

private:
    // 内部辅助函数
    _Tp_alloc_type &_M_get_Tp_allocator() noexcept {
        return _M_impl;
    }
    const _Tp_alloc_type &_M_get_Tp_allocator() const noexcept {
        return _M_impl;
    }

    void _M_default_initialize(size_type __n) {
        pointer __cur = _M_impl._M_start;
        for (; __n > 0; --__n, ++__cur) {
            _Alloc_traits::construct(_M_impl, __cur);
        }
        _M_impl._M_finish = __cur;
    }

    void _M_fill_initialize(size_type __n, const _Tp &__value) {
        pointer __cur = _M_impl._M_start;
        for (; __n > 0; --__n, ++__cur) {
            _Alloc_traits::construct(_M_impl, __cur, __value);
        }
        _M_impl._M_finish = __cur;
    }

    template <typename _ForwardIterator>
    void _M_range_initialize(_ForwardIterator __first, _ForwardIterator __last, std::forward_iterator_tag) {
        const size_type __n = std::distance(__first, __last);
        _M_create_storage(__n);
        _M_impl._M_finish = std::uninitialized_copy(__first, __last, _M_impl._M_start);
    }

    template <typename _InputIterator>
    void _M_range_initialize(_InputIterator __first, _InputIterator __last, std::input_iterator_tag) {
        for (; __first != __last; ++__first) {
            push_back(*__first);
        }
    }

    void _M_create_storage(size_type __n) {
        _M_impl._M_start = _M_allocate(__n);
        _M_impl._M_finish = _M_impl._M_start;
        _M_impl._M_end_of_storage = _M_impl._M_start + __n;
    }

    void _M_clear() noexcept {
        if (_M_impl._M_start) {
            _M_erase_at_end(_M_impl._M_start);
        }
    }

    void _M_erase_at_end(pointer __pos) noexcept {
        pointer __old_finish = _M_impl._M_finish;
        while (__old_finish != __pos) {
            _Alloc_traits::destroy(_M_impl, --__old_finish);
        }
        _M_impl._M_finish = __pos;
    }

    void _M_default_append(size_type __n) {
        if (__n != 0) {
            const size_type __size = size();
            if (size_type(_M_impl._M_end_of_storage - _M_impl._M_finish) >= __n) {
                pointer __old_finish = _M_impl._M_finish;
                for (; __n > 0; --__n, ++__old_finish) {
                    _Alloc_traits::construct(_M_impl, __old_finish);
                }
                _M_impl._M_finish = __old_finish;
            } else {
                const size_type __len = _M_check_len(__n, "vector::_M_default_append");
                pointer __new_start = _M_allocate(__len);
                pointer __new_finish = __new_start;
                try {
                    __new_finish = std::uninitialized_copy(_M_impl._M_start, _M_impl._M_finish, __new_start);
                    for (; __n > 0; --__n, ++__new_finish) {
                        _Alloc_traits::construct(_M_impl, __new_finish);
                    }
                } catch (...) {
                    _Destroy(__new_start, __new_finish, _M_get_Tp_allocator());
                    _M_deallocate(__new_start, __len);
                    throw;
                }
                _Destroy(_M_impl._M_start, _M_impl._M_finish, _M_get_Tp_allocator());
                _M_deallocate(_M_impl._M_start, _M_impl._M_end_of_storage - _M_impl._M_start);
                _M_impl._M_start = __new_start;
                _M_impl._M_finish = __new_finish;
                _M_impl._M_end_of_storage = __new_start + __len;
            }
        }
    }

    void _M_fill_assign(size_type __n, const _Tp &__val) {
        if (__n > capacity()) {
            vector __tmp(__n, __val, _M_get_Tp_allocator());
            __tmp.swap(*this);
        } else if (__n > size()) {
            std::fill(begin(), end(), __val);
            _M_impl._M_finish = std::uninitialized_fill_n(_M_impl._M_finish, __n - size(), __val);
        } else {
            std::fill_n(begin(), __n, __val);
            _M_erase_at_end(_M_impl._M_start + __n);
        }
    }

    template <typename _ForwardIterator>
    void _M_assign_aux(_ForwardIterator __first, _ForwardIterator __last, std::forward_iterator_tag) {
        const size_type __len = std::distance(__first, __last);
        if (__len > capacity()) {
            pointer __tmp = _M_allocate_and_copy(__len, __first, __last);
            _Destroy(_M_impl._M_start, _M_impl._M_finish, _M_get_Tp_allocator());
            _M_deallocate(_M_impl._M_start, _M_impl._M_end_of_storage - _M_impl._M_start);
            _M_impl._M_start = __tmp;
            _M_impl._M_end_of_storage = _M_impl._M_start + __len;
        } else if (size() >= __len) {
            iterator __new_finish = std::copy(__first, __last, _M_impl._M_start);
            _M_erase_at_end(__new_finish);
        } else {
            _ForwardIterator __mid = __first;
            std::advance(__mid, size());
            std::copy(__first, __mid, _M_impl._M_start);
            _M_impl._M_finish = std::uninitialized_copy(__mid, __last, _M_impl._M_finish);
        }
    }

    template <typename _InputIterator>
    void _M_assign_aux(_InputIterator __first, _InputIterator __last, std::input_iterator_tag) {
        iterator __cur = begin();
        for (; __first != __last && __cur != end(); ++__cur, ++__first) {
            *__cur = *__first;
        }
        if (__first == __last) {
            _M_erase_at_end(__cur);
        } else {
            insert(end(), __first, __last);
        }
    }

    template <typename _InputIterator>
    void _M_assign_dispatch(_InputIterator __first, _InputIterator __last, std::input_iterator_tag) {
        _M_assign_aux(__first, __last, std::input_iterator_tag());
    }

    template <typename _ForwardIterator>
    void _M_assign_dispatch(_ForwardIterator __first, _ForwardIterator __last, std::forward_iterator_tag) {
        _M_assign_aux(__first, __last, std::forward_iterator_tag());
    }

    template <typename _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last) {
        pointer __result = _M_allocate(__n);
        try {
            std::uninitialized_copy(__first, __last, __result);
            return __result;
        } catch (...) {
            _M_deallocate(__result, __n);
            throw;
        }
    }

    void _M_push_back_slow_case(const _Tp &__x) {
        const size_type __len = _M_check_len(size_type(1), "vector::push_back");
        pointer __new_start = _M_allocate(__len);
        pointer __new_finish = __new_start;
        try {
            __new_finish = std::uninitialized_copy(_M_impl._M_start, _M_impl._M_finish, __new_start);
            _Alloc_traits::construct(_M_impl, __new_finish, __x);
            ++__new_finish;
        } catch (...) {
            _Destroy(__new_start, __new_finish, _M_get_Tp_allocator());
            _M_deallocate(__new_start, __len);
            throw;
        }
        _Destroy(_M_impl._M_start, _M_impl._M_finish, _M_get_Tp_allocator());
        _M_deallocate(_M_impl._M_start, _M_impl._M_end_of_storage - _M_impl._M_start);
        _M_impl._M_start = __new_start;
        _M_impl._M_finish = __new_finish;
        _M_impl._M_end_of_storage = __new_start + __len;
    }

    template <typename... _Args>
    void _M_emplace_back_aux(_Args &&...__args) {
        const size_type __len = _M_check_len(size_type(1), "vector::emplace_back");
        pointer __new_start = _M_allocate(__len);
        pointer __new_finish = __new_start;
        try {
            __new_finish = std::uninitialized_move(_M_impl._M_start, _M_impl._M_finish, __new_start);
            _Alloc_traits::construct(_M_impl, __new_finish, std::forward<_Args>(__args)...);
            ++__new_finish;
        } catch (...) {
            _Destroy(__new_start, __new_finish, _M_get_Tp_allocator());
            _M_deallocate(__new_start, __len);
            throw;
        }
        _Destroy(_M_impl._M_start, _M_impl._M_finish, _M_get_Tp_allocator());
        _M_deallocate(_M_impl._M_start, _M_impl._M_end_of_storage - _M_impl._M_start);
        _M_impl._M_start = __new_start;
        _M_impl._M_finish = __new_finish;
        _M_impl._M_end_of_storage = __new_start + __len;
    }

    size_type _M_check_len(size_type __n, const char *__s) const {
        if (max_size() - size() < __n) {
            throw std::length_error(__s);
        }
        const size_type __len = size() + std::max(size(), __n);
        return (__len < size() || __len > max_size()) ? max_size() : __len;
    }

    void _M_reallocate(size_type __n) {
        pointer __new_start = _M_allocate(__n);
        pointer __new_finish = __new_start;
        try {
            __new_finish = std::uninitialized_move(_M_impl._M_start, _M_impl._M_finish, __new_start);
        } catch (...) {
            _Destroy(__new_start, __new_finish, _M_get_Tp_allocator());
            _M_deallocate(__new_start, __n);
            throw;
        }
        _Destroy(_M_impl._M_start, _M_impl._M_finish, _M_get_Tp_allocator());
        _M_deallocate(_M_impl._M_start, _M_impl._M_end_of_storage - _M_impl._M_start);
        _M_impl._M_start = __new_start;
        _M_impl._M_finish = __new_finish;
        _M_impl._M_end_of_storage = __new_start + __n;
    }

    void _M_shrink_to_fit() {
        if (capacity() > size()) {
            vector __tmp(*this);
            swap(__tmp);
        }
    }

    // 插入/删除辅助函数
    iterator _M_erase(const_iterator __position) {
        iterator __pos = const_cast<iterator>(__position);
        std::move(__pos + 1, end(), __pos);
        _Alloc_traits::destroy(_M_impl, _M_impl._M_finish - 1);
        --_M_impl._M_finish;
        return __pos;
    }

    iterator _M_erase(const_iterator __first, const_iterator __last) {
        if (__first != __last) {
            iterator __f = const_cast<iterator>(__first);
            iterator __l = const_cast<iterator>(__last);
            iterator __next = std::move(__l, end(), __f);
            _M_erase_at_end(__next);
        }
        return const_cast<iterator>(__first);
    }

    template <typename... _Args>
    iterator _M_emplace_aux(const_iterator __position, _Args &&...__args) {
        iterator __pos = const_cast<iterator>(__position);
        if (_M_impl._M_finish != _M_impl._M_end_of_storage) {
            if (__pos == end()) {
                _Alloc_traits::construct(_M_impl, _M_impl._M_finish, std::forward<_Args>(__args)...);
                ++_M_impl._M_finish;
            } else {
                _Alloc_traits::construct(_M_impl, _M_impl._M_finish, std::move(back()));
                ++_M_impl._M_finish;
                std::move_backward(__pos, end() - 2, end() - 1);
                *__pos = _Tp(std::forward<_Args>(__args)...);
            }
        } else {
            const size_type __len = _M_check_len(size_type(1), "vector::_M_emplace_aux");
            pointer __new_start = _M_allocate(__len);
            pointer __new_finish = __new_start;
            try {
                __new_finish = std::uninitialized_move(_M_impl._M_start, __pos, __new_start);
                _Alloc_traits::construct(_M_impl, __new_finish, std::forward<_Args>(__args)...);
                ++__new_finish;
                __new_finish = std::uninitialized_move(__pos, _M_impl._M_finish, __new_finish);
            } catch (...) {
                _Destroy(__new_start, __new_finish, _M_get_Tp_allocator());
                _M_deallocate(__new_start, __len);
                throw;
            }
            _Destroy(_M_impl._M_start, _M_impl._M_finish, _M_get_Tp_allocator());
            _M_deallocate(_M_impl._M_start, _M_impl._M_end_of_storage - _M_impl._M_start);
            _M_impl._M_start = __new_start;
            _M_impl._M_finish = __new_finish;
            _M_impl._M_end_of_storage = __new_start + __len;
            __pos = __new_start + (__position - begin());
        }
        return __pos;
    }

    iterator _M_insert_aux(const_iterator __position, const _Tp &__x) {
        return _M_emplace_aux(__position, __x);
    }

    iterator _M_insert_aux(const_iterator __position, _Tp &&__x) {
        return _M_emplace_aux(__position, std::move(__x));
    }

    iterator _M_fill_insert(const_iterator __position, size_type __n, const _Tp &__x) {
        iterator __pos = const_cast<iterator>(__position);
        if (__n == 0)
            return __pos;

        if (size_type(_M_impl._M_end_of_storage - _M_impl._M_finish) >= __n) {
            const size_type __elems_after = end() - __pos;
            pointer __old_finish = _M_impl._M_finish;
            if (__elems_after > __n) {
                std::uninitialized_move(_M_impl._M_finish - __n, _M_impl._M_finish, _M_impl._M_finish);
                _M_impl._M_finish += __n;
                std::move_backward(__pos, __old_finish - __n, __old_finish);
                std::fill(__pos, __pos + __n, __x);
            } else {
                _M_impl._M_finish = std::uninitialized_fill_n(_M_impl._M_finish, __n - __elems_after, __x);
                _M_impl._M_finish = std::uninitialized_move(__pos, __old_finish, _M_impl._M_finish);
                std::fill(__pos, __old_finish, __x);
            }
        } else {
            const size_type __len = _M_check_len(__n, "vector::_M_fill_insert");
            pointer __new_start = _M_allocate(__len);
            pointer __new_finish = __new_start;
            try {
                __new_finish = std::uninitialized_move(_M_impl._M_start, __pos, __new_start);
                __new_finish = std::uninitialized_fill_n(__new_finish, __n, __x);
                __new_finish = std::uninitialized_move(__pos, _M_impl._M_finish, __new_finish);
            } catch (...) {
                _Destroy(__new_start, __new_finish, _M_get_Tp_allocator());
                _M_deallocate(__new_start, __len);
                throw;
            }
            _Destroy(_M_impl._M_start, _M_impl._M_finish, _M_get_Tp_allocator());
            _M_deallocate(_M_impl._M_start, _M_impl._M_end_of_storage - _M_impl._M_start);
            _M_impl._M_start = __new_start;
            _M_impl._M_finish = __new_finish;
            _M_impl._M_end_of_storage = __new_start + __len;
        }
        return __pos;
    }

    template <typename _ForwardIterator>
    iterator _M_range_insert(const_iterator __position, _ForwardIterator __first, _ForwardIterator __last, std::forward_iterator_tag) {
        iterator __pos = const_cast<iterator>(__position);
        const size_type __n = std::distance(__first, __last);
        if (__n == 0)
            return __pos;

        if (size_type(_M_impl._M_end_of_storage - _M_impl._M_finish) >= __n) {
            const size_type __elems_after = end() - __pos;
            pointer __old_finish = _M_impl._M_finish;
            if (__elems_after > __n) {
                std::uninitialized_move(_M_impl._M_finish - __n, _M_impl._M_finish, _M_impl._M_finish);
                _M_impl._M_finish += __n;
                std::move_backward(__pos, __old_finish - __n, __old_finish);
                std::copy(__first, __last, __pos);
            } else {
                _ForwardIterator __mid = __first;
                std::advance(__mid, __elems_after);
                _M_impl._M_finish = std::uninitialized_copy(__mid, __last, _M_impl._M_finish);
                _M_impl._M_finish = std::uninitialized_move(__pos, __old_finish, _M_impl._M_finish);
                std::copy(__first, __mid, __pos);
            }
        } else {
            const size_type __len = _M_check_len(__n, "vector::_M_range_insert");
            pointer __new_start = _M_allocate(__len);
            pointer __new_finish = __new_start;
            try {
                __new_finish = std::uninitialized_move(_M_impl._M_start, __pos, __new_start);
                __new_finish = std::uninitialized_copy(__first, __last, __new_finish);
                __new_finish = std::uninitialized_move(__pos, _M_impl._M_finish, __new_finish);
            } catch (...) {
                _Destroy(__new_start, __new_finish, _M_get_Tp_allocator());
                _M_deallocate(__new_start, __len);
                throw;
            }
            _Destroy(_M_impl._M_start, _M_impl._M_finish, _M_get_Tp_allocator());
            _M_deallocate(_M_impl._M_start, _M_impl._M_end_of_storage - _M_impl._M_start);
            _M_impl._M_start = __new_start;
            _M_impl._M_finish = __new_finish;
            _M_impl._M_end_of_storage = __new_start + __len;
        }
        return __pos;
    }

    template <typename _InputIterator>
    iterator _M_range_insert(const_iterator __position, _InputIterator __first, _InputIterator __last, std::input_iterator_tag) {
        iterator __pos = const_cast<iterator>(__position);
        for (; __first != __last; ++__first) {
            __pos = insert(__pos, *__first);
            ++__pos;
        }
        return __pos;
    }

protected:
    using _Base::_M_allocate;
    using _Base::_M_deallocate;

// 使用 _GLIBCXX_NOEXCEPT 保持兼容性
#ifndef _GLIBCXX_NOEXCEPT
#define _GLIBCXX_NOEXCEPT noexcept
#endif
};

// 非成员函数
template <typename _Tp, typename _Alloc>
inline bool operator==(const vector<_Tp, _Alloc> &__x, const vector<_Tp, _Alloc> &__y) {
    return (__x.size() == __y.size() && std::equal(__x.begin(), __x.end(), __y.begin()));
}

template <typename _Tp, typename _Alloc>
inline bool operator!=(const vector<_Tp, _Alloc> &__x, const vector<_Tp, _Alloc> &__y) {
    return !(__x == __y);
}

template <typename _Tp, typename _Alloc>
inline bool operator<(const vector<_Tp, _Alloc> &__x, const vector<_Tp, _Alloc> &__y) {
    return std::lexicographical_compare(__x.begin(), __x.end(), __y.begin(), __y.end());
}

template <typename _Tp, typename _Alloc>
inline bool operator<=(const vector<_Tp, _Alloc> &__x, const vector<_Tp, _Alloc> &__y) {
    return !(__y < __x);
}

template <typename _Tp, typename _Alloc>
inline bool operator>(const vector<_Tp, _Alloc> &__x, const vector<_Tp, _Alloc> &__y) {
    return __y < __x;
}

template <typename _Tp, typename _Alloc>
inline bool operator>=(const vector<_Tp, _Alloc> &__x, const vector<_Tp, _Alloc> &__y) {
    return !(__x < __y);
}

template <typename _Tp, typename _Alloc>
inline void swap(vector<_Tp, _Alloc> &__x, vector<_Tp, _Alloc> &__y) _GLIBCXX_NOEXCEPT {
    __x.swap(__y);
}

} // namespace pvzstl

#endif // PVZ_STL_PVZSTL_VECTOR_H
