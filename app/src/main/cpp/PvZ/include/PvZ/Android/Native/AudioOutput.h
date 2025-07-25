#ifndef PVZ_ANDROID_NATIVE_AUDIO_OUTPUT_H
#define PVZ_ANDROID_NATIVE_AUDIO_OUTPUT_H

#include <jni.h>

namespace Native {
class NativeApp;

class AudioOutput {
public:
    NativeApp *mNativeApp;
    JNIEnv *mEnv;
    bool mUnkBool;
};
} // namespace Native


#endif // PVZ_ANDROID_NATIVE_AUDIO_OUTPUT_H
