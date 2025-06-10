//
// Created by 28636 on 2025/5/26.
//

#include "PvZ/SexyAppFramework/Widget/Widget.h"

using namespace Sexy;

template<>
void Widget::MarkDirty() {
    (*((void (**)(Sexy::Widget *))this->vTable + 25))(this); // MarkDirty();
}

template<>
void Widget::AddWidget(Sexy::Widget *child) {
    (*((void (**)(Sexy::Widget *, Sexy::Widget *))this->vTable + 6))(this, child); // AddWidget();
}

template<>
void Widget::RemoveWidget(Sexy::Widget *child) {
    (*((void (**)(Sexy::Widget *, Sexy::Widget *))this->vTable + 7))(this, child); // RemoveWidget();
}

template<>
Widget *Widget::FindWidget(int theId) {
    return (*((Sexy::Widget * (**)(Sexy::Widget *, int)) this->vTable + 9))(this, theId); // FindWidget();
}