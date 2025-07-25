package com.transmension.mobile;

import android.app.Activity;

/* loaded from: classes.dex */
public class InputManagerFactory {
    InputManager create(Activity activity) {
        return new AndroidInputManager((NativeActivity) activity);
    }
}