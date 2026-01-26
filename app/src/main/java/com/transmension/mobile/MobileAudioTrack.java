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

import android.media.AudioTrack;
import android.util.Log;

class MobileAudioTrack extends AudioTrack {
    private final int mFrameSize;

    public MobileAudioTrack(int streamType, int sampleRateInHz, int channelConfig, int audioFormat, int bufferSizeInBytes, int mode) throws IllegalArgumentException {
        super(streamType, sampleRateInHz, channelConfig, audioFormat, bufferSizeInBytes, mode);
        this.mFrameSize = audioFormat == 2 ? getChannelCount() * 2 : getChannelCount();
    }

    @Override
    public void play() throws IllegalStateException {
        Log.d("TAG", "play: ");
        super.play();
        initBuffer();
    }

    public void initBuffer() {
        Log.d("TAG", "initBuffer: ");
        byte[] audioData = new byte[getNativeFrameCount() * this.mFrameSize];
        write(audioData, 0, audioData.length);
    }
}