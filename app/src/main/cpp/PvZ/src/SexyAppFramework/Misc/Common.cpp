//
// Created by 28636 on 2025/5/26.
//

#include "PvZ/SexyAppFramework/Misc/Common.h"
#include "PvZ/SexyAppFramework/Misc/PerfTimer.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

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