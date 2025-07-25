#ifndef PVZ_SEXYAPPFRAMEWORK_WIDGET_BUTTON_LISTENER_H
#define PVZ_SEXYAPPFRAMEWORK_WIDGET_BUTTON_LISTENER_H

namespace Sexy {

class ButtonListener {
public:
    struct VTable {
        void *__Destructor = nullptr;
        void *__Destructor2 = nullptr;
        void *ButtonPress = nullptr;
        void *ButtonPress2 = nullptr;
        void *ButtonDepress = nullptr;
        void *ButtonDownTick = (void *)&ButtonListener::ButtonDownTick;
        void *ButtonMouseEnter = (void *)&ButtonListener::ButtonMouseEnter;
        void *ButtonMouseLeave = (void *)&ButtonListener::ButtonMouseLeave;
        void *ButtonMouseMove = (void *)&ButtonListener::ButtonMouseMove;
    };

    void ButtonDownTick(this ButtonListener &self, int id) {}
    void ButtonMouseEnter(this ButtonListener &self, int id) {}
    void ButtonMouseLeave(this ButtonListener &self, int id) {}
    void ButtonMouseMove(this ButtonListener &self, int id, int x, int y) {}

    const VTable *mVTable;
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_WIDGET_BUTTON_LISTENER_H
