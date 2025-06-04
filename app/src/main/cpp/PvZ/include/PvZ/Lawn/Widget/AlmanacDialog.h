#ifndef PVZ_LAWN_ALMANAC_DIALOG_H
#define PVZ_LAWN_ALMANAC_DIALOG_H

#include "PvZ/SexyAppFramework/Widget/CustomScrollbarWidget.h"
#include "PvZ/Symbols.h"

#include "LawnDialog.h"

namespace Sexy {
class WidgetManager;
//class CustomScrollbarWidget;
}

class Plant;
class Zombie;

class AlmanacDialog : public LawnDialog {
public:
    int *mScrollListener;                           // 191
    LawnApp *mApp;                                  // 192
    Sexy::GameButton *mPlantButton;                 // 193
    Sexy::GameButton *mZombieButton;                // 194
    Sexy::CustomScrollbarWidget *mScrollTextView;   // 195
    AlmanacPage mOpenPage;                          // 196
    SeedType mSelectedSeed;                         // 197
    ZombieType mSelectedZombie;                     // 198
    Plant *mPlant;                                  // 199
    Zombie *mZombie;                                // 200
    Sexy::Rect mUnkRect;                            // 201 ~ 204
    Sexy::Rect mDescriptionRect;                    // 205 ~ 208
    Sexy::Rect mCostRect;                           // 209 ~ 212
    Sexy::Rect mWaitTimeRect;                       // 213 ~ 216
    int unk2[2];                                    // 217 ~ 218
    char *mNameString;                              // 219
    char *mDescriptionHeaderString;                 // 220
    char *mDescriptionStringTmp;                    // 221
    char *mDescriptionString;                       // 222
    char *mCostString;                              // 223
    char *mWaitTimeString;                          // 224
    DrawStringJustification mWaitTimeJustification; // 225
    DrawStringJustification mJustification;         // 226
    bool mSetupFinished;                            // 227 * 4
    double unk3;                                    // 228 ~ 229
    int *mHelpBarWidget;                            // 230
    int unk4[11];                                   // 231 ~ 233
    // 115: 234, 111: 236

    void KeyDown(Sexy::KeyCode theKey) { reinterpret_cast<void (*)(AlmanacDialog *, Sexy::KeyCode)>(AlmanacDialog_KeyDownAddr)(this, theKey); }
    SeedType SeedHitTest(int x, int y) { return reinterpret_cast<SeedType (*)(AlmanacDialog *, int, int)>(AlmanacDialog_SeedHitTestAddr)(this, x, y); }
    ZombieType ZombieHitTest(int x, int y) { return reinterpret_cast<ZombieType (*)(AlmanacDialog *, int, int)>(AlmanacDialog_ZombieHitTestAddr)(this, x, y); }
    void SetupPlant() { reinterpret_cast<void (*)(AlmanacDialog *)>(AlmanacDialog_SetupPlantAddr)(this); }
    void SetupZombie() { reinterpret_cast<void (*)(AlmanacDialog *)>(AlmanacDialog_SetupZombieAddr)(this); }
    void GetSeedPosition(SeedType theSeedType, int& x, int& y) { reinterpret_cast<void (*)(AlmanacDialog *, SeedType, int &, int &)>(AlmanacDialog_GetSeedPositionAddr)(this, theSeedType, x, y); }

    AlmanacDialog(LawnApp *theApp);
    void Create( LawnApp *theApp);
    ~AlmanacDialog();
    void Delete2();
    void SetPage(AlmanacPage thePage);
    void RemovedFromManager(Sexy::WidgetManager *theWidgetManager);
    void ButtonDepress(int theId);
    void DrawPlants_Unmodified(Sexy::Graphics *g);
    void DrawPlants(Sexy::Graphics *g);
    void SetupLayoutPlants(Sexy::Graphics *g);

    void MouseDown(int x, int y, int theClickCount);
    void MouseDrag(int x, int y);
    void MouseUp(int x, int y, int theClickCount);
};


inline void (*old_AlmanacDialog_AlmanacDialog)(AlmanacDialog *almanacDialog, LawnApp *lawnApp);

inline void (*old_AlmanacDialog_SetPage)(AlmanacDialog *almanacDialog, AlmanacPage thePage);

inline void (*old_AlmanacDialog_MouseDrag)(AlmanacDialog *almanacDialog, int, int);

inline void (*old_AlmanacDialog_RemovedFromManager)(AlmanacDialog *almanacDialog, Sexy::WidgetManager *manager);

inline void (*old_AlmanacDialog_Delete2)(AlmanacDialog *almanacDialog);

inline void (*old_AlmanacDialog_DrawPlants)(AlmanacDialog *almanacDialog, Sexy::Graphics *graphics);

inline void (*old_AlmanacDialog_SetupLayoutPlants)(AlmanacDialog *almanacDialog, Sexy::Graphics *graphics);

#endif // PVZ_LAWN_ALMANAC_DIALOG_H
