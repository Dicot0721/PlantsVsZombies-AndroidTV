package com.transmension.mobile;

import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.content.Context;
import android.graphics.Rect;
import android.os.SystemClock;
import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewTreeObserver;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputConnection;
import android.widget.EditText;

/* loaded from: classes.dex */
public class NativeView extends SurfaceView implements SurfaceHolder.Callback2, ViewTreeObserver.OnGlobalLayoutListener {
    static final String TAG = "NativeView";
    final int[] mLocation;
    private final boolean shiLiuBiJiu;
    private final int widthAs, heightAs;
    NativeActivity mActivity;
    boolean mDispatchingUnhandledKeyEvent;
    int mLastContentHeight;
    int mLastContentWidth;
    int mLastContentX;
    int mLastContentY;
    private SurfaceHolder mCurSurfaceHolder;
    private EditText mTextInput;
    private AlertDialog mTextInputDialog;
    private TextInputManager mTextInputManager;

    public NativeView(Context context) {
        super(context);
        this.mLocation = new int[2];
        this.mDispatchingUnhandledKeyEvent = false;
        getViewTreeObserver().addOnGlobalLayoutListener(this);
        setText("");
        getHolder().addCallback(this);
        setFocusable(true);
        setFocusableInTouchMode(true);
        shiLiuBiJiu = context.getSharedPreferences("data", 0).getBoolean("shiLiuBiJiu", true);
        widthAs = context.getSharedPreferences("data", 0).getInt("width", 16);
        heightAs = context.getSharedPreferences("data", 0).getInt("height", 9);
    }

    public NativeView(Context context, AttributeSet attrs) {
        super(context, attrs);
        this.mLocation = new int[2];
        this.mDispatchingUnhandledKeyEvent = false;
        getViewTreeObserver().addOnGlobalLayoutListener(this);
        setText("");
        getHolder().addCallback(this);
        setFocusable(true);
        setFocusableInTouchMode(true);
        shiLiuBiJiu = context.getSharedPreferences("data", 0).getBoolean("shiLiuBiJiu", true);
        widthAs = context.getSharedPreferences("data", 0).getInt("width", 16);
        heightAs = context.getSharedPreferences("data", 0).getInt("height", 9);
    }

    protected native void onContentRectChangedNative(long j, int i, int i2, int i3, int i4);

    protected native void onKeyboardFrameNative(long j, int i);

    protected native void onSurfaceChangedNative(long j, Surface surface, int i, int i2, int i3);

    protected native void onSurfaceCreatedNative(long j, Surface surface);

    protected native void onSurfaceDestroyedNative(long j);

    protected native void onSurfaceRedrawNeededNative(long j, Surface surface);

    protected native void onTextChangedNative(long j, String str, int i, int i2, long j2);

    protected native void onTextInputNative(long j, String str);

    protected native void onWindowFocusChangedNative(long j, boolean z);

    void setActivity(NativeActivity activity) {
        this.mActivity = activity;
        this.mTextInputManager = activity.createTextInputManager(this);
        this.mTextInputManager.setListener(new TextInputManager.Listener() { // from class: com.transmension.mobile.NativeView.1
            @Override // com.transmension.mobile.TextInputManager.Listener
            public void onTextChanged(View view, String text, int start, int end, long cookie) {
                if (isAlive()) {
                    onTextChangedNative(getNativeHandle(), text, start, end, cookie);
                }
            }

            @Override // com.transmension.mobile.TextInputManager.Listener
            public void onEditorAction(int actionCode) {
                NativeView.this.onEditorAction(actionCode);
            }

            @Override // com.transmension.mobile.TextInputManager.Listener
            public void onTextInput(View view, String text) {
                if (isAlive()) {
                    onTextInputNative(getNativeHandle(), text);
                }
            }
        });
    }

    boolean isAlive() {
        return this.mActivity != null && this.mActivity.isAlive();
    }

    @Override // android.view.SurfaceHolder.Callback
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        if (isAlive()) {
            onSurfaceChangedNative(getNativeHandle(), holder.getSurface(), format, width, height);
        }
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        if (shiLiuBiJiu) {
            int width = MeasureSpec.getSize(widthMeasureSpec);
            int height = MeasureSpec.getSize(heightMeasureSpec);
            float mAspectRatio = widthAs / (float) heightAs;
            float scale = (float) width / height;
            if (scale > mAspectRatio) {
                width = (int) (height * mAspectRatio);
            } else {
                height = (int) (width / mAspectRatio);
            }

            int newWidthMeasureSpec = MeasureSpec.makeMeasureSpec(
                    width, MeasureSpec.EXACTLY);
            int newHeightMeasureSpec = MeasureSpec.makeMeasureSpec(
                    height, MeasureSpec.EXACTLY);

            super.onMeasure(newWidthMeasureSpec, newHeightMeasureSpec);
        } else {
            super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        }
    }

    private int getSurface() {
        return 0;
    }

    @Override // android.view.SurfaceHolder.Callback
    public void surfaceCreated(SurfaceHolder holder) {
        Log.i(TAG, "surfaceCreated()");
        this.mCurSurfaceHolder = holder;
        if (isAlive()) {
            onSurfaceCreatedNative(getNativeHandle(), holder.getSurface());
        }
    }

    @Override // android.view.SurfaceHolder.Callback
    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.i(TAG, "surfaceDestroyed()");
        this.mCurSurfaceHolder = holder;
        if (isAlive()) {
            onSurfaceDestroyedNative(getNativeHandle());
        }
    }

    @Override // android.view.ViewTreeObserver.OnGlobalLayoutListener
    public void onGlobalLayout() {
        getLocationInWindow(this.mLocation);
        int w = getWidth();
        int h = getHeight();
        if (this.mLocation[0] != this.mLastContentX || this.mLocation[1] != this.mLastContentY || w != this.mLastContentWidth || h != this.mLastContentHeight) {
            this.mLastContentX = this.mLocation[0];
            this.mLastContentY = this.mLocation[1];
            this.mLastContentWidth = w;
            this.mLastContentHeight = h;
        }
        Log.i(TAG, String.format("Content rect: [%d %d, %d %d]", this.mLastContentX, this.mLastContentY, this.mLastContentWidth, this.mLastContentHeight));
        Rect r = new Rect();
        getWindowVisibleDisplayFrame(r);
        int screenHeight = getRootView().getHeight();
        int heightDifference = screenHeight - (r.bottom - r.top);
        Log.d(TAG, "Size: " + heightDifference);
        if (isAlive()) {
            onKeyboardFrameNative(getNativeHandle(), heightDifference);
        }
    }

    @Override // android.view.SurfaceHolder.Callback2
    public void surfaceRedrawNeeded(SurfaceHolder holder) {
        this.mCurSurfaceHolder = holder;
    }

    @Override // android.view.View
    @SuppressLint({"ClickableViewAccessibility"})
    public boolean onTouchEvent(MotionEvent event) {
        if (isAlive()) {
            this.mActivity.onNativeMotionEvent(event);
            return true;
        }
        return true;
    }

    @Override // android.view.View
    public boolean onGenericMotionEvent(MotionEvent event) {
        if (isAlive()) {
            this.mActivity.onNativeMotionEvent(event);
            return true;
        }
        return true;
    }


    //LB键和RB键，用于主界面翻页。长按左键或右键时触发一次。
//    private final KeyEvent lbEventDown = new KeyEvent(0, 0, KeyEvent.ACTION_DOWN, KeyEvent.KEYCODE_BUTTON_L1, 0, 0, 0, 0, 0, InputDevice.SOURCE_CLASS_BUTTON);
//    private final KeyEvent rbEventDown = new KeyEvent(0, 0, KeyEvent.ACTION_DOWN, KeyEvent.KEYCODE_BUTTON_R1, 0, 0, 0, 0, 0, InputDevice.SOURCE_CLASS_BUTTON);

    @Override // android.view.View, android.view.KeyEvent.Callback
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (this.mDispatchingUnhandledKeyEvent || (event.isSystem() && keyCode != 4 && keyCode != 82)) {
            return false;
        }
        mActivity.onNativeKeyEvent(event);
//        if (event.getRepeatCount() == 1) {
//            if (keyCode == KeyEvent.KEYCODE_DPAD_LEFT) {
//                mActivity.dispatchKeyEvent(lbEventDown);
//            } else if (keyCode == KeyEvent.KEYCODE_DPAD_RIGHT) {
//                mActivity.dispatchKeyEvent(rbEventDown);
//            }
//        }
        return true;
    }

    @Override // android.view.View, android.view.KeyEvent.Callback
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        if (this.mDispatchingUnhandledKeyEvent || (event.isSystem() && keyCode != 4 && keyCode != 82)) {
            return false;
        }

        mActivity.onNativeKeyEvent(event);

        return true;
    }

    @Override // android.view.View
    public void onWindowFocusChanged(boolean hasWindowFocus) {
        Log.i(TAG, "onWindowFocusChanged: " + hasWindowFocus);
        super.onWindowFocusChanged(hasWindowFocus);
        if (isAlive()) {
            onWindowFocusChangedNative(getNativeHandle(), hasWindowFocus);
        }
    }

    @Override // android.view.SurfaceView, android.view.View
    public void onFocusChanged(boolean gainFocus, int direction, Rect previouslyFocusedRect) {
        super.onFocusChanged(gainFocus, direction, previouslyFocusedRect);
        Log.i(TAG, "onFocusChanged: " + gainFocus);
    }

    public void dispatchUnhandledKeyEvent(KeyEvent event) {
        try {
            this.mDispatchingUnhandledKeyEvent = true;
            super.dispatchKeyEvent(event);
        } finally {
            this.mDispatchingUnhandledKeyEvent = false;
        }
    }

    public long getInputCookie() {
        if (this.mTextInputManager != null) {
            return this.mTextInputManager.getInputCookie();
        }
        return 0L;
    }

    public void setInputCookie(long cookie) {
        Log.i(TAG, String.format("setInputCookie: 0x%x", cookie));
        if (this.mTextInputManager != null) {
            this.mTextInputManager.setInputCookie(cookie);
        }
    }

    public int getInputType() {
        if (this.mTextInputManager != null) {
            return this.mTextInputManager.getInputType();
        }
        return 0;
    }

    public void setInputType(int type) {
        Log.i(TAG, String.format("setInputType: 0x%x", type));
        if (this.mTextInputManager != null) {
            this.mTextInputManager.setInputType(type);
        }
    }

    public int getImeOptions() {
        if (this.mTextInputManager != null) {
            return this.mTextInputManager.getImeOptions();
        }
        return 0;
    }

    public void setImeOptions(int options) {
        Log.i(TAG, String.format("setImeOptions: 0x%x", options));
        if (this.mTextInputManager != null) {
            this.mTextInputManager.setImeOptions(options);
        }
    }

    public void setText(String text, int selectionStart, int selectionEnd) {
        Log.i(TAG, "setText: " + text + " " + selectionStart + ":" + selectionEnd);
        if (this.mTextInputManager != null) {
            this.mTextInputManager.setText(text, selectionStart, selectionEnd);
        }
    }

    public CharSequence getText() {
        return this.mTextInputManager != null ? this.mTextInputManager.getText() : "";
    }

    public void setText(String text) {
        Log.i(TAG, "setText: " + text);
        if (this.mTextInputManager != null) {
            this.mTextInputManager.setText(text);
        }
    }

    public void setSelection(int start, int end) {
        Log.i(TAG, "setSelection: " + start + ":" + end);
        if (this.mTextInputManager != null) {
            this.mTextInputManager.setSelection(start, end);
        }
    }

    public int getSelectionStart() {
        if (this.mTextInputManager != null) {
            return this.mTextInputManager.getSelectionStart();
        }
        return 0;
    }

    public int getSelectionEnd() {
        if (this.mTextInputManager != null) {
            return this.mTextInputManager.getSelectionEnd();
        }
        return 0;
    }

    public void onEditorAction(int actionCode) {
        long eventTime = SystemClock.uptimeMillis();
        this.mActivity.dispatchKeyEvent(new KeyEvent(eventTime, eventTime, 0, 66, 0, 0, 0, 0, 22));
        this.mActivity.dispatchKeyEvent(new KeyEvent(eventTime, eventTime, 1, 66, 0, 0, 0, 0, 22));
    }

    @Override // android.view.View
    public InputConnection onCreateInputConnection(EditorInfo outAttrs) {
        Log.i(TAG, "onCreateInputConnection()");
        InputConnection ic = null;
        if (this.mTextInputManager != null) {
            ic = this.mTextInputManager.onCreateInputConnection(outAttrs);
        }
        if (ic == null) {
            InputConnection ic2 = super.onCreateInputConnection(outAttrs);
            return ic2;
        }
        return ic;
    }

    public void showIme(int mode) {
        Log.i(TAG, "showIme()");
        if (this.mTextInputManager != null) {
            this.mTextInputManager.showIme(mode);
        }
    }

    public void hideIme(int mode) {
        Log.i(TAG, "hideIme()");
        if (this.mTextInputManager != null) {
            this.mTextInputManager.hideIme(mode);
        }
    }

    public void showTextInputDialog(int mode, String title, String hint, String initial) {
        Log.i(TAG, "showTextInputDialog()");
        if (this.mTextInputManager != null) {
            this.mTextInputManager.showTextInputDialog(mode, title, hint, initial);
        }
    }

    public void hideTextInputDialog() {
        Log.i(TAG, "hideTextInputDialog()");
        if (this.mTextInputManager != null) {
            this.mTextInputManager.hideTextInputDialog();
        }
    }

    public void hideTextInputDialog(boolean cancel) {
        Log.i(TAG, "hideTextInputDialog()");
        if (this.mTextInputManager != null) {
            this.mTextInputManager.hideTextInputDialog(cancel);
        }
    }

    public long getNativeHandle() {
        return this.mActivity.mNativeHandle;
    }

    public float getDensity() {
        Display display = this.mActivity.getWindowManager().getDefaultDisplay();
        DisplayMetrics metrics = new DisplayMetrics();
        display.getMetrics(metrics);
        Log.i(TAG, "density :" + metrics.density);
        return metrics.density;
    }
}