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

#include "PvZ/Lawn/Widget/ChallengeScreen.h"
#include "Homura/Logger.h"
#include "PvZ/Lawn/Board/Challenge.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/System/Music.h"
#include "PvZ/Lawn/Widget/GameButton.h"
#include "PvZ/Lawn/Widget/VSSetupMenu.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Common/TodCommon.h"
#include "PvZ/TodLib/Common/TodStringFile.h"

using namespace Sexy;

ChallengeDefinition gChallengeDefs[200] = {
    {GameMode::GAMEMODE_SURVIVAL_NORMAL_STAGE_1, 0, ChallengePage::CHALLENGE_PAGE_SURVIVAL, 0, 0, "[SURVIVAL_DAY_NORMAL]"},
    {GameMode::GAMEMODE_SURVIVAL_NORMAL_STAGE_2, 1, ChallengePage::CHALLENGE_PAGE_SURVIVAL, 0, 1, "[SURVIVAL_NIGHT_NORMAL]"},
    {GameMode::GAMEMODE_SURVIVAL_NORMAL_STAGE_3, 2, ChallengePage::CHALLENGE_PAGE_SURVIVAL, 0, 2, "[SURVIVAL_POOL_NORMAL]"},
    {GameMode::GAMEMODE_SURVIVAL_NORMAL_STAGE_4, 3, ChallengePage::CHALLENGE_PAGE_SURVIVAL, 0, 3, "[SURVIVAL_FOG_NORMAL]"},
    {GameMode::GAMEMODE_SURVIVAL_NORMAL_STAGE_5, 4, ChallengePage::CHALLENGE_PAGE_SURVIVAL, 0, 4, "[SURVIVAL_ROOF_NORMAL]"},
    {GameMode::GAMEMODE_SURVIVAL_HARD_STAGE_1, 5, ChallengePage::CHALLENGE_PAGE_SURVIVAL, 1, 0, "[SURVIVAL_DAY_HARD]"},
    {GameMode::GAMEMODE_SURVIVAL_HARD_STAGE_2, 6, ChallengePage::CHALLENGE_PAGE_SURVIVAL, 1, 1, "[SURVIVAL_NIGHT_HARD]"},
    {GameMode::GAMEMODE_SURVIVAL_HARD_STAGE_3, 7, ChallengePage::CHALLENGE_PAGE_SURVIVAL, 1, 2, "[SURVIVAL_POOL_HARD]"},
    {GameMode::GAMEMODE_SURVIVAL_HARD_STAGE_4, 8, ChallengePage::CHALLENGE_PAGE_SURVIVAL, 1, 3, "[SURVIVAL_FOG_HARD]"},
    {GameMode::GAMEMODE_SURVIVAL_HARD_STAGE_5, 9, ChallengePage::CHALLENGE_PAGE_SURVIVAL, 1, 4, "[SURVIVAL_ROOF_HARD]"},
    {GameMode::GAMEMODE_SURVIVAL_ENDLESS_STAGE_1, 10, ChallengePage::CHALLENGE_PAGE_SURVIVAL, 2, 0, "[SURVIVAL_DAY_ENDLESS]"},
    {GameMode::GAMEMODE_SURVIVAL_ENDLESS_STAGE_2, 11, ChallengePage::CHALLENGE_PAGE_SURVIVAL, 2, 1, "[SURVIVAL_NIGHT_ENDLESS]"},
    {GameMode::GAMEMODE_SURVIVAL_ENDLESS_STAGE_3, 12, ChallengePage::CHALLENGE_PAGE_SURVIVAL, 2, 2, "[SURVIVAL_POOL_ENDLESS]"},
    {GameMode::GAMEMODE_SURVIVAL_ENDLESS_STAGE_4, 13, ChallengePage::CHALLENGE_PAGE_SURVIVAL, 2, 3, "[SURVIVAL_FOG_ENDLESS]"},
    {GameMode::GAMEMODE_SURVIVAL_ENDLESS_STAGE_5, 14, ChallengePage::CHALLENGE_PAGE_SURVIVAL, 2, 4, "[SURVIVAL_ROOF_ENDLESS]"},
    {GameMode::GAMEMODE_CHALLENGE_WAR_AND_PEAS, 0, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 0, 0, "[WAR_AND_PEAS]"},
    {GameMode::GAMEMODE_CHALLENGE_WALLNUT_BOWLING, 1, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 0, 1, "[WALL_NUT_BOWLING]"},
    {GameMode::GAMEMODE_CHALLENGE_SLOT_MACHINE, 2, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 0, 2, "[SLOT_MACHINE]"},
    {GameMode::GAMEMODE_CHALLENGE_HEAVY_WEAPON, 36, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 0, 3, "[HEAVY_WEAPON]"},
    {GameMode::GAMEMODE_CHALLENGE_BEGHOULED, 4, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 0, 4, "[BEGHOULED]"},
    {GameMode::GAMEMODE_CHALLENGE_INVISIGHOUL, 5, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 1, 0, "[INVISIGHOUL]"},
    {GameMode::GAMEMODE_CHALLENGE_SEEING_STARS, 6, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 1, 1, "[SEEING_STARS]"},
    {GameMode::GAMEMODE_CHALLENGE_ZOMBIQUARIUM, 7, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 1, 2, "[ZOMBIQUARIUM]"},
    {GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST, 8, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 1, 3, "[BEGHOULED_TWIST]"},
    {GameMode::GAMEMODE_CHALLENGE_LITTLE_TROUBLE, 9, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 1, 4, "[LITTLE_TROUBLE]"},
    {GameMode::GAMEMODE_CHALLENGE_PORTAL_COMBAT, 10, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 2, 0, "[PORTAL_COMBAT]"},
    {GameMode::GAMEMODE_CHALLENGE_COLUMN, 11, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 2, 1, "[COLUMN_AS_YOU_SEE_EM]"},
    {GameMode::GAMEMODE_CHALLENGE_BOBSLED_BONANZA, 12, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 2, 2, "[BOBSLED_BONANZA]"},
    {GameMode::GAMEMODE_CHALLENGE_SPEED, 13, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 2, 3, "[ZOMBIES_ON_SPEED]"},
    {GameMode::GAMEMODE_CHALLENGE_WHACK_A_ZOMBIE, 14, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 2, 4, "[WHACK_A_ZOMBIE]"},
    {GameMode::GAMEMODE_CHALLENGE_LAST_STAND, 15, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 3, 0, "[LAST_STAND]"},
    {GameMode::GAMEMODE_CHALLENGE_WAR_AND_PEAS_2, 16, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 3, 1, "[WAR_AND_PEAS_2]"},
    {GameMode::GAMEMODE_CHALLENGE_WALLNUT_BOWLING_2, 17, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 3, 2, "[WALL_NUT_BOWLING_EXTREME]"},
    {GameMode::GAMEMODE_CHALLENGE_POGO_PARTY, 18, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 3, 3, "[POGO_PARTY]"},
    {GameMode::GAMEMODE_CHALLENGE_FINAL_BOSS, 19, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 3, 4, "[FINAL_BOSS]"},
    {GameMode::GAMEMODE_CHALLENGE_ART_CHALLENGE_WALLNUT, 20, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 4, 0, "[ART_CHALLENGE_WALL_NUT]"},
    {GameMode::GAMEMODE_CHALLENGE_SUNNY_DAY, 21, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 4, 1, "[SUNNY_DAY]"},
    {GameMode::GAMEMODE_CHALLENGE_RESODDED, 22, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 4, 2, "[UNSODDED]"},
    {GameMode::GAMEMODE_CHALLENGE_BIG_TIME, 23, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 4, 3, "[BIG_TIME]"},
    {GameMode::GAMEMODE_CHALLENGE_ART_CHALLENGE_SUNFLOWER, 24, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 4, 4, "[ART_CHALLENGE_SUNFLOWER]"},
    {GameMode::GAMEMODE_CHALLENGE_AIR_RAID, 25, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 5, 0, "[AIR_RAID]"},
    {GameMode::GAMEMODE_CHALLENGE_ICE, 6, ChallengePage::CHALLENGE_PAGE_LIMBO, 5, 1, "[ICE_LEVEL]"},
    {GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN, 7, ChallengePage::CHALLENGE_PAGE_LIMBO, 5, 2, "[ZEN_GARDEN]"},
    {GameMode::GAMEMODE_CHALLENGE_HIGH_GRAVITY, 26, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 5, 3, "[HIGH_GRAVITY]"},
    {GameMode::GAMEMODE_CHALLENGE_GRAVE_DANGER, 27, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 5, 4, "[GRAVE_DANGER]"},
    {GameMode::GAMEMODE_CHALLENGE_SHOVEL, 28, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 6, 0, "[CAN_YOU_DIG_IT]"},
    {GameMode::GAMEMODE_CHALLENGE_STORMY_NIGHT, 29, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 6, 1, "[DARK_STORMY_NIGHT]"},
    {GameMode::GAMEMODE_CHALLENGE_BUNGEE_BLITZ, 30, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 6, 2, "[BUNGEE_BLITZ]"},
    {GameMode::GAMEMODE_CHALLENGE_SQUIRREL, 31, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 6, 3, "[SQUIRREL]"},
    {GameMode::GAMEMODE_TREE_OF_WISDOM, 10, ChallengePage::CHALLENGE_PAGE_LIMBO, 0, 0, "[TREE_OF_WISDOM]"},
    {GameMode::GAMEMODE_SCARY_POTTER_1, 32, ChallengePage::CHALLENGE_PAGE_PUZZLE, 0, 0, "[SCARY_POTTER_1]"},
    {GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_1, 34, ChallengePage::CHALLENGE_PAGE_PUZZLE, 0, 1, "[I_ZOMBIE_1]"},
    {GameMode::GAMEMODE_SCARY_POTTER_2, 32, ChallengePage::CHALLENGE_PAGE_PUZZLE, 0, 2, "[SCARY_POTTER_2]"},
    {GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_2, 34, ChallengePage::CHALLENGE_PAGE_PUZZLE, 0, 3, "[I_ZOMBIE_2]"},
    {GameMode::GAMEMODE_SCARY_POTTER_3, 32, ChallengePage::CHALLENGE_PAGE_PUZZLE, 0, 4, "[SCARY_POTTER_3]"},
    {GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_3, 34, ChallengePage::CHALLENGE_PAGE_PUZZLE, 1, 0, "[I_ZOMBIE_3]"},
    {GameMode::GAMEMODE_SCARY_POTTER_4, 32, ChallengePage::CHALLENGE_PAGE_PUZZLE, 1, 1, "[SCARY_POTTER_4]"},
    {GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_4, 34, ChallengePage::CHALLENGE_PAGE_PUZZLE, 1, 2, "[I_ZOMBIE_4]"},
    {GameMode::GAMEMODE_SCARY_POTTER_5, 32, ChallengePage::CHALLENGE_PAGE_PUZZLE, 1, 3, "[SCARY_POTTER_5]"},
    {GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_5, 34, ChallengePage::CHALLENGE_PAGE_PUZZLE, 1, 4, "[I_ZOMBIE_5]"},
    {GameMode::GAMEMODE_SCARY_POTTER_6, 32, ChallengePage::CHALLENGE_PAGE_PUZZLE, 2, 0, "[SCARY_POTTER_6]"},
    {GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_6, 34, ChallengePage::CHALLENGE_PAGE_PUZZLE, 2, 1, "[I_ZOMBIE_6]"},
    {GameMode::GAMEMODE_SCARY_POTTER_7, 32, ChallengePage::CHALLENGE_PAGE_PUZZLE, 2, 2, "[SCARY_POTTER_7]"},
    {GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_7, 34, ChallengePage::CHALLENGE_PAGE_PUZZLE, 2, 2, "[I_ZOMBIE_7]"},
    {GameMode::GAMEMODE_SCARY_POTTER_8, 32, ChallengePage::CHALLENGE_PAGE_PUZZLE, 2, 4, "[SCARY_POTTER_8]"},
    {GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_8, 34, ChallengePage::CHALLENGE_PAGE_PUZZLE, 3, 0, "[I_ZOMBIE_8]"},
    {GameMode::GAMEMODE_SCARY_POTTER_9, 32, ChallengePage::CHALLENGE_PAGE_PUZZLE, 3, 1, "[SCARY_POTTER_9]"},
    {GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_9, 34, ChallengePage::CHALLENGE_PAGE_PUZZLE, 3, 2, "[I_ZOMBIE_9]"},
    {GameMode::GAMEMODE_SCARY_POTTER_ENDLESS, 33, ChallengePage::CHALLENGE_PAGE_PUZZLE, 3, 3, "[SCARY_POTTER_ENDLESS]"},
    {GameMode::GAMEMODE_PUZZLE_I_ZOMBIE_ENDLESS, 35, ChallengePage::CHALLENGE_PAGE_PUZZLE, 3, 4, "[I_ZOMBIE_ENDLESS]"},
    {GameMode::GAMEMODE_UPSELL, 10, ChallengePage::CHALLENGE_PAGE_LIMBO, 0, 0, "[UPSELL]"},
    {GameMode::GAMEMODE_INTRO, 10, ChallengePage::CHALLENGE_PAGE_LIMBO, 0, 1, "[INTRO]"},
    {GameMode::GAMEMODE_MULTI_PLAYER, 10, ChallengePage::CHALLENGE_PAGE_LIMBO, 0, 2, ""},
    {GameMode::GAMEMODE_MP_VS_HIDE, 10, ChallengePage::CHALLENGE_PAGE_LIMBO, 0, 3, ""},
    {GameMode::GAMEMODE_MP_VS, 10, ChallengePage::CHALLENGE_PAGE_LIMBO, 0, 0, ""},
    {GameMode::GAMEMODE_MP_VS_COOP, 18, ChallengePage::CHALLENGE_PAGE_LIMBO, 0, 1, "[COOP]"},
    {GameMode::GAMEMODE_MP_VS_UNKONWN, 18, ChallengePage::CHALLENGE_PAGE_LIMBO, 0, 2, ""},
    {GameMode::GAMEMODE_TWO_PLAYER_COOP_DAY, 0, ChallengePage::CHALLENGE_PAGE_COOP, 0, 0, "[COOP_1]"},
    {GameMode::GAMEMODE_TWO_PLAYER_COOP_NIGHT, 1, ChallengePage::CHALLENGE_PAGE_COOP, 0, 1, "[COOP_2]"},
    {GameMode::GAMEMODE_TWO_PLAYER_COOP_POOL, 2, ChallengePage::CHALLENGE_PAGE_COOP, 0, 2, "[COOP_3]"},
    {GameMode::GAMEMODE_TWO_PLAYER_COOP_ROOF, 4, ChallengePage::CHALLENGE_PAGE_COOP, 0, 3, "[COOP_4]"},
    {GameMode::GAMEMODE_TWO_PLAYER_COOP_BOWLING, 1, ChallengePage::CHALLENGE_PAGE_COOP, 0, 4, "[COOP_BOWLING]"},
    {GameMode::GAMEMODE_TWO_PLAYER_COOP_DAY_HARD, 5, ChallengePage::CHALLENGE_PAGE_COOP, 1, 0, "[COOP_HARD_1]"},
    {GameMode::GAMEMODE_TWO_PLAYER_COOP_NIGHT_HARD, 6, ChallengePage::CHALLENGE_PAGE_COOP, 1, 1, "[COOP_HARD_2]"},
    {GameMode::GAMEMODE_TWO_PLAYER_COOP_POOL_HARD, 7, ChallengePage::CHALLENGE_PAGE_COOP, 1, 2, "[COOP_HARD_3]"},
    {GameMode::GAMEMODE_TWO_PLAYER_COOP_ROOF_HARD, 9, ChallengePage::CHALLENGE_PAGE_COOP, 1, 3, "[COOP_HARD_4]"},
    {GameMode::GAMEMODE_TWO_PLAYER_COOP_BOSS, 19, ChallengePage::CHALLENGE_PAGE_COOP, 1, 4, "[COOP_FINAL_BOSS]"},
    {GameMode::GAMEMODE_TWO_PLAYER_COOP_ENDLESS, 12, ChallengePage::CHALLENGE_PAGE_COOP, 2, 0, "[COOP_ENDLESS]"},
    {GameMode::GAMEMODE_CHALLENGE_RAINING_SEEDS, 3, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 6, 4, "[ITS_RAINING_SEEDS]"},
    {GameMode::GAMEMODE_CHALLENGE_BUTTERED_POPCORN, 37, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 7, 0, "[BUTTERED_POPCORN]"},
    {GameMode::GAMEMODE_MP_VS_IN_PAGE, 0, ChallengePage::CHALLENGE_PAGE_VS, 0, 0, "[MP_VS_DAY]"},
    {GameMode::GAMEMODE_MP_VS_IN_PAGE, 1, ChallengePage::CHALLENGE_PAGE_VS, 0, 1, "[MP_VS_NIGHT]"},
    {GameMode::GAMEMODE_MP_VS_IN_PAGE, 2, ChallengePage::CHALLENGE_PAGE_VS, 0, 2, "[MP_VS_POOL_DAY]"},
    {GameMode::GAMEMODE_MP_VS_IN_PAGE, 3, ChallengePage::CHALLENGE_PAGE_VS, 0, 3, "[MP_VS_POOL_NIGHT]"},
    {GameMode::GAMEMODE_MP_VS_IN_PAGE, 4, ChallengePage::CHALLENGE_PAGE_VS, 0, 4, "[MP_VS_ROOF]"},
};

ChallengeDefinition gVSChallengeDefs[NUM_VS_MODES] = {
    {GameMode::GAMEMODE_MP_VS, 0, ChallengePage::CHALLENGE_PAGE_VS, 0, 0, "[MP_VS_DAY]"},
    {GameMode::GAMEMODE_MP_VS, 1, ChallengePage::CHALLENGE_PAGE_VS, 0, 1, "[MP_VS_NIGHT]"},
    {GameMode::GAMEMODE_MP_VS, 2, ChallengePage::CHALLENGE_PAGE_VS, 0, 2, "[MP_VS_POOL_DAY]"},
    {GameMode::GAMEMODE_MP_VS, 3, ChallengePage::CHALLENGE_PAGE_VS, 0, 3, "[MP_VS_POOL_NIGHT]"},
    {GameMode::GAMEMODE_MP_VS, 4, ChallengePage::CHALLENGE_PAGE_VS, 0, 4, "[MP_VS_ROOF]"},
    {GameMode::GAMEMODE_MP_VS, 0, ChallengePage::CHALLENGE_PAGE_VS, 1, 0, "[MP_VS_SHUFFLE_MODE]"},
};

void ChallengeScreen::_constructor(LawnApp *theApp, ChallengePage thePage) {
    //    Widget::_constructor();
    //
    //    auto vtableAddr = reinterpret_cast<uintptr_t>(vTableForChallengeScreenAddr);
    //    uintptr_t vptrAddr = vtableAddr + 8;
    //    vTable = reinterpret_cast<int*>(vptrAddr);
    //    uintptr_t vptrButtonListenerAddr = vtableAddr + 512;
    //    ButtonListener::mVTable = reinterpret_cast<VTable *>(vptrButtonListenerAddr);
    //
    //    mUtil = Curve1DUtil();
    //
    //    mApp = theApp;
    //    mPageIndex = thePage;
    //    mLockShakeX = 0.0f;
    //    mLockShakeY = 0.0f;
    //    mUnkFloat = 0.0f;
    //    mClip = false;
    //    mCheatEnableChallenges = false;
    //    mUnlockState = UnlockingState::UNLOCK_OFF;
    //    mUnlockStateCounter = 0;
    //    mScreenTopChallengeIndex = 0;
    //    mSelectedChallengeIndex = 0;
    //    mTotalGameInPage = 0;
    //    mUnlockChallengeIndex = -1;
    //
    //    for (int aChallengeMode = 0; aChallengeMode < NUM_CHALLENGE_MODES; aChallengeMode++) {
    //        ChallengeDefinition &aChlDef = GetChallengeDefinition(aChallengeMode);
    //        auto *aChallengeButton = new ButtonWidget_(ChallengeScreen::ChallengeScreen_Mode + aChallengeMode, this);
    //        mChallengeButtons[aChallengeMode] = aChallengeButton;
    //        aChallengeButton->mDoFinger = true;
    //        aChallengeButton->mFrameNoDraw = true;
    //        if (aChlDef.mPage == mPageIndex) {
    //            aChallengeButton->Resize(35, mTotalGameInPage * 120 + 80, 112, 65);
    //            mTotalGameInPage++;
    //            mUnk1[mTotalGameInPage] = GameMode(aChallengeMode);
    //        }
    //        if (MoreTrophiesNeeded(aChallengeMode)) {
    //            aChallengeButton->mDoFinger = false;
    //            aChallengeButton->mDisabled = true;
    //        }
    //        mUnk2[aChallengeMode] = 0;
    //    }
    //
    //    mToolTip = new ToolTipWidget();
    //    mToolTip->mCenter = true;
    //    mToolTip->mVisible = false;
    //
    //    mSelectedMode = GameMode::GAMEMODE_CHALLENGE_BUTTERED_POPCORN;
    //
    //    UpdateButtons();
    //
    //    if (mApp->mGameMode != GameMode::GAMEMODE_UPSELL || mApp->mGameScene != GameScenes::SCENE_LEVEL_INTRO) {
    //        mApp->mMusic->MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_CHOOSE_YOUR_SEEDS);
    //
    //        if (mPageIndex == CHALLENGE_PAGE_SURVIVAL) {
    //            if (mApp->mPlayerInfo->GetFlag(512)) {
    //                SetUnlockChallengeIndex(mPageIndex, false);
    //                mApp->mPlayerInfo->SetFlag(512, false);
    //            }
    //        } else if (mPageIndex == CHALLENGE_PAGE_CHALLENGE) {
    //            if (mApp->mPlayerInfo->GetFlag(64)) {
    //                SetUnlockChallengeIndex(mPageIndex, false);
    //                mApp->mPlayerInfo->SetFlag(64, false);
    //            }
    //        } else if (mPageIndex == CHALLENGE_PAGE_PUZZLE) {
    //            if (mApp->mPlayerInfo->GetFlag(128)) {
    //                SetUnlockChallengeIndex(mPageIndex, false);
    //                mApp->mPlayerInfo->SetFlag(128, false);
    //            } else if (mApp->mPlayerInfo->GetFlag(256)) {
    //                SetUnlockChallengeIndex(mPageIndex, true);
    //                mApp->mPlayerInfo->SetFlag(256, false);
    //            }
    //        }
    //    }
    //
    //    mHelpBarWidget = mApp->mHelpBarWidget;
    //    mHelpBarWidget->ClearButtons(0);
    //    mHelpBarWidget->AddButton(GamepadButton::GAMEPAD_BUTTON_A, "[BACK]", HelpBarWidget::HELP_ALIGN_NONE);
    //    mHelpBarWidget->mUnk[24] = 0;

    old_ChallengeScreen_ChallengeScreen(this, theApp, thePage);

    mBackButton = MakeNewButton(
        ChallengeScreen::ChallengeScreen_Back, this, this, "[CLOSE]", nullptr, *Sexy::IMAGE_SEEDCHOOSER_BUTTON_DISABLED, *Sexy::IMAGE_SEEDCHOOSER_BUTTON_GLOW, *Sexy::IMAGE_SEEDCHOOSER_BUTTON_GLOW);
    mBackButton->mTextOffsetX = -2;
    mBackButton->mTextOffsetY = -4;
    mBackButton->mTextDownOffsetX = 1;
    mBackButton->mTextDownOffsetY = 1;
    mBackButton->SetFont(*Sexy_FONT_DWARVENTODCRAFT18_Addr);
    (*mBackButton->mColors)[ButtonWidget::COLOR_LABEL_HILITE] = Color(0, 205, 0);
    mBackButton->Resize(800, 520, 160, 50);

    // 去除按钮对触控的遮挡
    for (auto *button : mChallengeButtons) {
        // 把按钮全部缩小至长宽为0
        button->Resize(button->mX, button->mY, 0, 0);
    }

    if (mPageIndex == ChallengePage::CHALLENGE_PAGE_VS) {
        mTotalGameInPage = NUM_VS_MODES;
        gIsVSShuffleMode = false;
        gChallengeScreenRequestState = 0;
    }
}

void ChallengeScreen::_destructor2() {
    // 删除按钮
    old_ChallengeScreen_Delete2(this);

    delete mBackButton;
}

ChallengeDefinition &GetChallengeDefinition(int theChallengeMode) {
    const int offset = 3; // 跳过 CHALLENGE_PAGE_VS 的4个废弃关卡
    for (int i = NUM_VS_MODES - 1; i >= 0; --i) {
        if (theChallengeMode + offset + i == GameMode::GAMEMODE_MP_VS) {
            return gVSChallengeDefs[abs(i - (NUM_VS_MODES - 1))];
        }
    }

    return gChallengeDefs[theChallengeMode];
}

void ChallengeScreen::Draw(Sexy::Graphics *g) {
    g->DrawImage(*Sexy::IMAGE_CHALLENGE_BACKGROUND, *LawnApp_FULLSCREEN_RECT_Addr, -60);

    pvzstl::string aTitleString = mPageIndex == CHALLENGE_PAGE_SURVIVAL ? "[PICK_AREA]"
        : mPageIndex == CHALLENGE_PAGE_PUZZLE                           ? "[SCARY_POTTER]"
        : mPageIndex == CHALLENGE_PAGE_VS                               ? "[VS_MODE]"
        : mPageIndex == CHALLENGE_PAGE_COOP                             ? "[XBOX_COOP]"
                                                                        : "[PICK_CHALLENGE]";
    TodDrawString(g, aTitleString, 400, 45, *Sexy_FONT_HOUSEOFTERROR28_Addr, Color(220, 220, 220), DS_ALIGN_CENTER);

    int aTrophiesGot = mApp->GetNumTrophies(mPageIndex);
    int aTrophiesTotal = (mPageIndex == CHALLENGE_PAGE_SURVIVAL || mPageIndex == CHALLENGE_PAGE_COOP) ? 10 : mPageIndex == CHALLENGE_PAGE_PUZZLE ? 18 : 0;
    if (mPageIndex == CHALLENGE_PAGE_CHALLENGE) {
        for (int i = 0; i < 94; ++i) {
            if (GetChallengeDefinition(i).mPage == ChallengePage::CHALLENGE_PAGE_CHALLENGE) {
                aTrophiesTotal++;
            }
        }
    }
    if (aTrophiesTotal > 0) {
        pvzstl::string aTrophyString = StrFormat(TodStringTranslate("[NUMBER_OF_TROPHIES]").c_str(), aTrophiesGot, aTrophiesTotal);
        TodDrawString(g, aTrophyString, 711, 62, *Sexy_FONT_BRIANNETOD16_Addr, Color(255, 240, 0), DS_ALIGN_CENTER);
    }
    if (mPageIndex != CHALLENGE_PAGE_VS) {
        TodDrawImageScaledF(g, *Sexy::IMAGE_TROPHY, 690.0f, 15.0f, 0.5f, 0.5f);
    }

    g->PushState();

    int scrollBarX = 760;
    int scrollBarY = 80;
    int scrollBarWidth = 40;
    int scrollBarHeight = 460;
    int scrollBarRectX = 766;
    int scrollBarRectY = 28;

    float scrollPosition = float(mScreenTopChallengeIndex);
    float scrollBarHeightFloat = float(scrollBarHeight);

    if (mScreenTopChallengeIndex == mSelectedChallengeIndex) {
        // 未滚动时的状态
        scrollBarHeightFloat = 448.0f;
        scrollBarHeight = 448;
        scrollBarY = 86;
    } else {
        // 滚动时的动画效果
        scrollPosition = TodAnimateCurveFloatTime(0.0f, 0.15f, mUnkFloat, scrollPosition, mSelectedChallengeIndex, TodCurves::CURVE_LINEAR);
        scrollBarHeight = scrollBarHeight - 12;
        scrollBarY = scrollBarY + 6;
        scrollBarRectX = scrollBarX + 6;
        scrollBarRectY = scrollBarWidth - 12;
        scrollBarHeightFloat = float(scrollBarHeight);
    }

    int thumbPosition = int((scrollPosition / mTotalGameInPage) * scrollBarHeightFloat);
    int thumbHeight = int(scrollBarHeightFloat * (5.0f / mTotalGameInPage));

    int thumbY = scrollBarY + thumbPosition;
    int actualThumbHeight = (thumbHeight > scrollBarHeight) ? scrollBarHeight : thumbHeight;

    // 设置裁剪区域并绘制滚动条
    g->ClipRect(scrollBarRectX, scrollBarY, scrollBarRectY, scrollBarHeight);

    Color scrollBarBgColor(0, 128);
    g->SetColor(scrollBarBgColor);
    g->FillRect(Rect(scrollBarX + 6, scrollBarY + 6, scrollBarWidth - 12, scrollBarHeight - 12));

    Color scrollBarThumbColor(140, 140, 140, 255);
    g->SetColor(scrollBarThumbColor);
    g->FillRect(Rect(scrollBarRectX, thumbY, scrollBarRectY, actualThumbHeight));

    g->ClearClipRect();
    g->SetColorizeImages(false);
    g->DrawImageBox(Rect(scrollBarX, scrollBarY, scrollBarWidth, scrollBarHeight), *Sexy::IMAGE_DLG_SELECTORFRAME);

    g->PopState();

    g->PushState();
    g->ClipRect(-20, 80, 1000, 475);
    g->TranslateF(0.0f, -(scrollPosition * 120.0f));

    if (mTotalGameInPage > 0) {
        float *unkFloatPtr = &mUnkFloat;
        for (int aChallengeMode = 0; aChallengeMode < mTotalGameInPage; ++aChallengeMode) {
            int aChallengeId = *reinterpret_cast<int *>(unkFloatPtr + 1);
            DrawButton(g, aChallengeId, aChallengeMode);
            unkFloatPtr += 1;
        }
    }

    g->ClearClipRect();

    if (mToolTip) {
        mToolTip->Draw(g);
    }

    g->PopState();


    if (mPageIndex == CHALLENGE_PAGE_VS) {
        //        if (gTcpConnected) {
        //            if (gNetDelayNow == 0) {
        //                g->DrawString(TodStringTranslate("[VS_STATUS_IN_ROOM]"), 370, -20);
        //            } else {
        //                pvzstl::string fmt = TodStringTranslate("[VS_STATUS_IN_ROOM_MS_FMT]");
        //                g->DrawString(StrFormat(fmt.c_str(), gNetDelayNow * 10), 370, -20);
        //            }
        //        } else if (gTcpClientSocket >= 0) {
        //            if (gNetDelayNow == 0) {
        //                g->DrawString(TodStringTranslate("[VS_STATUS_HOST]"), 380, -20);
        //            } else {
        //                pvzstl::string fmt = TodStringTranslate("[VS_STATUS_HOST_MS_FMT]");
        //                g->DrawString(StrFormat(fmt.c_str(), gNetDelayNow * 10), 380, -20);
        //            }
        //        }

        if (gChallengeScreenRequestState != 0) {
            // ======================
            // 我是 guest：已提醒房主...
            // (gTcpConnected == true 代表我作为 client 连接到 host)
            // ======================
            static constexpr const char *names[] = {
                "[MP_VS_DAY]",
                "[MP_VS_NIGHT]",
                "[MP_VS_POOL_DAY]",
                "[MP_VS_POOL_NIGHT]",
                "[MP_VS_ROOF]",
                "[MP_VS_SHUFFLE_MODE]",
            };

            if (gTcpConnected) {
                pvzstl::string fmt = TodStringTranslate("[CHALLENGESCREEN_TIP_REMIND_HOST_FMT]");
                pvzstl::string name = TodStringTranslate(names[gChallengeScreenRequestState - 68]);
                TodDrawString(g, StrFormat(fmt.c_str(), name.c_str()), 140, 620, *Sexy_FONT_HOUSEOFTERROR28_Addr, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
            }

            // ======================
            // 我是 host：对方想玩/想要...
            // (gTcpClientSocket >= 0 表示我作为 host 收到了 client 连接)
            // ======================
            if (gTcpClientSocket >= 0) {
                pvzstl::string fmt = TodStringTranslate("[CHALLENGESCREEN_TIP_OPPONENT_WANTS_PLAY_FMT]");
                pvzstl::string name = TodStringTranslate(names[gChallengeScreenRequestState - 68]);
                TodDrawString(g, StrFormat(fmt.c_str(), name.c_str()), 140, 620, *Sexy_FONT_HOUSEOFTERROR28_Addr, Color(255, 255, 153, 255), DrawStringJustification::DS_ALIGN_LEFT);
            }
        }
    }
}

void ChallengeScreen::Update() {
    // 记录当前游戏状态
    old_ChallengeScreen_Update(this);

    if (mPageIndex == ChallengePage::CHALLENGE_PAGE_VS) {

        if (mConnectDialog == nullptr && !gTcpConnected && gTcpClientSocket < 0) {
            mConnectDialog = new WaitForSecondPlayerDialog(mApp);
            mApp->AddDialog(mConnectDialog);

            int aButtonId = mConnectDialog->WaitForResult(true);
            if (aButtonId == WaitForSecondPlayerDialog::WaitForSecondPlayerDialog_Back) {
                mApp->KillBoard();
                mApp->ShowGameSelector();
            }
        }
    }
}

void ChallengeScreen::AddedToManager(WidgetManager *theWidgetManager) {
    // 记录当前游戏状态
    old_ChallengeScreen_AddedToManager(this, theWidgetManager);

    AddWidget(mBackButton);
}

void ChallengeScreen::RemovedFromManager(WidgetManager *theWidgetManager) {
    // 记录当前游戏状态
    old_ChallengeScreen_RemovedFromManager(this, theWidgetManager);

    RemoveWidget(mBackButton);
}

void ChallengeScreen::ButtonPress(int theButtonId) {
    // 空函数替换，去除原有的点击进入关卡的功能
}

void ChallengeScreen::ButtonDepress(int theId) {


    if (theId == ChallengeScreen::ChallengeScreen_Back) {
        mApp->KillChallengeScreen();
        mApp->DoBackToMain();
        return;
    }

    int aChallengeMode = theId - ChallengeScreen::ChallengeScreen_Mode;
    if (aChallengeMode >= 0 && aChallengeMode < NUM_CHALLENGE_MODES) {
        mApp->KillChallengeScreen();
        mApp->PreNewGame(GameMode(aChallengeMode + 2), true);
    }

    int aPageIndex = theId - ChallengeScreen::ChallengeScreen_Page;
    if (aPageIndex >= 0 && aPageIndex < MAX_CHALLANGE_PAGES) {
        mPageIndex = (ChallengePage)aPageIndex;
        UpdateButtons();
    }
}

void ChallengeScreen::UpdateButtons() {
    // 空函数替换，去除默认选取第一个游戏的功能
}

namespace {
int gChallengeScreenTouchDownX;
int gChallengeScreenTouchDownY;
int gChallengeItemHeight;
int gChallengeScreenGameIndex;
bool gChallengeItemMoved;
bool gTouchOutSide;

constexpr int mPageTop = 75;
constexpr int mPageBottom = 555;
} // namespace

void ChallengeScreen::MouseDown(int x, int y, int theClickCount) {
    if (y > mPageBottom || y < mPageTop) {
        gTouchOutSide = true;
    }
    gChallengeScreenTouchDownX = x;
    gChallengeScreenTouchDownY = y;
    gChallengeItemHeight = (*Sexy_IMAGE_CHALLENGE_NAME_BACK_Addr)->GetHeight() + 2; // 2为缝隙大小

    gChallengeScreenGameIndex = mScreenTopChallengeIndex;

    // int totalGamesInThisPage = a[376];//如果这个值是33
    // int currentSelectedGameIndex = ChallengeScreen_GetCurrentSelectedGameIndex(
    // a);//这里取值就是0~32。种子雨是32。

    // int firstGameInPageIndex = a->mScreenTopChallengeIndex;
    // int firstGameInPageIndex2 = a[186];
    // a->mSelectedMode = a[currentSelectedGameIndex + 1 + 188];//向下移动绿色光标，不可循环滚动
    // a->mSelectedMode = a[currentSelectedGameIndex - 1 + 188];//向上移动绿色光标，不可循环滚动

    // LOGD("dOWN:%d %d %d %d", x, y, firstGameInPageIndex, firstGameInPageIndex2);
}

void ChallengeScreen::MouseDrag(int x, int y) {
    if (gTouchOutSide)
        return;
    int triggerHeight = gChallengeItemHeight / 2; // 调节此处以修改小游戏列表的滚动速度。滚动太快就会有BUG，好烦。
    if (gChallengeScreenTouchDownY - y > triggerHeight) {
        int totalGamesInThisPage = mTotalGameInPage;
        gChallengeScreenGameIndex += 1;
        gChallengeScreenTouchDownY -= triggerHeight;
        int gameIndexToScroll = gChallengeScreenGameIndex >= totalGamesInThisPage - 4 ? totalGamesInThisPage - 4 : gChallengeScreenGameIndex;
        SetScrollTarget(gameIndexToScroll);
        // ChallengeScreen_UpdateButtons(a);
        gChallengeItemMoved = true;
    } else if (y - gChallengeScreenTouchDownY > triggerHeight) {
        gChallengeScreenGameIndex -= 1;
        gChallengeScreenTouchDownY += triggerHeight;
        int gameIndexToScroll = gChallengeScreenGameIndex <= 0 ? 0 : gChallengeScreenGameIndex;
        SetScrollTarget(gameIndexToScroll);
        // ChallengeScreen_UpdateButtons(a);
        gChallengeItemMoved = true;
    }
}

void ChallengeScreen::MouseUp(int x, int y) {
    if (!gTouchOutSide && !gChallengeItemMoved) {
        int gameIndex = mScreenTopChallengeIndex + (y - mPageTop) / gChallengeItemHeight;
        if (mSelectedMode == mUnk1[gameIndex]) {
            KeyDown(Sexy::KEYCODE_RETURN);
        } else {
            mApp->PlaySample(*Sexy_SOUND_BUTTONCLICK_Addr);
            if (gTcpConnected) {
                // 房客
                U16_Event event = {{EventType::EVENT_CLIENT_CHALLENGESCREEN_SELECT_MODE}, uint16_t(mUnk1[gameIndex])};
                netplay::PutEvent(event);
                gChallengeScreenRequestState = mUnk1[gameIndex];
            } else if (gTcpClientSocket >= 0) {
                // 房主
                mSelectedMode = mUnk1[gameIndex];
                U16_Event event = {{EventType::EVENT_SERVER_CHALLENGESCREEN_SELECT_MODE}, uint16_t(mSelectedMode)};
                netplay::PutEvent(event);
            } else {
                // 单机
                mSelectedMode = mUnk1[gameIndex];
            }
        }
    }
    gTouchOutSide = false;
    gChallengeItemMoved = false;
}

void ChallengeScreen::KeyDown(Sexy::KeyCode theKey) {
    if (theKey == Sexy::KEYCODE_RETURN) {
        // 更新对战战场选择
        if (mPageIndex == ChallengePage::CHALLENGE_PAGE_VS) {

            if (gChallengeScreenRequestState == mSelectedMode) {
                gChallengeScreenRequestState = 0;
            }

            if (gTcpConnected) {
                U16_Event event = {{EventType::EVENT_CLIENT_CHALLENGESCREEN_SELECT_MODE}, uint16_t(mSelectedMode)};
                netplay::PutEvent(event);
                gChallengeScreenRequestState = mSelectedMode;
                return;
            }

            if (gTcpClientSocket >= 0) {
                U16_Event event = {{EventType::EVENT_SERVER_CHALLENGESCREEN_BUTTON_DEPRESS}, uint16_t(mSelectedMode)};
                netplay::PutEvent(event);
            }


            switch (mSelectedMode) {
                case GAMEMODE_MP_VS_DAY:
                    gVSBackground = BackgroundType::BACKGROUND_1_DAY;
                    break;
                case GAMEMODE_MP_VS_NIGHT:
                    gVSBackground = BackgroundType::BACKGROUND_2_NIGHT;
                    break;
                case GAMEMODE_MP_VS_POOL_DAY:
                    gVSBackground = BackgroundType::BACKGROUND_3_POOL;
                    break;
                case GAMEMODE_MP_VS_POOL_NIGHT:
                    gVSBackground = BackgroundType::BACKGROUND_4_FOG;
                    break;
                case GAMEMODE_MP_VS_ROOF:
                    gVSBackground = BackgroundType::BACKGROUND_5_ROOF;
                    break;
                case GAMEMODE_MP_VS_SHUFFLE_MODE:
                    gVSBackground = BackgroundType::BACKGROUND_1_DAY;
                    gIsVSShuffleMode = true;
                    break;
                default:
                    break;
            }
        }
    }

    old_ChallengeScreen_KeyDown(this, theKey);
}

size_t ChallengeScreen::getClientEventSize(EventType type) {
    switch (type) {
        // --- 触摸相关事件 ---
        case EVENT_CLIENT_CHALLENGESCREEN_SELECT_MODE:
            return sizeof(U16_Event);
        default:
            return sizeof(BaseEvent);
    }
}


void ChallengeScreen::processClientEvent(void *buf, ssize_t bufSize) {
    BaseEvent *event = (BaseEvent *)buf;
    LOG_DEBUG("TYPE:{}", (int)event->type);
    switch (event->type) {
        case EVENT_CLIENT_CHALLENGESCREEN_SELECT_MODE: {
            U16_Event *eventButtonDepress = (U16_Event *)event;
            gChallengeScreenRequestState = eventButtonDepress->data;
        } break;

        default:
            break;
    }
}

size_t ChallengeScreen::getServerEventSize(EventType type) {
    switch (type) {
        // --- 触摸相关事件 ---
        case EVENT_SERVER_CHALLENGESCREEN_BUTTON_DEPRESS:
            return sizeof(U16_Event);
        case EVENT_SERVER_CHALLENGESCREEN_SELECT_MODE:
            return sizeof(U16_Event);
        default:
            return sizeof(BaseEvent);
    }
}

void ChallengeScreen::processServerEvent(void *buf, ssize_t bufSize) {
    BaseEvent *event = (BaseEvent *)buf;
    LOG_DEBUG("TYPE:{}", (int)event->type);
    switch (event->type) {
        case EVENT_SERVER_CHALLENGESCREEN_BUTTON_DEPRESS: {
            U16_Event *event1 = (U16_Event *)event;
            int theId = event1->data;
            LOG_DEBUG("theId={}", theId);
            gTcpConnected = false;
            mSelectedMode = GameMode(theId);
            KeyDown(Sexy::KEYCODE_RETURN);
            gTcpConnected = true;
        } break;
        case EVENT_SERVER_CHALLENGESCREEN_SELECT_MODE: {
            U16_Event *event1 = (U16_Event *)event;
            int theId = event1->data;
            mSelectedMode = GameMode(theId);
        } break;

        default:
            break;
    }
}
