//
// Created by 28636 on 2025/5/26.
//

#include "PvZ/SexyAppFramework/Misc/Common.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "PvZ/SexyAppFramework/Misc/PerfTimer.h"

using namespace Sexy;

void Sexy::StrFormat(int *holder, const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    vformat(holder, fmt, args);
    va_end(args);
}

void Sexy::StringDelete(int *holder) {
    auto my_exchange_and_add = [](int *ptr, int value) -> int {
        int original_value = *ptr;
        *ptr += value;
        return original_value;
    };

    int ptr = holder[0] - 12;
    if (my_exchange_and_add((int *)(holder[0] - 4), -1) <= 0) {
        operator delete((int *)ptr);
    }
}