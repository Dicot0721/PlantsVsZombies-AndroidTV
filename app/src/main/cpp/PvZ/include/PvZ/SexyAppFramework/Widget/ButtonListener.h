#ifndef PVZ_SEXYAPPFRAMEWORK_WIDGET_BUTTON_LISTENER_H
#define PVZ_SEXYAPPFRAMEWORK_WIDGET_BUTTON_LISTENER_H

namespace Sexy {

class ButtonListener {
public:
    struct VTable {
        void *__Destructor;
        void *__Destructor2;
        void *ButtonPress;
        void *ButtonPress2;
        void *ButtonDepress;
        void *ButtonDownTick;
        void *ButtonMouseEnter;
        void *ButtonMouseLeave;
        void *ButtonMouseMove;
    };

    const VTable *mVTable;
};

} // namespace Sexy

inline void Sexy_ButtonListener_ButtonMouseEnter(Sexy::ButtonListener *listener, int id) {}

inline void Sexy_ButtonListener_ButtonMouseMove(Sexy::ButtonListener *listener, int id, int x, int y) {}

inline void Sexy_ButtonListener_ButtonMouseLeave(Sexy::ButtonListener *listener, int id) {}

inline void Sexy_ButtonListener_ButtonDownTick(Sexy::ButtonListener *listener, int id) {}


#endif // PVZ_SEXYAPPFRAMEWORK_WIDGET_BUTTON_LISTENER_H
