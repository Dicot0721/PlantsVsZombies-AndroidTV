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
    mExtraPacketsMode = mApp->mPlayerInfo->mVS7PacketsMode;
    mExtraSeedsMode = mApp->mPlayerInfo->mVSExtraSeedsMode;
    mBanMode = mApp->mPlayerInfo->mVSBanMode;
    mBalancePatchMode = mApp->mPlayerInfo->mVSBalancePatchMode;
    Image *aCheckbox = *Sexy_IMAGE_OPTIONS_CHECKBOX0_Addr;
    Image *aCheckboxPressed = *Sexy_IMAGE_OPTIONS_CHECKBOX1_Addr;
    mExtraPacketsButton = MakeNewButton(VSSetupAddonWidget_ExtraPackets, mButtonListener, theVSSetupMenu, "", nullptr, aCheckbox, aCheckboxPressed, aCheckboxPressed);
    mExtraSeedsButton = MakeNewButton(VSSetupAddonWidget_ExtraSeeds, mButtonListener, theVSSetupMenu, "", nullptr, aCheckbox, aCheckboxPressed, aCheckboxPressed);
    mBanModeButton = MakeNewButton(VSSetupAddonWidget_BanMode, mButtonListener, theVSSetupMenu, "", nullptr, aCheckbox, aCheckboxPressed, aCheckboxPressed);
    mBalancePatchButton = MakeNewButton(VSSetupAddonWidget_BalancePatch, mButtonListener, theVSSetupMenu, "", nullptr, aCheckbox, aCheckboxPressed, aCheckboxPressed);
    mExtraPacketsButton->Resize(VS_ADDON_BUTTON_X, VS_BUTTON_EXTRA_PACKETS_Y, 175, 50);
    mExtraSeedsButton->Resize(VS_ADDON_BUTTON_X, VS_BUTTON_EXTRA_SEEDS_Y, 175, 50);
    mBanModeButton->Resize(VS_ADDON_BUTTON_X, VS_BUTTON_BAN_MODE_Y, 175, 50);
    mBalancePatchButton->Resize(VS_ADDON_BUTTON_X, VS_BUTTON_BALANCE_PATCH_Y, 175, 50);
    mApp->mBoard->AddWidget(mExtraPacketsButton);
    mApp->mBoard->AddWidget(mExtraSeedsButton);
    mApp->mBoard->AddWidget(mBanModeButton);
    mApp->mBoard->AddWidget(mBalancePatchButton);
}

VSSetupAddonWidget::~VSSetupAddonWidget() {
    SetDisable();
    gVSSetupAddonWidget = nullptr;
}

void VSSetupAddonWidget::Update() {
    Image *aCheckbox = *Sexy_IMAGE_OPTIONS_CHECKBOX0_Addr;
    Image *aCheckboxPressed = *Sexy_IMAGE_OPTIONS_CHECKBOX1_Addr;
    Image *a7PacketsImage = mExtraPacketsMode ? aCheckboxPressed : aCheckbox;
    Image *aExtraSeedsImage = mExtraSeedsMode ? aCheckboxPressed : aCheckbox;
    Image *aBanModeImage = mBanMode ? aCheckboxPressed : aCheckbox;
    Image *aBalancePatchImage = mBalancePatchMode ? aCheckboxPressed : aCheckbox;
    if (mButtonImage[VS_SETUP_ADDON_BUTTON_EXTRA_PACKETS] != a7PacketsImage)
        mExtraPacketsButton->mButtonImage = a7PacketsImage;
    if (mButtonImage[VS_SETUP_ADDON_BUTTON_EXTRA_SEEDS] != aExtraSeedsImage)
        mExtraSeedsButton->mButtonImage = aExtraSeedsImage;
    if (mButtonImage[VS_SETUP_ADDON_BUTTON_BAN] != aBanModeImage)
        mBanModeButton->mButtonImage = aBanModeImage;
    if (mButtonImage[VS_SETUP_ADDON_BUTTON_BALANCE_PATCH] != aBalancePatchImage)
        mBalancePatchButton->mButtonImage = aBalancePatchImage;
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
}

void VSSetupAddonWidget::CheckboxChecked(int theId, bool checked) {
    switch (theId) {
        case VSSetupAddonWidget_ExtraPackets:
            mExtraPacketsMode = !checked;
            mApp->mPlayerInfo->mVS7PacketsMode = mExtraPacketsMode;
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
        default:
            break;
    }

    mApp->mPlayerInfo->SaveDetails();
}