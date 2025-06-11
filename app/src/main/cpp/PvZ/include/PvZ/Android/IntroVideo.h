#ifndef PVZ_ANDROID_INTRO_VIDEO_H
#define PVZ_ANDROID_INTRO_VIDEO_H

#include "PvZ/Android/Native/BridgeApp.h"

#include <jni.h>

int AGVideoOpen(const char *videoPath);

bool AGVideoIsPlaying();

bool AGVideoShow(bool show);

bool AGVideoPlay();

int AGVideoEnable(bool show);

#endif // PVZ_ANDROID_INTRO_VIDEO_H
