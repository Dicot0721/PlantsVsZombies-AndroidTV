#include "PvZ/Lawn/Board/LawnMower.h"
#include "PvZ/GlobalVariable.h"

void LawnMower::Update() {
    if (!requestPause) {
        old_LawnMower_Update(this);
    }
}
