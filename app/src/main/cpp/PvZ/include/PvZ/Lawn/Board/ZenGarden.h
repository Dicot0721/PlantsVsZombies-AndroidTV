#ifndef PVZ_LAWN_BOARD_ZEN_GARDEN_H
#define PVZ_LAWN_BOARD_ZEN_GARDEN_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/Symbols.h"

class LawnApp;
class Board;
class PottedPlant;
class GridItem;
class SpecialGridPlacement {
public:
    int mPixelX;
    int mPixelY;
    int mGridX;
    int mGridY;
};

class ZenGarden {
public:
    LawnApp *mApp;          //+0x0
    Board *mBoard;          //+0x4
    GardenType mGardenType; //+0x8

    bool IsStinkyHighOnChocolate() {
        return reinterpret_cast<bool (*)(ZenGarden *)>(ZenGarden_IsStinkyHighOnChocolateAddr)(this);
    }
    void OpenStore() {
        reinterpret_cast<void (*)(ZenGarden *)>(ZenGarden_OpenStoreAddr)(this);
    }
    PottedPlant *GetPottedPlantInWheelbarrow() {
        return reinterpret_cast<PottedPlant *(*)(ZenGarden *)>(ZenGarden_GetPottedPlantInWheelbarrowAddr)(this);
    }
    void DrawPottedPlant(Sexy::Graphics *g, float x, float y, PottedPlant *thePottedPlant, float theScale, bool theDrawPot) {
        reinterpret_cast<void (*)(ZenGarden *, Sexy::Graphics *, float, float, PottedPlant *, float, bool)>(ZenGarden_DrawPottedPlantAddr)(this, g, x, y, thePottedPlant, theScale, theDrawPot);
    }

    SpecialGridPlacement *GetSpecialGridPlacements(int &theCount);
    int GridToPixelX(int theGridX, int theGridY);
    int GridToPixelY(int theGridX, int theGridY);
    GridItem *GetStinky();
    void DrawBackdrop(Sexy::Graphics *g);
};


inline void (*old_ZenGarden_DrawBackdrop)(ZenGarden *zenGarden, Sexy::Graphics *graphics);


#endif // PVZ_LAWN_BOARD_ZEN_GARDEN_H
