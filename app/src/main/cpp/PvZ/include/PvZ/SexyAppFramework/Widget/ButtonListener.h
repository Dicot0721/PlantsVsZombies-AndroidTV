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

#ifndef PVZ_SEXYAPPFRAMEWORK_WIDGET_BUTTON_LISTENER_H
#define PVZ_SEXYAPPFRAMEWORK_WIDGET_BUTTON_LISTENER_H

namespace Sexy {

class ButtonListener {
public:
    struct VTable {
        void *_destructor = (void *)&ButtonListener::_destructor;
        void *_destructor2 = (void *)&ButtonListener::_destructor2;
        void *ButtonPress = (void *)&ButtonListener::ButtonPress;
        void *ButtonPress2 = (void *)&ButtonListener::ButtonPress2;
        void *ButtonDepress = (void *)&ButtonListener::ButtonDepress;
        void *ButtonDownTick = (void *)&ButtonListener::ButtonDownTick;
        void *ButtonMouseEnter = (void *)&ButtonListener::ButtonMouseEnter;
        void *ButtonMouseLeave = (void *)&ButtonListener::ButtonMouseLeave;
        void *ButtonMouseMove = (void *)&ButtonListener::ButtonMouseMove;
    };

    const VTable *mVTable;

private:
    void _destructor(this ButtonListener &self) {}
    void _destructor2(this ButtonListener &self) {}
    void ButtonPress(this ButtonListener &self, int theId) {}
    void ButtonPress2(this ButtonListener &self, int theId, int theClickCount) {
        self.ButtonPress(theId);
    }
    void ButtonDepress(this ButtonListener &self, int theId) {}
    void ButtonDownTick(this ButtonListener &self, int id) {}
    void ButtonMouseEnter(this ButtonListener &self, int id) {}
    void ButtonMouseLeave(this ButtonListener &self, int id) {}
    void ButtonMouseMove(this ButtonListener &self, int id, int x, int y) {}
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_WIDGET_BUTTON_LISTENER_H
