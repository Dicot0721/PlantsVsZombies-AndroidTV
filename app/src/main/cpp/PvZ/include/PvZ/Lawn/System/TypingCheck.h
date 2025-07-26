/*
 * Copyright (C) 2023-2025  PvZ TV Touch Team
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
