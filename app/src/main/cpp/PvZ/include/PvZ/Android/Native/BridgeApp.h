#ifndef PVZ_ANDROID_NATIVE_BRIDGE_APP_H
#define PVZ_ANDROID_NATIVE_BRIDGE_APP_H

#include "PvZ/Symbols.h"

#include <jni.h>

namespace Native {
class NativeApp;

class BridgeApp {
public:
    int *mVtable;
    NativeApp *mNativeApp;

    JNIEnv *getJNIEnv() {
        return reinterpret_cast<JNIEnv *(*)(BridgeApp *)>(Native_BridgeApp_getJNIEnvAddr)(this);
    }
    static BridgeApp *getSingleton() {
        return reinterpret_cast<BridgeApp *(*)()>(Native_BridgeApp_getSingletonAddr)();
    }
};

} // namespace Native


#endif // PVZ_ANDROID_NATIVE_BRIDGE_APP_H
