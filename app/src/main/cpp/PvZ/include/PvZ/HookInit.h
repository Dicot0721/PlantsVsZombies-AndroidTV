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

#ifndef PVZ_HOOK_INIT_H
#define PVZ_HOOK_INIT_H

#include "Homura/HookFunc.h"
#include "Misc.h"
#include "PvZ/Android/IntroVideo.h"
#include "PvZ/Android/OpenSL.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Board/Challenge.h"
#include "PvZ/Lawn/Board/Coin.h"
#include "PvZ/Lawn/Board/CursorObject.h"
#include "PvZ/Lawn/Board/CutScene.h"
#include "PvZ/Lawn/Board/GridItem.h"
#include "PvZ/Lawn/Board/Plant.h"
#include "PvZ/Lawn/Board/Projectile.h"
#include "PvZ/Lawn/Board/SeedBank.h"
#include "PvZ/Lawn/Board/SeedPacket.h"
#include "PvZ/Lawn/Board/ZenGarden.h"
#include "PvZ/Lawn/Board/Zombie.h"
#include "PvZ/Lawn/GamepadControls.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/System/Music.h"
#include "PvZ/Lawn/System/PoolEffect.h"
#include "PvZ/Lawn/System/ReanimationLawn.h"
#include "PvZ/Lawn/System/SaveGame.h"
#include "PvZ/Lawn/Widget/AlmanacDialog.h"
#include "PvZ/Lawn/Widget/AwardScreen.h"
#include "PvZ/Lawn/Widget/ChallengeScreen.h"
#include "PvZ/Lawn/Widget/ConfirmBackToMainDialog.h"
#include "PvZ/Lawn/Widget/CreditScreen.h"
#include "PvZ/Lawn/Widget/HelpOptionsDialog.h"
#include "PvZ/Lawn/Widget/HelpTextScreen.h"
#include "PvZ/Lawn/Widget/HouseChooserDialog.h"
#include "PvZ/Lawn/Widget/ImitaterDialog.h"
#include "PvZ/Lawn/Widget/MailScreen.h"
#include "PvZ/Lawn/Widget/MainMenu.h"
#include "PvZ/Lawn/Widget/SeedChooserScreen.h"
#include "PvZ/Lawn/Widget/SettingsDialog.h"
#include "PvZ/Lawn/Widget/StoreScreen.h"
#include "PvZ/Lawn/Widget/TitleScreen.h"
#include "PvZ/Lawn/Widget/VSResultsMenu.h"
#include "PvZ/Lawn/Widget/VSSetupMenu.h"
#include "PvZ/Lawn/Widget/WaitForSecondPlayerDialog.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/SexyAppFramework/Widget/ButtonWidget.h"
#include "PvZ/SexyAppFramework/Widget/WidgetManager.h"
#include "PvZ/TodLib/Effect/Reanimator.h"
#include "Symbols.h"


inline void InitHookFunction() {
    homura::HookFunction(LawnApp_LawnAppAddr, &LawnApp::_constructor, &old_LawnApp_LawnApp);
    homura::HookFunction(LawnApp_InitAddr, &LawnApp::Init, &old_LawnApp_Init);
    homura::HookFunction(LawnApp_IsNightAddr, &LawnApp::IsNight, &old_LawnApp_IsNight);
    homura::HookFunction(LawnApp_HardwareInitAddr, &LawnApp::HardwareInit, &old_LawnApp_HardwareInit);
    homura::HookFunction(LawnApp_DoBackToMainAddr, &LawnApp::DoBackToMain, &old_LawnApp_DoBackToMain);
    homura::HookFunction(LawnApp_CanShopLevelAddr, &LawnApp::CanShopLevel, &old_LawnApp_CanShopLevel);
    homura::HookFunction(LawnApp_DoNewOptionsAddr, &LawnApp::DoNewOptions, &old_LawnApp_DoNewOptions);
    homura::HookFunction(LawnApp_GetNumPreloadingTasksAddr, &LawnApp::GetNumPreloadingTasks, &old_LawnApp_GetNumPreloadingTasks);
    homura::HookFunction(LawnApp_DoConfirmBackToMainAddr, &LawnApp::DoConfirmBackToMain, nullptr);
    homura::HookFunction(LawnApp_TrophiesNeedForGoldSunflowerAddr, &LawnApp::TrophiesNeedForGoldSunflower, nullptr);
    homura::HookFunction(LawnApp_GamepadToPlayerIndexAddr, &LawnApp::GamepadToPlayerIndex, &old_LawnApp_GamepadToPlayerIndex);
    homura::HookFunction(LawnApp_ShowCreditScreenAddr, &LawnApp::ShowCreditScreen, &old_LawnApp_ShowCreditScreen);
    homura::HookFunction(LawnApp_OnSessionTaskFailedAddr, &LawnApp::OnSessionTaskFailed, nullptr);
    homura::HookFunction(LawnApp_UpdateAppAddr, &LawnApp::UpdateApp, &old_LawnApp_UpDateApp);
    homura::HookFunction(LawnApp_ShowAwardScreenAddr, &LawnApp::ShowAwardScreen, &old_LawnApp_ShowAwardScreen);
    homura::HookFunction(LawnApp_KillAwardScreenAddr, &LawnApp::KillAwardScreen, &old_LawnApp_KillAwardScreen);
    homura::HookFunction(LawnApp_LoadLevelConfigurationAddr, &LawnApp::LoadLevelConfiguration, &old_LawnApp_LoadLevelConfiguration);
    homura::HookFunction(LawnApp_LoadingThreadProcAddr, &LawnApp::LoadingThreadProc, &old_LawnApp_LoadingThreadProc);
    homura::HookFunction(LawnApp_IsChallengeWithoutSeedBankAddr, &LawnApp::IsChallengeWithoutSeedBank, &old_LawnApp_IsChallengeWithoutSeedBank);
    homura::HookFunction(LawnApp_TryHelpTextScreenAddr, &LawnApp::TryHelpTextScreen, nullptr);
    homura::HookFunction(LawnApp_KillSeedChooserScreenAddr, &LawnApp::KillSeedChooserScreen, &old_LawnApp_KillSeedChooserScreen);
    homura::HookFunction(LawnApp_GetSeedsAvailableAddr, &LawnApp::GetSeedsAvailable, &old_LawnApp_GetSeedsAvailable);
    homura::HookFunction(LawnApp_ClearSecondPlayerAddr, &LawnApp::ClearSecondPlayer, &old_LawnApp_ClearSecondPlayer);
    // homura::HookFunction(LawnApp_HasSeedTypeAddr, &LawnApp_HasSeedType, &old_LawnApp_HasSeedType);


    homura::HookFunction(Board_DrawAddr, &Board::Draw, &old_Board_Draw);
    homura::HookFunction(Board_UpdateAddr, &Board::Update, &old_Board_Update);
    homura::HookFunction(Board_BoardAddr, &Board::_constructor, &old_Board_Board);
    homura::HookFunction(Board_InitLevelAddr, &Board::InitLevel, &old_Board_InitLevel);
    homura::HookFunction(Board_StartLevelAddr, &Board::StartLevel, &old_Board_StartLevel);
    homura::HookFunction(Board_RemovedFromManagerAddr, &Board::RemovedFromManager, &old_Board_RemovedFromManager);
    homura::HookFunction(Board_FadeOutLevelAddr, &Board::FadeOutLevel, &old_Board_FadeOutLevel);
    homura::HookFunction(Board_AddPlantAddr, &Board::AddPlant, &old_Board_AddPlant);
    homura::HookFunction(Board_AddSunMoneyAddr, &Board::AddSunMoney, &old_Board_AddSunMoney);
    homura::HookFunction(Board_AddDeathMoneyAddr, &Board::AddDeathMoney, &old_Board_AddDeathMoney);
    homura::HookFunction(Board_CanPlantAtAddr, &Board::CanPlantAt, &old_Board_CanPlantAt);
    homura::HookFunction(Board_PlantingRequirementsMetAddr, &Board::PlantingRequirementsMet, &old_Board_PlantingRequirementsMet);
    homura::HookFunction(Board_GetFlowerPotAtAddr, &Board::GetFlowerPotAt, nullptr);
    homura::HookFunction(Board_GetPumpkinAtAddr, &Board::GetPumpkinAt, nullptr);
    homura::HookFunction(Board_ZombiesWonAddr, &Board::ZombiesWon, &old_BoardZombiesWon);
    homura::HookFunction(Board_KeyDownAddr, &Board::KeyDown, &old_Board_KeyDown);
    homura::HookFunction(Board_UpdateSunSpawningAddr, &Board::UpdateSunSpawning, &old_Board_UpdateSunSpawning);
    homura::HookFunction(Board_UpdateZombieSpawningAddr, &Board::UpdateZombieSpawning, &old_Board_UpdateZombieSpawning);
    homura::HookFunction(Board_PickBackgroundAddr, &Board::PickBackground, &old_Board_PickBackground);
    homura::HookFunction(Board_DrawCoverLayerAddr, &Board::DrawCoverLayer, nullptr);
    homura::HookFunction(Board_UpdateGameAddr, &Board::UpdateGame, &old_Board_UpdateGame);
    homura::HookFunction(Board_UpdateGameObjectsAddr, &Board::UpdateGameObjects, &old_Board_UpdateGameObjects);
    homura::HookFunction(Board_IsFlagWaveAddr, &Board::IsFlagWave, &old_Board_IsFlagWave);
    homura::HookFunction(Board_SpawnZombieWaveAddr, &Board::SpawnZombieWave, &old_Board_SpawnZombieWave);
    homura::HookFunction(Board_DrawProgressMeterAddr, &Board::DrawProgressMeter, &old_Board_DrawProgressMeter);
    homura::HookFunction(Board_GetNumWavesPerFlagAddr, &Board::GetNumWavesPerFlag, nullptr);
    homura::HookFunction(Board_IsLevelDataLoadedAddr, &Board::IsLevelDataLoaded, &old_Board_IsLevelDataLoaded);
    homura::HookFunction(Board_NeedSaveGameAddr, &Board::NeedSaveGame, &old_Board_NeedSaveGame);
    homura::HookFunction(Board_UpdateFwooshAddr, &Board::UpdateFwoosh, &old_Board_UpdateFwoosh);
    homura::HookFunction(Board_UpdateFogAddr, &Board::UpdateFog, &old_Board_UpdateFog);
    homura::HookFunction(Board_DrawFogAddr, &Board::DrawFog, &old_Board_DrawFog);
    homura::HookFunction(Board_UpdateIceAddr, &Board::UpdateIce, &old_Board_UpdateIce);
    homura::HookFunction(Board_DrawBackdropAddr, &Board::DrawBackdrop, &old_Board_DrawBackdrop);
    homura::HookFunction(Board_RowCanHaveZombieTypeAddr, &Board::RowCanHaveZombieType, &old_Board_RowCanHaveZombieType);
    homura::HookFunction(Board_DrawDebugTextAddr, &Board::DrawDebugText, &old_Board_DrawDebugText);
    homura::HookFunction(Board_DrawDebugObjectRectsAddr, &Board::DrawDebugObjectRects, &old_Board_DrawDebugObjectRects);
    homura::HookFunction(Board_DrawFadeOutAddr, &Board::DrawFadeOut, nullptr);
    homura::HookFunction(Board_GetCurrentPlantCostAddr, &Board::GetCurrentPlantCost, &old_Board_GetCurrentPlantCost);
    homura::HookFunction(Board_PauseAddr, &Board::Pause, &old_Board_Pause);
    homura::HookFunction(Board_AddSecondPlayerAddr, &Board::AddSecondPlayer, nullptr);
    homura::HookFunction(Board_IsLastStandFinalStageAddr, &Board::IsLastStandFinalStage, nullptr);
    homura::HookFunction(Board_MouseHitTestAddr, &Board::MouseHitTest, &old_Board_MouseHitTest);
    homura::HookFunction(Board_DrawShovelAddr, &Board::DrawShovel, nullptr);
    homura::HookFunction(Board_StageHasPoolAddr, &Board::StageHasPool, nullptr);
    homura::HookFunction(Board_AddZombieInRowAddr, &Board::AddZombieInRow, &old_Board_AddZombieInRow);
    homura::HookFunction(Board_AddZombieAddr, &Board::AddZombie, nullptr);
    homura::HookFunction(Board_DoPlantingEffectsAddr, &Board::DoPlantingEffects, nullptr);
    homura::HookFunction(Board_InitLawnMowersAddr, &Board::InitLawnMowers, &old_Board_InitLawnMowers);
    homura::HookFunction(Board_PickZombieWavesAddr, &Board::PickZombieWaves, &old_Board_PickZombieWaves);
    homura::HookFunction(Board_DrawUITopAddr, &Board::DrawUITop, &old_Board_DrawUITop);
    homura::HookFunction(Board_GetShovelButtonRectAddr, &Board::GetShovelButtonRect, &old_Board_GetShovelButtonRect);
    homura::HookFunction(Board_UpdateLevelEndSequenceAddr, &Board::UpdateLevelEndSequence, &old_Board_UpdateLevelEndSequence);
    homura::HookFunction(Board_UpdateGridItemsAddr, &Board::UpdateGridItems, &old_Board_UpdateGridItems);
    homura::HookFunction(Board_ShakeBoardAddr, &Board::ShakeBoard, &old_Board_ShakeBoard);
    homura::HookFunction(Board_DrawZenButtonsAddr, &Board::DrawZenButtons, &old_Board_DrawZenButtons);
    homura::HookFunction(Board_DrawGameObjectsAddr, &Board::DrawGameObjects, &old_Board_DrawGameObjects);
    // homura::HookFunction(Board_AddProjectileAddr, &Board::AddProjectile, &old_Board_AddProjectile);
    // homura::HookFunction(Board_PixelToGridXAddr, &Board::PixelToGridX, &old_Board_PixelToGridX);
    // homura::HookFunction(Board_PixelToGridYAddr, &Board::PixelToGridY, &old_Board_PixelToGridY);
    homura::HookFunction(Board_GetNumSeedsInBankAddr, &Board::GetNumSeedsInBank, &old_Board_GetNumSeedsInBank);
    homura::HookFunction(Board_GetSeedPacketPositionXAddr, &Board::GetSeedPacketPositionX, nullptr);
    homura::HookFunction(Board_AddCoinAddr, &Board::AddCoin, &old_Board_AddCoin);
    homura::HookFunction(Board_AddAGraveStoneAddr, &Board::AddAGraveStone, &old_Board_AddAGraveStone);
    homura::HookFunction(Board_TakeSunMoneyAddr, &Board::TakeSunMoney, &old_Board_TakeSunMoney);
    homura::HookFunction(Board_TakeDeathMoneyAddr, &Board::TakeDeathMoney, &old_Board_TakeDeathMoney);


    homura::HookFunction(FixBoardAfterLoadAddr, &FixBoardAfterLoad, &old_FixBoardAfterLoad);
    homura::HookFunction(LawnSaveGameAddr, &LawnSaveGame, &old_LawnSaveGame);
    homura::HookFunction(LawnLoadGameAddr, &LawnLoadGame, &old_LawnLoadGame);

    homura::HookFunction(Challenge_UpdateAddr, &Challenge::Update, &old_Challenge_Update);
    homura::HookFunction(Challenge_ChallengeAddr, &Challenge::_constructor, &old_Challenge_Challenge);
    homura::HookFunction(Challenge_HeavyWeaponFireAddr, &Challenge::HeavyWeaponFire, &old_Challenge_HeavyWeaponFire);
    homura::HookFunction(Challenge_IZombieDrawPlantAddr, &Challenge::IZombieDrawPlant, nullptr);
    homura::HookFunction(Challenge_HeavyWeaponUpdateAddr, &Challenge::HeavyWeaponUpdate, &old_Challenge_HeavyWeaponUpdate);
    homura::HookFunction(Challenge_IZombieEatBrainAddr, &Challenge::IZombieEatBrain, nullptr);
    homura::HookFunction(Challenge_DrawArtChallengeAddr, &Challenge::DrawArtChallenge, nullptr);
    homura::HookFunction(Challenge_CanPlantAtAddr, &Challenge::CanPlantAt, nullptr);
    homura::HookFunction(Challenge_InitLevelAddr, &Challenge::InitLevel, &old_Challenge_InitLevel);
    homura::HookFunction(Challenge_InitZombieWavesAddr, &Challenge::InitZombieWaves, &old_Challenge_InitZombieWaves);
    homura::HookFunction(Challenge_TreeOfWisdomFertilizeAddr, &Challenge::TreeOfWisdomFertilize, &old_Challenge_TreeOfWisdomFertilize);
    homura::HookFunction(Challenge_LastStandUpdateAddr, &Challenge::LastStandUpdate, nullptr);
    homura::HookFunction(Challenge_DrawHeavyWeaponAddr, &Challenge::DrawHeavyWeapon, nullptr);
    homura::HookFunction(Challenge_UpdateZombieSpawningAddr, &Challenge::UpdateZombieSpawning, &old_Challenge_UpdateZombieSpawning);
    homura::HookFunction(Challenge_HeavyWeaponPacketClickedAddr, &Challenge::HeavyWeaponPacketClicked, &old_Challenge_HeavyWeaponPacketClicked);
    homura::HookFunction(Challenge_IZombieSeedTypeToZombieTypeAddr, &Challenge::IZombieSeedTypeToZombieType, nullptr);
    homura::HookFunction(Challenge_StartLevelAddr, &Challenge::StartLevel, &old_Challenge_StartLevel);
    homura::HookFunction(Challenge_DeleteAddr, &Challenge::_destructor, &old_Challenge_Delete);
    homura::HookFunction(Challenge_ScaryPotterOpenPotAddr, &Challenge::ScaryPotterOpenPot, &old_Challenge_ScaryPotterOpenPot);
    homura::HookFunction(Challenge_IZombieGetBrainTargetAddr, &Challenge::IZombieGetBrainTarget, &old_Challenge_IZombieGetBrainTarget);
    homura::HookFunction(Challenge_IZombieSquishBrainAddr, &Challenge::IZombieSquishBrain, &old_Challenge_IZombieSquishBrain);
    homura::HookFunction(Challenge_UpdateConveyorBeltAddr, &Challenge::UpdateConveyorBelt, &old_Challenge_UpdateConveyorBelt);


    homura::HookFunction(ChallengeScreen_AddedToManagerAddr, &ChallengeScreen::AddedToManager, &old_ChallengeScreen_AddedToManager);
    homura::HookFunction(ChallengeScreen_RemovedFromManagerAddr, &ChallengeScreen::RemovedFromManager, &old_ChallengeScreen_RemovedFromManager);
    homura::HookFunction(ChallengeScreen_Delete2Addr, &ChallengeScreen::__Destructor2, &old_ChallengeScreen_Delete2);
    homura::HookFunction(ChallengeScreen_UpdateAddr, &ChallengeScreen::Update, &old_ChallengeScreen_Update);
    homura::HookFunction(ChallengeScreen_ChallengeScreenAddr, &ChallengeScreen::_constructor, &old_ChallengeScreen_ChallengeScreen);
    homura::HookFunction(ChallengeScreen_DrawAddr, &ChallengeScreen::Draw, &old_ChallengeScreen_Draw);
    homura::HookFunction(ChallengeScreen_ButtonDepressAddr, &ChallengeScreen::ButtonDepress, nullptr);
    homura::HookFunction(ChallengeScreen_UpdateButtonsAddr, &ChallengeScreen::UpdateButtons, nullptr);
    homura::HookFunction(GetChallengeDefinitionAddr, &GetChallengeDefinition, &old_GetChallengeDefinition);


    homura::HookFunction(Coin_CoinInitializeAddr, &Coin::CoinInitialize, &old_Coin_CoinInitialize);
    homura::HookFunction(Coin_UpadteAddr, &Coin::Update, &old_Coin_Update);
    homura::HookFunction(Coin_GamepadCursorOverAddr, &Coin::GamepadCursorOver, &old_Coin_GamepadCursorOver);
    homura::HookFunction(Coin_MouseHitTestAddr, &Coin::MouseHitTest, &old_Coin_MouseHitTest);
    homura::HookFunction(Coin_UpdateFallAddr, &Coin::UpdateFall, &old_Coin_UpdateFall);
    homura::HookFunction(Coin_DrawAddr, &Coin::Draw, &old_Coin_Draw);
    homura::HookFunction(Coin_GetColorAddr, &Coin::GetColor, nullptr);


    homura::HookFunction(GamepadControls_ButtonDownFireCobcannonTestAddr, &GamepadControls::ButtonDownFireCobcannonTest, &old_GamepadControls_ButtonDownFireCobcannonTest);
    homura::HookFunction(GamepadControls_DrawAddr, &GamepadControls::Draw, &old_GamepadControls_Draw);
    homura::HookFunction(GamepadControls_GamepadControlsAddr, &GamepadControls::_constructor, &old_GamepadControls_GamepadControls);
    homura::HookFunction(GamepadControls_UpdateAddr, &GamepadControls::Update, &old_GamepadControls_Update);
    homura::HookFunction(GamepadControls_DrawPreviewAddr, &GamepadControls::DrawPreview, &old_GamepadControls_DrawPreview);
    homura::HookFunction(GamepadControls_UpdatePreviewReanimAddr, &GamepadControls::UpdatePreviewReanim, &old_GamepadControls_UpdatePreviewReanim);
    homura::HookFunction(GamepadControls_OnButtonDownAddr, &GamepadControls::OnButtonDown, &old_GamepadControls_OnButtonDown);


    homura::HookFunction(GridItem_UpdateAddr, &GridItem::Update, &old_GridItem_Update);
    homura::HookFunction(GridItem_UpdateScaryPotAddr, &GridItem::UpdateScaryPot, &old_GridItem_UpdateScaryPot);
    homura::HookFunction(GridItem_DrawStinkyAddr, &GridItem::DrawStinky, &old_GridItem_DrawStinky);
    homura::HookFunction(GridItem_DrawSquirrelAddr, &GridItem::DrawSquirrel, nullptr);
    homura::HookFunction(GridItem_DrawScaryPotAddr, &GridItem::DrawScaryPot, nullptr);
    homura::HookFunction(GridItem_DrawCraterAddr, &GridItem::DrawCrater, nullptr);


    homura::HookFunction(AlmanacDialog_RemovedFromManagerAddr, &AlmanacDialog::RemovedFromManager, &old_AlmanacDialog_RemovedFromManager);
    homura::HookFunction(AlmanacDialog_AlmanacDialogAddr, &AlmanacDialog::_constructor, &old_AlmanacDialog_AlmanacDialog);
    homura::HookFunction(AlmanacDialog_SetPageAddr, &AlmanacDialog::SetPage, &old_AlmanacDialog_SetPage);
    homura::HookFunction(AlmanacDialog_MouseDownAddr, &AlmanacDialog::MouseDown, nullptr);
    homura::HookFunction(AlmanacDialog_MouseUpAddr, &AlmanacDialog::MouseUp, nullptr);
    homura::HookFunction(AlmanacDialog_ButtonDepressAddr, &AlmanacDialog::ButtonDepress, nullptr);
    homura::HookFunction(AlmanacDialog_Delete2Addr, &AlmanacDialog::_destructor, &old_AlmanacDialog_Delete2);
    homura::HookFunction(AlmanacDialog_DrawPlantsAddr, &AlmanacDialog::DrawPlants, &old_AlmanacDialog_DrawPlants);
    homura::HookFunction(AlmanacDialog_SetupLayoutPlantsAddr, &AlmanacDialog::SetupLayoutPlants, &old_AlmanacDialog_SetupLayoutPlants);


    homura::HookFunction(SeedChooserScreen_EnableStartButtonAddr, &SeedChooserScreen::EnableStartButton, &old_SeedChooserScreen_EnableStartButton);
    homura::HookFunction(SeedChooserScreen_RebuildHelpbarAddr, &SeedChooserScreen::RebuildHelpbar, &old_SeedChooserScreen_RebuildHelpbar);
    homura::HookFunction(SeedChooserScreen_SeedChooserScreenAddr, &SeedChooserScreen::_constructor, &old_SeedChooserScreen_SeedChooserScreen);
    homura::HookFunction(SeedChooserScreen_GetZombieSeedTypeAddr, &SeedChooserScreen::GetZombieSeedType, nullptr);
    homura::HookFunction(SeedChooserScreen_GetZombieTypeAddr, &SeedChooserScreen::GetZombieType, nullptr);
    homura::HookFunction(SeedChooserScreen_ClickedSeedInChooserAddr, &SeedChooserScreen::ClickedSeedInChooser, &old_SeedChooserScreen_ClickedSeedInChooser);
    homura::HookFunction(SeedChooserScreen_CrazyDavePickSeedsAddr, &SeedChooserScreen::CrazyDavePickSeeds, &old_SeedChooserScreen_CrazyDavePickSeeds);
    homura::HookFunction(SeedChooserScreen_OnStartButtonAddr, &SeedChooserScreen::OnStartButton, &old_SeedChooserScreen_OnStartButton);
    homura::HookFunction(SeedChooserScreen_UpdateAddr, &SeedChooserScreen::Update, &old_SeedChooserScreen_Update);
    homura::HookFunction(SeedChooserScreen_SeedNotAllowedToPickAddr, &SeedChooserScreen::SeedNotAllowedToPick, &old_SeedChooserScreen_SeedNotAllowedToPick);
    homura::HookFunction(SeedChooserScreen_ClickedSeedInBankAddr, &SeedChooserScreen::ClickedSeedInBank, &old_SeedChooserScreen_ClickedSeedInBank);
    homura::HookFunction(SeedChooserScreen_GameButtonDownAddr, &SeedChooserScreen::GameButtonDown, &old_SeedChooserScreen_GameButtonDown);
    homura::HookFunction(SeedChooserScreen_DrawPacketAddr, &SeedChooserScreen::DrawPacket, nullptr);
    homura::HookFunction(SeedChooserScreen_ButtonDepressAddr, &SeedChooserScreen::ButtonDepress, &old_SeedChooserScreen_ButtonDepress);
    homura::HookFunction(SeedChooserScreen_GetSeedPositionInBankAddr, &SeedChooserScreen::GetSeedPositionInBank, &old_SeedChooserScreen_GetSeedPositionInBank);
    homura::HookFunction(SeedChooserScreen_GetSeedPositionInChooserAddr, &SeedChooserScreen::GetSeedPositionInChooser, nullptr);
    homura::HookFunction(SeedChooserScreen_ShowToolTipAddr, &SeedChooserScreen::ShowToolTip, &old_SeedChooserScreen_ShowToolTip);
    homura::HookFunction(SeedChooserScreen_GetNextSeedInDirAddr, &SeedChooserScreen::GetNextSeedInDir, nullptr);
    homura::HookFunction(SeedChooserScreen_DrawAddr, &SeedChooserScreen::Draw, &old_SeedChooserScreen_Draw);


    homura::HookFunction(MainMenu_KeyDownAddr, &MainMenu::KeyDown, &old_MainMenu_KeyDown);
    homura::HookFunction(MainMenu_ButtonDepressAddr, &MainMenu::ButtonDepress, &old_MainMenu_ButtonDepress);
    homura::HookFunction(MainMenu_UpdateAddr, &MainMenu::Update, &old_MainMenu_Update);
    homura::HookFunction(MainMenu_SyncProfileAddr, &MainMenu::SyncProfile, &old_MainMenu_SyncProfile);
    homura::HookFunction(MainMenu_EnterAddr, &MainMenu::Enter, &old_MainMenu_Enter);
    homura::HookFunction(MainMenu_ExitAddr, &MainMenu::Exit, &old_MainMenu_Exit);
    homura::HookFunction(MainMenu_UpdateExitAddr, &MainMenu::UpdateExit, &old_MainMenu_UpdateExit);
    homura::HookFunction(MainMenu_OnExitAddr, &MainMenu::OnExit, &old_MainMenu_OnExit);
    // MSHookFunction(MainMenu_SetSceneAddr, (void *) SetScene, (void **) &old_MainMenu_SetScene);
    homura::HookFunction(MainMenu_OnSceneAddr, &MainMenu::OnScene, &old_MainMenu_OnScene);
    homura::HookFunction(MainMenu_SyncButtonsAddr, &MainMenu::SyncButtons, &old_MainMenu_SyncButtons);
    homura::HookFunction(MainMenu_MainMenuAddr, &MainMenu::_constructor, &old_MainMenu_MainMenu);
    homura::HookFunction(MainMenu_UpdateCameraPositionAddr, &MainMenu::UpdateCameraPosition, &old_MainMenu_UpdateCameraPosition);
    homura::HookFunction(MainMenu_AddedToManagerAddr, &MainMenu::AddedToManager, &old_MainMenu_AddedToManager);
    homura::HookFunction(MainMenu_RemovedFromManagerAddr, &MainMenu::RemovedFromManager, &old_MainMenu_RemovedFromManager);
    homura::HookFunction(MainMenu_DrawOverlayAddr, &MainMenu::DrawOverlay, &old_MainMenu_DrawOverlay);
    homura::HookFunction(MainMenu_DrawFadeAddr, &MainMenu::DrawFade, &old_MainMenu_DrawFade);
    homura::HookFunction(MainMenu_Delete2Addr, &MainMenu::__Destructor2, &old_MainMenu_Delete2);
    homura::HookFunction(MainMenu_DrawAddr, &MainMenu::Draw, &old_MainMenu_Draw);


    homura::HookFunction(StoreScreen_UpdateAddr, &StoreScreen::Update, &old_StoreScreen_Update);
    homura::HookFunction(StoreScreen_SetupPageAddr, &StoreScreen::SetupPage, &old_StoreScreen_SetupPage);
    homura::HookFunction(StoreScreen_ButtonDepressAddr, &StoreScreen::ButtonDepress, &old_StoreScreen_ButtonDepress);
    homura::HookFunction(StoreScreen_AddedToManagerAddr, &StoreScreen::AddedToManager, &old_StoreScreen_AddedToManager);
    homura::HookFunction(StoreScreen_RemovedFromManagerAddr, &StoreScreen::RemovedFromManager, &old_StoreScreen_RemovedFromManager);
    homura::HookFunction(StoreScreen_PurchaseItemAddr, &StoreScreen::PurchaseItem, &old_StoreScreen_PurchaseItem);
    homura::HookFunction(StoreScreen_DrawAddr, &StoreScreen::Draw, &old_StoreScreen_Draw);
    homura::HookFunction(StoreScreen_DrawItemAddr, &StoreScreen::DrawItem, &old_StoreScreen_DrawItem);


    homura::HookFunction(Plant_UpdateAddr, &Plant::Update, &old_Plant_Update);
    homura::HookFunction(Plant_GetRefreshTimeAddr, &Plant::GetRefreshTime, &old_Plant_GetRefreshTime);
    homura::HookFunction(Plant_DoSpecialAddr, &Plant::DoSpecial, &old_Plant_DoSpecial);
    homura::HookFunction(Plant_DrawAddr, &Plant::Draw, &old_Plant_Draw);
    homura::HookFunction(Plant_DrawSeedTypeAddr, &Plant::DrawSeedType, nullptr);
    homura::HookFunction(Plant_IsUpgradeAddr, &Plant::IsUpgrade, &old_Plant_IsUpgrade);
    homura::HookFunction(Plant_PlantInitializeAddr, &Plant::PlantInitialize, &old_Plant_PlantInitialize);
    homura::HookFunction(Plant_SetSleepingAddr, &Plant::SetSleeping, &old_Plant_SetSleeping);
    homura::HookFunction(Plant_UpdateReanimColorAddr, &Plant::UpdateReanimColor, &old_Plant_UpdateReanimColor);
    homura::HookFunction(Plant_FindTargetZombieAddr, &Plant::FindTargetZombie, nullptr);
    homura::HookFunction(Plant_FindTargetGridItemAddr, &Plant::FindTargetGridItem, nullptr);
    homura::HookFunction(Plant_GetCostAddr, &Plant::GetCost, &old_Plant_GetCost);
    homura::HookFunction(Plant_DieAddr, &Plant::Die, nullptr);
    homura::HookFunction(Plant_PlayBodyReanimAddr, &Plant::PlayBodyReanim, &old_Plant_PlayBodyReanim);
    homura::HookFunction(Plant_UpdateProductionPlantAddr, &Plant::UpdateProductionPlant, &old_Plant_UpdateProductionPlant);
    homura::HookFunction(Plant_FireAddr, &Plant::Fire, nullptr);
    homura::HookFunction(Plant_UpdateShootingAddr, &Plant::UpdateShooting, nullptr);

    // MSHookFunction(Plant_CobCannonFireAddr, (void *) Plant_CobCannonFire, (void **) &old_Plant_CobCannonFire);
    // MSHookFunction(Plant_UpdateReanimAddr, (void *) Plant_UpdateReanim, (void **) &old_Plant_UpdateReanim);


    homura::HookFunction(Projectile_ProjectileInitializeAddr, &Projectile::ProjectileInitialize, &old_Projectile_ProjectileInitialize);
    homura::HookFunction(Projectile_ConvertToFireballAddr, &Projectile::ConvertToFireball, nullptr);
    homura::HookFunction(Projectile_ConvertToPeaAddr, &Projectile::ConvertToPea, &old_Projectile_ConvertToPea);
    homura::HookFunction(Projectile_UpdateAddr, &Projectile::Update, &old_Projectile_Update);
    homura::HookFunction(Projectile_UpdateNormalMotionAddr, &Projectile::UpdateNormalMotion, &old_Projectile_UpdateNormalMotion);
    homura::HookFunction(Projectile_DoImpactAddr, &Projectile::DoImpact, &old_Projectile_DoImpact);
    homura::HookFunction(Projectile_CheckForCollisionAddr, &Projectile::CheckForCollision, nullptr);
    homura::HookFunction(Projectile_GetProjectileDefAddr, &Projectile::GetProjectileDef, nullptr);
    homura::HookFunction(Projectile_DrawAddr, &Projectile::Draw, &old_Projectile_Draw);
    homura::HookFunction(Projectile_DrawShadowAddr, &Projectile::DrawShadow, &old_Projectile_DrawShadow);


    homura::HookFunction(SeedPacket_UpdateAddr, &SeedPacket::Update, &old_SeedPacket_Update);
    homura::HookFunction(SeedPacket_UpdateSelectedAddr, &SeedPacket::UpdateSelected, &old_SeedPacket_UpdateSelected);
    homura::HookFunction(SeedPacket_DrawOverlayAddr, &SeedPacket::DrawOverlay, &old_SeedPacket_DrawOverlay);
    homura::HookFunction(SeedPacket_DrawAddr, &SeedPacket::Draw, &old_SeedPacket_Draw);
    homura::HookFunction(SeedPacket_FlashIfReadyAddr, &SeedPacket::FlashIfReady, &old_SeedPacket_FlashIfReady);
    homura::HookFunction(SeedPacket_SetPacketTypeAddr, &SeedPacket::SetPacketType, &old_SeedPacket_SetPacketType);
    // MSHookFunction(SeedPacket_MouseDownAddr, (void *) SeedPacket_MouseDown, (void **) &old_SeedPacket_MouseDown);
    homura::HookFunction(SeedPacket_WasPlantedAddr, &SeedPacket::WasPlanted, &old_SeedPacket_WasPlanted);


    homura::HookFunction(Zombie_UpdateAddr, &Zombie::Update, &old_Zombie_Update);
    homura::HookFunction(Zombie_UpdateActionsAddr, &Zombie::UpdateActions, &old_Zombie_UpdateActions);
    homura::HookFunction(Zombie_UpdateYetiAddr, &Zombie::UpdateYeti, nullptr);
    homura::HookFunction(Zombie_UpdateZombieFlyerAddr, &Zombie::UpdateZombieFlyer, nullptr);
    homura::HookFunction(Zombie_UpdateZombieImpAddr, &Zombie::UpdateZombieImp, nullptr);
    homura::HookFunction(Zombie_UpdateZombieJackInTheBoxAddr, &Zombie::UpdateZombieJackInTheBox, nullptr);
    homura::HookFunction(Zombie_UpdateZombieGargantuarAddr, &Zombie::UpdateZombieGargantuar, nullptr);
    homura::HookFunction(Zombie_UpdateZombiePeaHeadAddr, &Zombie::UpdateZombiePeaHead, nullptr);
    homura::HookFunction(Zombie_UpdateZombieGatlingHeadAddr, &Zombie::UpdateZombieGatlingHead, nullptr);
    homura::HookFunction(Zombie_UpdateZombieJalapenoHeadAddr, &Zombie::UpdateZombieJalapenoHead, nullptr);
    homura::HookFunction(Zombie_UpdateZombieSquashHeadAddr, &Zombie::UpdateZombieSquashHead, nullptr);
    homura::HookFunction(Zombie_UpdateZombieRiseFromGraveAddr, &Zombie::UpdateZombieRiseFromGrave, nullptr);
    homura::HookFunction(Zombie_GetDancerFrameAddr, &Zombie::GetDancerFrame, nullptr);
    homura::HookFunction(Zombie_RiseFromGraveAddr, &Zombie::RiseFromGrave, &old_Zombie_RiseFromGrave);
    homura::HookFunction(Zombie_EatPlantAddr, &Zombie::EatPlant, &old_Zombie_EatPlant);
    homura::HookFunction(Zombie_DetachShieldAddr, &Zombie::DetachShield, &old_Zombie_DetachShield);
    homura::HookFunction(Zombie_CheckForBoardEdgeAddr, &Zombie::CheckForBoardEdge, nullptr);
    homura::HookFunction(Zombie_DrawAddr, &Zombie::Draw, &old_Zombie_Draw);
    homura::HookFunction(Zombie_DrawBossPartAddr, &Zombie::DrawBossPart, &old_Zombie_DrawBossPart);
    homura::HookFunction(ZombieTypeCanGoInPoolAddr, &Zombie::ZombieTypeCanGoInPool, &old_ZombieTypeCanGoInPool);
    homura::HookFunction(Zombie_BossSpawnAttackAddr, &Zombie::BossSpawnAttack, nullptr);
    homura::HookFunction(Zombie_DrawBungeeCordAddr, &Zombie::DrawBungeeCord, nullptr);
    homura::HookFunction(Zombie_IsTangleKelpTargetAddr, &Zombie::IsTangleKelpTarget, nullptr);
    homura::HookFunction(Zombie_IsTangleKelpTarget2Addr, &Zombie::IsTangleKelpTarget, nullptr);
    homura::HookFunction(Zombie_DrawReanimAddr, &Zombie::DrawReanim, nullptr);
    homura::HookFunction(Zombie_DropHeadAddr, &Zombie::DropHead, &old_Zombie_DropHead);
    homura::HookFunction(Zombie_DropArmAddr, &Zombie::DropArm, &old_Zombie_DropArm);
    homura::HookFunction(Zombie_ZombieInitializeAddr, &Zombie::ZombieInitialize, &old_Zombie_ZombieInitialize);
    homura::HookFunction(Zombie_DieNoLootAddr, &Zombie::DieNoLoot, &old_Zombie_DieNoLoot);
    homura::HookFunction(Zombie_StopZombieSoundAddr, &Zombie::StopZombieSound, nullptr);
    homura::HookFunction(GetZombieDefinitionAddr, &GetZombieDefinition, nullptr);
    homura::HookFunction(Zombie_FindPlantTargetAddr, &Zombie::FindPlantTarget, nullptr);
    homura::HookFunction(Zombie_FindZombieTargetAddr, &Zombie::FindZombieTarget, nullptr);
    homura::HookFunction(Zombie_TakeDamageAddr, &Zombie::TakeDamage, &old_Zombie_TakeDamage);
    homura::HookFunction(Zombie_TakeHelmDamageAddr, &Zombie::TakeHelmDamage, &old_Zombie_TakeHelmDamage);
    homura::HookFunction(Zombie_PlayZombieReanimAddr, &Zombie::PlayZombieReanim, &old_Zombie_PlayZombieReanim);
    homura::HookFunction(Zombie_StartWalkAnimAddr, &Zombie::StartWalkAnim, &old_Zombie_StartWalkAnim);
    homura::HookFunction(Zombie_ReanimShowPrefixAddr, &Zombie::ReanimShowPrefix, &old_Zombie_ReanimShowPrefix);
    homura::HookFunction(Zombie_ReanimShowTrackAddr, &Zombie::ReanimShowTrack, &old_Zombie_ReanimShowTrack);
    homura::HookFunction(Zombie_GetPosYBasedOnRowAddr, &Zombie::GetPosYBasedOnRow, &old_Zombie_GetPosYBasedOnRow);
    homura::HookFunction(Zombie_SetRowAddr, &Zombie::SetRow, &old_Zombie_SetRow);
    homura::HookFunction(Zombie_StartMindControlledAddr, &Zombie::StartMindControlled, &old_Zombie_StartMindControlled);
    homura::HookFunction(Zombie_UpdateReanimAddr, &Zombie::UpdateReanim, &old_Zombie_UpdateReanim);
    homura::HookFunction(Zombie_GetBobsledPositionAddr, &Zombie::GetBobsledPosition, &old_Zombie_GetBobsledPosition);
    homura::HookFunction(Zombie_SquishAllInSquareAddr, &Zombie::SquishAllInSquare, &old_Zombie_SquishAllInSquare);
    homura::HookFunction(Zombie_StopEatingAddr, &Zombie::StopEating, nullptr);
    homura::HookFunction(Zombie_PickRandomSpeedAddr, &Zombie::PickRandomSpeed, nullptr);
    homura::HookFunction(Zombie_UpdateZombieWalkingAddr, &Zombie::UpdateZombieWalking, &old_Zombie_UpdateZombieWalking);
    homura::HookFunction(Zombie_UpdateDamageStatesAddr, &Zombie::UpdateDamageStates, nullptr);
    homura::HookFunction(Zombie_DropLootAddr, &Zombie::DropLoot, &old_Zombie_DropLoot);
    homura::HookFunction(Zombie_ApplyBurnAddr, &Zombie::ApplyBurn, &old_Zombie_ApplyBurn);
    homura::HookFunction(Zombie_CheckIfPreyCaughtAddr, &Zombie::CheckIfPreyCaught, nullptr);
    homura::HookFunction(Zombie_CanTargetPlantAddr, &Zombie::CanTargetPlant, nullptr);


    homura::HookFunction(Sexy_Dialog_AddedToManagerWidgetManagerAddr, &SexyDialog_AddedToManager, &old_SexyDialog_AddedToManager);
    homura::HookFunction(Sexy_Dialog_RemovedFromManagerAddr, &SexyDialog_RemovedFromManager, &old_SexyDialog_RemovedFromManager);


    homura::HookFunction(SeedBank_DrawAddr, &SeedBank::Draw, &old_SeedBank_Draw);
    homura::HookFunction(SeedBank_MouseHitTestAddr, &SeedBank::MouseHitTest, nullptr);
    // homura::HookFunction(SeedBank_SeedBankAddr, &SeedBank::Create, &old_SeedBank_SeedBank);
    // homura::HookFunction(SeedBank_UpdateWidthAddr, &SeedBank::UpdateWidth, &old_SeedBank_UpdateWidth);
    homura::HookFunction(SeedBank_MoveAddr, &SeedBank::Move, nullptr);
    homura::HookFunction(SeedBank_AddSeedAddr, &SeedBank::AddSeed, &old_SeedBank_AddSeed);


    homura::HookFunction(AwardScreen_MouseDownAddr, &AwardScreen::MouseDown, &old_AwardScreen_MouseDown);
    homura::HookFunction(AwardScreen_MouseUpAddr, &AwardScreen::MouseUp, &old_AwardScreen_MouseUp);


    homura::HookFunction(VSSetupMenu_VSSetupMenuAddr, &VSSetupMenu::_constructor, &old_VSSetupMenu_Constructor);
    homura::HookFunction(VSSetupMenu_Delete2Addr, &VSSetupMenu::_destructor, &old_VSSetupMenu_Destructor);
    homura::HookFunction(VSSetupMenu_DrawAddr, &VSSetupMenu::Draw, &old_VSSetupMenu_Draw);
    homura::HookFunction(VSSetupMenu_AddedToManagerAddr, &VSSetupMenu::AddedToManager, &old_VSSetupMenu_AddedToManager);
    homura::HookFunction(VSSetupMenu_UpdateAddr, &VSSetupMenu::Update, &old_VSSetupMenu_Update);
    homura::HookFunction(VSSetupMenu_KeyDownAddr, &VSSetupMenu::KeyDown, &old_VSSetupMenu_KeyDown);
    homura::HookFunction(VSSetupMenu_OnStateEnterAddr, &VSSetupMenu::OnStateEnter, &old_VSSetupMenu_OnStateEnter);
    // homura::HookFunction(VSSetupMenu_ButtonPressAddr, &VSSetupMenu::ButtonPress, &old_VSSetupMenu_ButtonPress);
    homura::HookFunction(VSSetupMenu_ButtonDepressAddr, &VSSetupMenu::ButtonDepress, &old_VSSetupMenu_ButtonDepress);
    homura::HookFunction(VSSetupMenu_PickRandomZombiesAddr, &VSSetupMenu::PickRandomZombies, &old_VSSetupMenu_PickRandomZombies);
    homura::HookFunction(VSSetupMenu_PickRandomPlantsAddr, &VSSetupMenu::PickRandomPlants, &old_VSSetupMenu_PickRandomPlants);


    homura::HookFunction(VSResultsMenu_UpdateAddr, &VSResultsMenu_Update, &old_VSResultsMenu_Update);
    homura::HookFunction(VSResultsMenu_OnExitAddr, &VSResultsMenu_OnExit, &old_VSResultsMenu_OnExit);
    homura::HookFunction(VSResultsMenu_DrawInfoBoxAddr, &VSResultsMenu_DrawInfoBox, &old_VSResultsMenu_DrawInfoBox);
    homura::HookFunction(VSResultsMenu_ButtonDepressAddr, &VSResultsMenu_ButtonDepress, nullptr);


    homura::HookFunction(ImitaterDialog_ImitaterDialogAddr, &ImitaterDialog_ImitaterDialog, &old_ImitaterDialog_ImitaterDialog);
    homura::HookFunction(ImitaterDialog_MouseDownAddr, &ImitaterDialog::MouseDown, &old_ImitaterDialog_MouseDown);
    // MSHookFunction(ImitaterDialog_OnKeyDownAddr, (void *) ImitaterDialog_OnKeyDown,(void **) &old_ImitaterDialog_OnKeyDown);
    homura::HookFunction(ImitaterDialog_KeyDownAddr, &ImitaterDialog_KeyDown, &old_ImitaterDialog_KeyDown);
    homura::HookFunction(ImitaterDialog_ShowToolTipAddr, &ImitaterDialog_ShowToolTip, &old_ImitaterDialog_ShowToolTip);


    homura::HookFunction(MailScreen_MailScreenAddr, &MailScreen::_constructor, &old_MailScreen_MailScreen);
    homura::HookFunction(MailScreen_AddedToManagerAddr, &MailScreen::AddedToManager, &old_MailScreen_AddedToManager);
    homura::HookFunction(MailScreen_RemovedFromManagerAddr, &MailScreen::RemovedFromManager, &old_MailScreen_RemovedFromManager);
    homura::HookFunction(MailScreen_Delete2Addr, &MailScreen::__Destructor2, &old_MailScreen_Delete2);


    homura::HookFunction(ZenGardenControls_UpdateAddr, &ZenGardenControls::Update, &old_ZenGardenControls_Update);
    homura::HookFunction(ZenGarden_DrawBackdropAddr, &ZenGarden::DrawBackdrop, &old_ZenGarden_DrawBackdrop);
    // MSHookFunction(ZenGarden_MouseDownWithFeedingToolAddr, (void *) ZenGarden_MouseDownWithFeedingTool, (void **) &old_ZenGarden_MouseDownWithFeedingTool);
    // MSHookFunction(ZenGarden_DrawPottedPlantAddr, (void *) ZenGarden_DrawPottedPlant, nullptr);

    // MSHookFunction( Sexy_GamepadApp_CheckGamepadAddr,(void *) Sexy_GamepadApp_CheckGamepad,nullptr);
    // MSHookFunction( Sexy_GamepadApp_HasGamepadAddr,(void *) Sexy_GamepadApp_HasGamepad,nullptr);


    homura::HookFunction(CutScene_ShowShovelAddr, &CutScene::ShowShovel, &old_CutScene_ShowShovel);
    homura::HookFunction(CutScene_UpdateAddr, &CutScene::Update, &old_CutScene_Update);


    homura::HookFunction(BaseGamepadControls_GetGamepadVelocityAddr, &__BaseGamepadControls::GetGamepadVelocity, nullptr);

    homura::HookFunction(LookupFoleyAddr, &LookupFoley, &old_LookupFoley);

    // MSHookFunction(TodDrawStringWrappedHelperAddr, (void *) TodDrawStringWrappedHelper, (void **) &old_TodDrawStringWrappedHelper);
    homura::HookFunction(MessageWidget_ClearLabelAddr, &CustomMessageWidget::ClearLabel, &old_MessageWidget_ClearLabel);
    homura::HookFunction(MessageWidget_SetLabelAddr, &CustomMessageWidget::SetLabel, &old_MessageWidget_SetLabel);
    homura::HookFunction(MessageWidget_UpdateAddr, &CustomMessageWidget::Update, &old_MessageWidget_Update);
    homura::HookFunction(MessageWidget_DrawAddr, &CustomMessageWidget::Draw, &old_MessageWidget_Draw);

    homura::HookFunction(Sexy_ExtractLoadingSoundsResourcesAddr, &Sexy_ExtractLoadingSoundsResources, &old_Sexy_ExtractLoadingSoundsResources);
    // MSHookFunction(Sexy_ScrollbarWidget_MouseDownAddr, (void *) Sexy_ScrollbarWidget_MouseDown,nullptr);

    homura::HookFunction(CustomScrollbarWidget_RemoveScrollButtonsAddr, &Sexy::CustomScrollbarWidget::RemoveScrollButtons, nullptr);

    homura::HookFunction(CreditScreen_CreditScreenAddr, &CreditScreen_CreditScreen, &old_CreditScreen_CreditScreen);
    homura::HookFunction(CreditScreen_RemovedFromManagerAddr, &CreditScreen_RemovedFromManager, &old_CreditScreen_RemovedFromManager);
    homura::HookFunction(CreditScreen_Delete2Addr, &CreditScreen_Delete2, &old_CreditScreen_Delete2);

    homura::HookFunction(HelpTextScreen_AddedToManagerAddr, &HelpTextScreen_AddedToManager, &old_HelpTextScreen_AddedToManager);
    homura::HookFunction(HelpTextScreen_RemovedFromManagerAddr, &HelpTextScreen_RemovedFromManager, &old_HelpTextScreen_RemovedFromManager);
    homura::HookFunction(HelpTextScreen_HelpTextScreenAddr, &HelpTextScreen_HelpTextScreen, &old_HelpTextScreen_HelpTextScreen);
    homura::HookFunction(HelpTextScreen_Delete2Addr, &HelpTextScreen_Delete2, &old_HelpTextScreen_Delete2);
    homura::HookFunction(HelpTextScreen_UpdateAddr, &HelpTextScreen_Update, &old_HelpTextScreen_Update);
    homura::HookFunction(HelpOptionsDialog_ButtonDepressAddr, &HelpOptionsDialog_ButtonDepress, &old_HelpOptionsDialog_ButtonDepress);
    homura::HookFunction(HelpOptionsDialog_HelpOptionsDialogAddr, &HelpOptionsDialog_HelpOptionsDialog, &old_HelpOptionsDialog_HelpOptionsDialog);
    homura::HookFunction(HelpOptionsDialog_ResizeAddr, &HelpOptionsDialog_Resize, &old_HelpOptionsDialog_Resize);

    homura::HookFunction(WaitForSecondPlayerDialog_WaitForSecondPlayerDialogAddr, &WaitForSecondPlayerDialog::_constructor, &old_WaitForSecondPlayerDialog_WaitForSecondPlayerDialog);
    homura::HookFunction(WaitForSecondPlayerDialog_DeleteAddr, &WaitForSecondPlayerDialog::_destructor, &old_WaitForSecondPlayerDialog_Delete);


    homura::HookFunction(Sexy_WidgetManager_MouseDownAddr, &Sexy::WidgetManager::MouseDown, &old_Sexy_WidgetManager_MouseDown);
    homura::HookFunction(Sexy_WidgetManager_MouseDragAddr, &Sexy::WidgetManager::MouseDrag, &old_Sexy_WidgetManager_MouseDrag);
    homura::HookFunction(Sexy_WidgetManager_MouseUpAddr, &Sexy::WidgetManager::MouseUp, &old_Sexy_WidgetManager_MouseUp);
    // MSHookFunction(Sexy_WidgetManager_AxisMovedAddr, (void *) Sexy_WidgetManager_AxisMoved, nullptr);


    homura::HookFunction(LawnMower_UpdateAddr, &LawnMower::Update, &old_LawnMower_Update);
    homura::HookFunction(ConfirmBackToMainDialog_ButtonDepressAddr, &ConfirmBackToMainDialog_ButtonDepress, &old_ConfirmBackToMainDialog_ButtonDepress);
    homura::HookFunction(ConfirmBackToMainDialog_AddedToManagerAddr, &ConfirmBackToMainDialog_AddedToManager, &old_ConfirmBackToMainDialog_AddedToManager);
    // MSHookFunction(FilterEffectDisposeForAppAddr,(void *) FilterEffectDisposeForApp,nullptr);
    // MSHookFunction(FilterEffectGetImageAddr,(void *) FilterEffectGetImage,nullptr);
    homura::HookFunction(Reanimation_DrawTrackAddr, &Reanimation::DrawTrack, &old_Reanimation_DrawTrack);


    homura::HookFunction(ReanimatorCache_ReanimatorCacheInitializeAddr, &ReanimatorCache::ReanimatorCacheInitialize, nullptr);
    homura::HookFunction(ReanimatorCache_ReanimatorCacheDisposeAddr, &ReanimatorCache::ReanimatorCacheDispose, nullptr);
    homura::HookFunction(ReanimatorCache_DrawCachedPlantAddr, &ReanimatorCache::DrawCachedPlant, &old_ReanimatorCache_DrawCachedPlant);
    homura::HookFunction(ReanimatorCache_UpdateReanimationForVariationAddr, &ReanimatorCache::UpdateReanimationForVariation, &old_ReanimatorCache_UpdateReanimationForVariation);
    homura::HookFunction(ReanimatorCache_LoadCachedImagesAddr, &ReanimatorCache::LoadCachedImages, &old_ReanimatorCache_LoadCachedImages);
    homura::HookFunction(ReanimatorCache_MakeCachedZombieFrameAddr, &ReanimatorCache::MakeCachedZombieFrame, nullptr);


    homura::HookFunction(HelpBarWidget_HelpBarWidgetAddr, &HelpBarWidget_HelpBarWidget, &old_HelpBarWidget_HelpBarWidget);
    homura::HookFunction(DrawSeedTypeAddr, &DrawSeedType, nullptr);
    homura::HookFunction(DrawSeedPacketAddr, &DrawSeedPacket, nullptr);

    homura::HookFunction(Music_PlayMusicAddr, &Music::PlayMusic, nullptr);
    homura::HookFunction(Music_MusicUpdateAddr, &Music::MusicUpdate, nullptr);
    homura::HookFunction(Music_UpdateMusicBurstAddr, &Music::UpdateMusicBurst, &old_Music_UpdateMusicBurst);
    homura::HookFunction(Music2_Music2Addr, &Music2::_constructor, &old_Music2_Music2);

    homura::HookFunction(LawnPlayerInfo_AddCoinsAddr, &LawnPlayerInfo::AddCoins, nullptr);
    homura::HookFunction(MaskHelpWidget_UpdateAddr, &MaskHelpWidget_Update, nullptr);
    homura::HookFunction(MaskHelpWidget_DrawAddr, &MaskHelpWidget_Draw, nullptr);
    // MSHookFunction(DaveHelp_DaveHelpAddr, (void *) DaveHelp_DaveHelp, (void **) &old_DaveHelp_DaveHelp);
    homura::HookFunction(DaveHelp_UpdateAddr, &DaveHelp_Update, nullptr);
    homura::HookFunction(DaveHelp_DrawAddr, &DaveHelp_Draw, nullptr);
    homura::HookFunction(DaveHelp_Delete2Addr, &DaveHelp_Delete2, &old_DaveHelp_Delete2);
    homura::HookFunction(DaveHelp_DealClickAddr, &DaveHelp_DealClick, nullptr);
    homura::HookFunction(TrashBin_TrashBinAddr, &TrashBin::_constructor, &old_TrashBin_TrashBin);
    homura::HookFunction(Sexy_SexyAppBase_Is3DAcceleratedAddr, &LawnApp::Is3DAccelerated, nullptr);
    homura::HookFunction(Sexy_SexyAppBase_SexyAppBaseAddr, &Sexy::SexyAppBase::_constructor, &old_Sexy_SexyAppBase_SexyAppBase);
    homura::HookFunction(SettingsDialog_AddedToManagerAddr, &SettingsDialog_AddedToManager, &old_SettingsDialog_AddedToManager);
    homura::HookFunction(SettingsDialog_RemovedFromManagerAddr, &SettingsDialog_RemovedFromManager, &old_SettingsDialog_RemovedFromManager);
    homura::HookFunction(SettingsDialog_DrawAddr, &SettingsDialog_Draw, &old_SettingsDialog_Draw);
    homura::HookFunction(SettingsDialog_Delete2Addr, &SettingsDialog_Delete2, &old_SettingsDialog_Delete2);
    homura::HookFunction(ReanimatorLoadDefinitionsAddr, &ReanimatorLoadDefinitions, &old_ReanimatorLoadDefinitions);
    homura::HookFunction(DefinitionGetCompiledFilePathFromXMLFilePathAddr, &DefinitionGetCompiledFilePathFromXMLFilePath, &old_DefinitionGetCompiledFilePathFromXMLFilePath);
    homura::HookFunction(TestMenuWidget_DeleteAddr, &TestMenuWidget_Delete, &old_TestMenuWidget_Delete);
    homura::HookFunction(TestMenuWidget_Delete2Addr, &TestMenuWidget_Delete2, &old_TestMenuWidget_Delete2);
    homura::HookFunction(SaveGameContext_SyncReanimationDefAddr, &SaveGameContext::SyncReanimationDef, nullptr);
    homura::HookFunction(PoolEffect_PoolEffectDrawAddr, &PoolEffect::PoolEffectDraw, nullptr);
    homura::HookFunction(Sexy_MemoryImage_ClearRectAddr, &Sexy::MemoryImage::ClearRect, nullptr);


    homura::HookFunction(TitleScreen_DrawAddr, &TitleScreen_Draw, &old_TitleScreen_Draw);
    homura::HookFunction(TitleScreen_UpdateAddr, &TitleScreen_Update, &old_TitleScreen_Update);
    homura::HookFunction(TitleScreen_SwitchStateAddr, &TitleScreen_SwitchState, nullptr);
}

inline void InitVTableHookFunction() {
    homura::HookVirtualFunc(vTableForCursorObjectAddr, 4, &CursorObject::BeginDraw, &old_CursorObject_BeginDraw);
    homura::HookVirtualFunc(vTableForCursorObjectAddr, 5, &CursorObject::EndDraw, &old_CursorObject_EndDraw);

    homura::HookVirtualFunc(vTableForBoardAddr, 77, &Board::MouseMove, &old_Board_MouseMove);
    homura::HookVirtualFunc(vTableForBoardAddr, 78, &Board::MouseDown, &old_Board_MouseDown);
    homura::HookVirtualFunc(vTableForBoardAddr, 81, &Board::MouseUp, &old_Board_MouseUp);
    homura::HookVirtualFunc(vTableForBoardAddr, 83, &Board::MouseDrag, &old_Board_MouseDrag);
    homura::HookVirtualFunc(vTableForBoardAddr, 133, &Board::ButtonDepress, &old_Board_ButtonDepress);

    homura::HookVirtualFunc(vTableForStoreScreenAddr, 78, &StoreScreen::MouseDown, &old_StoreScreen_MouseDown);
    homura::HookVirtualFunc(vTableForStoreScreenAddr, 81, &StoreScreen::MouseUp, &old_StoreScreen_MouseUp);
    // VTableHookFunction(vTableForStoreScreenAddr, 83, (void *) StoreScreen_MouseDrag,(void **) &old_StoreScreen_MouseDrag);

    homura::HookVirtualFunc(vTableForMailScreenAddr, 78, &MailScreen::MouseDown, &old_MailScreen_MouseDown);
    homura::HookVirtualFunc(vTableForMailScreenAddr, 81, &MailScreen::MouseUp, &old_MailScreen_MouseUp);
    homura::HookVirtualFunc(vTableForMailScreenAddr, 83, &MailScreen::MouseDrag, &old_MailScreen_MouseDrag);
    homura::HookVirtualFunc(vTableForMailScreenAddr, 140, &MailScreen::ButtonPress, &old_MailScreen_ButtonPress);
    homura::HookVirtualFunc(vTableForMailScreenAddr, 141, &MailScreen::ButtonDepress, &old_MailScreen_ButtonDepress);

    homura::HookVirtualFunc(vTableForChallengeScreenAddr, 78, &ChallengeScreen::MouseDown, &old_ChallengeScreen_MouseDown);
    homura::HookVirtualFunc(vTableForChallengeScreenAddr, 81, &ChallengeScreen::MouseUp, &old_ChallengeScreen_MouseUp);
    homura::HookVirtualFunc(vTableForChallengeScreenAddr, 83, &ChallengeScreen::MouseDrag, &old_ChallengeScreen_MouseDrag);
    homura::HookVirtualFunc(vTableForChallengeScreenAddr, 130, &ChallengeScreen::ButtonPress, nullptr);

    // homura::HookVirtualFunc(vTableForVSResultsMenuAddr, 78, &VSResultsMenu::MouseDown,nullptr);
    // homura::HookVirtualFunc(vTableForVSResultsMenuAddr, 81, &VSResultsMenu::MouseUp,nullptr);
    // homura::HookVirtualFunc(vTableForVSResultsMenuAddr, 83, &VSResultsMenu::MouseDrag,nullptr);


    homura::HookVirtualFunc(vTableForVSSetupMenuAddr, 78, &VSSetupMenu::MouseDown, nullptr);
    homura::HookVirtualFunc(vTableForVSSetupMenuAddr, 81, &VSSetupMenu::MouseUp, nullptr);
    homura::HookVirtualFunc(vTableForVSSetupMenuAddr, 83, &VSSetupMenu::MouseDrag, nullptr);

    homura::HookVirtualFunc(vTableForSeedChooserScreenAddr, 77, &SeedChooserScreen::MouseMove, &old_SeedChooserScreen_MouseMove);
    homura::HookVirtualFunc(vTableForSeedChooserScreenAddr, 78, &SeedChooserScreen::MouseDown, &old_SeedChooserScreen_MouseDown);
    homura::HookVirtualFunc(vTableForSeedChooserScreenAddr, 81, &SeedChooserScreen::MouseUp, &old_SeedChooserScreen_MouseUp);
    homura::HookVirtualFunc(vTableForSeedChooserScreenAddr, 83, &SeedChooserScreen::MouseDrag, &old_SeedChooserScreen_MouseDrag);
    homura::HookVirtualFunc(vTableForSeedChooserScreenAddr, 135, &SeedChooserScreen::ButtonPress, nullptr);


    homura::HookVirtualFunc(vTableForHelpTextScreenAddr, 38, &HelpTextScreen_Draw, &old_HelpTextScreen_Draw);
    homura::HookVirtualFunc(vTableForHelpTextScreenAddr, 78, &HelpTextScreen::MouseDown, &old_HelpTextScreen_MouseDown);
    // VTableHookFunction(vTableForHelpTextScreenAddr, 81, (void *) HelpTextScreen_MouseUp,(void **) &old_HelpTextScreen_MouseUp);
    // VTableHookFunction(vTableForHelpTextScreenAddr, 83, (void *) HelpTextScreen_MouseDrag,(void **) &old_HelpTextScreen_MouseDrag);
    homura::HookVirtualFunc(vTableForHelpTextScreenAddr, 136, &HelpTextScreen_ButtonDepress, &old_HelpTextScreen_ButtonDepress);

    homura::HookVirtualFunc(vTableForAlmanacDialogAddr, 83, &AlmanacDialog::MouseDrag, &old_AlmanacDialog_MouseDrag);

    homura::HookVirtualFunc(vTableForHouseChooserDialogAddr, 73, &HouseChooserDialog::KeyDown, &old_HouseChooserDialog_KeyDown);
    homura::HookVirtualFunc(vTableForHouseChooserDialogAddr, 78, &HouseChooserDialog::MouseDown, &old_HouseChooserDialog_MouseDown);

    homura::HookVirtualFunc(vTableForSeedPacketAddr, 4, &SeedPacket::BeginDraw, &old_SeedPacket_BeginDraw);
    homura::HookVirtualFunc(vTableForSeedPacketAddr, 5, &SeedPacket::EndDraw, &old_SeedPacket_EndDraw);

    homura::HookVirtualFunc(vTableForSeedBankAddr, 4, &SeedBank::BeginDraw, &old_SeedBank_BeginDraw);
    homura::HookVirtualFunc(vTableForSeedBankAddr, 5, &SeedBank::EndDraw, &old_SeedBank_EndDraw);

    homura::HookVirtualFunc(vTableForGraphicsAddr, 4, &Sexy::Graphics::PushTransform, &old_Sexy_Graphics_PushTransform);
    homura::HookVirtualFunc(vTableForGraphicsAddr, 5, &Sexy::Graphics::PopTransform, &old_Sexy_Graphics_PopTransform);


    homura::HookVirtualFunc(vTableForImageAddr, 37, &Sexy::Image::PushTransform, &old_Sexy_Image_PushTransform);
    homura::HookVirtualFunc(vTableForImageAddr, 38, &Sexy::Image::PopTransform, &old_Sexy_Image_PopTransform);

    homura::HookVirtualFunc(vTableForGLImageAddr, 37, &Sexy::GLImage::PushTransform, &old_Sexy_GLImage_PushTransform);
    homura::HookVirtualFunc(vTableForGLImageAddr, 38, &Sexy::GLImage::PopTransform, &old_Sexy_GLImage_PopTransform);

    homura::HookVirtualFunc(vTableForMemoryImageAddr, 37, &Sexy::MemoryImage::PushTransform, &old_Sexy_MemoryImage_PushTransform);
    homura::HookVirtualFunc(vTableForMemoryImageAddr, 38, &Sexy::MemoryImage::PopTransform, &old_Sexy_MemoryImage_PopTransform);


    homura::HookVirtualFunc(vTableForMusic2Addr, 7, &Music2::StopAllMusic, &old_Music2_StopAllMusic);
    homura::HookVirtualFunc(vTableForMusic2Addr, 8, &Music2::StartGameMusic, &old_Music2_StartGameMusic);
    homura::HookVirtualFunc(vTableForMusic2Addr, 11, &Music2::GameMusicPause, &old_Music2_GameMusicPause);
    // VTableHookFunction(vTableForMusic2Addr, 12, (void *) Music2_UpdateMusicBurst,(void **) &old_Music2_UpdateMusicBurst);
    // VTableHookFunction(vTableForMusic2Addr, 13, (void *) Music2_StartBurst,(void **) &old_Music2_StartBurst);
    homura::HookVirtualFunc(vTableForMusic2Addr, 17, &Music2::FadeOut, &old_Music2_FadeOut);


    // VTableHookFunction(vTableForMaskHelpWidgetAddr, 71, (void *) MaskHelpWidget_KeyDown,nullptr);
    homura::HookVirtualFunc(vTableForMaskHelpWidgetAddr, 78, &MaskHelpWidget_MouseDown, nullptr);
    homura::HookVirtualFunc(vTableForMaskHelpWidgetAddr, 81, &MaskHelpWidget_MouseUp, nullptr);
    homura::HookVirtualFunc(vTableForMaskHelpWidgetAddr, 83, &MaskHelpWidget_MouseDrag, nullptr);


    homura::HookVirtualFunc(vTableForDaveHelpAddr, 78, &DaveHelp_MouseDown, nullptr);
    homura::HookVirtualFunc(vTableForDaveHelpAddr, 81, &DaveHelp_MouseUp, nullptr);
    homura::HookVirtualFunc(vTableForDaveHelpAddr, 83, &DaveHelp_MouseDrag, nullptr);
    homura::HookVirtualFunc(vTableForDaveHelpAddr, 73, &DaveHelp_KeyDown, nullptr);


    homura::HookVirtualFunc(vTableForTestMenuWidgetAddr, 32, &TestMenuWidget_RemovedFromManager, &old_TestMenuWidget_RemovedFromManager);
    homura::HookVirtualFunc(vTableForTestMenuWidgetAddr, 33, &TestMenuWidget_Update, nullptr);
    homura::HookVirtualFunc(vTableForTestMenuWidgetAddr, 38, &TestMenuWidget_Draw, nullptr);
    homura::HookVirtualFunc(vTableForTestMenuWidgetAddr, 78, &TestMenuWidget_MouseDown, nullptr);
    homura::HookVirtualFunc(vTableForTestMenuWidgetAddr, 81, &TestMenuWidget_MouseUp, nullptr);
    homura::HookVirtualFunc(vTableForTestMenuWidgetAddr, 83, &TestMenuWidget_MouseDrag, nullptr);
    homura::HookVirtualFunc(vTableForTestMenuWidgetAddr, 73, &TestMenuWidget_KeyDown, nullptr);


    homura::HookVirtualFunc(vTableForTrashBinAddr, 38, &TrashBin::Draw, nullptr);

    homura::HookVirtualFunc(vTableForConfirmBackToMainDialogAddr, 83, &ConfirmBackToMainDialog_MouseDrag, &old_ConfirmBackToMainDialog_MouseDrag);

    homura::HookVirtualFunc(vTableForSettingsDialogAddr, 153, &SettingsDialog_CheckboxChecked, nullptr);

    homura::HookVirtualFunc(vTableForCreditScreenAddr, 133, &CreditScreen::ButtonDepress, nullptr);

    homura::HookVirtualFunc(vTableForMainMenuAddr, 139, &MainMenu::ButtonPress, nullptr);


    homura::HookVirtualFunc(vTableForWaitForSecondPlayerDialogAddr, 142, &WaitForSecondPlayerDialog_ButtonDepress, &old_WaitForSecondPlayerDialog_ButtonDepress);
    homura::HookVirtualFunc(vTableForWaitForSecondPlayerDialogAddr, 33, &WaitForSecondPlayerDialog::Update, nullptr);
    homura::HookVirtualFunc(vTableForWaitForSecondPlayerDialogAddr, 38, &WaitForSecondPlayerDialog::Draw, &old_WaitForSecondPlayerDialog_Draw);
    homura::HookVirtualFunc(vTableForWaitForSecondPlayerDialogAddr, 52, &WaitForSecondPlayerDialog::Resize, nullptr);
}

inline void InitOpenSL() {
    homura::HookFunction(Native_AudioOutput_setupAddr, &Native_AudioOutput_setup, &old_Native_AudioOutput_setup);
    homura::HookFunction(Native_AudioOutput_shutdownAddr, &Native_AudioOutput_shutdown, &old_Native_AudioOutput_shutdown);
    // MSHookFunction(Native_AudioOutput_writeAddr,(void *) Native_AudioOutput_write,(void **) &old_Native_AudioOutput_write);
    homura::HookFunction(j_AGAudioWriteAddr, &AudioWrite, nullptr);
}

inline void InitIntroVideo() {

    // MSHookFunction(j_AGVideoOpenAddr,(void *) AGVideoOpen,nullptr);
    // MSHookFunction(j_AGVideoShowAddr,(void *) AGVideoShow,nullptr);
    // MSHookFunction(j_AGVideoEnableAddr,(void *) AGVideoEnable,nullptr);
    // MSHookFunction(j_AGVideoIsPlayingAddr,(void *) AGVideoIsPlaying,nullptr);
    // MSHookFunction(j_AGVideoPlayAddr,(void *) AGVideoPlay,nullptr);
    // MSHookFunction(j_AGVideoPauseAddr,(void *) AGVideoPause,nullptr);
    // MSHookFunction(j_AGVideoResumeAddr,(void *) AGVideoResume,nullptr);

    // const std::string libGameMain = "libGameMain.so";
    // homura::HookPltFunction(libGameMain, AGVideoOpenOffset, AGVideoOpen, nullptr);
    // homura::HookPltFunction(libGameMain, AGVideoShowOffset, AGVideoShow, nullptr);
    // homura::HookPltFunction(libGameMain, AGVideoEnableOffset, AGVideoEnable, nullptr);
    // homura::HookPltFunction(libGameMain, AGVideoIsPlayingOffset, AGVideoIsPlaying, nullptr);
    // homura::HookPltFunction(libGameMain, AGVideoPlayOffset, AGVideoPlay, nullptr);
}


inline void CallHook() {
    InitHookFunction();
    InitVTableHookFunction();
    // InitIntroVideo();
}

#endif // PVZ_HOOK_INIT_H
