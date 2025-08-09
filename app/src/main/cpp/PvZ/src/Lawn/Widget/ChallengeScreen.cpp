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

#include "PvZ/Lawn/Widget/ChallengeScreen.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/Widget/GameButton.h"
#include "PvZ/Misc.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Common/TodStringFile.h"

using namespace Sexy;

static GameButton *gChallengeScreenCloseButton;

ChallengeScreen::ChallengeScreen(LawnApp *theApp, ChallengePage thePage) {
    __Constructor(theApp, thePage);
}

void ChallengeScreen::__Constructor(LawnApp *theApp, ChallengePage thePage) {
    // 去除按钮对触控的遮挡
    old_ChallengeScreen_ChallengeScreen(this, theApp, thePage);

    for (auto *button : mButtons) {
        // 把按钮全部缩小至长宽为0
        button->Resize(button->mX, button->mY, 0, 0);
    }

    //    if (thePage == CHALLENGE_PAGE_VS) {
    ////        SetUnlockChallengeIndex(thePage, false);
    //        mUnlockState = UNLOCK_SHAKING;
    //        mUnlockStateCounter = 100;
    //        mUnlockChallengeIndex = 0;
    //
    //        for (int aChallengeMode = 0; aChallengeMode < NUM_CHALLENGE_MODES; aChallengeMode++) {
    //            ChallengeDefinition& aDef = GetChallengeDefinition(aChallengeMode);
    //            if (aDef.mPage == thePage)
    //                mUnlockChallengeIndex = aChallengeMode;
    //        }
    //    }
}

namespace {
ChallengeDefinition gButteredPopcornDef = {GameMode::GAMEMODE_CHALLENGE_BUTTERED_POPCORN, 37, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 6, 1, "[BUTTERED_POPCORN]"};
[[maybe_unused]] ChallengeDefinition gPoolPartyDef = {GameMode::GAMEMODE_CHALLENGE_POOL_PARTY, 37, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 6, 2, "[POOL_PARTY]"};
[[maybe_unused]] ChallengeDefinition gVSDayDef = {GameMode::GAMEMODE_MP_VS_DAY, 37, ChallengePage::CHALLENGE_PAGE_VS, 6, 1, "[MP_VS_DAY]"};
[[maybe_unused]] ChallengeDefinition gVSNightDef = {GameMode::GAMEMODE_MP_VS_NIGHT, 37, ChallengePage::CHALLENGE_PAGE_VS, 6, 1, "[MP_VS_NIGHT]"};
[[maybe_unused]] ChallengeDefinition gVSPoolDayDef = {GameMode::GAMEMODE_MP_VS_POOL_DAY, 37, ChallengePage::CHALLENGE_PAGE_VS, 6, 1, "[MP_VS_POOL_DAY]"};
[[maybe_unused]] ChallengeDefinition gVSPoolNightDef = {GameMode::GAMEMODE_MP_VS_POOL_NIGHT, 37, ChallengePage::CHALLENGE_PAGE_VS, 6, 1, "[MP_VS_POOL_NIGHT]"};
[[maybe_unused]] ChallengeDefinition gVSRoofDef = {GameMode::GAMEMODE_MP_VS_ROOF, 37, ChallengePage::CHALLENGE_PAGE_VS, 6, 1, "[MP_VS_ROOF]"};
} // namespace

ChallengeDefinition &GetChallengeDefinition(int theChallengeMode) {
    if (theChallengeMode + 2 == GameMode::GAMEMODE_CHALLENGE_BUTTERED_POPCORN) {
        return gButteredPopcornDef;
    }

    // if (theChallengeMode + 2 == GameMode::GAMEMODE_CHALLENGE_POOL_PARTY) {
    //     return gPoolPartyDef;
    // }

    //    if (theChallengeMode + 4 == GameMode::GAMEMODE_MP_VS) {
    //        return gVSDayDef;
    //    } else if (theChallengeMode + 3 == GameMode::GAMEMODE_MP_VS) {
    //        return gVSNightDef;
    //    } else if (theChallengeMode + 2 == GameMode::GAMEMODE_MP_VS) {
    //        return gVSPoolDayDef;
    //    } else if (theChallengeMode + 1 == GameMode::GAMEMODE_MP_VS) {
    //        return gVSPoolNightDef;
    //    } else if (theChallengeMode == GameMode::GAMEMODE_MP_VS) {
    //        return gVSRoofDef;
    //    }

    return old_GetChallengeDefinition(theChallengeMode);
}

void ChallengeScreen::Draw(Sexy::Graphics *graphics) {
    // 修复小游戏界面显示奖杯数不正确
    old_ChallengeScreen_Draw(this, graphics);

    int mTotalTrophiesInPage = 0;
    switch (mPageIndex) {
        case ChallengePage::CHALLENGE_PAGE_SURVIVAL:
            mTotalTrophiesInPage = 10;
            break;
        case ChallengePage::CHALLENGE_PAGE_CHALLENGE:
            for (int i = 0; i < 94; ++i) {
                if (GetChallengeDefinition(i).mPage == ChallengePage::CHALLENGE_PAGE_CHALLENGE) {
                    mTotalTrophiesInPage++;
                }
            }
            break;
        case ChallengePage::CHALLENGE_PAGE_COOP:
            mTotalTrophiesInPage = 10;
            break;
        case ChallengePage::CHALLENGE_PAGE_PUZZLE:
            mTotalTrophiesInPage = 18;
            break;
        default:
            break;
    }

    pvzstl::string str = StrFormat("%d/%d", mApp->GetNumTrophies(mPageIndex), mTotalTrophiesInPage);
    Color theColor = {255, 240, 0, 255};
    TodDrawString(graphics, str, 711, 62, *Sexy_FONT_BRIANNETOD16_Addr, theColor, DrawStringJustification::DS_ALIGN_CENTER);
}

void ChallengeScreen::AddedToManager(int *theWidgetManager) {
    // 记录当前游戏状态
    pvzstl::string str = TodStringTranslate("[CLOSE]");
    gChallengeScreenCloseButton = MakeButton(1000, &mButtonListener, this, str);
    gChallengeScreenCloseButton->Resize(800, 520, 170, 50);
    AddWidget(gChallengeScreenCloseButton);

    return old_ChallengeScreen_AddedToManager(this, theWidgetManager);
}

void ChallengeScreen::Update() {
    // 记录当前游戏状态
    return old_ChallengeScreen_Update(this);
}

void ChallengeScreen::RemovedFromManager(int *theWidgetManager) {
    // 记录当前游戏状态
    old_ChallengeScreen_RemovedFromManager(this, theWidgetManager);

    if (gChallengeScreenCloseButton != nullptr) {
        RemoveWidget(gChallengeScreenCloseButton);
    }
}

void ChallengeScreen::__Destructor2() {
    // 删除按钮
    old_ChallengeScreen_Delete2(this);

    if (gChallengeScreenCloseButton != nullptr) {
        gChallengeScreenCloseButton->~GameButton();
        gChallengeScreenCloseButton = nullptr;
    }
}

void ChallengeScreen::ButtonPress(int theButtonId) {
    // 空函数替换，去除原有的点击进入关卡的功能
}

void ChallengeScreen::ButtonDepress(int theId) {
    // 去除原有的点击进入关卡的功能
    if (theId == 1000) {
        mApp->KillChallengeScreen();
        mApp->DoBackToMain();
    }
}

void ChallengeScreen::UpdateButtons() {
    // 空函数替换，去除默认选取第一个游戏的功能
}

namespace {
int gChallengeScreenTouchDownX;
int gChallengeScreenTouchDownY;
int gChallengeItemHeight;
int gChallengeScreenGameIndex;
bool gChallengeItemMoved;
bool gTouchOutSide;

constexpr int mPageTop = 75;
constexpr int mPageBottom = 555;
} // namespace

void ChallengeScreen::MouseDown(int x, int y, int theClickCount) {
    if (y > mPageBottom || y < mPageTop) {
        gTouchOutSide = true;
    }
    gChallengeScreenTouchDownX = x;
    gChallengeScreenTouchDownY = y;
    gChallengeItemHeight = (*Sexy_IMAGE_CHALLENGE_NAME_BACK_Addr)->GetHeight() + 2; // 2为缝隙大小

    gChallengeScreenGameIndex = mScreenTopChallengeIndex;

    //    int totalGamesInThisPage = a[376];//如果这个值是33
    //    int currentSelectedGameIndex = ChallengeScreen_GetCurrentSelectedGameIndex(
    //            a);//这里取值就是0~32。种子雨是32。

    //    int firstGameInPageIndex = a->mScreenTopChallengeIndex;
    //    int firstGameInPageIndex2 = a[186];
    //    a->mSelectedMode = a[currentSelectedGameIndex + 1 + 188];//向下移动绿色光标，不可循环滚动
    //    a->mSelectedMode = a[currentSelectedGameIndex - 1 + 188];//向上移动绿色光标，不可循环滚动

    //    LOGD("dOWN:%d %d %d %d", x, y, firstGameInPageIndex, firstGameInPageIndex2);
}

void ChallengeScreen::MouseDrag(int x, int y) {
    if (gTouchOutSide)
        return;
    int triggerHeight = gChallengeItemHeight / 2; // 调节此处以修改小游戏列表的滚动速度。滚动太快就会有BUG，好烦。
    if (gChallengeScreenTouchDownY - y > triggerHeight) {
        int totalGamesInThisPage = mTotalGameInPage;
        gChallengeScreenGameIndex += 1;
        gChallengeScreenTouchDownY -= triggerHeight;
        int gameIndexToScroll = gChallengeScreenGameIndex >= totalGamesInThisPage - 4 ? totalGamesInThisPage - 4 : gChallengeScreenGameIndex;
        SetScrollTarget(gameIndexToScroll);
        //        ChallengeScreen_UpdateButtons(a);
        gChallengeItemMoved = true;
    } else if (y - gChallengeScreenTouchDownY > triggerHeight) {
        gChallengeScreenGameIndex -= 1;
        gChallengeScreenTouchDownY += triggerHeight;
        int gameIndexToScroll = gChallengeScreenGameIndex <= 0 ? 0 : gChallengeScreenGameIndex;
        SetScrollTarget(gameIndexToScroll);
        //        ChallengeScreen_UpdateButtons(a);
        gChallengeItemMoved = true;
    }
}

void ChallengeScreen::MouseUp(int x, int y) {
    if (!gTouchOutSide && !gChallengeItemMoved) {
        int gameIndex = mScreenTopChallengeIndex + (y - mPageTop) / gChallengeItemHeight;
        if (mSelectedMode == mUnk1[gameIndex]) {
            KeyDown(Sexy::KEYCODE_ACCEPT);
        } else {
            LawnApp_PlaySample(mApp, *Sexy_SOUND_BUTTONCLICK_Addr);
            mSelectedMode = mUnk1[gameIndex];
        }
    }
    gTouchOutSide = false;
    gChallengeItemMoved = false;
}
