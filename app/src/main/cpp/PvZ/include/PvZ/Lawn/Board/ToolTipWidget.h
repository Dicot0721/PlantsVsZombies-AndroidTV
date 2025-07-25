#ifndef PVZ_LAWN_BOARD_TOOL_TIP_WIDGET_H
#define PVZ_LAWN_BOARD_TOOL_TIP_WIDGET_H

#include "PvZ/STL/pvzstl_string.h"
#include "PvZ/Symbols.h"

class ToolTipWidget {
public:
    void SetWarningText(const pvzstl::string &theWarningText) {
        reinterpret_cast<void (*)(ToolTipWidget *, const pvzstl::string &)>(ToolTipWidget_SetWarningTextAddr)(this, theWarningText);
    }
};

#endif // PVZ_LAWN_BOARD_TOOL_TIP_WIDGET_H
