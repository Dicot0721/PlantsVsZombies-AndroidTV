#include "PvZ/Misc.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Board/Challenge.h"
#include "PvZ/Lawn/Board/Plant.h"
#include "PvZ/Lawn/Board/Zombie.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/System/PoolEffect.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/SexyAppFramework/Widget/CustomScrollbarWidget.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Effect/Reanimator.h"
#include "PvZ/SexyAppFramework/Graphics/Font.h"

#include <cassert>

#include <algorithm>
#include <random>
#include <unordered_map>

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

void Sexy_Image_PushTransform(Sexy::Image *image, int *transform, bool concatenate) {
    old_Sexy_Image_PushTransform(image, transform, concatenate);
}

void Sexy_Image_PopTransform(Sexy::Image *image) {
    old_Sexy_Image_PopTransform(image);
}

void Sexy_GLImage_PushTransform(Sexy::Image *image, int *transform, bool concatenate) {
    old_Sexy_GLImage_PushTransform(image, transform, concatenate);
}

void Sexy_GLImage_PopTransform(Sexy::Image *image) {
    old_Sexy_GLImage_PopTransform(image);
}

void Sexy_MemoryImage_PushTransform(Sexy::Image *image, int *transform, bool concatenate) {
    old_Sexy_MemoryImage_PushTransform(image, transform, concatenate);
}

void Sexy_MemoryImage_PopTransform(Sexy::Image *image) {
    old_Sexy_MemoryImage_PopTransform(image);
}

int SexyDialog_AddedToManager(void *instance, void *instance1) {
    // 记录当前游戏状态
    return old_SexyDialog_AddedToManager(instance, instance1);
}

int SexyDialog_RemovedFromManager(void *instance, void *instance1) {
    // 记录当前游戏状态
    return old_SexyDialog_RemovedFromManager(instance, instance1);
}

void LawnMower_Update(int *lawnMover) {
    if (requestPause) {
        return;
    }
    old_LawnMower_Update(lawnMover);
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

unsigned int AverageNearByPixels(Sexy::Image *theImage, unsigned int *aPixel, int y, int x) {
    int v6;                         // edx
    unsigned int *v8;               // eax
    uint32_t *v9;                   // eax
    uint32_t *v10;                  // eax
    int mWidth;                     // ecx
    int v12;                        // eax
    int v13;                        // esi
    int v14;                        // edi
    int v15;                        // ecx
    int v17;                        // eax
    int v18;                        // ebx
    int v19;                        // eax
    int v20;                        // esi
    int v21;                        // eax
    int v22;                        // ecx
    unsigned int aPixelAdjacent[8]; // [esp+Ch] [ebp-24h] BYREF
    int v24;                        // [esp+2Ch] [ebp-4h]

    memset(&aPixelAdjacent[1], 0, 28);
    v24 = 0;
    v6 = -1;
    v8 = aPixelAdjacent;
    do {
        if (v6) {
            v9 = v8 + 1;
            if (x) {
                if (y || v6 != -1) {
                    if (y == theImage->mHeight - 1 && v6 == 1)
                        *v9 = 0;
                    else
                        *v9 = aPixel[v6 * theImage->mWidth - 1];
                } else {
                    *v9 = 0;
                }
            } else {
                *v9 = 0;
            }
            v10 = v9 + 1;
            if (y || v6 != -1) {
                if (y == theImage->mHeight - 1 && v6 == 1)
                    *v10 = 0;
                else
                    *v10 = aPixel[v6 * theImage->mWidth];
            } else {
                *v10 = 0;
            }
            mWidth = theImage->mWidth;
            v8 = v10 + 1;
            if (x == mWidth - 1) {
                *v8 = 0;
            } else if (y || v6 != -1) {
                if (y == theImage->mHeight - 1 && v6 == 1)
                    *v8 = 0;
                else
                    *v8 = aPixel[v6 * mWidth + 1];
            } else {
                *v8 = 0;
            }
        }
        ++v6;
    } while (v6 <= 1);
    v12 = 0;
    v13 = 0;
    v14 = 0;
    v15 = 0;
    if (HIBYTE(aPixelAdjacent[1])) {
        v12 = BYTE2(aPixelAdjacent[1]);
        v13 = BYTE1(aPixelAdjacent[1]);
        v14 = LOBYTE(aPixelAdjacent[1]);
        v15 = 1;
    }
    if (HIBYTE(aPixelAdjacent[2])) {
        v12 += BYTE2(aPixelAdjacent[2]);
        v13 += BYTE1(aPixelAdjacent[2]);
        v14 += LOBYTE(aPixelAdjacent[2]);
        ++v15;
    }
    if (HIBYTE(aPixelAdjacent[3])) {
        v12 += BYTE2(aPixelAdjacent[3]);
        v13 += BYTE1(aPixelAdjacent[3]);
        v14 += LOBYTE(aPixelAdjacent[3]);
        ++v15;
    }
    if (HIBYTE(aPixelAdjacent[4])) {
        v12 += BYTE2(aPixelAdjacent[4]);
        v13 += BYTE1(aPixelAdjacent[4]);
        v14 += LOBYTE(aPixelAdjacent[4]);
        ++v15;
    }
    if (HIBYTE(aPixelAdjacent[5])) {
        v12 += BYTE2(aPixelAdjacent[5]);
        v13 += BYTE1(aPixelAdjacent[5]);
        v14 += LOBYTE(aPixelAdjacent[5]);
        ++v15;
    }
    if (HIBYTE(aPixelAdjacent[6])) {
        v12 += BYTE2(aPixelAdjacent[6]);
        v13 += BYTE1(aPixelAdjacent[6]);
        v14 += LOBYTE(aPixelAdjacent[6]);
        ++v15;
    }
    if (HIBYTE(aPixelAdjacent[7])) {
        v12 += BYTE2(aPixelAdjacent[7]);
        v13 += BYTE1(aPixelAdjacent[7]);
        v14 += LOBYTE(aPixelAdjacent[7]);
        ++v15;
    }
    if (HIBYTE(v24)) {
        v12 += BYTE2(v24);
        v13 += BYTE1(v24);
        v14 += LOBYTE(v24);
        ++v15;
    }
    if (!v15)
        return 0;
    v17 = v12 / v15;
    if (v17 > 0) {
        v18 = 255;
        if (v17 < 255)
            v18 = v17;
    } else {
        v18 = 0;
    }
    v19 = v13 / v15;
    if (v13 / v15 > 0) {
        v20 = 255;
        if (v19 < 255)
            v20 = v19;
    } else {
        v20 = 0;
    }
    v21 = v14 / v15;
    if (v14 / v15 <= 0)
        return (v20 | (v18 << 8)) << 8;
    v22 = 255;
    if (v21 < 255)
        v22 = v21;
    return v22 | ((v20 | (v18 << 8)) << 8);
}

void FixPixelsOnAlphaEdgeForBlending(Sexy::Image *theImage) {
    int v3; // eax
    int i;  // ebx
    int y;  // [esp+Ch] [ebp+4h]

    unsigned int *mBits = theImage->mBits;
    if (mBits != nullptr) {
        v3 = 0;
        y = 0;
        if (theImage->mHeight > 0) {
            do {
                for (i = 0; i < theImage->mWidth; ++i) {
                    unsigned int *v6 = mBits++;
                    if (!*((uint8_t *)v6 + 3)) {
                        *v6 = AverageNearByPixels(theImage, v6, v3, i);
                        v3 = y;
                    }
                }
                y = ++v3;
            } while (v3 < theImage->mHeight);
        }
        //++theImage->mBitsChangedCount;
        Sexy_MemoryImage_BitsChanged(theImage);
    }
}

Sexy::Image *FilterEffectCreateImage(Sexy::Image *theImage, FilterEffect theFilterEffect) {
    Sexy::Image *memoryImage = ((LawnApp *)*gLawnApp_Addr)->CopyImage(theImage);
    memoryImage->mWidth = theImage->mWidth;
    memoryImage->mHeight = theImage->mHeight;
    FixPixelsOnAlphaEdgeForBlending(memoryImage);
    switch (theFilterEffect) {
        case FilterEffect::FILTEREFFECT_WASHED_OUT:
            FilterEffectDoWashedOut(memoryImage);
            break;
        case FilterEffect::FILTEREFFECT_LESS_WASHED_OUT:
            FilterEffectDoLessWashedOut(memoryImage);
            break;
        case FilterEffect::FILTEREFFECT_WHITE:
            FilterEffectDoWhite(memoryImage);
            break;
        case FilterEffect::FILTEREFFECT_CUSTOM:
            FilterEffectDoLumSat(memoryImage, 1.05, 0.8); // 仅MainMenu显示房子雾蒙蒙效果时用到。数值是自己瞎调的
            break;
    }
    //    ++memoryImage->mBitsChangedCount;
    Sexy_MemoryImage_BitsChanged(memoryImage);
    memoryImage->mNumCols = theImage->mNumCols;
    memoryImage->mNumRows = theImage->mNumRows;
    return memoryImage;
}


static std::unordered_map<Sexy::Image *, Sexy::Image *> gFilterEffectMaps[FilterEffect::NUM_FILTEREFFECT];

Image *FilterEffectGetImage(Image *image, FilterEffect mFilterEffect) {
    // 变灰的植物贴图在这里处理
    if (!imitater) {
        return image;
    }
    if (mFilterEffect == FilterEffect::FILTEREFFECT_NONE) {
        return image;
    }
    std::unordered_map<Sexy::Image *, Sexy::Image *> &currentMap = gFilterEffectMaps[mFilterEffect];
    auto it = currentMap.find(image);
    if (it != currentMap.end()) {
        return it->second;
    } else {
        Sexy::Image *theFilterEffectImage = FilterEffectCreateImage(image, mFilterEffect);
        currentMap.emplace(image, theFilterEffectImage);
        return theFilterEffectImage;
    }
}

void FilterEffectDisposeForApp() {}

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

void HelpBarWidget_HelpBarWidget(Sexy::Widget *a) {
    // 缩小HelpBar，以防止它挡住触控区域。
    old_HelpBarWidget_HelpBarWidget(a);
    Sexy_Widget_Resize(a, 0, 0, 0, 0);
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

void Sexy_Widget_MarkDirty(Sexy::Widget *widget) {
    (*((void (**)(Sexy::Widget *))widget->vTable + 25))(widget); // MarkDirty();
}

void Sexy_Widget_AddWidget(Sexy::Widget *parent, Sexy::Widget *child) {
    (*((void (**)(Sexy::Widget *, Sexy::Widget *))parent->vTable + 6))(parent, child); // AddWidget();
}

void Sexy_Widget_RemoveWidget(Sexy::Widget *parent, Sexy::Widget *child) {
    (*((void (**)(Sexy::Widget *, Sexy::Widget *))parent->vTable + 7))(parent, child); // RemoveWidget();
}

Sexy::Widget *Sexy_Widget_FindWidget(Sexy::Widget *parent, int id) {
    return (*((Sexy::Widget * (**)(Sexy::Widget *, int)) parent->vTable + 9))(parent, id); // FindWidget();
}

void Sexy_MemoryImage_ClearRect(Sexy::Image *a1, Sexy::Rect *a2) {
    int mY;     // r4
    int *mBits; // r7
    int v7;     // r0

    mBits = (int *)(*((int (**)(Sexy::Image *))a1->vTable + 13))(a1);
    mY = a2->mY;
    if (mY < a2->mHeight + mY) {
        do {
            v7 = a2->mX + (*((int (**)(Sexy::Image *))a1->vTable + 6))(a1)*mY++;
            memset(&mBits[v7], 0, 4 * a2->mWidth);
        } while (a2->mHeight + a2->mY > mY);
    }
    (*((void (**)(Sexy::Image *))a1->vTable + 47))(a1);
}
