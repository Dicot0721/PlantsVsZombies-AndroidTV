#ifndef PVZ_LAWN_WIDGET_SHOP_SEED_PACKET_H
#define PVZ_LAWN_WIDGET_SHOP_SEED_PACKET_H

#include "PvZ/Symbols.h"

class ShopSeedPacket {
public:
    void Update() {
        reinterpret_cast<void (*)(ShopSeedPacket *)>(ShopSeedPacket_UpdateAddr)(this);
    }
};

#endif // PVZ_LAWN_WIDGET_SHOP_SEED_PACKET_H
