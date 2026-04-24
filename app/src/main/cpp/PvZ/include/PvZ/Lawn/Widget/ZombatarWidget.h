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

#ifndef PVZ_LAWN_WIDGET_ZOMBATAR_WIDGET_H
#define PVZ_LAWN_WIDGET_ZOMBATAR_WIDGET_H

#include "GameButton.h"
#include "PvZ/SexyAppFramework/Widget/MenuWidget.h"

class Zombie;
class Reanimation;

class ZombatarWidget : public Sexy::MenuWidget {
public:
    enum {
        ZombatarWidget_Back = 1000,
        ZombatarWidget_Finish,
        ZombatarWidget_ViewPortrait,
        ZombatarWidget_New,
        ZombatarWidget_Delete,
    };

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
    GameButton *mBackButton;
    GameButton *mFinishButton;
    GameButton *mViewPortraitButton;
    GameButton *mNewButton;
    GameButton *mDeleteButton;
    Reanimation *mZombatarReanim;
    bool mShowExistingZombatarPortrait = false;
    bool mShowZombieTypeSelection = false;
    unsigned char mSelectedTab = 0;
    unsigned char mSelectedSkinColor = 0;
    unsigned char mSelectedHair = 255;
    unsigned char mSelectedHairColor = 255;
    unsigned char mSelectedFHair = 255;
    unsigned char mSelectedFHairColor = 255;
    unsigned char mSelectedFHairPage = 0; // 0 or 1
    unsigned char mSelectedTidBit = 255;
    unsigned char mSelectedTidBitColor = 255;
    unsigned char mSelectedEyeWear = 255;
    unsigned char mSelectedEyeWearColor = 255;
    unsigned char mSelectedCloth = 255;
    unsigned char mSelectedAccessory = 255;
    unsigned char mSelectedAccessoryColor = 255;
    unsigned char mSelectedHat = 255;
    unsigned char mSelectedHatColor = 255;
    unsigned char mSelectedBackground = 0;
    unsigned char mSelectedBackgroundColor = 255;
    unsigned char mSelectedBackgroundPage = 0;

    ZombatarWidget(LawnApp *theApp);
    ~ZombatarWidget();

    void Delete();
    void RemovedFromManager(Sexy::WidgetManager *theWidgetManager);
    void SetDefault();
    void Update();
    void DrawZombieSelection(Sexy::Graphics *g);
    void DrawSkin(Sexy::Graphics *graphics);
    void DrawHair(Sexy::Graphics *g);
    void DrawFHair(Sexy::Graphics *g);
    void DrawTidBit(Sexy::Graphics *g);
    void DrawEyeWear(Sexy::Graphics *g);
    void DrawCloth(Sexy::Graphics *g);
    void DrawAccessory(Sexy::Graphics *g);
    void DrawHat(Sexy::Graphics *g);
    void DrawBackground(Sexy::Graphics *g);
    void DrawPortrait(Sexy::Graphics *g, int x, int y);
    void DrawPreView(Sexy::Graphics *g);
    void Draw(Sexy::Graphics *g);

    static Sexy::Image *GetTabButtonImageByIndex(int index);
    static Sexy::Image *GetTabButtonDownImageByIndex(int index);
    static Sexy::Image *GetHairImageByIndex(int index);
    static Sexy::Image *GetHairMaskImageByIndex(int index);
    static Sexy::Image *GetFHairImageByIndex(int index);
    static Sexy::Image *GetFHairMaskImageByIndex(int index);
    static Sexy::Image *GetTidBitImageByIndex(int index);
    static Sexy::Image *GetEyeWearImageByIndex(int index);
    static Sexy::Image *GetEyeWearMaskImageByIndex(int index);
    static Sexy::Image *GetClothImageByIndex(int index);
    static Sexy::Image *GetAccessoryImageByIndex(int index);
    static Sexy::Image *GetHatImageByIndex(int index);
    static Sexy::Image *GetHatMaskImageByIndex(int index);
    static Sexy::Image *GetBackgroundImageByIndex(int index);

    static int GetTidBitImageOffsetXByIndex(int index);
    static int GetTidBitImageOffsetYByIndex(int index);
    static int GetEyeWearImageOffsetXByIndex(int index);
    static int GetEyeWearImageOffsetYByIndex(int index);
    static int GetAccessoryImageOffsetXByIndex(int index);
    static int GetAccessoryImageOffsetYByIndex(int index);
    static int GetFHairImageOffsetXByIndex(int index);
    static int GetFHairImageOffsetYByIndex(int index);
    static int GetFHairMaskImageOffsetXByIndex(int index);
    static int GetFHairMaskImageOffsetYByIndex(int index);
    static int GetHairImageOffsetXByIndex(int index);
    static int GetHairImageOffsetYByIndex(int index);
    static int GetHairMaskImageOffsetXByIndex(int index);
    static int GetHairMaskImageOffsetYByIndex(int index);
    static int GetClothImageOffsetXByIndex(int index);
    static int GetClothImageOffsetYByIndex(int index);
    static int GetHatImageOffsetXByIndex(int index);
    static int GetHatImageOffsetYByIndex(int index);
    static int GetHatMaskImageOffsetXByIndex(int index);
    static int GetHatMaskImageOffsetYByIndex(int index);

    static bool AccessoryIsColorized(int tab, int accessory);

    void MouseDownSkin(int x, int y);
    void MouseDownHair(int x, int y);
    void MouseDownFHair(int x, int y);
    void MouseDownTidBit(int x, int y);
    void MouseDownEyeWear(int x, int y);
    void MouseDownCloth(int x, int y);
    void MouseDownAccessory(int x, int y);
    void MouseDownHat(int x, int y);
    void MouseDownBackground(int x, int y);
    void MouseDown(int x, int y);
    void MouseUp(int x, int y);
    void MouseDrag(int x, int y);
    void KeyDown(Sexy::KeyCode theKey);

    void ButtonPress(this ZombatarWidget &self, int id) {}

    void ButtonDepress(this ZombatarWidget &self, int theId);

private:
    static inline const Sexy::ButtonListener::VTable sButtonListenerVtable{
        // .ButtonPress = (void *)LeaderboardsWidget_ButtonPress;
        .ButtonPress2 = (void *)&ZombatarWidget::ButtonPress,
        .ButtonDepress = (void *)&ZombatarWidget::ButtonDepress,
    };

    static inline Sexy::ButtonListener sButtonListener{&sButtonListenerVtable};

}; // 我想用ZombatarWidget取代TestMenuWidget。;

class TestMenuWidget : public Sexy::MenuWidget {
public:
    TestMenuWidget() {
        _constructor();
    }
    ~TestMenuWidget() = delete;

protected:
    void _constructor() {
        reinterpret_cast<void (*)(TestMenuWidget *)>(TestMenuWidget_TestMenuWidgetAddr)(this);
    }
};

inline void (*old_TestMenuWidget_RemovedFromManager)(ZombatarWidget *zombatarWidget, Sexy::WidgetManager *manager);
inline void (*old_TestMenuWidget_Delete2)(ZombatarWidget *zombatarWidget);
inline void (*old_TestMenuWidget_Delete)(ZombatarWidget *zombatarWidget);


void TestMenuWidget_Delete(ZombatarWidget *zombatarWidget);
void TestMenuWidget_Delete2(ZombatarWidget *zombatarWidget);
void TestMenuWidget_RemovedFromManager(ZombatarWidget *zombatarWidget, Sexy::WidgetManager *manager);
void TestMenuWidget_Update(ZombatarWidget *zombatarWidget);
void TestMenuWidget_Draw(ZombatarWidget *zombatarWidget, Sexy::Graphics *graphics);
void TestMenuWidget_MouseDown(ZombatarWidget *zombatarWidget, int x, int y);
void TestMenuWidget_MouseUp(ZombatarWidget *zombatarWidget, int x, int y);
void TestMenuWidget_MouseDrag(ZombatarWidget *zombatarWidget, int x, int y);
void TestMenuWidget_KeyDown(ZombatarWidget *zombatarWidget, Sexy::KeyCode keyCode);

#endif // PVZ_LAWN_WIDGET_ZOMBATAR_WIDGET_H
