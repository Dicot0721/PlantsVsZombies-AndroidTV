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

#ifndef PVZ_LAWN_WIDGET_LEADERBOARDS_WIDGET_H
#define PVZ_LAWN_WIDGET_LEADERBOARDS_WIDGET_H

#include "GameButton.h"
#include "HouseChooserDialog.h"
#include "PvZ/SexyAppFramework/Widget/ButtonListener.h"
#include "PvZ/SexyAppFramework/Widget/Widget.h"
#include "TrashBin.h"

class Reanimation;

struct GameStats {

    enum MiscStat {
        ZOMBIES_KILLED = 0,
        PLANTS_KILLED = 1,
        MONEY = 2,
        TREE_HEIGHT = 4,
        ENDLESS_FLAGS = 5,
        MOWED_ZOMBIES = 6,
        STINKY_CHOCOLATES = 7,
    };

    HouseType mHouseType;      // 0
    int unk;                   // 1
    int mMiscStats[9];         // 2 ~ 10
    char mFavoritePlants[49];  // 44 ~ 92
    char mFavoriteZombies[47]; // 93 ~ 139
    float mSurvivalCompletion; // 35
    float mMiniGameCompletion; // 36
    float mPuzzleCompletion;   // 37
}; // 大小38个整数

struct LeaderboardReanimations {
    Reanimation *backgroundReanim[5];
    Reanimation *achievementReanim[12];
};

class LeaderboardsWidget : public Sexy::__Widget {
public:
    LeaderboardsWidget(LawnApp *theApp);

    void ButtonPress(this LeaderboardsWidget &self, int id, int theCount) {}

    void ButtonDepress(this LeaderboardsWidget &self, int id);

    LawnApp *mApp;                                     // 64
    TrashBin *mZombieTrashBin;                         // 65
    TrashBin *mPlantTrashBin;                          // 66
    bool mAchievements[12];                            // 67 ~ 69
    LeaderboardReanimations *mLeaderboardReanimations; // 70
    int mLongestRecordPool;                            // 71
    GameButton *mBackButton;
    Sexy::ButtonListener *mButtonListener = &sButtonListener;
    int mFocusedAchievementIndex;
    bool mHighLightAchievement;

private:
    static constexpr Sexy::ButtonListener::VTable sButtonListenerVtable{// .ButtonPress = (void *)LeaderboardsWidget_ButtonPress;
                                                                        .ButtonPress2 = (void *)&LeaderboardsWidget::ButtonPress,
                                                                        .ButtonDepress = (void *)&LeaderboardsWidget::ButtonDepress};

    static inline Sexy::ButtonListener sButtonListener{&sButtonListenerVtable};

}; // 我想用LeaderboardsWidget取代DaveHelp。

class DaveHelp : public Sexy::__Widget {
public:
    DaveHelp(LawnApp *theApp) {
        __Constructor(theApp);
    }

protected:
    void __Constructor(LawnApp *theApp) {
        reinterpret_cast<void (*)(DaveHelp *, LawnApp *)>(DaveHelp_DaveHelpAddr)(this, theApp);
    }
};


inline void (*old_DaveHelp_Delete2)(LeaderboardsWidget *leaderboardsWidget);

void DaveHelp_Delete2(LeaderboardsWidget *leaderboardsWidget);

void DaveHelp_Update(LeaderboardsWidget *leaderboardsWidget);

void DaveHelp_Draw(LeaderboardsWidget *leaderboardsWidget, Sexy::Graphics *g);

void DaveHelp_DealClick(LeaderboardsWidget *leaderboardsWidget, int id);

void DaveHelp_MouseDown(LeaderboardsWidget *leaderboardsWidget, int x, int y, int theClickCount);

void DaveHelp_MouseUp(LeaderboardsWidget *leaderboardsWidget, int x, int y);

void DaveHelp_MouseDrag(LeaderboardsWidget *leaderboardsWidget, int x, int y);

void DaveHelp_KeyDown(LeaderboardsWidget *leaderboardsWidget, int keyCode);

#endif // PVZ_LAWN_WIDGET_LEADERBOARDS_WIDGET_H
