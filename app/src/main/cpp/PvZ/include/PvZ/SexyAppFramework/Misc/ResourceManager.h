//
// Created by 28636 on 2025/5/27.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_RESOURCEMANAGER_H
#define PLANTSVSZOMBIES_ANDROIDTV_RESOURCEMANAGER_H

#include "Common.h"
#include "../Graphics/Image.h"
//#include "../SexyAppBase.h"
#include <string>
#include <map>

namespace Sexy {

class ResourceManager
{
protected:
    enum ResType {
        ResType_Image,
        ResType_Sound,
        ResType_Font,
        ResType_Music,
        ResType_Reanim,
        ResType_Particle,
        ResType_Trail
    };

    struct BaseRes {
        int *vTable;            // 0
        ResType mType;          // 1
        int *mResourceRef;      // 2
        int unk[8];             // 3 ~ 10
        char *mPath;            // 11
        int unk2[13];           // 12 ~ 24
    }; // 大小25个整数

    struct SoundRes : public BaseRes {
        int mSoundId;   // 25
        double mVolume; // 26 ~ 27
        int mPanning;   // 28
        int unk3;       // 29
    }; // 大小30个整数
};

}

#endif // PLANTSVSZOMBIES_ANDROIDTV_RESOURCEMANAGER_H
