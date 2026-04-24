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

#ifndef PVZ_LAWN_WIDGET_VSSETUPADDONWIDGET_H
#define PVZ_LAWN_WIDGET_VSSETUPADDONWIDGET_H

#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/SexyAppFramework/Widget/ButtonListener.h"
#include "PvZ/SexyAppFramework/Widget/CheckboxListener.h"
#include "PvZ/SexyAppFramework/Widget/Widget.h"

constexpr int VS_ADDON_BUTTON_X = 800;
constexpr int VS_BUTTON_EXTRA_PACKETS_Y = 200;
constexpr int VS_BUTTON_EXTRA_SEEDS_Y = 240;
constexpr int VS_BUTTON_BAN_MODE_Y = 280;
constexpr int VS_BUTTON_BALANCE_PATCH_Y = 320;

namespace Sexy {
class ButtonWidget;
class Widget;
} // namespace Sexy

class VSSetupAddonWidget : public Sexy::CheckboxListener {
public:
    enum {
        VSSetupAddonWidget_ExtraPackets = 12,
        VSSetupAddonWidget_ExtraSeeds,
        VSSetupAddonWidget_BanMode,
        VSSetupAddonWidget_BalancePatch,
        VSSetupAddonWidget_Back,
    };

    static inline bool msBalancePatchMode = false;

    LawnApp *mApp = *gLawnApp_Addr;
    Board *mBoard = mApp->mBoard;
    Sexy::ButtonListener *mButtonListener;
    NewLawnButton *mBackButton = nullptr;
    Sexy::Checkbox *mExtraPacketsCheckbox = nullptr;
    Sexy::Checkbox *mExtraSeedsCheckbox = nullptr;
    Sexy::Checkbox *mBanModeCheckbox = nullptr;
    Sexy::Checkbox *mBalancePatchCheckbox = nullptr;
    bool mExtraPacketsMode = false;
    bool mExtraSeedsMode = false;
    bool mBanMode = false;
    bool mBalancePatchMode = false;
    bool mDrawString = true;

    VSSetupAddonWidget(VSSetupMenu *theVSSetupMenu);
    ~VSSetupAddonWidget();
    void SetDisable(Sexy::Widget *theWidget);
    void ButtonDepress(this VSSetupAddonWidget &self, int theId);
    void CheckboxChecked(int theId, bool checked) override;
    void Draw(Sexy::Graphics *g);
    bool GetAddonMode(int theId) const;
    void SetAddonMode(int theId, bool checked, bool saveDetails);

private:
    static inline const Sexy::ButtonListener::VTable sButtonListenerVtable{
        .ButtonDepress = (void *)&VSSetupAddonWidget::ButtonDepress,
    };

    static inline Sexy::ButtonListener sButtonListener{&sButtonListenerVtable};
};

void PickMPRandomSeeds(LawnApp *theApp, std::vector<SeedType> &thePlantSeeds, std::vector<SeedType> &theZombieSeeds, bool theIsZombie);
void PickShuffleSeeds(LawnApp *theApp, std::vector<SeedType> &thePlantSeeds, std::vector<SeedType> &theZombieSeeds, bool theIsZombie);
SeedType PickNextRandomSeed(LawnApp *theApp, std::vector<SeedType> &thePlantSeeds, std::vector<SeedType> &theZombieSeeds, bool theIsZombie, int theSeedIndex);
bool NeedSeedInstantCoffee(LawnApp *theApp, const std::vector<SeedType> &thePlantSeeds = {}, bool theIsShuffle = false);
bool NeedSeedTallnut(LawnApp *theApp);
bool NeedSeedUmbrella(LawnApp *theApp);
bool NeedSeedMagnetshroom(LawnApp *theApp);
bool NeedSeedSplitPea(LawnApp *theApp);
bool IsPeaSeedType(SeedType theSeedType);
int CountPeasOnScreen(LawnApp *theApp);
bool NeedSeedTorchwood(LawnApp *theApp, const std::vector<SeedType> &thePlantSeeds = {}, bool theIsShuffle = false);
bool NeedSeedZombieImp(LawnApp *theApp);

#endif // PVZ_LAWN_WIDGET_VSSETUPADDONWIDGET_H
