//
// Created by 28636 on 2025/7/17.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_GAMEPADAPP_H
#define PLANTSVSZOMBIES_ANDROIDTV_GAMEPADAPP_H

#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Symbols.h"

namespace Sexy {

class GamepadApp : public SexyAppBase {
public:
    bool HasGamepad() {
        return reinterpret_cast<bool(*)(GamepadApp *)>(Sexy_GamepadApp_HasGamepadAddr)(this);
    }
};

}

#endif // PLANTSVSZOMBIES_ANDROIDTV_GAMEPADAPP_H
