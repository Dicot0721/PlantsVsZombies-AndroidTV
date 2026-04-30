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

#ifndef PVZ_LAWN_WIDGET_ACHIEVEMENTS_WIDGET_H
#define PVZ_LAWN_WIDGET_ACHIEVEMENTS_WIDGET_H

#include "PvZ/SexyAppFramework/Widget/ButtonListener.h"
#include "PvZ/SexyAppFramework/Widget/Widget.h"
#include "PvZ/Symbols.h"

inline constexpr int ACHIEVEMENT_HOLE_LENGTH = 136;
inline constexpr int ACHIEVEMENT_HOLE_WORM_POS = 0x10;
inline constexpr int ACHIEVEMENT_HOLE_GEMS_POS = 0x19;
inline constexpr int ACHIEVEMENT_HOLE_CHUZZLE_POS = 0x26;
inline constexpr int ACHIEVEMENT_HOLE_BJORN_POS = 0x34;
inline constexpr int ACHIEVEMENT_HOLE_PIPE_POS = 0x45;
inline constexpr int ACHIEVEMENT_HOLE_TIKI_POS = 0x55;
inline constexpr int ACHIEVEMENT_HOLE_HEAVY_ROCKS_POS = 0x65;
inline constexpr int ACHIEVEMENT_HOLE_DU_WEI_POS = 0x72;
inline constexpr int MAIN_MENU_HEIGHT = 720 - 2; // 作用：将成就界面上升2个像素点，以更紧密地贴合主界面。奇怪，理论上720是严丝合缝，为什么实际有2像素偏差呢？
inline constexpr int KEYBOARD_SCROLL_TIME = 20;

enum AchievementType {
    ACHIEVEMENT_HOME_SECURITY = 0,
    ACHIEVEMENT_MORTICULTURALIST = 1,
    ACHIEVEMENT_IMMORTAL = 2,
    ACHIEVEMENT_SOILPLANTS = 3,
    ACHIEVEMENT_CLOSESHAVE = 4,
    ACHIEVEMENT_CHOMP = 5,
    ACHIEVEMENT_VERSUS = 6,
    ACHIEVEMENT_GARG = 7,
    ACHIEVEMENT_COOP = 8,
    ACHIEVEMENT_SHOP = 9,
    ACHIEVEMENT_EXPLODONATOR = 10,
    ACHIEVEMENT_TREE = 11,
    NUM_ACHIEVEMENT_TYPES
};

enum AchievementWidgetState {
    NOT_SHOWING = 0,
    SLIDING_IN = 1,
    SHOWING = 2,
    SLIDING_OUT = 3,
};

inline AchievementWidgetState gAchievementState = NOT_SHOWING;

class LawnApp;

namespace Sexy {
class Graphics;
}

class AchievementsWidget : public Sexy::Widget, public Sexy::ButtonListener {
public:
    LawnApp *mApp; // 65
    int mMouseDownY;
    int mLastDownY;
    int mLastDownY1;
    long mLastTimeMs;
    long mLastTimeMs1;
    float mVelocity;
    float mAccY;
    bool mIsScrolling;

    AchievementsWidget(LawnApp *theApp);
    ~AchievementsWidget();

    void Update();
    void Draw(Sexy::Graphics *g);
    void MouseDown(int x, int y, int theClickCount);
    void MouseUp(int x, int y);
    void MouseDrag(int x, int y);
};

class MaskHelpWidget : public Sexy::Widget {
public:
    MaskHelpWidget(LawnApp *theApp) {
        _constructor(theApp);
    }
    ~MaskHelpWidget() {
        _destructor();
    }

protected:
    void _constructor(LawnApp *theApp) {
        reinterpret_cast<void (*)(MaskHelpWidget *, LawnApp *)>(MaskHelpWidget_MaskHelpWidgetAddr)(this, theApp);
    }
    void _destructor() {
        reinterpret_cast<void (*)(MaskHelpWidget *)>(MaskHelpWidget_DeleteAddr)(this);
    }
};

void MaskHelpWidget_Update(AchievementsWidget *achievementsWidget);
void MaskHelpWidget_Draw(AchievementsWidget *achievementsWidget, Sexy::Graphics *g);
void MaskHelpWidget_MouseDown(AchievementsWidget *achievementsWidget, int x, int y, int theClickCount);
void MaskHelpWidget_MouseUp(AchievementsWidget *achievementsWidget, int x, int y);
void MaskHelpWidget_MouseDrag(AchievementsWidget *achievementsWidget, int x, int y);

#endif // PVZ_LAWN_WIDGET_ACHIEVEMENTS_WIDGET_H
