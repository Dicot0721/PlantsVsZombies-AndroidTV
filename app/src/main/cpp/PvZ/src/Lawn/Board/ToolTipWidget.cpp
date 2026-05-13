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

#include "PvZ/Lawn/Board/ToolTipWidget.h"
#include "PvZ/SexyAppFramework/Graphics/Font.h"
#include "PvZ/TodLib/Common/TodStringFile.h"

using namespace Sexy;

void ToolTipWidget::Draw(Graphics *g) {
    // TODO: 半透明处理已选卡的描述文本
    //    g->SetColorizeImages(true);
    //    g->SetColor(Color(255, 255, 255, 125));

    old_ToolTipWidget_Draw(this, g);
}