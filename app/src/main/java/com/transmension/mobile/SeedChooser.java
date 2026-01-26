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

 import static com.transmension.mobile.Global.currentSelectedSeed;
 import static com.transmension.mobile.Global.globalIsSelected;
 import static com.transmension.mobile.Global.seedName;

 import android.app.ActionBar;
 import android.app.Activity;
 import android.content.Context;
 import android.content.Intent;
 import android.content.res.Configuration;
 import android.graphics.Color;
 import android.graphics.drawable.ColorDrawable;
 import android.os.Build;
 import android.os.Bundle;
 import android.view.KeyEvent;
 import android.view.View;
 import android.view.Window;
 import android.widget.Button;
 import android.widget.GridLayout;
 import android.widget.ScrollView;
 import android.widget.TextView;

 public class SeedChooser extends Activity {

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

//        setTitle(getString(R.string.seed_chooser_title));

         GridLayout gridLayout = new GridLayout(this);
         gridLayout.setColumnCount(5);
         gridLayout.setRowCount(14);

         Button[][] button = new Button[14][5];
         for (int row = 0; row < 14; row++) {
             for (int col = 0; col < 5; col++) {
                 int index = row * 5 + col; // 计算按钮序号
                 button[row][col] = new Button(this);
                 button[row][col].setText(seedName[index]);
                 if (row < 8) {
                     button[row][col].setBackgroundColor(getResources().getColor(android.R.color.holo_green_light));
                 } else {
                     button[row][col].setBackgroundColor(getResources().getColor(android.R.color.holo_purple));
                 }
                 button[row][col].setTextColor(getResources().getColor(android.R.color.white));

                 GridLayout.LayoutParams params = new GridLayout.LayoutParams();
                 params.width = 0;
                 params.height = GridLayout.LayoutParams.WRAP_CONTENT;
                 params.columnSpec = GridLayout.spec(col, 1f);
                 params.rowSpec = GridLayout.spec(row);
                 params.setMargins(4, 4, 4, 4);

                 gridLayout.addView(button[row][col], params);

                 button[row][col].setOnClickListener(v -> {
                     startActivity(new Intent(SeedChooser.this, CustomBalanceAdjustment.class));
                     globalIsSelected = true;
                     int seedIndex = gridLayout.indexOfChild(v);
                     currentSelectedSeed = seedName[seedIndex]; // 从全局数组获取数据
                     finish();
                 });
             }
         }

         ScrollView scrollView = new ScrollView(this);
         scrollView.addView(gridLayout);
         setContentView(scrollView);
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
