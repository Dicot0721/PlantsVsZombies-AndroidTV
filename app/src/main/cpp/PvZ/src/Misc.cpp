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

void DrawSeedPacket(Sexy::Graphics *g,
                    float x,
                    float y,
                    SeedType seedType,
                    SeedType imitaterType,
                    float coolDownPercent,
                    int grayness,
                    bool drawCostText,
                    bool isInGame,
                    bool isZombieSeed,
                    bool isSeedPacketSelected) {
    // 修复选中紫卡、模仿者卡时卡片背景变为普通卡片背景

    SeedType realSeedType = imitaterType != SeedType::SEED_NONE && seedType == SeedType::SEED_IMITATER ? imitaterType : seedType;
    if (grayness != 255) {
        Color theColor = {grayness, grayness, grayness, 255};
        g->SetColor(theColor);
        g->SetColorizeImages(true);
    } else if (coolDownPercent > 0.0f) {
        Color theColor = {128, 128, 128, 255};
        g->SetColor(theColor);
        g->SetColorizeImages(true);
    }
    int celToDraw;
    if (seedType == SeedType::SEED_IMITATER) {
        celToDraw = 0;
    } else if (Plant::IsUpgrade(realSeedType)) {
        celToDraw = 1;
    } else if (seedType == SeedType::SEED_BEGHOULED_BUTTON_CRATER) {
        celToDraw = 3;
    } else if (seedType == SeedType::SEED_BEGHOULED_BUTTON_SHUFFLE) {
        celToDraw = 4;
    } else if (seedType == SeedType::SEED_SLOT_MACHINE_SUN) {
        celToDraw = 5;
    } else if (seedType == SeedType::SEED_SLOT_MACHINE_DIAMOND) {
        celToDraw = 6;
    } else if (seedType == SeedType::SEED_ZOMBIQUARIUM_SNORKLE) {
        celToDraw = 7;
    } else if (seedType == SeedType::SEED_ZOMBIQUARIUM_TROPHY) {
        celToDraw = 8;
    } else {
        celToDraw = 2;
    }

    if (isSeedPacketSelected) {
        if (g->mScaleX > 1.0f && seedType <= SeedType::SEED_LEFTPEATER) {
            // 紫卡背景BUG就是在这里修复的
            if (celToDraw == 2) {
                TodDrawImageCelScaledF(g, *Sexy_IMAGE_SEEDPACKET_LARGER_Addr, x, y, 0, 0, g->mScaleX * 0.5f, g->mScaleY * 0.5f);
            } else {
                TodDrawImageCelScaledF(g, *Sexy_IMAGE_SEEDS_Addr, x, y, celToDraw, 0, g->mScaleX, g->mScaleY);
            }
        } else if (isZombieSeed) {
            float heightOffset = g->mScaleX > 1.2 ? -1.5f : 0.0f;
            TodDrawImageScaledF(g, *Sexy_IMAGE_ZOMBIE_SEEDPACKET_Addr, x, y + heightOffset, g->mScaleX, g->mScaleY);
        } else {
            TodDrawImageCelScaledF(g, *Sexy_IMAGE_SEEDS_Addr, x, y, celToDraw, 0, g->mScaleX, g->mScaleY);
        }
    }
    bool isPlant = seedType < SeedType::SEED_BEGHOULED_BUTTON_SHUFFLE || seedType > SeedType::SEED_ZOMBIQUARIUM_TROPHY;
    float offsetY, offsetX, theDrawScale;
    switch (realSeedType) {
        case SeedType::SEED_TALLNUT:
            offsetY = 22.0;
            offsetX = 12.0;
            theDrawScale = 0.3;
            break;
        case SeedType::SEED_INSTANT_COFFEE:
            offsetY = 9.0;
            offsetX = 0.0;
            theDrawScale = 0.55;
            break;
        case SeedType::SEED_COBCANNON:
            offsetY = 22.0;
            offsetX = 6.0;
            theDrawScale = 0.26;
            break;
        case SeedType::SEED_CACTUS:
            offsetY = 13.0;
            offsetX = 9.0;
            theDrawScale = 0.5;
            break;
        case SeedType::SEED_MAGNETSHROOM:
            offsetY = 12.0;
            offsetX = 5.0;
            theDrawScale = 0.5;
            break;
        case SeedType::SEED_TWINSUNFLOWER:
        case SeedType::SEED_GLOOMSHROOM:
            offsetY = 14.0;
            offsetX = 7.0;
            theDrawScale = 0.45;
            break;
        case SeedType::SEED_CATTAIL:
            offsetY = 13.0;
            offsetX = 8.0;
            theDrawScale = 0.45;
            break;
        case SeedType::SEED_UMBRELLA:
            offsetY = 10.0;
            offsetX = 5.0;
            theDrawScale = 0.5;
            break;
        case SeedType::SEED_KERNELPULT:
            offsetY = 14.0;
            offsetX = 13.0;
            theDrawScale = 0.4;
            break;
        case SeedType::SEED_CABBAGEPULT:
            offsetY = 14.0;
            offsetX = 15.0;
            theDrawScale = 0.4;
            break;
        case SeedType::SEED_GRAVEBUSTER:
            offsetY = 15.0;
            offsetX = 10.0;
            theDrawScale = 0.4;
            break;
        case SeedType::SEED_SPLITPEA:
            offsetY = 12.0;
            offsetX = 12.0;
            theDrawScale = 0.45;
            break;
        case SeedType::SEED_BLOVER:
            offsetY = 17.0;
            offsetX = 8.0;
            theDrawScale = 0.4;
            break;
        case SeedType::SEED_STARFRUIT:
            offsetY = 8.0;
            offsetX = 6.0;
            theDrawScale = 0.5;
            break;
        case SeedType::SEED_THREEPEATER:
            offsetY = 10.0;
            offsetX = 5.0;
            theDrawScale = 0.5;
            break;
        case SeedType::SEED_GATLINGPEA:
            offsetY = 8.0;
            offsetX = 2.0;
            theDrawScale = 0.5;
            break;
        case SeedType::SEED_ZOMBIE_POLEVAULTER:
            offsetY = -12.0;
            offsetX = -8.0;
            theDrawScale = 0.35;
            break;
        case SeedType::SEED_MELONPULT:
        case SeedType::SEED_WINTERMELON:
            offsetY = 19.0;
            offsetX = 18.0;
            theDrawScale = 0.35;
            break;
        case SeedType::SEED_POTATOMINE:
        case SeedType::SEED_FUMESHROOM:
        case SeedType::SEED_TANGLEKELP:
        case SeedType::SEED_PUMPKINSHELL:
        case SeedType::SEED_CHOMPER:
        case SeedType::SEED_DOOMSHROOM:
        case SeedType::SEED_SQUASH:
        case SeedType::SEED_HYPNOSHROOM:
        case SeedType::SEED_SPIKEWEED:
        case SeedType::SEED_SPIKEROCK:
        case SeedType::SEED_PLANTERN:
        case SeedType::SEED_TORCHWOOD:
            offsetY = 12.0;
            offsetX = 8.0;
            theDrawScale = 0.4;
            break;
        case SeedType::SEED_ZOMBIE_NORMAL:
        case SeedType::SEED_ZOMBIE_NEWSPAPER:
        case SeedType::SEED_ZOMBIE_FLAG:
        case SeedType::SEED_ZOMBIE_TRAFFIC_CONE:
        case SeedType::SEED_ZOMBIE_PAIL:
        case SeedType::SEED_ZOMBIE_DANCER:
            offsetY = -7.0;
            offsetX = -3.0;
            theDrawScale = 0.35;
            break;
        case SeedType::SEED_ZOMBIE_LADDER:
        case SeedType::SEED_ZOMBIE_DIGGER:
        case SeedType::SEED_ZOMBIE_SCREEN_DOOR:
        case SeedType::SEED_ZOMBIE_TRASH_BIN:
        case SeedType::SEED_ZOMBIE_POGO:
        case SeedType::SEED_ZOMBIE_JACK_IN_THE_BOX:
        case SeedType::SEED_ZOMBIE_DUCKY_TUBE:
            offsetY = -10.0;
            offsetX = -3.0;
            theDrawScale = 0.35;
            break;
        case SeedType::SEED_ZOMBIE_DOLPHIN_RIDER:
            offsetY = -12.0;
            offsetX = -3.0;
            theDrawScale = 0.35;
            break;
        case SeedType::SEED_ZOMBIE_SNORKEL:
            offsetY = -8.0;
            offsetX = -3.0;
            theDrawScale = 0.32;
            break;
        case SeedType::SEED_ZOMBIE_BUNGEE:
            offsetY = -1.0;
            offsetX = 1.0;
            theDrawScale = 0.3;
            break;
        case SeedType::SEED_ZOMBIE_FOOTBALL:
            offsetY = -9.0;
            offsetX = -7.0;
            theDrawScale = 0.33;
            break;
        case SeedType::SEED_ZOMBIE_BALLOON:
            offsetY = -5.0;
            offsetX = -3.0;
            theDrawScale = 0.35;
            break;
        case SeedType::SEED_ZOMBIE_IMP:
            offsetY = -17.0;
            offsetX = -12.0;
            theDrawScale = 0.4;
            break;
        case SeedType::SEED_ZOMBONI:
            offsetY = 3.0;
            offsetX = -5.0;
            theDrawScale = 0.23;
            break;
        case SeedType::SEED_ZOMBIE_CATAPULT:
            offsetY = 3.0;
            offsetX = 1.0;
            theDrawScale = 0.23;
            break;
        case SeedType::SEED_ZOMBIE_GARGANTUAR:
        case SeedType::SEED_ZOMBIE_UNKNOWN:
            offsetY = 3.0;
            offsetX = 4.0;
            theDrawScale = 0.23;
            break;
        default:
            offsetY = 8.0;
            offsetX = 5.0;
            theDrawScale = 0.5;
            break;
    }
    LawnApp *lawnApp = (LawnApp *)*gLawnApp_Addr;
    float v28, v29;
    if (lawnApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BIG_TIME) {
        if (realSeedType == SeedType::SEED_SUNFLOWER || realSeedType == SeedType::SEED_WALLNUT || realSeedType == SeedType::SEED_MARIGOLD) {
            offsetY = 34.0;
            offsetX = 16.0;
        }
    }
    v28 = offsetX * g->mScaleX;
    v29 = (offsetY + 1.0f) * g->mScaleY;
    if (realSeedType == SeedType::SEED_GIANT_WALLNUT) {
        v29 = 59.0;
        theDrawScale = theDrawScale * 0.75;
        v28 = 52.0;
    }

    if (isPlant && isSeedPacketSelected)
        DrawSeedType(g, x, y, realSeedType, imitaterType, v28, v29, theDrawScale);

    if (coolDownPercent > 0.0) {
        float coolDownHeight = coolDownPercent * 68.0 + 2.5;
        Graphics aPlantG(*g);
        Color theColor = {64, 64, 64, 255};
        aPlantG.SetColor(theColor);
        aPlantG.SetColorizeImages(true);
        aPlantG.ClipRect(x, y, g->mScaleX * 50.0f, coolDownHeight * g->mScaleY);
        if (isSeedPacketSelected) {
            if (Challenge::IsMPSeedType(seedType)) {
                TodDrawImageScaledF(&aPlantG, *Sexy_IMAGE_ZOMBIE_SEEDPACKET_Addr, x, y, g->mScaleX, g->mScaleY);
            } else {
                TodDrawImageCelScaledF(&aPlantG, *Sexy_IMAGE_SEEDS_Addr, x, y, celToDraw, 0, g->mScaleX, g->mScaleY);
            }
        }
        if (isPlant && isSeedPacketSelected)
            DrawSeedType(&aPlantG, x, y, seedType, imitaterType, v28, v29, theDrawScale);
    }
    if (drawCostText) {
        int tmpHolder[1];
        Board *board = lawnApp->mBoard;
        if (board != nullptr && board->PlantUsesAcceleratedPricing(realSeedType)) {
            if (isInGame) {
                int CurrentPlantCost = board->GetCurrentPlantCost(seedType, imitaterType);
                Sexy_StrFormat(tmpHolder, "%d", CurrentPlantCost);
            } else {
                int Cost = Plant::GetCost(seedType, imitaterType);
                Sexy_StrFormat(tmpHolder, "%d+", Cost);
            }
        } else {
            int Cost = Plant::GetCost(seedType, imitaterType);
            Sexy_StrFormat(tmpHolder, "%d", Cost);
        }
        Sexy::Font *font = *Sexy_FONT_BRIANNETOD12_Addr;
        int width = 31 - (*((int (**)(Sexy::Font *, int *))font->vTable + 8))(font, tmpHolder);
        ;                                                                      // 33  ----- >  31，微调一下文字位置，左移2个像素点
        int height = 48 + (*((int (**)(Sexy::Font *))font->vTable + 2))(font); // 50  ---- >  48, 微调一下文字位置，上移2个像素点
        Color theColor = {0, 0, 0, 255};
        g->PushState();;
        if (g->mScaleX == 1.0 && g->mScaleY == 1.0) {
            TodDrawString(g, tmpHolder, width + x, height + y, font, theColor, DrawStringJustification::DS_ALIGN_LEFT);
        } else {
            int matrix[25];
            Sexy_SexyMatrix3_SexyMatrix3(matrix);
            TodScaleTransformMatrix(matrix, x + g->mTransX + width * g->mScaleX, y + g->mTransY + height * g->mScaleY - 1.0, g->mScaleX, g->mScaleY);
            if (g->mScaleX > 1.8) {
                g->SetLinearBlend(false);
            }
            TodDrawStringMatrix(g, font, matrix, tmpHolder, &theColor);
            g->SetLinearBlend(true);
        }
        g->PopState();
        Sexy_String_Delete(tmpHolder);
    }
    g->SetColorizeImages(false);
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

Sexy::Image *FilterEffectCreateImage(Sexy::Image *image, FilterEffect theFilterEffect) {
    Sexy::Image *memoryImage = Sexy_SexyAppBase_CopyImage((LawnApp *)*gLawnApp_Addr, image);
    memoryImage->mWidth = image->mWidth;
    memoryImage->mHeight = image->mHeight;
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
    memoryImage->mNumCols = image->mNumCols;
    memoryImage->mNumRows = image->mNumRows;
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

bool Sexy_SexyAppBase_Is3DAccelerated(LawnApp *lawnApp) {
    // 修复关闭3D加速后MV错位
    return lawnApp->mNewIs3DAccelerated || lawnApp->mCreditScreen != nullptr;
}

void Sexy_SexyAppBase_Set3DAccelerated(LawnApp *lawnApp, bool isAccelerated) {
    lawnApp->mNewIs3DAccelerated = isAccelerated;
    lawnApp->mPlayerInfo->mIs3DAcceleratedClosed = !isAccelerated;
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
