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

 import static com.transmension.mobile.Global.currentSelectedSeed;
 import static com.transmension.mobile.Global.globalIsSelected;

 import android.app.ActionBar;
 import android.app.Activity;
 import android.content.Context;
 import android.content.Intent;
 import android.content.SharedPreferences;
 import android.content.res.Configuration;
 import android.graphics.Color;
 import android.graphics.drawable.ColorDrawable;
 import android.os.Build;
 import android.os.Bundle;
 import android.view.Gravity;
 import android.view.KeyEvent;
 import android.view.View;
 import android.view.Window;
 import android.widget.Button;
 import android.widget.LinearLayout;
 import android.widget.NumberPicker;
 import android.widget.ScrollView;
 import android.widget.TableLayout;
 import android.widget.TextView;
 import android.widget.Toast;

 import com.trans.pvztv.R;

 public class CustomBalanceAdjustment extends Activity {

     private SharedPreferences sharedPreferences;
     private NumberPicker costPicker;
     private NumberPicker refreshPicker;

     @Override
     protected void onCreate(Bundle savedInstanceState) {
         super.onCreate(savedInstanceState);

         boolean isNight = (getResources().getConfiguration().uiMode & Configuration.UI_MODE_NIGHT_YES) == Configuration.UI_MODE_NIGHT_YES;

         if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH)
             setTheme(isNight ? android.R.style.Theme_DeviceDefault : android.R.style.Theme_DeviceDefault_Light);
         Window window = getWindow();
         if (!isNight) {
             window.getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR | View.SYSTEM_UI_FLAG_LIGHT_NAVIGATION_BAR);
         }
         if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q)
             window.setNavigationBarContrastEnforced(false);
         if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
             window.setStatusBarColor(Color.TRANSPARENT);
             window.setNavigationBarColor(Color.TRANSPARENT);
         }
         ActionBar actionBar = getActionBar();
         if (actionBar != null) {
             actionBar.setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));
         }

         // 配置标题
         setTitle(getString(R.string.custom_balance_title));

         // 初始化SharedPreferences
         sharedPreferences = getSharedPreferences("data", 0);

         ScrollView scrollView = new ScrollView(this);
         LinearLayout linearLayout = new LinearLayout(this);
         TextView intro = new TextView(this);
         intro.setGravity(Gravity.CENTER);
         intro.setText(R.string.addon_custom_balance_adjustment_info);
         intro.setTextSize(20f);

         TableLayout tableLayout = new TableLayout(this);
         tableLayout.setLayoutParams(new TableLayout.LayoutParams(TableLayout.LayoutParams.MATCH_PARENT, TableLayout.LayoutParams.WRAP_CONTENT));
         tableLayout.setStretchAllColumns(false);

         LinearLayout.LayoutParams wrapWrapParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
         wrapWrapParams.gravity = Gravity.CENTER;
         LinearLayout.LayoutParams matchWrapParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
         matchWrapParams.gravity = Gravity.CENTER;
         LinearLayout.LayoutParams weightParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
         weightParams.weight = 1;
         LinearLayout.LayoutParams marginParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
         marginParams.setMargins(0, 20, 0, 20);

         Button seedPicker = new Button(this);
         boolean isSelected = globalIsSelected;
         seedPicker.setText(isSelected ? currentSelectedSeed : "选择卡片类型");
         seedPicker.setTextSize(17f);
         seedPicker.setOnClickListener(view -> {
             startActivity(new Intent(CustomBalanceAdjustment.this, SeedChooser.class));
             finish();
         });

         LinearLayout balanceChooser = new LinearLayout(this);
         balanceChooser.setOrientation(LinearLayout.HORIZONTAL);

         TextView costInfo = new TextView(this);
         costInfo.setGravity(Gravity.CENTER);
         costInfo.setText("花费");
         costInfo.setTextSize(15f);

         // 初始化UI组件
         NumberPicker costPicker = getCostPicker();
         NumberPicker refreshPicker = getRefreshPicker();

         // 配置NumberPicker
         costPicker.setLayoutParams(weightParams);
         costPicker.setValue(sharedPreferences.getInt("cost", 0));
         refreshPicker.setLayoutParams(weightParams);
         refreshPicker.setValue(sharedPreferences.getInt("refresh", 0));

         // 配置文本
         TextView refreshInfo = new TextView(this);
         refreshInfo.setGravity(Gravity.CENTER);
         refreshInfo.setText("冷却时间");
         refreshInfo.setTextSize(15f);

         balanceChooser.addView(costInfo);
         balanceChooser.addView(costPicker);
         balanceChooser.addView(refreshInfo);
         balanceChooser.addView(refreshPicker);

         // 设置保存按钮
         Button saveButton = new Button(this);
         saveButton.setText("保存修改");
         saveButton.setTextSize(17f);
         saveButton.setOnClickListener(view -> {
             saveSeedConfig();
         });

         linearLayout.setOrientation(LinearLayout.VERTICAL);
         linearLayout.addView(intro);
         linearLayout.addView(tableLayout);
         linearLayout.addView(seedPicker);
         linearLayout.addView(balanceChooser);
         linearLayout.addView(saveButton);
         scrollView.addView(linearLayout);
         setContentView(scrollView);
     }

     private NumberPicker getCostPicker() {
         costPicker = new NumberPicker(this);
         costPicker.setFormatter(new NumberPicker.Formatter() {
             @Override
             public String format(int value) {
                 return String.valueOf(value * 25); // 步长25
             }
         });
         costPicker.setMinValue(0);
         costPicker.setMaxValue(12);

         return costPicker;
     }

     private NumberPicker getRefreshPicker() {
         refreshPicker = new NumberPicker(this);
         refreshPicker.setFormatter(value -> {
             switch (value) {
                 case 0:
                     return "短";
                 case 1:
                     return "中等";
                 case 2:
                     return "长";
                 case 3:
                     return "很长";
                 default:
                     return "0";
             }
         });
         refreshPicker.setMinValue(0);
         refreshPicker.setMaxValue(3);

         return refreshPicker;
     }

     private void onSeedSelected(String seedName) {
         currentSelectedSeed = seedName;
//         selectedSeedView.setText("当前选择: " + seedName);

         // 加载已保存的值
         costPicker.setValue(sharedPreferences.getInt(seedName + "_cost", 100));
         refreshPicker.setValue(sharedPreferences.getInt(seedName + "_refresh", 30));
     }


     private void saveSeedConfig() {
         if (currentSelectedSeed == null) {
             Toast.makeText(this, "请先选择种子", Toast.LENGTH_SHORT).show();
             return;
         }

         sharedPreferences.edit()
                 .putInt(currentSelectedSeed + "_cost", costPicker.getValue())
                 .putInt(currentSelectedSeed + "_refresh", refreshPicker.getValue())
                 .apply();

         Toast.makeText(this,
                 currentSelectedSeed + "配置已保存",
                 Toast.LENGTH_SHORT).show();
     }

     @Override
     public boolean onKeyDown(int keyCode, KeyEvent event) {

         return super.onKeyDown(keyCode, event);
     }

     static class MyTextView extends TextView {

         public final int preferenceId;

         public MyTextView(Context context) {
             super(context);
             preferenceId = -1;
         }

         public MyTextView(Context context, int id) {
             super(context);
             preferenceId = id;
         }
     }

 }