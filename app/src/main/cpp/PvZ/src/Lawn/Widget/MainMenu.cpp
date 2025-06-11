#include "PvZ/Lawn/Widget/MainMenu.h"
#include "Homura/Logger.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Board/Zombie.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Misc.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/SexyAppFramework/Graphics/MemoryImage.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Effect/Reanimator.h"
#include "PvZ/SexyAppFramework/Widget/GameButton.h"
#include "PvZ/TodLib/Effect/Attachment.h"
#include "PvZ/TodLib/Common/TodStringFile.h"
#include "PvZ/TodLib/Common/TodFoley.h"

#include <cstddef>

#include <algorithm>

using namespace Sexy;
using namespace std;

namespace {

enum AchievementWidgetState {
    NOT_SHOWING = 0,
    SLIDING_IN = 1,
    SHOWING = 2,
    SLIDING_OUT = 3,
};

constexpr int gAchievementHoleLength = 136;
constexpr int gAchievementHoleWormPos = 0x10;
constexpr int gAchievementHoleGemsPos = 0x19;
constexpr int gAchievementHoleChuzzlePos = 0x26;
constexpr int gAchievementHoleBjornPos = 0x34;
constexpr int gAchievementHolePipePos = 0x45;
constexpr int gAchievementHoleTikiPos = 0x55;
constexpr int gAchievementHoleHeavyRocksPos = 0x65;
constexpr int gAchievementHoleDuWeiPos = 0x72;

constexpr int gKeyboardScrollTime = 20;
constexpr int gMainMenuHeight = 720 - 2; // 作用：将成就界面上升2个像素点，以更紧密地贴合主界面。奇怪，理论上720是严丝合缝，为什么实际有2像素偏差呢？

bool isPatched;
int gMainMenuAchievementCounter;
int gMainMenuAchievementsWidgetY;
int gMainMenuAchievementsKeyboardScrollWidgetY;
int gMainMenuAchievementKeyboardScrollCounter;
bool gMainMenuAchievementKeyboardScrollDirection;

AchievementWidgetState gAchievementState = NOT_SHOWING;
AchievementsWidget *gMainMenuAchievementsWidget;
Sexy::GameButton *gMainMenuAchievementsBack;
int gFoleyVolumeCounter;

} // namespace


static FoleyType MainMenu_GetFoleyTypeByScene(int scene) {
    FoleyType theType = FoleyType::FOLEY_MENU_CENTRE;
    switch (scene) {
        case 0:
            theType = FoleyType::FOLEY_MENU_LEFT;
            break;
        case 1:
            theType = FoleyType::FOLEY_MENU_CENTRE;
            break;
        case 2:
            theType = FoleyType::FOLEY_MENU_RIGHT;
    }
    return theType;
}

void MainMenu::Update() {
    // 成就界面处理
    isMainMenu = true;
    requestDrawShovelInCursor = false;
    requestDrawButterInCursor = false;

    if (!isPatched) {
        game_patches::autoPickupSeedPacketDisable.Modify();
        isPatched = true;
    }

    // 白噪音播放和淡入淡出
    if (mIsFading) {
        float num = mFadeCounterFloat + 0.005;
        mFadeCounterFloat = fmin(num, 1.0f);
    } else {
        if (InTransition()) {
            gFoleyVolumeCounter++;
            FoleyType aType = MainMenu_GetFoleyTypeByScene(mScene);
            FoleyType aNextType = MainMenu_GetFoleyTypeByScene(mSceneNext);
            if (!mApp->mSoundSystem->IsFoleyPlaying(aNextType)) {
                mApp->PlayFoley(aNextType);
                mApp->SetFoleyVolume(aNextType, 0);
            }
            float theVolume = TodAnimateCurveFloat(0, 93, gFoleyVolumeCounter, mApp->mPlayerInfo->mSoundVolume, 0, TodCurves::CURVE_BOUNCE_SLOW_MIDDLE);
            if (gFoleyVolumeCounter >= 46) {
                mApp->SetFoleyVolume(aNextType, theVolume);
                if (mApp->mSoundSystem->IsFoleyPlaying(aType)) {
                    mApp->mSoundSystem->StopFoley(aType);
                }
            } else {
                mApp->SetFoleyVolume(aType, theVolume);
            }
        } else {
            gFoleyVolumeCounter = 0;
            FoleyType aType = MainMenu_GetFoleyTypeByScene(mScene);
            if (gAchievementState == NOT_SHOWING) {
                if (!mApp->mSoundSystem->IsFoleyPlaying(aType) && mExitCounter == 0) {
                    //                    mApp->PlayFoley(aType);
                    mApp->SetFoleyVolume(aType, 0);
                }
                if (mEnterReanimationCounter > 0) {
                    float theVolume = TodAnimateCurveFloat(110, 0, mEnterReanimationCounter, 0, mApp->mPlayerInfo->mSoundVolume, TodCurves::CURVE_LINEAR);
                    mApp->SetFoleyVolume(aType, theVolume);
                }
            }
            if (gAchievementState == SLIDING_IN) {
                float theVolume = TodAnimateCurveFloat(100, 0, gMainMenuAchievementCounter, mApp->mPlayerInfo->mSoundVolume, 0, TodCurves::CURVE_LINEAR);
                mApp->SetFoleyVolume(aType, theVolume);
            }
            if (gAchievementState == SLIDING_OUT && gMainMenuAchievementCounter <= 100) {
                float theVolume = TodAnimateCurveFloat(100, 0, gMainMenuAchievementCounter, 0, mApp->mPlayerInfo->mSoundVolume, TodCurves::CURVE_LINEAR);
                mApp->SetFoleyVolume(aType, theVolume);
            }
        }
    }


    if (gMainMenuAchievementKeyboardScrollCounter != 0) {
        gMainMenuAchievementKeyboardScrollCounter--;
        if (gMainMenuAchievementsWidget != nullptr) {
            int theY = TodAnimateCurve(gKeyboardScrollTime, 0, gMainMenuAchievementKeyboardScrollCounter, 0, 192, TodCurves::CURVE_LINEAR);
            int theNewY = gMainMenuAchievementsKeyboardScrollWidgetY - (gMainMenuAchievementKeyboardScrollDirection ? theY : -theY);
            if (theNewY > gMainMenuHeight)
                theNewY = gMainMenuHeight;
            if (theNewY < 720 + gMainMenuHeight - (gAchievementHoleLength + 1) * addonImages.hole->mHeight)
                theNewY = 720 + gMainMenuHeight - (gAchievementHoleLength + 1) * addonImages.hole->mHeight;
            gMainMenuAchievementsWidget->Move(gMainMenuAchievementsWidget->mX, theNewY);
        }
    }

    if (gAchievementState == SLIDING_IN) {
        gMainMenuAchievementCounter--;
        if (gMainMenuAchievementsWidget != nullptr) {
            int theY = TodAnimateCurve(100, 0, gMainMenuAchievementCounter, 660, -60, TodCurves::CURVE_EASE_IN_OUT);
            Move(mX, -720 + theY);
        }
        if (gMainMenuAchievementCounter == 0) {
            gAchievementState = SHOWING;
            int holder1[1];
            TodStringTranslate(holder1, "[CLOSE]");
            gMainMenuAchievementsBack = MakeButton(ACHIEVEMENTS_BACK_BUTTON, &mButtonListener, this, (SexyString &)holder1);
            gMainMenuAchievementsBack->Resize(1000, 564 + 720, 170, 50);
            StringDelete(holder1);
            AddWidget((Widget*)gMainMenuAchievementsBack);
        }
    }

    if (gAchievementState == SLIDING_OUT) {
        gMainMenuAchievementCounter--;
        if (gMainMenuAchievementsWidget != nullptr) {
            if (gMainMenuAchievementCounter <= 100) {
                int theY = TodAnimateCurve(100, 0, gMainMenuAchievementCounter, -780, -60, TodCurves::CURVE_EASE_IN_OUT);
                Move(mX, theY);
            } else {
                int theAchievementsY = TodAnimateCurve(150, 100, gMainMenuAchievementCounter, gMainMenuAchievementsWidgetY, gMainMenuHeight, TodCurves::CURVE_EASE_IN_OUT);
                gMainMenuAchievementsWidget->Move(gMainMenuAchievementsWidget->mX, theAchievementsY);
            }
        }
        if (gMainMenuAchievementCounter == 0) {
            gAchievementState = NOT_SHOWING;
           RemoveWidget(gMainMenuAchievementsWidget);
            MaskHelpWidget_Delete(gMainMenuAchievementsWidget);
            gMainMenuAchievementsWidget = nullptr;
            if (gMainMenuAchievementsBack != nullptr) {
                RemoveWidget((Widget*)gMainMenuAchievementsBack);
                gMainMenuAchievementsBack->Destroy();
                gMainMenuAchievementsBack = nullptr;
            }
            Sexy::Widget *achievementsButton = FindWidget(ACHIEVEMENTS_BUTTON);
            mFocusedChildWidget = achievementsButton;
            if (!mIsFading)
                mApp->PlayFoley(FoleyType::FOLEY_MENU_CENTRE);
        }
    }
    if (gAchievementState == SHOWING) {
        return;
    }

    return old_MainMenu_Update(this);
}

void MainMenu::ButtonPress(MainMenuButtonId theSelectedButton) {
    // 按下按钮的声音
    LawnApp *gLawnApp = (LawnApp *)*gLawnApp_Addr;
    if (gLawnApp->mGameSelector->InTransition())
        return;

    switch (theSelectedButton) {
        case HOUSE_BUTTON:
        case ACHIEVEMENTS_BUTTON:
        case HELP_AND_OPTIONS_BUTTON:
        case UNLOCK_BUTTON:
        case RETURN_TO_ARCADE_BUTTON:
        case MORE_BUTTON:
        case BACK_POT_BUTTON:
        case STORE_BUTTON:
        case ZEN_BUTTON:
        case ALMANAC_BUTTON:
        case MAIL_BUTTON:
            //            LawnApp_PlaySample(lawnApp, *Sexy_SOUND_CERAMIC_Addr);
            gLawnApp->PlayFoley(FoleyType::FOLEY_CERAMIC);
            break;
        default:
            LawnApp_PlaySample(gLawnApp, *Sexy_SOUND_GRAVEBUTTON_Addr);
            break;
    }
}

void MainMenu::ButtonDepress(MainMenuButtonId theSelectedButton) {
    // 为1.1.5解锁触控或确认键进入“更多游戏模式”
    if (InTransition())
        return;
    if (mIsFading)
        return;
    if (mEnterReanimationCounter > 0)
        return;
    if (gAchievementState == SLIDING_IN || gAchievementState == SLIDING_OUT)
        return; // 在进入、退出成就时不允许玩家操作
    if (theSelectedButton == MORE_WAYS_BUTTON) {
        // 如果当前选中的按钮为"更多游戏方式"
        SetScene(MENUSCENE_MORE_WAYS);
        return;
    }

    // 为1.1.1添加触控或确认进入对战结盟模式，并检测是否解锁对战结盟
    switch (theSelectedButton) {
        case ADVENTURE_BUTTON:
        case START_ADVENTURE_BUTTON:
            StartAdventureMode();
            if (LawnPlayerInfo_GetFlag(mApp->mPlayerInfo, 4096) && mApp->mPlayerInfo->mLevel == 35) {
                mPressedButtonId = STORE_BUTTON;
                unkBool3 = true;
                (*(void (**)(MainMenu *))(*(uint32_t *)this + 496))(this);
            } else {
                mPressedButtonId = ADVENTURE_BUTTON;
                mApp->mGameMode = GameMode::GAMEMODE_ADVENTURE;
                (*(void (**)(MainMenu *))(*(uint32_t *)this + 496))(this);
            }
            return;
        case VS_BUTTON: // 如果按下了对战按钮
            if (mVSModeLocked) {
                // 如果没解锁结盟（冒险2-1解锁）
                mApp->LawnMessageBox(Dialogs::DIALOG_MESSAGE, "[MODE_LOCKED]", "[VS_LOCKED_MESSAGE]", "[DIALOG_BUTTON_OK]", "", 3);
                return;
            }
            mPressedButtonId = theSelectedButton;
            (*(int (**)(MainMenu *))(*(uint32_t *)this + 496))(this);
            return;
        case VS_COOP_BUTTON: // 如果按下了结盟按钮
            if (mCoopModeLocked) {
                // 如果没解锁结盟（冒险2-1解锁）
                mApp->LawnMessageBox(Dialogs::DIALOG_MESSAGE, "[MODE_LOCKED]", "[COOP_LOCKED_MESSAGE]", "[DIALOG_BUTTON_OK]", "", 3);
                return;
            }
            mPressedButtonId = theSelectedButton;
            (*(void (**)(MainMenu *))(*(uint32_t *)this + 496))(this);
            return;
        case ACHIEVEMENTS_BUTTON:
        case ACHIEVEMENTS_BACK_BUTTON:
            if (gMainMenuAchievementsWidget == nullptr) {
                gAchievementState = SLIDING_IN;
                gMainMenuAchievementCounter = 100;
                gMainMenuAchievementsWidget = (AchievementsWidget *)operator new(sizeof(AchievementsWidget));
                MaskHelpWidget_MaskHelpWidget(gMainMenuAchievementsWidget, mApp);
                gMainMenuAchievementsWidget->mIsScrolling = false;
                gMainMenuAchievementsWidget->Resize(0, gMainMenuHeight, 1280, addonImages.hole->mHeight * (gAchievementHoleLength + 1));
                gMainMenuAchievementsWidget->mWidgetId = ACHIEVEMENTS_BUTTON;
                AddWidget(gMainMenuAchievementsWidget);
            } else {
                gAchievementState = SLIDING_OUT;
                gMainMenuAchievementCounter = gMainMenuAchievementsWidget->mY == gMainMenuHeight ? 100 : 150;
                gMainMenuAchievementsWidgetY = gMainMenuAchievementsWidget->mY;
            }
            return;
        case HOUSE_BUTTON:
            mPressedButtonId = theSelectedButton;
            (*(void (**)(MainMenu *))(*(uint32_t *)this + 496))(this);
            return;
        case UNLOCK_BUTTON:
            mPressedButtonId = theSelectedButton;
            (*(void (**)(MainMenu *))(*(uint32_t *)this + 496))(this);
            return;
        default:
            old_MainMenu_ButtonDepress(this, theSelectedButton);
            return;
    }
}

void MainMenu::KeyDown(Sexy::KeyCode theKeyCode) {
    // 为1.1.5解锁左方向键进入“更多游戏模式”
    if (InTransition())
        return;
    if (mIsFading)
        return;
    if (mEnterReanimationCounter > 0)
        return;

    if (gMainMenuAchievementsWidget != nullptr) {
        if (gAchievementState != SHOWING)
            return;
        if (theKeyCode == Sexy::KEYCODE_ESCAPE || theKeyCode == Sexy::KEYCODE_ESCAPE2) {
            MainMenu::ButtonDepress(ACHIEVEMENTS_BUTTON);
        } else if (theKeyCode == Sexy::KEYCODE_UP || theKeyCode == Sexy::KEYCODE_DOWN) {
            if (gMainMenuAchievementKeyboardScrollCounter != 0) {
                return;
                //                int theNewY = gMainMenuAchievementsKeyboardScrollWidgetY -(gMainMenuAchievementKeyboardScrollDirection ? 192 : -192);
                //                if (theNewY > gMainMenuHeight) theNewY = gMainMenuHeight;
                //                if (theNewY < 720 +gMainMenuHeight - (gAchievementHoleLength + 1) * addonImages.hole->mHeight) theNewY =  720 +gMainMenuHeight - (gAchievementHoleLength + 1) *
                //                addonImages.hole->mHeight; Sexy_Widget_Move(gMainMenuAchievementsWidget, gMainMenuAchievementsWidget->mX, theNewY);
            }
            gMainMenuAchievementKeyboardScrollCounter = gKeyboardScrollTime;
            gMainMenuAchievementsKeyboardScrollWidgetY = gMainMenuAchievementsWidget->mY;
            gMainMenuAchievementKeyboardScrollDirection = theKeyCode == Sexy::KEYCODE_DOWN;
        }
        return;
    }

    MainMenuButtonId mSelectedButton = (MainMenuButtonId)mFocusedChildWidget->mWidgetId;
    if ((mSelectedButton == ADVENTURE_BUTTON || mSelectedButton == MORE_WAYS_BUTTON || mSelectedButton == START_ADVENTURE_BUTTON) && theKeyCode == Sexy::KEYCODE_LEFT) {
        // 如果当前选中的按钮为"冒险模式"或者为"更多游戏方式"，同时玩家又按下了左方向键
        SetScene(MENUSCENE_MORE_WAYS);
        return;
    }

    old_MainMenu_KeyDown(this, theKeyCode);
}

void MainMenu::UpdateHouseReanim() {
    Reanimation *aMainMenuReanim = mApp->ReanimationTryToGet(mMainMenuReanimID);
    if (aMainMenuReanim == nullptr)
        return;
    Reanimation_SetImageDefinition(aMainMenuReanim, "leaderboards", addonImages.leaderboards);
    if (!showHouse)
        return;
    Reanimation_SetImageOrigin(aMainMenuReanim, "Hood1", addonImages.hood1_house);
    Reanimation_SetImageOrigin(aMainMenuReanim, "Hood2", addonImages.hood2_house);
    Reanimation_SetImageOrigin(aMainMenuReanim, "Hood3", addonImages.hood3_house);
    Reanimation_SetImageOrigin(aMainMenuReanim, "Hood4", addonImages.hood4_house);
    Reanimation_SetImageOrigin(aMainMenuReanim, "ground color copy", addonImages.house_hill_house);
    Reanimation *aHouseReanim = mApp->ReanimationTryToGet(mHouseReanimID);
    if (aHouseReanim == nullptr)
        return;
    mApp->SetHouseReanim(aHouseReanim);
}

void MainMenu::SyncProfile(bool a2) {
    //    LOGD("MainMenu_SyncProfile");
    old_MainMenu_SyncProfile(this, a2);
    mApp->mNewIs3DAccelerated = mApp->mPlayerInfo == nullptr || !mApp->mPlayerInfo->mIs3DAcceleratedClosed;
}


constexpr int mZombatarButtonX = 2800;
constexpr int mZombatarButtonY = -20;

void MainMenu::EnableButtons() {
    Sexy::Widget *achievementsButton = FindWidget(ACHIEVEMENTS_BUTTON);
    ((Sexy::GameButton *)achievementsButton)->SetDisabled(false);
    Sexy::Widget *leaderboardsButton = FindWidget(HOUSE_BUTTON);
    leaderboardsButton->SetVisible(true);
    Sexy::Widget *helpButton = FindWidget(HELP_AND_OPTIONS_BUTTON);
    Sexy::Widget *backButton = FindWidget(RETURN_TO_ARCADE_BUTTON);
    helpButton->mFocusLinks[3] = backButton;
    backButton->mFocusLinks[2] = helpButton;
    Sexy::Widget *zombatarButton = FindWidget(UNLOCK_BUTTON);
    ((Sexy::GameButton *)zombatarButton)->SetDisabled(false);
    ((Sexy::GameButton *)zombatarButton)->mButtonImage = addonImages.SelectorScreen_WoodSign3;
    ((Sexy::GameButton *)zombatarButton)->mDownImage = addonImages.SelectorScreen_WoodSign3_press;
    ((Sexy::GameButton *)zombatarButton)->mOverImage = addonImages.SelectorScreen_WoodSign3_press;
    zombatarButton->mFocusLinks[0] = FindWidget(BACK_POT_BUTTON);
    zombatarButton->mFocusLinks[1] = zombatarButton->mFocusLinks[0];
    zombatarButton->mFocusLinks[2] = zombatarButton->mFocusLinks[0];
    zombatarButton->mFocusLinks[3] = zombatarButton->mFocusLinks[0];
    zombatarButton->Resize(addonImages.SelectorScreen_WoodSign3->mWidth / 2, 0, addonImages.SelectorScreen_WoodSign3->mWidth, addonImages.SelectorScreen_WoodSign3->mHeight);
    Reanimation *mainMenuReanim = mApp->ReanimationTryToGet(mMainMenuReanimID);
    if (mainMenuReanim == nullptr)
        return;
    Reanimation_HideTrack(mainMenuReanim, "unlock stem", true);

    int index[3] = {mainMenuReanim->FindTrackIndex("unlock"), mainMenuReanim->FindTrackIndex("unlock pressed"), mainMenuReanim->FindTrackIndex("unlock selected")};

    for (int i = 0; i < 3; ++i) {
        ReanimatorTrack *reanimatorTrack = mainMenuReanim->mDefinition->mTracks + index[i];
        int mTransformCount = reanimatorTrack->mTransformCount;
        for (int j = 0; j < mTransformCount; ++j) {
            reanimatorTrack->mTransforms[j].mTransX = mZombatarButtonX;
            reanimatorTrack->mTransforms[j].mTransY = mZombatarButtonY;
        }
    }

    //    if (mainMenu->mPressedButtonId == UNLOCK_BUTTON) {
    //        LOGD("123123213");
    //        Reanimation *mainMenuReanim = ReanimationTryToGet(mainMenu->mApp, mainMenu->mMainMenuReanimID);
    //        if (mainMenuReanim != nullptr) {
    //            int index = Reanimation_FindTrackIndex(mainMenuReanim, "unlock");
    //            ReanimatorTrack *reanimatorTrack = mainMenuReanim->mDefinition->mTracks + index;
    //            int mTransformCount = reanimatorTrack->mTransformCount;
    //            int theX = mZombatarButtonX + addonImages.SelectorScreen_WoodSign3->mWidth;
    //            for (int j = 0; j < mTransformCount; ++j) {
    //                reanimatorTrack->mTransforms[j].mTransX = theX;
    //            }
    //        }
    //    }
}

void MainMenu::Enter() {
    old_MainMenu_Enter(this);
    UpdateHouseReanim();
    // 解除成就按钮的禁用状态
    EnableButtons();
}

bool MainMenu::UpdateExit() {
    return old_MainMenu_UpdateExit(this);
}

void MainMenu::Exit() {
    old_MainMenu_Exit(this);
    // 解除成就按钮的禁用状态
    EnableButtons();
}

void MainMenu::OnExit() {
    mApp->mSoundSystem->StopFoley(FoleyType::FOLEY_MENU_LEFT);
    mApp->mSoundSystem->StopFoley(FoleyType::FOLEY_MENU_CENTRE);
    mApp->mSoundSystem->StopFoley(FoleyType::FOLEY_MENU_RIGHT);

    if (mPressedButtonId == HOUSE_BUTTON) {
        mApp->KillMainMenu();
        mApp->ShowLeaderboards();
    }

    if (mPressedButtonId == UNLOCK_BUTTON) {
        mApp->KillMainMenu();
        mApp->ShowZombatarScreen();
    }

    //    if (mPressedButtonId == VS_BUTTON) {
    //        KillMainMenu(mApp);
    ////        TODO:为对战添加选择场景
    //        LawnApp_ShowChallengeScreen(mApp, CHALLENGE_PAGE_VS);
    //        return;
    //    }

    old_MainMenu_OnExit(this);
}

void MainMenu::OnScene(int theScene) {
    return old_MainMenu_OnScene(this, theScene);
}

void MainMenu::SyncButtons() {
    // 解除成就按钮的禁用状态,同时刷新房子
    old_MainMenu_SyncButtons(this);
    UpdateHouseReanim();
    EnableButtons();
}

void MainMenu::Create(LawnApp *theApp) {
    old_MainMenu_MainMenu(this, theApp);
}


namespace {
int theOffsetX = 1792;
int theOffsetY = 220;
int theOffsetX1 = 237;
int theOffsetY1 = 60;
} // namespace

void MainMenu::UpdateCameraPosition() {
    old_MainMenu_UpdateCameraPosition(this);
    if (showHouse) {
        Reanimation *houseAnim = mApp->ReanimationTryToGet(mHouseReanimID);
        if (houseAnim != nullptr) {
            houseAnim->SetPosition(mCameraPositionX + theOffsetX, mCameraPositionY + theOffsetY);
        }
    }
}

void MainMenu::AddedToManager(int *a2) {
    old_MainMenu_AddedToManager(this, a2);
    if (!showHouse)
        return;
    Reanimation *reanimation = mApp->AddReanimation(0, 0, 0, ReanimationType::REANIM_LEADERBOARDS_HOUSE);
    //    Reanimation *reanimation = LawnApp_AddReanimation(mainMenu->mApp, mainMenu->mCameraPositionX + theOffsetX,mainMenu->mCameraPositionY + theOffsetY, 0,
    //    ReanimationType::REANIM_LEADERBOARDS_HOUSE);
    reanimation->mCustomFilterEffectColor = {142, 146, 232, 92};
    reanimation->mFilterEffect = FilterEffect::FILTEREFFECT_CUSTOM;


    mApp->SetHouseReanim(reanimation);
    mHouseReanimID = mApp->ReanimationGetID(reanimation);
}

void MainMenu::RemovedFromManager(int *a2) {
    // 记录当前游戏状态
    if (gMainMenuAchievementsWidget != nullptr) {
        RemoveWidget(gMainMenuAchievementsWidget);
    }
    if (gMainMenuAchievementsBack != nullptr) {
        RemoveWidget((Widget*)gMainMenuAchievementsBack);
    }
    old_MainMenu_RemovedFromManager(this, a2);
}

void MainMenu::Delete2() {
    old_MainMenu_Delete2(this);
    if (gMainMenuAchievementsWidget != nullptr) {
        MaskHelpWidget_Delete(gMainMenuAchievementsWidget);
        gMainMenuAchievementsWidget = nullptr;
    }

    if (gMainMenuAchievementsBack != nullptr) {
        gMainMenuAchievementsBack->Destroy();;
        gMainMenuAchievementsBack = nullptr;
    }
}

void MainMenu::Draw(Sexy::Graphics *g) {
    // 实现绘制房子
    if (!showHouse)
        return old_MainMenu_Draw(this, g);

    if (mWidgetManager == nullptr)
        return;
    if ((*(int (**)(LawnApp *, int))(*(uint32_t *)mApp + 412))(mApp, 6) || (*(int (**)(LawnApp *, int))(*(uint32_t *)mApp + 412))(mApp, 5)
        || (*(int (**)(LawnApp *, int))(*(uint32_t *)mApp + 412))(mApp, 60))
        return;

    Reanimation *mainMenuReanim = mApp->ReanimationTryToGet(mMainMenuReanimID);
    if (mainMenuReanim == nullptr)
        return;
    Reanimation *skyReanim = mApp->ReanimationTryToGet(mSkyReanimID);
    Reanimation *sky2Reanim = mApp->ReanimationTryToGet(mSky2ReanimID);

    if (skyReanim != nullptr && sky2Reanim != nullptr) {
        skyReanim->DrawRenderGroup(g, 0);
        SexyTransform2D tmp = sky2Reanim->mOverlayMatrix;
        sky2Reanim->mOverlayMatrix.Scale(1.0, 0.4);
        sky2Reanim->mOverlayMatrix.m[0][1] = -0.4;
        sky2Reanim->DrawRenderGroup(g, 2);
        sky2Reanim->mOverlayMatrix = tmp;
    }
    mainMenuReanim->DrawRenderGroup(g, 1);
    Reanimation *houseAnim = mApp->ReanimationTryToGet(mHouseReanimID);
    if (houseAnim != nullptr) {

        int TrackIndex = mainMenuReanim->FindTrackIndex("House");
        ReanimatorTransform v52{};
        SexyTransform2D v48{};
        mainMenuReanim->GetCurrentTransform(TrackIndex, &v52);
        Reanimation::MatrixFromTransform(v52, (SexyMatrix3 &)v48);
        v48.Translate(mCameraPositionX + theOffsetX1, mCameraPositionY + theOffsetY1);
        houseAnim->mOverlayMatrix = v48;
        houseAnim->DrawRenderGroup(g, 0);
    }
    mainMenuReanim->DrawRenderGroup(g, 0);
    if (mScene == 2) {
        Reanimation *butterFlyReanim = mApp->ReanimationTryToGet(mButterflyReanimID);
        if (butterFlyReanim != nullptr) {
            butterFlyReanim->Draw(g);
        }
    }
    Reanimation *crowReanim = mApp->ReanimationTryToGet(mCrowReanimID);
    if (crowReanim != nullptr && (!unkBool5 || mExitCounter <= 65)) {
        crowReanim->Draw(g);
    }
    MenuWidget::Draw(g);
    DeferOverlay(0);
    if (!InTransition())
        (*((void (**)(MainMenu *, Sexy::Graphics *))vTable + 129))(this, g);
    SexyTransform2D aSexyTransform2D;
    ReanimatorTransform v43;
    int mailAlertTrackIndex = mainMenuReanim->FindTrackIndex("mail alert");
    if (mailAlertTrackIndex > 0 && Mailbox_GetNumUnseenMessages(mApp->mMailBox) > 0) {
        PlayerInfo *mPlayerInfo = mApp->mPlayerInfo;
        if (mPlayerInfo->mLevel > 0 || LawnPlayerInfo_GetFlag(mPlayerInfo, 1)) {
            v43 = ReanimatorTransform();
            mainMenuReanim->GetCurrentTransform(mailAlertTrackIndex, &v43);
            Sexy::Image *mailAlertImage = v43.mImage;
            aSexyTransform2D.Create();
            Reanimation::MatrixFromTransform(v43, (SexyMatrix3 &)aSexyTransform2D);
            aSexyTransform2D.Translate(mCameraPositionX, mCameraPositionY);
            int v14 = mailAlertImage->mWidth;
            int v15 = v14 + 3;
            int v16 = v14 < 0;
            int v17 = v14 & ~(v14 >> 31);
            if (v16)
                v17 = v15;
            aSexyTransform2D.Translate(v17 >> 2, 0.0);
            int v18 = unkMems3[3];
            if (v18 > 99)
                v18 = 0;
            unkMems3[3] = v18;
            TodAnimateCurveFloat(0, 100, v18, 0.75, 0.8, TodCurves::CURVE_SIN_WAVE);
            Sexy::Rect v38 = {0, 0, mailAlertImage->mWidth, mailAlertImage->mHeight};
            g->DrawImageMatrix(mailAlertImage, (SexyMatrix3 &)aSexyTransform2D, v38, 0.0, 0.0, 1);
        }
    }
    int moreTrackIndex = mainMenuReanim->FindTrackIndex("more");
    v43 = ReanimatorTransform();
    mainMenuReanim->GetCurrentTransform(moreTrackIndex, &v43);
    aSexyTransform2D.Create();
    Reanimation::MatrixFromTransform(v43, (SexyMatrix3 &)aSexyTransform2D);
    aSexyTransform2D.Translate(mCameraPositionX, mCameraPositionY);
    aSexyTransform2D.Translate(120.0, 200.0);

    Sexy::Rect v37 = {0, 0, m2DMarkImage->mWidth, m2DMarkImage->mHeight};
    g->DrawImageMatrix(m2DMarkImage, (SexyMatrix3 &)aSexyTransform2D, v37, 0.0, 0.0, 1);
    Sexy::Rect v38 = {15, 15, 90, 90};
    aSexyTransform2D.Translate(-4.0, -16.0);
    g->DrawImageMatrix(mApp->mQRCodeImage, (SexyMatrix3 &)aSexyTransform2D, v38, 0.0, 0.0, 1);
}

void MainMenu::DrawOverlay(Sexy::Graphics *g) {
    // 在成就界面存在时，不显示冒险关卡数
    if (gMainMenuAchievementsWidget != nullptr) {
        return;
    }
    return old_MainMenu_DrawOverlay(this, g);
}

void MainMenu::DrawFade(Sexy::Graphics *g) {
    // 修复主界面的退出动画在高帧率设备上的加速。原理是将计时器的更新从Draw移动至Update
    float num = mFadeCounterFloat;
    //    if (mainMenu->mFadeCounterFloat < 0.992) {
    //        mainMenu->mFadeCounterFloat -= 0.008;
    //    }
    old_MainMenu_DrawFade(this, g);
    mFadeCounterFloat = num;
}

void MaskHelpWidget_Draw(AchievementsWidget *achievementsWidget, Sexy::Graphics *g) {
    int theY = 0;
    int theDiffY = addonImages.hole->mHeight;
    for (int i = 0; i < gAchievementHoleLength; i++) {
        if (i == gAchievementHoleWormPos) {
            g->DrawImage(addonImages.hole_worm, 0, theY);
        } else if (i == gAchievementHoleGemsPos) {
            g->DrawImage(addonImages.hole_gems, 0, theY);
        } else if (i == gAchievementHoleChuzzlePos) {
            g->DrawImage(addonImages.hole_chuzzle, 0, theY);
        } else if (i == gAchievementHoleBjornPos) {
            g->DrawImage(addonImages.hole_bjorn, 0, theY);
        } else if (i == gAchievementHolePipePos) {
            g->DrawImage(addonImages.hole_pipe, 0, theY);
        } else if (i == gAchievementHoleTikiPos) {
            g->DrawImage(addonImages.hole_tiki, 0, theY);
        } else if (i == gAchievementHoleHeavyRocksPos) {
            g->DrawImage(addonImages.hole_heavyrocks, 0, theY);
        } else if (i == gAchievementHoleDuWeiPos) {
            g->DrawImage(addonImages.hole_duwei, 0, theY);
        } else {
            g->DrawImage(addonImages.hole, 0, theY);
        }
        theY += theDiffY;
    }
    g->DrawImage(addonImages.hole_china, 0, theY);
    g->DrawImage(addonImages.hole_top, 0, 0);
    int theAchievementY = 300;
    for (int i = 0; i < AchievementId::MAX_ACHIEVEMENTS; ++i) {
        if (!achievementsWidget->mApp->mPlayerInfo->mAchievements[i]) {
            g->SetColorizeImages(true);
            g->SetColor(gray);
        }
        g->DrawImage(GetIconByAchievementId((AchievementId)i), 330, theAchievementY - 5);
        const char *theAchievementName = GetNameByAchievementId((AchievementId)i);
        int holder[1];
        int holder1[1];
        StrFormat(holder, "[%s]", theAchievementName);
        StrFormat(holder1, "[%s_TEXT]", theAchievementName);
        Color theColor = {0, 255, 0, 255};
        Color theColor1 = {255, 255, 255, 255};
        Sexy::Rect rect = {460, theAchievementY + 60, 540, 0};
        TodDrawString(g, holder, 460, theAchievementY + 40, *Sexy_FONT_HOUSEOFTERROR28_Addr, theColor, DrawStringJustification::DS_ALIGN_LEFT);
        if (i == AchievementId::ACHIEVEMENT_SHOP) {
            int holder2[1];
            TodReplaceNumberString(holder2, holder1, "{coin}", achievementsWidget->mApp->mPlayerInfo->mUsedCoins * 10);
            TodDrawStringWrapped(g, holder2, &rect, *Sexy_FONT_HOUSEOFTERROR20_Addr, &theColor1, DrawStringJustification::DS_ALIGN_LEFT, false);
            StringDelete(holder2);
        } else {
            TodDrawStringWrapped(g, holder1, &rect, *Sexy_FONT_HOUSEOFTERROR20_Addr, &theColor1, DrawStringJustification::DS_ALIGN_LEFT, false);
        }
        StringDelete(holder);
        StringDelete(holder1);
        g->SetColorizeImages(false);
        theAchievementY += theDiffY * 2 / 3;
    }
    int theAccomplishedNum = 0;
    for (int i = 0; i < AchievementId::MAX_ACHIEVEMENTS; ++i) {
        if (achievementsWidget->mApp->mPlayerInfo->mAchievements[i]) {
            theAccomplishedNum++;
        }
    }
    int holder[1];
    StrFormat(holder, "%d/%d", theAccomplishedNum, AchievementId::MAX_ACHIEVEMENTS);
    Color theColor = {255, 240, 0, 255};
    TodDrawString(g, holder, 1060, 173, *Sexy_FONT_DWARVENTODCRAFT18_Addr, theColor, DrawStringJustification::DS_ALIGN_CENTER);
    StringDelete(holder);
}

void MaskHelpWidget_MouseDown(AchievementsWidget *achievementsWidget, int x, int y, int theClickCount) {
    achievementsWidget->mIsScrolling = false;
    achievementsWidget->mMouseDownY = y;
    achievementsWidget->mLastDownY = y;
    achievementsWidget->mLastDownY1 = achievementsWidget->mLastDownY;
    struct timeval tp;
    gettimeofday(&tp, nullptr);
    achievementsWidget->mLastTimeMs = tp.tv_sec * 1000 + tp.tv_usec / 1000; // Convert to milliseconds
    achievementsWidget->mLastTimeMs1 = achievementsWidget->mLastTimeMs;
}

void MaskHelpWidget_MouseDrag(AchievementsWidget *achievementsWidget, int x, int y) {
    if (gAchievementState != SHOWING)
        return;
    int theNewY = std::clamp(achievementsWidget->mY + (y - achievementsWidget->mMouseDownY), 720 + gMainMenuHeight - (gAchievementHoleLength + 1) * addonImages.hole->mHeight, gMainMenuHeight);
    achievementsWidget->Move(achievementsWidget->mX, theNewY);
    achievementsWidget->mLastDownY1 = achievementsWidget->mLastDownY;
    achievementsWidget->mLastDownY = y;
    struct timeval tp;
    gettimeofday(&tp, nullptr);
    achievementsWidget->mLastTimeMs1 = achievementsWidget->mLastTimeMs;
    achievementsWidget->mLastTimeMs = tp.tv_sec * 1000 + tp.tv_usec / 1000; // Convert to milliseconds
}

void MaskHelpWidget_MouseUp(AchievementsWidget *achievementsWidget, int x, int y) {
    struct timeval tp;
    gettimeofday(&tp, nullptr);
    long currentTimeMs = tp.tv_sec * 1000 + tp.tv_usec / 1000; // Convert to milliseconds
    long deltaT = currentTimeMs - achievementsWidget->mLastTimeMs;
    int deltaX = achievementsWidget->mLastDownY - achievementsWidget->mMouseDownY;
    if (deltaT == 0) {
        deltaT = currentTimeMs - achievementsWidget->mLastTimeMs1;
        deltaX = achievementsWidget->mLastDownY1 - achievementsWidget->mMouseDownY;
    }
    if (deltaX != 0 && deltaT != 0) {
        achievementsWidget->mIsScrolling = true;
        achievementsWidget->mVelocity = 5.0f * deltaX / deltaT;
    }
    achievementsWidget->mLastTimeMs = currentTimeMs;
}

void MaskHelpWidget_Update(AchievementsWidget *achievementsWidget) {
    // 实现滚动
    if (achievementsWidget->mIsScrolling) {
        int theNewY = std::clamp<int>(achievementsWidget->mY + achievementsWidget->mVelocity, 720 + gMainMenuHeight - (gAchievementHoleLength + 1) * addonImages.hole->mHeight, gMainMenuHeight);
        achievementsWidget->Move(achievementsWidget->mX, theNewY);
        achievementsWidget->mVelocity *= 0.96;
        if (fabs(achievementsWidget->mVelocity) < 1.0f) {
            achievementsWidget->mIsScrolling = false;
        }
    }
    achievementsWidget->MarkDirty();
}

void ZombatarWidget_SetDefault(ZombatarWidget *zombatarWidget) {
    zombatarWidget->mSelectedTab = 0;
    zombatarWidget->mSelectedSkinColor = 0;
    zombatarWidget->mSelectedHair = 255;
    zombatarWidget->mSelectedHairColor = 255;
    zombatarWidget->mSelectedFHair = 255;
    zombatarWidget->mSelectedFHairColor = 255;
    zombatarWidget->mSelectedFHairPage = 0;
    zombatarWidget->mSelectedTidBit = 255;
    zombatarWidget->mSelectedTidBitColor = 255;
    zombatarWidget->mSelectedEyeWear = 255;
    zombatarWidget->mSelectedEyeWearColor = 255;
    zombatarWidget->mSelectedCloth = 255;
    zombatarWidget->mSelectedAccessory = 255;
    zombatarWidget->mSelectedAccessoryColor = 255;
    zombatarWidget->mSelectedHat = 255;
    zombatarWidget->mSelectedHatColor = 255;
    zombatarWidget->mSelectedBackground = 0;
    zombatarWidget->mSelectedBackgroundColor = 255;
    zombatarWidget->mSelectedBackgroundPage = 0;
    Reanimation_SetZombatarReanim(zombatarWidget->mZombatarReanim);
}

void ZombatarWidget_ButtonPress(ZombatarWidget *zombatarWidget, int id) {}

void ZombatarWidget_ButtonDepress(ZombatarWidget *zombatarWidget, int id) {
    if (id == 1000) {
        LawnApp *lawnApp = (LawnApp *)*gLawnApp_Addr;
        lawnApp->KillZombatarScreen();
        lawnApp->ShowMainMenuScreen();
        return;
    }

    if (id == 1001) {
        LawnApp *lawnApp = (LawnApp *)*gLawnApp_Addr;
        if (lawnApp->LawnMessageBox(Dialogs::DIALOG_MESSAGE, "[ZOMBATAR_FINISHED_WARNING_HEADER]", "[ZOMBATAR_FINISHED_WARNING_TEXT]", "[ZOMBATAR_FINISHED_BUTTON_TEXT]", "[ZOMBATAR_BACK_BUTTON_TEXT]", 1)
            == 1001)
            return;

        if (addonImages.zombatar_portrait != nullptr) {
            reinterpret_cast<MemoryImage *>(addonImages.zombatar_portrait)->Delete();
        }

        MemoryImage* aImage = new MemoryImage();
        aImage->Create(addonZombatarImages.zombatar_background_blank->mWidth, addonZombatarImages.zombatar_background_blank->mHeight);
        aImage->SetImageMode(true, true);
        aImage->mIsVolatile = true;
        Graphics graphics = Graphics(reinterpret_cast<Image *>(aImage));
        TestMenuWidget_DrawPortrait(gMainMenuZombatarWidget, &graphics, 0, 0);
        int holder[1];
        StrFormat(holder, "ZOMBATAR.PNG");
        aImage->WriteToPng(holder);
        StringDelete(holder);
        addonImages.zombatar_portrait = reinterpret_cast<Image *>(aImage);
        gMainMenuZombatarWidget->mShowExistingZombatarPortrait = true;
        gMainMenuZombatarWidget->mShowZombieTypeSelection = false;

        PlayerInfo *playerInfo = lawnApp->mPlayerInfo;
        playerInfo->mZombatarHat = gMainMenuZombatarWidget->mSelectedHat;
        playerInfo->mZombatarHatColor = gMainMenuZombatarWidget->mSelectedHatColor;
        playerInfo->mZombatarHair = gMainMenuZombatarWidget->mSelectedHair;
        playerInfo->mZombatarHairColor = gMainMenuZombatarWidget->mSelectedHairColor;
        playerInfo->mZombatarFacialHair = gMainMenuZombatarWidget->mSelectedFHair;
        playerInfo->mZombatarFacialHairColor = gMainMenuZombatarWidget->mSelectedFHairColor;
        playerInfo->mZombatarAccessory = gMainMenuZombatarWidget->mSelectedAccessory;
        playerInfo->mZombatarAccessoryColor = gMainMenuZombatarWidget->mSelectedAccessoryColor;
        playerInfo->mZombatarTidBit = gMainMenuZombatarWidget->mSelectedTidBit;
        playerInfo->mZombatarTidBitColor = gMainMenuZombatarWidget->mSelectedTidBitColor;
        playerInfo->mZombatarEyeWear = gMainMenuZombatarWidget->mSelectedEyeWear;
        playerInfo->mZombatarEyeWearColor = gMainMenuZombatarWidget->mSelectedEyeWearColor;


        playerInfo->mZombatarEnabled = playerInfo->mZombatarHat != 255 || playerInfo->mZombatarHair != 255 || playerInfo->mZombatarFacialHair != 255 || playerInfo->mZombatarAccessory != 255
            || playerInfo->mZombatarTidBit != 255 || playerInfo->mZombatarEyeWear != 255;
        ZombatarWidget_SetDefault(gMainMenuZombatarWidget);
        return;
    }

    if (id == 1002) {
        if (addonImages.zombatar_portrait != nullptr) {
            gMainMenuZombatarWidget->mShowExistingZombatarPortrait = true;
            ZombatarWidget_SetDefault(gMainMenuZombatarWidget);
        }
        return;
    }

    if (id == 1003) {
        gMainMenuZombatarWidget->mShowExistingZombatarPortrait = false;
        return;
    }

    if (id == 1004) {
        LawnApp *lawnApp = (LawnApp *)*gLawnApp_Addr;
        if (lawnApp->LawnMessageBox(Dialogs::DIALOG_MESSAGE, "[ZOMBATAR_DELETE_HEADER]", "[ZOMBATAR_DELETE_BODY]", "[BUTTON_OK]", "[BUTTON_CANCEL]", 1) == 1001)
            return;
        gMainMenuZombatarWidget->mShowExistingZombatarPortrait = false;
        if (addonImages.zombatar_portrait != nullptr) {
            reinterpret_cast<MemoryImage *>(addonImages.zombatar_portrait)->Delete();
            addonImages.zombatar_portrait = nullptr;
            lawnApp->EraseFile("ZOMBATAR.PNG");
        }
        return;
    }
}

Sexy::Image *ZombatarWidget_GetTabButtonImageByIndex(int index) {
    switch (index) {
        case ZombatarWidget::SKIN:
            return addonZombatarImages.zombatar_skin_button;
        case ZombatarWidget::HAIR:
            return addonZombatarImages.zombatar_hair_button;
        case ZombatarWidget::FHAIR:
            return addonZombatarImages.zombatar_fhair_button;
        case ZombatarWidget::TIDBIT:
            return addonZombatarImages.zombatar_tidbits_button;
        case ZombatarWidget::EYEWEAR:
            return addonZombatarImages.zombatar_eyewear_button;
        case ZombatarWidget::CLOTHES:
            return addonZombatarImages.zombatar_clothes_button;
        case ZombatarWidget::ACCESSORY:
            return addonZombatarImages.zombatar_accessory_button;
        case ZombatarWidget::HAT:
            return addonZombatarImages.zombatar_hats_button;
        case ZombatarWidget::BACKGROUND:
            return addonZombatarImages.zombatar_backdrops_button;
        default:
            return nullptr;
    }
}

Sexy::Image *ZombatarWidget_GetTabButtonDownImageByIndex(int index) {
    switch (index) {
        case ZombatarWidget::SKIN:
            return addonZombatarImages.zombatar_skin_button_highlight;
        case ZombatarWidget::HAIR:
            return addonZombatarImages.zombatar_hair_button_highlight;
        case ZombatarWidget::FHAIR:
            return addonZombatarImages.zombatar_fhair_button_highlight;
        case ZombatarWidget::TIDBIT:
            return addonZombatarImages.zombatar_tidbits_button_highlight;
        case ZombatarWidget::EYEWEAR:
            return addonZombatarImages.zombatar_eyewear_button_highlight;
        case ZombatarWidget::CLOTHES:
            return addonZombatarImages.zombatar_clothes_button_highlight;
        case ZombatarWidget::ACCESSORY:
            return addonZombatarImages.zombatar_accessory_button_highlight;
        case ZombatarWidget::HAT:
            return addonZombatarImages.zombatar_hats_button_highlight;
        case ZombatarWidget::BACKGROUND:
            return addonZombatarImages.zombatar_backdrops_button_highlight;
        default:
            return nullptr;
    }
}

Sexy::Image *ZombatarWidget_GetHairImageByIndex(int index) {
    switch (index) {
        case 0:
            return addonZombatarImages.zombatar_hair_1;
        case 1:
            return addonZombatarImages.zombatar_hair_2;
        case 2:
            return addonZombatarImages.zombatar_hair_3;
        case 3:
            return addonZombatarImages.zombatar_hair_4;
        case 4:
            return addonZombatarImages.zombatar_hair_5;
        case 5:
            return addonZombatarImages.zombatar_hair_6;
        case 6:
            return addonZombatarImages.zombatar_hair_7;
        case 7:
            return addonZombatarImages.zombatar_hair_8;
        case 8:
            return addonZombatarImages.zombatar_hair_9;
        case 9:
            return addonZombatarImages.zombatar_hair_10;
        case 10:
            return addonZombatarImages.zombatar_hair_11;
        case 11:
            return addonZombatarImages.zombatar_hair_12;
        case 12:
            return addonZombatarImages.zombatar_hair_13;
        case 13:
            return addonZombatarImages.zombatar_hair_14;
        case 14:
            return addonZombatarImages.zombatar_hair_15;
        case 15:
            return addonZombatarImages.zombatar_hair_16;
        default:
            return nullptr;
    }
}

Sexy::Image *ZombatarWidget_GetHairMaskImageByIndex(int index) {
    switch (index) {
        case 0:
            return addonZombatarImages.zombatar_hair_1_mask;
        case 1:
            return addonZombatarImages.zombatar_hair_2_mask;
        case 10:
            return addonZombatarImages.zombatar_hair_11_mask;
        case 11:
            return addonZombatarImages.zombatar_hair_12_mask;
        case 12:
            return addonZombatarImages.zombatar_hair_13_mask;
        case 13:
            return addonZombatarImages.zombatar_hair_14_mask;
        case 14:
            return addonZombatarImages.zombatar_hair_15_mask;
        default:
            return nullptr;
    }
}

Sexy::Image *ZombatarWidget_GetFHairImageByIndex(int index) {
    switch (index) {
        case 0:
            return addonZombatarImages.zombatar_facialhair_1;
        case 1:
            return addonZombatarImages.zombatar_facialhair_2;
        case 2:
            return addonZombatarImages.zombatar_facialhair_3;
        case 3:
            return addonZombatarImages.zombatar_facialhair_4;
        case 4:
            return addonZombatarImages.zombatar_facialhair_5;
        case 5:
            return addonZombatarImages.zombatar_facialhair_6;
        case 6:
            return addonZombatarImages.zombatar_facialhair_7;
        case 7:
            return addonZombatarImages.zombatar_facialhair_8;
        case 8:
            return addonZombatarImages.zombatar_facialhair_9;
        case 9:
            return addonZombatarImages.zombatar_facialhair_10;
        case 10:
            return addonZombatarImages.zombatar_facialhair_11;
        case 11:
            return addonZombatarImages.zombatar_facialhair_12;
        case 12:
            return addonZombatarImages.zombatar_facialhair_13;
        case 13:
            return addonZombatarImages.zombatar_facialhair_14;
        case 14:
            return addonZombatarImages.zombatar_facialhair_15;
        case 15:
            return addonZombatarImages.zombatar_facialhair_16;
        case 16:
            return addonZombatarImages.zombatar_facialhair_17;
        case 17:
            return addonZombatarImages.zombatar_facialhair_18;
        case 18:
            return addonZombatarImages.zombatar_facialhair_19;
        case 19:
            return addonZombatarImages.zombatar_facialhair_20;
        case 20:
            return addonZombatarImages.zombatar_facialhair_21;
        case 21:
            return addonZombatarImages.zombatar_facialhair_22;
        case 22:
            return addonZombatarImages.zombatar_facialhair_23;
        case 23:
            return addonZombatarImages.zombatar_facialhair_24;
        default:
            return nullptr;
    }
}

Sexy::Image *ZombatarWidget_GetFHairMaskImageByIndex(int index) {
    switch (index) {
        case 0:
            return addonZombatarImages.zombatar_facialhair_1_mask;
        case 3:
            return addonZombatarImages.zombatar_facialhair_4_mask;
        case 7:
            return addonZombatarImages.zombatar_facialhair_8_mask;
        case 8:
            return addonZombatarImages.zombatar_facialhair_9_mask;
        case 9:
            return addonZombatarImages.zombatar_facialhair_10_mask;
        case 10:
            return addonZombatarImages.zombatar_facialhair_11_mask;
        case 11:
            return addonZombatarImages.zombatar_facialhair_12_mask;
        case 13:
            return addonZombatarImages.zombatar_facialhair_14_mask;
        case 14:
            return addonZombatarImages.zombatar_facialhair_15_mask;
        case 15:
            return addonZombatarImages.zombatar_facialhair_16_mask;
        case 17:
            return addonZombatarImages.zombatar_facialhair_18_mask;
        case 20:
            return addonZombatarImages.zombatar_facialhair_21_mask;
        case 21:
            return addonZombatarImages.zombatar_facialhair_22_mask;
        case 22:
            return addonZombatarImages.zombatar_facialhair_23_mask;
        case 23:
            return addonZombatarImages.zombatar_facialhair_24_mask;
        default:
            return nullptr;
    }
}

Sexy::Image *ZombatarWidget_GetTidBitImageByIndex(int index) {
    switch (index) {
        case 0:
            return addonZombatarImages.zombatar_tidbits_1;
        case 1:
            return addonZombatarImages.zombatar_tidbits_2;
        case 2:
            return addonZombatarImages.zombatar_tidbits_3;
        case 3:
            return addonZombatarImages.zombatar_tidbits_4;
        case 4:
            return addonZombatarImages.zombatar_tidbits_5;
        case 5:
            return addonZombatarImages.zombatar_tidbits_6;
        case 6:
            return addonZombatarImages.zombatar_tidbits_7;
        case 7:
            return addonZombatarImages.zombatar_tidbits_8;
        case 8:
            return addonZombatarImages.zombatar_tidbits_9;
        case 9:
            return addonZombatarImages.zombatar_tidbits_10;
        case 10:
            return addonZombatarImages.zombatar_tidbits_11;
        case 11:
            return addonZombatarImages.zombatar_tidbits_12;
        case 12:
            return addonZombatarImages.zombatar_tidbits_13;
        case 13:
            return addonZombatarImages.zombatar_tidbits_14;
        default:
            return nullptr;
    }
}

Sexy::Image *ZombatarWidget_GetEyeWearImageByIndex(int index) {
    switch (index) {
        case 0:
            return addonZombatarImages.zombatar_eyewear_1;
        case 1:
            return addonZombatarImages.zombatar_eyewear_2;
        case 2:
            return addonZombatarImages.zombatar_eyewear_3;
        case 3:
            return addonZombatarImages.zombatar_eyewear_4;
        case 4:
            return addonZombatarImages.zombatar_eyewear_5;
        case 5:
            return addonZombatarImages.zombatar_eyewear_6;
        case 6:
            return addonZombatarImages.zombatar_eyewear_7;
        case 7:
            return addonZombatarImages.zombatar_eyewear_8;
        case 8:
            return addonZombatarImages.zombatar_eyewear_9;
        case 9:
            return addonZombatarImages.zombatar_eyewear_10;
        case 10:
            return addonZombatarImages.zombatar_eyewear_11;
        case 11:
            return addonZombatarImages.zombatar_eyewear_12;
        case 12:
            return addonZombatarImages.zombatar_eyewear_13;
        case 13:
            return addonZombatarImages.zombatar_eyewear_14;
        case 14:
            return addonZombatarImages.zombatar_eyewear_15;
        case 15:
            return addonZombatarImages.zombatar_eyewear_16;
        default:
            return nullptr;
    }
}

Sexy::Image *ZombatarWidget_GetEyeWearMaskImageByIndex(int index) {
    switch (index) {
        case 0:
            return addonZombatarImages.zombatar_eyewear_1_mask;
        case 1:
            return addonZombatarImages.zombatar_eyewear_2_mask;
        case 2:
            return addonZombatarImages.zombatar_eyewear_3_mask;
        case 3:
            return addonZombatarImages.zombatar_eyewear_4_mask;
        case 4:
            return addonZombatarImages.zombatar_eyewear_5_mask;
        case 5:
            return addonZombatarImages.zombatar_eyewear_6_mask;
        case 6:
            return addonZombatarImages.zombatar_eyewear_7_mask;
        case 7:
            return addonZombatarImages.zombatar_eyewear_8_mask;
        case 8:
            return addonZombatarImages.zombatar_eyewear_9_mask;
        case 9:
            return addonZombatarImages.zombatar_eyewear_10_mask;
        case 10:
            return addonZombatarImages.zombatar_eyewear_11_mask;
        case 11:
            return addonZombatarImages.zombatar_eyewear_12_mask;
        default:
            return nullptr;
    }
}

Sexy::Image *ZombatarWidget_GetClothImageByIndex(int index) {
    switch (index) {
        case 0:
            return addonZombatarImages.zombatar_clothes_1;
        case 1:
            return addonZombatarImages.zombatar_clothes_2;
        case 2:
            return addonZombatarImages.zombatar_clothes_3;
        case 3:
            return addonZombatarImages.zombatar_clothes_4;
        case 4:
            return addonZombatarImages.zombatar_clothes_5;
        case 5:
            return addonZombatarImages.zombatar_clothes_6;
        case 6:
            return addonZombatarImages.zombatar_clothes_7;
        case 7:
            return addonZombatarImages.zombatar_clothes_8;
        case 8:
            return addonZombatarImages.zombatar_clothes_9;
        case 9:
            return addonZombatarImages.zombatar_clothes_10;
        case 10:
            return addonZombatarImages.zombatar_clothes_11;
        case 11:
            return addonZombatarImages.zombatar_clothes_12;
        default:
            return nullptr;
    }
}

Sexy::Image *ZombatarWidget_GetAccessoryImageByIndex(int index) {
    switch (index) {
        case 0:
            return addonZombatarImages.zombatar_accessory_1;
        case 1:
            return addonZombatarImages.zombatar_accessory_2;
        case 2:
            return addonZombatarImages.zombatar_accessory_3;
        case 3:
            return addonZombatarImages.zombatar_accessory_4;
        case 4:
            return addonZombatarImages.zombatar_accessory_5;
        case 5:
            return addonZombatarImages.zombatar_accessory_6;
        case 6:
            return addonZombatarImages.zombatar_accessory_8;
        case 7:
            return addonZombatarImages.zombatar_accessory_9;
        case 8:
            return addonZombatarImages.zombatar_accessory_10;
        case 9:
            return addonZombatarImages.zombatar_accessory_11;
        case 10:
            return addonZombatarImages.zombatar_accessory_12;
        case 11:
            return addonZombatarImages.zombatar_accessory_13;
        case 12:
            return addonZombatarImages.zombatar_accessory_14;
        case 13:
            return addonZombatarImages.zombatar_accessory_15;
        case 14:
            return addonZombatarImages.zombatar_accessory_16;
        default:
            return nullptr;
    }
}

Sexy::Image *ZombatarWidget_GetHatImageByIndex(int index) {
    switch (index) {
        case 0:
            return addonZombatarImages.zombatar_hats_1;
        case 1:
            return addonZombatarImages.zombatar_hats_2;
        case 2:
            return addonZombatarImages.zombatar_hats_3;
        case 3:
            return addonZombatarImages.zombatar_hats_4;
        case 4:
            return addonZombatarImages.zombatar_hats_5;
        case 5:
            return addonZombatarImages.zombatar_hats_6;
        case 6:
            return addonZombatarImages.zombatar_hats_7;
        case 7:
            return addonZombatarImages.zombatar_hats_8;
        case 8:
            return addonZombatarImages.zombatar_hats_9;
        case 9:
            return addonZombatarImages.zombatar_hats_10;
        case 10:
            return addonZombatarImages.zombatar_hats_11;
        case 11:
            return addonZombatarImages.zombatar_hats_12;
        case 12:
            return addonZombatarImages.zombatar_hats_13;
        case 13:
            return addonZombatarImages.zombatar_hats_14;
        default:
            return nullptr;
    }
}

Sexy::Image *ZombatarWidget_GetHatMaskImageByIndex(int index) {
    switch (index) {
        case 0:
            return addonZombatarImages.zombatar_hats_1_mask;
        case 2:
            return addonZombatarImages.zombatar_hats_3_mask;
        case 5:
            return addonZombatarImages.zombatar_hats_6_mask;
        case 6:
            return addonZombatarImages.zombatar_hats_7_mask;
        case 7:
            return addonZombatarImages.zombatar_hats_8_mask;
        case 8:
            return addonZombatarImages.zombatar_hats_9_mask;
        case 10:
            return addonZombatarImages.zombatar_hats_11_mask;
        default:
            return nullptr;
    }
}

Sexy::Image *ZombatarWidget_GetBackgroundImageByIndex(int index) {
    switch (index) {
        case 0:
            return addonZombatarImages.zombatar_background_blank;
        case 1:
            return addonZombatarImages.zombatar_background_hood;
        case 2:
            return addonZombatarImages.zombatar_background_hood_blue;
        case 3:
            return addonZombatarImages.zombatar_background_hood_brown;
        case 4:
            return addonZombatarImages.zombatar_background_hood_yellow;
        case 5:
            return addonZombatarImages.zombatar_background_crazydave;
        case 6:
            return addonZombatarImages.zombatar_background_crazydave_night;
        case 7:
            return addonZombatarImages.zombatar_background_menu_dos;
        case 8:
            return addonZombatarImages.zombatar_background_menu;
        case 9:
            return addonZombatarImages.zombatar_background_sky_day;
        case 10:
            return addonZombatarImages.zombatar_background_sky_night;
        case 11:
            return addonZombatarImages.zombatar_background_mausoleum;
        case 12:
            return addonZombatarImages.zombatar_background_day_RV;
        case 13:
            return addonZombatarImages.zombatar_background_night_RV;
        case 14:
            return addonZombatarImages.zombatar_background_pool_sunshade;
        case 15:
            return addonZombatarImages.zombatar_background_fog_sunshade;
        case 16:
            return addonZombatarImages.zombatar_background_roof;
        case 17:
            return addonZombatarImages.zombatar_background_roof_distant;
        case 18:
            return addonZombatarImages.zombatar_background_moon;
        case 19:
            return addonZombatarImages.zombatar_background_moon_distant;
        case 20:
            return addonZombatarImages.zombatar_background_aquarium;
        case 21:
            return addonZombatarImages.zombatar_background_garden_moon;
        case 22:
            return addonZombatarImages.zombatar_background_garden_mushrooms;
        case 23:
            return addonZombatarImages.zombatar_background_garden_hd;
        case 24:
            return addonZombatarImages.zombatar_background_sky_purple;
        case 25:
            return addonZombatarImages.zombatar_background_7;
        case 26:
            return addonZombatarImages.zombatar_background_8;
        case 27:
            return addonZombatarImages.zombatar_background_9;
        case 28:
            return addonZombatarImages.zombatar_background_10;
        case 29:
            return addonZombatarImages.zombatar_background_11;
        case 30:
            return addonZombatarImages.zombatar_background_11_1;
        case 31:
            return addonZombatarImages.zombatar_background_12;
        case 32:
            return addonZombatarImages.zombatar_background_12_1;
        case 33:
            return addonZombatarImages.zombatar_background_13;
        case 34:
            return addonZombatarImages.zombatar_background_13_1;
        case 35:
            return addonZombatarImages.zombatar_background_14;
        case 36:
            return addonZombatarImages.zombatar_background_14_1;
        case 37:
            return addonZombatarImages.zombatar_background_15;
        case 38:
            return addonZombatarImages.zombatar_background_15_1;
        case 39:
            return addonZombatarImages.zombatar_background_16;
        case 40:
            return addonZombatarImages.zombatar_background_16_1;
        case 41:
            return addonZombatarImages.zombatar_background_17;
        case 42:
            return addonZombatarImages.zombatar_background_17_1;
        case 43:
            return addonZombatarImages.zombatar_background_bej3_bridge_shroom_castles;
        case 44:
            return addonZombatarImages.zombatar_background_bej3_canyon_wall;
        case 45:
            return addonZombatarImages.zombatar_background_bej3_crystal_mountain_peak;
        case 46:
            return addonZombatarImages.zombatar_background_bej3_dark_cave_thing;
        case 47:
            return addonZombatarImages.zombatar_background_bej3_desert_pyramids_sunset;
        case 48:
            return addonZombatarImages.zombatar_background_bej3_fairy_cave_village;
        case 49:
            return addonZombatarImages.zombatar_background_bej3_floating_rock_city;
        case 50:
            return addonZombatarImages.zombatar_background_bej3_horse_forset_tree;
        case 51:
            return addonZombatarImages.zombatar_background_bej3_jungle_ruins_path;
        case 52:
            return addonZombatarImages.zombatar_background_bej3_lantern_plants_world;
        case 53:
            return addonZombatarImages.zombatar_background_bej3_lightning;
        case 54:
            return addonZombatarImages.zombatar_background_bej3_lion_tower_cascade;
        case 55:
            return addonZombatarImages.zombatar_background_bej3_pointy_ice_path;
        case 56:
            return addonZombatarImages.zombatar_background_bej3_pointy_ice_path_purple;
        case 57:
            return addonZombatarImages.zombatar_background_bej3_rock_city_lake;
        case 58:
            return addonZombatarImages.zombatar_background_bej3_snowy_cliffs_castle;
        case 59:
            return addonZombatarImages.zombatar_background_bej3_treehouse_waterfall;
        case 60:
            return addonZombatarImages.zombatar_background_bej3_tube_forest_night;
        case 61:
            return addonZombatarImages.zombatar_background_bej3_water_bubble_city;
        case 62:
            return addonZombatarImages.zombatar_background_bej3_water_fall_cliff;
        case 63:
            return addonZombatarImages.zombatar_background_bejblitz_6;
        case 64:
            return addonZombatarImages.zombatar_background_bejblitz_8;
        case 65:
            return addonZombatarImages.zombatar_background_bejblitz_main_menu;
        case 66:
            return addonZombatarImages.zombatar_background_peggle_bunches;
        case 67:
            return addonZombatarImages.zombatar_background_peggle_fever;
        case 68:
            return addonZombatarImages.zombatar_background_peggle_level1;
        case 69:
            return addonZombatarImages.zombatar_background_peggle_level4;
        case 70:
            return addonZombatarImages.zombatar_background_peggle_level5;
        case 71:
            return addonZombatarImages.zombatar_background_peggle_menu;
        case 72:
            return addonZombatarImages.zombatar_background_peggle_nights_bjorn3;
        case 73:
            return addonZombatarImages.zombatar_background_peggle_nights_bjorn4;
        case 74:
            return addonZombatarImages.zombatar_background_peggle_nights_claude5;
        case 75:
            return addonZombatarImages.zombatar_background_peggle_nights_kalah1;
        case 76:
            return addonZombatarImages.zombatar_background_peggle_nights_kalah4;
        case 77:
            return addonZombatarImages.zombatar_background_peggle_nights_master5;
        case 78:
            return addonZombatarImages.zombatar_background_peggle_nights_renfield5;
        case 79:
            return addonZombatarImages.zombatar_background_peggle_nights_tut5;
        case 80:
            return addonZombatarImages.zombatar_background_peggle_nights_warren3;
        case 81:
            return addonZombatarImages.zombatar_background_peggle_paperclips;
        case 82:
            return addonZombatarImages.zombatar_background_peggle_waves;
        default:
            return nullptr;
    }
}

int ZombatarWidget_GetTidBitImageOffsetXByIndex(int index) {
    switch (index) {
        case 0:
            return 35;
        case 1:
            return 35;
        case 2:
            return 55;
        case 3:
            return 38;
        case 4:
            return 40;
        case 5:
            return 34;
        case 6:
            return 33;
        case 7:
            return 40;
        case 8:
            return 26;
        case 9:
            return 44;
        case 10:
            return 43;
        case 11:
            return 103;
        case 12:
            return 106;
        case 13:
            return 136;
        default:
            return 0;
    }
}

int ZombatarWidget_GetTidBitImageOffsetYByIndex(int index) {
    switch (index) {
        case 0:
            return 76;
        case 1:
            return 76;
        case 2:
            return 133;
        case 3:
            return 74;
        case 4:
            return 70;
        case 5:
            return 79;
        case 6:
            return 86;
        case 7:
            return 66;
        case 8:
            return 91;
        case 9:
            return 86;
        case 10:
            return 84;
        case 11:
            return 110;
        case 12:
            return 60;
        case 13:
            return 137;
        default:
            return 0;
    }
}

int ZombatarWidget_GetEyeWearImageOffsetXByIndex(int index) {
    switch (index) {
        case 0:
            return 34;
        case 1:
            return 38;
        case 2:
            return 34;
        case 3:
            return 34;
        case 4:
            return 37;
        case 5:
            return 36;
        case 6:
            return 61;
        case 7:
            return 39;
        case 8:
            return 44;
        case 9:
            return 37;
        case 10:
            return 37;
        case 11:
            return 46;
        case 12:
            return 36;
        case 13:
            return 41;
        case 14:
            return 50;
        case 15:
            return 41;
        default:
            return 0;
    }
}

int ZombatarWidget_GetEyeWearImageOffsetYByIndex(int index) {
    switch (index) {
        case 0:
            return 87;
        case 1:
            return 101;
        case 2:
            return 84;
        case 3:
            return 94;
        case 4:
            return 91;
        case 5:
            return 92;
        case 6:
            return 107;
        case 7:
            return 84;
        case 8:
            return 120;
        case 9:
            return 89;
        case 10:
            return 80;
        case 11:
            return 113;
        case 12:
            return 97;
        case 13:
            return 77;
        case 14:
            return 78;
        case 15:
            return 78;
        default:
            return 0;
    }
}

int ZombatarWidget_GetAccessoryImageOffsetXByIndex(int index) {
    switch (index) {
        case 0:
            return 124;
        case 1:
            return 130;
        case 2:
            return 103;
        case 3:
            return 157;
        case 4:
            return 158;
        case 5:
            return 158;
        case 6:
            return 126;
        case 7:
            return 144;
        case 8:
            return 74;
        case 9:
            return 52;
        case 10:
            return 163;
        case 11:
            return 94;
        case 12:
            return 83;
        case 13:
            return 160;
        case 14:
            return 16;
        default:
            return 0;
    }
}

int ZombatarWidget_GetAccessoryImageOffsetYByIndex(int index) {
    switch (index) {
        case 0:
            return 132;
        case 1:
            return 132;
        case 2:
            return 135;
        case 3:
            return 115;
        case 4:
            return 120;
        case 5:
            return 120;
        case 6:
            return 132;
        case 7:
            return 80;
        case 8:
            return 141;
        case 9:
            return 119;
        case 10:
            return 110;
        case 11:
            return 157;
        case 12:
            return 174;
        case 13:
            return 84;
        case 14:
            return 48;
        default:
            return 0;
    }
}

int ZombatarWidget_GetFHairImageOffsetXByIndex(int index) {
    switch (index) {
        case 0:
            return 42;
        case 1:
            return 61;
        case 2:
            return 54;
        case 3:
            return 46;
        case 4:
            return 85;
        case 5:
            return 59;
        case 6:
            return 16;
        case 7:
            return 54;
        case 8:
            return 51;
        case 9:
            return 54;
        case 10:
            return 52;
        case 11:
            return 35;
        case 12:
            return 52;
        case 13:
            return 23;
        case 14:
            return 76;
        case 15:
            return 76;
        case 16:
            return 52;
        case 17:
            return 71;
        case 18:
            return 55;
        case 19:
            return 137;
        case 20:
            return 141;
        case 21:
            return 18;
        case 22:
            return 71;
        case 23:
            return 46;
        default:
            return 0;
    }
}

int ZombatarWidget_GetFHairImageOffsetYByIndex(int index) {
    switch (index) {
        case 0:
            return 129;
        case 1:
            return 132;
        case 2:
            return 132;
        case 3:
            return 126;
        case 4:
            return 174;
        case 5:
            return 135;
        case 6:
            return 128;
        case 7:
            return 125;
        case 8:
            return 126;
        case 9:
            return 134;
        case 10:
            return 105;
        case 11:
            return 125;
        case 12:
            return 131;
        case 13:
            return 122;
        case 14:
            return 174;
        case 15:
            return 168;
        case 16:
            return 132;
        case 17:
            return 115;
        case 18:
            return 110;
        case 19:
            return 96;
        case 20:
            return 102;
        case 21:
            return 104;
        case 22:
            return 174;
        case 23:
            return 129;
        default:
            return 0;
    }
}

int ZombatarWidget_GetFHairMaskImageOffsetXByIndex(int index) {
    switch (index) {
        case 3:
            return 3;
        case 7:
            return 1;
        case 10:
            return 1;
        case 11:
            return 9;
        case 13:
            return 2;
        case 14:
            return 2;
        case 17:
            return 2;
        case 21:
            return 4;
        case 22:
            return 1;
        case 23:
            return 4;
        default:
            return 0;
    }
}

int ZombatarWidget_GetFHairMaskImageOffsetYByIndex(int index) {
    switch (index) {
        case 3:
            return 1;
        case 9:
            return 1;
        case 10:
            return 3;
        case 11:
            return 1;
        case 13:
            return 1;
        case 17:
            return 2;
        case 21:
            return 3;
        default:
            return 0;
    }
}

int ZombatarWidget_GetHairImageOffsetXByIndex(int index) {
    switch (index) {
        case 0:
            return 28;
        case 1:
            return 29;
        case 2:
            return 28;
        case 3:
            return 35;
        case 4:
            return 44;
        case 5:
            return 47;
        case 6:
            return 61;
        case 7:
            return 33;
        case 8:
            return 154;
        case 9:
            return 27;
        case 10:
            return 30;
        case 11:
            return 60;
        case 12:
            return 39;
        case 13:
            return 9;
        case 14:
            return 55;
        case 15:
            return 31;
        default:
            return 0;
    }
}

int ZombatarWidget_GetHairImageOffsetYByIndex(int index) {
    switch (index) {
        case 0:
            return 0;
        case 1:
            return 31;
        case 2:
            return 36;
        case 3:
            return 17;
        case 4:
            return 45;
        case 5:
            return 16;
        case 6:
            return 26;
        case 7:
            return 17;
        case 8:
            return 66;
        case 9:
            return 37;
        case 10:
            return 22;
        case 11:
            return -5;
        case 12:
            return 16;
        case 13:
            return -2;
        case 14:
            return 4;
        case 15:
            return 23;
        default:
            return 0;
    }
}

int ZombatarWidget_GetHairMaskImageOffsetXByIndex(int index) {
    switch (index) {
        case 0:
            return 10;
        case 1:
            return 2;
        case 10:
            return 2;
        case 11:
            return 2;
        case 12:
            return 2;
        case 13:
            return 2;
        case 14:
            return -1;
        default:
            return 0;
    }
}

int ZombatarWidget_GetHairMaskImageOffsetYByIndex(int index) {
    switch (index) {
        case 0:
            return 2;
        case 1:
            return 3;
        case 10:
            return 2;
        case 11:
            return 2;
        case 12:
            return 1;
        case 13:
            return 5;
        default:
            return 0;
    }
}

int ZombatarWidget_GetClothImageOffsetXByIndex(int index) {
    switch (index) {
        case 0:
            return -addonZombatarImages.zombatar_clothes_1->mWidth + 2;
        case 1:
            return -addonZombatarImages.zombatar_clothes_2->mWidth + 11;
        case 2:
            return -addonZombatarImages.zombatar_clothes_3->mWidth + 1;
        case 3:
            return -addonZombatarImages.zombatar_clothes_4->mWidth + 1;
        case 4:
            return -addonZombatarImages.zombatar_clothes_5->mWidth + 0;
        case 5:
            return -addonZombatarImages.zombatar_clothes_6->mWidth - 20;
        case 6:
            return -addonZombatarImages.zombatar_clothes_7->mWidth + 3;
        case 7:
            return -addonZombatarImages.zombatar_clothes_8->mWidth + 2;
        case 8:
            return -addonZombatarImages.zombatar_clothes_9->mWidth + 10;
        case 9:
            return -addonZombatarImages.zombatar_clothes_10->mWidth + 3;
        case 10:
            return -addonZombatarImages.zombatar_clothes_11->mWidth + 1;
        case 11:
            return -addonZombatarImages.zombatar_clothes_12->mWidth + 3;
        default:
            return 0;
    }
}

int ZombatarWidget_GetClothImageOffsetYByIndex(int index) {
    switch (index) {
        case 0:
            return -addonZombatarImages.zombatar_clothes_1->mHeight + 0;
        case 1:
            return -addonZombatarImages.zombatar_clothes_2->mHeight + 6;
        case 2:
            return -addonZombatarImages.zombatar_clothes_3->mHeight + 0;
        case 3:
            return -addonZombatarImages.zombatar_clothes_4->mHeight + 2;
        case 4:
            return -addonZombatarImages.zombatar_clothes_5->mHeight + 2;
        case 5:
            return -addonZombatarImages.zombatar_clothes_6->mHeight - 24;
        case 6:
            return -addonZombatarImages.zombatar_clothes_7->mHeight + 2;
        case 7:
            return -addonZombatarImages.zombatar_clothes_8->mHeight + 4;
        case 8:
            return -addonZombatarImages.zombatar_clothes_9->mHeight + 3;
        case 9:
            return -addonZombatarImages.zombatar_clothes_10->mHeight + 2;
        case 10:
            return -addonZombatarImages.zombatar_clothes_11->mHeight + 2;
        case 11:
            return -addonZombatarImages.zombatar_clothes_12->mHeight + 3;
        default:
            return 0;
    }
}

int ZombatarWidget_GetHatImageOffsetXByIndex(int index) {
    switch (index) {
        case 0:
            return 33;
        case 1:
            return 58;
        case 2:
            return 44;
        case 3:
            return 12;
        case 4:
            return 49;
        case 5:
            return 22;
        case 6:
            return 64;
        case 7:
            return 4;
        case 8:
            return 45;
        case 9:
            return 17;
        case 10:
            return 76;
        case 11:
            return 51;
        case 12:
            return 22;
        case 13:
            return 28;
        default:
            return 0;
    }
}

int ZombatarWidget_GetHatImageOffsetYByIndex(int index) {
    switch (index) {
        case 0:
            return 5;
        case 1:
            return 14;
        case 2:
            return 25;
        case 3:
            return 12;
        case 4:
            return 20;
        case 5:
            return 5;
        case 6:
            return 21;
        case 7:
            return -1;
        case 8:
            return -1;
        case 9:
            return 56;
        case 10:
            return 10;
        case 11:
            return 16;
        case 12:
            return 0;
        case 13:
            return 6;
        default:
            return 0;
    }
}

int ZombatarWidget_GetHatMaskImageOffsetXByIndex(int index) {
    switch (index) {
        case 0:
            return 1;
        case 2:
            return 18;
        case 5:
            return 5;
        case 7:
            return -3;
        case 8:
            return -1;
        case 10:
            return 1;
        default:
            return 0;
    }
}

int ZombatarWidget_GetHatMaskImageOffsetYByIndex(int index) {
    switch (index) {
        case 0:
            return 1;
        case 5:
            return -2;
        case 6:
            return 17;
        case 7:
            return -3;
        case 8:
            return -2;
        case 10:
            return 16;
        default:
            return 0;
    }
}

bool ZombatarWidget_AccessoryIsColorized(int tab, int accessory) {
    switch (tab) {
        case ZombatarWidget::HAIR:
            return accessory != 2;
        case ZombatarWidget::FHAIR:
            return true;
        case ZombatarWidget::TIDBIT:
            return accessory == 0 || accessory == 1 || accessory == 2 || accessory == 9 || accessory == 10 || accessory == 11;
        case ZombatarWidget::EYEWEAR:
            return accessory <= 11;
        case ZombatarWidget::CLOTHES:
            return false;
        case ZombatarWidget::ACCESSORY:
            return accessory == 7 || accessory == 9 || accessory == 11 || accessory == 12;
        case ZombatarWidget::HAT:
            return accessory != 12;
        case ZombatarWidget::BACKGROUND:
            return accessory == 0;
        default:
            return false;
    }
}

ZombatarWidget::ZombatarWidget(LawnApp *theApp) {
    TestMenuWidget_TestMenuWidget(this);
    theApp->LoadZombatarResources();
    theApp->Load("DelayLoad_Almanac");
    mApp = theApp;
    Sexy::ButtonListener *mZombatarListener = (Sexy::ButtonListener *)operator new(sizeof(Sexy::ButtonListener));
    mButtonListener = mZombatarListener;
    Sexy::ButtonListenerVTable *mVTable = (Sexy::ButtonListenerVTable *)operator new(sizeof(Sexy::ButtonListenerVTable));
    mZombatarListener->vTable = mVTable;
    //    mVTable->ButtonPress = (void *) ZombatarWidget_ButtonPress;
    mVTable->ButtonPress2 = (void *)ZombatarWidget_ButtonPress;
    mVTable->ButtonDepress = (void *)ZombatarWidget_ButtonDepress;
    mVTable->ButtonMouseEnter = (void *)Sexy_ButtonListener_ButtonMouseEnter;
    mVTable->ButtonMouseMove = (void *)Sexy_ButtonListener_ButtonMouseMove;
    mVTable->ButtonMouseLeave = (void *)Sexy_ButtonListener_ButtonMouseLeave;
    mVTable->ButtonDownTick = (void *)Sexy_ButtonListener_ButtonDownTick;

    int holder[1];
    StrFormat(holder, "[CLOSE]");
    Sexy::GameButton *backButton = MakeButton(1000, mZombatarListener, this, (SexyString &)holder);
    StringDelete(holder);
    backButton->Resize(471, 628, addonZombatarImages.zombatar_mainmenuback_highlight->mWidth, addonZombatarImages.zombatar_mainmenuback_highlight->mHeight);
    AddWidget(backButton);
    backButton->mDrawStoneButton = false;
    backButton->mButtonImage = *IMAGE_BLANK;
    backButton->mDownImage = addonZombatarImages.zombatar_mainmenuback_highlight;
    backButton->mOverImage = addonZombatarImages.zombatar_mainmenuback_highlight;
    mBackButton = backButton;

    int holder1[1];
    StrFormat(holder1, "[OK]");
    Sexy::GameButton *finishButton = MakeButton(1001, mZombatarListener, nullptr, (SexyString &)holder1);
    StringDelete(holder1);
    finishButton->Resize(160 + 523, 565, addonZombatarImages.zombatar_finished_button->mWidth, addonZombatarImages.zombatar_finished_button->mHeight);
    AddWidget(finishButton);
    finishButton->mDrawStoneButton = false;
    finishButton->mButtonImage = addonZombatarImages.zombatar_finished_button;
    finishButton->mDownImage = addonZombatarImages.zombatar_finished_button_highlight;
    finishButton->mOverImage = addonZombatarImages.zombatar_finished_button_highlight;
    mFinishButton = finishButton;

    int holder2[1];
    StrFormat(holder2, "[OK]");
    Sexy::GameButton *viewPortraitButton = MakeButton(1002, mZombatarListener, nullptr, (SexyString &)holder2);
    StringDelete(holder2);
    viewPortraitButton->Resize(160 + 75, 565, addonZombatarImages.zombatar_view_button->mWidth, addonZombatarImages.zombatar_view_button->mHeight);
    AddWidget((Widget*)viewPortraitButton);
    viewPortraitButton->mDrawStoneButton = false;
    viewPortraitButton->mButtonImage = addonZombatarImages.zombatar_view_button;
    viewPortraitButton->mDownImage = addonZombatarImages.zombatar_view_button_highlight;
    viewPortraitButton->mOverImage = addonZombatarImages.zombatar_view_button_highlight;
    mViewPortraitButton = viewPortraitButton;

    int holder3[1];
    StrFormat(holder3, "[ZOMBATAR_NEW_BUTTON]");
    Sexy::GameButton *newButton = MakeButton(1003, mZombatarListener, nullptr, (SexyString &)holder3);
    StringDelete(holder3);
    newButton->Resize(578, 490, 170, 50);
    AddWidget((Widget*)newButton);
    mNewButton = newButton;

    int holder4[1];
    StrFormat(holder4, "[ZOMBATAR_DELETE_BUTTON]");
    Sexy::GameButton *deleteButton = MakeButton(1004, mZombatarListener, nullptr, (SexyString &)holder4);
    StringDelete(holder4);
    deleteButton->Resize(314, 490, 170, 50);
    AddWidget((Widget*)deleteButton);
    mDeleteButton = deleteButton;

    auto *aZombie = new Zombie;
    aZombie->mBoard = nullptr;
    aZombie->ZombieInitialize(0, ZombieType::ZOMBIE_FLAG, false, nullptr, -3, true);
    Reanimation *aBodyReanim = aZombie->mApp->ReanimationGet(aZombie->mBodyReanimID);
    ReanimatorTrackInstance *aHeadTrackInstance = aBodyReanim->GetTrackInstanceByName("anim_head1");
    aHeadTrackInstance->mImageOverride = *IMAGE_BLANK;

    Reanimation *aZombatarHeadReanim = theApp->AddReanimation(0, 0, 0, ReanimationType::REANIM_ZOMBATAR_HEAD);
    aZombatarHeadReanim->PlayReanim("anim_head_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0);
    aZombatarHeadReanim->AssignRenderGroupToTrack("anim_hair", -1);
    aZombie->mBossFireBallReanimID = aZombie->mApp->ReanimationGetID(aZombatarHeadReanim);
    AttachEffect *attachEffect = AttachReanim(aHeadTrackInstance->mAttachmentID, aZombatarHeadReanim, 0.0f, 0.0f);
    TodScaleRotateTransformMatrix((SexyMatrix3 &)attachEffect->mOffset, -20.0, -1.0, 0.2, 1.0, 1.0);
    mZombatarReanim = aZombatarHeadReanim;
    aZombie->ReanimShowPrefix("anim_hair", -1);
    aZombie->ReanimShowPrefix("anim_head2", -1);
    aZombie->Update();
    mPreviewZombie = aZombie;

    mShowExistingZombatarPortrait = addonImages.zombatar_portrait != nullptr;
    mShowZombieTypeSelection = false;

    ZombatarWidget_SetDefault(this);
}

void TestMenuWidget_Update(ZombatarWidget *zombatarWidget) {
    zombatarWidget->mPreviewZombie->Update();
    zombatarWidget->mFinishButton->mDisabled = zombatarWidget->mShowExistingZombatarPortrait;
    zombatarWidget->mFinishButton->mBtnNoDraw = zombatarWidget->mShowExistingZombatarPortrait;
    zombatarWidget->mViewPortraitButton->mDisabled = zombatarWidget->mShowExistingZombatarPortrait || addonImages.zombatar_portrait == nullptr;
    zombatarWidget->mViewPortraitButton->mBtnNoDraw = zombatarWidget->mShowExistingZombatarPortrait || addonImages.zombatar_portrait == nullptr;
    zombatarWidget->mNewButton->mDisabled = !zombatarWidget->mShowExistingZombatarPortrait;
    zombatarWidget->mNewButton->mBtnNoDraw = !zombatarWidget->mShowExistingZombatarPortrait;
    zombatarWidget->mDeleteButton->mDisabled = !zombatarWidget->mShowExistingZombatarPortrait;
    zombatarWidget->mDeleteButton->mBtnNoDraw = !zombatarWidget->mShowExistingZombatarPortrait;
    zombatarWidget->MarkDirty();
}

void TestMenuWidget_DrawZombieSelection(ZombatarWidget *zombatarWidget, Sexy::Graphics *graphics) {
    // TODO: 做僵尸选择功能
    ZombieType types[] = {ZombieType::ZOMBIE_NORMAL,
                          ZombieType::ZOMBIE_FLAG,
                          ZombieType::ZOMBIE_TRAFFIC_CONE,
                          ZombieType::ZOMBIE_DOOR,
                          ZombieType::ZOMBIE_TRASH_BIN,
                          ZombieType::ZOMBIE_PAIL,
                          ZombieType::ZOMBIE_DUCKY_TUBE};
}

void TestMenuWidget_DrawSkin(ZombatarWidget *zombatarWidget, Sexy::Graphics *graphics) {
    for (int i = 0; i < 12; ++i) {
        int theX = 160 + 285 + (i % 9) * 30;
        int theY = 432 + i / 9 * 30;
        Color color = gZombatarSkinColor[i];
        if (zombatarWidget->mSelectedSkinColor != i) {
            color.mAlpha = 64;
        }
        Sexy_Graphics_DrawImageColorized(graphics, addonZombatarImages.zombatar_colorpicker, &color, theX, theY);
    }

    int holder[1];
    TodStringTranslate(holder, "[ZOMBATAR_START_TEXT]");
    Sexy::Rect rect = {160 + 295, 211, 250, 100};
    Sexy::Font *font = *Sexy_FONT_DWARVENTODCRAFT18_Addr;
    TodDrawStringWrapped(graphics, holder, &rect, font, &yellow, DrawStringJustification::DS_ALIGN_CENTER, false);
    StringDelete(holder);
}

void TestMenuWidget_DrawHair(ZombatarWidget *zombatarWidget, Sexy::Graphics *g) {
    Color theAlphaColor = {255, 255, 255, 64};
    for (int i = 0; i < 16; ++i) {
        int theX = 160 + 198 + (i % 6) * 73;
        int theY = 162 + i / 6 * 79;
        Sexy::Image *image = ZombatarWidget_GetHairImageByIndex(i);
        Sexy::Image *image1 = ZombatarWidget_GetHairMaskImageByIndex(i);
        int mWidth = image->mWidth;
        int mHeight = image->mHeight;
        int max = std::max(mWidth, mHeight);
        float ratio = 58.0 / max;
        if (ratio > 1.3)
            ratio = 1.3;
        float widthOffset = (58 - ratio * mWidth) / 2;
        float heightOffset = (58 - ratio * mHeight) / 2;

        if (zombatarWidget->mSelectedHair == i) {
            g->DrawImage(addonZombatarImages.zombatar_accessory_bg_highlight, theX, theY);
            if (image1 != nullptr) {
                int widthOffset2 = ZombatarWidget_GetHairMaskImageOffsetXByIndex(i);
                int heightOffset2 = ZombatarWidget_GetHairMaskImageOffsetYByIndex(i);
                TodDrawImageScaledF(g, image1, theX + 12 + widthOffset + widthOffset2 * ratio, theY + 12 + heightOffset + heightOffset2 * ratio, ratio, ratio);
            }
            TodDrawImageScaledF(g, image, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
        } else {
            Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg, &theAlphaColor, theX, theY);
            if (image1 != nullptr) {
                int widthOffset2 = ZombatarWidget_GetHairMaskImageOffsetXByIndex(i);
                int heightOffset2 = ZombatarWidget_GetHairMaskImageOffsetYByIndex(i);
                Sexy_Graphics_DrawImageColorizedScaled(g, image1, &theAlphaColor, theX + 12 + widthOffset + widthOffset2 * ratio, theY + 12 + heightOffset + heightOffset2 * ratio, ratio, ratio);
            }
            Sexy_Graphics_DrawImageColorizedScaled(g, image, &theAlphaColor, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
        }
    }
    int theX = 160 + 198 + (16 % 6) * 73;
    int theY = 162 + 16 / 6 * 79;
    Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg_none, &theAlphaColor, theX, theY);

    if (zombatarWidget->mSelectedHair == 255) {
        int holder[1];
        TodStringTranslate(holder, "[ZOMBATAR_COLOR_ITEM_NOT_CHOSEN]");
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = *Sexy_FONT_BRIANNETOD12_Addr;
        TodDrawStringWrapped(g, holder, &rect, font, &white, DrawStringJustification::DS_ALIGN_LEFT, false);
        StringDelete(holder);
    } else if (ZombatarWidget_AccessoryIsColorized(zombatarWidget->mSelectedTab, zombatarWidget->mSelectedHair)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Color color = gZombatarAccessoryColor[i];
            if (zombatarWidget->mSelectedHairColor != i) {
                color.mAlpha = 64;
            }
            Sexy_Graphics_DrawImageColorized(g, i == 17 ? addonZombatarImages.zombatar_colorpicker_none : addonZombatarImages.zombatar_colorpicker, &color, theX, theY);
        }
    } else {
        int holder[1];
        TodStringTranslate(holder, "[ZOMBATAR_COLOR_NOT_APPLICABLE]");
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = *Sexy_FONT_BRIANNETOD12_Addr;
        TodDrawStringWrapped(g, holder, &rect, font, &white, DrawStringJustification::DS_ALIGN_LEFT, false);
        StringDelete(holder);
    }
}

void TestMenuWidget_DrawFHair(ZombatarWidget *zombatarWidget, Sexy::Graphics *g) {
    if (zombatarWidget->mSelectedFHairPage == 0) {
        Color theAlphaColor = {255, 255, 255, 64};
        for (int i = 0; i < 17; ++i) {
            int theX = 160 + 198 + (i % 6) * 73;
            int theY = 162 + i / 6 * 79;
            Sexy::Image *image = ZombatarWidget_GetFHairImageByIndex(i);
            Sexy::Image *image1 = ZombatarWidget_GetFHairMaskImageByIndex(i);
            int mWidth = image->mWidth;
            int mHeight = image->mHeight;
            int max = std::max(mWidth, mHeight);
            float ratio = 58.0 / max;
            if (ratio > 1.3)
                ratio = 1.3;
            float widthOffset = (58 - ratio * mWidth) / 2;
            float heightOffset = (58 - ratio * mHeight) / 2;

            if (zombatarWidget->mSelectedFHair == i) {
                g->DrawImage(addonZombatarImages.zombatar_accessory_bg_highlight, theX, theY);
                if (image1 != nullptr) {
                    int widthOffset2 = ZombatarWidget_GetFHairMaskImageOffsetXByIndex(i);
                    int heightOffset2 = ZombatarWidget_GetFHairMaskImageOffsetYByIndex(i);
                    TodDrawImageScaledF(g, image1, theX + 12 + widthOffset + widthOffset2 * ratio, theY + 12 + heightOffset + heightOffset2 * ratio, ratio, ratio);
                }
                TodDrawImageScaledF(g, image, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
            } else {
                Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg, &theAlphaColor, theX, theY);
                if (image1 != nullptr) {
                    int widthOffset2 = ZombatarWidget_GetFHairMaskImageOffsetXByIndex(i);
                    int heightOffset2 = ZombatarWidget_GetFHairMaskImageOffsetYByIndex(i);
                    Sexy_Graphics_DrawImageColorizedScaled(g, image1, &theAlphaColor, theX + 12 + widthOffset + widthOffset2 * ratio, theY + 12 + heightOffset + heightOffset2 * ratio, ratio, ratio);
                }
                Sexy_Graphics_DrawImageColorizedScaled(g, image, &theAlphaColor, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
            }
        }
        int theX = 160 + 198 + (17 % 6) * 73;
        int theY = 162 + 17 / 6 * 79;
        Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg_none, &theAlphaColor, theX, theY);
        Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_prev_button, &theAlphaColor, 160 + 209, 436);
        g->DrawImage(addonZombatarImages.zombatar_next_button, 160 + 588, 436);
    } else if (zombatarWidget->mSelectedFHairPage == 1) {
        Color theAlphaColor = {255, 255, 255, 64};
        for (int i = 0; i < 7; ++i) {
            int theX = 160 + 198 + (i % 6) * 73;
            int theY = 162 + i / 6 * 79;
            Sexy::Image *image = ZombatarWidget_GetFHairImageByIndex(i + 17);
            Sexy::Image *image1 = ZombatarWidget_GetFHairMaskImageByIndex(i + 17);
            int mWidth = image->mWidth;
            int mHeight = image->mHeight;
            int max = std::max(mWidth, mHeight);
            float ratio = 58.0 / max;
            if (ratio > 1.3)
                ratio = 1.3;
            float widthOffset = (58 - ratio * mWidth) / 2;
            float heightOffset = (58 - ratio * mHeight) / 2;

            if (zombatarWidget->mSelectedFHair == i + 17) {
                g->DrawImage(addonZombatarImages.zombatar_accessory_bg_highlight, theX, theY);
                if (image1 != nullptr) {
                    int widthOffset2 = ZombatarWidget_GetFHairMaskImageOffsetXByIndex(i + 17);
                    int heightOffset2 = ZombatarWidget_GetFHairMaskImageOffsetYByIndex(i + 17);
                    TodDrawImageScaledF(g, image1, theX + 12 + widthOffset + widthOffset2 * ratio, theY + 12 + heightOffset + heightOffset2 * ratio, ratio, ratio);
                }
                TodDrawImageScaledF(g, image, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
            } else {
                Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg, &theAlphaColor, theX, theY);
                if (image1 != nullptr) {
                    int widthOffset2 = ZombatarWidget_GetFHairMaskImageOffsetXByIndex(i + 17);
                    int heightOffset2 = ZombatarWidget_GetFHairMaskImageOffsetYByIndex(i + 17);
                    Sexy_Graphics_DrawImageColorizedScaled(g, image1, &theAlphaColor, theX + 12 + widthOffset + widthOffset2 * ratio, theY + 12 + heightOffset + heightOffset2 * ratio, ratio, ratio);
                }
                Sexy_Graphics_DrawImageColorizedScaled(g, image, &theAlphaColor, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
            }
        }
        int theX = 160 + 198 + (7 % 6) * 73;
        int theY = 162 + 7 / 6 * 79;
        Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg_none, &theAlphaColor, theX, theY);
        g->DrawImage(addonZombatarImages.zombatar_prev_button, 160 + 209, 436);
        Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_next_button, &theAlphaColor, 160 + 588, 436);
    }

    if (zombatarWidget->mSelectedFHair == 255) {
        int holder[1];
        TodStringTranslate(holder, "[ZOMBATAR_COLOR_ITEM_NOT_CHOSEN]");
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = *Sexy_FONT_BRIANNETOD12_Addr;
        TodDrawStringWrapped(g, holder, &rect, font, &white, DrawStringJustification::DS_ALIGN_LEFT, false);
        StringDelete(holder);
    } else if (ZombatarWidget_AccessoryIsColorized(zombatarWidget->mSelectedTab, zombatarWidget->mSelectedFHair)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Color color = gZombatarAccessoryColor[i];
            if (zombatarWidget->mSelectedFHairColor != i) {
                color.mAlpha = 64;
            }
            Sexy_Graphics_DrawImageColorized(g, i == 17 ? addonZombatarImages.zombatar_colorpicker_none : addonZombatarImages.zombatar_colorpicker, &color, theX, theY);
        }
    } else {
        int holder[1];
        TodStringTranslate(holder, "[ZOMBATAR_COLOR_NOT_APPLICABLE]");
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = *Sexy_FONT_BRIANNETOD12_Addr;
        TodDrawStringWrapped(g, holder, &rect, font, &white, DrawStringJustification::DS_ALIGN_LEFT, false);
        StringDelete(holder);
    }

    int holder[1];
    StrFormat(holder, "PAGE %d/%d", zombatarWidget->mSelectedFHairPage + 1, 2);
    TodDrawString(g, holder, 160 + 410, 525, *Sexy_FONT_BRIANNETOD16_Addr, black, DrawStringJustification::DS_ALIGN_CENTER);
    StringDelete(holder);
}

void TestMenuWidget_DrawTidBit(ZombatarWidget *zombatarWidget, Sexy::Graphics *g) {
    Color theAlphaColor = {255, 255, 255, 64};
    for (int i = 0; i < 14; ++i) {
        int theX = 160 + 198 + (i % 6) * 73;
        int theY = 162 + i / 6 * 79;
        Sexy::Image *image = ZombatarWidget_GetTidBitImageByIndex(i);
        int mWidth = image->mWidth;
        int mHeight = image->mHeight;
        int max = std::max(mWidth, mHeight);
        float ratio = 58.0 / max;
        if (ratio > 1.3)
            ratio = 1.3;
        float widthOffset = (58 - ratio * mWidth) / 2;
        float heightOffset = (58 - ratio * mHeight) / 2;

        if (zombatarWidget->mSelectedTidBit == i) {
            g->DrawImage(addonZombatarImages.zombatar_accessory_bg_highlight, theX, theY);
            TodDrawImageScaledF(g, image, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
            if (i == 0) {
                TodDrawImageScaledF(g, ZombatarWidget_GetTidBitImageByIndex(2), theX + 23, theY + 44, ratio, ratio);
            }
        } else {
            Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg, &theAlphaColor, theX, theY);
            Sexy_Graphics_DrawImageColorizedScaled(g, image, &theAlphaColor, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
            if (i == 0) {
                Sexy_Graphics_DrawImageColorizedScaled(g, ZombatarWidget_GetTidBitImageByIndex(2), &theAlphaColor, theX + 23, theY + 44, ratio, ratio);
            }
        }
    }
    int theX = 160 + 198 + (14 % 6) * 73;
    int theY = 162 + 14 / 6 * 79;
    Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg_none, &theAlphaColor, theX, theY);

    if (zombatarWidget->mSelectedTidBit == 255) {
        int holder[1];
        TodStringTranslate(holder, "[ZOMBATAR_COLOR_ITEM_NOT_CHOSEN]");
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = *Sexy_FONT_BRIANNETOD12_Addr;
        TodDrawStringWrapped(g, holder, &rect, font, &white, DrawStringJustification::DS_ALIGN_LEFT, false);
        StringDelete(holder);
    } else if (ZombatarWidget_AccessoryIsColorized(zombatarWidget->mSelectedTab, zombatarWidget->mSelectedTidBit)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Color color = gZombatarAccessoryColor2[i];
            if (zombatarWidget->mSelectedTidBitColor != i) {
                color.mAlpha = 64;
            }
            Sexy_Graphics_DrawImageColorized(g, i == 17 ? addonZombatarImages.zombatar_colorpicker_none : addonZombatarImages.zombatar_colorpicker, &color, theX, theY);
        }
    } else {
        int holder[1];
        TodStringTranslate(holder, "[ZOMBATAR_COLOR_NOT_APPLICABLE]");
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = *Sexy_FONT_BRIANNETOD12_Addr;
        TodDrawStringWrapped(g, holder, &rect, font, &white, DrawStringJustification::DS_ALIGN_LEFT, false);
        StringDelete(holder);
    }
}

void TestMenuWidget_DrawEyeWear(ZombatarWidget *zombatarWidget, Sexy::Graphics *g) {
    Color theAlphaColor = {255, 255, 255, 64};
    for (int i = 0; i < 16; ++i) {
        int theX = 160 + 198 + (i % 6) * 73;
        int theY = 162 + i / 6 * 79;
        Sexy::Image *image = ZombatarWidget_GetEyeWearImageByIndex(i);
        Sexy::Image *image1 = ZombatarWidget_GetEyeWearMaskImageByIndex(i);
        int mWidth = image->mWidth;
        int mHeight = image->mHeight;
        int max = std::max(mWidth, mHeight);
        float ratio = 58.0 / max;
        if (ratio > 1.3)
            ratio = 1.3;
        float widthOffset = (58 - ratio * mWidth) / 2;
        float heightOffset = (58 - ratio * mHeight) / 2;

        if (zombatarWidget->mSelectedEyeWear == i) {
            g->DrawImage(addonZombatarImages.zombatar_accessory_bg_highlight, theX, theY);
            if (image1 != nullptr) {
                TodDrawImageScaledF(g, image1, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
            }
            TodDrawImageScaledF(g, image, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
        } else {
            if (image1 != nullptr) {
                Sexy_Graphics_DrawImageColorizedScaled(g, image1, &theAlphaColor, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
            }
            Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg, &theAlphaColor, theX, theY);
            Sexy_Graphics_DrawImageColorizedScaled(g, image, &theAlphaColor, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
        }
    }
    int theX = 160 + 198 + (16 % 6) * 73;
    int theY = 162 + 16 / 6 * 79;
    Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg_none, &theAlphaColor, theX, theY);

    if (zombatarWidget->mSelectedEyeWear == 255) {
        int holder[1];
        TodStringTranslate(holder, "[ZOMBATAR_COLOR_ITEM_NOT_CHOSEN]");
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = *Sexy_FONT_BRIANNETOD12_Addr;
        TodDrawStringWrapped(g, holder, &rect, font, &white, DrawStringJustification::DS_ALIGN_LEFT, false);
        StringDelete(holder);
    } else if (ZombatarWidget_AccessoryIsColorized(zombatarWidget->mSelectedTab, zombatarWidget->mSelectedEyeWear)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Color color = gZombatarAccessoryColor2[i];
            if (zombatarWidget->mSelectedEyeWearColor != i) {
                color.mAlpha = 64;
            }
            Sexy_Graphics_DrawImageColorized(g, i == 17 ? addonZombatarImages.zombatar_colorpicker_none : addonZombatarImages.zombatar_colorpicker, &color, theX, theY);
        }
    } else {
        int holder[1];
        TodStringTranslate(holder, "[ZOMBATAR_COLOR_NOT_APPLICABLE]");
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = *Sexy_FONT_BRIANNETOD12_Addr;
        TodDrawStringWrapped(g, holder, &rect, font, &white, DrawStringJustification::DS_ALIGN_LEFT, false);
        StringDelete(holder);
    }
}

void TestMenuWidget_DrawCloth(ZombatarWidget *zombatarWidget, Sexy::Graphics *g) {
    Color theAlphaColor = {255, 255, 255, 64};
    Color theAlphaColor2 = {255, 255, 255, 128};
    for (int i = 0; i < 12; ++i) {
        int theX = 160 + 198 + (i % 6) * 73;
        int theY = 162 + i / 6 * 79;
        Sexy::Image *image = addonZombatarImages.zombatar_zombie_blank_part;
        Sexy::Image *image1 = ZombatarWidget_GetClothImageByIndex(i);
        int mWidth = image->mWidth;
        int mHeight = image->mHeight;
        int max = std::max(mWidth, mHeight);
        float ratio = 58.0 / max;
        if (ratio > 1.3)
            ratio = 1.3;
        float widthOffset = (58 - ratio * mWidth) / 2;
        float heightOffset = (58 - ratio * mHeight) / 2;
        int offsetX = addonZombatarImages.zombatar_zombie_blank_part->mWidth + ZombatarWidget_GetClothImageOffsetXByIndex(i);
        int offsetY = addonZombatarImages.zombatar_zombie_blank_part->mHeight + ZombatarWidget_GetClothImageOffsetYByIndex(i);
        if (zombatarWidget->mSelectedCloth == i) {
            g->DrawImage(addonZombatarImages.zombatar_accessory_bg_highlight, theX, theY);
            Sexy_Graphics_DrawImageColorizedScaled(
                g,
                                                   addonZombatarImages.zombatar_zombie_blank_skin_part,
                                                   &gZombatarSkinColor[zombatarWidget->mSelectedSkinColor],
                                                   theX + 12 + widthOffset,
                                                   theY + 12 + heightOffset,
                                                   ratio,
                                                   ratio);
            TodDrawImageScaledF(g, image, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
            TodDrawImageScaledF(g, image1, theX + 12 + ratio * offsetX, theY + 12 + ratio * offsetY, ratio, ratio);
        } else {
            Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg, &theAlphaColor, theX, theY);
            Sexy_Graphics_DrawImageColorizedScaled(
                g,
                                                   addonZombatarImages.zombatar_zombie_blank_skin_part,
                                                   &gZombatarSkinColor[zombatarWidget->mSelectedSkinColor],
                                                   theX + 12 + widthOffset,
                                                   theY + 12 + heightOffset,
                                                   ratio,
                                                   ratio);
            TodDrawImageScaledF(g, image, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
            Sexy_Graphics_DrawImageColorizedScaled(g, image1, &theAlphaColor2, theX + 12 + ratio * offsetX, theY + 12 + ratio * offsetY, ratio, ratio);
        }
    }
    int theX = 160 + 198 + (12 % 6) * 73;
    int theY = 162 + 12 / 6 * 79;
    Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg_none, &theAlphaColor, theX, theY);

    if (zombatarWidget->mSelectedCloth == 255) {
        int holder[1];
        TodStringTranslate(holder, "[ZOMBATAR_COLOR_ITEM_NOT_CHOSEN]");
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = *Sexy_FONT_BRIANNETOD12_Addr;
        TodDrawStringWrapped(g, holder, &rect, font, &white, DrawStringJustification::DS_ALIGN_LEFT, false);
        StringDelete(holder);
    } else {
        int holder[1];
        TodStringTranslate(holder, "[ZOMBATAR_COLOR_NOT_APPLICABLE]");
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = *Sexy_FONT_BRIANNETOD12_Addr;
        TodDrawStringWrapped(g, holder, &rect, font, &white, DrawStringJustification::DS_ALIGN_LEFT, false);
        StringDelete(holder);
    }
}

void TestMenuWidget_DrawAccessory(ZombatarWidget *zombatarWidget, Sexy::Graphics *g) {
    Color theAlphaColor = {255, 255, 255, 64};
    for (int i = 0; i < 15; ++i) {
        int theX = 160 + 198 + (i % 6) * 73;
        int theY = 162 + i / 6 * 79;
        Sexy::Image *image = ZombatarWidget_GetAccessoryImageByIndex(i);
        int mWidth = image->mWidth;
        int mHeight = image->mHeight;
        int max = std::max(mWidth, mHeight);
        float ratio = 58.0 / max;
        if (ratio > 1.3)
            ratio = 1.3;
        float widthOffset = (58 - ratio * mWidth) / 2;
        float heightOffset = (58 - ratio * mHeight) / 2;

        if (zombatarWidget->mSelectedAccessory == i) {
            g->DrawImage(addonZombatarImages.zombatar_accessory_bg_highlight, theX, theY);
            TodDrawImageScaledF(g, image, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
        } else {
            Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg, &theAlphaColor, theX, theY);
            Sexy_Graphics_DrawImageColorizedScaled(g, image, &theAlphaColor, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
        }
    }
    int theX = 160 + 198 + (15 % 6) * 73;
    int theY = 162 + 15 / 6 * 79;
    Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg_none, &theAlphaColor, theX, theY);

    if (zombatarWidget->mSelectedAccessory == 255) {
        int holder[1];
        TodStringTranslate(holder, "[ZOMBATAR_COLOR_ITEM_NOT_CHOSEN]");
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = *Sexy_FONT_BRIANNETOD12_Addr;
        TodDrawStringWrapped(g, holder, &rect, font, &white, DrawStringJustification::DS_ALIGN_LEFT, false);
        StringDelete(holder);
    } else if (ZombatarWidget_AccessoryIsColorized(zombatarWidget->mSelectedTab, zombatarWidget->mSelectedAccessory)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Color color = gZombatarAccessoryColor2[i];
            if (zombatarWidget->mSelectedAccessoryColor != i) {
                color.mAlpha = 64;
            }
            Sexy_Graphics_DrawImageColorized(g, i == 17 ? addonZombatarImages.zombatar_colorpicker_none : addonZombatarImages.zombatar_colorpicker, &color, theX, theY);
        }
    } else {
        int holder[1];
        TodStringTranslate(holder, "[ZOMBATAR_COLOR_NOT_APPLICABLE]");
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = *Sexy_FONT_BRIANNETOD12_Addr;
        TodDrawStringWrapped(g, holder, &rect, font, &white, DrawStringJustification::DS_ALIGN_LEFT, false);
        StringDelete(holder);
    }
}

void TestMenuWidget_DrawHat(ZombatarWidget *zombatarWidget, Sexy::Graphics *g) {
    Color theAlphaColor = {255, 255, 255, 64};
    for (int i = 0; i < 14; ++i) {
        int theX = 160 + 198 + (i % 6) * 73;
        int theY = 162 + i / 6 * 79;
        Sexy::Image *image = ZombatarWidget_GetHatImageByIndex(i);
        Sexy::Image *image1 = ZombatarWidget_GetHatMaskImageByIndex(i);
        int mWidth = image->mWidth;
        int mHeight = image->mHeight;
        int max = std::max(mWidth, mHeight);
        float ratio = 58.0 / max;
        if (ratio > 1.3)
            ratio = 1.3;
        float widthOffset = (58 - ratio * mWidth) / 2;
        float heightOffset = (58 - ratio * mHeight) / 2;
        float widthOffset2 = ZombatarWidget_GetHatMaskImageOffsetXByIndex(i) * ratio;
        float heightOffset2 = ZombatarWidget_GetHatMaskImageOffsetYByIndex(i) * ratio;

        if (zombatarWidget->mSelectedHat == i) {
            g->DrawImage(addonZombatarImages.zombatar_accessory_bg_highlight, theX, theY);
            if (image1 != nullptr) {
                TodDrawImageScaledF(g, image1, theX + 12 + widthOffset + widthOffset2, theY + 12 + heightOffset + heightOffset2, ratio, ratio);
            }
            TodDrawImageScaledF(g, image, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
        } else {
            Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg, &theAlphaColor, theX, theY);
            if (image1 != nullptr) {
                Sexy_Graphics_DrawImageColorizedScaled(g, image1, &theAlphaColor, theX + 12 + widthOffset + widthOffset2, theY + 12 + heightOffset + heightOffset2, ratio, ratio);
            }
            Sexy_Graphics_DrawImageColorizedScaled(g, image, &theAlphaColor, theX + 12 + widthOffset, theY + 12 + heightOffset, ratio, ratio);
        }
    }
    int theX = 160 + 198 + (14 % 6) * 73;
    int theY = 162 + 14 / 6 * 79;
    Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg_none, &theAlphaColor, theX, theY);

    if (zombatarWidget->mSelectedHat == 255) {
        int holder[1];
        TodStringTranslate(holder, "[ZOMBATAR_COLOR_ITEM_NOT_CHOSEN]");
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = *Sexy_FONT_BRIANNETOD12_Addr;
        TodDrawStringWrapped(g, holder, &rect, font, &white, DrawStringJustification::DS_ALIGN_LEFT, false);
        StringDelete(holder);
    } else if (ZombatarWidget_AccessoryIsColorized(zombatarWidget->mSelectedTab, zombatarWidget->mSelectedHat)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Color color = gZombatarAccessoryColor2[i];
            if (zombatarWidget->mSelectedHatColor != i) {
                color.mAlpha = 64;
            }
            Sexy_Graphics_DrawImageColorized(g, i == 17 ? addonZombatarImages.zombatar_colorpicker_none : addonZombatarImages.zombatar_colorpicker, &color, theX, theY);
        }
    } else {
        int holder[1];
        TodStringTranslate(holder, "[ZOMBATAR_COLOR_NOT_APPLICABLE]");
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = *Sexy_FONT_BRIANNETOD12_Addr;
        TodDrawStringWrapped(g, holder, &rect, font, &white, DrawStringJustification::DS_ALIGN_LEFT, false);
        StringDelete(holder);
    }
}

void TestMenuWidget_DrawBackground(ZombatarWidget *zombatarWidget, Sexy::Graphics *g) {
    Color theAlphaColor = {255, 255, 255, 64};
    if (zombatarWidget->mSelectedBackgroundPage == 0) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 198 + (i % 6) * 73;
            int theY = 162 + i / 6 * 79;
            if (zombatarWidget->mSelectedBackground == i) {
                g->DrawImage(addonZombatarImages.zombatar_accessory_bg_highlight, theX, theY);
                TodDrawImageScaledF(g, ZombatarWidget_GetBackgroundImageByIndex(i), theX + 12, theY + 12, 58.0 / 216.0, 58.0 / 216.0);
            }
            Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg, &theAlphaColor, theX, theY);
            Sexy_Graphics_DrawImageColorizedScaled(g, ZombatarWidget_GetBackgroundImageByIndex(i), &theAlphaColor, theX + 12, theY + 12, 58.0 / 216.0, 58.0 / 216.0);
        }
        Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_prev_button, &theAlphaColor, 160 + 209, 436);
        g->DrawImage(addonZombatarImages.zombatar_next_button, 160 + 588, 436);
    } else if (zombatarWidget->mSelectedBackgroundPage == 4) {
        for (int i = 0; i < 11; ++i) {
            int theX = 160 + 198 + (i % 6) * 73;
            int theY = 162 + i / 6 * 79;
            if (zombatarWidget->mSelectedBackground == i + 18 * zombatarWidget->mSelectedBackgroundPage) {
                g->DrawImage(addonZombatarImages.zombatar_accessory_bg_highlight, theX, theY);
                TodDrawImageScaledF(g, ZombatarWidget_GetBackgroundImageByIndex(i + 18 * zombatarWidget->mSelectedBackgroundPage), theX + 12, theY + 12, 58.0 / 216.0, 58.0 / 216.0);
            }
            Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg, &theAlphaColor, theX, theY);
            Sexy_Graphics_DrawImageColorizedScaled(
                g, ZombatarWidget_GetBackgroundImageByIndex(i + 18 * zombatarWidget->mSelectedBackgroundPage), &theAlphaColor, theX + 12, theY + 12, 58.0 / 216.0, 58.0 / 216.0);
        }
        g->DrawImage(addonZombatarImages.zombatar_prev_button, 160 + 209, 436);
        Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_next_button, &theAlphaColor, 160 + 588, 436);
    } else {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 198 + (i % 6) * 73;
            int theY = 162 + i / 6 * 79;
            if (zombatarWidget->mSelectedBackground == i + zombatarWidget->mSelectedBackgroundPage * 18) {
                g->DrawImage(addonZombatarImages.zombatar_accessory_bg_highlight, theX, theY);
                TodDrawImageScaledF(g, ZombatarWidget_GetBackgroundImageByIndex(i + zombatarWidget->mSelectedBackgroundPage * 18), theX + 12, theY + 12, 58.0 / 216.0, 58.0 / 216.0);
            }
            Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_accessory_bg, &theAlphaColor, theX, theY);
            Sexy_Graphics_DrawImageColorizedScaled(
                g, ZombatarWidget_GetBackgroundImageByIndex(i + zombatarWidget->mSelectedBackgroundPage * 18), &theAlphaColor, theX + 12, theY + 12, 58.0 / 216.0, 58.0 / 216.0);
        }
        g->DrawImage(addonZombatarImages.zombatar_prev_button, 160 + 209, 436);
        g->DrawImage(addonZombatarImages.zombatar_next_button, 160 + 588, 436);
    }


    if (ZombatarWidget_AccessoryIsColorized(zombatarWidget->mSelectedTab, zombatarWidget->mSelectedBackground)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Color color = gZombatarAccessoryColor2[i];
            if (zombatarWidget->mSelectedBackgroundColor != i) {
                color.mAlpha = 64;
            }
            Sexy_Graphics_DrawImageColorized(g, i == 17 ? addonZombatarImages.zombatar_colorpicker_none : addonZombatarImages.zombatar_colorpicker, &color, theX, theY);
        }
    } else {
        int holder[1];
        TodStringTranslate(holder, "[ZOMBATAR_COLOR_NOT_APPLICABLE]");
        Sexy::Rect rect = {160 + 288, 445, 250, 100};
        Sexy::Font *font = *Sexy_FONT_BRIANNETOD12_Addr;
        TodDrawStringWrapped(g, holder, &rect, font, &white, DrawStringJustification::DS_ALIGN_LEFT, false);
        StringDelete(holder);
    }

    int holder[1];
    StrFormat(holder, "PAGE %d/%d", zombatarWidget->mSelectedBackgroundPage + 1, 5);
    TodDrawString(g, holder, 160 + 410, 525, *Sexy_FONT_BRIANNETOD16_Addr, black, DrawStringJustification::DS_ALIGN_CENTER);
    StringDelete(holder);
}

void TestMenuWidget_DrawPortrait(ZombatarWidget *zombatarWidget, Sexy::Graphics *g, int x, int y) {
    Sexy::Image *backgroundImage = ZombatarWidget_GetBackgroundImageByIndex(zombatarWidget->mSelectedBackground);
    if (ZombatarWidget_AccessoryIsColorized(ZombatarWidget::BACKGROUND, zombatarWidget->mSelectedBackground) && zombatarWidget->mSelectedBackgroundColor != 255) {
        Sexy_Graphics_DrawImageColorized(g, backgroundImage, &gZombatarAccessoryColor2[zombatarWidget->mSelectedBackgroundColor], x, y);
    } else {
        g->DrawImage(backgroundImage, x, y);
    }
    Sexy_Graphics_DrawImageColorized(g, addonZombatarImages.zombatar_zombie_blank_skin, &gZombatarSkinColor[zombatarWidget->mSelectedSkinColor], x + 46, y + 48);
    g->DrawImage(addonZombatarImages.zombatar_zombie_blank, x + 46, y + 48);

    Sexy::Image *clothImage = ZombatarWidget_GetClothImageByIndex(zombatarWidget->mSelectedCloth);
    if (clothImage != nullptr) {
        int offsetX = addonZombatarImages.zombatar_background_blank->mWidth + ZombatarWidget_GetClothImageOffsetXByIndex(zombatarWidget->mSelectedCloth);
        int offsetY = addonZombatarImages.zombatar_background_blank->mHeight + ZombatarWidget_GetClothImageOffsetYByIndex(zombatarWidget->mSelectedCloth);
        g->DrawImage(clothImage, x + offsetX, y + offsetY);
    }

    Sexy::Image *tidBitImage = ZombatarWidget_GetTidBitImageByIndex(zombatarWidget->mSelectedTidBit);
    if (tidBitImage != nullptr) {
        int offsetX = ZombatarWidget_GetTidBitImageOffsetXByIndex(zombatarWidget->mSelectedTidBit);
        int offsetY = ZombatarWidget_GetTidBitImageOffsetYByIndex(zombatarWidget->mSelectedTidBit);
        if (zombatarWidget->mSelectedTidBitColor != 255 && ZombatarWidget_AccessoryIsColorized(ZombatarWidget::TIDBIT, zombatarWidget->mSelectedTidBit) && zombatarWidget->mSelectedTidBit != 0) {
            Sexy_Graphics_DrawImageColorized(g, tidBitImage, &gZombatarAccessoryColor2[zombatarWidget->mSelectedTidBitColor], x + offsetX, y + offsetY);
        } else {
            g->DrawImage(tidBitImage, x + offsetX, y + offsetY);
        }
    }
    if (zombatarWidget->mSelectedTidBit == 0) {
        Sexy::Image *tidBitImage = ZombatarWidget_GetTidBitImageByIndex(2);
        if (tidBitImage != nullptr) {
            int offsetX = ZombatarWidget_GetTidBitImageOffsetXByIndex(2);
            int offsetY = ZombatarWidget_GetTidBitImageOffsetYByIndex(2);
            if (zombatarWidget->mSelectedTidBitColor != 255 && ZombatarWidget_AccessoryIsColorized(ZombatarWidget::TIDBIT, 2)) {
                Sexy_Graphics_DrawImageColorized(g, tidBitImage, &gZombatarAccessoryColor2[zombatarWidget->mSelectedTidBitColor], x + offsetX, y + offsetY);
            } else {
                g->DrawImage(tidBitImage, x + offsetX, y + offsetY);
            }
        }
    }

    Sexy::Image *eyeWearImage = ZombatarWidget_GetEyeWearImageByIndex(zombatarWidget->mSelectedEyeWear);
    if (eyeWearImage != nullptr) {
        int offsetX = ZombatarWidget_GetEyeWearImageOffsetXByIndex(zombatarWidget->mSelectedEyeWear);
        int offsetY = ZombatarWidget_GetEyeWearImageOffsetYByIndex(zombatarWidget->mSelectedEyeWear);
        Sexy::Image *eyeWearMaskImage = ZombatarWidget_GetEyeWearMaskImageByIndex(zombatarWidget->mSelectedEyeWear);
        if (zombatarWidget->mSelectedEyeWearColor != 255 && ZombatarWidget_AccessoryIsColorized(ZombatarWidget::EYEWEAR, zombatarWidget->mSelectedEyeWear)) {
            if (eyeWearMaskImage != nullptr) {
                Sexy_Graphics_DrawImageColorized(g, eyeWearMaskImage, &gZombatarAccessoryColor2[zombatarWidget->mSelectedEyeWearColor], x + offsetX, y + offsetY);
                g->DrawImage(eyeWearImage, x + offsetX, y + offsetY);
            } else {
                Sexy_Graphics_DrawImageColorized(g, eyeWearImage, &gZombatarAccessoryColor2[zombatarWidget->mSelectedEyeWearColor], x + offsetX, y + offsetY);
            }
        } else {
            if (eyeWearMaskImage != nullptr) {
                g->DrawImage(eyeWearMaskImage, x + offsetX, y + offsetY);
                g->DrawImage(eyeWearImage, x + offsetX, y + offsetY);
            } else {
                g->DrawImage(eyeWearImage, x + offsetX, y + offsetY);
            }
        }
    }

    Sexy::Image *accessoryImage = ZombatarWidget_GetAccessoryImageByIndex(zombatarWidget->mSelectedAccessory);
    if (accessoryImage != nullptr) {
        int offsetX = ZombatarWidget_GetAccessoryImageOffsetXByIndex(zombatarWidget->mSelectedAccessory);
        int offsetY = ZombatarWidget_GetAccessoryImageOffsetYByIndex(zombatarWidget->mSelectedAccessory);
        if (zombatarWidget->mSelectedAccessoryColor != 255 && ZombatarWidget_AccessoryIsColorized(ZombatarWidget::ACCESSORY, zombatarWidget->mSelectedAccessory)) {
            Sexy_Graphics_DrawImageColorized(g, accessoryImage, &gZombatarAccessoryColor2[zombatarWidget->mSelectedAccessoryColor], x + offsetX, y + offsetY);
        } else {
            g->DrawImage(accessoryImage, x + offsetX, y + offsetY);
        }
    }

    Sexy::Image *fHairImage = ZombatarWidget_GetFHairImageByIndex(zombatarWidget->mSelectedFHair);
    if (fHairImage != nullptr) {
        int offsetX = ZombatarWidget_GetFHairImageOffsetXByIndex(zombatarWidget->mSelectedFHair);
        int offsetY = ZombatarWidget_GetFHairImageOffsetYByIndex(zombatarWidget->mSelectedFHair);
        int offsetX2 = ZombatarWidget_GetFHairMaskImageOffsetXByIndex(zombatarWidget->mSelectedFHair);
        int offsetY2 = ZombatarWidget_GetFHairMaskImageOffsetYByIndex(zombatarWidget->mSelectedFHair);
        Sexy::Image *fHairMaskImage = ZombatarWidget_GetFHairMaskImageByIndex(zombatarWidget->mSelectedFHair);
        if (zombatarWidget->mSelectedFHairColor != 255 && ZombatarWidget_AccessoryIsColorized(ZombatarWidget::FHAIR, zombatarWidget->mSelectedFHair)) {
            if (fHairMaskImage != nullptr) {
                Sexy_Graphics_DrawImageColorized(g, fHairMaskImage, &gZombatarAccessoryColor[zombatarWidget->mSelectedFHairColor], x + offsetX + offsetX2, y + offsetY + offsetY2);
                g->DrawImage(fHairImage, x + offsetX, y + offsetY);
            } else {
                Sexy_Graphics_DrawImageColorized(g, fHairImage, &gZombatarAccessoryColor[zombatarWidget->mSelectedFHairColor], x + offsetX, y + offsetY);
            }
        } else {
            if (fHairMaskImage != nullptr) {
                g->DrawImage(fHairMaskImage, x + offsetX + offsetX2, y + offsetY + offsetY2);
                g->DrawImage(fHairImage, x + offsetX, y + offsetY);
            } else {
                g->DrawImage(fHairImage, x + offsetX, y + offsetY);
            }
        }
    }


    Sexy::Image *hairImage = ZombatarWidget_GetHairImageByIndex(zombatarWidget->mSelectedHair);
    if (hairImage != nullptr) {
        int offsetX = ZombatarWidget_GetHairImageOffsetXByIndex(zombatarWidget->mSelectedHair);
        int offsetY = ZombatarWidget_GetHairImageOffsetYByIndex(zombatarWidget->mSelectedHair);
        int offsetX2 = ZombatarWidget_GetHairMaskImageOffsetXByIndex(zombatarWidget->mSelectedHair);
        int offsetY2 = ZombatarWidget_GetHairMaskImageOffsetYByIndex(zombatarWidget->mSelectedHair);
        Sexy::Image *hairMaskImage = ZombatarWidget_GetHairMaskImageByIndex(zombatarWidget->mSelectedHair);
        if (zombatarWidget->mSelectedHairColor != 255 && ZombatarWidget_AccessoryIsColorized(ZombatarWidget::HAIR, zombatarWidget->mSelectedHair)) {
            if (hairMaskImage != nullptr) {
                Sexy_Graphics_DrawImageColorized(g, hairMaskImage, &gZombatarAccessoryColor[zombatarWidget->mSelectedHairColor], x + offsetX + offsetX2, y + offsetY + offsetY2);
                g->DrawImage(hairImage, x + offsetX, y + offsetY);
            } else {
                Sexy_Graphics_DrawImageColorized(g, hairImage, &gZombatarAccessoryColor[zombatarWidget->mSelectedHairColor], x + offsetX, y + offsetY);
            }
        } else {
            if (hairMaskImage != nullptr) {
                g->DrawImage(hairMaskImage, x + offsetX + offsetX2, y + offsetY + offsetY2);
                g->DrawImage(hairImage, x + offsetX, y + offsetY);
            } else {
                g->DrawImage(hairImage, x + offsetX, y + offsetY);
            }
        }
    }

    Sexy::Image *hatImage = ZombatarWidget_GetHatImageByIndex(zombatarWidget->mSelectedHat);
    if (hatImage != nullptr) {
        int offsetX = ZombatarWidget_GetHatImageOffsetXByIndex(zombatarWidget->mSelectedHat);
        int offsetY = ZombatarWidget_GetHatImageOffsetYByIndex(zombatarWidget->mSelectedHat);
        int offsetX2 = ZombatarWidget_GetHatMaskImageOffsetXByIndex(zombatarWidget->mSelectedHat);
        int offsetY2 = ZombatarWidget_GetHatMaskImageOffsetYByIndex(zombatarWidget->mSelectedHat);
        Sexy::Image *hatMaskImage = ZombatarWidget_GetHatMaskImageByIndex(zombatarWidget->mSelectedHat);
        if (zombatarWidget->mSelectedHatColor != 255 && ZombatarWidget_AccessoryIsColorized(ZombatarWidget::HAT, zombatarWidget->mSelectedHat)) {
            if (hatMaskImage != nullptr) {
                Sexy_Graphics_DrawImageColorized(g, hatMaskImage, &gZombatarAccessoryColor2[zombatarWidget->mSelectedHatColor], x + offsetX + offsetX2, y + offsetY + offsetY2);
                g->DrawImage(hatImage, x + offsetX, y + offsetY);
            } else {
                Sexy_Graphics_DrawImageColorized(g, hatImage, &gZombatarAccessoryColor2[zombatarWidget->mSelectedHatColor], x + offsetX, y + offsetY);
            }
        } else {
            if (hatMaskImage != nullptr) {
                g->DrawImage(hatMaskImage, x + offsetX + offsetX2, y + offsetY + offsetY2);
                g->DrawImage(hatImage, x + offsetX, y + offsetY);
            } else {
                g->DrawImage(hatImage, x + offsetX, y + offsetY);
            }
        }
    }
}

void TestMenuWidget_DrawPreView(ZombatarWidget *zombatarWidget, Sexy::Graphics *g) {
    g->DrawImage(*Sexy_IMAGE_ALMANAC_GROUNDDAY_Addr, 160 + 729, 376);
    float tmpX = g->mTransX;
    float tmpY = g->mTransY;
    g->mTransX += 160 + 778;
    g->mTransY += 434;
    zombatarWidget->mPreviewZombie->Draw(g);
    g->mTransX = tmpX;
    g->mTransY = tmpY;
}

void TestMenuWidget_Draw(ZombatarWidget *zombatarWidget, Sexy::Graphics *g) {

    g->DrawImage(addonZombatarImages.zombatar_main_bg, 0, 0);
    g->DrawImage(addonZombatarImages.zombatar_widget_bg, 160 + 26, 27);

    if (zombatarWidget->mShowExistingZombatarPortrait && addonImages.zombatar_portrait != nullptr) {
        g->DrawImage(addonImages.zombatar_portrait, 160 + 260, 210);
        int holder[1];
        TodStringTranslate(holder, "[ZOMBATAR_VIEW_PORTRAIT]");
        Sexy::Rect rect = {160 + 178, 450, 400, 100};
        TodDrawStringWrapped(g, holder, &rect, *Sexy_FONT_BRIANNETOD16_Addr, &white, DrawStringJustification::DS_ALIGN_CENTER, false);
        StringDelete(holder);
    } else if (zombatarWidget->mShowZombieTypeSelection) {
        TestMenuWidget_DrawZombieSelection(zombatarWidget, g);
    } else {
        g->DrawImage(addonZombatarImages.zombatar_widget_inner_bg, 160 + 179, 148);
        for (int i = 0; i < ZombatarWidget::MaxTabNum; ++i) {
            g->DrawImage(i == zombatarWidget->mSelectedTab ? ZombatarWidget_GetTabButtonDownImageByIndex(i) : ZombatarWidget_GetTabButtonImageByIndex(i), 160 + 67, 152 + i * 43);
        }
        g->DrawImage(addonZombatarImages.zombatar_colors_bg, 160 + 260, 394);
        switch (zombatarWidget->mSelectedTab) {
            case ZombatarWidget::SKIN:
                TestMenuWidget_DrawSkin(zombatarWidget, g);
                break;
            case ZombatarWidget::HAIR:
                TestMenuWidget_DrawHair(zombatarWidget, g);
                break;
            case ZombatarWidget::FHAIR:
                TestMenuWidget_DrawFHair(zombatarWidget, g);
                break;
            case ZombatarWidget::TIDBIT:
                TestMenuWidget_DrawTidBit(zombatarWidget, g);
                break;
            case ZombatarWidget::EYEWEAR:
                TestMenuWidget_DrawEyeWear(zombatarWidget, g);
                break;
            case ZombatarWidget::CLOTHES:
                TestMenuWidget_DrawCloth(zombatarWidget, g);
                break;
            case ZombatarWidget::ACCESSORY:
                TestMenuWidget_DrawAccessory(zombatarWidget, g);
                break;
            case ZombatarWidget::HAT:
                TestMenuWidget_DrawHat(zombatarWidget, g);
                break;
            case ZombatarWidget::BACKGROUND:
                TestMenuWidget_DrawBackground(zombatarWidget, g);
                break;
        }
    }

    TestMenuWidget_DrawPortrait(zombatarWidget, g, 160 + 708, 140);
    TestMenuWidget_DrawPreView(zombatarWidget, g);

    g->DrawImage(addonZombatarImages.zombatar_display_window, 160 + 0, 0);
}


void TestMenuWidget_RemovedFromManager(ZombatarWidget *zombatarWidget, int *manager) {
    old_TestMenuWidget_RemovedFromManager(zombatarWidget, manager);
    zombatarWidget->RemoveWidget((Sexy::Widget*)zombatarWidget->mBackButton);
    zombatarWidget->RemoveWidget((Sexy::Widget*)zombatarWidget->mFinishButton);
    zombatarWidget->RemoveWidget((Sexy::Widget*)zombatarWidget->mViewPortraitButton);
    zombatarWidget->RemoveWidget((Sexy::Widget*)zombatarWidget->mNewButton);
    zombatarWidget->RemoveWidget((Sexy::Widget*)zombatarWidget->mDeleteButton);
}

void TestMenuWidget_Delete2(ZombatarWidget *zombatarWidget) {
    // TODO:解决五个按钮的内存泄露问题。GameButton_Delete会闪退，暂不清楚原因。
    zombatarWidget->mPreviewZombie->DieNoLoot();
    (*((void (**)(Zombie *))zombatarWidget->mPreviewZombie->vTable + 1))(zombatarWidget->mPreviewZombie); // Delete();

    (*((void (**)(Sexy::Widget *, Sexy::Widget *))zombatarWidget->vTable + 7))(zombatarWidget, zombatarWidget->mBackButton);
    zombatarWidget->mBackButton->mDrawStoneButton = true;
    zombatarWidget->mBackButton->mButtonImage = nullptr;
    zombatarWidget->mBackButton->mDownImage = nullptr;
    zombatarWidget->mBackButton->mOverImage = nullptr;
    // GameButton_Delete(zombatarWidget->mBackButton);

    (*((void (**)(Sexy::Widget *, Sexy::Widget *))zombatarWidget->vTable + 7))(zombatarWidget, zombatarWidget->mFinishButton);
    zombatarWidget->mFinishButton->mDrawStoneButton = true;
    zombatarWidget->mFinishButton->mButtonImage = nullptr;
    zombatarWidget->mFinishButton->mDownImage = nullptr;
    zombatarWidget->mFinishButton->mOverImage = nullptr;
    // GameButton_Delete(zombatarWidget->mFinishButton);


    (*((void (**)(Sexy::Widget *, Sexy::Widget *))zombatarWidget->vTable + 7))(zombatarWidget, zombatarWidget->mViewPortraitButton);
    zombatarWidget->mViewPortraitButton->mDrawStoneButton = true;
    zombatarWidget->mViewPortraitButton->mButtonImage = nullptr;
    zombatarWidget->mViewPortraitButton->mDownImage = nullptr;
    zombatarWidget->mViewPortraitButton->mOverImage = nullptr;
    // GameButton_Delete(zombatarWidget->mViewPortraitButton);

    // GameButton_Delete(zombatarWidget->mNewButton);
    // GameButton_Delete(zombatarWidget->mDeleteButton);
    operator delete(zombatarWidget->mButtonListener->vTable);
    operator delete(zombatarWidget->mButtonListener);
    old_TestMenuWidget_Delete2(zombatarWidget);
}

void TestMenuWidget_Delete(ZombatarWidget *zombatarWidget) {
    TestMenuWidget_Delete2(zombatarWidget);
    delete zombatarWidget;
}

void TestMenuWidget_MouseDownSkin(ZombatarWidget *zombatarWidget, int x, int y) {
    for (int i = 0; i < 12; ++i) {
        int theX = 160 + 285 + (i % 9) * 30;
        int theY = 432 + i / 9 * 30;
        Sexy::Rect rect = {theX, theY, 30, 30};
        if (TRect_Contains(&rect, x, y)) {
            zombatarWidget->mSelectedSkinColor = i;
            return;
        }
    }
}

void TestMenuWidget_MouseDownHair(ZombatarWidget *zombatarWidget, int x, int y) {
    for (int i = 0; i < 16; ++i) {
        int theX = 160 + 198 + (i % 6) * 73;
        int theY = 162 + i / 6 * 79;
        Sexy::Rect rect = {theX, theY, 73, 79};
        if (TRect_Contains(&rect, x, y)) {
            zombatarWidget->mSelectedHair = i;
            Reanimation_SetZombatarHair(zombatarWidget->mZombatarReanim, zombatarWidget->mSelectedHair, zombatarWidget->mSelectedHairColor);
            return;
        }
    }
    int theX = 160 + 198 + (16 % 6) * 73;
    int theY = 162 + 16 / 6 * 79;
    Sexy::Rect rect = {theX, theY, 73, 79};
    if (TRect_Contains(&rect, x, y)) {
        zombatarWidget->mSelectedHair = 255;
        Reanimation_SetZombatarHair(zombatarWidget->mZombatarReanim, zombatarWidget->mSelectedHair, zombatarWidget->mSelectedHairColor);
        return;
    }
    if (zombatarWidget->mSelectedHair != 255 && ZombatarWidget_AccessoryIsColorized(zombatarWidget->mSelectedTab, zombatarWidget->mSelectedHair)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Sexy::Rect rect = {theX, theY, 30, 30};
            if (TRect_Contains(&rect, x, y)) {
                zombatarWidget->mSelectedHairColor = i;
                Reanimation_SetZombatarHair(zombatarWidget->mZombatarReanim, zombatarWidget->mSelectedHair, zombatarWidget->mSelectedHairColor);
                return;
            }
        }
    }
}

void TestMenuWidget_MouseDownFHair(ZombatarWidget *zombatarWidget, int x, int y) {
    if (zombatarWidget->mSelectedFHairPage == 0) {
        for (int i = 0; i < 17; ++i) {
            int theX = 160 + 198 + (i % 6) * 73;
            int theY = 162 + i / 6 * 79;
            Sexy::Rect rect = {theX, theY, 73, 79};
            if (TRect_Contains(&rect, x, y)) {
                zombatarWidget->mSelectedFHair = i;
                Reanimation_SetZombatarFHair(zombatarWidget->mZombatarReanim, zombatarWidget->mSelectedFHair, zombatarWidget->mSelectedFHairColor);
                return;
            }
        }
        int theX = 160 + 198 + (17 % 6) * 73;
        int theY = 162 + 17 / 6 * 79;
        Sexy::Rect rect = {theX, theY, 73, 79};
        if (TRect_Contains(&rect, x, y)) {
            zombatarWidget->mSelectedFHair = 255;
            Reanimation_SetZombatarFHair(zombatarWidget->mZombatarReanim, zombatarWidget->mSelectedFHair, zombatarWidget->mSelectedFHairColor);
            return;
        }
        Sexy::Rect next = {160 + 588, 436, addonZombatarImages.zombatar_next_button->mWidth, addonZombatarImages.zombatar_next_button->mHeight};
        if (TRect_Contains(&next, x, y)) {
            zombatarWidget->mSelectedFHairPage++;
            return;
        }
    } else if (zombatarWidget->mSelectedFHairPage == 1) {
        for (int i = 0; i < 7; ++i) {
            int theX = 160 + 198 + (i % 6) * 73;
            int theY = 162 + i / 6 * 79;
            Sexy::Rect rect = {theX, theY, 73, 79};
            if (TRect_Contains(&rect, x, y)) {
                zombatarWidget->mSelectedFHair = i + 17;
                Reanimation_SetZombatarFHair(zombatarWidget->mZombatarReanim, zombatarWidget->mSelectedFHair, zombatarWidget->mSelectedFHairColor);
                return;
            }
        }
        int theX = 160 + 198 + (7 % 6) * 73;
        int theY = 162 + 7 / 6 * 79;
        Sexy::Rect rect = {theX, theY, 73, 79};
        if (TRect_Contains(&rect, x, y)) {
            zombatarWidget->mSelectedFHair = 255;
            Reanimation_SetZombatarFHair(zombatarWidget->mZombatarReanim, zombatarWidget->mSelectedFHair, zombatarWidget->mSelectedFHairColor);
            return;
        }
        Sexy::Rect prev = {160 + 209, 436, addonZombatarImages.zombatar_next_button->mWidth, addonZombatarImages.zombatar_next_button->mHeight};
        if (TRect_Contains(&prev, x, y)) {
            zombatarWidget->mSelectedFHairPage--;
            return;
        }
    }

    if (zombatarWidget->mSelectedFHair != 255 && ZombatarWidget_AccessoryIsColorized(zombatarWidget->mSelectedTab, zombatarWidget->mSelectedFHair)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Sexy::Rect rect = {theX, theY, 30, 30};
            if (TRect_Contains(&rect, x, y)) {
                zombatarWidget->mSelectedFHairColor = i;
                Reanimation_SetZombatarFHair(zombatarWidget->mZombatarReanim, zombatarWidget->mSelectedFHair, zombatarWidget->mSelectedFHairColor);
                return;
            }
        }
    }
}

void TestMenuWidget_MouseDownTidBit(ZombatarWidget *zombatarWidget, int x, int y) {
    for (int i = 0; i < 14; ++i) {
        int theX = 160 + 198 + (i % 6) * 73;
        int theY = 162 + i / 6 * 79;
        Sexy::Rect rect = {theX, theY, 73, 79};
        if (TRect_Contains(&rect, x, y)) {
            zombatarWidget->mSelectedTidBit = i;
            Reanimation_SetZombatarTidBits(zombatarWidget->mZombatarReanim, zombatarWidget->mSelectedTidBit, zombatarWidget->mSelectedTidBitColor);
            return;
        }
    }
    int theX = 160 + 198 + (14 % 6) * 73;
    int theY = 162 + 14 / 6 * 79;
    Sexy::Rect rect = {theX, theY, 73, 79};
    if (TRect_Contains(&rect, x, y)) {
        zombatarWidget->mSelectedTidBit = 255;
        Reanimation_SetZombatarTidBits(zombatarWidget->mZombatarReanim, zombatarWidget->mSelectedTidBit, zombatarWidget->mSelectedTidBitColor);
        return;
    }

    if (zombatarWidget->mSelectedTidBit != 255 && ZombatarWidget_AccessoryIsColorized(zombatarWidget->mSelectedTab, zombatarWidget->mSelectedTidBit)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Sexy::Rect rect = {theX, theY, 30, 30};
            if (TRect_Contains(&rect, x, y)) {
                zombatarWidget->mSelectedTidBitColor = i;
                Reanimation_SetZombatarTidBits(zombatarWidget->mZombatarReanim, zombatarWidget->mSelectedTidBit, zombatarWidget->mSelectedTidBitColor);
                return;
            }
        }
    }
}

void TestMenuWidget_MouseDownEyeWear(ZombatarWidget *zombatarWidget, int x, int y) {

    for (int i = 0; i < 16; ++i) {
        int theX = 160 + 198 + (i % 6) * 73;
        int theY = 162 + i / 6 * 79;
        Sexy::Rect rect = {theX, theY, 73, 79};
        if (TRect_Contains(&rect, x, y)) {
            zombatarWidget->mSelectedEyeWear = i;
            Reanimation_SetZombatarEyeWear(zombatarWidget->mZombatarReanim, zombatarWidget->mSelectedEyeWear, zombatarWidget->mSelectedEyeWearColor);
            return;
        }
    }
    int theX = 160 + 198 + (16 % 6) * 73;
    int theY = 162 + 16 / 6 * 79;
    Sexy::Rect rect = {theX, theY, 73, 79};
    if (TRect_Contains(&rect, x, y)) {
        zombatarWidget->mSelectedEyeWear = 255;
        Reanimation_SetZombatarEyeWear(zombatarWidget->mZombatarReanim, zombatarWidget->mSelectedEyeWear, zombatarWidget->mSelectedEyeWearColor);
        return;
    }

    if (zombatarWidget->mSelectedEyeWear != 255 && ZombatarWidget_AccessoryIsColorized(zombatarWidget->mSelectedTab, zombatarWidget->mSelectedEyeWear)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Sexy::Rect rect = {theX, theY, 30, 30};
            if (TRect_Contains(&rect, x, y)) {
                zombatarWidget->mSelectedEyeWearColor = i;
                Reanimation_SetZombatarEyeWear(zombatarWidget->mZombatarReanim, zombatarWidget->mSelectedEyeWear, zombatarWidget->mSelectedEyeWearColor);
                return;
            }
        }
    }
}

void TestMenuWidget_MouseDownCloth(ZombatarWidget *zombatarWidget, int x, int y) {
    for (int i = 0; i < 12; ++i) {
        int theX = 160 + 198 + (i % 6) * 73;
        int theY = 162 + i / 6 * 79;
        Sexy::Rect rect = {theX, theY, 73, 79};
        if (TRect_Contains(&rect, x, y)) {
            zombatarWidget->mSelectedCloth = i;
            return;
        }
    }
    int theX = 160 + 198 + (12 % 6) * 73;
    int theY = 162 + 12 / 6 * 79;
    Sexy::Rect rect = {theX, theY, 73, 79};
    if (TRect_Contains(&rect, x, y)) {
        zombatarWidget->mSelectedCloth = 255;
        return;
    }
}

void TestMenuWidget_MouseDownAccessory(ZombatarWidget *zombatarWidget, int x, int y) {
    for (int i = 0; i < 15; ++i) {
        int theX = 160 + 198 + (i % 6) * 73;
        int theY = 162 + i / 6 * 79;
        Sexy::Rect rect = {theX, theY, 73, 79};
        if (TRect_Contains(&rect, x, y)) {
            zombatarWidget->mSelectedAccessory = i;
            Reanimation_SetZombatarAccessories(zombatarWidget->mZombatarReanim, zombatarWidget->mSelectedAccessory, zombatarWidget->mSelectedAccessoryColor);
            return;
        }
    }
    int theX = 160 + 198 + (15 % 6) * 73;
    int theY = 162 + 15 / 6 * 79;
    Sexy::Rect rect = {theX, theY, 73, 79};
    if (TRect_Contains(&rect, x, y)) {
        zombatarWidget->mSelectedAccessory = 255;
        Reanimation_SetZombatarAccessories(zombatarWidget->mZombatarReanim, zombatarWidget->mSelectedAccessory, zombatarWidget->mSelectedAccessoryColor);
        return;
    }

    if (zombatarWidget->mSelectedAccessory != 255 && ZombatarWidget_AccessoryIsColorized(zombatarWidget->mSelectedTab, zombatarWidget->mSelectedAccessory)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Sexy::Rect rect = {theX, theY, 30, 30};
            if (TRect_Contains(&rect, x, y)) {
                zombatarWidget->mSelectedAccessoryColor = i;
                Reanimation_SetZombatarAccessories(zombatarWidget->mZombatarReanim, zombatarWidget->mSelectedAccessory, zombatarWidget->mSelectedAccessoryColor);
                return;
            }
        }
    }
}

void TestMenuWidget_MouseDownHat(ZombatarWidget *zombatarWidget, int x, int y) {
    for (int i = 0; i < 14; ++i) {
        int theX = 160 + 198 + (i % 6) * 73;
        int theY = 162 + i / 6 * 79;
        Sexy::Rect rect = {theX, theY, 73, 79};
        if (TRect_Contains(&rect, x, y)) {
            zombatarWidget->mSelectedHat = i;
            Reanimation_SetZombatarHats(zombatarWidget->mZombatarReanim, zombatarWidget->mSelectedHat, zombatarWidget->mSelectedHatColor);
            return;
        }
    }
    int theX = 160 + 198 + (14 % 6) * 73;
    int theY = 162 + 14 / 6 * 79;
    Sexy::Rect rect = {theX, theY, 73, 79};
    if (TRect_Contains(&rect, x, y)) {
        zombatarWidget->mSelectedHat = 255;
        Reanimation_SetZombatarHats(zombatarWidget->mZombatarReanim, zombatarWidget->mSelectedHat, zombatarWidget->mSelectedHatColor);
        return;
    }

    if (zombatarWidget->mSelectedHat != 255 && ZombatarWidget_AccessoryIsColorized(zombatarWidget->mSelectedTab, zombatarWidget->mSelectedHat)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Sexy::Rect rect = {theX, theY, 30, 30};
            if (TRect_Contains(&rect, x, y)) {
                zombatarWidget->mSelectedHatColor = i;
                Reanimation_SetZombatarHats(zombatarWidget->mZombatarReanim, zombatarWidget->mSelectedHat, zombatarWidget->mSelectedHatColor);
                return;
            }
        }
    }
}

void TestMenuWidget_MouseDownBackground(ZombatarWidget *zombatarWidget, int x, int y) {
    if (zombatarWidget->mSelectedBackgroundPage == 0) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 198 + (i % 6) * 73;
            int theY = 162 + i / 6 * 79;
            Sexy::Rect rect = {theX, theY, 73, 79};
            if (TRect_Contains(&rect, x, y)) {
                zombatarWidget->mSelectedBackground = i;
                return;
            }
        }
        Sexy::Rect next = {160 + 588, 436, addonZombatarImages.zombatar_next_button->mWidth, addonZombatarImages.zombatar_next_button->mHeight};
        if (TRect_Contains(&next, x, y)) {
            zombatarWidget->mSelectedBackgroundPage++;
            return;
        }
    } else if (zombatarWidget->mSelectedBackgroundPage == 4) {
        for (int i = 0; i < 11; ++i) {
            int theX = 160 + 198 + (i % 6) * 73;
            int theY = 162 + i / 6 * 79;
            Sexy::Rect rect = {theX, theY, 73, 79};
            if (TRect_Contains(&rect, x, y)) {
                zombatarWidget->mSelectedBackground = i + 18 * zombatarWidget->mSelectedBackgroundPage;
                return;
            }
        }
        Sexy::Rect prev = {160 + 209, 436, addonZombatarImages.zombatar_next_button->mWidth, addonZombatarImages.zombatar_next_button->mHeight};
        if (TRect_Contains(&prev, x, y)) {
            zombatarWidget->mSelectedBackgroundPage--;
            return;
        }
    } else {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 198 + (i % 6) * 73;
            int theY = 162 + i / 6 * 79;
            Sexy::Rect rect = {theX, theY, 73, 79};
            if (TRect_Contains(&rect, x, y)) {
                zombatarWidget->mSelectedBackground = i + 18 * zombatarWidget->mSelectedBackgroundPage;
                return;
            }
        }
        Sexy::Rect next = {160 + 588, 436, addonZombatarImages.zombatar_next_button->mWidth, addonZombatarImages.zombatar_next_button->mHeight};
        if (TRect_Contains(&next, x, y)) {
            zombatarWidget->mSelectedBackgroundPage++;
            return;
        }
        Sexy::Rect prev = {160 + 209, 436, addonZombatarImages.zombatar_next_button->mWidth, addonZombatarImages.zombatar_next_button->mHeight};
        if (TRect_Contains(&prev, x, y)) {
            zombatarWidget->mSelectedBackgroundPage--;
            return;
        }
    }


    if (ZombatarWidget_AccessoryIsColorized(zombatarWidget->mSelectedTab, zombatarWidget->mSelectedBackground)) {
        for (int i = 0; i < 18; ++i) {
            int theX = 160 + 285 + (i % 9) * 30;
            int theY = 432 + i / 9 * 30;
            Sexy::Rect rect = {theX, theY, 30, 30};
            if (TRect_Contains(&rect, x, y)) {
                zombatarWidget->mSelectedBackgroundColor = i;
                return;
            }
        }
    }
}

void TestMenuWidget_MouseDown(ZombatarWidget *zombatarWidget, int x, int y) {
    xx = x;
    yy = y;
    //    Sexy_Widget_Move(zombatarWidget->mBackButton,xx,yy);
    //    zombatarWidget->mPreviewZombie->mX = x;
    //    zombatarWidget->mPreviewZombie->mY = y;
    LOG_DEBUG("{} {}", x, y);

    if (gMainMenuZombatarWidget->mShowExistingZombatarPortrait) {
        return;
    }
    for (char i = 0; i < ZombatarWidget::MaxTabNum; ++i) {
        Sexy::Rect rect = {160 + 67, 152 + i * 43, 125, 47};
        if (TRect_Contains(&rect, x, y)) {
            zombatarWidget->mSelectedTab = i;
            zombatarWidget->mSelectedFHairPage = 0;
            zombatarWidget->mSelectedBackgroundPage = 0;
            return;
        }
    }

    switch (zombatarWidget->mSelectedTab) {
        case ZombatarWidget::SKIN:
            TestMenuWidget_MouseDownSkin(zombatarWidget, x, y);
            break;
        case ZombatarWidget::HAIR:
            TestMenuWidget_MouseDownHair(zombatarWidget, x, y);
            break;
        case ZombatarWidget::FHAIR:
            TestMenuWidget_MouseDownFHair(zombatarWidget, x, y);
            break;
        case ZombatarWidget::TIDBIT:
            TestMenuWidget_MouseDownTidBit(zombatarWidget, x, y);
            break;
        case ZombatarWidget::EYEWEAR:
            TestMenuWidget_MouseDownEyeWear(zombatarWidget, x, y);
            break;
        case ZombatarWidget::CLOTHES:
            TestMenuWidget_MouseDownCloth(zombatarWidget, x, y);
            break;
        case ZombatarWidget::ACCESSORY:
            TestMenuWidget_MouseDownAccessory(zombatarWidget, x, y);
            break;
        case ZombatarWidget::HAT:
            TestMenuWidget_MouseDownHat(zombatarWidget, x, y);
            break;
        case ZombatarWidget::BACKGROUND:
            TestMenuWidget_MouseDownBackground(zombatarWidget, x, y);
            break;
    }
}

void TestMenuWidget_MouseDrag(ZombatarWidget *zombatarWidget, int x, int y) {
    xx = x;
    yy = y;
    //    Sexy_Widget_Move(zombatarWidget->mBackButton,xx,yy);
    //    zombatarWidget->mPreviewZombie->mX = x;
    //    zombatarWidget->mPreviewZombie->mY = y;
    LOG_DEBUG("{} {}", x, y);
}

void TestMenuWidget_MouseUp(ZombatarWidget *zombatarWidget, int x, int y) {}

void TestMenuWidget_KeyDown(ZombatarWidget *zombatarWidget, int keyCode) {
    if (keyCode == Sexy::KEYCODE_ESCAPE || keyCode == Sexy::KEYCODE_ESCAPE2) {
        LawnApp *lawnApp = (LawnApp *)*gLawnApp_Addr;
        lawnApp->KillZombatarScreen();
        lawnApp->ShowMainMenuScreen();
        return;
    }
    if (keyCode == Sexy::KEYCODE_UP) {
        yy--;
        LOG_DEBUG("{} {}", xx, yy);
    }
    if (keyCode == Sexy::KEYCODE_DOWN) {
        yy++;
        LOG_DEBUG("{} {}", xx, yy);
    }
    if (keyCode == Sexy::KEYCODE_LEFT) {
        xx--;
        LOG_DEBUG("{} {}", xx, yy);
    }
    if (keyCode == Sexy::KEYCODE_RIGHT) {
        xx++;
        LOG_DEBUG("{} {}", xx, yy);
    }
    if (keyCode == Sexy::KEYCODE_UP || keyCode == Sexy::KEYCODE_DOWN || keyCode == Sexy::KEYCODE_LEFT || keyCode == Sexy::KEYCODE_RIGHT) {
        return;
    }
}
