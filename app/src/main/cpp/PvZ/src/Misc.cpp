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

#include "PvZ/Misc.h"
#include "PvZ/Android/Native/BridgeApp.h"
#include "PvZ/Android/Native/NativeApp.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/LawnApp.h"

#include <cassert>
#include <random>

int RandomInt(int a, int b) {
    assert(a <= b);
    static std::random_device rd;
    static std::mt19937 gen{rd()};
    std::uniform_int_distribution distrib{a, b};
    return distrib(gen);
}

void TriggerVibration(VibrationEffect theVibrationEffect) {
    LawnApp *app = *gLawnApp_Addr;
    if (app->mPlayerInfo->mIsVibrateClosed) {
        return;
    }

    Native::BridgeApp *bridgeApp = Native::BridgeApp::getSingleton();
    JNIEnv *env = bridgeApp->getJNIEnv();
    jobject activity = bridgeApp->mNativeApp->getActivity();
    jclass clazz = env->GetObjectClass(activity);
    //    jmethodID methodID = env->GetMethodID(clazz, "startVibration", "(Lcom/transmension/mobile/EnhanceActivity$HapiticEffect;)V");
    jmethodID methodID = env->GetMethodID(clazz, "startVibration", "(I)V");
    env->CallVoidMethod(activity, methodID, theVibrationEffect);
    env->DeleteLocalRef(clazz);
}