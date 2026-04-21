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

#include "PvZ/Lawn/Widget/VSSetupAddonWidget.h"
#include "Homura/MemberUtils.h"
#include "PvZ/Lawn/Board/Challenge.h"
#include "PvZ/Lawn/Board/SeedBank.h"
#include "PvZ/Lawn/Board/SeedPacket.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/Widget/VSSetupMenu.h"
#include "PvZ/SexyAppFramework/Widget/Checkbox.h"
#include "PvZ/TodLib/Common/TodStringFile.h"

using namespace Sexy;

VSSetupAddonWidget::VSSetupAddonWidget(VSSetupMenu *theVSSetupMenu) {
    mButtonListener = theVSSetupMenu;

    mBackButton = MakeNewButton(VSSetupAddonWidget::VSSetupAddonWidget_Back,
                                mButtonListener,
                                theVSSetupMenu,
                                "[BACK_TO_MODE_SELECT]",
                                nullptr,
                                *Sexy::IMAGE_SEEDCHOOSER_BUTTON_DISABLED,
                                *Sexy::IMAGE_SEEDCHOOSER_BUTTON_GLOW,
                                *Sexy::IMAGE_SEEDCHOOSER_BUTTON_GLOW);
    mBackButton->mTextOffsetX = -2;
    mBackButton->mTextOffsetY = -4;
    mBackButton->mTextDownOffsetX = 1;
    mBackButton->mTextDownOffsetY = 1;
    mBackButton->SetFont(*Sexy_FONT_DWARVENTODCRAFT18_Addr);
    (*mBackButton->mColors)[ButtonWidget::COLOR_LABEL] = Color(0, 205, 0);
    mBackButton->Resize(800, 520, 160, 50);
    mBoard->AddWidget(mBackButton);

    mExtraPacketsMode = mApp->mPlayerInfo->mVSExtraPacketsMode;
    mExtraSeedsMode = mApp->mPlayerInfo->mVSExtraSeedsMode;
    mBanMode = mApp->mPlayerInfo->mVSBanMode;
    mBalancePatchMode = mApp->mPlayerInfo->mVSBalancePatchMode;

    mExtraPacketsCheckbox = MakeNewCheckbox(VSSetupAddonWidget_ExtraPackets, this, theVSSetupMenu, mExtraPacketsMode);
    mExtraSeedsCheckbox = MakeNewCheckbox(VSSetupAddonWidget_ExtraSeeds, this, theVSSetupMenu, mExtraSeedsMode);
    mBanModeCheckbox = MakeNewCheckbox(VSSetupAddonWidget_BanMode, this, theVSSetupMenu, mBanMode);
    mBalancePatchCheckbox = MakeNewCheckbox(VSSetupAddonWidget_BalancePatch, this, theVSSetupMenu, mBalancePatchMode);

    mBoard->AddWidget(mExtraPacketsCheckbox);
    mBoard->AddWidget(mExtraSeedsCheckbox);
    mBoard->AddWidget(mBanModeCheckbox);
    mBoard->AddWidget(mBalancePatchCheckbox);

    mExtraPacketsCheckbox->Resize(VS_ADDON_BUTTON_X, VS_BUTTON_EXTRA_PACKETS_Y, 175, 50);
    mExtraSeedsCheckbox->Resize(VS_ADDON_BUTTON_X, VS_BUTTON_EXTRA_SEEDS_Y, 175, 50);
    mBanModeCheckbox->Resize(VS_ADDON_BUTTON_X, VS_BUTTON_BAN_MODE_Y, 175, 50);
    mBalancePatchCheckbox->Resize(VS_ADDON_BUTTON_X, VS_BUTTON_BALANCE_PATCH_Y, 175, 50);

    if (gIsVSShuffleMode) {
        SetDisable(mExtraPacketsCheckbox);
        SetDisable(mExtraSeedsCheckbox);
        SetDisable(mBanModeCheckbox);
        mBanMode = false;
        mBanModeCheckbox->SetChecked(false, false);
    }
}

VSSetupAddonWidget::~VSSetupAddonWidget() {
    if (mBoard) {
        if (mBackButton) {
            mBoard->RemoveWidget(mBackButton);
        }
        if (mExtraPacketsCheckbox) {
            mBoard->RemoveWidget(mExtraPacketsCheckbox);
        }
        if (mExtraSeedsCheckbox) {
            mBoard->RemoveWidget(mExtraSeedsCheckbox);
        }
        if (mBanModeCheckbox) {
            mBoard->RemoveWidget(mBanModeCheckbox);
        }
        if (mBalancePatchCheckbox) {
            mBoard->RemoveWidget(mBalancePatchCheckbox);
        }
    }

    delete mBackButton;
    if (mExtraPacketsCheckbox) {
        homura::CallVirtualFunc<Sexy::Widget, 1, void>(static_cast<Sexy::Widget *>(mExtraPacketsCheckbox));
        mExtraPacketsCheckbox = nullptr;
    }
    if (mExtraSeedsCheckbox) {
        homura::CallVirtualFunc<Sexy::Widget, 1, void>(static_cast<Sexy::Widget *>(mExtraSeedsCheckbox));
        mExtraSeedsCheckbox = nullptr;
    }
    if (mBanModeCheckbox) {
        homura::CallVirtualFunc<Sexy::Widget, 1, void>(static_cast<Sexy::Widget *>(mBanModeCheckbox));
        mBanModeCheckbox = nullptr;
    }
    if (mBalancePatchCheckbox) {
        homura::CallVirtualFunc<Sexy::Widget, 1, void>(static_cast<Sexy::Widget *>(mBalancePatchCheckbox));
        mBalancePatchCheckbox = nullptr;
    }
}

void VSSetupAddonWidget::SetDisable(Sexy::Widget *theWidget) {
    theWidget->mDisabled = true;
    theWidget->SetVisible(false);
}

void VSSetupAddonWidget::ButtonDepress(this VSSetupAddonWidget &self, int theId) {
    if (theId == VSSetupAddonWidget_Back) {
        self.mApp->mVSSetupMenu->CloseVSSetup(true);
        self.mApp->KillBoard();
        self.mApp->ShowChallengeScreen(ChallengePage::CHALLENGE_PAGE_VS);
    }
}

void VSSetupAddonWidget::CheckboxChecked(int theId, bool checked) {
    mApp->PlaySample(*Sexy_SOUND_BUTTONCLICK_Addr);

    switch (theId) {
        case VSSetupAddonWidget_ExtraPackets:
            mExtraPacketsMode = checked;
            mExtraPacketsCheckbox->SetChecked(mExtraPacketsMode, false);
            mApp->mPlayerInfo->mVSExtraPacketsMode = mExtraPacketsMode;
            break;
        case VSSetupAddonWidget_ExtraSeeds:
            mExtraSeedsMode = checked;
            mExtraSeedsCheckbox->SetChecked(mExtraSeedsMode, false);
            mApp->mPlayerInfo->mVSExtraSeedsMode = mExtraSeedsMode;
            break;
        case VSSetupAddonWidget_BanMode:
            mBanMode = checked;
            mBanModeCheckbox->SetChecked(mBanMode, false);
            mApp->mPlayerInfo->mVSBanMode = mBanMode;
            break;
        case VSSetupAddonWidget_BalancePatch:
            mBalancePatchMode = checked;
            mBalancePatchCheckbox->SetChecked(mBalancePatchMode, false);
            mApp->mPlayerInfo->mVSBalancePatchMode = mBalancePatchMode;
            break;
        default:
            break;
    }

    mApp->mPlayerInfo->SaveDetails();
}

void VSSetupAddonWidget::Draw(Graphics *g) {
    if (!mDrawString)
        return;

    g->SetFont(*Sexy_FONT_DWARVENTODCRAFT18_Addr);
    if (mExtraPacketsCheckbox->mVisible) {
        g->SetColor(mExtraPacketsMode ? Color(255, 255, 153) : Color(0, 205, 0, 255));
        g->DrawString(TodStringTranslate("[VS_UI_EXTRA_SLOTS]"), VS_ADDON_BUTTON_X + 40, VS_BUTTON_EXTRA_PACKETS_Y + 25);
    }
    if (mExtraSeedsCheckbox->mVisible) {
        g->SetColor(mExtraSeedsMode ? Color(255, 255, 153) : Color(0, 205, 0, 255));
        g->DrawString(TodStringTranslate("[VS_UI_EXTRA_SEEDS]"), VS_ADDON_BUTTON_X + 40, VS_BUTTON_EXTRA_SEEDS_Y + 25);
    }
    if (mBanModeCheckbox->mVisible) {
        g->SetColor(mBanMode ? Color(255, 255, 153) : Color(0, 205, 0, 255));
        g->DrawString(TodStringTranslate("[VS_UI_BAN_MODE]"), VS_ADDON_BUTTON_X + 40, VS_BUTTON_BAN_MODE_Y + 25);
        if (mBanMode) {
            g->SetColor(Color(205, 0, 0, 255));
            g->DrawString(TodStringTranslate("[VS_UI_BAN_PHASE_BIG]"), 200, 45);
        }
    }
    if (mBalancePatchCheckbox->mVisible) {
        g->SetColor(mBalancePatchMode ? Color(255, 255, 153) : Color(0, 205, 0, 255));
        g->DrawString(TodStringTranslate("[VS_UI_BALANCE_PATCH]"), VS_ADDON_BUTTON_X + 40, VS_BUTTON_BALANCE_PATCH_Y + 25);
    }
}

void PickMPRandomSeeds(LawnApp *theApp, std::vector<SeedType> &thePlantSeeds, std::vector<SeedType> &theZombieSeeds, bool theIsZombie) {
    thePlantSeeds.clear();
    theZombieSeeds.clear();

    int alreadyPicked = 0;
    if ((theApp->mPlayerInfo->mLevel > 20 || theApp->HasFinishedAdventure()) && Sexy::Rand(5) == 1) {
        thePlantSeeds.push_back(SEED_INSTANT_COFFEE);
        alreadyPicked = theIsZombie ? 0 : 1;
    }

    std::vector<SeedType> &aSeeds = theIsZombie ? theZombieSeeds : thePlantSeeds;
    const int poolGroupOffset = 3 * alreadyPicked;

    for (int num_possible = alreadyPicked; num_possible < theApp->mBoard->GetNumSeedsInBank(true) - 1; ++num_possible) {
        int pool = 0;
        if (num_possible == 2 || num_possible == 3)
            pool = 1;
        else if (num_possible == 4)
            pool = 2;

        const int poolBase = theIsZombie ? 6 + pool : poolGroupOffset + pool;

        int numSeedsInPool = 0;
        for (int i = 0; i < 8; ++i) {
            const SeedType aSeedType = VSSetupMenu::msRandomPools[poolBase][i];
            if (aSeedType == SeedType::SEED_NONE)
                break;

            ++numSeedsInPool;
        }

        SeedType aSeedType = SeedType::SEED_NONE;
        for (;;) {
            do {
                const int seedIndex = Sexy::Rand(numSeedsInPool);
                aSeedType = VSSetupMenu::msRandomPools[poolBase][seedIndex];
            } while (std::ranges::contains(aSeeds, aSeedType));

            if (theApp->HasSeedType(aSeedType, theIsZombie))
                break;
        }

        aSeeds.push_back(aSeedType);
    }

    if (theIsZombie) {
        if (NeedSeedZombieImp(theApp)) {
            auto it = std::ranges::find(aSeeds, SeedType::SEED_ZOMBIE_NORMAL);
            if (it != aSeeds.end()) {
                *it = SeedType::SEED_ZOMBIE_IMP;
            }
        }
    } else {
        if (NeedSeedInstantCoffee(theApp)) {
            // 如果随到的新卡组中没有蘑菇则不生成咖啡豆
            for (int i = 0; i < 5; ++i) {
                if (!Plant::IsNocturnal(aSeeds[i]))
                    return;
            }

            if (!aSeeds.empty()) {
                aSeeds[0] = SeedType::SEED_INSTANT_COFFEE;
            }
        } else {
            // 如果卡组中有咖啡豆但没有蘑菇, 则替换豌豆射手为卡池3里的随机蘑菇(小喷, 大喷, 胆小)
            if (!aSeeds.empty() && aSeeds[0] == SeedType::SEED_INSTANT_COFFEE) {
                bool replacePeashooter = false;
                for (int i = 0; i < 5; ++i) {
                    if (!Plant::IsNocturnal(aSeeds[i])) {
                        replacePeashooter = true;
                        break;
                    }
                }
                if (replacePeashooter) {
                    const int seedIndex = Sexy::Rand(3);
                    aSeeds[1] = VSSetupMenu::msRandomPools[3][seedIndex];
                }
            }
        }
        if (NeedSeedTallnut(theApp)) {
            auto it = std::ranges::find(aSeeds, SeedType::SEED_WALLNUT);
            if (it != aSeeds.end()) {
                *it = SeedType::SEED_TALLNUT;
            }
        }
        if (NeedSeedUmbrella(theApp)) {
            auto it = std::ranges::find(aSeeds, SeedType::SEED_SPIKEWEED);
            if (it != aSeeds.end()) {
                *it = SeedType::SEED_UMBRELLA;
            }
        }
        if (NeedSeedMagnetshroom(theApp)) {
            auto it = std::ranges::find(aSeeds, SeedType::SEED_ICESHROOM);
            if (it != aSeeds.end()) {
                *it = SeedType::SEED_MAGNETSHROOM;
            }
        }
        if (NeedSeedSplitPea(theApp)) {
            auto it = std::ranges::find(aSeeds, SeedType::SEED_PEASHOOTER);
            if (it != aSeeds.end()) {
                *it = SeedType::SEED_SPLITPEA;
            }
        }

        int numPeasInBank = 0;
        for (int i = 0; i < 5; ++i) {
            if (IsPeaSeedType(aSeeds[i])) {
                ++numPeasInBank;
            }
        }
        if (numPeasInBank >= 2 || CountPeasOnScreen(theApp) >= 3) {
            auto it = std::ranges::find(aSeeds, SeedType::SEED_GRAVEBUSTER);
            if (it != aSeeds.end()) {
                *it = SeedType::SEED_TORCHWOOD;
            }
        }
    }
}

SeedType PickNextRandomSeed(LawnApp *theApp, std::vector<SeedType> &thePlantSeeds, std::vector<SeedType> &theZombieSeeds, bool theIsZombie, int theSeedIndex) {
    PickMPRandomSeeds(theApp, thePlantSeeds, theZombieSeeds, theIsZombie);
    SeedType aSeedType = theIsZombie ? theZombieSeeds[theSeedIndex - 1] : thePlantSeeds[theSeedIndex - 1];

    if (theIsZombie) {
        if (NeedSeedTallnut(theApp) && aSeedType == SeedType::SEED_ZOMBIE_NORMAL) {
            aSeedType = SeedType::SEED_ZOMBIE_IMP;
        }
    } else {
        if (NeedSeedInstantCoffee(theApp) && theSeedIndex == 1) {
            aSeedType = SeedType::SEED_INSTANT_COFFEE;
        }
        if (NeedSeedTallnut(theApp) && aSeedType == SeedType::SEED_WALLNUT) {
            aSeedType = SeedType::SEED_TALLNUT;
        }
        if (NeedSeedUmbrella(theApp) && aSeedType == SeedType::SEED_SPIKEWEED) {
            aSeedType = SeedType::SEED_UMBRELLA;
        }
        if (NeedSeedMagnetshroom(theApp) && aSeedType == SeedType::SEED_ICESHROOM) {
            aSeedType = SeedType::SEED_MAGNETSHROOM;
        }
        if (NeedSeedSplitPea(theApp) && aSeedType == SeedType::SEED_PEASHOOTER) {
            aSeedType = SeedType::SEED_SPLITPEA;
        }
        if (NeedSeedTorchwood(theApp) && aSeedType == SeedType::SEED_GRAVEBUSTER) {
            aSeedType = SeedType::SEED_TORCHWOOD;
        }
    }

    return aSeedType;
}

bool NeedSeedInstantCoffee(LawnApp *theApp) {
    // 种子栏存在可用的蘑菇
    for (int i = 1; i < 6; ++i) {
        SeedPacket aSeedPacket = theApp->mBoard->mSeedBank[0]->mSeedPackets[i];
        if (Plant::IsNocturnal(aSeedPacket.mPacketType) && aSeedPacket.mActive) {
            return true;
        }
    }
    // 场上存在未唤醒且头顶没有咖啡豆的植物
    Plant *aPlant = nullptr;
    while (theApp->mBoard->IteratePlants(aPlant)) {
        if (aPlant->mIsAsleep && !theApp->mBoard->GetTopPlantAt(aPlant->mPlantCol, aPlant->mRow, PlantPriority::TOPPLANT_ONLY_FLYING)) {
            return true;
        }
    }
    return false;
}

bool NeedSeedTallnut(LawnApp *theApp) {
    // 僵尸种子栏存在可用的蹦蹦僵尸
    for (int i = 1; i < 6; ++i) {
        SeedPacket aSeedPacket = theApp->mBoard->mSeedBank[1]->mSeedPackets[i];
        if (aSeedPacket.mPacketType == SeedType::SEED_ZOMBIE_POGO && aSeedPacket.mActive) {
            return true;
        }
    }
    // 场上存在正在弹跳的蹦蹦僵尸
    Zombie *aZombie = nullptr;
    while (theApp->mBoard->IterateZombies(aZombie)) {
        if (aZombie->mMindControlled) {
            continue;
        }
        if (aZombie->IsBouncingPogo()) {
            return true;
        }
    }
    return false;
}

bool NeedSeedUmbrella(LawnApp *theApp) {
    // 僵尸种子栏存在可用的蹦极僵尸或投篮僵尸
    for (int i = 1; i < 6; ++i) {
        SeedPacket aSeedPacket = theApp->mBoard->mSeedBank[1]->mSeedPackets[i];
        if ((aSeedPacket.mPacketType == SeedType::SEED_ZOMBIE_BUNGEE || aSeedPacket.mPacketType == SeedType::SEED_ZOMBIE_CATAPULT) && aSeedPacket.mActive) {
            return true;
        }
    }
    // 场上剩余篮球数大于15的投篮僵尸
    Zombie *aZombie = nullptr;
    while (theApp->mBoard->IterateZombies(aZombie)) {
        if (aZombie->IsDeadOrDying()) {
            continue;
        }
        if (aZombie->mZombieType == ZombieType::ZOMBIE_CATAPULT && aZombie->mSummonCounter > 15) {
            return true;
        }
    }
    return false;
}

bool IsIronItemZombieType(ZombieType theZombieType) {
    return theZombieType == ZombieType::ZOMBIE_PAIL || theZombieType == ZombieType::ZOMBIE_DOOR || theZombieType == ZombieType::ZOMBIE_FOOTBALL || theZombieType == ZombieType::ZOMBIE_JACK_IN_THE_BOX
        || theZombieType == ZombieType::ZOMBIE_DIGGER || theZombieType == ZombieType::ZOMBIE_POGO || theZombieType == ZombieType::ZOMBIE_LADDER || theZombieType == ZombieType::ZOMBIE_TRASHCAN;
}

bool NeedSeedMagnetshroom(LawnApp *theApp) {
    // 僵尸种子栏存在2个以上可用的铁具类僵尸
    int aNumIronItemZombies = 0;
    for (int i = 1; i < 6; ++i) {
        SeedPacket aSeedPacket = theApp->mBoard->mSeedBank[1]->mSeedPackets[i];
        ZombieType aZombieType = Challenge::IZombieSeedTypeToZombieType(aSeedPacket.mPacketType);
        if (IsIronItemZombieType(aZombieType) && aSeedPacket.mActive) {
            ++aNumIronItemZombies;
        }
    }
    if (aNumIronItemZombies >= 2) {
        return true;
    }
    // 场上存在3个以上的铁具类僵尸
    int aCount = 0;
    Zombie *aZombie = nullptr;
    while (theApp->mBoard->IterateZombies(aZombie)) {
        if (aZombie->mMindControlled) {
            continue;
        }
        if (IsIronItemZombieType(aZombie->mZombieType)) {
            ++aCount;
        }
    }
    if (aCount >= 3) {
        return true;
    }
    return false;
}

bool NeedSeedSplitPea(LawnApp *theApp) {
    // 僵尸种子栏存在可用的矿工僵尸
    for (int i = 1; i < 6; ++i) {
        SeedPacket aSeedPacket = theApp->mBoard->mSeedBank[1]->mSeedPackets[i];
        if (aSeedPacket.mPacketType == SeedType::SEED_ZOMBIE_DIGGER && aSeedPacket.mActive) {
            return true;
        }
    }
    // 场上存在存活且未被魅惑的矿工僵尸
    Zombie *aZombie = nullptr;
    while (theApp->mBoard->IterateZombies(aZombie)) {
        if (aZombie->IsDeadOrDying() || aZombie->mMindControlled) {
            continue;
        }
        if (aZombie->mZombieType == ZombieType::ZOMBIE_DIGGER) {
            return true;
        }
    }
    return false;
}

bool IsPeaSeedType(SeedType theSeedType) {
    return theSeedType == SeedType::SEED_PEASHOOTER || theSeedType == SeedType::SEED_REPEATER || theSeedType == SeedType::SEED_THREEPEATER || theSeedType == SeedType::SEED_SPLITPEA;
}

int CountPeasOnScreen(LawnApp *theApp) {
    int aCount = 0;
    Plant *aPlant = nullptr;
    while (theApp->mBoard->IteratePlants(aPlant)) {
        if (IsPeaSeedType(aPlant->mSeedType)) {
            ++aCount;
        }
    }
    return aCount;
}

bool NeedSeedTorchwood(LawnApp *theApp) {
    // 种子栏存在2株以上可用的豌豆类种子
    int aNumPeasInBank = 0;
    for (int i = 1; i < 6; ++i) {
        SeedPacket aSeedPacket = theApp->mBoard->mSeedBank[0]->mSeedPackets[i];
        if (IsPeaSeedType(aSeedPacket.mPacketType) && aSeedPacket.mActive) {
            ++aNumPeasInBank;
        }
    }
    if (aNumPeasInBank >= 2) {
        return true;
    }
    // 场上存在3株以上的豌豆类植物
    if (CountPeasOnScreen(theApp) >= 3) {
        return true;
    }
    return false;
}

bool NeedSeedZombieImp(LawnApp *theApp) {
    // 种子栏存在可用的土豆雷
    for (int i = 1; i < 6; ++i) {
        SeedPacket aSeedPacket = theApp->mBoard->mSeedBank[0]->mSeedPackets[i];
        if (aSeedPacket.mPacketType == SeedType::SEED_POTATOMINE && aSeedPacket.mActive) {
            return true;
        }
    }
    // 场上存在土豆雷
    Plant *aPlant = nullptr;
    while (theApp->mBoard->IteratePlants(aPlant)) {
        if (aPlant->mSeedType == SeedType::SEED_POTATOMINE) {
            return true;
        }
    }
    return false;
}
