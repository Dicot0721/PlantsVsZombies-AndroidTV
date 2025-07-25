package com.transmension.mobile;

import android.content.Context;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.os.Build;
import android.util.Log;

import java.nio.ByteBuffer;

/* loaded from: classes.dex */
public class AudioOutput {
    private final Context mContext;
    byte[] mAudioData = new byte[8192];
    String TAG = "TAG";

//    private native void nativePause();
//    private native void nativeResume();
//    private native void nativeWrite(byte[] data, int length);
    private MobileAudioTrack mAudioTrack = null;

    public AudioOutput(Context context) {
        mContext = context;
    }

    public int getPreferredSampleRate() {
        if (Build.VERSION.SDK_INT < 17) {
            return -1;
        }
        AudioManager audioManager = (AudioManager) mContext.getSystemService(Context.AUDIO_SERVICE);
        String v = audioManager.getProperty("android.media.property.OUTPUT_SAMPLE_RATE");
        if (v != null) {
            return Integer.parseInt(v);
        }
        return -1;
    }

    public int getPreferredFramesPerBuffer() {
        if (Build.VERSION.SDK_INT < 17) {
            return -1;
        }
        AudioManager audioManager = (AudioManager) mContext.getSystemService(Context.AUDIO_SERVICE);
        String v = audioManager.getProperty("android.media.property.OUTPUT_FRAMES_PER_BUFFER");
        if (v != null) {
            return Integer.parseInt(v);
        }
        return -1;
    }

    public boolean setup(int sampleRate, int channels, int bits) {
        if (mContext.getSharedPreferences("data", 0).getBoolean("useOpenSL", true)) {
            return true;
        }
        if (mAudioTrack == null && sampleRate == 44100 && channels == 2 && bits == 16) {
            int bufferSize = AudioTrack.getMinBufferSize(sampleRate, 3, 2);
            mAudioTrack = new MobileAudioTrack(3, sampleRate, 3, 2, bufferSize, 1);
            if (mAudioTrack.getState() != 1) {
                mAudioTrack.release();
                mAudioTrack = null;
                return false;
            }
//            mAudioTrack.setPlaybackPositionUpdateListener(new AudioTrack.OnPlaybackPositionUpdateListener() { // from class: com.transmension.mobile.AudioOutput.1
//                @Override // android.media.AudioTrack.OnPlaybackPositionUpdateListener
//                public void onMarkerReached(AudioTrack track) {
//                }
//
//                @Override // android.media.AudioTrack.OnPlaybackPositionUpdateListener
//                public void onPeriodicNotification(AudioTrack track) {
//                }
//            });
            mAudioTrack.play();
            return true;
        }
        return false;
    }

    public synchronized void shutdown() {
        Log.d(TAG, "shutdown: ");
        if (mAudioTrack != null) {
            mAudioTrack.stop();
            mAudioTrack.release();
            mAudioTrack = null;
        }
    }

    public synchronized void write(ByteBuffer data, int offset, int len) {
        Log.d(TAG, "write: ");
//    public void write(ByteBuffer data, int offset, int len) {
        if (mAudioTrack != null) {
            data.position(offset);
//            mAudioTrack.write(data, len, AudioTrack.WRITE_BLOCKING);
            data.get(mAudioData, 0, len);
            mAudioTrack.write(mAudioData, 0, len);
        }
    }

    public synchronized void onPause() {
        Log.d(TAG, "onPause: ");
        if (mAudioTrack != null && mAudioTrack.getPlayState() == 3) {
            mAudioTrack.pause();
        }
    }

    public synchronized void onResume() {
        Log.d(TAG, "onResume: ");
        if (mAudioTrack != null && mAudioTrack.getPlayState() != 3) {
            mAudioTrack.play();
        }
    }
}