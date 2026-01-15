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

#ifndef PVZ_LAWN_WIDGET_VSSETUPADDONWIDGET_H
#define PVZ_LAWN_WIDGET_VSSETUPADDONWIDGET_H

#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/SexyAppFramework/Widget/ButtonListener.h"
#include "PvZ/SexyAppFramework/Widget/Widget.h"

constexpr int VS_BUTTON_MORE_PACKETS_X = 800;
constexpr int VS_BUTTON_MORE_PACKETS_Y = 200;
constexpr int VS_BUTTON_BAN_MODE_X = 800;
constexpr int VS_BUTTON_BAN_MODE_Y = 240;
constexpr int NUM_VS_BAN_PACKETS = 4;

enum VSSetupAddonButton {
    VS_SETUP_ADDON_BUTTON_7PACKETS,
    VS_SETUP_ADDON_BUTTON_BAN,
    NUM_VS_SETUP_ADDON_BUTTON,
};

class BannedSeed {
public:
    int mX = 0;
    int mY = 0;
    int mChosenPlayerIndex = 0;
    SeedType mSeedType = SEED_NONE;
    BannedSeedState mSeedState = SEED_NOT_BANNED;
};

namespace Sexy {
class ButtonWidget;
}

class VSSetupAddonWidget {
public:
    enum {
        VSSetupAddonWidget_7Packets_Mode = 12,
        VSSetupAddonWidget_Ban_Mode = 13,
    };

public:
    LawnApp *mApp = *gLawnApp_Addr;
    Sexy::ButtonListener *mButtonListener;
    Sexy::ButtonWidget *m7PacketsModeButton = nullptr;
    Sexy::ButtonWidget *mBanModeButton = nullptr;
    Sexy::Image *mButtonImage[NUM_VS_SETUP_ADDON_BUTTON] = {nullptr};
    Sexy::Image *mButtonImageDown[NUM_VS_SETUP_ADDON_BUTTON] = {nullptr};
    bool m7PacketsMode = false;
    bool mBanMode = false;
    int mNumBanPackets = NUM_VS_BAN_PACKETS;
    int mSeedsInBothBanned = 0;
    BannedSeed mBannedSeed[NUM_ZOMBIE_SEED_TYPES];
    bool mDrawString = true;

    VSSetupAddonWidget(VSSetupMenu *theVSSetupMenu);
    ~VSSetupAddonWidget();
    void Update();
    void SetDisable();
    void SwapButtonImage(Sexy::ButtonWidget *theButton, int theIndex);
    void ButtonDepress(this VSSetupAddonWidget &self, int theId);
    void CheckboxChecked(int theId, bool checked);

private:
    static constexpr Sexy::ButtonListener::VTable sButtonListenerVtable{
        .ButtonDepress = (void *)&VSSetupAddonWidget::ButtonDepress,
    };

    static inline Sexy::ButtonListener sButtonListener{&sButtonListenerVtable};
};

inline Sexy::ButtonWidget *gVSSelectBgDayButton;
inline Sexy::ButtonWidget *gVSSelectBgNightButton;
inline Sexy::ButtonWidget *gVSSelectBgPoolButton;
inline Sexy::ButtonWidget *gVSSelectBgPoolNightButton;
inline Sexy::ButtonWidget *gVSSelectBgRoofButton;
inline Sexy::ButtonWidget *gVSSelectBgRoofNightButton;


#endif // PVZ_LAWN_WIDGET_VSSETUPADDONWIDGET_H
