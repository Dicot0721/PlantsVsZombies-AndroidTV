#ifndef PVZ_STL_PVZSTL_CONCEPTS_H
#define PVZ_STL_PVZSTL_CONCEPTS_H

#include <type_traits>

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

} // namespace pvzstl

#endif // PVZ_STL_PVZSTL_CONCEPTS_H
