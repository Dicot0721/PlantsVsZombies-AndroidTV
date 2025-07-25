#ifndef PVZ_SEXYAPPFRAMEWORK_WIDGET_GAME_BUTTON_H
#define PVZ_SEXYAPPFRAMEWORK_WIDGET_GAME_BUTTON_H

#include "PvZ/Symbols.h"

#include "DialogButton.h"

namespace Sexy {

class GameButton : public DialogButton {
public:
    int unkMems4[2];          // 210  ~ 211
    bool unkBool5;            // 848
    int unkMems5[2];          // 213 ~ 214
    Image *mOverOverlayImage; // 215
    bool mDrawStoneButton;    // 864
    int unkMems6[13];         // 217 ~ 229
    // 大小230个整数

    //    GameButton(int theId, ButtonListener *theListener) { Create(); }
    ~GameButton() {
        __Destructor();
    }
    bool IsMouseOver() {
        return reinterpret_cast<bool (*)(GameButton *)>(GameButton_IsMouseOverAddr)(this);
    }
    void Resize(int theX, int theY, int theWidth, int theHeight) {
        reinterpret_cast<void (*)(GameButton *, int, int, int, int)>(GameButton_ResizeAddr)(this, theX, theY, theWidth, theHeight);
    }
    void Update() {
        reinterpret_cast<void (*)(GameButton *)>(GameButton_UpdateAddr)(this);
    }
    void SetLabel(const pvzstl::string &theLabel) {
        reinterpret_cast<void (*)(GameButton *, const pvzstl::string &)>(GameButton_SetLabelAddr)(this, theLabel);
    }
    void SetDisabled(bool theDisabled) {
        reinterpret_cast<void (*)(GameButton *, bool)>(GameButton_SetDisabledAddr)(this, theDisabled);
    }
    void Draw(Graphics *g) {
        reinterpret_cast<void (*)(GameButton *, Graphics *)>(GameButton_DrawAddr)(this, g);
    }
    void OnPressed() {
        reinterpret_cast<void (*)(GameButton *)>(GameButton_OnPressedAddr)(this);
    }

protected:
    void __Constructor(int theId, ButtonListener *theListener) {
        reinterpret_cast<void (*)(GameButton *, int, ButtonListener *)>(GameButton_GameButtonAddr)(this, theId, theListener);
    }
    void __Destructor() {
        reinterpret_cast<void (*)(GameButton *)>(GameButton_DeleteAddr)(this);
    }
};

inline GameButton *MakeButton(int theId, ButtonListener *theListener, __Widget *theParent, const pvzstl::string &theText) {
    return reinterpret_cast<GameButton *(*)(int, ButtonListener *, __Widget *, const pvzstl::string &)>(MakeButtonAddr)(theId, theListener, theParent, theText);
}


} // namespace Sexy


#endif // PVZ_SEXYAPPFRAMEWORK_WIDGET_GAME_BUTTON_H
