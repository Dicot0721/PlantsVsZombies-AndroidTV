/*
 * Copyright (C) 2023-2025  PvZ TV Touch Team
 *
 * This file is part of PlantsVsZombies-AndroidTV.
 *
 * PlantsVsZombies-AndroidTV is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * PlantsVsZombies-AndroidTV is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * PlantsVsZombies-AndroidTV.  If not, see <https://www.gnu.org/licenses/>.
 */

package com.transmension.mobile;

import android.annotation.SuppressLint;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

@SuppressLint({"UseSparseArrays"})
/* loaded from: classes.dex */
public class AndroidInputManager implements InputManager, SensorEventListener {
    private final NativeActivity mActivity;
    protected SensorManager mSensorManager;
    protected HashMap<Integer, Sensor> mSensorMap = new HashMap<>();
    protected HashMap<Sensor, Integer> mSensorToIdMap = new HashMap<>();
    protected HashMap<Integer, Sensor> mSensorIdMap = new HashMap<>();
    protected int mSensorIdNext = 0;

    public AndroidInputManager(NativeActivity activity) {
        this.mSensorManager = null;
        this.mActivity = activity;
        this.mSensorManager = (SensorManager) activity.getSystemService(Context.SENSOR_SERVICE);
        refreshSensorIds();
    }

    public void refreshSensorIds() {
        List<Sensor> sensors;
        if (this.mSensorManager != null && (sensors = this.mSensorManager.getSensorList(-1)) != null) {
            this.mSensorIdMap.clear();
            this.mSensorToIdMap.clear();
            for (Sensor sensor : sensors) {
                this.mSensorIdMap.put(this.mSensorIdNext, sensor);
                HashMap<Sensor, Integer> hashMap = this.mSensorToIdMap;
                int i = this.mSensorIdNext;
                this.mSensorIdNext = i + 1;
                hashMap.put(sensor, i);
            }
        }
    }

    public int getSensorId(Sensor sensor) {
        if (this.mSensorToIdMap.containsKey(sensor)) {
            return this.mSensorToIdMap.get(sensor);
        }
        return -1;
    }

    public Sensor getSensorById(int type, int id) {
        if (this.mSensorManager == null) {
            return null;
        }
        if (id < 0 && type >= 0) {
            return this.mSensorManager.getDefaultSensor(type);
        }
        if (this.mSensorIdMap.containsKey(id)) {
            return this.mSensorIdMap.get(id);
        }
        return null;
    }

    @Override // com.transmension.mobile.InputManager
    public String getName() {
        return "Android";
    }

    @Override // com.transmension.mobile.InputManager
    public int getFeatures() {
        return 0;
    }

    @Override // com.transmension.mobile.InputManager
    public int[] getDeviceList() {
        return InputDevice.getDeviceIds();
    }

    @Override // com.transmension.mobile.InputManager
    public String getDeviceName(int id) {
        InputDevice device = InputDevice.getDevice(id);
        if (device == null) {
            return null;
        }
        return device.getName();
    }

    @Override // com.transmension.mobile.InputManager
    public String getDeviceVendor(int id) {
        InputDevice device = InputDevice.getDevice(id);
        if (device == null) {
            return null;
        }
        return "";
    }

    @Override // com.transmension.mobile.InputManager
    public int getDeviceSources(int id) {
        InputDevice device = InputDevice.getDevice(id);
        if (device == null) {
            return 0;
        }
        return device.getSources();
    }

    @Override // com.transmension.mobile.InputManager
    public List<MotionRange> getDeviceMotionRanges(int id) {
        InputDevice device;
        List<InputDevice.MotionRange> ranges;
        List<MotionRange> result = null;
        if ((device = InputDevice.getDevice(id)) != null && (ranges = device.getMotionRanges()) != null) {
            result = new ArrayList<>();
            for (InputDevice.MotionRange range : ranges) {
                MotionRange mr = new MotionRange();
                mr.mAxis = range.getAxis();
                mr.mFlat = range.getFlat();
                mr.mFuzz = range.getFuzz();
                mr.mMax = range.getMax();
                mr.mMin = range.getMin();
                mr.mRange = range.getRange();
                mr.mSource = range.getSource();
                result.add(mr);
            }
        }
        return result;
    }

    @Override // com.transmension.mobile.InputManager
    public int[] getSensorList() {
        List<Sensor> sensors = this.mSensorManager.getSensorList(-1);
        if (sensors == null) {
            return null;
        }
        int[] ids = new int[sensors.size()];
        int idx = 0;
        for (Sensor sensor : sensors) {
            ids[idx] = getSensorId(sensor);
            idx++;
        }
        return ids;
    }

    @Override // com.transmension.mobile.InputManager
    public boolean hasSensor(int type) {
        return this.mSensorManager != null && this.mSensorManager.getDefaultSensor(type) != null;
    }

    @Override // com.transmension.mobile.InputManager
    public boolean startSensor(int type, int id) {
        int key;
        if (this.mSensorMap.get(id) != null) {
            return true;
        }
        Sensor sensor = getSensorById(type, id);
        if (sensor != null && (key = getSensorId(sensor)) >= 0 && this.mSensorManager.registerListener(this, sensor, 1)) {
            this.mSensorMap.put(key, sensor);
            return true;
        }
        return false;
    }

    @Override // com.transmension.mobile.InputManager
    public boolean stopSensor(int type, int id) {
        int key;
        Sensor sensor = getSensorById(type, id);
        if (sensor != null && (key = getSensorId(sensor)) >= 0) {
            this.mSensorManager.unregisterListener(this, sensor);
            this.mSensorMap.remove(key);
            return true;
        }
        return false;
    }

    @Override // com.transmension.mobile.InputManager
    public int getSensorType(int id) {
        Sensor sensor = getSensorById(-1, id);
        if (sensor == null) {
            return -1;
        }
        return sensor.getType();
    }

    @Override // com.transmension.mobile.InputManager
    public String getSensorName(int type, int id) {
        Sensor sensor = getSensorById(type, id);
        return sensor == null ? "" : sensor.getName();
    }

    @Override // com.transmension.mobile.InputManager
    public String getSensorVendor(int type, int id) {
        Sensor sensor = getSensorById(type, id);
        return sensor == null ? "" : sensor.getVendor();
    }

    @Override // com.transmension.mobile.InputManager
    public float getSensorMaxRange(int type, int id) {
        Sensor sensor = getSensorById(type, id);
        if (sensor == null) {
            return 0.0f;
        }
        return sensor.getMaximumRange();
    }

    @Override // com.transmension.mobile.InputManager
    public float getSensorResolution(int type, int id) {
        Sensor sensor = getSensorById(type, id);
        if (sensor == null) {
            return 0.0f;
        }
        return sensor.getResolution();
    }

    @Override // com.transmension.mobile.InputManager
    public void onMotionEvent(MotionEvent event) {
        if (this.mActivity != null) {
            if ((event.getSource() & 16) != 0) {
                NativeInputManager.onJoystickEventNative(this.mActivity.mNativeHandle, this, JoystickEvent.translate(event));
            } else if ((event.getSource() & 4) == 0) {
                NativeInputManager.onTouchEventNative(this.mActivity.mNativeHandle, this, PointerEvent.translate(event));
            }
        }
    }

    @Override // com.transmension.mobile.InputManager
    public void onKeyEvent(KeyEvent event) {
        if (this.mActivity != null) {
            KeyInputEvent keyEvent = KeyInputEvent.translate(event);
            NativeInputManager.onKeyInputEventNative(this.mActivity.mNativeHandle, this, keyEvent);
        }
    }

    @Override // com.transmension.mobile.InputManager
    public void onStart() {
    }

    @Override // com.transmension.mobile.InputManager
    public void onPause() {
        for (Sensor sensor : this.mSensorMap.values()) {
            this.mSensorManager.unregisterListener(this, sensor);
        }
    }

    @Override // com.transmension.mobile.InputManager
    public void onResume() {
        for (Sensor sensor : this.mSensorMap.values()) {
            this.mSensorManager.registerListener(this, sensor, 1);
        }
    }

    @Override // com.transmension.mobile.InputManager
    public void onStop() {
    }

    @Override // com.transmension.mobile.InputManager
    public void onDestroy() {
    }

    @Override // android.hardware.SensorEventListener
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
    }

    @Override // android.hardware.SensorEventListener
    public void onSensorChanged(SensorEvent event) {
        if (this.mActivity != null) {
            SensorInputEvent sensorEvent = SensorInputEvent.translate(event, getSensorId(event.sensor));
            NativeInputManager.onSensorInputEventNative(this.mActivity.mNativeHandle, this, sensorEvent);
        }
    }
}