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

#ifndef PVZ_LAWN_WIDGET_HELP_BAR_WIDGET_H
#define PVZ_LAWN_WIDGET_HELP_BAR_WIDGET_H

#include "PvZ/SexyAppFramework/Widget/Widget.h"

class HelpBarWidget : public Sexy::Widget {
public:
    char mUnk[40]; // 65 ~ 74

    HelpBarWidget() {
        _constructor();
    }

protected:
    friend void InitHookFunction();

    void _constructor();
};

inline void (*old_HelpBarWidget_HelpBarWidget)(HelpBarWidget *);

#endif // PVZ_LAWN_WIDGET_HELP_BAR_WIDGET_H
