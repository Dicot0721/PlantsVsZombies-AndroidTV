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

#include "PvZ/Lawn/Board/ToolTipWidget.h"
#include "PvZ/SexyAppFramework/Graphics/Font.h"
#include "PvZ/TodLib/Common/TodStringFile.h"
#include <PvZ/Lawn/LawnApp.h>
#include <vector>

using namespace Sexy;

void ToolTipWidget::Draw(Sexy::Graphics *g) {
    if (!mVisible)
        return;

    float transX = g->mTransX;
    float transY = g->mTransY;

    float screenOffsetX;
    if (*(bool *)(gLawnApp->unkMem2[19] + 96))
        screenOffsetX = (float)LawnApp::FULLSCREEN_RECT.mX;
    else
        screenOffsetX = -80.0f;

    int drawX = mX - transX - screenOffsetX;

    if (mCenter)
        drawX -= mWidth / 2;

    int drawY = mY;

    float minLeft = (float)mMinLeft - transX;
    if ((float)drawX < minLeft) {
        int oldDrawX = (int)transX;
        drawX = mMinLeft - oldDrawX;
    }

    float minTop = -transY;
    if ((float)drawY < minTop)
        drawY = (int)minTop;

    if (mWidth > 0 && mHeight > 0) {
        g->SetColor(Sexy::Color(255, 255, 200, 255));
        g->FillRect(Rect(drawX, drawY, mWidth, mHeight));

        g->SetColor(Sexy::Color::Black);
        g->DrawRect(Rect(drawX, drawY, mWidth - 1, mHeight - 1));
    }

    int textY = drawY + 1;

    // title
    if (!mTitle.empty()) {
        g->SetFont(mTitleFont);
        g->SetColor(mTitleTextColor);

        int titleWidth = ((int (*)(Sexy::Font *, const pvzstl::string *))mTitleFont->vTable[8])(mTitleFont, &mTitle); // int titleWidth = mTitleFont->StringWidth(mTitle);
        int titleX = drawX + (mWidth - titleWidth) / 2;
        int titleBaselineY = textY + mTitleFont->GetAscent();

        g->DrawString(mTitle, titleX, titleBaselineY);

        textY += mTitleFont->GetAscent() + 2;
    }

    // warning text
    if (!mWarningText.empty()) {
        g->SetFont(mWarningTextFont);

        int warningWidth =
            ((int (*)(Sexy::Font *, const pvzstl::string *))mWarningTextFont->vTable[8])(mWarningTextFont, &mWarningText); // int warningWidth = mWarningTextFont->StringWidth(mWarningText);
        int warningX = drawX + (mWidth - warningWidth) / 2;
        int warningBaselineY = textY + mWarningTextFont->GetAscent();

        Sexy::Color warningColor(255, 0, 0);

        if (mWarningFlashCounter > 0 && mWarningFlashCounter % 20 <= 9)
            warningColor = Sexy::Color(0, 0, 0);

        g->SetColor(warningColor);
        g->DrawString(mWarningText, warningX, warningBaselineY);

        g->SetColor(Sexy::Color::Black);

        textY += mWarningTextFont->GetAscent() + 2;
    }

    // body lines
    int bodyY = textY + 1;

    std::vector<pvzstl::string> lines;
    GetLines(lines);

    g->SetFont(mWarningTextFont);
    g->SetColor(Sexy::Color(32, 32, 32));

    for (const pvzstl::string &line : lines) {
        int lineWidth = ((int (*)(Sexy::Font *, const pvzstl::string *))mWarningTextFont->vTable[8])(mWarningTextFont, &line); // int lineWidth = mWarningTextFont->StringWidth(line);
        int lineX = drawX + (mWidth - lineWidth) / 2;
        int lineBaselineY = bodyY + mWarningTextFont->GetAscent();

        g->DrawString(line, lineX, lineBaselineY);

        bodyY += mWarningTextFont->GetAscent() + 2;
    }
}