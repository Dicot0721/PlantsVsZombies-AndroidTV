#ifndef PVZ_LAWN_HELP_OPTIONS_DIALOG_H
#define PVZ_LAWN_HELP_OPTIONS_DIALOG_H

#include "LawnDialog.h"
#include "PvZ/Lawn/Common/ConstEnums.h"

class HelpOptionsDialog : public LawnDialog {
public:
    LawnApp *mApp;                       // 191
    Sexy::GameButton *mHowToPlayButton;  // 192
    Sexy::GameButton *mSwitchUserButton; // 193
    Sexy::GameButton *mSettingsButton;   // 194
    Sexy::GameButton *mCreditsButton;    // 195
    Sexy::GameButton *mBackButton;       // 196
    Sexy::GameButton *mUnkButton;        // 197
}; // 115: 198, 111: 200

inline void (*old_HelpOptionsDialog_ButtonDepress)(HelpOptionsDialog *a, int a2);

inline void (*old_HelpOptionsDialog_HelpOptionsDialog)(HelpOptionsDialog *a, LawnApp *a2);

inline void (*old_HelpOptionsDialog_Resize)(HelpOptionsDialog *a, int a2, int a3, int a4, int a5);


void HelpOptionsDialog_ButtonDepress(HelpOptionsDialog *a, int a2);

void HelpOptionsDialog_HelpOptionsDialog(HelpOptionsDialog *a, LawnApp *a2);

void HelpOptionsDialog_Resize(HelpOptionsDialog *a, int a2, int a3, int a4, int a5);

#endif // PVZ_LAWN_HELP_OPTIONS_DIALOG_H
