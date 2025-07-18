#include "PvZ/Lawn/Widget/HouseChooserDialog.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Misc.h"
#include "PvZ/Symbols.h"

bool HouseChooserDialog::IsHouseAvaliable(HouseType houseType) {
    LawnApp *lawnApp = (LawnApp *)*gLawnApp_Addr;
    return houseType == 0 || lawnApp->mPlayerInfo->mPurchases[StoreItem::STORE_ITEM_BLUEPRINT_BLING + houseType] > 0;
}

void HouseChooserDialog::MouseDown(int x, int y, int theClickCount) {
    old_HouseChooserDialog_MouseDown(this, x, y, theClickCount);

    int width = (*Sexy_IMAGE_STORE_BLUEPRINT_CLOWN_Addr)->GetWidth();
    int height = (*Sexy_IMAGE_STORE_BLUEPRINT_CLOWN_Addr)->GetHeight();
    int houseIdToSelect = HouseType::BLUEPRINT_INVALID;
    for (int i = 0; i < 5; ++i) {
        Sexy::Rect rect = {75 + 90 * i, 125, width, height};
        if (TRect_Contains(&rect, x, y) && IsHouseAvaliable((HouseType)i)) {
            houseIdToSelect = i;
            break;
        }
    }
    if (houseIdToSelect == HouseType::BLUEPRINT_INVALID)
        return;
    int currentHouseType = mSelectedHouseType;
    if (currentHouseType == houseIdToSelect) {
        GameButtonDown(ButtonCode::BUTTONCODE_A, 0, 0);
    } else {
        while (mSelectedHouseType != houseIdToSelect) {
            GameButtonDown(ButtonCode::BUTTONCODE_LLEFT, 0, 0);
        }
    }
}

void HouseChooserDialog::KeyDown(KeyCode keyCode) {
    old_HouseChooserDialog_KeyDown(this, keyCode);

    switch (keyCode) {
        case 37:
            return GameButtonDown(ButtonCode::BUTTONCODE_LLEFT, 0, 0);
        case 38:
            return GameButtonDown(ButtonCode::BUTTONCODE_LUP, 0, 0);
        case 39:
            return GameButtonDown(ButtonCode::BUTTONCODE_LRIGHT, 0, 0);
        case 40:
            return GameButtonDown(ButtonCode::BUTTONCODE_LDOWN, 0, 0);
        case 27:
        case 305:
            return GameButtonDown(ButtonCode::BUTTONCODE_B, 0, 0);
        case 13:
        case 304:
            return GameButtonDown(ButtonCode::BUTTONCODE_A, 0, 0);
    }
}
