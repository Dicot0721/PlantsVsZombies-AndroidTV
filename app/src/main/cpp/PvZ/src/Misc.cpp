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

int SexyDialog_AddedToManager(void *instance, void *instance1) {
    // 记录当前游戏状态
    return old_SexyDialog_AddedToManager(instance, instance1);
}

int SexyDialog_RemovedFromManager(void *instance, void *instance1) {
    // 记录当前游戏状态
    return old_SexyDialog_RemovedFromManager(instance, instance1);
}

void DrawSeedType(Sexy::Graphics *g, float x, float y, SeedType theSeedType, SeedType theImitaterType, float xOffset, float yOffset, float scale) {
    // 和Plant::DrawSeedType配合使用，用于绘制卡槽内的模仿者SeedPacket变白效果。
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