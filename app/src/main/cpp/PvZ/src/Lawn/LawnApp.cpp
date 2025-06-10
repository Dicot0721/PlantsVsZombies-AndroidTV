#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/System/Music.h"
#include "PvZ/Lawn/Widget/ChallengeScreen.h"
#include "PvZ/Lawn/Widget/ConfirmBackToMainDialog.h"
#include "PvZ/Lawn/Widget/MainMenu.h"
#include "PvZ/Misc.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Effect/Reanimator.h"

using namespace Sexy;

// 此处写明具体每个贴图对应哪个文件.
void LawnApp::LoadAddonImages() {
    addonImages.pole_night = GetImageByFileName("addonFiles/images/pole_night");
    addonImages.trees_night = GetImageByFileName("addonFiles/images/trees_night");
    addonImages.googlyeye = GetImageByFileName("addonFiles/images/googlyeye");
    addonImages.squirrel = GetImageByFileName("addonFiles/images/squirrel");
    addonImages.stripe_day_coop = GetImageByFileName("addonFiles/images/stripe_day_coop");
    addonImages.stripe_pool_coop = GetImageByFileName("addonFiles/images/stripe_pool_coop");
    addonImages.stripe_roof_left = GetImageByFileName("addonFiles/images/stripe_roof_left");
    addonImages.butter_glove = GetImageByFileName("addonFiles/images/butter_glove");
    addonImages.custom_cobcannon = GetImageByFileName("addonFiles/images/custom_cobcannon");
    addonImages.hood1_house = GetImageByFileName("addonFiles/images/hood1_house");
    addonImages.hood2_house = GetImageByFileName("addonFiles/images/hood2_house");
    addonImages.hood3_house = GetImageByFileName("addonFiles/images/hood3_house");
    addonImages.hood4_house = GetImageByFileName("addonFiles/images/hood4_house");
    addonImages.house_hill_house = GetImageByFileName("addonFiles/images/house_hill_house");
    addonImages.achievement_homeLawnsecurity = GetImageByFileName("addonFiles/images/achievement_homeLawnsecurity");
    addonImages.achievement_chomp = GetImageByFileName("addonFiles/images/achievement_chomp");
    addonImages.achievement_closeshave = GetImageByFileName("addonFiles/images/achievement_closeshave");
    addonImages.achievement_coop = GetImageByFileName("addonFiles/images/achievement_coop");
    addonImages.achievement_explodonator = GetImageByFileName("addonFiles/images/achievement_explodonator");
    addonImages.achievement_garg = GetImageByFileName("addonFiles/images/achievement_garg");
    addonImages.achievement_immortal = GetImageByFileName("addonFiles/images/achievement_immortal");
    addonImages.achievement_shop = GetImageByFileName("addonFiles/images/achievement_shop");
    addonImages.achievement_soilplants = GetImageByFileName("addonFiles/images/achievement_soilplants");
    addonImages.achievement_tree = GetImageByFileName("addonFiles/images/achievement_tree");
    addonImages.achievement_versusz = GetImageByFileName("addonFiles/images/achievement_versusz");
    addonImages.achievement_morticulturalist = GetImageByFileName("addonFiles/images/achievement_morticulturalist");
    addonImages.hole = GetImageByFileName("addonFiles/images/hole");
    addonImages.hole_bjorn = GetImageByFileName("addonFiles/images/hole_bjorn");
    addonImages.hole_china = GetImageByFileName("addonFiles/images/hole_china");
    addonImages.hole_gems = GetImageByFileName("addonFiles/images/hole_gems");
    addonImages.hole_chuzzle = GetImageByFileName("addonFiles/images/hole_chuzzle");
    addonImages.hole_heavyrocks = GetImageByFileName("addonFiles/images/hole_heavyrocks");
    addonImages.hole_duwei = GetImageByFileName("addonFiles/images/hole_duwei");
    addonImages.hole_pipe = GetImageByFileName("addonFiles/images/hole_pipe");
    addonImages.hole_tiki = GetImageByFileName("addonFiles/images/hole_tiki");
    addonImages.hole_worm = GetImageByFileName("addonFiles/images/hole_worm");
    addonImages.hole_top = GetImageByFileName("addonFiles/images/hole_top");
    addonImages.plant_can = GetImageByFileName("addonFiles/images/plant_can");
    addonImages.zombie_can = GetImageByFileName("addonFiles/images/zombie_can");
    addonImages.plant_pile01_stack01 = GetImageByFileName("addonFiles/images/plant_pile01_stack01");
    addonImages.plant_pile01_stack02 = GetImageByFileName("addonFiles/images/plant_pile01_stack02");
    addonImages.plant_pile02_stack01 = GetImageByFileName("addonFiles/images/plant_pile02_stack01");
    addonImages.plant_pile02_stack02 = GetImageByFileName("addonFiles/images/plant_pile02_stack02");
    addonImages.plant_pile03_stack01 = GetImageByFileName("addonFiles/images/plant_pile03_stack01");
    addonImages.plant_pile03_stack02 = GetImageByFileName("addonFiles/images/plant_pile03_stack02");
    addonImages.zombie_pile01_stack01 = GetImageByFileName("addonFiles/images/zombie_pile01_stack01");
    addonImages.zombie_pile01_stack02 = GetImageByFileName("addonFiles/images/zombie_pile01_stack02");
    addonImages.zombie_pile01_stack03 = GetImageByFileName("addonFiles/images/zombie_pile01_stack03");
    addonImages.zombie_pile02_stack01 = GetImageByFileName("addonFiles/images/zombie_pile02_stack01");
    addonImages.zombie_pile02_stack02 = GetImageByFileName("addonFiles/images/zombie_pile02_stack02");
    addonImages.zombie_pile02_stack03 = GetImageByFileName("addonFiles/images/zombie_pile02_stack03");
    addonImages.zombie_pile03_stack01 = GetImageByFileName("addonFiles/images/zombie_pile03_stack01");
    addonImages.zombie_pile03_stack02 = GetImageByFileName("addonFiles/images/zombie_pile03_stack02");
    addonImages.zombie_pile03_stack03 = GetImageByFileName("addonFiles/images/zombie_pile03_stack03");
    addonImages.survival_button = GetImageByFileName("addonFiles/images/survival_button");
    addonImages.leaderboards = GetImageByFileName("addonFiles/images/leaderboards");
    addonImages.SelectorScreen_WoodSign3 = GetImageByFileName("addonFiles/images/ZombatarWidget/SelectorScreen_WoodSign3");
    addonImages.SelectorScreen_WoodSign3_press = GetImageByFileName("addonFiles/images/ZombatarWidget/SelectorScreen_WoodSign3_press");
    addonImages.zombatar_portrait = GetImageByFileName("ZOMBATAR");
    addonImages.crater_night_roof_center = GetImageByFileName("addonFiles/images/crater_night_roof_center");
    addonImages.crater_night_roof_center->mNumRows = 1;
    addonImages.crater_night_roof_center->mNumCols = 2;
    addonImages.crater_night_roof_left = GetImageByFileName("addonFiles/images/crater_night_roof_left");
    addonImages.crater_night_roof_left->mNumRows = 1;
    addonImages.crater_night_roof_left->mNumCols = 2;

    int addonImagesNum = (sizeof(AddonImages) / sizeof(Sexy::Image *));
    mCompletedLoadingThreadTasks += 9 * addonImagesNum;

    //    for (int i = 0; i < addonImagesNum; ++i) {
    //        if (*((Sexy::Image **) ((char *) &AddonImages + i * sizeof(Sexy::Image *))) == NULL){
    //            LOGD("没成功%d",i);
    //        }
    //    }
}

// 此处写明具体每个音频对应哪个文件.
void LawnApp::LoadAddonSounds() {
    addonSounds.achievement = GetSoundByFileName("addonFiles/sounds/achievement");

    int addonSoundsNum = (sizeof(addonSounds) / sizeof(int));
    mCompletedLoadingThreadTasks += 54 * addonSoundsNum;
}

Image *LawnApp::GetImageByFileName(const char *theFileName) {
    // 根据贴图文件路径获得贴图
    int holder[1];
    Sexy_StrFormat(holder, theFileName);
    Image *theImage = GetImage(reinterpret_cast<string &>(holder), true);
    Sexy_String_Delete(holder);
    return theImage;
}

int LawnApp::GetSoundByFileName(const char *theFileName) {
    // 根据音频文件路径获得音频
    int holder[1];
    Sexy_StrFormat(holder, theFileName);
    int theSoundId = Sexy_AudiereSoundManager_LoadSound(mSoundManager, holder);
    Sexy_String_Delete(holder);
    return theSoundId;
}

void LawnApp::DoConfirmBackToMain(bool theIsSave) {
    // 实现在花园直接退出而不是弹窗退出；同时实现新版暂停菜单
    if (mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN) {
        mBoardResult = BoardResult::BOARDRESULT_QUIT;
        //        if (theIsSave) Board_TryToSaveGame(lawnApp->mBoard);
        DoBackToMain();
        return;
    }
    if ((mGameMode == GameMode::GAMEMODE_MP_VS || mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM || enableNewOptionsDialog) && GetDialog(Dialogs::DIALOG_NEWOPTIONS) == nullptr) {
        DoNewOptions(false, 0);
        return;
    }
    auto *dialog = (ConfirmBackToMainDialog *)operator new(sizeof(ConfirmBackToMainDialog));
    ConfirmBackToMainDialog_ConfirmBackToMainDialog(dialog, theIsSave);
    (*(void (**)(LawnApp *, int, Sexy::Widget *))(*(uint32_t *)this + 416))(this, Dialogs::DIALOG_CONFIRM_BACK_TO_MAIN, dialog);
    (*(void (**)(uint32_t, Sexy::Widget *))(**((uint32_t **)this + 165) + 48))(*((uint32_t *)this + 165), dialog);
}

void LawnApp::DoBackToMain() {
    // 实现每次退出游戏后都清空2P
    ClearSecondPlayer();

    return old_LawnApp_DoBackToMain(this);
}

void LawnApp::DoNewOptions(bool theFromGameSelector, unsigned int a3) {
    return old_LawnApp_DoNewOptions(this, theFromGameSelector, a3);
}

void LawnApp::OnSessionTaskFailed() {
    // 用此空函数替换游戏原有的LawnApp_OnSessionTaskFailed()函数，从而去除启动游戏时的“网络错误：255”弹窗
}

int LawnApp::GamepadToPlayerIndex(unsigned int thePlayerIndex) {
    // 实现双人结盟中1P选卡选满后自动切换为2P选卡
    if (IsCoopMode()) {
        return !m1PChoosingSeeds;
    }

    if (thePlayerIndex <= 3 )
    {
        if ( mPlayerInfo && thePlayerIndex == (*((int (__fastcall **)(PlayerInfo *))mPlayerInfo->vTable + 2))(mPlayerInfo) )
            return 0;

        if ( mTwoPlayerState != -1 && mTwoPlayerState == thePlayerIndex)
            return 1;
    }
    return -1;
}

void LawnApp::UpdateApp() {
    // 去除道具教学关卡
    if (mPlayerInfo != nullptr) {
        mPlayerInfo->mPassedShopSeedTutorial = true; // 标记玩家已经通过1-1的道具教学关卡
    }
    if (doCheatDialog) {
        if (!isMainMenu)
            DoCheatDialog();
        doCheatDialog = false;
    }
    if (doCheatCodeDialog) {
        if (!isMainMenu)
            DoCheatCodeDialog();
        doCheatCodeDialog = false;
    }
    if (doKeyboardTwoPlayerDialog && mTitleScreen == nullptr) {
        LawnMessageBox(Dialogs::DIALOG_MESSAGE, "双人模式已开启", "已经进入双人模式；再次按下切换键即可退出此模式。", "[DIALOG_BUTTON_OK]", "", 3);
        doKeyboardTwoPlayerDialog = false;
    }

    old_LawnApp_UpDateApp(this);
}

void LawnApp::ShowAwardScreen(AwardType theAwardType) {
    //记录当前游戏状态
    return old_LawnApp_ShowAwardScreen(this, theAwardType);
}

void LawnApp::KillAwardScreen() {
    //记录当前游戏状态
    return old_LawnApp_KillAwardScreen(this);
}

bool LawnApp::CanShopLevel() {
    // 决定是否在当前关卡显示道具栏
    if (disableShop)
        return false;
    if (mGameMode == GameMode::GAMEMODE_MP_VS || IsCoopMode())
        return false;

    return old_LawnApp_CanShopLevel(this);
}

void LawnApp::KillDialog(Dialogs theId) {
    (*(void (**)(LawnApp *, Dialogs))(*(uint32_t *)this + 428))(this, theId); // KillDialog(Dialogs::DIALOG_HELPOPTIONS)
}

void LawnApp::ShowCreditScreen(bool theIsFromMainMenu) {
    // 用于一周目之后点击"制作人员"按钮播放MV
    TodFoley_StopFoley(mSoundSystem, FoleyType::FOLEY_MENU_LEFT);
    TodFoley_StopFoley(mSoundSystem, FoleyType::FOLEY_MENU_CENTRE);
    TodFoley_StopFoley(mSoundSystem, FoleyType::FOLEY_MENU_RIGHT);
    if (theIsFromMainMenu && HasFinishedAdventure()) {
        theIsFromMainMenu = false;
        KillMainMenu();
        KillNewOptionsDialog();
        KillDialog(Dialogs::DIALOG_HELPOPTIONS);
    }

    return old_LawnApp_ShowCreditScreen(this, theIsFromMainMenu);
}

void LawnApp::LoadLevelConfiguration(int a2, int a3) {
    // 如果开启了恢复出怪，则什么都不做，以做到禁止从levels.xml加载出怪。
    if (normalLevel)
        return;

    old_LawnApp_LoadLevelConfiguration(this, a2, a3);
}

void LawnApp::TryHelpTextScreen(int theScreenPage) {
    // 空函数替换，去除初次进入对战结盟模式时的操作提示。
}

void LawnApp_PlaySample(LawnApp *lawnApp, int soundAddr) {
    (*(void (**)(LawnApp *, int, int))(*(uint32_t *)lawnApp + LAWNAPP_PLAYSAMPLE_OFFSET))(lawnApp, soundAddr, 1);
}

void LawnApp::Create() {
    old_LawnApp_LawnApp(this);
    mLawnMouseMode = true; // 开启触控
}

void LawnApp::Init() {
    // 试图修复默认加载名为player用户的问题。

    old_LawnApp_Init(this);
    if (mPlayerInfo == nullptr && mProfileMgr[7] > 0) {
        mPlayerInfo = Sexy_DefaultProfileMgr_GetAnyProfile(mProfileMgr);
    }

    mNewIs3DAccelerated = mPlayerInfo == nullptr || !mPlayerInfo->mIs3DAcceleratedClosed;
}

void LawnApp::Load(const char *theGroupName) {
    int holder[1];
    Sexy_StrFormat(holder, "%s", theGroupName);
    TodLoadResources(holder);
    Sexy_String_Delete(holder);
}

void LawnApp::DoConvertImitaterImages() {
    for (int i = 0;; ++i) {
        int holder[1];
        int holder1[1];
        int holder2[1];
        Sexy_StrFormat(holder, "convertImitaterImages/pic%d", i);
        Sexy_StrFormat(holder1, "ImitaterNormalpic%d.png", i);
        Sexy_StrFormat(holder2, "ImitaterLesspic%d.png", i);
        Image *imageFromFile = GetImage(reinterpret_cast<string &>(holder), true);

        if (imageFromFile == nullptr) {
            break;
        }
        Image *imageImitater = FilterEffectGetImage(imageFromFile, FilterEffect::FILTEREFFECT_WASHED_OUT);
        Image *imageImitaterLess = FilterEffectGetImage(imageFromFile, FilterEffect::FILTEREFFECT_LESS_WASHED_OUT);
        Sexy_MemoryImage_WriteToPng(imageImitater, holder1);
        Sexy_MemoryImage_WriteToPng(imageImitaterLess, holder2);
        Sexy_MemoryImage_Delete(imageFromFile);
        Sexy_MemoryImage_Delete(imageImitater);
        Sexy_MemoryImage_Delete(imageImitaterLess);

        Sexy_String_Delete(holder);
        Sexy_String_Delete(holder1);
        Sexy_String_Delete(holder2);
    }
}

void LawnApp::LoadingThreadProc() {
    // 加载新增资源用
    old_LawnApp_LoadingThreadProc(this);

    LoadAddonImages();
    LoadAddonSounds();
    //    LawnApp_DoConvertImitaterImages(lawnApp);
    TodStringListLoad("addonFiles/properties/AddonStrings.txt"); // 加载自定义字符串

    // 加载三个主界面背景白噪音Foley
    *gFoleyParamArraySizeAddr += 3;
    gMenuLeftFoley.mSfxID[0] = Sexy_SOUND_MENU_L_ST_Addr;
    gMenuCenterFoley.mSfxID[0] = Sexy_SOUND_MENU_C_ST_Addr;
    gMenuRightFoley.mSfxID[0] = Sexy_SOUND_MENU_R_ST_Addr;

    //    //试图修复偶现的地图错位现象。不知道是否有效
    //    LawnApp_Load(lawnApp,"DelayLoad_Background1");
    //    LawnApp_Load(lawnApp,"DelayLoad_BackgroundUnsodded");
    //    LawnApp_Load(lawnApp,"DelayLoad_Background2");
    //    LawnApp_Load(lawnApp,"DelayLoad_Background3");
    //    LawnApp_Load(lawnApp,"DelayLoad_Background4");
    //    LawnApp_Load(lawnApp,"DelayLoad_Background5");
    //    LawnApp_Load(lawnApp,"DelayLoad_Background6");

    if (showHouse) {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_LEADERBOARDS_HOUSE, true);
        mCompletedLoadingThreadTasks += 136;
    }
    ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_ZOMBATAR_HEAD, true);
    mCompletedLoadingThreadTasks += 136;
}

bool LawnApp::IsChallengeWithoutSeedBank() {
    // 黄油爆米花专用
    return mGameMode == GameMode::GAMEMODE_CHALLENGE_BUTTERED_POPCORN || old_LawnApp_IsChallengeWithoutSeedBank(this);
}

int LawnApp::GetSeedsAvailable(int theIsZombieChooser) {
    // 解锁僵尸方六张隐藏卡片
    if (theIsZombieChooser) {
        return 86;
    }

    return old_LawnApp_GetSeedsAvailable(this, theIsZombieChooser);
}

void LawnApp::HardwareInit() {
    old_LawnApp_HardwareInit(this);
    //    if (useXboxMusic) {
    //        Music2_Delete(lawnApp->mMusic);
    //        lawnApp->mMusic = (Music2*) operator new(104u);
    //        Music_Music(lawnApp->mMusic); // 使用Music而非Music2
    //    }
}

int LawnApp::GetNumPreloadingTasks() {
    int oldResult = old_LawnApp_GetNumPreloadingTasks(this);

    int addonReanimsNum = (NUM_REANIMS - REANIM_ZOMBATAR_HEAD) + (showHouse ? 1 : 0);
    int addonSoundsNum = (sizeof(addonSounds) / sizeof(int));
    int addonImagesNum = (sizeof(AddonImages) / sizeof(Sexy::Image *));

    oldResult += 136 * addonReanimsNum;
    oldResult += 54 * addonSoundsNum;
    oldResult += 9 * addonImagesNum;

    return oldResult;
}

bool LawnApp::GrantAchievement(AchievementId theAchievementId) {
    // 一些非Board的成就在这里处理
    if (!mPlayerInfo->mAchievements[theAchievementId]) {
        LawnApp_PlaySample(this, addonSounds.achievement);
        //    int holder[1];
        //    Sexy_StrFormat(holder,"一二三四五六 成就达成！");
        //    ((CustomMessageWidget*)board->mAdvice)->mIcon = GetIconByAchievementId(theAchievementId);
        //    Board_DisplayAdviceAgain(board, holder, a::MESSAGE_STYLE_ACHIEVEMENT, AdviceType::ADVICE_NEED_ACHIVEMENT_EARNED);
        //    Sexy_String_Delete(holder);
        mPlayerInfo->mAchievements[theAchievementId] = true;
        return true;
    }

    return false;
}

bool LawnApp::IsNight() {
    // 添加非冒险模式（如：小游戏、花园、智慧树）关卡内进商店的昼夜判定
    if (mBoard != nullptr) {
        return Board_StageIsNight(mBoard);
    }

    if (IsIceDemo() || mPlayerInfo == nullptr)
        return false;

    return (mPlayerInfo->mLevel >= 11 && mPlayerInfo->mLevel <= 20) || (mPlayerInfo->mLevel >= 31 && mPlayerInfo->mLevel <= 40) || mPlayerInfo->mLevel == 50;
}

int LawnApp::TrophiesNeedForGoldSunflower() {
    // 修复新增的小游戏不记入金向日葵达成条件
    int theNumMiniGames = 0;
    for (int i = 0; i < 94; ++i) {
        if (GetChallengeDefinition(i).mPage == ChallengePage::CHALLENGE_PAGE_CHALLENGE) {
            theNumMiniGames++;
        }
    }
    return theNumMiniGames + 18 + 10 - GetNumTrophies(ChallengePage::CHALLENGE_PAGE_SURVIVAL) - GetNumTrophies(ChallengePage::CHALLENGE_PAGE_CHALLENGE)
        - GetNumTrophies(ChallengePage::CHALLENGE_PAGE_PUZZLE);
}

void LawnApp::SetFoleyVolume(FoleyType theFoleyType, double theVolume) {
    FoleyTypeData *foleyTypeData = &mSoundSystem->mTypeData[theFoleyType];
    for (FoleyInstance &foleyInstance : foleyTypeData->mFoleyInstances) {
        if (foleyInstance.mRefCount != 0) {
            int *mInstance = foleyInstance.mInstance;
            (*(void (**)(int *, uint32_t, double))(*mInstance + 28))(mInstance, *(uint32_t *)(*mInstance + 28), theVolume);
        }
    }
}

void LawnApp::ShowLeaderboards() {
    gMainMenuLeaderboardsWidget = (LeaderboardsWidget *)operator new(sizeof(LeaderboardsWidget));
    LeaderboardsWidget(this);
//    gMainMenuLeaderboardsWidget = new LeaderboardsWidget(this);
    (*(void (**)(int *, Sexy::Widget *))(*mWidgetManager + 24))(mWidgetManager, gMainMenuLeaderboardsWidget); // AddWidget()
    (*(void (**)(int *, Sexy::Widget *))(*mWidgetManager + 48))(mWidgetManager, gMainMenuLeaderboardsWidget); // SetFocusedWidget()
}

void LawnApp::KillLeaderboards() {
    if (gMainMenuLeaderboardsWidget == nullptr)
        return;

    (*(void (**)(int *, Sexy::Widget *))(*mWidgetManager + 28))(mWidgetManager, gMainMenuLeaderboardsWidget); // RemoveWidget()
    (*((void (**)(LawnApp *, Sexy::Widget *))vTable + 47))(this, gMainMenuLeaderboardsWidget);                      // MSGBOX()
    gMainMenuLeaderboardsWidget = nullptr;
}

void LawnApp::ShowZombatarScreen() {
    gMainMenuZombatarWidget = new ZombatarWidget(this);
    //    Sexy_Widget_Resize(gMainMenuZombatarWidget,-80,-60,960,720);
    (*(void (**)(int *, Sexy::Widget *))(*mWidgetManager + 24))(mWidgetManager, gMainMenuZombatarWidget); // AddWidget()
    (*(void (**)(int *, Sexy::Widget *))(*mWidgetManager + 48))(mWidgetManager, gMainMenuZombatarWidget); // SetFocusedWidget()
}

void LawnApp::KillZombatarScreen() {
    if (gMainMenuZombatarWidget == nullptr)
        return;

    (*(void (**)(int *, Sexy::Widget *))(*mWidgetManager + 28))(mWidgetManager, gMainMenuZombatarWidget); // RemoveWidget()
    (*((void (**)(LawnApp *, Sexy::Widget *))vTable + 47))(this, gMainMenuZombatarWidget);                      // MSGBOX()
    gMainMenuZombatarWidget = nullptr;
}


namespace {
char houseControl[6][15] = {"anim_house_1_1", "anim_house_1_1", "anim_house_1_2", "anim_house_1_3", "anim_house_1_4", "anim_house_1_5"};
char housePrefix[5][8] = {"house_1", "house_2", "house_3", "house_4", "house_5"};
} // namespace

void LawnApp::SetHouseReanim(Reanimation *theHouseAnim) {
    if (mPlayerInfo == nullptr)
        return;

    HouseType currentHouseType = mPlayerInfo->mGameStats.mHouseType;
    int currentHouseLevel = mPlayerInfo->mPurchases[StoreItem::STORE_ITEM_BLUEPRINT_BLING + currentHouseType];
    if (currentHouseType == HouseType::BLUEPRINT_BLING) {
        if (CanShowStore()) {
            currentHouseLevel += 3;
        } else if (CanShowAlmanac()) {
            currentHouseLevel += 2;
        } else {
            currentHouseLevel += 1;
        }
    }

    theHouseAnim->PlayReanim(houseControl[currentHouseLevel], ReanimLoopType::REANIM_LOOP, 0, 12.0f);

    for (int i = 0; i < 5; ++i) {
        Reanimation_HideTrackByPrefix(theHouseAnim, housePrefix[i], i != currentHouseType);
    }

    Reanimation_HideTrackByPrefix(theHouseAnim, "achievement", true);
}

bool LawnApp::IsIZombieLevel() {
    if (mBoard == nullptr)
        return false;

    return mGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_1 || mGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_2 || mGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_3
        || mGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_4 || mGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_5 || mGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_6
        || mGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_7 || mGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_8 || mGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_9
        || mGameMode == GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_ENDLESS;
}

bool LawnApp::IsWallnutBowlingLevel() {
    if (mBoard == nullptr)
        return false;

    if (mGameMode == GameMode::GAMEMODE_CHALLENGE_WALLNUT_BOWLING || mGameMode == GameMode::GAMEMODE_CHALLENGE_WALLNUT_BOWLING_2)
        return true;

    return IsAdventureMode() && mPlayerInfo->mLevel == 5;
}

bool LawnApp::IsAdventureMode() {
    return mGameMode == GameMode::GAMEMODE_ADVENTURE;
}

bool LawnApp::IsPuzzleMode() {
    return mGameMode >= GameMode::GAMEMODE_SCARY_POTTER_1 && mGameMode <= GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_ENDLESS;
}

bool LawnApp::IsLittleTroubleLevel() {
    return (mBoard && (mGameMode == GameMode::GAMEMODE_CHALLENGE_LITTLE_TROUBLE || (mGameMode == GameMode::GAMEMODE_ADVENTURE && mPlayerInfo->mLevel == 25)));
}

bool LawnApp::IsScaryPotterLevel() {
    if (mGameMode >= GameMode::GAMEMODE_SCARY_POTTER_1 && mGameMode <= GameMode::GAMEMODE_SCARY_POTTER_ENDLESS)
        return true;

    return IsAdventureMode() && mPlayerInfo->mLevel == 35;
}

bool LawnApp::IsSlotMachineLevel()
{
    return (mBoard && mGameMode == GameMode::GAMEMODE_CHALLENGE_SLOT_MACHINE);
}

bool LawnApp::IsArtChallenge() {
    if (mBoard == nullptr)
        return false;

    return
        mGameMode == GameMode::GAMEMODE_CHALLENGE_ART_CHALLENGE_WALLNUT ||
        mGameMode == GameMode::GAMEMODE_CHALLENGE_ART_CHALLENGE_SUNFLOWER ||
        mGameMode == GameMode::GAMEMODE_CHALLENGE_SEEING_STARS;
}

bool LawnApp::IsSquirrelLevel()
{
    return mBoard && mGameMode == GameMode::GAMEMODE_CHALLENGE_SQUIRREL;
}

bool LawnApp::IsSurvivalEndless(GameMode theGameMode)
{
    int aLevel = theGameMode - GameMode::GAMEMODE_SURVIVAL_ENDLESS_STAGE_1;
    return aLevel >= 0 && aLevel <= 4;
}

bool LawnApp::IsWhackAZombieLevel() {
    if (mBoard == nullptr)
        return false;

    if (mGameMode == GameMode::GAMEMODE_CHALLENGE_WHACK_A_ZOMBIE)
        return true;

    return IsAdventureMode() && mPlayerInfo->mLevel == 15;
}

bool LawnApp::IsCoopMode() {
    return mGameMode >= GameMode::GAMEMODE_TWO_PLAYER_COOP_DAY && mGameMode <= GameMode::GAMEMODE_TWO_PLAYER_COOP_ENDLESS;
}

bool LawnApp::IsTwinSunbankMode() {
    return IsCoopMode();
}

bool LawnApp::IsFinalBossLevel() {
    if (mBoard == nullptr)
        return false;

    if (mGameMode == GameMode::GAMEMODE_CHALLENGE_FINAL_BOSS)
        return true;

    return IsAdventureMode() && mPlayerInfo->mLevel == 50;
}

PottedPlant* LawnApp::GetPottedPlantByIndex(int thePottedPlantIndex) {
    return &mPlayerInfo->mPottedPlants[thePottedPlantIndex];
}

static bool zombatarResLoaded;

void LawnApp::LoadZombatarResources() {
    if (zombatarResLoaded)
        return;

    addonZombatarImages.zombatar_main_bg = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_main_bg");
    addonZombatarImages.zombatar_widget_bg = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_widget_bg");
    addonZombatarImages.zombatar_widget_inner_bg = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_widget_inner_bg");
    addonZombatarImages.zombatar_display_window = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_display_window");
    addonZombatarImages.zombatar_mainmenuback_highlight = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_mainmenuback_highlight");
    addonZombatarImages.zombatar_finished_button = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_finished_button");
    addonZombatarImages.zombatar_finished_button_highlight = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_finished_button_highlight");
    addonZombatarImages.zombatar_view_button = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_view_button");
    addonZombatarImages.zombatar_view_button_highlight = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_view_button_highlight");
    addonZombatarImages.zombatar_skin_button = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_skin_button");
    addonZombatarImages.zombatar_skin_button_highlight = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_skin_button_highlight");
    addonZombatarImages.zombatar_hair_button = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hair_button");
    addonZombatarImages.zombatar_hair_button_highlight = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hair_button_highlight");
    addonZombatarImages.zombatar_fhair_button = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_fhair_button");
    addonZombatarImages.zombatar_fhair_button_highlight = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_fhair_button_highlight");
    addonZombatarImages.zombatar_tidbits_button = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_tidbits_button");
    addonZombatarImages.zombatar_tidbits_button_highlight = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_tidbits_button_highlight");
    addonZombatarImages.zombatar_eyewear_button = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_button");
    addonZombatarImages.zombatar_eyewear_button_highlight = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_button_highlight");
    addonZombatarImages.zombatar_clothes_button = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_clothes_button");
    addonZombatarImages.zombatar_clothes_button_highlight = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_clothes_button_highlight");
    addonZombatarImages.zombatar_accessory_button = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_accessory_button");
    addonZombatarImages.zombatar_accessory_button_highlight = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_accessory_button_highlight");
    addonZombatarImages.zombatar_hats_button = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hats_button");
    addonZombatarImages.zombatar_hats_button_highlight = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hats_button_highlight");
    addonZombatarImages.zombatar_next_button = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_next_button");
    addonZombatarImages.zombatar_prev_button = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_prev_button");
    addonZombatarImages.zombatar_backdrops_button = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_backdrops_button");
    addonZombatarImages.zombatar_backdrops_button_highlight = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_backdrops_button_highlight");
    addonZombatarImages.zombatar_background_crazydave = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_crazydave");
    addonZombatarImages.zombatar_background_menu = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_menu");
    addonZombatarImages.zombatar_background_menu_dos = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_menu_dos");
    addonZombatarImages.zombatar_background_roof = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_roof");
    addonZombatarImages.zombatar_background_blank = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_blank");
    addonZombatarImages.zombatar_background_aquarium = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_aquarium");
    addonZombatarImages.zombatar_background_crazydave_night = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_crazydave_night");
    addonZombatarImages.zombatar_background_day_RV = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_day_RV");
    addonZombatarImages.zombatar_background_fog_sunshade = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_fog_sunshade");
    addonZombatarImages.zombatar_background_garden_hd = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_garden_hd");
    addonZombatarImages.zombatar_background_garden_moon = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_garden_moon");
    addonZombatarImages.zombatar_background_garden_mushrooms = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_garden_mushrooms");
    addonZombatarImages.zombatar_background_hood = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_hood");
    addonZombatarImages.zombatar_background_hood_blue = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_hood_blue");
    addonZombatarImages.zombatar_background_hood_brown = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_hood_brown");
    addonZombatarImages.zombatar_background_hood_yellow = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_hood_yellow");
    addonZombatarImages.zombatar_background_mausoleum = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_mausoleum");
    addonZombatarImages.zombatar_background_moon = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_moon");
    addonZombatarImages.zombatar_background_moon_distant = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_moon_distant");
    addonZombatarImages.zombatar_background_night_RV = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_night_RV");
    addonZombatarImages.zombatar_background_pool_sunshade = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_pool_sunshade");
    addonZombatarImages.zombatar_background_roof_distant = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_roof_distant");
    addonZombatarImages.zombatar_background_sky_day = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_sky_day");
    addonZombatarImages.zombatar_background_sky_night = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_sky_night");
    addonZombatarImages.zombatar_background_sky_purple = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_sky_purple");
    addonZombatarImages.zombatar_background_7 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_7");
    addonZombatarImages.zombatar_background_8 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_8");
    addonZombatarImages.zombatar_background_9 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_9");
    addonZombatarImages.zombatar_background_10 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_10");
    addonZombatarImages.zombatar_background_11 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_11");
    addonZombatarImages.zombatar_background_11_1 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_11_1");
    addonZombatarImages.zombatar_background_12 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_12");
    addonZombatarImages.zombatar_background_12_1 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_12_1");
    addonZombatarImages.zombatar_background_13 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_13");
    addonZombatarImages.zombatar_background_13_1 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_13_1");
    addonZombatarImages.zombatar_background_14 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_14");
    addonZombatarImages.zombatar_background_14_1 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_14_1");
    addonZombatarImages.zombatar_background_15 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_15");
    addonZombatarImages.zombatar_background_15_1 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_15_1");
    addonZombatarImages.zombatar_background_16 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_16");
    addonZombatarImages.zombatar_background_16_1 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_16_1");
    addonZombatarImages.zombatar_background_17 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_17");
    addonZombatarImages.zombatar_background_17_1 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_17_1");
    addonZombatarImages.zombatar_background_bej3_bridge_shroom_castles = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_bej3_bridge_shroom_castles");
    addonZombatarImages.zombatar_background_bej3_canyon_wall = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_bej3_canyon_wall");
    addonZombatarImages.zombatar_background_bej3_crystal_mountain_peak = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_bej3_crystal_mountain_peak");
    addonZombatarImages.zombatar_background_bej3_dark_cave_thing = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_bej3_dark_cave_thing");
    addonZombatarImages.zombatar_background_bej3_desert_pyramids_sunset = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_bej3_desert_pyramids_sunset");
    addonZombatarImages.zombatar_background_bej3_fairy_cave_village = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_bej3_fairy_cave_village");
    addonZombatarImages.zombatar_background_bej3_floating_rock_city = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_bej3_floating_rock_city");
    addonZombatarImages.zombatar_background_bej3_horse_forset_tree = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_bej3_horse_forset_tree");
    addonZombatarImages.zombatar_background_bej3_jungle_ruins_path = GetImageByFileName("PvZ/addonFiles/images/ZombatarWidget/zombatar_background_bej3_jungle_ruins_pa.h");
    addonZombatarImages.zombatar_background_bej3_lantern_plants_world = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_bej3_lantern_plants_world");
    addonZombatarImages.zombatar_background_bej3_lightning = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_bej3_lightning");
    addonZombatarImages.zombatar_background_bej3_lion_tower_cascade = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_bej3_lion_tower_cascade");
    addonZombatarImages.zombatar_background_bej3_pointy_ice_path = GetImageByFileName("PvZ/addonFiles/images/ZombatarWidget/zombatar_background_bej3_pointy_ice_pa.h");
    addonZombatarImages.zombatar_background_bej3_pointy_ice_path_purple = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_bej3_pointy_ice_path_purple");
    addonZombatarImages.zombatar_background_bej3_rock_city_lake = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_bej3_rock_city_lake");
    addonZombatarImages.zombatar_background_bej3_snowy_cliffs_castle = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_bej3_snowy_cliffs_castle");
    addonZombatarImages.zombatar_background_bej3_treehouse_waterfall = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_bej3_treehouse_waterfall");
    addonZombatarImages.zombatar_background_bej3_tube_forest_night = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_bej3_tube_forest_night");
    addonZombatarImages.zombatar_background_bej3_water_bubble_city = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_bej3_water_bubble_city");
    addonZombatarImages.zombatar_background_bej3_water_fall_cliff = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_bej3_water_fall_cliff");
    addonZombatarImages.zombatar_background_bejblitz_6 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_bejblitz_6");
    addonZombatarImages.zombatar_background_bejblitz_8 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_bejblitz_8");
    addonZombatarImages.zombatar_background_bejblitz_main_menu = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_bejblitz_main_menu");
    addonZombatarImages.zombatar_background_peggle_bunches = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_peggle_bunches");
    addonZombatarImages.zombatar_background_peggle_fever = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_peggle_fever");
    addonZombatarImages.zombatar_background_peggle_level1 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_peggle_level1");
    addonZombatarImages.zombatar_background_peggle_level4 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_peggle_level4");
    addonZombatarImages.zombatar_background_peggle_level5 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_peggle_level5");
    addonZombatarImages.zombatar_background_peggle_menu = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_peggle_menu");
    addonZombatarImages.zombatar_background_peggle_nights_bjorn3 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_peggle_nights_bjorn3");
    addonZombatarImages.zombatar_background_peggle_nights_bjorn4 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_peggle_nights_bjorn4");
    addonZombatarImages.zombatar_background_peggle_nights_claude5 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_peggle_nights_claude5");
    addonZombatarImages.zombatar_background_peggle_nights_kalah1 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_peggle_nights_kalah1");
    addonZombatarImages.zombatar_background_peggle_nights_kalah4 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_peggle_nights_kalah4");
    addonZombatarImages.zombatar_background_peggle_nights_master5 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_peggle_nights_master5");
    addonZombatarImages.zombatar_background_peggle_nights_renfield5 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_peggle_nights_renfield5");
    addonZombatarImages.zombatar_background_peggle_nights_tut5 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_peggle_nights_tut5");
    addonZombatarImages.zombatar_background_peggle_nights_warren3 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_peggle_nights_warren3");
    addonZombatarImages.zombatar_background_peggle_paperclips = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_peggle_paperclips");
    addonZombatarImages.zombatar_background_peggle_waves = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_background_peggle_waves");
    addonZombatarImages.zombatar_hair_1 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hair_1");
    addonZombatarImages.zombatar_hair_1_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hair_1_mask");
    addonZombatarImages.zombatar_hair_2 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hair_2");
    addonZombatarImages.zombatar_hair_2_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hair_2_mask");
    addonZombatarImages.zombatar_hair_3 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hair_3");
    addonZombatarImages.zombatar_hair_4 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hair_4");
    addonZombatarImages.zombatar_hair_5 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hair_5");
    addonZombatarImages.zombatar_hair_6 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hair_6");
    addonZombatarImages.zombatar_hair_7 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hair_7");
    addonZombatarImages.zombatar_hair_8 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hair_8");
    addonZombatarImages.zombatar_hair_9 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hair_9");
    addonZombatarImages.zombatar_hair_10 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hair_10");
    addonZombatarImages.zombatar_hair_11 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hair_11");
    addonZombatarImages.zombatar_hair_11_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hair_11_mask");
    addonZombatarImages.zombatar_hair_12 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hair_12");
    addonZombatarImages.zombatar_hair_12_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hair_12_mask");
    addonZombatarImages.zombatar_hair_13 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hair_13");
    addonZombatarImages.zombatar_hair_13_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hair_13_mask");
    addonZombatarImages.zombatar_hair_14 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hair_14");
    addonZombatarImages.zombatar_hair_14_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hair_14_mask");
    addonZombatarImages.zombatar_hair_15 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hair_15");
    addonZombatarImages.zombatar_hair_15_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hair_15_mask");
    addonZombatarImages.zombatar_hair_16 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hair_16");
    addonZombatarImages.zombatar_facialhair_1 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_1");
    addonZombatarImages.zombatar_facialhair_1_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_1_mask");
    addonZombatarImages.zombatar_facialhair_2 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_2");
    addonZombatarImages.zombatar_facialhair_3 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_3");
    addonZombatarImages.zombatar_facialhair_4 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_4");
    addonZombatarImages.zombatar_facialhair_4_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_4_mask");
    addonZombatarImages.zombatar_facialhair_5 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_5");
    addonZombatarImages.zombatar_facialhair_6 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_6");
    addonZombatarImages.zombatar_facialhair_7 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_7");
    addonZombatarImages.zombatar_facialhair_8 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_8");
    addonZombatarImages.zombatar_facialhair_8_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_8_mask");
    addonZombatarImages.zombatar_facialhair_9 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_9");
    addonZombatarImages.zombatar_facialhair_9_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_9_mask");
    addonZombatarImages.zombatar_facialhair_10 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_10");
    addonZombatarImages.zombatar_facialhair_10_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_10_mask");
    addonZombatarImages.zombatar_facialhair_11 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_11");
    addonZombatarImages.zombatar_facialhair_11_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_11_mask");
    addonZombatarImages.zombatar_facialhair_12 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_12");
    addonZombatarImages.zombatar_facialhair_12_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_12_mask");
    addonZombatarImages.zombatar_facialhair_13 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_13");
    addonZombatarImages.zombatar_facialhair_14 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_14");
    addonZombatarImages.zombatar_facialhair_14_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_14_mask");
    addonZombatarImages.zombatar_facialhair_15 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_15");
    addonZombatarImages.zombatar_facialhair_15_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_15_mask");
    addonZombatarImages.zombatar_facialhair_16 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_16");
    addonZombatarImages.zombatar_facialhair_16_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_16_mask");
    addonZombatarImages.zombatar_facialhair_17 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_17");
    addonZombatarImages.zombatar_facialhair_18 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_18");
    addonZombatarImages.zombatar_facialhair_18_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_18_mask");
    addonZombatarImages.zombatar_facialhair_19 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_19");
    addonZombatarImages.zombatar_facialhair_20 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_20");
    addonZombatarImages.zombatar_facialhair_21 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_21");
    addonZombatarImages.zombatar_facialhair_21_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_21_mask");
    addonZombatarImages.zombatar_facialhair_22 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_22");
    addonZombatarImages.zombatar_facialhair_22_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_22_mask");
    addonZombatarImages.zombatar_facialhair_23 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_23");
    addonZombatarImages.zombatar_facialhair_23_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_23_mask");
    addonZombatarImages.zombatar_facialhair_24 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_24");
    addonZombatarImages.zombatar_facialhair_24_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_facialhair_24_mask");
    addonZombatarImages.zombatar_eyewear_1 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_1");
    addonZombatarImages.zombatar_eyewear_1_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_1_mask");
    addonZombatarImages.zombatar_eyewear_2 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_2");
    addonZombatarImages.zombatar_eyewear_2_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_2_mask");
    addonZombatarImages.zombatar_eyewear_3 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_3");
    addonZombatarImages.zombatar_eyewear_3_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_3_mask");
    addonZombatarImages.zombatar_eyewear_4 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_4");
    addonZombatarImages.zombatar_eyewear_4_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_4_mask");
    addonZombatarImages.zombatar_eyewear_5 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_5");
    addonZombatarImages.zombatar_eyewear_5_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_5_mask");
    addonZombatarImages.zombatar_eyewear_6 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_6");
    addonZombatarImages.zombatar_eyewear_6_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_6_mask");
    addonZombatarImages.zombatar_eyewear_7 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_7");
    addonZombatarImages.zombatar_eyewear_7_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_7_mask");
    addonZombatarImages.zombatar_eyewear_8 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_8");
    addonZombatarImages.zombatar_eyewear_8_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_8_mask");
    addonZombatarImages.zombatar_eyewear_9 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_9");
    addonZombatarImages.zombatar_eyewear_9_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_9_mask");
    addonZombatarImages.zombatar_eyewear_10 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_10");
    addonZombatarImages.zombatar_eyewear_10_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_10_mask");
    addonZombatarImages.zombatar_eyewear_11 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_11");
    addonZombatarImages.zombatar_eyewear_11_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_11_mask");
    addonZombatarImages.zombatar_eyewear_12 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_12");
    addonZombatarImages.zombatar_eyewear_12_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_12_mask");
    addonZombatarImages.zombatar_eyewear_13 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_13");
    addonZombatarImages.zombatar_eyewear_14 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_14");
    addonZombatarImages.zombatar_eyewear_15 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_15");
    addonZombatarImages.zombatar_eyewear_16 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_eyewear_16");
    addonZombatarImages.zombatar_accessory_1 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_accessory_1");
    addonZombatarImages.zombatar_accessory_2 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_accessory_2");
    addonZombatarImages.zombatar_accessory_3 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_accessory_3");
    addonZombatarImages.zombatar_accessory_4 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_accessory_4");
    addonZombatarImages.zombatar_accessory_5 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_accessory_5");
    addonZombatarImages.zombatar_accessory_6 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_accessory_6");
    addonZombatarImages.zombatar_accessory_8 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_accessory_8");
    addonZombatarImages.zombatar_accessory_9 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_accessory_9");
    addonZombatarImages.zombatar_accessory_10 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_accessory_10");
    addonZombatarImages.zombatar_accessory_11 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_accessory_11");
    addonZombatarImages.zombatar_accessory_12 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_accessory_12");
    addonZombatarImages.zombatar_accessory_13 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_accessory_13");
    addonZombatarImages.zombatar_accessory_14 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_accessory_14");
    addonZombatarImages.zombatar_accessory_15 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_accessory_15");
    addonZombatarImages.zombatar_accessory_16 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_accessory_16");
    addonZombatarImages.zombatar_hats_1 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hats_1");
    addonZombatarImages.zombatar_hats_1_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hats_1_mask");
    addonZombatarImages.zombatar_hats_2 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hats_2");
    addonZombatarImages.zombatar_hats_3 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hats_3");
    addonZombatarImages.zombatar_hats_3_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hats_3_mask");
    addonZombatarImages.zombatar_hats_4 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hats_4");
    addonZombatarImages.zombatar_hats_5 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hats_5");
    addonZombatarImages.zombatar_hats_6 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hats_6");
    addonZombatarImages.zombatar_hats_6_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hats_6_mask");
    addonZombatarImages.zombatar_hats_7 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hats_7");
    addonZombatarImages.zombatar_hats_7_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hats_7_mask");
    addonZombatarImages.zombatar_hats_8 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hats_8");
    addonZombatarImages.zombatar_hats_8_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hats_8_mask");
    addonZombatarImages.zombatar_hats_9 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hats_9");
    addonZombatarImages.zombatar_hats_9_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hats_9_mask");
    addonZombatarImages.zombatar_hats_10 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hats_10");
    addonZombatarImages.zombatar_hats_11 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hats_11");
    addonZombatarImages.zombatar_hats_11_mask = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hats_11_mask");
    addonZombatarImages.zombatar_hats_12 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hats_12");
    addonZombatarImages.zombatar_hats_13 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hats_13");
    addonZombatarImages.zombatar_hats_14 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_hats_14");
    addonZombatarImages.zombatar_tidbits_1 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_tidbits_1");
    addonZombatarImages.zombatar_tidbits_2 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_tidbits_2");
    addonZombatarImages.zombatar_tidbits_3 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_tidbits_3");
    addonZombatarImages.zombatar_tidbits_4 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_tidbits_4");
    addonZombatarImages.zombatar_tidbits_5 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_tidbits_5");
    addonZombatarImages.zombatar_tidbits_6 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_tidbits_6");
    addonZombatarImages.zombatar_tidbits_7 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_tidbits_7");
    addonZombatarImages.zombatar_tidbits_8 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_tidbits_8");
    addonZombatarImages.zombatar_tidbits_9 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_tidbits_9");
    addonZombatarImages.zombatar_tidbits_10 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_tidbits_10");
    addonZombatarImages.zombatar_tidbits_11 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_tidbits_11");
    addonZombatarImages.zombatar_tidbits_12 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_tidbits_12");
    addonZombatarImages.zombatar_tidbits_13 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_tidbits_13");
    addonZombatarImages.zombatar_tidbits_14 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_tidbits_14");
    addonZombatarImages.zombatar_clothes_1 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_clothes_1");
    addonZombatarImages.zombatar_clothes_2 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_clothes_2");
    addonZombatarImages.zombatar_clothes_3 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_clothes_3");
    addonZombatarImages.zombatar_clothes_4 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_clothes_4");
    addonZombatarImages.zombatar_clothes_5 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_clothes_5");
    addonZombatarImages.zombatar_clothes_6 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_clothes_6");
    addonZombatarImages.zombatar_clothes_7 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_clothes_7");
    addonZombatarImages.zombatar_clothes_8 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_clothes_8");
    addonZombatarImages.zombatar_clothes_9 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_clothes_9");
    addonZombatarImages.zombatar_clothes_10 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_clothes_10");
    addonZombatarImages.zombatar_clothes_11 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_clothes_11");
    addonZombatarImages.zombatar_clothes_12 = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_clothes_12");
    addonZombatarImages.zombatar_zombie_blank = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_zombie_blank");
    addonZombatarImages.zombatar_zombie_blank_skin = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_zombie_blank_skin");
    int xClip = 130;
    int yClip = 130;
    Sexy::Rect rect = {addonZombatarImages.zombatar_zombie_blank->mWidth - xClip, addonZombatarImages.zombatar_zombie_blank->mHeight - yClip, xClip, yClip};
    addonZombatarImages.zombatar_zombie_blank_part = CopyImage(addonZombatarImages.zombatar_zombie_blank, rect);
    addonZombatarImages.zombatar_zombie_blank_skin_part = CopyImage(addonZombatarImages.zombatar_zombie_blank_skin, rect);
    addonZombatarImages.zombatar_colors_bg = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_colors_bg");
    addonZombatarImages.zombatar_colorpicker = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_colorpicker");
    addonZombatarImages.zombatar_colorpicker_none = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_colorpicker_none");
    addonZombatarImages.zombatar_accessory_bg = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_accessory_bg");
    addonZombatarImages.zombatar_accessory_bg_highlight = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_accessory_bg_highlight");
    addonZombatarImages.zombatar_accessory_bg_none = GetImageByFileName("addonFiles/images/ZombatarWidget/zombatar_accessory_bg_none");
    zombatarResLoaded = true;
}
