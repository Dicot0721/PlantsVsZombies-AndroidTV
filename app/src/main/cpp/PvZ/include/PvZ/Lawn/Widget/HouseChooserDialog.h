#ifndef PVZ_LAWN_WIDGET_HOUSE_CHOOSER_DIALOG_H
#define PVZ_LAWN_WIDGET_HOUSE_CHOOSER_DIALOG_H

#include "LawnDialog.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Lawn/GamepadControls.h"
#include "PvZ/SexyAppFramework/Misc/KeyCodes.h"

enum HouseType {
    BLUEPRINT_INVALID = -1,
    BLUEPRINT_BLING = 0,
    BLUEPRINT_SCARY = 1,
    BLUEPRINT_TRAILER = 2,
    BLUEPRINT_FUTURE = 3,
    BLUEPRINT_CLOWN = 4,
};

class HouseChooserDialog : public LawnDialog {
public:
    HouseType mSelectedHouseType; // 191
    int unk[2];                   // 192 ~ 193
    // 115: 194, 111: 196

    void GameButtonDown(ButtonCode theButton, int thePlayerIndex, bool a4) {
        reinterpret_cast<void (*)(HouseChooserDialog *, ButtonCode, int, bool)>(HouseChooserDialog_GameButtonDownAddr)(this, theButton, thePlayerIndex, a4);
    }

    static bool IsHouseAvaliable(HouseType houseType);

    void MouseDown(int x, int y, int theClickCount);
    void KeyDown(Sexy::KeyCode theKey);
};


inline void (*old_HouseChooserDialog_MouseDown)(HouseChooserDialog *a, int x, int y, int theClickCount);

inline void (*old_HouseChooserDialog_KeyDown)(HouseChooserDialog *a, Sexy::KeyCode theKey);

#endif // PVZ_LAWN_WIDGET_HOUSE_CHOOSER_DIALOG_H
