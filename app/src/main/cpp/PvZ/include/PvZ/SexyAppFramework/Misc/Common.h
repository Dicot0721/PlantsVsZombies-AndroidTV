#ifndef PVZ_SEXYAPPFRAMEWORK_MISC_COMMON_H
#define PVZ_SEXYAPPFRAMEWORK_MISC_COMMON_H

#include "PvZ/STL/pvzstl_string.h"
#include "PvZ/Symbols.h"

#include <algorithm>
#include <cstdlib>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

using uchar = unsigned char;
using ushort = unsigned short;
using uint = unsigned int;
using ulong = unsigned long;

enum PixelFormat {
    kPixelFormat_None = -1,
    kPixelFormat_Automatic,
    kPixelFormat_RGBA8888,
    kPixelFormat_RGBA4444,
    kPixelFormat_RGBA5551,
    kPixelFormat_RGB565,
    kPixelFormat_RGB888,
    kPixelFormat_L8,
    kPixelFormat_A8,
    kPixelFormat_LA88,
    kPixelFormat_RGB_PVRTC2,
    kPixelFormat_RGB_PVRTC4,
    kPixelFormat_RGBA_PVRTC2,
    kPixelFormat_RGBA_PVRTC4
};

namespace Sexy {

inline int Rand(int range) {
    return reinterpret_cast<int (*)(int)>(Sexy_RandIntAddr)(range);
}

inline float Rand(float range) {
    return reinterpret_cast<float (*)(float)>(Sexy_RandFloatAddr)(range);
}

inline void vformat(pvzstl::string &output, const char *fmt, va_list vList) {
    reinterpret_cast<void (*)(pvzstl::string &, const char *, va_list)>(Sexy_vformatAddr)(output, fmt, vList);
}

[[gnu::format(printf, 1, 2)]] inline pvzstl::string StrFormat(const char *fmt, ...) {
    pvzstl::string output;
    va_list args;
    va_start(args, fmt);
    vformat(output, fmt, args);
    va_end(args);
    return output;
}

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_MISC_COMMON_H
