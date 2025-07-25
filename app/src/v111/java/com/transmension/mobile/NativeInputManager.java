package com.transmension.mobile;

public class NativeInputManager {
    public static native void onJoystickEventNative(long j, InputManager inputManager, InputManager.JoystickEvent joystickEvent);

    public static native void onKeyInputEventNative(long j, InputManager inputManager, InputManager.KeyInputEvent keyInputEvent);

    public static native void onSensorInputEventNative(long j, InputManager inputManager, InputManager.SensorInputEvent sensorInputEvent);

    public static native void onTouchEventNative(long j, InputManager inputManager, InputManager.PointerEvent pointerEvent);
}