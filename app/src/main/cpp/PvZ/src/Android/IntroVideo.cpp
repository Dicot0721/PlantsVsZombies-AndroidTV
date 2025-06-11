#include "PvZ/Android/IntroVideo.h"
#include "Homura/Logger.h"
#include "PvZ/Android/OpenSL.h"
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
    //    env->DeleteLocalRef(activity);
    env->DeleteLocalRef(videoPathUTF);
    LOG_DEBUG("activity: {}, methodID {}", (void *)activity, (void *)methodID);
    return result ? 0 : -1;
}

bool AGVideoIsPlaying() {
    //    LOGD("AGVideoIsPlaying");
    Native::BridgeApp *bridgeApp = Native::BridgeApp::getSingleton();
    JNIEnv *env = bridgeApp->getJNIEnv();
    jobject activity = bridgeApp->mNativeApp->getActivity();
    jclass cls = env->GetObjectClass(activity);
    jmethodID methodID = env->GetMethodID(cls, "videoIsPlaying", "()Z");
    jboolean result = env->CallBooleanMethod(activity, methodID);
    env->DeleteLocalRef(cls);
    //    env->DeleteLocalRef(activity);
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
