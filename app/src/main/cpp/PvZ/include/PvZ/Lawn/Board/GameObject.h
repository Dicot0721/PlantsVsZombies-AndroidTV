/*
 * Copyright (C) 2023-2026  PvZ TV Touch Team
 *
 * This file is part of PlantsVsZombies-AndroidTV.
 *
 * PlantsVsZombies-AndroidTV is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * PlantsVsZombies-AndroidTV is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * PlantsVsZombies-AndroidTV.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef PVZ_LAWN_BOARD_GAME_OBJECT_H
#define PVZ_LAWN_BOARD_GAME_OBJECT_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/Symbols.h"

class LawnApp;
class Board;

class __GameObject {
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

    void _destructor() {
        reinterpret_cast<bool (*)(__GameObject *)>(GameObject_Delete2Addr)(this);
    }
    bool BeginDraw(Sexy::Graphics *g) {
        return reinterpret_cast<bool (*)(__GameObject *, Sexy::Graphics *)>(GameObject_BeginDrawAddr)(this, g);
    }
    void EndDraw(Sexy::Graphics *g) {
        reinterpret_cast<void (*)(__GameObject *, Sexy::Graphics *)>(GameObject_EndDrawAddr)(this, g);
    }
    void MakeParentGraphicsFrame(Sexy::Graphics *g) {
        reinterpret_cast<void (*)(__GameObject *, Sexy::Graphics *)>(GameObject_MakeParentGraphicsFrameAddr)(this, g);
    }

protected:
    __GameObject() = default;
    ~__GameObject() = default;
};

class GameObject : public __GameObject {
public:
    GameObject() = delete;
    ~GameObject() {
        __GameObject::_destructor();
    };
};

#endif // PVZ_LAWN_BOARD_GAME_OBJECT_H
