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

#include "PvZ/Lawn/Widget/StoreScreen.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Misc.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/Symbols.h"

using namespace Sexy;

void StoreScreen::AddedToManager(int a2) {
    old_StoreScreen_AddedToManager(this, a2);
}

void StoreScreen::RemovedFromManager(int a2) {
    old_StoreScreen_RemovedFromManager(this, a2);
}

void StoreScreen::Update() {
    old_StoreScreen_Update(this);
}

void StoreScreen::SetupPage() {
    old_StoreScreen_SetupPage(this);
    // for (int i = 0; i < 8; ++i) {
    // a::a StoreItemType = StoreScreen_GetStoreItemType(a, i);
    // if (StoreScreen_IsPottedPlant(a, StoreItemType)) {
    // Sexy::Image *theImage = *((Sexy::Image **) a + i + 217);
    // Sexy::Graphics g;
    // Sexy_Graphics_Graphics2(&g, theImage);
    // Sexy_Graphics_ClearRect(&g, 0, 0, 100, 70);
    // Sexy_Graphics_Translate(&g, -10, -50);
    // DrawImage(&g, addonImages.seed_cached_52, 0, 0);
    // Sexy_Graphics_Delete2(&g);
    // }
    // }
}

void StoreScreen::DrawItem(Sexy::Graphics *g, int a3, StoreItem theStoreItem) {
    // if (StoreScreen_IsItemUnavailable(a1, item)) return;
    // if (StoreScreen_IsPottedPlant(a1, item)){
    // int theX = 0;
    // int theY = 0;
    // int theCount = 0;
    // Sexy::Image *theImage = nullptr;
    // StoreScreen_GetStoreItemInfo(a1, 0, item, &theImage, &theX, &theY, &theCount);
    // DrawImage(thePlayerIndex,addonImages.seed_cached_52,theX,theY);
    // }
    old_StoreScreen_DrawItem(this, g, a3, theStoreItem);
}

void StoreScreen::ButtonDepress(int theId) {
    // if (!showHouse) return old_StoreScreen_ButtonDepress(storeScreen, buttonId);
    StorePages newPageIndex;
    switch (theId) {
        case StoreScreen::StoreScreen_Back:
            mResult = 1000;
            break;
        case StoreScreen::StoreScreen_Prev:
            mHatchTimer = 50;
            unk197 = 1;
            mApp->PlaySample(*Sexy_SOUND_HATCHBACK_CLOSE_Addr);
            mBubbleCountDown = 0;
            mApp->CrazyDaveStopTalking();
            EnableButtons(false);
            do {
                newPageIndex = (StorePages)(mPage - 1);
                if (newPageIndex < StorePages::STORE_PAGE_SLOT_UPGRADES) {
                    newPageIndex = StorePages::NUM_STORE_PAGES;
                    mPage = StorePages::NUM_STORE_PAGES;
                } else if (newPageIndex == StorePages::STORE_PAGE_HOUSE) {
                    newPageIndex = StorePages::STORE_PAGE_ZEN2;
                    mPage = StorePages::STORE_PAGE_ZEN2;
                } else {
                    mPage = newPageIndex;
                }
            } while (!IsPageShown(newPageIndex));
            break;
        case StoreScreen::StoreScreen_Next:
            mHatchTimer = 50;
            unk197 = 2;
            mApp->PlaySample(*Sexy_SOUND_HATCHBACK_CLOSE_Addr);
            mBubbleCountDown = 0;
            mApp->CrazyDaveStopTalking();
            EnableButtons(false);
            do {
                newPageIndex = (StorePages)(mPage + 1);
                if (newPageIndex == 4) {
                    newPageIndex = StorePages::NUM_STORE_PAGES;
                    mPage = StorePages::NUM_STORE_PAGES;
                } else if (newPageIndex > 4) {
                    newPageIndex = StorePages::STORE_PAGE_SLOT_UPGRADES;
                    mPage = StorePages::STORE_PAGE_SLOT_UPGRADES;
                } else {
                    mPage = newPageIndex;
                }
            } while (!IsPageShown(newPageIndex));
            break;
    }
}

void StoreScreen::PurchaseItem(StoreItem item) {
    old_StoreScreen_PurchaseItem(this, item);

    DefaultPlayerInfo *aPlayerInfo = mApp->mPlayerInfo;

    // 检查植物全收集成就
    for (int i = StoreItem::STORE_ITEM_PLANT_GATLINGPEA; i <= StoreItem::STORE_ITEM_PLANT_IMITATER; ++i) {
        if (aPlayerInfo->mPurchases[i] == 0) {
            return;
        }
    }
    mApp->GrantAchievement(AchievementId::ACHIEVEMENT_SHOP);
}

void StoreScreen::Draw(Sexy::Graphics *g) {
    // 绘制商店页数字符串
    old_StoreScreen_Draw(this, g);

    int aNumPages = 0;
    if (mApp->HasFinishedAdventure() && showHouse) {
        aNumPages = StorePages::NUM_STORE_PAGES;
    } else {
        for (StorePages aPage = STORE_PAGE_SLOT_UPGRADES; aPage < STORE_PAGE_HOUSE; aPage = (StorePages)(aPage + 1)) {
            if (IsPageShown(aPage)) {
                aNumPages++;
            }
        }
    }

    if (aNumPages <= 1) {
        return;
    }

    int aPage = mPage == 5 ? 5 : mPage + 1;
    pvzstl::string aPageString = StrFormat("%d/%d", aPage, aNumPages);
    TodDrawString(g, aPageString, 410, 512, *Sexy_FONT_BRIANNETOD16_Addr, Color(200, 200, 200, 255), DrawStringJustification::DS_ALIGN_CENTER);
}

bool StoreScreen::IsPottedPlant(StoreItem theStoreItem) {
    return theStoreItem == STORE_ITEM_POTTED_MARIGOLD_1 || theStoreItem == STORE_ITEM_POTTED_MARIGOLD_2 || theStoreItem == STORE_ITEM_POTTED_MARIGOLD_3;
}


static StoreScreenTouchState gStoreScreenTouchState = StoreScreenTouchState::None;

void StoreScreen::MouseDown(int x, int y, int theClickCount) {
    if (mBubbleClickToContinue) {
        // 初次捡到戴夫车钥匙时会进入商店并且有一段戴夫对话，这里用于识别戴夫对话
        AdvanceCrazyDaveDialog();
        return;
    }
    if (!CanInteractWithButtons()) {
        // 翻页过程中无法触控
        return;
    }
    int mPrevButtonWidth = (*Sexy_IMAGE_STORE_PREVBUTTON_Addr)->GetWidth();
    int mPrevButtonHeight = (*Sexy_IMAGE_STORE_PREVBUTTON_Addr)->GetHeight();
    int mNextButtonWidth = (*Sexy_IMAGE_STORE_NEXTBUTTON_Addr)->GetWidth();
    int mNextButtonHeight = (*Sexy_IMAGE_STORE_NEXTBUTTON_Addr)->GetHeight();
    int mBackButtonWidth = (*Sexy_IMAGE_STORE_MAINMENUBUTTON_Addr)->GetWidth();
    int mBackButtonHeight = (*Sexy_IMAGE_STORE_MAINMENUBUTTON_Addr)->GetHeight();
    Sexy::Rect mPrevButtonRect = {mShakeX + 172, mShakeY + 375, mPrevButtonWidth, mPrevButtonHeight};
    Sexy::Rect mNextButtonRect = {mShakeX + 573, mShakeY + 373, mNextButtonWidth, mNextButtonHeight};
    Sexy::Rect mBackButtonRect = {mShakeX + 305, mShakeY + 510, mBackButtonWidth, mBackButtonHeight};

    if (TRect_Contains(&mBackButtonRect, x, y)) {
        gStoreScreenTouchState = StoreScreenTouchState::Back;
        return;
    }

    bool isPageShown = IsPageShown(StorePages::STORE_PAGE_PLANT_UPGRADES);
    if (isPageShown) {

        if (TRect_Contains(&mPrevButtonRect, x, y)) {
            gStoreScreenTouchState = StoreScreenTouchState::Prev;
            return;
        }

        if (TRect_Contains(&mNextButtonRect, x, y)) {
            gStoreScreenTouchState = StoreScreenTouchState::Next;
            return;
        }
    }

    // StoreScreen_PurchaseItem(storeScreen, a::STORE_ITEM_BLUEPRINT_CHANGE);

    for (int i = 0; i < 8; i++) {
        StoreItem storeItemType = GetStoreItemType(i);
        if (storeItemType != StoreItem::STORE_ITEM_INVALID) {
            int theX = 0;
            int theY = 0;
            int theCount = 0;
            Sexy::Image *theImage = nullptr;
            GetStoreItemInfo(i, storeItemType, theImage, theX, theY, theCount);
            int theImageWidth = 80;
            int theImageHeight = 80;
            if (theImage != nullptr) {
                theImageWidth = theImage->GetWidth();
                theImageHeight = theImage->GetHeight();
            }

            // LOGD("i:%d storeItemType:%d theX:%d theY:%d x:%d y:%d theImageWidth:%d theImageHeight:%d", i, storeItemType, theX, theY, x, y, theImageWidth,
            // theImageHeight);
            // int theImageWidth = 80;
            // int theImageHeight = 80;
            Sexy::Rect itemRect = {theX - theImageWidth / 2, theY - theImageHeight, theImageWidth, theImageHeight};
            if (TRect_Contains(&itemRect, x, y)) {
                if (mSelectedStoreItemType != storeItemType) {
                    SetSelectedSlot(i);
                } else {
                    if (IsItemSoldOut(storeItemType) || IsItemUnavailable(storeItemType) || IsComingSoon(storeItemType)) {
                        return;
                    }
                    PurchaseItem(storeItemType);
                }
            }
        }
    }
}

void StoreScreen::MouseUp(int x, int y, int theClickCount) {
    switch (gStoreScreenTouchState) {
        case StoreScreenTouchState::Back:
            ButtonDepress(StoreScreen::StoreScreen_Back);
            break;
        case StoreScreenTouchState::Prev:
            ButtonDepress(StoreScreen::StoreScreen_Prev);
            break;
        case StoreScreenTouchState::Next:
            ButtonDepress(StoreScreen::StoreScreen_Next);
            break;
        default:
            break;
    }
    gStoreScreenTouchState = StoreScreenTouchState::None;
}
