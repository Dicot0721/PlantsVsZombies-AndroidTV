#ifndef PVZ_LAWN_VS_SETUP_MENU_H
#define PVZ_LAWN_VS_SETUP_MENU_H

#include "PvZ/SexyAppFramework/Widget/Widget.h"
#include "PvZ/Symbols.h"

class LawnApp;

class VSSetupMenu : public Sexy::Widget {
public:
    int unkInt[6];      // 64 ~ 69
    int mInt70;         // 70
    int mInt71;         // 71
    int mInt72;         // 72
    LawnApp *mAppBase;  // 73
    int mState;         // 74
    int mInt75;         // 75
    LawnApp *mApp;      // 76
    int mInt77;         // 77
    int mInt78;         // 78
    int unkInt79;       // 79
    int mInt80;         // 80
    int mInt81;         // 81
    int mInt82;         // 82
    int unkInt83[85];   // 83 ~ 167
    int mInt168;        // 168
    int unkInt169[59];  // 169 ~ 227
    int mInt228;        // 228
    int unkInt229[63];  // 229 ~ 291
    int mInt292;        // 292
    int unkInt293[3];   // 293 ~ 295
    int mInt296;        // 296
    int unkInt297[7];   // 297 ~ 303
    int mInt304;        // 304
    int unkInt305[11];  // 305 ~ 315
    bool mBool316;      // 316
    bool mBool317;      // 317
    int unkInt318[2];   // 318 ~ 319
    int mInt320;        // 320
    int unkInt321[3];   // 321 ~ 323
    int mInt324;        // 324
    int unkInt325[3];   // 325 ~ 327
    int mInt328;        // 328
    int unkInt329[3];   // 329 ~ 331
    int mInt332;        // 332
    int unkInt333[28];  // 333 ~ 360
    bool mBoo361;       // 361
    bool mBool362;      // 362
    int unkInt363[161]; // 363 ~ 523
    int mInt524;        // 524

    void GameButtonDown(int theButton, unsigned int thePlayerIndex, int a4) {
        reinterpret_cast<void (*)(VSSetupMenu *, int, unsigned int, int)>(VSSetupMenu_GameButtonDownAddr)(this, theButton, thePlayerIndex, a4);
    }

    void Update();
    void KeyDown(Sexy::KeyCode theKey);
};


inline void (*old_VSSetupMenu_Update)(VSSetupMenu *a);

inline void (*old_VSSetupMenu_KeyDown)(VSSetupMenu *a, Sexy::KeyCode a2);


#endif // PVZ_LAWN_VS_SETUP_MENU_H
