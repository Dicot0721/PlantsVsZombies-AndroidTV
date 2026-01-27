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

#include "PvZ/Android/IntroVideo.h"
#include "Homura/Logger.h"
#include "PvZ/Android/Native/AudioOutput.h"
#include "PvZ/Android/Native/NativeApp.h"

int AGVideoOpen(const char *videoPath) {
    LOG_DEBUG("videoPath: {}", videoPath);
    Native::BridgeApp *bridgeApp = Native::BridgeApp::getSingleton();
    JNIEnv *env = bridgeApp->getJNIEnv();
    jobject activity = bridgeApp->mNativeApp->getActivity();
    jclass cls = env->GetObjectClass(activity);
    jmethodID methodID = env->GetMethodID(cls, "videoOpen", "(Ljava/lang/String;)Z");
    jobject videoPathUTF = env->functions->NewStringUTF(env, videoPath);
    jboolean result = env->CallBooleanMethod(activity, methodID, videoPathUTF);
    env->DeleteLocalRef(cls);
    // env->DeleteLocalRef(activity);
    env->DeleteLocalRef(videoPathUTF);
    LOG_DEBUG("activity: {}, methodID {}", (void *)activity, (void *)methodID);
    return result ? 0 : -1;
}

bool AGVideoIsPlaying() {
    // LOGD("AGVideoIsPlaying");
    Native::BridgeApp *bridgeApp = Native::BridgeApp::getSingleton();
    JNIEnv *env = bridgeApp->getJNIEnv();
    jobject activity = bridgeApp->mNativeApp->getActivity();
    jclass cls = env->GetObjectClass(activity);
    jmethodID methodID = env->GetMethodID(cls, "videoIsPlaying", "()Z");
    jboolean result = env->CallBooleanMethod(activity, methodID);
    env->DeleteLocalRef(cls);
    // env->DeleteLocalRef(activity);
    return result;
}

bool AGVideoShow(bool show) {
    LOG_DEBUG("AGVideoShow {}", show);
    Native::BridgeApp *bridgeApp = Native::BridgeApp::getSingleton();
    JNIEnv *env = bridgeApp->getJNIEnv();
    jobject activity = bridgeApp->mNativeApp->getActivity();
    jclass cls = env->GetObjectClass(activity);
    jmethodID methodID = env->GetMethodID(cls, "videoShow", "(Z)V");
    env->CallVoidMethod(activity, methodID, show);
    env->DeleteLocalRef(cls);
    // env->DeleteLocalRef(activity);
    return true;
}

bool AGVideoPlay() {
    LOG_DEBUG("AGVideoPlay");
    Native::BridgeApp *bridgeApp = Native::BridgeApp::getSingleton();
    JNIEnv *env = bridgeApp->getJNIEnv();
    jobject activity = bridgeApp->mNativeApp->getActivity();
    jclass cls = env->GetObjectClass(activity);
    jmethodID methodID = env->GetMethodID(cls, "videoPlay", "()Z");
    jboolean result = env->CallBooleanMethod(activity, methodID);
    env->DeleteLocalRef(cls);
    // env->DeleteLocalRef(activity);
    return result;
}

int AGVideoEnable(bool show) {
    LOG_DEBUG("AGVideoEnable");
    return false;
}
