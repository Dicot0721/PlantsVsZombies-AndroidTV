/*
 * Copyright (C) 2023-2025  PvZ TV Touch Team
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

#ifndef PVZ_LAWN_LAWN_APP_H
#define PVZ_LAWN_LAWN_APP_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Lawn/System/TypingCheck.h"
#include "PvZ/Lawn/Widget/AchievementsWidget.h"
#include "PvZ/SexyAppFramework/GamepadApp.h"
#include "PvZ/SexyAppFramework/Misc/ProfileMgr.h"
#include "PvZ/SexyAppFramework/SexyAppBase.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Common/TodFoley.h"
#include "PvZ/TodLib/Effect/EffectSystem.h"

class ZenGarden;
class Board;
class TitleScreen;
class MainMenu;
class SeedChooserScreen;
class CreditScreen;
class ChallengeScreen;
class PoolEffect;
class ReanimatorCache;
class Music2;
class TodFoley;
class DefaultPlayerInfo;
class PottedPlant;
class VSSetupMenu;
class VSResultsMenu;
class MailBox;

class LawnApp : public Sexy::__GamepadApp {
public:
    Board *mBoard;                           // 552
    TitleScreen *mTitleScreen;               // 553
    MainMenu *mGameSelector;                 // 554
    int unk1[2];                             // 555 ~ 556
    int *mHelpTextScreen;                    // 557
    int unkUnk;                              // 558
    VSSetupMenu *mVSSetupScreen;             // 559
    VSResultsMenu *mVSResultsScreen;         // 560
    SeedChooserScreen *mSeedChooserScreen;   // 561
    SeedChooserScreen *mZombieChooserScreen; // 562
    int *mAwardScreen;                       // 563
    CreditScreen *mCreditScreen;             // 564
    ChallengeScreen *mChallengeScreen;       // 565
    TodFoley *mSoundSystem;                  // 566
    int *unk3[7];                            // 567 ~ 573
    bool mRegisterResourcesLoaded;           // 2296
    bool mTodCheatKeys;                      // 2297
    bool mNewIs3DAccelerated;                // 2298，在对齐间隙插入新成员
    GameMode mGameMode;                      // 575
    GameScenes mGameScene;                   // 576
    bool mLoadingZombiesThreadCompleted;     // 2308
    bool mFirstTimeGameSelector;             // 2309
    int mGamesPlayed;                        // 578
    int mMaxExecutions;                      // 579
    int mMaxPlays;                           // 580
    int mMaxTime;                            // 581
    bool mEasyPlantingCheat;                 // 2328
    PoolEffect *mPoolEffect;                 // 583
    ZenGarden *mZenGarden;                   // 584
    EffectSystem *mEffectSystem;             // 585
    ReanimatorCache *mReanimatorCache;       // 586
    DefaultPlayerInfo *mPlayerInfo;          // 587
    DefaultPlayerInfo *mPlayerInfo2P;        // 588 , 游戏内没有初始化，但有一些判定
    int *mLastLevelStats;                    // 589
    bool mCloseRequest;                      // 2360
    int mAppCounter;                         // 591
    Music2 *mMusic;                          // 592
    int mCrazyDaveReanimID;                  // 593
    CrazyDaveState mCrazyDaveState;          // 594
    int mCrazyDaveBlinkCounter;              // 595
    int mCrazyDaveBlinkReanimID;             // 596
    int mCrazyDaveMessageIndex;              // 597
    int *mCrazyDaveMessageText;              // 598
    int mAppRandSeed;                        // 599;
    int unk7;                                // 600
    Sexy::DefaultProfileMgr *mProfileMgr;    // 601
    int unk8[35];                            // 602 ~ 636
    Sexy::Image *mQRCodeImage;               // 637
    int unk8_1[7];                           // 638 ~ 644
    int mInitialSunMoney;                    // 645     // 这个数据能给玩家加初始阳光
    bool mIsFullVersion;                     // 2584
    int unk9_1[3];                           // 647 ~ 649
    int mVsInitialPlantMode;                 // 650
    int unk9_2[3];                           // 651 ~ 653
    BoardResult mBoardResult;                // 654
    bool mKilledYetiAndRestarted;            // 2620
    TypingCheck *mKonamiCheck;               // 656
    TypingCheck *mMustacheCheck;             // 657
    TypingCheck *mMoustacheCheck;            // 658
    TypingCheck *mSuperMowerCheck;           // 659
    TypingCheck *mSuperMowerCheck2;          // 660
    TypingCheck *mFutureCheck;               // 661
    TypingCheck *mPinataCheck;               // 662
    TypingCheck *mDanceCheck;                // 663
    TypingCheck *mDaisyCheck;                // 664
    TypingCheck *mSukhbirCheck;              // 665
    bool mMustacheMode;                      // 2664
    bool mSuperMowerMode;                    // 2665
    bool mFutureMode;                        // 2666
    bool mPinataMode;                        // 2667
    bool mDanceMode;                         // 2668
    bool mDaisyMode;                         // 2669
    bool mSukhbirMode;                       // 2670
    int unk10;                               // 668
    bool mMuteSoundsForCutscene;             // 2676
    bool unkBool2;                           // 2677
    int unk11;                               // 670
    int mTwoPlayerState;                     // 671
    int unk12[3];                            // 672 ~ 674
    MailBox *mMailBox;                       // 675
    int unk13_1[14];                         // 676 ~
    bool unkBool3[4];
    int unk13_2[6];
    pvzstl::string mGameInfoStrings[5];      //  ~ 701
    int *mHelpBarWidget;                     // 702
    int unk14;                               // 703
    int *mLogComposer;                       // 704
    int *MLogManager;                        // 705
    int mLaunchTime;                         // 706
    DaveHelp *mDaveHelp;                     // 707
    MaskHelpWidget *mMaskHelpWidget;         // 708

    Reanimation *ReanimationGet(ReanimationID theReanimationID) {
        return reinterpret_cast<Reanimation *(*)(LawnApp *, ReanimationID)>(LawnApp_ReanimationGetAddr)(this, theReanimationID);
    }
    Reanimation *ReanimationTryToGet(ReanimationID theReanimationID) {
        return reinterpret_cast<Reanimation *(*)(LawnApp *, ReanimationID)>(LawnApp_ReanimationTryToGetAddr)(this, theReanimationID);
    }
    // void ClearSecondPlayer() {
    // reinterpret_cast<void (*)(LawnApp *)>(LawnApp_ClearSecondPlayerAddr)(this);
    // }
    bool CanShowStore() {
        return reinterpret_cast<bool (*)(LawnApp *)>(LawnApp_CanShowStoreAddr)(this);
    }
    bool CanShowAlmanac() {
        return reinterpret_cast<bool (*)(LawnApp *)>(LawnApp_CanShowAlmanacAddr)(this);
    }
    void KillNewOptionsDialog() {
        reinterpret_cast<void (*)(LawnApp *)>(LawnApp_KillNewOptionsDialogAddr)(this);
    }
    void KillMainMenu() {
        reinterpret_cast<void (*)(LawnApp *)>(LawnApp_KillMainMenuAddr)(this);
    }
    void KillChallengeScreen() {
        reinterpret_cast<void (*)(LawnApp *)>(LawnApp_KillChallengeScreenAddr)(this);
    }
    void PreNewGame(GameMode theGameMode, bool theLookForSavedGame) {
        reinterpret_cast<void (*)(LawnApp *, GameMode, bool)>(LawnApp_PreNewGameAddr)(this, theGameMode, theLookForSavedGame);
    }
    void PlayFoleyPitch(FoleyType theFoleyType, float thePitch) {
        reinterpret_cast<void (*)(LawnApp *, FoleyType, float)>(LawnApp_PreNewGameAddr)(this, theFoleyType, thePitch);
    }
    void DoCheatDialog() {
        reinterpret_cast<void (*)(LawnApp *)>(LawnApp_DoCheatDialogAddr)(this);
    }
    void DoCheatCodeDialog() {
        reinterpret_cast<void (*)(LawnApp *)>(LawnApp_DoCheatCodeDialogAddr)(this);
    }
    void DoUserDialog() {
        reinterpret_cast<void (*)(LawnApp *)>(LawnApp_DoUserDialogAddr)(this);
    }
    Sexy::Dialog *DoConfirmRestartDialog() {
        return reinterpret_cast<Sexy::Dialog *(*)(LawnApp *)>(LawnApp_DoConfirmRestartDialogAddr)(this);
    }
    bool IsFirstTimeAdventureMode() {
        return reinterpret_cast<bool (*)(LawnApp *)>(LawnApp_IsFirstTimeAdventureModeAddr)(this);
    }
    // 阻塞式函数，能创建并立即展示一个带按钮的对话框。按钮个数由最后一个参数决定。其返回值就是用户按下的按钮ID，一般情况下只可能为1000或1001。
    int LawnMessageBox(Dialogs theDialogId, // 用于标识本对话框的ID，以便于用KillDialog(theDialogId)关闭此对话框。一般用不到，所以随便填个数字就可以。
                       const char *theHeaderName,
                       const char *theLinesName,
                       const char *theButton1Name,
                       const char *theButton2Name,
                       int theButtonMode) // 取值为0，1，2，3。其中0就是无按钮；1和2会展示两个按钮，其ID分别为1000和1001；3只会展示一个按钮，其ID为1000。
    {
        return reinterpret_cast<int (*)(LawnApp *, Dialogs, const char *, const char *, const char *, const char *, int)>(LawnApp_LawnMessageBoxAddr)(
            this, theDialogId, theHeaderName, theLinesName, theButton1Name, theButton2Name, theButtonMode);
    }
    TodParticleSystem *AddTodParticle(float theX, float theY, int theRenderOrder, ParticleEffect theEffect) {
        return reinterpret_cast<TodParticleSystem *(*)(LawnApp *, float, float, int, ParticleEffect)>(LawnApp_AddTodParticleAddr)(this, theX, theY, theRenderOrder, theEffect);
    }
    ParticleSystemID ParticleGetID(TodParticleSystem *theParticle) {
        return reinterpret_cast<ParticleSystemID (*)(LawnApp *, TodParticleSystem *)>(LawnApp_ParticleGetIDAddr)(this, theParticle);
    }
    Reanimation *AddReanimation(float theX, float theY, int theRenderOrder, ReanimationType theReanimationType) {
        return reinterpret_cast<Reanimation *(*)(LawnApp *, float, float, int, ReanimationType)>(LawnApp_AddReanimationAddr)(this, theX, theY, theRenderOrder, theReanimationType);
    }
    bool IsSurvivalMode() {
        return reinterpret_cast<bool (*)(LawnApp *)>(LawnApp_IsSurvivalModeAddr)(this);
    }
    bool HasFinishedAdventure() {
        return reinterpret_cast<bool (*)(LawnApp *)>(LawnApp_HasFinishedAdventureAddr)(this);
    }
    void KillVSResultsScreen() {
        reinterpret_cast<void (*)(LawnApp *)>(LawnApp_KillVSResultsScreenAddr)(this);
    }
    void HideHelpBarWidget() {
        reinterpret_cast<void (*)(LawnApp *)>(LawnApp_HideHelpBarWidgetAddr)(this);
    }
    void ShowHelpTextScreen(int a) {
        reinterpret_cast<void (*)(LawnApp *, int)>(LawnApp_ShowHelpTextScreenAddr)(this, a);
    }
    void ShowChallengeScreen(ChallengePage thePage) {
        reinterpret_cast<void (*)(LawnApp *, ChallengePage)>(LawnApp_ShowHelpTextScreenAddr)(this, thePage);
    }
    void CrazyDaveStopTalking() {
        reinterpret_cast<void (*)(LawnApp *)>(LawnApp_CrazyDaveStopTalkingAddr)(this);
    }
    void DoRetryAchievementsDialog() {
        reinterpret_cast<void (*)(LawnApp *)>(LawnApp_DoRetryAchievementsDialogAddr)(this);
    }
    bool EarnedGoldTrophy() {
        return reinterpret_cast<bool (*)(LawnApp *)>(LawnApp_EarnedGoldTrophyAddr)(this);
    }
    void RemoveReanimation(ReanimationID theReanimationID) {
        reinterpret_cast<void (*)(LawnApp *, ReanimationID)>(LawnApp_RemoveReanimationAddr)(this, theReanimationID);
    }
    ReanimationID ReanimationGetID(Reanimation *theReanimation) {
        return reinterpret_cast<ReanimationID (*)(LawnApp *, Reanimation *)>(LawnApp_ReanimationGetIDAddr)(this, theReanimation);
    }
    void KillAlmanacDialog() {
        reinterpret_cast<void (*)(LawnApp *)>(LawnApp_KillAlmanacDialogAddr)(this);
    }
    int GetNumTrophies(ChallengePage thePage) {
        return reinterpret_cast<int (*)(LawnApp *, ChallengePage)>(LawnApp_GetNumTrophiesAddr)(this, thePage);
    }
    void ShowMainMenuScreen() {
        reinterpret_cast<void (*)(LawnApp *)>(LawnApp_ShowMainMenuScreenAddr)(this);
    }
    TodParticleSystem *ParticleTryToGet(ParticleSystemID theParticleID) {
        return reinterpret_cast<TodParticleSystem *(*)(LawnApp *, ParticleSystemID)>(LawnApp_ParticleTryToGetAddr)(this, theParticleID);
    }
    void KillHelpTextScreen() {
        reinterpret_cast<void (*)(LawnApp *)>(LawnApp_KillHelpTextScreenAddr)(this);
    }
    void NextTestDialog() {
        reinterpret_cast<void (*)(LawnApp *)>(LawnApp_NextTestDialogAddr)(this);
    }
    void KillBoard() {
        reinterpret_cast<void (*)(LawnApp *)>(LawnApp_KillBoardAddr)(this);
    }
    void SetBoardResult(int result) {
        reinterpret_cast<void (*)(LawnApp *, int)>(LawnApp_SetBoardResultAddr)(this, result);
    }
    void ShowVSResultsScreen() {
        reinterpret_cast<void (*)(LawnApp *)>(LawnApp_ShowVSResultsScreenAddr)(this);
    }
    void ShowGameSelector() {
        reinterpret_cast<void (*)(LawnApp *)>(LawnApp_ShowGameSelectorAddr)(this);
    }
    void SetSecondPlayer(int thePlayerIndex) {
        reinterpret_cast<void (*)(LawnApp *, int)>(LawnApp_SetSecondPlayerAddr)(this, thePlayerIndex);
    }
    void PlayFoley(FoleyType theFoleyType) {
        reinterpret_cast<void (*)(LawnApp *, FoleyType)>(LawnApp_PlayFoleyAddr)(this, theFoleyType);
    }
    void PlaySample(int theSoundNum) {
        reinterpret_cast<void (*)(LawnApp *, int, bool unknown)>(LawnApp_PlaySampleAddr)(this, theSoundNum, true);
    }
    int PlayerToGamepadIndex(int thePlayerIndex) {
        return reinterpret_cast<int (*)(LawnApp *, int)>(LawnApp_PlayerToGamepadIndexAddr)(this, thePlayerIndex);
    }
    bool HasSeedType(SeedType theSeedType, int thePlayerIndex) {
        return reinterpret_cast<int (*)(LawnApp *, SeedType, int)>(LawnApp_HasSeedTypeAddr)(this, theSeedType, thePlayerIndex);
    }
    void SafeDeleteWidget(Sexy::__Widget *widget) { // vTable + 4 * 47
        reinterpret_cast<void (*)(LawnApp *, Sexy::__Widget *)>(LawnApp_SafeDeleteWidgetAddr)(this, widget);
    }
    LawnApp() {
        _constructor();
    };
    void Init();
    bool IsNight();
    inline bool IsIceDemo() {
        return false;
    }
    void HardwareInit();
    void DoBackToMain();
    bool CanShopLevel();
    void DoNewOptions(bool theFromGameSelector, unsigned int a3);
    int GetNumPreloadingTasks();
    void DoConfirmBackToMain(bool theIsSave);
    int TrophiesNeedForGoldSunflower();
    int GamepadToPlayerIndex(unsigned int thePlayerIndex);
    void ShowCreditScreen(bool theIsFromMainMenu);
    void OnSessionTaskFailed();
    void UpdateApp();
    void ShowAwardScreen(AwardType theAwardType);
    void KillAwardScreen();
    void LoadLevelConfiguration(int a2, int a3);
    void LoadingThreadProc();
    bool IsChallengeWithoutSeedBank();
    void TryHelpTextScreen(int theScreenPage);
    void KillSeedChooserScreen();
    bool IsIZombieLevel();
    bool IsWallnutBowlingLevel();
    bool IsAdventureMode();
    bool IsPuzzleMode();
    bool IsLittleTroubleLevel();
    bool IsScaryPotterLevel();
    bool IsSlotMachineLevel();
    bool IsArtChallenge();
    bool IsSquirrelLevel();
    bool IsSurvivalEndless(GameMode theGameMode);
    bool IsWhackAZombieLevel();
    bool IsVSMode();
    bool IsCoopMode();
    bool IsTwinSunbankMode();
    bool IsFinalBossLevel();
    void LoadAddonImages();
    void LoadAddonSounds();
    Sexy::Image *GetImageByFileName(const char *theFileName);
    int GetSoundByFileName(const char *theFileName);
    void KillDialog(Dialogs theId);
    void Load(const char *theGroupName);
    void DoConvertImitaterImages();
    int GetSeedsAvailable(bool theIsZombieChooser);
    bool GrantAchievement(AchievementId theAchievementId);
    void SetFoleyVolume(FoleyType theFoleyType, double theVolume);
    void ShowLeaderboards();
    void KillLeaderboards();
    void ShowZombatarScreen();
    void KillZombatarScreen();
    void SetHouseReanim(Reanimation *theHouseAnim);
    void LoadZombatarResources();
    PottedPlant *GetPottedPlantByIndex(int thePottedPlantIndex);
    void ClearSecondPlayer();
    bool Is3DAccelerated() {
        // 修复关闭3D加速后MV错位
        return mNewIs3DAccelerated || (mCreditScreen != nullptr);
    }

    void Set3DAccelerated(bool isAccelerated) {
        mNewIs3DAccelerated = isAccelerated;
        mPlayerInfo->mIs3DAcceleratedClosed = !isAccelerated;
    }
    void HandleTcpClientMessage(void *buf, ssize_t bufSize);
    void HandleTcpServerMessage(void *buf, ssize_t bufSize);
    void UpdateFrames();
    void ShowSeedChooserScreen();
    void ShowZombieChooserScreen();
    void KillZombieChooserScreen();

protected:
    friend void InitHookFunction();

    void _constructor();
};

/***************************************************************************************************************/
inline bool disableShop;
inline bool doCheatDialog;     // 菜单DoCheatDialog
inline bool doCheatCodeDialog; // 菜单DoCheatCodeDialog
inline int gPingNetPingPongCounter = 0;
inline int gPingNetDelayCounter = -1;
inline int gNetDelayNow = 0;

inline void (*old_LawnApp_ClearSecondPlayer)(LawnApp *lawnApp);

inline void (*old_LawnApp_DoBackToMain)(LawnApp *lawnApp);

inline void (*old_LawnApp_ShowAwardScreen)(LawnApp *lawnApp, AwardType a2);

inline void (*old_LawnApp_KillAwardScreen)(LawnApp *lawnApp);

inline int (*old_LawnApp_GamepadToPlayerIndex)(LawnApp *lawnApp, unsigned int a2);

inline void (*old_LawnApp_UpDateApp)(LawnApp *lawnApp);

inline bool (*old_LawnApp_CanShopLevel)(LawnApp *lawnApp);

inline void (*old_LawnApp_ShowCreditScreen)(LawnApp *lawnApp, bool isFromMainMenu);

inline void (*old_LawnApp_LoadLevelConfiguration)(LawnApp *lawnApp, int a2, int a3);

inline void (*old_LawnApp_LawnApp)(LawnApp *lawnApp);

inline void (*old_LawnApp_Init)(LawnApp *lawnApp);

inline void (*old_LawnApp_LoadingThreadProc)(LawnApp *lawnApp);

inline bool (*old_LawnApp_IsChallengeWithoutSeedBank)(LawnApp *lawnApp);

inline int (*old_LawnApp_GetSeedsAvailable)(LawnApp *lawnApp, bool isZombieChooser);

inline void (*old_LawnApp_HardwareInit)(LawnApp *lawnApp);

inline int (*old_LawnApp_GetNumPreloadingTasks)(LawnApp *lawnApp);

inline bool (*old_LawnApp_IsNight)(LawnApp *lawnApp);

inline bool (*old_LawnApp_HasSeedType)(LawnApp *lawnApp, SeedType theSeedType, int playerIndex);

inline void (*old_LawnApp_DoNewOptions)(LawnApp *lawnApp, bool a2, unsigned int a3);

inline void (*old_LawnApp_UpdateFrames)(LawnApp *lawnApp);

inline void (*old_LawnApp_KillSeedChooserScreen)(LawnApp *lawnApp);

#endif // PVZ_LAWN_LAWN_APP_H
