//
// Created by 28636 on 2025/5/26.
//

#include "PvZ/SexyAppFramework/Widget/Widget.h"

using namespace Sexy;

void __Widget::MarkDirty() {
    (*((void (**)(__Widget *))this->vTable + 25))(this); // MarkDirty();
}

void __Widget::AddWidget(__Widget *child) {
    (*((void (**)(__Widget *, __Widget *))this->vTable + 6))(this, child); // AddWidget();
}

void __Widget::RemoveWidget(__Widget *child) {
    (*((void (**)(__Widget *, __Widget *))this->vTable + 7))(this, child); // RemoveWidget();
}

Widget *__Widget::FindWidget(int theId) {
    return (*((Widget * (**)(__Widget *, int)) this->vTable + 9))(this, theId); // FindWidget();
}
