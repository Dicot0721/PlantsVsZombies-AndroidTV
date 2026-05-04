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

#include "PvZ/HookInit.h"
#include "Homura/HookUtils.h"
#include "Homura/MemberUtils.h"
#include "PvZ/Android/IntroVideo.h"
#include "PvZ/Android/Native/AudioOutput.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Board/Challenge.h"
#include "PvZ/Lawn/Board/Coin.h"
#include "PvZ/Lawn/Board/CursorObject.h"
#include "PvZ/Lawn/Board/CutScene.h"
#include "PvZ/Lawn/Board/GridItem.h"
#include "PvZ/Lawn/Board/MessageWidget.h"
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
#include "PvZ/Lawn/Widget/HelpBarWidget.h"
#include "PvZ/Lawn/Widget/HelpOptionsDialog.h"
#include "PvZ/Lawn/Widget/HelpTextScreen.h"
#include "PvZ/Lawn/Widget/HouseChooserDialog.h"
#include "PvZ/Lawn/Widget/ImitaterDialog.h"
#include "PvZ/Lawn/Widget/MailScreen.h"
#include "PvZ/Lawn/Widget/MainMenu.h"
#include "PvZ/Lawn/Widget/NewOptionsDialog.h"
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
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Effect/Reanimator.h"

void InitHookFunction() {
    homura::HookFunction(LawnApp_LawnAppAddr, homura::MemFuncPtrWrapper<&LawnApp::_constructor>::Get(), &old_LawnApp_LawnApp);
    homura::HookFunction(LawnApp_InitAddr, homura::MemFuncPtrWrapper<&LawnApp::Init>::Get(), &old_LawnApp_Init);
    homura::HookFunction(LawnApp_IsNightAddr, homura::MemFuncPtrWrapper<&LawnApp::IsNight>::Get(), &old_LawnApp_IsNight);
    homura::HookFunction(LawnApp_HardwareInitAddr, homura::MemFuncPtrWrapper<&LawnApp::HardwareInit>::Get(), &old_LawnApp_HardwareInit);
    homura::HookFunction(LawnApp_DoBackToMainAddr, homura::MemFuncPtrWrapper<&LawnApp::DoBackToMain>::Get(), &old_LawnApp_DoBackToMain);
    homura::HookFunction(LawnApp_DoSettingsDialogAddr, homura::MemFuncPtrWrapper<&LawnApp::DoSettingsDialog>::Get(), nullptr);
    homura::HookFunction(LawnApp_CanShopLevelAddr, homura::MemFuncPtrWrapper<&LawnApp::CanShopLevel>::Get(), &old_LawnApp_CanShopLevel);
    homura::HookFunction(LawnApp_DoNewOptionsAddr, homura::MemFuncPtrWrapper<&LawnApp::DoNewOptions>::Get(), &old_LawnApp_DoNewOptions);
    homura::HookFunction(LawnApp_GetNumPreloadingTasksAddr, homura::MemFuncPtrWrapper<&LawnApp::GetNumPreloadingTasks>::Get(), &old_LawnApp_GetNumPreloadingTasks);
    homura::HookFunction(LawnApp_DoConfirmBackToMainAddr, homura::MemFuncPtrWrapper<&LawnApp::DoConfirmBackToMain>::Get(), nullptr);
    homura::HookFunction(LawnApp_TrophiesNeedForGoldSunflowerAddr, homura::MemFuncPtrWrapper<&LawnApp::TrophiesNeedForGoldSunflower>::Get(), nullptr);
    homura::HookFunction(LawnApp_GamepadToPlayerIndexAddr, homura::MemFuncPtrWrapper<&LawnApp::GamepadToPlayerIndex>::Get(), &old_LawnApp_GamepadToPlayerIndex);
    homura::HookFunction(LawnApp_ShowCreditScreenAddr, homura::MemFuncPtrWrapper<&LawnApp::ShowCreditScreen>::Get(), &old_LawnApp_ShowCreditScreen);
    homura::HookFunction(LawnApp_OnSessionTaskFailedAddr, homura::MemFuncPtrWrapper<&LawnApp::OnSessionTaskFailed>::Get(), nullptr);
    homura::HookFunction(LawnApp_UpdateAppAddr, homura::MemFuncPtrWrapper<&LawnApp::UpdateApp>::Get(), &old_LawnApp_UpDateApp);
    homura::HookFunction(LawnApp_ShowAwardScreenAddr, homura::MemFuncPtrWrapper<&LawnApp::ShowAwardScreen>::Get(), &old_LawnApp_ShowAwardScreen);
    homura::HookFunction(LawnApp_KillAwardScreenAddr, homura::MemFuncPtrWrapper<&LawnApp::KillAwardScreen>::Get(), &old_LawnApp_KillAwardScreen);
    homura::HookFunction(LawnApp_LoadLevelConfigurationAddr, homura::MemFuncPtrWrapper<&LawnApp::LoadLevelConfiguration>::Get(), &old_LawnApp_LoadLevelConfiguration);
    homura::HookFunction(LawnApp_LoadingThreadProcAddr, homura::MemFuncPtrWrapper<&LawnApp::LoadingThreadProc>::Get(), &old_LawnApp_LoadingThreadProc);
    homura::HookFunction(LawnApp_IsChallengeWithoutSeedBankAddr, homura::MemFuncPtrWrapper<&LawnApp::IsChallengeWithoutSeedBank>::Get(), &old_LawnApp_IsChallengeWithoutSeedBank);
    homura::HookFunction(LawnApp_TryHelpTextScreenAddr, homura::MemFuncPtrWrapper<&LawnApp::TryHelpTextScreen>::Get(), nullptr);
    homura::HookFunction(LawnApp_GetSeedsAvailableAddr, homura::MemFuncPtrWrapper<&LawnApp::GetSeedsAvailable>::Get(), &old_LawnApp_GetSeedsAvailable);
    homura::HookFunction(LawnApp_ClearSecondPlayerAddr, homura::MemFuncPtrWrapper<&LawnApp::ClearSecondPlayer>::Get(), &old_LawnApp_ClearSecondPlayer);
    // homura::HookFunction(LawnApp_HasSeedTypeAddr, &LawnApp_HasSeedType, &old_LawnApp_HasSeedType);
    homura::HookFunction(LawnApp_UpdateFramesAddr, homura::MemFuncPtrWrapper<&LawnApp::UpdateFrames>::Get(), &old_LawnApp_UpdateFrames);
    homura::HookFunction(LawnApp_ShowSeedChooserScreenAddr, homura::MemFuncPtrWrapper<&LawnApp::ShowSeedChooserScreen>::Get(), nullptr);
    homura::HookFunction(LawnApp_KillSeedChooserScreenAddr, homura::MemFuncPtrWrapper<&LawnApp::KillSeedChooserScreen>::Get(), nullptr);
    homura::HookFunction(LawnApp_ShowZombieChooserScreenAddr, homura::MemFuncPtrWrapper<&LawnApp::ShowZombieChooserScreen>::Get(), nullptr);
    homura::HookFunction(LawnApp_KillZombieChooserScreenAddr, homura::MemFuncPtrWrapper<&LawnApp::KillZombieChooserScreen>::Get(), nullptr);
    homura::HookFunction(LawnApp_ShowChallengeScreenAddr, homura::MemFuncPtrWrapper<&LawnApp::ShowChallengeScreen>::Get(), nullptr);
    homura::HookFunction(LawnApp_KillChallengeScreenAddr, homura::MemFuncPtrWrapper<&LawnApp::KillChallengeScreen>::Get(), nullptr);
    homura::HookFunction(LawnApp_ShowVSSetupScreenAddr, homura::MemFuncPtrWrapper<&LawnApp::ShowVSSetupScreen>::Get(), nullptr);
    homura::HookFunction(LawnApp_PreNewGameAddr, homura::MemFuncPtrWrapper<&LawnApp::PreNewGame>::Get(), &old_LawnApp_PreNewGame);
    homura::HookFunction(LawnApp_NewGameAddr, homura::MemFuncPtrWrapper<&LawnApp::NewGame>::Get(), nullptr);
    homura::HookFunction(LawnApp_HasBeatenChallengeAddr, homura::MemFuncPtrWrapper<&LawnApp::HasBeatenChallenge>::Get(), nullptr);


    homura::HookFunction(Board_DrawAddr, homura::MemFuncPtrWrapper<&Board::Draw>::Get(), &old_Board_Draw);
    homura::HookFunction(Board_UpdateAddr, homura::MemFuncPtrWrapper<&Board::Update>::Get(), &old_Board_Update);
    homura::HookFunction(Board_BoardAddr, homura::MemFuncPtrWrapper<&Board::_constructor>::Get(), &old_Board_Board);
    homura::HookFunction(Board__destructorAddr, homura::MemFuncPtrWrapper<&Board::_destructor>::Get(), &old_Board__destructor);
    homura::HookFunction(Board_InitLevelAddr, homura::MemFuncPtrWrapper<&Board::InitLevel>::Get(), &old_Board_InitLevel);
    homura::HookFunction(Board_StartLevelAddr, homura::MemFuncPtrWrapper<&Board::StartLevel>::Get(), &old_Board_StartLevel);
    homura::HookFunction(Board_RemovedFromManagerAddr, homura::MemFuncPtrWrapper<&Board::RemovedFromManager>::Get(), &old_Board_RemovedFromManager);
    homura::HookFunction(Board_FadeOutLevelAddr, homura::MemFuncPtrWrapper<&Board::FadeOutLevel>::Get(), &old_Board_FadeOutLevel);
    homura::HookFunction(Board_AddPlantAddr, homura::MemFuncPtrWrapper<&Board::AddPlant>::Get(), &old_Board_AddPlant);
    homura::HookFunction(Board_AddSunMoneyAddr, homura::MemFuncPtrWrapper<&Board::AddSunMoney>::Get(), &old_Board_AddSunMoney);
    homura::HookFunction(Board_AddDeathMoneyAddr, homura::MemFuncPtrWrapper<&Board::AddDeathMoney>::Get(), &old_Board_AddDeathMoney);
    homura::HookFunction(Board_CanPlantAtAddr, homura::MemFuncPtrWrapper<&Board::CanPlantAt>::Get(), nullptr);
    homura::HookFunction(Board_PlantingRequirementsMetAddr, homura::MemFuncPtrWrapper<&Board::PlantingRequirementsMet>::Get(), &old_Board_PlantingRequirementsMet);
    homura::HookFunction(Board_GetFlowerPotAtAddr, homura::MemFuncPtrWrapper<&Board::GetFlowerPotAt>::Get(), nullptr);
    homura::HookFunction(Board_GetPumpkinAtAddr, homura::MemFuncPtrWrapper<&Board::GetPumpkinAt>::Get(), nullptr);
    homura::HookFunction(Board_ZombiesWonAddr, homura::MemFuncPtrWrapper<&Board::ZombiesWon>::Get(), &old_BoardZombiesWon);
    homura::HookFunction(Board_KeyDownAddr, homura::MemFuncPtrWrapper<&Board::KeyDown>::Get(), &old_Board_KeyDown);
    homura::HookFunction(Board_UpdateSunSpawningAddr, homura::MemFuncPtrWrapper<&Board::UpdateSunSpawning>::Get(), nullptr);
    homura::HookFunction(Board_UpdateZombieSpawningAddr, homura::MemFuncPtrWrapper<&Board::UpdateZombieSpawning>::Get(), &old_Board_UpdateZombieSpawning);
    homura::HookFunction(Board_PickBackgroundAddr, homura::MemFuncPtrWrapper<&Board::PickBackground>::Get(), &old_Board_PickBackground);
    homura::HookFunction(Board_DrawCoverLayerAddr, homura::MemFuncPtrWrapper<&Board::DrawCoverLayer>::Get(), nullptr);
    homura::HookFunction(Board_UpdateGameAddr, homura::MemFuncPtrWrapper<&Board::UpdateGame>::Get(), &old_Board_UpdateGame);
    homura::HookFunction(Board_UpdateGameObjectsAddr, homura::MemFuncPtrWrapper<&Board::UpdateGameObjects>::Get(), &old_Board_UpdateGameObjects);
    homura::HookFunction(Board_IsFlagWaveAddr, homura::MemFuncPtrWrapper<&Board::IsFlagWave>::Get(), &old_Board_IsFlagWave);
    homura::HookFunction(Board_SpawnZombieWaveAddr, homura::MemFuncPtrWrapper<&Board::SpawnZombieWave>::Get(), &old_Board_SpawnZombieWave);
    homura::HookFunction(Board_DrawProgressMeterAddr, homura::MemFuncPtrWrapper<&Board::DrawProgressMeter>::Get(), &old_Board_DrawProgressMeter);
    homura::HookFunction(Board_GetNumWavesPerFlagAddr, homura::MemFuncPtrWrapper<&Board::GetNumWavesPerFlag>::Get(), nullptr);
    homura::HookFunction(Board_IsLevelDataLoadedAddr, homura::MemFuncPtrWrapper<&Board::IsLevelDataLoaded>::Get(), &old_Board_IsLevelDataLoaded);
    homura::HookFunction(Board_NeedSaveGameAddr, homura::MemFuncPtrWrapper<&Board::NeedSaveGame>::Get(), &old_Board_NeedSaveGame);
    homura::HookFunction(Board_UpdateFwooshAddr, homura::MemFuncPtrWrapper<&Board::UpdateFwoosh>::Get(), &old_Board_UpdateFwoosh);
    homura::HookFunction(Board_UpdateFogAddr, homura::MemFuncPtrWrapper<&Board::UpdateFog>::Get(), &old_Board_UpdateFog);
    homura::HookFunction(Board_DrawFogAddr, homura::MemFuncPtrWrapper<&Board::DrawFog>::Get(), &old_Board_DrawFog);
    homura::HookFunction(Board_UpdateIceAddr, homura::MemFuncPtrWrapper<&Board::UpdateIce>::Get(), &old_Board_UpdateIce);
    homura::HookFunction(Board_DrawBackdropAddr, homura::MemFuncPtrWrapper<&Board::DrawBackdrop>::Get(), &old_Board_DrawBackdrop);
    homura::HookFunction(Board_RowCanHaveZombieTypeAddr, homura::MemFuncPtrWrapper<&Board::RowCanHaveZombieType>::Get(), &old_Board_RowCanHaveZombieType);
    homura::HookFunction(Board_DrawDebugTextAddr, homura::MemFuncPtrWrapper<&Board::DrawDebugText>::Get(), &old_Board_DrawDebugText);
    homura::HookFunction(Board_DrawDebugObjectRectsAddr, homura::MemFuncPtrWrapper<&Board::DrawDebugObjectRects>::Get(), &old_Board_DrawDebugObjectRects);
    homura::HookFunction(Board_DrawFadeOutAddr, homura::MemFuncPtrWrapper<&Board::DrawFadeOut>::Get(), nullptr);
    homura::HookFunction(Board_GetCurrentPlantCostAddr, homura::MemFuncPtrWrapper<&Board::GetCurrentPlantCost>::Get(), &old_Board_GetCurrentPlantCost);
    homura::HookFunction(Board_PauseAddr, homura::MemFuncPtrWrapper<&Board::Pause>::Get(), &old_Board_Pause);
    homura::HookFunction(Board_AddSecondPlayerAddr, homura::MemFuncPtrWrapper<&Board::AddSecondPlayer>::Get(), nullptr);
    homura::HookFunction(Board_IsLastStandFinalStageAddr, homura::MemFuncPtrWrapper<&Board::IsLastStandFinalStage>::Get(), nullptr);
    homura::HookFunction(Board_MouseHitTestAddr, homura::MemFuncPtrWrapper<&Board::MouseHitTest>::Get(), &old_Board_MouseHitTest);
    homura::HookFunction(Board_DrawShovelAddr, homura::MemFuncPtrWrapper<&Board::DrawShovel>::Get(), nullptr);
    homura::HookFunction(Board_StageHasPoolAddr, homura::MemFuncPtrWrapper<&Board::StageHasPool>::Get(), nullptr);
    homura::HookFunction(Board_AddZombieInRowAddr, homura::MemFuncPtrWrapper<&Board::AddZombieInRow>::Get(), &old_Board_AddZombieInRow);
    homura::HookFunction(Board_AddZombieAddr, homura::MemFuncPtrWrapper<&Board::AddZombie>::Get(), nullptr);
    homura::HookFunction(Board_DoPlantingEffectsAddr, homura::MemFuncPtrWrapper<&Board::DoPlantingEffects>::Get(), nullptr);
    homura::HookFunction(Board_InitLawnMowersAddr, homura::MemFuncPtrWrapper<&Board::InitLawnMowers>::Get(), &old_Board_InitLawnMowers);
    homura::HookFunction(Board_PickZombieWavesAddr, homura::MemFuncPtrWrapper<&Board::PickZombieWaves>::Get(), &old_Board_PickZombieWaves);
    homura::HookFunction(Board_DrawUITopAddr, homura::MemFuncPtrWrapper<&Board::DrawUITop>::Get(), &old_Board_DrawUITop);
    homura::HookFunction(Board_GetShovelButtonRectAddr, homura::MemFuncPtrWrapper<&Board::GetShovelButtonRect>::Get(), &old_Board_GetShovelButtonRect);
    homura::HookFunction(Board_UpdateLevelEndSequenceAddr, homura::MemFuncPtrWrapper<&Board::UpdateLevelEndSequence>::Get(), &old_Board_UpdateLevelEndSequence);
    homura::HookFunction(Board_UpdateGridItemsAddr, homura::MemFuncPtrWrapper<&Board::UpdateGridItems>::Get(), &old_Board_UpdateGridItems);
    homura::HookFunction(Board_ShakeBoardAddr, homura::MemFuncPtrWrapper<&Board::ShakeBoard>::Get(), &old_Board_ShakeBoard);
    homura::HookFunction(Board_DrawZenButtonsAddr, homura::MemFuncPtrWrapper<&Board::DrawZenButtons>::Get(), &old_Board_DrawZenButtons);
    homura::HookFunction(Board_DrawGameObjectsAddr, homura::MemFuncPtrWrapper<&Board::DrawGameObjects>::Get(), &old_Board_DrawGameObjects);
    // homura::HookFunction(Board_AddProjectileAddr, homura::MemFuncPtrWrapper<&Board::AddProjectile>::Get(), &old_Board_AddProjectile);
    // homura::HookFunction(Board_PixelToGridXAddr, homura::MemFuncPtrWrapper<&Board::PixelToGridX>::Get(), &old_Board_PixelToGridX);
    // homura::HookFunction(Board_PixelToGridYAddr, homura::MemFuncPtrWrapper<&Board::PixelToGridY>::Get(), &old_Board_PixelToGridY);
    homura::HookFunction(Board_GetNumSeedsInBankAddr, homura::MemFuncPtrWrapper<&Board::GetNumSeedsInBank>::Get(), &old_Board_GetNumSeedsInBank);
    homura::HookFunction(Board_GetSeedPacketPositionXAddr, homura::MemFuncPtrWrapper<&Board::GetSeedPacketPositionX>::Get(), nullptr);
    homura::HookFunction(Board_AddCoinAddr, homura::MemFuncPtrWrapper<&Board::AddCoin>::Get(), &old_Board_AddCoin);
    homura::HookFunction(Board_AddAGraveStoneAddr, homura::MemFuncPtrWrapper<&Board::AddAGraveStone>::Get(), &old_Board_AddAGraveStone);
    homura::HookFunction(Board_TakeSunMoneyAddr, homura::MemFuncPtrWrapper<&Board::TakeSunMoney>::Get(), &old_Board_TakeSunMoney);
    homura::HookFunction(Board_TakeDeathMoneyAddr, homura::MemFuncPtrWrapper<&Board::TakeDeathMoney>::Get(), &old_Board_TakeDeathMoney);
    homura::HookFunction(Board_SpawnZombiesFromGravesAddr, homura::MemFuncPtrWrapper<&Board::SpawnZombiesFromGraves>::Get(), nullptr);
    homura::HookFunction(Board_CanAddGraveStoneAtAddr, homura::MemFuncPtrWrapper<&Board::CanAddGraveStoneAt>::Get(), nullptr);
    homura::HookFunction(Board_DrawLevelAddr, homura::MemFuncPtrWrapper<&Board::DrawLevel>::Get(), &old_Board_DrawLevel);
    homura::HookFunction(Board_CanAddBobSledMPAddr, homura::MemFuncPtrWrapper<&Board::CanAddBobSledMP>::Get(), nullptr);
    homura::HookFunction(Board_AddMPTargetAddr, homura::MemFuncPtrWrapper<&Board::AddMPTarget>::Get(), nullptr);
    homura::HookFunction(Board_PlantsWonAddr, homura::MemFuncPtrWrapper<&Board::PlantsWon>::Get(), &old_Board_PlantsWon);
    homura::HookFunction(Board_AddALadderAddr, homura::MemFuncPtrWrapper<&Board::AddALadder>::Get(), nullptr);
    homura::HookFunction(Board_AddACraterAddr, homura::MemFuncPtrWrapper<&Board::AddACrater>::Get(), nullptr);


    homura::HookFunction(FixBoardAfterLoadAddr, &FixBoardAfterLoad, &old_FixBoardAfterLoad);
    homura::HookFunction(LawnSaveGameAddr, &LawnSaveGame, &old_LawnSaveGame);
    homura::HookFunction(LawnLoadGameAddr, &LawnLoadGame, &old_LawnLoadGame);

    homura::HookFunction(Challenge_UpdateAddr, homura::MemFuncPtrWrapper<&Challenge::Update>::Get(), &old_Challenge_Update);
    homura::HookFunction(Challenge_ChallengeAddr, homura::MemFuncPtrWrapper<&Challenge::_constructor>::Get(), &old_Challenge_Challenge);
    homura::HookFunction(Challenge_HeavyWeaponFireAddr, homura::MemFuncPtrWrapper<&Challenge::HeavyWeaponFire>::Get(), &old_Challenge_HeavyWeaponFire);
    homura::HookFunction(Challenge_IZombieDrawPlantAddr, homura::MemFuncPtrWrapper<&Challenge::IZombieDrawPlant>::Get(), nullptr);
    homura::HookFunction(Challenge_HeavyWeaponUpdateAddr, homura::MemFuncPtrWrapper<&Challenge::HeavyWeaponUpdate>::Get(), &old_Challenge_HeavyWeaponUpdate);
    homura::HookFunction(Challenge_IZombieEatBrainAddr, homura::MemFuncPtrWrapper<&Challenge::IZombieEatBrain>::Get(), nullptr);
    homura::HookFunction(Challenge_DrawArtChallengeAddr, homura::MemFuncPtrWrapper<&Challenge::DrawArtChallenge>::Get(), nullptr);
    homura::HookFunction(Challenge_CanPlantAtAddr, homura::MemFuncPtrWrapper<&Challenge::CanPlantAt>::Get(), nullptr);
    homura::HookFunction(Challenge_InitLevelAddr, homura::MemFuncPtrWrapper<&Challenge::InitLevel>::Get(), &old_Challenge_InitLevel);
    homura::HookFunction(Challenge_InitZombieWavesAddr, homura::MemFuncPtrWrapper<&Challenge::InitZombieWaves>::Get(), &old_Challenge_InitZombieWaves);
    homura::HookFunction(Challenge_TreeOfWisdomFertilizeAddr, homura::MemFuncPtrWrapper<&Challenge::TreeOfWisdomFertilize>::Get(), &old_Challenge_TreeOfWisdomFertilize);
    homura::HookFunction(Challenge_LastStandUpdateAddr, homura::MemFuncPtrWrapper<&Challenge::LastStandUpdate>::Get(), nullptr);
    homura::HookFunction(Challenge_DrawHeavyWeaponAddr, homura::MemFuncPtrWrapper<&Challenge::DrawHeavyWeapon>::Get(), nullptr);
    homura::HookFunction(Challenge_UpdateZombieSpawningAddr, homura::MemFuncPtrWrapper<&Challenge::UpdateZombieSpawning>::Get(), &old_Challenge_UpdateZombieSpawning);
    homura::HookFunction(Challenge_HeavyWeaponPacketClickedAddr, homura::MemFuncPtrWrapper<&Challenge::HeavyWeaponPacketClicked>::Get(), &old_Challenge_HeavyWeaponPacketClicked);
    homura::HookFunction(Challenge_IZombieSeedTypeToZombieTypeAddr, &Challenge::IZombieSeedTypeToZombieType, nullptr);
    homura::HookFunction(Challenge_StartLevelAddr, homura::MemFuncPtrWrapper<&Challenge::StartLevel>::Get(), &old_Challenge_StartLevel);
    homura::HookFunction(Challenge_DeleteAddr, homura::MemFuncPtrWrapper<&Challenge::_destructor>::Get(), &old_Challenge_Delete);
    homura::HookFunction(Challenge_ScaryPotterOpenPotAddr, homura::MemFuncPtrWrapper<&Challenge::ScaryPotterOpenPot>::Get(), &old_Challenge_ScaryPotterOpenPot);
    homura::HookFunction(Challenge_IZombieGetBrainTargetAddr, homura::MemFuncPtrWrapper<&Challenge::IZombieGetBrainTarget>::Get(), &old_Challenge_IZombieGetBrainTarget);
    homura::HookFunction(Challenge_IZombieSquishBrainAddr, homura::MemFuncPtrWrapper<&Challenge::IZombieSquishBrain>::Get(), &old_Challenge_IZombieSquishBrain);
    homura::HookFunction(Challenge_UpdateConveyorBeltAddr, homura::MemFuncPtrWrapper<&Challenge::UpdateConveyorBelt>::Get(), &old_Challenge_UpdateConveyorBelt);
    homura::HookFunction(Challenge_MouseDownWhackAZombieAddr, homura::MemFuncPtrWrapper<&Challenge::MouseDownWhackAZombie>::Get(), nullptr);
    homura::HookFunction(Challenge_DrawWeatherAddr, homura::MemFuncPtrWrapper<&Challenge::DrawWeather>::Get(), nullptr);
    homura::HookFunction(Challenge_UpdateMPGraveStonesAddr, homura::MemFuncPtrWrapper<&Challenge::UpdateMPGraveStones>::Get(), nullptr);
    homura::HookFunction(Challenge_IsMPResourceProducerAddr, &Challenge::IsMPResourceProducer, nullptr);
    homura::HookFunction(Challenge_ISMPSeedSuddenDeathDisabledAddr, homura::MemFuncPtrWrapper<&Challenge::ISMPSeedSuddenDeathDisabled>::Get(), nullptr);


    homura::HookFunction(ChallengeScreen_AddedToManagerAddr, homura::MemFuncPtrWrapper<&ChallengeScreen::AddedToManager>::Get(), &old_ChallengeScreen_AddedToManager);
    homura::HookFunction(ChallengeScreen_RemovedFromManagerAddr, homura::MemFuncPtrWrapper<&ChallengeScreen::RemovedFromManager>::Get(), &old_ChallengeScreen_RemovedFromManager);
    homura::HookFunction(ChallengeScreen_Delete2Addr, homura::MemFuncPtrWrapper<&ChallengeScreen::_destructor2>::Get(), &old_ChallengeScreen_Delete2);
    homura::HookFunction(ChallengeScreen_UpdateAddr, homura::MemFuncPtrWrapper<&ChallengeScreen::Update>::Get(), &old_ChallengeScreen_Update);
    homura::HookFunction(ChallengeScreen_ChallengeScreenAddr, homura::MemFuncPtrWrapper<&ChallengeScreen::_constructor>::Get(), &old_ChallengeScreen_ChallengeScreen);
    homura::HookFunction(ChallengeScreen_DrawAddr, homura::MemFuncPtrWrapper<&ChallengeScreen::Draw>::Get(), &old_ChallengeScreen_Draw);
    homura::HookFunction(ChallengeScreen_KeyDownAddr, homura::MemFuncPtrWrapper<&ChallengeScreen::KeyDown>::Get(), &old_ChallengeScreen_KeyDown);
    homura::HookFunction(ChallengeScreen_ButtonDepressAddr, homura::MemFuncPtrWrapper<&ChallengeScreen::ButtonDepress>::Get(), nullptr);
    homura::HookFunction(ChallengeScreen_UpdateButtonsAddr, homura::MemFuncPtrWrapper<&ChallengeScreen::UpdateButtons>::Get(), nullptr);
    homura::HookFunction(ChallengeScreen_DrawButtonAddr, homura::MemFuncPtrWrapper<&ChallengeScreen::DrawButton>::Get(), &old_ChallengeScreen_DrawButton);
    homura::HookFunction(GetChallengeDefinitionAddr, &GetChallengeDefinition, nullptr);


    homura::HookFunction(Coin_CoinInitializeAddr, homura::MemFuncPtrWrapper<&Coin::CoinInitialize>::Get(), &old_Coin_CoinInitialize);
    homura::HookFunction(Coin_UpadteAddr, homura::MemFuncPtrWrapper<&Coin::Update>::Get(), &old_Coin_Update);
    homura::HookFunction(Coin_GamepadCursorOverAddr, homura::MemFuncPtrWrapper<&Coin::GamepadCursorOver>::Get(), &old_Coin_GamepadCursorOver);
    homura::HookFunction(Coin_MouseHitTestAddr, homura::MemFuncPtrWrapper<&Coin::MouseHitTest>::Get(), &old_Coin_MouseHitTest);
    homura::HookFunction(Coin_UpdateFallAddr, homura::MemFuncPtrWrapper<&Coin::UpdateFall>::Get(), &old_Coin_UpdateFall);
    homura::HookFunction(Coin_DrawAddr, homura::MemFuncPtrWrapper<&Coin::Draw>::Get(), &old_Coin_Draw);
    homura::HookFunction(Coin_GetColorAddr, homura::MemFuncPtrWrapper<&Coin::GetColor>::Get(), nullptr);


    homura::HookFunction(
        GamepadControls_ButtonDownFireCobcannonTestAddr, homura::MemFuncPtrWrapper<&GamepadControls::ButtonDownFireCobcannonTest>::Get(), &old_GamepadControls_ButtonDownFireCobcannonTest);
    homura::HookFunction(GamepadControls_DrawAddr, homura::MemFuncPtrWrapper<&GamepadControls::Draw>::Get(), &old_GamepadControls_Draw);
    homura::HookFunction(GamepadControls_GamepadControlsAddr, homura::MemFuncPtrWrapper<&GamepadControls::_constructor>::Get(), &old_GamepadControls_GamepadControls);
    homura::HookFunction(GamepadControls_UpdateAddr, homura::MemFuncPtrWrapper<&GamepadControls::Update>::Get(), &old_GamepadControls_Update);
    homura::HookFunction(GamepadControls_DrawPreviewAddr, homura::MemFuncPtrWrapper<&GamepadControls::DrawPreview>::Get(), &old_GamepadControls_DrawPreview);
    homura::HookFunction(GamepadControls_UpdatePreviewReanimAddr, homura::MemFuncPtrWrapper<&GamepadControls::UpdatePreviewReanim>::Get(), &old_GamepadControls_UpdatePreviewReanim);
    homura::HookFunction(GamepadControls_OnButtonDownAddr, homura::MemFuncPtrWrapper<&GamepadControls::OnButtonDown>::Get(), &old_GamepadControls_OnButtonDown);


    homura::HookFunction(GridItem__constructorAddr, homura::MemFuncPtrWrapper<&GridItem::_constructor>::Get(), &old_GridItem_GridItem);
    homura::HookFunction(GridItem_GridItemDieAddr, homura::MemFuncPtrWrapper<&GridItem::GridItemDie>::Get(), &old_GridItem_GridItemDie);
    homura::HookFunction(GridItem_DrawGridItemAddr, homura::MemFuncPtrWrapper<&GridItem::DrawGridItem>::Get(), &old_GridItem_DrawGridItem);
    homura::HookFunction(GridItem_UpdateAddr, homura::MemFuncPtrWrapper<&GridItem::Update>::Get(), &old_GridItem_Update);
    homura::HookFunction(GridItem_UpdateScaryPotAddr, homura::MemFuncPtrWrapper<&GridItem::UpdateScaryPot>::Get(), &old_GridItem_UpdateScaryPot);
    homura::HookFunction(GridItem_UpdateBurialMoundAddr, homura::MemFuncPtrWrapper<&GridItem::UpdateBurialMound>::Get(), nullptr);
    homura::HookFunction(GridItem_GetMoundUpgradeCostAddr, homura::MemFuncPtrWrapper<&GridItem::GetMoundUpgradeCost>::Get(), nullptr);
    homura::HookFunction(GridItem_DrawStinkyAddr, homura::MemFuncPtrWrapper<&GridItem::DrawStinky>::Get(), &old_GridItem_DrawStinky);
    homura::HookFunction(GridItem_DrawSquirrelAddr, homura::MemFuncPtrWrapper<&GridItem::DrawSquirrel>::Get(), nullptr);
    homura::HookFunction(GridItem_DrawScaryPotAddr, homura::MemFuncPtrWrapper<&GridItem::DrawScaryPot>::Get(), nullptr);
    homura::HookFunction(GridItem_DrawCraterAddr, homura::MemFuncPtrWrapper<&GridItem::DrawCrater>::Get(), nullptr);
    homura::HookFunction(GridItem_DrawGraveStoneAddr, homura::MemFuncPtrWrapper<&GridItem::DrawGraveStone>::Get(), nullptr);
    homura::HookFunction(GridItem_AddGraveStoneParticlesAddr, homura::MemFuncPtrWrapper<&GridItem::AddGraveStoneParticles>::Get(), nullptr);
    //    homura::HookFunction(GridItem_DrawMPTargetAddr, homura::MemFuncPtrWrapper<&GridItem::DrawMPTarget>::Get(), &old_GridItem_DrawMPTarget);
    homura::HookFunction(GridItem_TakeDamgaeAddr, homura::MemFuncPtrWrapper<&GridItem::TakeDamgae>::Get(), &old_GridItem_TakeDamage);


    homura::HookFunction(AlmanacDialog_RemovedFromManagerAddr, homura::MemFuncPtrWrapper<&AlmanacDialog::RemovedFromManager>::Get(), &old_AlmanacDialog_RemovedFromManager);
    homura::HookFunction(AlmanacDialog_AlmanacDialogAddr, homura::MemFuncPtrWrapper<&AlmanacDialog::_constructor>::Get(), &old_AlmanacDialog_AlmanacDialog);
    homura::HookFunction(AlmanacDialog_SetPageAddr, homura::MemFuncPtrWrapper<&AlmanacDialog::SetPage>::Get(), &old_AlmanacDialog_SetPage);
    homura::HookFunction(AlmanacDialog_MouseDownAddr, homura::MemFuncPtrWrapper<&AlmanacDialog::MouseDown>::Get(), nullptr);
    homura::HookFunction(AlmanacDialog_MouseUpAddr, homura::MemFuncPtrWrapper<&AlmanacDialog::MouseUp>::Get(), nullptr);
    homura::HookFunction(AlmanacDialog_ButtonDepressAddr, homura::MemFuncPtrWrapper<&AlmanacDialog::ButtonDepress>::Get(), nullptr);
    homura::HookFunction(AlmanacDialog_Delete2Addr, homura::MemFuncPtrWrapper<&AlmanacDialog::_destructor>::Get(), &old_AlmanacDialog_Delete2);
    homura::HookFunction(AlmanacDialog_DrawPlantsAddr, homura::MemFuncPtrWrapper<&AlmanacDialog::DrawPlants>::Get(), &old_AlmanacDialog_DrawPlants);
    homura::HookFunction(AlmanacDialog_SetupLayoutPlantsAddr, homura::MemFuncPtrWrapper<&AlmanacDialog::SetupLayoutPlants>::Get(), &old_AlmanacDialog_SetupLayoutPlants);


    homura::HookFunction(SeedChooserScreen_SeedChooserScreenAddr, homura::MemFuncPtrWrapper<&SeedChooserScreen::_constructor>::Get(), &old_SeedChooserScreen_SeedChooserScreen);
    homura::HookFunction(SeedChooserScreen__destructorAddr, homura::MemFuncPtrWrapper<&SeedChooserScreen::_destructor>::Get(), &old_SeedChooserScreen__destructor);
    homura::HookFunction(SeedChooserScreen_EnableStartButtonAddr, homura::MemFuncPtrWrapper<&SeedChooserScreen::EnableStartButton>::Get(), &old_SeedChooserScreen_EnableStartButton);
    homura::HookFunction(SeedChooserScreen_RebuildHelpbarAddr, homura::MemFuncPtrWrapper<&SeedChooserScreen::RebuildHelpbar>::Get(), &old_SeedChooserScreen_RebuildHelpbar);
    homura::HookFunction(SeedChooserScreen_GetZombieSeedTypeAddr, homura::MemFuncPtrWrapper<&SeedChooserScreen::GetZombieSeedType>::Get(), nullptr);
    homura::HookFunction(SeedChooserScreen_ClickedSeedInChooserAddr, homura::MemFuncPtrWrapper<&SeedChooserScreen::ClickedSeedInChooser>::Get(), &old_SeedChooserScreen_ClickedSeedInChooser);
    homura::HookFunction(SeedChooserScreen_CrazyDavePickSeedsAddr, homura::MemFuncPtrWrapper<&SeedChooserScreen::CrazyDavePickSeeds>::Get(), &old_SeedChooserScreen_CrazyDavePickSeeds);
    homura::HookFunction(SeedChooserScreen_OnStartButtonAddr, homura::MemFuncPtrWrapper<&SeedChooserScreen::OnStartButton>::Get(), &old_SeedChooserScreen_OnStartButton);
    homura::HookFunction(SeedChooserScreen_UpdateAddr, homura::MemFuncPtrWrapper<&SeedChooserScreen::Update>::Get(), &old_SeedChooserScreen_Update);
    homura::HookFunction(SeedChooserScreen_SeedNotAllowedToPickAddr, homura::MemFuncPtrWrapper<&SeedChooserScreen::SeedNotAllowedToPick>::Get(), &old_SeedChooserScreen_SeedNotAllowedToPick);
    homura::HookFunction(SeedChooserScreen_ClickedSeedInBankAddr, homura::MemFuncPtrWrapper<&SeedChooserScreen::ClickedSeedInBank>::Get(), &old_SeedChooserScreen_ClickedSeedInBank);
    homura::HookFunction(SeedChooserScreen_GameButtonDownAddr, homura::MemFuncPtrWrapper<&SeedChooserScreen::GameButtonDown>::Get(), &old_SeedChooserScreen_GameButtonDown);
    homura::HookFunction(SeedChooserScreen_DrawPacketAddr, homura::MemFuncPtrWrapper<&SeedChooserScreen::DrawPacket>::Get(), nullptr);
    homura::HookFunction(SeedChooserScreen_ButtonDepressAddr, homura::MemFuncPtrWrapper<&SeedChooserScreen::ButtonDepress>::Get(), &old_SeedChooserScreen_ButtonDepress);
    homura::HookFunction(SeedChooserScreen_GetSeedPositionInBankAddr, homura::MemFuncPtrWrapper<&SeedChooserScreen::GetSeedPositionInBank>::Get(), &old_SeedChooserScreen_GetSeedPositionInBank);
    homura::HookFunction(SeedChooserScreen_GetSeedPositionInChooserAddr, homura::MemFuncPtrWrapper<&SeedChooserScreen::GetSeedPositionInChooser>::Get(), nullptr);
    homura::HookFunction(SeedChooserScreen_ShowToolTipAddr, homura::MemFuncPtrWrapper<&SeedChooserScreen::ShowToolTip>::Get(), &old_SeedChooserScreen_ShowToolTip);
    homura::HookFunction(SeedChooserScreen_GetNextSeedInDirAddr, homura::MemFuncPtrWrapper<&SeedChooserScreen::GetNextSeedInDir>::Get(), nullptr);
    homura::HookFunction(SeedChooserScreen_DrawAddr, homura::MemFuncPtrWrapper<&SeedChooserScreen::Draw>::Get(), &old_SeedChooserScreen_Draw);
    homura::HookFunction(SeedChooserScreen_SeedHitTestAddr, homura::MemFuncPtrWrapper<&SeedChooserScreen::SeedHitTest>::Get(), &old_SeedChooserScreen_SeedHitTest);
    homura::HookFunction(SeedChooserScreen_OnKeyDownAddr, homura::MemFuncPtrWrapper<&SeedChooserScreen::OnKeyDown>::Get(), &old_SeedChooserScreen_OnKeyDown);
    homura::HookFunction(SeedChooserScreen_VSAutoPickResourceGenAddr, homura::MemFuncPtrWrapper<&SeedChooserScreen::VSAutoPickResourceGen>::Get(), nullptr);


    homura::HookFunction(MainMenu_KeyDownAddr, homura::MemFuncPtrWrapper<&MainMenu::KeyDown>::Get(), &old_MainMenu_KeyDown);
    homura::HookFunction(MainMenu_ButtonDepressAddr, homura::MemFuncPtrWrapper<&MainMenu::ButtonDepress>::Get(), &old_MainMenu_ButtonDepress);
    homura::HookFunction(MainMenu_UpdateAddr, homura::MemFuncPtrWrapper<&MainMenu::Update>::Get(), &old_MainMenu_Update);
    homura::HookFunction(MainMenu_SyncProfileAddr, homura::MemFuncPtrWrapper<&MainMenu::SyncProfile>::Get(), &old_MainMenu_SyncProfile);
    homura::HookFunction(MainMenu_EnterAddr, homura::MemFuncPtrWrapper<&MainMenu::Enter>::Get(), &old_MainMenu_Enter);
    homura::HookFunction(MainMenu_ExitAddr, homura::MemFuncPtrWrapper<&MainMenu::Exit>::Get(), &old_MainMenu_Exit);
    homura::HookFunction(MainMenu_UpdateExitAddr, homura::MemFuncPtrWrapper<&MainMenu::UpdateExit>::Get(), &old_MainMenu_UpdateExit);
    homura::HookFunction(MainMenu_OnExitAddr, homura::MemFuncPtrWrapper<&MainMenu::OnExit>::Get(), &old_MainMenu_OnExit);
    // homura::HookFunction(MainMenu_SetSceneAddr, SetScene, &old_MainMenu_SetScene);
    homura::HookFunction(MainMenu_OnSceneAddr, homura::MemFuncPtrWrapper<&MainMenu::OnScene>::Get(), &old_MainMenu_OnScene);
    homura::HookFunction(MainMenu_SyncButtonsAddr, homura::MemFuncPtrWrapper<&MainMenu::SyncButtons>::Get(), &old_MainMenu_SyncButtons);
    homura::HookFunction(MainMenu_MainMenuAddr, homura::MemFuncPtrWrapper<&MainMenu::_constructor>::Get(), &old_MainMenu_MainMenu);
    homura::HookFunction(MainMenu_UpdateCameraPositionAddr, homura::MemFuncPtrWrapper<&MainMenu::UpdateCameraPosition>::Get(), &old_MainMenu_UpdateCameraPosition);
    homura::HookFunction(MainMenu_AddedToManagerAddr, homura::MemFuncPtrWrapper<&MainMenu::AddedToManager>::Get(), &old_MainMenu_AddedToManager);
    homura::HookFunction(MainMenu_RemovedFromManagerAddr, homura::MemFuncPtrWrapper<&MainMenu::RemovedFromManager>::Get(), &old_MainMenu_RemovedFromManager);
    homura::HookFunction(MainMenu_DrawOverlayAddr, homura::MemFuncPtrWrapper<&MainMenu::DrawOverlay>::Get(), &old_MainMenu_DrawOverlay);
    homura::HookFunction(MainMenu_DrawFadeAddr, homura::MemFuncPtrWrapper<&MainMenu::DrawFade>::Get(), &old_MainMenu_DrawFade);
    homura::HookFunction(MainMenu_Delete2Addr, homura::MemFuncPtrWrapper<&MainMenu::_destructor2>::Get(), &old_MainMenu_Delete2);
    homura::HookFunction(MainMenu_DrawAddr, homura::MemFuncPtrWrapper<&MainMenu::Draw>::Get(), &old_MainMenu_Draw);


    homura::HookFunction(StoreScreen_UpdateAddr, homura::MemFuncPtrWrapper<&StoreScreen::Update>::Get(), &old_StoreScreen_Update);
    homura::HookFunction(StoreScreen_SetupPageAddr, homura::MemFuncPtrWrapper<&StoreScreen::SetupPage>::Get(), &old_StoreScreen_SetupPage);
    homura::HookFunction(StoreScreen_IsPageShownAddr, homura::MemFuncPtrWrapper<&StoreScreen::IsPageShown>::Get(), nullptr);
    homura::HookFunction(StoreScreen_ButtonDepressAddr, homura::MemFuncPtrWrapper<&StoreScreen::ButtonDepress>::Get(), &old_StoreScreen_ButtonDepress);
    homura::HookFunction(StoreScreen_AddedToManagerAddr, homura::MemFuncPtrWrapper<&StoreScreen::AddedToManager>::Get(), &old_StoreScreen_AddedToManager);
    homura::HookFunction(StoreScreen_RemovedFromManagerAddr, homura::MemFuncPtrWrapper<&StoreScreen::RemovedFromManager>::Get(), &old_StoreScreen_RemovedFromManager);
    homura::HookFunction(StoreScreen_PurchaseItemAddr, homura::MemFuncPtrWrapper<&StoreScreen::PurchaseItem>::Get(), &old_StoreScreen_PurchaseItem);
    homura::HookFunction(StoreScreen_DrawAddr, homura::MemFuncPtrWrapper<&StoreScreen::Draw>::Get(), &old_StoreScreen_Draw);
    homura::HookFunction(StoreScreen_DrawItemAddr, homura::MemFuncPtrWrapper<&StoreScreen::DrawItem>::Get(), &old_StoreScreen_DrawItem);


    homura::HookFunction(Plant_UpdateAddr, homura::MemFuncPtrWrapper<&Plant::Update>::Get(), &old_Plant_Update);
    homura::HookFunction(Plant_GetRefreshTimeAddr, &Plant::GetRefreshTime, &old_Plant_GetRefreshTime);
    homura::HookFunction(Plant_DoSpecialAddr, homura::MemFuncPtrWrapper<&Plant::DoSpecial>::Get(), nullptr);
    homura::HookFunction(Plant_DrawAddr, homura::MemFuncPtrWrapper<&Plant::Draw>::Get(), &old_Plant_Draw);
    homura::HookFunction(Plant_DrawSeedTypeAddr, &Plant::DrawSeedType, nullptr);
    homura::HookFunction(Plant_IsUpgradeAddr, &Plant::IsUpgrade, &old_Plant_IsUpgrade);
    homura::HookFunction(Plant_PlantInitializeAddr, homura::MemFuncPtrWrapper<&Plant::PlantInitialize>::Get(), &old_Plant_PlantInitialize);
    homura::HookFunction(Plant_SetSleepingAddr, homura::MemFuncPtrWrapper<&Plant::SetSleeping>::Get(), &old_Plant_SetSleeping);
    homura::HookFunction(Plant_UpdateReanimColorAddr, homura::MemFuncPtrWrapper<&Plant::UpdateReanimColor>::Get(), &old_Plant_UpdateReanimColor);
    homura::HookFunction(Plant_FindTargetZombieAddr, homura::MemFuncPtrWrapper<&Plant::FindTargetZombie>::Get(), nullptr);
    homura::HookFunction(Plant_FindTargetGridItemAddr, homura::MemFuncPtrWrapper<&Plant::FindTargetGridItem>::Get(), nullptr);
    homura::HookFunction(Plant_GetCostAddr, &Plant::GetCost, &old_Plant_GetCost);
    homura::HookFunction(Plant_DieAddr, homura::MemFuncPtrWrapper<&Plant::Die>::Get(), nullptr);
    homura::HookFunction(Plant_PlayBodyReanimAddr, homura::MemFuncPtrWrapper<&Plant::PlayBodyReanim>::Get(), &old_Plant_PlayBodyReanim);
    homura::HookFunction(Plant_UpdateProductionPlantAddr, homura::MemFuncPtrWrapper<&Plant::UpdateProductionPlant>::Get(), &old_Plant_UpdateProductionPlant);
    homura::HookFunction(Plant_FireAddr, homura::MemFuncPtrWrapper<&Plant::Fire>::Get(), nullptr);
    homura::HookFunction(Plant_DoRowAreaDamageAddr, homura::MemFuncPtrWrapper<&Plant::DoRowAreaDamage>::Get(), nullptr);
    //    homura::HookFunction(Plant_UpdateShootingAddr, homura::MemFuncPtrWrapper<&Plant::UpdateShooting>::Get(), nullptr);
    homura::HookFunction(Plant_UpdateShooterAddr, homura::MemFuncPtrWrapper<&Plant::UpdateShooter>::Get(), nullptr);
    homura::HookFunction(Plant_IceZombiesAddr, homura::MemFuncPtrWrapper<&Plant::IceZombies>::Get(), nullptr);
    homura::HookFunction(Plant_FindTargetAndFireAddr, homura::MemFuncPtrWrapper<&Plant::FindTargetAndFire>::Get(), &old_Plant_FindTargetAndFire);
    homura::HookFunction(Plant_UpdateChomperAddr, homura::MemFuncPtrWrapper<&Plant::UpdateChomper>::Get(), nullptr);
    homura::HookFunction(Plant_UpdateMagnetShroomAddr, homura::MemFuncPtrWrapper<&Plant::UpdateMagnetShroom>::Get(), nullptr);
    homura::HookFunction(Plant_UpdateSquashAddr, homura::MemFuncPtrWrapper<&Plant::UpdateSquash>::Get(), &old_Plant_UpdateSquash);

    // homura::HookFunction(Plant_CobCannonFireAddr, Plant_CobCannonFire, &old_Plant_CobCannonFire);
    // homura::HookFunction(Plant_UpdateReanimAddr, Plant_UpdateReanim, &old_Plant_UpdateReanim);


    homura::HookFunction(Projectile_ProjectileInitializeAddr, homura::MemFuncPtrWrapper<&Projectile::ProjectileInitialize>::Get(), &old_Projectile_ProjectileInitialize);
    homura::HookFunction(Projectile_ConvertToFireballAddr, homura::MemFuncPtrWrapper<&Projectile::ConvertToFireball>::Get(), nullptr);
    homura::HookFunction(Projectile_ConvertToPeaAddr, homura::MemFuncPtrWrapper<&Projectile::ConvertToPea>::Get(), &old_Projectile_ConvertToPea);
    homura::HookFunction(Projectile_UpdateAddr, homura::MemFuncPtrWrapper<&Projectile::Update>::Get(), &old_Projectile_Update);
    homura::HookFunction(Projectile_UpdateNormalMotionAddr, homura::MemFuncPtrWrapper<&Projectile::UpdateNormalMotion>::Get(), &old_Projectile_UpdateNormalMotion);
    homura::HookFunction(Projectile_DoImpactAddr, homura::MemFuncPtrWrapper<&Projectile::DoImpact>::Get(), &old_Projectile_DoImpact);
    homura::HookFunction(Projectile_DoSplashDamageAddr, homura::MemFuncPtrWrapper<&Projectile::DoSplashDamage>::Get(), &old_Projectile_DoSplashDamage);
    homura::HookFunction(Projectile_CheckForCollisionAddr, homura::MemFuncPtrWrapper<&Projectile::CheckForCollision>::Get(), nullptr);
    homura::HookFunction(Projectile_GetProjectileDefAddr, homura::MemFuncPtrWrapper<&Projectile::GetProjectileDef>::Get(), nullptr);
    homura::HookFunction(Projectile_DrawAddr, homura::MemFuncPtrWrapper<&Projectile::Draw>::Get(), &old_Projectile_Draw);
    homura::HookFunction(Projectile_DrawShadowAddr, homura::MemFuncPtrWrapper<&Projectile::DrawShadow>::Get(), &old_Projectile_DrawShadow);
    homura::HookFunction(Projectile_FindCollisionTargetGridItemAddr, homura::MemFuncPtrWrapper<&Projectile::FindCollisionTargetGridItem>::Get(), nullptr);


    homura::HookFunction(SeedPacket_UpdateAddr, homura::MemFuncPtrWrapper<&SeedPacket::Update>::Get(), nullptr);
    homura::HookFunction(SeedPacket_UpdateSelectedAddr, homura::MemFuncPtrWrapper<&SeedPacket::UpdateSelected>::Get(), &old_SeedPacket_UpdateSelected);
    homura::HookFunction(SeedPacket_DrawOverlayAddr, homura::MemFuncPtrWrapper<&SeedPacket::DrawOverlay>::Get(), &old_SeedPacket_DrawOverlay);
    homura::HookFunction(SeedPacket_DrawAddr, homura::MemFuncPtrWrapper<&SeedPacket::Draw>::Get(), &old_SeedPacket_Draw);
    homura::HookFunction(SeedPacket_FlashIfReadyAddr, homura::MemFuncPtrWrapper<&SeedPacket::FlashIfReady>::Get(), &old_SeedPacket_FlashIfReady);
    homura::HookFunction(SeedPacket_SetPacketTypeAddr, homura::MemFuncPtrWrapper<&SeedPacket::SetPacketType>::Get(), &old_SeedPacket_SetPacketType);
    //    homura::HookFunction(SeedPacket_MouseDownAddr, homura::MemFuncPtrWrapper<&SeedPacket::MouseDown>::Get(), &old_SeedPacket_MouseDown);
    homura::HookFunction(SeedPacket_WasPlantedAddr, homura::MemFuncPtrWrapper<&SeedPacket::WasPlanted>::Get(), &old_SeedPacket_WasPlanted);
    homura::HookFunction(SeedPacket_SlotMachineStartAddr, homura::MemFuncPtrWrapper<&SeedPacket::SlotMachineStart>::Get(), nullptr);


    homura::HookFunction(Zombie_UpdateAddr, homura::MemFuncPtrWrapper<&Zombie::Update>::Get(), nullptr);
    homura::HookFunction(Zombie_UpdateActionsAddr, homura::MemFuncPtrWrapper<&Zombie::UpdateActions>::Get(), &old_Zombie_UpdateActions);
    homura::HookFunction(Zombie_UpdatePlayingAddr, homura::MemFuncPtrWrapper<&Zombie::UpdatePlaying>::Get(), nullptr);
    homura::HookFunction(Zombie_UpdateYetiAddr, homura::MemFuncPtrWrapper<&Zombie::UpdateYeti>::Get(), nullptr);
    homura::HookFunction(Zombie_UpdateZombieFlyerAddr, homura::MemFuncPtrWrapper<&Zombie::UpdateZombieFlyer>::Get(), nullptr);
    homura::HookFunction(Zombie_UpdateZombieImpAddr, homura::MemFuncPtrWrapper<&Zombie::UpdateZombieImp>::Get(), nullptr);
    homura::HookFunction(Zombie_UpdateZombieJackInTheBoxAddr, homura::MemFuncPtrWrapper<&Zombie::UpdateZombieJackInTheBox>::Get(), nullptr);
    homura::HookFunction(Zombie_UpdateZombiePolevaulterAddr, homura::MemFuncPtrWrapper<&Zombie::UpdateZombiePolevaulter>::Get(), nullptr);
    homura::HookFunction(Zombie_UpdateZombieGargantuarAddr, homura::MemFuncPtrWrapper<&Zombie::UpdateZombieGargantuar>::Get(), nullptr);
    homura::HookFunction(Zombie_UpdateZombiePeaHeadAddr, homura::MemFuncPtrWrapper<&Zombie::UpdateZombiePeaHead>::Get(), nullptr);
    homura::HookFunction(Zombie_UpdateZombieGatlingHeadAddr, homura::MemFuncPtrWrapper<&Zombie::UpdateZombieGatlingHead>::Get(), nullptr);
    homura::HookFunction(Zombie_UpdateZombieJalapenoHeadAddr, homura::MemFuncPtrWrapper<&Zombie::UpdateZombieJalapenoHead>::Get(), nullptr);
    homura::HookFunction(Zombie_UpdateZombieSquashHeadAddr, homura::MemFuncPtrWrapper<&Zombie::UpdateZombieSquashHead>::Get(), nullptr);
    homura::HookFunction(Zombie_UpdateZombieDancerAddr, homura::MemFuncPtrWrapper<&Zombie::UpdateZombieDancer>::Get(), nullptr);
    homura::HookFunction(Zombie_UpdateZombieBobsledAddr, homura::MemFuncPtrWrapper<&Zombie::UpdateZombieBobsled>::Get(), nullptr);
    homura::HookFunction(Zombie_UpdateZombieRiseFromGraveAddr, homura::MemFuncPtrWrapper<&Zombie::UpdateZombieRiseFromGrave>::Get(), nullptr);
    homura::HookFunction(Zombie_GetDancerFrameAddr, homura::MemFuncPtrWrapper<&Zombie::GetDancerFrame>::Get(), nullptr);
    homura::HookFunction(Zombie_RiseFromGraveAddr, homura::MemFuncPtrWrapper<&Zombie::RiseFromGrave>::Get(), &old_Zombie_RiseFromGrave);
    homura::HookFunction(Zombie_EatPlantAddr, homura::MemFuncPtrWrapper<&Zombie::EatPlant>::Get(), nullptr);
    homura::HookFunction(Zombie_DetachShieldAddr, homura::MemFuncPtrWrapper<&Zombie::DetachShield>::Get(), &old_Zombie_DetachShield);
    homura::HookFunction(Zombie_CheckForBoardEdgeAddr, homura::MemFuncPtrWrapper<&Zombie::CheckForBoardEdge>::Get(), nullptr);
    homura::HookFunction(Zombie_DrawAddr, homura::MemFuncPtrWrapper<&Zombie::Draw>::Get(), &old_Zombie_Draw);
    homura::HookFunction(Zombie_DrawShadowAddr, homura::MemFuncPtrWrapper<&Zombie::DrawShadow>::Get(), nullptr);
    homura::HookFunction(Zombie_DrawBossPartAddr, homura::MemFuncPtrWrapper<&Zombie::DrawBossPart>::Get(), &old_Zombie_DrawBossPart);
    homura::HookFunction(ZombieTypeCanGoInPoolAddr, &Zombie::ZombieTypeCanGoInPool, nullptr);
    homura::HookFunction(Zombie_BossSpawnAttackAddr, homura::MemFuncPtrWrapper<&Zombie::BossSpawnAttack>::Get(), nullptr);
    homura::HookFunction(Zombie_DrawBungeeCordAddr, homura::MemFuncPtrWrapper<&Zombie::DrawBungeeCord>::Get(), nullptr);
    homura::HookFunction(Zombie_IsTangleKelpTargetAddr, homura::MemFuncPtrWrapper<&Zombie::IsTangleKelpTarget>::Get(), nullptr);
    homura::HookFunction(Zombie_IsTangleKelpTarget2Addr, homura::MemFuncPtrWrapper<&Zombie::IsTangleKelpTarget>::Get(), nullptr);
    homura::HookFunction(Zombie_DrawReanimAddr, homura::MemFuncPtrWrapper<&Zombie::DrawReanim>::Get(), nullptr);
    homura::HookFunction(Zombie_DropHeadAddr, homura::MemFuncPtrWrapper<&Zombie::DropHead>::Get(), &old_Zombie_DropHead);
    homura::HookFunction(Zombie_DropHelmAddr, homura::MemFuncPtrWrapper<&Zombie::DropHelm>::Get(), nullptr);
    homura::HookFunction(Zombie_DropArmAddr, homura::MemFuncPtrWrapper<&Zombie::DropArm>::Get(), &old_Zombie_DropArm);
    homura::HookFunction(Zombie_ZombieInitializeAddr, homura::MemFuncPtrWrapper<&Zombie::ZombieInitialize>::Get(), &old_Zombie_ZombieInitialize);
    homura::HookFunction(Zombie_DieNoLootAddr, homura::MemFuncPtrWrapper<&Zombie::DieNoLoot>::Get(), &old_Zombie_DieNoLoot);
    homura::HookFunction(Zombie_StopZombieSoundAddr, homura::MemFuncPtrWrapper<&Zombie::StopZombieSound>::Get(), nullptr);
    homura::HookFunction(GetZombieDefinitionAddr, &GetZombieDefinition, nullptr);
    homura::HookFunction(Zombie_FindPlantTargetAddr, homura::MemFuncPtrWrapper<&Zombie::FindPlantTarget>::Get(), nullptr);
    homura::HookFunction(Zombie_FindZombieTargetAddr, homura::MemFuncPtrWrapper<&Zombie::FindZombieTarget>::Get(), nullptr);
    homura::HookFunction(Zombie_TakeDamageAddr, homura::MemFuncPtrWrapper<&Zombie::TakeDamage>::Get(), nullptr);
    homura::HookFunction(Zombie_TakeHelmDamageAddr, homura::MemFuncPtrWrapper<&Zombie::TakeHelmDamage>::Get(), &old_Zombie_TakeHelmDamage);
    homura::HookFunction(Zombie_PlayZombieReanimAddr, homura::MemFuncPtrWrapper<&Zombie::PlayZombieReanim>::Get(), &old_Zombie_PlayZombieReanim);
    homura::HookFunction(Zombie_StartWalkAnimAddr, homura::MemFuncPtrWrapper<&Zombie::StartWalkAnim>::Get(), nullptr);
    homura::HookFunction(Zombie_ReanimShowPrefixAddr, homura::MemFuncPtrWrapper<&Zombie::ReanimShowPrefix>::Get(), &old_Zombie_ReanimShowPrefix);
    homura::HookFunction(Zombie_ReanimShowTrackAddr, homura::MemFuncPtrWrapper<&Zombie::ReanimShowTrack>::Get(), &old_Zombie_ReanimShowTrack);
    homura::HookFunction(Zombie_GetPosYBasedOnRowAddr, homura::MemFuncPtrWrapper<&Zombie::GetPosYBasedOnRow>::Get(), &old_Zombie_GetPosYBasedOnRow);
    homura::HookFunction(Zombie_SetRowAddr, homura::MemFuncPtrWrapper<&Zombie::SetRow>::Get(), &old_Zombie_SetRow);
    homura::HookFunction(Zombie_StartMindControlledAddr, homura::MemFuncPtrWrapper<&Zombie::StartMindControlled>::Get(), &old_Zombie_StartMindControlled);
    homura::HookFunction(Zombie_UpdateReanimAddr, homura::MemFuncPtrWrapper<&Zombie::UpdateReanim>::Get(), &old_Zombie_UpdateReanim);
    homura::HookFunction(Zombie_GetBobsledPositionAddr, homura::MemFuncPtrWrapper<&Zombie::GetBobsledPosition>::Get(), &old_Zombie_GetBobsledPosition);
    homura::HookFunction(Zombie_SquishAllInSquareAddr, homura::MemFuncPtrWrapper<&Zombie::SquishAllInSquare>::Get(), &old_Zombie_SquishAllInSquare);
    homura::HookFunction(Zombie_StopEatingAddr, homura::MemFuncPtrWrapper<&Zombie::StopEating>::Get(), nullptr);
    homura::HookFunction(Zombie_BungeeDropZombieAddr, homura::MemFuncPtrWrapper<&Zombie::BungeeDropZombie>::Get(), nullptr);
    homura::HookFunction(Zombie_PickRandomSpeedAddr, homura::MemFuncPtrWrapper<&Zombie::PickRandomSpeed>::Get(), nullptr);
    homura::HookFunction(Zombie_UpdateZombieWalkingAddr, homura::MemFuncPtrWrapper<&Zombie::UpdateZombieWalking>::Get(), nullptr);
    homura::HookFunction(Zombie_UpdateDamageStatesAddr, homura::MemFuncPtrWrapper<&Zombie::UpdateDamageStates>::Get(), nullptr);
    homura::HookFunction(Zombie_DropLootAddr, homura::MemFuncPtrWrapper<&Zombie::DropLoot>::Get(), &old_Zombie_DropLoot);
    homura::HookFunction(Zombie_ApplyBurnAddr, homura::MemFuncPtrWrapper<&Zombie::ApplyBurn>::Get(), &old_Zombie_ApplyBurn);
    homura::HookFunction(Zombie_CheckIfPreyCaughtAddr, homura::MemFuncPtrWrapper<&Zombie::CheckIfPreyCaught>::Get(), nullptr);
    homura::HookFunction(Zombie_CanTargetPlantAddr, homura::MemFuncPtrWrapper<&Zombie::CanTargetPlant>::Get(), nullptr);
    homura::HookFunction(Zombie_HitIceTrapAddr, homura::MemFuncPtrWrapper<&Zombie::HitIceTrap>::Get(), nullptr);
    homura::HookFunction(Zombie_UpdateYuckyFaceAddr, homura::MemFuncPtrWrapper<&Zombie::UpdateYuckyFace>::Get(), &old_Zombie_UpdateYuckyFace);
    homura::HookFunction(Zombie_UpdateZombiePoolAddr, homura::MemFuncPtrWrapper<&Zombie::UpdateZombiePool>::Get(), nullptr);
    homura::HookFunction(Zombie_SummonBackupDancersAddr, homura::MemFuncPtrWrapper<&Zombie::SummonBackupDancers>::Get(), nullptr);
    homura::HookFunction(Zombie_UpdateZombiePogoAddr, homura::MemFuncPtrWrapper<&Zombie::UpdateZombiePogo>::Get(), nullptr);
    homura::HookFunction(Zombie_UpdateZombieCatapultAddr, homura::MemFuncPtrWrapper<&Zombie::UpdateZombieCatapult>::Get(), nullptr);
    homura::HookFunction(Zombie_BungeeLandingAddr, homura::MemFuncPtrWrapper<&Zombie::BungeeLanding>::Get(), nullptr);
    homura::HookFunction(Zombie_UpdateLadderAddr, homura::MemFuncPtrWrapper<&Zombie::UpdateLadder>::Get(), nullptr);
    homura::HookFunction(Zombie_GetDrawPosAddr, homura::MemFuncPtrWrapper<&Zombie::GetDrawPos>::Get(), nullptr);


    homura::HookFunction(Sexy_Dialog_AddedToManagerWidgetManagerAddr, homura::MemFuncPtrWrapper<&Sexy::Dialog::AddedToManager>::Get(), &old_Sexy_Dialog_AddedToManager);
    homura::HookFunction(Sexy_Dialog_RemovedFromManagerAddr, homura::MemFuncPtrWrapper<&Sexy::Dialog::RemovedFromManager>::Get(), &old_Sexy_Dialog_RemovedFromManager);


    homura::HookFunction(SeedBank_DrawAddr, homura::MemFuncPtrWrapper<&SeedBank::Draw>::Get(), &old_SeedBank_Draw);
    homura::HookFunction(SeedBank_MouseHitTestAddr, homura::MemFuncPtrWrapper<&SeedBank::MouseHitTest>::Get(), nullptr);
    // homura::HookFunction(SeedBank_SeedBankAddr, homura::MemFuncPtrWrapper<&SeedBank::Create>::Get(), &old_SeedBank_SeedBank);
    // homura::HookFunction(SeedBank_UpdateWidthAddr, homura::MemFuncPtrWrapper<&SeedBank::UpdateWidth>::Get(), &old_SeedBank_UpdateWidth);
    homura::HookFunction(SeedBank_MoveAddr, homura::MemFuncPtrWrapper<&SeedBank::Move>::Get(), nullptr);
    homura::HookFunction(SeedBank_AddSeedAddr, homura::MemFuncPtrWrapper<&SeedBank::AddSeed>::Get(), &old_SeedBank_AddSeed);


    homura::HookFunction(AwardScreen_MouseDownAddr, homura::MemFuncPtrWrapper<&AwardScreen::MouseDown>::Get(), &old_AwardScreen_MouseDown);
    homura::HookFunction(AwardScreen_MouseUpAddr, homura::MemFuncPtrWrapper<&AwardScreen::MouseUp>::Get(), &old_AwardScreen_MouseUp);


    homura::HookFunction(VSSetupMenu_VSSetupMenuAddr, homura::MemFuncPtrWrapper<&VSSetupMenu::_constructor>::Get(), &old_VSSetupMenu_Constructor);
    homura::HookFunction(VSSetupMenu_Delete2Addr, homura::MemFuncPtrWrapper<&VSSetupMenu::_destructor>::Get(), &old_VSSetupMenu_Destructor);
    homura::HookFunction(VSSetupMenu_DrawAddr, homura::MemFuncPtrWrapper<&VSSetupMenu::Draw>::Get(), &old_VSSetupMenu_Draw);
    homura::HookFunction(VSSetupMenu_DrawOverlayAddr, homura::MemFuncPtrWrapper<&VSSetupMenu::DrawOverlay>::Get(), &old_VSSetupMenu_DrawOverlay);
    homura::HookFunction(VSSetupMenu_AddedToManagerAddr, homura::MemFuncPtrWrapper<&VSSetupMenu::AddedToManager>::Get(), &old_VSSetupMenu_AddedToManager);
    homura::HookFunction(VSSetupMenu_CloseVSSetupAddr, homura::MemFuncPtrWrapper<&VSSetupMenu::CloseVSSetup>::Get(), &old_VSSetupMenu_CloseVSSetup);
    homura::HookFunction(VSSetupMenu_UpdateAddr, homura::MemFuncPtrWrapper<&VSSetupMenu::Update>::Get(), &old_VSSetupMenu_Update);
    homura::HookFunction(VSSetupMenu_KeyDownAddr, homura::MemFuncPtrWrapper<&VSSetupMenu::KeyDown>::Get(), &old_VSSetupMenu_KeyDown);
    homura::HookFunction(VSSetupMenu_OnStateEnterAddr, homura::MemFuncPtrWrapper<&VSSetupMenu::OnStateEnter>::Get(), &old_VSSetupMenu_OnStateEnter);
    // homura::HookFunction(VSSetupMenu_ButtonPressAddr, homura::MemFuncPtrWrapper<&VSSetupMenu::ButtonPress>::Get(), &old_VSSetupMenu_ButtonPress);
    homura::HookFunction(VSSetupMenu_ButtonDepressAddr, homura::MemFuncPtrWrapper<&VSSetupMenu::ButtonDepress>::Get(), &old_VSSetupMenu_ButtonDepress);
    homura::HookFunction(VSSetupMenu_PickRandomZombiesAddr, homura::MemFuncPtrWrapper<&VSSetupMenu::PickRandomZombies>::Get(), nullptr);
    homura::HookFunction(VSSetupMenu_PickRandomPlantsAddr, homura::MemFuncPtrWrapper<&VSSetupMenu::PickRandomPlants>::Get(), nullptr);


    homura::HookFunction(VSResultsMenu_VSResultsMenuAddr, homura::MemFuncPtrWrapper<&VSResultsMenu::_constructor>::Get(), &old_VSResultsMenu_Constructor);
    homura::HookFunction(VSResultsMenu_UpdateAddr, homura::MemFuncPtrWrapper<&VSResultsMenu::Update>::Get(), &old_VSResultsMenu_Update);
    homura::HookFunction(VSResultsMenu_OnExitAddr, homura::MemFuncPtrWrapper<&VSResultsMenu::OnExit>::Get(), nullptr);
    homura::HookFunction(VSResultsMenu_DrawAddr, homura::MemFuncPtrWrapper<&VSResultsMenu::Draw>::Get(), &old_VSResultsMenu_Draw);
    homura::HookFunction(VSResultsMenu_DrawInfoBoxAddr, homura::MemFuncPtrWrapper<&VSResultsMenu::DrawInfoBox>::Get(), &old_VSResultsMenu_DrawInfoBox);
    homura::HookFunction(VSResultsMenu_ButtonDepressAddr, homura::MemFuncPtrWrapper<&VSResultsMenu::ButtonDepress>::Get(), nullptr);


    homura::HookFunction(ImitaterDialog_ImitaterDialogAddr, &ImitaterDialog_ImitaterDialog, &old_ImitaterDialog_ImitaterDialog);
    homura::HookFunction(ImitaterDialog_MouseDownAddr, homura::MemFuncPtrWrapper<&ImitaterDialog::MouseDown>::Get(), &old_ImitaterDialog_MouseDown);
    // homura::HookFunction(ImitaterDialog_OnKeyDownAddr, ImitaterDialog_OnKeyDown, &old_ImitaterDialog_OnKeyDown);
    homura::HookFunction(ImitaterDialog_KeyDownAddr, &ImitaterDialog_KeyDown, &old_ImitaterDialog_KeyDown);
    homura::HookFunction(ImitaterDialog_ShowToolTipAddr, &ImitaterDialog_ShowToolTip, &old_ImitaterDialog_ShowToolTip);


    homura::HookFunction(MailScreen_MailScreenAddr, homura::MemFuncPtrWrapper<&MailScreen::_constructor>::Get(), &old_MailScreen_MailScreen);
    homura::HookFunction(MailScreen_AddedToManagerAddr, homura::MemFuncPtrWrapper<&MailScreen::AddedToManager>::Get(), &old_MailScreen_AddedToManager);
    homura::HookFunction(MailScreen_RemovedFromManagerAddr, homura::MemFuncPtrWrapper<&MailScreen::RemovedFromManager>::Get(), &old_MailScreen_RemovedFromManager);
    homura::HookFunction(MailScreen_Delete2Addr, homura::MemFuncPtrWrapper<&MailScreen::_destructor2>::Get(), &old_MailScreen_Delete2);


    homura::HookFunction(ZenGardenControls_UpdateAddr, homura::MemFuncPtrWrapper<&ZenGardenControls::Update>::Get(), &old_ZenGardenControls_Update);
    homura::HookFunction(ZenGarden_DrawBackdropAddr, homura::MemFuncPtrWrapper<&ZenGarden::DrawBackdrop>::Get(), &old_ZenGarden_DrawBackdrop);
    // homura::HookFunction(ZenGarden_MouseDownWithFeedingToolAddr, ZenGarden_MouseDownWithFeedingTool, &old_ZenGarden_MouseDownWithFeedingTool);
    // homura::HookFunction(ZenGarden_DrawPottedPlantAddr, ZenGarden_DrawPottedPlant, nullptr);

    // homura::HookFunction( Sexy_GamepadApp_CheckGamepadAddr, Sexy_GamepadApp_CheckGamepad,nullptr);
    // homura::HookFunction( Sexy_GamepadApp_HasGamepadAddr, Sexy_GamepadApp_HasGamepad,nullptr);


    homura::HookFunction(CutScene_ShowShovelAddr, homura::MemFuncPtrWrapper<&CutScene::ShowShovel>::Get(), &old_CutScene_ShowShovel);
    homura::HookFunction(CutScene_UpdateAddr, homura::MemFuncPtrWrapper<&CutScene::Update>::Get(), &old_CutScene_Update);
    homura::HookFunction(CutScene_PlaceLawnItemsAddr, homura::MemFuncPtrWrapper<&CutScene::PlaceLawnItems>::Get(), &old_CutScene_PlaceLawnItems);
    homura::HookFunction(CutScene_AddFlowerPotsAddr, homura::MemFuncPtrWrapper<&CutScene::AddFlowerPots>::Get(), &old_CutScene_AddFlowerPots);
    homura::HookFunction(CutScene_LoadUpsellChallengeScreenAddr, homura::MemFuncPtrWrapper<&CutScene::LoadUpsellChallengeScreen>::Get(), nullptr);


    homura::HookFunction(NewOptionsDialog_ButtonDepressAddr, homura::MemFuncPtrWrapper<&NewOptionsDialog::ButtonDepress>::Get(), &old_NewOptionsDialog_ButtonDepress);

    homura::HookFunction(BaseGamepadControls_GetGamepadVelocityAddr, homura::MemFuncPtrWrapper<&BaseGamepadControls::GetGamepadVelocity>::Get(), nullptr);

    homura::HookFunction(LookupFoleyAddr, &LookupFoley, nullptr);

    // homura::HookFunction(TodDrawStringWrappedHelperAddr, TodDrawStringWrappedHelper, &old_TodDrawStringWrappedHelper);
    homura::HookFunction(MessageWidget_ClearLabelAddr, homura::MemFuncPtrWrapper<&CustomMessageWidget::ClearLabel>::Get(), &old_MessageWidget_ClearLabel);
    homura::HookFunction(MessageWidget_SetLabelAddr, homura::MemFuncPtrWrapper<&CustomMessageWidget::SetLabel>::Get(), &old_MessageWidget_SetLabel);
    homura::HookFunction(MessageWidget_UpdateAddr, homura::MemFuncPtrWrapper<&CustomMessageWidget::Update>::Get(), &old_MessageWidget_Update);
    homura::HookFunction(MessageWidget_DrawAddr, homura::MemFuncPtrWrapper<&CustomMessageWidget::Draw>::Get(), &old_MessageWidget_Draw);

    homura::HookFunction(Sexy_ExtractLoadingSoundsResourcesAddr, &Sexy::ExtractLoadingSoundsResources, &old_Sexy_ExtractLoadingSoundsResources);
    // homura::HookFunction(Sexy_ScrollbarWidget_MouseDownAddr, Sexy_ScrollbarWidget_MouseDown, nullptr);

    homura::HookFunction(CustomScrollbarWidget_RemoveScrollButtonsAddr, homura::MemFuncPtrWrapper<&Sexy::CustomScrollbarWidget::RemoveScrollButtons>::Get(), nullptr);

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

    homura::HookFunction(
        WaitForSecondPlayerDialog_WaitForSecondPlayerDialogAddr, homura::MemFuncPtrWrapper<&WaitForSecondPlayerDialog::_constructor>::Get(), &old_WaitForSecondPlayerDialog_WaitForSecondPlayerDialog);
    homura::HookFunction(WaitForSecondPlayerDialog_DeleteAddr, homura::MemFuncPtrWrapper<&WaitForSecondPlayerDialog::_destructor>::Get(), &old_WaitForSecondPlayerDialog_Delete);


    homura::HookFunction(Sexy_WidgetManager_MouseDownAddr, homura::MemFuncPtrWrapper<&Sexy::WidgetManager::MouseDown>::Get(), &old_Sexy_WidgetManager_MouseDown);
    homura::HookFunction(Sexy_WidgetManager_MouseDragAddr, homura::MemFuncPtrWrapper<&Sexy::WidgetManager::MouseDrag>::Get(), &old_Sexy_WidgetManager_MouseDrag);
    homura::HookFunction(Sexy_WidgetManager_MouseUpAddr, homura::MemFuncPtrWrapper<&Sexy::WidgetManager::MouseUp>::Get(), &old_Sexy_WidgetManager_MouseUp);
    // homura::HookFunction(Sexy_WidgetManager_AxisMovedAddr, Sexy_WidgetManager_AxisMoved, nullptr);


    homura::HookFunction(LawnMower_UpdateAddr, homura::MemFuncPtrWrapper<&LawnMower::Update>::Get(), &old_LawnMower_Update);
    homura::HookFunction(LawnMower_StartMowerAddr, homura::MemFuncPtrWrapper<&LawnMower::StartMower>::Get(), &old_LawnMower_StartMower);
    homura::HookFunction(ConfirmBackToMainDialog_ButtonDepressAddr, &ConfirmBackToMainDialog_ButtonDepress, &old_ConfirmBackToMainDialog_ButtonDepress);
    homura::HookFunction(ConfirmBackToMainDialog_AddedToManagerAddr, &ConfirmBackToMainDialog_AddedToManager, &old_ConfirmBackToMainDialog_AddedToManager);
    // homura::HookFunction(FilterEffectDisposeForAppAddr, FilterEffectDisposeForApp, nullptr);
    // homura::HookFunction(FilterEffectGetImageAddr, FilterEffectGetImage, nullptr);
    homura::HookFunction(Reanimation_DrawTrackAddr, homura::MemFuncPtrWrapper<&Reanimation::DrawTrack>::Get(), &old_Reanimation_DrawTrack);


    homura::HookFunction(ReanimatorCache_ReanimatorCacheInitializeAddr, homura::MemFuncPtrWrapper<&ReanimatorCache::ReanimatorCacheInitialize>::Get(), nullptr);
    homura::HookFunction(ReanimatorCache_ReanimatorCacheDisposeAddr, homura::MemFuncPtrWrapper<&ReanimatorCache::ReanimatorCacheDispose>::Get(), nullptr);
    homura::HookFunction(ReanimatorCache_DrawCachedPlantAddr, homura::MemFuncPtrWrapper<&ReanimatorCache::DrawCachedPlant>::Get(), &old_ReanimatorCache_DrawCachedPlant);
    homura::HookFunction(
        ReanimatorCache_UpdateReanimationForVariationAddr, homura::MemFuncPtrWrapper<&ReanimatorCache::UpdateReanimationForVariation>::Get(), &old_ReanimatorCache_UpdateReanimationForVariation);
    homura::HookFunction(ReanimatorCache_LoadCachedImagesAddr, homura::MemFuncPtrWrapper<&ReanimatorCache::LoadCachedImages>::Get(), &old_ReanimatorCache_LoadCachedImages);
    homura::HookFunction(ReanimatorCache_MakeCachedZombieFrameAddr, homura::MemFuncPtrWrapper<&ReanimatorCache::MakeCachedZombieFrame>::Get(), nullptr);
    homura::HookFunction(ReanimatorCache_DrawCachedZombieAddr, homura::MemFuncPtrWrapper<&ReanimatorCache::DrawCachedZombie>::Get(), nullptr);


    homura::HookFunction(HelpBarWidget_HelpBarWidgetAddr, homura::MemFuncPtrWrapper<&HelpBarWidget::_constructor>::Get(), &old_HelpBarWidget_HelpBarWidget);


    homura::HookFunction(DrawSeedTypeAddr, &DrawSeedType, nullptr);
    homura::HookFunction(DrawSeedPacketAddr, &DrawSeedPacket, nullptr);

    homura::HookFunction(Music_PlayMusicAddr, homura::MemFuncPtrWrapper<&Music::PlayMusic>::Get(), nullptr);
    homura::HookFunction(Music_MusicUpdateAddr, homura::MemFuncPtrWrapper<&Music::MusicUpdate>::Get(), nullptr);
    homura::HookFunction(Music_UpdateMusicBurstAddr, homura::MemFuncPtrWrapper<&Music::UpdateMusicBurst>::Get(), &old_Music_UpdateMusicBurst);
    homura::HookFunction(Music2_Music2Addr, homura::MemFuncPtrWrapper<&Music2::_constructor>::Get(), &old_Music2_Music2);

    homura::HookFunction(LawnPlayerInfo_AddCoinsAddr, homura::MemFuncPtrWrapper<&LawnPlayerInfo::AddCoins>::Get(), nullptr);
    homura::HookFunction(MaskHelpWidget_UpdateAddr, &MaskHelpWidget_Update, nullptr);
    homura::HookFunction(MaskHelpWidget_DrawAddr, &MaskHelpWidget_Draw, nullptr);
    // homura::HookFunction(DaveHelp_DaveHelpAddr,  DaveHelp_DaveHelp,  &old_DaveHelp_DaveHelp);
    homura::HookFunction(DaveHelp_UpdateAddr, &DaveHelp_Update, nullptr);
    homura::HookFunction(DaveHelp_DrawAddr, &DaveHelp_Draw, nullptr);
    homura::HookFunction(DaveHelp_Delete2Addr, &DaveHelp_Delete2, &old_DaveHelp_Delete2);
    homura::HookFunction(DaveHelp_DealClickAddr, &DaveHelp_DealClick, nullptr);
    homura::HookFunction(TrashBin_TrashBinAddr, homura::MemFuncPtrWrapper<&TrashBin::_constructor>::Get(), &old_TrashBin_TrashBin);
    homura::HookFunction(Sexy_SexyAppBase_Is3DAcceleratedAddr, homura::MemFuncPtrWrapper<&LawnApp::Is3DAccelerated>::Get(), nullptr);
    homura::HookFunction(Sexy_SexyAppBase_SexyAppBaseAddr, homura::MemFuncPtrWrapper<&Sexy::SexyAppBase::_constructor>::Get(), &old_Sexy_SexyAppBase_SexyAppBase);
    homura::HookFunction(Sexy_SexyAppBase_EraseFileAddr, homura::MemFuncPtrWrapper<&Sexy::SexyAppBase::EraseFile>::Get(), nullptr);

    homura::HookFunction(SettingsDialog_SettingsDialogAddr, homura::MemFuncPtrWrapper<&SettingsDialog::_constructor>::Get(), &old_SettingsDialog__constructor);
    homura::HookFunction(SettingsDialog__destructorAddr, homura::MemFuncPtrWrapper<&SettingsDialog::_destructor>::Get(), &old_SettingsDialog__destructor);
    homura::HookFunction(SettingsDialog_AddedToManagerAddr, homura::MemFuncPtrWrapper<&SettingsDialog::AddedToManager>::Get(), &old_SettingsDialog_AddedToManager);
    homura::HookFunction(SettingsDialog_RemovedFromManagerAddr, homura::MemFuncPtrWrapper<&SettingsDialog::RemovedFromManager>::Get(), &old_SettingsDialog_RemovedFromManager);
    homura::HookFunction(SettingsDialog_DrawAddr, homura::MemFuncPtrWrapper<&SettingsDialog::Draw>::Get(), &old_SettingsDialog_Draw);
    homura::HookFunction(ReanimatorLoadDefinitionsAddr, &ReanimatorLoadDefinitions, &old_ReanimatorLoadDefinitions);
    homura::HookFunction(DefinitionGetCompiledFilePathFromXMLFilePathAddr, &DefinitionGetCompiledFilePathFromXMLFilePath, &old_DefinitionGetCompiledFilePathFromXMLFilePath);
    homura::HookFunction(TestMenuWidget_DeleteAddr, &TestMenuWidget_Delete, &old_TestMenuWidget_Delete);
    homura::HookFunction(TestMenuWidget_Delete2Addr, &TestMenuWidget_Delete2, &old_TestMenuWidget_Delete2);
    homura::HookFunction(SaveGameContext_SyncReanimationDefAddr, homura::MemFuncPtrWrapper<&SaveGameContext::SyncReanimationDef>::Get(), nullptr);
    homura::HookFunction(PoolEffect_PoolEffectDrawAddr, homura::MemFuncPtrWrapper<&PoolEffect::PoolEffectDraw>::Get(), nullptr);
    homura::HookFunction(Sexy_MemoryImage_ClearRectAddr, homura::MemFuncPtrWrapper<&Sexy::MemoryImage::ClearRect>::Get(), nullptr);


    homura::HookFunction(TitleScreen_TitleScreenAddr, homura::MemFuncPtrWrapper<&TitleScreen::_constructor>::Get(), &old_TitleScreen_TitleScreen);
    homura::HookFunction(TitleScreen_DrawAddr, &TitleScreen_Draw, &old_TitleScreen_Draw);
    homura::HookFunction(TitleScreen_UpdateAddr, &TitleScreen_Update, &old_TitleScreen_Update);
    homura::HookFunction(TitleScreen_SwitchStateAddr, &TitleScreen_SwitchState, nullptr);
}

void InitVTableHookFunction() {
    homura::HookVirtualFunc(vTableForCursorObjectAddr, 4, homura::MemFuncPtrWrapper<&CursorObject::BeginDraw>::Get(), &old_CursorObject_BeginDraw);
    homura::HookVirtualFunc(vTableForCursorObjectAddr, 5, homura::MemFuncPtrWrapper<&CursorObject::EndDraw>::Get(), &old_CursorObject_EndDraw);

    homura::HookVirtualFunc(vTableForBoardAddr, 77, homura::MemFuncPtrWrapper<&Board::MouseMove>::Get(), &old_Board_MouseMove);
    homura::HookVirtualFunc(vTableForBoardAddr, 78, homura::MemFuncPtrWrapper<&Board::MouseDown>::Get(), &old_Board_MouseDown);
    homura::HookVirtualFunc(vTableForBoardAddr, 81, homura::MemFuncPtrWrapper<&Board::MouseUp>::Get(), &old_Board_MouseUp);
    homura::HookVirtualFunc(vTableForBoardAddr, 83, homura::MemFuncPtrWrapper<&Board::MouseDrag>::Get(), &old_Board_MouseDrag);
    homura::HookVirtualFunc(vTableForBoardAddr, 133, homura::MemFuncPtrWrapper<&Board::ButtonDepress>::Get(), &old_Board_ButtonDepress);

    homura::HookVirtualFunc(vTableForStoreScreenAddr, 78, homura::MemFuncPtrWrapper<&StoreScreen::MouseDown>::Get(), &old_StoreScreen_MouseDown);
    homura::HookVirtualFunc(vTableForStoreScreenAddr, 81, homura::MemFuncPtrWrapper<&StoreScreen::MouseUp>::Get(), &old_StoreScreen_MouseUp);
    // VTableHookFunction(vTableForStoreScreenAddr, 83, (void *) StoreScreen_MouseDrag,(void **) &old_StoreScreen_MouseDrag);

    homura::HookVirtualFunc(vTableForMailScreenAddr, 78, homura::MemFuncPtrWrapper<&MailScreen::MouseDown>::Get(), &old_MailScreen_MouseDown);
    homura::HookVirtualFunc(vTableForMailScreenAddr, 81, homura::MemFuncPtrWrapper<&MailScreen::MouseUp>::Get(), &old_MailScreen_MouseUp);
    homura::HookVirtualFunc(vTableForMailScreenAddr, 83, homura::MemFuncPtrWrapper<&MailScreen::MouseDrag>::Get(), &old_MailScreen_MouseDrag);
    homura::HookVirtualFunc(vTableForMailScreenAddr, 140, homura::MemFuncPtrWrapper<&MailScreen::ButtonPress>::Get(), &old_MailScreen_ButtonPress);
    homura::HookVirtualFunc(vTableForMailScreenAddr, 141, homura::MemFuncPtrWrapper<&MailScreen::ButtonDepress>::Get(), &old_MailScreen_ButtonDepress);

    homura::HookVirtualFunc(vTableForChallengeScreenAddr, 78, homura::MemFuncPtrWrapper<&ChallengeScreen::MouseDown>::Get(), &old_ChallengeScreen_MouseDown);
    homura::HookVirtualFunc(vTableForChallengeScreenAddr, 81, homura::MemFuncPtrWrapper<&ChallengeScreen::MouseUp>::Get(), &old_ChallengeScreen_MouseUp);
    homura::HookVirtualFunc(vTableForChallengeScreenAddr, 83, homura::MemFuncPtrWrapper<&ChallengeScreen::MouseDrag>::Get(), &old_ChallengeScreen_MouseDrag);
    homura::HookVirtualFunc(vTableForChallengeScreenAddr, 130, homura::MemFuncPtrWrapper<&ChallengeScreen::ButtonPress>::Get(), nullptr);

    // homura::HookVirtualFunc(vTableForVSResultsMenuAddr, 78, &VSResultsMenu::MouseDown,nullptr);
    // homura::HookVirtualFunc(vTableForVSResultsMenuAddr, 81, &VSResultsMenu::MouseUp,nullptr);
    // homura::HookVirtualFunc(vTableForVSResultsMenuAddr, 83, &VSResultsMenu::MouseDrag,nullptr);


    homura::HookVirtualFunc(vTableForVSSetupMenuAddr, 78, homura::MemFuncPtrWrapper<&VSSetupMenu::MouseDown>::Get(), nullptr);
    homura::HookVirtualFunc(vTableForVSSetupMenuAddr, 81, homura::MemFuncPtrWrapper<&VSSetupMenu::MouseUp>::Get(), nullptr);
    homura::HookVirtualFunc(vTableForVSSetupMenuAddr, 83, homura::MemFuncPtrWrapper<&VSSetupMenu::MouseDrag>::Get(), nullptr);

    homura::HookVirtualFunc(vTableForSeedChooserScreenAddr, 77, homura::MemFuncPtrWrapper<&SeedChooserScreen::MouseMove>::Get(), &old_SeedChooserScreen_MouseMove);
    homura::HookVirtualFunc(vTableForSeedChooserScreenAddr, 78, homura::MemFuncPtrWrapper<&SeedChooserScreen::MouseDown>::Get(), &old_SeedChooserScreen_MouseDown);
    homura::HookVirtualFunc(vTableForSeedChooserScreenAddr, 81, homura::MemFuncPtrWrapper<&SeedChooserScreen::MouseUp>::Get(), &old_SeedChooserScreen_MouseUp);
    homura::HookVirtualFunc(vTableForSeedChooserScreenAddr, 83, homura::MemFuncPtrWrapper<&SeedChooserScreen::MouseDrag>::Get(), &old_SeedChooserScreen_MouseDrag);
    homura::HookVirtualFunc(vTableForSeedChooserScreenAddr, 135, homura::MemFuncPtrWrapper<&SeedChooserScreen::ButtonPress>::Get(), nullptr);


    homura::HookVirtualFunc(vTableForHelpTextScreenAddr, 38, &HelpTextScreen_Draw, &old_HelpTextScreen_Draw);
    homura::HookVirtualFunc(vTableForHelpTextScreenAddr, 78, homura::MemFuncPtrWrapper<&HelpTextScreen::MouseDown>::Get(), &old_HelpTextScreen_MouseDown);
    // VTableHookFunction(vTableForHelpTextScreenAddr, 81, (void *) HelpTextScreen_MouseUp,(void **) &old_HelpTextScreen_MouseUp);
    // VTableHookFunction(vTableForHelpTextScreenAddr, 83, (void *) HelpTextScreen_MouseDrag,(void **) &old_HelpTextScreen_MouseDrag);
    homura::HookVirtualFunc(vTableForHelpTextScreenAddr, 136, &HelpTextScreen_ButtonDepress, &old_HelpTextScreen_ButtonDepress);

    homura::HookVirtualFunc(vTableForAlmanacDialogAddr, 83, homura::MemFuncPtrWrapper<&AlmanacDialog::MouseDrag>::Get(), &old_AlmanacDialog_MouseDrag);

    homura::HookVirtualFunc(vTableForHouseChooserDialogAddr, 73, homura::MemFuncPtrWrapper<&HouseChooserDialog::KeyDown>::Get(), &old_HouseChooserDialog_KeyDown);
    homura::HookVirtualFunc(vTableForHouseChooserDialogAddr, 78, homura::MemFuncPtrWrapper<&HouseChooserDialog::MouseDown>::Get(), &old_HouseChooserDialog_MouseDown);

    homura::HookVirtualFunc(vTableForSeedPacketAddr, 4, homura::MemFuncPtrWrapper<&SeedPacket::BeginDraw>::Get(), &old_SeedPacket_BeginDraw);
    homura::HookVirtualFunc(vTableForSeedPacketAddr, 5, homura::MemFuncPtrWrapper<&SeedPacket::EndDraw>::Get(), &old_SeedPacket_EndDraw);

    homura::HookVirtualFunc(vTableForSeedBankAddr, 4, homura::MemFuncPtrWrapper<&SeedBank::BeginDraw>::Get(), &old_SeedBank_BeginDraw);
    homura::HookVirtualFunc(vTableForSeedBankAddr, 5, homura::MemFuncPtrWrapper<&SeedBank::EndDraw>::Get(), &old_SeedBank_EndDraw);

    homura::HookVirtualFunc(vTableForGraphicsAddr, 4, homura::MemFuncPtrWrapper<&Sexy::Graphics::PushTransform>::Get(), &old_Sexy_Graphics_PushTransform);
    homura::HookVirtualFunc(vTableForGraphicsAddr, 5, homura::MemFuncPtrWrapper<&Sexy::Graphics::PopTransform>::Get(), &old_Sexy_Graphics_PopTransform);


    homura::HookVirtualFunc(vTableForImageAddr, 37, homura::MemFuncPtrWrapper<&Sexy::Image::PushTransform>::Get(), &old_Sexy_Image_PushTransform);
    homura::HookVirtualFunc(vTableForImageAddr, 38, homura::MemFuncPtrWrapper<&Sexy::Image::PopTransform>::Get(), &old_Sexy_Image_PopTransform);

    homura::HookVirtualFunc(vTableForGLImageAddr, 37, homura::MemFuncPtrWrapper<&Sexy::GLImage::PushTransform>::Get(), &old_Sexy_GLImage_PushTransform);
    homura::HookVirtualFunc(vTableForGLImageAddr, 38, homura::MemFuncPtrWrapper<&Sexy::GLImage::PopTransform>::Get(), &old_Sexy_GLImage_PopTransform);

    homura::HookVirtualFunc(vTableForMemoryImageAddr, 37, homura::MemFuncPtrWrapper<&Sexy::MemoryImage::PushTransform>::Get(), &old_Sexy_MemoryImage_PushTransform);
    homura::HookVirtualFunc(vTableForMemoryImageAddr, 38, homura::MemFuncPtrWrapper<&Sexy::MemoryImage::PopTransform>::Get(), &old_Sexy_MemoryImage_PopTransform);


    homura::HookVirtualFunc(vTableForMusic2Addr, 7, homura::MemFuncPtrWrapper<&Music2::StopAllMusic>::Get(), &old_Music2_StopAllMusic);
    homura::HookVirtualFunc(vTableForMusic2Addr, 8, homura::MemFuncPtrWrapper<&Music2::StartGameMusic>::Get(), &old_Music2_StartGameMusic);
    homura::HookVirtualFunc(vTableForMusic2Addr, 11, homura::MemFuncPtrWrapper<&Music2::GameMusicPause>::Get(), &old_Music2_GameMusicPause);
    // VTableHookFunction(vTableForMusic2Addr, 12, (void *) Music2_UpdateMusicBurst,(void **) &old_Music2_UpdateMusicBurst);
    // VTableHookFunction(vTableForMusic2Addr, 13, (void *) Music2_StartBurst,(void **) &old_Music2_StartBurst);
    homura::HookVirtualFunc(vTableForMusic2Addr, 17, homura::MemFuncPtrWrapper<&Music2::FadeOut>::Get(), &old_Music2_FadeOut);


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


    homura::HookVirtualFunc(vTableForTrashBinAddr, 38, homura::MemFuncPtrWrapper<&TrashBin::Draw>::Get(), nullptr);

    homura::HookVirtualFunc(vTableForConfirmBackToMainDialogAddr, 83, &ConfirmBackToMainDialog_MouseDrag, &old_ConfirmBackToMainDialog_MouseDrag);

    homura::HookVirtualFunc(vTableForSettingsDialogAddr, 153, homura::MemFuncPtrWrapper<&SettingsDialog::CheckboxChecked>::Get(), nullptr);

    homura::HookVirtualFunc(vTableForCreditScreenAddr, 133, homura::MemFuncPtrWrapper<&CreditScreen::ButtonDepress>::Get(), nullptr);

    homura::HookVirtualFunc(vTableForMainMenuAddr, 139, homura::MemFuncPtrWrapper<&MainMenu::ButtonPress>::Get(), nullptr);


    homura::HookVirtualFunc(vTableForWaitForSecondPlayerDialogAddr, 142, &WaitForSecondPlayerDialog::ButtonDepress_Thunk, &old_WaitForSecondPlayerDialog_ButtonDepress);
    homura::HookVirtualFunc(vTableForWaitForSecondPlayerDialogAddr, 33, homura::MemFuncPtrWrapper<&WaitForSecondPlayerDialog::Update>::Get(), nullptr);
    homura::HookVirtualFunc(vTableForWaitForSecondPlayerDialogAddr, 38, homura::MemFuncPtrWrapper<&WaitForSecondPlayerDialog::Draw>::Get(), &old_WaitForSecondPlayerDialog_Draw);
    homura::HookVirtualFunc(vTableForWaitForSecondPlayerDialogAddr, 52, homura::MemFuncPtrWrapper<&WaitForSecondPlayerDialog::Resize>::Get(), nullptr);
    homura::HookVirtualFunc(vTableForWaitForSecondPlayerDialogAddr, 78, homura::MemFuncPtrWrapper<&WaitForSecondPlayerDialog::MouseDown>::Get(), nullptr);
}

void InitOpenSL() {
    homura::HookFunction(Native_AudioOutput_setupAddr, homura::MemFuncPtrWrapper<&Native::AudioOutput::setup>::Get(), &old_Native_AudioOutput_setup);
    homura::HookFunction(Native_AudioOutput_shutdownAddr, homura::MemFuncPtrWrapper<&Native::AudioOutput::shutdown>::Get(), &old_Native_AudioOutput_shutdown);
    // homura::HookFunction(Native_AudioOutput_writeAddr, Native_AudioOutput_write, &old_Native_AudioOutput_write);
    homura::HookFunction(j_AGAudioWriteAddr, &AudioWrite, nullptr);
}

void InitIntroVideo() {

    // homura::HookFunction(j_AGVideoOpenAddr, AGVideoOpen, nullptr);
    // homura::HookFunction(j_AGVideoShowAddr, AGVideoShow, nullptr);
    // homura::HookFunction(j_AGVideoEnableAddr, AGVideoEnable, nullptr);
    // homura::HookFunction(j_AGVideoIsPlayingAddr, AGVideoIsPlaying, nullptr);
    // homura::HookFunction(j_AGVideoPlayAddr, AGVideoPlay, nullptr);
    // homura::HookFunction(j_AGVideoPauseAddr, AGVideoPause, nullptr);
    // homura::HookFunction(j_AGVideoResumeAddr, AGVideoResume, nullptr);

    // constexpr auto &libGameMain = "libGameMain.so";
    // homura::HookPltFunction(libGameMain, AGVideoOpenOffset, AGVideoOpen, nullptr);
    // homura::HookPltFunction(libGameMain, AGVideoShowOffset, AGVideoShow, nullptr);
    // homura::HookPltFunction(libGameMain, AGVideoEnableOffset, AGVideoEnable, nullptr);
    // homura::HookPltFunction(libGameMain, AGVideoIsPlayingOffset, AGVideoIsPlaying, nullptr);
    // homura::HookPltFunction(libGameMain, AGVideoPlayOffset, AGVideoPlay, nullptr);
}
