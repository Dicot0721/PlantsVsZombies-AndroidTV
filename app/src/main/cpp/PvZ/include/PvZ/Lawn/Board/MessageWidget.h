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

#ifndef PVZ_LAWN_BOARD_MESSAGE_WIDGET_H
#define PVZ_LAWN_BOARD_MESSAGE_WIDGET_H

#include "PvZ//SexyAppFramework/Misc/Common.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/SexyAppFramework/Graphics/Image.h"

constexpr int MAX_MESSAGE_LENGTH = 128;
constexpr int MAX_REANIM_LINES = 5;

class LawnApp;
class Reanimation;
namespace Sexy {
class Font;
class Graphics;
} // namespace Sexy

class MessageWidget {
public:
    int *vTable;                                    // 0
    int unk[3];                                     // 1 ~ 3
    LawnApp *mApp;                                  // 4
    char mLabel[128];                               // 5 ~ 36
    int mDisplayTime;                               // 37
    int mDuration;                                  // 38
    MessageStyle mMessageStyle;                     // 39
    Reanimation *mTextReanimID[MAX_MESSAGE_LENGTH]; // 40 ~ 167
    ReanimationType mReanimType;                    // 168
    int mSlideOffTime;                              // 169
    float unkFloatWithInitValue1;                   // 170
    char mLabelNext[MAX_MESSAGE_LENGTH];            // 171 ~ 202
    MessageStyle mMessageStyleNext;                 // 203
    // 大小204个整数

    MessageWidget(LawnApp *theApp) {
        __Constructor(theApp);
    }
    ~MessageWidget() {
        __Destructor();
    }
    Sexy::Font *GetFont() {
        return reinterpret_cast<Sexy::Font *(*)(MessageWidget *)>(MessageWidget_GetFontAddr)(this);
    }

protected:
    void __Constructor(LawnApp *theApp) {
        reinterpret_cast<void (*)(MessageWidget *, LawnApp *)>(MessageWidget_MessageWidgetAddr)(this, theApp);
    }
    void __Destructor() {
        reinterpret_cast<void (*)(MessageWidget *)>(MessageWidget_DeleteAddr)(this);
    }
};

class CustomMessageWidget : public MessageWidget {
public:
    Sexy::Image *mIcon; // 204
    // 大小205个整数

    CustomMessageWidget(LawnApp *theApp)
        : MessageWidget(theApp)
        , mIcon(nullptr) {}

    ~CustomMessageWidget() = default;

    void ClearLabel();
    void SetLabel(const pvzstl::string &theLabel, MessageStyle theStyle);
    void Update();
    void Draw(Sexy::Graphics *g);
};

inline void (*old_MessageWidget_Draw)(CustomMessageWidget *messageWidget, Sexy::Graphics *a2);

inline void (*old_MessageWidget_ClearLabel)(CustomMessageWidget *messageWidget);

inline void (*old_MessageWidget_SetLabel)(CustomMessageWidget *messageWidget, const pvzstl::string &theLabel, MessageStyle theStyle);

inline void (*old_MessageWidget_Update)(CustomMessageWidget *messageWidget);

#endif // PVZ_LAWN_BOARD_MESSAGE_WIDGET_H
