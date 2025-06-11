#include "PvZ/Lawn/Board/Board.h"
#include "Homura/Formation.h"
#include "Homura/Logger.h"
#include "PvZ/Android/IntroVideo.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Challenge.h"
#include "PvZ/Lawn/Board/Coin.h"
#include "PvZ/Lawn/Board/CutScene.h"
#include "PvZ/Lawn/Board/GridItem.h"
#include "PvZ/Lawn/Board/Plant.h"
#include "PvZ/Lawn/Board/Projectile.h"
#include "PvZ/Lawn/Board/SeedBank.h"
#include "PvZ/Lawn/Board/SeedPacket.h"
#include "PvZ/Lawn/Board/ZenGarden.h"
#include "PvZ/Lawn/Board/Zombie.h"
#include "PvZ/Lawn/Common/LawnCommon.h"
#include "PvZ/Lawn/Common/Resources.h"
#include "PvZ/Lawn/GamepadControls.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/System/Music.h"
#include "PvZ/Lawn/Widget/ChallengeScreen.h"
#include "PvZ/Lawn/Widget/SeedChooserScreen.h"
#include "PvZ/Misc.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/SexyAppFramework/Widget/GameButton.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Effect/Reanimator.h"
#include "PvZ/Lawn/Board/CursorObject.h"
#include "PvZ/TodLib/Effect/TodParticle.h"

#include <cstddef>
#include <cstdio>

using namespace Sexy;
using namespace std;

Board::Board(LawnApp *theApp) {
    Create(theApp);
}

void Board::Create(LawnApp *theApp) {
    old_Board_Board(this, theApp);

    if (gBoardMenuButton != nullptr) {
        gBoardMenuButton->Destroy();
        operator delete (gBoardMenuButton);
    }
    if (gBoardStoreButton != nullptr) {
        gBoardStoreButton->Destroy();
        operator delete (gBoardStoreButton);
    }
    int holder[1];
    TodStringTranslate(holder, (theApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN || theApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM) ? "[MAIN_MENU_BUTTON]" : "[MENU_BUTTON]");
    gBoardMenuButton = MakeButton(1000, &mButtonListener, this, (SexyString &)holder);
    gBoardMenuButton->Resize(705, -3, 120, 80);
    gBoardMenuButton->mBtnNoDraw = true;
    gBoardMenuButton->mDisabled = true;
    if (theApp->IsCoopMode() || theApp->mGameMode == GameMode::GAMEMODE_MP_VS) {
        gBoardMenuButton->Resize(880, -3, 120, 80);
    } else if (theApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN || theApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM) {
        gBoardMenuButton->Resize(650, 550, 170, 120);
    }
    StringDelete(holder);

    if (theApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_LAST_STAND) {
        int holder1[1];
        TodStringTranslate(holder1, "[START_ONSLAUGHT]");
        gBoardStoreButton = MakeButton(1001, &mButtonListener, this, (SexyString &)holder1);
        gBoardStoreButton->Resize(0, 0, 0, 0);
        gBoardStoreButton->mBtnNoDraw = true;
        gBoardStoreButton->mDisabled = true;
        StringDelete(holder1);
    } else {
        int holder1[1];
        TodStringTranslate(holder1, "[SHOP_BUTTON]");
        gBoardStoreButton = MakeButton(1001, &mButtonListener, this, (SexyString &)holder1);
        StringDelete(holder1);
        if (theApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN || theApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM) {
            gBoardStoreButton->Resize(0, 550, 170, 120);
        } else {
            gBoardStoreButton->Resize(0, 0, 0, 0);
            gBoardStoreButton->mBtnNoDraw = true;
            gBoardStoreButton->mDisabled = true;
        }
    }
    AddWidget(gBoardMenuButton);
    AddWidget(gBoardStoreButton);
    mAdvice->Delete();
    CustomMessageWidget *aAdvice = new CustomMessageWidget(mApp);
    mAdvice = aAdvice;
}

void Board::InitLevel() {
    old_Board_InitLevel(this);
    mNewWallNutAndSunFlowerAndChomperOnly =
        !(mApp->IsScaryPotterLevel() || mApp->IsIZombieLevel() || mApp->IsWhackAZombieLevel() || HasConveyorBeltSeedBank(0) || mApp->IsChallengeWithoutSeedBank());
    mNewPeaShooterCount = 0;
}

void Board_SetGrids(Board *board) {
    // 更换场地时需要，用于初始化每一个格子的类型。
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 6; j++) {
            if (board->mPlantRow[j] == PlantRowType::PLANTROW_DIRT) {
                board->mGridSquareType[i][j] = GridSquareType::GRIDSQUARE_DIRT;
            } else if (board->mPlantRow[j] == PlantRowType::PLANTROW_POOL && i >= 0 && i <= 8) {
                board->mGridSquareType[i][j] = GridSquareType::GRIDSQUARE_POOL;
            } else if (board->mPlantRow[j] == PlantRowType::PLANTROW_HIGH_GROUND && i >= 4 && i <= 8) {
                board->mGridSquareType[i][j] = GridSquareType::GRIDSQUARE_HIGH_GROUND;
            }
        }
    }
}

int LawnSaveGame(Board *board, int *a2) {
    //    Zombie *zombie = nullptr;
    //    while (IterateZombies(this, &zombie)) {
    //        if (zombie->mZombieType == ZombieType::ZOMBIE_FLAG) {
    //            LawnApp_RemoveReanimation(zombie->mApp, zombie->mBossFireBallReanimID);
    //            zombie->mBossFireBallReanimID = 0;
    //        }
    //    }
    return old_LawnSaveGame(board, a2);
}

void Board::ShovelDown() {
    // 用于铲掉光标正下方的植物。
    requestDrawShovelInCursor = false;
    bool isInShovelTutorial = (unsigned int)(mTutorialState - 15) <= 2;
    if (isInShovelTutorial) {
        // 如果正在铲子教学中(即冒险1-5的保龄球的开场前，戴夫要求你铲掉三个豌豆的这段时间),则发送铲除键来铲除。
        mGamepadControls1->OnKeyDown(KeyCode::KEYCODE_SHOVEL, 1112);
        ClearCursor(0);
        RefreshSeedPacketFromCursor(0);
        return;
    }
    // 下方就是自己写的铲除逻辑喽。
    float aXPos = mGamepadControls1->mCursorPositionX;
    float aYPos = mGamepadControls1->mCursorPositionY;
    Plant *aPlantUnderShovel = ToolHitTest(aXPos, aYPos);
    if (aPlantUnderShovel != nullptr) {
        mApp->PlayFoley(FoleyType::FOLEY_USE_SHOVEL); // 播放铲除音效
        aPlantUnderShovel->Die();                     // 让被铲的植物趋势
        SeedType aSeedType = aPlantUnderShovel->mSeedType;
        int aRow = aPlantUnderShovel->mRow;
        if (aSeedType == SeedType::SEED_CATTAIL && GetTopPlantAt(aPlantUnderShovel->mPlantCol, aRow, PlantPriority::TOPPLANT_ONLY_PUMPKIN) != nullptr) {
            // 如果铲的是南瓜套内的猫尾草,则再在原地种植一个荷叶
            NewPlant(aPlantUnderShovel->mPlantCol, aRow, SeedType::SEED_LILYPAD, SeedType::SEED_NONE, -1);
        }
        if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_LAST_STAND) {
            if (mChallenge->mChallengeState == ChallengeState::STATECHALLENGE_NORMAL && mApp->mGameScene == GameScenes::SCENE_PLAYING) {
                int aCost = Plant::GetCost(aSeedType, aPlantUnderShovel->mImitaterType);
                int num = aCost / 25;
                if (aSeedType == SeedType::SEED_GARLIC || aSeedType == SeedType::SEED_WALLNUT || aSeedType == SeedType::SEED_TALLNUT || aSeedType == SeedType::SEED_PUMPKINSHELL) {
                    int mPlantHealth = aPlantUnderShovel->mPlantHealth;
                    int mPlantMaxHealth = aPlantUnderShovel->mPlantMaxHealth;
                    num = (mPlantHealth * 3 > mPlantMaxHealth * 2) ? num : 0;
                }
                for (int i = 0; i < num; i++) {
                    Coin *aCoin = AddCoin(aXPos, aYPos, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
                    aCoin->Collect(0);
                }
            }
        }
    }

    ClearCursor(0);
    RefreshSeedPacketFromCursor(0);
}

void Board::UpdateGame() {
    if (requestPause) {
        UpdateGameObjects();
        return;
    }

    old_Board_UpdateGame(this);

    // 防止选卡界面浓雾遮挡僵尸
    if (this->mFogBlownCountDown > 0 && this->mApp->mGameScene == GameScenes::SCENE_LEVEL_INTRO) {
        float thePositionStart = 1065.0 - 4 * 80.0 + 100; // 1065f - LeftFogColumn() * 80f + Constants.BOARD_EXTRA_ROOM;
        this->mFogOffset = TodAnimateCurveFloat(200, 0, this->mFogBlownCountDown, thePositionStart, 0.0, TodCurves::CURVE_EASE_OUT);
    }
}

void Board::UpdateGameObjects() {
    // 修复过关后游戏卡住不动
    if (mBoardFadeOutCounter > 0) {
        // 如果已经过关，则手动刷新植物，僵尸，子弹
        Plant *aPlant = nullptr;
        while (IteratePlants(aPlant)) {
            aPlant->Update();
        }
        Zombie *aZombie = nullptr;
        while (IterateZombies(aZombie)) {
            aZombie->Update();
        }
        Projectile *aProjectile = nullptr;
        while (IterateProjectiles(aProjectile)) {
            aProjectile->Update();
        }
    }

    old_Board_UpdateGameObjects(this);
}

void Board::DrawDebugText(Sexy::Graphics *g) {
    // 出僵DEBUG功能
    if (drawDebugText) {
        DebugTextMode tmp = mDebugTextMode;
        mDebugTextMode = DebugTextMode::DEBUG_TEXT_ZOMBIE_SPAWN;
        old_Board_DrawDebugText(this, g);
        mDebugTextMode = tmp;
        return;
    }

    old_Board_DrawDebugText(this, g);
}

void Board::DrawDebugObjectRects(Sexy::Graphics *g) {
    // 碰撞体积绘制
    if (drawDebugRects) {
        DebugTextMode tmp = mDebugTextMode;
        mDebugTextMode = DebugTextMode::DEBUG_TEXT_COLLISION;
        old_Board_DrawDebugObjectRects(this, g);
        mDebugTextMode = tmp;
        return;
    }

    old_Board_DrawDebugObjectRects(this, g);
}

void Board::DrawFadeOut(Sexy::Graphics *g) {
    // 修复关卡完成后的白色遮罩无法遮住整个屏幕
    if (mBoardFadeOutCounter < 0) {
        return;
    }

    if (IsSurvivalStageWithRepick()) {
        return;
    }

    int theAlpha = TodAnimateCurve(200, 0, mBoardFadeOutCounter, 0, 255, TodCurves::CURVE_LINEAR);
    if (mLevel == 9 || mLevel == 19 || mLevel == 29 || mLevel == 39 || mLevel == 49) {
        Color theColor = {0, 0, 0, theAlpha};
        g->SetColor(theColor);
    } else {
        Color theColor = {255, 255, 255, theAlpha};
        g->SetColor(theColor);
    }

    g->SetColorizeImages(true);
    Rect fullScreenRect = {-240, -60, 1280, 720};
    // 修复BUG的核心原理，就是不要在此处PushState和PopState，而是直接FillRect。这将保留graphics的trans属性。
    g->FillRect(fullScreenRect);
}

int Board::GetCurrentPlantCost(SeedType theSeedType, SeedType theImitaterType) {
    // 无限阳光
    if (infiniteSun)
        return 0;

    return old_Board_GetCurrentPlantCost(this, theSeedType, theImitaterType);
}

void Board::AddSunMoney(int theAmount, int thePlayerIndex) {
    // 无限阳光
    if (infiniteSun) {
        if (thePlayerIndex == 0) {
            mSunMoney1 = 9990;
        } else {
            mSunMoney2 = 9990;
        }
    } else {
        old_Board_AddSunMoney(this, theAmount, thePlayerIndex);
    }
}

void Board::AddDeathMoney(int theAmount) {
    // 无限阳光
    if (infiniteSun) {
        mDeathMoney = 9990;
    } else {
        old_Board_AddDeathMoney(this, theAmount);
    }
}

PlantingReason Board::CanPlantAt(int theGridX, int theGridY, SeedType theSeedType) {
    // 自由种植！
    if (FreePlantAt) {
        return PlantingReason::PLANTING_OK;
    }
    return old_Board_CanPlantAt(this, theGridX, theGridY, theSeedType);
}


bool Board::PlantingRequirementsMet(SeedType theSeedType) {
    // 紫卡直接种植！
    if (FreePlantAt) {
        return true;
    }
    return old_Board_PlantingRequirementsMet(this, theSeedType);
}

void Board::ZombiesWon(Zombie *theZombie) {
    if (theZombie == nullptr) { // 如果是IZ或者僵尸水族馆，第二个参数是NULL，此时就返回原函数。否则闪退
        return old_BoardZombiesWon(this, theZombie);
    }
    if (ZombieCanNotWon) {
        theZombie->ApplyBurn();
        theZombie->Zombie::DieNoLoot();
        return;
    }
    return old_BoardZombiesWon(this, theZombie);
}

int Board::CountPlantByType(SeedType theSeedType)
{
    int aCount = 0;
    Plant* aPlant = nullptr;
    while (IteratePlants(aPlant))
    {
        if (aPlant->mSeedType == theSeedType)
        {
            aCount++;
        }
    }
    return aCount;
}

Plant *Board::AddPlant(int theGridX, int theGridY, SeedType theSeedType, SeedType theImitaterType, int thePlayerIndex, bool theIsDoEffect) {

    Plant *aPlant = NewPlant(theGridX, theGridY, theSeedType, theImitaterType, thePlayerIndex);
    if (theIsDoEffect) {
        DoPlantingEffects(theGridX, theGridY, aPlant);
    }
    mChallenge->PlantAdded(aPlant);

    // 检查成就！
    DoPlantingAchievementCheck(theSeedType);
    int aSunPlantsCount = CountPlantByType(SeedType::SEED_SUNFLOWER) + CountPlantByType(SeedType::SEED_SUNSHROOM);
    if (aSunPlantsCount > mMaxSunPlants) {
        mMaxSunPlants = aSunPlantsCount;
    }
    if (theSeedType == SeedType::SEED_CABBAGEPULT || theSeedType == SeedType::SEED_KERNELPULT || theSeedType == SeedType::SEED_MELONPULT || theSeedType == SeedType::SEED_WINTERMELON) {
        mCatapultPlantsUsed = true;
    }
    if (theSeedType == SeedType::SEED_PUMPKINSHELL && PumpkinWithLadder && GetLadderAt(theGridX, theGridY) == nullptr) {
        AddALadder(theGridX, theGridY);
    }

    return aPlant;
}

// 布阵用
void Board::ParseFormationSegment(char *theSegment) {
    SeedType seedType = SeedType::SEED_PEASHOOTER;
    bool isIZombieLevel = mApp->IsIZombieLevel();
    bool wakeUp = false;
    bool imitaterMorphed = false;
    bool addLadder = false;
    int damageState = 0;
    int offset = 0;

    if (sscanf(theSegment, "%d%n", &seedType, &offset) != 1) {
        return; // Failed to parse
    }

    // Move cursor to the next position after the parsed integer
    const char *cursor = theSegment + offset;
    while (*cursor != '\0') {
        if (*cursor == 'W') {
            wakeUp = true;
        } else if (*cursor == 'I') {
            imitaterMorphed = true;
        } else if (*cursor == 'L') {
            addLadder = true;
        } else if (*cursor == 'D') {
            damageState++;
            if (damageState > 2) {
                damageState = 2;
            }
        } else if (*cursor >= '0' && *cursor <= '9') {
            // Parse coordinates
            int x = 0, y = 0;
            if (!sscanf(cursor, "%d,%d", &x, &y)) {
                continue;
            }
            Plant *plant = old_Board_AddPlant(this, x, y, seedType, imitaterMorphed ? SeedType::SEED_IMITATER : SeedType::SEED_NONE, 1, false);
            if (imitaterMorphed) {
                plant->SetImitaterFilterEffect();
            }
            if (wakeUp) {
                plant->SetSleeping(false);
            }
            if (addLadder && GetLadderAt(x, y) == nullptr) {
                AddALadder(x, y);
            }
            if (damageState > 0) {
                plant->mPlantHealth = (plant->mPlantMaxHealth * (3 - damageState) / 3) - 1;
            }
            if (isIZombieLevel) {
                mChallenge->IZombieSetupPlant(plant);
            }
            // Skip to next coordinate
            while (*cursor != ' ' && *cursor != '\0') {
                cursor++;
            }
            continue;
        }
        cursor++;
    }
}

// 布阵用
void Board::LoadFormation(char *theFormation) {
    RemoveAllPlants();
    const char *segmentDelimiters = ";";
    char *aSegment = strtok(theFormation, segmentDelimiters);
    while (aSegment != nullptr) {
        ParseFormationSegment(aSegment);
        aSegment = strtok(nullptr, segmentDelimiters);
    }
}


bool Board_ZenGardenItemNumIsZero(Board *board, CursorType cursorType) {
    // 消耗性工具的数量是否为0个
    LawnApp *lawnApp = board->mApp;
    switch (cursorType) {
        case CursorType::CURSOR_TYPE_FERTILIZER:
            return lawnApp->mPlayerInfo->mPurchases[StoreItem::STORE_ITEM_FERTILIZER] <= 1000;
        case CursorType::CURSOR_TYPE_BUG_SPRAY:
            return lawnApp->mPlayerInfo->mPurchases[StoreItem::STORE_ITEM_BUG_SPRAY] <= 1000;
        case CursorType::CURSOR_TYPE_CHOCOLATE:
            return lawnApp->mPlayerInfo->mPurchases[StoreItem::STORE_ITEM_CHOCOLATE] <= 1000;
        case CursorType::CURSOR_TYPE_TREE_FOOD:
            return lawnApp->mPlayerInfo->mPurchases[StoreItem::STORE_ITEM_TREE_FOOD] <= 1000;
    }
    return false;
}

void Board::DrawZenButtons(Sexy::Graphics *g) {
    return old_Board_DrawZenButtons(this, g);
}

void Board::KeyDown(KeyCode theKey) {
    // 用于切换键盘模式，自动开关砸罐子老虎机种子雨关卡内的"自动拾取植物卡片"功能
    if (theKey >= 37 && theKey <= 40) {
        if (!keyboardMode) {
            game_patches::autoPickupSeedPacketDisable.Restore();
        }
        keyboardMode = true;
        requestDrawShovelInCursor = false;
    }

    return old_Board_KeyDown(this, theKey);
}

void Board::UpdateSunSpawning() {
    if (requestPause) {
        // 如果开了高级暂停
        return;
    }
    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BUTTERED_POPCORN) {
        return;
    }

    return old_Board_UpdateSunSpawning(this);
}

void Board::UpdateZombieSpawning() {
    if (requestPause) {
        // 如果开了高级暂停
        return;
    }

    // 在黄油爆米花关卡改变出怪倒计时。
    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BUTTERED_POPCORN) {
        int mZombieCountDown = mZombieCountDown;
        if (mZombieCountDown >= 2500 && mZombieCountDown <= 3100) {
            mZombieCountDown = 750;
            mZombieCountDownStart = mZombieCountDown;
        }
    }
    //    int *lawnApp = (int *) this[69];
    //    GameMode::GameMode mGameMode = (GameMode::GameMode)*(lawnApp + LAWNAPP_GAMEMODE_OFFSET);
    //    if(mGameMode == GameMode::GAMEMODE_CHALLENGE_BUTTERED_POPCORN){
    //        int mFinalWaveSoundCounter = this[5660];
    //        if (mFinalWaveSoundCounter > 0) {
    //            mFinalWaveSoundCounter--;
    //            this[5660] = mFinalWaveSoundCounter;
    //            if (mFinalWaveSoundCounter == 0) {
    //                LawnApp_PlaySample(lawnApp,Sexy_SOUND_FINALWAVE_Addr);
    //            }
    //        }
    //        if (Board_HasLevelAwardDropped(this)) {
    //            return;
    //        }
    //
    //        int mRiseFromGraveCounter = this[5540];
    //        if (mRiseFromGraveCounter > 0) {
    //            mRiseFromGraveCounter--;
    //            this[5540] = mRiseFromGraveCounter;
    //            if (mRiseFromGraveCounter == 0) {
    //                Board_SpawnZombiesFromGraves(this);
    //            }
    //        }
    //
    //        int mHugeWaveCountDown = this[5552];
    //        if (mHugeWaveCountDown > 0) {
    //            mHugeWaveCountDown--;
    //            this[5552] = mHugeWaveCountDown;
    //            if (mHugeWaveCountDown == 0) {
    //                Board_ClearAdvice(this, 42);
    //                Board_NextWaveComing(this);
    //                this[5550] = 1; //  mZombieCountDown = 1;
    //            }else if(mHugeWaveCountDown == 725){
    //                LawnApp_PlaySample(lawnApp,Sexy_SOUND_FINALWAVE_Addr);
    //            }
    //        }
    //
    //        int mZombieCountDown = this[5550];
    //        mZombieCountDown--; //  mZombieCountDown--;
    //        this[5550] = mZombieCountDown;
    //
    //        int mZombieCountDownStart = this[5551];
    //        int mCurrentWave = this[5542];
    //        int mZombieHealthToNextWave = this[5548];
    //        int num2 = mZombieCountDownStart - mZombieCountDown;
    //        if (mZombieCountDown > 5 && num2 > 400) {
    //            int num3 = Board_TotalZombiesHealthInWave(this, mCurrentWave - 1);
    //            if (num3 <= mZombieHealthToNextWave && mZombieCountDown > 200) {
    //                this[5550] = 200;//  mZombieCountDown = 200;
    //            }
    //        }
    //
    //        if (mZombieCountDown == 5) {
    //            if (IsFlagWave(this, mCurrentWave)) {
    //                Board_ClearAdviceImmediately(this);
    //                int holder[1];
    //                StrFormat(holder,"[ADVICE_HUGE_WAVE]");
    //                Board_DisplayAdviceAgain(this, holder, 15, 42);
    //                StringDelete(holder);
    //                mHugeWaveCountDown = 750;
    //                return;
    //            }
    //            Board_NextWaveComing(this);
    //        }
    //
    //        if (mZombieCountDown != 0) {
    //            return;
    //        }
    //        Board_SpawnZombieWave(this);
    //        this[5549] = Board_TotalZombiesHealthInWave(this, mCurrentWave -1);
    //        //mZombieHealthWaveStart = Board_TotalZombiesHealthInWave(this,mCurrentWave - 1);
    //        if (IsFlagWave(this, mCurrentWave)) {
    //            this[5548] = 0;//  mZombieHealthToNextWave = 0;
    //            this[5550] = 0;//  mZombieCountDown = 4500;
    //        } else {
    //            this[5548] = (int) (RandRangeFloat(0.5f, 0.65f) * this[5549]);
    //            this[5550] = 750;//   mZombieCountDown = 750;
    //        }
    //        this[5551] = mZombieCountDown;
    //        return;
    //    }
    return old_Board_UpdateZombieSpawning(this);
}

void Board::UpdateIce() {
    if (requestPause) {
        // 如果开了高级暂停
        return;
    }

    return old_Board_UpdateIce(this);
}

void Board::DrawCoverLayer(Sexy::Graphics *g, int theRow) {
    if (mBackground < BackgroundType::BACKGROUND_1_DAY || hideCoverLayer) {
        // 如果背景非法，或玩家“隐藏草丛和电线杆”，则终止绘制函数
        return;
    }

    if (mBackground <= BackgroundType::BACKGROUND_4_FOG) {
        // 如果是前院(0 1)或者泳池(2 3)，则绘制草丛。整个草丛都是动画而非贴图，没有僵尸来的时候草丛会保持在动画第一帧。
        Reanimation *aReanim = mApp->ReanimationTryToGet(mCoverLayerAnimIDs[theRow]);
        if (aReanim != nullptr) {
            (aReanim)->Draw(g);
        }
    }
    if (theRow == 6) {
        // 绘制栏杆和电线杆
        switch (mBackground) {
            case BackgroundType::BACKGROUND_1_DAY: // 前院白天
                if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_HEAVY_WEAPON) {
                    // 在重型武器关卡中不绘制栏杆。
                    return;
                }
                g->DrawImage(*Sexy_IMAGE_BACKGROUND1_COVER_Addr, 684, 557);
                break;
            case BackgroundType::BACKGROUND_2_NIGHT: // 前院夜晚
                g->DrawImage(*Sexy_IMAGE_BACKGROUND2_COVER_Addr, 684, 557);
                break;
            case BackgroundType::BACKGROUND_3_POOL: // 泳池白天
                g->DrawImage(*Sexy_IMAGE_BACKGROUND3_COVER_Addr, 671, 613);
                break;
            case BackgroundType::BACKGROUND_4_FOG: // 泳池夜晚
                g->DrawImage(*Sexy_IMAGE_BACKGROUND4_COVER_Addr, 672, 613);
                break;
            case BackgroundType::BACKGROUND_5_ROOF: // 屋顶白天
                g->DrawImage(*Sexy_IMAGE_ROOF_TREE_Addr, mOffsetMoved * 1.5f + 628, -60);
                g->DrawImage(*Sexy_IMAGE_ROOF_POLE_Addr, mOffsetMoved * 2.0f + 628, -60);
                break;
            case BackgroundType::BACKGROUND_6_BOSS:
                // 可在此处添加代码绘制月夜电线杆喔
                //                if(LawnApp_IsFinalBossLevel(mApp))  return;

                g->DrawImage(addonImages.trees_night, mOffsetMoved * 1.5f + 628, -60);
                g->DrawImage(addonImages.pole_night, mOffsetMoved * 2.0f + 628, -60);
                break;
            default:
                return;
        }
    }
}

void Board::PickBackground() {
    // 用于控制关卡的场地选取。可选择以下场地：前院白天/夜晚，泳池白天/夜晚，屋顶白天/夜晚
    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BUTTERED_POPCORN) {
        mBackground = BackgroundType::BACKGROUND_3_POOL;
        LoadBackgroundImages();
        mPlantRow[0] = PlantRowType::PLANTROW_NORMAL;
        mPlantRow[1] = PlantRowType::PLANTROW_NORMAL;
        mPlantRow[2] = PlantRowType::PLANTROW_POOL;
        mPlantRow[3] = PlantRowType::PLANTROW_POOL;
        mPlantRow[4] = PlantRowType::PLANTROW_NORMAL;
        mPlantRow[5] = PlantRowType::PLANTROW_NORMAL;
        InitCoverLayer();
        Board_SetGrids(this);
    } else if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_POOL_PARTY) {
        mBackground = BackgroundType::BACKGROUND_3_POOL;
        LoadBackgroundImages();
        mPlantRow[0] = PlantRowType::PLANTROW_POOL;
        mPlantRow[1] = PlantRowType::PLANTROW_POOL;
        mPlantRow[2] = PlantRowType::PLANTROW_POOL;
        mPlantRow[3] = PlantRowType::PLANTROW_POOL;
        mPlantRow[4] = PlantRowType::PLANTROW_POOL;
        mPlantRow[5] = PlantRowType::PLANTROW_NORMAL;
        InitCoverLayer();
        Board_SetGrids(this);
        //    } else if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS) {

    } else {
        switch (VSBackGround) {
            case 1:
                mBackground = BackgroundType::BACKGROUND_1_DAY;
                LoadBackgroundImages();
                mPlantRow[0] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[1] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[2] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[3] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[4] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[5] = PlantRowType::PLANTROW_DIRT;
                InitCoverLayer();
                Board_SetGrids(this);
                break;
            case 2:
                mBackground = BackgroundType::BACKGROUND_2_NIGHT;
                LoadBackgroundImages();
                mPlantRow[0] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[1] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[2] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[3] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[4] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[5] = PlantRowType::PLANTROW_DIRT;
                InitCoverLayer();
                Board_SetGrids(this);
                break;
            case 3:
                mBackground = BackgroundType::BACKGROUND_3_POOL;
                LoadBackgroundImages();
                mPlantRow[0] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[1] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[2] = PlantRowType::PLANTROW_POOL;
                mPlantRow[3] = PlantRowType::PLANTROW_POOL;
                mPlantRow[4] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[5] = PlantRowType::PLANTROW_NORMAL;
                InitCoverLayer();
                Board_SetGrids(this);
                break;
            case 4:
                mBackground = BackgroundType::BACKGROUND_4_FOG;
                LoadBackgroundImages();
                mPlantRow[0] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[1] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[2] = PlantRowType::PLANTROW_POOL;
                mPlantRow[3] = PlantRowType::PLANTROW_POOL;
                mPlantRow[4] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[5] = PlantRowType::PLANTROW_NORMAL;
                InitCoverLayer();
                Board_SetGrids(this);
                break;
            case 5:
                mBackground = BackgroundType::BACKGROUND_5_ROOF;
                LoadBackgroundImages();
                mPlantRow[0] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[1] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[2] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[3] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[4] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[5] = PlantRowType::PLANTROW_DIRT;
                InitCoverLayer();
                Board_SetGrids(this);
                if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS) {
                    AddPlant(0, 1, SeedType::SEED_FLOWERPOT, SeedType::SEED_NONE, 1, 0);
                    AddPlant(0, 3, SeedType::SEED_FLOWERPOT, SeedType::SEED_NONE, 1, 0);
                    for (int i = 3; i < 5; ++i) {
                        for (int j = 0; j < 5; ++j) {
                            AddPlant(i, j, SeedType::SEED_FLOWERPOT, SeedType::SEED_NONE, 1, 0);
                        }
                    }
                }
                break;
            case 6:
                mBackground = BackgroundType::BACKGROUND_6_BOSS;
                LoadBackgroundImages();
                mPlantRow[0] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[1] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[2] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[3] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[4] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[5] = PlantRowType::PLANTROW_DIRT;
                InitCoverLayer();
                Board_SetGrids(this);
                if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS) {
                    AddPlant(0, 1, SeedType::SEED_FLOWERPOT, SeedType::SEED_NONE, 1, 0);
                    AddPlant(0, 3, SeedType::SEED_FLOWERPOT, SeedType::SEED_NONE, 1, 0);
                    for (int i = 3; i < 4; ++i) {
                        for (int j = 0; j < 5; ++j) {
                            AddPlant(i, j, SeedType::SEED_FLOWERPOT, SeedType::SEED_NONE, 1, 0);
                        }
                    }
                }
                break;
            case 7:
                mBackground = BackgroundType::BACKGROUND_GREENHOUSE;
                LoadBackgroundImages();
                mPlantRow[0] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[1] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[2] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[3] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[4] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[5] = PlantRowType::PLANTROW_DIRT;
                InitCoverLayer();
                Board_SetGrids(this);
                break;
            case 8:
                mBackground = BackgroundType::BACKGROUND_MUSHROOM_GARDEN;
                LoadBackgroundImages();
                mPlantRow[0] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[1] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[2] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[3] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[4] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[5] = PlantRowType::PLANTROW_DIRT;
                InitCoverLayer();
                Board_SetGrids(this);
                break;
            case 9:
                mBackground = BackgroundType::BACKGROUND_ZOMBIQUARIUM;
                LoadBackgroundImages();
                mPlantRow[0] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[1] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[2] = PlantRowType::PLANTROW_POOL;
                mPlantRow[3] = PlantRowType::PLANTROW_POOL;
                mPlantRow[4] = PlantRowType::PLANTROW_NORMAL;
                mPlantRow[5] = PlantRowType::PLANTROW_NORMAL;
                InitCoverLayer();
                Board_SetGrids(this);
                break;
            default:
                old_Board_PickBackground(this);
        };
    }
}

bool Board_StageIsNight(Board *board) {
    // 关系到天上阳光掉落与否。
    BackgroundType mBackground = board->mBackground;
    return mBackground == BackgroundType::BACKGROUND_2_NIGHT || mBackground == BackgroundType::BACKGROUND_4_FOG || mBackground == BackgroundType::BACKGROUND_MUSHROOM_GARDEN
        || mBackground == BackgroundType::BACKGROUND_6_BOSS;
}

bool Board::StageHasPool() {
    // 关系到泳池特有的僵尸，如救生圈僵尸、海豚僵尸、潜水僵尸在本关出现与否。此处我们添加水族馆场景。
    //    return mBackground == BackgroundType::Zombiquarium || old_Board_StageHasPool(this);
    return (mBackground == BackgroundType::BACKGROUND_ZOMBIQUARIUM && mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZOMBIQUARIUM) || mBackground == BackgroundType::BACKGROUND_3_POOL
        || mBackground == BackgroundType::BACKGROUND_4_FOG;
}

bool Board::StageHasRoof() {
    return (mBackground == BackgroundType::BACKGROUND_5_ROOF || mBackground == BackgroundType::BACKGROUND_6_BOSS);
}

bool Board_StageHasRoof(Board *board) {
    BackgroundType mBackground = board->mBackground;
    return mBackground == BackgroundType::BACKGROUND_5_ROOF || mBackground == BackgroundType::BACKGROUND_6_BOSS;
}

bool Board_StageHas6Rows(Board *board) {
    // 关系到第六路可否操控（比如种植植物）。
    BackgroundType mBackground = board->mBackground;
    return mBackground == BackgroundType::BACKGROUND_3_POOL || mBackground == BackgroundType::BACKGROUND_4_FOG;
}


void Board::UpdateFwoosh() {
    if (requestPause) {
        return;
    }

    old_Board_UpdateFwoosh(this);
}

void Board::UpdateFog() {
    if (requestPause) {
        return;
    }

    old_Board_UpdateFog(this);
}

void Board::DrawFog(Sexy::Graphics *g) {
    if (noFog) {
        return;
    }

    old_Board_DrawFog(this, g);
}

Zombie *Board::AddZombieInRow(ZombieType theZombieType, int theRow, int theFromWave, bool theIsRustle) {
    // 修复蹦极僵尸出现时草丛也会摇晃
    if (theZombieType == ZombieType::ZOMBIE_BUNGEE)
        theIsRustle = false;

    return old_Board_AddZombieInRow(this, theZombieType, theRow, theFromWave, theIsRustle);
}

Zombie *Board::AddZombie(ZombieType theZombieType, int theFromWave, bool theIsRustle) {
    return old_Board_AddZombie(this, theZombieType, theFromWave, theIsRustle);
}

// void (*old_Board_UpdateCoverLayer)(Board *this);
//
// void Board_UpdateCoverLayer(Board *this) {
//     if (requestPause) {
//         return;
//     }
//     old_Board_UpdateCoverLayer(this);
// }

void Board::SpeedUpUpdate() {
    UpdateGridItems();
    UpdateFwoosh();
    UpdateGame();
    UpdateFog();
    //    Board_UpdateCoverLayer(this);
    mChallenge->Update();
}

bool TRect_Contains(Rect *rect, int x, int y) {
    return rect->mX < x && rect->mY < y && rect->mX + rect->mWidth > x && rect->mY + rect->mHeight > y;
}

void Board::Update() {
    isMainMenu = false;

    if (requestDrawButterInCursor) {
        Zombie *aZombieUnderButter = ZombieHitTest(mGamepadControls2->mCursorPositionX, mGamepadControls2->mCursorPositionY, 1);
        if (aZombieUnderButter != nullptr) {
            aZombieUnderButter->AddButter();
        }
    }

    if (requestDrawShovelInCursor) {
        Plant *plantUnderShovel = ToolHitTest(mGamepadControls1->mCursorPositionX, mGamepadControls1->mCursorPositionY);
        if (plantUnderShovel != nullptr) {
            // 让这个植物高亮
            plantUnderShovel->mEatenFlashCountdown = 1000; // 1000是为了不和其他闪光效果冲突
        }
    }

    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BUTTERED_POPCORN && mApp->mGameScene == GameScenes::SCENE_PLAYING) {
        Zombie *aZombieUnderButter = ZombieHitTest(mGamepadControls1->mCursorPositionX, mGamepadControls1->mCursorPositionY, 1);
        if (aZombieUnderButter != nullptr) {
            aZombieUnderButter->AddButter();
        }
        if (mGamepadControls2->mPlayerIndex2 != -1) {
            Zombie *aZombieUnderButter = ZombieHitTest(mGamepadControls2->mCursorPositionX, mGamepadControls2->mCursorPositionY, 1);
            if (aZombieUnderButter != nullptr) {
                aZombieUnderButter->AddButter();
            }
        }
    }
    //    GameButton_Update(gBoardMenuButton);
    if (isKeyboardTwoPlayerMode) {
        mGamepadControls1->mIsInShopSeedBank = false;
        mGamepadControls2->mIsInShopSeedBank = false;
        mGamepadControls1->mPlayerIndex2 = 0;
        mGamepadControls2->mPlayerIndex2 = 1;
        mGamepadControls1->mGamepadState = 7;
        mGamepadControls2->mGamepadState = 7;
    }

    if (!mPaused && mTimeStopCounter <= 0) {
        switch (speedUpMode) {
            case 1:
                if (speedUpCounter++ % 5 == 0) {
                    SpeedUpUpdate();
                }
                break;
            case 2:
                if (speedUpCounter++ % 2 == 0) {
                    SpeedUpUpdate();
                }
                break;
            case 3:
                SpeedUpUpdate();
                break;
            case 4:
                SpeedUpUpdate();
                if (speedUpCounter++ % 2 == 0) {
                    SpeedUpUpdate();
                }
                break;
            case 5:
                for (int i = 0; i < 2; ++i) {
                    SpeedUpUpdate();
                }
                break;
            case 6:
                for (int i = 0; i < 4; ++i) {
                    SpeedUpUpdate();
                }
                break;
            case 7:
                for (int i = 0; i < 9; ++i) {
                    SpeedUpUpdate();
                }
                break;
            default:
                break;
        }

        // 为夜晚泳池场景补全泳池反射闪光特效
        //        if ( this->mBackground == BackgroundType::BACKGROUND_4_FOG && this->mPoolSparklyParticleID == 0 && this->mDrawCount > 0 ){
        //            TodParticleSystem * poolSparklyParticle = AddTodParticle(this->mApp, 450.0, 295.0, 220000, a::PARTICLE_POOL_SPARKLY);
        //            this->mPoolSparklyParticleID = LawnApp_ParticleGetID(this->mApp, poolSparklyParticle);
        //        }
    }

    if (ClearAllPlant) {
        RemoveAllPlants();
        ClearAllPlant = false;
    }

    if (clearAllZombies) {
        RemoveAllZombies();
        clearAllZombies = false;
    }

    if (clearAllGraves) {
        GridItem *aGridItem = nullptr;
        while (IterateGridItems(aGridItem)) {
            if (aGridItem->mGridItemType == GridItemType::GRIDITEM_GRAVESTONE) {
                aGridItem->GridItemDie();
            }
        }
        clearAllGraves = false;
    }

    if (clearAllMowers) {
        if (mApp->mGameScene == GameScenes::SCENE_PLAYING) {
            RemoveAllMowers();
        }
        clearAllMowers = false;
    }

    if (recoverAllMowers) {
        if (mApp->mGameScene == GameScenes::SCENE_PLAYING) {
            //            Board_RemoveAllMowers(this);
            ResetLawnMowers();
        }
        recoverAllMowers = false;
    }

    if (passNowLevel) {
        mLevelComplete = true;
        mApp->mBoardResult = mApp->mGameMode == GameMode::GAMEMODE_MP_VS ? BoardResult::BOARDRESULT_VS_PLANT_WON : BoardResult::BOARDRESULT_WON;
        passNowLevel = false;
    }
    // 魅惑所有僵尸
    if (hypnoAllZombies) {
        Zombie *aZombie = nullptr;
        while (IterateZombies(aZombie)) {
            if (aZombie->mZombieType != ZombieType::ZOMBIE_BOSS) {
                aZombie->mMindControlled = true;
            }
        }
        hypnoAllZombies = false;
    }

    if (freezeAllZombies) {
        for (Zombie *aZombie = nullptr; IterateZombies(aZombie); aZombie->HitIceTrap())
            ;
        freezeAllZombies = false;
    }

    if (startAllMowers) {
        if (mApp->mGameScene == GameScenes::SCENE_PLAYING)
            for (LawnMower *alawnMower = nullptr; IterateLawnMowers(alawnMower); alawnMower->StartMower())
                ;
        startAllMowers = false;
    }

    // 修改卡槽
    if (setSeedPacket && choiceSeedType != SeedType::SEED_NONE) {
        if (targetSeedBank == 1) {
            if (choiceSeedType < SeedType::NUM_SEED_TYPES && !mGamepadControls1->mIsZombie) {
                mSeedBank1->mSeedPackets[choiceSeedPacketIndex].mPacketType = isImitaterSeed ? SeedType::SEED_IMITATER : choiceSeedType;
                mSeedBank1->mSeedPackets[choiceSeedPacketIndex].mImitaterType = isImitaterSeed ? choiceSeedType : SeedType::SEED_NONE;
            } else if (choiceSeedType > SeedType::SEED_ZOMBIE_TOMBSTONE && mGamepadControls1->mIsZombie) // IZ模式里用不了墓碑
                mSeedBank1->mSeedPackets[choiceSeedPacketIndex].mPacketType = choiceSeedType;
        } else if (targetSeedBank == 2 && mSeedBank2 != nullptr) {
            if (choiceSeedType < SeedType::NUM_SEED_TYPES && !mGamepadControls2->mIsZombie) {
                mSeedBank2->mSeedPackets[choiceSeedPacketIndex].mPacketType = isImitaterSeed ? SeedType::SEED_IMITATER : choiceSeedType;
                mSeedBank2->mSeedPackets[choiceSeedPacketIndex].mImitaterType = isImitaterSeed ? choiceSeedType : SeedType::SEED_NONE;
            } else if (Challenge::IsZombieSeedType(choiceSeedType) && mGamepadControls2->mIsZombie)
                mSeedBank2->mSeedPackets[choiceSeedPacketIndex].mPacketType = choiceSeedType;
        }
        setSeedPacket = false;
    }

    if (passNowLevel) {
        mLevelComplete = true;
        mApp->mBoardResult = mApp->mGameMode == GameMode::GAMEMODE_MP_VS ? BoardResult::BOARDRESULT_VS_PLANT_WON : BoardResult::BOARDRESULT_WON;
        passNowLevel = false;
    }

    // 布置选择阵型
    if (layChoseFormation) // 用按钮触发, 防止进入游戏时自动布阵
    {
        if (formationId != -1) {
            const char *formation = GetFormationByIndex(formationId);
            char *copiedStr = strdup(formation);
            LoadFormation(copiedStr);
        }
        layChoseFormation = false;
    }

    // 布置粘贴阵型
    if (layPastedFormation) {
        if (customFormation != "") {
            char *copiedStr = strdup(customFormation.c_str());
            LoadFormation(copiedStr);
        }
        layPastedFormation = false;
    }

    if (ladderBuild) {
        if (theBuildLadderX < 9 && theBuildLadderY < (Board_StageHas6Rows(this) ? 6 : 5) && GetLadderAt(theBuildLadderX, theBuildLadderY) == nullptr)
            // 防止选“所有行”或“所有列”的时候放置到场外
            AddALadder(theBuildLadderX, theBuildLadderY);
        ladderBuild = false;
    }


    // 植物放置
    if (plantBuild && theBuildPlantType != SeedType::SEED_NONE) {
        int colsCount = (theBuildPlantType == SeedType::SEED_COBCANNON) ? 8 : 9; // 玉米加农炮不种在九列
        int width = (theBuildPlantType == SeedType::SEED_COBCANNON) ? 2 : 1;     // 玉米加农炮宽度两列
        int rowsCount = Board_StageHas6Rows(this) ? 6 : 5;
        bool isIZMode = mApp->IsIZombieLevel();
        // 全场
        if (theBuildPlantX == 9 && theBuildPlantY == 6) {
            for (int x = 0; x < colsCount; x += width) {
                for (int y = 0; y < rowsCount; y++) {
                    Plant *theBuiltPlant = AddPlant(x, y, theBuildPlantType, (isImitaterPlant ? SeedType::SEED_IMITATER : SeedType::SEED_NONE), 0, true);
                    if (isImitaterPlant)
                        theBuiltPlant->SetImitaterFilterEffect();
                    if (isIZMode)
                        mChallenge->IZombieSetupPlant(theBuiltPlant);
                }
            }
        }
        // 单行
        else if (theBuildPlantX == 9 && theBuildPlantY < 6) {
            for (int x = 0; x < colsCount; x += width) {
                Plant *theBuiltPlant = AddPlant(x, theBuildPlantY, theBuildPlantType, (isImitaterPlant ? SeedType::SEED_IMITATER : SeedType::SEED_NONE), 0, true);
                if (isImitaterPlant)
                    theBuiltPlant->SetImitaterFilterEffect();
                if (isIZMode)
                    mChallenge->IZombieSetupPlant(theBuiltPlant);
            }
        }
        // 单列
        else if (theBuildPlantX < 9 && theBuildPlantY == 6) {
            for (int y = 0; y < rowsCount; y++) {
                Plant *theBuiltPlant = AddPlant(theBuildPlantX, y, theBuildPlantType, (isImitaterPlant ? SeedType::SEED_IMITATER : SeedType::SEED_NONE), 0, true);
                if (isImitaterPlant)
                    theBuiltPlant->SetImitaterFilterEffect();
                if (isIZMode)
                    mChallenge->IZombieSetupPlant(theBuiltPlant);
            }
        }
        // 单格
        else if (theBuildPlantX < colsCount && theBuildPlantY < rowsCount) {
            Plant *theBuiltPlant = AddPlant(theBuildPlantX, theBuildPlantY, theBuildPlantType, (isImitaterPlant ? SeedType::SEED_IMITATER : SeedType::SEED_NONE), 0, true);
            if (isImitaterPlant)
                theBuiltPlant->SetImitaterFilterEffect();
            if (isIZMode)
                mChallenge->IZombieSetupPlant(theBuiltPlant);
        }
        plantBuild = false;
    }

    // 僵尸放置
    if (zombieBuild && theBuildZombieType != ZombieType::ZOMBIE_INVALID) {
        if (theBuildZombieType == ZombieType::ZOMBIE_BOSS)
            AddZombieInRow(theBuildZombieType, 0, 0, true);
        else {
            int colsCount = 9;
            int rowsCount = Board_StageHas6Rows(this) ? 6 : 5;
            // 僵尸出生线
            if (BuildZombieX == 10 && BuildZombieY == 6)
                for (int y = 0; y < rowsCount; ++y)
                    AddZombieInRow(theBuildZombieType, y, mCurrentWave, true);
            // 僵尸出生点
            else if (BuildZombieX == 10 && BuildZombieY < 6)
                AddZombieInRow(theBuildZombieType, BuildZombieY, mCurrentWave, true);
            // 全场
            else if (BuildZombieX == 9 && BuildZombieY == 6)
                for (int x = 0; x < colsCount; ++x)
                    for (int y = 0; y < rowsCount; ++y)
                        mChallenge->IZombiePlaceZombie(theBuildZombieType, x, y);
            // 单行
            else if (BuildZombieX == 9 && BuildZombieY < 6)
                for (int x = 0; x < colsCount; ++x)
                    mChallenge->IZombiePlaceZombie(theBuildZombieType, x, BuildZombieY);
            // 单列
            else if (BuildZombieX < 9 && BuildZombieY == 6)
                for (int y = 0; y < rowsCount; ++y)
                    mChallenge->IZombiePlaceZombie(theBuildZombieType, BuildZombieX, y);
            // 单格
            else if (BuildZombieX < colsCount && BuildZombieY < rowsCount)
                mChallenge->IZombiePlaceZombie(theBuildZombieType, BuildZombieX, BuildZombieY);
        }
        zombieBuild = false;
    }

    // 放置墓碑
    if (graveBuild) {
        int colsCount = 9;
        int rowsCount = Board_StageHas6Rows(this) ? 6 : 5;
        // 全场
        if (BuildZombieX == 9 && BuildZombieY == 6) {
            GridItem *aGridItem = nullptr;
            while (IterateGridItems(aGridItem)) {
                if (aGridItem->mGridItemType == GridItemType::GRIDITEM_GRAVESTONE) {
                    aGridItem->GridItemDie();
                }
            }
            for (int x = 0; x < colsCount; ++x) {
                for (int y = 0; y < rowsCount; ++y) {
                    mChallenge->GraveDangerSpawnGraveAt(x, y);
                }
            }
        }
        // 单行
        else if (BuildZombieX == 9 && BuildZombieY < 6) {
            GridItem *aGridItem = nullptr;
            while (IterateGridItems(aGridItem)) {
                if (aGridItem->mGridItemType == GridItemType::GRIDITEM_GRAVESTONE && aGridItem->mGridY == BuildZombieY) {
                    aGridItem->GridItemDie();
                }
            }
            for (int x = 0; x < colsCount; ++x) {
                mChallenge->GraveDangerSpawnGraveAt(x, BuildZombieY);
            }
        }
        // 单列
        else if (BuildZombieX < 9 && BuildZombieY == 6) {
            GridItem *aGridItem = nullptr;
            while (IterateGridItems(aGridItem)) {
                if (aGridItem->mGridItemType == GridItemType::GRIDITEM_GRAVESTONE && aGridItem->mGridX == BuildZombieX) {
                    aGridItem->GridItemDie();
                }
            }
            for (int y = 0; y < rowsCount; ++y) {
                mChallenge->GraveDangerSpawnGraveAt(BuildZombieX, y);
            }
        }
        // 单格
        else if (BuildZombieX < 9 && BuildZombieY < 6) {
            mChallenge->GraveDangerSpawnGraveAt(BuildZombieX, BuildZombieY);
        }
        graveBuild = false;
    }

    // 出怪设置
    if (buttonSetSpawn && choiceSpawnMode != 0) {
        int typesCount = 0;                          // 已选僵尸种类数
        int typesList[ZombieType::NUM_ZOMBIE_TYPES]; // 已选僵尸种类列表
        // 将僵尸代号放入种类列表, 并更新已选种类数
        for (int type = 0; type < ZombieType::NUM_ZOMBIE_TYPES; ++type) {
            if (checkZombiesAllowed[type] && type != ZombieType::ZOMBIE_BUNGEE) // 飞贼僵尸不应作为正常僵尸出现在出怪列表中
            {
                typesList[typesCount] = type;
                ++typesCount;
            }
        }
        if (typesCount > 0) // 设置出怪需要选择至少 1 种除飞贼以外的僵尸
        {
            // 自然出怪
            if (choiceSpawnMode == 1) {
                // 清空出怪列表
                for (int wave = 0; wave < mNumWaves; ++wave) {
                    for (int index = 0; index < MAX_ZOMBIES_IN_WAVE; ++index)
                        mZombiesInWave[wave][index] = ZombieType::ZOMBIE_INVALID;
                }
                // 设置游戏中的僵尸允许类型
                for (int type = 0; type < ZombieType::NUM_ZOMBIE_TYPES; ++type)
                    mZombieAllowed[type] = checkZombiesAllowed[type];
                mZombieAllowed[ZombieType::ZOMBIE_NORMAL] = true; // 自然出怪下必须含有普通僵尸
                // 由游戏生成出怪列表
                PickZombieWaves();
            }
            // 极限出怪
            else if (choiceSpawnMode == 2) {
                int indexInLevel = 0;
                // 均匀填充出怪列表
                for (int wave = 0; wave < mNumWaves; ++wave) {
                    for (int indexInWave = 0; indexInWave < MAX_ZOMBIES_IN_WAVE; ++indexInWave) {
                        // 使用僵尸的“关内序号”遍历设置出怪可能会比使用“波内序号”更加均匀
                        mZombiesInWave[wave][indexInWave] = (ZombieType)typesList[indexInLevel % typesCount];
                        ++indexInLevel;
                    }
                    if (IsFlagWave(wave)) {
                        mZombiesInWave[wave][0] = ZombieType::ZOMBIE_FLAG; // 生成旗帜僵尸
                        if (checkZombiesAllowed[ZombieType::ZOMBIE_BUNGEE]) {
                            // 生成飞贼僵尸
                            for (int index : {1, 2, 3, 4})
                                mZombiesInWave[wave][index] = ZombieType::ZOMBIE_BUNGEE;
                        }
                    }
                }
                // 不能只出雪人僵尸, 在第一波生成 1 只普通僵尸
                if (checkZombiesAllowed[ZombieType::ZOMBIE_YETI] && typesCount == 1)
                    mZombiesInWave[0][0] = ZombieType::ZOMBIE_NORMAL;
            }
            // 重新生成选卡预览僵尸
            if (mApp->mGameScene == GameScenes::SCENE_LEVEL_INTRO) {
                RemoveCutsceneZombies();
                mCutScene->mPlacedZombies = false;
            }
        }
        buttonSetSpawn = false;
    }

    Board_UpdateButtons(this);
    return old_Board_Update(this);
}

int Board::GetNumWavesPerFlag() {
    // 修改此函数，以做到在进度条上正常绘制旗帜波的旗帜。
    if (mApp->IsFirstTimeAdventureMode() && mNumWaves < 10) {
        return mNumWaves;
    }

    // 额外添加一段判断逻辑，判断关卡代码20且波数少于10的情况
    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_HEAVY_WEAPON && mNumWaves < 10) {
        return mNumWaves;
    }

    return 10;
}

bool Board::IsFlagWave(int theWaveNumber) {
    // 修改此函数，以做到正常出旗帜波僵尸。
    if (!normalLevel) {
        return old_Board_IsFlagWave(this, theWaveNumber);
    }

    if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS)
        return true;

    if (mApp->IsFirstTimeAdventureMode() && mLevel == 1)
        return false;

    int aWavesPerFlag = GetNumWavesPerFlag();
    return theWaveNumber % aWavesPerFlag == aWavesPerFlag - 1;
}

void Board::SpawnZombieWave() {
    // 在对战模式中放出一大波僵尸时播放大波僵尸音效
    if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS) {
        LawnApp_PlaySample(mApp, *Sexy_SOUND_HUGE_WAVE_Addr);
    }

    old_Board_SpawnZombieWave(this);
}

void Board::DrawProgressMeter(Sexy::Graphics *g, int theX, int theY) {
    // 修改此函数，以做到在进度条上正常绘制旗帜波的旗帜。
    if (normalLevel) {
        if (mApp->IsAdventureMode() && ProgressMeterHasFlags()) {
            mApp->mGameMode = GameMode::GAMEMODE_CHALLENGE_HEAVY_WEAPON; // 修改关卡信息为非冒险模式
            old_Board_DrawProgressMeter(this, g, theX, theY);
            mApp->mGameMode = GameMode::GAMEMODE_ADVENTURE; // 再把关卡信息改回冒险模式
            return;
        }
        old_Board_DrawProgressMeter(this, g, theX, theY);
    } else {
        old_Board_DrawProgressMeter(this, g, theX, theY);
    }
}

bool Board::IsLevelDataLoaded() {
    // 确保在开启原版难度时，所有用到levels.xml的地方都不生效
    if (normalLevel)
        return false;

    return old_Board_IsLevelDataLoaded(this);
}

bool Board::NeedSaveGame() {
    // 可以让结盟关卡存档，但是好多BUG啊
    //    if (IsCoopMode(*((int **) this + 69))) {
    //        return true;
    //    }
    return old_Board_NeedSaveGame(this);
}

void Board_DrawHammerButton(Board *board, Sexy::Graphics *g, LawnApp *lawnApp) {
    if (!keyboardMode)
        return;
    float tmp = g->mTransY;
    Rect rect = board->GetButterButtonRect();
    g->DrawImage(*Sexy_IMAGE_SHOVELBANK_Addr, rect.mX, rect.mY);
    g->DrawImage(*Sexy_IMAGE_HAMMER_ICON_Addr, rect.mX - 7, rect.mY - 3);

    if (Sexy_GamepadApp_HasGamepad(lawnApp) || (lawnApp->mGamePad1IsOn && lawnApp->mGamePad2IsOn)) {
        g->DrawImageCel(*Sexy_IMAGE_HELP_BUTTONS_Addr, rect.mX + 36, rect.mY + 40, 2);
    } else {
        g->DrawImageCel(*Sexy_IMAGE_HELP_BUTTONS2_Addr, rect.mX + 36, rect.mY + 40, 2);
    }
    g->SetColorizeImages(false);
    g->mTransY = tmp;
}

void Board_DrawButterButton(Board *board, Sexy::Graphics *g, LawnApp *theApp) {
    if (!theApp->IsCoopMode()) {
        if (!theApp->IsAdventureMode())
            return;
        if (theApp->mTwoPlayerState == -1)
            return;
    }
    float tmp = g->mTransY;
    Rect rect = board->GetButterButtonRect();
    g->DrawImage(*Sexy_IMAGE_SHOVELBANK_Addr, rect.mX, rect.mY);
    if (board->mChallenge->mChallengeState == ChallengeState::STATECHALLENGE_SHOVEL_FLASHING) {
        Color color;
        GetFlashingColor(&color, board->mMainCounter, 75);
        g->SetColorizeImages(true);
        g->SetColor(color);
    }
    // 实现拿着黄油的时候不在栏内绘制黄油
    if (!requestDrawButterInCursor) {
        g->DrawImage(*Sexy_IMAGE_BUTTER_ICON_Addr, rect.mX - 7, rect.mY - 3);
    }
    if (keyboardMode) {
        g->DrawImageCel(*Sexy_IMAGE_HELP_BUTTONS_Addr, rect.mX + 36, rect.mY + 40, 2);
    }
    g->SetColorizeImages(false);
    g->mTransY = tmp;
}

void Board::DrawShovelButton(Sexy::Graphics *g, LawnApp *theApp) {
    // 实现玩家拿着铲子时不在ShovelBank中绘制铲子、实现在对战模式中添加铲子按钮

    if (theApp->mGameMode == GameMode::GAMEMODE_MP_VS) {
        //        LOGD("%d %d",rect[0],rect[1]);
        // return;  原版游戏在此处就return了，所以对战中不绘制铲子按钮。
        if (keyboardMode)
            return;
        TodDrawImageScaledF(g, *Sexy_IMAGE_SHOVELBANK_Addr, mTouchVSShovelRect.mX, mTouchVSShovelRect.mY, 0.8f, 0.8f);
        if (!requestDrawShovelInCursor)
            TodDrawImageScaledF(g, *Sexy_IMAGE_SHOVEL_Addr, -6, 78, 0.8f, 0.8f);
        return;
    }

    float tmp = g->mTransY;
    Rect rect = GetShovelButtonRect();
    g->DrawImage(*Sexy_IMAGE_SHOVELBANK_Addr, rect.mX, rect.mY);

    if (mChallenge->mChallengeState == ChallengeState::STATECHALLENGE_SHOVEL_FLASHING) {
        Color color;
        GetFlashingColor(&color, mMainCounter, 75);
        g->SetColorizeImages(true);
        g->SetColor(color);
    }

    // 实现拿着铲子的时候不在栏内绘制铲子
    if (!requestDrawShovelInCursor) {
        if (theApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_LAST_STAND) {
            Challenge *challenge = mChallenge;
            if (challenge->mChallengeState == ChallengeState::STATECHALLENGE_NORMAL && theApp->mGameScene == GameScenes::SCENE_PLAYING) {
                g->DrawImage(*Sexy_IMAGE_ZEN_MONEYSIGN_Addr, rect.mX - 7, rect.mY - 3);
            } else {
                g->DrawImage(*Sexy_IMAGE_SHOVEL_Addr, rect.mX - 7, rect.mY - 3);
            }
        } else {
            g->DrawImage(*Sexy_IMAGE_SHOVEL_Addr, rect.mX - 7, rect.mY - 3);
        }
    }

    if (keyboardMode) {
        if (theApp->IsCoopMode()) {
            g->DrawImageCel(*Sexy_IMAGE_HELP_BUTTONS_Addr, rect.mX + 40, rect.mY + 40, 1);
        } else {
            if (Sexy_GamepadApp_HasGamepad(theApp) || (theApp->mGamePad1IsOn && theApp->mGamePad2IsOn)) {
                g->DrawImageCel(*Sexy_IMAGE_HELP_BUTTONS_Addr, rect.mX + 50, rect.mY + 40, 1);
            } else {
                g->DrawImageCel(*Sexy_IMAGE_HELP_BUTTONS2_Addr, rect.mX + 50, rect.mY + 40, 1);
            }
        }
    }

    g->SetColorizeImages(false);
    g->mTransY = tmp;
}

void Board::DrawShovel(Sexy::Graphics *g) {
    // 实现拿着铲子、黄油的时候不在栏内绘制铲子、黄油，同时为对战模式添加铲子按钮
    GameMode mGameMode = mApp->mGameMode;
    if (mGameMode == GameMode::GAMEMODE_CHALLENGE_HEAVY_WEAPON)
        return;

    if (mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM || mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN) { // 如果是花园或智慧树
        return DrawZenButtons(g);
    }

    if (mShowHammer) { // 绘制锤子按钮
        Board_DrawHammerButton(this, g, mApp);
    }

    if (mShowButter) { // 绘制黄油按钮
        Board_DrawButterButton(this, g, mApp);
    }

    if (mShowShovel) { // 绘制铲子按钮
        DrawShovelButton(g, mApp);
    }
}

void Board::Pause(bool thePause) {
    //  能在这里得知游戏是否暂停
    //    if (thePause) Music2_StopAllMusic((Music2*)this->mApp->mMusic);
    //    else Music2_StartGameMusic((Music2*)this->mApp->mMusic, true);
    old_Board_Pause(this, thePause);
}

void Board::AddSecondPlayer(int a2) {
    // 去除加入2P时的声音

    //    (*(void (**)(int, int, int))(*(uint32_t *)this[69] + 680))(this[69], Sexy::SOUND_CHIME, 1);
    //    ((void (*)(int *, const char *, int))loc_2F098C)(v2 + 25, "[P2_JOINED]", 11);
    mUnkIntSecondPlayer1 = 3;
    mUnkBoolSecondPlayer = false;
    mUnkIntSecondPlayer2 = 0;
}

bool Board::IsLastStandFinalStage() {
    // 无尽坚不可摧
    if (endlessLastStand)
        return false;

    return mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_LAST_STAND && mChallenge->mSurvivalStage == 4;
}

Plant *Board::GetFlowerPotAt(int theGridX, int theGridY) {
    // 修复 屋顶关卡加农炮无法种植在第三第四列的组合上
    Plant *aPlant = nullptr;
    while (IteratePlants(aPlant)) {
        if (aPlant->mSeedType == SeedType::SEED_FLOWERPOT && aPlant->mRow == theGridY && aPlant->mPlantCol == theGridX && !aPlant->NotOnGround()) {
            return aPlant;
        }
    }

    return nullptr;
}

Plant *Board::GetPumpkinAt(int theGridX, int theGridY) {
    Plant *aPlant = nullptr;
    while (IteratePlants(aPlant)) {
        if (aPlant->mSeedType == SeedType::SEED_PUMPKINSHELL && aPlant->mRow == theGridY && aPlant->mPlantCol == theGridX && !aPlant->NotOnGround()) {
            return aPlant;
        }
    }

    return nullptr;
}

void Board::DoPlantingEffects(int theGridX, int theGridY, Plant *thePlant) {
    int num = GridToPixelX(theGridX, theGridY) + 41;
    int num2 = GridToPixelY(theGridX, theGridY) + 74;
    SeedType mSeedType = thePlant->mSeedType;
    if (mSeedType == SeedType::SEED_LILYPAD) {
        num2 += 15;
    } else if (mSeedType == SeedType::SEED_FLOWERPOT) {
        num2 += 30;
    }

    if (mBackground == BackgroundType::BACKGROUND_GREENHOUSE) {
        mApp->PlayFoley(FoleyType::FOLEY_CERAMIC);
        return;
    }

    if (mBackground == BackgroundType::BACKGROUND_ZOMBIQUARIUM) {
        mApp->PlayFoley(FoleyType::FOLEY_PLANT_WATER);
        return;
    }

    if (Plant::IsFlying(mSeedType)) {
        mApp->PlayFoley(FoleyType::FOLEY_PLANT);
        return;
    }

    if (IsPoolSquare(theGridX, theGridY)) {
        mApp->PlayFoley(FoleyType::FOLEY_PLANT_WATER);
        mApp->AddTodParticle(num, num2, 400000, ParticleEffect::PARTICLE_PLANTING_POOL);
        return;
    }

    mApp->PlayFoley(FoleyType::FOLEY_PLANT);
    //    switch (mSeedType) {
    //        case a::SEED_SUNFLOWER:
    //            LawnApp_PlaySample(mApp, Addon_Sounds.achievement);
    //            break;
    //        default:
    //            PlayFoley(mApp, FoleyType::Plant);
    //            break;
    //    }
    mApp->AddTodParticle(num, num2, 400000, ParticleEffect::PARTICLE_PLANTING);
}


void Board::InitLawnMowers() {
    if (banMower)
        return;

    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BUTTERED_POPCORN)
        return;

    return old_Board_InitLawnMowers(this);
}

void ZombiePickerInitForWave(ZombiePicker *theZombiePicker) {
    memset(theZombiePicker, 0, sizeof(ZombiePicker));
}

void ZombiePickerInit(ZombiePicker *theZombiePicker) {
    ZombiePickerInitForWave(theZombiePicker);
    memset(theZombiePicker->mAllWavesZombieTypeCount, 0, sizeof(theZombiePicker->mAllWavesZombieTypeCount));
}

void Board::PutZombieInWave(ZombieType theZombieType, int theWaveNumber, ZombiePicker* theZombiePicker)
{
    mZombiesInWave[theWaveNumber][theZombiePicker->mZombieCount++] = theZombieType;
    if (theZombiePicker->mZombieCount < MAX_ZOMBIES_IN_WAVE)
    {
        mZombiesInWave[theWaveNumber][theZombiePicker->mZombieCount] = ZombieType::ZOMBIE_INVALID;
    }
    theZombiePicker->mZombiePoints -= GetZombieDefinition(theZombieType).mZombieValue;
    theZombiePicker->mZombieTypeCount[theZombieType]++;
    theZombiePicker->mAllWavesZombieTypeCount[theZombieType]++;
}

void Board::PickZombieWaves() {
    // 有问题，在111和115里，冒险中锤僵尸的mNumWaves从8变6了
    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BUTTERED_POPCORN && !IsLevelDataLoaded()) {
        mNumWaves = 20;
        ZombiePicker zombiePicker;
        ZombiePickerInit(&zombiePicker);
        //        ZombieType introducedZombieType = Board_GetIntroducedZombieType(this);
        for (int i = 0; i < mNumWaves; i++) {
            ZombiePickerInitForWave(&zombiePicker);
            mZombiesInWave[i][0] = ZombieType::ZOMBIE_INVALID;
            bool isFlagWave = IsFlagWave(i);
            //            bool isBeforeLastWave = i == mNumWaves - 1;
            int &aZombiePoints = zombiePicker.mZombiePoints;
            aZombiePoints = i * 4 / 5 + 1;
            if (isFlagWave) {
                int num2 = min(zombiePicker.mZombiePoints, 8);
                zombiePicker.mZombiePoints = (int)(zombiePicker.mZombiePoints * 2.5f);
                for (int k = 0; k < num2; k++) {
                    PutZombieInWave(ZombieType::ZOMBIE_NORMAL, i, &zombiePicker);
                }
                PutZombieInWave(ZombieType::ZOMBIE_FLAG, i, &zombiePicker);
            }
            if (i == mNumWaves - 1)
                PutZombieInWave(ZombieType::ZOMBIE_GARGANTUAR, i, &zombiePicker);
            while (aZombiePoints > 0 && zombiePicker.mZombieCount < MAX_ZOMBIES_IN_WAVE) {
                ZombieType aZombieType = PickZombieType(aZombiePoints, i, &zombiePicker);
                PutZombieInWave(aZombieType, i, &zombiePicker);
            }
        }
        return;
    }

    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_POOL_PARTY && !IsLevelDataLoaded()) {
        mNumWaves = 20;
        ZombiePicker zombiePicker;
        ZombiePickerInit(&zombiePicker);
        //        ZombieType introducedZombieType = Board_GetIntroducedZombieType(this);
        for (int i = 0; i < mNumWaves; i++) {
            ZombiePickerInitForWave(&zombiePicker);
            mZombiesInWave[i][0] = ZombieType::ZOMBIE_INVALID;
            bool isFlagWave = IsFlagWave(i);
            //            bool isBeforeLastWave = i == mNumWaves - 1;
            int &aZombiePoints = zombiePicker.mZombiePoints;
            aZombiePoints = i * 4 / 5 + 1;
            if (isFlagWave) {
                int num2 = min(zombiePicker.mZombiePoints, 8);
                zombiePicker.mZombiePoints = (int)(zombiePicker.mZombiePoints * 2.5f);
                for (int k = 0; k < num2; k++) {
                    PutZombieInWave(ZombieType::ZOMBIE_NORMAL, i, &zombiePicker);
                }
                PutZombieInWave(ZombieType::ZOMBIE_FLAG, i, &zombiePicker);
            }
            while (aZombiePoints > 0 && zombiePicker.mZombieCount < MAX_ZOMBIES_IN_WAVE) {
                ZombieType aZombieType = PickZombieType(aZombiePoints, i, &zombiePicker);
                PutZombieInWave(aZombieType, i, &zombiePicker);
            }
        }
        return;
    }

    return old_Board_PickZombieWaves(this);
}

int Board::GetLiveGargantuarCount() {
    int num = 0;
    Zombie *aZombie = nullptr;
    while (IterateZombies(aZombie)) {
        if (!aZombie->mDead && aZombie->mHasHead && !aZombie->IsDeadOrDying() && aZombie->IsOnBoard()
            && (aZombie->mZombieType == ZombieType::ZOMBIE_GARGANTUAR || aZombie->mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR)) {
            num++;
        }
    }
    return num;
}

int Board::GetLiveZombiesCount() {
    int num = 0;
    Zombie *aZombie = nullptr;
    while (IterateZombies(aZombie)) {
        if (!aZombie->mDead && aZombie->mHasHead && !aZombie->IsDeadOrDying() && aZombie->IsOnBoard()) {
            num++;
        }
    }
    return num;
}

void Board::UpdateLevelEndSequence() {
    // 修复无尽最后一波僵尸出现后高级暂停无法暂停下一关的到来
    if (requestPause)
        return;

    return old_Board_UpdateLevelEndSequence(this);
}

void Board::UpdateGridItems() {
    if (requestPause)
        return;

    return old_Board_UpdateGridItems(this);
}

void Board::MouseMove(int x, int y) {
    // 无用。鼠标指针移动、但左键未按下时调用
    // LOGD("Move%d %d", x, y);
    old_Board_MouseMove(this, x, y);
    //    positionAutoFix = false;
    //    LawnApp *mApp = this->mApp;
    //    GameMode::GameMode mGameMode = mApp->mGameMode;
    //    GamepadControls* gamepadControls1 = this->mGamepadControls1;
    //    CursorObject* cursorObject = this->mCursorObject1;
    //    CursorType::CursorType mCursorType = cursorObject->mCursorType;
    //    if (mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST) {
    //        gamepadControls1->mCursorPositionX = x - 40;
    //        gamepadControls1->mCursorPositionY = y - 40;
    //    } else if (mCursorType == CursorType::CURSOR_TYPE_WATERING_CAN && mApp->mPlayerInfo->mPurchases[a::STORE_ITEM_GOLD_WATERINGCAN] != 0) {
    //        gamepadControls1->mCursorPositionX = x - 40;
    //        gamepadControls1->mCursorPositionY = y - 40;
    //    }else {
    //        gamepadControls1->mCursorPositionX = x;
    //        gamepadControls1->mCursorPositionY = y;
    //    }
}

bool Board::MouseHitTest(int x, int y, HitResult *theHitResult, bool thePlayerIndex) {
    GameMode mGameMode = mApp->mGameMode;
    if (mGameMode == GameMode::GAMEMODE_MP_VS) {
        if (TRect_Contains(&mTouchVSShovelRect, x, y)) {
            theHitResult->mObjectType = GameObjectType::OBJECT_TYPE_SHOVEL;
            return true;
        }
    } else {
        Rect shovelButtonRect = GetShovelButtonRect();
        if (mShowShovel && TRect_Contains(&shovelButtonRect, x, y)) {
            theHitResult->mObjectType = GameObjectType::OBJECT_TYPE_SHOVEL;
            return true;
        }
    }

    if (mApp->IsCoopMode()) {
        Rect butterButtonRect = GetButterButtonRect();
        if (mShowButter && TRect_Contains(&butterButtonRect, x, y)) {
            theHitResult->mObjectType = GameObjectType::OBJECT_TYPE_BUTTER;
            return true;
        }
    }

    if (mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN || mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM) {
        Rect rect;
        for (int i = GameObjectType::OBJECT_TYPE_WATERING_CAN; i <= GameObjectType::OBJECT_TYPE_TREE_OF_WISDOM_GARDEN; i++) {
            if (CanUseGameObject((GameObjectType)i)) {
                rect = GetZenButtonRect((GameObjectType)i);
                if (TRect_Contains(&rect, x, y)) {
                    theHitResult->mObjectType = (GameObjectType)i;
                    return true;
                }
            }
        }
    }

    if (old_Board_MouseHitTest(this, x, y, theHitResult, thePlayerIndex)) {
        if (theHitResult->mObjectType == GameObjectType::OBJECT_TYPE_TREE_OF_WISDOM_GARDEN) {
            theHitResult->mObjectType = GameObjectType::OBJECT_TYPE_NONE;
            return false;
        }
        return true;
    }

    SeedBank *mSeedBank = mGamepadControls1->GetSeedBank();
    if (mSeedBank->ContainsPoint(x, y)) {
        if (mSeedBank->SeedBank::MouseHitTest(x, y, theHitResult)) {
            CursorType mCursorType = mCursorObject1->mCursorType;
            if (mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_BANK) {
                RefreshSeedPacketFromCursor(0);
            }
            return true;
        }
        theHitResult->mObjectType = GameObjectType::OBJECT_TYPE_SEED_BANK_BLANK;
        return false;
    }

    if (mGameMode == GameMode::GAMEMODE_MP_VS || mApp->IsCoopMode()) {
        SeedBank *mSeedBank2 = mGamepadControls2->GetSeedBank();
        if (mSeedBank2->ContainsPoint(x, y)) {
            if (mSeedBank2->SeedBank::MouseHitTest(x, y, theHitResult)) {
                CursorType mCursorType_2P = mCursorObject2->mCursorType;
                if (mCursorType_2P == CursorType::CURSOR_TYPE_PLANT_FROM_BANK) {
                    RefreshSeedPacketFromCursor(1);
                }
                return true;
            }
            theHitResult->mObjectType = GameObjectType::OBJECT_TYPE_SEED_BANK_BLANK;
            return false;
        }
    }

    return false;
}


namespace {
constexpr int mTouchShovelRectWidth = 72;
constexpr int mTouchButterRectWidth = 72;
constexpr int mTouchTrigger = 40;

int mTouchLastX;
int mTouchLastY;
int mTouchDownX;
int mTouchDownY;
bool mSendKeyWhenTouchUp;
TouchState mTouchState = TouchState::TOUCHSTATE_NONE;
float mHeavyWeaponX;
Rect slotMachineRect = {250, 0, 320, 100};
} // namespace

// 触控落下手指在此处理
void Board::MouseDown(int x, int y, int theClickCount) {
    old_Board_MouseDown(this, x, y, theClickCount);
    mTouchDownX = x;
    mTouchDownY = y;
    mTouchLastX = x;
    mTouchLastY = y;
    //    xx = x;
    //    yy = y;
    //    LOGD("%d %d",x,y);
    if (keyboardMode) {
        game_patches::autoPickupSeedPacketDisable.Modify();
    }
    keyboardMode = false;
    SeedBank *seedBank = mGamepadControls1->GetSeedBank();
    int currentSeedBankIndex = mGamepadControls1->mSelectedSeedIndex;
    int mGameState = mGamepadControls1->mGamepadState;
    bool isCobCannonSelected = mGamepadControls1->mIsCobCannonSelected;

    SeedBank *seedBank_2P = mGamepadControls2->GetSeedBank();
    int currentSeedBankIndex_2P = mGamepadControls2->mSelectedSeedIndex;
    int mGameState_2P = mGamepadControls2->mGamepadState;
    bool isCobCannonSelected_2P = mGamepadControls2->mIsCobCannonSelected;
    HitResult hitResult;
    MouseHitTest(x, y, &hitResult, false);
    GameObjectType mObjectType = hitResult.mObjectType;
    GameMode mGameMode = mApp->mGameMode;
    bool isTwoSeedBankMode = (mGameMode == GameMode::GAMEMODE_MP_VS || mApp->IsCoopMode());
    GameScenes mGameScene = mApp->mGameScene;

    SeedChooserScreen *mSeedChooserScreen = mApp->mSeedChooserScreen;
    if (mGameScene == GameScenes::SCENE_LEVEL_INTRO && mSeedChooserScreen != nullptr && mSeedChooserScreen->mChooseState == SeedChooserState::CHOOSE_VIEW_LAWN) {
        mSeedChooserScreen->GameButtonDown(BUTTONCODE_A, 0);
        return;
    }
    if (mGameScene == GameScenes::SCENE_LEVEL_INTRO) {
        mCutScene->MouseDown(x, y);
    }


    if (mObjectType == GameObjectType::OBJECT_TYPE_SEEDPACKET) {
        if (mGameScene == GameScenes::SCENE_LEVEL_INTRO)
            return;
        SeedPacket *seedPacket = (SeedPacket *)hitResult.mObject;
        gPlayerIndex = (TouchPlayerIndex)seedPacket->GetPlayerIndex(); // 玩家1或玩家2
        if (gPlayerIndex == TouchPlayerIndex::TOUCHPLAYER_PLAYER1) {
            requestDrawShovelInCursor = false; // 不再绘制铲子
            if (isCobCannonSelected) {         // 如果拿着加农炮，将其放下
                mGamepadControls1->OnKeyDown(KeyCode::KEYCODE_ESCAPE, 1096);
            }
            if (mGameMode == GameMode::GAMEMODE_CHALLENGE_HEAVY_WEAPON || mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED || mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST
                || mGameMode == GameMode::GAMEMODE_CHALLENGE_ZOMBIQUARIUM) {
                if (seedPacket->CanPickUp()) {
                    mSendKeyWhenTouchUp = true;
                } else {
                    LawnApp_PlaySample(mApp, *Sexy_SOUND_BUZZER_Addr);
                    return;
                }
            }
            if (mGameMode == GameMode::GAMEMODE_CHALLENGE_SLOT_MACHINE) { // 拉老虎机用
                mGamepadControls1->OnKeyDown(KeyCode::KEYCODE_HAMMER, 1112);
                return;
            }
            mTouchState = TouchState::TOUCHSTATE_SEED_BANK; // 记录本次触控的状态
            RefreshSeedPacketFromCursor(0);
            int newSeedPacketIndex = seedPacket->mIndex;
            mGamepadControls1->mSelectedSeedIndex = newSeedPacketIndex;
            seedBank->mSeedPackets[newSeedPacketIndex].mLastSelectedTime = 0.0f; // 动画效果专用
            if (currentSeedBankIndex != newSeedPacketIndex || mGameState != 7) {
                mGamepadControls1->mGamepadState = 7;
                mGamepadControls1->mIsInShopSeedBank = false;
                LawnApp_PlaySample(mApp, *Sexy_SOUND_SEEDLIFT_Addr);
            } else if (currentSeedBankIndex == newSeedPacketIndex && mGameState == 7) {
                mGamepadControls1->mGamepadState = 1;
                if (!isTwoSeedBankMode)
                    mGamepadControls1->mIsInShopSeedBank = true;
            }
        } else {
            requestDrawButterInCursor = false; // 不再绘制黄油
            SeedPacket *seedPacket = (SeedPacket *)hitResult.mObject;
            if (isCobCannonSelected_2P) { // 如果拿着加农炮，将其放下
                mGamepadControls2->OnKeyDown(KeyCode::KEYCODE_ESCAPE, 1096);
            }
            if (mGameMode == GameMode::GAMEMODE_CHALLENGE_HEAVY_WEAPON || mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED || mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST
                || mGameMode == GameMode::GAMEMODE_CHALLENGE_ZOMBIQUARIUM) {
                if (seedPacket->CanPickUp()) {
                    mSendKeyWhenTouchUp = true;
                } else {
                    LawnApp_PlaySample(mApp, *Sexy_SOUND_BUZZER_Addr);
                    return;
                }
            }
            if (mGameMode == GameMode::GAMEMODE_CHALLENGE_SLOT_MACHINE) { // 拉老虎机用
                mGamepadControls2->OnKeyDown(KeyCode::KEYCODE_HAMMER, 1112);
                return;
            }
            mTouchState = TouchState::TOUCHSTATE_SEED_BANK; // 记录本次触控的状态
            RefreshSeedPacketFromCursor(1);
            int newSeedPacketIndex_2P = seedPacket->mIndex;
            mGamepadControls2->mSelectedSeedIndex = newSeedPacketIndex_2P;
            seedBank_2P->mSeedPackets[newSeedPacketIndex_2P].mLastSelectedTime = 0.0f; // 动画效果专用

            if (currentSeedBankIndex_2P != newSeedPacketIndex_2P || mGameState_2P != 7) {
                mGamepadControls2->mGamepadState = 7;
                mGamepadControls2->mIsInShopSeedBank = false;
                LawnApp_PlaySample(mApp, *Sexy_SOUND_SEEDLIFT_Addr);
            } else if (currentSeedBankIndex_2P == newSeedPacketIndex_2P && mGameState_2P == 7) {
                mGamepadControls2->mGamepadState = 1;
                if (!isTwoSeedBankMode)
                    mGamepadControls2->mIsInShopSeedBank = true;
            }
        }
        return;
    }

    if (mObjectType == GameObjectType::OBJECT_TYPE_SEED_BANK_BLANK) {
        return;
    }

    CursorType mCursorType = mCursorObject1->mCursorType;
    CursorType mCursorType_2P = mCursorObject2->mCursorType;

    if (mObjectType == GameObjectType::OBJECT_TYPE_SHOVEL) {
        gPlayerIndex = TouchPlayerIndex::TOUCHPLAYER_PLAYER1; // 玩家1
        mTouchState = TouchState::TOUCHSTATE_SHOVEL_RECT;
        if (mGameState == 7) {
            mGamepadControls1->mGamepadState = 1;
            if (!isTwoSeedBankMode)
                mGamepadControls1->mIsInShopSeedBank = true;
            int newSeedPacketIndex = mGamepadControls1->mSelectedSeedIndex;
            SeedBank *seedBank = mGamepadControls1->GetSeedBank();
            seedBank->mSeedPackets[newSeedPacketIndex].mLastSelectedTime = 0.0f; // 动画效果专用
        }
        if (mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_USABLE_COIN) {
            RefreshSeedPacketFromCursor(0);
            ClearCursor(0);
        }
        if (isCobCannonSelected) { // 如果拿着加农炮，将其放下
            mGamepadControls1->OnKeyDown(KeyCode::KEYCODE_ESCAPE, 1096);
        }
        RefreshSeedPacketFromCursor(0);
        if (requestDrawShovelInCursor) {
            requestDrawShovelInCursor = false;
        } else {
            requestDrawShovelInCursor = true;
            mApp->PlayFoley(FoleyType::FOLEY_SHOVEL);
        }
        return;
    }
    if (mObjectType == GameObjectType::OBJECT_TYPE_BUTTER) {
        gPlayerIndex = TouchPlayerIndex::TOUCHPLAYER_PLAYER2; // 玩家2
        mTouchState = TouchState::TOUCHSTATE_BUTTER_RECT;
        if (mGameState == 7) {
            mGamepadControls2->mGamepadState = 1;
            if (!isTwoSeedBankMode)
                mGamepadControls2->mIsInShopSeedBank = true;
            int newSeedPacketIndex_2P = mGamepadControls2->mSelectedSeedIndex;
            SeedBank *seedBank_2P = mGamepadControls2->GetSeedBank();
            seedBank_2P->mSeedPackets[newSeedPacketIndex_2P].mLastSelectedTime = 0.0f; // 动画效果专用
        }
        if (mCursorType_2P == CursorType::CURSOR_TYPE_PLANT_FROM_USABLE_COIN) {
            RefreshSeedPacketFromCursor(1);
            ClearCursor(1);
        }
        if (isCobCannonSelected_2P) { // 如果拿着加农炮，将其放下
            mGamepadControls2->OnKeyDown(KeyCode::KEYCODE_ESCAPE, 1096);
        }
        RefreshSeedPacketFromCursor(1);
        if (requestDrawButterInCursor) {
            requestDrawButterInCursor = false;
        } else {
            requestDrawButterInCursor = true;
            mApp->PlayFoley(FoleyType::FOLEY_FLOOP);
        }
        return;
    }

    if (mGameMode == GameMode::GAMEMODE_MP_VS) {
        gPlayerIndex = PixelToGridX(x, y) > 5 ? TouchPlayerIndex::TOUCHPLAYER_PLAYER2 : TouchPlayerIndex::TOUCHPLAYER_PLAYER1;
    } else if (mGameMode >= GameMode::GAMEMODE_TWO_PLAYER_COOP_DAY && mGameMode <= GameMode::GAMEMODE_TWO_PLAYER_COOP_ENDLESS) {
        gPlayerIndex = x > 400 ? TouchPlayerIndex::TOUCHPLAYER_PLAYER2 : TouchPlayerIndex::TOUCHPLAYER_PLAYER1;
    } else {
        gPlayerIndex = TouchPlayerIndex::TOUCHPLAYER_PLAYER1;
    }

    if (gPlayerIndexSecond != TouchPlayerIndex::TOUCHPLAYER_NONE && gPlayerIndexSecond == gPlayerIndex) {
        gPlayerIndex = TouchPlayerIndex::TOUCHPLAYER_NONE;
        mTouchState = TouchState::TOUCHSTATE_NONE;
        return;
    }

    if (mObjectType == GameObjectType::OBJECT_TYPE_COIN) {
        Coin *coin = (Coin *)hitResult.mObject;
        if (coin->mType == CoinType::COIN_USABLE_SEED_PACKET) {
            mTouchState = TouchState::TOUCHSTATE_USEFUL_SEED_PACKET;
            requestDrawShovelInCursor = false;
            //            if (mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_USABLE_COIN) {
            //                LOGD("5656565656");
            //                GamepadControls_OnKeyDown(gamepadCon
            //               trols1, 27, 1096);//放下手上的植物卡片
            //                mSendKeyWhenTouchUp = false;
            //            }
            RefreshSeedPacketFromCursor(0);
            old_Coin_GamepadCursorOver(coin, 0); // 捡起植物卡片
            //            Coin_Collect((int) coin, 0);
        }
    }

    if (mGameMode == GameMode::GAMEMODE_CHALLENGE_SLOT_MACHINE) { // 拉老虎机用
        if (TRect_Contains(&slotMachineRect, x, y)) {
            mGamepadControls1->OnKeyDown(KeyCode::KEYCODE_HAMMER, 1112);
            return;
        }
    }

    if (mGameMode == GameMode::GAMEMODE_CHALLENGE_HEAVY_WEAPON) { // 移动重型武器
        mTouchState = TouchState::TOUCHSTATE_HEAVY_WEAPON;
        mHeavyWeaponX = mChallenge->mHeavyWeaponX;
        return;
    }

    if (mChallenge->MouseDown(x, y, 0, &hitResult, 0)) {
        if (mApp->IsScaryPotterLevel()) {
            requestDrawShovelInCursor = false;
        }
        if (mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST) {
            mGamepadControls1->mCursorPositionX = x - 40;
            mGamepadControls1->mCursorPositionY = y - 40;
        } else {
            mGamepadControls1->mCursorPositionX = x;
            mGamepadControls1->mCursorPositionY = y;
        }
        if (!mApp->IsWhackAZombieLevel() || mGameState != 7)
            return; // 这一行代码的意义：在锤僵尸关卡，手持植物时，允许拖动种植。
    }
    if (mObjectType == GameObjectType::OBJECT_TYPE_WATERING_CAN || mObjectType == GameObjectType::OBJECT_TYPE_FERTILIZER || mObjectType == GameObjectType::OBJECT_TYPE_BUG_SPRAY
        || mObjectType == GameObjectType::OBJECT_TYPE_PHONOGRAPH || mObjectType == GameObjectType::OBJECT_TYPE_CHOCOLATE || mObjectType == GameObjectType::OBJECT_TYPE_GLOVE
        || mObjectType == GameObjectType::OBJECT_TYPE_MONEY_SIGN || mObjectType == GameObjectType::OBJECT_TYPE_WHEELBARROW || mObjectType == GameObjectType::OBJECT_TYPE_TREE_FOOD) {
        PickUpTool(mObjectType, 0);
        ((ZenGardenControls *)mGamepadControls1)->mObjectType = mObjectType;
        mTouchState = TouchState::TOUCHSTATE_ZEN_GARDEN_TOOLS;
        return;
    }

    if (mObjectType == GameObjectType::OBJECT_TYPE_ZEN_GARDEN || mObjectType == GameObjectType::OBJECT_TYPE_MUSHROOM_GARDEN || mObjectType == GameObjectType::OBJECT_TYPE_QUARIUM_GARDEN
        || mObjectType == GameObjectType::OBJECT_TYPE_TREE_OF_WISDOM_GARDEN) {
        ((ZenGardenControls *)mGamepadControls1)->mObjectType = mObjectType;
        MouseDownWithTool(x, y, 0, (CursorType)(mObjectType + 3), 0);
        return;
    }


    if (mCursorType == CursorType::CURSOR_TYPE_WATERING_CAN || mCursorType == CursorType::CURSOR_TYPE_FERTILIZER || mCursorType == CursorType::CURSOR_TYPE_BUG_SPRAY
        || mCursorType == CursorType::CURSOR_TYPE_PHONOGRAPH || mCursorType == CursorType::CURSOR_TYPE_CHOCOLATE || mCursorType == CursorType::CURSOR_TYPE_GLOVE
        || mCursorType == CursorType::CURSOR_TYPE_MONEY_SIGN || mCursorType == CursorType::CURSOR_TYPE_WHEEELBARROW || mCursorType == CursorType::CURSOR_TYPE_TREE_FOOD
        || mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_GLOVE) {
        mSendKeyWhenTouchUp = true;
    }

    //    *(uint32_t *) (mGamepadControls1 + 152) = 0;//疑似用于设置该gamepadControls1属于玩家1。可能的取值：-1，0，1
    //    其中，1P恒为0，2P禁用时为-1，2P启用时为1。

    if (gPlayerIndex == TouchPlayerIndex::TOUCHPLAYER_PLAYER1) {
        if (mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST) {
            mGamepadControls1->mCursorPositionX = x - 40;
            mGamepadControls1->mCursorPositionY = y - 40;
        } else if (mCursorType == CursorType::CURSOR_TYPE_WATERING_CAN && mApp->mPlayerInfo->mPurchases[StoreItem::STORE_ITEM_GOLD_WATERINGCAN] != 0) {
            mGamepadControls1->mCursorPositionX = x - 40;
            mGamepadControls1->mCursorPositionY = y - 40;
        } else {
            mGamepadControls1->mCursorPositionX = x;
            mGamepadControls1->mCursorPositionY = y;
        }
    } else {
        if (mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST) {
            mGamepadControls2->mCursorPositionX = x - 40;
            mGamepadControls2->mCursorPositionY = y - 40;
        } else {
            mGamepadControls2->mCursorPositionX = x;
            mGamepadControls2->mCursorPositionY = y;
        }
    }


    int seekBankPosition = mGamepadControls1->mSelectedSeedIndex;
    if (gPlayerIndex == TouchPlayerIndex::TOUCHPLAYER_PLAYER1) {
        if (mGameState == 7 || isCobCannonSelected || requestDrawShovelInCursor || (mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_USABLE_COIN && mTouchState != TouchState::TOUCHSTATE_USEFUL_SEED_PACKET)) {
            mTouchState = TouchState::TOUCHSTATE_PICKING_SOMETHING;
            mSendKeyWhenTouchUp = true;
        }
    } else {
        if (mGameState_2P == 7 || isCobCannonSelected_2P || requestDrawButterInCursor
            || (mCursorType_2P == CursorType::CURSOR_TYPE_PLANT_FROM_USABLE_COIN && mTouchState != TouchState::TOUCHSTATE_USEFUL_SEED_PACKET)) {
            mTouchState = TouchState::TOUCHSTATE_PICKING_SOMETHING;
            mSendKeyWhenTouchUp = true;
        }
    }


    if (mObjectType == GameObjectType::OBJECT_TYPE_PLANT) {
        if (gPlayerIndex == TouchPlayerIndex::TOUCHPLAYER_PLAYER1 && requestDrawShovelInCursor)
            return;
        Plant *plant = (Plant *)hitResult.mObject;
        bool isValidCobCannon = plant->mSeedType == SeedType::SEED_COBCANNON && plant->mState == PlantState::STATE_COBCANNON_READY;
        if (isValidCobCannon) {
            if (gPlayerIndex == TouchPlayerIndex::TOUCHPLAYER_PLAYER1) {
                if (mGameState == 7) {
                    mGamepadControls1->mGamepadState = 1;
                    mSendKeyWhenTouchUp = false;
                    if (!isTwoSeedBankMode)
                        mGamepadControls1->mIsInShopSeedBank = true;
                    int newSeedPacketIndex = mGamepadControls1->mSelectedSeedIndex;
                    SeedBank *seedBank = mGamepadControls1->GetSeedBank();
                    seedBank->mSeedPackets[newSeedPacketIndex].mLastSelectedTime = 0.0f; // 动画效果专用
                }
                if (mGamepadControls2->mIsCobCannonSelected && mGamepadControls2->mCobCannonPlantIndexInList == plant->mPlantIndexInList) {
                    // 不能同时选同一个加农炮！
                    mTouchState = TouchState::TOUCHSTATE_NONE;
                    return;
                }
                GamepadControls_pickUpCobCannon((int)mGamepadControls1, (int)plant);
            } else {
                if (mGameState_2P == 7) {
                    mGamepadControls2->mGamepadState = 1;
                    mSendKeyWhenTouchUp = false;
                    if (!isTwoSeedBankMode)
                        mGamepadControls2->mIsInShopSeedBank = true;
                    int newSeedPacketIndex_2P = mGamepadControls2->mSelectedSeedIndex;
                    SeedBank *seedBank_2P = mGamepadControls2->GetSeedBank();
                    seedBank_2P->mSeedPackets[newSeedPacketIndex_2P].mLastSelectedTime = 0.0f; // 动画效果专用
                }
                if (mGamepadControls1->mIsCobCannonSelected && mGamepadControls1->mCobCannonPlantIndexInList == plant->mPlantIndexInList) {
                    // 不能同时选同一个加农炮！
                    mTouchState = TouchState::TOUCHSTATE_NONE;
                    return;
                }
                GamepadControls_pickUpCobCannon((int)mGamepadControls2, (int)plant);
            }
            mTouchState = TouchState::TOUCHSTATE_VALID_COBCONON;
            return;
        }
    }
    if (mTouchState == TouchState::TOUCHSTATE_NONE)
        mTouchState = TouchState::TOUCHSTATE_BOARD;
}

void Board::MouseDrag(int x, int y) {
    // Drag函数仅仅负责移动光标即可
    old_Board_MouseDrag(this, x, y);
    //    xx = x;
    //    yy = y;
    //    LOGD("%d %d",x,y);
    if (mTouchState == TouchState::TOUCHSTATE_NONE)
        return;

    bool isCobCannonSelected = mGamepadControls1->mIsCobCannonSelected;
    SeedBank *seedBank = mGamepadControls1->GetSeedBank();
    SeedBank *seedBank_2P = mGamepadControls2->GetSeedBank();
    int currentSeedBankIndex_2P = mGamepadControls2->mSelectedSeedIndex;
    int mGameState_2P = mGamepadControls2->mGamepadState;
    bool isCobCannonSelected_2P = mGamepadControls2->mIsCobCannonSelected;
    int mGameState = mGamepadControls1->mGamepadState;
    GameMode mGameMode = mApp->mGameMode;
    bool isTwoSeedBankMode = (mGameMode == GameMode::GAMEMODE_MP_VS || (mGameMode >= GameMode::GAMEMODE_TWO_PLAYER_COOP_DAY && mGameMode <= GameMode::GAMEMODE_TWO_PLAYER_COOP_ENDLESS));
    int seedBankHeight = mApp->IsChallengeWithoutSeedBank() ? 87 : seedBank->mY + seedBank->mHeight;
    if (mTouchState == TouchState::TOUCHSTATE_SEED_BANK && mTouchLastY < seedBankHeight && y >= seedBankHeight) {
        mTouchState = TouchState::TOUCHSTATE_BOARD_MOVED_FROM_SEED_BANK;
        if (gPlayerIndex == TouchPlayerIndex::TOUCHPLAYER_PLAYER1) {
            mGamepadControls1->mGamepadState = 7;
            mGamepadControls1->mIsInShopSeedBank = false;
            requestDrawShovelInCursor = false;
        } else {
            mGamepadControls2->mGamepadState = 7;
            mGamepadControls2->mIsInShopSeedBank = false;
            requestDrawButterInCursor = false;
        }
        mSendKeyWhenTouchUp = true;
    }

    if (mTouchState == TouchState::TOUCHSTATE_SHOVEL_RECT) {
        if (mGameMode == GameMode::GAMEMODE_MP_VS) {
            if (TRect_Contains(&mTouchVSShovelRect, mTouchLastX, mTouchLastY) && !TRect_Contains(&mTouchVSShovelRect, x, y)) {
                mTouchState = TouchState::TOUCHSTATE_BOARD_MOVED_FROM_SHOVEL_RECT;
                if (!requestDrawShovelInCursor)
                    mApp->PlayFoley(FoleyType::FOLEY_SHOVEL);
                requestDrawShovelInCursor = true;
                mGamepadControls1->mGamepadState = 1;
                mSendKeyWhenTouchUp = true;
            }
        } else if (mTouchLastY < mTouchShovelRectWidth && y >= mTouchShovelRectWidth) {
            mTouchState = TouchState::TOUCHSTATE_BOARD_MOVED_FROM_SHOVEL_RECT;
            if (!requestDrawShovelInCursor)
                mApp->PlayFoley(FoleyType::FOLEY_SHOVEL);
            requestDrawShovelInCursor = true;
            mGamepadControls1->mGamepadState = 1;
            mSendKeyWhenTouchUp = true;
        }
    }

    if (mTouchState == TouchState::TOUCHSTATE_BUTTER_RECT && mTouchLastY < mTouchButterRectWidth && y >= mTouchButterRectWidth) {
        mTouchState = TouchState::TOUCHSTATE_BOARD_MOVED_FROM_BUTTER_RECT;
        if (!requestDrawButterInCursor)
            mApp->PlayFoley(FoleyType::FOLEY_FLOOP);
        requestDrawButterInCursor = true;
        mGamepadControls2->mGamepadState = 1;
        mSendKeyWhenTouchUp = true;
    }

    if (mTouchState == TouchState::TOUCHSTATE_VALID_COBCONON || mTouchState == TouchState::TOUCHSTATE_USEFUL_SEED_PACKET) {
        if (!mSendKeyWhenTouchUp && (abs(x - mTouchDownX) > mTouchTrigger || abs(y - mTouchDownY) > mTouchTrigger)) {
            mSendKeyWhenTouchUp = true;
        }
    }

    if (mTouchState == TouchState::TOUCHSTATE_ZEN_GARDEN_TOOLS && mTouchLastY < mTouchButterRectWidth && y >= mTouchButterRectWidth) {
        mTouchState = TouchState::TOUCHSTATE_BOARD_MOVED_FROM_ZEN_GARDEN_TOOLS;
        mSendKeyWhenTouchUp = true;
    }

    if (gPlayerIndex == TouchPlayerIndex::TOUCHPLAYER_PLAYER1 && isCobCannonSelected && mTouchLastY > seedBankHeight && y <= seedBankHeight) {
        mGamepadControls1->OnKeyDown(KeyCode::KEYCODE_ESCAPE, 1096); // 退选加农炮
        mTouchState = TouchState::TOUCHSTATE_NONE;
        mSendKeyWhenTouchUp = false;
    }
    if (gPlayerIndex == TouchPlayerIndex::TOUCHPLAYER_PLAYER2 && isCobCannonSelected_2P && mTouchLastY > seedBankHeight && y <= seedBankHeight) {
        mGamepadControls2->OnKeyDown(KeyCode::KEYCODE_ESCAPE, 1096); // 退选加农炮
        mTouchState = TouchState::TOUCHSTATE_NONE;
        mSendKeyWhenTouchUp = false;
    }

    if (gPlayerIndex == TouchPlayerIndex::TOUCHPLAYER_PLAYER1) {
        if (mGameState == 7 && mTouchLastY > seedBankHeight && y <= seedBankHeight) {
            mGamepadControls1->mGamepadState = 1; // 退选植物
            if (!isTwoSeedBankMode)
                mGamepadControls1->mIsInShopSeedBank = true;
            int newSeedPacketIndex = mGamepadControls1->mSelectedSeedIndex;
            SeedBank *seedBank = mGamepadControls1->GetSeedBank();
            seedBank->mSeedPackets[newSeedPacketIndex].mLastSelectedTime = 0.0f; // 动画效果专用
            mTouchState = TouchState::TOUCHSTATE_NONE;
            mSendKeyWhenTouchUp = false;
        }
    } else {
        if (mGameState_2P == 7 && mTouchLastY > seedBankHeight && y <= seedBankHeight) {
            mGamepadControls2->mGamepadState = 1; // 退选植物
            if (!isTwoSeedBankMode)
                mGamepadControls2->mIsInShopSeedBank = true;
            int newSeedPacketIndex_2P = mGamepadControls2->mSelectedSeedIndex;
            SeedBank *seedBank_2P = mGamepadControls2->GetSeedBank();
            seedBank_2P->mSeedPackets[newSeedPacketIndex_2P].mLastSelectedTime = 0.0f; // 动画效果专用
            mTouchState = TouchState::TOUCHSTATE_NONE;
            mSendKeyWhenTouchUp = false;
        }
    }


    if (mGameMode == GameMode::GAMEMODE_CHALLENGE_HEAVY_WEAPON && mTouchState == TouchState::TOUCHSTATE_HEAVY_WEAPON) {
        mChallenge->mHeavyWeaponX = mHeavyWeaponX + x - mTouchDownX; // 移动重型武器X坐标
        return;
    }

    if (mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST) {
        return;
    }

    if (mTouchState != TouchState::TOUCHSTATE_SEED_BANK && mTouchState != TouchState::TOUCHSTATE_ZEN_GARDEN_TOOLS) {
        if (x > 770)
            x = 770;
        if (x < 40)
            x = 40;
        if (y > 580)
            y = 580;
        if (gPlayerIndex == TouchPlayerIndex::TOUCHPLAYER_PLAYER1) {
            CursorType mCursorType = mCursorObject1->mCursorType;
            if (mCursorType == CursorType::CURSOR_TYPE_WATERING_CAN && mApp->mPlayerInfo->mPurchases[StoreItem::STORE_ITEM_GOLD_WATERINGCAN] != 0) {
                mGamepadControls1->mCursorPositionX = x - 40;
                mGamepadControls1->mCursorPositionY = y - 40;
            } else {
                mGamepadControls1->mCursorPositionX = x;
                mGamepadControls1->mCursorPositionY = y;
            }
        } else {
            mGamepadControls2->mCursorPositionX = x;
            mGamepadControls2->mCursorPositionY = y;
        }
    }
    mTouchLastX = x;
    mTouchLastY = y;
}

void Board::MouseUp(int x, int y, int theClickCount) {
    old_Board_MouseUp(this, x, y, theClickCount);
    if (mTouchState != TouchState::TOUCHSTATE_NONE && mSendKeyWhenTouchUp) {
        SeedBank *seedBank = mGamepadControls1->GetSeedBank();
        int currentSeedBankIndex = mGamepadControls1->mSelectedSeedIndex;
        int numSeedsInBank = seedBank->GetNumSeedsOnConveyorBelt();
        int mGameState = mGamepadControls1->mGamepadState;
        bool isCobCannonSelected = mGamepadControls1->mIsCobCannonSelected;

        SeedBank *seedBank_2P = mGamepadControls2->GetSeedBank();
        int numSeedsInBank_2P = seedBank_2P->GetNumSeedsOnConveyorBelt();
        int currentSeedBankIndex_2P = mGamepadControls2->mSelectedSeedIndex;
        int mGameState_2P = mGamepadControls2->mGamepadState;
        bool isCobCannonSelected_2P = mGamepadControls2->mIsCobCannonSelected;

        GameMode mGameMode = mApp->mGameMode;
        CursorType mCursorType = mCursorObject1->mCursorType;
        CursorType mCursorType_2P = mCursorObject2->mCursorType;
        ChallengeState mChallengeState = mChallenge->mChallengeState;

        if (gPlayerIndex == TouchPlayerIndex::TOUCHPLAYER_PLAYER1) {
            if (requestDrawShovelInCursor) {
                ShovelDown();
            } else if (mGameState == 7 || isCobCannonSelected || mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_USABLE_COIN) {
                if (mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED || mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST) {
                    mGamepadControls1->OnKeyDown(KeyCode::KEYCODE_ESCAPE, 1096);
                    mGamepadControls1->OnKeyDown(KeyCode::KEYCODE_ACCEPT, 1096);
                    mGamepadControls1->OnKeyDown(KeyCode::KEYCODE_ACCEPT, 1096);
                    mGamepadControls1->OnKeyDown(KeyCode::KEYCODE_ESCAPE, 1096);
                } else if ((mGameMode == GameMode::GAMEMODE_CHALLENGE_LAST_STAND && mChallenge->mChallengeState == ChallengeState::STATECHALLENGE_NORMAL
                            && mApp->mGameScene == GameScenes::SCENE_PLAYING)
                           || mGameMode == GameMode::GAMEMODE_MP_VS) {
                    mGamepadControls1->OnButtonDown(ButtonCode::BUTTONCODE_A, 0, 0);
                } else {
                    mGamepadControls1->OnKeyDown(KeyCode::KEYCODE_ACCEPT, 1096);
                }
                int mGameStateNew = mGamepadControls1->mGamepadState;
                int seedPacketIndexNew = mGamepadControls1->mSelectedSeedIndex;
                int numSeedsInBankNew = seedBank->GetNumSeedsOnConveyorBelt();
                mGamepadControls1->mIsInShopSeedBank = mGameStateNew != 7;
                if (mGameState != mGameStateNew) {
                    if (!HasConveyorBeltSeedBank(0) || numSeedsInBank == numSeedsInBankNew) { // 修复传送带关卡种植之后SeedBank动画不正常
                        SeedBank *seedBank = mGamepadControls1->GetSeedBank();
                        seedBank->mSeedPackets[seedPacketIndexNew].mLastSelectedTime = 0.0f; // 动画效果专用
                    }
                }
            } else if (mCursorType == CursorType::CURSOR_TYPE_WATERING_CAN || mCursorType == CursorType::CURSOR_TYPE_FERTILIZER || mCursorType == CursorType::CURSOR_TYPE_BUG_SPRAY
                       || mCursorType == CursorType::CURSOR_TYPE_PHONOGRAPH || mCursorType == CursorType::CURSOR_TYPE_CHOCOLATE || mCursorType == CursorType::CURSOR_TYPE_GLOVE
                       || mCursorType == CursorType::CURSOR_TYPE_MONEY_SIGN || mCursorType == CursorType::CURSOR_TYPE_WHEEELBARROW || mCursorType == CursorType::CURSOR_TYPE_TREE_FOOD
                       || mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_GLOVE) {
                if (!Board_ZenGardenItemNumIsZero(this, mCursorType)) {
                    if (mCursorType == CursorType::CURSOR_TYPE_WATERING_CAN && mApp->mPlayerInfo->mPurchases[StoreItem::STORE_ITEM_GOLD_WATERINGCAN] != 0) {
                        MouseDownWithTool(x - 40, y - 40, 0, mCursorType, 0);
                    } else {
                        MouseDownWithTool(x, y, 0, mCursorType, 0);
                    }
                }
            }
        } else {
            if (requestDrawButterInCursor) {
                requestDrawButterInCursor = false;
            } else if (mGameState_2P == 7 || isCobCannonSelected_2P || mCursorType_2P == CursorType::CURSOR_TYPE_PLANT_FROM_USABLE_COIN) {
                if (mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED || mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST) {
                    mGamepadControls2->OnKeyDown(KeyCode::KEYCODE_ESCAPE, 1096);
                    mGamepadControls2->OnKeyDown(KeyCode::KEYCODE_ACCEPT, 1096);
                    mGamepadControls2->OnKeyDown(KeyCode::KEYCODE_ACCEPT, 1096);
                    mGamepadControls2->OnKeyDown(KeyCode::KEYCODE_ESCAPE, 1096);
                } else if ((mGameMode == GameMode::GAMEMODE_CHALLENGE_LAST_STAND && mChallengeState == ChallengeState::STATECHALLENGE_NORMAL && mApp->mGameScene == GameScenes::SCENE_PLAYING)
                           || mGameMode == GameMode::GAMEMODE_MP_VS) {
                    mGamepadControls2->OnButtonDown(ButtonCode::BUTTONCODE_A, 1, 0);
                } else {
                    mGamepadControls2->OnKeyDown(KeyCode::KEYCODE_ACCEPT, 1096);
                }
                int mGameStateNew_2P = mGamepadControls2->mGamepadState;
                int seedPacketIndexNew_2P = mGamepadControls2->mSelectedSeedIndex;
                int numSeedsInBankNew_2P = seedBank_2P->GetNumSeedsOnConveyorBelt();
                mGamepadControls2->mIsInShopSeedBank = mGameStateNew_2P != 7;
                if (mGameState_2P != mGameStateNew_2P) {
                    if (!HasConveyorBeltSeedBank(1) || numSeedsInBank_2P == numSeedsInBankNew_2P) { // 修复传送带关卡种植之后SeedBank动画不正常
                        SeedBank *seedBank_2P = mGamepadControls2->GetSeedBank();
                        seedBank_2P->mSeedPackets[seedPacketIndexNew_2P].mLastSelectedTime = 0.0f; // 动画效果专用
                    }
                }
                if (mGameMode == GameMode::GAMEMODE_MP_VS) {
                    mGamepadControls2->mGamepadState = 1;
                }
            }
        }
    }
    gPlayerIndex = TouchPlayerIndex::TOUCHPLAYER_NONE;
    mSendKeyWhenTouchUp = false;
    mTouchState = TouchState::TOUCHSTATE_NONE;
}


namespace {
int gTouchLastXSecond;
int gTouchLastYSecond;
int gTouchDownXSecond;
int gTouchDownYSecond;
bool gSendKeyWhenTouchUpSecond;
TouchState gTouchStateSecond = TouchState::TOUCHSTATE_NONE;
} // namespace

void Board::MouseDownSecond(int x, int y, int theClickCount) {
    // 触控落下手指在此处理
    gTouchDownXSecond = x;
    gTouchDownYSecond = y;
    gTouchLastXSecond = x;
    gTouchLastYSecond = y;
    if (keyboardMode) {
        game_patches::autoPickupSeedPacketDisable.Modify();
    }
    keyboardMode = false;

    SeedBank *seedBank = mGamepadControls1->GetSeedBank();
    int currentSeedBankIndex = mGamepadControls1->mSelectedSeedIndex;
    int mGameState = mGamepadControls1->mGamepadState;
    bool isCobCannonSelected = mGamepadControls1->mIsCobCannonSelected;

    SeedBank *seedBank_2P = mGamepadControls2->GetSeedBank();
    int currentSeedBankIndex_2P = mGamepadControls2->mSelectedSeedIndex;
    int mGameState_2P = mGamepadControls2->mGamepadState;
    bool isCobCannonSelected_2P = mGamepadControls2->mIsCobCannonSelected;
    HitResult hitResult;
    MouseHitTest(x, y, &hitResult, false);
    GameObjectType mObjectType = hitResult.mObjectType;
    GameMode mGameMode = mApp->mGameMode;
    bool isTwoSeedBankMode = (mGameMode == GameMode::GAMEMODE_MP_VS || (mGameMode >= GameMode::GAMEMODE_TWO_PLAYER_COOP_DAY && mGameMode <= GameMode::GAMEMODE_TWO_PLAYER_COOP_ENDLESS));
    GameScenes mGameScene = mApp->mGameScene;

    SeedChooserScreen *mSeedChooserScreen = mApp->mSeedChooserScreen;
    if (mGameScene == GameScenes::SCENE_LEVEL_INTRO && mSeedChooserScreen != nullptr && mSeedChooserScreen->mChooseState == SeedChooserState::CHOOSE_VIEW_LAWN) {
        mSeedChooserScreen->GameButtonDown(BUTTONCODE_A, 0);
        return;
    }
    if (mGameScene == GameScenes::SCENE_LEVEL_INTRO) {
        mCutScene->MouseDown(x, y);
    }

    if (mObjectType == GameObjectType::OBJECT_TYPE_SEEDPACKET) {
        if (mGameScene == GameScenes::SCENE_LEVEL_INTRO)
            return;
        SeedPacket *seedPacket = (SeedPacket *)hitResult.mObject;
        int newSeedPacketIndex = seedPacket->mIndex;
        gPlayerIndexSecond = (TouchPlayerIndex)seedPacket->GetPlayerIndex(); // 玩家1或玩家2

        if (gPlayerIndexSecond == TouchPlayerIndex::TOUCHPLAYER_PLAYER1) {
            requestDrawShovelInCursor = false; // 不再绘制铲子
            if (isCobCannonSelected) {         // 如果拿着加农炮，将其放下
                mGamepadControls1->OnKeyDown(KeyCode::KEYCODE_ESCAPE, 1096);
            }
            if (mGameMode == GameMode::GAMEMODE_CHALLENGE_HEAVY_WEAPON || mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED || mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST
                || mGameMode == GameMode::GAMEMODE_CHALLENGE_ZOMBIQUARIUM) {
                if (seedPacket->CanPickUp()) {
                    gSendKeyWhenTouchUpSecond = true;
                } else {
                    LawnApp_PlaySample(mApp, *Sexy_SOUND_BUZZER_Addr);
                    return;
                }
            }
            if (mGameMode == GameMode::GAMEMODE_CHALLENGE_SLOT_MACHINE) { // 拉老虎机用
                mGamepadControls1->OnKeyDown(KeyCode::KEYCODE_HAMMER, 1112);
                return;
            }
            gTouchStateSecond = TouchState::TOUCHSTATE_SEED_BANK; // 记录本次触控的状态
            RefreshSeedPacketFromCursor(0);

            mGamepadControls1->mSelectedSeedIndex = newSeedPacketIndex;
            seedBank->mSeedPackets[newSeedPacketIndex].mLastSelectedTime = 0.0f; // 动画效果专用

            if (currentSeedBankIndex != newSeedPacketIndex || mGameState != 7) {
                mGamepadControls1->mGamepadState = 7;
                mGamepadControls1->mIsInShopSeedBank = false;
                LawnApp_PlaySample(mApp, *Sexy_SOUND_SEEDLIFT_Addr);
            } else if (currentSeedBankIndex == newSeedPacketIndex && mGameState == 7) {
                mGamepadControls1->mGamepadState = 1;
                if (!isTwoSeedBankMode)
                    mGamepadControls1->mIsInShopSeedBank = true;
            }
        } else {
            requestDrawButterInCursor = false; // 不再绘制黄油
            SeedPacket *seedPacket = (SeedPacket *)hitResult.mObject;
            if (isCobCannonSelected_2P) { // 如果拿着加农炮，将其放下
                mGamepadControls2->OnKeyDown(KeyCode::KEYCODE_ESCAPE, 1096);
            }
            if (mGameMode == GameMode::GAMEMODE_CHALLENGE_HEAVY_WEAPON || mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED || mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST
                || mGameMode == GameMode::GAMEMODE_CHALLENGE_ZOMBIQUARIUM) {
                if (seedPacket->CanPickUp()) {
                    gSendKeyWhenTouchUpSecond = true;
                } else {
                    LawnApp_PlaySample(mApp, *Sexy_SOUND_BUZZER_Addr);
                    return;
                }
            }
            if (mGameMode == GameMode::GAMEMODE_CHALLENGE_SLOT_MACHINE) { // 拉老虎机用
                mGamepadControls2->OnKeyDown(KeyCode::KEYCODE_HAMMER, 1112);
                return;
            }
            gTouchStateSecond = TouchState::TOUCHSTATE_SEED_BANK; // 记录本次触控的状态
            RefreshSeedPacketFromCursor(1);
            int newSeedPacketIndex_2P = seedPacket->mIndex;
            mGamepadControls2->mSelectedSeedIndex = newSeedPacketIndex_2P;
            seedBank_2P->mSeedPackets[newSeedPacketIndex_2P].mLastSelectedTime = 0.0f; // 动画效果专用

            if (currentSeedBankIndex_2P != newSeedPacketIndex_2P || mGameState_2P != 7) {
                mGamepadControls2->mGamepadState = 7;
                mGamepadControls2->mIsInShopSeedBank = false;
                LawnApp_PlaySample(mApp, *Sexy_SOUND_SEEDLIFT_Addr);
            } else if (currentSeedBankIndex_2P == newSeedPacketIndex_2P && mGameState_2P == 7) {
                mGamepadControls2->mGamepadState = 1;
                if (!isTwoSeedBankMode)
                    mGamepadControls2->mIsInShopSeedBank = true;
            }
        }
        return;
    }

    if (mObjectType == GameObjectType::OBJECT_TYPE_SEED_BANK_BLANK) {
        return;
    }

    CursorType mCursorType = mCursorObject1->mCursorType;
    CursorType mCursorType_2P = mCursorObject2->mCursorType;
    //    if (mCursorType == CursorType::CURSOR_TYPE_WATERING_CAN || mCursorType == CursorType::CURSOR_TYPE_FERTILIZER ||
    //        mCursorType == CursorType::CURSOR_TYPE_BUG_SPRAY || mCursorType == CursorType::OBJECT_TYPE_PHONOGRAPH ||
    //        mCursorType == CursorType::OBJECT_TYPE_CHOCOLATE || mCursorType == CursorType::OBJECT_TYPE_GLOVE ||
    //        mCursorType == CursorType::CURSOR_TYPE_MONEY_SIGN || mCursorType == CursorType::CURSOR_TYPE_WHEEELBARROW ||
    //        mCursorType == CursorType::CURSOR_TYPE_TREE_FOOD) {
    //        MouseDownWithTool(this, x, y, 0, mCursorType, false);
    //        return;
    //    }

    if (mObjectType == GameObjectType::OBJECT_TYPE_SHOVEL) {
        if (!useNewShovel) {
            mGamepadControls1->OnKeyDown(KeyCode::KEYCODE_SHOVEL, 1112);
            return;
        }
        gPlayerIndexSecond = TouchPlayerIndex::TOUCHPLAYER_PLAYER1; // 玩家1
        gTouchStateSecond = TouchState::TOUCHSTATE_SHOVEL_RECT;
        if (mGameState == 7) {
            mGamepadControls1->mGamepadState = 1;
            if (!isTwoSeedBankMode)
                mGamepadControls1->mIsInShopSeedBank = true;
            int newSeedPacketIndex = mGamepadControls1->mSelectedSeedIndex;
            SeedBank *seedBank = mGamepadControls1->GetSeedBank();
            seedBank->mSeedPackets[newSeedPacketIndex].mLastSelectedTime = 0.0f; // 动画效果专用
        }
        if (mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_USABLE_COIN) {
            RefreshSeedPacketFromCursor(0);
            ClearCursor(0);
        }
        if (isCobCannonSelected) { // 如果拿着加农炮，将其放下
            mGamepadControls1->OnKeyDown(KeyCode::KEYCODE_ESCAPE, 1096);
        }
        RefreshSeedPacketFromCursor(0);
        if (requestDrawShovelInCursor) {
            requestDrawShovelInCursor = false;
        } else {
            requestDrawShovelInCursor = true;
            mApp->PlayFoley(FoleyType::FOLEY_SHOVEL);
        }
        return;
    }

    if (mObjectType == GameObjectType::OBJECT_TYPE_BUTTER) {
        gPlayerIndexSecond = TouchPlayerIndex::TOUCHPLAYER_PLAYER2; // 玩家2
        gTouchStateSecond = TouchState::TOUCHSTATE_BUTTER_RECT;
        if (mGameState == 7) {
            mGamepadControls2->mGamepadState = 1;
            if (!isTwoSeedBankMode)
                mGamepadControls2->mIsInShopSeedBank = true;
            int newSeedPacketIndex_2P = mGamepadControls2->mSelectedSeedIndex;
            SeedBank *seedBank_2P = mGamepadControls2->GetSeedBank();
            seedBank_2P->mSeedPackets[newSeedPacketIndex_2P].mLastSelectedTime = 0.0f; // 动画效果专用
        }
        if (mCursorType_2P == CursorType::CURSOR_TYPE_PLANT_FROM_USABLE_COIN) {
            RefreshSeedPacketFromCursor(1);
            ClearCursor(1);
        }
        if (isCobCannonSelected_2P) { // 如果拿着加农炮，将其放下
            mGamepadControls2->OnKeyDown(KeyCode::KEYCODE_ESCAPE, 1096);
        }
        RefreshSeedPacketFromCursor(1);
        if (requestDrawButterInCursor) {
            requestDrawButterInCursor = false;
        } else {
            requestDrawButterInCursor = true;
            mApp->PlayFoley(FoleyType::FOLEY_FLOOP);
        }
        return;
    }

    if (mGameMode == GameMode::GAMEMODE_MP_VS) {
        gPlayerIndexSecond = PixelToGridX(x, y) > 5 ? TouchPlayerIndex::TOUCHPLAYER_PLAYER2 : TouchPlayerIndex::TOUCHPLAYER_PLAYER1;
    } else if (mGameMode >= GameMode::GAMEMODE_TWO_PLAYER_COOP_DAY && mGameMode <= GameMode::GAMEMODE_TWO_PLAYER_COOP_ENDLESS) {
        gPlayerIndexSecond = x > 400 ? TouchPlayerIndex::TOUCHPLAYER_PLAYER2 : TouchPlayerIndex::TOUCHPLAYER_PLAYER1;
    } else {
        gPlayerIndexSecond = TouchPlayerIndex::TOUCHPLAYER_PLAYER1;
    }

    if (gPlayerIndex != TouchPlayerIndex::TOUCHPLAYER_NONE && gPlayerIndexSecond == gPlayerIndex) {
        if (mObjectType == GameObjectType::OBJECT_TYPE_PLANT) {
            Plant *plant = (Plant *)hitResult.mObject;
            bool isValidCobCannon = plant->mSeedType == SeedType::SEED_COBCANNON && plant->mState == PlantState::STATE_COBCANNON_READY;
            if (!isValidCobCannon) {
                gPlayerIndexSecond = TouchPlayerIndex::TOUCHPLAYER_NONE;
                gTouchStateSecond = TouchState::TOUCHSTATE_NONE;
                return;
            }
        } else {
            gPlayerIndexSecond = TouchPlayerIndex::TOUCHPLAYER_NONE;
            gTouchStateSecond = TouchState::TOUCHSTATE_NONE;
            return;
        }
    }

    if (mObjectType == GameObjectType::OBJECT_TYPE_COIN) {
        Coin *coin = (Coin *)hitResult.mObject;

        if (coin->mType == CoinType::COIN_USABLE_SEED_PACKET) {
            gTouchStateSecond = TouchState::TOUCHSTATE_USEFUL_SEED_PACKET;
            requestDrawShovelInCursor = false;
            //            if (mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_USABLE_COIN) {
            //                LOGD("5656565656");
            //                GamepadControls_OnKeyDown(gamepadCon
            //               trols1, 27, 1096);//放下手上的植物卡片
            //                mSendKeyWhenTouchUp = false;
            //            }
            RefreshSeedPacketFromCursor(0);
            old_Coin_GamepadCursorOver(coin, 0); // 捡起植物卡片
            //            Coin_Collect((int) coin, 0);
        }
    }

    if (mGameMode == GameMode::GAMEMODE_CHALLENGE_SLOT_MACHINE) { // 拉老虎机用
        if (TRect_Contains(&slotMachineRect, x, y)) {
            mGamepadControls1->OnKeyDown(KeyCode::KEYCODE_HAMMER, 1112);
            return;
        }
    }

    if (mGameMode == GameMode::GAMEMODE_CHALLENGE_HEAVY_WEAPON) { // 移动重型武器
        gTouchStateSecond = TouchState::TOUCHSTATE_HEAVY_WEAPON;
        mHeavyWeaponX = *((float *)mChallenge + 67);
        return;
    }

    if (mChallenge->MouseDown(x, y, 0, &hitResult, 0)) {
        if (mApp->IsScaryPotterLevel()) {
            requestDrawShovelInCursor = false;
        }
        if (mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST) {
            mGamepadControls1->mCursorPositionX = x - 40;
            mGamepadControls1->mCursorPositionY = y - 40;
        } else {
            mGamepadControls1->mCursorPositionX = x;
            mGamepadControls1->mCursorPositionY = y;
        }
        if (!mApp->IsWhackAZombieLevel() || mGameState != 7)
            return; // 这一行代码的意义：在锤僵尸关卡，手持植物时，允许拖动种植。
    }


    if (mObjectType == GameObjectType::OBJECT_TYPE_WATERING_CAN || mObjectType == GameObjectType::OBJECT_TYPE_FERTILIZER || mObjectType == GameObjectType::OBJECT_TYPE_BUG_SPRAY
        || mObjectType == GameObjectType::OBJECT_TYPE_PHONOGRAPH || mObjectType == GameObjectType::OBJECT_TYPE_CHOCOLATE || mObjectType == GameObjectType::OBJECT_TYPE_GLOVE
        || mObjectType == GameObjectType::OBJECT_TYPE_MONEY_SIGN || mObjectType == GameObjectType::OBJECT_TYPE_WHEELBARROW || mObjectType == GameObjectType::OBJECT_TYPE_TREE_FOOD) {
        PickUpTool(mObjectType, 0);
        return;
    }

    float tmpX1, tmpY1;
    tmpX1 = mGamepadControls1->mCursorPositionX;
    tmpY1 = mGamepadControls1->mCursorPositionY;

    if (gPlayerIndexSecond == TouchPlayerIndex::TOUCHPLAYER_PLAYER1) {
        if (mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST) {
            mGamepadControls1->mCursorPositionX = x - 40;
            mGamepadControls1->mCursorPositionY = y - 40;
        } else {
            mGamepadControls1->mCursorPositionX = x;
            mGamepadControls1->mCursorPositionY = y;
        }
    } else {
        if (mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST) {
            mGamepadControls2->mCursorPositionX = x - 40;
            mGamepadControls2->mCursorPositionY = y - 40;
        } else {
            mGamepadControls2->mCursorPositionX = x;
            mGamepadControls2->mCursorPositionY = y;
        }
    }


    int seekBankPosition = mGamepadControls1->mSelectedSeedIndex;
    if (gPlayerIndexSecond == TouchPlayerIndex::TOUCHPLAYER_PLAYER1) {
        if (mGameState == 7 || isCobCannonSelected || requestDrawShovelInCursor
            || (mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_USABLE_COIN && gTouchStateSecond != TouchState::TOUCHSTATE_USEFUL_SEED_PACKET)) {
            gTouchStateSecond = TouchState::TOUCHSTATE_PICKING_SOMETHING;
            gSendKeyWhenTouchUpSecond = true;
        }
    } else {
        if (mGameState_2P == 7 || isCobCannonSelected_2P || requestDrawButterInCursor
            || (mCursorType_2P == CursorType::CURSOR_TYPE_PLANT_FROM_USABLE_COIN && gTouchStateSecond != TouchState::TOUCHSTATE_USEFUL_SEED_PACKET)) {
            gTouchStateSecond = TouchState::TOUCHSTATE_PICKING_SOMETHING;
            gSendKeyWhenTouchUpSecond = true;
        }
    }


    if (mObjectType == GameObjectType::OBJECT_TYPE_PLANT) {
        if (gPlayerIndexSecond == TouchPlayerIndex::TOUCHPLAYER_PLAYER1 && requestDrawShovelInCursor)
            return;
        Plant *plant = (Plant *)hitResult.mObject;
        bool isValidCobCannon = plant->mSeedType == SeedType::SEED_COBCANNON && plant->mState == PlantState::STATE_COBCANNON_READY;
        if (isValidCobCannon) {
            if (gPlayerIndex == TouchPlayerIndex::TOUCHPLAYER_PLAYER1 && mGamepadControls2->mPlayerIndex2 == -1) {
                mGamepadControls2->mCursorPositionX = x;
                mGamepadControls2->mCursorPositionY = y;
                mGamepadControls1->mCursorPositionX = tmpX1;
                mGamepadControls1->mCursorPositionY = tmpY1;
                if (mGamepadControls1->mIsCobCannonSelected && mGamepadControls1->mCobCannonPlantIndexInList == plant->mPlantIndexInList) {
                    // 不能同时选同一个加农炮！
                    gTouchStateSecond = TouchState::TOUCHSTATE_NONE;
                    return;
                }
                mGamepadControls2->mIsInShopSeedBank = true;
                mGamepadControls2->mPlayerIndex2 = 1;
                gPlayerIndexSecond = TouchPlayerIndex::TOUCHPLAYER_PLAYER2;
                GamepadControls_pickUpCobCannon((int)mGamepadControls2, (int)plant);
                gTouchStateSecond = TouchState::TOUCHSTATE_VALID_COBCONON_SECOND;
                return;
            } else if (gPlayerIndexSecond == TouchPlayerIndex::TOUCHPLAYER_PLAYER1) {
                if (mGameState == 7) {
                    mGamepadControls1->mGamepadState = 1;
                    gSendKeyWhenTouchUpSecond = false;
                    if (!isTwoSeedBankMode)
                        mGamepadControls1->mIsInShopSeedBank = true;
                    int newSeedPacketIndex = mGamepadControls1->mSelectedSeedIndex;
                    SeedBank *seedBank = mGamepadControls1->GetSeedBank();
                    seedBank->mSeedPackets[newSeedPacketIndex].mLastSelectedTime = 0.0f; // 动画效果专用
                }
                GamepadControls_pickUpCobCannon((int)mGamepadControls1, (int)plant);
            } else {
                if (mGameState_2P == 7) {
                    mGamepadControls2->mGamepadState = 1;
                    gSendKeyWhenTouchUpSecond = false;
                    if (!isTwoSeedBankMode)
                        mGamepadControls2->mIsInShopSeedBank = true;
                    int newSeedPacketIndex_2P = mGamepadControls2->mSelectedSeedIndex;
                    SeedBank *seedBank_2P = mGamepadControls2->GetSeedBank();
                    seedBank_2P->mSeedPackets[newSeedPacketIndex_2P].mLastSelectedTime = 0.0f; // 动画效果专用
                }
                GamepadControls_pickUpCobCannon((int)mGamepadControls2, (int)plant);
            }
            gTouchStateSecond = TouchState::TOUCHSTATE_VALID_COBCONON;
            return;
        }
    }

    if (gTouchStateSecond == TouchState::TOUCHSTATE_NONE)
        gTouchStateSecond = TouchState::TOUCHSTATE_BOARD;
}


void Board::MouseDragSecond(int x, int y) {
    // Drag函数仅仅负责移动光标即可
    if (gTouchStateSecond == TouchState::TOUCHSTATE_NONE)
        return;

    bool isCobCannonSelected = mGamepadControls1->mIsCobCannonSelected;
    bool isCobCannonSelected_2P = mGamepadControls2->mIsCobCannonSelected;
    SeedBank *aSeedBank = mGamepadControls1->GetSeedBank();
    SeedBank *aSeedBank_2P = mGamepadControls2->GetSeedBank();
    int currentSeedBankIndex_2P = mGamepadControls2->mSelectedSeedIndex;
    int aGameState = mGamepadControls1->mGamepadState;
    int aGameState_2P = mGamepadControls2->mGamepadState;
    GameMode aGameMode = mApp->mGameMode;
    bool isTwoSeedBankMode = (aGameMode == GameMode::GAMEMODE_MP_VS || (aGameMode >= GameMode::GAMEMODE_TWO_PLAYER_COOP_DAY && aGameMode <= GameMode::GAMEMODE_TWO_PLAYER_COOP_ENDLESS));
    int seedBankHeight = mApp->IsChallengeWithoutSeedBank() ? 87 : aSeedBank->mY + aSeedBank->mHeight;
    if (gTouchStateSecond == TouchState::TOUCHSTATE_SEED_BANK && gTouchLastYSecond < seedBankHeight && y >= seedBankHeight) {
        gTouchStateSecond = TouchState::TOUCHSTATE_BOARD_MOVED_FROM_SEED_BANK;
        if (gPlayerIndexSecond == TouchPlayerIndex::TOUCHPLAYER_PLAYER1) {
            mGamepadControls1->mGamepadState = 7;
            mGamepadControls1->mIsInShopSeedBank = false;
            requestDrawShovelInCursor = false;
        } else {
            mGamepadControls2->mGamepadState = 7;
            mGamepadControls2->mIsInShopSeedBank = false;
            requestDrawButterInCursor = false;
        }
        gSendKeyWhenTouchUpSecond = true;
    }

    if (gTouchStateSecond == TouchState::TOUCHSTATE_SHOVEL_RECT) {
        if (aGameMode == GameMode::GAMEMODE_MP_VS) {
            if (TRect_Contains(&mTouchVSShovelRect, gTouchLastXSecond, gTouchLastYSecond) && !TRect_Contains(&mTouchVSShovelRect, x, y)) {
                gTouchStateSecond = TouchState::TOUCHSTATE_BOARD_MOVED_FROM_SHOVEL_RECT;
                if (!requestDrawShovelInCursor)
                    mApp->PlayFoley(FoleyType::FOLEY_SHOVEL);
                requestDrawShovelInCursor = true;
                mGamepadControls1->mGamepadState = 1;
                gSendKeyWhenTouchUpSecond = true;
            }
        } else if (gTouchLastYSecond < mTouchShovelRectWidth && y >= mTouchShovelRectWidth) {
            gTouchStateSecond = TouchState::TOUCHSTATE_BOARD_MOVED_FROM_SHOVEL_RECT;
            if (!requestDrawShovelInCursor)
                mApp->PlayFoley(FoleyType::FOLEY_SHOVEL);
            requestDrawShovelInCursor = true;
            mGamepadControls1->mGamepadState = 1;
            gSendKeyWhenTouchUpSecond = true;
        }
    }

    if (gTouchStateSecond == TouchState::TOUCHSTATE_BUTTER_RECT && gTouchLastYSecond < mTouchButterRectWidth && y >= mTouchButterRectWidth) {
        gTouchStateSecond = TouchState::TOUCHSTATE_BOARD_MOVED_FROM_BUTTER_RECT;
        if (!requestDrawButterInCursor)
            mApp->PlayFoley(FoleyType::FOLEY_FLOOP);
        requestDrawButterInCursor = true;
        mGamepadControls2->mGamepadState = 1;
        gSendKeyWhenTouchUpSecond = true;
    }

    if (gTouchStateSecond == TouchState::TOUCHSTATE_VALID_COBCONON || gTouchStateSecond == TouchState::TOUCHSTATE_VALID_COBCONON_SECOND
        || gTouchStateSecond == TouchState::TOUCHSTATE_USEFUL_SEED_PACKET) {
        if (!gSendKeyWhenTouchUpSecond && (abs(x - gTouchDownXSecond) > mTouchTrigger || abs(y - gTouchDownYSecond) > mTouchTrigger)) {
            gSendKeyWhenTouchUpSecond = true;
        }
    }


    if (gPlayerIndexSecond == TouchPlayerIndex::TOUCHPLAYER_PLAYER1 && isCobCannonSelected && gTouchLastYSecond > seedBankHeight && y <= seedBankHeight) {
        mGamepadControls1->OnKeyDown(KeyCode::KEYCODE_ESCAPE, 1096); // 退选加农炮
        gTouchStateSecond = TouchState::TOUCHSTATE_NONE;
        gSendKeyWhenTouchUpSecond = false;
    }

    if (gPlayerIndexSecond == TouchPlayerIndex::TOUCHPLAYER_PLAYER2 && isCobCannonSelected_2P && gTouchLastYSecond > seedBankHeight && y <= seedBankHeight) {
        mGamepadControls2->OnKeyDown(KeyCode::KEYCODE_ESCAPE, 1096); // 退选加农炮
        if (gTouchStateSecond == TouchState::TOUCHSTATE_VALID_COBCONON_SECOND) {
            mApp->ClearSecondPlayer();
            mGamepadControls2->mPlayerIndex2 = -1;
        }
        gTouchStateSecond = TouchState::TOUCHSTATE_NONE;
        gSendKeyWhenTouchUpSecond = false;
    }

    if (gPlayerIndexSecond == TouchPlayerIndex::TOUCHPLAYER_PLAYER1) {
        if (aGameState == 7 && gTouchLastYSecond > seedBankHeight && y <= seedBankHeight) {
            mGamepadControls1->mGamepadState = 1; // 退选植物
            if (!isTwoSeedBankMode)
                mGamepadControls1->mIsInShopSeedBank = true;
            int newSeedPacketIndex = mGamepadControls1->mSelectedSeedIndex;
            SeedBank *seedBank = mGamepadControls1->GetSeedBank();
            seedBank->mSeedPackets[newSeedPacketIndex].mLastSelectedTime = 0.0f; // 动画效果专用
            gTouchStateSecond = TouchState::TOUCHSTATE_NONE;
            gSendKeyWhenTouchUpSecond = false;
        }
    } else {
        if (aGameState_2P == 7 && gTouchLastYSecond > seedBankHeight && y <= seedBankHeight) {
            mGamepadControls2->mGamepadState = 1; // 退选植物
            if (!isTwoSeedBankMode)
                mGamepadControls2->mIsInShopSeedBank = true;
            int newSeedPacketIndex_2P = mGamepadControls2->mSelectedSeedIndex;
            SeedBank *seedBank_2P = mGamepadControls2->GetSeedBank();
            seedBank_2P->mSeedPackets[newSeedPacketIndex_2P].mLastSelectedTime = 0.0f; // 动画效果专用
            gTouchStateSecond = TouchState::TOUCHSTATE_NONE;
            gSendKeyWhenTouchUpSecond = false;
        }
    }


    if (aGameMode == GameMode::GAMEMODE_CHALLENGE_HEAVY_WEAPON && gTouchStateSecond == TouchState::TOUCHSTATE_HEAVY_WEAPON) {
        mChallenge->mHeavyWeaponX = mHeavyWeaponX + x - gTouchDownXSecond; // 移动重型武器X坐标
        return;
    }

    if (aGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST) {
        return;
    }

    if (gTouchStateSecond != TouchState::TOUCHSTATE_SEED_BANK) {
        if (x > 770)
            x = 770;
        if (x < 40)
            x = 40;
        if (y > 580)
            y = 580;
        if (gPlayerIndexSecond == TouchPlayerIndex::TOUCHPLAYER_PLAYER1) {
            mGamepadControls1->mCursorPositionX = x;
            mGamepadControls1->mCursorPositionY = y;
        } else {
            mGamepadControls2->mCursorPositionX = x;
            mGamepadControls2->mCursorPositionY = y;
        }
    }

    gTouchLastXSecond = x;
    gTouchLastYSecond = y;
}


void Board::MouseUpSecond(int x, int y, int theClickCount) {
    if (gTouchStateSecond != TouchState::TOUCHSTATE_NONE && gSendKeyWhenTouchUpSecond) {
        SeedBank *aSeedBank = mGamepadControls1->GetSeedBank();
        int aNumSeedsOnConveyor = aSeedBank->GetNumSeedsOnConveyorBelt();
        int aCurrentSeedBankIndex = mGamepadControls1->mSelectedSeedIndex;
        int aGameState = mGamepadControls1->mGamepadState;
        bool aIsCobCannonSelected = mGamepadControls1->mIsCobCannonSelected;

        SeedBank *aSeedBank_2P = mGamepadControls2->GetSeedBank();
        int aNumSeedsOnConveyor_2P = aSeedBank_2P->GetNumSeedsOnConveyorBelt();
        int aCurrentSeedBankIndex_2P = mGamepadControls2->mSelectedSeedIndex;
        int aGameState_2P = mGamepadControls2->mGamepadState;
        bool aIsCobCannonSelected_2P = mGamepadControls2->mIsCobCannonSelected;

        GameMode aGameMode = mApp->mGameMode;
        CursorType aCursorType = mCursorObject1->mCursorType;
        CursorType aCursorType_2P = mCursorObject2->mCursorType;
        ChallengeState aChallengeState = mChallenge->mChallengeState;
        GameScenes aGameScene = mApp->mGameScene;

        if (gPlayerIndexSecond == TouchPlayerIndex::TOUCHPLAYER_PLAYER1) {
            if (requestDrawShovelInCursor) {
                ShovelDown();
            } else if (aGameState == 7 || aIsCobCannonSelected || aCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_USABLE_COIN) {

                if (aGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED || aGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST) {
                    mGamepadControls1->OnKeyDown(KeyCode::KEYCODE_ESCAPE, 1096);
                    mGamepadControls1->OnKeyDown(KeyCode::KEYCODE_ACCEPT, 1096);
                    mGamepadControls1->OnKeyDown(KeyCode::KEYCODE_ACCEPT, 1096);
                    mGamepadControls1->OnKeyDown(KeyCode::KEYCODE_ESCAPE, 1096);
                } else if ((aGameMode == GameMode::GAMEMODE_CHALLENGE_LAST_STAND && aChallengeState == ChallengeState::STATECHALLENGE_NORMAL && aGameScene == GameScenes::SCENE_PLAYING)
                           || aGameMode == GameMode::GAMEMODE_MP_VS) {
                    mGamepadControls1->OnButtonDown(ButtonCode::BUTTONCODE_A, 0, 0);
                } else {
                    mGamepadControls1->OnKeyDown(KeyCode::KEYCODE_ACCEPT, 1096);
                }
                int mGameStateNew = mGamepadControls1->mGamepadState;
                int numSeedsInBankNew = aSeedBank->GetNumSeedsOnConveyorBelt();
                int seedPacketIndexNew = mGamepadControls1->mSelectedSeedIndex;
                mGamepadControls1->mIsInShopSeedBank = mGameStateNew != 7;
                if (aGameState != mGameStateNew) {
                    if (!HasConveyorBeltSeedBank(0) || aNumSeedsOnConveyor == numSeedsInBankNew) { // 修复传送带关卡种植之后SeedBank动画不正常
                        SeedBank *seedBank = mGamepadControls1->GetSeedBank();
                        seedBank->mSeedPackets[seedPacketIndexNew].mLastSelectedTime = 0.0f; // 动画效果专用
                    }
                }
            }
        } else {
            if (requestDrawButterInCursor) {
                requestDrawButterInCursor = false;
            } else if (aGameState_2P == 7 || aIsCobCannonSelected_2P || aCursorType_2P == CursorType::CURSOR_TYPE_PLANT_FROM_USABLE_COIN) {
                if (aGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED || aGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST) {
                    mGamepadControls2->OnKeyDown(KeyCode::KEYCODE_ESCAPE, 1096);
                    mGamepadControls2->OnKeyDown(KeyCode::KEYCODE_ACCEPT, 1096);
                    mGamepadControls2->OnKeyDown(KeyCode::KEYCODE_ACCEPT, 1096);
                    mGamepadControls2->OnKeyDown(KeyCode::KEYCODE_ESCAPE, 1096);
                } else if ((aGameMode == GameMode::GAMEMODE_CHALLENGE_LAST_STAND && aChallengeState == ChallengeState::STATECHALLENGE_NORMAL && aGameScene == GameScenes::SCENE_PLAYING)
                           || aGameMode == GameMode::GAMEMODE_MP_VS) {
                    mGamepadControls2->OnButtonDown(ButtonCode::BUTTONCODE_A, 1, 0);
                } else {
                    mGamepadControls2->OnKeyDown(KeyCode::KEYCODE_ACCEPT, 1096);
                }
                int mGameStateNew_2P = mGamepadControls2->mGamepadState;
                int numSeedsInBankNew_2P = aSeedBank_2P->GetNumSeedsOnConveyorBelt();
                int seedPacketIndexNew_2P = mGamepadControls2->mSelectedSeedIndex;
                mGamepadControls2->mIsInShopSeedBank = mGameStateNew_2P != 7;
                if (aGameState_2P != mGameStateNew_2P) {
                    if (!HasConveyorBeltSeedBank(1) || aNumSeedsOnConveyor_2P == numSeedsInBankNew_2P) { // 修复传送带关卡种植之后SeedBank动画不正常
                        SeedBank *seedBank_2P = mGamepadControls2->GetSeedBank();
                        seedBank_2P->mSeedPackets[seedPacketIndexNew_2P].mLastSelectedTime = 0.0f; // 动画效果专用
                    }
                }
                if (aGameMode == GameMode::GAMEMODE_MP_VS) {
                    mGamepadControls2->mGamepadState = 1;
                }
                if (gTouchStateSecond == TouchState::TOUCHSTATE_VALID_COBCONON_SECOND) {
                    mApp->ClearSecondPlayer();
                    mGamepadControls2->mPlayerIndex2 = -1;
                }
            }
        }
    }
    gPlayerIndexSecond = TouchPlayerIndex::TOUCHPLAYER_NONE;
    gSendKeyWhenTouchUpSecond = false;
    gTouchStateSecond = TouchState::TOUCHSTATE_NONE;
}


void Board_startLevel(Board *board) {
    old_Board_startLevel(board);
}


void Board::RemovedFromManager(WidgetManager *theManager) {
    RemoveWidget(gBoardMenuButton);
    RemoveWidget(gBoardStoreButton);
    gBoardMenuButton->Destroy();
//    operator delete (gBoardMenuButton);
    gBoardStoreButton->Destroy();
//    operator delete (gBoardStoreButton);
    gBoardMenuButton = nullptr;
    gBoardStoreButton = nullptr;

    old_Board_RemovedFromManager(this, theManager);
}

void Board_UpdateButtons(Board *board) {
    if (board->mApp->mGameMode == GameMode::GAMEMODE_MP_VS) {
        gBoardMenuButton->mBtnNoDraw = false;
        gBoardMenuButton->mDisabled = false;
    } else {
        if (board->mApp->mGameScene == GameScenes::SCENE_PLAYING) {
            gBoardMenuButton->mBtnNoDraw = false;
            gBoardMenuButton->mDisabled = false;
        } else {
            gBoardMenuButton->mBtnNoDraw = true;
            gBoardMenuButton->mDisabled = true;
        }
    }
    if (board->mBoardFadeOutCounter > 0) {
        gBoardMenuButton->mBtnNoDraw = true;
        gBoardMenuButton->mDisabled = true;
    }
}


void Board::ButtonDepress(int theId) {
    if (theId == 1000) {
        LawnApp *lawnApp = (LawnApp *)*gLawnApp_Addr;
        if (lawnApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN || lawnApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM) {
            lawnApp->DoBackToMain();
            return;
        }
        LawnApp_PlaySample(lawnApp, *Sexy_SOUND_PAUSE_Addr);
        lawnApp->DoNewOptions(false, 0);
        return;
    } else if (theId == 1001) {
        LawnApp *lawnApp = (LawnApp *)*gLawnApp_Addr;
        if (lawnApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_LAST_STAND) {
            Board *mBoard = lawnApp->mBoard;
            mBoard->mChallenge->mChallengeState = ChallengeState::STATECHALLENGE_LAST_STAND_ONSLAUGHT;
            gBoardStoreButton->mBtnNoDraw = true;
            gBoardStoreButton->mDisabled = true;
            gBoardStoreButton->Resize(0, 0, 0, 0);
            mBoard->mZombieCountDown = 9;
            mBoard->mZombieCountDownStart = mBoard->mZombieCountDown;
        } else if (lawnApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN) {
            lawnApp->mZenGarden->OpenStore();
        } else if (lawnApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM) {
            lawnApp->mBoard->mChallenge->TreeOfWisdomOpenStore();
        }
    }
    old_Board_ButtonDepress(this, theId);
}

Image *GetIconByAchievementId(AchievementId theAchievementId) {
    switch (theAchievementId) {
        case AchievementId::ACHIEVEMENT_HOME_SECURITY:
            return addonImages.achievement_homeLawnsecurity;
        case AchievementId::ACHIEVEMENT_MORTICULTURALIST:
            return addonImages.achievement_morticulturalist;
        case AchievementId::ACHIEVEMENT_IMMORTAL:
            return addonImages.achievement_immortal;
        case AchievementId::ACHIEVEMENT_SOILPLANTS:
            return addonImages.achievement_soilplants;
        case AchievementId::ACHIEVEMENT_CLOSESHAVE:
            return addonImages.achievement_closeshave;
        case AchievementId::ACHIEVEMENT_CHOMP:
            return addonImages.achievement_chomp;
        case AchievementId::ACHIEVEMENT_VERSUS:
            return addonImages.achievement_versusz;
        case AchievementId::ACHIEVEMENT_GARG:
            return addonImages.achievement_garg;
        case AchievementId::ACHIEVEMENT_COOP:
            return addonImages.achievement_coop;
        case AchievementId::ACHIEVEMENT_SHOP:
            return addonImages.achievement_shop;
        case AchievementId::ACHIEVEMENT_EXPLODONATOR:
            return addonImages.achievement_explodonator;
        case AchievementId::ACHIEVEMENT_TREE:
            return addonImages.achievement_tree;
        default:
            return nullptr;
    }
}

const char *GetNameByAchievementId(AchievementId theAchievementId) {
    switch (theAchievementId) {
        case AchievementId::ACHIEVEMENT_HOME_SECURITY:
            return "ACHIEVEMENT_HOME_SECURITY";
        case AchievementId::ACHIEVEMENT_MORTICULTURALIST:
            return "ACHIEVEMENT_MORTICULTURALIST";
        case AchievementId::ACHIEVEMENT_IMMORTAL:
            return "ACHIEVEMENT_IMMORTAL";
        case AchievementId::ACHIEVEMENT_SOILPLANTS:
            return "ACHIEVEMENT_SOILPLANTS";
        case AchievementId::ACHIEVEMENT_CLOSESHAVE:
            return "ACHIEVEMENT_CLOSESHAVE";
        case AchievementId::ACHIEVEMENT_CHOMP:
            return "ACHIEVEMENT_CHOMP";
        case AchievementId::ACHIEVEMENT_VERSUS:
            return "ACHIEVEMENT_VERSUS";
        case AchievementId::ACHIEVEMENT_GARG:
            return "ACHIEVEMENT_GARG";
        case AchievementId::ACHIEVEMENT_COOP:
            return "ACHIEVEMENT_COOP";
        case AchievementId::ACHIEVEMENT_SHOP:
            return "ACHIEVEMENT_SHOP";
        case AchievementId::ACHIEVEMENT_EXPLODONATOR:
            return "ACHIEVEMENT_EXPLODONATOR";
        case AchievementId::ACHIEVEMENT_TREE:
            return "ACHIEVEMENT_TREE";
        default:
            return "";
    }
}

bool Board::GrantAchievement(AchievementId theAchievementId, bool theIsShow) {
    LawnApp *lawnApp = mApp;
    PlayerInfo *playerInfo = lawnApp->mPlayerInfo;
    if (!playerInfo->mAchievements[theAchievementId]) {
        LawnApp_PlaySample(lawnApp, addonSounds.achievement);
        ClearAdviceImmediately();
        const char *theAchievementName = GetNameByAchievementId(theAchievementId);
        int holder[1];
        int holder1[1];
        int holder2[1];
        TodStringTranslate(holder, "[ACHIEVEMENT_GRANTED]");
        StrFormat(holder1, "[%s]", theAchievementName);
        TodReplaceString(holder2, holder, "{achievement}", holder1);
        DisplayAdviceAgain(_S("[ACHIEVEMENT_GRANTED]"), MessageStyle::MESSAGE_STYLE_ACHIEVEMENT, AdviceType::ADVICE_NEED_ACHIVEMENT_EARNED);
        ((CustomMessageWidget *)mAdvice)->mIcon = GetIconByAchievementId(theAchievementId);
        StringDelete(holder);
        StringDelete(holder1);
        StringDelete(holder2);
        playerInfo->mAchievements[theAchievementId] = true;
        return true;
    }
    return false;
}

void Board::FadeOutLevel() {
    old_Board_FadeOutLevel(this);

    if (mApp->IsSurvivalMode() && mChallenge->mSurvivalStage >= 19) {
        GrantAchievement(AchievementId::ACHIEVEMENT_IMMORTAL, true);
    }

    if (!mApp->IsSurvivalMode()) {
        int theNumLawnMowers = 0;
        for (int i = 0; i < 6; ++i) {
            if (mPlantRow[i] != PlantRowType::PLANTROW_DIRT) {
                theNumLawnMowers++;
            }
        }
        if (mTriggeredLawnMowers == theNumLawnMowers) {
            GrantAchievement(AchievementId::ACHIEVEMENT_CLOSESHAVE, true);
        }
    }
    if (mLevel == 50) {
        GrantAchievement(AchievementId::ACHIEVEMENT_HOME_SECURITY, true);
    }

    if (mApp->mGameMode == GameMode::GAMEMODE_TWO_PLAYER_COOP_BOWLING) {
        GrantAchievement(AchievementId::ACHIEVEMENT_COOP, true);
    }

    if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS) {
        if ((VSResultsMenu_msPlayerRecords_Addr[3] == 4 && mApp->mBoardResult == BoardResult::BOARDRESULT_VS_PLANT_WON)
            || (VSResultsMenu_msPlayerRecords_Addr[8] == 4 && mApp->mBoardResult == BoardResult::BOARDRESULT_VS_ZOMBIE_WON)) {
            GrantAchievement(AchievementId::ACHIEVEMENT_VERSUS, true);
        }
    }

    if (mNewWallNutAndSunFlowerAndChomperOnly && !mApp->IsSurvivalMode() && !HasConveyorBeltSeedBank(0)) {
        int num = mSeedBank1->mNumPackets;
        for (int i = 0; i < num; ++i) {
            SeedType theType = mSeedBank1->mSeedPackets[i].mPacketType;
            if (theType == SeedType::SEED_CHOMPER || theType == SeedType::SEED_WALLNUT || theType == SeedType::SEED_SUNFLOWER) {
                GrantAchievement(AchievementId::ACHIEVEMENT_CHOMP, true);
                break;
            }
        }
    }
}

void Board::DoPlantingAchievementCheck(SeedType theSeedType) {
    if (theSeedType != SeedType::SEED_CHOMPER && theSeedType != SeedType::SEED_SUNFLOWER && theSeedType != SeedType::SEED_WALLNUT) {
        mNewWallNutAndSunFlowerAndChomperOnly = false;
    }
    if (theSeedType == SeedType::SEED_PEASHOOTER && !HasConveyorBeltSeedBank(0)) {
        mNewPeaShooterCount++;
        if (mNewPeaShooterCount >= 10) {
            GrantAchievement(AchievementId::ACHIEVEMENT_SOILPLANTS, true);
        }
    }
}

void Board::DrawUITop(Sexy::Graphics *g) {
    if (seedBankPin && !mApp->IsSlotMachineLevel()) {
        if (mApp->mGameScene != GameScenes::SCENE_LEADER_BOARD && mApp->mGameScene != GameScenes::SCENE_ZOMBIES_WON) {
            if (mSeedBank1->BeginDraw(g)) {
                mSeedBank1->SeedBank::Draw(g);
                mSeedBank1->EndDraw(g);
            }

            if (mSeedBank2 != nullptr) {
                if (mSeedBank2->BeginDraw(g)) {
                    mSeedBank2->SeedBank::Draw(g);
                    mSeedBank2->EndDraw(g);
                }
            }
        }
    }

    old_Board_DrawUITop(this, g);
}

int Board::GetSeedBankExtraWidth() {
    int aNumPackets = mSeedBank1->mNumPackets;
    return aNumPackets <= 6 ? 0 : aNumPackets == 7 ? 60 : aNumPackets == 8 ? 76 : aNumPackets == 9 ? 112 : 153;
}

Rect Board::GetShovelButtonRect() {
//    Rect aRect(GetSeedBankExtraWidth() + 456, 0, Sexy::IMAGE_SHOVELBANK->GetWidth(), Sexy::IMAGE_SEEDBANK->GetHeight());
//    if (mApp->IsSlotMachineLevel() || mApp->IsSquirrelLevel())
//    {
//        aRect.mX = 600;
//    }
//    return aRect;

    return old_Board_GetShovelButtonRect(this);
}

void Board::DrawBackdrop(Sexy::Graphics *g) {
    // 实现泳池动态效果、实现对战结盟分界线
    old_Board_DrawBackdrop(this, g);

    //    if (mBackground == BackgroundType::BACKGROUND_3_POOL || mBackground == BackgroundType::BACKGROUND_4_FOG) {
    //        PoolEffect_PoolEffectDraw(this->mApp->mPoolEffect, g, Board_StageIsNight(this));
    //    }

    GameMode mGameMode = mApp->mGameMode;
    if (mGameMode == GameMode::GAMEMODE_MP_VS) {
        switch (mBackground) {
            case BackgroundType::BACKGROUND_1_DAY:
                g->DrawImage(*Sexy_IMAGE_WALLNUT_BOWLINGSTRIPE_Addr, 512, 73);
                break;
            case BackgroundType::BACKGROUND_2_NIGHT:
                g->DrawImage(*Sexy_IMAGE_WALLNUT_BOWLINGSTRIPE_Addr, 512, 73);
                break;
            case BackgroundType::BACKGROUND_3_POOL:
                g->DrawImage(*Sexy_IMAGE_WALLNUT_BOWLINGSTRIPE_Addr, 512, 73);
                break;
            case BackgroundType::BACKGROUND_4_FOG:
                g->DrawImage(*Sexy_IMAGE_WALLNUT_BOWLINGSTRIPE_Addr, 512, 73);
                break;
            case BackgroundType::BACKGROUND_5_ROOF:
                g->DrawImage(*Sexy_IMAGE_WALLNUT_BOWLINGSTRIPE_Addr, 512, 73);
                break;
            case BackgroundType::BACKGROUND_6_BOSS:
                g->DrawImage(*Sexy_IMAGE_WALLNUT_BOWLINGSTRIPE_Addr, 512, 73);
                break;
            default:
                break;
        }
        return;
    }
    if (mGameMode >= GameMode::GAMEMODE_TWO_PLAYER_COOP_DAY && mGameMode <= GameMode::GAMEMODE_TWO_PLAYER_COOP_ENDLESS && mGameMode != GameMode::GAMEMODE_TWO_PLAYER_COOP_BOWLING) {
        switch (mBackground) {
            case BackgroundType::BACKGROUND_1_DAY:
                g->DrawImage(addonImages.stripe_day_coop, 384, 69);
                break;
            case BackgroundType::BACKGROUND_2_NIGHT:
                g->DrawImage(addonImages.stripe_day_coop, 384, 69);
                break;
            case BackgroundType::BACKGROUND_3_POOL:
                g->DrawImage(addonImages.stripe_pool_coop, 348, 72);
                break;
            case BackgroundType::BACKGROUND_4_FOG:
                g->DrawImage(addonImages.stripe_pool_coop, 348, 72);
                break;
            case BackgroundType::BACKGROUND_5_ROOF:
                g->DrawImage(addonImages.stripe_roof_left, 365, 82);
                break;
            case BackgroundType::BACKGROUND_6_BOSS:
                g->DrawImage(addonImages.stripe_roof_left, 365, 82);
                break;
            default:
                break;
        }
    }
}

bool Board::RowCanHaveZombieType(int theRow, ZombieType theZombieType) {
    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_POOL_PARTY) {
        return Zombie::ZombieTypeCanGoInPool(theZombieType);
    }

    return old_Board_RowCanHaveZombieType(this, theRow, theZombieType);
}

void Board::ShakeBoard(int theShakeAmountX, int theShakeAmountY) {
    // 添加 手机震动效果
    old_Board_ShakeBoard(this, theShakeAmountX, theShakeAmountY);

    if (mApp->mPlayerInfo->mIsVibrateClosed) {
        return;
    }

    Native::BridgeApp *bridgeApp = Native_BridgeApp_getSingleton();
    JNIEnv *env = Native_BridgeApp_getJNIEnv(bridgeApp);
    jobject activity = Native_NativeApp_getActivity(bridgeApp->mNativeApp);
    jclass cls = env->GetObjectClass(activity);
    jmethodID methodID = env->GetMethodID(cls, "vibrate", "(I)V");
    //    env->CallVoidMethod(activity, methodID, 120);
    env->CallVoidMethod(activity, methodID, (abs(theShakeAmountX) + abs(theShakeAmountY)) * 50);
    env->DeleteLocalRef(cls);
}

int Board::GetNumSeedsInBank(bool thePlayerIndex) {
    return old_Board_GetNumSeedsInBank(this, thePlayerIndex);
}

void Board::RemoveParticleByType(ParticleEffect theEffectType) {
    TodParticleSystem *aParticle = nullptr;
    while (IterateParticles(aParticle)) {
        if (aParticle->mEffectType == theEffectType) {
            TodParticleSystem_ParticleSystemDie(aParticle);
        }
    }
}

GridItem *Board::GetGridItemAt(GridItemType theGridItemType, int theGridX, int theGridY) {
    GridItem *aGridItem = nullptr;
    while (IterateGridItems(aGridItem)) {
        if (aGridItem->mGridX == theGridX && aGridItem->mGridY == theGridY && aGridItem->mGridItemType == theGridItemType) {
            return aGridItem;
        }
    }
    return nullptr;
}

GridItem* Board::GetLadderAt(int theGridX, int theGridY) {
    return GetGridItemAt(GridItemType::GRIDITEM_LADDER, theGridX, theGridY);
}

GridItem *Board::GetScaryPotAt(int theGridX, int theGridY) {
    return GetGridItemAt(GridItemType::GRIDITEM_SCARY_POT, theGridX, theGridY);
}

int Board::PixelToGridXKeepOnBoard(int theX, int theY) {
    int aGridX = PixelToGridX(theX, theY);
    return max(aGridX, 0);
}

int Board::GridToPixelX(int theGridX, int theGridY) {
    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN) {
        if (mBackground == BackgroundType::BACKGROUND_GREENHOUSE || mBackground == BackgroundType::BACKGROUND_MUSHROOM_GARDEN || mBackground == BackgroundType::BACKGROUND_ZOMBIQUARIUM) {
            return mApp->mZenGarden->GridToPixelX(theGridX, theGridY);
        }
    }

    return theGridX * 80 + LAWN_XMIN;
}

int Board::PixelToGridYKeepOnBoard(int theX, int theY) {
    int aGridY = PixelToGridY(max(theX, 80), theY);
    return max(aGridY, 0);
}

int Board::GridToPixelY(int theGridX, int theGridY) {
    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN) {
        if (mBackground == BackgroundType::BACKGROUND_GREENHOUSE || mBackground == BackgroundType::BACKGROUND_MUSHROOM_GARDEN || mBackground == BackgroundType::BACKGROUND_ZOMBIQUARIUM) {
            return mApp->mZenGarden->GridToPixelY(theGridX, theGridY);
        }
    }

    int aY;
    if (StageHasRoof()) {
        int aSlopeOffset;
        if (theGridX < 5) {
            aSlopeOffset = (5 - theGridX) * 20;
        } else {
            aSlopeOffset = 0;
        }
        aY = theGridY * 85 + aSlopeOffset + LAWN_YMIN - 10;
    } else if (StageHasPool()) {
        aY = theGridY * 85 + LAWN_YMIN;
    } else {
        aY = theGridY * 100 + LAWN_YMIN;
    }

    if (theGridX != -1 && mGridSquareType[theGridX][theGridY] == GridSquareType::GRIDSQUARE_HIGH_GROUND) {
        aY -= HIGH_GROUND_HEIGHT;
    }

    return aY;
}

int GetRectOverlap(const Rect &rect1, const Rect &rect2) {
    int xmax, rmin, rmax;

    if (rect1.mX < rect2.mX) {
        rmin = rect1.mX + rect1.mWidth;
        rmax = rect2.mX + rect2.mWidth;
        xmax = rect2.mX;
    } else {
        rmin = rect2.mX + rect2.mWidth;
        rmax = rect1.mX + rect1.mWidth;
        xmax = rect1.mX;
    }

    if (rmin > xmax && rmin > rmax) {
        rmin = rmax;
    }

    return rmin - xmax;
}

bool GetCircleRectOverlap(int theCircleX, int theCircleY, int theRadius, const Rect &theRect) {
    int dx = 0;        // 圆心与矩形较近一条纵边的横向距离
    int dy = 0;        // 圆心与矩形较近一条横边的纵向距离
    bool xOut = false; // 圆心横坐标是否不在矩形范围内
    bool yOut = false; // 圆心纵坐标是否不在矩形范围内

    if (theCircleX < theRect.mX) {
        xOut = true;
        dx = theRect.mX - theCircleX;
    } else if (theCircleX > theRect.mX + theRect.mWidth) {
        xOut = true;
        dx = theCircleX - theRect.mX - theRect.mWidth;
    }
    if (theCircleY < theRect.mY) {
        yOut = true;
        dy = theRect.mY - theCircleY;
    } else if (theCircleY > theRect.mY + theRect.mHeight) {
        yOut = true;
        dy = theCircleY - theRect.mY - theRect.mHeight;
    }

    if (!xOut && !yOut) // 如果圆心在矩形内
    {
        return true;
    } else if (xOut && yOut) {
        return dx * dx + dy * dy <= theRadius * theRadius;
    } else if (xOut) {
        return dx <= theRadius;
    } else {
        return dy <= theRadius;
    }
}

int Board::MakeRenderOrder(RenderLayer theRenderLayer, int theRow, int theLayerOffset) {
    return theRow * (int)RenderLayer::RENDER_LAYER_ROW_OFFSET + theRenderLayer + theLayerOffset;
}

void FixBoardAfterLoad(Board* theBoard) {
    // 修复读档后的各种问题
    old_FixBoardAfterLoad(theBoard);
    theBoard->FixReanimErrorAfterLoad();
}

void Board::FixReanimErrorAfterLoad() {
    // 修复读档后的各种问题
    Zombie *aZombie = nullptr;
    while (IterateZombies(aZombie)) {
        ZombieType zombieType = aZombie->mZombieType;
        Reanimation *mBodyReanim = mApp->ReanimationGet(aZombie->mBodyReanimID);
        if (mBodyReanim == nullptr)
            return;

        if (!aZombie->mHasArm) {
            aZombie->SetupLostArmReanim();
        }
        // 修复读档后豌豆、机枪、倭瓜僵尸头部变为普通僵尸
        if (zombieType == ZombieType::ZOMBIE_PEA_HEAD || zombieType == ZombieType::ZOMBIE_GATLING_HEAD || zombieType == ZombieType::ZOMBIE_SQUASH_HEAD) {
            mBodyReanim->SetImageOverride("anim_head1", *IMAGE_BLANK);
        }

        // 修复读档后盾牌贴图变为满血盾牌贴图、垃圾桶变为铁门
        if (aZombie->mShieldType != ShieldType::SHIELDTYPE_NONE) {
            int shieldDamageIndex = aZombie->GetShieldDamageIndex();
            switch (aZombie->mShieldType) {
                case ShieldType::SHIELDTYPE_DOOR:
                    switch (shieldDamageIndex) {
                        case 1:
                            mBodyReanim->SetImageOverride("anim_screendoor", *Sexy_IMAGE_REANIM_ZOMBIE_SCREENDOOR2_Addr);
                            break;
                        case 2:
                            mBodyReanim->SetImageOverride("anim_screendoor", *Sexy_IMAGE_REANIM_ZOMBIE_SCREENDOOR3_Addr);
                            break;
                    }
                    break;
                case ShieldType::SHIELDTYPE_NEWSPAPER:
                    switch (shieldDamageIndex) {
                        case 1:
                            mBodyReanim->SetImageOverride("Zombie_paper_paper", *Sexy_IMAGE_REANIM_ZOMBIE_PAPER_PAPER2_Addr);
                            break;
                        case 2:
                            mBodyReanim->SetImageOverride("Zombie_paper_paper", *Sexy_IMAGE_REANIM_ZOMBIE_PAPER_PAPER3_Addr);
                            break;
                    }
                    break;
                case ShieldType::SHIELDTYPE_LADDER:
                    switch (shieldDamageIndex) {
                        case 1:
                            mBodyReanim->SetImageOverride("Zombie_ladder_1", *Sexy_IMAGE_REANIM_ZOMBIE_LADDER_1_DAMAGE1_Addr);
                            break;
                        case 2:
                            mBodyReanim->SetImageOverride("Zombie_ladder_1", *Sexy_IMAGE_REANIM_ZOMBIE_LADDER_1_DAMAGE2_Addr);
                            break;
                    }
                    break;
                case ShieldType::SHIELDTYPE_TRASH_BIN:
                    switch (shieldDamageIndex) {
                        case 0:
                            mBodyReanim->SetImageOverride("anim_screendoor", *Sexy_IMAGE_REANIM_ZOMBIE_TRASHCAN1_Addr);
                            break;
                        case 1:
                            mBodyReanim->SetImageOverride("anim_screendoor", *Sexy_IMAGE_REANIM_ZOMBIE_TRASHCAN2_Addr);
                            break;
                        case 2:
                            mBodyReanim->SetImageOverride("anim_screendoor", *Sexy_IMAGE_REANIM_ZOMBIE_TRASHCAN3_Addr);
                            break;
                    }
                    break;
            }
        }

        // 修复读档后头盔贴图变为满血头盔贴图
        if (aZombie->mHelmType != HelmType::HELMTYPE_NONE) {
            int helmDamageIndex = aZombie->GetHelmDamageIndex();
            switch (aZombie->mHelmType) {
                case HelmType::HELMTYPE_TRAFFIC_CONE:
                    switch (helmDamageIndex) {
                        case 1:
                            mBodyReanim->SetImageOverride("anim_cone", *Sexy_IMAGE_REANIM_ZOMBIE_CONE2_Addr);
                            break;
                        case 2:
                            mBodyReanim->SetImageOverride("anim_cone", *Sexy_IMAGE_REANIM_ZOMBIE_CONE3_Addr);
                            break;
                    }
                    break;
                case HelmType::HELMTYPE_PAIL:
                    switch (helmDamageIndex) {
                        case 1:
                            mBodyReanim->SetImageOverride("anim_bucket", *Sexy_IMAGE_REANIM_ZOMBIE_BUCKET2_Addr);
                            break;
                        case 2:
                            mBodyReanim->SetImageOverride("anim_bucket", *Sexy_IMAGE_REANIM_ZOMBIE_BUCKET3_Addr);
                            break;
                    }
                    break;
                case HelmType::HELMTYPE_FOOTBALL:
                    switch (helmDamageIndex) {
                        case 1:
                            mBodyReanim->SetImageOverride("zombie_football_helmet", *Sexy_IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET2_Addr);
                            break;
                        case 2:
                            mBodyReanim->SetImageOverride("zombie_football_helmet", *Sexy_IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET3_Addr);
                            break;
                    }
                    break;
                case HelmType::HELMTYPE_DIGGER:
                    switch (helmDamageIndex) {
                        case 1:
                            mBodyReanim->SetImageOverride("Zombie_digger_hardhat", *Sexy_IMAGE_REANIM_ZOMBIE_DIGGER_HARDHAT2_Addr);
                            break;
                        case 2:
                            mBodyReanim->SetImageOverride("Zombie_digger_hardhat", *Sexy_IMAGE_REANIM_ZOMBIE_DIGGER_HARDHAT3_Addr);
                            break;
                    }
                    break;
                case HelmType::HELMTYPE_WALLNUT: {
                    Reanimation *mSpecialHeadReanim = mApp->ReanimationGet(aZombie->mSpecialHeadReanimID);
                    switch (helmDamageIndex) {
                        case 1:
                            mSpecialHeadReanim->SetImageOverride("anim_face", *Sexy_IMAGE_REANIM_WALLNUT_CRACKED1_Addr);
                            break;
                        case 2:
                            mSpecialHeadReanim->SetImageOverride("anim_face", *Sexy_IMAGE_REANIM_WALLNUT_CRACKED2_Addr);
                            break;
                    }
                } break;
                case HelmType::HELMTYPE_TALLNUT: {
                    Reanimation *mSpecialHeadReanim = mApp->ReanimationGet(aZombie->mSpecialHeadReanimID);
                    switch (helmDamageIndex) {
                        case 1:
                            mSpecialHeadReanim->SetImageOverride("anim_face", *Sexy_IMAGE_REANIM_TALLNUT_CRACKED1_Addr);
                            break;
                        case 2:
                            mSpecialHeadReanim->SetImageOverride("anim_face", *Sexy_IMAGE_REANIM_TALLNUT_CRACKED2_Addr);
                            break;
                    }
                } break;
            }
        }

        // 修复读档后巨人僵尸创可贴消失、红眼巨人变白眼巨人
        if (zombieType == ZombieType::ZOMBIE_GARGANTUAR || zombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR) {
            int bodyDamageIndex = aZombie->GetBodyDamageIndex();
            switch (bodyDamageIndex) {
                case 0:
                    if (zombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR)
                        mBodyReanim->SetImageOverride("anim_head1", *Sexy_IMAGE_REANIM_ZOMBIE_GARGANTUAR_HEAD_REDEYE_Addr);
                    break;
                case 1:
                    if (zombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR)
                        mBodyReanim->SetImageOverride("anim_head1", *Sexy_IMAGE_REANIM_ZOMBIE_GARGANTUAR_HEAD_REDEYE_Addr);
                    mBodyReanim->SetImageOverride("Zombie_gargantua_body1", *Sexy_IMAGE_REANIM_ZOMBIE_GARGANTUAR_BODY1_2_Addr);
                    mBodyReanim->SetImageOverride("Zombie_gargantuar_outerarm_lower", *Sexy_IMAGE_REANIM_ZOMBIE_GARGANTUAR_OUTERARM_LOWER2_Addr);
                    break;
                case 2:
                    mBodyReanim->SetImageOverride("Zombie_gargantua_body1", *Sexy_IMAGE_REANIM_ZOMBIE_GARGANTUAR_BODY1_3_Addr);
                    mBodyReanim->SetImageOverride("Zombie_gargantuar_outerleg_foot", *Sexy_IMAGE_REANIM_ZOMBIE_GARGANTUAR_FOOT2_Addr);
                    mBodyReanim->SetImageOverride("Zombie_gargantuar_outerarm_lower", *Sexy_IMAGE_REANIM_ZOMBIE_GARGANTUAR_OUTERARM_LOWER2_Addr);
                    mBodyReanim->SetImageOverride(
                        "anim_head1", zombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR ? *Sexy_IMAGE_REANIM_ZOMBIE_GARGANTUAR_HEAD2_REDEYE_Addr : *Sexy_IMAGE_REANIM_ZOMBIE_GARGANTUAR_HEAD2_Addr);
            }
        }

        // 修复读档后僵尸博士机甲变全新机甲
        if (zombieType == ZombieType::ZOMBIE_BOSS) {
            int bodyDamageIndex = aZombie->GetBodyDamageIndex();
            switch (bodyDamageIndex) {
                case 1:
                    mBodyReanim->SetImageOverride("Boss_head", *Sexy_IMAGE_REANIM_ZOMBIE_BOSS_HEAD_DAMAGE1_Addr);
                    mBodyReanim->SetImageOverride("Boss_jaw", *Sexy_IMAGE_REANIM_ZOMBIE_BOSS_JAW_DAMAGE1_Addr);
                    mBodyReanim->SetImageOverride("Boss_outerarm_hand", *Sexy_IMAGE_REANIM_ZOMBIE_BOSS_OUTERARM_HAND_DAMAGE1_Addr);
                    mBodyReanim->SetImageOverride("Boss_outerarm_thumb2", *Sexy_IMAGE_REANIM_ZOMBIE_BOSS_OUTERARM_THUMB_DAMAGE1_Addr);
                    mBodyReanim->SetImageOverride("Boss_innerleg_foot", *Sexy_IMAGE_REANIM_ZOMBIE_BOSS_FOOT_DAMAGE1_Addr);
                    break;
                case 2:
                    mBodyReanim->SetImageOverride("Boss_head", *Sexy_IMAGE_REANIM_ZOMBIE_BOSS_HEAD_DAMAGE2_Addr);
                    mBodyReanim->SetImageOverride("Boss_jaw", *Sexy_IMAGE_REANIM_ZOMBIE_BOSS_JAW_DAMAGE2_Addr);
                    mBodyReanim->SetImageOverride("Boss_outerarm_hand", *Sexy_IMAGE_REANIM_ZOMBIE_BOSS_OUTERARM_HAND_DAMAGE2_Addr);
                    mBodyReanim->SetImageOverride("Boss_outerarm_thumb2", *Sexy_IMAGE_REANIM_ZOMBIE_BOSS_OUTERARM_THUMB_DAMAGE2_Addr);
                    mBodyReanim->SetImageOverride("Boss_outerleg_foot", *Sexy_IMAGE_REANIM_ZOMBIE_BOSS_FOOT_DAMAGE2_Addr);
                    break;
            }
        }

        // 修复读档后冰车变全新冰车
        if (zombieType == ZombieType::ZOMBIE_ZAMBONI) {
            int bodyDamageIndex = aZombie->GetBodyDamageIndex();
            switch (bodyDamageIndex) {
                case 1:
                    mBodyReanim->SetImageOverride("Zombie_zamboni_1", *Sexy_IMAGE_REANIM_ZOMBIE_ZAMBONI_1_DAMAGE1_Addr);
                    mBodyReanim->SetImageOverride("Zombie_zamboni_2", *Sexy_IMAGE_REANIM_ZOMBIE_ZAMBONI_2_DAMAGE1_Addr);
                    break;
                case 2:
                    mBodyReanim->SetImageOverride("Zombie_zamboni_1", *Sexy_IMAGE_REANIM_ZOMBIE_ZAMBONI_1_DAMAGE2_Addr);
                    mBodyReanim->SetImageOverride("Zombie_zamboni_2", *Sexy_IMAGE_REANIM_ZOMBIE_ZAMBONI_2_DAMAGE2_Addr);
                    break;
            }
        }

        // 修复读档后投篮车变全新投篮车
        if (zombieType == ZombieType::ZOMBIE_CATAPULT) {
            int bodyDamageIndex = aZombie->GetBodyDamageIndex();
            switch (bodyDamageIndex) {
                case 1:
                case 2:
                    mBodyReanim->SetImageOverride("Zombie_catapult_siding", *Sexy_IMAGE_REANIM_ZOMBIE_CATAPULT_SIDING_DAMAGE_Addr);
                    break;
            }
        }
    }

    // 修复读档后雏菊、糖果变色、泳池闪光消失
    TodParticleSystem *aParticle = nullptr;
    while (IterateParticles(aParticle)) {
        if (aParticle->mEffectType == ParticleEffect::PARTICLE_ZOMBIE_DAISIES || aParticle->mEffectType == ParticleEffect::PARTICLE_ZOMBIE_PINATA) {
            // 设置颜色
            TodParticleSystem_OverrideColor(aParticle, nullptr, &white);
        } else if (aParticle->mEffectType == ParticleEffect::PARTICLE_POOL_SPARKLY) {
            // 直接删除泳池闪光特效
            TodParticleSystem_ParticleSystemDie(aParticle);
            mPoolSparklyParticleID = PARTICLESYSTEMID_NULL;
        }
    }

    if (mBackground == BackgroundType::BACKGROUND_3_POOL) {
        // 添加泳池闪光特效
        TodParticleSystem *poolSparklyParticle = mApp->AddTodParticle(450.0, 295.0, 220000, ParticleEffect::PARTICLE_POOL_SPARKLY);
        mPoolSparklyParticleID = mApp->ParticleGetID(poolSparklyParticle);
    }
}

bool Board::PlantUsesAcceleratedPricing(SeedType theSeedType)
{
    return Plant::IsUpgrade(theSeedType) && mApp->IsSurvivalEndless(mApp->mGameMode);
}

bool Board::IsPlantInCursor() {
    return
        mCursorObject1->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_BANK ||
        mCursorObject1->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_USABLE_COIN ||
        mCursorObject1->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_GLOVE ||
        mCursorObject1->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_DUPLICATOR ||
        mCursorObject1->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_WHEEL_BARROW;
}

void Board::RemoveAllPlants() {
    for ( Plant *aPlant = nullptr; IteratePlants(aPlant); aPlant->Die() );
}

void Board::RemoveAllZombies() {
    Zombie *aZombie = nullptr;
    while (IterateZombies(aZombie)) {
        if (!aZombie->IsDeadOrDying())
            aZombie->DieNoLoot();
    }
}

bool Board::IsValidCobCannonSpotHelper(int theGridX, int theGridY) {
    PlantsOnLawn aPlantOnLawn;
    GetPlantsOnLawn(theGridX, theGridY, &aPlantOnLawn);
    if (aPlantOnLawn.mPumpkinPlant)
        return false;

    if (aPlantOnLawn.mNormalPlant && aPlantOnLawn.mNormalPlant->mSeedType == SeedType::SEED_KERNELPULT)
        return true;

    return mApp->mEasyPlantingCheat && CanPlantAt(theGridX, theGridY, SeedType::SEED_KERNELPULT) == PlantingReason::PLANTING_OK;
}

bool Board::IsPoolSquare(int theGridX, int theGridY) {
    if (theGridX >= 0 && theGridY >= 0) {
        return mGridSquareType[theGridX][theGridY] == GridSquareType::GRIDSQUARE_POOL;
    }
    return false;
}

int Board::TotalZombiesHealthInWave(int theWaveIndex) {
    int aTotalHealth = 0;
    Zombie* aZombie = nullptr;
    while (IterateZombies(aZombie))
    {
        if (aZombie->mFromWave == theWaveIndex && !aZombie->mMindControlled && !aZombie->IsDeadOrDying() &&
            aZombie->mZombieType != ZombieType::ZOMBIE_BUNGEE && aZombie->mRelatedZombieID == ZombieID::ZOMBIEID_NULL)
        {
            aTotalHealth += aZombie->mBodyHealth + aZombie->mHelmHealth + aZombie->mShieldHealth * 0.2f + aZombie->mFlyingHealth;
        }
    }
    return aTotalHealth;
}

void Board::KillAllZombiesInRadius(int theRow, int theX, int theY, int theRadius, int theRowRange, bool theBurn, int theDamageRangeFlags) {
    Zombie* aZombie = nullptr;
    while (IterateZombies(aZombie))
    {
        if (aZombie->EffectedByDamage(theDamageRangeFlags))
        {
            Rect aZombieRect = aZombie->GetZombieRect();
            int aRowDist = aZombie->mRow - theRow;
            if (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS)
            {
                aRowDist = 0;
            }

            if (aRowDist <= theRowRange && aRowDist >= -theRowRange && GetCircleRectOverlap(theX, theY, theRadius, aZombieRect))
            {
                if (theBurn)
                {
                    aZombie->ApplyBurn();
                }
                else
                {
                    aZombie->TakeDamage(1800, 18U);
                }
            }
        }
    }

    int aGridX = PixelToGridXKeepOnBoard(theX, theY);
    int aGridY = PixelToGridYKeepOnBoard(theX, theY);
    GridItem* aGridItem = nullptr;
    while (IterateGridItems(aGridItem))
    {
        if (aGridItem->mGridItemType == GridItemType::GRIDITEM_LADDER)
        {
            if (GridInRange(aGridItem->mGridX, aGridItem->mGridY, aGridX, aGridY, theRowRange, theRowRange))
            {
                aGridItem->GridItemDie();
            }
        }
    }
}

void Board::RemoveCutsceneZombies() {
    Zombie* aZombie = nullptr;
    while (IterateZombies(aZombie))
    {
        if (aZombie->mFromWave == Zombie::ZOMBIE_WAVE_CUTSCENE)
        {
            aZombie->DieNoLoot();
        }
    }
}

int Board::CountZombiesOnScreen() {
    int aCount = 0;
    Zombie* aZombie = nullptr;
    while (IterateZombies(aZombie))
    {
        if (aZombie->mHasHead && !aZombie->IsDeadOrDying() && !aZombie->mMindControlled && aZombie->IsOnBoard())
        {
            aCount++;
        }
    }
    return aCount;
}

float Board::GetPosYBasedOnRow(float thePosX, int theRow) {
    if (StageHasRoof())
    {
        float aSlopeOffset = 0.0f;
        if (thePosX < 440.0f)
        {
            aSlopeOffset = (440.0f - thePosX) * 0.25f;
        }

        return GridToPixelY(8, theRow) + aSlopeOffset;
    }

    return GridToPixelY(0, theRow);
}

Zombie* Board::GetBossZombie() {
    Zombie* aZombie = nullptr;
    while (IterateZombies(aZombie))
    {
        if (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS)
        {
            return aZombie;
        }
    }
    return nullptr;
}

GamepadControls *Board::GetGamepadControlsByPlayerIndex(int thePlayerIndex) {
    GamepadControls *mGamepadControls1 = this->mGamepadControls1;
    if ( mGamepadControls1->mPlayerIndex1 != thePlayerIndex)
    {
        mGamepadControls1 = this->mGamepadControls2;
        if ( mGamepadControls1->mPlayerIndex1 != thePlayerIndex)
            return nullptr;
    }

    return mGamepadControls1;
}