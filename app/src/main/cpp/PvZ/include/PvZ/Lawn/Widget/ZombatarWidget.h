//
// Created by 28636 on 2025/5/26.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_ZOMBATARWIDGET_H
#define PLANTSVSZOMBIES_ANDROIDTV_ZOMBATARWIDGET_H

#include "PvZ/SexyAppFramework/Widget/GameButton.h"
#include "PvZ/SexyAppFramework/Widget/MenuWidget.h"

class Zombie;
class Reanimation;

class ZombatarWidget : public Sexy::MenuWidget {
public:
    enum TabType { SKIN = 0, HAIR = 1, FHAIR = 2, TIDBIT = 3, EYEWEAR = 4, CLOTHES = 5, ACCESSORY = 6, HAT = 7, BACKGROUND = 8, MaxTabNum = 9 };

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

    Sexy::ButtonListener *mButtonListener;
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

}; // 我想用ZombatarWidget取代TestMenuWidget。;

class TestMenuWidget : public Sexy::MenuWidget {
public:
    void Create() { reinterpret_cast<void (*)(TestMenuWidget *)>(TestMenuWidget_TestMenuWidgetAddr)(this); }

    TestMenuWidget() { Create(); }
};

#endif // PLANTSVSZOMBIES_ANDROIDTV_ZOMBATARWIDGET_H
