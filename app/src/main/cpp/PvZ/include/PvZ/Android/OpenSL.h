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

#ifndef PVZ_ANDROID_OPENSL_H
#define PVZ_ANDROID_OPENSL_H


#include "PvZ/Android/Native/AudioOutput.h"

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <jni.h>


inline void (*old_Native_AudioOutput_initialize)(Native::AudioOutput *audioOutput);

inline bool (*old_Native_AudioOutput_setup)(Native::AudioOutput *audioOutput, int sampleRate, int channels, int bits);

inline void (*old_Native_AudioOutput_shutdown)(Native::AudioOutput *audioOutput);

inline int (*old_Native_AudioOutput_write)(Native::AudioOutput *audioOutput, const void *a2, int a3);


void AudioWrite(const void *data, int dataSize);

void Native_AudioOutput_initialize(Native::AudioOutput *audioOutput);

bool Native_AudioOutput_setup(Native::AudioOutput *audioOutput, int sampleRate, int channels, int bits);

void Native_AudioOutput_shutdown(Native::AudioOutput *audioOutput);

int Native_AudioOutput_write(Native::AudioOutput *audioOutput, const void *a2, int a3);

#endif // PVZ_ANDROID_OPENSL_H
