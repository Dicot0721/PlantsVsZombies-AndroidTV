package com.transmension.mobile;

import android.app.Activity;
import android.content.res.AssetManager;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import java.io.File;
import java.util.HashSet;
import java.util.Set;

/* loaded from: classes.dex */
public class NativeApp {
    private static boolean mLoaded = false;
    private static final Set<String> mLoadedLibraries = new HashSet<>();

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

    public static native void onWebBrowserClosed(long j);

    public static native void processWorksNative(long j);

    public static native void unloadNativeApp(long j);

    public static boolean loadLibrary(String[] libPaths, String name) {
        return loadLibrary(libPaths, name, true);
    }

    public static boolean loadLibrary(String[] libPaths, String name, boolean verbose) {
        boolean loaded = false;
        if (libPaths != null && libPaths.length > 0) {
            String fullName = System.mapLibraryName(name);
            for (String libPath : libPaths) {
                File fullPath = new File(libPath, fullName);
                if (verbose) {
                    try {
                        Log.i("jni", "Loading " + fullName + " from " + libPath);
                    } catch (Throwable ignored) {
                    }
                }
                String absPath = fullPath.getAbsolutePath();
                if (!mLoadedLibraries.contains(absPath)) {
                    System.load(absPath);
                    mLoadedLibraries.add(absPath);
                }
                loaded = true;
                break;
            }
        }
        if (loaded) {
            return true;
        }
        try {
            if (!mLoadedLibraries.contains(name)) {
                System.load(name);
                mLoadedLibraries.add(name);
            }
            loaded = true;
        } catch (Throwable ignored) {
        }
        return loaded;
    }

    public static boolean load() {
        return load(null);
    }

    public static boolean load(String[] libPath) {
        return load("native_code", libPath, null);
    }

    public static boolean load(String mainLib, String[] libPath) {
        return load(mainLib, libPath, null);
    }

    public static boolean load(String mainLib, String[] libPath, String[] depends) {
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
        mLoaded = loadLibrary(libPath, mainLib);
        return mLoaded;
    }

    public static boolean isLoaded() {
        return mLoaded;
    }
}