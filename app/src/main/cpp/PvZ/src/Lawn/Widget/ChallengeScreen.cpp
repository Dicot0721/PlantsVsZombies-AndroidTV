#include "PvZ/Lawn/Widget/ChallengeScreen.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Misc.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/Symbols.h"
#include "PvZ/SexyAppFramework/Widget/GameButton.h"

using namespace Sexy;

static Sexy::GameButton *gChallengeScreenCloseButton;

ChallengeScreen::ChallengeScreen(LawnApp *theApp, ChallengePage thePage) {
    Create(theApp, thePage);
}

void ChallengeScreen::Create(LawnApp *theApp, ChallengePage thePage) {
    // 去除按钮对触控的遮挡
    old_ChallengeScreen_ChallengeScreen(this, theApp, thePage);

    for (auto *button : mButtons) {
        // 把按钮全部缩小至长宽为0
        button->Resize(button->mX, button->mY, 0, 0);
    }

    //    if (thePage == CHALLENGE_PAGE_VS) {
    ////        SetUnlockChallengeIndex(thePage, false);
    //        mUnlockState = UNLOCK_SHAKING;
    //        mUnlockStateCounter = 100;
    //        mUnlockChallengeIndex = 0;
    //
    //        for (int aChallengeMode = 0; aChallengeMode < NUM_CHALLENGE_MODES; aChallengeMode++) {
    //            ChallengeDefinition& aDef = GetChallengeDefinition(aChallengeMode);
    //            if (aDef.mPage == thePage)
    //                mUnlockChallengeIndex = aChallengeMode;
    //        }
    //    }
}

namespace {
ChallengeDefinition gButteredPopcornDef = {GameMode::GAMEMODE_CHALLENGE_BUTTERED_POPCORN, 37, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 6, 1, "[BUTTERED_POPCORN]"};
ChallengeDefinition gPoolPartyDef = {GameMode::GAMEMODE_CHALLENGE_POOL_PARTY, 37, ChallengePage::CHALLENGE_PAGE_CHALLENGE, 6, 2, "[POOL_PARTY]"};
ChallengeDefinition gVSDayDef = {GameMode::GAMEMODE_MP_VS_DAY, 37, ChallengePage::CHALLENGE_PAGE_VS, 6, 1, "[MP_VS_DAY]"};
ChallengeDefinition gVSNightDef = {GameMode::GAMEMODE_MP_VS_NIGHT, 37, ChallengePage::CHALLENGE_PAGE_VS, 6, 1, "[MP_VS_NIGHT]"};
ChallengeDefinition gVSPoolDayDef = {GameMode::GAMEMODE_MP_VS_POOL_DAY, 37, ChallengePage::CHALLENGE_PAGE_VS, 6, 1, "[MP_VS_POOL_DAY]"};
ChallengeDefinition gVSPoolNightDef = {GameMode::GAMEMODE_MP_VS_POOL_NIGHT, 37, ChallengePage::CHALLENGE_PAGE_VS, 6, 1, "[MP_VS_POOL_NIGHT]"};
ChallengeDefinition gVSRoofDef = {GameMode::GAMEMODE_MP_VS_ROOF, 37, ChallengePage::CHALLENGE_PAGE_VS, 6, 1, "[MP_VS_ROOF]"};
} // namespace

ChallengeDefinition &GetChallengeDefinition(int theChallengeMode) {
    if (theChallengeMode + 2 == GameMode::GAMEMODE_CHALLENGE_BUTTERED_POPCORN) {
        return gButteredPopcornDef;
    }

    // if (theChallengeMode + 2 == GameMode::GAMEMODE_CHALLENGE_POOL_PARTY) {
    //     return gPoolPartyDef;
    // }

    //    if (theChallengeMode + 4 == GameMode::GAMEMODE_MP_VS) {
    //        return gVSDayDef;
    //    } else if (theChallengeMode + 3 == GameMode::GAMEMODE_MP_VS) {
    //        return gVSNightDef;
    //    } else if (theChallengeMode + 2 == GameMode::GAMEMODE_MP_VS) {
    //        return gVSPoolDayDef;
    //    } else if (theChallengeMode + 1 == GameMode::GAMEMODE_MP_VS) {
    //        return gVSPoolNightDef;
    //    } else if (theChallengeMode == GameMode::GAMEMODE_MP_VS) {
    //        return gVSRoofDef;
    //    }

    return old_GetChallengeDefinition(theChallengeMode);
}

void ChallengeScreen::Draw(Sexy::Graphics *graphics) {
    // 修复小游戏界面显示奖杯数不正确
    old_ChallengeScreen_Draw(this, graphics);

    int mTotalTrophiesInPage = 0;
    switch (mPageIndex) {
        case ChallengePage::CHALLENGE_PAGE_SURVIVAL:
            mTotalTrophiesInPage = 10;
            break;
        case ChallengePage::CHALLENGE_PAGE_CHALLENGE:
            for (int i = 0; i < 94; ++i) {
                if (GetChallengeDefinition(i).mPage == ChallengePage::CHALLENGE_PAGE_CHALLENGE) {
                    mTotalTrophiesInPage++;
                }
            }
            break;
        case ChallengePage::CHALLENGE_PAGE_COOP:
            mTotalTrophiesInPage = 10;
            break;
        case ChallengePage::CHALLENGE_PAGE_PUZZLE:
            mTotalTrophiesInPage = 18;
            break;
    }

    int holder[1];
    Sexy_StrFormat(holder, "%d/%d", mApp->GetNumTrophies(mPageIndex), mTotalTrophiesInPage);
    Color theColor = {255, 240, 0, 255};
    TodDrawString(graphics, holder, 711, 62, *Sexy_FONT_BRIANNETOD16_Addr, theColor, DrawStringJustification::DS_ALIGN_CENTER);
    Sexy_String_Delete(holder);
}

void ChallengeScreen::AddedToManager(int *theWidgetManager) {
    // 记录当前游戏状态
    int holder[1];
    TodStringTranslate(holder, "[CLOSE]");
    gChallengeScreenCloseButton = MakeButton(1000, &mButtonListener, this, (SexyString &)holder);
    gChallengeScreenCloseButton->Resize(800, 520, 170, 50);
    Sexy_String_Delete(holder);
    AddWidget(gChallengeScreenCloseButton);

    return old_ChallengeScreen_AddedToManager(this, theWidgetManager);
}

void ChallengeScreen::Update() {
    // 记录当前游戏状态
    return old_ChallengeScreen_Update(this);
}

void ChallengeScreen::RemovedFromManager(int *theWidgetManager) {
    // 记录当前游戏状态
    old_ChallengeScreen_RemovedFromManager(this, theWidgetManager);

    if (gChallengeScreenCloseButton != nullptr) {
        RemoveWidget(gChallengeScreenCloseButton);
    }
}

void ChallengeScreen::Delete2() {
    // 删除按钮
    old_ChallengeScreen_Delete2(this);

    if (gChallengeScreenCloseButton != nullptr) {
        gChallengeScreenCloseButton->Destroy();
        gChallengeScreenCloseButton = nullptr;
    }
}

void ChallengeScreen::ButtonPress(int theButtonId) {
    // 空函数替换，去除原有的点击进入关卡的功能
}

void ChallengeScreen::ButtonDepress(int theId) {
    // 去除原有的点击进入关卡的功能
    if (theId == 1000) {
        mApp->KillChallengeScreen();
        mApp->DoBackToMain();
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

    //    int totalGamesInThisPage = a[376];//如果这个值是33
    //    int currentSelectedGameIndex = ChallengeScreen_GetCurrentSelectedGameIndex(
    //            a);//这里取值就是0~32。种子雨是32。

    //    int firstGameInPageIndex = a->mScreenTopChallengeIndex;
    //    int firstGameInPageIndex2 = a[186];
    //    a->mSelectedMode = a[currentSelectedGameIndex + 1 + 188];//向下移动绿色光标，不可循环滚动
    //    a->mSelectedMode = a[currentSelectedGameIndex - 1 + 188];//向上移动绿色光标，不可循环滚动

    //    LOGD("dOWN:%d %d %d %d", x, y, firstGameInPageIndex, firstGameInPageIndex2);
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
        //        ChallengeScreen_UpdateButtons(a);
        gChallengeItemMoved = true;
    } else if (y - gChallengeScreenTouchDownY > triggerHeight) {
        gChallengeScreenGameIndex -= 1;
        gChallengeScreenTouchDownY += triggerHeight;
        int gameIndexToScroll = gChallengeScreenGameIndex <= 0 ? 0 : gChallengeScreenGameIndex;
        SetScrollTarget(gameIndexToScroll);
        //        ChallengeScreen_UpdateButtons(a);
        gChallengeItemMoved = true;
    }
}

void ChallengeScreen::MouseUp(int x, int y) {
    if (!gTouchOutSide && !gChallengeItemMoved) {
        int gameIndex = mScreenTopChallengeIndex + (y - mPageTop) / gChallengeItemHeight;
        if (mSelectedMode == mUnk1[gameIndex]) {
            KeyDown(Sexy::KEYCODE_ACCEPT);
        } else {
            LawnApp_PlaySample(mApp, *Sexy_SOUND_BUTTONCLICK_Addr);
            mSelectedMode = mUnk1[gameIndex];
        }
    }
    gTouchOutSide = false;
    gChallengeItemMoved = false;
}
