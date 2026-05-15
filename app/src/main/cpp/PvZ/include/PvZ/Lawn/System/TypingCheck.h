/*
 * Copyright (C) 2023-2026  PvZ TV Touch Team
 *
 * This file is part of PlantsVsZombies-AndroidTV.
 *
 * PlantsVsZombies-AndroidTV is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * PlantsVsZombies-AndroidTV is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * PlantsVsZombies-AndroidTV.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef PVZ_LAWN_SYSTEM_TYPING_CHECK_H
#define PVZ_LAWN_SYSTEM_TYPING_CHECK_H

#include "Homura/TypeUtils.h"
#include "PvZ/STL/string.h"
#include "PvZ/Symbols.h"

class TypingCheck {
protected:
    homura::Storage<pvzstl::string> mPhrase;

public:
    TypingCheck() {
        reinterpret_cast<void (*)(TypingCheck *)>(TypingCheck_TypingCheckAddr)(this);
    }

    TypingCheck(pvzstl::string thePhrase) {
        reinterpret_cast<void (*)(TypingCheck *, pvzstl::string)>(TypingCheck_CreateByStringAddr)(this, std::move(thePhrase));
    }

    void SetPhrase(const pvzstl::string &thePhrase) {
        reinterpret_cast<void (*)(TypingCheck *, const pvzstl::string &)>(TypingCheck_SetPhraseAddr)(this, thePhrase);
    }

    void AddChar(char theChar) {
        reinterpret_cast<void (*)(TypingCheck *, char)>(TypingCheck_AddCharAddr)(this, theChar);
    }
};

#endif // PVZ_LAWN_SYSTEM_TYPING_CHECK_H
