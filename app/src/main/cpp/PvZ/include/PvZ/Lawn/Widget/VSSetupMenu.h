/*
 * Copyright (C) 2023-2025  PvZ TV Touch Team
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
#include "PvZ/SexyAppFramework/Widget/ButtonListener.h"
#include "PvZ/SexyAppFramework/Widget/Widget.h"
#include "PvZ/Symbols.h"

namespace Sexy {
class ButtonWidget;
}

class LawnApp;
class Board;
class DefaultPlayerInfo;

class VSSetupWidget {
public:
    LawnApp *mApp = reinterpret_cast<LawnApp *>(*gLawnApp_Addr);
    Sexy::ButtonWidget *mMoreSeedsButton = nullptr;
    Sexy::ButtonWidget *mMorePacketsButton = nullptr;
    Sexy::Image *mCheckboxImage = nullptr;
    Sexy::Image *mCheckboxImagePress = nullptr;
    bool mIsMorePackets = false;
    bool mDrawString = false;

    VSSetupWidget();
    ~VSSetupWidget();
    void SetDisable();
    void ButtonDepress(this VSSetupWidget &self, int theId);
    void CheckboxChecked(int theId, bool checked);
    void GetZombieSeedType();

private:
    static constexpr Sexy::ButtonListener::VTable sButtonListenerVtable{
        .ButtonDepress = (void *)&VSSetupWidget::ButtonDepress,
    };

    static inline Sexy::ButtonListener sButtonListener{&sButtonListenerVtable};
};

constexpr const int MORE_PACKETS_BUTTON_X = 800;
constexpr const int MORE_PACKETS_BUTTON_Y = 200;
inline VSSetupWidget *gVSMorePacketsButton;


class VSSetupMenu : public Sexy::__Widget {
public:
    Sexy::ButtonListener mButtonListener; // 64
    int unkInt[5];                        // 65 ~ 69
    int mInt70;                           // 70
    int mInt71;                           // 71
    int mInt72;                           // 72
    LawnApp *mApp;                        // 73
    int mState;                           // 74
    int mInt75;                           // 75
    int mInt76;                           // 76
    int mInt77;                           // 77
    int mInt78;                           // 78
    int unkInt79;                         // 79
    int mInt80;                           // 80
    int mInt81;                           // 81
    int mInt82;                           // 82
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

    void GameButtonDown(ButtonCode theButton, unsigned int thePlayerIndex, int a4) {
        reinterpret_cast<void (*)(VSSetupMenu *, ButtonCode, unsigned int, int)>(VSSetupMenu_GameButtonDownAddr)(this, theButton, thePlayerIndex, a4);
    }
    void SetSecondPlayerIndex(int thePlayerIndex) {
        reinterpret_cast<void (*)(VSSetupMenu *, int)>(VSSetupMenu_SetSecondPlayerIndexAddr)(this, thePlayerIndex);
    }
    void GoToState(int theState) {
        reinterpret_cast<void (*)(VSSetupMenu *, int)>(VSSetupMenu_GoToStateAddr)(this, theState);
    }
    void CloseVSSetup(bool close) {
        reinterpret_cast<void (*)(VSSetupMenu *, bool)>(VSSetupMenu_CloseVSSetupAddr)(this, close);
    }
    void PickRandomZombies(std::vector<SeedType> &theZombieSeeds) {
        reinterpret_cast<void (*)(VSSetupMenu *, std::vector<SeedType> &)>(VSSetupMenu_PickRandomZombiesAddr)(this, theZombieSeeds);
    }
    void PickRandomPlants(std::vector<SeedType> &thePlantSeeds, std::vector<SeedType> const &theZombieSeeds) {
        reinterpret_cast<void (*)(VSSetupMenu *, std::vector<SeedType> &, std::vector<SeedType> const &)>(VSSetupMenu_PickRandomPlantsAddr)(this, thePlantSeeds, theZombieSeeds);
    }

    VSSetupMenu() {
        _constructor();
    }
    ~VSSetupMenu() {
        _destructor();
    }
    void Draw(Sexy::Graphics *g);
    void Update();
    void KeyDown(Sexy::KeyCode theKey);
    void OnStateEnter(int theState);
    void ButtonPress(int theId);
    void ButtonDepress(int theId);

protected:
    friend void InitHookFunction();
    void _constructor();
    void _destructor();
};


inline void (*old_VSSetupMenu_Constructor)(VSSetupMenu *);

inline void (*old_VSSetupMenu_Destructor)(VSSetupMenu *);

inline void (*old_VSSetupMenu_Draw)(VSSetupMenu *, Sexy::Graphics *g);

inline void (*old_VSSetupMenu_Update)(VSSetupMenu *a);

inline void (*old_VSSetupMenu_KeyDown)(VSSetupMenu *a, Sexy::KeyCode a2);

inline void (*old_VSSetupMenu_OnStateEnter)(VSSetupMenu *menu, int theState);

inline void (*old_VSSetupMenu_ButtonPress)(VSSetupMenu *, int theId);

inline void (*old_VSSetupMenu_ButtonDepress)(VSSetupMenu *, int theId);

#endif // PVZ_LAWN_WIDGET_VS_SETUP_MENU_H
