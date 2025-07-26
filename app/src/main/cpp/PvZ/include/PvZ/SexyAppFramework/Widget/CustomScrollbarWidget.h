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

#ifndef PVZ_SEXYAPPFRAMEWORK_WIDGET_CUSTOM_SCROLLBAR_WIDGET_H
#define PVZ_SEXYAPPFRAMEWORK_WIDGET_CUSTOM_SCROLLBAR_WIDGET_H

#include "ScrollbarWidget.h"

namespace Sexy {

// 没有新增任何成员，6，这个类不知道什么意义
class CustomScrollbarWidget : public ScrollbarWidget {
public:
    // 大小90个整数

    void RemoveScrollButtons();
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_WIDGET_CUSTOM_SCROLLBAR_WIDGET_H
