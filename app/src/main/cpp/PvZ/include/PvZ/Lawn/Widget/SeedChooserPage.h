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

#ifndef PVZ_LAWN_WIDGET_SEED_CHOOSER_PAGE_H
#define PVZ_LAWN_WIDGET_SEED_CHOOSER_PAGE_H


#include "GameButton.h"
#include "PvZ/SexyAppFramework/Widget/ButtonListener.h"

class SeedChooserPage {
public:
    enum {
        NUM_SEED_CHOOSER_PAGE = 1,
        SeedChooserPage_NextPage = 107,
    };

    int mPage = 0;
    GameButton *mNextPageButton = nullptr;

    SeedChooserPage();
    ~SeedChooserPage();
    int GetPage();
    void PageUp();
    //    void PageDown();
    void ButtonDepress(this SeedChooserPage &self, int theId);

private:
    static constexpr Sexy::ButtonListener::VTable sButtonListenerVtable{
        .ButtonDepress = (void *)&SeedChooserPage::ButtonDepress,
    };
};

inline SeedChooserPage *gSeedChooserPage;


#endif // PVZ_LAWN_WIDGET_SEED_CHOOSER_PAGE_H
