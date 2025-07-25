#ifndef PVZ_LAWN_BOARD_GAME_OBJECT_H
#define PVZ_LAWN_BOARD_GAME_OBJECT_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/Symbols.h"

class LawnApp;
class Board;

class GameObject {
public:
    int *vTable;        // 0
    int placeHolder[3]; // 1 ~ 3
    LawnApp *mApp;      // 4
    Board *mBoard;      // 5
    int mX;             // 6
    int mY;             // 7
    int mWidth;         // 8
    int mHeight;        // 9
    bool mVisible;      // 40
    int mRow;           // 11
    int mRenderOrder;   // 12
    // 大小13个整数

    bool BeginDraw(Sexy::Graphics *g) {
        return reinterpret_cast<bool (*)(GameObject *, Sexy::Graphics *)>(GameObject_BeginDrawAddr)(this, g);
    }
    void EndDraw(Sexy::Graphics *g) {
        reinterpret_cast<void (*)(GameObject *, Sexy::Graphics *)>(GameObject_EndDrawAddr)(this, g);
    }
    void MakeParentGraphicsFrame(Sexy::Graphics *g) {
        reinterpret_cast<void (*)(GameObject *, Sexy::Graphics *)>(GameObject_MakeParentGraphicsFrameAddr)(this, g);
    }
};


#endif // PVZ_LAWN_BOARD_GAME_OBJECT_H
