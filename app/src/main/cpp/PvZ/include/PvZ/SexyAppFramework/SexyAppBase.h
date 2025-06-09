//
// Created by 28636 on 2025/5/27.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_SEXYAPPBASE_H
#define PLANTSVSZOMBIES_ANDROIDTV_SEXYAPPBASE_H

#include "Graphics/Color.h"
#include "Misc/Common.h"
#include "Misc/Rect.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/MagicAddr.h"
#include "PvZ/Symbols.h"
#include "Widget/ButtonListener.h"

namespace Sexy {

class Dialog;

class SexyAppBase {
public:
    int *vTable;                            // 0
    int unkMem1[164];                       // 1 ~ 164
    int *mWidgetManager;                    // 165
    int unkMem2[70];                        // 166 ~ 235, musicinterface 193
    bool mLawnMouseMode;                    // 944
    int unkMem3[77];                        // 237 ~ 313
    int *mSoundManager;                     // 314
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

    Dialog *GetDialog(Dialogs theDialogId) { return reinterpret_cast<Dialog *(*)(SexyAppBase *, Dialogs)>(Sexy_SexyAppBase_GetDialogAddr)(this, theDialogId); }
    void EraseFile(const std::string& theFileName) { reinterpret_cast<void (*)(SexyAppBase *, const std::string&)>(Sexy_SexyAppBase_EraseFileAddr)(this, theFileName); }
    Image *GetImage(const std::string &theFileName, bool commitBits = true) {
        return reinterpret_cast<Image *(*)(SexyAppBase *, const std::string &, bool)>(Sexy_SexyAppBase_GetImageAddr)(this, theFileName, commitBits);
    }

    SexyAppBase() { Create(); }
    void Create();
};

}

inline void (*old_Sexy_SexyAppBase_SexyAppBase)(Sexy::SexyAppBase *appBase);

#endif // PLANTSVSZOMBIES_ANDROIDTV_SEXYAPPBASE_H
