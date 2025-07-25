package com.transmension.mobile;

import android.media.AudioTrack;

/* compiled from: AudioOutput.java */
/* loaded from: classes.dex */
class MobileAudioTrack extends AudioTrack {
    private final int mFrameSize;

    public MobileAudioTrack(int streamType, int sampleRateInHz, int channelConfig, int audioFormat, int bufferSizeInBytes, int mode) throws IllegalArgumentException {
        super(streamType, sampleRateInHz, channelConfig, audioFormat, bufferSizeInBytes, mode);
        if (audioFormat == 2) {
            this.mFrameSize = getChannelCount() * 2;
        } else {
            this.mFrameSize = getChannelCount();
        }
    }

    @Override // android.media.AudioTrack
    public void play() throws IllegalStateException {
        super.play();
        initBuffer();
    }

    public void initBuffer() {
        byte[] audioData = new byte[getNativeFrameCount() * this.mFrameSize];
        write(audioData, 0, audioData.length);
    }
}