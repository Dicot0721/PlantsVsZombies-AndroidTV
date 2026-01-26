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

#ifndef PVZ_LAWN_WIDGET_VS_SETUP_MENU_H
#define PVZ_LAWN_WIDGET_VS_SETUP_MENU_H

#include "PvZ/Lawn/GamepadControls.h"
#include "PvZ/Lawn/Widget/VSSetupAddonWidget.h"
#include "PvZ/SexyAppFramework/Widget/ButtonListener.h"
#include "PvZ/SexyAppFramework/Widget/Widget.h"
#include "PvZ/Symbols.h"
#include "WaitForSecondPlayerDialog.h"

enum VSSetupState {
    VS_SETUP_STATE_CONTROLLERS = 0,
    VS_SETUP_STATE_SIDES = 1,
    VS_SETUP_STATE_SELECT_BATTLE = 2,
    VS_SETUP_STATE_CUSTOM_BATTLE = 3,
};

enum VSSetupMode {
    VS_SETUP_MODE_QUICK_PLAY = 0,    // 快速游戏
    VS_SETUP_MODE_CUSTOM_BATTLE = 1, // 自定义战场
    VS_SETUP_MODE_RANDOM_BATTLE = 2, // 随机战场
};

namespace Sexy {
class ButtonWidget;
}

class LawnApp;
class Board;
class DefaultPlayerInfo;

class VSSetupMenu : public Sexy::__Widget {
public:
    enum {
        VSSetupMenu_Quick_Play = 9,     // 快速游戏
        VSSetupMenu_Custom_Battle = 10, // 自定义战场
        VSSetupMenu_Random_Battle = 11, // 随机战场
    };

    Sexy::ButtonListener mButtonListener; // 64
    int unkInt[5];                        // 65 ~ 69
    int mInt70;                           // 70
    int mInt71;                           // 71
    int mInt72;                           // 72
    LawnApp *mApp;                        // 73
    VSSetupState mState;                  // 74 0:WaitForSecondPlayerDialog,1:未分配手柄阵营,2:已分配手柄阵营,3:自定义战场选卡中
    int mController1Index;                // 75 // 0:手柄1, 1:手柄2
    int mController2Index;                // 76 // 0:手柄1, 1:手柄2
    int mController1Position;             // 77  // -1 0 1， 分别位于中 左 右
    int mController2Position;             // 78  // -1 0 1， 分别位于中 左 右
    int unkInt79;                         // 79
    int mNextFirstPick;                   // 80
    int mInt81;                           // 81
    VSSetupMode mSetupMode;               // 82
    int unkInt83[85];                     // 83 ~ 167
    int mInt168;                          // 168
    int unkInt169[59];                    // 169 ~ 227
    int mInt228;                          // 228
    int unkInt229[63];                    // 229 ~ 291
    int mInt292;                          // 292
    int unkInt293[3];                     // 293 ~ 295
    int mInt296;                          // 296
    int unkInt297[7];                     // 297 ~ 303
    int mInt304;                          // 304
    int unkInt305[11];                    // 305 ~ 315
    bool mBool316;                        // 316
    bool mBool317;                        // 317
    int unkInt318[2];                     // 318 ~ 319
    int mInt320;                          // 320
    int unkInt321[3];                     // 321 ~ 323
    int mInt324;                          // 324
    int unkInt325[3];                     // 325 ~ 327
    int mInt328;                          // 328
    int unkInt329[3];                     // 329 ~ 331
    int mInt332;                          // 332
    int unkInt333[28];                    // 333 ~ 360
    bool mBoo361;                         // 361
    bool mBool362;                        // 362
    int unkInt363[161];                   // 363 ~ 523
    int mInt524;                          // 524

    void GameButtonDown(GamepadButton theButton, unsigned int thePlayerIndex, int a4) {
        reinterpret_cast<void (*)(VSSetupMenu *, GamepadButton, unsigned int, int)>(VSSetupMenu_GameButtonDownAddr)(this, theButton, thePlayerIndex, a4);
    }
    void SetSecondPlayerIndex(int thePlayerIndex) {
        reinterpret_cast<void (*)(VSSetupMenu *, int)>(VSSetupMenu_SetSecondPlayerIndexAddr)(this, thePlayerIndex);
    }
    void GoToState(VSSetupState theState) {
        reinterpret_cast<void (*)(VSSetupMenu *, int)>(VSSetupMenu_GoToStateAddr)(this, theState);
    }
    void OnPlayerPickedSeed(int thePlayerIndex) {
        reinterpret_cast<void (*)(VSSetupMenu *, int)>(VSSetupMenu_OnPlayerPickedSeedAddr)(this, thePlayerIndex);
    }

    VSSetupMenu() {
        _constructor();
    }
    ~VSSetupMenu() {
        _destructor();
    }

    static size_t getServerEventSize(EventType type);
    static size_t getClientEventSize(EventType type);

    void Draw(Sexy::Graphics *g);
    void DrawOverlay(Sexy::Graphics *g);
    void Update();
    void KeyDown(Sexy::KeyCode theKey);
    void OnStateEnter(VSSetupState theState);
    void ButtonPress(int theId);
    void ButtonDepress(int theId);
    void MouseDown(int x, int y, int theCount);
    void MouseDrag(int x, int y);
    void MouseUp(int x, int y, int theCount);
    void AddedToManager(Sexy::WidgetManager *a2);

    void processClientEvent(void *buf, ssize_t bufSize);
    void processServerEvent(void *buf, ssize_t bufSize);
    void CloseVSSetup(bool a2);

protected:
    friend void InitHookFunction();
    void _constructor();
    void _destructor();

    void PickRandomZombies(std::vector<SeedType> &theZombieSeeds);
    void PickRandomPlants(std::vector<SeedType> &thePlantSeeds, std::vector<SeedType> const &theZombieSeeds);

    void PickBackgroundImmediately();
};

inline VSSetupAddonWidget *gVSSetupAddonWidget;

inline int gVSSetupRequestState = 0;

inline bool is1PControllerMoving;
inline bool is2PControllerMoving;
inline int touchDownX;
inline int touchingOnWhichController; // 0 NONE, 1 1P, 2 2P
inline int drawTipArrowAlphaCounter;


inline void (*old_VSSetupMenu_Constructor)(VSSetupMenu *);

inline void (*old_VSSetupMenu_Destructor)(VSSetupMenu *);

inline void (*old_VSSetupMenu_Draw)(VSSetupMenu *, Sexy::Graphics *g);

inline void (*old_VSSetupMenu_DrawOverlay)(VSSetupMenu *, Sexy::Graphics *g);

inline void (*old_VSSetupMenu_Update)(VSSetupMenu *a);

inline void (*old_VSSetupMenu_KeyDown)(VSSetupMenu *a, Sexy::KeyCode a2);

inline void (*old_VSSetupMenu_OnStateEnter)(VSSetupMenu *menu, VSSetupState theState);

inline void (*old_VSSetupMenu_ButtonPress)(VSSetupMenu *, int theId);

inline void (*old_VSSetupMenu_ButtonDepress)(VSSetupMenu *, int theId);

inline void (*old_VSSetupMenu_PickRandomZombies)(VSSetupMenu *, std::vector<SeedType> &theVector);

inline void (*old_VSSetupMenu_PickRandomPlants)(VSSetupMenu *, std::vector<SeedType> &theVector1, std::vector<SeedType> const &theVector2);

inline void (*old_VSSetupMenu_AddedToManager)(VSSetupMenu *, Sexy::WidgetManager *a2);

inline void (*old_VSSetupMenu_CloseVSSetup)(VSSetupMenu *, bool a2);

#endif // PVZ_LAWN_WIDGET_VS_SETUP_MENU_H
