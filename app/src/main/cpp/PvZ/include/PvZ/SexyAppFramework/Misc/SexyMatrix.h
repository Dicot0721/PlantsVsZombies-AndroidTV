//
// Created by 28636 on 2025/5/26.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_SEXYMATRIX_H
#define PLANTSVSZOMBIES_ANDROIDTV_SEXYMATRIX_H

#include "SexyVector.h"

namespace Sexy {

class SexyMatrix3 {
public:
    union {
        float m[3][3];
        struct {
            float m00, m01, m02;
            float m10, m11, m12;
            float m20, m21, m22;
        };
    };
};

class SexyTransform2D : public Sexy::SexyMatrix3 {
public:
//    SexyTransform2D() { Create(); }
//    Create() { reinterpret_cast<void (*)()>; }
};

}


#endif // PLANTSVSZOMBIES_ANDROIDTV_SEXYMATRIX_H
