//
// Created by 28636 on 2025/5/26.
//

#include "PvZ/Lawn/Board/MessageWidget.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/SexyAppFramework/Graphics/Font.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/SexyAppFramework/Misc/Common.h"
#include "PvZ/STL/string.h"
#include "PvZ/TodLib/Common/TodStringFile.h"

using namespace Sexy;

void CustomMessageWidget::Draw(Graphics *g) {
    // 用于绘制成就图标、在LevelIntro时显示“player的房子”、修复老虎机提示文字位置靠上
    if (mMessageStyle == MessageStyle::MESSAGE_STYLE_SLOT_MACHINE) {
        float tmpTransY = g->mTransY;
        g->mTransY += 8;
        old_MessageWidget_Draw(this, g);
        g->mTransY = tmpTransY;
    } else {
        old_MessageWidget_Draw(this, g);
    }

    if (mMessageStyle == MessageStyle::MESSAGE_STYLE_HOUSE_NAME && mReanimType == ReanimationType::REANIM_NONE) {
        Font *theFont = GetFont();
        int num2 = 530; // 原版为550，此处改为530！
        Color theColor = {255, 255, 255, std::clamp(mDuration * 15, 0, 255)};
        pvzstl::string str{mLabel};
        Rect theRect;
        theRect.mY = num2 - theFont->mAscent;
        theRect.mHeight = 600;
        theRect.mWidth = 800;
        theRect.mX = -mApp->mBoard->mX;
        TodDrawStringWrapped(g, (int *)(&str), &theRect, theFont, &theColor, DrawStringJustification::DS_ALIGN_CENTER, false);
    }

    if (mIcon != nullptr) {
        TodDrawImageCenterScaledF(g, mIcon, 100, 470, 0.55f, 0.55f);
    }
}

void CustomMessageWidget::ClearLabel() {
    // 用于绘制成就图标
    old_MessageWidget_ClearLabel(this);

    mIcon = nullptr;
}

void CustomMessageWidget::SetLabel(const SexyString &theLabel, MessageStyle theStyle) {
    // 用于绘制成就图标
    old_MessageWidget_SetLabel(this, theLabel, theStyle);

    mIcon = nullptr;
}

void CustomMessageWidget::Update() {
    // 用于绘制成就图标
    if (mDuration == 1) {
        mIcon = nullptr;
    }

    old_MessageWidget_Update(this);
}