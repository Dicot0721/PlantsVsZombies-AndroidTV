//
// Created by 28636 on 2025/5/26.
//

#include "PvZ/Lawn/System/PoolEffect.h"
#include "PvZ/Misc.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/SexyAppFramework/Graphics/MemoryImage.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/SexyAppFramework/Misc/SexyVertex2D.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/TodLib/Effect/TodParticle.h"

using namespace Sexy;

void PoolEffect_UpdateWaterEffect(PoolEffect *poolEffect) {
    int v2;            // eax
    unsigned int v4;   // ecx
    int v5;            // ebp
    int v6;            // eax
    unsigned int *v7;  // edi
    int v8;            // esi
    int v9;            // eax
    unsigned char v10; // cl
    int y;             // [esp+10h] [ebp-14h]
    int i;             // [esp+14h] [ebp-10h]
    int v13;           // [esp+18h] [ebp-Ch]
    unsigned int v;    // [esp+1Ch] [ebp-8h]
    int v15;           // [esp+20h] [ebp-4h]

    v2 = 0;
    for (i = 0;; v2 = i) {
        v13 = (256 - v2) << 17;
        v4 = v2 << 17;
        v5 = 0;
        v6 = v2 << 9;
        v = v4;
        for (y = v6;; v6 = y) {
            v7 = (unsigned int *)((char *)poolEffect->mCausticImage->mBits + v6);
            v8 = poolEffect->mPoolCounter << 16;
            v15 = (unsigned char)PoolEffect_BilinearLookupFixedPoint(poolEffect, v5 - (((unsigned short)poolEffect->mPoolCounter + 1) << 16) / 6, v13 + v8 / 8);
            v9 = ((unsigned char)PoolEffect_BilinearLookupFixedPoint(poolEffect, v5 + v8 / 10, v) + v15) / 2;
            if ((unsigned char)v9 < 0xA0u)
                v10 = (unsigned char)v9 < 0x80u ? 0 : 5 * (v9 + 0x80);
            else
                v10 = 63 - 2 * v9;
            y += 4;
            v5 += 0x20000;
            *v7 = (*v7 & 0xFFFFFF) + ((v10 / 3) << 24);
            if (v5 >= 0x1000000)
                break;
        }
        if (++i >= 64)
            break;
    }
    poolEffect->mCausticImage->BitsChanged();
}

void PoolEffect_PoolEffectDraw(PoolEffect *poolEffect, Sexy::Graphics *g, bool theIsNight) {
    float v6;                     // st7
    float v7;                     // st6
    float v8;                     // st5
    int v9;                       // ebx
    float v10;                    // st4
    float v11;                    // st3
    float *v12;                   // esi
    float v13;                    // rt0
    float v14;                    // st3
    float v15;                    // rt2
    float v16;                    // st3
    int v17;                      // edi
    float v18;                    // rt2
    float v19;                    // st3
    float v20;                    // rt0
    float v21;                    // st3
    float v22;                    // st2
    float v23;                    // st6
    float v24;                    // st4
    float v25;                    // st7
    float v26;                    // st6
    float v27;                    // st5
    float v28;                    // st4
    float v29;                    // st3
    float v30;                    // rt0
    float v31;                    // st3
    float v32;                    // st3
    float v33;                    // st5
    float v34;                    // rt0
    float v35;                    // st3
    float v36;                    // rt1
    float v37;                    // st7
    float v40;                    // st5
    int v41;                      // eax
    Sexy::SexyVertex2D *v42;      // esi
    char *v43;                    // ecx
    int v44;                      // edi
    int v45;                      // ebx
    float v46;                    // st4
    int v47;                      // eax
    float v48;                    // st3
    int v49;                      // edi
    int v50;                      // ecx
    int v51;                      // eax
    int v52;                      // edx
    float v53;                    // st3
    int mX;                       // ecx
    int mY;                       // eax
    float v66;                    // [esp+Ch] [ebp-72F4h]
    float v67;                    // [esp+Ch] [ebp-72F4h]
    float v68;                    // [esp+Ch] [ebp-72F4h]
    float v69;                    // [esp+Ch] [ebp-72F4h]
    float v70;                    // [esp+Ch] [ebp-72F4h]
    float v71;                    // [esp+Ch] [ebp-72F4h]
    float v72;                    // [esp+Ch] [ebp-72F4h]
    float v73;                    // [esp+Ch] [ebp-72F4h]
    float v74;                    // [esp+Ch] [ebp-72F4h]
    float v75;                    // [esp+Ch] [ebp-72F4h]
    float v76;                    // [esp+Ch] [ebp-72F4h]
    float v77;                    // [esp+Ch] [ebp-72F4h]
    float v78;                    // [esp+Ch] [ebp-72F4h]
    float v79;                    // [esp+Ch] [ebp-72F4h]
    float v80;                    // [esp+Ch] [ebp-72F4h]
    float v81;                    // [esp+Ch] [ebp-72F4h]
    float v82;                    // [esp+Ch] [ebp-72F4h]
    float v83;                    // [esp+Ch] [ebp-72F4h]
    float v84;                    // [esp+Ch] [ebp-72F4h]
    float v85;                    // [esp+Ch] [ebp-72F4h]
    float v86;                    // [esp+Ch] [ebp-72F4h]
    float v87;                    // [esp+Ch] [ebp-72F4h]
    float v88;                    // [esp+Ch] [ebp-72F4h]
    float v89;                    // [esp+Ch] [ebp-72F4h]
    float v90;                    // [esp+Ch] [ebp-72F4h]
    float v91;                    // [esp+Ch] [ebp-72F4h]
    float v92;                    // [esp+Ch] [ebp-72F4h]
    float v93;                    // [esp+Ch] [ebp-72F4h]
    float v94;                    // [esp+Ch] [ebp-72F4h]
    float v95;                    // [esp+Ch] [ebp-72F4h]
    int v97;                      // [esp+10h] [ebp-72F0h]
    float v98;                    // [esp+10h] [ebp-72F0h]
    float v99;                    // [esp+10h] [ebp-72F0h]
    float v101;                   // [esp+14h] [ebp-72ECh]
    float v102;                   // [esp+14h] [ebp-72ECh]
    float v103;                   // [esp+14h] [ebp-72ECh]
    float v104;                   // [esp+14h] [ebp-72ECh]
    float v105;                   // [esp+18h] [ebp-72E8h]
    int j;                        // [esp+18h] [ebp-72E8h]
    float theTriangleCelWidtha;   // [esp+1Ch] [ebp-72E4h]
    float theTriangleCelWidth;    // [esp+1Ch] [ebp-72E4h]
    float theTriangleCelWidthb;   // [esp+1Ch] [ebp-72E4h]
    float theTriangleCelWidthc;   // [esp+1Ch] [ebp-72E4h]
    float v113;                   // [esp+20h] [ebp-72E0h]
    int k;                        // [esp+20h] [ebp-72E0h]
    int v115;                     // [esp+24h] [ebp-72DCh]
    float v116;                   // [esp+24h] [ebp-72DCh]
    float v117;                   // [esp+28h] [ebp-72D8h]
    int i;                        // [esp+28h] [ebp-72D8h]
    float _Ptra;                  // [esp+2Ch] [ebp-72D4h]
    char *_Ptr;                   // [esp+2Ch] [ebp-72D4h]
    float v121;                   // [esp+30h] [ebp-72D0h]
    float v122;                   // [esp+30h] [ebp-72D0h]
    float v123;                   // [esp+30h] [ebp-72D0h]
    float v124;                   // [esp+30h] [ebp-72D0h]
    float v125;                   // [esp+30h] [ebp-72D0h]
    float v126;                   // [esp+30h] [ebp-72D0h]
    float v127;                   // [esp+38h] [ebp-72C8h]
    float v128;                   // [esp+38h] [ebp-72C8h]
    float v129;                   // [esp+38h] [ebp-72C8h]
    float v130;                   // [esp+38h] [ebp-72C8h]
    float v131;                   // [esp+38h] [ebp-72C8h]
    float v132;                   // [esp+44h] [ebp-72BCh]
    float v133;                   // [esp+48h] [ebp-72B8h]
    float v135;                   // [esp+50h] [ebp-72B0h]
    float theTriangleCelHeight;   // [esp+54h] [ebp-72ACh]
    int v137[6];                  // [esp+58h] [ebp-72A8h]
    int v138[6];                  // [esp+70h] [ebp-7290h]
    SexyVertex2D v140[3][150][3]; // [esp+988h] [ebp-6978h] BYREF

    theTriangleCelWidth = (*Sexy_IMAGE_POOL_Addr)->GetCelWidth() / 15.0;
    theTriangleCelHeight = (*Sexy_IMAGE_POOL_Addr)->GetHeight() / 5.0;

    v6 = 0.0;
    float v139[576] = {0.0};
    v7 = 0.06666667014360428;
    v8 = 0.2;
    v9 = 0;
    v10 = 6.283185482025146;
    v115 = 0;
    v11 = 3.0;
    v12 = &v139[385];
    while (1) {
        v17 = 0;
        v97 = 0;
        v135 = (float)v115;
        v116 = v135 * v7;
        while (1) {
            *(v12 - 1) = v116;
            v98 = (float)v97;
            v22 = v98;
            v99 = v98 * v8;
            *v12 = v99;
            if (!v9 || v9 == 15 || !v17 || v17 == 5) {
                v30 = v11;
                *(v12 - 385) = v6;
                *(v12 - 384) = v6;
                *(v12 - 193) = v6;
                *(v12 - 192) = v6;
                v31 = v10;
                v28 = v6;
                v25 = v31;
                v32 = v8;
                v33 = v30;
                v34 = v32;
                v35 = v7;
                v26 = v34;
                v36 = v35;
                v29 = v33;
                v27 = v36;
            } else {
                v23 = v10;
                v24 = poolEffect->mPoolCounter * v10;
                v101 = v24 / 800.0;
                v133 = v101;
                v117 = v24 / 150.0;
                _Ptra = v24 / 900.0;
                v132 = v101;
                v102 = v24 / 110.0;
                v105 = v135 * v11 * v23 / 15.0;
                v113 = v22 * v11 * v23 / 5.0;
                v67 = v113 + v133;
                v68 = sin(v67);
                v127 = v68 * 0.005;
                v69 = v113 + v117;
                v70 = sin(v69);
                *(v12 - 385) = v70 * 0.002 + v127;
                v71 = v105 + v132;
                v72 = sin(v71);
                v128 = v72 * 0.005;
                v73 = v105 + _Ptra;
                v74 = sin(v73);
                v129 = v74 * 0.015 + v128;
                v75 = v105 + v102;
                v76 = sin(v75);
                *(v12 - 384) = v76 * 0.01 + v129;
                v130 = v113 * 0.2;
                v77 = v130 + v133;
                v78 = sin(v77);
                v121 = v78 * 0.012;
                v79 = v117 + v130;
                v80 = sin(v79);
                *(v12 - 193) = v80 * 0.015 + v121;
                v131 = v105 * 0.2;
                v81 = v131 + v132;
                v82 = sin(v81);
                v122 = v82 * 0.02;
                v83 = _Ptra + v131;
                v84 = sin(v83);
                v123 = v84 * 0.015 + v122;
                v85 = v102 + v131;
                v86 = sin(v85);
                *(v12 - 192) = v86 * 0.005 + v123;
                v87 = v117 * 1.5 + v113;
                v88 = sin(v87);
                v124 = v88 * 0.005;
                v89 = v133 * 1.5 + v113;
                v90 = sin(v89);
                *(v12 - 1) = v90 * 0.004 + v124 + v116;
                v91 = v105 * 3.0 + v132 * 2.5;
                v92 = sin(v91);
                v125 = v92 * 0.02;
                v93 = v105 + v105 + _Ptra * 2.5;
                v94 = sin(v93);
                v126 = v94 * 0.04 + v125;
                v95 = v105 * 4.0 + v102 * 2.5;
                v66 = sin(v95);
                *v12 = v66 * 0.005 + v126 + v99;
                v25 = 6.283185482025146;
                v26 = 0.2;
                v27 = 0.0666666;
                v28 = 0.0;
                v29 = 3.0;
            }
            ++v17;
            v12 += 2;
            v97 = v17;
            if (v17 > 5)
                break;
            v18 = v29;
            v19 = v25;
            v6 = v28;
            v20 = v19;
            v21 = v26;
            v7 = v27;
            v8 = v21;
            v11 = v18;
            v10 = v20;
        }
        v115 = ++v9;
        if (v9 > 15)
            break;
        v13 = v29;
        v14 = v25;
        v6 = v28;
        v15 = v14;
        v16 = v26;
        v7 = v27;
        v8 = v16;
        v11 = v13;
        v10 = v15;
    }
    v137[0] = 0;
    v137[1] = 0;
    v37 = v27;
    v137[3] = 0;
    v138[0] = 0;
    v138[3] = 0;
    v138[5] = 0;
    v137[2] = 1;
    v137[4] = 1;
    v137[5] = 1;
    v138[1] = 1;
    v138[2] = 1;
    v138[4] = 1;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 150; ++j) {
            for (int k = 0; k < 3; ++k) {
                v140[i][j][k].color = 0;
            }
        }
    }
    v40 = theTriangleCelWidth;
    for (i = 0; i < 15; ++i) {
        for (j = 0; j < 5; ++j) {
            v41 = 0;
            for (k = 0;; v41 = k) {
                v42 = &v140[0][120 * v41][90 * v41 + 24 * i + 6 * i + 6 * j];
                v43 = 0;
                for (_Ptr = 0;; v43 = _Ptr) {
                    v44 = i + *(int *)((char *)v137 + (uint32_t)v43);
                    v45 = j + *(int *)((char *)v138 + (uint32_t)v43);
                    v103 = (float)v44;
                    v46 = v103;
                    if (v41 == 2) {
                        v52 = 3 * v44 + 96;
                        theTriangleCelWidthb = 46.93333435058594 * v46 + 45.0;
                        v53 = theTriangleCelWidthb;
                        v42->x = theTriangleCelWidthb;
                        v104 = (float)v45;
                        theTriangleCelWidthc = 30.0 * v104 + 288.0;
                        v42->y = theTriangleCelWidthc;
                        v42->u = v139[4 * v52 + 2 * v45] + v46 * v37;
                        v42->v = v139[4 * v52 + 1 + 2 * v45] + v104 * v26;
                        theTriangleCelWidth = (int)theTriangleCelWidthc;
                        mX = g->mClipRect.mX;
                        if ((int)v53 < mX)
                            goto LABEL_37;
                        if ((int)v53 < mX + g->mClipRect.mWidth && (mY = g->mClipRect.mY, theTriangleCelWidth >= mY) && theTriangleCelWidth < mY + g->mClipRect.mHeight) {
                            if (v44 && v44 != 15 && v45)
                                v42->color = theIsNight ? 0x30FFFFFF : v44 <= 7 ? 0xC0FFFFFF : 0x80FFFFFF;
                            else
                                v42->color = 0x20FFFFFF;
                        } else {
                        LABEL_37:
                            v42->color = 0xFFFFFF;
                        }
                    } else {
                        v47 = 2 * (v45 + 6 * (v44 + 16 * v41));
                        theTriangleCelWidtha = v46 * v40 + 35.0;
                        v42->color = -1;
                        v48 = theTriangleCelWidtha;
                        v42->x = theTriangleCelWidtha;
                        v104 = (float)v45;
                        theTriangleCelWidth = v104 * theTriangleCelHeight + 279.0;
                        v42->y = theTriangleCelWidth;
                        v42->u = v139[v47] + v46 * v37;
                        v42->v = v139[v47 + 1] + v104 * v26;
                        v49 = (int)theTriangleCelWidth;
                        v50 = g->mClipRect.mX;
                        if ((int)v48 < v50 || (int)v48 >= v50 + g->mClipRect.mWidth || (v51 = g->mClipRect.mY, v49 < v51) || v49 >= v51 + g->mClipRect.mHeight) {
                            v42->color = 0xFFFFFF;
                        }
                    }
                    v42++;
                    _Ptr += 4;
                    v41 = k;
                    if ((int)_Ptr >= 24)
                        break;
                }
                if (++k >= 3)
                    break;
            }
        }
    }
    if (theIsNight) {
        g->DrawTrianglesTex(*Sexy_IMAGE_POOL_BASE_NIGHT_Addr, v140[0], 150);
        g->DrawTrianglesTex(*Sexy_IMAGE_POOL_SHADING_NIGHT_Addr, v140[1], 150);
    } else {
        g->DrawTrianglesTex(*Sexy_IMAGE_POOL_BASE_Addr, v140[0], 150);
        g->DrawTrianglesTex(*Sexy_IMAGE_POOL_SHADING_Addr, v140[1], 150);
    }
    PoolEffect_UpdateWaterEffect(poolEffect);
    Graphics aPoolG(*g);
    aPoolG.SetWrapMode(0, 0);
    aPoolG.DrawTrianglesTex((Image*)poolEffect->mCausticImage, v140[2], 150);
    aPoolG.SetWrapMode(1, 1);

    if (poolEffect->mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_POOL_PARTY && poolEffect->mApp->mBoard != nullptr) {
        float theTmpTransY = g->mTransY;
        int thePoolOffsetY[2] = {164, -175};
        for (int i = 0; i < 2; ++i) {
            g->mTransY += thePoolOffsetY[i];
            if (theIsNight) {
                g->DrawImage(*Sexy_IMAGE_POOL_NIGHT_Addr, 34, 278);
                g->DrawTrianglesTex(*Sexy_IMAGE_POOL_BASE_NIGHT_Addr, v140[0], 150);
                g->DrawTrianglesTex(*Sexy_IMAGE_POOL_SHADING_NIGHT_Addr, v140[1], 150);
            } else {
                g->DrawImage(*Sexy_IMAGE_POOL_Addr, 34, 278);
                g->DrawTrianglesTex(*Sexy_IMAGE_POOL_BASE_Addr, v140[0], 150);
                g->DrawTrianglesTex(*Sexy_IMAGE_POOL_SHADING_Addr, v140[1], 150);
                TodParticleSystem *aPoolSparkle = poolEffect->mApp->ParticleTryToGet(poolEffect->mApp->mBoard->mPoolSparklyParticleID);
                if (aPoolSparkle != nullptr) {
                    aPoolSparkle->Draw(g);
                }
            }
            Graphics aPoolG2(*g);
            aPoolG2.SetWrapMode(0, 0);
            aPoolG2.DrawTrianglesTex((Image*)poolEffect->mCausticImage, v140[2], 150);
            aPoolG2.SetWrapMode(1, 1);
            g->mTransY = theTmpTransY;
        }
    }
}