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
import android.content.SharedPreferences;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.hardware.SensorManager;
import android.os.Build;
import android.os.Bundle;
import android.os.FileObserver;
import android.os.Handler;
import android.preference.PreferenceManager;
import android.util.DisplayMetrics;
import android.util.TypedValue;
import android.util.Xml;
import android.view.Gravity;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.OrientationEventListener;
import android.view.View;
import android.view.WindowManager;
import android.view.animation.Animation;
import android.view.animation.ScaleAnimation;
import android.widget.FrameLayout;
import android.widget.ImageView;

import com.android.support.CkHomuraMenu;
import com.trans.pvztv.R;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;
import java.util.Calendar;
import java.util.HashMap;
import java.util.Locale;

public class EnhanceActivity extends MainActivity {


    final float width = 1280, height = 720, boardMarginX = 240, boardMarginY = 60;
    //这几个按键事件是玩游戏需要的按键事件。用NativeInputManager.onKeyInputEventNative(mNativeHandle, null, keyEvent)就可以发送按键事件给游戏了。
    private final InputManager.KeyInputEvent enterEventDown = InputManager.KeyInputEvent.translate(new KeyEvent(0, 0, KeyEvent.ACTION_DOWN, KeyEvent.KEYCODE_BUTTON_A, 0, 0, 0, 0, 0, InputDevice.SOURCE_CLASS_BUTTON));
    private final InputManager.KeyInputEvent enterEventUp = InputManager.KeyInputEvent.translate(new KeyEvent(0, 0, KeyEvent.ACTION_UP, KeyEvent.KEYCODE_BUTTON_A, 0, 0, 0, 0, 0, InputDevice.SOURCE_CLASS_BUTTON));
    private final InputManager.KeyInputEvent backEventDown = InputManager.KeyInputEvent.translate(new KeyEvent(0, 0, KeyEvent.ACTION_DOWN, KeyEvent.KEYCODE_BACK, 0, 0, 0, 0, 0, InputDevice.SOURCE_CLASS_BUTTON));
    private final InputManager.KeyInputEvent backEventUp = InputManager.KeyInputEvent.translate(new KeyEvent(0, 0, KeyEvent.ACTION_UP, KeyEvent.KEYCODE_BACK, 0, 0, 0, 0, 0, InputDevice.SOURCE_CLASS_BUTTON));
    private final InputManager.KeyInputEvent shovelEventDown = InputManager.KeyInputEvent.translate(new KeyEvent(0, 0, KeyEvent.ACTION_DOWN, KeyEvent.KEYCODE_1, 0, 0, 0, 0, 0, InputDevice.SOURCE_CLASS_BUTTON));
    private final InputManager.KeyInputEvent shovelEventUp = InputManager.KeyInputEvent.translate(new KeyEvent(0, 0, KeyEvent.ACTION_UP, KeyEvent.KEYCODE_1, 0, 0, 0, 0, 0, InputDevice.SOURCE_CLASS_BUTTON));
    private final InputManager.KeyInputEvent hammerEventDown = InputManager.KeyInputEvent.translate(new KeyEvent(0, 0, KeyEvent.ACTION_DOWN, KeyEvent.KEYCODE_2, 0, 0, 0, 0, 0, InputDevice.SOURCE_CLASS_BUTTON));
    private final InputManager.KeyInputEvent hammerEventUp = InputManager.KeyInputEvent.translate(new KeyEvent(0, 0, KeyEvent.ACTION_UP, KeyEvent.KEYCODE_2, 0, 0, 0, 0, 0, InputDevice.SOURCE_CLASS_BUTTON));
    //四个方向键的按键事件（注意，游戏并不会识别方向键的抬起事件，只会识别方向键的按下事件。）
    private final InputManager.KeyInputEvent upEventDown = InputManager.KeyInputEvent.translate(new KeyEvent(0, 0, KeyEvent.ACTION_DOWN, KeyEvent.KEYCODE_DPAD_UP, 0, 0, 0, 0, 0, InputDevice.SOURCE_CLASS_BUTTON));
    private final InputManager.KeyInputEvent downEventDown = InputManager.KeyInputEvent.translate(new KeyEvent(0, 0, KeyEvent.ACTION_DOWN, KeyEvent.KEYCODE_DPAD_DOWN, 0, 0, 0, 0, 0, InputDevice.SOURCE_CLASS_BUTTON));
    private final InputManager.KeyInputEvent leftEventDown = InputManager.KeyInputEvent.translate(new KeyEvent(0, 0, KeyEvent.ACTION_DOWN, KeyEvent.KEYCODE_DPAD_LEFT, 0, 0, 0, 0, 0, InputDevice.SOURCE_CLASS_BUTTON));
    private final InputManager.KeyInputEvent rightEventDown = InputManager.KeyInputEvent.translate(new KeyEvent(0, 0, KeyEvent.ACTION_DOWN, KeyEvent.KEYCODE_DPAD_RIGHT, 0, 0, 0, 0, 0, InputDevice.SOURCE_CLASS_BUTTON));
    //LB键和RB键，用于商店翻页。
    private final InputManager.KeyInputEvent lbEventDown = InputManager.KeyInputEvent.translate(new KeyEvent(0, 0, KeyEvent.ACTION_DOWN, KeyEvent.KEYCODE_BUTTON_L1, 0, 0, 0, 0, 0, InputDevice.SOURCE_CLASS_BUTTON));
    private final InputManager.KeyInputEvent rbEventDown = InputManager.KeyInputEvent.translate(new KeyEvent(0, 0, KeyEvent.ACTION_DOWN, KeyEvent.KEYCODE_BUTTON_R1, 0, 0, 0, 0, 0, InputDevice.SOURCE_CLASS_BUTTON));
    private final Handler hammerHandler = new Handler();
    private final HashMap<Integer, Integer> keyMap = new HashMap<>();
    //这些东西都是专门用在锤子键连点上的，长按锤子键即可触发每秒20次的锤子键连点。僵尸表示：NND，开挂是吧，举报了
    private boolean isHammerButtonPressed = false, isHammerHandlerRunning = false;
    private final Runnable hammerRunnable = new Runnable() {
        @Override
        public void run() {
            if (!isHammerButtonPressed) return;
            NativeInputManager.onKeyInputEventNative(mNativeHandle, null, hammerEventDown);
            hammerHandler.postDelayed(this, 50);
        }
    };
    private boolean isTwoPlayerKeyBoardMode = false;
    //fileObserver用于观测存放文件夹变动，并在存档更新时保留一份备份
    private boolean isFileObserverLaunched = false;
    private FileObserver fileObserver;
    private OrientationEventListener mOrientationListener;
    private boolean heavyWeaponAccel = false;
    private ImageView enterButton, backButton, shovelButton, hammerButton, dpadButton, stopButton;
    private boolean isVisible;
    private boolean isAddonWindowLoaded = false;
    private ImageView visibilityWindow;
    private WindowManager mWindowManager;
    private float gameViewWidth = 0, gameViewHeight = 0;
    private float boardWidgetLeft, boardWidgetRight, boardWidgetTop, boardWidgetBottom;
    private int keyCodePause = 0, keyCodeSwitchTwoPlayerMode = 0;
    private boolean useSpecialPause = false;

    //用于fileObserver备份存档的一些函数
    public static void copyDir(File srcDir, File destDir) throws IOException {
        if (srcDir.isDirectory()) {
            if (!destDir.exists()) {
                destDir.mkdirs();
            }

            String[] children = srcDir.list();
            if (children == null) return;
            for (String child : children) {
                File srcFile = new File(srcDir, child);
                File destFile = new File(destDir, child);
                copyDir(srcFile, destFile);
            }
        } else {
            FileInputStream inputStream = new FileInputStream(srcDir);
            FileOutputStream outputStream = new FileOutputStream(destDir);

            byte[] buffer = new byte[1024];
            int bytesRead;
            while ((bytesRead = inputStream.read(buffer)) > 0) {
                outputStream.write(buffer, 0, bytesRead);
            }

            inputStream.close();
            outputStream.close();
        }
    }

    public static void deleteRecursive(File file) {
        if (file.isFile()) {
            file.delete();
        } else {
            // 如果是文件夹，递归删除
            File[] files = file.listFiles();
            if (files != null && files.length > 0) {
                for (File subFile : files) {
                    deleteRecursive(subFile);
                }
            }
            file.delete();
        }
    }

    public static native void nativeEnableManualCollect();//开启手动收集。

    public static native void nativeUseXboxMusics();

    public static native void nativeDisableShop();//关闭道具栏。

    public static native void nativeEnableNewOptionsDialog();//使用新的暂停菜单。

    public static native void nativeSetHeavyWeaponAngle(int i);//设定重型武器发射角度，i的值可以为0~180

    public static native void native1PButtonDown(int code);//1P按下按键。用于对战和结盟中操作1P种植

    public static native void native2PButtonDown(int code);//2P按下按键。用于对战和结盟中操作2P种植

    public static native void nativeSwitchTwoPlayerMode(boolean isOn);

    public static native boolean nativeIsInGame();

    public static native void nativeGaoJiPause(boolean enable);//高级暂停

    public static native boolean nativeIsGaoJiPaused();//高级暂停状态

    public static native void nativeHideCoverLayer();

    public static native void nativeShowCoolDown();

    public static native void nativeEnableNormalLevelMode();

    public static native void nativeEnableNewShovel();

    public static native void nativeEnableImitater();

    public static native void nativeDisableTrashBinZombie();

    public static native void nativeSendSecondTouch(int x, int y, int action);

    public static native void nativeShowHouse();//显示房屋。

    public static native void nativeUseNewCobCannon();//新版加农炮光标。

    public static native void nativeAutoFixPosition();

    public static native void nativeSeedBankPin();

    public static native void nativeDynamicPreview();

    public static native void nativeEnableOpenSL();

    public static native void nativeSendButtonEvent(boolean isButtonDown, int buttonCode);//发送按键

    public File getUserDataFile() {
        SharedPreferences sharedPreferences = getSharedPreferences("data", 0);
        if (sharedPreferences.getBoolean("useExternalPath", Build.VERSION.SDK_INT >= Build.VERSION_CODES.M)) {
            return getExternalFilesDir(null);
        } else {
            return getFilesDir();
        }
    }

    public void checkAndDeleteOldBackups() {
        File file = getUserDataFile();
        if (file.exists() && file.isDirectory()) {

            File[] files = file.listFiles((dir1, name) -> {
                // 判断只要是数字开头的文件夹就符合要求
                return new File(dir1, name).isDirectory() && Character.isDigit(name.charAt(0));
            });
            if (files != null && files.length > 0) {
                // 对文件夹按照修改时间进行排序
                Arrays.sort(files, (f1, f2) -> Long.compare(f2.lastModified(), f1.lastModified()));
                // 如果文件夹数量超过20个，进行删除操作
                if (files.length > 20) {
                    // 遍历排序后的文件夹列表，删除20个之后的文件夹
                    for (int i = 20; i < files.length; i++) {
                        File oldFolder = files[i];
                        // 删除文件夹
                        deleteRecursive(oldFolder);
                    }
                }
            }
        }
    }

    public void loadPreferencesFromAssetsFile(SharedPreferences preferences, SharedPreferences sharedPreferences) {
        //如果是初次启动，则载入assets文件夹中的data.xml
        if (preferences.getBoolean("firstLaunch", true)) {
            try {
                InputStream inputStream = getAssets().open("defaultSetting.xml");
                if (inputStream != null) {
                    XmlPullParser parser = Xml.newPullParser();
                    parser.setInput(inputStream, "utf-8");

                    int eventType = parser.getEventType();
                    SharedPreferences.Editor editor = sharedPreferences.edit();
                    while (eventType != XmlPullParser.END_DOCUMENT) {
                        if (eventType == XmlPullParser.START_TAG) {
                            switch (parser.getName()) {
                                case "boolean": {
                                    String name = parser.getAttributeValue(null, "name");
                                    boolean value = Boolean.parseBoolean(parser.getAttributeValue(null, "value"));
                                    editor.putBoolean(name, value);
                                    break;
                                }
                                case "int": {
                                    String name = parser.getAttributeValue(null, "name");
                                    int value = Integer.parseInt(parser.getAttributeValue(null, "value"));
                                    editor.putInt(name, value);
                                    break;
                                }
                            }
                        }
                        eventType = parser.next();
                    }
                    editor.apply();
                    inputStream.close();
                }
            } catch (IOException | XmlPullParserException e) {
                e.printStackTrace();
            }
            preferences.edit().putBoolean("firstLaunch", false).apply();
        }
    }

    public void launchFileObserver(SharedPreferences sharedPreferences) {
        //这一段用于启动fileObserver，备份玩家存档
        isFileObserverLaunched = sharedPreferences.getBoolean("autoBackUp", true);
        if (isFileObserverLaunched) {
            checkAndDeleteOldBackups();
            File userdata = new File(getUserDataFile(), "userdata");
            fileObserver = new FileObserver(userdata.getAbsolutePath(), FileObserver.CLOSE_WRITE) {
                @Override
                public void onEvent(int i, String s) {
                    try {
                        Calendar calendar = Calendar.getInstance();
                        File destDir = new File(getUserDataFile(), String.format(Locale.getDefault(), "%d月%02d日%02d:%02d_userdata备份", calendar.get(Calendar.MONTH) + 1, calendar.get(Calendar.DAY_OF_MONTH), calendar.get(Calendar.HOUR_OF_DAY), calendar.get(Calendar.MINUTE)));
                        copyDir(userdata, destDir);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            };
            fileObserver.startWatching();
        }
    }

    public void addKeyboardButtons(SharedPreferences sharedPreferences) {
        if (!sharedPreferences.getBoolean("useInGameKeyboard", true)) {
            return;
        }
        isVisible = sharedPreferences.getBoolean("isVisible", false);

        float density = getResources().getDisplayMetrics().density;

        //为游戏添加ENTER按钮
        enterButton = new ImageView(this);
        enterButton.setFocusable(false);
        enterButton.setImageDrawable(getResources().getDrawable(R.drawable.button_a));
        enterButton.setAlpha(sharedPreferences.getInt("enterTran", 90) / 100f);
        if (!sharedPreferences.getBoolean("enterKeep", false))
            enterButton.setVisibility(isVisible ? View.VISIBLE : View.GONE);
        enterButton.setOnTouchListener((view, motionEvent) -> {
            switch (motionEvent.getAction()) {
                case MotionEvent.ACTION_DOWN:
                    animateScale(view, true);
                    NativeInputManager.onKeyInputEventNative(mNativeHandle, null, enterEventDown);
                    break;
                case MotionEvent.ACTION_UP:
                    animateScale(view, false);
                    NativeInputManager.onKeyInputEventNative(mNativeHandle, null, enterEventUp);
                    break;
            }
            return true;
        });
        int enterSize = (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, sharedPreferences.getInt("enterSize", 100), getResources().getDisplayMetrics());
        FrameLayout.LayoutParams enterParams = new FrameLayout.LayoutParams(enterSize, enterSize, Gravity.CENTER);
        enterParams.leftMargin = sharedPreferences.getInt("enterX", (int) (310 * density));
        enterParams.topMargin = sharedPreferences.getInt("enterY", (int) (125 * density));
        enterButton.setLayoutParams(enterParams);


        //为游戏添加BACK按钮
        backButton = new ImageView(this);
        backButton.setFocusable(false);
        backButton.setImageDrawable(getResources().getDrawable(R.drawable.button_b));
        backButton.setAlpha(sharedPreferences.getInt("backTran", 90) / 100f);
        if (!sharedPreferences.getBoolean("backKeep", false))
            backButton.setVisibility(isVisible ? View.VISIBLE : View.GONE);
        backButton.setOnTouchListener((view, motionEvent) -> {
            switch (motionEvent.getAction()) {
                case MotionEvent.ACTION_DOWN:
                    animateScale(view, true);
                    NativeInputManager.onKeyInputEventNative(mNativeHandle, null, backEventDown);
                    break;
                case MotionEvent.ACTION_UP:
                    animateScale(view, false);
                    NativeInputManager.onKeyInputEventNative(mNativeHandle, null, backEventUp);
                    break;
            }
            return true;
        });
        int backSize = (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, sharedPreferences.getInt("backSize", 75), getResources().getDisplayMetrics());
        FrameLayout.LayoutParams backParams = new FrameLayout.LayoutParams(backSize, backSize, Gravity.CENTER);
        backParams.leftMargin = sharedPreferences.getInt("backX", (int) (180 * density));
        backParams.topMargin = sharedPreferences.getInt("backY", (int) (135 * density));
        backButton.setLayoutParams(backParams);


        //为游戏添加方向按钮
        dpadButton = new CustomView(this);
        dpadButton.setFocusable(false);
        dpadButton.setImageDrawable(getResources().getDrawable(R.drawable.dpad));
        dpadButton.setAlpha(sharedPreferences.getInt("dpadTran", 90) / 100f);
        if (!sharedPreferences.getBoolean("dpadKeep", false))
            dpadButton.setVisibility(isVisible ? View.VISIBLE : View.GONE);
        int dpadSize = (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, sharedPreferences.getInt("dpadSize", 200), getResources().getDisplayMetrics());
        FrameLayout.LayoutParams dpadParams = new FrameLayout.LayoutParams(dpadSize, dpadSize, Gravity.CENTER);
        dpadParams.leftMargin = sharedPreferences.getInt("dpadX", (int) (-250 * density));
        dpadParams.topMargin = sharedPreferences.getInt("dpadY", (int) (85 * density));
        dpadButton.setLayoutParams(dpadParams);


        //为游戏添加SHOVEL按钮
        shovelButton = new ImageView(this);
        shovelButton.setFocusable(false);
        shovelButton.setImageDrawable(getResources().getDrawable(R.drawable.button_x));
        shovelButton.setAlpha(sharedPreferences.getInt("shovelTran", 90) / 100f);
        if (!sharedPreferences.getBoolean("shovelKeep", false))
            shovelButton.setVisibility(isVisible ? View.VISIBLE : View.GONE);
        shovelButton.setOnTouchListener((view, motionEvent) -> {
            switch (motionEvent.getAction()) {
                case MotionEvent.ACTION_DOWN:
                    animateScale(view, true);
                    NativeInputManager.onKeyInputEventNative(mNativeHandle, null, shovelEventDown);
                    break;
                case MotionEvent.ACTION_UP:
                    animateScale(view, false);
                    NativeInputManager.onKeyInputEventNative(mNativeHandle, null, shovelEventUp);
                    break;
            }
            return true;
        });
        int shovelSize = (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, sharedPreferences.getInt("shovelSize", 75), getResources().getDisplayMetrics());
        FrameLayout.LayoutParams shovelParams = new FrameLayout.LayoutParams(shovelSize, shovelSize, Gravity.CENTER);
        shovelParams.leftMargin = sharedPreferences.getInt("shovelX", (int) (210 * density));
        shovelParams.topMargin = sharedPreferences.getInt("shovelY", (int) (25 * density));
        shovelButton.setLayoutParams(shovelParams);


        //为游戏添加HAMMER按钮(HAMMER长按可以触发连点，所以其点击事件和其他按钮不一样)
        hammerButton = new ImageView(this);
        hammerButton.setFocusable(false);
        hammerButton.setImageDrawable(getResources().getDrawable(R.drawable.button_y));
        hammerButton.setAlpha(sharedPreferences.getInt("hammerTran", 90) / 100f);
        if (!sharedPreferences.getBoolean("hammerKeep", false))
            hammerButton.setVisibility(isVisible ? View.VISIBLE : View.GONE);
        hammerButton.setOnTouchListener((view, motionEvent) -> {
            switch (motionEvent.getAction()) {
                case MotionEvent.ACTION_DOWN:
                    animateScale(view, true);
                    isHammerButtonPressed = true;
                    NativeInputManager.onKeyInputEventNative(mNativeHandle, null, hammerEventDown);
                    if (!isHammerHandlerRunning) {
                        isHammerHandlerRunning = true;
                        hammerHandler.postDelayed(hammerRunnable, 400);
                    }
                    break;
                case MotionEvent.ACTION_UP:
                    animateScale(view, false);
                    NativeInputManager.onKeyInputEventNative(mNativeHandle, null, hammerEventUp);
                    isHammerButtonPressed = false;
                    isHammerHandlerRunning = false;
                    hammerHandler.removeCallbacksAndMessages(null);
                    break;
            }
            return true;
        });
        int hammerSize = (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, sharedPreferences.getInt("hammerSize", 75), getResources().getDisplayMetrics());
        FrameLayout.LayoutParams hammerParams = new FrameLayout.LayoutParams(hammerSize, hammerSize, Gravity.CENTER);
        hammerParams.leftMargin = sharedPreferences.getInt("hammerX", (int) (330 * density));
        hammerParams.topMargin = sharedPreferences.getInt("hammerY", (int) (7 * density));
        hammerButton.setLayoutParams(hammerParams);

        //为游戏添加STOP按钮
        stopButton = new ImageView(this);
        stopButton.setFocusable(false);
        stopButton.setImageDrawable(getResources().getDrawable(R.drawable.button_stop));
        stopButton.setAlpha(sharedPreferences.getInt("stopTran", 90) / 100f);
        if (!sharedPreferences.getBoolean("stopKeep", false))
            stopButton.setVisibility(isVisible ? View.VISIBLE : View.GONE);
        stopButton.setOnTouchListener((view, motionEvent) -> {
            switch (motionEvent.getAction()) {
                case MotionEvent.ACTION_DOWN:
                    animateScale(view, true);


                    if (useSpecialPause) nativeGaoJiPause(!nativeIsGaoJiPaused());
                    else {
                        NativeApp.onPauseNative(mNativeHandle);
                        NativeApp.onResumeNative(mNativeHandle);
                    }

                    break;
                case MotionEvent.ACTION_UP:
                    animateScale(view, false);
                    break;
            }
            return true;
        });
        int stopSize = (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, sharedPreferences.getInt("stopSize", 75), getResources().getDisplayMetrics());
        FrameLayout.LayoutParams stopParams = new FrameLayout.LayoutParams(stopSize, stopSize, Gravity.CENTER);
        stopParams.leftMargin = sharedPreferences.getInt("stopX", (int) (220 * density));
        stopParams.topMargin = sharedPreferences.getInt("stopY", (int) (-100 * density));
        stopButton.setLayoutParams(stopParams);


        container.addView(enterButton);
        container.addView(backButton);
        container.addView(dpadButton);
        container.addView(shovelButton);
        container.addView(hammerButton);
        container.addView(stopButton);

    }

    public void loadGameSettings(SharedPreferences sharedPreferences) {

        //读取设置中的“使用XBOX版背景音乐”设置项，决定是否使用XBOX版背景音乐
        if (sharedPreferences.getBoolean("useXboxMusics", false))
            nativeUseXboxMusics();

        //读取设置中的“手动收集阳光金币”设置项，决定是否开启手动收集
        if (sharedPreferences.getBoolean("enableManualCollect", false))
            nativeEnableManualCollect();

        //读取设置中的“关闭道具栏”设置项，决定是否关闭道具栏
        if (sharedPreferences.getBoolean("disableShop", false))
            nativeDisableShop();

        //读取设置中的“使用新暂停菜单”设置项，决定是否使用新暂停菜单
        if (sharedPreferences.getBoolean("enableNewOptionsDialog", true))
            nativeEnableNewOptionsDialog();

        //读取设置中的“去除草丛和电线杆”设置项，决定是否使用新暂停菜单
        if (sharedPreferences.getBoolean("hideCoverLayer", false))
            nativeHideCoverLayer();

        //读取设置中的“显示卡片冷却进度”设置项，决定是否显示卡片冷却进度
        if (sharedPreferences.getBoolean("showCoolDown", false))
            nativeShowCoolDown();

        //读取设置中的“使用原版出怪”设置项，决定是否使用原版出怪
        if (sharedPreferences.getBoolean("normalLevel", true))
            nativeEnableNormalLevelMode();

        //读取设置中的“经典铲子”设置项，决定是否使用经典铲子
        if (sharedPreferences.getBoolean("useNewShovel", true))
            nativeEnableNewShovel();

        //读取设置中的“模仿者变灰色植物”设置项，决定是否让模仿者变灰色植物
        if (sharedPreferences.getBoolean("imitater", true))
            nativeEnableImitater();

        //读取设置中的“禁止无尽出垃圾桶”设置项，决定是否禁止无尽出垃圾桶
        if (sharedPreferences.getBoolean("disableTrashBin", false))
            nativeDisableTrashBinZombie();

        //读取设置中的“显示房子”设置项，决定是否显示房子
        if (sharedPreferences.getBoolean("showHouse", true))
            nativeShowHouse();

        //读取设置中的“原版加农炮光标”设置项，决定是否使用原版加农炮光标
        if (sharedPreferences.getBoolean("useNewCobCannon", true))
            nativeUseNewCobCannon();

        //读取设置中的“自动归位游戏光标”设置项，决定是否自动归位游戏光标
        if (sharedPreferences.getBoolean("positionAutoFix", true))
            nativeAutoFixPosition();

        if (sharedPreferences.getBoolean("seedBankPin", false))
            nativeSeedBankPin();

        if (sharedPreferences.getBoolean("dynamicPreview", true))
            nativeDynamicPreview();

        if (sharedPreferences.getBoolean("useOpenSL", true))
            nativeEnableOpenSL();

        //读取设置中的“重型武器重力感应”设置项，决定是否开启重力感应
        heavyWeaponAccel = sharedPreferences.getBoolean("heavyWeaponAccel", false);
    }

    private void addVisibilityButton(SharedPreferences sharedPreferences) {
        if (!sharedPreferences.getBoolean("useInGameKeyboard", true)) {
            return;
        }
        //显示眼睛按钮，点击眼睛可以显示\隐藏游戏键盘
        mWindowManager = getWindowManager();
        DisplayMetrics metrics = getResources().getDisplayMetrics();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1)
            mWindowManager.getDefaultDisplay().getRealMetrics(metrics);
        final int SCREEN_WIDTH = metrics.widthPixels;
        final int SCREEN_HEIGHT = metrics.heightPixels;
        float density = getResources().getDisplayMetrics().density;
        final int visibilitySize = (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, sharedPreferences.getInt("visibilitySize", 40), getResources().getDisplayMetrics());
        final int visibilityX = sharedPreferences.getInt("visibilityX", (int) (380 * density));
        final int visibilityY = sharedPreferences.getInt("visibilityY", (int) (-110 * density));
        final boolean isVisibilityLockPosition = sharedPreferences.getBoolean("isVisibilityLockPosition", false);
        WindowManager.LayoutParams visibilityParams = new WindowManager.LayoutParams(visibilitySize, visibilitySize, visibilityX, visibilityY, WindowManager.LayoutParams.TYPE_APPLICATION_PANEL, WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |
                WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN | WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL, PixelFormat.TRANSPARENT);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P)
            visibilityParams.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
        visibilityParams.gravity = Gravity.CENTER;
        visibilityParams.alpha = sharedPreferences.getInt("visibilityTran", 90) / 100f;
        visibilityWindow = new ImageView(this);
        visibilityWindow.setImageDrawable(getResources().getDrawable(isVisible ? R.drawable.button_visible : R.drawable.button_invisible));
        final View[] views = new View[]{enterButton, backButton, shovelButton, hammerButton, dpadButton, stopButton};
        final boolean[] viewsKeep = {sharedPreferences.getBoolean("enterKeep", false), sharedPreferences.getBoolean("backKeep", false), sharedPreferences.getBoolean("shovelKeep", false), sharedPreferences.getBoolean("hammerKeep", false), sharedPreferences.getBoolean("dpadKeep", false), sharedPreferences.getBoolean("stopKeep", false)};
        visibilityWindow.setOnTouchListener(new View.OnTouchListener() {
            float lastX = 0, lastY = 0;
            long downTime;
            boolean moved;

            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {

                switch (motionEvent.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        moved = false;
                        downTime = System.currentTimeMillis();
                        lastX = motionEvent.getRawX();
                        lastY = motionEvent.getRawY();
                        break;
                    case MotionEvent.ACTION_MOVE:
                        float rawX = motionEvent.getRawX();
                        float rawY = motionEvent.getRawY();
                        int dx = Math.round(rawX - lastX);
                        int dy = Math.round(rawY - lastY);
                        if (Math.abs(dx) > 5 || Math.abs(dy) > 5) moved = true;
                        if (isVisibilityLockPosition) break;
                        lastX += dx;
                        lastY += dy;
                        visibilityParams.x += dx;
                        visibilityParams.y += dy;
                        mWindowManager.updateViewLayout(view, visibilityParams);
                        break;
                    case MotionEvent.ACTION_UP:
                        if (!moved) {
                            for (int i = 0; i < views.length; i++) {
                                if (!viewsKeep[i]) {
                                    //views[i].clearAnimation();
                                    views[i].setVisibility(isVisible ? View.GONE : View.VISIBLE);
                                    //views[i].invalidate();
                                }
                            }
                            // container.invalidate();
                            visibilityWindow.setImageDrawable(getResources().getDrawable(isVisible ? R.drawable.button_invisible : R.drawable.button_visible));
                            isVisible = !isVisible;
                            sharedPreferences.edit().putBoolean("isVisible", isVisible).apply();
                        }

                        //自动贴边
                        visibilityParams.x = Math.min(Math.max(visibilityParams.x, -SCREEN_WIDTH / 2), SCREEN_WIDTH / 2);
                        visibilityParams.y = Math.min(Math.max(visibilityParams.y, -SCREEN_HEIGHT / 2), SCREEN_HEIGHT / 2);
                        mWindowManager.updateViewLayout(view, visibilityParams);

                        //存储悬浮球位置
                        sharedPreferences.edit().putInt("visibilityX", visibilityParams.x).putInt("visibilityY", visibilityParams.y).apply();
                }
                return false;
            }
        });
        mWindowManager.addView(visibilityWindow, visibilityParams);
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(this);
        SharedPreferences sharedPreferences = getSharedPreferences("data", 0);

        loadPreferencesFromAssetsFile(preferences, sharedPreferences);

        super.onCreate(savedInstanceState);

        //Hook，启动！
        System.loadLibrary("Homura");

        loadGameSettings(sharedPreferences);
        launchFileObserver(sharedPreferences);
        addKeyboardButtons(sharedPreferences);


        //设置游戏窗口的放大倍数
        mLayout.setScaleX(1f + sharedPreferences.getInt("scaleX", 0) / 100f);
        mLayout.setScaleY(1f + sharedPreferences.getInt("scaleY", 0) / 100f);

        //测量游戏边界,会在游戏窗口初次加载和每次窗口大小变动时触发测量。
        mNativeView.addOnLayoutChangeListener((view, i, i1, i2, i3, i4, i5, i6, i7) -> refreshNativeViewBorders(view));

//        container.setBackgroundDrawable(getResources().getDrawable(R.drawable.menu_icon));
        //重型武器重力感应用的东西
        mOrientationListener = new OrientationEventListener(this, SensorManager.SENSOR_DELAY_GAME) {
            @Override
            public void onOrientationChanged(int i) {
                if (i == OrientationEventListener.ORIENTATION_UNKNOWN) return;
                nativeSetHeavyWeaponAngle(i > 180 ? i - 180 : i);
            }
        };


    }

    //用于按下、抬起按钮时的缩放动画
    public void animateScale(View view, boolean isDown) {
        ScaleAnimation a2 = isDown ? new ScaleAnimation(1f, 0.65f, 1f, 0.65f, view.getWidth() / 2f, view.getHeight() / 2f) : new ScaleAnimation(0.65f, 1f, 0.65f, 1f, view.getWidth() / 2f, view.getHeight() / 2f);
        a2.setDuration(100);
        a2.setFillAfter(true);
        if (!isDown) a2.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) {
            }

            @Override
            public void onAnimationEnd(Animation animation) {
                view.clearAnimation();
            }

            @Override
            public void onAnimationRepeat(Animation animation) {

            }
        });
        view.startAnimation(a2);
    }

    public void refreshNativeViewBorders(View view) {

        gameViewWidth = view.getWidth() / width;
        gameViewHeight = view.getHeight() / height;

        boardWidgetLeft = gameViewWidth * 240;
        boardWidgetRight = gameViewWidth * 1040;
        boardWidgetTop = gameViewHeight * 60;
        boardWidgetBottom = gameViewHeight * 660;
    }

    void sendMotionEventNative(MotionEvent motionEvent) {
        if ((motionEvent.getSource() & 16) != 0)
            NativeInputManager.onJoystickEventNative(mNativeHandle, null, InputManager.JoystickEvent.translate(motionEvent));
        else if ((motionEvent.getSource() & 4) == 0)
            NativeInputManager.onTouchEventNative(mNativeHandle, null, InputManager.PointerEvent.translate(motionEvent));
    }

    void initKeyMap(SharedPreferences sharedPreferences) {
        keyMap.put(sharedPreferences.getInt("P1PAUSE", KeyEvent.KEYCODE_ESCAPE), 5);

        keyMap.put(KeyEvent.KEYCODE_ENTER, 6);
        keyMap.put(sharedPreferences.getInt("P1A", KeyEvent.KEYCODE_1), 6);
        keyMap.put(sharedPreferences.getInt("P1B", KeyEvent.KEYCODE_2), 7);
        keyMap.put(sharedPreferences.getInt("P1X", KeyEvent.KEYCODE_3), 8);
        keyMap.put(sharedPreferences.getInt("P1Y", KeyEvent.KEYCODE_4), 9);

        keyMap.put(sharedPreferences.getInt("P1L1", KeyEvent.KEYCODE_0), 10);
        keyMap.put(sharedPreferences.getInt("P1R1", KeyEvent.KEYCODE_PERIOD), 11);
        keyMap.put(sharedPreferences.getInt("P1L2", KeyEvent.KEYCODE_5), 12);
        keyMap.put(sharedPreferences.getInt("P1R2", KeyEvent.KEYCODE_6), 13);

        keyMap.put(sharedPreferences.getInt("P1TL", KeyEvent.KEYCODE_7), 14);
        keyMap.put(sharedPreferences.getInt("P1TR", KeyEvent.KEYCODE_8), 15);

        keyMap.put(sharedPreferences.getInt("P1UP", KeyEvent.KEYCODE_DPAD_UP), 16);
        keyMap.put(sharedPreferences.getInt("P1DOWN", KeyEvent.KEYCODE_DPAD_DOWN), 17);
        keyMap.put(sharedPreferences.getInt("P1LEFT", KeyEvent.KEYCODE_DPAD_LEFT), 18);
        keyMap.put(sharedPreferences.getInt("P1RIGHT", KeyEvent.KEYCODE_DPAD_RIGHT), 19);


        keyMap.put(sharedPreferences.getInt("P2A", KeyEvent.KEYCODE_J), 6 + 256);
        keyMap.put(sharedPreferences.getInt("P2B", KeyEvent.KEYCODE_K), 7 + 256);
        keyMap.put(sharedPreferences.getInt("P2X", KeyEvent.KEYCODE_L), 8 + 256);
        keyMap.put(sharedPreferences.getInt("P2Y", KeyEvent.KEYCODE_SEMICOLON), 9 + 256);

        keyMap.put(sharedPreferences.getInt("P2L1", KeyEvent.KEYCODE_Q), 10 + 256);
        keyMap.put(sharedPreferences.getInt("P2R1", KeyEvent.KEYCODE_E), 11 + 256);
        keyMap.put(sharedPreferences.getInt("P2L2", KeyEvent.KEYCODE_U), 12 + 256);
        keyMap.put(sharedPreferences.getInt("P2R2", KeyEvent.KEYCODE_I), 13 + 256);

        keyMap.put(sharedPreferences.getInt("P2TL", KeyEvent.KEYCODE_O), 14 + 256);
        keyMap.put(sharedPreferences.getInt("P2TR", KeyEvent.KEYCODE_P), 15 + 256);

        keyMap.put(sharedPreferences.getInt("P2UP", KeyEvent.KEYCODE_W), 16 + 256);
        keyMap.put(sharedPreferences.getInt("P2DOWN", KeyEvent.KEYCODE_S), 17 + 256);
        keyMap.put(sharedPreferences.getInt("P2LEFT", KeyEvent.KEYCODE_A), 18 + 256);
        keyMap.put(sharedPreferences.getInt("P2RIGHT", KeyEvent.KEYCODE_D), 19 + 256);
    }

    @SuppressLint("ClickableViewAccessibility")
    @Override
    public void onWindowFocusChanged(boolean hasFocus) {

        if (!isAddonWindowLoaded) {
            isAddonWindowLoaded = true;

            SharedPreferences sharedPreferences = getSharedPreferences("data", 0);

            //读取设置中的“开启菜单修改器”设置项，决定是否开启菜单修改器
            if (sharedPreferences.getBoolean("useMenu", true))
                try {
                    CkHomuraMenu menu = new CkHomuraMenu(this);
                    menu.SetWindowManagerActivity();
                    menu.ShowMenu();
                } catch (NoClassDefFoundError ignored) {
                }

            if (heavyWeaponAccel) mOrientationListener.enable();

            //暂停键
            keyCodePause = sharedPreferences.getInt("keyCodePause", KeyEvent.KEYCODE_T);

            // 切换键盘双人模式键
            keyCodeSwitchTwoPlayerMode = sharedPreferences.getInt("keyCodeTwoPlayer", KeyEvent.KEYCODE_M);

            initKeyMap(sharedPreferences);
            //是否使用高级暂停
            useSpecialPause = sharedPreferences.getBoolean("useSpecialPause", false);

            //触控实现的核心逻辑就在这里！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
            mNativeView.setOnTouchListener(new View.OnTouchListener() {
                private int mFirstTouchId = -1, mSecondTouchId = -1;


                @Override
                public boolean onTouch(View view, MotionEvent motionEvent) {
                    try {
                        switch (motionEvent.getActionMasked()) {
                            case MotionEvent.ACTION_DOWN:
                            case MotionEvent.ACTION_POINTER_DOWN:

                                final int pointerIndex = motionEvent.getActionIndex();
                                final int pointerId = motionEvent.getPointerId(pointerIndex);

                                if (mFirstTouchId == -1) {
                                    mFirstTouchId = pointerId;
                                    sendMotionEventNative(motionEvent);
                                } else if (mSecondTouchId == -1) {
                                    final float x = motionEvent.getX(pointerIndex);
                                    final float y = motionEvent.getY(pointerIndex);
                                    if (x > boardWidgetLeft && x < boardWidgetRight && y > boardWidgetTop && y < boardWidgetBottom) {
                                        mSecondTouchId = pointerId;
                                        nativeSendSecondTouch(Math.round((x - boardWidgetLeft) / gameViewWidth), Math.round((y - boardWidgetTop) / gameViewHeight), 0);
                                    }
                                }
                                break;
                            case MotionEvent.ACTION_MOVE:
                                if (mFirstTouchId != -1) {
                                    sendMotionEventNative(motionEvent);
                                }
                                if (mSecondTouchId != -1) {
                                    final int secondPointerIndex = motionEvent.findPointerIndex(mSecondTouchId);
                                    final float x1 = motionEvent.getX(secondPointerIndex);
                                    final float y1 = motionEvent.getY(secondPointerIndex);
                                    nativeSendSecondTouch(Math.round((x1 - boardWidgetLeft) / gameViewWidth), Math.round((y1 - boardWidgetTop) / gameViewHeight), 1);
                                }
                                break;

                            case MotionEvent.ACTION_POINTER_UP:
                            case MotionEvent.ACTION_UP:
                            case MotionEvent.ACTION_CANCEL:
                                final int pointerIndex1 = motionEvent.getActionIndex();
                                final int pointerId1 = motionEvent.getPointerId(pointerIndex1);

                                if (mFirstTouchId == pointerId1) {
                                    sendMotionEventNative(motionEvent);
                                    mFirstTouchId = -1;
                                } else if (mSecondTouchId == pointerId1) {
                                    final int secondPointerIndex = motionEvent.findPointerIndex(mSecondTouchId);
                                    final float x1 = motionEvent.getX(secondPointerIndex);
                                    final float y1 = motionEvent.getY(secondPointerIndex);
                                    nativeSendSecondTouch(Math.round((x1 - boardWidgetLeft) / gameViewWidth), Math.round((y1 - boardWidgetTop) / gameViewHeight), 2);
                                    mSecondTouchId = -1;
                                }
                                break;
                        }
                    } catch (IllegalArgumentException ignored) {
                    }

                    return true;
                }
            });
            mNativeView.requestFocus();
            addVisibilityButton(sharedPreferences);
        }
        super.onWindowFocusChanged(hasFocus);
    }

    // 玩家2的键值在上述键值的基础上添加256即可
    @Override // com.transmension.mobile.NativeActivity
    public void onNativeKeyEvent(KeyEvent event) {
        int keyCode = event.getKeyCode();

        if (event.getRepeatCount() == 0 && event.getAction() == KeyEvent.ACTION_DOWN && keyCode == keyCodeSwitchTwoPlayerMode) {
            isTwoPlayerKeyBoardMode = !isTwoPlayerKeyBoardMode;
            nativeSwitchTwoPlayerMode(isTwoPlayerKeyBoardMode);
            return;
        }

        if (isTwoPlayerKeyBoardMode && nativeIsInGame()) {

            if (keyCode == keyCodePause) {
                if (useSpecialPause) nativeGaoJiPause(!nativeIsGaoJiPaused());
                else {
                    NativeApp.onPauseNative(mNativeHandle);
                    NativeApp.onResumeNative(mNativeHandle);
                }
                return;
            }

            Integer i = keyMap.get(keyCode);
            if (i != null) {
                nativeSendButtonEvent(event.getAction() != KeyEvent.ACTION_UP, i);
            }
            return;
        }

        if (event.getRepeatCount() == 0 && event.getAction() == KeyEvent.ACTION_DOWN) {

            if (keyCode == KeyEvent.KEYCODE_BUTTON_L1) {
                native1PButtonDown(10);
            } else if (keyCode == KeyEvent.KEYCODE_BUTTON_R1) {
                native1PButtonDown(11);
            }
        }


        super.onNativeKeyEvent(event);
    }

    // 左摇杆上 0
    // 左摇杆下 1
    // 左摇杆左 2
    // 左摇杆右 3

    // 未知键 4
    // 暂停键 5

    // A 6
    // B 7
    // X 8
    // Y 9

    // L1 10
    // R1 11
    // L2 12
    // R2 13

    // TL 14
    // TR 15

    // 上 16
    // 下 17
    // 左 18
    // 右 19

    @Override
    public void onDestroy() {
        if (isFileObserverLaunched) fileObserver.stopWatching();
        if (isAddonWindowLoaded && visibilityWindow != null) mWindowManager.removeViewImmediate(visibilityWindow);
        if (heavyWeaponAccel) mOrientationListener.disable();
        super.onDestroy();
    }

    //方向键单独自定义一个类，这样方便我们自定义它
    public class CustomView extends ImageView implements View.OnTouchListener {

        private final Paint paint;      // 用于绘制的画笔
        private final long longPressInterval; //长按方向键后触发连点的时间间隔
        private final Handler dpadHandler = new Handler();
        private int cellWidth;          // 单元格宽度
        private int cellHeight;         // 单元格高度
        private int selectedRow = 1;    // 当前选中的行
        private int selectedCol = 1;    // 当前选中的列
        //这些东西用于连点功能
        private boolean rowPressed = false, colPressed = false, isHandlerRunning = false;
        private final Runnable dpadRunnable = new Runnable() {
            @Override
            public void run() {
                if (!rowPressed && !colPressed) return;
                if (rowPressed)
                    NativeInputManager.onKeyInputEventNative(mNativeHandle, null, selectedRow == 0 ? upEventDown : downEventDown);
                if (colPressed)
                    NativeInputManager.onKeyInputEventNative(mNativeHandle, null, selectedCol == 0 ? leftEventDown : rightEventDown);
                dpadHandler.postDelayed(this, longPressInterval);
            }
        };


        public CustomView(Context context) {
            super(context);
            longPressInterval = 1000 / context.getSharedPreferences("data", 0).getInt("longPress", 8);
            paint = new Paint(Paint.ANTI_ALIAS_FLAG);
            paint.setColor(0x80292929);
            setOnTouchListener(this);
        }

        @Override
        protected void onSizeChanged(int w, int h, int oldW, int oldH) {
            super.onSizeChanged(w, h, oldW, oldH);
            cellWidth = w / 3;
            cellHeight = h / 3;
        }


        @Override
        protected void onDraw(Canvas canvas) {
            super.onDraw(canvas);
            // 绘制当前按下的单元格
            if (selectedRow == 1 && selectedCol == 1) return;
            int left = selectedCol * cellWidth;
            int top = selectedRow * cellHeight;
            int right = left + cellWidth;
            int bottom = top + cellHeight;
            canvas.drawCircle((left + right) / 2f, (top + bottom) / 2f, (right - left) / 3f, paint);
        }

        @Override
        public boolean onTouch(View v, MotionEvent event) {
            int action = event.getActionMasked();
            switch (action) {
                case MotionEvent.ACTION_DOWN:
                case MotionEvent.ACTION_MOVE:
                    int row = (int) (event.getY() / cellHeight);
                    int col = (int) (event.getX() / cellWidth);
                    if (row == selectedRow && col == selectedCol) return true;
                    if (row != selectedRow) {
                        selectedRow = row;
                        if (selectedRow != 1) {
                            rowPressed = true;
                            NativeInputManager.onKeyInputEventNative(mNativeHandle, null, selectedRow == 0 ? upEventDown : downEventDown);
                        }
                    }
                    if (col != selectedCol) {
                        selectedCol = col;
                        if (selectedCol != 1) {
                            colPressed = true;
                            NativeInputManager.onKeyInputEventNative(mNativeHandle, null, selectedCol == 0 ? leftEventDown : rightEventDown);
                        }
                    }

                    if ((selectedCol != 1 || selectedRow != 1) && !isHandlerRunning) {
                        isHandlerRunning = true;
                        dpadHandler.postDelayed(dpadRunnable, 400);
                    }
                    invalidate();
                    break;
                case MotionEvent.ACTION_UP:
                case MotionEvent.ACTION_CANCEL:
                    rowPressed = false;
                    colPressed = false;
                    selectedRow = 1;
                    selectedCol = 1;
                    dpadHandler.removeCallbacksAndMessages(null);
                    isHandlerRunning = false;
                    invalidate();
                    break;
            }
            return true;
        }
    }

}
