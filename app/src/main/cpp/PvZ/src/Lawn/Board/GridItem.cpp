#include "PvZ/Lawn/Board/GridItem.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Board/Challenge.h"
#include "PvZ/Lawn/Board/ZenGarden.h"
#include "PvZ/Lawn/GamepadControls.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/MagicAddr.h"
#include "PvZ/Misc.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Effect/Reanimator.h"

#include <numbers>

void GridItem::DrawScaryPot(Sexy::Graphics* g) {
    // 修复路灯花照透罐子

    int aImageCol = mGridItemState - GridItemState::GRIDITEM_STATE_SCARY_POT_QUESTION;

    int aXPos = mBoard->GridToPixelX(mGridX, mGridY) - 5;
    int aYPos = mBoard->GridToPixelY(mGridX, mGridY) - 15;
    TodDrawImageCelCenterScaledF(g, *Sexy_IMAGE_PLANTSHADOW2_Addr, aXPos - 5.0, aYPos + 72.0, 0, 1.3, 1.3);

    if (mTransparentCounter > 0) { // 如果罐子要被照透(透明度不为0)
        Sexy_Graphics_DrawImageCel(g, *Sexy_IMAGE_SCARY_POT_Addr, aXPos, aYPos, aImageCol, 0);

        Sexy::Graphics aInsideGraphics;
        Sexy_Graphics_Graphics(&aInsideGraphics, g);
        if (mScaryPotType == ScaryPotType::SCARYPOT_SEED) {
            aInsideGraphics.mScaleX = 0.7f;
            aInsideGraphics.mScaleY = 0.7f;
            DrawSeedPacket(&aInsideGraphics, aXPos + 23, aYPos + 33, mSeedType, SeedType::SEED_NONE, 0.0, 255, false, false, false, true);
        } else if (mScaryPotType == ScaryPotType::SCARYPOT_ZOMBIE) {
            aInsideGraphics.mScaleX = 0.4f;
            aInsideGraphics.mScaleY = 0.4f;
            float theOffsetX = 6.0;
            float theOffsetY = 19.0;
            if (mZombieType == ZombieType::ZOMBIE_FOOTBALL) {
                theOffsetX = 1.0;
                theOffsetY = 16.0;
            } else if (mZombieType == ZombieType::ZOMBIE_GARGANTUAR) {
                theOffsetX = 15.0;
                theOffsetY = 26.0;
                aInsideGraphics.mScaleX = 0.3f;
                aInsideGraphics.mScaleY = 0.3f;
            }
            ReanimatorCache_DrawCachedZombie(mApp->mReanimatorCache, &aInsideGraphics, theOffsetX + aXPos, theOffsetY + aYPos, mZombieType);
        } else if (mScaryPotType == ScaryPotType::SCARYPOT_SUN) {
            int aSuns = mBoard->mChallenge->ScaryPotterCountSunInPot(this);

            Reanimation aReanim;
            Reanimation_Reanimation(&aReanim);
            Reanimation_ReanimationInitializeType(&aReanim, 0.0, 0.0, ReanimationType::REANIM_SUN);
            Reanimation_OverrideScale(&aReanim, 0.5f, 0.5f);
            Reanimation_Update(&aReanim);                                                  // 一次Update是必要的，否则绘制出来是Empty
            aReanim.mFrameStart = (mBoard->mMainCounter / 10) % (aReanim.mFrameCount - 1); // 这行代码可让阳光动起来

            for (int i = 0; i < aSuns; i++) {
                float aOffsetX = 42.0f;
                float aOffsetY = 62.0f;
                switch (i) {
                    case 1:
                        aOffsetX += 3.0f;
                        aOffsetY -= 20.0f;
                        break;
                    case 2:
                        aOffsetX -= 6.0f;
                        aOffsetY -= 10.0f;
                        break;
                    case 3:
                        aOffsetX += 6.0f;
                        aOffsetY -= 5.0f;
                        break;
                    case 4:
                        aOffsetX += 5.0f; // aOffsetY -= 15.0f;          break;
                }

                Reanimation_SetPosition(&aReanim, aXPos + aOffsetX, aYPos + aOffsetY);
                Reanimation_Draw(&aReanim, g);
            }
            Reanimation_Delete2(&aReanim);
        }

        int aAlpha = TodAnimateCurve(0, 50, mTransparentCounter, 255, 58, TodCurves::CURVE_LINEAR);
        Sexy_Graphics_SetColorizeImages(g, true);
        Color aColor = {255, 255, 255, aAlpha};
        Sexy_Graphics_SetColor(g, &aColor);
        Sexy_Graphics_Delete2(&aInsideGraphics);
    }

    Sexy_Graphics_DrawImageCel(g, *Sexy_IMAGE_SCARY_POT_Addr, aXPos, aYPos, aImageCol, 1);
    if (mHighlighted) {
        Sexy_Graphics_SetDrawMode(g, DrawMode::DRAWMODE_ADDITIVE);
        Sexy_Graphics_SetColorizeImages(g, true);
        if (mTransparentCounter == 0) {
            Color aColor = {255, 255, 255, 196};
            Sexy_Graphics_SetColor(g, &aColor);
        }
        Sexy_Graphics_DrawImageCel(g, *Sexy_IMAGE_SCARY_POT_Addr, aXPos, aYPos, aImageCol, 1);
        Sexy_Graphics_SetDrawMode(g, DrawMode::DRAWMODE_NORMAL);
    }

    return Sexy_Graphics_SetColorizeImages(g, false);
}

void GridItem::Update() {
    if (requestPause) {
        return; // 高级暂停
    }

    old_GridItem_Update(this);
}

void GridItem::UpdateScaryPot() {
    old_GridItem_UpdateScaryPot(this);

    if (transparentVase) { // 如果玩家开启“罐子透视”
        if (mTransparentCounter < 50) {
            // 透明度如果小于50，则为透明度加2
            mTransparentCounter += 2;
        }
    }
}

void GridItem::DrawStinky(Sexy::Graphics* g) {
    // 在玩家选取巧克力时，高亮显示光标下方且没喂巧克力的Stinky。
    // 从而修复Stinky无法在醒着时喂巧克力、修复Stinky在喂过巧克力后还能继续喂巧克力。
    // 因为游戏通过Stinky是否高亮来判断是否能喂Stinky。这个机制是为鼠标操作而生，但渡维不加改动地将其用于按键操作，导致无法在Stinky醒着时喂它。
    GamepadControls* aGamePad = mBoard->mGamepadControls1;
    int aCursorX = aGamePad->mCursorPositionX;
    int aCursorY = aGamePad->mCursorPositionY;
    int aCursorGridX = mBoard->PixelToGridX(aCursorX, aCursorY);
    int aCursorGridY = mBoard->PixelToGridY(aCursorX, aCursorY);
    int aStinkyGridX = mBoard->PixelToGridX(mPosX, mPosY);
    int aStinkyGridY = mBoard->PixelToGridY(mPosX, mPosY);
    if (aStinkyGridX != aCursorGridX || aStinkyGridY != aCursorGridY) {
        // 如果Stinky不在光标位置处，则取消高亮。
        mHighlighted = false;
        return old_GridItem_DrawStinky(this, g);
    }
    // 如果Stinky在光标位置处
    CursorObject* aCursorObject = mBoard->mCursorObject1;
    CursorType aCursorType = aCursorObject->mCursorType;
    if (aCursorType == CursorType::CURSOR_TYPE_CHOCOLATE) {
        // 如果光标类型为巧克力
        bool isStinkyHighOnChocolate = ZenGarden_IsStinkyHighOnChocolate(mApp->mZenGarden);
        mHighlighted = !isStinkyHighOnChocolate; // 为没喂巧克力的Stinky加入高亮效果
    }

    return old_GridItem_DrawStinky(this, g);
}

void GridItem::DrawSquirrel(Sexy::Graphics* g) {
    // 绘制松鼠
    float aXPos = mBoard->GridToPixelX(mGridX, mGridY);
    float aYPos = mBoard->GridToPixelY(mGridX, mGridY);
    switch (mGridItemState) {
        case GridItemState::GRIDITEM_STATE_SQUIRREL_PEEKING:
            aYPos += TodAnimateCurve(50, 0, mGridItemCounter, 0, -40.0f, TodCurves::CURVE_BOUNCE_SLOW_MIDDLE);
            break;
        case GridItemState::GRIDITEM_STATE_SQUIRREL_RUNNING_UP:
            aYPos += TodAnimateCurve(50, 0, mGridItemCounter, 100, 0.0f, TodCurves::CURVE_EASE_IN);
            break;
        case GridItemState::GRIDITEM_STATE_SQUIRREL_RUNNING_DOWN:
            aYPos += TodAnimateCurve(50, 0, mGridItemCounter, -100, 0.0f, TodCurves::CURVE_EASE_IN);
            break;
        case GridItemState::GRIDITEM_STATE_SQUIRREL_RUNNING_LEFT:
            aXPos += TodAnimateCurve(50, 0, mGridItemCounter, 80, 0.0f, TodCurves::CURVE_EASE_IN);
            break;
        case GridItemState::GRIDITEM_STATE_SQUIRREL_RUNNING_RIGHT:
            aXPos += TodAnimateCurve(50, 0, mGridItemCounter, -80, 0.0f, TodCurves::CURVE_EASE_IN);
            break;
    }

    Sexy_Graphics_DrawImage(g, addonImages.squirrel, aXPos, aYPos);
}

void GridItem::DrawCrater(Sexy::Graphics* g) {
    // 绘制屋顶月夜弹坑
    float aXPos = mBoard->GridToPixelX(mGridX, mGridY) - 8.0f;
    float aYPos = mBoard->GridToPixelY(mGridX, mGridY) + 40.0f;
    if (mGridItemCounter < 25) {
        int aAlpha = TodAnimateCurve(25, 0, mGridItemCounter, 255, 0, TodCurves::CURVE_LINEAR);
        Color color = {255, 255, 255, aAlpha};
        Sexy_Graphics_SetColor(g, &color);
        Sexy_Graphics_SetColorizeImages(g, true);
    }

    bool fading = mGridItemCounter < 9000;
    Sexy::Image* aImage = *Sexy_IMAGE_CRATER_Addr;
    int theCelCol = 0;

    if (mBoard->IsPoolSquare(mGridX, mGridY)) {
        if (Board_StageIsNight(mBoard)) {
            aImage = *Sexy_IMAGE_CRATER_WATER_NIGHT_Addr;
        } else {
            aImage = *Sexy_IMAGE_CRATER_WATER_DAY_Addr;
        }
        if (fading) {
            theCelCol = 1;
        }
        float aPos = mGridY * std::numbers::pi_v<float> + mGridX * std::numbers::pi_v<float> * 0.25f;
        float aTime = mBoard->mMainCounter * std::numbers::pi_v<float> * 2.0f / 200.0f;
        aYPos = sin(aPos + aTime) * 2.0f;
    } else if (Board_StageHasRoof(mBoard)) {
        if (mGridX < 5) {
            if (Board_StageIsNight(mBoard)) {
                aImage = addonImages.crater_night_roof_left;
            } else {
                aImage = *Sexy_IMAGE_CRATER_ROOF_LEFT_Addr;
            }
            aXPos += 16.0f;
            aYPos += -16.0f;
        } else {
            if (Board_StageIsNight(mBoard)) {
                aImage = addonImages.crater_night_roof_center;
            } else {
                aImage = *Sexy_IMAGE_CRATER_ROOF_CENTER_Addr;
            }
            aXPos += 18.0f;
            aYPos += -9.0f;
        }
        if (fading) {
            theCelCol = 1;
        }
    } else if (Board_StageIsNight(mBoard)) {
        theCelCol = 1;
        if (fading) {
            aImage = *Sexy_IMAGE_CRATER_FADING_Addr;
        }
    } else if (fading) {
        aImage = *Sexy_IMAGE_CRATER_FADING_Addr;
    }

    TodDrawImageCelF(g, aImage, aXPos, aYPos, theCelCol, 0);
    Sexy_Graphics_SetColorizeImages(g, false);
}
