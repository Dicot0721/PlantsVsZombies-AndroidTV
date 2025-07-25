#ifndef PVZ_ANDROID_NATIVE_NATIVE_APP_H
#define PVZ_ANDROID_NATIVE_NATIVE_APP_H

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

#endif // PVZ_ANDROID_NATIVE_NATIVE_APP_H
