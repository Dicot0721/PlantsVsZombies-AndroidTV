//
// Created by 28636 on 2025/5/26.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_SEXYMATRIX_H
#define PLANTSVSZOMBIES_ANDROIDTV_SEXYMATRIX_H

#include "PvZ/Symbols.h"

#include "SexyVector.h"

namespace Sexy {

template <bool IS_AS_BASE = false>
class __SexyMatrix3 {
public:
    union {
        float m[3][3];
        struct {
            float m00, m01, m02;
            float m10, m11, m12;
            float m20, m21, m22;
        };
    };

    __SexyMatrix3()
        requires(!IS_AS_BASE)
    {
        Create();
    }

    void Create() { reinterpret_cast<void (*)(__SexyMatrix3 *)>(Sexy_SexyMatrix3_SexyMatrix3Addr)(this); }

protected:
    __SexyMatrix3()
        requires IS_AS_BASE
    {}
};

class SexyTransform2D : public __SexyMatrix3<true> {
public:
    SexyTransform2D() { Create(); }
    void Create() { reinterpret_cast<void (*)(SexyTransform2D *)>(Sexy_SexyTransform2D_SexyTransform2DAddr)(this); }
    void Scale(float sx, float sy) { reinterpret_cast<void (*)(SexyTransform2D *, float, float)>(Sexy_SexyTransform2D_ScaleAddr)(this, sx, sy); }
    void Translate(float tx, float ty) { reinterpret_cast<void (*)(SexyTransform2D *, float, float)>(Sexy_SexyTransform2D_TranslateAddr)(this, tx, ty); }
    void RotateRad(float rot) { reinterpret_cast<void (*)(SexyTransform2D *, float)>(Sexy_SexyTransform2D_RotateRadAddr)(this, rot); }
};

using SexyMatrix3 = __SexyMatrix3<>;

}; // namespace Sexy


#endif // PLANTSVSZOMBIES_ANDROIDTV_SEXYMATRIX_H
