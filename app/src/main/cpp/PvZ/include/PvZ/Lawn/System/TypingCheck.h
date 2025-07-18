//
// Created by 28636 on 2025/7/18.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_TYPINGCHECK_H
#define PLANTSVSZOMBIES_ANDROIDTV_TYPINGCHECK_H

#include "PvZ/STL/pvzstl_string.h"
#include "PvZ/Symbols.h"

class TypingCheck {
protected:
    pvzstl::string mPhrase;
    pvzstl::string mRecentTyping;

public:
    void SetPhrase(const pvzstl::string& thePhrase) { reinterpret_cast<void (*)(const pvzstl::string&)>(TypingCheck_SetPhraseAddr)(thePhrase); }
};

#endif // PLANTSVSZOMBIES_ANDROIDTV_TYPINGCHECK_H
