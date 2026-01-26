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

import android.app.ActionBar;
import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.res.Configuration;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Build;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;
import android.widget.Toast;

import com.trans.pvztv.R;

public class KeyboardSetActivity extends Activity {

    private final String[] preferenceNames1P = new String[]{"keyCodeTwoPlayer", "P1A", "P1B", "P1X", "P1L1", "P1R1", "P1UP", "P1DOWN", "P1LEFT", "P1RIGHT", "P2A", "P2B", "P2X", "P2L1", "P2R1", "P2UP", "P2DOWN", "P2LEFT", "P2RIGHT", "keyCodePause"};

    private String[] preferenceIntros1P = null;
    private MyTextView keyToSet = null;

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

        setTitle(getString(R.string.secondplayer_title));
        SharedPreferences sharedPreferences = getSharedPreferences("data", 0);

        preferenceIntros1P = new String[]{getString(R.string.secondplayer_preferencesIntro0), getString(R.string.secondplayer_preferencesIntro1), getString(R.string.secondplayer_preferencesIntro2), getString(R.string.secondplayer_preferencesIntro3), getString(R.string.secondplayer_preferencesIntro4), getString(R.string.secondplayer_preferencesIntro5), getString(R.string.secondplayer_preferencesIntro6), getString(R.string.secondplayer_preferencesIntro7), getString(R.string.secondplayer_preferencesIntro8), getString(R.string.secondplayer_preferencesIntro9), getString(R.string.secondplayer_preferencesIntro10), getString(R.string.secondplayer_preferencesIntro11), getString(R.string.secondplayer_preferencesIntro12), getString(R.string.secondplayer_preferencesIntro13), getString(R.string.secondplayer_preferencesIntro14), getString(R.string.secondplayer_preferencesIntro15), getString(R.string.secondplayer_preferencesIntro16), getString(R.string.secondplayer_preferencesIntro17), getString(R.string.secondplayer_preferencesIntro18), getString(R.string.secondplayer_preferencesIntro19)};

        ScrollView scrollView = new ScrollView(this);
        LinearLayout linearLayout = new LinearLayout(this);
        TextView intro = new TextView(this);
        //intro.setGravity(Gravity.CENTER);
        intro.setText(R.string.secondplayer_info);
        intro.setTextSize(20f);

        TableLayout tableLayout = new TableLayout(this);
        tableLayout.setLayoutParams(new TableLayout.LayoutParams(TableLayout.LayoutParams.MATCH_PARENT, TableLayout.LayoutParams.WRAP_CONTENT));
        tableLayout.setStretchAllColumns(false);
        int length = preferenceNames1P.length;
        for (int i = 0; i < length; i++) {
            TableRow tableRow = new TableRow(this);
            tableRow.setLayoutParams(new TableRow.LayoutParams(TableRow.LayoutParams.MATCH_PARENT, TableRow.LayoutParams.WRAP_CONTENT));
            MyTextView textView = new MyTextView(this, i);
            Button button = new Button(this);
            int keyCode = sharedPreferences.getInt(preferenceNames1P[i], KeyEvent.KEYCODE_UNKNOWN);
            textView.setLayoutParams(new TableRow.LayoutParams(TableRow.LayoutParams.WRAP_CONTENT, TableRow.LayoutParams.WRAP_CONTENT));
            textView.setText(String.format("%s: %s", preferenceIntros1P[i], keyCode == KeyEvent.KEYCODE_UNKNOWN ? getString(R.string.secondplayer_default) : KeyEvent.keyCodeToString(keyCode).replace("KEYCODE_", "")));
            textView.setTextColor(isNight ? Color.WHITE : Color.BLACK);
            button.setText(R.string.secondplayer_edit);
            button.setTextSize(16f);
            button.setFocusable(false);
            button.setPadding(0, 15, 0, 15);
            button.setOnClickListener(view -> {
                if (keyToSet != null)
                    keyToSet.setTextColor(isNight ? Color.WHITE : Color.BLACK);
                keyToSet = textView;
                keyToSet.setTextColor(Color.GREEN);
                Toast.makeText(this, R.string.secondplayer_edit_toast, Toast.LENGTH_SHORT).show();
            });

            tableRow.addView(textView);
            tableRow.addView(button);
            tableLayout.addView(tableRow);
        }
        linearLayout.setOrientation(LinearLayout.VERTICAL);
        linearLayout.addView(intro);
        linearLayout.addView(tableLayout);
        scrollView.addView(linearLayout);
        setContentView(scrollView);
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode != 4 && keyCode != 82 && event.getRepeatCount() == 0) {
            if (keyToSet != null) {
                SharedPreferences sharedPreferences = getSharedPreferences("data", 0);
                keyToSet.setText(String.format("%s: %s", preferenceIntros1P[keyToSet.preferenceId], keyCode == KeyEvent.KEYCODE_UNKNOWN ? (getString(R.string.secondplayer_default) + "(" + KeyEvent.keyCodeToString(keyCode).replace("KEYCODE_", "") + ")") : KeyEvent.keyCodeToString(keyCode).replace("KEYCODE_", "")));
                sharedPreferences.edit().putInt(preferenceNames1P[keyToSet.preferenceId], keyCode).apply();
            }

        }
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
