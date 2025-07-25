//
// Created by 28636 on 2025/5/26.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_FONT_H
#define PLANTSVSZOMBIES_ANDROIDTV_FONT_H

#include "../Misc/Common.h"
#include "../Misc/Rect.h"

#include "Color.h"

namespace Sexy {

class Graphics;

class Font {
public:
    int *vTable;
    int mAscent;
    int mAscentPadding;
    int mHeight;
    int mLineSpacingOffset;

    //    virtual void DrawString(Graphics* g, int theX, int theY, const pvzstl::string& theString, const Color& theColor, const Rect& theClipRect);
};

} // namespace Sexy


#endif // PLANTSVSZOMBIES_ANDROIDTV_FONT_H
