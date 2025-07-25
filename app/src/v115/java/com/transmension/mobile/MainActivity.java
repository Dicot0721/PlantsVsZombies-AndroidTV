package com.transmension.mobile;

import android.os.Build;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.WindowManager;

public class MainActivity extends NativeActivity {
    private static final String TAG = "MainActivity";
    private final AudioOutput mAudioOutput = new AudioOutput(this);
    private InputManager mInputManager;
    private String mInputManagerFactoryName = InputManagerFactory.class.getName();

    public void hideSystemNagvigationBar() {
        if (Build.VERSION.SDK_INT >= 14) {
            getWindow().getDecorView().setSystemUiVisibility(2);
        }
    }


    @Override // com.transmension.mobile.NativeActivity, android.app.Activity
    public void onDestroy() {
        if (this.mInputManager != null) {
            this.mInputManager.onDestroy();
        }
        super.onDestroy();
    }

    @Override // com.transmension.mobile.NativeActivity, android.app.Activity
    public void onStart() {
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
//        NotificationManager manager = getAppNotificationManager();
//        if (manager != null) {
//            manager.onStart();
//        }
//        if (this.mSocialManager != null) {
//            this.mSocialManager.onStart();
//        }
//        this.mExtensionManager.onStart();
//        if (this.mInputManager != null) {
//            this.mInputManager.onStart();
//        }
//        if (this.mGameCenter != null) {
//            this.mGameCenter.onStart();
//        }
//        if (this.mShareManager != null) {
//            this.mShareManager.onStart();
//        }
//        if (this.mNotificationManager != null && this.mNotificationManager != manager) {
//            this.mNotificationManager.onStart();
//        }
//        if (this.mAudioRecorder != null) {
//            this.mAudioRecorder.onStart();
//        }
//        if (this.mUpdateManager != null) {
//            this.mUpdateManager.onStart();
//        }
        super.onStart();
    }

    @Override // com.transmension.mobile.NativeActivity, android.app.Activity
    public void onStop() {
        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
//        NotificationManager manager = getAppNotificationManager();
//        if (manager != null) {
//            manager.onStop();
//        }
//        if (this.mSocialManager != null) {
//            this.mSocialManager.onStop();
//        }
//        this.mExtensionManager.onStop();
//        if (this.mInputManager != null) {
//            this.mInputManager.onStop();
//        }
//        if (this.mGameCenter != null) {
//            this.mGameCenter.onStop();
//        }
//        if (this.mShareManager != null) {
//            this.mShareManager.onStop();
//        }
//        if (this.mNotificationManager != null && this.mNotificationManager != manager) {
//            this.mNotificationManager.onStop();
//        }
//        if (this.mAudioRecorder != null) {
//            this.mAudioRecorder.onStop();
//        }
//        if (this.mUpdateManager != null) {
//            this.mUpdateManager.onStop();
//        }
        super.onStop();
    }

    public AudioOutput getAudioOutput() {
        return this.mAudioOutput;
    }

    public String getInputManagerFactoryName() {
        return this.mInputManagerFactoryName;
    }

    public void setInputManagerFactoryName(String name) {
        this.mInputManagerFactoryName = name;
    }

    public InputManager createInputManager() {
        if (this.mInputManager != null) {
            return this.mInputManager;
        }
        try {
            Class<?> clazz = Class.forName(this.mInputManagerFactoryName);
            try {
                InputManagerFactory factory = (InputManagerFactory) clazz.newInstance();
                this.mInputManager = factory.create(this);
                Log.i(TAG, "InputManager: " + this.mInputManager.getName());
                onInputManagerCreated();
                return this.mInputManager;
            } catch (IllegalAccessException e) {
                e.printStackTrace();
                return null;
            } catch (InstantiationException e2) {
                e2.printStackTrace();
                return null;
            }
        } catch (ClassNotFoundException e3) {
            e3.printStackTrace();
            return null;
        }
    }

    public boolean hasInputManager() {
        return this.mInputManager != null;
    }

    public InputManager getInputManager() {
        return this.mInputManager;
    }

    public void onInputManagerCreated() {
    }


    @Override // com.transmension.mobile.NativeActivity
    public void onNativeKeyEvent(KeyEvent event) {
        if (this.mInputManager == null) {
            createInputManager();
        }
        if (this.mInputManager == null) {
            super.onNativeKeyEvent(event);
        } else {
            this.mInputManager.onKeyEvent(event);
        }
    }

    @Override // com.transmension.mobile.NativeActivity
    public void onNativeMotionEvent(MotionEvent event) {
        if (this.mInputManager == null) {
            createInputManager();
        }
        if (this.mInputManager == null) {
            super.onNativeMotionEvent(event);
        } else {
            this.mInputManager.onMotionEvent(event);
        }
    }

}