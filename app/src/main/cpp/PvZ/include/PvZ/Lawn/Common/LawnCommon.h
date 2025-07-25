#ifndef PVZ_COMMON_LAWN_COMMON_H
#define PVZ_COMMON_LAWN_COMMON_H

#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/SexyAppFramework/Widget/EditWidget.h"
#include "PvZ/Symbols.h"

class Board;
class LawnStoneButton;
class NewLawnButton;

namespace Sexy {
class Dialog;
class Checkbox;
class DialogButton;
class CheckboxListener;
} // namespace Sexy

// ====================================================================================================
// ★ 常用逻辑判断
// ====================================================================================================
/*inline*/ bool ModInRange(int theNumber, int theMod, int theRange = 0);
/*inline*/ bool GridInRange(int x1, int y1, int x2, int y2, int theRangeX = 1, int theRangeY = 1);

// ====================================================================================================
// ★ 控件
// ====================================================================================================
inline Sexy::Checkbox *MakeNewCheckbox(int theId, Sexy::CheckboxListener *theListener, Sexy::__Widget *theParent, bool theDefault) {
    return reinterpret_cast<Sexy::Checkbox *(*)(int, Sexy::CheckboxListener *, Sexy::__Widget *, bool)>(MakeNewCheckboxAddr)(theId, theListener, theParent, theDefault);
}

#endif // PVZ_COMMON_LAWN_COMMON_H
