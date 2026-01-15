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

#ifndef PVZ_SEXYAPPFRAMEWORK_SEXY_APP_BASE_H
#define PVZ_SEXYAPPFRAMEWORK_SEXY_APP_BASE_H

#include "Graphics/Color.h"
#include "Graphics/MemoryImage.h"
#include "Misc/Common.h"
#include "Misc/Rect.h"
#include "PvZ/MagicAddr.h"
#include "PvZ/STL/pvzstl_string.h"
#include "PvZ/SexyAppFramework/Widget/WidgetManager.h"
#include "PvZ/Symbols.h"
#include "Sound/AudiereSoundManager.h"
#include "Widget/ButtonListener.h"


void InitHookFunction();

namespace Sexy {

class Dialog;
class __Dialog;

class __SexyAppBase {
public:
    int *vTable;                            // 0
    int unkMem1_1[30];                      // 1 ~ 30
    bool unkBool_1[4];                      // 31
    int unkMem1_2[133];                     // 32 ~ 164
    WidgetManager *mWidgetManager;          // 165
    int unkMem2[70];                        // 166 ~ 235, musicinterface 193
    bool mLawnMouseMode;                    // 944
    int unkMem3[77];                        // 237 ~ 313
    AudiereSoundManager *mSoundManager;     // 314
    int unkMem4[39];                        // 315 ~ 353
    int mNumLoadingThreadTasks;             // 354
    int mCompletedLoadingThreadTasks;       // 355
    int unkMem5[30];                        // 356 ~ 385
    bool mIs3DAcclerated;                   // 1544
    int unkMem6[SexyAppBasePartSize - 387]; // 387 ~ 525
    bool unkBool1;                          // 2104
    bool mGamePad1IsOn;                     // 2105
    int unkMem7[22];                        // 527 ~ 548
    bool unkBool2;                          // 2196
    bool mGamePad2IsOn;                     // 2197
    int unkMem8[2];                         // 550 ~ 551
    // 115： 552 , 111： 553

    Dialog *GetDialog(Dialogs theDialogId) { // vTable + 4 * 103
        return reinterpret_cast<Dialog *(*)(__SexyAppBase *, Dialogs)>(Sexy_SexyAppBase_GetDialogAddr)(this, theDialogId);
    }
    void EraseFile(const pvzstl::string &theFileName) {
        reinterpret_cast<void (*)(__SexyAppBase *, const pvzstl::string &)>(Sexy_SexyAppBase_EraseFileAddr)(this, theFileName);
    }
    Image *GetImage(const pvzstl::string &theFileName, bool commitBits = true) {
        return reinterpret_cast<Image *(*)(__SexyAppBase *, const pvzstl::string &, bool)>(Sexy_SexyAppBase_GetImageAddr)(this, theFileName, commitBits);
    }
    bool RegistryReadString(const pvzstl::string &theValueName, pvzstl::string *theString) {
        return reinterpret_cast<bool (*)(__SexyAppBase *, const pvzstl::string &, pvzstl::string *)>(Sexy_SexyAppBase_RegistryReadStringAddr)(this, theValueName, theString);
    }
    Image *CopyImage(Image *theImage) {
        return reinterpret_cast<Image *(*)(__SexyAppBase *, Image *)>(Sexy_SexyAppBase_CopyImageAddr)(this, theImage);
    }
    Image *CopyImage(Image *theImage, const Rect &theRect) {
        return reinterpret_cast<Image *(*)(__SexyAppBase *, Image *, const Rect &)>(Sexy_SexyAppBase_CopyImage2Addr)(this, theImage, theRect);
    }
    void AddDialog(__Dialog *theDialog) {
        reinterpret_cast<void (*)(__SexyAppBase *, __Dialog *)>(Sexy_SexyAppBase_AddDialogAddr)(this, theDialog);
    }
    void DoParseCmdLine() { // vTable + 4 * 62
        reinterpret_cast<void (*)(__SexyAppBase *)>(Sexy_SexyAppBase_DoParseCmdLineAddr)(this);
    }
    int GetInteger(const pvzstl::string &theName, int defValue) {
        return reinterpret_cast<bool (*)(__SexyAppBase *, const pvzstl::string &, int)>(Sexy_SexyAppBase_GetIntegerAddr)(this, theName, defValue);
    }
    void LoadResourceManifest() {
        reinterpret_cast<void (*)(__SexyAppBase *)>(Sexy_SexyAppBase_LoadResourceManifestAddr)(this);
    }


protected:
    __SexyAppBase() = default;
    ~__SexyAppBase() = default;

    friend void ::InitHookFunction();

    void _constructor();
};

class SexyAppBase : public __SexyAppBase {
public:
    SexyAppBase() {
        _constructor();
    }

    ~SexyAppBase() = delete;
    bool UpdateApp();
};

} // namespace Sexy

inline void (*old_Sexy_SexyAppBase_SexyAppBase)(Sexy::__SexyAppBase *appBase);

inline bool (*old_Sexy_SexyAppBase_UpdateApp)(Sexy::SexyAppBase *appBase);

#endif // PVZ_SEXYAPPFRAMEWORK_SEXY_APP_BASE_H
