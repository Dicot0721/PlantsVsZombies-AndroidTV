//
// Created by 28636 on 2025/5/26.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_MEMORYIMAGE_H
#define PLANTSVSZOMBIES_ANDROIDTV_MEMORYIMAGE_H

#include "Image.h"

namespace Sexy {

class SexyMatrix3;

class MemoryImage : public __Image {
public:
    LawnApp *mApp;                    // 28
    int unkMems4;                     // 29
    ulong *mBits;                     // 30
    int mBitsChangedCount;            // 31
    int unk1[4];                      // 32 ~ 35
    bool mUnkModeRelated;             // 144
    bool mUnkMode1;                   // 145
    bool mUnkMode2;                   // 146
    bool mUnkBool;                    // 147
    bool mIsVolatile;                 // 148
    int unk2[20];                     // 38 ~ 57
    // 大小58个整数

    MemoryImage() { __Constructor(); }
    ~MemoryImage() { __Destructor(); }

    ulong *GetBits() { return reinterpret_cast<ulong *(*)(MemoryImage *)>(Sexy_MemoryImage_GetBitsAddr)(this); }
    void Create(int theWidth, int theHeight) { reinterpret_cast<void (*)(MemoryImage *, int, int)>(Sexy_MemoryImage_CreateAddr)(this, theWidth, theHeight); }
    void SetImageMode(bool hasTrans, bool hasAlpha) { reinterpret_cast<void (*)(MemoryImage *, bool, bool)>(Sexy_MemoryImage_SetImageModeAddr)(this, hasTrans, hasAlpha); }
    void WriteToPng(int *theString) { reinterpret_cast<void (*)(MemoryImage *, int *)>(Sexy_MemoryImage_WriteToPngAddr)(this, theString); }
    void WriteToJPEG(int *theString) { reinterpret_cast<void (*)(MemoryImage *, int *)>(Sexy_MemoryImage_WriteToJPEGAddr)(this, theString); }
    void BitsChanged() { reinterpret_cast<void (*)(MemoryImage *)>(Sexy_MemoryImage_BitsChangedAddr)(this); }

    void ClearRect(const Rect& theRect);
    void PushTransform(const SexyMatrix3 &theTransform, bool concatenate);
    void PopTransform();

protected:
    void __Constructor() { reinterpret_cast<void (*)(MemoryImage *)>(Sexy_MemoryImage_MemoryImageAddr)(this); }
    void __Destructor() { reinterpret_cast<void (*)(MemoryImage *)>(Sexy_MemoryImage_DeleteAddr)(this); }
};

}

inline void (*old_Sexy_MemoryImage_PushTransform)(Sexy::MemoryImage *image, const Sexy::SexyMatrix3 &theTransform, bool concatenate);

inline void (*old_Sexy_MemoryImage_PopTransform)(Sexy::MemoryImage *image);


#endif // PLANTSVSZOMBIES_ANDROIDTV_MEMORYIMAGE_H
