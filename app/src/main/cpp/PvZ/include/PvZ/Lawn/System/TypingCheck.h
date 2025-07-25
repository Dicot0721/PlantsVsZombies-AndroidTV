#ifndef PVZ_LAWN_SYSTEM_TYPING_CHECK_H
#define PVZ_LAWN_SYSTEM_TYPING_CHECK_H

#include "PvZ/STL/pvzstl_string.h"
#include "PvZ/Symbols.h"

class TypingCheck {
protected:
    pvzstl::string mPhrase;
    pvzstl::string mRecentTyping;

public:
    void SetPhrase(const pvzstl::string& thePhrase) {
        reinterpret_cast<void (*)(const pvzstl::string&)>(TypingCheck_SetPhraseAddr)(thePhrase);
    }
};

#endif // PVZ_LAWN_SYSTEM_TYPING_CHECK_H
