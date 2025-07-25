#include "PvZ/Lawn/GamepadControls.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Board/Challenge.h"
#include "PvZ/Lawn/Board/CursorObject.h"
#include "PvZ/Lawn/Board/Plant.h"
#include "PvZ/Lawn/Board/SeedBank.h"
#include "PvZ/Lawn/Board/ZenGarden.h"
#include "PvZ/Lawn/Board/Zombie.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/MagicAddr.h"
#include "PvZ/Misc.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Effect/Attachment.h"
#include "PvZ/TodLib/Effect/Reanimator.h"

using namespace Sexy;

GamepadControls::GamepadControls(Board *theBoard, int thePlayerIndex1, int thePlayerIndex2) {
    __Constructor(theBoard, thePlayerIndex1, thePlayerIndex2);
}

void GamepadControls::__Constructor(Board *theBoard, int thePlayerIndex1, int thePlayerIndex2) {
    old_GamepadControls_GamepadControls(this, theBoard, thePlayerIndex1, thePlayerIndex2);

    if (isKeyboardTwoPlayerMode)
        return;

    GameMode aGameMode = mGameObject.mApp->mGameMode;
    bool isTwoSeedBankMode = (aGameMode == GameMode::GAMEMODE_MP_VS || (aGameMode >= GameMode::GAMEMODE_TWO_PLAYER_COOP_DAY && aGameMode <= GameMode::GAMEMODE_TWO_PLAYER_COOP_ENDLESS));
    if (!keyboardMode && !isTwoSeedBankMode && aGameMode != GameMode::GAMEMODE_CHALLENGE_SLOT_MACHINE) {
        mIsInShopSeedBank = true; // 是否在Shop栏。
    }
}

void GamepadControls_pickUpCobCannon(int gamePad, int cobCannon) {
    // 用于拿起指定的加农炮
    int v8;        // r6
    int v9;        // r1
    int *v10;      // r0
    int v11;       // r3
    int v12;       // r3
    int v13;       // r7
    int v14;       // r6
    int v15;       // r1
    int v16;       // s14
    int v17;       // s15
    uint32_t *v18; // r3
    int v19;       // r3

    v8 = 0;
    v9 = *((uint32_t *)gamePad + 37);
    v10 = (int *)*((uint32_t *)gamePad + 14);
    if (v9)
        v11 = 140;
    else
        v11 = 141;
    v12 = *((uint32_t *)v10 + v11);
    if (*(uint32_t *)(v12 + 152) != -1) {
        if (*(uint8_t *)(v12 + 200)) {
            v19 = *(uint32_t *)(v12 + 196);
            if (v19) {
                if ((unsigned int)(unsigned int)v19 < *((uint32_t *)v10 + 79)) {
                    if (v19 == *(uint32_t *)(*((uint32_t *)v10 + 77) + 352 * (unsigned int)v19 + 348))
                        v8 = *((uint32_t *)v10 + 77) + 352 * (unsigned int)v19;
                    else
                        v8 = 0;
                }
            } else {
                v8 = 0;
            }
        } else {
            v8 = *(uint8_t *)(v12 + 200);
        }
    }
    v13 = cobCannon;
    if (cobCannon != v8 && *(uint32_t *)(cobCannon + 76) == 37 && *((uint32_t *)gamePad + 24) != 8) {
        v14 = *((uint8_t *)gamePad + 200);
        if (!*((uint8_t *)gamePad + 200)) {
            Board *aBoard = reinterpret_cast<Board *>(v10); // 显式类型转换
            aBoard->ClearCursor(v9);
            v15 = *(uint32_t *)(v13 + 348);
            v16 = (int)*((float *)gamePad + 27);
            v17 = (int)*((float *)gamePad + 28);
            v18 = (uint32_t *)(*((uint32_t *)gamePad + 14) + 22528);
            v18[29] = 30;
            v18[30] = v16;
            v18[31] = v17;
            *((uint32_t *)gamePad + 49) = v15;
            *((uint32_t *)gamePad + 55) = v14;
            *((uint8_t *)gamePad + 200) = 1;
        }
    }
}

void GamepadControls::Draw(Sexy::Graphics *g) {
    // 实现在光标内绘制铲子和黄油手套(黄油手套其实就是花园的手套),并在锤僵尸关卡绘制种植预览

    if (mPlayerIndex2 != -1) {
        LawnApp *aApp = mGameObject.mApp;
        bool is2P = mPlayerIndex1 == 1;
        CursorObject *aCursorObject = is2P ? mBoard->mCursorObject2 : mBoard->mCursorObject1;
        if (is2P) {
            if (requestDrawButterInCursor) {
                if (!aApp->IsCoopMode())
                    requestDrawButterInCursor = false;
                g->DrawImage(addonImages.butter_glove, mCursorPositionX, mCursorPositionY);
            }
        } else {
            if (requestDrawShovelInCursor) {
                if (!mBoard->mShowShovel)
                    requestDrawShovelInCursor = false;
                if (aApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_LAST_STAND) {
                    if (mBoard->mChallenge->mChallengeState == ChallengeState::STATECHALLENGE_NORMAL && aApp->mGameScene == GameScenes::SCENE_PLAYING) {
                        aCursorObject->mCursorType = CursorType::CURSOR_TYPE_MONEY_SIGN;
                        aCursorObject->mX = mCursorPositionX;
                        aCursorObject->mY = mCursorPositionY;
                    } else {
                        aCursorObject->mCursorType = CursorType::CURSOR_TYPE_SHOVEL;
                        aCursorObject->mX = mCursorPositionX - 20;
                        aCursorObject->mY = mCursorPositionY - 20;
                    }
                } else {
                    aCursorObject->mCursorType = CursorType::CURSOR_TYPE_SHOVEL;
                    aCursorObject->mX = mCursorPositionX - 20;
                    aCursorObject->mY = mCursorPositionY - 20;
                }

                if (aCursorObject->BeginDraw(g)) {
                    aCursorObject->Draw(g);
                    aCursorObject->EndDraw(g);
                }
            }
        }
        if (aApp->IsWhackAZombieLevel()) {
            int theGridX = mBoard->PixelToGridXKeepOnBoard(mCursorPositionX, mCursorPositionY);
            int theGridY = mBoard->PixelToGridYKeepOnBoard(mCursorPositionX, mCursorPositionY);
            int thePixelX = mBoard->GridToPixelX(theGridX, theGridY);
            int thePixelY = mBoard->GridToPixelY(theGridX, theGridY);
            g->mTransX += thePixelX;
            g->mTransY += thePixelY;
            DrawPreview(g);
            g->mTransX -= thePixelX;
            g->mTransY -= thePixelY;
        }
    }
    if (mIsCobCannonSelected && useNewCobCannon && !keyboardMode) {
        Sexy::Image *cobcannon_1 = *Sexy_IMAGE_COBCANNON_TARGET_1_Addr;
        *Sexy_IMAGE_COBCANNON_TARGET_1_Addr = addonImages.custom_cobcannon;
        old_GamepadControls_Draw(this, g);
        *Sexy_IMAGE_COBCANNON_TARGET_1_Addr = cobcannon_1;
        return;
    }

    return old_GamepadControls_Draw(this, g);
}

void GamepadControls::Update(float a2) {
    LawnApp *aApp = mGameObject.mApp;
    int theGridX = mBoard->PixelToGridXKeepOnBoard(mCursorPositionX, mCursorPositionY);
    int theGridY = mBoard->PixelToGridYKeepOnBoard(mCursorPositionX, mCursorPositionY);
    int mGridCenterPositionX = mBoard->GridToPixelX(theGridX, theGridY) + mBoard->GridCellWidth(theGridX, theGridY) / 2;
    int mGridCenterPositionY = mBoard->GridToPixelY(theGridX, theGridY) + mBoard->GridCellHeight(theGridX, theGridY) / 2;
    // 键盘双人模式 平滑移动光标
    if (isKeyboardTwoPlayerMode) {
        if (mPlayerIndex2 == 0) {
            mGamepadVelocityLeftX = GamepadVelocityXOfPlayer1;
            mGamepadVelocityLeftY = GamepadVelocityYOfPlayer1;
            if (GamepadVelocityXOfPlayer1 == 0) {
                mCursorPositionX += (mGridCenterPositionX - mCursorPositionX) / 10;
            }
            if (GamepadVelocityYOfPlayer1 == 0) {
                mCursorPositionY += (mGridCenterPositionY - mCursorPositionY) / 10;
            }
        } else if (mPlayerIndex2 == 1) {
            mGamepadVelocityLeftX = GamepadVelocityXOfPlayer2;
            mGamepadVelocityLeftY = GamepadVelocityYOfPlayer2;
            if (GamepadVelocityXOfPlayer2 == 0) {
                mCursorPositionX += (mGridCenterPositionX - mCursorPositionX) / 10;
            }
            if (GamepadVelocityYOfPlayer2 == 0) {
                mCursorPositionY += (mGridCenterPositionY - mCursorPositionY) / 10;
            }
        }
    }

    if (positionAutoFix && !aApp->IsWhackAZombieLevel() && aApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZOMBIQUARIUM) {
        if (mPlayerIndex2 == 0 && gPlayerIndex != TouchPlayerIndex::TOUCHPLAYER_PLAYER1 && gPlayerIndexSecond != TouchPlayerIndex::TOUCHPLAYER_PLAYER1) {
            mCursorPositionX += (mGridCenterPositionX - mCursorPositionX) / 10;
            mCursorPositionY += (mGridCenterPositionY - mCursorPositionY) / 10;
        }
        if (mPlayerIndex2 == 1 && gPlayerIndex != TouchPlayerIndex::TOUCHPLAYER_PLAYER2 && gPlayerIndexSecond != TouchPlayerIndex::TOUCHPLAYER_PLAYER2) {
            mCursorPositionX += (mGridCenterPositionX - mCursorPositionX) / 10;
            mCursorPositionY += (mGridCenterPositionY - mCursorPositionY) / 10;
        }
    }


    old_GamepadControls_Update(this, a2);

    //    Reanimation *mCursorReanim = ReanimationTryToGet(gamepadControls->mGameObject.aApp, gamepadControls->mCursorReanimID);
    //    LOGD("%d",mCursorReanim);
    //    if (mCursorReanim != nullptr) {
    //        if ((gamepadControls->mPlayerIndex2 == 0 &&(mIsZombie == TouchPlayerIndex::TOUCHPLAYER_PLAYER1 || gPlayerIndexSecond == TouchPlayerIndex::TOUCHPLAYER_PLAYER1)) ||
    //        (gamepadControls->mPlayerIndex2 == 1
    //        &&(mIsZombie == TouchPlayerIndex::TOUCHPLAYER_PLAYER2 || gPlayerIndexSecond == TouchPlayerIndex::TOUCHPLAYER_PLAYER2))) {
    //            if (!Reanimation_IsAnimPlaying(mCursorReanim, "anim_depressed"))
    //                Reanimation_PlayReanim(mCursorReanim, "anim_depressed", a::REANIM_LOOP, 0,12.0);
    //            LOGD("456456");
    //        } else if (!Reanimation_IsAnimPlaying(mCursorReanim, "anim_bounce")) {
    //            Reanimation_PlayReanim(mCursorReanim, "anim_bounce", a::REANIM_LOOP, 0, 12.0);
    //        }
    //    }

    if (!isKeyboardTwoPlayerMode && !aApp->CanShopLevel() && mGamepadState == 6 && mIsInShopSeedBank) {
        mIsInShopSeedBank = false;
    }
}

void GamepadControls::ButtonDownFireCobcannonTest() {
    // 解除加农炮选取半秒后才能发射的限制
    mBoard->mCobCannonCursorDelayCounter = 0;

    return old_GamepadControls_ButtonDownFireCobcannonTest(this);
}

void GamepadControls_InvalidatePreviewReanim(GamepadControls *gamepadControls) {
    Reanimation *mPreviewReanim4 = gamepadControls->mGameObject.mApp->ReanimationTryToGet(gamepadControls->mPreviewReanimID4);
    if (mPreviewReanim4 != nullptr) {
        mPreviewReanim4->ReanimationDie();
        gamepadControls->mPreviewReanimID4 = ReanimationID::REANIMATIONID_NULL;
    }
    ReanimationID mPreviewReanim3 = gamepadControls->mPreviewReanimID3;
    if (mPreviewReanim3 != 0) {
        gamepadControls->mGameObject.mApp->RemoveReanimation(mPreviewReanim3);
        gamepadControls->mPreviewReanimID3 = REANIMATIONID_NULL;
    }
}

FilterEffect GetFilterEffectTypeBySeedType(SeedType mSeedType) {
    if (mSeedType == SeedType::SEED_HYPNOSHROOM || mSeedType == SeedType::SEED_SQUASH || mSeedType == SeedType::SEED_POTATOMINE || mSeedType == SeedType::SEED_GARLIC
        || mSeedType == SeedType::SEED_LILYPAD) {
        return FilterEffect::FILTEREFFECT_LESS_WASHED_OUT;
    }

    return FilterEffect::FILTEREFFECT_WASHED_OUT;
}

void GamepadControls::UpdatePreviewReanim() {
    // 动态预览!!

    // TV后续版本仅在PreviewingSeedType切换时进行一次Reanimation::Update，而TV 1.0.1则是无时无刻进行Reanimation::Update。我们恢复1.0.1的逻辑即可。

    LawnApp *aApp = mGameObject.mApp;
    CursorObject *aCursorObject = mPlayerIndex1 ? mBoard->mCursorObject2 : mBoard->mCursorObject1;

    if (!dynamicPreview) { // 如果没开启动态预览，则开启砸罐子无尽和锤僵尸关卡的动态预览，并执行旧游戏函数。
        if ((aApp->IsWhackAZombieLevel() || aApp->IsScaryPotterLevel()) && mGamepadState == 7) {
            SeedBank *seedBank = GetSeedBank();
            SeedPacket *seedPacket = &seedBank->mSeedPackets[mSelectedSeedIndex];
            aCursorObject->mType = seedPacket->mPacketType;
            aCursorObject->mImitaterType = seedPacket->mImitaterType;
        }
        return old_GamepadControls_UpdatePreviewReanim(this);
    }

    GameMode aGameMode = aApp->mGameMode;
    int theGridX = mBoard->PixelToGridXKeepOnBoard(mCursorPositionX, mCursorPositionY);
    int theGridY = mBoard->PixelToGridYKeepOnBoard(mCursorPositionX, mCursorPositionY);
    SeedBank *seedBank = GetSeedBank();
    SeedType mSeedType = aCursorObject->mType;
    bool isImitater = seedBank->mSeedPackets[mSelectedSeedIndex].mPacketType == SeedType::SEED_IMITATER;

    if ((aApp->IsWhackAZombieLevel() || aGameMode == GameMode::GAMEMODE_SCARY_POTTER_ENDLESS) && mGamepadState == 7) {
        // 开启砸罐子无尽和锤僵尸关卡的动态预览
        SeedBank *seedBank = GetSeedBank();
        SeedPacket *seedPacket = &seedBank->mSeedPackets[mSelectedSeedIndex];
        aCursorObject->mType = seedPacket->mPacketType;
        aCursorObject->mImitaterType = seedPacket->mImitaterType;
    }

    if (mIsZombie) {
        mSeedType = seedBank->mSeedPackets[mSelectedSeedIndex].mPacketType;
    }
    if (mSeedType == SeedType::SEED_IMITATER) {
        mSeedType = aCursorObject->mImitaterType;
    }

    bool flagUpdateCanPlant = true;
    bool flagDrawGray = false;
    bool flagUpdateChangeType = false;
    if (mPreviewingSeedType != mSeedType && mSeedType != SeedType::SEED_NONE) {
        Reanimation *theNewPreviewingReanim = nullptr;
        GamepadControls_InvalidatePreviewReanim(this);
        RenderLayer aRenderLayer = mIsZombie ? RENDER_LAYER_ZOMBIE : RENDER_LAYER_PLANT;
        int aRenderOrder = Board::MakeRenderOrder(aRenderLayer, theGridY, 100);
        float theDrawHeightOffset = PlantDrawHeightOffset(mBoard, 0, mSeedType, theGridX, theGridY);
        if (mIsZombie) {
            ZombieType theZombieType = Challenge::IZombieSeedTypeToZombieType(mSeedType);
            switch (theZombieType) {
                case ZombieType::ZOMBIE_INVALID:
                    mPreviewingSeedType = mSeedType;
                    return;
                case ZombieType::ZOMBIE_GARGANTUAR:
                case ZombieType::ZOMBIE_REDEYE_GARGANTUAR: // 在对战里可能用得到
                    theDrawHeightOffset += 30.0;
                    break;
                case ZombieType::ZOMBIE_POLEVAULTER:
                    theDrawHeightOffset += 15.0;
                    break;
            }
            ZombieDefinition &theZombieDefinition = GetZombieDefinition(theZombieType);
            Reanimation *zombieReanim = aApp->AddReanimation(-20.0, -35 - theDrawHeightOffset, aRenderOrder + 1, theZombieDefinition.mReanimationType);
            Zombie::SetupReanimLayers(zombieReanim, theZombieType);
            if (theZombieType == ZombieType::ZOMBIE_DOOR || theZombieType == ZombieType::ZOMBIE_TRASH_BIN || theZombieType == ZombieType::ZOMBIE_NEWSPAPER
                || theZombieType == ZombieType::ZOMBIE_LADDER) {
                Zombie::SetupShieldReanims(theZombieType, zombieReanim);
            }
            zombieReanim->mIsAttachment = true;
            if (theZombieType == ZombieType::ZOMBIE_POGO) {
                zombieReanim->PlayReanim("anim_pogo", ReanimLoopType::REANIM_LOOP, 0, 12.0);
            } else if (theZombieType == ZombieType::ZOMBIE_DANCER) {
                zombieReanim->PlayReanim("anim_armraise", ReanimLoopType::REANIM_LOOP, 0, 12.0);
            } else if (theZombieType == ZombieType::ZOMBIE_ZAMBONI) {
                zombieReanim->PlayReanim("anim_drive", ReanimLoopType::REANIM_LOOP, 0, 12.0);
            } else if (theZombieType == ZombieType::ZOMBIE_IMP) {
                zombieReanim->PlayReanim("anim_walk", ReanimLoopType::REANIM_LOOP, 0, 12.0);
            } else {
                if (theZombieType == ZombieType::ZOMBIE_FLAG) {
                    Reanimation *zombieReanimAttachment = aApp->AddReanimation(0, 0, 0, ReanimationType::REANIM_ZOMBIE_FLAGPOLE);
                    zombieReanimAttachment->PlayReanim("Zombie_flag", ReanimLoopType::REANIM_LOOP, 0, 15.0);
                    mPreviewReanimID3 = aApp->ReanimationGetID(zombieReanimAttachment);
                    ReanimatorTrackInstance *TrackInstanceByName = zombieReanim->GetTrackInstanceByName("Zombie_flaghand");
                    AttachReanim(TrackInstanceByName->mAttachmentID, zombieReanimAttachment, 0.0, 0.0);
                    zombieReanim->mFrameBasePose = 0;
                } else if (theZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR) {
                    zombieReanim->SetImageOverride("anim_head1", *Sexy_IMAGE_REANIM_ZOMBIE_GARGANTUAR_HEAD_REDEYE_Addr);
                } else if (theZombieType == ZombieType::ZOMBIE_PEA_HEAD) {
                    Reanimation_HideTrackByPrefix(zombieReanim, "anim_hair", true);
                    Reanimation_HideTrackByPrefix(zombieReanim, "anim_head2", true);
                    zombieReanim->SetFramesForLayer("anim_walk2");
                    ReanimatorTrackInstance *aTrackInstance = zombieReanim->GetTrackInstanceByName("anim_head1");
                    aTrackInstance->mImageOverride = *IMAGE_BLANK;
                    Reanimation *aPeaHeadReanim = aApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_PEASHOOTER);
                    aPeaHeadReanim->PlayReanim("anim_head_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
                    AttachEffect *aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aPeaHeadReanim, 0.0f, 0.0f);
                    zombieReanim->mFrameBasePose = 0;
                    TodScaleRotateTransformMatrix((SexyMatrix3 &)aAttachEffect->mOffset, 65.0f, -8.0f, 0.2f, -1.0f, 1.0f);
                } else if (theZombieType == ZombieType::ZOMBIE_WALLNUT_HEAD) {
                    Reanimation_HideTrackByPrefix(zombieReanim, "anim_hair", true);
                    Reanimation_HideTrackByPrefix(zombieReanim, "anim_head", true);
                    Reanimation_HideTrackByPrefix(zombieReanim, "Zombie_tie", true);
                    zombieReanim->SetFramesForLayer("anim_walk2");
                    ReanimatorTrackInstance *aTrackInstance = zombieReanim->GetTrackInstanceByName("zombie_body");
                    Reanimation *aWallnutHeadReanim = aApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_WALLNUT);
                    aWallnutHeadReanim->PlayReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
                    AttachEffect *aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aWallnutHeadReanim, 0.0f, 0.0f);
                    zombieReanim->mFrameBasePose = 0;
                    TodScaleRotateTransformMatrix((SexyMatrix3 &)aAttachEffect->mOffset, 50.0f, 0.0f, 0.2f, -0.8f, 0.8f);
                } else if (theZombieType == ZombieType::ZOMBIE_TALLNUT_HEAD) {
                    Reanimation_HideTrackByPrefix(zombieReanim, "anim_hair", true);
                    Reanimation_HideTrackByPrefix(zombieReanim, "anim_head", true);
                    Reanimation_HideTrackByPrefix(zombieReanim, "Zombie_tie", true);
                    zombieReanim->SetFramesForLayer("anim_walk2");
                    ReanimatorTrackInstance *aTrackInstance = zombieReanim->GetTrackInstanceByName("zombie_body");
                    Reanimation *aTallnutHeadReanim = aApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_TALLNUT);
                    aTallnutHeadReanim->PlayReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
                    AttachEffect *aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aTallnutHeadReanim, 0.0f, 0.0f);
                    zombieReanim->mFrameBasePose = 0;
                    TodScaleRotateTransformMatrix((SexyMatrix3 &)aAttachEffect->mOffset, 37.0f, 0.0f, 0.2f, -0.8f, 0.8f);
                } else if (theZombieType == ZombieType::ZOMBIE_JALAPENO_HEAD) {
                    Reanimation_HideTrackByPrefix(zombieReanim, "anim_hair", true);
                    Reanimation_HideTrackByPrefix(zombieReanim, "anim_head", true);
                    Reanimation_HideTrackByPrefix(zombieReanim, "Zombie_tie", true);
                    zombieReanim->SetFramesForLayer("anim_walk2");
                    ReanimatorTrackInstance *aTrackInstance = zombieReanim->GetTrackInstanceByName("zombie_body");
                    Reanimation *aJalapenoHeadReanim = aApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_JALAPENO);
                    aJalapenoHeadReanim->PlayReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
                    AttachEffect *aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aJalapenoHeadReanim, 0.0f, 0.0f);
                    zombieReanim->mFrameBasePose = 0;
                    TodScaleRotateTransformMatrix((SexyMatrix3 &)aAttachEffect->mOffset, 55.0f, -5.0f, 0.2f, -1.0f, 1.0f);
                } else if (theZombieType == ZombieType::ZOMBIE_GATLING_HEAD) {
                    Reanimation_HideTrackByPrefix(zombieReanim, "anim_hair", true);
                    Reanimation_HideTrackByPrefix(zombieReanim, "anim_head2", true);
                    zombieReanim->SetFramesForLayer("anim_walk2");
                    ReanimatorTrackInstance *aTrackInstance = zombieReanim->GetTrackInstanceByName("anim_head1");
                    aTrackInstance->mImageOverride = *IMAGE_BLANK;
                    Reanimation *aGatlingHeadReanim = aApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_GATLINGPEA);
                    aGatlingHeadReanim->PlayReanim("anim_head_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
                    AttachEffect *aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aGatlingHeadReanim, 0.0f, 0.0f);
                    zombieReanim->mFrameBasePose = 0;
                    TodScaleRotateTransformMatrix((SexyMatrix3 &)aAttachEffect->mOffset, 65.0f, -5.0f, 0.2f, -1.0f, 1.0f);
                } else if (theZombieType == ZombieType::ZOMBIE_SQUASH_HEAD) {
                    Reanimation_HideTrackByPrefix(zombieReanim, "anim_hair", true);
                    Reanimation_HideTrackByPrefix(zombieReanim, "anim_head2", true);
                    zombieReanim->SetFramesForLayer("anim_walk2");
                    ReanimatorTrackInstance *aTrackInstance = zombieReanim->GetTrackInstanceByName("anim_head1");
                    aTrackInstance->mImageOverride = *IMAGE_BLANK;
                    Reanimation *aSquashHeadReanim = aApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_SQUASH);
                    aSquashHeadReanim->PlayReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
                    AttachEffect *aAttachEffect = AttachReanim(aTrackInstance->mAttachmentID, aSquashHeadReanim, 0.0f, 0.0f);
                    zombieReanim->mFrameBasePose = 0;
                    TodScaleRotateTransformMatrix((SexyMatrix3 &)aAttachEffect->mOffset, 55.0f, -15.0f, 0.2f, -0.75f, 0.75f);
                }
                zombieReanim->PlayReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, 12.0);
            }
            theNewPreviewingReanim = zombieReanim;
        } else {
            if (mSeedType >= SeedType::NUM_SEED_TYPES || mSeedType == SeedType::NUM_SEEDS_IN_CHOOSER)
                return;
            Reanimation *plantReanim = aApp->AddReanimation(0.0, theDrawHeightOffset, aRenderOrder + 2, GetPlantDefinition(mSeedType).mReanimationType);
            plantReanim->mIsAttachment = true;
            if (isImitater)
                plantReanim->mFilterEffect = GetFilterEffectTypeBySeedType(mSeedType);
            plantReanim->PlayReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 0, 12.0);

            // 为豌豆家族加入其stem动画
            if (mSeedType == SeedType::SEED_PEASHOOTER || mSeedType == SeedType::SEED_SNOWPEA || mSeedType == SeedType::SEED_REPEATER || mSeedType == SeedType::SEED_GATLINGPEA
                || mSeedType == SeedType::SEED_LEFTPEATER) {
                Reanimation *plantReanimAttachment = aApp->AddReanimation(0.0, theDrawHeightOffset, aRenderOrder + 3, GetPlantDefinition(mSeedType).mReanimationType);
                plantReanimAttachment->mLoopType = ReanimLoopType::REANIM_LOOP;
                if (isImitater)
                    plantReanimAttachment->mFilterEffect = GetFilterEffectTypeBySeedType(mSeedType);
                plantReanimAttachment->SetFramesForLayer("anim_head_idle");
                const char *trackName = "anim_stem";
                if (plantReanim->TrackExists(trackName) || (trackName = "anim_idle", plantReanim->TrackExists(trackName))) {
                    plantReanimAttachment->AttachToAnotherReanimation(plantReanim, trackName);
                }
            }
            // 为反向射手的两个头、三发射手的三个头加入动画
            if (mSeedType == SeedType::SEED_SPLITPEA) {
                Reanimation *plantReanimAttachment1 = aApp->AddReanimation(0.0, 0.0, aRenderOrder + 3, GetPlantDefinition(mSeedType).mReanimationType);
                plantReanimAttachment1->mAnimRate = plantReanim->mAnimRate;
                plantReanimAttachment1->mLoopType = ReanimLoopType::REANIM_LOOP;
                if (isImitater)
                    plantReanimAttachment1->mFilterEffect = GetFilterEffectTypeBySeedType(mSeedType);
                plantReanimAttachment1->SetFramesForLayer("anim_head_idle");
                plantReanimAttachment1->AttachToAnotherReanimation(plantReanim, "anim_idle");
                Reanimation *plantReanimAttachment2 = aApp->AddReanimation(0.0, 0.0, aRenderOrder + 3, GetPlantDefinition(mSeedType).mReanimationType);
                plantReanimAttachment2->mAnimRate = plantReanim->mAnimRate;
                plantReanimAttachment2->mLoopType = ReanimLoopType::REANIM_LOOP;
                if (isImitater)
                    plantReanimAttachment2->mFilterEffect = GetFilterEffectTypeBySeedType(mSeedType);
                plantReanimAttachment2->SetFramesForLayer("anim_splitpea_idle");
                plantReanimAttachment2->AttachToAnotherReanimation(plantReanim, "anim_idle");
            } else if (mSeedType == SeedType::SEED_THREEPEATER) {
                plantReanim->mAnimRate = RandRangeFloat(15.0, 20.0);
                Reanimation *plantReanimAttachment1 = aApp->AddReanimation(0.0, 0.0, aRenderOrder + 3, GetPlantDefinition(mSeedType).mReanimationType);
                plantReanimAttachment1->mAnimRate = plantReanim->mAnimRate;
                plantReanimAttachment1->mLoopType = ReanimLoopType::REANIM_LOOP;
                if (isImitater)
                    plantReanimAttachment1->mFilterEffect = GetFilterEffectTypeBySeedType(mSeedType);
                plantReanimAttachment1->SetFramesForLayer("anim_head_idle1");
                plantReanimAttachment1->AttachToAnotherReanimation(plantReanim, "anim_head1");
                Reanimation *plantReanimAttachment2 = aApp->AddReanimation(0.0, 0.0, aRenderOrder + 3, GetPlantDefinition(mSeedType).mReanimationType);
                plantReanimAttachment2->mAnimRate = plantReanim->mAnimRate;
                plantReanimAttachment2->mLoopType = ReanimLoopType::REANIM_LOOP;
                if (isImitater)
                    plantReanimAttachment2->mFilterEffect = GetFilterEffectTypeBySeedType(mSeedType);
                plantReanimAttachment2->SetFramesForLayer("anim_head_idle2");
                plantReanimAttachment2->AttachToAnotherReanimation(plantReanim, "anim_head2");
                Reanimation *plantReanimAttachment3 = aApp->AddReanimation(0.0, 0.0, aRenderOrder + 3, GetPlantDefinition(mSeedType).mReanimationType);
                plantReanimAttachment3->mAnimRate = plantReanim->mAnimRate;
                plantReanimAttachment3->mLoopType = ReanimLoopType::REANIM_LOOP;
                if (isImitater)
                    plantReanimAttachment3->mFilterEffect = GetFilterEffectTypeBySeedType(mSeedType);
                plantReanimAttachment3->SetFramesForLayer("anim_head_idle3");
                plantReanimAttachment3->AttachToAnotherReanimation(plantReanim, "anim_head3");
            }
            theNewPreviewingReanim = plantReanim;
        }
        mPreviewingSeedType = mSeedType;
        mPreviewReanimID4 = aApp->ReanimationGetID(theNewPreviewingReanim);
        flagUpdateChangeType = true;
    } else {
        // 如果目标预览植物类型没变化, 则为模仿者上色
        Reanimation *mPreviewReanim4 = aApp->ReanimationTryToGet(mPreviewReanimID4);
        if (mPreviewReanim4 != nullptr) {
            FilterEffect aFilterEffect = isImitater ? GetFilterEffectTypeBySeedType(mSeedType) : FilterEffect::FILTEREFFECT_NONE;
            mPreviewReanim4->mFilterEffect = aFilterEffect;
            if (mSeedType == SeedType::SEED_THREEPEATER || mSeedType == SeedType::SEED_SPLITPEA || mSeedType == SeedType::SEED_PEASHOOTER || mSeedType == SeedType::SEED_SNOWPEA
                || mSeedType == SeedType::SEED_REPEATER || mSeedType == SeedType::SEED_GATLINGPEA || mSeedType == SeedType::SEED_LEFTPEATER) {
                int mTrackCount = mPreviewReanim4->mDefinition->mTrackCount;
                for (int i = 0; i < mTrackCount; i++) {
                    ReanimatorTrackInstance *reanimatorTrackInstance = mPreviewReanim4->mTrackInstances + i;
                    unsigned short mAttachmentID = reanimatorTrackInstance->mAttachmentID;
                    if (mAttachmentID == 0)
                        continue;
                    Attachment **attachmentSystem = *(Attachment ***)(*gEffectSystem_Addr + 12);
                    Attachment *attachment = &(*attachmentSystem)[mAttachmentID];
                    int mNumEffects = attachment->mNumEffects;
                    for (int j = 0; j < mNumEffects; ++j) {
                        if (attachment->mEffectArray[j].mEffectType == EffectType::EFFECT_REANIM) {
                            Reanimation *attachReanim = aApp->ReanimationTryToGet(attachment->mEffectArray[j].mEffectID);
                            if (attachReanim != nullptr) {
                                attachReanim->mFilterEffect = aFilterEffect;
                            }
                        }
                    }
                }
            }
        }
    }

    Reanimation *mPreviewReanim4 = aApp->ReanimationTryToGet(mPreviewReanimID4);
    if (mPreviewReanim4 == nullptr)
        return;

    if (aCursorObject->mCursorType != CursorType::CURSOR_TYPE_PLANT_FROM_USABLE_COIN && mGamepadState != 7)
        return;
    if (mSelectedSeedIndex == -1)
        return;

    SeedPacket *seedPacket = &GetSeedBank()->mSeedPackets[mSelectedSeedIndex];
    if (!seedPacket->mActive) {
        flagUpdateCanPlant = false;
        flagDrawGray = true;
    }
    if (mBoard->CanPlantAt(theGridX, theGridY, mSeedType) != PlantingReason::PLANTING_OK) {
        flagUpdateCanPlant = false;
        flagDrawGray = true;
    }
    if (!mBoard->HasConveyorBeltSeedBank(mPlayerIndex2) && aCursorObject->mCursorType != CursorType::CURSOR_TYPE_PLANT_FROM_USABLE_COIN) {
        if (mIsZombie && aGameMode == GameMode::GAMEMODE_MP_VS) {
            if (!mBoard->CanTakeDeathMoney(mBoard->GetCurrentPlantCost(mSeedType, SeedType::SEED_NONE))) {
                flagUpdateCanPlant = false;
                flagDrawGray = true;
            }
        } else {
            if (!mBoard->CanTakeSunMoney(mBoard->GetCurrentPlantCost(mSeedType, SeedType::SEED_NONE), mPlayerIndex2)) {
                flagUpdateCanPlant = false;
                flagDrawGray = true;
            }
        }
    }

    Graphics newGraphics(mPreviewImage);
    newGraphics.ClearRect(0, 0, mPreviewImage->mWidth, mPreviewImage->mHeight);
    newGraphics.Translate(256, 256);
    if (flagUpdateCanPlant || flagUpdateChangeType)
        mPreviewReanim4->Update();
    if (flagDrawGray) {
        newGraphics.SetColorizeImages(true);
        (newGraphics).SetColor(gray);
    }
    mPreviewReanim4->Draw(&newGraphics);
    mPreviewReanim4->DrawRenderGroup(&newGraphics, 2);
    mPreviewReanim4->DrawRenderGroup(&newGraphics, 1);
    mPreviewReanim4->DrawRenderGroup(&newGraphics, 3);
}

void GamepadControls::DrawPreview(Sexy::Graphics *g) {
    // 修复排山倒海、砸罐子无尽、锤僵尸、种子雨不显示植物预览的问题。
    LawnApp *aApp = mGameObject.mApp;
    GameMode mGameMode = aApp->mGameMode;
    if (mGameMode == GameMode::GAMEMODE_CHALLENGE_RAINING_SEEDS) { // 为种子雨添加种植预览
        CursorObject *cursorObject = mPlayerIndex1 ? mBoard->mCursorObject2 : mBoard->mCursorObject1;
        if (cursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_USABLE_COIN) {
            mGamepadState = 7;
            old_GamepadControls_DrawPreview(this, g);
            mGamepadState = 1;
            return;
        }
    }

    if (aApp->IsWhackAZombieLevel() || aApp->IsScaryPotterLevel()) {
        if (mGamepadState == 7) {
            SeedBank *seedBank = GetSeedBank();
            SeedPacket *seedPacket = &seedBank->mSeedPackets[mSelectedSeedIndex];
            mSelectedSeedType = seedPacket->mPacketType == SeedType::SEED_IMITATER ? seedPacket->mImitaterType : seedPacket->mPacketType;
            old_GamepadControls_DrawPreview(this, g);
            return;
        }
    }
    if (mGameMode == GameMode::GAMEMODE_CHALLENGE_COLUMN) {
        int mGridX = mBoard->PixelToGridXKeepOnBoard(mCursorPositionX, mCursorPositionY);
        int mGridY = mBoard->PixelToGridYKeepOnBoard(mCursorPositionX, mCursorPositionY);
        if (mSelectedSeedType != SeedType::SEED_NONE) {
            g->SetColorizeImages(true);
            Sexy::Color theColor = {255, 255, 255, 125};
            g->SetColor(theColor);
            g->Translate(-256, -256);
            if (dynamicPreview) { // 修复动态预览时植物错位
                int thePixelY = mBoard->GridToPixelY(mGridX, mGridY);
                for (int i = 0; i != 6; ++i) {
                    if (mBoard->CanPlantAt(mGridX, i, mSelectedSeedType) == PlantingReason::PLANTING_OK) {
                        int theGridPixelY = mBoard->GridToPixelY(mGridX, i);
                        g->DrawImage(mPreviewImage, 0, theGridPixelY - thePixelY);
                    }
                }
            } else {
                for (int i = 0; i != 6; ++i) {
                    if (mBoard->CanPlantAt(mGridX, i, mSelectedSeedType) == PlantingReason::PLANTING_OK) {
                        float offset = PlantDrawHeightOffset(mBoard, 0, mSelectedSeedType, mGridX, i);
                        g->DrawImage(mPreviewImage, 0, offset + (i - mGridY) * 85);
                    }
                }
            }
            g->Translate(256, 256);
            g->SetColorizeImages(false);
        }
        return;
    }

    return old_GamepadControls_DrawPreview(this, g);
}

void ZenGardenControls::Update(float a2) {
    old_ZenGardenControls_Update(this, a2);
}
