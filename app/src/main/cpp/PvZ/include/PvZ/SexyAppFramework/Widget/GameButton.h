//
// Created by 28636 on 2025/5/26.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_GAMEBUTTON_H
#define PLANTSVSZOMBIES_ANDROIDTV_GAMEBUTTON_H

#include "PvZ/Symbols.h"

#include "DialogButton.h"

namespace Sexy {

class GameButton : public DialogButton {
public:
    int unkMems4[2];                // 210  ~ 211
    bool unkBool5;                  // 848
    int unkMems5[2];                // 213 ~ 214
    Image *mOverOverlayImage; // 215
    bool mDrawStoneButton;          // 864
    int unkMems6[13];               // 217 ~ 229
    // 大小230个整数

    GameButton(int theId, Sexy::ButtonListener *theListener) { reinterpret_cast<void (*)(Sexy::GameButton *, int, Sexy::ButtonListener *)>(GameButton_GameButtonAddr)(this, theId, theListener); }
    ~GameButton() { reinterpret_cast<void (*)(Sexy::GameButton *)>(GameButton_DeleteAddr)(this); }
    bool IsMouseOver() { return reinterpret_cast<bool (*)(Sexy::GameButton *)>(GameButton_IsMouseOverAddr)(this); }
    void Resize(int theX, int theY, int theWidth, int theHeight) { reinterpret_cast<void (*)(Sexy::GameButton *, int, int, int, int)>(GameButton_ResizeAddr)(this, theX, theY, theWidth, theHeight); }
    void Update() { reinterpret_cast<void (*)(Sexy::GameButton *)>(GameButton_UpdateAddr)(this); }
    void SetLabel(const SexyString &theLabel) { reinterpret_cast<void (*)(Sexy::GameButton *, const SexyString&)>(GameButton_SetLabelAddr)(this, theLabel); }
    void SetDisabled(bool theDisabled) { reinterpret_cast<void (*)(Sexy::GameButton *, bool)>(GameButton_SetDisabledAddr)(this, theDisabled); }
    void Draw(Sexy::Graphics *g) { reinterpret_cast<void (*)(Sexy::GameButton *, Sexy::Graphics *)>(GameButton_DrawAddr)(this, g); }
    void OnPressed() { reinterpret_cast<void (*)(Sexy::GameButton *)>(GameButton_OnPressedAddr)(this); }
};

inline Sexy::GameButton *MakeButton(int theId, Sexy::ButtonListener *theListener, Sexy::Widget *theParent, const SexyString &theText) {
    return reinterpret_cast<Sexy::GameButton *(*)(int, Sexy::ButtonListener *, Sexy::Widget *, const SexyString &)>(MakeButtonAddr)(theId, theListener, theParent, theText);
}



}


#endif // PLANTSVSZOMBIES_ANDROIDTV_GAMEBUTTON_H
