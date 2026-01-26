/*
 * Copyright (C) 2023-2026  PvZ TV Touch Team
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

/* loaded from: classes.dex */
public class NativeInputManager {
    public static native void onJoystickEventNative(long j, InputManager inputManager, InputManager.JoystickEvent joystickEvent);

    public static native void onKeyInputEventNative(long j, InputManager inputManager, InputManager.KeyInputEvent keyInputEvent);

    public static native void onSensorInputEventNative(long j, InputManager inputManager, InputManager.SensorInputEvent sensorInputEvent);

    public static native void onTouchEventNative(long j, InputManager inputManager, InputManager.PointerEvent pointerEvent);
}