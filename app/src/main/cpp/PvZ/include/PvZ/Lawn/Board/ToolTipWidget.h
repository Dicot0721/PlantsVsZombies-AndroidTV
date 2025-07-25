//
// Created by 28636 on 2025/7/18.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_TOOLTIPWIDGET_H
#define PLANTSVSZOMBIES_ANDROIDTV_TOOLTIPWIDGET_H

#include "PvZ/STL/pvzstl_string.h"
#include "PvZ/Symbols.h"

class ToolTipWidget {
public:
    void SetWarningText(const pvzstl::string &theWarningText) {
        reinterpret_cast<void (*)(ToolTipWidget *, const pvzstl::string &)>(ToolTipWidget_SetWarningTextAddr)(this, theWarningText);
    }
};

#endif // PLANTSVSZOMBIES_ANDROIDTV_TOOLTIPWIDGET_H
