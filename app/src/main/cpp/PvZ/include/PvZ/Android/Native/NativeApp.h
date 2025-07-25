//
// Created by 28636 on 2025/6/12.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_NATIVEAPP_H
#define PLANTSVSZOMBIES_ANDROIDTV_NATIVEAPP_H

#include <jni.h>

namespace Native {

class NativeApp {
public:
    jobject getActivity() {
        return reinterpret_cast<jobject (*)(NativeApp *)>(Native_NativeApp_getActivityAddr)(this);
    }
    char *getPackageName() {
        return reinterpret_cast<char *(*)(NativeApp *)>(Native_NativeApp_getPackageNameAddr)(this);
    }
};

} // namespace Native

#endif // PLANTSVSZOMBIES_ANDROIDTV_NATIVEAPP_H
