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

#include "PvZ/Lawn/Widget/TrashBin.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Misc.h"

TrashBin::TrashBin(TrashPileType theTrashPileType, float theHeight) {
    __Constructor(theTrashPileType, theHeight);
}

void TrashBin::__Constructor(TrashPileType theTrashPileType, float theHeight) {
    old_TrashBin_TrashBin(this, theTrashPileType, theHeight);

    int thePileNum = theHeight / (theTrashPileType == TrashBin::ZOMBIE_PILE ? zombiePileHeight : plantPileHeight) + 1;
    if (thePileNum >= 10)
        thePileNum = 10;
    DefaultPlayerInfo *mPlayerInfo = ((LawnApp *)*gLawnApp_Addr)->mPlayerInfo;
    int aLevel = mPlayerInfo->mLevel;
    LawnPlayerInfo *aPlayerInfo = reinterpret_cast<LawnPlayerInfo *>(mPlayerInfo);
    if (aPlayerInfo->GetFlag(1) || aPlayerInfo->GetFlag(0x8000))
        aLevel = 50;
    mPileNum = 0;
    for (int i = 0; i < thePileNum; ++i) {
        if (mTrashPileType == TrashBin::PLANT_PILE) {
            mPileImage[i] = GetPlantTrashPiece(aLevel);
        } else {
            mPileImage[i] = GetZombieTrashPiece(aLevel);
        }
        mMirrorPile[i] = RandRangeInt(0, 100) <= 49;
        mPileNum++;
    }
}

Sexy::Image *TrashBin::GetZombieTrashPiece(int theLevel) {
    int v3; // r0
    int v4; // r2
    int v5; // r3
    int v6; // r3

    v3 = RandRangeInt(0, 100);
    v4 = mPileNum;
    if (v4 >= 3)
        v5 = 3;
    else
        v5 = mPileNum;
    if (v3 >= *((uint32_t *)this + v5 + 79) && v3 <= *((uint32_t *)this + v5 + 82)) {
        while (1) {
            v3 = RandRangeInt(0, 100);
            v4 = mPileNum;
            v6 = v4 >= 3 ? 3 : mPileNum;
            if (*((uint32_t *)this + v6 + 79) > v3)
                break;
            if (*((uint32_t *)this + v6 + 82) < v3) {
                if (!v4)
                    goto LABEL_11;
                goto LABEL_16;
            }
        }
    }
    if (v4) {
    LABEL_16:
        if (v4 == 1) {
            if (v3 <= 29) {
                *((uint32_t *)this + 80) = 0;
                *((uint32_t *)this + 83) = 30;
                return addonImages.zombie_pile02_stack02;
            } else if (v3 > 59) {
                *((uint32_t *)this + 80) = 61;
                *((uint32_t *)this + 83) = 100;
                return addonImages.zombie_pile02_stack03;
            } else {
                *((uint32_t *)this + 80) = 31;
                *((uint32_t *)this + 83) = 60;
                return addonImages.zombie_pile02_stack01;
            }
        } else if (v3 <= 29) {
            *((uint32_t *)this + 81) = 0;
            *((uint32_t *)this + 84) = 30;
            return addonImages.zombie_pile03_stack02;
        } else if (v3 <= 59) {
            *((uint32_t *)this + 81) = 31;
            *((uint32_t *)this + 84) = 60;
            return addonImages.zombie_pile03_stack03;
        } else {
            *((uint32_t *)this + 81) = 61;
            *((uint32_t *)this + 84) = 100;
            return addonImages.zombie_pile03_stack01;
        }
    } else {
    LABEL_11:
        if (v3 <= 29) {
            *((uint32_t *)this + 82) = 30;
            *((uint32_t *)this + 79) = v4;
            return addonImages.zombie_pile01_stack02;
        } else if (v3 > 59) {
            *((uint32_t *)this + 79) = 61;
            *((uint32_t *)this + 82) = 100;
            return addonImages.zombie_pile01_stack03;
        } else {
            *((uint32_t *)this + 79) = 31;
            *((uint32_t *)this + 82) = 60;
            return addonImages.zombie_pile01_stack01;
        }
    }
}

Sexy::Image *TrashBin::GetPlantTrashPiece(int theLevel) {
    if (mPileNum != 0) {
        if (mPileNum == 1) {
            if (RandRangeInt(mRandomPileWeight[1], mRandomPileWeight[4]) > 49) {
                mRandomPileWeight[1] = 0;
                mRandomPileWeight[4] = 50;
                return addonImages.plant_pile02_stack02;
            } else {
                mRandomPileWeight[1] = 50;
                mRandomPileWeight[4] = 100;
                return addonImages.plant_pile02_stack01;
            }
        } else if (RandRangeInt(mRandomPileWeight[2], mRandomPileWeight[5]) <= 49) {
            mRandomPileWeight[2] = 50;
            mRandomPileWeight[5] = 100;
            return addonImages.plant_pile03_stack01;
        } else {
            mRandomPileWeight[2] = 0;
            mRandomPileWeight[5] = 50;
            return addonImages.plant_pile03_stack02;
        }
    } else if (RandRangeInt(mRandomPileWeight[0], mRandomPileWeight[3]) > 49) {
        mRandomPileWeight[3] = 50;
        mRandomPileWeight[0] = 0;
        return addonImages.plant_pile01_stack02;
    } else {
        mRandomPileWeight[0] = 50;
        mRandomPileWeight[3] = 100;
        return addonImages.plant_pile01_stack01;
    }
}

void TrashBin::Draw(Sexy::Graphics *g) {
    int thePileHeight = mTrashPileType == TrashBin::ZOMBIE_PILE ? zombiePileHeight : plantPileHeight;
    int thePileNum = mTrashHeight / thePileHeight;
    if (thePileNum >= 10)
        thePileNum = 10;
    if (mTrashPileType == TrashBin::PLANT_PILE) {
        g->DrawImageF(addonImages.plant_can, mX - 22, mY + 15);
    } else {
        g->DrawImageF(addonImages.zombie_can, mX, mY);
    }

    if (thePileNum != 10) {
        g->DrawImageMirror(mPileImage[thePileNum], mX, mHeight % thePileHeight - thePileHeight + mY - thePileHeight * thePileNum, mMirrorPile[thePileNum]);
    }
    if (thePileNum <= 0)
        return;
    for (int i = 0; i < thePileNum; ++i) {
        g->DrawImageMirror(mPileImage[i], mX, mHeight % thePileHeight - thePileHeight + mY - thePileHeight * i, mMirrorPile[i]);
    }

    //    int holder[1];
    //    int holder1[1];
    //    TodStringTranslate(holder,"[TRASH_HEIGHT]");
    //    TodReplaceNumberString(holder1,holder,"{HEIGHT}",(int)trashBin->mTrashHeight);
    //
    //    if (mTrashPileType == ZOMBIE_TRASH_BIN::PLANT_PILE) {
    //        Rect theRect ={317,658,120,50};
    //        Sexy::Font* theFont = *Sexy_FONT_CONTINUUMBOLD14_Addr;
    //        TodDrawStringWrapped(g, holder1, &theRect, theFont, &yellow, DrawStringJustification::DS_ALIGN_CENTER, false);
    //    }else {
    //        Sexy_Graphics_DrawImageF(g, addonImages.zombie_can, trashBin->mX, trashBin->mY);
    //    }
}
