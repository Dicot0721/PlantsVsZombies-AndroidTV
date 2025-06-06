#include "PvZ/Lawn/System/Music.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/Symbols.h"
#include "PvZ/TodLib/Common/TodCommon.h"
#include "PvZ/Lawn/System/PlayerInfo.h"
#include "PvZ/SexyAppFramework/Sound/AudiereMusicInterface.h"

#define HIWORD(a) ((a) >> 16)
#define LOWORD(a) ((a) & 0xFFFF)


void Music::StartGameMusic(bool theStart) {
    old_Music_StartGameMusic(this, theStart);
}


namespace {
bool muteMusic;
int theCounter;
} // namespace

void Music::SetupMusicFileForTune1(MusicFile theMusicFile, MusicTune theMusicTune) {
    int v7;  // r7
    int v9;  // r9
    int v10; // r8
    int v11; // r4
    int v8;
    float v13; // r3
    bool v14;  // r2
    int v15;   // r2

    if (theMusicTune == MusicTune::MUSIC_TUNE_DAY_GRASSWALK) {
        switch (theMusicFile) {
            case MusicFile::MUSIC_FILE_MAIN_MUSIC:
                v8 = -1;
                v7 = 23;
                v9 = 0;
                v10 = 29;
                break;
            case MusicFile::MUSIC_FILE_DRUMS:
                v8 = -1;
                v7 = 26;
                v9 = 24;
                v10 = 29;
                break;
            case MusicFile::MUSIC_FILE_HIHATS:
                v7 = 27;
                v8 = -1;
                v9 = 27;
                v10 = 29;
                break;
        }
    } else if (theMusicTune == MusicTune::MUSIC_TUNE_POOL_WATERYGRAVES) {
        switch (theMusicFile) {
            case MusicFile::MUSIC_FILE_MAIN_MUSIC:
                v8 = -1;
                v7 = 17;
                v10 = 29;
                break;
            case MusicFile::MUSIC_FILE_DRUMS:
                v8 = -1;
                v7 = 28;
                v9 = 18;
                v10 = 29;
                break;
            case MusicFile::MUSIC_FILE_HIHATS:
                v8 = 29;
                v7 = 24;
                v10 = 29;
                v9 = 18;
                break;
        }
    } else if (theMusicTune == MusicTune::MUSIC_TUNE_FOG_RIGORMORMIST) {
        switch (theMusicFile) {
            case MusicFile::MUSIC_FILE_MAIN_MUSIC:
                v9 = 0;
                v8 = -1;
                v7 = 15;
                v10 = 29;
                break;
            case MusicFile::MUSIC_FILE_DRUMS:
                v8 = -1;
                v7 = 22;
                v9 = 16;
                v10 = 29;
                break;
            case MusicFile::MUSIC_FILE_HIHATS:
                v7 = 23;
                v8 = -1;
                v9 = 23;
                v10 = 29;
                break;
        }
    } else if (theMusicTune == MusicTune::MUSIC_TUNE_ROOF_GRAZETHEROOF) {
        switch (theMusicFile) {
            case MusicFile::MUSIC_FILE_MAIN_MUSIC:
                v8 = -1;
                v7 = 17;
                v9 = 0;
                v10 = 29;
                break;
            case MusicFile::MUSIC_FILE_DRUMS:
                v8 = -1;
                v7 = 20;
                v9 = 18;
                v10 = 29;
                break;
            case MusicFile::MUSIC_FILE_HIHATS:
                v7 = 21;
                v8 = -1;
                v9 = 21;
                v10 = 29;
                break;
        }
    } else if ((unsigned int)(theMusicFile - 1) <= 2) {
        v7 = 29;
        v9 = 0;
        v10 = 29;
        v8 = -1;
    } else {
        v8 = -1;
        v10 = 0;
        v7 = -1;
        v9 = -1;
    }
    v11 = 0;
    do {
        v13 = 1.0;
        v14 = v11 <= v7;
        if (v11 < v9)
            v14 = 0;
        if (!v14) {
            if (v8 == v11)
                v13 = 1.0;
            else
                v13 = 0.0;
        }
        v15 = v11++;
        mMusicInterface->SetChannelVolume(theMusicFile, v15, v13);
    } while (v10 >= v11);
}

void Music::PlayFromOffset(MusicFile theMusicFile, int theOffset, double theVolume) {
    mMusicInterface->StopMusic(theMusicFile);
    SetupMusicFileForTune(theMusicFile, mCurMusicTune);
    mMusicInterface->PlayMusic(theMusicFile, theOffset, theMusicFile == MusicFile::MUSIC_FILE_CREDITS_ZOMBIES_ON_YOUR_LAWN);
    //    Sexy_AudiereMusicInterface_SetSongVolume(music->mMusicInterface, theMusicFile, theVolume);

    if (theMusicFile == MusicFile::MUSIC_FILE_MAIN_MUSIC) {
        // 加90ms静音，用于去除莫名其妙的开头鼓点声
        muteMusic = true;
        // theCounter = (Sexy_SexyAppBase_GetDialog(music->mApp,Dialogs::DIALOG_STORE) == nullptr || Sexy_SexyAppBase_GetDialog(music->mApp,Dialogs::DIALOG_ALMANAC) == nullptr) ? 10 : 18;
        theCounter = 10;
        mMusicInterface->SetVolume(0.0);
    }
}

void Music::PlayMusic(MusicTune theMusicTune, int theOffset, int theDrumsOffset) {
    if (mMusicDisabled)
        return;
    mLastMusicTune = theMusicTune;
    mCurMusicTune = theMusicTune;
    mCurMusicFileMain = MusicFile::MUSIC_FILE_NONE;
    mCurMusicFileDrums = MusicFile::MUSIC_FILE_NONE;
    mCurMusicFileHihats = MusicFile::MUSIC_FILE_NONE;
    switch (theMusicTune) {
        case MusicTune::MUSIC_TUNE_DAY_GRASSWALK:
            if (theOffset == -1) {
                theOffset = 0;
            }
            mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
            mCurMusicFileDrums = MusicFile::MUSIC_FILE_DRUMS;
            mCurMusicFileHihats = MusicFile::MUSIC_FILE_HIHATS;
            PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
            PlayFromOffset(mCurMusicFileDrums, theOffset, 0.0);
            PlayFromOffset(mCurMusicFileHihats, theOffset, 0.0);
            break;
        case MusicTune::MUSIC_TUNE_NIGHT_MOONGRAINS:
            if (theOffset == -1) {
                theDrumsOffset = 0;
                theOffset = 48;
            }
            mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
            mCurMusicFileDrums = MusicFile::MUSIC_FILE_DRUMS_NIGHTMOONGRAINS;
            PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
            PlayFromOffset(mCurMusicFileDrums, theDrumsOffset, 0.0);
            break;
        case MusicTune::MUSIC_TUNE_POOL_WATERYGRAVES:
            if (theOffset == -1) {
                theOffset = 94;
            }
            mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
            mCurMusicFileDrums = MusicFile::MUSIC_FILE_DRUMS;
            mCurMusicFileHihats = MusicFile::MUSIC_FILE_HIHATS;
            PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
            PlayFromOffset(mCurMusicFileDrums, theOffset, 0.0);
            PlayFromOffset(mCurMusicFileHihats, theOffset, 0.0);
            break;
        case MusicTune::MUSIC_TUNE_FOG_RIGORMORMIST:
            if (theOffset == -1) {
                theOffset = 125;
            }
            mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
            mCurMusicFileDrums = MusicFile::MUSIC_FILE_DRUMS;
            mCurMusicFileHihats = MusicFile::MUSIC_FILE_HIHATS;
            PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
            PlayFromOffset(mCurMusicFileDrums, theOffset, 0.0);
            PlayFromOffset(mCurMusicFileHihats, theOffset, 0.0);
            break;
        case MusicTune::MUSIC_TUNE_ROOF_GRAZETHEROOF:
            if (theOffset == -1) {
                theOffset = 184;
            }
            mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
            mCurMusicFileDrums = MusicFile::MUSIC_FILE_DRUMS;
            mCurMusicFileHihats = MusicFile::MUSIC_FILE_HIHATS;
            PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
            PlayFromOffset(mCurMusicFileDrums, theOffset, 0.0);
            PlayFromOffset(mCurMusicFileHihats, theOffset, 0.0);
            break;
        case MusicTune::MUSIC_TUNE_CHOOSE_YOUR_SEEDS:
            mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
            if (theOffset == -1) {
                theOffset = 122;
            }
            PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
            break;
        case MusicTune::MUSIC_TUNE_TITLE_CRAZY_DAVE_MAIN_THEME:
            mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
            if (theOffset == -1) {
                theOffset = 152;
            }
            PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
            break;
        case MusicTune::MUSIC_TUNE_ZEN_GARDEN:
            mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
            if (theOffset == -1) {
                theOffset = 221;
            }
            PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
            break;
        case MusicTune::MUSIC_TUNE_PUZZLE_CEREBRAWL:
            mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
            if (theOffset == -1) {
                theOffset = 177;
            }
            PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
            break;
        case MusicTune::MUSIC_TUNE_MINIGAME_LOONBOON:
            mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
            if (theOffset == -1) {
                theOffset = 166;
            }
            PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
            break;
        case MusicTune::MUSIC_TUNE_CONVEYER:
            mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
            if (theOffset == -1) {
                theOffset = 212;
            }
            PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
            break;
        case MusicTune::MUSIC_TUNE_FINAL_BOSS_BRAINIAC_MANIAC:
            mCurMusicFileMain = MusicFile::MUSIC_FILE_MAIN_MUSIC;
            if (theOffset == -1) {
                theOffset = 158;
            }
            PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
            break;
        case MusicTune::MUSIC_TUNE_CREDITS_ZOMBIES_ON_YOUR_LAWN:
            mCurMusicFileMain = MusicFile::MUSIC_FILE_CREDITS_ZOMBIES_ON_YOUR_LAWN;
            if (theOffset == -1) {
                theOffset = 0;
            }
            PlayFromOffset(mCurMusicFileMain, theOffset, 1.0);
            break;
        default:
            break;
    }
}

void Music::UpdateMusicBurst() {
    // 加90ms静音，用于去除莫名其妙的开头鼓点声
    if (muteMusic) {
        theCounter--;
        if (theCounter == 0) {
            muteMusic = false;
            PlayerInfo *playerInfo = mApp->mPlayerInfo;
            float theVolume = playerInfo == nullptr ? 1.0f : playerInfo->mMusicVolume / 0.8f;
            mMusicInterface->SetVolume(theVolume);
        }
    }
    UpdateMusicBurst2();
}

void Music::UpdateMusicBurst2() {
    int MusicOrder;                                    // ebx
    double v7;                                         // st7
    double v9;                                         // st6
    int v11;                                           // eax
    unsigned int v14;                                  // eax
    MusicTune v15;                                     // ebx
    int v16;                                           // edi
    int aQueuedDrumTrackPackedOrder_low;               // ecx
    int v18;                                           // eax
    float aPositionStart;                            // [esp+4h] [ebp-2Ch]
    float aPositionEnd;                              // [esp+8h] [ebp-28h]
    float aFadeTrackVolume;                            // [esp+1Ch] [ebp-14h]
    float aMainTrackVolume;                            // [esp+20h] [ebp-10h]
    float aDrumsJumpOrder;                             // [esp+24h] [ebp-Ch]
    unsigned int aPackedOrderMain;                     // [esp+28h] [ebp-8h]
    unsigned short v29;                                // [esp+2Ch] [ebp-4h]

    if (mApp->mBoard == nullptr || mApp->mGameMode == GameMode::GAMEMODE_INTRO) {
        return;
    }
    if (mCurMusicTune > MusicTune::MUSIC_TUNE_ROOF_GRAZETHEROOF) {
        return;
    }
    bool isNightMoonGrainsMode = mCurMusicTune == MusicTune::MUSIC_TUNE_NIGHT_MOONGRAINS;
    MusicOrder = GetMusicOrder(mCurMusicFileMain);
    v29 = MusicOrder;
    if (mBurstStateCounter > 0)
        mBurstStateCounter = mBurstStateCounter - 1;
    if (mDrumsStateCounter > 0)
        mDrumsStateCounter = mDrumsStateCounter - 1;
    v7 = 0.0;
    aMainTrackVolume = 0.0;
    aFadeTrackVolume = 0.0;
    v9 = 1.0;
    aDrumsJumpOrder = 1.0;
    switch (mMusicBurstState) {
        case MusicBurstState::MUSIC_BURST_OFF:
            if (mApp->mBoard->CountZombiesOnScreen() > 10 || mBurstOverride == 1) {
                mMusicBurstState = MusicBurstState::MUSIC_BURST_STARTING;
                mBurstStateCounter = 400;
            }
            break;
        case MusicBurstState::MUSIC_BURST_STARTING:
            if (!isNightMoonGrainsMode) {
                aMainTrackVolume = TodAnimateCurveFloat(400, 0, mBurstStateCounter, 0.0, 1.0, TodCurves::CURVE_LINEAR);
                v11 = mBurstStateCounter;
                if (v11 == 300) {
                    mMusicDrumsState = MusicDrumsState::MUSIC_DRUMS_ON_QUEUED;
                    mQueuedDrumTrackPackedOrder = MusicOrder;
                }
                if (v11 == 0) {
                    mMusicBurstState = MusicBurstState::MUSIC_BURST_ON;
                    mBurstStateCounter = 800;
                }
                break;
            }
            if (isNightMoonGrainsMode) {
                if (mMusicDrumsState != MusicDrumsState::MUSIC_DRUMS_OFF) {
                    if (mMusicDrumsState != MusicDrumsState::MUSIC_DRUMS_ON_QUEUED) {
                        aDrumsJumpOrder = TodAnimateCurveFloat(400, 0, mBurstStateCounter, 1.0, 0.0, TodCurves::CURVE_LINEAR);
                        if (!mBurstStateCounter) {
                            mMusicBurstState = MusicBurstState::MUSIC_BURST_ON;
                            mBurstStateCounter = 800;
                        }
                        break;
                    }
                    mBurstStateCounter = 400;
                } else {
                    mMusicDrumsState = MusicDrumsState::MUSIC_DRUMS_ON_QUEUED;
                    mQueuedDrumTrackPackedOrder = MusicOrder;
                    mBurstStateCounter = 400;
                }
            }
            break;
        case MusicBurstState::MUSIC_BURST_ON:
            aMainTrackVolume = 1.0;
            if (isNightMoonGrainsMode)
                aDrumsJumpOrder = 0.0;
            if (mBurstStateCounter)
                break;
            if (mApp->mBoard->CountZombiesOnScreen() < 4 && mBurstOverride == -1 || mBurstOverride == 2) {
                if (!isNightMoonGrainsMode) {
                    mMusicBurstState = MusicBurstState::MUSIC_BURST_FINISHING;
                    mBurstStateCounter = 800;
                    mMusicDrumsState = MusicDrumsState::MUSIC_DRUMS_OFF_QUEUED;
                    mQueuedDrumTrackPackedOrder = MusicOrder;
                } else if (isNightMoonGrainsMode) {
                    mMusicBurstState = MusicBurstState::MUSIC_BURST_FINISHING;
                    mBurstStateCounter = 1100;
                    mMusicDrumsState = MusicDrumsState::MUSIC_DRUMS_FADING;
                    mDrumsStateCounter = 800;
                }
            }
            break;
        case MusicBurstState::MUSIC_BURST_FINISHING:
            if (!isNightMoonGrainsMode)
                aMainTrackVolume = TodAnimateCurveFloat(800, 0, mBurstStateCounter, 1.0, 0.0, TodCurves::CURVE_LINEAR);
            else
                aDrumsJumpOrder = TodAnimateCurveFloat(400, 0, mBurstStateCounter, 0.0, 1.0, TodCurves::CURVE_LINEAR);
            if (!mBurstStateCounter && mMusicDrumsState == MusicDrumsState::MUSIC_DRUMS_OFF)
                mMusicBurstState = MusicBurstState::MUSIC_BURST_OFF;
            break;
    }
    v14 = MusicOrder;
    v15 = mCurMusicTune;
    v16 = 0;
    aQueuedDrumTrackPackedOrder_low = 0;
    v18 = HIWORD(v14);
    aPackedOrderMain = -1;
    if (!isNightMoonGrainsMode) {
        v16 = v18 / 128;
        aQueuedDrumTrackPackedOrder_low = HIWORD(mQueuedDrumTrackPackedOrder) / 128;
    } else if (isNightMoonGrainsMode) {
        v16 = v29;
        aQueuedDrumTrackPackedOrder_low = LOWORD(mQueuedDrumTrackPackedOrder);
        if (v18 > 252)
            v16 = v29 + 1;
        if (HIWORD(mQueuedDrumTrackPackedOrder) > 252u)
            ++aQueuedDrumTrackPackedOrder_low;
    }

    switch (mMusicDrumsState) {
        case MusicDrumsState::MUSIC_DRUMS_ON_QUEUED:
            if (isNightMoonGrainsMode) {
                if (v16 != aQueuedDrumTrackPackedOrder_low) {
                    aFadeTrackVolume = v9;
                    mMusicDrumsState = MusicDrumsState::MUSIC_DRUMS_ON;
                    aPackedOrderMain = (v16 % 2 != 0) + 74;
                }
            } else {
                aFadeTrackVolume = TodAnimateCurveFloat(300, 0, mBurstStateCounter, 0.0, 1.0, TodCurves::CURVE_LINEAR);
                if (mBurstStateCounter == 1) {
                    mMusicDrumsState = MusicDrumsState::MUSIC_DRUMS_ON;
                }
            }

            break;
        case MusicDrumsState::MUSIC_DRUMS_ON:
            aFadeTrackVolume = v9;
            break;
        case MusicDrumsState::MUSIC_DRUMS_OFF_QUEUED:
            aFadeTrackVolume = v9;
            if (!isNightMoonGrainsMode) {
                mMusicDrumsState = MusicDrumsState::MUSIC_DRUMS_FADING;
                mDrumsStateCounter = 50;
            }
            break;
        case MusicDrumsState::MUSIC_DRUMS_FADING:
            aPositionEnd = v7;
            aPositionStart = v9;
            aFadeTrackVolume = TodAnimateCurveFloat(isNightMoonGrainsMode ? 800 : 50, 0, mDrumsStateCounter, aPositionStart, aPositionEnd, TodCurves::CURVE_LINEAR);
            if (mDrumsStateCounter == 0)
                mMusicDrumsState = MusicDrumsState::MUSIC_DRUMS_OFF;
            break;
    }
    if (!isNightMoonGrainsMode) {
        mMusicInterface->SetSongVolume(MusicFile::MUSIC_FILE_HIHATS, aMainTrackVolume * mPauseVolume);
        mMusicInterface->SetSongVolume(MusicFile::MUSIC_FILE_DRUMS, aFadeTrackVolume * mPauseVolume);
    } else if (isNightMoonGrainsMode) {
        mMusicInterface->SetSongVolume(MusicFile::MUSIC_FILE_MAIN_MUSIC, aDrumsJumpOrder * mPauseVolume);
        mMusicInterface->SetSongVolume(MusicFile::MUSIC_FILE_DRUMS_NIGHTMOONGRAINS, aFadeTrackVolume * mPauseVolume);
        if (aPackedOrderMain != -1) {
            mMusicInterface->PlayMusic(mCurMusicFileDrums, aPackedOrderMain, 0);
        }
    }
}

void Music::StartBurst() {
    if (mMusicBurstState == MusicBurstState::MUSIC_BURST_OFF) {
        mMusicBurstState = MusicBurstState::MUSIC_BURST_STARTING;
        mBurstStateCounter = 400;
    }
}

void Music::MusicUpdate() {
    if (mFadeOutCounter <= 0) {
        if (mNormalVolume != mPauseVolume) {
            mNormalVolume = mPauseVolume;
            mMusicInterface->SetSongVolume(mCurMusicFileMain, mPauseVolume);
        }
    } else {
        mFadeOutCounter--;
        if (mFadeOutCounter > 0) {
            float theVolume = TodAnimateCurveFloat(mFadeOutDuration, 0, mFadeOutCounter, 1.0, 0.0, TodCurves::CURVE_LINEAR);
            mMusicInterface->SetSongVolume(mCurMusicFileMain, theVolume * mPauseVolume);
        } else {
            StopAllMusic();
        }
    }
    UpdateMusicBurst();
    Board *board = mApp->mBoard;
    if ((board == nullptr || !board->mPaused) && mCurMusicFileMain != MusicFile::MUSIC_FILE_NONE) {
        MusicResync();
    }
}

void Music::ResyncChannel(MusicFile theFile1, MusicFile theFile2) {}

void Music2::Create() {
    // 选择使用哪一版本的音乐。xbox版是xm格式，有鼓点；TV版则是ogg格式，无鼓点。
    if (useXboxMusic) {
        Music::Create();
        return;
    }

    old_Music2_Music2(this);
}

void Music::MusicResync() {
    if (mCurMusicFileMain != MusicFile::MUSIC_FILE_NONE) {
        if (mCurMusicFileDrums != MusicFile::MUSIC_FILE_NONE)
            ResyncChannel(mCurMusicFileMain, mCurMusicFileDrums);
        if (mCurMusicFileHihats != MusicFile::MUSIC_FILE_NONE)
            ResyncChannel(mCurMusicFileMain, mCurMusicFileHihats);
    }
}

void Music2::StopAllMusic() {
    old_Music2_StopAllMusic(this);
}

void Music2::StartGameMusic(bool theStart) {
    old_Music2_StartGameMusic(this, theStart);
}

void Music2::GameMusicPause(bool thePause) {
    old_Music2_GameMusicPause(this, thePause);
}

void Music2::FadeOut(int theFadeOutDuration) {
    old_Music2_FadeOut(this, theFadeOutDuration);
}
