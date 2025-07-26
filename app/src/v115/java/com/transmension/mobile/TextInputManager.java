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

import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputConnection;

/* loaded from: classes.dex */
public interface TextInputManager {
    int KEYBOARD_EMAIL = 3;
    int KEYBOARD_NORMAL = 0;
    int KEYBOARD_PASSWORD = 1;
    int KEYBOARD_URL = 2;
    int KEYBOARD_USERNAME = 4;

    int getImeOptions();

    void setImeOptions(int i);

    long getInputCookie();

    void setInputCookie(long j);

    int getInputType();

    void setInputType(int i);

    Listener getListener();

    void setListener(Listener listener);

    int getSelectionEnd();

    int getSelectionStart();

    CharSequence getText();

    void setText(String str);

    void hideIme(int i);

    void hideTextInputDialog();

    void hideTextInputDialog(boolean z);

    InputConnection onCreateInputConnection(EditorInfo editorInfo);

    void setSelection(int i, int i2);

    void setText(String str, int i, int i2);

    void showIme(int i);

    void showTextInputDialog(int i, String str, String str2, String str3);

    /* loaded from: classes.dex */
    abstract class Listener {
        public abstract void onEditorAction(int i);

        public abstract void onTextChanged(View view, String str, int i, int i2, long j);

        public abstract void onTextInput(View view, String str);
    }
}