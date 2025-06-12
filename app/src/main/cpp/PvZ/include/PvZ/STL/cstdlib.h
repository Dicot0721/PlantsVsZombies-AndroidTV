#ifndef PVZ_PVZSTL_MEMORY_H
#define PVZ_PVZSTL_MEMORY_H

#include "PvZ/Symbols.h"

#include <cstddef>

namespace pvzstl {

inline void *malloc(std::size_t size) {
    return reinterpret_cast<void *(*)(std::size_t)>(mallocAddr)(size);
}

inline void free(void *ptr) {
    reinterpret_cast<void (*)(void *)>(freeAddr)(ptr);
}

} // namespace pvzstl

#endif // PVZ_PVZSTL_MEMORY_H
