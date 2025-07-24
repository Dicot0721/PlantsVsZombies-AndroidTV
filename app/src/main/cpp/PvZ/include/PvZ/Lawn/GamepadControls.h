#ifndef PVZ_LAWN_GAMEPAD_CONTROLS_H
#define PVZ_LAWN_GAMEPAD_CONTROLS_H

#include "BaseGamepadControls.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
//#include "PvZ/TodLib/Effect/FilterEffect.h"
#include "PvZ/SexyAppFramework/Misc/KeyCodes.h"

// buttonCode 是按键键值，对应关系如下：
enum ButtonCode {
    BUTTONCODE_LUP = 0,      // 左摇杆上 0
    BUTTONCODE_LDOWN = 1,    // 左摇杆下 1
    BUTTONCODE_LLEFT = 2,    // 左摇杆左 2
    BUTTONCODE_LRIGHT = 3,   // 左摇杆右 3
    BUTTONCODE_UNKNOWN = 4,  // 未知键 4
    BUTTONCODE_PAUSE = 5,    // 暂停键 5
    BUTTONCODE_A = 6,        // A 6
    BUTTONCODE_B = 7,        // B 7
    BUTTONCODE_X = 8,        // X 8
    BUTTONCODE_Y = 9,        // Y 9
    BUTTONCODE_L1 = 10,      // L1 10
    BUTTONCODE_R1 = 11,      // R1 11
    BUTTONCODE_L2 = 12,      // L2 12
    BUTTONCODE_R2 = 13,      // R2 13
    BUTTONCODE_TL = 14,      // TL 14
    BUTTONCODE_TR = 15,      // TR 15
    BUTTONCODE_UP = 16,      // 上 16
    BUTTONCODE_DOWN = 17,    // 下 17
    BUTTONCODE_LEFT = 18,    // 左 18
    BUTTONCODE_RIGHT = 19    // 右 19
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

    SeedBank *GetSeedBank() { return reinterpret_cast<SeedBank *(*)(GamepadControls *)>(GamepadControls_GetSeedBankAddr)(this); }
    void OnButtonDown(ButtonCode theButton, int theGamepadIndex, unsigned int a4) {
        reinterpret_cast<void (*)(GamepadControls *, ButtonCode, int, unsigned int)>(GamepadControls_OnButtonDownAddr)(this, theButton, theGamepadIndex, a4);
    }
    void OnButtonUp(ButtonCode theButton, int theGamepadIndex, unsigned int a4) {
        reinterpret_cast<void (*)(GamepadControls *, ButtonCode, int, unsigned int)>(GamepadControls_OnButtonUpAddr)(this, theButton, theGamepadIndex, a4);
    }
    // theGamepadIndex 根据手柄决定是0还是1
    // a4 恒定为0
    void OnKeyDown(Sexy::KeyCode theKey, unsigned int a3) { reinterpret_cast<void (*)(GamepadControls *, int, unsigned int)>(GamepadControls_OnKeyDownAddr)(this, theKey, a3); }
    // 确定 13 1096
    // 返回 27 1096
    // 左 37 1096
    // 上 38 1096
    // 右 39 1096
    // 下 40 1096
    // 铲子 49 1112
    // 锤子 50 1112

    GamepadControls(Board *theBoard, int thePlayerIndex1, int thePlayerIndex2);
    void __Constructor(Board *theBoard, int thePlayerIndex1, int thePlayerIndex2);
    void ButtonDownFireCobcannonTest();
    void Draw(Sexy::Graphics *g);
    void Update(float a2);
    void DrawPreview(Sexy::Graphics *g);
    void UpdatePreviewReanim();
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

inline void (*old_ZenGardenControls_Update)(ZenGardenControls *a1, float a2);

//FilterEffect GetFilterEffectTypeBySeedType(SeedType mSeedType);

void GamepadControls_pickUpCobCannon(int, int);

#endif // PVZ_LAWN_GAMEPAD_CONTROLS_H
