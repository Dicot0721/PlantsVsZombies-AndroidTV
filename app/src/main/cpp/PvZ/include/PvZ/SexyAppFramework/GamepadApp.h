#ifndef PVZ_SEXYAPPFRAMEWORK_GAMEPAD_APP_H
#define PVZ_SEXYAPPFRAMEWORK_GAMEPAD_APP_H

#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Symbols.h"

namespace Sexy {

class GamepadApp : public __SexyAppBase {
public:
    bool HasGamepad() {
        return reinterpret_cast<bool (*)(GamepadApp *)>(Sexy_GamepadApp_HasGamepadAddr)(this);
    }
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_GAMEPAD_APP_H
