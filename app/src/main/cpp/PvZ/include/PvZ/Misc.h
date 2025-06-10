#ifndef PVZ_MISC_H
#define PVZ_MISC_H

#include "PvZ/Lawn/Board/GameObject.h"
#include "PvZ/Lawn/Board/MessageWidget.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Lawn/System/PlayerInfo.h"
#include "PvZ/Lawn/System/ReanimationLawn.h"
#include "PvZ/Lawn/Widget/AchievementsWidget.h"
#include "PvZ/Lawn/Widget/ZombatarWidget.h"
#include "PvZ/SexyAppFramework/Misc/SexyMatrix.h"
#include "PvZ/SexyAppFramework/SexyAppBase.h"
#include "PvZ/SexyAppFramework/Widget/Checkbox.h"
#include "PvZ/TodLib/Common/TodFoley.h"
#include "PvZ/TodLib/Common/TodList.h"
#include "TodLib/Common/TodCommon.h"

class TodParticleSystem;

inline int (*WaitForSecondPlayerDialog_GameButtonDown)(int *a1, int a2, int a3);

inline void (*Sexy_StrFormat)(int *holder, const char *theFormat, ...);

inline void (*TodStringTranslate)(int *holder, const char *theText);

inline int (*LawnPlayerInfo_GetFlag)(PlayerInfo *, int);

inline void (*GetFlashingColor)(Sexy::Color *, int, int);

inline bool (*Sexy_GamepadApp_HasGamepad)(LawnApp *);

inline void (*ShopSeedPacket_Update)(int *seedPacket);

inline void (*TypingCheck_SetPhrase)(int *a1, int *a2);

inline void (*Sexy_MemoryImage_MemoryImage)(Sexy::Image *);

inline unsigned int *(*Sexy_MemoryImage_GetBits)(Sexy::Image *);

inline void (*Sexy_MemoryImage_Create)(Sexy::Image *, int x, int y);

inline void (*Sexy_MemoryImage_SetImageMode)(Sexy::Image *, bool, bool);

inline void (*Sexy_MemoryImage_WriteToPng)(Sexy::Image *, int *);

inline void (*Sexy_MemoryImage_WriteToJPEG)(Sexy::Image *, int *);

inline void (*Sexy_MemoryImage_Delete)(Sexy::Image *);

inline void (*Sexy_MemoryImage_BitsChanged)(Sexy::Image *);

inline void (*FilterEffectDoWashedOut)(Sexy::Image *);

inline void (*FilterEffectDoLessWashedOut)(Sexy::Image *);

inline void (*FilterEffectDoWhite)(Sexy::Image *);

inline void (*FilterEffectDoLumSat)(Sexy::Image *theImage, float aLum, float aSat);

inline int **(*ReanimAtlas_GetEncodedReanimAtlas)(int *, int *);

inline int *(*Sexy_ResourceManager_GetSoundThrow)(int *, int *);

inline bool (*TodFoley_IsFoleyPlaying)(TodFoley *soundSystem, FoleyType theFoleyType);

inline void (*TodFoley_StopFoley)(TodFoley *soundSystem, FoleyType theFoleyType);

inline FoleyInstance *(*SoundSystemFindInstance)(TodFoley *, FoleyType);

inline int (*Sexy_AudiereSoundManager_LoadSound)(int *mSoundManager, int *theFileName);

inline void (*MessageWidget_MessageWidget)(MessageWidget *messageWidget, LawnApp *a2);

inline void (*MessageWidget_Delete)(MessageWidget *messageWidget);

inline Sexy::Font *(*MessageWidget_GetFont)(MessageWidget *messageWidget);

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

inline void (*old_Sexy_Image_PushTransform)(Sexy::Image *image, int *transform, bool concatenate);

inline void (*old_Sexy_Image_PopTransform)(Sexy::Image *image);

inline void (*old_Sexy_GLImage_PushTransform)(Sexy::Image *image, int *transform, bool concatenate);

inline void (*old_Sexy_GLImage_PopTransform)(Sexy::Image *image);

inline void (*old_Sexy_MemoryImage_PushTransform)(Sexy::Image *image, int *transform, bool concatenate);

inline void (*old_Sexy_MemoryImage_PopTransform)(Sexy::Image *image);

inline int (*old_SexyDialog_AddedToManager)(void *instance, void *instance1);

inline int (*old_SexyDialog_RemovedFromManager)(void *instance, void *instance1);

inline void (*old_LawnMower_Update)(int *lawnMover);

inline void (*old_HelpBarWidget_HelpBarWidget)(Sexy::Widget *a);

inline void (*old_Sexy_ExtractLoadingSoundsResources)(int *a, int *theManager);

inline FoleyParams *(*old_LookupFoley)(FoleyType type);

inline void (*old_MessageWidget_Draw)(CustomMessageWidget *messageWidget, Sexy::Graphics *a2);

inline void (*old_MessageWidget_ClearLabel)(CustomMessageWidget *messageWidget);

inline void (*old_MessageWidget_SetLabel)(CustomMessageWidget *messageWidget, int *label, MessageStyle theStyle);

inline void (*old_MessageWidget_Update)(CustomMessageWidget *messageWidget);


int randomInt(int a, int b);

void DrawSeedType(Sexy::Graphics *g, float x, float y, SeedType theSeedType, SeedType theImitaterType, float xOffset, float yOffset, float scale);

void LawnMower_Update(int *lawnMover);

void Sexy_String_Delete(int *holder);

void Sexy_Image_PushTransform(Sexy::Image *image, int *transform, bool concatenate);

void Sexy_Image_PopTransform(Sexy::Image *image);

void Sexy_GLImage_PushTransform(Sexy::Image *image, int *transform, bool concatenate);

void Sexy_GLImage_PopTransform(Sexy::Image *image);

void Sexy_MemoryImage_PushTransform(Sexy::Image *image, int *transform, bool concatenate);

void Sexy_MemoryImage_PopTransform(Sexy::Image *image);

void Sexy_MemoryImage_ClearRect(Sexy::Image *a1, Sexy::Rect *a2);

int SexyDialog_AddedToManager(void *instance, void *instance1);

int SexyDialog_RemovedFromManager(void *instance, void *instance1);

FoleyParams *LookupFoley(FoleyType theFoleyType);

void CreditScreen_ButtonDepress(Sexy::Widget *creditScreen, int id);

void MessageWidget_ClearLabel(CustomMessageWidget *messageWidget);

void MessageWidget_SetLabel(CustomMessageWidget *messageWidget, int *label, MessageStyle theStyle);

void MessageWidget_Update(CustomMessageWidget *messageWidget);

void MessageWidget_Draw(CustomMessageWidget *messageWidget, Sexy::Graphics *graphics);

void Sexy_ExtractLoadingSoundsResources(int *a, int *theManager);

void WaitForSecondPlayerDialog_WaitForSecondPlayerDialog(int *a, int *a2);

void HelpBarWidget_HelpBarWidget(Sexy::Widget *a);

void LawnPlayerInfo_AddCoins(PlayerInfo *playerInfo, int theAmount);

void SaveGameContext_SyncReanimationDef(int *theSaveGameContext, ReanimatorDefinition **a2);


#endif // PVZ_MISC_H
