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

#ifndef PVZ_LAWN_GAMEPAD_CONTROLS_H
#define PVZ_LAWN_GAMEPAD_CONTROLS_H

#include "BaseGamepadControls.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/SexyAppFramework/Misc/KeyCodes.h"

enum GamepadButton {
    GAMEPAD_BUTTON_NONE = -1,
    GAMEPAD_BUTTON_UP = 0,
    GAMEPAD_BUTTON_DOWN = 1,
    GAMEPAD_BUTTON_LEFT = 2,
    GAMEPAD_BUTTON_RIGHT = 3,
    GAMEPAD_BUTTON_SELECT = 4, // GAMEPAD_BUTTON_Z
    GAMEPAD_BUTTON_START = 5,  // GAMEPAD_BUTTON_C
    GAMEPAD_BUTTON_A = 6,
    GAMEPAD_BUTTON_B = 7,
    GAMEPAD_BUTTON_X = 8,
    GAMEPAD_BUTTON_Y = 9,
    GAMEPAD_BUTTON_TL = 10,
    GAMEPAD_BUTTON_TR = 11,
    GAMEPAD_BUTTON_TL2 = 12,
    GAMEPAD_BUTTON_TR2 = 13,
    GAMEPAD_BUTTON_THUMBL = 14,
    GAMEPAD_BUTTON_THUMBR = 15,
    GAMEPAD_BUTTON_DPAD_UP = 16,
    GAMEPAD_BUTTON_DPAD_DOWN = 17,
    GAMEPAD_BUTTON_DPAD_LEFT = 18,
    GAMEPAD_BUTTON_DPAD_RIGHT = 19,
};

class Zombie;

class GamepadControls : public BaseGamepadControls {
public:
    float mRangeFrom0to30UpdateFresh;  // 43
    int *mSelectorParticle;            // 44
    int mSelectedSeedIndex;            // 45
    SeedType mSelectedSeedType;        // 46
    bool mIsZombie;                    // 188
    bool mCanPickUp;                   // 189
    int mSelectedUpgradableType;       // 48
    int mCobCannonPlantIndexInList;    // 49
    bool mIsCobCannonSelected;         // 200
    float mUpdateAdd_a2_Or_Minus_2xa2; // 51
    ReanimationID mPreviewReanimID1;   // 52
    ReanimationID mCursorReanimID;     // 53
    ReanimationID mPreviewReanimID3;   // 54
    int mUnkCobCannonRelated;          // 55
    ReanimationID mPreviewReanimID4;   // 56
    SeedType mPreviewingSeedType;      // 57
    Sexy::Image *mPreviewImage;        // 58
    Zombie *mButterZombie;             // 59
    int mShovelRelated2;               // 60
    bool mIsShowingDigIndicator;       // 244
    bool mIsInShopSeedBank;            // 245
    int mSelectedShopSeedIndex;        // 62
    int unk4[3];                       // 63 ~ 65
    // 大小66个整数

    SeedBank *GetSeedBank() {
        return reinterpret_cast<SeedBank *(*)(GamepadControls *)>(GamepadControls_GetSeedBankAddr)(this);
    }
    void OnButtonUp(GamepadButton theButton, int theGamepadIndex, unsigned int a4) {
        reinterpret_cast<void (*)(GamepadControls *, GamepadButton, int, unsigned int)>(GamepadControls_OnButtonUpAddr)(this, theButton, theGamepadIndex, a4);
    }
    // theGamepadIndex 根据手柄决定是0还是1
    // a4 恒定为0
    void OnKeyDown(Sexy::KeyCode theKey, unsigned int a3) {
        reinterpret_cast<void (*)(GamepadControls *, int, unsigned int)>(GamepadControls_OnKeyDownAddr)(this, theKey, a3);
    }
    // 确定 13 1096
    // 返回 27 1096
    // 左 37 1096
    // 上 38 1096
    // 右 39 1096
    // 下 40 1096
    // 铲子 49 1112
    // 锤子 50 1112

    // GamepadControls(Board *theBoard, int thePlayerIndex1, int thePlayerIndex2);
    void ButtonDownFireCobcannonTest();
    void InvalidatePreviewReanim();
    void Draw(Sexy::Graphics *g);
    void Update(float a2);
    void DrawPreview(Sexy::Graphics *g);
    void UpdatePreviewReanim();
    void OnButtonDown(GamepadButton theButton, int thePlayerIndex, unsigned int unk);

    GamepadControls() = delete;
    ~GamepadControls() = delete;

protected:
    friend void InitHookFunction();

    void _constructor(Board *theBoard, int thePlayerIndex1, int thePlayerIndex2);
};

class ZenGardenControls : public GamepadControls {
public:
    GameObjectType mObjectType; // 66
    // 大小67个整数

    void Update(float a2);
};

/***************************************************************************************************************/


inline void (*old_GamepadControls_Draw)(GamepadControls *gamePad, Sexy::Graphics *graphics);

inline void (*old_GamepadControls_Update)(GamepadControls *gamepadControls, float a2);

inline void (*old_GamepadControls_GamepadControls)(GamepadControls *gamePad, Board *board, int a3, int a4);

inline void (*old_GamepadControls_ButtonDownFireCobcannonTest)(GamepadControls *gamepadControls);

inline void (*old_GamepadControls_UpdatePreviewReanim)(GamepadControls *gamePad);

inline void (*old_GamepadControls_DrawPreview)(GamepadControls *gamePad, Sexy::Graphics *graphics);

inline void (*old_GamepadControls_OnButtonDown)(GamepadControls *, GamepadButton theButton, int thePlayerIndex, unsigned int unk);

inline void (*old_ZenGardenControls_Update)(ZenGardenControls *a1, float a2);

// FilterEffect GetFilterEffectTypeBySeedType(SeedType mSeedType);

void GamepadControls_pickUpCobCannon(int, int);

#endif // PVZ_LAWN_GAMEPAD_CONTROLS_H
