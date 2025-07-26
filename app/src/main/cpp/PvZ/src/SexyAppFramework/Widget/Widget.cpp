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

#include "PvZ/SexyAppFramework/Widget/Widget.h"

using namespace Sexy;

void __Widget::MarkDirty() {
    (*((void (**)(__Widget *))this->vTable + 25))(this); // MarkDirty();
}

void __Widget::AddWidget(__Widget *child) {
    (*((void (**)(__Widget *, __Widget *))this->vTable + 6))(this, child); // AddWidget();
}

void __Widget::RemoveWidget(__Widget *child) {
    (*((void (**)(__Widget *, __Widget *))this->vTable + 7))(this, child); // RemoveWidget();
}

Widget *__Widget::FindWidget(int theId) {
    return (*((Widget * (**)(__Widget *, int)) this->vTable + 9))(this, theId); // FindWidget();
}
