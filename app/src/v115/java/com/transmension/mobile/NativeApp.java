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

import android.app.Activity;
import android.content.res.AssetManager;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;

import java.io.File;

/* loaded from: classes.dex */
public class NativeApp {
    private static boolean mLoaded = false;

    public static native long loadNativeApp(String str, String str2, Activity activity, View view, String str3, String str4, int i, AssetManager assetManager);

    public static native void onConfigurationChangedNative(long j);

    public static native void onHeadsetStateChanged(long j, String str, int i, int i2);

    public static native void onLowMemoryNative(long j);

    public static native void onMessageBoxButtonClickedNative(long j, int i, int i2);

    public static native void onNetworkConnectivityChanged(long j, boolean z, int i);

    public static native void onNewIntentNative(long j);

    public static native void onPauseNative(long j);

    public static native void onResumeNative(long j);

    public static native void onStartNative(long j);

    public static native void onStopNative(long j);

    public static native void processWorksNative(long j);

    public static native void unloadNativeApp(long j);

    public static boolean loadLibrary(String[] libPaths, String name) {
        return loadLibrary(libPaths, name, true);
    }

    public static boolean loadLibrary(String[] libPaths, String name, boolean verbose) {
        boolean loaded = false;
        if (libPaths != null && libPaths.length > 0) {
            String fullName = System.mapLibraryName(name);
            int length = libPaths.length;
            int i = 0;
            while (true) {
                if (i < length) {
                    String libPath = libPaths[i];
                    File fullPath = new File(libPath, fullName);
                    if (verbose) {
                        try {
                            Log.i("jni", "Loading " + fullName + " from " + libPath);
                        } catch (Throwable th) {
                            i++;
                        }
                    }
                    System.load(fullPath.getAbsolutePath());
                    loaded = true;
                    break;
                }
                break;
            }
        }
        if (loaded) {
            return true;
        }
        try {
            System.loadLibrary(name);
            loaded = true;
        } catch (Throwable th2) {
        }
        return loaded;
    }

    public static boolean load() {
        return load(null);
    }

    public static boolean load(String[] libPath) {
        return load(libPath, null);
    }

    public static boolean load(String[] libPath, String[] depends) {
        if (isLoaded()) {
            return true;
        }
        if (depends != null) {
            for (String depend : depends) {
                if (!TextUtils.isEmpty(depend)) {
                    loadLibrary(libPath, depend);
                }
            }
        }
        mLoaded = loadLibrary(libPath, "native_code");
        return mLoaded;
    }

    public static boolean isLoaded() {
        return mLoaded;
    }
}