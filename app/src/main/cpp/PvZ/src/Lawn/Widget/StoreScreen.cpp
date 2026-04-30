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

#include "PvZ/Lawn/Widget/StoreScreen.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/Symbols.h"

using namespace Sexy;

namespace {
enum class StoreScreenTouchState { Prev, Next, Back, None };
}

void StoreScreen::AddedToManager(WidgetManager *theWidgetManager) {
    old_StoreScreen_AddedToManager(this, theWidgetManager);
}

void StoreScreen::RemovedFromManager(WidgetManager *theWidgetManager) {
    old_StoreScreen_RemovedFromManager(this, theWidgetManager);
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

bool StoreScreen::IsPageShown(StorePages thePage) {
    if constexpr (/* mApp->IsTrialStageLocked() */ false) {
        return thePage == STORE_PAGE_SLOT_UPGRADES;
    }
    // 一周目完成后，所有页全解锁
    bool hasFinishedAdventure = mApp->HasFinishedAdventure();
    switch (thePage) {
        case STORE_PAGE_PLANT_UPGRADES: // 到达或已通过冒险模式 5-2 关卡时，显示紫卡页
            return hasFinishedAdventure || mApp->mPlayerInfo->mLevel >= 42;
        case STORE_PAGE_ZEN1: // 到达或已通过冒险模式 5-5 关卡时，显示花园工具页
            return hasFinishedAdventure || mApp->mPlayerInfo->mLevel >= 45;
        case STORE_PAGE_ZEN2: // 冒险模式未完成时，不显示智慧树工具页
            return hasFinishedAdventure;
        case STORE_PAGE_HOUSE: // TV 原版不显示房子页
            return hasFinishedAdventure && showHouse;
        default:
            return true;
    }
}

void StoreScreen::ButtonDepress(int theId) {
    // if (!showHouse) return old_StoreScreen_ButtonDepress(storeScreen, buttonId);
    switch (theId) {
        case StoreScreen::StoreScreen_Back:
            mResult = 1000;
            break;
        case StoreScreen::StoreScreen_Prev: {
            mHatchTimer = 50;
            unk304 = 1;
            mApp->PlaySample(Sexy::SOUND_HATCHBACK_CLOSE);
            mBubbleCountDown = 0;
            mApp->CrazyDaveStopTalking();
            EnableButtons(false);
            do {
                mPage = StorePages(mPage - 1);
                if (mPage == NUM_STORE_PAGES) {
                    mPage = StorePages(mPage - 1);
                } else if (mPage < STORE_PAGE_SLOT_UPGRADES) {
                    mPage = STORE_PAGE_HOUSE;
                }
            } while (!IsPageShown(mPage));
        } break;
        case StoreScreen::StoreScreen_Next: {
            mHatchTimer = 50;
            unk304 = 2;
            mApp->PlaySample(Sexy::SOUND_HATCHBACK_CLOSE);
            mBubbleCountDown = 0;
            mApp->CrazyDaveStopTalking();
            EnableButtons(false);
            do {
                mPage = StorePages(mPage + 1);
                if (mPage == NUM_STORE_PAGES) {
                    mPage = StorePages(mPage + 1);
                } else if (mPage > STORE_PAGE_HOUSE) {
                    mPage = STORE_PAGE_SLOT_UPGRADES;
                }
            } while (!IsPageShown(mPage));
        } break;
        default:
            break;
    }
}

void StoreScreen::PurchaseItem(StoreItem theStoreItem) {
    old_StoreScreen_PurchaseItem(this, theStoreItem);

    DefaultPlayerInfo *aPlayerInfo = mApp->mPlayerInfo;

    // 检查植物全收集成就
    for (int i = StoreItem::STORE_ITEM_PLANT_GATLINGPEA; i <= StoreItem::STORE_ITEM_PLANT_IMITATER; ++i) {
        if (aPlayerInfo->mPurchases[i] == 0) {
            return;
        }
    }
    mApp->GrantAchievement(AchievementType::ACHIEVEMENT_MORTICULTURALIST);
}

void StoreScreen::Draw(Sexy::Graphics *g) {
    old_StoreScreen_Draw(this, g);

    // 绘制商店页数字符串
    int aNumPages = 0;
    for (StorePages aPage = STORE_PAGE_SLOT_UPGRADES; aPage <= STORE_PAGE_HOUSE; aPage = StorePages(aPage + 1)) {
        if (aPage == NUM_STORE_PAGES) {
            continue;
        }
        if (IsPageShown(aPage)) {
            ++aNumPages;
        }
    }
    if (aNumPages <= 1) {
        return;
    }
    int aPage = (mPage < NUM_STORE_PAGES) ? mPage + 1 : mPage;
    pvzstl::string aPageString = StrFormat("%d/%d", aPage, aNumPages);
    TodDrawString(g, aPageString, 410, 512, Sexy::FONT_BRIANNETOD16, Color(200, 200, 200, 255), DrawStringJustification::DS_ALIGN_CENTER);
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
    int mPrevButtonWidth = (Sexy::IMAGE_STORE_PREVBUTTON)->GetWidth();
    int mPrevButtonHeight = (Sexy::IMAGE_STORE_PREVBUTTON)->GetHeight();
    int mNextButtonWidth = (Sexy::IMAGE_STORE_NEXTBUTTON)->GetWidth();
    int mNextButtonHeight = (Sexy::IMAGE_STORE_NEXTBUTTON)->GetHeight();
    int mBackButtonWidth = (Sexy::IMAGE_STORE_MAINMENUBUTTON)->GetWidth();
    int mBackButtonHeight = (Sexy::IMAGE_STORE_MAINMENUBUTTON)->GetHeight();
    Sexy::Rect mPrevButtonRect = {mShakeX + 172, mShakeY + 375, mPrevButtonWidth, mPrevButtonHeight};
    Sexy::Rect mNextButtonRect = {mShakeX + 573, mShakeY + 373, mNextButtonWidth, mNextButtonHeight};
    Sexy::Rect mBackButtonRect = {mShakeX + 305, mShakeY + 510, mBackButtonWidth, mBackButtonHeight};

    if (mBackButtonRect.Contains(x, y)) {
        gStoreScreenTouchState = StoreScreenTouchState::Back;
        return;
    }

    if (IsPageShown(StorePages::STORE_PAGE_PLANT_UPGRADES)) {
        if (mPrevButtonRect.Contains(x, y)) {
            gStoreScreenTouchState = StoreScreenTouchState::Prev;
            return;
        }
        if (mNextButtonRect.Contains(x, y)) {
            gStoreScreenTouchState = StoreScreenTouchState::Next;
            return;
        }
    }

    // StoreScreen_PurchaseItem(storeScreen, a::STORE_ITEM_BLUEPRINT_CHANGE);

    for (int aItemPos = 0; aItemPos < MAX_PAGE_SPOTS; ++aItemPos) {
        StoreItem aItemType = GetStoreItemType(aItemPos);
        if (aItemType == StoreItem::STORE_ITEM_INVALID) {
            continue;
        }
        Sexy::Image *aImage = nullptr;
        int aItemX = 0, aItemY = 0;
        int aCount = 0;
        GetStoreItemInfo(aItemPos, aItemType, aImage, aItemX, aItemY, aCount);
        int aWidth = 80;
        int aHeight = 80;
        if (aImage != nullptr) {
            aWidth = aImage->GetWidth();
            aHeight = aImage->GetHeight();
        }
        if (Rect{aItemX - aWidth / 2, aItemY - aHeight, aWidth, aHeight}.Contains(x, y)) {
            if (mSelectedStoreItemType != aItemType) {
                SetSelectedSlot(aItemPos);
            } else if (!IsItemSoldOut(aItemType) && !IsItemUnavailable(aItemType) && !IsComingSoon(aItemType)) {
                PurchaseItem(aItemType);
            }
            break;
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
