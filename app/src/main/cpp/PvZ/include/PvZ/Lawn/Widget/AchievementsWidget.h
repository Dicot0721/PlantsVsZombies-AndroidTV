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

enum AchievementId {
    ACHIEVEMENT_HOME_SECURITY = 0,    // 完成冒险模式
    ACHIEVEMENT_MORTICULTURALIST = 1, // 收集全部49种植物
    ACHIEVEMENT_IMMORTAL = 2,         // 生存泳池无尽达到40波
    ACHIEVEMENT_SOILPLANTS = 3,       // 在一个关卡内种植10个豌豆射手
    ACHIEVEMENT_CLOSESHAVE = 4,       // 用完全部小推车过一关
    ACHIEVEMENT_CHOMP = 5,            // 只使用大嘴花、坚果墙、向日葵过一关
    ACHIEVEMENT_VERSUS = 6,           // 对战模式获得5连胜
    ACHIEVEMENT_GARG = 7,             // 打败巨人僵尸
    ACHIEVEMENT_COOP = 8,             // 通关结盟坚果保龄球关卡
    ACHIEVEMENT_SHOP = 9,             // 在戴夫商店消费25000元
    ACHIEVEMENT_EXPLODONATOR = 10,    // 使用一个樱桃炸弹消灭10只僵尸
    ACHIEVEMENT_TREE = 11,            // 让智慧树长到100英尺高
    MAX_ACHIEVEMENTS
};

class AchievementsWidget : public Sexy::__Widget {
public:
    Sexy::ButtonListener mButtonListener; // 64
    LawnApp *mApp;                        // 65
    int mMouseDownY;
    int mLastDownY;
    int mLastDownY1;
    long mLastTimeMs;
    long mLastTimeMs1;
    float mVelocity;
    float mAccY;
    bool mIsScrolling;
}; // 我想用AchievementsWidget取代MaskHelpWidget。MaskHelpWidget的大小是66个整数。

class MaskHelpWidget : public Sexy::__Widget {
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

#endif // PVZ_LAWN_WIDGET_ACHIEVEMENTS_WIDGET_H
