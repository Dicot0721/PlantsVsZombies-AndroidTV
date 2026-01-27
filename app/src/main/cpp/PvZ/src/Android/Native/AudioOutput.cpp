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

#include "PvZ/Android/Native/AudioOutput.h"

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <jni.h>

#include <condition_variable>
#include <mutex>

namespace {

// 同步机制变量
std::mutex mtx;
std::condition_variable cv;
bool bufferConsumed;

// OpenSL ES engine interfaces
SLObjectItf engineObject;
SLEngineItf engineEngine;

// Output mix interfaces
SLObjectItf outputMixObject;

// Player interfaces
SLObjectItf playerObject;
SLPlayItf playerPlay;
SLAndroidSimpleBufferQueueItf playerBufferQueue;

// Buffer
[[maybe_unused]] unsigned char buffer[8192];

// Callback to handle buffer queue events
void playerCallback(SLAndroidSimpleBufferQueueItf bq, void *context) {
    {
        std::lock_guard lock{mtx};
        bufferConsumed = true;
    }
    cv.notify_all(); // 通知主线程
}

void waitForBufferConsumption() {
    std::unique_lock lock{mtx};
    cv.wait(lock, [] { return bufferConsumed; }); // 阻塞等待
    bufferConsumed = false;
}

namespace opensl {
    void setup(int sampleRate, int channels, int bits) {
        slCreateEngine(&engineObject, 0, nullptr, 0, nullptr, nullptr);
        (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
        (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
        (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 0, nullptr, nullptr);
        (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);

        SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 1};
        SLDataFormat_PCM format_pcm = {
            SL_DATAFORMAT_PCM, 2, SL_SAMPLINGRATE_44_1, SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16, SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT, SL_BYTEORDER_LITTLEENDIAN};
        SLDataSource audioSrc = {&loc_bufq, &format_pcm};

        // Configure audio sink
        SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
        SLDataSink audioSnk = {&loc_outmix, nullptr};

        // Create audio player
        const SLInterfaceID ids[1] = {SL_IID_BUFFERQUEUE};
        const SLboolean req[1] = {SL_BOOLEAN_TRUE};
        (*engineEngine)->CreateAudioPlayer(engineEngine, &playerObject, &audioSrc, &audioSnk, 1, ids, req);
        (*playerObject)->Realize(playerObject, SL_BOOLEAN_FALSE);
        (*playerObject)->GetInterface(playerObject, SL_IID_PLAY, &playerPlay);
        (*playerObject)->GetInterface(playerObject, SL_IID_BUFFERQUEUE, &playerBufferQueue);
        (*playerBufferQueue)->RegisterCallback(playerBufferQueue, playerCallback, nullptr);
    }

    // Shutdown and cleanup
    void shutdown() {
        if (playerObject != nullptr) {
            (*playerObject)->Destroy(playerObject);
            playerObject = nullptr;
            playerPlay = nullptr;
            playerBufferQueue = nullptr;
        }

        // Destroy the output mix object
        if (outputMixObject != nullptr) {
            (*outputMixObject)->Destroy(outputMixObject);
            outputMixObject = nullptr;
        }

        // Destroy the engine object
        if (engineObject != nullptr) {
            (*engineObject)->Destroy(engineObject);
            engineObject = nullptr;
            engineEngine = nullptr;
        }
    }

    void play() {
        if (playerPlay != nullptr) {
            (*playerPlay)->SetPlayState(playerPlay, SL_PLAYSTATE_PLAYING);
        }
    }

    // Stop playback
    void stop() {
        if (playerPlay != nullptr) {
            (*playerPlay)->SetPlayState(playerPlay, SL_PLAYSTATE_STOPPED);
        }
    }

} // namespace opensl

} // namespace


// 写入音频数据到音频流
void AudioWrite(const void *data, int dataSize) {
    (*playerBufferQueue)->Enqueue(playerBufferQueue, data, dataSize);
    waitForBufferConsumption();
}

void Native::AudioOutput::initialize() {
    old_Native_AudioOutput_initialize(this);
}

bool Native::AudioOutput::setup(int sampleRate, int channels, int bits) {
    bool result = old_Native_AudioOutput_setup(this, sampleRate, channels, bits);
    opensl::setup(sampleRate, channels, bits);
    opensl::play();
    // int *mAudioOutput = *(int **)(*(uint32_t *)mNativeApp + 188);
    *(uint32_t *)(*(uint32_t *)mNativeApp + 188) = 0;

    return result;
}

void Native::AudioOutput::shutdown() {
    opensl::stop();
    opensl::shutdown();
    old_Native_AudioOutput_shutdown(this);
}

int Native::AudioOutput::write(const void *data, int dataSize) {
    // return old_Native_AudioOutput_write(audioOutput,thePlayerIndex,a3);
    AudioWrite(data, dataSize);
    return dataSize;
}
