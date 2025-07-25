//
// Created by 28636 on 2025/7/17.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_SHOPSEEDPACKET_H
#define PLANTSVSZOMBIES_ANDROIDTV_SHOPSEEDPACKET_H

#include "PvZ/Symbols.h"

class ShopSeedPacket {
public:
    void Update() {
        reinterpret_cast<void (*)(ShopSeedPacket *)>(ShopSeedPacket_UpdateAddr)(this);
    }
};

#endif // PLANTSVSZOMBIES_ANDROIDTV_SHOPSEEDPACKET_H
