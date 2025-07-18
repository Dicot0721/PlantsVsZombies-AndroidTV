//
// Created by 28636 on 2025/7/18.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_TESTMENUWIDGET_H
#define PLANTSVSZOMBIES_ANDROIDTV_TESTMENUWIDGET_H

#include "PvZ/Symbols.h"

class TestMenuWidget {
public:
    void Create() { reinterpret_cast<void (*)(TestMenuWidget *)>(TestMenuWidget_TestMenuWidgetAddr)(this); }

    TestMenuWidget() { Create(); }
};

#endif // PLANTSVSZOMBIES_ANDROIDTV_TESTMENUWIDGET_H
