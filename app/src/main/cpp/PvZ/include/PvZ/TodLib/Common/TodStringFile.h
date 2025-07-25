//
// Created by 28636 on 2025/6/11.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_TODSTRINGFILE_H
#define PLANTSVSZOMBIES_ANDROIDTV_TODSTRINGFILE_H

#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/Symbols.h"

inline void TodDrawStringWrapped(
    Sexy::Graphics *g, const pvzstl::string &theText, const Sexy::Rect &theRect, Sexy::Font *theFont, const Sexy::Color &theColor, DrawStringJustification theJustification, bool drawString) {
    reinterpret_cast<void (*)(Sexy::Graphics *, const pvzstl::string &, const Sexy::Rect &, Sexy::Font *, const Sexy::Color &, DrawStringJustification, bool)>(TodDrawStringWrappedAddr)(
        g, theText, theRect, theFont, theColor, theJustification, drawString);
}

inline int TodDrawStringWrappedHelper(
    Sexy::Graphics *g, const pvzstl::string &theText, const Sexy::Rect &theRect, Sexy::Font *theFont, const Sexy::Color &theColor, DrawStringJustification theJustification, bool drawString, bool i2) {
    return reinterpret_cast<int (*)(Sexy::Graphics *, const pvzstl::string &, const Sexy::Rect &, Sexy::Font *, const Sexy::Color &, DrawStringJustification, bool, bool)>(
        TodDrawStringWrappedHelperAddr)(g, theText, theRect, theFont, theColor, theJustification, drawString, i2);
}

inline pvzstl::string TodStringTranslate(const char *theString) {
    pvzstl::string str;
    reinterpret_cast<void (*)(pvzstl::string &, const char *)>(TodStringTranslateAddr)(str, theString);
    return str;
}

inline void TodStringListLoad(const char *theFileName) {
    reinterpret_cast<void (*)(const char *)>(TodStringListLoadAddr)(theFileName);
}

inline void TodStringListFind(pvzstl::string &result, const pvzstl::string &theName) {
    reinterpret_cast<void (*)(pvzstl::string &, const pvzstl::string &)>(TodStringListFindAddr)(result, theName);
}

// inline void TodStringListLoad(const char* theFileName) {
//     reinterpret_cast<void (*)(const char*)>(TodStringListLoadAddr)(theFileName);
// }

#endif // PLANTSVSZOMBIES_ANDROIDTV_TODSTRINGFILE_H