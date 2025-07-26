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

package com.android.support;


import android.app.Activity;
import android.app.AlertDialog;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.ColorStateList;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.graphics.PorterDuff;
import android.graphics.Typeface;
import android.net.Uri;
import android.os.Build;
import android.text.Html;
import android.text.InputFilter;
import android.text.InputType;
import android.text.TextUtils;
import android.text.method.DigitsKeyListener;
import android.util.DisplayMetrics;
import android.util.Log;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;
import android.webkit.WebView;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.RelativeLayout;
import android.widget.ScrollView;
import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import com.trans.pvztv.R;

import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

public class CkHomuraMenu {
    //********** Here you can easly change the menu appearance **********//

    //region Variable
    public static final String TAG = "Mod_Menu"; //Tag for logcat

    int TEXT_COLOR = Color.parseColor("#82CAFD");
    int TEXT_COLOR_2 = Color.parseColor("#FFFFFF");
    int BTN_COLOR = Color.parseColor("#1C262D");
    int MENU_BG_COLOR = Color.parseColor("#EE1C2A35"); //#AARRGGBB
    int MENU_FEATURE_BG_COLOR = Color.parseColor("#DD141C22"); //#AARRGGBB
    int MENU_WIDTH = 200;
    int POS_X = 150;
    int POS_Y = 150;

    // float MENU_CORNER = 4f;
    int ICON_SIZE = 45;
    int ToggleON = Color.CYAN;
    int ToggleOFF = Color.RED;
    int BtnON = Color.parseColor("#00ffff");
    int BtnOFF = Color.parseColor("#7f0000");
    int CategoryBG = Color.parseColor("#2F3D4C");
    int SeekBarColor = Color.parseColor("#80CBC4");
    int SeekBarProgressColor = Color.parseColor("#80CBC4");
    int CheckBoxColor = Color.parseColor("#80CBC4");
    int RadioColor = Color.parseColor("#FFFFFF");
    String NumberTxtColor = "#41c300";
    //********************************************************************//

    RelativeLayout mCollapsed, mRootContainer;
    LinearLayout mExpanded, mods, mSettings, mCollapse;

    WindowManager mWindowManager;
    WindowManager.LayoutParams vmParams;
    ImageView startimage;
    FrameLayout rootFrame;
    ScrollView scrollView;
    boolean settingsOpen;
    Context getContext;
    private int SCREEN_WIDTH, SCREEN_HEIGHT;

    public CkHomuraMenu(Context context) {

        getContext = context;
        Preferences.context = context;
        rootFrame = new FrameLayout(context);
        rootFrame.setOnTouchListener(onTouchListener());
        mRootContainer = new RelativeLayout(context);
        mCollapsed = new RelativeLayout(context);
        mCollapsed.setVisibility(View.VISIBLE);

        //********** The box of the mod menu **********
        mExpanded = new LinearLayout(context);
        mExpanded.setVisibility(View.GONE);
        mExpanded.setBackgroundColor(MENU_BG_COLOR);
        mExpanded.setOrientation(LinearLayout.VERTICAL);
        // mExpanded.setPadding(1, 1, 1, 1); //So borders would be visible
        mExpanded.setLayoutParams(new LinearLayout.LayoutParams(dp(MENU_WIDTH), LinearLayout.LayoutParams.WRAP_CONTENT));
//        GradientDrawable gdMenuBody = new GradientDrawable();
//        gdMenuBody.setCornerRadius(MENU_CORNER); //Set corner
//        gdMenuBody.setColor(MENU_BG_COLOR); //Set background color
//        gdMenuBody.setStroke(1, Color.parseColor("#32cb00")); //Set border
//        mExpanded.setBackground(gdMenuBody); //Apply GradientDrawable to it

        //********** The icon to open mod menu **********
        startimage = new ImageView(context);
        startimage.setLayoutParams(new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT));
        int applyDimension = (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, ICON_SIZE, context.getResources().getDisplayMetrics()); //Icon size
        startimage.getLayoutParams().height = applyDimension;
        startimage.getLayoutParams().width = applyDimension;
        startimage.setScaleType(ImageView.ScaleType.FIT_XY);
        startimage.setImageDrawable(getContext.getResources().getDrawable(R.drawable.menu_icon));
        ((ViewGroup.MarginLayoutParams) startimage.getLayoutParams()).topMargin = (int) ((((float) 10) * getContext.getResources().getDisplayMetrics().density) + 0.5f);
        //Initialize event handlers for buttons, etc.
        startimage.setOnTouchListener(onTouchListener());
        startimage.setOnClickListener(view -> {
            mCollapsed.setVisibility(View.GONE);
            mExpanded.setVisibility(View.VISIBLE);
        });

        //********** Settings icon **********
        TextView settings = new TextView(context); //Android 5 can't show ⚙, instead show other icon instead
        settings.setText(Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.M ? "\uD83C\uDF41" : "\uD83D\uDD27");
        settings.setTextColor(TEXT_COLOR);
        settings.setTypeface(Typeface.DEFAULT_BOLD);
        settings.setTextSize(20.0f);
        RelativeLayout.LayoutParams rlsettings = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
        rlsettings.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
        settings.setLayoutParams(rlsettings);
        settings.setOnClickListener(v -> {
            try {
                settingsOpen = !settingsOpen;
                if (settingsOpen) {
                    scrollView.removeView(mods);
                    scrollView.addView(mSettings);
                    scrollView.scrollTo(0, 0);
                } else {
                    scrollView.removeView(mSettings);
                    scrollView.addView(mods);
                }
            } catch (IllegalStateException ignored) {
            }
        });

        //********** Settings **********
        mSettings = new LinearLayout(context);
        mSettings.setOrientation(LinearLayout.VERTICAL);
        featureList(SettingsList(), mSettings);

        //********** Title **********
        RelativeLayout titleText = new RelativeLayout(context);
        titleText.setPadding(10, 5, 10, 5);
        titleText.setVerticalGravity(16);

        TextView title = new TextView(context);
        title.setTextColor(TEXT_COLOR);
        title.setTextSize(18.0f);
        title.setText(Html.fromHtml("<b>PvZ-TV</b>"));
        title.setGravity(Gravity.CENTER);
        RelativeLayout.LayoutParams rl = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
        rl.addRule(RelativeLayout.CENTER_HORIZONTAL);
        title.setLayoutParams(rl);

        //********** Sub title **********
        TextView subTitle = new TextView(context);
        subTitle.setEllipsize(TextUtils.TruncateAt.MARQUEE);
        subTitle.setMarqueeRepeatLimit(-1);
        subTitle.setSingleLine(true);
        subTitle.setSelected(true);
        subTitle.setText(Html.fromHtml("<b><marquee><p style=\"font-size:30\"><p style=\"color:green;\">PvZ</p> | PvZ-Mod</p> </marquee></b>"));
        subTitle.setTextColor(TEXT_COLOR);
        subTitle.setTextSize(10.0f);
        subTitle.setGravity(Gravity.CENTER);
        subTitle.setPadding(0, 0, 0, 5);

        //********** Mod menu feature list **********
        scrollView = new ScrollView(context);
        LinearLayout.LayoutParams scrlLLExpanded = new LinearLayout.LayoutParams(mExpanded.getLayoutParams());
        scrlLLExpanded.weight = 1.0f;
        scrollView.setLayoutParams(scrlLLExpanded);
        scrollView.setBackgroundColor(MENU_FEATURE_BG_COLOR);
        mods = new LinearLayout(context);
        mods.setOrientation(LinearLayout.VERTICAL);

        //********** RelativeLayout for buttons **********
        RelativeLayout relativeLayout = new RelativeLayout(context);
        relativeLayout.setPadding(10, 3, 10, 3);
        relativeLayout.setVerticalGravity(Gravity.CENTER);

        //**********  Hide/Kill button **********
        RelativeLayout.LayoutParams lParamsHideBtn = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
        lParamsHideBtn.addRule(RelativeLayout.ALIGN_PARENT_LEFT);

        Button hideBtn = new Button(context);
        hideBtn.setLayoutParams(lParamsHideBtn);
        hideBtn.setBackgroundColor(Color.TRANSPARENT);
        hideBtn.setText("结束");
        hideBtn.setTextColor(TEXT_COLOR);
        hideBtn.setOnClickListener(view -> Toast.makeText(getContext, "长按结束菜单服务", Toast.LENGTH_LONG).show());
        hideBtn.setOnLongClickListener(view -> {
            Toast.makeText(getContext, "菜单服务结束", Toast.LENGTH_LONG).show();
            rootFrame.removeView(mRootContainer);
            mWindowManager.removeViewImmediate(rootFrame);
            return false;
        });

        //********** Close button **********
        RelativeLayout.LayoutParams lParamsCloseBtn = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
        lParamsCloseBtn.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);

        Button closeBtn = new Button(context);
        closeBtn.setLayoutParams(lParamsCloseBtn);
        closeBtn.setBackgroundColor(Color.TRANSPARENT);
        closeBtn.setText("最小化");
        closeBtn.setTextColor(TEXT_COLOR);
        closeBtn.setOnClickListener(view -> {

            mCollapsed.setVisibility(View.VISIBLE);
            mExpanded.setVisibility(View.GONE);
        });

        //********** Adding view components **********
        mRootContainer.addView(mCollapsed);
        mRootContainer.addView(mExpanded);
        mCollapsed.addView(startimage);
        titleText.addView(title);
        titleText.addView(settings);
        mExpanded.addView(titleText);
        mExpanded.addView(subTitle);
        scrollView.addView(mods);
        mExpanded.addView(scrollView);
        relativeLayout.addView(hideBtn);
        relativeLayout.addView(closeBtn);
        mExpanded.addView(relativeLayout);

    }

    native String[] GetFeatureList();

    native String[] SettingsList();

    native String GetCurrentFormation();

    public void ShowMenu() {
        rootFrame.addView(mRootContainer);
        mods.removeAllViews();
        featureList(GetFeatureList(), mods);
    }

    public void SetWindowManagerActivity() {
        SharedPreferences sharedPreferences = getContext.getSharedPreferences("data", 0);
        vmParams = new WindowManager.LayoutParams(
                WindowManager.LayoutParams.WRAP_CONTENT,
                WindowManager.LayoutParams.WRAP_CONTENT,
                sharedPreferences.getInt("menuX", POS_X),
                sharedPreferences.getInt("menuY", POS_Y),
                WindowManager.LayoutParams.TYPE_APPLICATION_PANEL,
                WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |
                        WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN |
                        WindowManager.LayoutParams.FLAG_SPLIT_TOUCH,
                PixelFormat.TRANSPARENT
        );
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P)
            vmParams.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
        vmParams.gravity = Gravity.START | Gravity.TOP;
        mWindowManager = ((Activity) getContext).getWindowManager();
        GetWidthHeight();
        mWindowManager.addView(rootFrame, vmParams);
    }

    void GetWidthHeight() {
        DisplayMetrics metrics = new DisplayMetrics();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {
            mWindowManager.getDefaultDisplay().getRealMetrics(metrics);
        } else {
            mWindowManager.getDefaultDisplay().getMetrics(metrics);
        }
        SCREEN_WIDTH = metrics.widthPixels;
        SCREEN_HEIGHT = metrics.heightPixels;
    }

    private View.OnTouchListener onTouchListener() {
        SharedPreferences sharedPreferences = getContext.getSharedPreferences("data", 0);
        return new View.OnTouchListener() {
            final View collapsedView = mCollapsed;
            final View expandedView = mExpanded;
            private float initialTouchX, initialTouchY;
            private int initialX, initialY;
            private long initialTime;

            public boolean onTouch(View view, MotionEvent motionEvent) {
                switch (motionEvent.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        initialX = vmParams.x;
                        initialY = vmParams.y;
                        initialTouchX = motionEvent.getRawX();
                        initialTouchY = motionEvent.getRawY();
                        initialTime = System.nanoTime();
                        return true;
                    case MotionEvent.ACTION_MOVE:
                        mExpanded.setAlpha(0.5f);
                        mCollapsed.setAlpha(0.5f);
                        vmParams.x = initialX + ((int) (motionEvent.getRawX() - initialTouchX));
                        vmParams.y = initialY + ((int) (motionEvent.getRawY() - initialTouchY));
                        mWindowManager.updateViewLayout(rootFrame, vmParams);
                        return true;
                    case MotionEvent.ACTION_UP:
                        int rawX = (int) Math.abs(motionEvent.getRawX() - initialTouchX);
                        int rawY = (int) Math.abs(motionEvent.getRawY() - initialTouchY);
                        vmParams.x = Math.min(Math.max(vmParams.x, 0), SCREEN_WIDTH);
                        vmParams.y = Math.min(Math.max(vmParams.y, 0), SCREEN_HEIGHT);
                        sharedPreferences.edit().putInt("menuX", vmParams.x).putInt("menuY", vmParams.y).apply();
                        mWindowManager.updateViewLayout(rootFrame, vmParams);
                        mExpanded.setAlpha(1f);
                        mCollapsed.setAlpha(1f);
                        if (rawX < 10 && rawY < 10 && isViewCollapsed() && System.nanoTime() - initialTime < 500000000) {
                            try {
                                collapsedView.setVisibility(View.GONE);
                                expandedView.setVisibility(View.VISIBLE);
                            } catch (NullPointerException ignored) {

                            }
                        }
                        return true;

                    default:
                        return false;
                }
            }
        };
    }

    private void featureList(String[] listFT, LinearLayout linearLayout) {
        //Currently looks messy right now. Let me know if you have improvements
        int featNum, subFeat = 0;
        LinearLayout llBak = linearLayout;

        for (int i = 0; i < listFT.length; i++) {
            boolean switchedOn = false;
            //Log.i("featureList", listFT[i]);
            String feature = listFT[i];
            if (feature.contains("_True")) {
                switchedOn = true;
                feature = feature.replaceFirst("_True", "");
            }

            linearLayout = llBak;
            if (feature.contains("CollapseAdd_")) {
                //if (collapse != null)
                linearLayout = mCollapse;
                feature = feature.replaceFirst("CollapseAdd_", "");
            }
            String[] str = feature.split("_");

            //Assign feature number
            if (TextUtils.isDigitsOnly(str[0]) || str[0].matches("-\\d*")) {
                featNum = Integer.parseInt(str[0]);
                feature = feature.replaceFirst(str[0] + "_", "");
                subFeat++;
            } else {
                //Subtract feature number. We don't want to count ButtonLink, Category, RichTextView and RichWebView
                featNum = i - subFeat;
            }
            String[] strSplit = feature.split("_");
            switch (strSplit[0]) {
                case "Toggle":
                    Switch(linearLayout, featNum, strSplit[1], switchedOn);
                    break;
                case "SeekBar":
                    SeekBar(linearLayout, featNum, strSplit[1], Integer.parseInt(strSplit[2]), Integer.parseInt(strSplit[3]));
                    break;
                case "Button":
                    Button(linearLayout, featNum, strSplit[1]);
                    break;
                case "ButtonOnOff":
                    ButtonOnOff(linearLayout, featNum, strSplit[1], switchedOn);
                    break;
                case "Spinner":
                    TextView(linearLayout, strSplit[1]);
                    Spinner(linearLayout, featNum, strSplit[1], strSplit[2]);
                    break;
                case "InputText":
                    InputText(linearLayout, featNum, strSplit[1]);
                    break;
                case "InputValue":
                    if (strSplit.length == 3)
                        InputNum(linearLayout, featNum, strSplit[2], Integer.parseInt(strSplit[1]));
                    if (strSplit.length == 2)
                        InputNum(linearLayout, featNum, strSplit[1], 0);
                    break;
                case "CheckBox":
                    CheckBox(linearLayout, featNum, strSplit[1], switchedOn);
                    break;
                case "RadioButton":
                    RadioButton(linearLayout, featNum, strSplit[1], strSplit[2]);
                    break;
                case "Collapse":
                    Collapse(linearLayout, strSplit[1], switchedOn);
                    subFeat++;
                    break;
                case "ButtonLink":
                    subFeat++;
                    ButtonLink(linearLayout, strSplit[1], strSplit[2]);
                    break;
                case "Category":
                    subFeat++;
                    Category(linearLayout, strSplit[1]);
                    break;
                case "RichTextView":
                    subFeat++;
                    TextView(linearLayout, strSplit[1]);
                    break;
                case "RichWebView":
                    subFeat++;
                    WebTextView(linearLayout, strSplit[1]);
                    break;
                case "OnceCheckBox":
                    subFeat++;
                    OnceCheckBox(linearLayout, featNum, strSplit[1]);
                    break;
                case "FormationCopy":
                    subFeat++;
                    FormationCopy(linearLayout, featNum, strSplit[1]);
                    break;
            }
        }
    }

    private void FormationCopy(LinearLayout linLayout, final int featNum, final String featName) {
        final Button button = new Button(getContext);
        button.setText(featName);
        LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
        layoutParams.setMargins(7, 5, 7, 5);
        button.setLayoutParams(layoutParams);
        button.setTextColor(TEXT_COLOR_2);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH) {
            button.setAllCaps(false); //Disable caps to support html
        }
        button.setBackgroundColor(BTN_COLOR);
        button.setOnClickListener((buttonView) -> {
            String s = GetCurrentFormation();
            Toast.makeText(getContext, "已将阵型拷贝至剪切板", Toast.LENGTH_SHORT).show();
            ((ClipboardManager) getContext.getSystemService(Context.CLIPBOARD_SERVICE)).setPrimaryClip(ClipData.newPlainText("c", s));

        });
        linLayout.addView(button);
    }
//    private void SelectValueXY(LinearLayout linearLayout, String featName1, int featNum1, int minValue1, int maxValue1, String featName2, int featNum2, int minValue2, int maxValue2) {
//        LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
//        layoutParams.weight = 1;
//        LinearLayout layout = new LinearLayout(getContext);
//        layout.setOrientation(LinearLayout.HORIZONTAL);
//        NumberPicker numberPicker1 = new NumberPicker(getContext);
//        numberPicker1.setMaxValue(maxValue1);
//        numberPicker1.setMinValue(minValue1);
//        numberPicker1.setLayoutParams(layoutParams);
//        numberPicker1.setOnValueChangedListener((numberPicker, i, i1) -> Preferences.changeFeatureInt(featName1, featNum1, numberPicker.getValue()));
//        NumberPicker numberPicker2 = new NumberPicker(getContext);
//        numberPicker2.setMaxValue(maxValue2);
//        numberPicker2.setMinValue(minValue2);
//        numberPicker2.setLayoutParams(layoutParams);
//        numberPicker2.setOnValueChangedListener((numberPicker, i, i1) -> Preferences.changeFeatureInt(featName2, featNum2, numberPicker.getValue()));
//
//        Field[] pickerFields = NumberPicker.class.getDeclaredFields();
//        for (Field field : pickerFields) {
//            if (field.getName().equals("mSelectionDivider")) {
//                field.setAccessible(true);
//                try {
//                    field.set(numberPicker1, null);
//                    field.set(numberPicker2, null);
//                } catch (Exception ignored) {
//                }
//                break;
//            }
//        }
//
//        layout.addView(numberPicker1);
//        layout.addView(numberPicker2);
//        linearLayout.addView(layout);
//    }

    private void OnceCheckBox(LinearLayout linLayout, final int featNum, final String featName) {
        final Button button = new Button(getContext);
        button.setText(featName);
        LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
        layoutParams.setMargins(7, 5, 7, 5);
        button.setLayoutParams(layoutParams);
        button.setTextColor(TEXT_COLOR_2);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH) {
            button.setAllCaps(false); //Disable caps to support html
        }
        button.setBackgroundColor(BTN_COLOR);
        button.setOnClickListener((buttonView) -> Preferences.changeFeatureBool(featName, featNum, true));
        linLayout.addView(button);
    }

    private void Switch(LinearLayout linLayout, final int featNum, final String featName, boolean swiOn) {
        if (android.os.Build.VERSION.SDK_INT < Build.VERSION_CODES.ICE_CREAM_SANDWICH) {
            final CheckBox switchR = new CheckBox(getContext);
            switchR.setText(featName);
            switchR.setTextColor(TEXT_COLOR_2);
            switchR.setPadding(10, 5, 0, 5);
            switchR.setChecked(Preferences.loadPrefBool(featName, featNum, swiOn));
            switchR.setOnCheckedChangeListener((compoundButton, bool) -> {
                Preferences.changeFeatureBool(featName, featNum, bool);
                if (featNum == -1) { //Save perferences
                    Preferences.with(switchR.getContext()).writeBoolean(-1, bool);
                    if (!bool)
                        Preferences.with(switchR.getContext()).clear();
                }
            });
            linLayout.addView(switchR);
            return;
        }
        final Switch switchR = new Switch(getContext);
        ColorStateList buttonStates = new ColorStateList(
                new int[][]{
                        new int[]{-android.R.attr.state_enabled},
                        new int[]{android.R.attr.state_checked},
                        new int[]{}
                },
                new int[]{
                        Color.BLUE,
                        ToggleON, // ON
                        ToggleOFF // OFF
                }
        );
        //Set colors of the switch. Comment out if you don't like it
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            try {
                switchR.getThumbDrawable().setTintList(buttonStates);
                switchR.getTrackDrawable().setTintList(buttonStates);
            } catch (NullPointerException ex) {
                Log.d(TAG, String.valueOf(ex));
            }
        }
        switchR.setText(featName);
        switchR.setTextColor(TEXT_COLOR_2);
        switchR.setPadding(10, 5, 0, 5);
        switchR.setChecked(Preferences.loadPrefBool(featName, featNum, swiOn));
        switchR.setOnCheckedChangeListener((compoundButton, bool) -> {
            Preferences.changeFeatureBool(featName, featNum, bool);
            if (featNum == -1) { //Save perferences
                Preferences.with(switchR.getContext()).writeBoolean(-1, bool);
                if (!bool)
                    Preferences.with(switchR.getContext()).clear(); //Clear perferences if switched off
            }
        });

        linLayout.addView(switchR);
    }

    private void SeekBar(LinearLayout linLayout, final int featNum, final String featName, final int min, int max) {
        int loadedProg = Preferences.loadPrefInt(featName, featNum);
        LinearLayout linearLayout = new LinearLayout(getContext);
        linearLayout.setPadding(10, 5, 0, 5);
        linearLayout.setOrientation(LinearLayout.VERTICAL);
        linearLayout.setGravity(Gravity.CENTER);

        final TextView textView = new TextView(getContext);
        textView.setText(Html.fromHtml(featName + ": <font color='" + NumberTxtColor + "'>" + ((loadedProg == 0) ? min : loadedProg)));
        textView.setTextColor(TEXT_COLOR_2);

        SeekBar seekBar = new SeekBar(getContext);
        seekBar.setPadding(25, 10, 35, 10);
        seekBar.setMax(max);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O)
            seekBar.setMin(min); //setMin for Oreo and above
        seekBar.setProgress((loadedProg == 0) ? min : loadedProg);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN) {
            seekBar.getThumb().setColorFilter(SeekBarColor, PorterDuff.Mode.SRC_ATOP);
        }
        seekBar.getProgressDrawable().setColorFilter(SeekBarProgressColor, PorterDuff.Mode.SRC_ATOP);
        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            public void onStopTrackingTouch(SeekBar seekBar) {
            }

            public void onProgressChanged(SeekBar seekBar, int i, boolean z) {
                //if progress is greater than minimum, don't go below. Else, set progress
                seekBar.setProgress(Math.max(i, min));
                Preferences.changeFeatureInt(featName, featNum, Math.max(i, min));
                textView.setText(Html.fromHtml(featName + ": <font color='" + NumberTxtColor + "'>" + (Math.max(i, min))));
            }
        });
        linearLayout.addView(textView);
        linearLayout.addView(seekBar);

        linLayout.addView(linearLayout);
    }

    private void Button(LinearLayout linLayout, final int featNum, final String featName) {
        final Button button = new Button(getContext);
        LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
        layoutParams.setMargins(7, 5, 7, 5);
        button.setLayoutParams(layoutParams);
        button.setTextColor(TEXT_COLOR_2);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH) {
            button.setAllCaps(false); //Disable caps to support html
        }
        button.setText(Html.fromHtml(featName));
        button.setBackgroundColor(BTN_COLOR);
        button.setOnClickListener(v -> {
            switch (featNum) {

                case -6:
                    settingsOpen = !settingsOpen;
                    scrollView.removeView(mSettings);
                    scrollView.addView(mods);
                    break;
            }
            Preferences.changeFeatureInt(featName, featNum, 0);
        });

        linLayout.addView(button);
    }

    private void ButtonLink(LinearLayout linLayout, final String featName, final String url) {
        final Button button = new Button(getContext);
        LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
        layoutParams.setMargins(7, 5, 7, 5);
        button.setLayoutParams(layoutParams);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH) {
            button.setAllCaps(false); //Disable caps to support html
        }
        button.setTextColor(TEXT_COLOR_2);
        button.setText(Html.fromHtml(featName));
        button.setBackgroundColor(BTN_COLOR);
        button.setOnClickListener(v -> {
            Intent intent = new Intent(Intent.ACTION_VIEW);
            intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            intent.setData(Uri.parse(url));
            getContext.startActivity(intent);
        });
        linLayout.addView(button);
    }

    private void ButtonOnOff(LinearLayout linLayout, final int featNum, String featName, boolean switchedOn) {
        final Button button = new Button(getContext);
        LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
        layoutParams.setMargins(7, 5, 7, 5);
        button.setLayoutParams(layoutParams);
        button.setTextColor(TEXT_COLOR_2);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH) {
            button.setAllCaps(false); //Disable caps to support html
        }

        final String finalfeatName = featName.replace("OnOff_", "");
        boolean isOn = Preferences.loadPrefBool(featName, featNum, switchedOn);
        if (isOn) {
            button.setText(Html.fromHtml(finalfeatName + ": ON"));
            button.setBackgroundColor(BtnON);
            isOn = false;
        } else {
            button.setText(Html.fromHtml(finalfeatName + ": OFF"));
            button.setBackgroundColor(BtnOFF);
            isOn = true;
        }
        final boolean finalIsOn = isOn;
        button.setOnClickListener(new View.OnClickListener() {
            boolean isOn = finalIsOn;

            public void onClick(View v) {
                Preferences.changeFeatureBool(finalfeatName, featNum, isOn);
                //Log.d(TAG, finalfeatName + " " + featNum + " " + isActive2);
                if (isOn) {
                    button.setText(Html.fromHtml(finalfeatName + ": ON"));
                    button.setBackgroundColor(BtnON);
                    isOn = false;
                } else {
                    button.setText(Html.fromHtml(finalfeatName + ": OFF"));
                    button.setBackgroundColor(BtnOFF);
                    isOn = true;
                }
            }
        });
        linLayout.addView(button);
    }

    private void Spinner(LinearLayout linLayout, final int featNum, final String featName, final String list) {
//        Log.d(TAG, "spinner " + featNum + " " + featName + " " + list);
        final List<String> lists = new LinkedList<>(Arrays.asList(list.split(",")));

        // Create another LinearLayout as a workaround to use it as a background
        // to keep the down arrow symbol. No arrow symbol if setBackgroundColor set
        LinearLayout linearLayout2 = new LinearLayout(getContext);
        LinearLayout.LayoutParams layoutParams2 = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        layoutParams2.setMargins(7, 2, 7, 2);
        linearLayout2.setOrientation(LinearLayout.VERTICAL);
        linearLayout2.setBackgroundColor(BTN_COLOR);
        linearLayout2.setLayoutParams(layoutParams2);

        final Spinner spinner = new Spinner(getContext, Spinner.MODE_DIALOG);
        spinner.setLayoutParams(layoutParams2);
        spinner.setDescendantFocusability(ViewGroup.FOCUS_BLOCK_DESCENDANTS);
        spinner.getBackground().setColorFilter(1, PorterDuff.Mode.SRC_ATOP); //trick to show blue down arrow color
        //Creating the ArrayAdapter instance having the list
        ArrayAdapter<String> aa = new ArrayAdapter<>(getContext, android.R.layout.simple_spinner_dropdown_item, lists);
        aa.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        //Setting the ArrayAdapter data on the Spinner'
        spinner.setAdapter(aa);
        spinner.setSelection(Preferences.loadPrefInt(featName, featNum));
        spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parentView, View selectedItemView, int position, long id) {
                Preferences.changeFeatureInt(spinner.getSelectedItem().toString(), featNum, position);
                ((TextView) parentView.getChildAt(0)).setTextColor(TEXT_COLOR_2);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {
            }
        });
        linearLayout2.addView(spinner);
        linLayout.addView(linearLayout2);
    }

    private void InputNum(LinearLayout linLayout, final int featNum, final String featName, final int maxValue) {
        LinearLayout linearLayout = new LinearLayout(getContext);
        LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
        layoutParams.setMargins(7, 5, 7, 5);

        final Button button = new Button(getContext);
        int num = Preferences.loadPrefInt(featName, featNum);
        button.setText(Html.fromHtml(featName + ": <font color='" + NumberTxtColor + "'>" + ((num == 0) ? 1 : num) + "</font>"));
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH) {
            button.setAllCaps(false);
        }
        button.setLayoutParams(layoutParams);
        button.setBackgroundColor(BTN_COLOR);
        button.setTextColor(TEXT_COLOR_2);
        button.setOnClickListener(view -> {
            AlertDialog.Builder alertName = new AlertDialog.Builder(getContext);
            final EditText editText = new EditText(getContext);
            if (maxValue != 0)
                editText.setHint("Max value: " + maxValue);
            editText.setInputType(InputType.TYPE_CLASS_NUMBER);
            editText.setKeyListener(DigitsKeyListener.getInstance("0123456789-"));
            InputFilter[] FilterArray = new InputFilter[1];
            FilterArray[0] = new InputFilter.LengthFilter(10);
            editText.setFilters(FilterArray);
            editText.setOnFocusChangeListener((v, hasFocus) -> {
                InputMethodManager imm = (InputMethodManager) getContext.getSystemService(Context.INPUT_METHOD_SERVICE);
                if (hasFocus) {
                    imm.toggleSoftInput(InputMethodManager.SHOW_FORCED, InputMethodManager.HIDE_IMPLICIT_ONLY);
                } else {
                    imm.toggleSoftInput(InputMethodManager.HIDE_IMPLICIT_ONLY, 0);
                }
            });
            editText.requestFocus();

            alertName.setTitle("Input number");
            alertName.setView(editText);
            LinearLayout layoutName = new LinearLayout(getContext);
            layoutName.setOrientation(LinearLayout.VERTICAL);
            layoutName.addView(editText); // displays the user input bar
            alertName.setView(layoutName);

            alertName.setPositiveButton("OK", (dialog, whichButton) -> {
                int num1;
                try {
                    num1 = Integer.parseInt(TextUtils.isEmpty(editText.getText().toString()) ? "0" : editText.getText().toString());
                    if (maxValue != 0 && num1 >= maxValue)
                        num1 = maxValue;
                } catch (NumberFormatException ex) {
                    if (maxValue != 0)
                        num1 = maxValue;
                    else
                        num1 = 2147483640;
                }

                button.setText(Html.fromHtml(featName + ": <font color='" + NumberTxtColor + "'>" + num1 + "</font>"));
                Preferences.changeFeatureInt(featName, featNum, num1);

                editText.setFocusable(false);
            });

            alertName.setNegativeButton("Cancel", (dialog, whichButton) -> {
                // dialog.cancel(); // closes dialog
                InputMethodManager imm = (InputMethodManager) getContext.getSystemService(Context.INPUT_METHOD_SERVICE);
                imm.toggleSoftInput(InputMethodManager.HIDE_IMPLICIT_ONLY, 0);
            });


            alertName.show();

        });

        linearLayout.addView(button);
        linLayout.addView(linearLayout);
    }

    private void InputText(LinearLayout linLayout, final int featNum, final String featName) {
        LinearLayout linearLayout = new LinearLayout(getContext);
        LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
        layoutParams.setMargins(7, 5, 7, 5);

        final Button button = new Button(getContext);

        String string = Preferences.loadPrefString(featName, featNum);
        button.setText(Html.fromHtml(featName + ": <font color='" + NumberTxtColor + "'>" + string + "</font>"));

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH) {
            button.setAllCaps(false);
        }
        button.setLayoutParams(layoutParams);
        button.setBackgroundColor(BTN_COLOR);
        button.setTextColor(TEXT_COLOR_2);
        button.setOnClickListener(view -> {
            AlertDialog.Builder alertName = new AlertDialog.Builder(getContext);

            final EditText editText = new EditText(getContext);
            editText.setOnFocusChangeListener((v, hasFocus) -> {
                InputMethodManager imm = (InputMethodManager) getContext.getSystemService(Context.INPUT_METHOD_SERVICE);
                if (hasFocus) {
                    imm.toggleSoftInput(InputMethodManager.SHOW_FORCED, InputMethodManager.HIDE_IMPLICIT_ONLY);
                } else {
                    imm.toggleSoftInput(InputMethodManager.HIDE_IMPLICIT_ONLY, 0);
                }
            });
            editText.requestFocus();

            alertName.setTitle("Input text");
            alertName.setView(editText);
            LinearLayout layoutName = new LinearLayout(getContext);
            layoutName.setOrientation(LinearLayout.VERTICAL);
            layoutName.addView(editText); // displays the user input bar
            alertName.setView(layoutName);

            alertName.setPositiveButton("OK", (dialog, whichButton) -> {
                String str = editText.getText().toString();
                button.setText(Html.fromHtml(featName + ": <font color='" + NumberTxtColor + "'>" + str + "</font>"));
                Preferences.changeFeatureString(featName, featNum, str);
                editText.setFocusable(false);
            });

            alertName.setNegativeButton("Cancel", (dialog, whichButton) -> {
                //dialog.cancel(); // closes dialog
                InputMethodManager imm = (InputMethodManager) getContext.getSystemService(Context.INPUT_METHOD_SERVICE);
                imm.toggleSoftInput(InputMethodManager.HIDE_IMPLICIT_ONLY, 0);
            });

            alertName.show();
        });

        linearLayout.addView(button);
        linLayout.addView(linearLayout);
    }

    private void CheckBox(LinearLayout linLayout, final int featNum, final String featName, boolean switchedOn) {
        final CheckBox checkBox = new CheckBox(getContext);
        checkBox.setText(featName);
        checkBox.setTextColor(TEXT_COLOR_2);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP)
            checkBox.setButtonTintList(ColorStateList.valueOf(CheckBoxColor));
        checkBox.setChecked(Preferences.loadPrefBool(featName, featNum, switchedOn));
        checkBox.setOnCheckedChangeListener((buttonView, isChecked) -> {
            if (checkBox.isChecked()) {
                Preferences.changeFeatureBool(featName, featNum, isChecked);
            } else {
                Preferences.changeFeatureBool(featName, featNum, isChecked);
            }
        });
        LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        params.gravity = Gravity.CENTER;
        checkBox.setLayoutParams(params);
        linLayout.addView(checkBox);
    }

    private void RadioButton(LinearLayout linLayout, final int featNum, String featName, final String list) {
        //CreditsPhase: LoraZalora
        final List<String> lists = new LinkedList<>(Arrays.asList(list.split(",")));

        final TextView textView = new TextView(getContext);
        textView.setText(String.format("%s:", featName));
        textView.setTextColor(TEXT_COLOR_2);

        final RadioGroup radioGroup = new RadioGroup(getContext);
        radioGroup.setPadding(10, 5, 10, 5);
        radioGroup.setOrientation(LinearLayout.VERTICAL);
        radioGroup.addView(textView);

        for (int i = 0; i < lists.size(); i++) {
            final RadioButton Radioo = new RadioButton(getContext);
            final String finalfeatName = featName, radioName = lists.get(i);
            View.OnClickListener first_radio_listener = v -> {
                textView.setText(Html.fromHtml(finalfeatName + ": <font color='" + NumberTxtColor + "'>" + radioName));
                Preferences.changeFeatureInt(finalfeatName, featNum, radioGroup.indexOfChild(Radioo));
            };
            System.out.println(lists.get(i));
            Radioo.setText(lists.get(i));
            Radioo.setTextColor(Color.LTGRAY);
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP)
                Radioo.setButtonTintList(ColorStateList.valueOf(RadioColor));
            Radioo.setOnClickListener(first_radio_listener);
            radioGroup.addView(Radioo);
        }

        int index = Preferences.loadPrefInt(featName, featNum);
        if (index > 0) { //Preventing it to get an index less than 1. below 1 = null = crash
            textView.setText(Html.fromHtml(featName + ": <font color='" + NumberTxtColor + "'>" + lists.get(index - 1)));
            ((RadioButton) radioGroup.getChildAt(index)).setChecked(true);
        }
        linLayout.addView(radioGroup);
    }

    private void Collapse(LinearLayout linLayout, final String text, final boolean expanded) {
        LinearLayout.LayoutParams layoutParamsLL = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
        layoutParamsLL.setMargins(0, 5, 0, 0);

        LinearLayout collapse = new LinearLayout(getContext);
        collapse.setLayoutParams(layoutParamsLL);
        collapse.setVerticalGravity(16);
        collapse.setOrientation(LinearLayout.VERTICAL);

        final LinearLayout collapseSub = new LinearLayout(getContext);
        collapseSub.setVerticalGravity(16);
        collapseSub.setPadding(0, 5, 0, 5);
        collapseSub.setOrientation(LinearLayout.VERTICAL);
        collapseSub.setBackgroundColor(Color.parseColor("#222D38"));
        collapseSub.setVisibility(View.GONE);
        mCollapse = collapseSub;

        final TextView textView = new TextView(getContext);
        textView.setBackgroundColor(CategoryBG);
        textView.setText(String.format("▽ %s ▽", text));
        textView.setGravity(Gravity.CENTER);
        textView.setTextColor(TEXT_COLOR_2);
        textView.setTypeface(null, Typeface.BOLD);
        textView.setPadding(0, 20, 0, 20);

        if (expanded) {
            collapseSub.setVisibility(View.VISIBLE);
            textView.setText(String.format("△ %s △", text));
        }

        textView.setOnClickListener(new View.OnClickListener() {
            boolean isChecked = expanded;

            @Override
            public void onClick(View v) {

                boolean z = !isChecked;
                isChecked = z;
                if (z) {
                    collapseSub.setVisibility(View.VISIBLE);
                    textView.setText(String.format("△ %s △", text));
                    return;
                }
                collapseSub.setVisibility(View.GONE);
                textView.setText(String.format("▽ %s ▽", text));
            }
        });
        collapse.addView(textView);
        collapse.addView(collapseSub);
        linLayout.addView(collapse);
    }

    private void Category(LinearLayout linLayout, String text) {
        TextView textView = new TextView(getContext);
        textView.setBackgroundColor(CategoryBG);
        textView.setText(Html.fromHtml(text));
        textView.setGravity(Gravity.CENTER);
        textView.setTextColor(TEXT_COLOR_2);
        textView.setTypeface(null, Typeface.BOLD);
        textView.setPadding(0, 5, 0, 5);
        linLayout.addView(textView);
    }

    private void TextView(LinearLayout linLayout, String text) {
        TextView textView = new TextView(getContext);
        textView.setText(Html.fromHtml(text));
        textView.setTextColor(TEXT_COLOR_2);
        textView.setPadding(10, 5, 10, 5);
        linLayout.addView(textView);
    }

    private void WebTextView(LinearLayout linLayout, String text) {
        WebView wView = new WebView(getContext);
        wView.loadData(text, "text/html", "utf-8");
        wView.setBackgroundColor(0x00000000); //Transparent
        wView.setPadding(0, 5, 0, 5);
        linLayout.addView(wView);
    }

    private boolean isViewCollapsed() {
        return rootFrame == null || mCollapsed.getVisibility() == View.VISIBLE;
    }


    private int dp(int i) {
        return (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, (float) i, getContext.getResources().getDisplayMetrics());
    }

    public void onDestroy() {
        if (rootFrame != null) {
            mWindowManager.removeView(rootFrame);
        }
    }
}