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

#include "PvZ/Android/OpenSL.h"
#include "PvZ/Lawn/Common/ConstEnums.h"

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <jni.h>
#include <pthread.h>

using namespace Native;

namespace {

// 同步机制变量
pthread_mutex_t mutex;
pthread_cond_t cond;
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

} // namespace


// Callback to handle buffer queue events
static void playerCallback(SLAndroidSimpleBufferQueueItf bq, void *context) {
    pthread_mutex_lock(&mutex);
    bufferConsumed = true;
    pthread_cond_signal(&cond); // 通知主线程
    pthread_mutex_unlock(&mutex);
}

static void waitForBufferConsumption() {
    pthread_mutex_lock(&mutex);
    while (!bufferConsumed) {
        pthread_cond_wait(&cond, &mutex); // 阻塞等待
    }
    bufferConsumed = false;
    pthread_mutex_unlock(&mutex);
}

static void setup(int sampleRate, int channels, int bits) {
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&cond, nullptr);
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

static void play() {
    if (playerPlay != nullptr) {
        (*playerPlay)->SetPlayState(playerPlay, SL_PLAYSTATE_PLAYING);
    }
}

// Stop playback
static void stop() {
    if (playerPlay != nullptr) {
        (*playerPlay)->SetPlayState(playerPlay, SL_PLAYSTATE_STOPPED);
    }
}

// Shutdown and cleanup
static void shutdown() {
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
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}

// 写入音频数据到音频流
void AudioWrite(const void *data, int dataSize) {
    (*playerBufferQueue)->Enqueue(playerBufferQueue, data, dataSize);
    waitForBufferConsumption();
}

void Native_AudioOutput_initialize(Native::AudioOutput *audioOutput) {
    old_Native_AudioOutput_initialize(audioOutput);
}

bool Native_AudioOutput_setup(Native::AudioOutput *audioOutput, int sampleRate, int channels, int bits) {
    bool result = old_Native_AudioOutput_setup(audioOutput, sampleRate, channels, bits);
    setup(sampleRate, channels, bits);
    play();
    Native::NativeApp *mNativeApp = audioOutput->mNativeApp;
    // int *mAudioOutput = *(int **)(*(uint32_t *)mNativeApp + 188);
    *(uint32_t *)(*(uint32_t *)mNativeApp + 188) = 0;

    return result;
}

void Native_AudioOutput_shutdown(Native::AudioOutput *audioOutput) {
    old_Native_AudioOutput_shutdown(audioOutput);
    stop();
    shutdown();
}

int Native_AudioOutput_write(Native::AudioOutput *audioOutput, const void *a2, int a3) {
    // return old_Native_AudioOutput_write(audioOutput,thePlayerIndex,a3);
    AudioWrite(a2, a3);
    return a3;
}
