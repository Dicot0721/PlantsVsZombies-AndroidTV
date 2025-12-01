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

#include "PvZ/Lawn/Widget/SeedChooserPage.h"

SeedChooserPage::SeedChooserPage() {
    ;
}

SeedChooserPage::~SeedChooserPage() {
    mNextPageButton->mBtnNoDraw = true;
    mNextPageButton->mDisabled = true;
    gSeedChooserPage = nullptr;
}

int SeedChooserPage::GetPage() {
    return mPage;
}

void SeedChooserPage::PageUp() {
    if (mPage < NUM_SEED_CHOOSER_PAGE) {
        mPage++;
    } else {
        mPage = 0;
    }
}

void SeedChooserPage::ButtonDepress(this SeedChooserPage &self, int theId) {
    if (theId == SeedChooserPage_NextPage) {
        self.PageUp();
    }
}