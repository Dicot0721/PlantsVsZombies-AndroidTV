#ifndef PVZ_LAWN_SYSTEM_SAVE_GAME_H
#define PVZ_LAWN_SYSTEM_SAVE_GAME_H

#include "PvZ/Symbols.h"

class SaveGameContext {
public:
    void SyncInt(int &theInt) {
        reinterpret_cast<void (*)(SaveGameContext *, int &)>(SaveGameContext_SyncIntAddr)(this, theInt);
    }

    void SyncReanimationDef(ReanimatorDefinition *&theDefinition);
};


#endif // PVZ_LAWN_SYSTEM_SAVE_GAME_H
