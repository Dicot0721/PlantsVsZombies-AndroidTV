//
// Created by 28636 on 2025/5/26.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_BUTTONLISTENER_H
#define PLANTSVSZOMBIES_ANDROIDTV_BUTTONLISTENER_H


namespace Sexy {

struct ButtonListenerVTable {
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

class ButtonListener {
public:
    ButtonListenerVTable *vTable;
};

}

inline void Sexy_ButtonListener_ButtonMouseEnter(Sexy::ButtonListener *listener, int id) {}

inline void Sexy_ButtonListener_ButtonMouseMove(Sexy::ButtonListener *listener, int id, int x, int y) {}

inline void Sexy_ButtonListener_ButtonMouseLeave(Sexy::ButtonListener *listener, int id) {}

inline void Sexy_ButtonListener_ButtonDownTick(Sexy::ButtonListener *listener, int id) {}



#endif // PLANTSVSZOMBIES_ANDROIDTV_BUTTONLISTENER_H
