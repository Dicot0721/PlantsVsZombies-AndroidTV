//
// Created by 28636 on 2025/5/27.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_SEXYAPPBASE_H
#define PLANTSVSZOMBIES_ANDROIDTV_SEXYAPPBASE_H

#include "Graphics/Color.h"
#include "Misc/Common.h"
#include "Misc/Rect.h"
#include "PvZ/Symbols.h"
#include "Widget/ButtonListener.h"
#include "Graphics/MemoryImage.h"
#include "Sound/AudiereSoundManager.h"
#include "PvZ/STL/string.h"

namespace Sexy {

class Dialog;

template <bool IS_AS_BASE = false>
class __SexyAppBase {
public:
    int *vTable;                            // 0
    int unkMem1[164];                       // 1 ~ 164
    int *mWidgetManager;                    // 165
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

    Dialog *GetDialog(Dialogs theDialogId) { return reinterpret_cast<Dialog *(*)(__SexyAppBase *, Dialogs)>(Sexy_SexyAppBase_GetDialogAddr)(this, theDialogId); }
    void EraseFile(const std::string &theFileName) { reinterpret_cast<void (*)(__SexyAppBase *, const std::string &)>(Sexy_SexyAppBase_EraseFileAddr)(this, theFileName); }
    Image *GetImage(const pvzstl::string &theFileName, bool commitBits = true) {
        return reinterpret_cast<Image *(*)(__SexyAppBase *, const pvzstl::string &, bool)>(Sexy_SexyAppBase_GetImageAddr)(this, theFileName, commitBits);
    }
    bool RegistryReadString(const std::string &theValueName, std::string *theString) {
        return reinterpret_cast<bool(*)(__SexyAppBase *, const std::string &, std::string *)>(Sexy_SexyAppBase_RegistryReadStringAddr)(this, theValueName, theString);
    }
    Image *CopyImage(Image *theImage) { return reinterpret_cast<Image *(*)(__SexyAppBase *, Image *)>(Sexy_SexyAppBase_CopyImageAddr)(this, theImage); }
    Image *CopyImage(Image *theImage, const Rect &theRect) { return reinterpret_cast<Image *(*)(__SexyAppBase *, Image *, const Rect &)>(Sexy_SexyAppBase_CopyImage2Addr)(this, theImage, theRect); }

    __SexyAppBase()
        requires(!IS_AS_BASE)
    {
        Create();
    }

    void Create();

    bool Is3DAccelerated(this const auto& self) {
        // 修复关闭3D加速后MV错位
        return self.mNewIs3DAccelerated || self.mCreditScreen != nullptr;
    }
    void Set3DAccelerated(this auto& self, bool isAccelerated) {
        self.mNewIs3DAccelerated = isAccelerated;
        self.mPlayerInfo->mIs3DAcceleratedClosed = !isAccelerated;
    }

protected:
    __SexyAppBase()
        requires IS_AS_BASE
    {}
};

using SexyAppBase = __SexyAppBase<>;

}

inline void (*old_Sexy_SexyAppBase_SexyAppBase)(Sexy::SexyAppBase *appBase);

#endif // PLANTSVSZOMBIES_ANDROIDTV_SEXYAPPBASE_H
