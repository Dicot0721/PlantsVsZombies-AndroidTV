#include "PvZ/Misc.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Board/Challenge.h"
#include "PvZ/Lawn/Board/Plant.h"
#include "PvZ/Lawn/Board/Zombie.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/System/PoolEffect.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/SexyAppFramework/Graphics/MemoryImage.h"
#include "PvZ/SexyAppFramework/Widget/CustomScrollbarWidget.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Effect/Reanimator.h"
#include "PvZ/SexyAppFramework/Graphics/Font.h"

#include <cassert>

#include <algorithm>
#include <random>

#define HIBYTE(a) ((a) >> 24)
#define BYTE2(a) ((a) >> 16 & 0xFF)
#define BYTE1(a) ((a) >> 8 & 0xFF)
#define LOBYTE(a) ((a) & 0xFF)

using namespace Sexy;

int randomInt(int a, int b) {
    assert(a <= b);
    static std::random_device rd;
    static std::mt19937 gen{rd()};
    std::uniform_int_distribution distrib{a, b};
    return distrib(gen);
}

void WaitForSecondPlayerDialog_WaitForSecondPlayerDialog(int *a, int *a2) {
    // 自动跳过等待2P对话框
    old_WaitForSecondPlayerDialog_WaitForSecondPlayerDialog(a, a2);
    WaitForSecondPlayerDialog_GameButtonDown(a, 6, 1);
    WaitForSecondPlayerDialog_GameButtonDown(a, 6, 1);
}

int SexyDialog_AddedToManager(void *instance, void *instance1) {
    // 记录当前游戏状态
    return old_SexyDialog_AddedToManager(instance, instance1);
}

int SexyDialog_RemovedFromManager(void *instance, void *instance1) {
    // 记录当前游戏状态
    return old_SexyDialog_RemovedFromManager(instance, instance1);
}

void Sexy_String_Delete(int *holder) {
    auto my_exchange_and_add = [](int *ptr, int value) -> int {
        int original_value = *ptr;
        *ptr += value;
        return original_value;
    };

    int ptr = holder[0] - 12;
    if (my_exchange_and_add((int *)(holder[0] - 4), -1) <= 0) {
        operator delete((int *)ptr);
    }
}

void DrawSeedType(Sexy::Graphics *g, float x, float y, SeedType theSeedType, SeedType theImitaterType, float xOffset, float yOffset, float scale) {
    // 和Plant_DrawSeedType配合使用，用于绘制卡槽内的模仿者SeedPacket变白效果。
    g->PushState();
    g->mScaleX = g->mScaleX * scale;
    g->mScaleY = g->mScaleY * scale;
    if (theSeedType == SeedType::SEED_ZOMBIE_TOMBSTONE) {
        TodDrawImageCelScaledF(g, *Sexy_IMAGE_MP_TOMBSTONE_Addr, x + xOffset, y + yOffset, 0, 0, g->mScaleX, g->mScaleY);
    } else {
        if (theSeedType == SeedType::SEED_IMITATER && theImitaterType != SeedType::SEED_NONE) {
            // 卡槽内的模仿者SeedPacket卡且为冷却状态，此时需要交换theImitaterType和theSeedType。
            Plant::DrawSeedType(g, theImitaterType, theSeedType, DrawVariation::VARIATION_NORMAL, x + xOffset, y + yOffset);
        } else {
            Plant::DrawSeedType(g, theSeedType, theImitaterType, DrawVariation::VARIATION_NORMAL, x + xOffset, y + yOffset);
        }
    }
    return g->PopState();
}

void HelpBarWidget_HelpBarWidget(Sexy::Widget *theWidget) {
    // 缩小HelpBar，以防止它挡住触控区域。
    old_HelpBarWidget_HelpBarWidget(theWidget);

    theWidget->Resize(0, 0, 0, 0);
}

void Sexy_ExtractLoadingSoundsResources(int *a, int *theManager) {
    old_Sexy_ExtractLoadingSoundsResources(a, theManager);
}

FoleyParams *LookupFoley(FoleyType theFoleyType) {
    // 新增三个Foley，用于主界面白噪音
    if (theFoleyType == FoleyType::FOLEY_MENU_LEFT) {
        return &gMenuLeftFoley;
    } else if (theFoleyType == FoleyType::FOLEY_MENU_CENTRE) {
        return &gMenuCenterFoley;
    } else if (theFoleyType == FoleyType::FOLEY_MENU_RIGHT) {
        return &gMenuRightFoley;
    } else
        return old_LookupFoley(theFoleyType);
}

void MessageWidget_Draw(CustomMessageWidget *messageWidget, Sexy::Graphics *graphics) {
    // 用于绘制成就图标、在LevelIntro时显示“player的房子”、修复老虎机提示文字位置靠上
    if (messageWidget->mMessageStyle == MessageStyle::MESSAGE_STYLE_SLOT_MACHINE) {
        float tmpTransY = graphics->mTransY;
        graphics->mTransY += 8;
        old_MessageWidget_Draw(messageWidget, graphics);
        graphics->mTransY = tmpTransY;
    } else {
        old_MessageWidget_Draw(messageWidget, graphics);
    }

    if (messageWidget->mMessageStyle == MessageStyle::MESSAGE_STYLE_HOUSE_NAME && messageWidget->mReanimType == ReanimationType::REANIM_NONE) {
        Sexy::Font *theFont = MessageWidget_GetFont(messageWidget);
        int num2 = 530; // 原版为550，此处改为530！
        Color theColor = {255, 255, 255, std::clamp(messageWidget->mDuration * 15, 0, 255)};
        int holder[1];
        Sexy_StrFormat(holder, messageWidget->mLabel);
        Sexy::Rect theRect;
        theRect.mY = num2 - theFont->mAscent;
        theRect.mHeight = 600;
        theRect.mWidth = 800;
        theRect.mX = -messageWidget->mApp->mBoard->mX;
        TodDrawStringWrapped(graphics, holder, &theRect, theFont, &theColor, DrawStringJustification::DS_ALIGN_CENTER, false);
        Sexy_String_Delete(holder);
    }
    if (messageWidget->mIcon != nullptr) {
        TodDrawImageCenterScaledF(graphics, messageWidget->mIcon, 100, 470, 0.55f, 0.55f);
    }
}

void MessageWidget_ClearLabel(CustomMessageWidget *messageWidget) {
    // 用于绘制成就图标
    old_MessageWidget_ClearLabel(messageWidget);
    messageWidget->mIcon = nullptr;
}

void MessageWidget_SetLabel(CustomMessageWidget *messageWidget, int *label, MessageStyle theStyle) {
    // 用于绘制成就图标
    old_MessageWidget_SetLabel(messageWidget, label, theStyle);
    messageWidget->mIcon = nullptr;
}

void MessageWidget_Update(CustomMessageWidget *messageWidget) {
    // 用于绘制成就图标
    if (messageWidget->mDuration == 1) {
        messageWidget->mIcon = nullptr;
    }
    old_MessageWidget_Update(messageWidget);
}

void LawnPlayerInfo_AddCoins(PlayerInfo *playerInfo, int theAmount) {
    // 用于 购物愉快 成就
    int theCoins = playerInfo->mCoins + theAmount;
    if (theCoins > 99999) {
        theCoins = 99999;
    }
    if (theCoins <= 0) {
        theCoins = 0;
    }
    playerInfo->mCoins = theCoins;
    if (theAmount < 0) {
        playerInfo->mUsedCoins -= theAmount;
        if (playerInfo->mUsedCoins >= 2500) {
            LawnApp *gLawnApp = (LawnApp *)*gLawnApp_Addr;
            gLawnApp->GrantAchievement(AchievementId::ACHIEVEMENT_SHOP);
        }
    }
}

void SaveGameContext_SyncReanimationDef(int *theSaveGameContext, ReanimatorDefinition **a2) {
    // 解决大头贴动画的读档问题
    if (*((uint8_t *)theSaveGameContext + 29)) {
        int reanimationType;
        SaveGameContext_SyncInt(theSaveGameContext, &reanimationType);
        if (reanimationType == -1) {
            *a2 = nullptr;
        } else if (reanimationType <= ReanimationType::REANIM_ZOMBATAR_HEAD) {
            ReanimatorEnsureDefinitionLoaded((ReanimationType)reanimationType, true);
            ReanimatorDefinition *v6 = *gReanimatorDefArray_Addr; // r3
            *a2 = v6 + reanimationType;
        } else {
            *((uint8_t *)theSaveGameContext + 28) = true;
        }
    } else {
        int v3 = 0;
        int reanimationType = -1;
        ReanimatorDefinition *v5 = *a2;                       // r1
        ReanimatorDefinition *v6 = *gReanimatorDefArray_Addr; // r3
        while (v5 != v6++) {
            if (++v3 == ReanimationType::REANIM_ZOMBATAR_HEAD + 1)
                goto LABEL_7;
        }
        reanimationType = v3;
    LABEL_7:
        SaveGameContext_SyncInt(theSaveGameContext, &reanimationType);
    }
}