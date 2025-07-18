#ifndef PVZ_LAWN_SEED_CHOOSER_SCREEN_H
#define PVZ_LAWN_SEED_CHOOSER_SCREEN_H

#include "PvZ/Lawn/Board/ToolTipWidget.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Lawn/GamepadControls.h"
#include "PvZ/SexyAppFramework/Widget/Widget.h"
#include "PvZ/SexyAppFramework/Widget/ButtonListener.h"
#include "PvZ/Symbols.h"

class Board;
class LawnApp;
class SeedBank;
namespace Sexy {
class GameButton;
}

enum SeedChooserTouchState {
    ViewLawnButton,
    SeedChooser,
    StoreButton,
    StartButton,
    AlmanacButton,
    SEEDCHOOSER_TOUCHSTATE_NONE
};

class ChosenSeed {
public:
    int mX;                     // 0
    int mY;                     // 1
    int mTimeStartMotion;       // 2
    int mTimeEndMotion;         // 3
    int mStartX;                // 4
    int mStartY;                // 5
    int mEndX;                  // 6
    int mEndY;                  // 7
    int mChosenPlayerIndex;     // 8
    SeedType mSeedType;         // 9
    ChosenSeedState mSeedState; // 10
    int mSeedIndexInBank;       // 11
    bool mRefreshing;           // 12
    int mRefreshCounter;        // 13
    SeedType mImitaterType;     // 14
    bool mCrazyDavePicked;      // 60
};

class SeedChooserScreen : public Sexy::Widget {
private:
    enum {
        SeedChooserScreen_Start = 100,
        SeedChooserScreen_Random = 101,
        SeedChooserScreen_ViewLawn = 102,
        SeedChooserScreen_Almanac = 103,
        SeedChooserScreen_Menu = 104,
        SeedChooserScreen_Store = 105,
        SeedChooserScreen_Imitater = 106
    };

public:
    Sexy::ButtonListener mButtonListener; // 64
    int unkMem1;                          // 65
    Sexy::GameButton *mImitaterButton;    // 66
    ChosenSeed mChosenSeeds[54];          // 67 ~ 930
    LawnApp *mApp;                        // 931
    Board *mBoard;                        // 932
    int mSeedChooserAge;                  // 933
    int mSeedsInFlight;                   // 934
    int mSeedsInBothBank;                 // 935
    int mSeedsIn1PBank;                   // 936
    int unkMem2;                          // 937
    ToolTipWidget *mToolTip1;             // 938
    ToolTipWidget *mToolTip2;             // 939
    int mToolTipWidgetState1;             // 940
    int mToolTipWidgetState2;             // 941
    int mCursorPositionX1;                // 942
    int mCursorPositionX2;                // 943
    int mCursorPositionY1;                // 944
    int mCursorPositionY2;                // 945
    SeedChooserState mChooseState;        // 946
    int mViewLawnTime;                    // 947
    bool unkBool;                         // 3792
    int mPlayerIndex;                     // 949
    SeedType mSeedType1;                  // 950
    SeedType mSeedType2;                  // 951
    float unkF;                           // 952
    bool mIsZombieChooser;                // 3812
    SeedBank *mSeedBank1;                 // 954
    SeedBank *mSeedBank2;                 // 955
    int unkCounter;                       // 956
    int mImitaterDialogOpened;            // 957
    Sexy::GameButton *mViewLawnButton;    // 958
    Sexy::GameButton *mStoreButton;       // 959
    Sexy::GameButton *mStartButton;       // 960
    Sexy::GameButton *mAlmanacButton;     // 961
    int unkMems3[4];                      // 962 ~ 965
    // 大小966个整数

    void CloseSeedChooser() { reinterpret_cast<void (*)(SeedChooserScreen *)>(SeedChooserScreen_CloseSeedChooserAddr)(this); }
    SeedType FindSeedInBank(int theIndexInBank, int thePlayerIndex) {
        return reinterpret_cast<SeedType (*)(SeedChooserScreen *, int, int)>(SeedChooserScreen_FindSeedInBankAddr)(this, theIndexInBank, thePlayerIndex);
    }
    bool HasPacket(SeedType theSeedType, bool theIsZombieChooser) {
        return reinterpret_cast<bool (*)(SeedChooserScreen *, SeedType, bool)>(SeedChooserScreen_HasPacketAddr)(this, theSeedType, theIsZombieChooser);
    }
    bool Has7Rows() { return reinterpret_cast<bool (*)(SeedChooserScreen *)>(SeedChooserScreen_Has7RowsAddr)(this); }
    bool CancelLawnView() { return reinterpret_cast<bool (*)(SeedChooserScreen *)>(SeedChooserScreen_CancelLawnViewAddr)(this); }
    void GetNextSeedInDir(int theNumSeed, int thePlayerIndex) { reinterpret_cast<void (*)(SeedChooserScreen *, int, int)>(SeedChooserScreen_GetNextSeedInDirAddr)(this, theNumSeed, thePlayerIndex); }
    void GetSeedPositionInChooser(SeedType theSeedType, int &x, int &y) {
        reinterpret_cast<void (*)(SeedChooserScreen *, SeedType, int &, int &)>(SeedChooserScreen_GetSeedPositionInChooserAddr)(this, theSeedType, x, y);
    }
    void UpdateImitaterButton() { reinterpret_cast<void (*)(SeedChooserScreen *)>(SeedChooserScreen_UpdateImitaterButtonAddr)(this); }
    SeedType SeedHitTest(int x, int y) { return reinterpret_cast<SeedType (*)(SeedChooserScreen *, int, int)>(SeedChooserScreen_SeedHitTestAddr)(this, x, y); }
    void LandFlyingSeed(ChosenSeed &theChosenSeed) { reinterpret_cast<void (*)(SeedChooserScreen *, ChosenSeed &)>(SeedChooserScreen_LandFlyingSeedAddr)(this, theChosenSeed); }

    SeedChooserScreen(bool theIsZombieChooser);
    void Create(bool theIsZombieChooser);
    void EnableStartButton(int theIsEnabled);
    void RebuildHelpbar();
    SeedType GetZombieSeedType(SeedType theSeedType);
    ZombieType GetZombieType(ZombieType theZombieType);
    void ClickedSeedInChooser(ChosenSeed *theChosenSeed, int thePlayerIndex);
    void CrazyDavePickSeeds();
    void OnStartButton();
    void Update();
    bool SeedNotAllowedToPick(SeedType theSeedType);
    void ClickedSeedInBank(ChosenSeed *theChosenSeed, unsigned int thePlayerIndex);
    void GameButtonDown(ButtonCode theButton, unsigned int thePlayerIndex);
    void DrawPacket(Sexy::Graphics *g, int x, int y, SeedType theSeedType, SeedType theImitaterType, float thePercentDark, int theGrayness, Sexy::Color *theColor, bool theDrawCost, bool theUseCurrentCost);
    void ButtonDepress(int theId);
    void GetSeedPositionInBank(int theIndex, int *x, int *y, int thePlayerIndex);
    void ShowToolTip(unsigned int thePlayerIndex);
    static SeedType GetZombieIndexBySeedType(SeedType theSeedType);

    void MouseMove(int x, int y);
    void MouseDown(int x, int y, int theClickCount);
    void MouseUp(int x, int y);
    void MouseDrag(int x, int y);
    void ButtonPress(int theId);
};

/***************************************************************************************************************/
inline bool daveNoPickSeeds;


inline void (*old_SeedChooserScreen_RebuildHelpbar)(SeedChooserScreen *instance);

inline void (*old_SeedChooserScreen_SeedChooserScreen)(SeedChooserScreen *seedChooserScreen, bool isZombieChooser);

inline void (*old_LawnApp_KillSeedChooserScreen)(LawnApp *lawnApp);

inline void (*old_SeedChooserScreen_Update)(SeedChooserScreen *a);

inline void (*old_SeedChooserScreen_EnableStartButton)(SeedChooserScreen *seedChooserScreen, int isEnabled);

inline void (*old_SeedChooserScreen_OnStartButton)(SeedChooserScreen *seedChooserScreen);

inline bool (*old_SeedChooserScreen_SeedNotAllowedToPick)(SeedChooserScreen *seedChooserScreen, SeedType theSeedType);

inline void (*old_SeedChooserScreen_ClickedSeedInChooser)(SeedChooserScreen *a1, ChosenSeed *a2, int a3);

inline void (*old_SeedChooserScreen_CrazyDavePickSeeds)(SeedChooserScreen *a);

inline void (*old_SeedChooserScreen_ClickedSeedInBank)(SeedChooserScreen *seedChooserScreen, ChosenSeed *theChosenSeed, unsigned int playerIndex);

inline void (*old_SeedChooserScreen_GameButtonDown)(SeedChooserScreen *a1, ButtonCode a2, unsigned int a3);

inline void (*old_SeedChooserScreen_ButtonDepress)(SeedChooserScreen *seedChooserScreen, int id);

inline void (*old_SeedChooserScreen_GetSeedPositionInBank)(SeedChooserScreen *seedChooserScreen, int theIndex, int *x, int *y, int playerIndex);

inline void (*old_SeedChooserScreen_ShowToolTip)(SeedChooserScreen *seedChooserScreen, unsigned int playerIndex);

inline void (*old_SeedChooserScreen_MouseMove)(SeedChooserScreen *seedChooserScreen, int x, int y);

inline void (*old_SeedChooserScreen_MouseDown)(SeedChooserScreen *a, int x, int y, int theClickCount);

inline void (*old_SeedChooserScreen_MouseDrag)(SeedChooserScreen *seedChooserScreen, int x, int y);

inline void (*old_SeedChooserScreen_MouseUp)(SeedChooserScreen *seedChooserScreen, int x, int y);

#endif // PVZ_LAWN_SEED_CHOOSER_SCREEN_H
