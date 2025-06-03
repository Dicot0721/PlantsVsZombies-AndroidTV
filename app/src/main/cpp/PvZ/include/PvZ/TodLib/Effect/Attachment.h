//
// Created by 28636 on 2025/5/26.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_ATTACHMENT_H
#define PLANTSVSZOMBIES_ANDROIDTV_ATTACHMENT_H

#include "PvZ/TodLib/Common/DataArray.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/SexyAppFramework/Misc/SexyMatrix.h"
#include "PvZ/Symbols.h"

class AttachEffect {
public:
    ReanimationID mEffectID;      // 0
    EffectType mEffectType;       // 1
    Sexy::SexyTransform2D mOffset;// 2 ~ 10
    bool mDontDrawIfParentHidden; // 44
    bool mDontPropogateColor;     // 45
}; // 大小12个整数

class Attachment {
public:
    int unk[4];                    // 0 ~ 3
    AttachEffect mEffectArray[16]; // 4 ~ 195
    int mNumEffects;               // 196
    bool mDead;                    //
    bool mActive;
    bool mUsesClipping;
    int mAttachmentID;
}; // 大小199个整数

inline AttachEffect *AttachReanim(AttachmentID &theAttachmentID, Reanimation *theReanimation, float theOffsetX, float theOffsetY) {
    return reinterpret_cast<AttachEffect *(*)(AttachmentID &, Reanimation *, float, float)>(AttachReanimAddr)(theAttachmentID, theReanimation, theOffsetX, theOffsetY);
}
inline void AttachmentDraw(AttachmentID& theAttachmentID, Graphics* g, bool theParentHidden) {
    reinterpret_cast<void (*)(AttachmentID &, Graphics*, bool)>(AttachmentDrawAddr)(theAttachmentID, g, theParentHidden);
}
inline void AttachmentDie(AttachmentID& theAttachmentID) { reinterpret_cast<void (*)(AttachmentID&)>(AttachmentDieAddr)(theAttachmentID); }

class AttachmentHolder {};

#endif // PLANTSVSZOMBIES_ANDROIDTV_ATTACHMENT_H
