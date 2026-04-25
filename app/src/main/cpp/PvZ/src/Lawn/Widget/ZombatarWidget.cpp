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

#include "PvZ/Lawn/Widget/ZombatarWidget.h"
#include "Homura/Logger.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Zombie.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/Widget/MainMenu.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/SexyAppFramework/Graphics/MemoryImage.h"
#include "PvZ/SexyAppFramework/Misc/KeyCodes.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Common/TodStringFile.h"
#include "PvZ/TodLib/Effect/Attachment.h"
#include "PvZ/TodLib/Effect/Reanimator.h"

#include <algorithm>
#include <cstddef>
#include <type_traits>
#include <utility>

using namespace Sexy;

namespace {

using AddonZombatarImagesType = std::remove_reference_t<decltype(addonZombatarImages)>;
using AddonImageMember = Image *AddonZombatarImagesType::*;

template <size_t N>
Image *GetAddonImageByIndex(int index, const AddonImageMember (&table)[N]) {
    if (index < 0 || index >= static_cast<int>(N)) {
        return nullptr;
    }
    AddonImageMember member = table[index];
    return member == nullptr ? nullptr : addonZombatarImages.*member;
}

template <size_t N>
int GetIntByIndex(int index, const int (&table)[N], int defaultValue = 0) {
    if (index < 0 || index >= static_cast<int>(N)) {
        return defaultValue;
    }
    return table[index];
}

template <size_t N>
int GetSparseIntByIndex(int index, const std::pair<int, int> (&table)[N], int defaultValue = 0) {
    for (const auto &[k, v] : table) {
        if (k == index) {
            return v;
        }
    }
    return defaultValue;
}

template <size_t N>
int GetAddonDimensionOffsetByIndex(int index, const AddonImageMember (&table)[N], const int (&delta)[N], bool width, int defaultValue = 0) {
    if (index < 0 || index >= static_cast<int>(N)) {
        return defaultValue;
    }
    Image *image = addonZombatarImages.*table[index];
    return (width ? -image->mWidth : -image->mHeight) + delta[index];
}

} // namespace

ZombatarWidget::ZombatarWidget(LawnApp *theApp) {
    new (this) TestMenuWidget{};
    theApp->LoadZombatarResources();
    theApp->Load("DelayLoad_Almanac");
    mApp = theApp;
    mBackButton = MakeButton(ZombatarWidget::ZombatarWidget_Back, mButtonListener, this, "[CLOSE]");
    mBackButton->Resize(471, 628, addonZombatarImages.zombatar_mainmenuback_highlight->mWidth, addonZombatarImages.zombatar_mainmenuback_highlight->mHeight);
    AddWidget(mBackButton);
    mBackButton->mDrawStoneButton = false;
    mBackButton->mButtonImage = IMAGE_BLANK;
    mBackButton->mDownImage = addonZombatarImages.zombatar_mainmenuback_highlight;
    mBackButton->mOverImage = addonZombatarImages.zombatar_mainmenuback_highlight;

    mFinishButton = MakeButton(ZombatarWidget::ZombatarWidget_Finish, mButtonListener, nullptr, "[OK]");
    mFinishButton->Resize(160 + 523, 565, addonZombatarImages.zombatar_finished_button->mWidth, addonZombatarImages.zombatar_finished_button->mHeight);
    AddWidget(mFinishButton);
    mFinishButton->mDrawStoneButton = false;
    mFinishButton->mButtonImage = addonZombatarImages.zombatar_finished_button;
    mFinishButton->mDownImage = addonZombatarImages.zombatar_finished_button_highlight;
    mFinishButton->mOverImage = addonZombatarImages.zombatar_finished_button_highlight;

    mViewPortraitButton = MakeButton(ZombatarWidget::ZombatarWidget_ViewPortrait, mButtonListener, nullptr, "[OK]");
    mViewPortraitButton->Resize(160 + 75, 565, addonZombatarImages.zombatar_view_button->mWidth, addonZombatarImages.zombatar_view_button->mHeight);
    AddWidget(mViewPortraitButton);
    mViewPortraitButton->mDrawStoneButton = false;
    mViewPortraitButton->mButtonImage = addonZombatarImages.zombatar_view_button;
    mViewPortraitButton->mDownImage = addonZombatarImages.zombatar_view_button_highlight;
    mViewPortraitButton->mOverImage = addonZombatarImages.zombatar_view_button_highlight;

    mNewButton = MakeButton(ZombatarWidget::ZombatarWidget_New, mButtonListener, nullptr, "[ZOMBATAR_NEW_BUTTON]");
    mNewButton->Resize(578, 490, 170, 50);
    AddWidget(mNewButton);

    mDeleteButton = MakeButton(ZombatarWidget::ZombatarWidget_Delete, mButtonListener, nullptr, "[ZOMBATAR_DELETE_BUTTON]");
    mDeleteButton->Resize(314, 490, 170, 50);
    AddWidget(mDeleteButton);

    auto *aZombie = new Zombie;
    aZombie->mBoard = nullptr;
    aZombie->ZombieInitialize(0, ZombieType::ZOMBIE_FLAG, false, nullptr, -3, true);
    Reanimation *aBodyReanim = aZombie->mApp->ReanimationGet(aZombie->mBodyReanimID);
    ReanimatorTrackInstance *aHeadTrackInstance = aBodyReanim->GetTrackInstanceByName("anim_head1");
    aHeadTrackInstance->mImageOverride = IMAGE_BLANK;

    Reanimation *aZombatarHeadReanim = theApp->AddReanimation(0, 0, 0, ReanimationType::REANIM_ZOMBATAR_HEAD);
    aZombatarHeadReanim->PlayReanim("anim_head_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0);
    aZombatarHeadReanim->AssignRenderGroupToTrack("anim_hair", -1);
    aZombie->mBossFireBallReanimID = aZombie->mApp->ReanimationGetID(aZombatarHeadReanim);
    AttachEffect *attachEffect = AttachReanim(aHeadTrackInstance->mAttachmentID, aZombatarHeadReanim, 0.0f, 0.0f);
    TodScaleRotateTransformMatrix((SexyMatrix3 &)attachEffect->mOffset, -20.0, -1.0, 0.2, 1.0, 1.0);
    mZombatarReanim = aZombatarHeadReanim;
    aZombie->ReanimShowPrefix("anim_hair", -1);
    aZombie->ReanimShowPrefix("anim_head2", -1);
    aZombie->Update();
    mPreviewZombie = aZombie;

    mShowExistingZombatarPortrait = addonImages.zombatar_portrait != nullptr;

    Reanimation_SetZombatarReanim(mZombatarReanim);
}

ZombatarWidget::~ZombatarWidget() {
    // TODO:解决五个按钮的内存泄露问题。GameButton_Delete会闪退，暂不清楚原因。
    mPreviewZombie->DieNoLoot();
    (*((void (**)(Zombie *))mPreviewZombie->vTable + 1))(mPreviewZombie); // Delete();

    (*((void (**)(Sexy::Widget *, Sexy::Widget *))vTable + 7))(this, mBackButton);
    mBackButton->mDrawStoneButton = true;
    mBackButton->mButtonImage = nullptr;
    mBackButton->mDownImage = nullptr;
    mBackButton->mOverImage = nullptr;
    // GameButton_Delete(mBackButton);

    (*((void (**)(Sexy::Widget *, Sexy::Widget *))vTable + 7))(this, mFinishButton);
    mFinishButton->mDrawStoneButton = true;
    mFinishButton->mButtonImage = nullptr;
    mFinishButton->mDownImage = nullptr;
    mFinishButton->mOverImage = nullptr;
    // GameButton_Delete(mFinishButton);

    (*((void (**)(Sexy::Widget *, Sexy::Widget *))vTable + 7))(this, mViewPortraitButton);
    mViewPortraitButton->mDrawStoneButton = true;
    mViewPortraitButton->mButtonImage = nullptr;
    mViewPortraitButton->mDownImage = nullptr;
    mViewPortraitButton->mOverImage = nullptr;
    // GameButton_Delete(mViewPortraitButton);

    // GameButton_Delete(mNewButton);
    // GameButton_Delete(mDeleteButton);
    old_TestMenuWidget_Delete2(this);
}

void ZombatarWidget::Delete() {
    delete this;
}

void ZombatarWidget::RemovedFromManager(WidgetManager *theWidgetManager) {
    old_TestMenuWidget_RemovedFromManager(this, theWidgetManager);

    RemoveWidget(mBackButton);
    RemoveWidget(mFinishButton);
    RemoveWidget(mViewPortraitButton);
    RemoveWidget(mNewButton);
    RemoveWidget(mDeleteButton);
}

void ZombatarWidget::SetDefault() {
    mSelectedTab = 0;
    mSelectedSkinColor = 0;
    mSelectedHair = 255;
    mSelectedHairColor = 255;
    mSelectedFHair = 255;
    mSelectedFHairColor = 255;
    mSelectedFHairPage = 0;
    mSelectedTidBit = 255;
    mSelectedTidBitColor = 255;
    mSelectedEyeWear = 255;
    mSelectedEyeWearColor = 255;
    mSelectedCloth = 255;
    mSelectedAccessory = 255;
    mSelectedAccessoryColor = 255;
    mSelectedHat = 255;
    mSelectedHatColor = 255;
    mSelectedBackground = 0;
    mSelectedBackgroundColor = 255;
    mSelectedBackgroundPage = 0;
    Reanimation_SetZombatarReanim(mZombatarReanim);
}

void ZombatarWidget::Update() {
    mPreviewZombie->Update();
    mFinishButton->mDisabled = mShowExistingZombatarPortrait;
    mFinishButton->mBtnNoDraw = mShowExistingZombatarPortrait;
    mViewPortraitButton->mDisabled = mShowExistingZombatarPortrait || addonImages.zombatar_portrait == nullptr;
    mViewPortraitButton->mBtnNoDraw = mShowExistingZombatarPortrait || addonImages.zombatar_portrait == nullptr;
    mNewButton->mDisabled = !mShowExistingZombatarPortrait;
    mNewButton->mBtnNoDraw = !mShowExistingZombatarPortrait;
    mDeleteButton->mDisabled = !mShowExistingZombatarPortrait;
    mDeleteButton->mBtnNoDraw = !mShowExistingZombatarPortrait;
    MarkDirty();
}

void ZombatarWidget::DrawZombieSelection(Sexy::Graphics *g) {
    // TODO: 做僵尸选择功能
    [[maybe_unused]] ZombieType types[] = {
        ZombieType::ZOMBIE_NORMAL,
        ZombieType::ZOMBIE_FLAG,
        ZombieType::ZOMBIE_TRAFFIC_CONE,
        ZombieType::ZOMBIE_DOOR,
        ZombieType::ZOMBIE_TRASHCAN,
        ZombieType::ZOMBIE_PAIL,
        ZombieType::ZOMBIE_DUCKY_TUBE,
    };
}

void ZombatarWidget::DrawSkin(Sexy::Graphics *graphics) {
    for (int i = 0; i < 12; ++i) {
        int theX = 160 + 285 + (i % 9) * 30;
        int theY = 432 + i / 9 * 30;
        Color color = gZombatarSkinColor[i];
        if (mSelectedSkinColor != i) {
            color.mAlpha = 64;
        }
        Sexy_Graphics_DrawImageColorized(graphics, addonZombatarImages.zombatar_colorpicker, &color, theX, theY);
    }

    Sexy::Rect rect = {160 + 295, 211, 250, 100};
    Sexy::Font *font = Sexy::FONT_DWARVENTODCRAFT18;
    TodDrawStringWrapped(graphics, "[ZOMBATAR_START_TEXT]", rect, font, gColorYellow, DrawStringJustification::DS_ALIGN_CENTER, false);
}

void ZombatarWidget::DrawHair(Sexy::Graphics *g) {
    Color theAlphaColor = {255, 255, 255, 64};
    for (int i = 0; i < 16; ++i) {
        int theX = 160 + 198 + (i % 6) * 73;
        int theY = 162 + i / 6 * 79;
        Sexy::Image *image = ZombatarWidget::GetHairImageByIndex(i);
        Sexy::Image *image1 = ZombatarWidget::GetHairMaskImageByIndex(i);
        int mWidth = image->mWidth;
        int mHeight = image->mHeight;
        float ratio = 58 / float(std::max(mWidth, mHeight));
        if (ratio > 1.3)
            ratio = 1.3;
        float widthOffset = (58 - ratio * mWidth) / 2;
        float heightOffset = (58 - ratio * mHeight) / 2;

        if (mSelectedHair == i) {
            g->DrawImage(addonZombatarImages.zombatar_accessory_bg_highlight, theX, theY);
            if (image1 != nullptr) {
                int widthOffset2 = ZombatarWidget::GetHairMaskImageOffsetXByIndex(i);
                int heightOffset2 = ZombatarWidget::GetHairMaskImageOffsetYByIndex(i);
                TodDrawImageScaledF(g, image1, theX + 12 + widthOffset + widthOffset2 * ratio, theY + 12 + heightOffset + heightOffset2 * ratio, ratio, ratio);
            }
            TodDrawImageScaledF(g, image, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
        } else {
            Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg, &theAlphaColor, theX, theY);
            if (image1 != nullptr) {
                int widthOffset2 = ZombatarWidget::GetHairMaskImageOffsetXByIndex(i);
                int heightOffset2 = ZombatarWidget::GetHairMaskImageOffsetYByIndex(i);
                Sexy_Graphics_DrawImageColorizedScaled(g, image1, &theAlphaColor, theX + 12 + widthOffset + widthOffset2 * ratio, theY + 12 + heightOffset + heightOffset2 * ratio, ratio, ratio);
            }
            Sexy_Graphics_DrawImageColorizedScaled(g, image, &theAlphaColor, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
        }
    }
    int theX = 160 + 198 + (16 % 6) * 73;
    int theY = 162 + 16 / 6 * 79;
    Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg_none, &theAlphaColor, theX, theY);

    if (mSelectedHair == 255) {
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = Sexy::FONT_BRIANNETOD12;
        TodDrawStringWrapped(g, "[ZOMBATAR_COLOR_ITEM_NOT_CHOSEN]", rect, font, gColorWhite, DrawStringJustification::DS_ALIGN_LEFT, false);
    } else if (ZombatarWidget::AccessoryIsColorized(mSelectedTab, mSelectedHair)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Color color = gZombatarAccessoryColor[i];
            if (mSelectedHairColor != i) {
                color.mAlpha = 64;
            }
            Sexy_Graphics_DrawImageColorized(g, i == 17 ? addonZombatarImages.zombatar_colorpicker_none : addonZombatarImages.zombatar_colorpicker, &color, theX, theY);
        }
    } else {
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = Sexy::FONT_BRIANNETOD12;
        TodDrawStringWrapped(g, "[ZOMBATAR_COLOR_NOT_APPLICABLE]", rect, font, gColorWhite, DrawStringJustification::DS_ALIGN_LEFT, false);
    }
}

void ZombatarWidget::DrawFHair(Sexy::Graphics *g) {
    if (mSelectedFHairPage == 0) {
        Color theAlphaColor = {255, 255, 255, 64};
        for (int i = 0; i < 17; ++i) {
            int theX = 160 + 198 + (i % 6) * 73;
            int theY = 162 + i / 6 * 79;
            Sexy::Image *image = ZombatarWidget::GetFHairImageByIndex(i);
            Sexy::Image *image1 = ZombatarWidget::GetFHairMaskImageByIndex(i);
            int mWidth = image->mWidth;
            int mHeight = image->mHeight;
            float ratio = 58 / float(std::max(mWidth, mHeight));
            if (ratio > 1.3)
                ratio = 1.3;
            float widthOffset = (58 - ratio * mWidth) / 2;
            float heightOffset = (58 - ratio * mHeight) / 2;

            if (mSelectedFHair == i) {
                g->DrawImage(addonZombatarImages.zombatar_accessory_bg_highlight, theX, theY);
                if (image1 != nullptr) {
                    int widthOffset2 = ZombatarWidget::GetFHairMaskImageOffsetXByIndex(i);
                    int heightOffset2 = ZombatarWidget::GetFHairMaskImageOffsetYByIndex(i);
                    TodDrawImageScaledF(g, image1, theX + 12 + widthOffset + widthOffset2 * ratio, theY + 12 + heightOffset + heightOffset2 * ratio, ratio, ratio);
                }
                TodDrawImageScaledF(g, image, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
            } else {
                Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg, &theAlphaColor, theX, theY);
                if (image1 != nullptr) {
                    int widthOffset2 = ZombatarWidget::GetFHairMaskImageOffsetXByIndex(i);
                    int heightOffset2 = ZombatarWidget::GetFHairMaskImageOffsetYByIndex(i);
                    Sexy_Graphics_DrawImageColorizedScaled(g, image1, &theAlphaColor, theX + 12 + widthOffset + widthOffset2 * ratio, theY + 12 + heightOffset + heightOffset2 * ratio, ratio, ratio);
                }
                Sexy_Graphics_DrawImageColorizedScaled(g, image, &theAlphaColor, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
            }
        }
        int theX = 160 + 198 + (17 % 6) * 73;
        int theY = 162 + 17 / 6 * 79;
        Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg_none, &theAlphaColor, theX, theY);
        Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_prev_button, &theAlphaColor, 160 + 209, 436);
        g->DrawImage(addonZombatarImages.zombatar_next_button, 160 + 588, 436);
    } else if (mSelectedFHairPage == 1) {
        Color theAlphaColor = {255, 255, 255, 64};
        for (int i = 0; i < 7; ++i) {
            int theX = 160 + 198 + (i % 6) * 73;
            int theY = 162 + i / 6 * 79;
            Sexy::Image *image = ZombatarWidget::GetFHairImageByIndex(i + 17);
            Sexy::Image *image1 = ZombatarWidget::GetFHairMaskImageByIndex(i + 17);
            int mWidth = image->mWidth;
            int mHeight = image->mHeight;
            float ratio = 58 / float(std::max(mWidth, mHeight));
            if (ratio > 1.3)
                ratio = 1.3;
            float widthOffset = (58 - ratio * mWidth) / 2;
            float heightOffset = (58 - ratio * mHeight) / 2;

            if (mSelectedFHair == i + 17) {
                g->DrawImage(addonZombatarImages.zombatar_accessory_bg_highlight, theX, theY);
                if (image1 != nullptr) {
                    int widthOffset2 = ZombatarWidget::GetFHairMaskImageOffsetXByIndex(i + 17);
                    int heightOffset2 = ZombatarWidget::GetFHairMaskImageOffsetYByIndex(i + 17);
                    TodDrawImageScaledF(g, image1, theX + 12 + widthOffset + widthOffset2 * ratio, theY + 12 + heightOffset + heightOffset2 * ratio, ratio, ratio);
                }
                TodDrawImageScaledF(g, image, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
            } else {
                Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg, &theAlphaColor, theX, theY);
                if (image1 != nullptr) {
                    int widthOffset2 = ZombatarWidget::GetFHairMaskImageOffsetXByIndex(i + 17);
                    int heightOffset2 = ZombatarWidget::GetFHairMaskImageOffsetYByIndex(i + 17);
                    Sexy_Graphics_DrawImageColorizedScaled(g, image1, &theAlphaColor, theX + 12 + widthOffset + widthOffset2 * ratio, theY + 12 + heightOffset + heightOffset2 * ratio, ratio, ratio);
                }
                Sexy_Graphics_DrawImageColorizedScaled(g, image, &theAlphaColor, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
            }
        }
        int theX = 160 + 198 + (7 % 6) * 73;
        int theY = 162 + 7 / 6 * 79;
        Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg_none, &theAlphaColor, theX, theY);
        g->DrawImage(addonZombatarImages.zombatar_prev_button, 160 + 209, 436);
        Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_next_button, &theAlphaColor, 160 + 588, 436);
    }

    if (mSelectedFHair == 255) {
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = Sexy::FONT_BRIANNETOD12;
        TodDrawStringWrapped(g, "[ZOMBATAR_COLOR_ITEM_NOT_CHOSEN]", rect, font, gColorWhite, DrawStringJustification::DS_ALIGN_LEFT, false);
    } else if (ZombatarWidget::AccessoryIsColorized(mSelectedTab, mSelectedFHair)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Color color = gZombatarAccessoryColor[i];
            if (mSelectedFHairColor != i) {
                color.mAlpha = 64;
            }
            Sexy_Graphics_DrawImageColorized(g, i == 17 ? addonZombatarImages.zombatar_colorpicker_none : addonZombatarImages.zombatar_colorpicker, &color, theX, theY);
        }
    } else {
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = Sexy::FONT_BRIANNETOD12;
        TodDrawStringWrapped(g, "[ZOMBATAR_COLOR_NOT_APPLICABLE]", rect, font, gColorWhite, DrawStringJustification::DS_ALIGN_LEFT, false);
    }

    pvzstl::string str = StrFormat("PAGE %d/%d", mSelectedFHairPage + 1, 2);
    TodDrawString(g, str, 160 + 410, 525, Sexy::FONT_BRIANNETOD16, gColorBlack, DrawStringJustification::DS_ALIGN_CENTER);
}

void ZombatarWidget::DrawTidBit(Sexy::Graphics *g) {
    Color theAlphaColor = {255, 255, 255, 64};
    for (int i = 0; i < 14; ++i) {
        int theX = 160 + 198 + (i % 6) * 73;
        int theY = 162 + i / 6 * 79;
        Sexy::Image *image = ZombatarWidget::GetTidBitImageByIndex(i);
        int mWidth = image->mWidth;
        int mHeight = image->mHeight;
        float ratio = 58 / float(std::max(mWidth, mHeight));
        if (ratio > 1.3)
            ratio = 1.3;
        float widthOffset = (58 - ratio * mWidth) / 2;
        float heightOffset = (58 - ratio * mHeight) / 2;

        if (mSelectedTidBit == i) {
            g->DrawImage(addonZombatarImages.zombatar_accessory_bg_highlight, theX, theY);
            TodDrawImageScaledF(g, image, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
            if (i == 0) {
                TodDrawImageScaledF(g, ZombatarWidget::GetTidBitImageByIndex(2), theX + 23, theY + 44, ratio, ratio);
            }
        } else {
            Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg, &theAlphaColor, theX, theY);
            Sexy_Graphics_DrawImageColorizedScaled(g, image, &theAlphaColor, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
            if (i == 0) {
                Sexy_Graphics_DrawImageColorizedScaled(g, ZombatarWidget::GetTidBitImageByIndex(2), &theAlphaColor, theX + 23, theY + 44, ratio, ratio);
            }
        }
    }
    int theX = 160 + 198 + (14 % 6) * 73;
    int theY = 162 + 14 / 6 * 79;
    Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg_none, &theAlphaColor, theX, theY);

    if (mSelectedTidBit == 255) {
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = Sexy::FONT_BRIANNETOD12;
        TodDrawStringWrapped(g, "[ZOMBATAR_COLOR_ITEM_NOT_CHOSEN]", rect, font, gColorWhite, DrawStringJustification::DS_ALIGN_LEFT, false);
    } else if (ZombatarWidget::AccessoryIsColorized(mSelectedTab, mSelectedTidBit)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Color color = gZombatarAccessoryColor2[i];
            if (mSelectedTidBitColor != i) {
                color.mAlpha = 64;
            }
            Sexy_Graphics_DrawImageColorized(g, i == 17 ? addonZombatarImages.zombatar_colorpicker_none : addonZombatarImages.zombatar_colorpicker, &color, theX, theY);
        }
    } else {
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = Sexy::FONT_BRIANNETOD12;
        TodDrawStringWrapped(g, "[ZOMBATAR_COLOR_NOT_APPLICABLE]", rect, font, gColorWhite, DrawStringJustification::DS_ALIGN_LEFT, false);
    }
}

void ZombatarWidget::DrawEyeWear(Sexy::Graphics *g) {
    Color theAlphaColor = {255, 255, 255, 64};
    for (int i = 0; i < 16; ++i) {
        int theX = 160 + 198 + (i % 6) * 73;
        int theY = 162 + i / 6 * 79;
        Sexy::Image *image = ZombatarWidget::GetEyeWearImageByIndex(i);
        Sexy::Image *image1 = ZombatarWidget::GetEyeWearMaskImageByIndex(i);
        int mWidth = image->mWidth;
        int mHeight = image->mHeight;
        float ratio = 58 / float(std::max(mWidth, mHeight));
        if (ratio > 1.3)
            ratio = 1.3;
        float widthOffset = (58 - ratio * mWidth) / 2;
        float heightOffset = (58 - ratio * mHeight) / 2;

        if (mSelectedEyeWear == i) {
            g->DrawImage(addonZombatarImages.zombatar_accessory_bg_highlight, theX, theY);
            if (image1 != nullptr) {
                TodDrawImageScaledF(g, image1, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
            }
            TodDrawImageScaledF(g, image, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
        } else {
            if (image1 != nullptr) {
                Sexy_Graphics_DrawImageColorizedScaled(g, image1, &theAlphaColor, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
            }
            Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg, &theAlphaColor, theX, theY);
            Sexy_Graphics_DrawImageColorizedScaled(g, image, &theAlphaColor, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
        }
    }
    int theX = 160 + 198 + (16 % 6) * 73;
    int theY = 162 + 16 / 6 * 79;
    Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg_none, &theAlphaColor, theX, theY);

    if (mSelectedEyeWear == 255) {
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = Sexy::FONT_BRIANNETOD12;
        TodDrawStringWrapped(g, "[ZOMBATAR_COLOR_ITEM_NOT_CHOSEN]", rect, font, gColorWhite, DrawStringJustification::DS_ALIGN_LEFT, false);
    } else if (ZombatarWidget::AccessoryIsColorized(mSelectedTab, mSelectedEyeWear)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Color color = gZombatarAccessoryColor2[i];
            if (mSelectedEyeWearColor != i) {
                color.mAlpha = 64;
            }
            Sexy_Graphics_DrawImageColorized(g, i == 17 ? addonZombatarImages.zombatar_colorpicker_none : addonZombatarImages.zombatar_colorpicker, &color, theX, theY);
        }
    } else {
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = Sexy::FONT_BRIANNETOD12;
        TodDrawStringWrapped(g, "[ZOMBATAR_COLOR_NOT_APPLICABLE]", rect, font, gColorWhite, DrawStringJustification::DS_ALIGN_LEFT, false);
    }
}

void ZombatarWidget::DrawCloth(Sexy::Graphics *g) {
    Color theAlphaColor = {255, 255, 255, 64};
    Color theAlphaColor2 = {255, 255, 255, 128};
    for (int i = 0; i < 12; ++i) {
        int theX = 160 + 198 + (i % 6) * 73;
        int theY = 162 + i / 6 * 79;
        Sexy::Image *image = addonZombatarImages.zombatar_zombie_blank_part;
        Sexy::Image *image1 = ZombatarWidget::GetClothImageByIndex(i);
        int mWidth = image->mWidth;
        int mHeight = image->mHeight;
        float ratio = 58.0 / float(std::max(mWidth, mHeight));
        if (ratio > 1.3)
            ratio = 1.3;
        float widthOffset = (58 - ratio * mWidth) / 2;
        float heightOffset = (58 - ratio * mHeight) / 2;
        int offsetX = addonZombatarImages.zombatar_zombie_blank_part->mWidth + ZombatarWidget::GetClothImageOffsetXByIndex(i);
        int offsetY = addonZombatarImages.zombatar_zombie_blank_part->mHeight + ZombatarWidget::GetClothImageOffsetYByIndex(i);
        if (mSelectedCloth == i) {
            g->DrawImage(addonZombatarImages.zombatar_accessory_bg_highlight, theX, theY);
            Sexy_Graphics_DrawImageColorizedScaled(
                g, addonZombatarImages.zombatar_zombie_blank_skin_part, &gZombatarSkinColor[mSelectedSkinColor], theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
            TodDrawImageScaledF(g, image, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
            TodDrawImageScaledF(g, image1, theX + 12 + ratio * offsetX, theY + 12 + ratio * offsetY, ratio, ratio);
        } else {
            Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg, &theAlphaColor, theX, theY);
            Sexy_Graphics_DrawImageColorizedScaled(
                g, addonZombatarImages.zombatar_zombie_blank_skin_part, &gZombatarSkinColor[mSelectedSkinColor], theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
            TodDrawImageScaledF(g, image, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
            Sexy_Graphics_DrawImageColorizedScaled(g, image1, &theAlphaColor2, theX + 12 + ratio * offsetX, theY + 12 + ratio * offsetY, ratio, ratio);
        }
    }
    int theX = 160 + 198 + (12 % 6) * 73;
    int theY = 162 + 12 / 6 * 79;
    Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg_none, &theAlphaColor, theX, theY);

    if (mSelectedCloth == 255) {
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = Sexy::FONT_BRIANNETOD12;
        TodDrawStringWrapped(g, "[ZOMBATAR_COLOR_ITEM_NOT_CHOSEN]", rect, font, gColorWhite, DrawStringJustification::DS_ALIGN_LEFT, false);
    } else {
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = Sexy::FONT_BRIANNETOD12;
        TodDrawStringWrapped(g, "[ZOMBATAR_COLOR_NOT_APPLICABLE]", rect, font, gColorWhite, DrawStringJustification::DS_ALIGN_LEFT, false);
    }
}

void ZombatarWidget::DrawAccessory(Sexy::Graphics *g) {
    Color theAlphaColor = {255, 255, 255, 64};
    for (int i = 0; i < 15; ++i) {
        int theX = 160 + 198 + (i % 6) * 73;
        int theY = 162 + i / 6 * 79;
        Sexy::Image *image = ZombatarWidget::GetAccessoryImageByIndex(i);
        int mWidth = image->mWidth;
        int mHeight = image->mHeight;
        float ratio = 58 / float(std::max(mWidth, mHeight));
        if (ratio > 1.3)
            ratio = 1.3;
        float widthOffset = (58 - ratio * mWidth) / 2;
        float heightOffset = (58 - ratio * mHeight) / 2;

        if (mSelectedAccessory == i) {
            g->DrawImage(addonZombatarImages.zombatar_accessory_bg_highlight, theX, theY);
            TodDrawImageScaledF(g, image, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
        } else {
            Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg, &theAlphaColor, theX, theY);
            Sexy_Graphics_DrawImageColorizedScaled(g, image, &theAlphaColor, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
        }
    }
    int theX = 160 + 198 + (15 % 6) * 73;
    int theY = 162 + 15 / 6 * 79;
    Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg_none, &theAlphaColor, theX, theY);

    if (mSelectedAccessory == 255) {
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = Sexy::FONT_BRIANNETOD12;
        TodDrawStringWrapped(g, "[ZOMBATAR_COLOR_ITEM_NOT_CHOSEN]", rect, font, gColorWhite, DrawStringJustification::DS_ALIGN_LEFT, false);
    } else if (ZombatarWidget::AccessoryIsColorized(mSelectedTab, mSelectedAccessory)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Color color = gZombatarAccessoryColor2[i];
            if (mSelectedAccessoryColor != i) {
                color.mAlpha = 64;
            }
            Sexy_Graphics_DrawImageColorized(g, i == 17 ? addonZombatarImages.zombatar_colorpicker_none : addonZombatarImages.zombatar_colorpicker, &color, theX, theY);
        }
    } else {
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = Sexy::FONT_BRIANNETOD12;
        TodDrawStringWrapped(g, "[ZOMBATAR_COLOR_NOT_APPLICABLE]", rect, font, gColorWhite, DrawStringJustification::DS_ALIGN_LEFT, false);
    }
}

void ZombatarWidget::DrawHat(Sexy::Graphics *g) {
    Color theAlphaColor = {255, 255, 255, 64};
    for (int i = 0; i < 14; ++i) {
        int theX = 160 + 198 + (i % 6) * 73;
        int theY = 162 + i / 6 * 79;
        Sexy::Image *image = ZombatarWidget::GetHatImageByIndex(i);
        Sexy::Image *image1 = ZombatarWidget::GetHatMaskImageByIndex(i);
        int mWidth = image->mWidth;
        int mHeight = image->mHeight;
        float ratio = 58 / float(std::max(mWidth, mHeight));
        if (ratio > 1.3)
            ratio = 1.3;
        float widthOffset = (58 - ratio * mWidth) / 2;
        float heightOffset = (58 - ratio * mHeight) / 2;
        float widthOffset2 = ZombatarWidget::GetHatMaskImageOffsetXByIndex(i) * ratio;
        float heightOffset2 = ZombatarWidget::GetHatMaskImageOffsetYByIndex(i) * ratio;

        if (mSelectedHat == i) {
            g->DrawImage(addonZombatarImages.zombatar_accessory_bg_highlight, theX, theY);
            if (image1 != nullptr) {
                TodDrawImageScaledF(g, image1, theX + 12 + widthOffset + widthOffset2, theY + 12 + heightOffset + heightOffset2, ratio, ratio);
            }
            TodDrawImageScaledF(g, image, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
        } else {
            Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg, &theAlphaColor, theX, theY);
            if (image1 != nullptr) {
                Sexy_Graphics_DrawImageColorizedScaled(g, image1, &theAlphaColor, theX + 12 + widthOffset + widthOffset2, theY + 12 + heightOffset + heightOffset2, ratio, ratio);
            }
            Sexy_Graphics_DrawImageColorizedScaled(g, image, &theAlphaColor, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
        }
    }
    int theX = 160 + 198 + (14 % 6) * 73;
    int theY = 162 + 14 / 6 * 79;
    Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg_none, &theAlphaColor, theX, theY);

    if (mSelectedHat == 255) {
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = Sexy::FONT_BRIANNETOD12;
        TodDrawStringWrapped(g, "[ZOMBATAR_COLOR_ITEM_NOT_CHOSEN]", rect, font, gColorWhite, DrawStringJustification::DS_ALIGN_LEFT, false);
    } else if (ZombatarWidget::AccessoryIsColorized(mSelectedTab, mSelectedHat)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Color color = gZombatarAccessoryColor2[i];
            if (mSelectedHatColor != i) {
                color.mAlpha = 64;
            }
            Sexy_Graphics_DrawImageColorized(g, i == 17 ? addonZombatarImages.zombatar_colorpicker_none : addonZombatarImages.zombatar_colorpicker, &color, theX, theY);
        }
    } else {
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = Sexy::FONT_BRIANNETOD12;
        TodDrawStringWrapped(g, "[ZOMBATAR_COLOR_NOT_APPLICABLE]", rect, font, gColorWhite, DrawStringJustification::DS_ALIGN_LEFT, false);
    }
}

void ZombatarWidget::DrawBackground(Sexy::Graphics *g) {
    Color theAlphaColor = {255, 255, 255, 64};
    if (mSelectedBackgroundPage == 0) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 198 + (i % 6) * 73;
            int theY = 162 + i / 6 * 79;
            if (mSelectedBackground == i) {
                g->DrawImage(addonZombatarImages.zombatar_accessory_bg_highlight, theX, theY);
                TodDrawImageScaledF(g, ZombatarWidget::GetBackgroundImageByIndex(i), theX + 12, theY + 12, 58.0 / 216.0, 58.0 / 216.0);
            }
            Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg, &theAlphaColor, theX, theY);
            Sexy_Graphics_DrawImageColorizedScaled(g, ZombatarWidget::GetBackgroundImageByIndex(i), &theAlphaColor, theX + 12, theY + 12, 58.0 / 216.0, 58.0 / 216.0);
        }
        Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_prev_button, &theAlphaColor, 160 + 209, 436);
        g->DrawImage(addonZombatarImages.zombatar_next_button, 160 + 588, 436);
    } else if (mSelectedBackgroundPage == 4) {
        for (int i = 0; i < 11; ++i) {
            int theX = 160 + 198 + (i % 6) * 73;
            int theY = 162 + i / 6 * 79;
            if (mSelectedBackground == i + 18 * mSelectedBackgroundPage) {
                g->DrawImage(addonZombatarImages.zombatar_accessory_bg_highlight, theX, theY);
                TodDrawImageScaledF(g, ZombatarWidget::GetBackgroundImageByIndex(i + 18 * mSelectedBackgroundPage), theX + 12, theY + 12, 58.0 / 216.0, 58.0 / 216.0);
            }
            Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg, &theAlphaColor, theX, theY);
            Sexy_Graphics_DrawImageColorizedScaled(g, ZombatarWidget::GetBackgroundImageByIndex(i + 18 * mSelectedBackgroundPage), &theAlphaColor, theX + 12, theY + 12, 58.0 / 216.0, 58.0 / 216.0);
        }
        g->DrawImage(addonZombatarImages.zombatar_prev_button, 160 + 209, 436);
        Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_next_button, &theAlphaColor, 160 + 588, 436);
    } else {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 198 + (i % 6) * 73;
            int theY = 162 + i / 6 * 79;
            if (mSelectedBackground == i + mSelectedBackgroundPage * 18) {
                g->DrawImage(addonZombatarImages.zombatar_accessory_bg_highlight, theX, theY);
                TodDrawImageScaledF(g, ZombatarWidget::GetBackgroundImageByIndex(i + mSelectedBackgroundPage * 18), theX + 12, theY + 12, 58.0 / 216.0, 58.0 / 216.0);
            }
            Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg, &theAlphaColor, theX, theY);
            Sexy_Graphics_DrawImageColorizedScaled(g, ZombatarWidget::GetBackgroundImageByIndex(i + mSelectedBackgroundPage * 18), &theAlphaColor, theX + 12, theY + 12, 58.0 / 216.0, 58.0 / 216.0);
        }
        g->DrawImage(addonZombatarImages.zombatar_prev_button, 160 + 209, 436);
        g->DrawImage(addonZombatarImages.zombatar_next_button, 160 + 588, 436);
    }


    if (ZombatarWidget::AccessoryIsColorized(mSelectedTab, mSelectedBackground)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Color color = gZombatarAccessoryColor2[i];
            if (mSelectedBackgroundColor != i) {
                color.mAlpha = 64;
            }
            Sexy_Graphics_DrawImageColorized(g, i == 17 ? addonZombatarImages.zombatar_colorpicker_none : addonZombatarImages.zombatar_colorpicker, &color, theX, theY);
        }
    } else {
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = Sexy::FONT_BRIANNETOD12;
        TodDrawStringWrapped(g, "[ZOMBATAR_COLOR_NOT_APPLICABLE]", rect, font, gColorWhite, DrawStringJustification::DS_ALIGN_LEFT, false);
    }

    pvzstl::string str = StrFormat("PAGE %d/%d", mSelectedBackgroundPage + 1, 5);
    TodDrawString(g, str, 160 + 410, 525, Sexy::FONT_BRIANNETOD16, gColorBlack, DrawStringJustification::DS_ALIGN_CENTER);
    // StringDelete(holder);
}

void ZombatarWidget::DrawPortrait(Sexy::Graphics *g, int x, int y) {
    Sexy::Image *backgroundImage = ZombatarWidget::GetBackgroundImageByIndex(mSelectedBackground);
    if (ZombatarWidget::AccessoryIsColorized(ZombatarWidget::BACKGROUND, mSelectedBackground) && mSelectedBackgroundColor != 255) {
        Sexy_Graphics_DrawImageColorized(g, backgroundImage, &gZombatarAccessoryColor2[mSelectedBackgroundColor], x, y);
    } else {
        g->DrawImage(backgroundImage, x, y);
    }
    Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_zombie_blank_skin, &gZombatarSkinColor[mSelectedSkinColor], x + 46, y + 48);
    g->DrawImage(addonZombatarImages.zombatar_zombie_blank, x + 46, y + 48);

    Sexy::Image *clothImage = ZombatarWidget::GetClothImageByIndex(mSelectedCloth);
    if (clothImage != nullptr) {
        int offsetX = addonZombatarImages.zombatar_background_blank->mWidth + ZombatarWidget::GetClothImageOffsetXByIndex(mSelectedCloth);
        int offsetY = addonZombatarImages.zombatar_background_blank->mHeight + ZombatarWidget::GetClothImageOffsetYByIndex(mSelectedCloth);
        g->DrawImage(clothImage, x + offsetX, y + offsetY);
    }

    Sexy::Image *tidBitImage = ZombatarWidget::GetTidBitImageByIndex(mSelectedTidBit);
    if (tidBitImage != nullptr) {
        int offsetX = ZombatarWidget::GetTidBitImageOffsetXByIndex(mSelectedTidBit);
        int offsetY = ZombatarWidget::GetTidBitImageOffsetYByIndex(mSelectedTidBit);
        if (mSelectedTidBitColor != 255 && ZombatarWidget::AccessoryIsColorized(ZombatarWidget::TIDBIT, mSelectedTidBit) && mSelectedTidBit != 0) {
            Sexy_Graphics_DrawImageColorized(g, tidBitImage, &gZombatarAccessoryColor2[mSelectedTidBitColor], x + offsetX, y + offsetY);
        } else {
            g->DrawImage(tidBitImage, x + offsetX, y + offsetY);
        }
    }
    if (mSelectedTidBit == 0) {
        Sexy::Image *tidBitImage = ZombatarWidget::GetTidBitImageByIndex(2);
        if (tidBitImage != nullptr) {
            int offsetX = ZombatarWidget::GetTidBitImageOffsetXByIndex(2);
            int offsetY = ZombatarWidget::GetTidBitImageOffsetYByIndex(2);
            if (mSelectedTidBitColor != 255 && ZombatarWidget::AccessoryIsColorized(ZombatarWidget::TIDBIT, 2)) {
                Sexy_Graphics_DrawImageColorized(g, tidBitImage, &gZombatarAccessoryColor2[mSelectedTidBitColor], x + offsetX, y + offsetY);
            } else {
                g->DrawImage(tidBitImage, x + offsetX, y + offsetY);
            }
        }
    }

    Sexy::Image *eyeWearImage = ZombatarWidget::GetEyeWearImageByIndex(mSelectedEyeWear);
    if (eyeWearImage != nullptr) {
        int offsetX = ZombatarWidget::GetEyeWearImageOffsetXByIndex(mSelectedEyeWear);
        int offsetY = ZombatarWidget::GetEyeWearImageOffsetYByIndex(mSelectedEyeWear);
        Sexy::Image *eyeWearMaskImage = ZombatarWidget::GetEyeWearMaskImageByIndex(mSelectedEyeWear);
        if (mSelectedEyeWearColor != 255 && ZombatarWidget::AccessoryIsColorized(ZombatarWidget::EYEWEAR, mSelectedEyeWear)) {
            if (eyeWearMaskImage != nullptr) {
                Sexy_Graphics_DrawImageColorized(g, eyeWearMaskImage, &gZombatarAccessoryColor2[mSelectedEyeWearColor], x + offsetX, y + offsetY);
                g->DrawImage(eyeWearImage, x + offsetX, y + offsetY);
            } else {
                Sexy_Graphics_DrawImageColorized(g, eyeWearImage, &gZombatarAccessoryColor2[mSelectedEyeWearColor], x + offsetX, y + offsetY);
            }
        } else {
            if (eyeWearMaskImage != nullptr) {
                g->DrawImage(eyeWearMaskImage, x + offsetX, y + offsetY);
                g->DrawImage(eyeWearImage, x + offsetX, y + offsetY);
            } else {
                g->DrawImage(eyeWearImage, x + offsetX, y + offsetY);
            }
        }
    }

    Sexy::Image *accessoryImage = ZombatarWidget::GetAccessoryImageByIndex(mSelectedAccessory);
    if (accessoryImage != nullptr) {
        int offsetX = ZombatarWidget::GetAccessoryImageOffsetXByIndex(mSelectedAccessory);
        int offsetY = ZombatarWidget::GetAccessoryImageOffsetYByIndex(mSelectedAccessory);
        if (mSelectedAccessoryColor != 255 && ZombatarWidget::AccessoryIsColorized(ZombatarWidget::ACCESSORY, mSelectedAccessory)) {
            Sexy_Graphics_DrawImageColorized(g, accessoryImage, &gZombatarAccessoryColor2[mSelectedAccessoryColor], x + offsetX, y + offsetY);
        } else {
            g->DrawImage(accessoryImage, x + offsetX, y + offsetY);
        }
    }

    Sexy::Image *fHairImage = ZombatarWidget::GetFHairImageByIndex(mSelectedFHair);
    if (fHairImage != nullptr) {
        int offsetX = ZombatarWidget::GetFHairImageOffsetXByIndex(mSelectedFHair);
        int offsetY = ZombatarWidget::GetFHairImageOffsetYByIndex(mSelectedFHair);
        int offsetX2 = ZombatarWidget::GetFHairMaskImageOffsetXByIndex(mSelectedFHair);
        int offsetY2 = ZombatarWidget::GetFHairMaskImageOffsetYByIndex(mSelectedFHair);
        Sexy::Image *fHairMaskImage = ZombatarWidget::GetFHairMaskImageByIndex(mSelectedFHair);
        if (mSelectedFHairColor != 255 && ZombatarWidget::AccessoryIsColorized(ZombatarWidget::FHAIR, mSelectedFHair)) {
            if (fHairMaskImage != nullptr) {
                Sexy_Graphics_DrawImageColorized(g, fHairMaskImage, &gZombatarAccessoryColor[mSelectedFHairColor], x + offsetX + offsetX2, y + offsetY + offsetY2);
                g->DrawImage(fHairImage, x + offsetX, y + offsetY);
            } else {
                Sexy_Graphics_DrawImageColorized(g, fHairImage, &gZombatarAccessoryColor[mSelectedFHairColor], x + offsetX, y + offsetY);
            }
        } else {
            if (fHairMaskImage != nullptr) {
                g->DrawImage(fHairMaskImage, x + offsetX + offsetX2, y + offsetY + offsetY2);
                g->DrawImage(fHairImage, x + offsetX, y + offsetY);
            } else {
                g->DrawImage(fHairImage, x + offsetX, y + offsetY);
            }
        }
    }


    Sexy::Image *hairImage = ZombatarWidget::GetHairImageByIndex(mSelectedHair);
    if (hairImage != nullptr) {
        int offsetX = ZombatarWidget::GetHairImageOffsetXByIndex(mSelectedHair);
        int offsetY = ZombatarWidget::GetHairImageOffsetYByIndex(mSelectedHair);
        int offsetX2 = ZombatarWidget::GetHairMaskImageOffsetXByIndex(mSelectedHair);
        int offsetY2 = ZombatarWidget::GetHairMaskImageOffsetYByIndex(mSelectedHair);
        Sexy::Image *hairMaskImage = ZombatarWidget::GetHairMaskImageByIndex(mSelectedHair);
        if (mSelectedHairColor != 255 && ZombatarWidget::AccessoryIsColorized(ZombatarWidget::HAIR, mSelectedHair)) {
            if (hairMaskImage != nullptr) {
                Sexy_Graphics_DrawImageColorized(g, hairMaskImage, &gZombatarAccessoryColor[mSelectedHairColor], x + offsetX + offsetX2, y + offsetY + offsetY2);
                g->DrawImage(hairImage, x + offsetX, y + offsetY);
            } else {
                Sexy_Graphics_DrawImageColorized(g, hairImage, &gZombatarAccessoryColor[mSelectedHairColor], x + offsetX, y + offsetY);
            }
        } else {
            if (hairMaskImage != nullptr) {
                g->DrawImage(hairMaskImage, x + offsetX + offsetX2, y + offsetY + offsetY2);
                g->DrawImage(hairImage, x + offsetX, y + offsetY);
            } else {
                g->DrawImage(hairImage, x + offsetX, y + offsetY);
            }
        }
    }

    Sexy::Image *hatImage = ZombatarWidget::GetHatImageByIndex(mSelectedHat);
    if (hatImage != nullptr) {
        int offsetX = ZombatarWidget::GetHatImageOffsetXByIndex(mSelectedHat);
        int offsetY = ZombatarWidget::GetHatImageOffsetYByIndex(mSelectedHat);
        int offsetX2 = ZombatarWidget::GetHatMaskImageOffsetXByIndex(mSelectedHat);
        int offsetY2 = ZombatarWidget::GetHatMaskImageOffsetYByIndex(mSelectedHat);
        Sexy::Image *hatMaskImage = ZombatarWidget::GetHatMaskImageByIndex(mSelectedHat);
        if (mSelectedHatColor != 255 && ZombatarWidget::AccessoryIsColorized(ZombatarWidget::HAT, mSelectedHat)) {
            if (hatMaskImage != nullptr) {
                Sexy_Graphics_DrawImageColorized(g, hatMaskImage, &gZombatarAccessoryColor2[mSelectedHatColor], x + offsetX + offsetX2, y + offsetY + offsetY2);
                g->DrawImage(hatImage, x + offsetX, y + offsetY);
            } else {
                Sexy_Graphics_DrawImageColorized(g, hatImage, &gZombatarAccessoryColor2[mSelectedHatColor], x + offsetX, y + offsetY);
            }
        } else {
            if (hatMaskImage != nullptr) {
                g->DrawImage(hatMaskImage, x + offsetX + offsetX2, y + offsetY + offsetY2);
                g->DrawImage(hatImage, x + offsetX, y + offsetY);
            } else {
                g->DrawImage(hatImage, x + offsetX, y + offsetY);
            }
        }
    }
}

void ZombatarWidget::DrawPreView(Sexy::Graphics *g) {
    g->DrawImage(Sexy::IMAGE_ALMANAC_GROUNDDAY, 160 + 729, 376);
    float tmpX = g->mTransX;
    float tmpY = g->mTransY;
    g->mTransX += 160 + 778;
    g->mTransY += 434;
    mPreviewZombie->Draw(g);
    g->mTransX = tmpX;
    g->mTransY = tmpY;
}

void ZombatarWidget::Draw(Sexy::Graphics *g) {

    g->DrawImage(addonZombatarImages.zombatar_main_bg, 0, 0);
    g->DrawImage(addonZombatarImages.zombatar_widget_bg, 160 + 26, 27);

    if (mShowExistingZombatarPortrait && addonImages.zombatar_portrait != nullptr) {
        g->DrawImage(addonImages.zombatar_portrait, 160 + 260, 210);
        Sexy::Rect rect = {160 + 178, 450, 400, 100};
        TodDrawStringWrapped(g, "[ZOMBATAR_VIEW_PORTRAIT]", rect, Sexy::FONT_BRIANNETOD16, gColorWhite, DrawStringJustification::DS_ALIGN_CENTER, false);
    } else if (mShowZombieTypeSelection) {
        DrawZombieSelection(g);
    } else {
        g->DrawImage(addonZombatarImages.zombatar_widget_inner_bg, 160 + 179, 148);
        for (int i = 0; i < ZombatarWidget::MAX_TAB_NUM; ++i) {
            g->DrawImage(i == mSelectedTab ? ZombatarWidget::GetTabButtonDownImageByIndex(i) : ZombatarWidget::GetTabButtonImageByIndex(i), 160 + 67, 152 + i * 43);
        }
        g->DrawImage(addonZombatarImages.zombatar_colors_bg, 160 + 260, 394);
        switch (mSelectedTab) {
            case ZombatarWidget::SKIN:
                DrawSkin(g);
                break;
            case ZombatarWidget::HAIR:
                DrawHair(g);
                break;
            case ZombatarWidget::FHAIR:
                DrawFHair(g);
                break;
            case ZombatarWidget::TIDBIT:
                DrawTidBit(g);
                break;
            case ZombatarWidget::EYEWEAR:
                DrawEyeWear(g);
                break;
            case ZombatarWidget::CLOTHES:
                DrawCloth(g);
                break;
            case ZombatarWidget::ACCESSORY:
                DrawAccessory(g);
                break;
            case ZombatarWidget::HAT:
                DrawHat(g);
                break;
            case ZombatarWidget::BACKGROUND:
                DrawBackground(g);
                break;
        }
    }

    DrawPortrait(g, 160 + 708, 140);
    DrawPreView(g);

    g->DrawImage(addonZombatarImages.zombatar_display_window, 160 + 0, 0);
}

void ZombatarWidget::ButtonDepress(this ZombatarWidget &self, int theId) {
    if (theId == ZombatarWidget::ZombatarWidget_Back) {
        LawnApp *lawnApp = gLawnApp;
        lawnApp->KillZombatarScreen();
        lawnApp->ShowMainMenuScreen();
        return;
    }

    if (theId == ZombatarWidget::ZombatarWidget_Finish) {
        LawnApp *lawnApp = gLawnApp;
        if (lawnApp->LawnMessageBox(
                Dialogs::DIALOG_MESSAGE, "[ZOMBATAR_FINISHED_WARNING_HEADER]", "[ZOMBATAR_FINISHED_WARNING_TEXT]", "[ZOMBATAR_FINISHED_BUTTON_TEXT]", "[ZOMBATAR_BACK_BUTTON_TEXT]", 1)
            == ZombatarWidget::ZombatarWidget_Finish)
            return;

        if (addonImages.zombatar_portrait != nullptr) {
            static_cast<MemoryImage *>(addonImages.zombatar_portrait)->~MemoryImage();
        }

        MemoryImage *aImage = new MemoryImage();
        aImage->Create(addonZombatarImages.zombatar_background_blank->mWidth, addonZombatarImages.zombatar_background_blank->mHeight);
        aImage->SetImageMode(true, true);
        aImage->mIsVolatile = true;
        Graphics graphics = Graphics(aImage);
        gMainMenuZombatarWidget->DrawPortrait(&graphics, 0, 0);
        aImage->WriteToPng("ZOMBATAR.PNG");
        addonImages.zombatar_portrait = aImage;
        gMainMenuZombatarWidget->mShowExistingZombatarPortrait = true;
        gMainMenuZombatarWidget->mShowZombieTypeSelection = false;

        DefaultPlayerInfo *playerInfo = lawnApp->mPlayerInfo;
        playerInfo->mZombatarHat = gMainMenuZombatarWidget->mSelectedHat;
        playerInfo->mZombatarHatColor = gMainMenuZombatarWidget->mSelectedHatColor;
        playerInfo->mZombatarHair = gMainMenuZombatarWidget->mSelectedHair;
        playerInfo->mZombatarHairColor = gMainMenuZombatarWidget->mSelectedHairColor;
        playerInfo->mZombatarFacialHair = gMainMenuZombatarWidget->mSelectedFHair;
        playerInfo->mZombatarFacialHairColor = gMainMenuZombatarWidget->mSelectedFHairColor;
        playerInfo->mZombatarAccessory = gMainMenuZombatarWidget->mSelectedAccessory;
        playerInfo->mZombatarAccessoryColor = gMainMenuZombatarWidget->mSelectedAccessoryColor;
        playerInfo->mZombatarTidBit = gMainMenuZombatarWidget->mSelectedTidBit;
        playerInfo->mZombatarTidBitColor = gMainMenuZombatarWidget->mSelectedTidBitColor;
        playerInfo->mZombatarEyeWear = gMainMenuZombatarWidget->mSelectedEyeWear;
        playerInfo->mZombatarEyeWearColor = gMainMenuZombatarWidget->mSelectedEyeWearColor;

        playerInfo->mZombatarEnabled = playerInfo->mZombatarHat != 255 || playerInfo->mZombatarHair != 255 || playerInfo->mZombatarFacialHair != 255 || playerInfo->mZombatarAccessory != 255
            || playerInfo->mZombatarTidBit != 255 || playerInfo->mZombatarEyeWear != 255;
        gMainMenuZombatarWidget->SetDefault();
        return;
    }

    if (theId == ZombatarWidget::ZombatarWidget_ViewPortrait) {
        if (addonImages.zombatar_portrait != nullptr) {
            gMainMenuZombatarWidget->mShowExistingZombatarPortrait = true;
            gMainMenuZombatarWidget->SetDefault();
        }
        return;
    }

    if (theId == ZombatarWidget::ZombatarWidget_New) {
        gMainMenuZombatarWidget->mShowExistingZombatarPortrait = false;
        return;
    }

    if (theId == ZombatarWidget::ZombatarWidget_Delete) {
        LawnApp *lawnApp = gLawnApp;
        if (lawnApp->LawnMessageBox(Dialogs::DIALOG_MESSAGE, "[ZOMBATAR_DELETE_HEADER]", "[ZOMBATAR_DELETE_BODY]", "[BUTTON_OK]", "[BUTTON_CANCEL]", 1) == 1001)
            return;
        gMainMenuZombatarWidget->mShowExistingZombatarPortrait = false;
        if (addonImages.zombatar_portrait != nullptr) {
            static_cast<MemoryImage *>(addonImages.zombatar_portrait)->~MemoryImage();
            addonImages.zombatar_portrait = nullptr;
            lawnApp->EraseFile("ZOMBATAR.PNG");
        }
        return;
    }
}

Image *ZombatarWidget::GetTabButtonImageByIndex(int index) {
    static constexpr AddonImageMember kTable[] = {
        &AddonZombatarImagesType::zombatar_skin_button,
        &AddonZombatarImagesType::zombatar_hair_button,
        &AddonZombatarImagesType::zombatar_fhair_button,
        &AddonZombatarImagesType::zombatar_tidbits_button,
        &AddonZombatarImagesType::zombatar_eyewear_button,
        &AddonZombatarImagesType::zombatar_clothes_button,
        &AddonZombatarImagesType::zombatar_accessory_button,
        &AddonZombatarImagesType::zombatar_hats_button,
        &AddonZombatarImagesType::zombatar_backdrops_button,
    };
    return GetAddonImageByIndex(index, kTable);
}

Image *ZombatarWidget::GetTabButtonDownImageByIndex(int index) {
    static constexpr AddonImageMember kTable[] = {
        &AddonZombatarImagesType::zombatar_skin_button_highlight,
        &AddonZombatarImagesType::zombatar_hair_button_highlight,
        &AddonZombatarImagesType::zombatar_fhair_button_highlight,
        &AddonZombatarImagesType::zombatar_tidbits_button_highlight,
        &AddonZombatarImagesType::zombatar_eyewear_button_highlight,
        &AddonZombatarImagesType::zombatar_clothes_button_highlight,
        &AddonZombatarImagesType::zombatar_accessory_button_highlight,
        &AddonZombatarImagesType::zombatar_hats_button_highlight,
        &AddonZombatarImagesType::zombatar_backdrops_button_highlight,
    };
    return GetAddonImageByIndex(index, kTable);
}

Image *ZombatarWidget::GetHairImageByIndex(int index) {
    static constexpr AddonImageMember kTable[] = {
        &AddonZombatarImagesType::zombatar_hair_1,
        &AddonZombatarImagesType::zombatar_hair_2,
        &AddonZombatarImagesType::zombatar_hair_3,
        &AddonZombatarImagesType::zombatar_hair_4,
        &AddonZombatarImagesType::zombatar_hair_5,
        &AddonZombatarImagesType::zombatar_hair_6,
        &AddonZombatarImagesType::zombatar_hair_7,
        &AddonZombatarImagesType::zombatar_hair_8,
        &AddonZombatarImagesType::zombatar_hair_9,
        &AddonZombatarImagesType::zombatar_hair_10,
        &AddonZombatarImagesType::zombatar_hair_11,
        &AddonZombatarImagesType::zombatar_hair_12,
        &AddonZombatarImagesType::zombatar_hair_13,
        &AddonZombatarImagesType::zombatar_hair_14,
        &AddonZombatarImagesType::zombatar_hair_15,
        &AddonZombatarImagesType::zombatar_hair_16,
    };
    return GetAddonImageByIndex(index, kTable);
}

Image *ZombatarWidget::GetHairMaskImageByIndex(int index) {
    static constexpr AddonImageMember kTable[] = {
        &AddonZombatarImagesType::zombatar_hair_1_mask,
        &AddonZombatarImagesType::zombatar_hair_2_mask,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        &AddonZombatarImagesType::zombatar_hair_11_mask,
        &AddonZombatarImagesType::zombatar_hair_12_mask,
        &AddonZombatarImagesType::zombatar_hair_13_mask,
        &AddonZombatarImagesType::zombatar_hair_14_mask,
        &AddonZombatarImagesType::zombatar_hair_15_mask,
    };
    return GetAddonImageByIndex(index, kTable);
}

Image *ZombatarWidget::GetFHairImageByIndex(int index) {
    static constexpr AddonImageMember kTable[] = {
        &AddonZombatarImagesType::zombatar_facialhair_1,  &AddonZombatarImagesType::zombatar_facialhair_2,  &AddonZombatarImagesType::zombatar_facialhair_3,
        &AddonZombatarImagesType::zombatar_facialhair_4,  &AddonZombatarImagesType::zombatar_facialhair_5,  &AddonZombatarImagesType::zombatar_facialhair_6,
        &AddonZombatarImagesType::zombatar_facialhair_7,  &AddonZombatarImagesType::zombatar_facialhair_8,  &AddonZombatarImagesType::zombatar_facialhair_9,
        &AddonZombatarImagesType::zombatar_facialhair_10, &AddonZombatarImagesType::zombatar_facialhair_11, &AddonZombatarImagesType::zombatar_facialhair_12,
        &AddonZombatarImagesType::zombatar_facialhair_13, &AddonZombatarImagesType::zombatar_facialhair_14, &AddonZombatarImagesType::zombatar_facialhair_15,
        &AddonZombatarImagesType::zombatar_facialhair_16, &AddonZombatarImagesType::zombatar_facialhair_17, &AddonZombatarImagesType::zombatar_facialhair_18,
        &AddonZombatarImagesType::zombatar_facialhair_19, &AddonZombatarImagesType::zombatar_facialhair_20, &AddonZombatarImagesType::zombatar_facialhair_21,
        &AddonZombatarImagesType::zombatar_facialhair_22, &AddonZombatarImagesType::zombatar_facialhair_23, &AddonZombatarImagesType::zombatar_facialhair_24,
    };
    return GetAddonImageByIndex(index, kTable);
}

Image *ZombatarWidget::GetFHairMaskImageByIndex(int index) {
    static constexpr AddonImageMember kTable[] = {
        &AddonZombatarImagesType::zombatar_facialhair_1_mask,
        nullptr,
        nullptr,
        &AddonZombatarImagesType::zombatar_facialhair_4_mask,
        nullptr,
        nullptr,
        nullptr,
        &AddonZombatarImagesType::zombatar_facialhair_8_mask,
        &AddonZombatarImagesType::zombatar_facialhair_9_mask,
        &AddonZombatarImagesType::zombatar_facialhair_10_mask,
        &AddonZombatarImagesType::zombatar_facialhair_11_mask,
        &AddonZombatarImagesType::zombatar_facialhair_12_mask,
        nullptr,
        &AddonZombatarImagesType::zombatar_facialhair_14_mask,
        &AddonZombatarImagesType::zombatar_facialhair_15_mask,
        &AddonZombatarImagesType::zombatar_facialhair_16_mask,
        nullptr,
        &AddonZombatarImagesType::zombatar_facialhair_18_mask,
        nullptr,
        nullptr,
        &AddonZombatarImagesType::zombatar_facialhair_21_mask,
        &AddonZombatarImagesType::zombatar_facialhair_22_mask,
        &AddonZombatarImagesType::zombatar_facialhair_23_mask,
        &AddonZombatarImagesType::zombatar_facialhair_24_mask,
    };
    return GetAddonImageByIndex(index, kTable);
}

Image *ZombatarWidget::GetTidBitImageByIndex(int index) {
    static constexpr AddonImageMember kTable[] = {
        &AddonZombatarImagesType::zombatar_tidbits_1,
        &AddonZombatarImagesType::zombatar_tidbits_2,
        &AddonZombatarImagesType::zombatar_tidbits_3,
        &AddonZombatarImagesType::zombatar_tidbits_4,
        &AddonZombatarImagesType::zombatar_tidbits_5,
        &AddonZombatarImagesType::zombatar_tidbits_6,
        &AddonZombatarImagesType::zombatar_tidbits_7,
        &AddonZombatarImagesType::zombatar_tidbits_8,
        &AddonZombatarImagesType::zombatar_tidbits_9,
        &AddonZombatarImagesType::zombatar_tidbits_10,
        &AddonZombatarImagesType::zombatar_tidbits_11,
        &AddonZombatarImagesType::zombatar_tidbits_12,
        &AddonZombatarImagesType::zombatar_tidbits_13,
        &AddonZombatarImagesType::zombatar_tidbits_14,
    };
    return GetAddonImageByIndex(index, kTable);
}

Image *ZombatarWidget::GetEyeWearImageByIndex(int index) {
    static constexpr AddonImageMember kTable[] = {
        &AddonZombatarImagesType::zombatar_eyewear_1,
        &AddonZombatarImagesType::zombatar_eyewear_2,
        &AddonZombatarImagesType::zombatar_eyewear_3,
        &AddonZombatarImagesType::zombatar_eyewear_4,
        &AddonZombatarImagesType::zombatar_eyewear_5,
        &AddonZombatarImagesType::zombatar_eyewear_6,
        &AddonZombatarImagesType::zombatar_eyewear_7,
        &AddonZombatarImagesType::zombatar_eyewear_8,
        &AddonZombatarImagesType::zombatar_eyewear_9,
        &AddonZombatarImagesType::zombatar_eyewear_10,
        &AddonZombatarImagesType::zombatar_eyewear_11,
        &AddonZombatarImagesType::zombatar_eyewear_12,
        &AddonZombatarImagesType::zombatar_eyewear_13,
        &AddonZombatarImagesType::zombatar_eyewear_14,
        &AddonZombatarImagesType::zombatar_eyewear_15,
        &AddonZombatarImagesType::zombatar_eyewear_16,
    };
    return GetAddonImageByIndex(index, kTable);
}

Image *ZombatarWidget::GetEyeWearMaskImageByIndex(int index) {
    static constexpr AddonImageMember kTable[] = {
        &AddonZombatarImagesType::zombatar_eyewear_1_mask,
        &AddonZombatarImagesType::zombatar_eyewear_2_mask,
        &AddonZombatarImagesType::zombatar_eyewear_3_mask,
        &AddonZombatarImagesType::zombatar_eyewear_4_mask,
        &AddonZombatarImagesType::zombatar_eyewear_5_mask,
        &AddonZombatarImagesType::zombatar_eyewear_6_mask,
        &AddonZombatarImagesType::zombatar_eyewear_7_mask,
        &AddonZombatarImagesType::zombatar_eyewear_8_mask,
        &AddonZombatarImagesType::zombatar_eyewear_9_mask,
        &AddonZombatarImagesType::zombatar_eyewear_10_mask,
        &AddonZombatarImagesType::zombatar_eyewear_11_mask,
        &AddonZombatarImagesType::zombatar_eyewear_12_mask,
    };
    return GetAddonImageByIndex(index, kTable);
}

Image *ZombatarWidget::GetClothImageByIndex(int index) {
    static constexpr AddonImageMember kTable[] = {
        &AddonZombatarImagesType::zombatar_clothes_1,
        &AddonZombatarImagesType::zombatar_clothes_2,
        &AddonZombatarImagesType::zombatar_clothes_3,
        &AddonZombatarImagesType::zombatar_clothes_4,
        &AddonZombatarImagesType::zombatar_clothes_5,
        &AddonZombatarImagesType::zombatar_clothes_6,
        &AddonZombatarImagesType::zombatar_clothes_7,
        &AddonZombatarImagesType::zombatar_clothes_8,
        &AddonZombatarImagesType::zombatar_clothes_9,
        &AddonZombatarImagesType::zombatar_clothes_10,
        &AddonZombatarImagesType::zombatar_clothes_11,
        &AddonZombatarImagesType::zombatar_clothes_12,
    };
    return GetAddonImageByIndex(index, kTable);
}

Image *ZombatarWidget::GetAccessoryImageByIndex(int index) {
    static constexpr AddonImageMember kTable[] = {
        &AddonZombatarImagesType::zombatar_accessory_1,
        &AddonZombatarImagesType::zombatar_accessory_2,
        &AddonZombatarImagesType::zombatar_accessory_3,
        &AddonZombatarImagesType::zombatar_accessory_4,
        &AddonZombatarImagesType::zombatar_accessory_5,
        &AddonZombatarImagesType::zombatar_accessory_6,
        &AddonZombatarImagesType::zombatar_accessory_8,
        &AddonZombatarImagesType::zombatar_accessory_9,
        &AddonZombatarImagesType::zombatar_accessory_10,
        &AddonZombatarImagesType::zombatar_accessory_11,
        &AddonZombatarImagesType::zombatar_accessory_12,
        &AddonZombatarImagesType::zombatar_accessory_13,
        &AddonZombatarImagesType::zombatar_accessory_14,
        &AddonZombatarImagesType::zombatar_accessory_15,
        &AddonZombatarImagesType::zombatar_accessory_16,
    };
    return GetAddonImageByIndex(index, kTable);
}

Image *ZombatarWidget::GetHatImageByIndex(int index) {
    static constexpr AddonImageMember kTable[] = {
        &AddonZombatarImagesType::zombatar_hats_1,
        &AddonZombatarImagesType::zombatar_hats_2,
        &AddonZombatarImagesType::zombatar_hats_3,
        &AddonZombatarImagesType::zombatar_hats_4,
        &AddonZombatarImagesType::zombatar_hats_5,
        &AddonZombatarImagesType::zombatar_hats_6,
        &AddonZombatarImagesType::zombatar_hats_7,
        &AddonZombatarImagesType::zombatar_hats_8,
        &AddonZombatarImagesType::zombatar_hats_9,
        &AddonZombatarImagesType::zombatar_hats_10,
        &AddonZombatarImagesType::zombatar_hats_11,
        &AddonZombatarImagesType::zombatar_hats_12,
        &AddonZombatarImagesType::zombatar_hats_13,
        &AddonZombatarImagesType::zombatar_hats_14,
    };
    return GetAddonImageByIndex(index, kTable);
}

Image *ZombatarWidget::GetHatMaskImageByIndex(int index) {
    static constexpr AddonImageMember kTable[] = {
        &AddonZombatarImagesType::zombatar_hats_1_mask,
        nullptr,
        &AddonZombatarImagesType::zombatar_hats_3_mask,
        nullptr,
        nullptr,
        &AddonZombatarImagesType::zombatar_hats_6_mask,
        &AddonZombatarImagesType::zombatar_hats_7_mask,
        &AddonZombatarImagesType::zombatar_hats_8_mask,
        &AddonZombatarImagesType::zombatar_hats_9_mask,
        nullptr,
        &AddonZombatarImagesType::zombatar_hats_11_mask,
    };
    return GetAddonImageByIndex(index, kTable);
}

Image *ZombatarWidget::GetBackgroundImageByIndex(int index) {
    static constexpr AddonImageMember kTable[] = {
        &AddonZombatarImagesType::zombatar_background_blank,
        &AddonZombatarImagesType::zombatar_background_hood,
        &AddonZombatarImagesType::zombatar_background_hood_blue,
        &AddonZombatarImagesType::zombatar_background_hood_brown,
        &AddonZombatarImagesType::zombatar_background_hood_yellow,
        &AddonZombatarImagesType::zombatar_background_crazydave,
        &AddonZombatarImagesType::zombatar_background_crazydave_night,
        &AddonZombatarImagesType::zombatar_background_menu_dos,
        &AddonZombatarImagesType::zombatar_background_menu,
        &AddonZombatarImagesType::zombatar_background_sky_day,
        &AddonZombatarImagesType::zombatar_background_sky_night,
        &AddonZombatarImagesType::zombatar_background_mausoleum,
        &AddonZombatarImagesType::zombatar_background_day_RV,
        &AddonZombatarImagesType::zombatar_background_night_RV,
        &AddonZombatarImagesType::zombatar_background_pool_sunshade,
        &AddonZombatarImagesType::zombatar_background_fog_sunshade,
        &AddonZombatarImagesType::zombatar_background_roof,
        &AddonZombatarImagesType::zombatar_background_roof_distant,
        &AddonZombatarImagesType::zombatar_background_moon,
        &AddonZombatarImagesType::zombatar_background_moon_distant,
        &AddonZombatarImagesType::zombatar_background_aquarium,
        &AddonZombatarImagesType::zombatar_background_garden_moon,
        &AddonZombatarImagesType::zombatar_background_garden_mushrooms,
        &AddonZombatarImagesType::zombatar_background_garden_hd,
        &AddonZombatarImagesType::zombatar_background_sky_purple,
        &AddonZombatarImagesType::zombatar_background_7,
        &AddonZombatarImagesType::zombatar_background_8,
        &AddonZombatarImagesType::zombatar_background_9,
        &AddonZombatarImagesType::zombatar_background_10,
        &AddonZombatarImagesType::zombatar_background_11,
        &AddonZombatarImagesType::zombatar_background_11_1,
        &AddonZombatarImagesType::zombatar_background_12,
        &AddonZombatarImagesType::zombatar_background_12_1,
        &AddonZombatarImagesType::zombatar_background_13,
        &AddonZombatarImagesType::zombatar_background_13_1,
        &AddonZombatarImagesType::zombatar_background_14,
        &AddonZombatarImagesType::zombatar_background_14_1,
        &AddonZombatarImagesType::zombatar_background_15,
        &AddonZombatarImagesType::zombatar_background_15_1,
        &AddonZombatarImagesType::zombatar_background_16,
        &AddonZombatarImagesType::zombatar_background_16_1,
        &AddonZombatarImagesType::zombatar_background_17,
        &AddonZombatarImagesType::zombatar_background_17_1,
        &AddonZombatarImagesType::zombatar_background_bej3_bridge_shroom_castles,
        &AddonZombatarImagesType::zombatar_background_bej3_canyon_wall,
        &AddonZombatarImagesType::zombatar_background_bej3_crystal_mountain_peak,
        &AddonZombatarImagesType::zombatar_background_bej3_dark_cave_thing,
        &AddonZombatarImagesType::zombatar_background_bej3_desert_pyramids_sunset,
        &AddonZombatarImagesType::zombatar_background_bej3_fairy_cave_village,
        &AddonZombatarImagesType::zombatar_background_bej3_floating_rock_city,
        &AddonZombatarImagesType::zombatar_background_bej3_horse_forset_tree,
        &AddonZombatarImagesType::zombatar_background_bej3_jungle_ruins_path,
        &AddonZombatarImagesType::zombatar_background_bej3_lantern_plants_world,
        &AddonZombatarImagesType::zombatar_background_bej3_lightning,
        &AddonZombatarImagesType::zombatar_background_bej3_lion_tower_cascade,
        &AddonZombatarImagesType::zombatar_background_bej3_pointy_ice_path,
        &AddonZombatarImagesType::zombatar_background_bej3_pointy_ice_path_purple,
        &AddonZombatarImagesType::zombatar_background_bej3_rock_city_lake,
        &AddonZombatarImagesType::zombatar_background_bej3_snowy_cliffs_castle,
        &AddonZombatarImagesType::zombatar_background_bej3_treehouse_waterfall,
        &AddonZombatarImagesType::zombatar_background_bej3_tube_forest_night,
        &AddonZombatarImagesType::zombatar_background_bej3_water_bubble_city,
        &AddonZombatarImagesType::zombatar_background_bej3_water_fall_cliff,
        &AddonZombatarImagesType::zombatar_background_bejblitz_6,
        &AddonZombatarImagesType::zombatar_background_bejblitz_8,
        &AddonZombatarImagesType::zombatar_background_bejblitz_main_menu,
        &AddonZombatarImagesType::zombatar_background_peggle_bunches,
        &AddonZombatarImagesType::zombatar_background_peggle_fever,
        &AddonZombatarImagesType::zombatar_background_peggle_level1,
        &AddonZombatarImagesType::zombatar_background_peggle_level4,
        &AddonZombatarImagesType::zombatar_background_peggle_level5,
        &AddonZombatarImagesType::zombatar_background_peggle_menu,
        &AddonZombatarImagesType::zombatar_background_peggle_nights_bjorn3,
        &AddonZombatarImagesType::zombatar_background_peggle_nights_bjorn4,
        &AddonZombatarImagesType::zombatar_background_peggle_nights_claude5,
        &AddonZombatarImagesType::zombatar_background_peggle_nights_kalah1,
        &AddonZombatarImagesType::zombatar_background_peggle_nights_kalah4,
        &AddonZombatarImagesType::zombatar_background_peggle_nights_master5,
        &AddonZombatarImagesType::zombatar_background_peggle_nights_renfield5,
        &AddonZombatarImagesType::zombatar_background_peggle_nights_tut5,
        &AddonZombatarImagesType::zombatar_background_peggle_nights_warren3,
        &AddonZombatarImagesType::zombatar_background_peggle_paperclips,
        &AddonZombatarImagesType::zombatar_background_peggle_waves,
    };
    return GetAddonImageByIndex(index, kTable);
}

int ZombatarWidget::GetTidBitImageOffsetXByIndex(int index) {
    static constexpr int kTable[] = {35, 35, 55, 38, 40, 34, 33, 40, 26, 44, 43, 103, 106, 136};
    return GetIntByIndex(index, kTable);
}

int ZombatarWidget::GetTidBitImageOffsetYByIndex(int index) {
    static constexpr int kTable[] = {76, 76, 133, 74, 70, 79, 86, 66, 91, 86, 84, 110, 60, 137};
    return GetIntByIndex(index, kTable);
}

int ZombatarWidget::GetEyeWearImageOffsetXByIndex(int index) {
    static constexpr int kTable[] = {34, 38, 34, 34, 37, 36, 61, 39, 44, 37, 37, 46, 36, 41, 50, 41};
    return GetIntByIndex(index, kTable);
}

int ZombatarWidget::GetEyeWearImageOffsetYByIndex(int index) {
    static constexpr int kTable[] = {87, 101, 84, 94, 91, 92, 107, 84, 120, 89, 80, 113, 97, 77, 78, 78};
    return GetIntByIndex(index, kTable);
}

int ZombatarWidget::GetAccessoryImageOffsetXByIndex(int index) {
    static constexpr int kTable[] = {124, 130, 103, 157, 158, 158, 126, 144, 74, 52, 163, 94, 83, 160, 16};
    return GetIntByIndex(index, kTable);
}

int ZombatarWidget::GetAccessoryImageOffsetYByIndex(int index) {
    static constexpr int kTable[] = {132, 132, 135, 115, 120, 120, 132, 80, 141, 119, 110, 157, 174, 84, 48};
    return GetIntByIndex(index, kTable);
}

int ZombatarWidget::GetFHairImageOffsetXByIndex(int index) {
    static constexpr int kTable[] = {
        42, 61, 54, 46, 85, 59, 16, 54, 51, 54, 52, 35, 52, 23, 76, 76, 52, 71, 55, 137, 141, 18, 71, 46,
    };
    return GetIntByIndex(index, kTable);
}

int ZombatarWidget::GetFHairImageOffsetYByIndex(int index) {
    static constexpr int kTable[] = {
        129, 132, 132, 126, 174, 135, 128, 125, 126, 134, 105, 125, 131, 122, 174, 168, 132, 115, 110, 96, 102, 104, 174, 129,
    };
    return GetIntByIndex(index, kTable);
}

int ZombatarWidget::GetFHairMaskImageOffsetXByIndex(int index) {
    static constexpr std::pair<int, int> kTable[] = {
        {3, 3},
        {7, 1},
        {10, 1},
        {11, 9},
        {13, 2},
        {14, 2},
        {17, 2},
        {21, 4},
        {22, 1},
        {23, 4},
    };
    return GetSparseIntByIndex(index, kTable);
}

int ZombatarWidget::GetFHairMaskImageOffsetYByIndex(int index) {
    static constexpr std::pair<int, int> kTable[] = {
        {3, 1},
        {9, 1},
        {10, 3},
        {11, 1},
        {13, 1},
        {17, 2},
        {21, 3},
    };
    return GetSparseIntByIndex(index, kTable);
}

int ZombatarWidget::GetHairImageOffsetXByIndex(int index) {
    static constexpr int kTable[] = {28, 29, 28, 35, 44, 47, 61, 33, 154, 27, 30, 60, 39, 9, 55, 31};
    return GetIntByIndex(index, kTable);
}

int ZombatarWidget::GetHairImageOffsetYByIndex(int index) {
    static constexpr int kTable[] = {0, 31, 36, 17, 45, 16, 26, 17, 66, 37, 22, -5, 16, -2, 4, 23};
    return GetIntByIndex(index, kTable);
}

int ZombatarWidget::GetHairMaskImageOffsetXByIndex(int index) {
    static constexpr std::pair<int, int> kTable[] = {
        {0, 10},
        {1, 2},
        {10, 2},
        {11, 2},
        {12, 2},
        {13, 2},
        {14, -1},
    };
    return GetSparseIntByIndex(index, kTable);
}

int ZombatarWidget::GetHairMaskImageOffsetYByIndex(int index) {
    static constexpr std::pair<int, int> kTable[] = {
        {0, 2},
        {1, 3},
        {10, 2},
        {11, 2},
        {12, 1},
        {13, 5},
    };
    return GetSparseIntByIndex(index, kTable);
}

int ZombatarWidget::GetClothImageOffsetXByIndex(int index) {
    static constexpr AddonImageMember kImages[] = {
        &AddonZombatarImagesType::zombatar_clothes_1,
        &AddonZombatarImagesType::zombatar_clothes_2,
        &AddonZombatarImagesType::zombatar_clothes_3,
        &AddonZombatarImagesType::zombatar_clothes_4,
        &AddonZombatarImagesType::zombatar_clothes_5,
        &AddonZombatarImagesType::zombatar_clothes_6,
        &AddonZombatarImagesType::zombatar_clothes_7,
        &AddonZombatarImagesType::zombatar_clothes_8,
        &AddonZombatarImagesType::zombatar_clothes_9,
        &AddonZombatarImagesType::zombatar_clothes_10,
        &AddonZombatarImagesType::zombatar_clothes_11,
        &AddonZombatarImagesType::zombatar_clothes_12,
    };
    static constexpr int kDelta[] = {2, 11, 1, 1, 0, -20, 3, 2, 10, 3, 1, 3};
    return GetAddonDimensionOffsetByIndex(index, kImages, kDelta, true);
}

int ZombatarWidget::GetClothImageOffsetYByIndex(int index) {
    static constexpr AddonImageMember kImages[] = {
        &AddonZombatarImagesType::zombatar_clothes_1,
        &AddonZombatarImagesType::zombatar_clothes_2,
        &AddonZombatarImagesType::zombatar_clothes_3,
        &AddonZombatarImagesType::zombatar_clothes_4,
        &AddonZombatarImagesType::zombatar_clothes_5,
        &AddonZombatarImagesType::zombatar_clothes_6,
        &AddonZombatarImagesType::zombatar_clothes_7,
        &AddonZombatarImagesType::zombatar_clothes_8,
        &AddonZombatarImagesType::zombatar_clothes_9,
        &AddonZombatarImagesType::zombatar_clothes_10,
        &AddonZombatarImagesType::zombatar_clothes_11,
        &AddonZombatarImagesType::zombatar_clothes_12,
    };
    static constexpr int kDelta[] = {0, 6, 0, 2, 2, -24, 2, 4, 3, 2, 2, 3};
    return GetAddonDimensionOffsetByIndex(index, kImages, kDelta, false);
}

int ZombatarWidget::GetHatImageOffsetXByIndex(int index) {
    static constexpr int kTable[] = {33, 58, 44, 12, 49, 22, 64, 4, 45, 17, 76, 51, 22, 28};
    return GetIntByIndex(index, kTable);
}

int ZombatarWidget::GetHatImageOffsetYByIndex(int index) {
    static constexpr int kTable[] = {5, 14, 25, 12, 20, 5, 21, -1, -1, 56, 10, 16, 0, 6};
    return GetIntByIndex(index, kTable);
}

int ZombatarWidget::GetHatMaskImageOffsetXByIndex(int index) {
    static constexpr std::pair<int, int> kTable[] = {
        {0, 1},
        {2, 18},
        {5, 5},
        {7, -3},
        {8, -1},
        {10, 1},
    };
    return GetSparseIntByIndex(index, kTable);
}

int ZombatarWidget::GetHatMaskImageOffsetYByIndex(int index) {
    static constexpr std::pair<int, int> kTable[] = {
        {0, 1},
        {5, -2},
        {6, 17},
        {7, -3},
        {8, -2},
        {10, 16},
    };
    return GetSparseIntByIndex(index, kTable);
}

bool ZombatarWidget::AccessoryIsColorized(int tab, int accessory) {
    switch (tab) {
        case ZombatarWidget::HAIR:
            return accessory != 2;
        case ZombatarWidget::FHAIR:
            return true;
        case ZombatarWidget::TIDBIT:
            return accessory == 0 || accessory == 1 || accessory == 2 || accessory == 9 || accessory == 10 || accessory == 11;
        case ZombatarWidget::EYEWEAR:
            return accessory <= 11;
        case ZombatarWidget::CLOTHES:
            return false;
        case ZombatarWidget::ACCESSORY:
            return accessory == 7 || accessory == 9 || accessory == 11 || accessory == 12;
        case ZombatarWidget::HAT:
            return accessory != 12;
        case ZombatarWidget::BACKGROUND:
            return accessory == 0;
        default:
            return false;
    }
}

void ZombatarWidget::MouseDownSkin(int x, int y) {
    for (int i = 0; i < 12; ++i) {
        int theX = 160 + 285 + (i % 9) * 30;
        int theY = 432 + i / 9 * 30;
        Sexy::Rect rect = {theX, theY, 30, 30};
        if (TRect_Contains(&rect, x, y)) {
            mSelectedSkinColor = i;
            return;
        }
    }
}

void ZombatarWidget::MouseDownHair(int x, int y) {
    for (int i = 0; i < 16; ++i) {
        int theX = 160 + 198 + (i % 6) * 73;
        int theY = 162 + i / 6 * 79;
        Sexy::Rect rect = {theX, theY, 73, 79};
        if (TRect_Contains(&rect, x, y)) {
            mSelectedHair = i;
            Reanimation_SetZombatarHair(mZombatarReanim, mSelectedHair, mSelectedHairColor);
            return;
        }
    }
    int theX = 160 + 198 + (16 % 6) * 73;
    int theY = 162 + 16 / 6 * 79;
    Sexy::Rect rect = {theX, theY, 73, 79};
    if (TRect_Contains(&rect, x, y)) {
        mSelectedHair = 255;
        Reanimation_SetZombatarHair(mZombatarReanim, mSelectedHair, mSelectedHairColor);
        return;
    }
    if (mSelectedHair != 255 && ZombatarWidget::AccessoryIsColorized(mSelectedTab, mSelectedHair)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Sexy::Rect rect = {theX, theY, 30, 30};
            if (TRect_Contains(&rect, x, y)) {
                mSelectedHairColor = i;
                Reanimation_SetZombatarHair(mZombatarReanim, mSelectedHair, mSelectedHairColor);
                return;
            }
        }
    }
}

void ZombatarWidget::MouseDownFHair(int x, int y) {
    if (mSelectedFHairPage == 0) {
        for (int i = 0; i < 17; ++i) {
            int theX = 160 + 198 + (i % 6) * 73;
            int theY = 162 + i / 6 * 79;
            Sexy::Rect rect = {theX, theY, 73, 79};
            if (TRect_Contains(&rect, x, y)) {
                mSelectedFHair = i;
                Reanimation_SetZombatarFHair(mZombatarReanim, mSelectedFHair, mSelectedFHairColor);
                return;
            }
        }
        int theX = 160 + 198 + (17 % 6) * 73;
        int theY = 162 + 17 / 6 * 79;
        Sexy::Rect rect = {theX, theY, 73, 79};
        if (TRect_Contains(&rect, x, y)) {
            mSelectedFHair = 255;
            Reanimation_SetZombatarFHair(mZombatarReanim, mSelectedFHair, mSelectedFHairColor);
            return;
        }
        Sexy::Rect next = {160 + 588, 436, addonZombatarImages.zombatar_next_button->mWidth, addonZombatarImages.zombatar_next_button->mHeight};
        if (TRect_Contains(&next, x, y)) {
            mSelectedFHairPage++;
            return;
        }
    } else if (mSelectedFHairPage == 1) {
        for (int i = 0; i < 7; ++i) {
            int theX = 160 + 198 + (i % 6) * 73;
            int theY = 162 + i / 6 * 79;
            Sexy::Rect rect = {theX, theY, 73, 79};
            if (TRect_Contains(&rect, x, y)) {
                mSelectedFHair = i + 17;
                Reanimation_SetZombatarFHair(mZombatarReanim, mSelectedFHair, mSelectedFHairColor);
                return;
            }
        }
        int theX = 160 + 198 + (7 % 6) * 73;
        int theY = 162 + 7 / 6 * 79;
        Sexy::Rect rect = {theX, theY, 73, 79};
        if (TRect_Contains(&rect, x, y)) {
            mSelectedFHair = 255;
            Reanimation_SetZombatarFHair(mZombatarReanim, mSelectedFHair, mSelectedFHairColor);
            return;
        }
        Sexy::Rect prev = {160 + 209, 436, addonZombatarImages.zombatar_next_button->mWidth, addonZombatarImages.zombatar_next_button->mHeight};
        if (TRect_Contains(&prev, x, y)) {
            mSelectedFHairPage--;
            return;
        }
    }

    if (mSelectedFHair != 255 && ZombatarWidget::AccessoryIsColorized(mSelectedTab, mSelectedFHair)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Sexy::Rect rect = {theX, theY, 30, 30};
            if (TRect_Contains(&rect, x, y)) {
                mSelectedFHairColor = i;
                Reanimation_SetZombatarFHair(mZombatarReanim, mSelectedFHair, mSelectedFHairColor);
                return;
            }
        }
    }
}

void ZombatarWidget::MouseDownTidBit(int x, int y) {
    for (int i = 0; i < 14; ++i) {
        int theX = 160 + 198 + (i % 6) * 73;
        int theY = 162 + i / 6 * 79;
        Sexy::Rect rect = {theX, theY, 73, 79};
        if (TRect_Contains(&rect, x, y)) {
            mSelectedTidBit = i;
            Reanimation_SetZombatarTidBits(mZombatarReanim, mSelectedTidBit, mSelectedTidBitColor);
            return;
        }
    }
    int theX = 160 + 198 + (14 % 6) * 73;
    int theY = 162 + 14 / 6 * 79;
    Sexy::Rect rect = {theX, theY, 73, 79};
    if (TRect_Contains(&rect, x, y)) {
        mSelectedTidBit = 255;
        Reanimation_SetZombatarTidBits(mZombatarReanim, mSelectedTidBit, mSelectedTidBitColor);
        return;
    }

    if (mSelectedTidBit != 255 && ZombatarWidget::AccessoryIsColorized(mSelectedTab, mSelectedTidBit)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Sexy::Rect rect = {theX, theY, 30, 30};
            if (TRect_Contains(&rect, x, y)) {
                mSelectedTidBitColor = i;
                Reanimation_SetZombatarTidBits(mZombatarReanim, mSelectedTidBit, mSelectedTidBitColor);
                return;
            }
        }
    }
}

void ZombatarWidget::MouseDownEyeWear(int x, int y) {

    for (int i = 0; i < 16; ++i) {
        int theX = 160 + 198 + (i % 6) * 73;
        int theY = 162 + i / 6 * 79;
        Sexy::Rect rect = {theX, theY, 73, 79};
        if (TRect_Contains(&rect, x, y)) {
            mSelectedEyeWear = i;
            Reanimation_SetZombatarEyeWear(mZombatarReanim, mSelectedEyeWear, mSelectedEyeWearColor);
            return;
        }
    }
    int theX = 160 + 198 + (16 % 6) * 73;
    int theY = 162 + 16 / 6 * 79;
    Sexy::Rect rect = {theX, theY, 73, 79};
    if (TRect_Contains(&rect, x, y)) {
        mSelectedEyeWear = 255;
        Reanimation_SetZombatarEyeWear(mZombatarReanim, mSelectedEyeWear, mSelectedEyeWearColor);
        return;
    }

    if (mSelectedEyeWear != 255 && ZombatarWidget::AccessoryIsColorized(mSelectedTab, mSelectedEyeWear)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Sexy::Rect rect = {theX, theY, 30, 30};
            if (TRect_Contains(&rect, x, y)) {
                mSelectedEyeWearColor = i;
                Reanimation_SetZombatarEyeWear(mZombatarReanim, mSelectedEyeWear, mSelectedEyeWearColor);
                return;
            }
        }
    }
}

void ZombatarWidget::MouseDownCloth(int x, int y) {
    for (int i = 0; i < 12; ++i) {
        int theX = 160 + 198 + (i % 6) * 73;
        int theY = 162 + i / 6 * 79;
        Sexy::Rect rect = {theX, theY, 73, 79};
        if (TRect_Contains(&rect, x, y)) {
            mSelectedCloth = i;
            return;
        }
    }
    int theX = 160 + 198 + (12 % 6) * 73;
    int theY = 162 + 12 / 6 * 79;
    Sexy::Rect rect = {theX, theY, 73, 79};
    if (TRect_Contains(&rect, x, y)) {
        mSelectedCloth = 255;
        return;
    }
}

void ZombatarWidget::MouseDownAccessory(int x, int y) {
    for (int i = 0; i < 15; ++i) {
        int theX = 160 + 198 + (i % 6) * 73;
        int theY = 162 + i / 6 * 79;
        Sexy::Rect rect = {theX, theY, 73, 79};
        if (TRect_Contains(&rect, x, y)) {
            mSelectedAccessory = i;
            Reanimation_SetZombatarAccessories(mZombatarReanim, mSelectedAccessory, mSelectedAccessoryColor);
            return;
        }
    }
    int theX = 160 + 198 + (15 % 6) * 73;
    int theY = 162 + 15 / 6 * 79;
    Sexy::Rect rect = {theX, theY, 73, 79};
    if (TRect_Contains(&rect, x, y)) {
        mSelectedAccessory = 255;
        Reanimation_SetZombatarAccessories(mZombatarReanim, mSelectedAccessory, mSelectedAccessoryColor);
        return;
    }

    if (mSelectedAccessory != 255 && ZombatarWidget::AccessoryIsColorized(mSelectedTab, mSelectedAccessory)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Sexy::Rect rect = {theX, theY, 30, 30};
            if (TRect_Contains(&rect, x, y)) {
                mSelectedAccessoryColor = i;
                Reanimation_SetZombatarAccessories(mZombatarReanim, mSelectedAccessory, mSelectedAccessoryColor);
                return;
            }
        }
    }
}

void ZombatarWidget::MouseDownHat(int x, int y) {
    for (int i = 0; i < 14; ++i) {
        int theX = 160 + 198 + (i % 6) * 73;
        int theY = 162 + i / 6 * 79;
        Sexy::Rect rect = {theX, theY, 73, 79};
        if (TRect_Contains(&rect, x, y)) {
            mSelectedHat = i;
            Reanimation_SetZombatarHats(mZombatarReanim, mSelectedHat, mSelectedHatColor);
            return;
        }
    }
    int theX = 160 + 198 + (14 % 6) * 73;
    int theY = 162 + 14 / 6 * 79;
    Sexy::Rect rect = {theX, theY, 73, 79};
    if (TRect_Contains(&rect, x, y)) {
        mSelectedHat = 255;
        Reanimation_SetZombatarHats(mZombatarReanim, mSelectedHat, mSelectedHatColor);
        return;
    }

    if (mSelectedHat != 255 && ZombatarWidget::AccessoryIsColorized(mSelectedTab, mSelectedHat)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Sexy::Rect rect = {theX, theY, 30, 30};
            if (TRect_Contains(&rect, x, y)) {
                mSelectedHatColor = i;
                Reanimation_SetZombatarHats(mZombatarReanim, mSelectedHat, mSelectedHatColor);
                return;
            }
        }
    }
}

void ZombatarWidget::MouseDownBackground(int x, int y) {
    if (mSelectedBackgroundPage == 0) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 198 + (i % 6) * 73;
            int theY = 162 + i / 6 * 79;
            Sexy::Rect rect = {theX, theY, 73, 79};
            if (TRect_Contains(&rect, x, y)) {
                mSelectedBackground = i;
                return;
            }
        }
        Sexy::Rect next = {160 + 588, 436, addonZombatarImages.zombatar_next_button->mWidth, addonZombatarImages.zombatar_next_button->mHeight};
        if (TRect_Contains(&next, x, y)) {
            mSelectedBackgroundPage++;
            return;
        }
    } else if (mSelectedBackgroundPage == 4) {
        for (int i = 0; i < 11; ++i) {
            int theX = 160 + 198 + (i % 6) * 73;
            int theY = 162 + i / 6 * 79;
            Sexy::Rect rect = {theX, theY, 73, 79};
            if (TRect_Contains(&rect, x, y)) {
                mSelectedBackground = i + 18 * mSelectedBackgroundPage;
                return;
            }
        }
        Sexy::Rect prev = {160 + 209, 436, addonZombatarImages.zombatar_next_button->mWidth, addonZombatarImages.zombatar_next_button->mHeight};
        if (TRect_Contains(&prev, x, y)) {
            mSelectedBackgroundPage--;
            return;
        }
    } else {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 198 + (i % 6) * 73;
            int theY = 162 + i / 6 * 79;
            Sexy::Rect rect = {theX, theY, 73, 79};
            if (TRect_Contains(&rect, x, y)) {
                mSelectedBackground = i + 18 * mSelectedBackgroundPage;
                return;
            }
        }
        Sexy::Rect next = {160 + 588, 436, addonZombatarImages.zombatar_next_button->mWidth, addonZombatarImages.zombatar_next_button->mHeight};
        if (TRect_Contains(&next, x, y)) {
            mSelectedBackgroundPage++;
            return;
        }
        Sexy::Rect prev = {160 + 209, 436, addonZombatarImages.zombatar_next_button->mWidth, addonZombatarImages.zombatar_next_button->mHeight};
        if (TRect_Contains(&prev, x, y)) {
            mSelectedBackgroundPage--;
            return;
        }
    }


    if (ZombatarWidget::AccessoryIsColorized(mSelectedTab, mSelectedBackground)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Sexy::Rect rect = {theX, theY, 30, 30};
            if (TRect_Contains(&rect, x, y)) {
                mSelectedBackgroundColor = i;
                return;
            }
        }
    }
}

void ZombatarWidget::MouseDown(int x, int y) {
    xx = x;
    yy = y;
    // Sexy_Widget_Move(mBackButton,xx,yy);
    // mPreviewZombie->mX = x;
    // mPreviewZombie->mY = y;
    LOG_DEBUG("{} {}", x, y);

    if (gMainMenuZombatarWidget->mShowExistingZombatarPortrait) {
        return;
    }
    for (char i = 0; i < ZombatarWidget::MAX_TAB_NUM; ++i) {
        Sexy::Rect rect = {160 + 67, 152 + i * 43, 125, 47};
        if (TRect_Contains(&rect, x, y)) {
            mSelectedTab = i;
            mSelectedFHairPage = 0;
            mSelectedBackgroundPage = 0;
            return;
        }
    }

    switch (mSelectedTab) {
        case ZombatarWidget::SKIN:
            MouseDownSkin(x, y);
            break;
        case ZombatarWidget::HAIR:
            MouseDownHair(x, y);
            break;
        case ZombatarWidget::FHAIR:
            MouseDownFHair(x, y);
            break;
        case ZombatarWidget::TIDBIT:
            MouseDownTidBit(x, y);
            break;
        case ZombatarWidget::EYEWEAR:
            MouseDownEyeWear(x, y);
            break;
        case ZombatarWidget::CLOTHES:
            MouseDownCloth(x, y);
            break;
        case ZombatarWidget::ACCESSORY:
            MouseDownAccessory(x, y);
            break;
        case ZombatarWidget::HAT:
            MouseDownHat(x, y);
            break;
        case ZombatarWidget::BACKGROUND:
            MouseDownBackground(x, y);
            break;
    }
}

void ZombatarWidget::MouseDrag(int x, int y) {
    xx = x;
    yy = y;
    // Sexy_Widget_Move(mBackButton,xx,yy);
    // mPreviewZombie->mX = x;
    // mPreviewZombie->mY = y;
    LOG_DEBUG("{} {}", x, y);
}

void ZombatarWidget::MouseUp(int x, int y) {}

void ZombatarWidget::KeyDown(KeyCode theKey) {
    if (theKey == KeyCode::KEYCODE_ESCAPE || theKey == KeyCode::KEYCODE_GAMEPAD_B) {
        LawnApp *lawnApp = gLawnApp;
        lawnApp->KillZombatarScreen();
        lawnApp->ShowMainMenuScreen();
        return;
    }
    if (theKey == KeyCode::KEYCODE_UP) {
        yy--;
        LOG_DEBUG("{} {}", xx, yy);
    }
    if (theKey == KeyCode::KEYCODE_DOWN) {
        yy++;
        LOG_DEBUG("{} {}", xx, yy);
    }
    if (theKey == KeyCode::KEYCODE_LEFT) {
        xx--;
        LOG_DEBUG("{} {}", xx, yy);
    }
    if (theKey == KeyCode::KEYCODE_RIGHT) {
        xx++;
        LOG_DEBUG("{} {}", xx, yy);
    }
    if (theKey == KeyCode::KEYCODE_UP || theKey == KeyCode::KEYCODE_DOWN || theKey == KeyCode::KEYCODE_LEFT || theKey == KeyCode::KEYCODE_RIGHT) {
        return;
    }
}

void TestMenuWidget_Delete(ZombatarWidget *zombatarWidget) {
    zombatarWidget->Delete();
}

void TestMenuWidget_Delete2(ZombatarWidget *zombatarWidget) {
    zombatarWidget->~ZombatarWidget();
}

void TestMenuWidget_RemovedFromManager(ZombatarWidget *zombatarWidget, Sexy::WidgetManager *manager) {
    zombatarWidget->RemovedFromManager(manager);
}

void TestMenuWidget_Update(ZombatarWidget *zombatarWidget) {
    zombatarWidget->Update();
}

void TestMenuWidget_Draw(ZombatarWidget *zombatarWidget, Sexy::Graphics *graphics) {
    zombatarWidget->Draw(graphics);
}

void TestMenuWidget_MouseDown(ZombatarWidget *zombatarWidget, int x, int y) {
    zombatarWidget->MouseDown(x, y);
}

void TestMenuWidget_MouseUp(ZombatarWidget *zombatarWidget, int x, int y) {
    zombatarWidget->MouseUp(x, y);
}

void TestMenuWidget_MouseDrag(ZombatarWidget *zombatarWidget, int x, int y) {
    zombatarWidget->MouseDrag(x, y);
}

void TestMenuWidget_KeyDown(ZombatarWidget *zombatarWidget, KeyCode keyCode) {
    zombatarWidget->KeyDown(keyCode);
}
