#ifndef PVZ_MISC_H
#define PVZ_MISC_H

#include "PvZ/Lawn/Board/MessageWidget.h"
#include "PvZ/Lawn/System/PlayerInfo.h"
#include "PvZ/Lawn/System/ReanimationLawn.h"
#include "PvZ/Lawn/Widget/AchievementsWidget.h"
#include "PvZ/Lawn/Widget/ZombatarWidget.h"
#include "PvZ/SexyAppFramework/Widget/Checkbox.h"
#include "PvZ/TodLib/Common/TodFoley.h"
//#include "PvZ/TodLib/Common/TodList.h"
//#include "TodLib/Common/TodCommon.h"

inline void (*MaskHelpWidget_MaskHelpWidget)(AchievementsWidget *, LawnApp *);

inline void (*MaskHelpWidget_Delete)(AchievementsWidget *);

inline void DrawCheckboxText(Sexy::Graphics *g, const pvzstl::string &theText1, Sexy::Checkbox *theCheckBox, const char *theText2) {
    reinterpret_cast<void (*)(Sexy::Graphics *, const pvzstl::string &, Sexy::Checkbox *, const char *)>(DrawCheckboxTextAddr)(g, theText1, theCheckBox, theText2);
}

inline void (*CreditScreen_PauseCredits)(Sexy::Widget *);

inline int (*Mailbox_GetNumUnseenMessages)(int *);


inline int (*old_SexyDialog_AddedToManager)(void *instance, void *instance1);

inline int (*old_SexyDialog_RemovedFromManager)(void *instance, void *instance1);

inline void (*old_HelpBarWidget_HelpBarWidget)(Sexy::Widget *a);

inline void (*old_Sexy_ExtractLoadingSoundsResources)(int *a, int *theManager);

inline FoleyParams *(*old_LookupFoley)(FoleyType type);


int randomInt(int a, int b);

void DrawSeedType(Sexy::Graphics *g, float x, float y, SeedType theSeedType, SeedType theImitaterType, float xOffset, float yOffset, float scale);

int SexyDialog_AddedToManager(void *instance, void *instance1);

int SexyDialog_RemovedFromManager(void *instance, void *instance1);

FoleyParams *LookupFoley(FoleyType theFoleyType);

void CreditScreen_ButtonDepress(Sexy::Widget *creditScreen, int id);

void Sexy_ExtractLoadingSoundsResources(int *a, int *theManager);

void WaitForSecondPlayerDialog_WaitForSecondPlayerDialog(int *a, int *a2);

void HelpBarWidget_HelpBarWidget(Sexy::Widget *a);


#endif // PVZ_MISC_H
