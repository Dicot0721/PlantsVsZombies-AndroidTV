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

#include "PvZ/Lawn/Widget/ChallengeScreen.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/Widget/GameButton.h"
#include "PvZ/Lawn/Widget/VSSetupMenu.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Common/TodCommon.h"
#include "PvZ/TodLib/Common/TodStringFile.h"

using namespace Sexy;

static GameButton *gChallengeScreenCloseButton;

ChallengeScreen::ChallengeScreen(LawnApp *theApp, ChallengePage thePage) {
    _constructor(theApp, thePage);
}

void ChallengeScreen::_constructor(LawnApp *theApp, ChallengePage thePage) {
    // 去除按钮对触控的遮挡
    old_ChallengeScreen_ChallengeScreen(this, theApp, thePage);

    for (auto *button : mButtons) {
        // 把按钮全部缩小至长宽为0
        button->Resize(button->mX, button->mY, 0, 0);
    }

    if (mPageIndex == ChallengePage::CHALLENGE_PAGE_VS) {
        mTotalGameInPage = 5;
    }
}

namespace {
ChallengeDefinition gButteredPopcornDef = {GameMode::GAMEMODE_CHALLENGE_BUTTERED_POPCORN, 37, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 6, 1, "[BUTTERED_POPCORN]"};
[[maybe_unused]] ChallengeDefinition gPoolPartyDef = {GameMode::GAMEMODE_CHALLENGE_POOL_PARTY, 37, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 6, 2, "[POOL_PARTY]"};
ChallengeDefinition gVSDayDef = {GameMode::GAMEMODE_MP_VS, 0, ChallengePage::CHALLENGE_PAGE_VS, 6, 1, "[MP_VS_DAY]"};
ChallengeDefinition gVSNightDef = {GameMode::GAMEMODE_MP_VS, 1, ChallengePage::CHALLENGE_PAGE_VS, 6, 1, "[MP_VS_NIGHT]"};
ChallengeDefinition gVSPoolDayDef = {GameMode::GAMEMODE_MP_VS, 2, ChallengePage::CHALLENGE_PAGE_VS, 6, 1, "[MP_VS_POOL_DAY]"};
ChallengeDefinition gVSPoolNightDef = {GameMode::GAMEMODE_MP_VS, 3, ChallengePage::CHALLENGE_PAGE_VS, 6, 1, "[MP_VS_POOL_NIGHT]"};
ChallengeDefinition gVSRoofDef = {GameMode::GAMEMODE_MP_VS, 4, ChallengePage::CHALLENGE_PAGE_VS, 6, 1, "[MP_VS_ROOF]"};
} // namespace

ChallengeDefinition &GetChallengeDefinition(int theChallengeMode) {
    if (theChallengeMode + 2 == GameMode::GAMEMODE_CHALLENGE_BUTTERED_POPCORN) {
        return gButteredPopcornDef;
    }

    // if (theChallengeMode + 2 == GameMode::GAMEMODE_CHALLENGE_POOL_PARTY) {
    // return gPoolPartyDef;
    // }

    if (theChallengeMode + 7 == GameMode::GAMEMODE_MP_VS) {
        gVSBackground = BackgroundType::BACKGROUND_1_DAY;
        return gVSDayDef;
    } else if (theChallengeMode + 6 == GameMode::GAMEMODE_MP_VS) {
        gVSBackground = BackgroundType::BACKGROUND_2_NIGHT;
        return gVSNightDef;
    } else if (theChallengeMode + 5 == GameMode::GAMEMODE_MP_VS) {
        gVSBackground = BackgroundType::BACKGROUND_3_POOL;
        return gVSPoolDayDef;
    } else if (theChallengeMode + 4 == GameMode::GAMEMODE_MP_VS) {
        gVSBackground = BackgroundType::BACKGROUND_4_FOG;
        return gVSPoolNightDef;
    } else if (theChallengeMode + 3 == GameMode::GAMEMODE_MP_VS) {
        gVSBackground = BackgroundType::BACKGROUND_5_ROOF;
        return gVSRoofDef;
    }

    return old_GetChallengeDefinition(theChallengeMode);
}

void ChallengeScreen::Draw(Sexy::Graphics *g) {
    g->DrawImage(*Sexy::IMAGE_CHALLENGE_BACKGROUND, *LawnApp_FULLSCREEN_RECT_Addr, -60);

    pvzstl::string aTitleString = mPageIndex == CHALLENGE_PAGE_SURVIVAL ? "[PICK_AREA]"
        : mPageIndex == CHALLENGE_PAGE_PUZZLE                           ? "[SCARY_POTTER]"
        : mPageIndex == CHALLENGE_PAGE_VS                               ? "[VS_MODE]"
        : mPageIndex == CHALLENGE_PAGE_COOP                             ? "[XBOX_COOP]"
                                                                        : "[PICK_CHALLENGE]";
    TodDrawString(g, aTitleString, 400, 45, *Sexy_FONT_HOUSEOFTERROR28_Addr, Color(220, 220, 220), DS_ALIGN_CENTER);

    int aTrophiesGot = mApp->GetNumTrophies(mPageIndex);
    int aTrophiesTotal = (mPageIndex == CHALLENGE_PAGE_SURVIVAL || mPageIndex == CHALLENGE_PAGE_COOP) ? 10 : mPageIndex == CHALLENGE_PAGE_PUZZLE ? 18 : 0;
    if (mPageIndex == CHALLENGE_PAGE_CHALLENGE) {
        for (int i = 0; i < 94; ++i) {
            if (GetChallengeDefinition(i).mPage == ChallengePage::CHALLENGE_PAGE_CHALLENGE) {
                aTrophiesTotal++;
            }
        }
    }
    if (aTrophiesTotal > 0) {
        pvzstl::string aTrophyString = StrFormat(TodStringTranslate("[NUMBER_OF_TROPHIES]").c_str(), aTrophiesGot, aTrophiesTotal);
        TodDrawString(g, aTrophyString, 711, 62, *Sexy_FONT_BRIANNETOD16_Addr, Color(255, 240, 0), DS_ALIGN_CENTER);
    }
    if (mPageIndex != CHALLENGE_PAGE_VS) {
        TodDrawImageScaledF(g, *Sexy::IMAGE_TROPHY, 690.0f, 15.0f, 0.5f, 0.5f);
    }

    g->PushState();

    int scrollBarX = 760;
    int scrollBarY = 80;
    int scrollBarWidth = 40;
    int scrollBarHeight = 460;
    int scrollBarRectX = 766;
    int scrollBarRectY = 28;

    float scrollPosition = float(mScreenTopChallengeIndex);
    float scrollBarHeightFloat = float(scrollBarHeight);

    if (mScreenTopChallengeIndex == mSelectedChallengeIndex) {
        // 未滚动时的状态
        scrollBarHeightFloat = 448.0f;
        scrollBarHeight = 448;
        scrollBarY = 86;
    } else {
        // 滚动时的动画效果
        scrollPosition = TodAnimateCurveFloatTime(0.0f, 0.15f, mUnkFloat, scrollPosition, mSelectedChallengeIndex, TodCurves::CURVE_LINEAR);
        scrollBarHeight = scrollBarHeight - 12;
        scrollBarY = scrollBarY + 6;
        scrollBarRectX = scrollBarX + 6;
        scrollBarRectY = scrollBarWidth - 12;
        scrollBarHeightFloat = float(scrollBarHeight);
    }

    int thumbPosition = int((scrollPosition / mTotalGameInPage) * scrollBarHeightFloat);
    int thumbHeight = int(scrollBarHeightFloat * (5.0f / mTotalGameInPage));

    int thumbY = scrollBarY + thumbPosition;
    int actualThumbHeight = (thumbHeight > scrollBarHeight) ? scrollBarHeight : thumbHeight;

    // 设置裁剪区域并绘制滚动条
    g->ClipRect(scrollBarRectX, scrollBarY, scrollBarRectY, scrollBarHeight);

    Color scrollBarBgColor(0, 128);
    g->SetColor(scrollBarBgColor);
    g->FillRect(Rect(scrollBarX + 6, scrollBarY + 6, scrollBarWidth - 12, scrollBarHeight - 12));

    Color scrollBarThumbColor(140, 140, 140, 255);
    g->SetColor(scrollBarThumbColor);
    g->FillRect(Rect(scrollBarRectX, thumbY, scrollBarRectY, actualThumbHeight));

    g->ClearClipRect();
    g->SetColorizeImages(false);
    g->DrawImageBox(Rect(scrollBarX, scrollBarY, scrollBarWidth, scrollBarHeight), *Sexy::IMAGE_DLG_SELECTORFRAME);

    g->PopState();

    g->PushState();
    g->ClipRect(-20, 80, 1000, 475);
    g->TranslateF(0.0f, -(scrollPosition * 120.0f));

    if (mTotalGameInPage > 0) {
        float *unkFloatPtr = &mUnkFloat;
        for (int aChallengeMode = 0; aChallengeMode < mTotalGameInPage; ++aChallengeMode) {
            int aChallengeId = *reinterpret_cast<int *>(unkFloatPtr + 1);
            DrawButton(g, aChallengeId, aChallengeMode);
            unkFloatPtr += 1;
        }
    }

    g->ClearClipRect();

    if (mToolTip) {
        mToolTip->Draw(g);
    }

    g->PopState();
}

void ChallengeScreen::AddedToManager(int *theWidgetManager) {
    // 记录当前游戏状态
    gChallengeScreenCloseButton = MakeButton(1000, this, this, "[CLOSE]");
    gChallengeScreenCloseButton->Resize(800, 520, 170, 50);
    AddWidget(gChallengeScreenCloseButton);

    old_ChallengeScreen_AddedToManager(this, theWidgetManager);
}

void ChallengeScreen::Update() {
    // 记录当前游戏状态
    old_ChallengeScreen_Update(this);
}

void ChallengeScreen::RemovedFromManager(int *theWidgetManager) {
    // 记录当前游戏状态
    old_ChallengeScreen_RemovedFromManager(this, theWidgetManager);

    if (gChallengeScreenCloseButton != nullptr) {
        RemoveWidget(gChallengeScreenCloseButton);
    }
}

void ChallengeScreen::_destructor2() {
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

    // int totalGamesInThisPage = a[376];//如果这个值是33
    // int currentSelectedGameIndex = ChallengeScreen_GetCurrentSelectedGameIndex(
    // a);//这里取值就是0~32。种子雨是32。

    // int firstGameInPageIndex = a->mScreenTopChallengeIndex;
    // int firstGameInPageIndex2 = a[186];
    // a->mSelectedMode = a[currentSelectedGameIndex + 1 + 188];//向下移动绿色光标，不可循环滚动
    // a->mSelectedMode = a[currentSelectedGameIndex - 1 + 188];//向上移动绿色光标，不可循环滚动

    // LOGD("dOWN:%d %d %d %d", x, y, firstGameInPageIndex, firstGameInPageIndex2);
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
        // ChallengeScreen_UpdateButtons(a);
        gChallengeItemMoved = true;
    } else if (y - gChallengeScreenTouchDownY > triggerHeight) {
        gChallengeScreenGameIndex -= 1;
        gChallengeScreenTouchDownY += triggerHeight;
        int gameIndexToScroll = gChallengeScreenGameIndex <= 0 ? 0 : gChallengeScreenGameIndex;
        SetScrollTarget(gameIndexToScroll);
        // ChallengeScreen_UpdateButtons(a);
        gChallengeItemMoved = true;
    }
}

void ChallengeScreen::MouseUp(int x, int y) {
    if (!gTouchOutSide && !gChallengeItemMoved) {
        int gameIndex = mScreenTopChallengeIndex + (y - mPageTop) / gChallengeItemHeight;
        if (mSelectedMode == mUnk1[gameIndex]) {
            KeyDown(Sexy::KEYCODE_RETURN);
        } else {
            mApp->PlaySample(*Sexy_SOUND_BUTTONCLICK_Addr);
            mSelectedMode = mUnk1[gameIndex];
        }
    }
    gTouchOutSide = false;
    gChallengeItemMoved = false;
}
