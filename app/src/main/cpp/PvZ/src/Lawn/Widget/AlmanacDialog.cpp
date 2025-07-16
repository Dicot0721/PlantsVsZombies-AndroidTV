#include "PvZ/Lawn/Widget/AlmanacDialog.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Board/Plant.h"
#include "PvZ/Lawn/Board/SeedPacket.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Misc.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/SexyAppFramework/Widget/GameButton.h"
#include "PvZ/Symbols.h"
#include "PvZ/Lawn/System/PoolEffect.h"
#include "PvZ/TodLib/Common/TodStringFile.h"

#include <cstddef>

#include <numbers>

using namespace Sexy;

namespace {

constexpr int theBackId = 110;
constexpr int theCloseId = 111;

// ButtonX和ButtonY是按钮的左上角坐标.
constexpr int mAlmanacBackButtonX = -170;
constexpr int mAlmanacBackButtonY = 520;
constexpr int mAlmanacCloseButtonX = 800;
constexpr int mAlmanacCloseButtonY = 520;

// 按钮长度和宽度为150和50，两个按钮都读取此长宽.
constexpr int mAlmanacButtonWidth = 170;
constexpr int mAlmanacButtonHeight = 50;

constexpr int mAlmanacPlantRectX = 521 - 8;
constexpr int mAlmanacPlantRectY = 107 + 20;

Sexy::GameButton *gAlmanacBackButton;
Sexy::GameButton *gAlmanacCloseButton;

} // namespace

void AlmanacDialog::Create(LawnApp *theApp) {
    // TODO: 解决部分植物的介绍文本显示不全问题

    old_AlmanacDialog_AlmanacDialog(this, theApp);

    pvzstl::string str{};
    TodStringTranslate(str, "[ALMANAC_INDEX]");
    gAlmanacBackButton = MakeButton(theBackId, &this->mButtonListener, this, (SexyString &)str);
    gAlmanacBackButton->Resize(0, 0, 0, 0);
    gAlmanacBackButton->mBtnNoDraw = true;
    gAlmanacBackButton->mDisabled = true;

    pvzstl::string str1{};
    TodStringTranslate(str1, "[CLOSE]");
    gAlmanacCloseButton = MakeButton(theCloseId, &this->mButtonListener, this, (SexyString &)str1);
    gAlmanacCloseButton->Resize(mAlmanacCloseButtonX, mAlmanacCloseButtonY, mAlmanacButtonWidth, mAlmanacButtonHeight);
    AddWidget(gAlmanacBackButton);
    AddWidget(gAlmanacCloseButton);


    //   为泳池背景加入PoolEffect。这里挖空背景图，挖出一块透明方形
    Sexy::Image *gPlantBackImage = *Sexy_IMAGE_ALMANAC_PLANTBACK_Addr;
    Sexy::Image *gPoolBackImage = *Sexy_IMAGE_ALMANAC_GROUNDNIGHTPOOL_Addr;
    Sexy::Rect blankRect = {mAlmanacPlantRectX + 240, mAlmanacPlantRectY + 60, gPoolBackImage->mWidth, gPoolBackImage->mHeight};
    reinterpret_cast<MemoryImage *>(gPlantBackImage)->ClearRect(blankRect);
}

void AlmanacDialog::Destroy() {
    old_AlmanacDialog_Delete2(this);

    gAlmanacBackButton->Destroy();
    gAlmanacCloseButton->Destroy();
    gAlmanacBackButton = nullptr;
    gAlmanacCloseButton = nullptr;
}

void AlmanacDialog::SetPage(AlmanacPage thePage) {
    // 修复点击气球僵尸进植物图鉴、点击介绍文字进植物图鉴
    if (thePage != AlmanacPage::ALMANAC_PAGE_INDEX) {
        // 在前往其他图鉴页面时，显示返回按钮
        if (gAlmanacBackButton != nullptr) {
            gAlmanacBackButton->Resize(mAlmanacBackButtonX, mAlmanacBackButtonY, mAlmanacButtonWidth, mAlmanacButtonHeight);
            gAlmanacBackButton->mBtnNoDraw = false;
            gAlmanacBackButton->mDisabled = false;
        }
        // 在前往其他图鉴页面时，将按钮缩小为0x0
        mPlantButton->Resize(0, 0, 0, 0);
        mZombieButton->Resize(0, 0, 0, 0);
    } else {
        // 回到图鉴首页时，将返回按钮禁用
        if (gAlmanacBackButton != nullptr) {
            gAlmanacBackButton->Resize(0, 0, 0, 0);
            gAlmanacBackButton->mBtnNoDraw = true;
            gAlmanacBackButton->mDisabled = true;
        }
        // 回到图鉴首页时，将按钮恢复为正常大小
        mPlantButton->Resize(130, 345, 156, 42);
        mZombieButton->Resize(487, 345, 210, 48);
    }

    return old_AlmanacDialog_SetPage(this, thePage);
}


namespace {
int mAlmanacDialogTouchDownY;
bool isTouchDownInTextRect;
Sexy::Rect mTextRect = {482, 360, 258, 173};
} // namespace

void AlmanacDialog::MouseDown(int x, int y, int a4) {
    // 修复点击气球僵尸进植物图鉴、点击介绍文字进植物图鉴
    if (mOpenPage == 0) {
        // 如果当前的Page是Index Page
        if (mPlantButton->IsMouseOver())
            LawnApp_PlaySample(mApp, *Sexy_SOUND_GRAVEBUTTON_Addr);
        if (mZombieButton->IsMouseOver())
            LawnApp_PlaySample(mApp, *Sexy_SOUND_GRAVEBUTTON_Addr);
        return;
    } else if (TRect_Contains(&mTextRect, x, y)) {
        isTouchDownInTextRect = true;
        mAlmanacDialogTouchDownY = y;
    }

    SeedType seedType = SeedHitTest(x, y);
    if (seedType != SeedType::SEED_NONE && seedType != mSelectedSeed) {
        mSelectedSeed = seedType;
        SetupPlant();
        LawnApp_PlaySample(mApp, *Sexy_SOUND_TAP_Addr);
    }
    ZombieType zombieType = ZombieHitTest(x, y);
    if (zombieType != -1 && zombieType != mSelectedZombie) {
        mSelectedZombie = zombieType;
        SetupZombie();
        LawnApp_PlaySample(mApp, *Sexy_SOUND_TAP_Addr);
    }
}

void AlmanacDialog::MouseDrag(int x, int y) {
    // 滚动图鉴文字

    if (isTouchDownInTextRect && mAlmanacDialogTouchDownY != y) {
        (*(void (**)(Sexy::Widget *, uint32_t, double))(*(uint32_t *)mScrollTextView + 500))(
            (Widget *)mScrollTextView, *(uint32_t *)(*(uint32_t *)mScrollTextView + 500), *((double *)mScrollTextView + 35) + 0.6 * (mAlmanacDialogTouchDownY - y));
        mAlmanacDialogTouchDownY = y;
    }
}

void AlmanacDialog::MouseUp(int x, int y, int theClickCount) {
    // 空函数替换，修复点击图鉴Index界面中任何位置都会跳转植物图鉴的问题
    isTouchDownInTextRect = false;
}

void AlmanacDialog::RemovedFromManager(WidgetManager *theWidgetManager) {
    // 记录当前游戏状态
    old_AlmanacDialog_RemovedFromManager(this, theWidgetManager);

    RemoveWidget(gAlmanacBackButton);
    RemoveWidget(gAlmanacCloseButton);
}

void AlmanacDialog::ButtonDepress(int theId) {
    if (theId == 0) {
        SetPage(AlmanacPage::ALMANAC_PAGE_PLANTS);
    } else if (theId == 1) {
        SetPage(AlmanacPage::ALMANAC_PAGE_ZOMBIES);
    } else if (theId == theBackId) {
        KeyDown(KeyCode::KEYCODE_ESCAPE);
    } else if (theId == theCloseId) {
        mApp->KillAlmanacDialog();
    }
}

void AlmanacDialog::DrawPlants_Unmodified(Sexy::Graphics *g) {
    // old_AlmanacDialog_DrawPlants(almanacDialog,g);

    // TODO:解决PoolEffect图层问题，和部分植物的介绍文本显示不全问题
    g->DrawImage(*Sexy_IMAGE_ALMANAC_PLANTBACK_Addr, -240, -60);
    pvzstl::string aHeaderText{};
    TodStringTranslate(aHeaderText, "[SUBURBAN_ALMANAC_PLANTS]");
    Color aHeaderColor = {213, 159, 43, 255};
    TodDrawString(g, aHeaderText, 400, 50, *Sexy_FONT_HOUSEOFTERROR20_Addr, aHeaderColor, DrawStringJustification::DS_ALIGN_CENTER);
    int theAlpha = std::sin((mUpdateCnt % 100) * 0.01 * std::numbers::pi) * 255.0;
    int x, y;
    for (SeedType aSeedType = SeedType::SEED_PEASHOOTER; aSeedType < SeedType::NUM_SEEDS_IN_CHOOSER; aSeedType = (SeedType)(aSeedType + 1)) {
        GetSeedPosition(aSeedType, x, y);
        if (aSeedType == SeedType::SEED_IMITATER) {
            bool tmp = g->GetColorizeImages();
            g->SetColorizeImages(true);
            if (mSelectedSeed == SeedType::SEED_IMITATER) {
                Color v39 = {255, 255, 0, theAlpha};
                g->SetColor(v39);
            } else {
                Color v39 = {255, 255, 255, 64};
                g->SetColor(v39);
            }
            g->DrawImage(*Sexy_IMAGE_ALMANAC_IMITATER_Addr, 18, 20);
            g->SetColor(white);
            g->SetColorizeImages(tmp);
        } else {
            if (mSelectedSeed == aSeedType) {
                g->SetScale(1.1, 1.1, x, y);
                DrawSeedPacket(g, x - 2, y - 4, mSelectedSeed, SeedType::SEED_NONE, 0.0, 255, true, false, false, true);
                bool tmp = g->GetColorizeImages();
                g->SetColorizeImages(true);
                Color v39 = {255, 255, 0, theAlpha};
                g->SetColor(v39);
                g->DrawImage(*Sexy_IMAGE_SEEDPACKETFLASH_Addr, x - 3, y - 5);
                g->SetColor(white);
                g->SetColorizeImages(tmp);
                g->SetScale(1.0, 1.0, 0.0, 0.0);
            } else {
                DrawSeedPacket(g, x, y, (SeedType)aSeedType, SeedType::SEED_NONE, 0.0, 255, true, false, false, true);
            }
        }
    }

    if (Plant::IsAquatic(mSelectedSeed)) {
        if (Plant::IsNocturnal(mSelectedSeed)) {
            g->DrawImage(*Sexy_IMAGE_ALMANAC_GROUNDNIGHTPOOL_Addr, mAlmanacPlantRectX, mAlmanacPlantRectY);
            if (mApp->Is3DAccelerated()) {
                g->SetClipRect(475, 0, 397, 500);
                g->mTransY = g->mTransY - 145.0f;
                PoolEffect_PoolEffectDraw(mApp->mPoolEffect, g, true);
                g->mTransY = g->mTransY + 145.0f;
                g->ClearClipRect();
            }
        } else {
            g->DrawImage(*Sexy_IMAGE_ALMANAC_GROUNDPOOL_Addr, mAlmanacPlantRectX, mAlmanacPlantRectY);
            if (mApp->Is3DAccelerated()) {
                g->SetClipRect(475, 0, 397, 500);
                g->mTransY = g->mTransY - 145.0f;
                PoolEffect_PoolEffectDraw(mApp->mPoolEffect, g, false);
                g->mTransY = g->mTransY + 145.0f;
                g->ClearClipRect();
            }
        }
    } else if (Plant::IsNocturnal(mSelectedSeed) || mSelectedSeed == SeedType::SEED_GRAVEBUSTER || mSelectedSeed == SeedType::SEED_PLANTERN) {
        g->DrawImage(*Sexy_IMAGE_ALMANAC_GROUNDNIGHT_Addr, mAlmanacPlantRectX, mAlmanacPlantRectY);
    } else if (mSelectedSeed == SeedType::SEED_FLOWERPOT) {
        g->DrawImage(*Sexy_IMAGE_ALMANAC_GROUNDROOF_Addr, mAlmanacPlantRectX, mAlmanacPlantRectY);
    } else {
        g->DrawImage(*Sexy_IMAGE_ALMANAC_GROUNDDAY_Addr, mAlmanacPlantRectX, mAlmanacPlantRectY);
    }


    if (mPlant != nullptr) {
        g->PushState();;
        g->mTransX = g->mTransX + mPlant->mX;
        g->mTransY = g->mTransY + mPlant->mY;
        mPlant->Draw(g);
        g->PopState();
    }

    g->DrawImage(*Sexy_IMAGE_ALMANAC_PLANTCARD_Addr, 459, 80);

    Color color = {213, 159, 43, 255};
    TodDrawString(g, mNameString, 617, 108, *Sexy_FONT_DWARVENTODCRAFT18_Addr, color, DrawStringJustification::DS_ALIGN_CENTER);

    if (mSelectedSeed != SeedType::SEED_IMITATER) {
        TodDrawStringWrapped(g, mCostString, mCostRect, *Sexy_FONT_BRIANNETOD16_Addr, white, DrawStringJustification::DS_ALIGN_LEFT, false);
        TodDrawStringWrapped(g, mWaitTimeString, mWaitTimeRect, *Sexy_FONT_BRIANNETOD16_Addr, white, DrawStringJustification::DS_ALIGN_RIGHT, false);
    }

    g->PushState();;
    g->ClipRect(mDescriptionRect.mX, mDescriptionRect.mY - 14, mDescriptionRect.mWidth, mDescriptionRect.mHeight + 8);
    float v22 = mScrollTextView->mValue * 0.01 * mDescriptionRect.mY;
    float v23 = g->mTransY + 2.0 - v22;
    *(float *)unk2 = -v22;
    g->mTransY = v23;
    Color v39 = {143, 67, 27, 255};
    TodDrawStringWrappedHelper(g, mDescriptionString, mDescriptionRect, *Sexy_FONT_BRIANNETOD16_Addr, v39, DrawStringJustification::DS_ALIGN_LEFT, true, true);
    g->PopState();
}

void AlmanacDialog::DrawPlants(Sexy::Graphics *g) {
    //    return old_AlmanacDialog_DrawPlants(almanacDialog,g);
    //   为泳池背景加入PoolEffect。此函数改变了原版绘制顺序，将背景图放在泳池的后面绘制

    if (Plant::IsAquatic(mSelectedSeed)) {
        if (Plant::IsNocturnal(mSelectedSeed)) {
            g->DrawImage(*Sexy_IMAGE_ALMANAC_GROUNDNIGHTPOOL_Addr, mAlmanacPlantRectX, mAlmanacPlantRectY + 10);
            if (mApp->Is3DAccelerated()) {
                //                Sexy_Graphics_SetClipRect(g, 475, 0, 397, 500);
                g->mTransY = g->mTransY - 115;
                PoolEffect_PoolEffectDraw(mApp->mPoolEffect, g, true);
                g->mTransY = g->mTransY + 115;
                //                Sexy_Graphics_ClearClipRect(g);
            }
        } else {
            g->DrawImage(*Sexy_IMAGE_ALMANAC_GROUNDPOOL_Addr, mAlmanacPlantRectX, mAlmanacPlantRectY + 10);
            if (mApp->Is3DAccelerated()) {
                //                Sexy_Graphics_SetClipRect(g, 475, 0, 397, 500);
                g->mTransY = g->mTransY - 115;
                PoolEffect_PoolEffectDraw(mApp->mPoolEffect, g, false);
                g->mTransY = g->mTransY + 115;
                //                Sexy_Graphics_ClearClipRect(g);
            }
        }
    } else if (Plant::IsNocturnal(mSelectedSeed) || mSelectedSeed == SeedType::SEED_GRAVEBUSTER || mSelectedSeed == SeedType::SEED_PLANTERN) {
        g->DrawImage(*Sexy_IMAGE_ALMANAC_GROUNDNIGHT_Addr, mAlmanacPlantRectX, mAlmanacPlantRectY);
    } else if (mSelectedSeed == SeedType::SEED_FLOWERPOT) {
        g->DrawImage(*Sexy_IMAGE_ALMANAC_GROUNDROOF_Addr, mAlmanacPlantRectX + 10, mAlmanacPlantRectY + 12);
    } else {
        g->DrawImage(*Sexy_IMAGE_ALMANAC_GROUNDDAY_Addr, mAlmanacPlantRectX, mAlmanacPlantRectY);
    }

    g->DrawImage(*Sexy_IMAGE_ALMANAC_PLANTBACK_Addr, -240, -60);
    pvzstl::string aHeaderText{};
    TodStringTranslate(aHeaderText, "[SUBURBAN_ALMANAC_PLANTS]");
    Color aHeaderColor = {213, 159, 43, 255};
    TodDrawString(g, aHeaderText, 400, 50, *Sexy_FONT_HOUSEOFTERROR20_Addr, aHeaderColor, DrawStringJustification::DS_ALIGN_CENTER);

    int theAlpha = sin((mUpdateCnt % 100) * 0.01 * std::numbers::pi) * 255.0;
    int x, y;
    for (SeedType aSeedType = SeedType::SEED_PEASHOOTER; aSeedType < SeedType::NUM_SEEDS_IN_CHOOSER; aSeedType = (SeedType)(aSeedType + 1)) {
        GetSeedPosition(aSeedType, x, y);
        if (aSeedType == SeedType::SEED_IMITATER) {
            bool tmp = g->GetColorizeImages();
            g->SetColorizeImages(true);
            if (mSelectedSeed == SeedType::SEED_IMITATER) {
                Color v39 = {255, 255, 0, theAlpha};
                g->SetColor(v39);
            } else {
                Color v39 = {255, 255, 255, 64};
                g->SetColor(v39);
            }
            g->DrawImage(*Sexy_IMAGE_ALMANAC_IMITATER_Addr, 18, 20);
            g->SetColor(white);
            g->SetColorizeImages(tmp);
        } else {
            if (mSelectedSeed == aSeedType) {
                g->SetScale(1.1, 1.1, x, y);
                DrawSeedPacket(g, x - 2, y - 4, mSelectedSeed, SeedType::SEED_NONE, 0.0, 255, true, false, false, true);
                bool tmp = g->GetColorizeImages();
                g->SetColorizeImages(true);
                Color v39 = {255, 255, 0, theAlpha};
                g->SetColor(v39);
                g->DrawImage(*Sexy_IMAGE_SEEDPACKETFLASH_Addr, x - 3, y - 5);
                g->SetColor(white);
                g->SetColorizeImages(tmp);
                g->SetScale(1.0, 1.0, 0.0, 0.0);
            } else {
                DrawSeedPacket(g, x, y, (SeedType)aSeedType, SeedType::SEED_NONE, 0.0, 255, true, false, false, true);
            }
        }
    }

    if (mPlant != nullptr) {
        g->PushState();;
        g->mTransX = g->mTransX + mPlant->mX;
        g->mTransY = g->mTransY + mPlant->mY;
        mPlant->Draw(g);
        g->PopState();
    }

    g->DrawImage(*Sexy_IMAGE_ALMANAC_PLANTCARD_Addr, 459, 80);

    Color color = {213, 159, 43, 255};
    TodDrawString(g, mNameString, 617, 108, *Sexy_FONT_DWARVENTODCRAFT18_Addr, color, DrawStringJustification::DS_ALIGN_CENTER);

    if (mSelectedSeed != SeedType::SEED_IMITATER) {
        TodDrawStringWrapped(g, mCostString, mCostRect, *Sexy_FONT_BRIANNETOD16_Addr, white, DrawStringJustification::DS_ALIGN_LEFT, false);
        TodDrawStringWrapped(g, mWaitTimeString, mWaitTimeRect, *Sexy_FONT_BRIANNETOD16_Addr, white, DrawStringJustification::DS_ALIGN_RIGHT, false);
    }

    g->PushState();;
    g->ClipRect(mDescriptionRect.mX, mDescriptionRect.mY - 14, mDescriptionRect.mWidth, mDescriptionRect.mHeight + 8);
    float v22 = mScrollTextView->mValue * 0.01 * mDescriptionRect.mY;
    float v23 = g->mTransY + 2.0 - v22;
    *(float *)unk2 = -v22;
    g->mTransY = v23;
    Color v39 = {143, 67, 27, 255};
    TodDrawStringWrappedHelper(g, mDescriptionString, mDescriptionRect, *Sexy_FONT_BRIANNETOD16_Addr, v39, DrawStringJustification::DS_ALIGN_LEFT, true, true);
    g->PopState();
}

void AlmanacDialog::SetupLayoutPlants(Sexy::Graphics *g) {
    // 修复介绍文字过长时的显示不全
    old_AlmanacDialog_SetupLayoutPlants(this, g);

    if (unk2[1] > 398) {
        // 文字过长
        unk2[1] *= 1.15;
        mScrollTextView->SetMaxValue(115);
    } else {
        mScrollTextView->SetMaxValue(100);
    }
}
