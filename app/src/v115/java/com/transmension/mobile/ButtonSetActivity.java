package com.transmension.mobile;

import android.app.Activity;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.text.Editable;
import android.text.InputFilter;
import android.text.InputType;
import android.text.TextWatcher;
import android.util.DisplayMetrics;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowInsets;
import android.view.WindowInsetsController;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.SeekBar;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;
import android.widget.Toast;

import com.trans.pvztv.R;

public class ButtonSetActivity extends Activity {
    public int SCREEN_WIDTH, SCREEN_HEIGHT;
    boolean isAddonWindowLoaded = false;
    private WindowManager mWindowManager;
    private WindowManager.LayoutParams enterParams, backParams, shovelParams, hammerParams, dpadParams, visibilityParams, stopParams;
    private ImageView enterButton, backButton, shovelButton, hammerButton, dpadButton, visibilityWindow, stopButton;
    private SeekBar seekBarSize, seekBarTran;
    private TableRow tableRow1, tableRow2;
    private EditText editSize, editTran;
    private CheckBox checkKeep;
    private ViewsToSet viewToSet = ViewsToSet.NULL_BUTTON;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH)
            setTheme(android.R.style.Theme_DeviceDefault_NoActionBar_Fullscreen);

        //设置导航栏透明，UI会好看些
        Window window = getWindow();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN)
            window.getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q)
            window.setNavigationBarContrastEnforced(false);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            window.setStatusBarColor(Color.TRANSPARENT);
            window.setNavigationBarColor(Color.TRANSPARENT);
        }
        if (Build.VERSION.SDK_INT >= 28) {
            window.getAttributes().layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
        } else {
            window.setFlags(1024, 1024);
        }
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            window.setDecorFitsSystemWindows(false);
            WindowInsetsController insetsController = getWindow().getInsetsController();
            if (insetsController != null) {
                insetsController.hide(WindowInsets.Type.statusBars() | WindowInsets.Type.displayCutout());
                insetsController.setSystemBarsBehavior(WindowInsetsController.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE);
            }
        }

        SharedPreferences sharedPreferences = getSharedPreferences("data", 0);
        ScrollView scrollView = new ScrollView(this);
        LinearLayout linearLayout = new LinearLayout(this);
        linearLayout.setOrientation(LinearLayout.VERTICAL);


        TextView helpText = new TextView(this);
        helpText.setGravity(Gravity.CENTER);
        helpText.setText("请拖动各个按钮至您想要的位置。");
        helpText.setTextSize(20f);

        LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        layoutParams.gravity = Gravity.CENTER;

        Button resetButton = new Button(this);
        resetButton.setText("长按此处，让键盘恢复默认设置");
        resetButton.setTextSize(17f);
        resetButton.setOnClickListener(view -> Toast.makeText(ButtonSetActivity.this, "请长按", Toast.LENGTH_SHORT).show());
        resetButton.setOnLongClickListener(view -> {
            sharedPreferences.edit()
                    .remove("enterSize").remove("enterX").remove("enterY").remove("enterTran").remove("enterKeep")
                    .remove("backSize").remove("backX").remove("backY").remove("backTran").remove("backKeep")
                    .remove("dpadSize").remove("dpadX").remove("dpadY").remove("dpadTran").remove("dpadKeep")
                    .remove("shovelSize").remove("shovelX").remove("shovelY").remove("shovelTran").remove("shovelKeep")
                    .remove("hammerSize").remove("hammerX").remove("hammerY").remove("hammerTran").remove("hammerKeep")
                    .remove("visibilitySize").remove("visibilityX").remove("visibilityY").remove("visibilityTran")
                    .remove("stopSize").remove("stopX").remove("stopY").remove("stopTran").remove("stopKeep")
                    .remove("longPress")
                    .remove("keyCodePause")
                    .remove("isVisibilityLockPosition")
                    .apply();
            Toast.makeText(ButtonSetActivity.this, "成功恢复默认设置，请重新打开本设置界面查看", Toast.LENGTH_SHORT).show();
            new Handler().postDelayed(ButtonSetActivity.this::finish, 1000);
            return true;
        });

        TableLayout tableLayout = new TableLayout(this);
        tableLayout.setLayoutParams(new TableLayout.LayoutParams(TableLayout.LayoutParams.MATCH_PARENT, TableLayout.LayoutParams.WRAP_CONTENT));
        tableLayout.setStretchAllColumns(false);


        tableRow1 = new TableRow(this);
        tableRow1.setLayoutParams(new TableRow.LayoutParams(TableRow.LayoutParams.MATCH_PARENT, TableRow.LayoutParams.WRAP_CONTENT));
        tableRow1.setVisibility(View.INVISIBLE);
        TextView textView1 = new TextView(this);
        textView1.setTextAppearance(this, android.R.style.TextAppearance_Medium);
        textView1.setTextColor(getResources().getColor(android.R.color.white));
        textView1.setGravity(Gravity.CENTER);
        textView1.setText("大小");
        TableRow.LayoutParams params1 = new TableRow.LayoutParams(TableRow.LayoutParams.WRAP_CONTENT, TableRow.LayoutParams.WRAP_CONTENT);
        textView1.setLayoutParams(params1);
        seekBarSize = new SeekBar(this);
        seekBarSize.setMax(250);
        seekBarSize.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                int size = (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, i, getResources().getDisplayMetrics());
                switch (viewToSet) {
                    case ENTER_BUTTON:
                        enterParams.width = size;
                        enterParams.height = size;
                        mWindowManager.updateViewLayout(enterButton, enterParams);
                        sharedPreferences.edit().putInt("enterSize", i).apply();
                        break;
                    case BACK_BUTTON:
                        backParams.width = size;
                        backParams.height = size;
                        mWindowManager.updateViewLayout(backButton, backParams);
                        sharedPreferences.edit().putInt("backSize", i).apply();
                        break;
                    case DPAD_BUTTON:
                        dpadParams.width = size;
                        dpadParams.height = size;
                        mWindowManager.updateViewLayout(dpadButton, dpadParams);
                        sharedPreferences.edit().putInt("dpadSize", i).apply();
                        break;
                    case SHOVEL_BUTTON:
                        shovelParams.width = size;
                        shovelParams.height = size;
                        mWindowManager.updateViewLayout(shovelButton, shovelParams);
                        sharedPreferences.edit().putInt("shovelSize", i).apply();
                        break;
                    case HAMMER_BUTTON:
                        hammerParams.width = size;
                        hammerParams.height = size;
                        mWindowManager.updateViewLayout(hammerButton, hammerParams);
                        sharedPreferences.edit().putInt("hammerSize", i).apply();
                        break;
                    case VISIBILITY_WINDOW:
                        visibilityParams.width = size;
                        visibilityParams.height = size;
                        mWindowManager.updateViewLayout(visibilityWindow, visibilityParams);
                        sharedPreferences.edit().putInt("visibilitySize", i).apply();
                        break;
                    case STOP_BUTTON:
                        stopParams.width = size;
                        stopParams.height = size;
                        mWindowManager.updateViewLayout(stopButton, stopParams);
                        sharedPreferences.edit().putInt("stopSize", i).apply();
                        break;
                }
                editSize.setText(String.valueOf(i));
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });
        params1 = new TableRow.LayoutParams(0, (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, 30, getResources().getDisplayMetrics()), 1f);
        seekBarSize.setLayoutParams(params1);
        InputFilter[] filters = new InputFilter[1];
        filters[0] = new InputFilter.LengthFilter(3);
        editSize = new EditText(this);
        editSize.setHint("大小(0~250)");
        editSize.setGravity(Gravity.CENTER);
        editSize.setInputType(InputType.TYPE_CLASS_NUMBER);
        editSize.setFilters(filters);
        editSize.setSelectAllOnFocus(true);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN)
            editSize.setBackground(null);
        editSize.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {
            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                if (charSequence == null || charSequence.length() == 0) return;
                int num = Integer.parseInt(String.valueOf(charSequence));
                seekBarSize.setProgress(Math.min(num, 250));
            }

            @Override
            public void afterTextChanged(Editable editable) {
            }
        });
        tableRow1.addView(textView1);
        tableRow1.addView(seekBarSize);
        tableRow1.addView(editSize);
        tableLayout.addView(tableRow1);


        tableRow2 = new TableRow(this);
        tableRow2.setLayoutParams(new TableRow.LayoutParams(TableRow.LayoutParams.MATCH_PARENT, TableRow.LayoutParams.WRAP_CONTENT));
        tableRow2.setVisibility(View.INVISIBLE);
        TextView textView2 = new TextView(this);
        textView2.setTextAppearance(this, android.R.style.TextAppearance_Medium);
        textView2.setTextColor(getResources().getColor(android.R.color.white));
        textView2.setGravity(Gravity.CENTER);
        textView2.setText("透明度");
        TableRow.LayoutParams params2 = new TableRow.LayoutParams(TableRow.LayoutParams.WRAP_CONTENT, TableRow.LayoutParams.WRAP_CONTENT);
        textView2.setLayoutParams(params2);
        seekBarTran = new SeekBar(this);
        seekBarTran.setMax(100);
        seekBarTran.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                switch (viewToSet) {
                    case ENTER_BUTTON:
                        enterParams.alpha = i / 100f;
                        mWindowManager.updateViewLayout(enterButton, enterParams);
                        sharedPreferences.edit().putInt("enterTran", i).apply();
                        break;
                    case BACK_BUTTON:
                        backParams.alpha = i / 100f;
                        mWindowManager.updateViewLayout(backButton, backParams);
                        sharedPreferences.edit().putInt("backTran", i).apply();
                        break;
                    case DPAD_BUTTON:
                        dpadParams.alpha = i / 100f;
                        mWindowManager.updateViewLayout(dpadButton, dpadParams);
                        sharedPreferences.edit().putInt("dpadTran", i).apply();
                        break;
                    case SHOVEL_BUTTON:
                        shovelParams.alpha = i / 100f;
                        mWindowManager.updateViewLayout(shovelButton, shovelParams);
                        sharedPreferences.edit().putInt("shovelTran", i).apply();
                        break;
                    case HAMMER_BUTTON:
                        hammerParams.alpha = i / 100f;
                        mWindowManager.updateViewLayout(hammerButton, hammerParams);
                        sharedPreferences.edit().putInt("hammerTran", i).apply();
                        break;
                    case VISIBILITY_WINDOW:
                        visibilityParams.alpha = i / 100f;
                        mWindowManager.updateViewLayout(visibilityWindow, visibilityParams);
                        sharedPreferences.edit().putInt("visibilityTran", i).apply();
                        break;
                    case STOP_BUTTON:
                        stopParams.alpha = i / 100f;
                        mWindowManager.updateViewLayout(stopButton, stopParams);
                        sharedPreferences.edit().putInt("stopTran", i).apply();
                        break;
                }
                editTran.setText(String.valueOf(i));
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });
        params2 = new TableRow.LayoutParams(0, (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, 30, getResources().getDisplayMetrics()), 1f);
        seekBarTran.setLayoutParams(params2);
        editTran = new EditText(this);
        editTran.setHint("透明度(0~100)");
        editTran.setGravity(Gravity.CENTER);
        editTran.setInputType(InputType.TYPE_CLASS_NUMBER);
        editTran.setFilters(filters);
        editTran.setSelectAllOnFocus(true);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN)
            editTran.setBackground(null);
        editTran.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {
            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {

                if (charSequence == null || charSequence.length() == 0) return;
                int num = Integer.parseInt(String.valueOf(charSequence));
                seekBarTran.setProgress(Math.min(num, 100));
            }

            @Override
            public void afterTextChanged(Editable editable) {
            }
        });
        tableRow2.addView(textView2);
        tableRow2.addView(seekBarTran);
        tableRow2.addView(editTran);
        tableLayout.addView(tableRow2);
        linearLayout.addView(helpText);
        linearLayout.addView(resetButton);
        linearLayout.addView(tableLayout);
        checkKeep = new CheckBox(this);
        checkKeep.setText("始终显示此按钮(点击隐藏按钮后不隐藏)");
        checkKeep.setTextSize(18);
        checkKeep.setLayoutParams(layoutParams);
        checkKeep.setVisibility(View.INVISIBLE);
        checkKeep.setOnCheckedChangeListener((compoundButton, b) -> {
            switch (viewToSet) {
                case ENTER_BUTTON:
                    sharedPreferences.edit().putBoolean("enterKeep", b).apply();
                    break;
                case BACK_BUTTON:
                    sharedPreferences.edit().putBoolean("backKeep", b).apply();
                    break;
                case DPAD_BUTTON:
                    sharedPreferences.edit().putBoolean("dpadKeep", b).apply();
                    break;
                case SHOVEL_BUTTON:
                    sharedPreferences.edit().putBoolean("shovelKeep", b).apply();
                    break;
                case HAMMER_BUTTON:
                    sharedPreferences.edit().putBoolean("hammerKeep", b).apply();
                    break;
                case STOP_BUTTON:
                    sharedPreferences.edit().putBoolean("stopKeep", b).apply();
                    break;
            }
        });
        linearLayout.addView(checkKeep);
        scrollView.addView(linearLayout);
        setContentView(scrollView);
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        if (!isAddonWindowLoaded) {
            isAddonWindowLoaded = true;
            SharedPreferences sharedPreferences = getSharedPreferences("data", 0);
            mWindowManager = getWindowManager();
            DisplayMetrics metrics = getResources().getDisplayMetrics();
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1)
                mWindowManager.getDefaultDisplay().getRealMetrics(metrics);
            SCREEN_WIDTH = metrics.widthPixels;
            SCREEN_HEIGHT = metrics.heightPixels;
            float density = getResources().getDisplayMetrics().density;
            final int enterSize = (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, sharedPreferences.getInt("enterSize", 100), getResources().getDisplayMetrics());
            final int enterX = sharedPreferences.getInt("enterX", (int) (310 * density));
            final int enterY = sharedPreferences.getInt("enterY", (int) (125 * density));
            enterParams = new WindowManager.LayoutParams(enterSize, enterSize, enterX, enterY, WindowManager.LayoutParams.TYPE_APPLICATION_PANEL, WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |
                    WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN | WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL, PixelFormat.TRANSPARENT);
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P)
                enterParams.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
            enterParams.gravity = Gravity.CENTER;
            enterParams.alpha = sharedPreferences.getInt("enterTran", 90) / 100f;
            enterButton = new ImageView(this);
            enterButton.setImageDrawable(getResources().getDrawable(R.drawable.button_a));
            enterButton.setOnTouchListener(new View.OnTouchListener() {
                float lastX = 0, lastY = 0;

                @Override
                public boolean onTouch(View view, MotionEvent motionEvent) {

                    switch (motionEvent.getAction()) {
                        case MotionEvent.ACTION_DOWN:
                            viewToSet = ViewsToSet.ENTER_BUTTON;
                            tableRow1.setVisibility(View.VISIBLE);
                            seekBarSize.setProgress(sharedPreferences.getInt("enterSize", 100));
                            tableRow2.setVisibility(View.VISIBLE);
                            seekBarTran.setProgress(sharedPreferences.getInt("enterTran", 90));
                            checkKeep.setVisibility(View.VISIBLE);
                            checkKeep.setChecked(sharedPreferences.getBoolean("enterKeep", false));
                            lastX = motionEvent.getRawX();
                            lastY = motionEvent.getRawY();
                            break;
                        case MotionEvent.ACTION_MOVE:
                            float rawX = motionEvent.getRawX();
                            float rawY = motionEvent.getRawY();
                            int dx = Math.round(rawX - lastX);
                            int dy = Math.round(rawY - lastY);
                            lastX += dx;
                            lastY += dy;
                            enterParams.x += dx;
                            enterParams.y += dy;
                            mWindowManager.updateViewLayout(view, enterParams);
                            break;
                        case MotionEvent.ACTION_UP:

                            //自动贴边
                            enterParams.x = Math.min(Math.max(enterParams.x, -(SCREEN_WIDTH - enterParams.width) / 2), (SCREEN_WIDTH - enterParams.width) / 2);
                            enterParams.y = Math.min(Math.max(enterParams.y, -(SCREEN_HEIGHT - enterParams.height) / 2), (SCREEN_HEIGHT - enterParams.height) / 2);
                            mWindowManager.updateViewLayout(view, enterParams);

                            //存储悬浮球位置
                            sharedPreferences.edit().putInt("enterX", enterParams.x).putInt("enterY", enterParams.y).apply();
                    }
                    return false;
                }
            });
            mWindowManager.addView(enterButton, enterParams);


            final int backSize = (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, sharedPreferences.getInt("backSize", 75), getResources().getDisplayMetrics());
            final int backX = sharedPreferences.getInt("backX", (int) (180 * density));
            final int backY = sharedPreferences.getInt("backY", (int) (135 * density));
            backParams = new WindowManager.LayoutParams(backSize, backSize, backX, backY, WindowManager.LayoutParams.TYPE_APPLICATION_PANEL, WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |
                    WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN | WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL, PixelFormat.TRANSPARENT);
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P)
                backParams.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
            backParams.gravity = Gravity.CENTER;
            backParams.alpha = sharedPreferences.getInt("backTran", 90) / 100f;
            backButton = new ImageView(this);
            backButton.setImageDrawable(getResources().getDrawable(R.drawable.button_b));
            backButton.setOnTouchListener(new View.OnTouchListener() {
                float lastX = 0, lastY = 0;

                @Override
                public boolean onTouch(View view, MotionEvent motionEvent) {

                    switch (motionEvent.getAction()) {
                        case MotionEvent.ACTION_DOWN:
                            viewToSet = ViewsToSet.BACK_BUTTON;
                            tableRow1.setVisibility(View.VISIBLE);
                            seekBarSize.setProgress(sharedPreferences.getInt("backSize", 75));
                            tableRow2.setVisibility(View.VISIBLE);
                            seekBarTran.setProgress(sharedPreferences.getInt("backTran", 90));
                            checkKeep.setVisibility(View.VISIBLE);
                            checkKeep.setChecked(sharedPreferences.getBoolean("backKeep", false));
                            lastX = motionEvent.getRawX();
                            lastY = motionEvent.getRawY();
                            break;
                        case MotionEvent.ACTION_MOVE:
                            float rawX = motionEvent.getRawX();
                            float rawY = motionEvent.getRawY();
                            int dx = Math.round(rawX - lastX);
                            int dy = Math.round(rawY - lastY);
                            lastX += dx;
                            lastY += dy;
                            backParams.x += dx;
                            backParams.y += dy;
                            mWindowManager.updateViewLayout(view, backParams);
                            break;
                        case MotionEvent.ACTION_UP:

                            //自动贴边
                            backParams.x = Math.min(Math.max(backParams.x, -(SCREEN_WIDTH - backParams.width) / 2), (SCREEN_WIDTH - backParams.width) / 2);
                            backParams.y = Math.min(Math.max(backParams.y, -(SCREEN_HEIGHT - backParams.height) / 2), (SCREEN_HEIGHT - backParams.height) / 2);
                            mWindowManager.updateViewLayout(view, backParams);

                            //存储悬浮球位置
                            sharedPreferences.edit().putInt("backX", backParams.x).putInt("backY", backParams.y).apply();
                    }
                    return false;
                }
            });
            mWindowManager.addView(backButton, backParams);


            final int dpadSize = (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, sharedPreferences.getInt("dpadSize", 200), getResources().getDisplayMetrics());
            final int dpadX = sharedPreferences.getInt("dpadX", (int) (-250 * density));
            final int dpadY = sharedPreferences.getInt("dpadY", (int) (85 * density));
            dpadParams = new WindowManager.LayoutParams(dpadSize, dpadSize, dpadX, dpadY, WindowManager.LayoutParams.TYPE_APPLICATION_PANEL, WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |
                    WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN | WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL, PixelFormat.TRANSPARENT);
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P)
                dpadParams.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
            dpadParams.gravity = Gravity.CENTER;
            dpadParams.alpha = sharedPreferences.getInt("dpadTran", 90) / 100f;
            dpadButton = new ImageView(this);
            dpadButton.setImageDrawable(getResources().getDrawable(R.drawable.dpad));
            dpadButton.setOnTouchListener(new View.OnTouchListener() {
                float lastX = 0, lastY = 0;

                @Override
                public boolean onTouch(View view, MotionEvent motionEvent) {

                    switch (motionEvent.getAction()) {
                        case MotionEvent.ACTION_DOWN:
                            viewToSet = ViewsToSet.DPAD_BUTTON;
                            tableRow1.setVisibility(View.VISIBLE);
                            seekBarSize.setProgress(sharedPreferences.getInt("dpadSize", 200));
                            tableRow2.setVisibility(View.VISIBLE);
                            seekBarTran.setProgress(sharedPreferences.getInt("dpadTran", 90));
                            checkKeep.setVisibility(View.VISIBLE);
                            checkKeep.setChecked(sharedPreferences.getBoolean("dpadKeep", false));
                            lastX = motionEvent.getRawX();
                            lastY = motionEvent.getRawY();
                            break;
                        case MotionEvent.ACTION_MOVE:
                            float rawX = motionEvent.getRawX();
                            float rawY = motionEvent.getRawY();
                            int dx = Math.round(rawX - lastX);
                            int dy = Math.round(rawY - lastY);
                            lastX += dx;
                            lastY += dy;
                            dpadParams.x += dx;
                            dpadParams.y += dy;
                            mWindowManager.updateViewLayout(view, dpadParams);
                            break;
                        case MotionEvent.ACTION_UP:

                            //自动贴边
                            dpadParams.x = Math.min(Math.max(dpadParams.x, -(SCREEN_WIDTH - dpadParams.width) / 2), (SCREEN_WIDTH - dpadParams.width) / 2);
                            dpadParams.y = Math.min(Math.max(dpadParams.y, -(SCREEN_HEIGHT - dpadParams.height) / 2), (SCREEN_HEIGHT - dpadParams.height) / 2);
                            mWindowManager.updateViewLayout(view, dpadParams);

                            //存储悬浮球位置
                            sharedPreferences.edit().putInt("dpadX", dpadParams.x).putInt("dpadY", dpadParams.y).apply();
                    }
                    return false;
                }
            });
            mWindowManager.addView(dpadButton, dpadParams);


            final int shovelSize = (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, sharedPreferences.getInt("shovelSize", 75), getResources().getDisplayMetrics());
            final int shovelX = sharedPreferences.getInt("shovelX", (int) (210 * density));
            final int shovelY = sharedPreferences.getInt("shovelY", (int) (25 * density));
            shovelParams = new WindowManager.LayoutParams(shovelSize, shovelSize, shovelX, shovelY, WindowManager.LayoutParams.TYPE_APPLICATION_PANEL, WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |
                    WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN | WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL, PixelFormat.TRANSPARENT);
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P)
                shovelParams.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
            shovelParams.gravity = Gravity.CENTER;
            shovelParams.alpha = sharedPreferences.getInt("shovelTran", 90) / 100f;
            shovelButton = new ImageView(this);
            shovelButton.setImageDrawable(getResources().getDrawable(R.drawable.button_x));
            shovelButton.setOnTouchListener(new View.OnTouchListener() {
                float lastX = 0, lastY = 0;

                @Override
                public boolean onTouch(View view, MotionEvent motionEvent) {

                    switch (motionEvent.getAction()) {
                        case MotionEvent.ACTION_DOWN:
                            viewToSet = ViewsToSet.SHOVEL_BUTTON;
                            tableRow1.setVisibility(View.VISIBLE);
                            seekBarSize.setProgress(sharedPreferences.getInt("shovelSize", 75));
                            tableRow2.setVisibility(View.VISIBLE);
                            seekBarTran.setProgress(sharedPreferences.getInt("shovelTran", 90));
                            checkKeep.setVisibility(View.VISIBLE);
                            checkKeep.setChecked(sharedPreferences.getBoolean("shovelKeep", false));
                            lastX = motionEvent.getRawX();
                            lastY = motionEvent.getRawY();
                            break;
                        case MotionEvent.ACTION_MOVE:
                            float rawX = motionEvent.getRawX();
                            float rawY = motionEvent.getRawY();
                            int dx = Math.round(rawX - lastX);
                            int dy = Math.round(rawY - lastY);
                            lastX += dx;
                            lastY += dy;
                            shovelParams.x += dx;
                            shovelParams.y += dy;
                            mWindowManager.updateViewLayout(view, shovelParams);
                            break;
                        case MotionEvent.ACTION_UP:

                            //自动贴边
                            shovelParams.x = Math.min(Math.max(shovelParams.x, -(SCREEN_WIDTH - shovelParams.width) / 2), (SCREEN_WIDTH - shovelParams.width) / 2);
                            shovelParams.y = Math.min(Math.max(shovelParams.y, -(SCREEN_HEIGHT - shovelParams.height) / 2), (SCREEN_HEIGHT - shovelParams.height) / 2);
                            mWindowManager.updateViewLayout(view, shovelParams);

                            //存储悬浮球位置
                            sharedPreferences.edit().putInt("shovelX", shovelParams.x).putInt("shovelY", shovelParams.y).apply();
                    }
                    return false;
                }
            });
            mWindowManager.addView(shovelButton, shovelParams);

            final int hammerSize = (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, sharedPreferences.getInt("hammerSize", 75), getResources().getDisplayMetrics());
            final int hammerX = sharedPreferences.getInt("hammerX", (int) (330 * density));
            final int hammerY = sharedPreferences.getInt("hammerY", (int) (7 * density));
            hammerParams = new WindowManager.LayoutParams(hammerSize, hammerSize, hammerX, hammerY, WindowManager.LayoutParams.TYPE_APPLICATION_PANEL, WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |
                    WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN | WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL, PixelFormat.TRANSPARENT);
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P)
                hammerParams.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
            hammerParams.gravity = Gravity.CENTER;
            hammerParams.alpha = sharedPreferences.getInt("hammerTran", 90) / 100f;
            hammerButton = new ImageView(this);
            hammerButton.setImageDrawable(getResources().getDrawable(R.drawable.button_y));
            hammerButton.setOnTouchListener(new View.OnTouchListener() {
                float lastX = 0, lastY = 0;

                @Override
                public boolean onTouch(View view, MotionEvent motionEvent) {

                    switch (motionEvent.getAction()) {
                        case MotionEvent.ACTION_DOWN:
                            viewToSet = ViewsToSet.HAMMER_BUTTON;
                            tableRow1.setVisibility(View.VISIBLE);
                            seekBarSize.setProgress(sharedPreferences.getInt("hammerSize", 75));
                            tableRow2.setVisibility(View.VISIBLE);
                            seekBarTran.setProgress(sharedPreferences.getInt("hammerTran", 90));
                            checkKeep.setVisibility(View.VISIBLE);
                            checkKeep.setChecked(sharedPreferences.getBoolean("hammerKeep", false));
                            lastX = motionEvent.getRawX();
                            lastY = motionEvent.getRawY();
                            break;
                        case MotionEvent.ACTION_MOVE:
                            float rawX = motionEvent.getRawX();
                            float rawY = motionEvent.getRawY();
                            int dx = Math.round(rawX - lastX);
                            int dy = Math.round(rawY - lastY);
                            lastX += dx;
                            lastY += dy;
                            hammerParams.x += dx;
                            hammerParams.y += dy;
                            mWindowManager.updateViewLayout(view, hammerParams);
                            break;
                        case MotionEvent.ACTION_UP:

                            //自动贴边
                            hammerParams.x = Math.min(Math.max(hammerParams.x, -(SCREEN_WIDTH - hammerParams.width) / 2), (SCREEN_WIDTH - hammerParams.width) / 2);
                            hammerParams.y = Math.min(Math.max(hammerParams.y, -(SCREEN_HEIGHT - hammerParams.height) / 2), (SCREEN_HEIGHT - hammerParams.height) / 2);
                            mWindowManager.updateViewLayout(view, hammerParams);

                            //存储悬浮球位置
                            sharedPreferences.edit().putInt("hammerX", hammerParams.x).putInt("hammerY", hammerParams.y).apply();
                    }
                    return false;
                }
            });
            mWindowManager.addView(hammerButton, hammerParams);


            final int visibilitySize = (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, sharedPreferences.getInt("visibilitySize", 40), getResources().getDisplayMetrics());
            final int visibilityX = sharedPreferences.getInt("visibilityX", (int) (380 * density));
            final int visibilityY = sharedPreferences.getInt("visibilityY", (int) (-110 * density));
            visibilityParams = new WindowManager.LayoutParams(visibilitySize, visibilitySize, visibilityX, visibilityY, WindowManager.LayoutParams.TYPE_APPLICATION_PANEL, WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |
                    WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN | WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL, PixelFormat.TRANSPARENT);
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P)
                visibilityParams.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
            visibilityParams.gravity = Gravity.CENTER;
            visibilityParams.alpha = sharedPreferences.getInt("visibilityTran", 90) / 100f;
            visibilityWindow = new ImageView(this);
            visibilityWindow.setImageDrawable(getResources().getDrawable(R.drawable.button_visible));
            visibilityWindow.setOnTouchListener(new View.OnTouchListener() {
                float lastX = 0, lastY = 0;

                @Override
                public boolean onTouch(View view, MotionEvent motionEvent) {

                    switch (motionEvent.getAction()) {
                        case MotionEvent.ACTION_DOWN:
                            viewToSet = ViewsToSet.VISIBILITY_WINDOW;
                            tableRow1.setVisibility(View.VISIBLE);
                            seekBarSize.setProgress(sharedPreferences.getInt("visibilitySize", 40));
                            tableRow2.setVisibility(View.VISIBLE);
                            seekBarTran.setProgress(sharedPreferences.getInt("visibilityTran", 90));
                            checkKeep.setVisibility(View.INVISIBLE);
                            lastX = motionEvent.getRawX();
                            lastY = motionEvent.getRawY();
                            break;
                        case MotionEvent.ACTION_MOVE:
                            float rawX = motionEvent.getRawX();
                            float rawY = motionEvent.getRawY();
                            int dx = Math.round(rawX - lastX);
                            int dy = Math.round(rawY - lastY);
                            lastX += dx;
                            lastY += dy;
                            visibilityParams.x += dx;
                            visibilityParams.y += dy;
                            mWindowManager.updateViewLayout(view, visibilityParams);
                            break;
                        case MotionEvent.ACTION_UP:

                            //自动贴边
                            visibilityParams.x = Math.min(Math.max(visibilityParams.x, -(SCREEN_WIDTH - visibilityParams.width) / 2), (SCREEN_WIDTH - visibilityParams.width) / 2);
                            visibilityParams.y = Math.min(Math.max(visibilityParams.y, -(SCREEN_HEIGHT - visibilityParams.height) / 2), (SCREEN_HEIGHT - visibilityParams.height) / 2);
                            mWindowManager.updateViewLayout(view, visibilityParams);

                            //存储悬浮球位置
                            sharedPreferences.edit().putInt("visibilityX", visibilityParams.x).putInt("visibilityY", visibilityParams.y).apply();
                    }
                    return false;
                }
            });
            mWindowManager.addView(visibilityWindow, visibilityParams);


            final int stopSize = (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, sharedPreferences.getInt("stopSize", 75), getResources().getDisplayMetrics());
            final int stopX = sharedPreferences.getInt("stopX", (int) (220 * density));
            final int stopY = sharedPreferences.getInt("stopY", (int) (-100 * density));
            stopParams = new WindowManager.LayoutParams(stopSize, stopSize, stopX, stopY, WindowManager.LayoutParams.TYPE_APPLICATION_PANEL, WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |
                    WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN | WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL, PixelFormat.TRANSPARENT);
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P)
                stopParams.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
            stopParams.gravity = Gravity.CENTER;
            stopParams.alpha = sharedPreferences.getInt("stopTran", 90) / 100f;
            stopButton = new ImageView(this);
            stopButton.setImageDrawable(getResources().getDrawable(R.drawable.button_stop));
            stopButton.setOnTouchListener(new View.OnTouchListener() {
                float lastX = 0, lastY = 0;

                @Override
                public boolean onTouch(View view, MotionEvent motionEvent) {

                    switch (motionEvent.getAction()) {
                        case MotionEvent.ACTION_DOWN:
                            viewToSet = ViewsToSet.STOP_BUTTON;
                            tableRow1.setVisibility(View.VISIBLE);
                            seekBarSize.setProgress(sharedPreferences.getInt("stopSize", 75));
                            tableRow2.setVisibility(View.VISIBLE);
                            seekBarTran.setProgress(sharedPreferences.getInt("stopTran", 90));
                            checkKeep.setVisibility(View.VISIBLE);
                            checkKeep.setChecked(sharedPreferences.getBoolean("stopKeep", false));
                            lastX = motionEvent.getRawX();
                            lastY = motionEvent.getRawY();
                            break;
                        case MotionEvent.ACTION_MOVE:
                            float rawX = motionEvent.getRawX();
                            float rawY = motionEvent.getRawY();
                            int dx = Math.round(rawX - lastX);
                            int dy = Math.round(rawY - lastY);
                            lastX += dx;
                            lastY += dy;
                            stopParams.x += dx;
                            stopParams.y += dy;
                            mWindowManager.updateViewLayout(view, stopParams);
                            break;
                        case MotionEvent.ACTION_UP:

                            stopParams.x = Math.min(Math.max(stopParams.x, -(SCREEN_WIDTH - stopParams.width) / 2), (SCREEN_WIDTH - stopParams.width) / 2);
                            stopParams.y = Math.min(Math.max(stopParams.y, -(SCREEN_HEIGHT - stopParams.height) / 2), (SCREEN_HEIGHT - stopParams.height) / 2);
                            mWindowManager.updateViewLayout(view, stopParams);

                            //存储悬浮球位置
                            sharedPreferences.edit().putInt("stopX", stopParams.x).putInt("stopY", stopParams.y).apply();
                    }
                    return false;
                }
            });
            mWindowManager.addView(stopButton, stopParams);

        }
        super.onWindowFocusChanged(hasFocus);
    }

    @Override
    protected void onDestroy() {
        mWindowManager.removeViewImmediate(enterButton);
        mWindowManager.removeViewImmediate(backButton);
        mWindowManager.removeViewImmediate(dpadButton);
        mWindowManager.removeViewImmediate(shovelButton);
        mWindowManager.removeViewImmediate(hammerButton);
        mWindowManager.removeViewImmediate(visibilityWindow);
        mWindowManager.removeViewImmediate(stopButton);
        super.onDestroy();
    }

    enum ViewsToSet {
        ENTER_BUTTON,
        BACK_BUTTON,
        SHOVEL_BUTTON,
        HAMMER_BUTTON,
        DPAD_BUTTON,
        VISIBILITY_WINDOW,
        STOP_BUTTON,
        NULL_BUTTON
    }

}
