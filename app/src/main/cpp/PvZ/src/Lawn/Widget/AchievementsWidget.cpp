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

#include "PvZ/Lawn/Widget/AchievementsWidget.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/TodLib/Common/TodStringFile.h"

#include <algorithm>
#include <cmath>
#include <sys/time.h>

using namespace Sexy;

AchievementsWidget::AchievementsWidget(LawnApp *theApp) {
    reinterpret_cast<void (*)(MaskHelpWidget *, LawnApp *)>(MaskHelpWidget_MaskHelpWidgetAddr)(reinterpret_cast<MaskHelpWidget *>(this), theApp);
    mApp = theApp;
    mMouseDownY = 0;
    mLastDownY = 0;
    mLastDownY1 = 0;
    mLastTimeMs = 0;
    mLastTimeMs1 = 0;
    mVelocity = 0.0f;
    mAccY = 0.0f;
    mIsScrolling = false;
}

AchievementsWidget::~AchievementsWidget() {
    reinterpret_cast<void (*)(MaskHelpWidget *)>(MaskHelpWidget_DeleteAddr)(reinterpret_cast<MaskHelpWidget *>(this));
}

void AchievementsWidget::Draw(Graphics *g) {
    int theY = 0;
    int theDiffY = addonImages.hole->mHeight;
    for (int i = 0; i < ACHIEVEMENT_HOLE_LENGTH; i++) {
        if (i == ACHIEVEMENT_HOLE_WORM_POS) {
            g->DrawImage(addonImages.hole_worm, 0, theY);
        } else if (i == ACHIEVEMENT_HOLE_GEMS_POS) {
            g->DrawImage(addonImages.hole_gems, 0, theY);
        } else if (i == ACHIEVEMENT_HOLE_CHUZZLE_POS) {
            g->DrawImage(addonImages.hole_chuzzle, 0, theY);
        } else if (i == ACHIEVEMENT_HOLE_BJORN_POS) {
            g->DrawImage(addonImages.hole_bjorn, 0, theY);
        } else if (i == ACHIEVEMENT_HOLE_PIPE_POS) {
            g->DrawImage(addonImages.hole_pipe, 0, theY);
        } else if (i == ACHIEVEMENT_HOLE_TIKI_POS) {
            g->DrawImage(addonImages.hole_tiki, 0, theY);
        } else if (i == ACHIEVEMENT_HOLE_HEAVY_ROCKS_POS) {
            g->DrawImage(addonImages.hole_heavyrocks, 0, theY);
        } else if (i == ACHIEVEMENT_HOLE_DU_WEI_POS) {
            g->DrawImage(addonImages.hole_duwei, 0, theY);
        } else {
            g->DrawImage(addonImages.hole, 0, theY);
        }
        theY += theDiffY;
    }
    g->DrawImage(addonImages.hole_china, 0, theY);
    g->DrawImage(addonImages.hole_top, 0, 0);
    int theAchievementY = 300;
    for (int i = 0; i < AchievementType::NUM_ACHIEVEMENT_TYPES; ++i) {
        if (!mApp->mPlayerInfo->mAchievements[i]) {
            g->SetColorizeImages(true);
            g->SetColor(gColorGray);
        }
        g->DrawImage(GetIconByAchievementId((AchievementType)i), 330, theAchievementY - 5);
        const char *theAchievementName = GetNameByAchievementId((AchievementType)i);
        pvzstl::string str = StrFormat("[%s]", theAchievementName);
        pvzstl::string str1 = StrFormat("[%s_TEXT]", theAchievementName);
        Color theColor = {0, 255, 0, 255};
        Color theColor1 = {255, 255, 255, 255};
        Rect rect = {460, theAchievementY + 60, 540, 0};
        TodDrawString(g, str, 460, theAchievementY + 40, FONT_HOUSEOFTERROR28, theColor, DrawStringJustification::DS_ALIGN_LEFT);
        if (i == AchievementType::ACHIEVEMENT_SHOP) {
            str = TodReplaceNumberString(str1, "{coin}", mApp->mPlayerInfo->mUsedCoins * 10);
            TodDrawStringWrapped(g, str, rect, FONT_HOUSEOFTERROR20, theColor1, DrawStringJustification::DS_ALIGN_LEFT, false);
        } else {
            TodDrawStringWrapped(g, str1, rect, FONT_HOUSEOFTERROR20, theColor1, DrawStringJustification::DS_ALIGN_LEFT, false);
        }
        g->SetColorizeImages(false);
        theAchievementY += theDiffY * 2 / 3;
    }
    int theAccomplishedNum = 0;
    for (int i = 0; i < AchievementType::NUM_ACHIEVEMENT_TYPES; ++i) {
        if (mApp->mPlayerInfo->mAchievements[i]) {
            theAccomplishedNum++;
        }
    }
    pvzstl::string str = StrFormat("%d/%d", theAccomplishedNum, AchievementType::NUM_ACHIEVEMENT_TYPES);
    Color theColor = {255, 240, 0, 255};
    TodDrawString(g, str, 1060, 173, FONT_DWARVENTODCRAFT18, theColor, DrawStringJustification::DS_ALIGN_CENTER);
}

void AchievementsWidget::MouseDown(int x, int y, int theClickCount) {
    (void)x;
    (void)theClickCount;
    mIsScrolling = false;
    mMouseDownY = y;
    mLastDownY = y;
    mLastDownY1 = mLastDownY;
    timeval tp;
    gettimeofday(&tp, nullptr);
    mLastTimeMs = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    mLastTimeMs1 = mLastTimeMs;
}

void AchievementsWidget::MouseDrag(int x, int y) {
    (void)x;
    if (gAchievementState != SHOWING)
        return;
    int theNewY = std::clamp(mY + (y - mMouseDownY), 720 + MAIN_MENU_HEIGHT - (ACHIEVEMENT_HOLE_LENGTH + 1) * addonImages.hole->mHeight, MAIN_MENU_HEIGHT);
    Move(mX, theNewY);
    mLastDownY1 = mLastDownY;
    mLastDownY = y;
    timeval tp;
    gettimeofday(&tp, nullptr);
    mLastTimeMs1 = mLastTimeMs;
    mLastTimeMs = tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

void AchievementsWidget::MouseUp(int x, int y) {
    (void)x;
    (void)y;
    timeval tp;
    gettimeofday(&tp, nullptr);
    long currentTimeMs = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    long deltaT = currentTimeMs - mLastTimeMs;
    int deltaX = mLastDownY - mMouseDownY;
    if (deltaT == 0) {
        deltaT = currentTimeMs - mLastTimeMs1;
        deltaX = mLastDownY1 - mMouseDownY;
    }
    if (deltaX != 0 && deltaT != 0) {
        mIsScrolling = true;
        mVelocity = 5.0f * deltaX / deltaT;
    }
    mLastTimeMs = currentTimeMs;
}

void AchievementsWidget::Update() {
    // 实现滚动
    if (mIsScrolling) {
        int theNewY = std::clamp<int>(mY + mVelocity, 720 + MAIN_MENU_HEIGHT - (ACHIEVEMENT_HOLE_LENGTH + 1) * addonImages.hole->mHeight, MAIN_MENU_HEIGHT);
        Move(mX, theNewY);
        mVelocity *= 0.96f;
        if (std::fabs(mVelocity) < 1.0f) {
            mIsScrolling = false;
        }
    }
    MarkDirty();
}

void MaskHelpWidget_Update(AchievementsWidget *achievementsWidget) {
    achievementsWidget->Update();
}

void MaskHelpWidget_Draw(AchievementsWidget *achievementsWidget, Graphics *g) {
    achievementsWidget->Draw(g);
}

void MaskHelpWidget_MouseDown(AchievementsWidget *achievementsWidget, int x, int y, int theClickCount) {
    achievementsWidget->MouseDown(x, y, theClickCount);
}

void MaskHelpWidget_MouseUp(AchievementsWidget *achievementsWidget, int x, int y) {
    achievementsWidget->MouseUp(x, y);
}

void MaskHelpWidget_MouseDrag(AchievementsWidget *achievementsWidget, int x, int y) {
    achievementsWidget->MouseDrag(x, y);
}
