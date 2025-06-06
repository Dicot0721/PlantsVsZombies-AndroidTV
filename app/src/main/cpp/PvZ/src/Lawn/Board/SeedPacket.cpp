#include "PvZ/Lawn/Board/SeedPacket.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Challenge.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Board/SeedBank.h"
#include "PvZ/Lawn/GamepadControls.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/MagicAddr.h"
#include "PvZ/Misc.h"
#include "PvZ/SexyAppFramework/Graphics/Font.h"
#include "PvZ/SexyAppFramework/Misc/SexyMatrix.h"
#include "PvZ/Symbols.h"

using namespace Sexy;

void SeedPacket::Update() {
    if (mRefreshing && seedPacketFastCoolDown) {
        // 正在刷新的种子立即冷却完毕
        mActive = true;
        mRefreshing = false;
    }

    if (requestPause) {

        if (mApp->IsIZombieLevel()) {
            // 在IZ模式不暂停刷新种子卡片
            return old_SeedPacket_Update(this);
        }

        if (mApp->mGameScene == GameScenes::SCENE_PLAYING && mPacketType != SeedType::SEED_NONE) {
            mRefreshCounter--;
        }
        if (mSlotMachineCountDown > 0) {
            mSlotMachineCountDown++;
        }
    }

    return old_SeedPacket_Update(this);
}

void SeedPacket::UpdateSelected() {
    if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS || mApp->IsCoopMode()) {
        // 如果是双人模式关卡(对战或结盟)，则使用下面的逻辑来更新当前选中的卡片。用于修复1P和2P的卡片选择框同时出现在两个人各自的植物栏里(也就是植物栏一共出现四个选择框)的问题。
        int mSelectedSeedIndex1P = mBoard->mGamepadControls1->mSelectedSeedIndex;
        int mSelectedSeedIndex2P = mBoard->mGamepadControls2->mSelectedSeedIndex;
        bool playerIndex = GetPlayerIndex();
        bool selectedBy1P = playerIndex == 0 && mIndex == mSelectedSeedIndex1P;
        bool selectedBy2P = playerIndex == 1 && mIndex == mSelectedSeedIndex2P;
        mSelectedBy1P = selectedBy1P || selectedBy2P;
        mSelectedBy2P = selectedBy2P;
        return;
    }
    return old_SeedPacket_UpdateSelected(this);
}

void SeedPacket::DrawOverlay(Sexy::Graphics *g) {
    // 绘制卡片冷却进度倒计时
    old_SeedPacket_DrawOverlay(this, g);

    if (mRefreshing && showCoolDown) {
        // 如果玩家开启了“显示冷却倒计时”，则绘制倒计时
        int holder[1];
        int coolDownRemaining = mRefreshTime - mRefreshCounter;
        Sexy_StrFormat(holder, "%1.1f", coolDownRemaining / 100.0f);
        g->SetColor(GetPlayerIndex() ? yellow : blue);
        g->SetFont(*Sexy_FONT_DWARVENTODCRAFT18_Addr);
        g->DrawString((SexyString&)holder, coolDownRemaining < 1000 ? 10 : 0, 39);
        Sexy_String_Delete(holder);
        g->SetFont(nullptr);
    }
}

void SeedPacket::Draw(Sexy::Graphics *g) {
    // 绘制卡片冷却进度倒计时
    old_SeedPacket_Draw(this, g);
}

void SeedPacket_MouseDown(SeedPacket *seedPacket, int x, int y, int c, int unk) {
    //    LOGD("SeedPacket_MouseDown:%d %d %d %d", x, y, c, unk);
    old_SeedPacket_MouseDown(seedPacket, x, y, c, unk);
}

bool SeedPacket::BeginDraw(Sexy::Graphics *g) {
    return old_SeedPacket_BeginDraw(this, g);
}

void SeedPacket::EndDraw(Sexy::Graphics *g) {
    old_SeedPacket_EndDraw(this, g);
}

void SeedPacket::FlashIfReady() {
    // 去除对战模式下的闪光效果的缩放

    if (!CanPickUp()) {
        return;
    }
    if (mApp->mEasyPlantingCheat) {
        return;
    }

    int playerIndex = 0;
    if (mSeedBank != nullptr) {
        playerIndex = mSeedBank->mIsZombie;
    }
    if (!mBoard->HasConveyorBeltSeedBank(playerIndex) && mSeedBank != nullptr) {
        mApp->AddTodParticle(mSeedBank->mX + mX, mSeedBank->mY + mY, 100000 + 2, ParticleEffect::PARTICLE_SEED_PACKET_FLASH);
    }
    TutorialState tutorialState = mBoard->mTutorialState;
    if (tutorialState == TutorialState::TUTORIAL_LEVEL_1_REFRESH_PEASHOOTER) {
        mBoard->SetTutorialState(TutorialState::TUTORIAL_LEVEL_1_PICK_UP_PEASHOOTER);
        return;
    }
    if (tutorialState == TutorialState::TUTORIAL_LEVEL_2_REFRESH_SUNFLOWER && mPacketType == SeedType::SEED_SUNFLOWER) {
        mBoard->SetTutorialState(TutorialState::TUTORIAL_LEVEL_2_PICK_UP_SUNFLOWER);
        return;
    }
    if (tutorialState == TutorialState::TUTORIAL_MORESUN_REFRESH_SUNFLOWER && mPacketType == SeedType::SEED_SUNFLOWER) {
        mBoard->SetTutorialState(TutorialState::TUTORIAL_MORESUN_PICK_UP_SUNFLOWER);
        return;
    }
}

void DrawSeedPacket(Sexy::Graphics *g,
                    float x,
                    float y,
                    SeedType theSeedType,
                    SeedType theImitaterType,
                    float thePercentDark,
                    int theGrayness,
                    bool theDrawCost,
                    bool theUseCurrentCost,
                    bool isZombieSeed,
                    bool isSeedPacketSelected) {
    // 修复选中紫卡、模仿者卡时卡片背景变为普通卡片背景

    SeedType realSeedType = theImitaterType != SeedType::SEED_NONE && theSeedType == SeedType::SEED_IMITATER ? theImitaterType : theSeedType;
    if (theGrayness != 255) {
        Color theColor = {theGrayness, theGrayness, theGrayness, 255};
        g->SetColor(theColor);
        g->SetColorizeImages(true);
    } else if (thePercentDark > 0.0f) {
        Color theColor = {128, 128, 128, 255};
        g->SetColor(theColor);
        g->SetColorizeImages(true);
    }
    int celToDraw;
    if (theSeedType == SeedType::SEED_IMITATER) {
        celToDraw = 0;
    } else if (Plant::IsUpgrade(realSeedType)) {
        celToDraw = 1;
    } else if (theSeedType == SeedType::SEED_BEGHOULED_BUTTON_CRATER) {
        celToDraw = 3;
    } else if (theSeedType == SeedType::SEED_BEGHOULED_BUTTON_SHUFFLE) {
        celToDraw = 4;
    } else if (theSeedType == SeedType::SEED_SLOT_MACHINE_SUN) {
        celToDraw = 5;
    } else if (theSeedType == SeedType::SEED_SLOT_MACHINE_DIAMOND) {
        celToDraw = 6;
    } else if (theSeedType == SeedType::SEED_ZOMBIQUARIUM_SNORKLE) {
        celToDraw = 7;
    } else if (theSeedType == SeedType::SEED_ZOMBIQUARIUM_TROPHY) {
        celToDraw = 8;
    } else {
        celToDraw = 2;
    }

    if (isSeedPacketSelected) {
        if (g->mScaleX > 1.0f && theSeedType <= SeedType::SEED_LEFTPEATER) {
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
    bool isPlant = theSeedType < SeedType::SEED_BEGHOULED_BUTTON_SHUFFLE || theSeedType > SeedType::SEED_ZOMBIQUARIUM_TROPHY;
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
        DrawSeedType(g, x, y, realSeedType, theImitaterType, v28, v29, theDrawScale);

    if (thePercentDark > 0.0) {
        float coolDownHeight = thePercentDark * 68.0 + 2.5;
        Graphics aPlantG(*g);
        Color theColor = {64, 64, 64, 255};
        aPlantG.SetColor(theColor);
        aPlantG.SetColorizeImages(true);
        aPlantG.ClipRect(x, y, g->mScaleX * 50.0f, coolDownHeight * g->mScaleY);
        if (isSeedPacketSelected) {
            if (Challenge::IsMPSeedType(theSeedType)) {
                TodDrawImageScaledF(&aPlantG, *Sexy_IMAGE_ZOMBIE_SEEDPACKET_Addr, x, y, g->mScaleX, g->mScaleY);
            } else {
                TodDrawImageCelScaledF(&aPlantG, *Sexy_IMAGE_SEEDS_Addr, x, y, celToDraw, 0, g->mScaleX, g->mScaleY);
            }
        }
        if (isPlant && isSeedPacketSelected)
            DrawSeedType(&aPlantG, x, y, theSeedType, theImitaterType, v28, v29, theDrawScale);
    }
    if (theDrawCost) {
        int tmpHolder[1];
        Board *board = lawnApp->mBoard;
        if (board != nullptr && board->PlantUsesAcceleratedPricing(realSeedType)) {
            if (theUseCurrentCost) {
                int CurrentPlantCost = board->GetCurrentPlantCost(theSeedType, theImitaterType);
                Sexy_StrFormat(tmpHolder, "%d", CurrentPlantCost);
            } else {
                int Cost = Plant::GetCost(theSeedType, theImitaterType);
                Sexy_StrFormat(tmpHolder, "%d+", Cost);
            }
        } else {
            int Cost = Plant::GetCost(theSeedType, theImitaterType);
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
            SexyMatrix3 aMatrix;
            TodScaleTransformMatrix(aMatrix, x + g->mTransX + width * g->mScaleX, y + g->mTransY + height * g->mScaleY - 1.0, g->mScaleX, g->mScaleY);
            if (g->mScaleX > 1.8) {
                g->SetLinearBlend(false);
            }
            TodDrawStringMatrix(g, font, aMatrix, tmpHolder, &theColor);
            g->SetLinearBlend(true);
        }
        g->PopState();
        Sexy_String_Delete(tmpHolder);
    }
    g->SetColorizeImages(false);
}
