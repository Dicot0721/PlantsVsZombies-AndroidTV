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

#include "PvZ/Lawn/Widget/VSSetupAddonWidget.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/Widget/VSSetupMenu.h"

using namespace Sexy;

VSSetupAddonWidget::VSSetupAddonWidget(VSSetupMenu *theVSSetupMenu) {
    mButtonListener = theVSSetupMenu;

    mExtraPacketsMode = mApp->mPlayerInfo->mVSExtraPacketsMode;
    mExtraSeedsMode = mApp->mPlayerInfo->mVSExtraSeedsMode;
    mBanMode = mApp->mPlayerInfo->mVSBanMode;
    mBalancePatchMode = mApp->mPlayerInfo->mVSBalancePatchMode;
    mShuffleMode = mApp->mPlayerInfo->mVSShuffleMode;

    Image *aCheckbox = *Sexy_IMAGE_OPTIONS_CHECKBOX0_Addr;
    Image *aCheckboxPressed = *Sexy_IMAGE_OPTIONS_CHECKBOX1_Addr;
    mExtraPacketsButton = MakeNewButton(VSSetupAddonWidget_ExtraPackets, mButtonListener, theVSSetupMenu, "", nullptr, aCheckbox, aCheckboxPressed, aCheckboxPressed);
    mExtraSeedsButton = MakeNewButton(VSSetupAddonWidget_ExtraSeeds, mButtonListener, theVSSetupMenu, "", nullptr, aCheckbox, aCheckboxPressed, aCheckboxPressed);
    mBanModeButton = MakeNewButton(VSSetupAddonWidget_BanMode, mButtonListener, theVSSetupMenu, "", nullptr, aCheckbox, aCheckboxPressed, aCheckboxPressed);
    mBalancePatchButton = MakeNewButton(VSSetupAddonWidget_BalancePatch, mButtonListener, theVSSetupMenu, "", nullptr, aCheckbox, aCheckboxPressed, aCheckboxPressed);
    mShuffleModeButton = MakeNewButton(VSSetupAddonWidget_ShuffleMode, mButtonListener, theVSSetupMenu, "", nullptr, aCheckbox, aCheckboxPressed, aCheckboxPressed);

    mExtraPacketsButton->Resize(VS_ADDON_BUTTON_X, VS_BUTTON_EXTRA_PACKETS_Y, 175, 50);
    mExtraSeedsButton->Resize(VS_ADDON_BUTTON_X, VS_BUTTON_EXTRA_SEEDS_Y, 175, 50);
    mBanModeButton->Resize(VS_ADDON_BUTTON_X, VS_BUTTON_BAN_MODE_Y, 175, 50);
    mBalancePatchButton->Resize(VS_ADDON_BUTTON_X, VS_BUTTON_BALANCE_PATCH_Y, 175, 50);
    mShuffleModeButton->Resize(VS_ADDON_BUTTON_X, VS_BUTTON_SHUFFLE_MODE_Y, 175, 50);

    mApp->mBoard->AddWidget(mExtraPacketsButton);
    mApp->mBoard->AddWidget(mExtraSeedsButton);
    mApp->mBoard->AddWidget(mBanModeButton);
    mApp->mBoard->AddWidget(mBalancePatchButton);
    mApp->mBoard->AddWidget(mShuffleModeButton);
}

VSSetupAddonWidget::~VSSetupAddonWidget() {
    if (mApp->mBoard) {
        if (mExtraPacketsButton) {
            mApp->mBoard->RemoveWidget(mExtraPacketsButton);
            mExtraPacketsButton->_destructor();
            mExtraPacketsButton = nullptr;
        }
        if (mExtraSeedsButton) {
            mApp->mBoard->RemoveWidget(mExtraSeedsButton);
            mExtraSeedsButton->_destructor();
            mExtraSeedsButton = nullptr;
        }
        if (mBanModeButton) {
            mApp->mBoard->RemoveWidget(mBanModeButton);
            mBanModeButton->_destructor();
            mBanModeButton = nullptr;
        }
        if (mBalancePatchButton) {
            mApp->mBoard->RemoveWidget(mBalancePatchButton);
            mBalancePatchButton->_destructor();
            mBalancePatchButton = nullptr;
        }
        if (mShuffleModeButton) {
            mApp->mBoard->RemoveWidget(mShuffleModeButton);
            mShuffleModeButton->_destructor();
            mShuffleModeButton = nullptr;
        }
    }

    gVSSetupAddonWidget = nullptr;
}

void VSSetupAddonWidget::Update() {
    Image *aCheckbox = *Sexy_IMAGE_OPTIONS_CHECKBOX0_Addr;
    Image *aCheckboxPressed = *Sexy_IMAGE_OPTIONS_CHECKBOX1_Addr;
    Image *a7PacketsImage = mExtraPacketsMode ? aCheckboxPressed : aCheckbox;
    Image *aExtraSeedsImage = mExtraSeedsMode ? aCheckboxPressed : aCheckbox;
    Image *aBanModeImage = mBanMode ? aCheckboxPressed : aCheckbox;
    Image *aBalancePatchImage = mBalancePatchMode ? aCheckboxPressed : aCheckbox;
    Image *aShuffleModeImage = mShuffleMode ? aCheckboxPressed : aCheckbox;
    if (mButtonImage[VS_SETUP_ADDON_BUTTON_EXTRA_PACKETS] != a7PacketsImage)
        mExtraPacketsButton->mButtonImage = a7PacketsImage;
    if (mButtonImage[VS_SETUP_ADDON_BUTTON_EXTRA_SEEDS] != aExtraSeedsImage)
        mExtraSeedsButton->mButtonImage = aExtraSeedsImage;
    if (mButtonImage[VS_SETUP_ADDON_BUTTON_BAN_MODE] != aBanModeImage)
        mBanModeButton->mButtonImage = aBanModeImage;
    if (mButtonImage[VS_SETUP_ADDON_BUTTON_BALANCE_PATCH] != aBalancePatchImage)
        mBalancePatchButton->mButtonImage = aBalancePatchImage;
    if (mButtonImage[VS_SETUP_ADDON_BUTTON_SHUFFLE_MODE] != aShuffleModeImage)
        mShuffleModeButton->mButtonImage = aShuffleModeImage;
}

void VSSetupAddonWidget::SetDisable() {
    mExtraPacketsButton->mBtnNoDraw = true;
    mExtraPacketsButton->mDisabled = true;
    mExtraSeedsButton->mBtnNoDraw = true;
    mExtraSeedsButton->mDisabled = true;
    mBanModeButton->mBtnNoDraw = true;
    mBanModeButton->mDisabled = true;
    mBalancePatchButton->mBtnNoDraw = true;
    mBalancePatchButton->mDisabled = true;
    mShuffleModeButton->mBtnNoDraw = true;
    mShuffleModeButton->mDisabled = true;
    mDrawString = false;
}

void VSSetupAddonWidget::ButtonDepress(this VSSetupAddonWidget &self, int theId) {
    if (theId == VSSetupAddonWidget_ExtraPackets) {
        self.CheckboxChecked(VSSetupAddonWidget_ExtraPackets, self.mExtraPacketsMode);
    }
    if (theId == VSSetupAddonWidget_ExtraSeeds) {
        self.CheckboxChecked(VSSetupAddonWidget_ExtraSeeds, self.mExtraSeedsMode);
    }
    if (theId == VSSetupAddonWidget_BanMode) {
        self.CheckboxChecked(VSSetupAddonWidget_BanMode, self.mBanMode);
    }
    if (theId == VSSetupAddonWidget_BalancePatch) {
        self.CheckboxChecked(VSSetupAddonWidget_BalancePatch, self.mBalancePatchMode);
    }
    if (theId == VSSetupAddonWidget_ShuffleMode) {
        self.CheckboxChecked(VSSetupAddonWidget_ShuffleMode, self.mShuffleMode);
    }
}

void VSSetupAddonWidget::CheckboxChecked(int theId, bool checked) {
    switch (theId) {
        case VSSetupAddonWidget_ExtraPackets:
            mExtraPacketsMode = !checked;
            mApp->mPlayerInfo->mVSExtraPacketsMode = mExtraPacketsMode;
            break;
        case VSSetupAddonWidget_ExtraSeeds:
            mExtraSeedsMode = !checked;
            mApp->mPlayerInfo->mVSExtraSeedsMode = mExtraSeedsMode;
            break;
        case VSSetupAddonWidget_BanMode:
            mBanMode = !checked;
            mApp->mPlayerInfo->mVSBanMode = mBanMode;
            break;
        case VSSetupAddonWidget_BalancePatch:
            mBalancePatchMode = !checked;
            mApp->mPlayerInfo->mVSBalancePatchMode = mBalancePatchMode;
            break;
        case VSSetupAddonWidget_ShuffleMode:
            mShuffleMode = !checked;
            mApp->mPlayerInfo->mVSShuffleMode = mShuffleMode;
            break;
        default:
            break;
    }

    mApp->mPlayerInfo->SaveDetails();
}

void PickMPRandomSeeds(LawnApp *theApp, std::vector<SeedType> &thePlantSeeds, std::vector<SeedType> &theZombieSeeds, bool theIsZombie) {
    thePlantSeeds.clear();
    theZombieSeeds.clear();

    int alreadyPicked = 0;
    if ((theApp->mPlayerInfo->mLevel > 20 || theApp->HasFinishedAdventure()) && Sexy::Rand(5) == 1) {
        thePlantSeeds.push_back(SEED_INSTANT_COFFEE);
        alreadyPicked = theIsZombie ? 0 : 1;
    }

    std::vector<SeedType> &aSeeds = theIsZombie ? theZombieSeeds : thePlantSeeds;
    const int poolGroupOffset = 3 * alreadyPicked;

    for (int num_possible = alreadyPicked; num_possible < theApp->mBoard->GetNumSeedsInBank(true) - 1; ++num_possible) {
        int pool = 0;
        if (num_possible == 2 || num_possible == 3)
            pool = 1;
        else if (num_possible == 4)
            pool = 2;

        const int poolBase = theIsZombie ? 6 + pool : poolGroupOffset + pool;

        int validCount = 0;
        for (int i = 0; i < 8; ++i) {
            const SeedType aSeedType = VSSetupMenu::msRandomPools[poolBase][i];
            if (aSeedType == SeedType::SEED_NONE)
                break;

            ++validCount;
        }

        SeedType aSeedType = SEED_NONE;
        for (;;) {
            do {
                const int idx = Sexy::Rand(validCount);
                aSeedType = VSSetupMenu::msRandomPools[poolBase][idx];
            } while (std::ranges::contains(aSeeds, aSeedType));

            if (theApp->HasSeedType(aSeedType, theIsZombie))
                break;
        }

        aSeeds.push_back(aSeedType);
    }
}

SeedType PickNextRandomSeed(LawnApp *theApp, std::vector<SeedType> &thePlantSeeds, std::vector<SeedType> &theZombieSeeds, bool theIsZombie, int theSeedIndex) {
    PickMPRandomSeeds(theApp, thePlantSeeds, theZombieSeeds, theIsZombie);
    SeedType aSeedType = theIsZombie ? theZombieSeeds[theSeedIndex - 1] : thePlantSeeds[theSeedIndex - 1];
    return aSeedType;
}