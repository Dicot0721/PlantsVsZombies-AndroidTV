//
// Created by 28636 on 2025/6/1.
//

// #include "PvZ/TodLib/Common/TodList.h"
// #include "PvZ/TodLib/Common/TodDebug.h"
#include "PvZ/TodLib/Common/TodCommon.h"
// #include "PvZ/Lawn/LawnApp.h"
// #include "PvZ/TodLib/Effect/EffectSystem.h"
// #include "PvZ/Lawn/Common/Resources.h"
// #include "PvZ/Lawn/Common/GameConstants.h"
// #include "PvZ/SexyAppFramework/Graphics/Font.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
// #include "PvZ/SexyAppFramework/Misc/PerfTimer.h"
// #include "PvZ/SexyAppFramework/Misc/SexyMatrix.h"

using namespace Sexy;

// int RandRangeInt(int theMin, int theMax)
//{
//     return Rand(theMax - theMin + 1) + theMin;
// }
//
// float RandRangeFloat(float theMin, float theMax)
//{
//     return Rand(theMax - theMin) + theMin;
// }

// void TodDrawImageCelF(Graphics* g, Image* theImageStrip, float thePosX, float thePosY, int theCelCol, int theCelRow)
//{
//     int aCelWidth = theImageStrip->GetCelWidth();
//     int aCelHeight = theImageStrip->GetCelHeight();
//     Rect theSrcRect(aCelWidth * theCelCol, aCelHeight * theCelRow, aCelWidth, aCelHeight);
//     g->DrawImageF(theImageStrip, thePosX, thePosY, theSrcRect);
// }