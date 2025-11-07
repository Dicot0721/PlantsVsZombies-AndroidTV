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

#ifndef PVZ_LAWN_WIDGET_STORE_SCREEN_H
#define PVZ_LAWN_WIDGET_STORE_SCREEN_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/SexyAppFramework/Widget/Dialog.h"
#include "PvZ/Symbols.h"

enum StoreScreenTouchState { Prev, Next, Back, None };

class StoreScreen : public Sexy::Dialog {
private:
    enum { StoreScreen_Back = 100, StoreScreen_Prev = 101, StoreScreen_Next = 102 };

public:
    LawnApp *mApp;                    // 184
    int unk185[3];                    // 185 ~ 187
    int mBubbleCountDown;             // 188
    int unk189[3];                    // 189 ~ 191
    StorePages mPage;                 // 192
    int unk193;                       // 193
    StoreItem mSelectedStoreItemType; // 194
    int mHatchTimer;                  // 195
    int unk196;                       // 196
    int unk197;                       // 197
    int mShakeX;                      // 198
    int mShakeY;                      // 199
    int unk200[556];                  // 200 ~ 755
    bool mBubbleClickToContinue;      // 756


    int SetSelectedSlot(int theIndex) {
        return reinterpret_cast<int (*)(StoreScreen *, int)>(StoreScreen_SetSelectedSlotAddr)(this, theIndex);
    }
    void EnableButtons(bool theEnable) {
        reinterpret_cast<void (*)(StoreScreen *, bool)>(StoreScreen_EnableButtonsAddr)(this, theEnable);
    }
    bool IsPageShown(StorePages thePage) {
        return reinterpret_cast<bool (*)(StoreScreen *, StorePages)>(StoreScreen_IsPageShownAddr)(this, thePage);
    }
    void AdvanceCrazyDaveDialog() {
        reinterpret_cast<void (*)(StoreScreen *)>(StoreScreen_AdvanceCrazyDaveDialogAddr)(this);
    }
    bool CanInteractWithButtons() {
        return reinterpret_cast<bool (*)(StoreScreen *)>(StoreScreen_CanInteractWithButtonsAddr)(this);
    }
    StoreItem GetStoreItemType(int theSpotIndex) {
        return reinterpret_cast<StoreItem (*)(StoreScreen *, int)>(StoreScreen_GetStoreItemTypeAddr)(this, theSpotIndex);
    }
    void GetStoreItemInfo(int theSpotIndex, StoreItem theStoreItem, Sexy::Image *theImage, int &theX, int &theY, int &theCount) {
        reinterpret_cast<void (*)(StoreScreen *, int, StoreItem, Sexy::Image *, int &, int &, int &)>(StoreScreen_GetStoreItemInfoAddr)(
            this, theSpotIndex, theStoreItem, theImage, theX, theY, theCount);
    }
    bool IsItemSoldOut(StoreItem theStoreItem) {
        return reinterpret_cast<bool (*)(StoreScreen *, StoreItem)>(StoreScreen_IsItemSoldOutAddr)(this, theStoreItem);
    }
    bool IsItemUnavailable(StoreItem theStoreItem) {
        return reinterpret_cast<bool (*)(StoreScreen *, StoreItem)>(StoreScreen_IsItemUnavailableAddr)(this, theStoreItem);
    }
    bool IsComingSoon(StoreItem theStoreItem) {
        return reinterpret_cast<bool (*)(StoreScreen *, StoreItem)>(StoreScreen_IsComingSoonAddr)(this, theStoreItem);
    }


    void AddedToManager(int a2);
    void RemovedFromManager(int a2);
    void Update();
    void SetupPage();
    void DrawItem(Sexy::Graphics *g, int a3, StoreItem theStoreItem);
    void ButtonDepress(int buttonId);
    void PurchaseItem(StoreItem item);
    void Draw(Sexy::Graphics *g);
    bool IsPottedPlant(StoreItem theStoreItem);

    void MouseDown(int x, int y, int theClickCount);
    void MouseUp(int x, int y, int theClickCount);
};


inline void (*old_StoreScreen_AddedToManager)(StoreScreen *a, int a2);

inline void (*old_StoreScreen_RemovedFromManager)(StoreScreen *a, int a2);

inline void (*old_StoreScreen_Update)(StoreScreen *a);

inline void (*old_StoreScreen_SetupPage)(StoreScreen *a);

inline void (*old_StoreScreen_DrawItem)(StoreScreen *a1, Sexy::Graphics *a2, int a3, StoreItem item);

inline void (*old_StoreScreen_ButtonDepress)(StoreScreen *, int);

inline void (*old_StoreScreen_PurchaseItem)(StoreScreen *, StoreItem);

inline void (*old_StoreScreen_Draw)(StoreScreen *storeScreen, Sexy::Graphics *a2);

inline void (*old_StoreScreen_MouseDown)(StoreScreen *storeScreen, int x, int y, int theClickCount);

inline void (*old_StoreScreen_MouseUp)(StoreScreen *storeScreen, int x, int y, int theClickCount);

#endif // PVZ_LAWN_WIDGET_STORE_SCREEN_H
