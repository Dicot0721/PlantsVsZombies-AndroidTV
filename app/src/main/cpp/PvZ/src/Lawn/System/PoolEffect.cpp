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

#include "PvZ/Lawn/System/PoolEffect.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Misc.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/SexyAppFramework/Graphics/MemoryImage.h"
#include "PvZ/SexyAppFramework/Misc/SexyVertex2D.h"
#include "PvZ/TodLib/Effect/TodParticle.h"

#include <numbers>

using namespace Sexy;

void PoolEffect::UpdateWaterEffect() {
    int idx = 0;
    for (int y = 0; y < CAUSTIC_IMAGE_HEIGHT; y++) {
        int timeV1 = (256 - y) << 17;
        int timeV0 = y << 17;

        for (int x = 0; x < CAUSTIC_IMAGE_WIDTH; x++) {
            unsigned long *pix = &mCausticImage->mBits[idx];

            int timeU = x << 17;
            int timePool0 = mPoolCounter << 16;
            int timePool1 = ((mPoolCounter & 65535) + 1) << 16;
            int a1 = (unsigned char)BilinearLookupFixedPoint(timeU - timePool1 / 6, timeV1 + timePool0 / 8);
            int a0 = (unsigned char)BilinearLookupFixedPoint(timeU + timePool0 / 10, timeV0);
            auto a = (unsigned char)((a0 + a1) / 2);

            unsigned char alpha;
            if (a >= 160U) {
                alpha = 255 - 2 * (a - 160U);
            } else if (a >= 128U) {
                alpha = 5 * (a - 128U);
            } else {
                alpha = 0;
            }

            *pix = (*pix & 0x00FFFFFF) + (((int)alpha / 3) << 24);
            idx++;
        }
    }

    mCausticImage->BitsChanged();
}

void PoolEffect::PoolEffectDraw(Sexy::Graphics *g, bool theIsNight) {
    // 添加3D加速开关绘制支持
    if (!mApp->Is3DAccelerated()) {

        if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_POOL_PARTY && mApp->mBoard != nullptr) {
            float theTmpTransY = g->mTransY;
            int thePoolOffsetY[2] = {164, -175};
            for (int aPoolOffsetY : thePoolOffsetY) {
                g->mTransY += (float)aPoolOffsetY;
                if (theIsNight) {
                    g->DrawImage(*Sexy_IMAGE_POOL_NIGHT_Addr, 34, 278);
                    //                g->DrawTrianglesTex(*Sexy_IMAGE_POOL_BASE_NIGHT_Addr, v140[0], 150);
                    //                g->DrawTrianglesTex(*Sexy_IMAGE_POOL_SHADING_NIGHT_Addr, v140[1], 150);
                } else {
                    g->DrawImage(*Sexy_IMAGE_POOL_Addr, 34, 278);
                    //                g->DrawTrianglesTex(*Sexy_IMAGE_POOL_BASE_Addr, v140[0], 150);
                    //                g->DrawTrianglesTex(*Sexy_IMAGE_POOL_SHADING_Addr, v140[1], 150);
                    TodParticleSystem *aPoolSparkle = mApp->ParticleTryToGet(mApp->mBoard->mPoolSparklyParticleID);
                    if (aPoolSparkle != nullptr) {
                        aPoolSparkle->Draw(g);
                    }
                }
                g->mTransY = theTmpTransY;
            }
        } else {
            if (theIsNight) {
                g->DrawImage(*Sexy_IMAGE_POOL_NIGHT_Addr, 34, 278);
            } else {
                g->DrawImage(*Sexy_IMAGE_POOL_Addr, 34, 278);
            }
        }
        return;
    }

    float aGridSquareX = (float)(*Sexy_IMAGE_POOL_Addr)->GetWidth() / 15.0f;
    float aGridSquareY = (float)(*Sexy_IMAGE_POOL_Addr)->GetHeight() / 5.0f;
    float aOffsetArray[3][16][6][2] = {{{{0.0f}}}};
    for (int x = 0; x <= 15; x++) {
        for (int y = 0; y <= 5; y++) {
            aOffsetArray[2][x][y][0] = (float)x / 15.0f;
            aOffsetArray[2][x][y][1] = (float)y / 5.0f;
            if (x != 0 && x != 15 && y != 0 && y != 5) {
                auto aPoolPhase = (float)(mPoolCounter * 2 * std::numbers::pi);
                float aWaveTime1 = aPoolPhase / 800.0f;
                float aWaveTime2 = aPoolPhase / 150.0f;
                float aWaveTime3 = aPoolPhase / 900.0f;
                float aWaveTime4 = aPoolPhase / 800.0f;
                float aWaveTime5 = aPoolPhase / 110.0f;
                float xPhase = (float)x * 3.0f * 2 * (float)std::numbers::pi / 15.0f;
                float yPhase = (float)y * 3.0f * 2 * (float)std::numbers::pi / 5.0f;

                aOffsetArray[0][x][y][0] = sin(yPhase + aWaveTime2) * 0.002f + sin(yPhase + aWaveTime1) * 0.005f;
                aOffsetArray[0][x][y][1] = sin(xPhase + aWaveTime5) * 0.01f + sin(xPhase + aWaveTime3) * 0.015f + sin(xPhase + aWaveTime4) * 0.005f;
                aOffsetArray[1][x][y][0] = sin(yPhase * 0.2f + aWaveTime2) * 0.015f + sin(yPhase * 0.2f + aWaveTime1) * 0.012f;
                aOffsetArray[1][x][y][1] = sin(xPhase * 0.2f + aWaveTime5) * 0.005f + sin(xPhase * 0.2f + aWaveTime3) * 0.015f + sin(xPhase * 0.2f + aWaveTime4) * 0.02f;
                aOffsetArray[2][x][y][0] += sin(yPhase + aWaveTime1 * 1.5f) * 0.004f + sin(yPhase + aWaveTime2 * 1.5f) * 0.005f;
                aOffsetArray[2][x][y][1] += sin(xPhase * 4.0f + aWaveTime5 * 2.5f) * 0.005f + sin(xPhase * 2.0f + aWaveTime3 * 2.5f) * 0.04f + sin(xPhase * 3.0f + aWaveTime4 * 2.5f) * 0.02f;
            } else {
                aOffsetArray[0][x][y][0] = 0.0f;
                aOffsetArray[0][x][y][1] = 0.0f;
                aOffsetArray[1][x][y][0] = 0.0f;
                aOffsetArray[1][x][y][1] = 0.0f;
            }
        }
    }

    int aIndexOffsetX[6] = {0, 0, 1, 0, 1, 1};
    int aIndexOffsetY[6] = {0, 1, 1, 0, 1, 0};
    SexyVertex2D aVertArray[3][150][3];

    for (int x = 0; x < 15; x++) {
        for (int y = 0; y < 5; y++) {
            for (int aLayer = 0; aLayer < 3; aLayer++) {
                SexyVertex2D *pVert = &aVertArray[aLayer][x * 10 + y * 2][0];
                for (int aVertIndex = 0; aVertIndex < 6; aVertIndex++, pVert++) {
                    int aIndexX = x + aIndexOffsetX[aVertIndex];
                    int aIndexY = y + aIndexOffsetY[aVertIndex];
                    if (aLayer == 2) {
                        pVert->x = (704.0f / 15.0f) * (float)aIndexX + 45.0f;
                        pVert->y = 30.0f * (float)aIndexY + 288.0f;
                        pVert->u = aOffsetArray[2][aIndexX][aIndexY][0] + (float)aIndexX / 15.0f;
                        pVert->v = aOffsetArray[2][aIndexX][aIndexY][1] + (float)aIndexY / 5.0f;

                        if (!g->mClipRect.Contains((int)pVert->x, (int)pVert->y)) {
                            pVert->color = 0x00FFFFFFUL;
                        } else if (aIndexX == 0 || aIndexX == 15 || aIndexY == 0) {
                            pVert->color = 0x20FFFFFFUL;
                        } else if (theIsNight) {
                            pVert->color = 0x30FFFFFFUL;
                        } else {
                            pVert->color = aIndexX <= 7 ? 0xC0FFFFFFUL : 0x80FFFFFFUL;
                        }
                    } else {
                        pVert->color = 0xFFFFFFFFUL;
                        pVert->x = (float)aIndexX * aGridSquareX + 35.0f;
                        pVert->y = (float)aIndexY * aGridSquareY + 279.0f;
                        pVert->u = aOffsetArray[aLayer][aIndexX][aIndexY][0] + (float)aIndexX / 15.0f;
                        pVert->v = aOffsetArray[aLayer][aIndexX][aIndexY][1] + (float)aIndexY / 5.0f;
                        if (!g->mClipRect.Contains((int)pVert->x, (int)pVert->y)) {
                            pVert->color = 0x00FFFFFFUL;
                        }
                    }
                }
            }
        }
    }
    Graphics aPoolEffectG(*g);
    aPoolEffectG.SetWrapMode(0, 0); // 用于修复除mCausticImage绘制重复·外其他都是裁切，g本应当为全局平铺重复

    if (theIsNight) {
        g->DrawTrianglesTex(*Sexy_IMAGE_POOL_BASE_NIGHT_Addr, aVertArray[0], 150);
        g->DrawTrianglesTex(*Sexy_IMAGE_POOL_SHADING_NIGHT_Addr, aVertArray[1], 150);
    } else {
        g->DrawTrianglesTex(*Sexy_IMAGE_POOL_BASE_Addr, aVertArray[0], 150);
        g->DrawTrianglesTex(*Sexy_IMAGE_POOL_SHADING_Addr, aVertArray[1], 150);
    }

    UpdateWaterEffect();
    g->DrawTrianglesTex((Image *)mCausticImage, aVertArray[2], 150);

    aPoolEffectG.SetWrapMode(1, 1);
}