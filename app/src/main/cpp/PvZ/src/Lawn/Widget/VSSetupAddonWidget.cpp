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

#include "PvZ/Lawn/Widget/VSSetupAddonWidget.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/Widget/VSSetupMenu.h"

using namespace Sexy;

VSSetupAddonWidget::VSSetupAddonWidget(VSSetupMenu *theVSSetupMenu) {
    mButtonListener = &theVSSetupMenu->mButtonListener;
    m7PacketsMode = mApp->mPlayerInfo->mVS7PacketsMode;
    mBanMode = mApp->mPlayerInfo->mVSBanMode;
    Image *aCheckbox = *Sexy_IMAGE_OPTIONS_CHECKBOX0_Addr;
    Image *aCheckboxPressed = *Sexy_IMAGE_OPTIONS_CHECKBOX1_Addr;
    m7PacketsModeButton = MakeNewButton(VSSetupAddonWidget_7Packets_Mode, mButtonListener, theVSSetupMenu, "", nullptr, aCheckbox, aCheckboxPressed, aCheckboxPressed);
    mBanModeButton = MakeNewButton(VSSetupAddonWidget_Ban_Mode, mButtonListener, theVSSetupMenu, "", nullptr, aCheckbox, aCheckboxPressed, aCheckboxPressed);
    m7PacketsModeButton->Resize(VS_BUTTON_MORE_PACKETS_X, VS_BUTTON_MORE_PACKETS_Y, 175, 50);
    mBanModeButton->Resize(VS_BUTTON_BAN_MODE_X, VS_BUTTON_BAN_MODE_Y, 175, 50);
    mApp->mBoard->AddWidget(m7PacketsModeButton);
    mApp->mBoard->AddWidget(mBanModeButton);
}

VSSetupAddonWidget::~VSSetupAddonWidget() {
    m7PacketsModeButton->mBtnNoDraw = true;
    m7PacketsModeButton->mDisabled = true;
    mBanModeButton->mBtnNoDraw = true;
    mBanModeButton->mDisabled = true;
    mDrawString = false;
    gVSSetupAddonWidget = nullptr;
}

void VSSetupAddonWidget::Update() {
    Image *aCheckbox = *Sexy_IMAGE_OPTIONS_CHECKBOX0_Addr;
    Image *aCheckboxPressed = *Sexy_IMAGE_OPTIONS_CHECKBOX1_Addr;
    Image *a7PacketsImage = m7PacketsMode ? aCheckboxPressed : aCheckbox;
    Image *aBanImage = mBanMode ? aCheckboxPressed : aCheckbox;
    if (mButtonImage[VS_SETUP_ADDON_BUTTON_7PACKETS] != a7PacketsImage)
        m7PacketsModeButton->mButtonImage = a7PacketsImage;
    if (mButtonImage[VS_SETUP_ADDON_BUTTON_BAN] != aBanImage)
        mBanModeButton->mButtonImage = aBanImage;
}

void VSSetupAddonWidget::SetDisable() {
    m7PacketsModeButton->mBtnNoDraw = true;
    m7PacketsModeButton->mDisabled = true;
    mBanModeButton->mBtnNoDraw = true;
    mBanModeButton->mDisabled = true;
    mDrawString = false;
}

void VSSetupAddonWidget::ButtonDepress(this VSSetupAddonWidget &self, int theId) {
    if (theId == VSSetupAddonWidget_7Packets_Mode) {
        self.CheckboxChecked(VSSetupAddonWidget_7Packets_Mode, self.m7PacketsMode);
    }
    if (theId == VSSetupAddonWidget_Ban_Mode) {
        self.CheckboxChecked(VSSetupAddonWidget_Ban_Mode, self.mBanMode);
    }
}

void VSSetupAddonWidget::CheckboxChecked(int theId, bool checked) {
    switch (theId) {
        case VSSetupAddonWidget_7Packets_Mode:
            m7PacketsMode = !checked;
            mApp->mPlayerInfo->mVS7PacketsMode = m7PacketsMode;
            break;
        case VSSetupAddonWidget_Ban_Mode:
            mBanMode = !checked;
            mApp->mPlayerInfo->mVSBanMode = mBanMode;
            break;
        default:
            break;
    }

    mApp->mPlayerInfo->SaveDetails();
}