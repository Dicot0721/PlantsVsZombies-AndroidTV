#include "PvZ/Lawn/Widget/HouseChooserDialog.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Misc.h"
#include "PvZ/Symbols.h"

bool HouseChooserDialog_IsHouseAvaliable(HouseType houseType) {
    LawnApp *lawnApp = (LawnApp *)*gLawnApp_Addr;
    return houseType == 0 || lawnApp->mPlayerInfo->mPurchases[StoreItem::STORE_ITEM_BLUEPRINT_BLING + houseType] > 0;
}

void HouseChooserDialog_MouseDown(HouseChooserDialog *a, int x, int y, int theCount) {
    old_HouseChooserDialog_MouseDown(a, x, y, theCount);
    int width = (*Sexy_IMAGE_STORE_BLUEPRINT_CLOWN_Addr)->GetWidth();
    int height = (*Sexy_IMAGE_STORE_BLUEPRINT_CLOWN_Addr)->GetHeight();
    int houseIdToSelect = HouseType::BLUEPRINT_INVALID;
    for (int i = 0; i < 5; ++i) {
        Sexy::Rect rect = {75 + 90 * i, 125, width, height};
        if (TRect_Contains(&rect, x, y) && HouseChooserDialog_IsHouseAvaliable((HouseType)i)) {
            houseIdToSelect = i;
            break;
        }
    }
    if (houseIdToSelect == HouseType::BLUEPRINT_INVALID)
        return;
    int currentHouseType = a->mSelectedHouseType;
    if (currentHouseType == houseIdToSelect) {
        HouseChooserDialog_GameButtonDown(a, 6, 0, 0);
    } else {
        while (a->mSelectedHouseType != houseIdToSelect) {
            HouseChooserDialog_GameButtonDown(a, 2, 0, 0);
        }
    }
}

void HouseChooserDialog_KeyDown(HouseChooserDialog *a, int keyCode) {
    old_HouseChooserDialog_KeyDown(a, keyCode);
    switch (keyCode) {
        case 37:
            return HouseChooserDialog_GameButtonDown(a, 2, 0, 0);
        case 38:
            return HouseChooserDialog_GameButtonDown(a, 0, 0, 0);
        case 39:
            return HouseChooserDialog_GameButtonDown(a, 3, 0, 0);
        case 40:
            return HouseChooserDialog_GameButtonDown(a, 1, 0, 0);
        case 27:
        case 305:
            return HouseChooserDialog_GameButtonDown(a, 7, 0, 0);
        case 13:
        case 304:
            return HouseChooserDialog_GameButtonDown(a, 6, 0, 0);
    }
}
