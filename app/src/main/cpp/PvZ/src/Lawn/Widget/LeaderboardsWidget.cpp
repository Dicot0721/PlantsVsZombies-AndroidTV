//
// Created by 28636 on 2025/5/26.
//

#include "PvZ/Lawn/Widget/LeaderboardsWidget.h"
#include "Homura/Logger.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Misc.h"
#include "PvZ/TodLib/Effect/Reanimator.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/TodLib/Common/TodStringFile.h"

using namespace Sexy;

static float gLeaderboardAchievementsPosition[12][2] = {
    {198, 496},
    {210, 492},
    {222, 306},
    {405, 501},
    {368, 500},
    {690, 478},
    {615, 487},
    {524, 326},
    {756, 373},
    {678, 390},
    {791, 278},
    {430, 362},
};

static Sexy::Rect gLeaderboardAchievementsRect[12][2] = {
    {{253, 594, 485, 62}, {833, 528, 198, 68}},
    {{209, 488, 91, 106}, {0, 0, 0, 0}},
    {{269, 330, 185, 122}, {298, 453, 91, 91}},
    {{452, 550, 151, 76}, {446, 512, 68, 37}},
    {{373, 504, 83, 34}, {389, 533, 63, 37}},
    {{744, 516, 86, 97}, {0, 0, 0, 0}},
    {{632, 490, 52, 40}, {636, 530, 60, 33}},
    {{561, 340, 77, 196}, {525, 404, 145, 56}},
    {{880, 384, 78, 82}, {891, 467, 26, 68}},
    {{715, 416, 104, 81}, {765, 384, 54, 35}},
    {{817, 332, 113, 171}, {850, 298, 46, 31}},
    {{456, 362, 43, 122}, {461, 484, 71, 30}},
};


int LeaderboardsWidget_GetAchievementIdByReanimationType(ReanimationType type) {
    AchievementId id = AchievementId::ACHIEVEMENT_HOME_SECURITY;
    switch (type) {
        case ReanimationType::REANIM_ACHIEVEMENT_HOME_SECURITY:
            id = AchievementId::ACHIEVEMENT_HOME_SECURITY;
            break;
        case ReanimationType::REANIM_ACHIEVEMENT_MORTICULTURALIST:
            id = AchievementId::ACHIEVEMENT_MORTICULTURALIST;
            break;
        case ReanimationType::REANIM_ACHIEVEMENT_SMARTY_BRANCHES:
            id = AchievementId::ACHIEVEMENT_TREE;
            break;
        case ReanimationType::REANIM_ACHIEVEMENT_CRASH_OF_THE_TITAN:
            id = AchievementId::ACHIEVEMENT_GARG;
            break;
        case ReanimationType::REANIM_ACHIEVEMENT_Zffs4Evr:
            id = AchievementId::ACHIEVEMENT_COOP;
            break;
        case ReanimationType::REANIM_ACHIEVEMENT_ALIVE_AND_PLANTING:
            id = AchievementId::ACHIEVEMENT_IMMORTAL;
            break;
        case ReanimationType::REANIM_ACHIEVEMENT_VERSUS:
            id = AchievementId::ACHIEVEMENT_VERSUS;
            break;
        case ReanimationType::REANIM_ACHIEVEMENT_SOIL_YOUR_PLANTS:
            id = AchievementId::ACHIEVEMENT_SOILPLANTS;
            break;
        case ReanimationType::REANIM_ACHIEVEMENT_EXPLODONATOR:
            id = AchievementId::ACHIEVEMENT_EXPLODONATOR;
            break;
        case ReanimationType::REANIM_ACHIEVEMENT_CLOSE_SHAVE:
            id = AchievementId::ACHIEVEMENT_CLOSESHAVE;
            break;
        case ReanimationType::REANIM_ACHIEVEMENT_SHOP:
            id = AchievementId::ACHIEVEMENT_SHOP;
            break;
        case ReanimationType::REANIM_ACHIEVEMENT_NOM_NOM_NOM:
            id = AchievementId::ACHIEVEMENT_CHOMP;
            break;
    }
    return id - AchievementId::ACHIEVEMENT_HOME_SECURITY;
}

int LeaderboardsWidget_GetAchievementIdByDrawOrder(int drawOrder) {
    ReanimationType type = ReanimationType::REANIM_ACHIEVEMENT_HOME_SECURITY;
    switch (drawOrder) {
        case 0:
            type = ReanimationType::REANIM_ACHIEVEMENT_CLOSE_SHAVE;
            break;
        case 1:
            type = ReanimationType::REANIM_ACHIEVEMENT_SHOP;
            break;
        case 2:
            type = ReanimationType::REANIM_ACHIEVEMENT_EXPLODONATOR;
            break;
        case 3:
            type = ReanimationType::REANIM_ACHIEVEMENT_ALIVE_AND_PLANTING;
            break;
        case 4:
            type = ReanimationType::REANIM_ACHIEVEMENT_SMARTY_BRANCHES;
            break;
        case 5:
            type = ReanimationType::REANIM_ACHIEVEMENT_NOM_NOM_NOM;
            break;
        case 6:
            type = ReanimationType::REANIM_ACHIEVEMENT_SOIL_YOUR_PLANTS;
            break;
        case 7:
            type = ReanimationType::REANIM_ACHIEVEMENT_VERSUS;
            break;
        case 8:
            type = ReanimationType::REANIM_ACHIEVEMENT_Zffs4Evr;
            break;
        case 9:
            type = ReanimationType::REANIM_ACHIEVEMENT_CRASH_OF_THE_TITAN;
            break;
        case 10:
            type = ReanimationType::REANIM_ACHIEVEMENT_MORTICULTURALIST;
            break;
        case 11:
            type = ReanimationType::REANIM_ACHIEVEMENT_HOME_SECURITY;
            break;
    }
    return type - ReanimationType::REANIM_ACHIEVEMENT_HOME_SECURITY;
}

void LeaderboardsWidget_ButtonDepress(Sexy::ButtonListener *listener, int id) {
    if (id == 1000) {
        LawnApp *lawnApp = (LawnApp *)*gLawnApp_Addr;
        lawnApp->KillLeaderboards();
        lawnApp->ShowMainMenuScreen();
    }
}

void LeaderboardsWidget_ButtonPress(Sexy::ButtonListener *listener, int id, int theCount) {}

LeaderboardsWidget::LeaderboardsWidget(LawnApp *theApp) {
    reinterpret_cast<DaveHelp *>(this)->Create(theApp);
    Resize(-240, -60, 1280, 720);
    mLeaderboardReanimations = (LeaderboardReanimations *)operator new(sizeof(LeaderboardReanimations));
    for (int i = 0; i < 5; ++i) {
//        Reanimation *reanim = (Reanimation *)operator new(sizeof(Reanimation));
//        Reanimation_Reanimation(reanim);
        Reanimation *reanim = new Reanimation;
        reanim->ReanimationInitializeType(0.0, 0.0, (ReanimationType)(ReanimationType::REANIM_LEADERBOARDS_HOUSE + i));
        reanim->SetAnimRate(0.0f);
        reanim->mLoopType = ReanimLoopType::REANIM_LOOP;
        if (i == 0) {
            mApp->SetHouseReanim(reanim);
            reanim->SetPosition(456.9f, 129.3f);
        }
        if (i == 1 || i == 2 || i == 3) {
            reanim->PlayReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, 12.0f);
        }
        if (i == 4) {
            reanim->PlayReanim("anim_float", ReanimLoopType::REANIM_LOOP, 0, 2.0f); // 云儿飘得慢一些
        }
        reanim->Update(); // 一次Update是必要的，否则绘制出来是Empty
        mLeaderboardReanimations->backgroundReanim[i] = reanim;
    }
    mLeaderboardReanimations->backgroundReanim[1]->AssignRenderGroupToTrack("survival button 1", 1);                               // 设置无尽模式按钮
    mLeaderboardReanimations->backgroundReanim[1]->SetImageOverride("survival button 1", addonImages.survival_button); // 设置无尽模式按钮
    Reanimation_HideTrack(mLeaderboardReanimations->backgroundReanim[1], "house 1", true);                                         // 隐藏默认房屋
    Reanimation_HideTrack(mLeaderboardReanimations->backgroundReanim[1], "house achievements 1", true);                            // 隐藏默认房屋
    //    Reanimation_HideTrack(this_->mLeaderboardReanimations->backgroundReanim[1],"house 2",true); // 隐藏默认房屋
    //    Reanimation_HideTrack(this_->mLeaderboardReanimations->backgroundReanim[1],"house achievements 2",true); // 隐藏默认房屋

    int zombieTrackIndex = mLeaderboardReanimations->backgroundReanim[0]->FindTrackIndex("zombie_trash");
    SexyTransform2D zombieSexyTransform2D;
    mLeaderboardReanimations->backgroundReanim[0]->GetTrackMatrix(zombieTrackIndex, zombieSexyTransform2D);
    mZombieTrashBin = new TrashBin(TrashBin::ZOMBIE_PILE, theApp->mPlayerInfo->mGameStats.mMiscStats[GameStats::ZOMBIES_KILLED] / 125.0f);
    mZombieTrashBin->Move(zombieSexyTransform2D.m[0][2], zombieSexyTransform2D.m[1][2]);

    int plantTrackIndex = mLeaderboardReanimations->backgroundReanim[0]->FindTrackIndex("plant_trash");
    SexyTransform2D plantSexyTransform2D;
    mLeaderboardReanimations->backgroundReanim[0]->GetTrackMatrix(plantTrackIndex, plantSexyTransform2D);
    mPlantTrashBin = new TrashBin(TrashBin::PLANT_PILE, theApp->mPlayerInfo->mGameStats.mMiscStats[GameStats::PLANTS_KILLED] / 125.0f);
    mPlantTrashBin->Move(plantSexyTransform2D.m[0][2], plantSexyTransform2D.m[1][2]);

    for (int i = 0; i < AchievementId::MAX_ACHIEVEMENTS; ++i) {
        mAchievements[i] = theApp->mPlayerInfo->mAchievements[LeaderboardsWidget_GetAchievementIdByReanimationType((ReanimationType)(ReanimationType::REANIM_ACHIEVEMENT_HOME_SECURITY + i))];
//        Reanimation *reanim = (Reanimation *)operator new(sizeof(Reanimation));
//        Reanimation_Reanimation(reanim);
        Reanimation *reanim = new Reanimation;
        reanim->ReanimationInitializeType(0.0, 0.0, (ReanimationType)(ReanimationType::REANIM_ACHIEVEMENT_HOME_SECURITY + i));
        reanim->SetPosition(gLeaderboardAchievementsPosition[i][0], gLeaderboardAchievementsPosition[i][1]);
        reanim->mLoopType = ReanimLoopType::REANIM_LOOP;
        reanim->Update(); // 一次Update是必要的，否则绘制出来是Empty
        mLeaderboardReanimations->achievementReanim[i] = reanim;
    }

    mLongestRecordPool = theApp->mPlayerInfo->mChallengeRecords[GameMode::GAMEMODE_SURVIVAL_ENDLESS_STAGE_3 - 2];
    //    this_->mLongestRecordPool = theApp->mPlayerInfo->mGameStats.mMiscStats[GameStats::ENDLESS_FLAGS];
    LOG_DEBUG("mLongestRecordPool {} ", mLongestRecordPool);

    auto *mLeaderboardsListener = new Sexy::ButtonListener();
    mButtonListener = mLeaderboardsListener;
    LOG_DEBUG("mButtonListener {} ", (void*)mButtonListener);
    auto *mVTable = (Sexy::ButtonListenerVTable *)operator new(sizeof(Sexy::ButtonListenerVTable));
    mLeaderboardsListener->vTable = mVTable;
    //    mVTable->ButtonPress = (void *) LeaderboardsWidget_ButtonPress;
    mVTable->ButtonPress2 = (void *)LeaderboardsWidget_ButtonPress;
    mVTable->ButtonDepress = (void *)LeaderboardsWidget_ButtonDepress;
    mVTable->ButtonMouseEnter = (void *)Sexy_ButtonListener_ButtonMouseEnter;
    mVTable->ButtonMouseMove = (void *)Sexy_ButtonListener_ButtonMouseMove;
    mVTable->ButtonMouseLeave = (void *)Sexy_ButtonListener_ButtonMouseLeave;
    mVTable->ButtonDownTick = (void *)Sexy_ButtonListener_ButtonDownTick;
    pvzstl::string str = TodStringTranslate("[CLOSE]");
    Sexy::GameButton *aBackButton = MakeButton(1000, mLeaderboardsListener, this, (SexyString &)str);
    LOG_DEBUG("aBackButton {} ", (void*)aBackButton);
    aBackButton->Resize(1040, 590, 120, 50);
    AddWidget((Sexy::Widget*)aBackButton);
    mBackButton = aBackButton;
    mFocusedAchievementIndex = 0;
    mHighLightAchievement = false;
}

void DaveHelp_Update(LeaderboardsWidget *leaderboardsWidget) {
    for (Reanimation *&i : leaderboardsWidget->mLeaderboardReanimations->backgroundReanim) {
        i->Update();
    }

    for (int i = 0; i < AchievementId::MAX_ACHIEVEMENTS; ++i) {
        if (!leaderboardsWidget->mAchievements[i])
            continue;
        leaderboardsWidget->mLeaderboardReanimations->achievementReanim[i]->Update();
    }
    leaderboardsWidget->MarkDirty();
}

void DaveHelp_Draw(LeaderboardsWidget *leaderboardsWidget, Sexy::Graphics *g) {
    for (int i = 4; i >= 0; i--) {
        leaderboardsWidget->mLeaderboardReanimations->backgroundReanim[i]->DrawRenderGroup(g, 0);
    }

    leaderboardsWidget->mPlantTrashBin->TrashBin::Draw(g);
    leaderboardsWidget->mZombieTrashBin->TrashBin::Draw(g);

    for (int i = 0; i < AchievementId::MAX_ACHIEVEMENTS; ++i) {
        int num = LeaderboardsWidget_GetAchievementIdByDrawOrder(i);
        if (!leaderboardsWidget->mAchievements[num])
            continue;
        if (leaderboardsWidget->mHighLightAchievement && num == leaderboardsWidget->mFocusedAchievementIndex) {
            auto id = AchievementId(LeaderboardsWidget_GetAchievementIdByReanimationType(ReanimationType(num + ReanimationType::REANIM_ACHIEVEMENT_HOME_SECURITY))
                                    + AchievementId::ACHIEVEMENT_HOME_SECURITY);
            Sexy::Image *image = GetIconByAchievementId(id);
            Color color = GetFlashingColor(leaderboardsWidget->mApp->mAppCounter, 120);
            g->SetColorizeImages(true);
            g->SetColor(color);
            leaderboardsWidget->mLeaderboardReanimations->achievementReanim[num]->Draw(g);
            g->SetColorizeImages(false);
            int offsetX = gLeaderboardAchievementsPosition[num][0] + 20;
            int offsetY = gLeaderboardAchievementsPosition[num][1] - 200;
            g->DrawImage(image, offsetX, offsetY);
            pvzstl::string str = StrFormat("[%s]", GetNameByAchievementId(id));
            Sexy::Rect rect = {offsetX - 42, offsetY + 125, 200, 200};
            Color theColor = {0, 255, 0, 255};
            TodDrawStringWrapped(g, str, rect, *Sexy_FONT_HOUSEOFTERROR28_Addr, theColor, DrawStringJustification::DS_ALIGN_CENTER, false);
        } else {
            leaderboardsWidget->mLeaderboardReanimations->achievementReanim[num]->Draw(g);
        }
    }

    if (leaderboardsWidget->mApp->HasFinishedAdventure()) {
        leaderboardsWidget->mLeaderboardReanimations->backgroundReanim[1]->DrawRenderGroup(g, 1);
        pvzstl::string str = TodStringTranslate("[LEADERBOARD_STREAK]");
        pvzstl::string str1 = TodReplaceNumberString(str, "{STREAK}", leaderboardsWidget->mLongestRecordPool);
        Sexy::Rect theRect = {317, 658, 120, 50};
        Sexy::Font *theFont = *Sexy_FONT_CONTINUUMBOLD14_Addr;
        TodDrawStringWrapped(g, str1, theRect, theFont, yellow, DrawStringJustification::DS_ALIGN_CENTER, false);
    }

    //    DrawImage(g, addonImages.survival_button, 270, 579);

    pvzstl::string  str2 = TodStringTranslate("[PLAYERS_HOUSE]");
    Sexy::Rect theRect1 = {240, 70, 800, 70};
    pvzstl::string str3 = StrFormat("%s", leaderboardsWidget->mApp->mPlayerInfo->mName);
    pvzstl::string str4 = TodReplaceString(str2, "{PLAYER}", str3);
    Sexy::Font *theFont1 = *Sexy_FONT_HOUSEOFTERROR28_Addr;
    TodDrawStringWrapped(g, str4, theRect1, theFont1, white, DrawStringJustification::DS_ALIGN_CENTER, false);

    //    int plantHeight = plantPileHeight * leaderboardsWidget->mPlantTrashBin->mPileNum;
    //    int zombieHeight = zombiePileHeight * leaderboardsWidget->mZombieTrashBin->mPileNum;
    //    Rect plantTrashBinRect = {leaderboardsWidget->mPlantTrashBin->mX,leaderboardsWidget->mPlantTrashBin->mY - plantHeight,addonImages.plant_can->mWidth,addonImages.plant_can->mHeight +
    //    plantHeight}; Rect zombieTrashBinRect = {leaderboardsWidget->mZombieTrashBin->mX,leaderboardsWidget->mZombieTrashBin->mY -
    //    zombieHeight,addonImages.zombie_can->mWidth,addonImages.zombie_can->mHeight + zombieHeight};
    //
    //    SetColor(g, &yellow);
    //    DrawRect(g, &plantTrashBinRect);
    //
    //    SetColor(g, &green);
    //    DrawRect(g, &zombieTrashBinRect);


    //    SetColor(g, &yellow);
    //    Rect rect = {xx,yy,xw,yh};
    //    DrawRect(g, &rect);
    //
    //    SetColor(g, &green);
    //    Rect rect2 = {xx1,yy1,xw1,yh1};
    //    DrawRect(g, &rect2);
    //    if (LawnApp_EarnedGoldTrophy(leaderboardsWidget->mApp)) {
    //        DrawImageCeliiii(g, *Sexy_IMAGE_SUNFLOWER_TROPHY_Addr, 1110, 290, 1, 0);
    //    } else if (LawnApp_HasFinishedAdventure(leaderboardsWidget->mApp)) {
    //        DrawImageCeliiii(g, *Sexy_IMAGE_SUNFLOWER_TROPHY_Addr, 1110, 290, 0, 0);
    //    }
}

void DaveHelp_Delete2(LeaderboardsWidget *leaderboardsWidget) {
    leaderboardsWidget->mZombieTrashBin->Delete();
    leaderboardsWidget->mPlantTrashBin->Delete();
    for (int i = 0; i < 5; ++i) {
        delete leaderboardsWidget->mLeaderboardReanimations->backgroundReanim[i];
    }
    for (int i = 0; i < AchievementId::MAX_ACHIEVEMENTS; ++i) {
        delete leaderboardsWidget->mLeaderboardReanimations->achievementReanim[i];
    }
    leaderboardsWidget->mBackButton->~GameButton();
    operator delete(leaderboardsWidget->mButtonListener->vTable);
    operator delete(leaderboardsWidget->mButtonListener);
    operator delete(leaderboardsWidget->mLeaderboardReanimations);

    old_DaveHelp_Delete2(leaderboardsWidget);
}

void DaveHelp_MouseDown(LeaderboardsWidget *leaderboardsWidget, int x, int y, int theClickCount) {
    for (int i = 0; i < AchievementId::MAX_ACHIEVEMENTS; ++i) {
        int num = LeaderboardsWidget_GetAchievementIdByDrawOrder(AchievementId::MAX_ACHIEVEMENTS - 1 - i);
        if (!leaderboardsWidget->mAchievements[num])
            continue;
        if (TRect_Contains(&gLeaderboardAchievementsRect[num][0], x, y) || TRect_Contains(&gLeaderboardAchievementsRect[num][1], x, y)) {
            if (leaderboardsWidget->mFocusedAchievementIndex == num && leaderboardsWidget->mHighLightAchievement) {
                leaderboardsWidget->mHighLightAchievement = false;
            } else {
                leaderboardsWidget->mFocusedAchievementIndex = num;
                leaderboardsWidget->mHighLightAchievement = true;
            }
            return;
        }
    }

    int plantHeight = plantPileHeight * leaderboardsWidget->mPlantTrashBin->mPileNum;
    Sexy::Rect plantTrashBinRect = {
        leaderboardsWidget->mPlantTrashBin->mX, leaderboardsWidget->mPlantTrashBin->mY - plantHeight, addonImages.plant_can->mWidth, addonImages.plant_can->mHeight + plantHeight};

    if (TRect_Contains(&plantTrashBinRect, x, y)) {
        LawnApp_PlaySample(leaderboardsWidget->mApp, *Sexy_SOUND_GRAVEBUTTON_Addr);
        pvzstl::string str1 = TodStringTranslate("[PLANTS_KILLED]");
        pvzstl::string str2 = TodReplaceNumberString(str1, "{PLANTS}", leaderboardsWidget->mApp->mPlayerInfo->mGameStats.mMiscStats[GameStats::PLANTS_KILLED]);
        leaderboardsWidget->mApp->LawnMessageBox(Dialogs::DIALOG_MESSAGE, str2.c_str(), "", "[DIALOG_BUTTON_OK]", "", 3);
        return;
    }

    int zombieHeight = zombiePileHeight * leaderboardsWidget->mZombieTrashBin->mPileNum;
    Sexy::Rect zombieTrashBinRect = {
        leaderboardsWidget->mZombieTrashBin->mX, leaderboardsWidget->mZombieTrashBin->mY - zombieHeight, addonImages.zombie_can->mWidth, addonImages.zombie_can->mHeight + zombieHeight};

    if (TRect_Contains(&zombieTrashBinRect, x, y)) {
        LawnApp_PlaySample(leaderboardsWidget->mApp, *Sexy_SOUND_GRAVEBUTTON_Addr);
        pvzstl::string str1 = TodStringTranslate("[ZOMBIES_KILLED]");
        pvzstl::string str2 = TodReplaceNumberString(str1, "{ZOMBIES}", leaderboardsWidget->mApp->mPlayerInfo->mGameStats.mMiscStats[GameStats::ZOMBIES_KILLED]);
        leaderboardsWidget->mApp->LawnMessageBox(Dialogs::DIALOG_MESSAGE, str2.c_str(), "", "[DIALOG_BUTTON_OK]", "", 3);
        return;
    }

    //    tmp = !tmp;
    //    if (tmp) {
    //        xx = x;
    //        yy = y;
    //        xw = 0;
    //        yh = 0;
    //        LOGD("%d %d", x, y);
    //    }else{
    //        xx1 = x;
    //        yy1 = y;
    //        xw1 = 0;
    //        yh1 = 0;
    //        LOGD("%d %d", x, y);
    //    }

    //    Rect rect = {1066, 574, 72, 72};
    //    if (TRect_Contains(&rect, x, y)) {
    //        leaderboardsWidget->mTouchDownInBackRect = true;
    //        LawnApp_PlaySample(leaderboardsWidget->mApp,*Sexy_SOUND_GRAVEBUTTON_Addr);
    //    }
}

void DaveHelp_MouseDrag(LeaderboardsWidget *leaderboardsWidget, int x, int y) {
    //    if (tmp) {
    //        xw = x - xx;
    //        yh = y - yy;
    //        LOGD("%d: %d, %d, %d, %d",leaderboardsWidget->mFocusedAchievementIndex,xx,yy,xw,yh);
    //    }else{
    //        xw1 = x - xx1;
    //        yh1 = y - yy1;
    //        LOGD("%d: %d, %d, %d, %d",leaderboardsWidget->mFocusedAchievementIndex,xx1,yy1,xw1,yh1);
    //    }
}

void DaveHelp_MouseUp(LeaderboardsWidget *leaderboardsWidget, int x, int y) {}

void DaveHelp_DealClick(LeaderboardsWidget *leaderboardsWidget, int id) {}

void DaveHelp_KeyDown(LeaderboardsWidget *leaderboardsWidget, int keyCode) {
    if (keyCode == Sexy::KEYCODE_ESCAPE || keyCode == Sexy::KEYCODE_ESCAPE2) {
        if (leaderboardsWidget->mHighLightAchievement) {
            leaderboardsWidget->mHighLightAchievement = false;
            return;
        }
        leaderboardsWidget->mApp->KillLeaderboards();
        leaderboardsWidget->mApp->ShowMainMenuScreen();
        return;
    }
    if (keyCode == Sexy::KEYCODE_UP || keyCode == Sexy::KEYCODE_DOWN || keyCode == Sexy::KEYCODE_LEFT || keyCode == Sexy::KEYCODE_RIGHT) {
        bool flag = false;
        for (bool mAchievement : leaderboardsWidget->mAchievements) {
            if (mAchievement) {
                flag = true;
                break;
            }
        }
        if (!flag) {
            return;
        }

        leaderboardsWidget->mHighLightAchievement = true;
        int mFocusedIndex = leaderboardsWidget->mFocusedAchievementIndex;
        if (keyCode == Sexy::KEYCODE_UP || keyCode == Sexy::KEYCODE_LEFT) {
            do {
                mFocusedIndex++;
                if (mFocusedIndex > 11) {
                    mFocusedIndex = 0;
                }
            } while (!leaderboardsWidget->mAchievements[mFocusedIndex]);
        } else {
            do {
                mFocusedIndex--;
                if (mFocusedIndex < 0) {
                    mFocusedIndex = 11;
                }
            } while (!leaderboardsWidget->mAchievements[mFocusedIndex]);
        }
        leaderboardsWidget->mFocusedAchievementIndex = mFocusedIndex;
        return;
    }
    if (keyCode == Sexy::KEYCODE_SHOVEL) {
        LawnApp_PlaySample(leaderboardsWidget->mApp, *Sexy_SOUND_GRAVEBUTTON_Addr);
        pvzstl::string str1 = TodStringTranslate("[PLANTS_KILLED]");
        pvzstl::string str2 = TodReplaceNumberString(str1, "{PLANTS}", leaderboardsWidget->mApp->mPlayerInfo->mGameStats.mMiscStats[GameStats::PLANTS_KILLED]);
        leaderboardsWidget->mApp->LawnMessageBox(Dialogs::DIALOG_MESSAGE, str2.c_str(), "", "[DIALOG_BUTTON_OK]", "", 3);
        return;
    }
    if (keyCode == Sexy::KEYCODE_HAMMER) {
        LawnApp_PlaySample(leaderboardsWidget->mApp, *Sexy_SOUND_GRAVEBUTTON_Addr);
        pvzstl::string str1 = TodStringTranslate("[ZOMBIES_KILLED]");
        pvzstl::string str2 = TodReplaceNumberString(str1, "{ZOMBIES}", leaderboardsWidget->mApp->mPlayerInfo->mGameStats.mMiscStats[GameStats::ZOMBIES_KILLED]);
        leaderboardsWidget->mApp->LawnMessageBox(Dialogs::DIALOG_MESSAGE, str2.c_str(), "", "[DIALOG_BUTTON_OK]", "", 3);
        return;
    }
}