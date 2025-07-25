//
// Created by 28636 on 2025/5/26.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_CHECKBOX_H
#define PLANTSVSZOMBIES_ANDROIDTV_CHECKBOX_H

#include "PvZ/Symbols.h"

#include "Widget.h"

namespace Sexy {

class Checkbox : public Widget {
public:
    int unknownMember[8];   // 64 ~ 71
    int *mListener;         // 72
    Image *mUncheckedImage; // 73
    Image *mCheckedImage;   // 74
    int mId;                // 75
    bool mChecked;          // 304
    Rect mCheckedRect;      // 77 ~ 80
    Rect mUncheckedRect;    // 81 ~ 84
    Color mOutlineColor;    // 85 ~ 88
    Color mBkgColor;        // 89 ~ 92
    Color mCheckColor;      // 93 ~ 96
    // 大小97个整数

    ~Checkbox() {
        __Destructor();
    }

protected:
    void __Destructor() {
        reinterpret_cast<void (*)(Checkbox *)>(Sexy_Checkbox_DeleteAddr)(this);
    }
};

} // namespace Sexy


#endif // PLANTSVSZOMBIES_ANDROIDTV_CHECKBOX_H
