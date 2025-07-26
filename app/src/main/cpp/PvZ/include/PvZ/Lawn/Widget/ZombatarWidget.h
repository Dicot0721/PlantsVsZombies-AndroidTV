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

#ifndef PVZ_LAWN_WIDGET_ZOMBATAR_WIDGET_H
#define PVZ_LAWN_WIDGET_ZOMBATAR_WIDGET_H

#include "PvZ/SexyAppFramework/Widget/GameButton.h"
#include "PvZ/SexyAppFramework/Widget/MenuWidget.h"

class Zombie;
class Reanimation;

class ZombatarWidget : public Sexy::MenuWidget {
public:
    enum TabType {
        SKIN = 0,
        HAIR = 1,
        FHAIR = 2,
        TIDBIT = 3,
        EYEWEAR = 4,
        CLOTHES = 5,
        ACCESSORY = 6,
        HAT = 7,
        BACKGROUND = 8,
        MAX_TAB_NUM = 9,
    };

    enum AccessoryNum {
        HairNum = 16,
        FHairNum = 24,
        TidBitNum = 14,
        EyeWearNum = 16,
        ClothNum = 12,
        AccessoryNum = 15,
        HatNum = 14,
        BackgroundNum = 83,
    };

    Sexy::ButtonListener *mButtonListener = &sButtonListener;
    LawnApp *mApp;
    Zombie *mPreviewZombie;
    Sexy::GameButton *mBackButton;
    Sexy::GameButton *mFinishButton;
    Sexy::GameButton *mViewPortraitButton;
    Sexy::GameButton *mNewButton;
    Sexy::GameButton *mDeleteButton;
    Reanimation *mZombatarReanim;
    bool mShowExistingZombatarPortrait;
    bool mShowZombieTypeSelection;
    unsigned char mSelectedTab;
    unsigned char mSelectedSkinColor;
    unsigned char mSelectedHair;
    unsigned char mSelectedHairColor;
    unsigned char mSelectedFHair;
    unsigned char mSelectedFHairColor;
    unsigned char mSelectedFHairPage; // 0 or 1
    unsigned char mSelectedTidBit;
    unsigned char mSelectedTidBitColor;
    unsigned char mSelectedEyeWear;
    unsigned char mSelectedEyeWearColor;
    unsigned char mSelectedCloth;
    unsigned char mSelectedAccessory;
    unsigned char mSelectedAccessoryColor;
    unsigned char mSelectedHat;
    unsigned char mSelectedHatColor;
    unsigned char mSelectedBackground;
    unsigned char mSelectedBackgroundColor;
    unsigned char mSelectedBackgroundPage;

    ZombatarWidget(LawnApp *theApp);

    void ButtonPress(this ZombatarWidget &self, int id) {}

    void ButtonDepress(this ZombatarWidget &self, int id);

private:
    static constexpr Sexy::ButtonListener::VTable sButtonListenerVtable{
        // .ButtonPress = (void *)LeaderboardsWidget_ButtonPress;
        .ButtonPress2 = (void *)&ZombatarWidget::ButtonPress,
        .ButtonDepress = (void *)&ZombatarWidget::ButtonDepress,
    };

    static inline Sexy::ButtonListener sButtonListener{&sButtonListenerVtable};

}; // 我想用ZombatarWidget取代TestMenuWidget。;

class TestMenuWidget : public Sexy::MenuWidget {
public:
    TestMenuWidget() {
        __Constructor();
    }

protected:
    void __Constructor() {
        reinterpret_cast<void (*)(TestMenuWidget *)>(TestMenuWidget_TestMenuWidgetAddr)(this);
    }
};

#endif // PVZ_LAWN_WIDGET_ZOMBATAR_WIDGET_H
