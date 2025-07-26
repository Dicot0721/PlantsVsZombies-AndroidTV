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

#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/SexyAppFramework/Misc/SexyVertex2D.h"
#include "PvZ/TodLib/Common/TodCommon.h"

using namespace Sexy;

void Graphics::PushTransform(int *theTransform, bool concatenate) {
    old_Sexy_Graphics_PushTransform(this, theTransform, concatenate);
}

void Graphics::PopTransform() {
    old_Sexy_Graphics_PopTransform(this);
}

void Graphics::DrawTrianglesTex2(Image *theTexture, TriVertex theVertices[][3], int theNumTriangles) {
    SexyVertex2D tmp[theNumTriangles][3];

    for (int i = 0; i < theNumTriangles; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int j = 0; j < 3; ++j) {
                tmp[i][j].x = theVertices[i][j].x;
                tmp[i][j].y = theVertices[i][j].y;
                tmp[i][j].u = theVertices[i][j].u;
                tmp[i][j].v = theVertices[i][j].v;
                tmp[i][j].color = theVertices[i][j].color;
            }
        }
        DrawTrianglesTex(theTexture, tmp, theNumTriangles);
    }
}

void Sexy_Graphics_DrawImageColorized(Sexy::Graphics *g, Sexy::Image *image, Sexy::Color *color, int x, int y) {
    g->SetColor(*color);
    g->SetColorizeImages(true);
    g->DrawImage(image, x, y);
    g->SetColorizeImages(false);
}

void Sexy_Graphics_DrawImageColorizedScaled(Sexy::Graphics *g, Sexy::Image *image, Sexy::Color *color, float x, float y, float xScaled, float yScaled) {
    g->SetColor(*color);
    g->SetColorizeImages(true);
    TodDrawImageScaledF(g, image, x, y, xScaled, yScaled);
    g->SetColorizeImages(false);
}

Sexy::Font *Graphics::GetFont() {
    return mFont;
}

void Graphics::SetFont(Sexy::Font *theFont) {
    mFont = theFont;
}

void Graphics::SetColor(const Color &theColor) {
    mColor = theColor;
}

const Color &Graphics::GetColor() {
    return mColor;
}

void Graphics::SetDrawMode(DrawMode theDrawMode) {
    mDrawMode = theDrawMode;
}

int Graphics::GetDrawMode() {
    return mDrawMode;
}

void Graphics::SetColorizeImages(bool colorizeImages) {
    mColorizeImages = colorizeImages;
}

bool Graphics::GetColorizeImages() {
    return mColorizeImages;
}

void Graphics::SetLinearBlend(bool linear) {
    mLinearBlend = linear;
}

bool Graphics::GetLinearBlend() {
    return mLinearBlend;
}

void Graphics::Translate(int theTransX, int theTransY) {
    mTransX += theTransX;
    mTransY += theTransY;
}

void Graphics::SetScale(float theScaleX, float theScaleY, float theOrigX, float theOrigY) {
    mScaleX = theScaleX;
    mScaleY = theScaleY;
    mScaleOrigX = theOrigX + mTransX;
    mScaleOrigY = theOrigY + mTransY;
}