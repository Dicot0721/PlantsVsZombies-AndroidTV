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

#ifndef PVZ_SEXYAPPFRAMEWORK_EFFECT_REANIM_TRACK_WIDGET_ANIMATOR_H
#define PVZ_SEXYAPPFRAMEWORK_EFFECT_REANIM_TRACK_WIDGET_ANIMATOR_H

#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/SexyAppFramework/Misc/WidgetAnimator.h"

class ReanimTrackWidgetAnimator : public Sexy::WidgetAnimator {
public:
    LawnApp *mApp;                 // 6
    Sexy::Widget *mLawnMenuWidget; // 7
    int *mTrackName1;              // 8
    int *mTrackName2;              // 9
    int *mTrackName3;              // 10
    int unk2[2];                   // 11 ~ 12
};

#endif // PVZ_SEXYAPPFRAMEWORK_EFFECT_REANIM_TRACK_WIDGET_ANIMATOR_H
