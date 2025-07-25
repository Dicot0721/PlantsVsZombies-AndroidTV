package com.transmension.mobile;

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
        mLocation = new int[2];
        mDispatchingUnhandledKeyEvent = false;
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
        mLocation = new int[2];
        mDispatchingUnhandledKeyEvent = false;
        getViewTreeObserver().addOnGlobalLayoutListener(this);
        setText("");
        getHolder().addCallback(this);
        setFocusable(true);
        setFocusableInTouchMode(true);
        shiLiuBiJiu = context.getSharedPreferences("data", 0).getBoolean("shiLiuBiJiu", true);
        widthAs = context.getSharedPreferences("data", 0).getInt("width", 16);
        heightAs = context.getSharedPreferences("data", 0).getInt("height", 9);
    }

    protected native static void onSurfaceChangedNative(long j, Surface surface, int i, int i2, int i3);

    protected native void onContentRectChangedNative(long j, int i, int i2, int i3, int i4);

    protected native void onKeyboardFrameNative(long j, int i);

    protected native void onSurfaceCreatedNative(long j, Surface surface);

    protected native void onSurfaceDestroyedNative(long j);

    protected native void onSurfaceRedrawNeededNative(long j, Surface surface);

    protected native void onTextChangedNative(long j, String str, int i, int i2, long j2);

    protected native void onTextInputNative(long j, String str);

    protected native void onWindowFocusChangedNative(long j, boolean z);

    /* JADX INFO: Access modifiers changed from: package-private */
    public void setActivity(NativeActivity activity) {
        mActivity = activity;
        mTextInputManager = activity.createTextInputManager(this);
        mTextInputManager.setListener(new TextInputManager.Listener() {
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

    boolean isAlive() {
        return mActivity != null && mActivity.isAlive();
    }

//    private static final int VIDEO_WIDTH = 1280;
//    private static final int VIDEO_HEIGHT = 720;
//    private static final int SCREEN_FRAME_RATE = 60;
//    private static final int SCREEN_FRAME_INTERVAL = 1;
//    MediaRecorder mMediaRecorder;
//
//    private void initRecord() {
//        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
//            try {
//                if (mMediaRecorder == null) {
//                    mMediaRecorder = new MediaRecorder();
//                }
//
//                //mMediaRecorder.setAudioSource(MediaRecorder.AudioSource.DEFAULT);//音频源  麦克风
//                // 设置录制视频源为Camera(相机)
//
//                mMediaRecorder.setVideoSource(MediaRecorder.VideoSource.SURFACE);//视频源
//                mMediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.MPEG_4);//视频输出格式
//                mMediaRecorder.setVideoEncoder(MediaRecorder.VideoEncoder.H264);//视频录制格式
//
//                mMediaRecorder.setVideoSize(1280, 720);
//                // 设置录制的视频帧率。必须放在设置编码和格式的后面，否则报错，这样设置变清晰
//                mMediaRecorder.setVideoEncodingBitRate(10 * 1024 * 1024);
//
//                mMediaRecorder.setOutputFile(new File(mActivity.getExternalDataPath(), "new.mp4").getAbsolutePath());
//
//                mMediaRecorder.prepare();
//                mMediaRecorder.start();
//                onSurfaceCreatedNative(getNativeHandle(), mMediaRecorder.getSurface());
//            } catch (Exception e) {
//                e.printStackTrace();
//            }
//            mMediaRecorder.stop();
//        }
//    }

    @Override // android.view.SurfaceHolder.Callback
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        if (isAlive()) {
            onSurfaceChangedNative(getNativeHandle(), holder.getSurface(), format, width, height);
        }
    }


    @Override // android.view.SurfaceHolder.Callback
    public void surfaceCreated(SurfaceHolder holder) {
        Log.i(TAG, "surfaceCreated()");
        mCurSurfaceHolder = holder;
        if (isAlive()) {
            onSurfaceCreatedNative(getNativeHandle(), holder.getSurface());
        }
    }

    @Override // android.view.SurfaceHolder.Callback
    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.i(TAG, "surfaceDestroyed()");
        mCurSurfaceHolder = holder;
        if (isAlive()) {
            onSurfaceDestroyedNative(getNativeHandle());
        }
    }

    @Override // android.view.ViewTreeObserver.OnGlobalLayoutListener
    public void onGlobalLayout() {
        getLocationInWindow(mLocation);
        int w = getWidth();
        int h = getHeight();
        if (mLocation[0] != mLastContentX || mLocation[1] != mLastContentY || w != mLastContentWidth || h != mLastContentHeight) {
            mLastContentX = mLocation[0];
            mLastContentY = mLocation[1];
            mLastContentWidth = w;
            mLastContentHeight = h;
        }
        Log.i(TAG, String.format("Content rect: [%d %d, %d %d]", mLastContentX, mLastContentY, mLastContentWidth, mLastContentHeight));
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
        mCurSurfaceHolder = holder;
    }

    @Override // android.view.View
    public boolean onTouchEvent(MotionEvent event) {
        if (isAlive()) {
            mActivity.onNativeMotionEvent(event);
            return true;
        }
        return true;
    }

    @Override // android.view.View
    public boolean onGenericMotionEvent(MotionEvent event) {
        if (isAlive()) {
            mActivity.onNativeMotionEvent(event);
            return true;
        }
        return true;
    }

    @Override // android.view.View, android.view.KeyEvent.Callback
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (mDispatchingUnhandledKeyEvent || (event.isSystem() && keyCode != 4 && keyCode != KeyEvent.KEYCODE_MENU)) {
            return false;
        }
        mActivity.onNativeKeyEvent(event);
        return true;
    }

    @Override // android.view.View, android.view.KeyEvent.Callback
    public boolean onKeyUp(int keyCode, KeyEvent event) {

        if (mDispatchingUnhandledKeyEvent || (event.isSystem() && keyCode != 4 && keyCode != KeyEvent.KEYCODE_MENU)) {
            return false;
        }
        if (isAlive()) {
            mActivity.onNativeKeyEvent(event);
        }
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
            mDispatchingUnhandledKeyEvent = true;
            super.dispatchKeyEvent(event);
        } finally {
            mDispatchingUnhandledKeyEvent = false;
        }
    }

    public long getInputCookie() {
        if (mTextInputManager != null) {
            return mTextInputManager.getInputCookie();
        }
        return 0L;
    }

    public void setInputCookie(long cookie) {
        Log.i(TAG, String.format("setInputCookie: 0x%x", cookie));
        if (mTextInputManager != null) {
            mTextInputManager.setInputCookie(cookie);
        }
    }

    public int getInputType() {
        if (mTextInputManager != null) {
            return mTextInputManager.getInputType();
        }
        return 0;
    }

    public void setInputType(int type) {
        Log.i(TAG, String.format("setInputType: 0x%x", type));
        if (mTextInputManager != null) {
            mTextInputManager.setInputType(type);
        }
    }

    public int getImeOptions() {
        if (mTextInputManager != null) {
            return mTextInputManager.getImeOptions();
        }
        return 0;
    }

    public void setImeOptions(int options) {
        Log.i(TAG, String.format("setImeOptions: 0x%x", options));
        if (mTextInputManager != null) {
            mTextInputManager.setImeOptions(options);
        }
    }

    public void setText(String text, int selectionStart, int selectionEnd) {
        Log.i(TAG, "setText: " + text + " " + selectionStart + ":" + selectionEnd);
        if (mTextInputManager != null) {
            mTextInputManager.setText(text, selectionStart, selectionEnd);
        }
    }

    public CharSequence getText() {
        return mTextInputManager != null ? mTextInputManager.getText() : "";
    }

    public void setText(String text) {
        Log.i(TAG, "setText: " + text);
        if (mTextInputManager != null) {
            mTextInputManager.setText(text);
        }
    }

    public void setSelection(int start, int end) {
        Log.i(TAG, "setSelection: " + start + ":" + end);
        if (mTextInputManager != null) {
            mTextInputManager.setSelection(start, end);
        }
    }

    public int getSelectionStart() {
        if (mTextInputManager != null) {
            return mTextInputManager.getSelectionStart();
        }
        return 0;
    }

    public int getSelectionEnd() {
        if (mTextInputManager != null) {
            return mTextInputManager.getSelectionEnd();
        }
        return 0;
    }

    public void onEditorAction(int actionCode) {
        long eventTime = SystemClock.uptimeMillis();
        mActivity.dispatchKeyEvent(new KeyEvent(eventTime, eventTime, 0, 66, 0, 0, 0, 0, 22));
        mActivity.dispatchKeyEvent(new KeyEvent(eventTime, eventTime, 1, 66, 0, 0, 0, 0, 22));
    }

    @Override // android.view.View
    public InputConnection onCreateInputConnection(EditorInfo outAttrs) {
        Log.i(TAG, "onCreateInputConnection()");
        InputConnection ic = null;
        if (mTextInputManager != null) {
            ic = mTextInputManager.onCreateInputConnection(outAttrs);
        }
        if (ic == null) {
            return super.onCreateInputConnection(outAttrs);
        }
        return ic;
    }

    public void showIme(int mode) {
        Log.i(TAG, "showIme()");
        if (mTextInputManager != null) {
            mTextInputManager.showIme(mode);
        }
    }

    public void hideIme(int mode) {
        Log.i(TAG, "hideIme()");
        if (mTextInputManager != null) {
            mTextInputManager.hideIme(mode);
        }
    }

    public void showTextInputDialog(int mode, String title, String hint, String initial) {
        Log.i(TAG, "showTextInputDialog()");
        if (mTextInputManager != null) {
            mTextInputManager.showTextInputDialog(mode, title, hint, initial);
        }
    }

    public void hideTextInputDialog() {
        Log.i(TAG, "hideTextInputDialog()");
        if (mTextInputManager != null) {
            mTextInputManager.hideTextInputDialog();
        }
    }

    public void hideTextInputDialog(boolean cancel) {
        Log.i(TAG, "hideTextInputDialog()");
        if (mTextInputManager != null) {
            mTextInputManager.hideTextInputDialog(cancel);
        }
    }

    public long getNativeHandle() {
        return mActivity.mNativeHandle;
    }

    public float getDensity() {
        Display display = mActivity.getWindowManager().getDefaultDisplay();
        DisplayMetrics metrics = new DisplayMetrics();
        display.getMetrics(metrics);
        Log.i(TAG, "density :" + metrics.density);
        return metrics.density;
    }
}