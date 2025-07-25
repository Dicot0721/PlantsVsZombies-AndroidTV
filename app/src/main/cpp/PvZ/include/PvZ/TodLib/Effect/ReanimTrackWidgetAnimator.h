#ifndef PVZ_SEXYAPPFRAMEWORK_EFFECT_REANIM_TRACK_WIDGET_ANIMATOR_H
#define PVZ_SEXYAPPFRAMEWORK_EFFECT_REANIM_TRACK_WIDGET_ANIMATOR_H

#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/SexyAppFramework/Misc/WidgetAnimator.h"

class ReanimTrackWidgetAnimator : public Sexy::WidgetAnimator {
public:
    LawnApp *mApp;                 // 6
    Sexy::Widget *mLawnMenuWidget; // 7
    int *mTrackName1;              // 8
    int *mTrackName2;              // 9
    int *mTrackName3;              // 10
    int unk2[2];                   // 11 ~ 12
};

#endif // PVZ_SEXYAPPFRAMEWORK_EFFECT_REANIM_TRACK_WIDGET_ANIMATOR_H
