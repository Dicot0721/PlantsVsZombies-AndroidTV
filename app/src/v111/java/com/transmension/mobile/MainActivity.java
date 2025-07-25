package com.transmension.mobile;

import android.view.WindowManager;

/* loaded from: classes.dex */
public class MainActivity extends NativeActivity {
    private static final String TAG = "MainActivity";
    private final AudioOutput mAudioOutput = new AudioOutput(this);

    @Override // com.transmension.mobile.NativeActivity, android.app.Activity
    public void onStart() {
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        super.onStart();
    }

    @Override // com.transmension.mobile.NativeActivity, android.app.Activity
    public void onStop() {
        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        super.onStop();
    }

    public AudioOutput getAudioOutput() {
        return this.mAudioOutput;
    }

}