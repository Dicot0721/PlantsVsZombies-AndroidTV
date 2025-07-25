#include "PvZ/Lawn/Board/CursorObject.h"

bool CursorObject::BeginDraw(Sexy::Graphics *g) {
    return old_CursorObject_BeginDraw(this, g);
}

void CursorObject::EndDraw(Sexy::Graphics *g) {
    return old_CursorObject_EndDraw(this, g);
}