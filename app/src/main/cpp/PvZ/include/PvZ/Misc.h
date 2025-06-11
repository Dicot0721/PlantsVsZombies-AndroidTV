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

class TodParticleSystem;

inline int (*WaitForSecondPlayerDialog_GameButtonDown)(int *a1, int a2, int a3);

inline void (*TodStringTranslate)(int *holder, const char *theText);

inline int (*LawnPlayerInfo_GetFlag)(PlayerInfo *, int);

inline void (*GetFlashingColor)(Sexy::Color *, int, int);

inline bool (*Sexy_GamepadApp_HasGamepad)(LawnApp *);

inline void (*ShopSeedPacket_Update)(int *seedPacket);

inline void (*TypingCheck_SetPhrase)(int *a1, int *a2);

inline int **(*ReanimAtlas_GetEncodedReanimAtlas)(int *, int *);

inline int *(*Sexy_ResourceManager_GetSoundThrow)(int *, int *);

inline bool (*TodFoley_IsFoleyPlaying)(TodFoley *soundSystem, FoleyType theFoleyType);

inline void (*TodFoley_StopFoley)(TodFoley *soundSystem, FoleyType theFoleyType);

inline FoleyInstance *(*SoundSystemFindInstance)(TodFoley *, FoleyType);

inline int (*Sexy_AudiereSoundManager_LoadSound)(int *mSoundManager, int *theFileName);

inline void (*TodStringListLoad)(const char *);

inline void (*TodReplaceString)(int *, int *, const char *, int *);

inline void (*TodReplaceNumberString)(int *, int *, const char *, int);

inline void (*MaskHelpWidget_MaskHelpWidget)(AchievementsWidget *, LawnApp *);

inline void (*MaskHelpWidget_Delete)(AchievementsWidget *);

inline void (*TodParticleSystem_OverrideColor)(TodParticleSystem *, char *theEmitterName, Sexy::Color *theColor);

inline void (*TodParticleSystem_OverrideImage)(TodParticleSystem *, char *theEmitterName, Sexy::Image *);

inline void (*TodParticleSystem_Delete2)(TodParticleSystem *);

inline void (*TodParticleSystem_ParticleSystemDie)(TodParticleSystem *);

inline PlayerInfo *(*Sexy_DefaultProfileMgr_GetAnyProfile)(int *);

inline void (*DrawCheckboxText)(Sexy::Graphics *graphics, int *text1, Sexy::Checkbox *a3, const char *text2);

inline void (*Sexy_Checkbox_Delete)(Sexy::Checkbox *);

inline void (*CreditScreen_PauseCredits)(Sexy::Widget *);

inline void (*ReanimatorEnsureDefinitionLoaded)(ReanimationType type, bool b);

inline int (*Mailbox_GetNumUnseenMessages)(int *);

inline TodAllocator *(*FindGlobalAllocator)(int aSize);

inline void (*AttachParticle)(AttachmentID *, TodParticleSystem *, float, float);

inline void (*TodLoadResources)(int *);

inline void (*TodParticleSystem_Draw)(TodParticleSystem *, Sexy::Graphics *);

inline void (*ToolTipWidget_SetWarningText)(int *, int *);

inline void (*TestMenuWidget_TestMenuWidget)(ZombatarWidget *);

inline void (*SaveGameContext_SyncInt)(int *, int *);

inline void (*old_WaitForSecondPlayerDialog_WaitForSecondPlayerDialog)(int *a, int *a2);

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

void LawnPlayerInfo_AddCoins(PlayerInfo *playerInfo, int theAmount);

void SaveGameContext_SyncReanimationDef(int *theSaveGameContext, ReanimatorDefinition **a2);


#endif // PVZ_MISC_H
