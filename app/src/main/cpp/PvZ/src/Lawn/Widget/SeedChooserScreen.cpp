#include "PvZ/Lawn/Widget/SeedChooserScreen.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Board/CutScene.h"
#include "PvZ/Lawn/Board/SeedBank.h"
#include "PvZ/Lawn/GamepadControls.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Misc.h"
#include "PvZ/Symbols.h"
#include "PvZ/SexyAppFramework/Widget/GameButton.h"
#include "PvZ/TodLib/Common/TodStringFile.h"

using namespace Sexy;

namespace {
constexpr int mSeedPacketWidth = 53;
constexpr int mSeedPacketHeight = 70;

Sexy::GameButton *gSeedChooserScreenMainMenuButton;
SeedChooserTouchState gSeedChooserTouchState = SeedChooserTouchState::SEEDCHOOSER_TOUCHSTATE_NONE;
} // namespace


SeedChooserScreen::SeedChooserScreen(bool theIsZombieChooser) {
    Create(theIsZombieChooser);
}


void SeedChooserScreen::Create(bool theIsZombieChooser) {
    // 记录当前游戏状态，同时修复在没解锁商店图鉴时依然显示相应按钮的问题、对战选种子界面的按钮问题；
    // 还添加了生存模式保留上次选卡，添加坚果艺术关卡默认选择坚果，添加向日葵艺术关卡默认选择坚果、杨桃、萝卜伞
    mApp = (LawnApp *)*Sexy_gSexyAppBase_Addr;
    mBoard = mApp->mBoard;
    GameMode mGameMode = mApp->mGameMode;
    if (mBoard->mCutScene->IsSurvivalRepick() && !mApp->IsCoopMode()) {
        GamepadControls *gamePad = mBoard->mGamepadControls1;
        SeedBank *mSeedBank = gamePad->GetSeedBank();
        int mNumPackets = mSeedBank->mNumPackets;
        std::vector<SeedType> aSeedArray(mNumPackets);
        SeedType aImitaterType = SeedType::SEED_NONE;
        for (int i = 0; i < mNumPackets; i++) {
            SeedPacket aSeedPacket = mSeedBank->mSeedPackets[i];
            aSeedArray[i] = aSeedPacket.mPacketType;
            if (aSeedPacket.mPacketType == SeedType::SEED_IMITATER && aImitaterType == SeedType::SEED_NONE) {
                aImitaterType = aSeedPacket.mImitaterType;
            }
        }

        old_SeedChooserScreen_SeedChooserScreen(this, theIsZombieChooser);

        // 实现无尽模式保留上次选卡。为什么不直接像WP版那样一一对应地选卡呢？因为玩家有可能通过爆炸坚果修改卡槽选中了多个相同类型的卡片或不在SeedChooser内的卡片，一一对应的话会有BUG
        int theValidChosenSeedNum = 0;
        for (int i = 0; i < mNumPackets; i++) {
            SeedType theSeed = aSeedArray[i];
            if (theSeed >= SeedType::NUM_SEEDS_IN_CHOOSER)
                continue;

            ChosenSeed *theChosenSeed = &(mChosenSeeds[theSeed]);
            if (theChosenSeed->mSeedType == SeedType::SEED_IMITATER) {
                theChosenSeed->mImitaterType = aImitaterType;
            }

            if (theChosenSeed->mSeedState == ChosenSeedState::SEED_IN_BANK)
                continue;

            GetSeedPositionInBank(theValidChosenSeedNum, &theChosenSeed->mX, &theChosenSeed->mY, 0);
            theChosenSeed->mEndX = theChosenSeed->mX;
            theChosenSeed->mEndY = theChosenSeed->mY;
            theChosenSeed->mStartX = theChosenSeed->mX;
            theChosenSeed->mStartY = theChosenSeed->mY;
            theChosenSeed->mSeedState = ChosenSeedState::SEED_IN_BANK;
            theChosenSeed->mSeedIndexInBank = theValidChosenSeedNum;
            theValidChosenSeedNum++;
        }

        mSeedsInBothBank = theValidChosenSeedNum;
        mSeedsIn1PBank = theValidChosenSeedNum;
        if (theValidChosenSeedNum == mNumPackets) {
            EnableStartButton(true);
        }

    } else {
        old_SeedChooserScreen_SeedChooserScreen(this, theIsZombieChooser);
    }

    if (mGameMode == GameMode::GAMEMODE_CHALLENGE_ART_CHALLENGE_WALLNUT) {
        ChosenSeed *theChosenSeed = &(mChosenSeeds[SeedType::SEED_WALLNUT]);
        theChosenSeed->mX = mBoard->GetSeedPacketPositionX(0, 0, 0);
        theChosenSeed->mY = 8;
        theChosenSeed->mEndX = theChosenSeed->mX;
        theChosenSeed->mEndY = theChosenSeed->mY;
        theChosenSeed->mStartX = theChosenSeed->mX;
        theChosenSeed->mStartY = theChosenSeed->mY;
        theChosenSeed->mSeedState = ChosenSeedState::SEED_IN_BANK;
        theChosenSeed->mSeedIndexInBank = 0;
        mSeedsInBothBank += 1;
        mSeedsIn1PBank += 1;
    } else if (mGameMode == GameMode::GAMEMODE_CHALLENGE_ART_CHALLENGE_SUNFLOWER) {
        SeedType types[] = {SeedType::SEED_WALLNUT, SeedType::SEED_STARFRUIT, SeedType::SEED_UMBRELLA};
        for (int i = 0; i < sizeof(types) / sizeof(types[0]); ++i) {
            ChosenSeed *theChosenSeed = &(mChosenSeeds[types[i]]);
            GetSeedPositionInBank(i, &theChosenSeed->mX, &theChosenSeed->mY, 0);
            theChosenSeed->mEndX = theChosenSeed->mX;
            theChosenSeed->mEndY = theChosenSeed->mY;
            theChosenSeed->mStartX = theChosenSeed->mX;
            theChosenSeed->mStartY = theChosenSeed->mY;
            theChosenSeed->mSeedState = ChosenSeedState::SEED_IN_BANK;
            theChosenSeed->mSeedIndexInBank = i;
            mSeedsInBothBank += 1;
            mSeedsIn1PBank += 1;
        }
    }

    // int mViewLawnButton = *((uint32_t *) seedChooserScreen + 958);

    if (mGameMode == GameMode::GAMEMODE_MP_VS) { // 去除对战中的冗余按钮
        if (mStoreButton != nullptr) {
            mStoreButton->mDisabled = true;
            mStoreButton->mBtnNoDraw = true;
        }
        if (mAlmanacButton != nullptr) {
            mAlmanacButton->mDisabled = true;
            mAlmanacButton->mBtnNoDraw = true;
        }
        if (mStartButton != nullptr) { // 此处仿照PS3版处理，同时去除双方的开始按钮
            mStartButton->mDisabled = true;
            mStartButton->mBtnNoDraw = true;
        }
    } else {
        if (mStoreButton != nullptr) {
            if (!mApp->CanShowStore()) { // 去除在未解锁商店时商店按钮
                mStoreButton->mDisabled = true;
                mStoreButton->mBtnNoDraw = true;
            }
        }
        if (mAlmanacButton != nullptr) {
            if (!mApp->CanShowAlmanac()) { // 去除在未解锁图鉴时的图鉴按钮
                mAlmanacButton->mDisabled = true;
                mAlmanacButton->mBtnNoDraw = true;
            }
        }
    }

    if (mApp->mGameMode != GameMode::GAMEMODE_MP_VS && !mIsZombieChooser) {
        pvzstl::string str = TodStringTranslate("[MENU_BUTTON]");
        gSeedChooserScreenMainMenuButton = MakeButton(104, &mButtonListener, this, (SexyString &)str);
        gSeedChooserScreenMainMenuButton->Resize(mApp->IsCoopMode() ? 345 : 650, -3, 120, 80);
        AddWidget(gSeedChooserScreenMainMenuButton);
    }
}


void LawnApp::KillSeedChooserScreen() {
    SeedChooserScreen *seedChooserScreen = mSeedChooserScreen;
    if (seedChooserScreen != nullptr && mGameMode != GameMode::GAMEMODE_MP_VS) {
        seedChooserScreen->RemoveWidget(gSeedChooserScreenMainMenuButton);
        gSeedChooserScreenMainMenuButton->Destroy();;
        gSeedChooserScreenMainMenuButton = nullptr;
    }

    old_LawnApp_KillSeedChooserScreen(this);
}

void SeedChooserScreen::RebuildHelpbar() {
    // 拓宽Widget大小
    if (mApp->mGameMode != GameMode::GAMEMODE_MP_VS && !mIsZombieChooser) {
        Resize(mX, mY, 800, 600); // 原本(472,521)，改为(800,600)，不然没办法点击模仿者按钮和底栏三按钮。
    } else {
        Resize(mX, mY, mWidth, 600);
    }

    return old_SeedChooserScreen_RebuildHelpbar(this);
}


void SeedChooserScreen::Update() {
    // 记录当前1P选卡是否选满
    GameMode mGameMode = mApp->mGameMode;
    if (mGameMode >= GameMode::GAMEMODE_TWO_PLAYER_COOP_DAY && mGameMode <= GameMode::GAMEMODE_TWO_PLAYER_COOP_ENDLESS) {
        m1PChoosingSeeds = mSeedsIn1PBank < 4;
    }

    return old_SeedChooserScreen_Update(this);
}

void SeedChooserScreen::EnableStartButton(int theIsEnabled) {
    // 双人键盘模式下结盟选满后直接开始
    if (theIsEnabled && mApp->IsCoopMode() && isKeyboardTwoPlayerMode) {
        old_SeedChooserScreen_EnableStartButton(this, theIsEnabled);
        OnStartButton();
        mBoard->mSeedBank2->mSeedPackets[3].mPacketType = mSeedType2;
        mBoard->mSeedBank2->mSeedPackets[3].mImitaterType = SeedType::SEED_NONE;
        return;
    }

    return old_SeedChooserScreen_EnableStartButton(this, theIsEnabled);
}

void SeedChooserScreen::OnStartButton() {
    if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS) {
        // 如果是对战模式，则直接关闭种子选择界面。用于修复对战模式选卡完毕后点击开始按钮导致的闪退
        return CloseSeedChooser();
    }

    return old_SeedChooserScreen_OnStartButton(this);
}

bool SeedChooserScreen::SeedNotAllowedToPick(SeedType theSeedType) {
    // 解除更多对战场地中的某些植物不能选取的问题，如泳池对战不能选荷叶，屋顶对战不能选花盆
    if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS && VSBackGround > 1) {
        // 直接在其他对战场景解锁全部植物即可
        return false;
    }
    // 此处添加一些逻辑，就可以自定义Ban卡
    // 此处Ban卡仅对植物方生效，theSeedType取值范围是0~39。

    return old_SeedChooserScreen_SeedNotAllowedToPick(this, theSeedType);
}

SeedType SeedChooserScreen::GetZombieSeedType(SeedType theSeedType) {
    int aSeedType = theSeedType + SEED_ZOMBIE_TOMBSTONE;
    return aSeedType > SEED_ZOMBIE_BALLOON ? SEED_NONE : (SeedType)aSeedType;
    // 解锁更多对战僵尸   return aSeedType > 90 ? -1 : aSeedType;
}

ZombieType SeedChooserScreen::GetZombieType(ZombieType theZombieType) {
    return theZombieType > ZOMBIE_REDEYE_GARGANTUAR ? ZOMBIE_INVALID : theZombieType;
}

void SeedChooserScreen::ClickedSeedInChooser(ChosenSeed *theChosenSeed, int thePlayerIndex) {
    // 实现1P结盟选卡选满后自动转换为2P选卡
    if (mApp->IsCoopMode())
        thePlayerIndex = !m1PChoosingSeeds;

    old_SeedChooserScreen_ClickedSeedInChooser(this, theChosenSeed, thePlayerIndex);
}

void SeedChooserScreen::CrazyDavePickSeeds() {
    if (daveNoPickSeeds) {
        return;
    }

    return old_SeedChooserScreen_CrazyDavePickSeeds(this);
}

void SeedChooserScreen::ClickedSeedInBank(ChosenSeed *theChosenSeed, unsigned int thePlayerIndex) {
    // 解决结盟1P选够4个种子之后，无法点击种子栏内的已选种子来退选的问题
    if (mApp->IsCoopMode()) {
        thePlayerIndex = theChosenSeed->mChosenPlayerIndex;
    }

    old_SeedChooserScreen_ClickedSeedInBank(this, theChosenSeed, thePlayerIndex);
}

void SeedChooserScreen::GameButtonDown(ButtonCode theButton, unsigned int thePlayerIndex) {
    // 修复结盟2P无法选择模仿者
    if (mApp->IsCoopMode() && theButton == 6) {
        if (mChooseState == SeedChooserState::CHOOSE_VIEW_LAWN) {
            return old_SeedChooserScreen_GameButtonDown(this, theButton, thePlayerIndex);
        }

        if (mApp->mTwoPlayerState == -1 && mPlayerIndex != thePlayerIndex)
            return;

        SeedType aSeedType = thePlayerIndex ? mSeedType2 : mSeedType1;
        int mSeedsInBank = mSeedsInBothBank;
        // 此处将判定条件改为选满8个种子时无法选取模仿者。原版游戏中此处是选满4个则无法选取，导致模仿者选取出现问题。
        if (aSeedType == SeedType::SEED_IMITATER && mSeedsInBank < 8) {
            if (mChosenSeeds[SeedType::SEED_IMITATER].mSeedState != ChosenSeedState::SEED_IN_BANK) {
                // 先将已选种子数改为0，然后执行旧函数，这样模仿者选取界面就被打开了。
                mSeedsInBothBank = 0;
                old_SeedChooserScreen_GameButtonDown(this, theButton, thePlayerIndex);
                // 然后再恢复已选种子数即可。
                mSeedsInBothBank = mSeedsInBank;
                return;
            }
        }
    }

    return old_SeedChooserScreen_GameButtonDown(this, theButton, thePlayerIndex);
}

void SeedChooserScreen::DrawPacket(
    Sexy::Graphics *g, int x, int y, SeedType theSeedType, SeedType theImitaterType, float thePercentDark, int theGrayness, Color *theColor, bool theDrawCost, bool theUseCurrentCost) {
    // 修复SeedChooser里的卡片亮度不正确。
    // 已选的卡片grayness为55，不推荐的卡片grayness为115。theColor则固定为{255,255,255,255}。

    // int theConvertedGrayness = ((theColor->mRed + theColor->mGreen + theColor->mBlue) / 3 + theGrayness) / 2;
    // 此算法用于在对战模式将非选卡的一方的卡片整体变暗。但这种算法下，55亮度会变成155亮度，115亮度会变成185亮度，严重影响非对战模式的选卡体验。所以需要修复。

    int theConvertedGrayness = (mApp->mGameMode == GameMode::GAMEMODE_MP_VS) ? ((theColor->mRed + theColor->mGreen + theColor->mBlue) / 3 + theGrayness) / 2 : theGrayness;
    DrawSeedPacket(g, x, y, theSeedType, theImitaterType, thePercentDark, theConvertedGrayness, theDrawCost, false, mIsZombieChooser, theUseCurrentCost);
}

void SeedChooserScreen::ButtonPress(int theId) {
    LawnApp *lawnApp = (LawnApp *)*gLawnApp_Addr;
    lawnApp->mSeedChooserScreen->mFocusedChildWidget = nullptr; // 修复点击菜单后无法按键选取植物卡片
}

void SeedChooserScreen::ButtonDepress(int theId) {

    if (mSeedsInFlight > 0 || mChooseState == SeedChooserState::CHOOSE_VIEW_LAWN || !mMouseVisible) {
        return;
    }

    if (theId == SeedChooserScreen_Menu) {
        LawnApp_PlaySample(mApp, *Sexy_SOUND_PAUSE_Addr);
        mApp->DoNewOptions(false, 0);
        return;
    }

    return old_SeedChooserScreen_ButtonDepress(this, theId);
}

void SeedChooserScreen::GetSeedPositionInBank(int theIndex, int *x, int *y, int thePlayerIndex) {
    // 修复对战选卡时的错位
    if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS) {
        int SeedPacketPositionX = mBoard->GetSeedPacketPositionX(theIndex, 0, mIsZombieChooser);
        *x = mSeedBank1->mX + SeedPacketPositionX - mX;
        *y = mSeedBank1->mY + 8 - mY;
        return;
    }

    return old_SeedChooserScreen_GetSeedPositionInBank(this, theIndex, x, y, thePlayerIndex);
}

void SeedChooserScreen::ShowToolTip(unsigned int thePlayerIndex) {
    old_SeedChooserScreen_ShowToolTip(this, thePlayerIndex);

    if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS && mIsZombieChooser) {
        SeedType aSeedType = SeedHitTest(mCursorPositionX2, mCursorPositionY2);
        if (mChosenSeeds[aSeedType - SeedType::SEED_ZOMBIE_TOMBSTONE].mSeedState == ChosenSeedState::SEED_IN_BANK && mChosenSeeds[aSeedType - SeedType::SEED_ZOMBIE_TOMBSTONE].mCrazyDavePicked) {
            pvzstl::string str = TodStringTranslate("[ZOMBIE_BOSS_WANTS]");
            mToolTip2->SetWarningText(str);
        }
    }
}

SeedType SeedChooserScreen::GetZombieIndexBySeedType(SeedType theSeedType) {
    return theSeedType - SEED_ZOMBIE_TOMBSTONE < 0 ? SeedType::SEED_NONE : (SeedType)(theSeedType - SEED_ZOMBIE_TOMBSTONE);
}

void SeedChooserScreen::MouseMove(int x, int y) {
    SeedType seedType = SeedHitTest(x, y);
    // 该函数探测不到模仿者位置
    if (seedType == SeedType::SEED_NONE) {
        return;
    }
    if (mIsZombieChooser) {
        SeedType zombieSeedType = GetZombieIndexBySeedType(seedType);
        GetSeedPositionInChooser(zombieSeedType, mCursorPositionX2, mCursorPositionY2);
        mSeedType2 = zombieSeedType;
    } else if (m1PChoosingSeeds) {
        if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS && seedType > SeedType::SEED_MELONPULT)
            return;
        GetSeedPositionInChooser(seedType, mCursorPositionX1, mCursorPositionY1);
        mSeedType1 = seedType;
    } else {
        GetSeedPositionInChooser(seedType, mCursorPositionX2, mCursorPositionY2);
        mSeedType2 = seedType;
    }
}

void SeedChooserScreen::MouseDown(int x, int y, int theClickCount) {
    GameMode mGameMode = mApp->mGameMode;

    m1PChoosingSeeds = !mApp->IsCoopMode() || mSeedsIn1PBank < 4;

    bool mViewLawnButtonDisabled = mViewLawnButton == nullptr || !mBoard->mCutScene->IsSurvivalRepick();
    bool mStoreButtonDisabled = mStoreButton == nullptr || mStoreButton->mDisabled;
    bool mStartButtonDisabled = mStartButton == nullptr || mStartButton->mDisabled;
    bool mAlmanacButtonDisabled = mAlmanacButton == nullptr || mAlmanacButton->mDisabled;

    if (!mViewLawnButtonDisabled) { // !mDisabled
        Sexy::Rect mViewLawnButtonRect = {mViewLawnButton->mX, mViewLawnButton->mY, mViewLawnButton->mWidth, 50};
        //        LOGD("mStoreButtonRect:%d %d %d %d",mStoreButtonRect[0],mStoreButtonRect[1],mStoreButtonRect[2],mStoreButtonRect[3]);
        if (TRect_Contains(&mViewLawnButtonRect, x, y)) {
            LawnApp_PlaySample(mApp, *Sexy_SOUND_TAP_Addr);
            gSeedChooserTouchState = SeedChooserTouchState::ViewLawnButton;
            //            GameButtonDown(seedChooserScreen, 8, 0);
            return;
        }
    }

    if (!mStoreButtonDisabled) { // !mDisabled
        Sexy::Rect mStoreButtonRect = {mStoreButton->mX, mStoreButton->mY, mStoreButton->mWidth, 50};
        //        LOGD("mStoreButtonRect:%d %d %d %d",mStoreButtonRect[0],mStoreButtonRect[1],mStoreButtonRect[2],mStoreButtonRect[3]);
        if (TRect_Contains(&mStoreButtonRect, x, y)) {
            LawnApp_PlaySample(mApp, *Sexy_SOUND_TAP_Addr);
            gSeedChooserTouchState = SeedChooserTouchState::StoreButton;
            //            GameButtonDown(seedChooserScreen, 8, 0);
            return;
        }
    }

    if (!mStartButtonDisabled) { // !mDisabled
        Sexy::Rect mStartButtonRect = {mStartButton->mX, mStartButton->mY, mStartButton->mWidth, 50};
        if (TRect_Contains(&mStartButtonRect, x, y)) {
            LawnApp_PlaySample(mApp, *Sexy_SOUND_TAP_Addr);
            gSeedChooserTouchState = SeedChooserTouchState::StartButton;

            //            SeedChooserScreen_OnStartButton(seedChooserScreen);
            return;
        }
    }

    if (!mAlmanacButtonDisabled) { // !mDisabled
        Sexy::Rect mAlmanacButtonRect = {mAlmanacButton->mX, mAlmanacButton->mY, mAlmanacButton->mWidth, 50};
        if (TRect_Contains(&mAlmanacButtonRect, x, y)) {
            LawnApp_PlaySample(mApp, *Sexy_SOUND_TAP_Addr);
            gSeedChooserTouchState = SeedChooserTouchState::AlmanacButton;

            //            GameButtonDown(seedChooserScreen, 9, 0);
            return;
        }
    }

    if (HasPacket(SeedType::SEED_IMITATER, 0) && mGameMode != GameMode::GAMEMODE_MP_VS && !mIsZombieChooser) {
        int mImitaterPositionX = 0;
        int mImitaterPositionY = 0;
        GetSeedPositionInChooser(SeedType::SEED_IMITATER, mImitaterPositionX, mImitaterPositionY);
        Sexy::Rect mImitaterPositionRect = {mImitaterPositionX, mImitaterPositionY, mSeedPacketWidth, mSeedPacketHeight};
        if (TRect_Contains(&mImitaterPositionRect, x, y)) {
            if (m1PChoosingSeeds) {
                mCursorPositionX1 = mImitaterPositionX;
                mCursorPositionY1 = mImitaterPositionY;
                mSeedType1 = SeedType::SEED_IMITATER;
            } else {
                mCursorPositionX2 = mImitaterPositionX;
                mCursorPositionY2 = mImitaterPositionY;
                mSeedType2 = SeedType::SEED_IMITATER;
            }
            GameButtonDown(BUTTONCODE_A, !m1PChoosingSeeds);
            return;
        }
    }
    SeedType aSeedType = SeedHitTest(x, y);
    // 该函数探测不到模仿者位置

    if (aSeedType == SeedType::SEED_NONE) {
        return;
    }

    if (!mIsZombieChooser && (mChosenSeeds[aSeedType].mSeedState == ChosenSeedState::SEED_FLYING_TO_BANK || mChosenSeeds[aSeedType].mSeedState == ChosenSeedState::SEED_FLYING_TO_CHOOSER)) {
        return;
    }

    if (mIsZombieChooser) {
        SeedType zombieSeedType = GetZombieIndexBySeedType(aSeedType);
        GetSeedPositionInChooser(zombieSeedType, mCursorPositionX2, mCursorPositionY2);
        mSeedType2 = zombieSeedType;
    } else if (m1PChoosingSeeds) {
        if (mGameMode == GameMode::GAMEMODE_MP_VS && aSeedType > SeedType::SEED_MELONPULT)
            return;
        GetSeedPositionInChooser(aSeedType, mCursorPositionX1, mCursorPositionY1);
        mSeedType1 = aSeedType;
    } else {
        GetSeedPositionInChooser(aSeedType, mCursorPositionX2, mCursorPositionY2);
        mSeedType2 = aSeedType;
    }
    gSeedChooserTouchState = SeedChooserTouchState::SeedChooser;
}

void SeedChooserScreen::MouseDrag(int x, int y) {
    if (gSeedChooserTouchState == SeedChooserTouchState::SeedChooser) {
        SeedType aSeedType = SeedHitTest(x, y);
        // 该函数探测不到模仿者位置
        if (aSeedType == SeedType::SEED_NONE) {
            return;
        }
        if (mIsZombieChooser) {
            SeedType zombieSeedType = GetZombieIndexBySeedType(aSeedType);
            GetSeedPositionInChooser(zombieSeedType, mCursorPositionX2, mCursorPositionY2);
            mSeedType2 = zombieSeedType;
        } else if (m1PChoosingSeeds) {
            if (mApp->mGameMode == GameMode::GAMEMODE_MP_VS && aSeedType > SeedType::SEED_MELONPULT)
                return;
            GetSeedPositionInChooser(aSeedType, mCursorPositionX1, mCursorPositionY1);
            mSeedType1 = aSeedType;
        } else {
            GetSeedPositionInChooser(aSeedType, mCursorPositionX2, mCursorPositionY2);
            mSeedType2 = aSeedType;
        }
    }
}

void SeedChooserScreen::MouseUp(int x, int y) {
    switch (gSeedChooserTouchState) {
        case SeedChooserTouchState::ViewLawnButton:
            ButtonDepress(SeedChooserScreen_ViewLawn);
            break;
        case SeedChooserTouchState::SeedChooser:
            if (mIsZombieChooser) {
                GameButtonDown(BUTTONCODE_A, 1);
            } else if (m1PChoosingSeeds) {
                GameButtonDown(BUTTONCODE_A, 0);
            } else {
                GameButtonDown(BUTTONCODE_A, 1);
            }
            break;
        case SeedChooserTouchState::StoreButton:
            ButtonDepress(SeedChooserScreen_Store);
            break;
        case SeedChooserTouchState::StartButton:
            ButtonDepress(SeedChooserScreen_Start);
            break;
        case SeedChooserTouchState::AlmanacButton:
            ButtonDepress(SeedChooserScreen_Almanac);
            break;
        default:
            break;
    }
    gSeedChooserTouchState = SeedChooserTouchState::SEEDCHOOSER_TOUCHSTATE_NONE;
}
