//
// Created by 28636 on 2025/7/17.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_SAVEGAME_H
#define PLANTSVSZOMBIES_ANDROIDTV_SAVEGAME_H

#include "PvZ/Symbols.h"

class SaveGameContext {
public:
    void SyncInt(int &theInt) { reinterpret_cast<void (*)(SaveGameContext *, int &)>(SaveGameContext_SyncIntAddr)(this, theInt); }

    void SyncReanimationDef(ReanimatorDefinition*& theDefinition);
};


#endif // PLANTSVSZOMBIES_ANDROIDTV_SAVEGAME_H
