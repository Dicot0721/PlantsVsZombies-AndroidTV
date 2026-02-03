/*
 * Copyright (C) 2023-2026  PvZ TV Touch Team
 *
 * This file is part of PlantsVsZombies-AndroidTV.
 *
 * PlantsVsZombies-AndroidTV is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * PlantsVsZombies-AndroidTV is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * PlantsVsZombies-AndroidTV.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef PVZ_SEXYAPPFRAMEWORK_WIDGET_GAME_BUTTON_H
#define PVZ_SEXYAPPFRAMEWORK_WIDGET_GAME_BUTTON_H

#include "PvZ/Symbols.h"

#include "PvZ/SexyAppFramework/Widget/DialogButton.h"

class GameButton : public Sexy::DialogButton {
public:
    int unkMems4[2];                // 210  ~ 211
    bool unkBool5;                  // 848
    int unkMems5[2];                // 213 ~ 214
    Sexy::Image *mOverOverlayImage; // 215
    bool mDrawStoneButton;          // 864
    int unkMems6[13];               // 217 ~ 229
    // 大小230个整数

    // GameButton(int theId, ButtonListener *theListener) { Create(); }
    ~GameButton() {
        _destructor();
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
    void Draw(Sexy::Graphics *g) {
        reinterpret_cast<void (*)(GameButton *, Sexy::Graphics *)>(GameButton_DrawAddr)(this, g);
    }
    void OnPressed() {
        reinterpret_cast<void (*)(GameButton *)>(GameButton_OnPressedAddr)(this);
    }

protected:
    void _constructor(int theId, Sexy::ButtonListener *theListener) {
        reinterpret_cast<void (*)(GameButton *, int, Sexy::ButtonListener *)>(GameButton_GameButtonAddr)(this, theId, theListener);
    }
    void _destructor() {
        reinterpret_cast<void (*)(GameButton *)>(GameButton_DeleteAddr)(this);
    }
};

class NewLawnButton : public Sexy::DialogButton {};

inline GameButton *MakeButton(int theId, Sexy::ButtonListener *theListener, Sexy::Widget *theParent, const pvzstl::string &theText) {
    return reinterpret_cast<GameButton *(*)(int, Sexy::ButtonListener *, Sexy::Widget *, const pvzstl::string &)>(MakeButtonAddr)(theId, theListener, theParent, theText);
}
inline NewLawnButton *MakeNewButton(int theId,
                                    Sexy::ButtonListener *theListener,
                                    Sexy::Widget *theWidget,
                                    const pvzstl::string &theText,
                                    Sexy::Font *theFont,
                                    Sexy::Image *theImageNormal,
                                    Sexy::Image *theImageOver,
                                    Sexy::Image *theImageDown) {
    return reinterpret_cast<NewLawnButton *(*)(int, Sexy::ButtonListener *, Sexy::Widget *, const pvzstl::string &, Sexy::Font *, Sexy::Image *, Sexy::Image *, Sexy::Image *)>(MakeNewButtonAddr)(
        theId, theListener, theWidget, theText, theFont, theImageNormal, theImageOver, theImageDown);
}


#endif // PVZ_SEXYAPPFRAMEWORK_WIDGET_GAME_BUTTON_H
