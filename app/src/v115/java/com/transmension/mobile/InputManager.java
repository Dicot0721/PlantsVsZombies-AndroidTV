package com.transmension.mobile;

import android.annotation.SuppressLint;
import android.hardware.SensorEvent;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;

import java.util.ArrayList;
import java.util.List;

/* loaded from: classes.dex */
public interface InputManager {
    int FEATURE_NULL = 0;

    int[] getDeviceList();

    List<MotionRange> getDeviceMotionRanges(int i);

    String getDeviceName(int i);

    int getDeviceSources(int i);

    String getDeviceVendor(int i);

    int getFeatures();

    String getName();

    int[] getSensorList();

    float getSensorMaxRange(int i, int i2);

    String getSensorName(int i, int i2);

    float getSensorResolution(int i, int i2);

    int getSensorType(int i);

    String getSensorVendor(int i, int i2);

    boolean hasSensor(int i);

    void onDestroy();

    void onKeyEvent(KeyEvent keyEvent);

    void onMotionEvent(MotionEvent motionEvent);

    void onPause();

    void onResume();

    void onStart();

    void onStop();

    boolean startSensor(int i, int i2);

    boolean stopSensor(int i, int i2);

    /* loaded from: classes.dex */
    class MotionRange {
        public int mAxis = 0;
        public float mFlat = 0.0f;
        public float mFuzz = 0.0f;
        public float mMax = 1.0f;
        public float mMin = -1.0f;
        public float mRange = 2.0f;
        public float mResolution = 0.0f;
        public int mSource = 0;

        int getAxis() {
            return this.mAxis;
        }

        float getFlat() {
            return this.mFlat;
        }

        float getFuzz() {
            return this.mFuzz;
        }

        float getMax() {
            return this.mMax;
        }

        float getMin() {
            return this.mMin;
        }

        float getRange() {
            return this.mRange;
        }

        float getResolution() {
            return this.mResolution;
        }

        int getSource() {
            return this.mSource;
        }
    }

    /* loaded from: classes.dex */
    class InputEvent {
        public int mDeviceId = -1;
        public long mEventTime = 0;
        public long mDownTime = 0;
        public int mAction = 0;
        public int mSource = 0;
        public int mFlags = 0;

        public int getDeviceId() {
            return this.mDeviceId;
        }

        public long getEventTime() {
            return this.mEventTime;
        }

        public long getDownTime() {
            return this.mDownTime;
        }

        public int getAction() {
            return this.mAction;
        }

        public int getSource() {
            return this.mSource;
        }

        public int getFlags() {
            return this.mFlags;
        }
    }

    /* loaded from: classes.dex */
    class PointerEvent extends InputEvent {
        public int mActionIndex = 0;
        public List<Pointer> mPointers = new ArrayList<>();

        static PointerEvent translate(MotionEvent motionEvent) {
            PointerEvent event = new PointerEvent();
            event.mDeviceId = motionEvent.getDeviceId();
            event.mEventTime = motionEvent.getEventTime();
            event.mDownTime = motionEvent.getDownTime();
            event.mAction = motionEvent.getAction();
            event.mActionIndex = motionEvent.getActionIndex();
            event.mSource = motionEvent.getSource();
            event.mFlags = motionEvent.getSource();
            for (int i = 0; i < motionEvent.getPointerCount(); i++) {
                Pointer p = new Pointer();
                p.mId = motionEvent.getPointerId(i);
                p.mX = motionEvent.getX(i);
                p.mY = motionEvent.getY(i);
                p.mPressure = motionEvent.getPressure(i);
                event.mPointers.add(p);
            }
            return event;
        }

        public int getActionIndex() {
            return this.mActionIndex;
        }

        public final List<Pointer> getPointers() {
            return this.mPointers;
        }

        /* loaded from: classes.dex */
        public static class Pointer {
            public int mId;
            public float mPressure;
            public float mX;
            public float mY;
        }
    }

    @SuppressLint({"NewApi"})
            /* loaded from: classes.dex */ class JoystickEvent extends InputEvent {
        public int mActionIndex = 0;
        public List<Axis> mAxes = new ArrayList<>();

        static JoystickEvent translate(MotionEvent motionEvent) {
            JoystickEvent event = new JoystickEvent();
            event.mDeviceId = motionEvent.getDeviceId();
            event.mEventTime = motionEvent.getEventTime();
            event.mDownTime = motionEvent.getDownTime();
            event.mAction = motionEvent.getAction();
            event.mActionIndex = motionEvent.getActionIndex();
            event.mSource = motionEvent.getSource();
            event.mFlags = motionEvent.getSource();
            InputDevice device = motionEvent.getDevice();
            for (InputDevice.MotionRange range : device.getMotionRanges()) {
                Axis a = new Axis();
                a.mAxis = range.getAxis();
                a.mValue = motionEvent.getAxisValue(range.getAxis());
                event.mAxes.add(a);
            }
            return event;
        }

        public int getActionIndex() {
            return this.mActionIndex;
        }

        /* loaded from: classes.dex */
        public static class Axis {
            public int mAxis;
            public float mValue;
        }
    }

    /* loaded from: classes.dex */
    class KeyInputEvent extends InputEvent {
        public int mMetaState = 0;
        public int mKeyCode = 0;
        public int mKeyChar = 0;
        public int mRepeatCount = 0;
        public int mScanCode = 0;
        public String mCharacters = "";

        static KeyInputEvent translate(KeyEvent keyEvent) {
            KeyInputEvent event = new KeyInputEvent();
            event.mDeviceId = keyEvent.getDeviceId();
            event.mEventTime = keyEvent.getEventTime();
            event.mDownTime = keyEvent.getDownTime();
            event.mAction = keyEvent.getAction();
            event.mSource = keyEvent.getSource();
            event.mFlags = keyEvent.getFlags();
            event.mMetaState = keyEvent.getMetaState();
            event.mKeyCode = keyEvent.getKeyCode();
            event.mKeyChar = keyEvent.getUnicodeChar();
            event.mRepeatCount = keyEvent.getRepeatCount();
            event.mScanCode = keyEvent.getScanCode();
            event.mCharacters = keyEvent.getCharacters();
            return event;
        }

        public int getMetaState() {
            return this.mMetaState;
        }

        public int getKeyCode() {
            return this.mKeyCode;
        }

        public int getKeyChar() {
            return this.mKeyChar;
        }

        public int getRepeatCount() {
            return this.mRepeatCount;
        }

        public int getScanCode() {
            return this.mScanCode;
        }

        public String getCharacters() {
            return this.mCharacters;
        }
    }

    /* loaded from: classes.dex */
    class SensorInputEvent {
        public float[] mValues;
        public int mDeviceId = 0;
        public int mType = 0;
        public int mAccuracy = 0;
        public long mEventTime = 0;

        public static SensorInputEvent translate(SensorEvent sensorEvent, int deviceId) {
            SensorInputEvent event = new SensorInputEvent();
            event.mDeviceId = deviceId;
            event.mType = sensorEvent.sensor.getType();
            event.mAccuracy = sensorEvent.accuracy;
            event.mEventTime = sensorEvent.timestamp;
            event.mValues = new float[sensorEvent.values.length];
            System.arraycopy(sensorEvent.values, 0, event.mValues, 0, sensorEvent.values.length);
            return event;
        }
    }
}