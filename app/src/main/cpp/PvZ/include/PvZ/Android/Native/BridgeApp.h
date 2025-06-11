//
// Created by 28636 on 2025/5/28.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_BRIDGEAPP_H
#define PLANTSVSZOMBIES_ANDROIDTV_BRIDGEAPP_H

#include "PvZ/Symbols.h"

#include <jni.h>

namespace Native {
class NativeApp;

class BridgeApp {
public:
    int *mVtable;
    NativeApp *mNativeApp;

    JNIEnv *getJNIEnv() { return reinterpret_cast<JNIEnv *(*)(BridgeApp *)>(Native_BridgeApp_getJNIEnvAddr)(this); }
    static BridgeApp *getSingleton() { return reinterpret_cast<BridgeApp *(*)()>(Native_BridgeApp_getSingletonAddr)(); }
};

}


#endif // PLANTSVSZOMBIES_ANDROIDTV_BRIDGEAPP_H
