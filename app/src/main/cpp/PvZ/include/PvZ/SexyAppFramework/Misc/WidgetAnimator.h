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

#ifndef PVZ_SEXYAPPFRAMEWORK_MISC_WIDGET_ANIMATOR_H
#define PVZ_SEXYAPPFRAMEWORK_MISC_WIDGET_ANIMATOR_H

#include "../Widget/Widget.h"

namespace Sexy {

class WidgetAnimator {
public:
    int *vTable;            // 0
    int unk[2];             // 1 ~ 2
    Widget *mWidget;        // 3
    int mUpdateCounter;     // 4
    bool mHideOnCompletion; // 20
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_MISC_WIDGET_ANIMATOR_H
