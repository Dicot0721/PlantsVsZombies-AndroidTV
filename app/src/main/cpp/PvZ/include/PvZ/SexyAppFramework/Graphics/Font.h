#ifndef PVZ_SEXYAPPFRAMEWORK_GRAPHICS_FONT_H
#define PVZ_SEXYAPPFRAMEWORK_GRAPHICS_FONT_H

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

#endif // PVZ_SEXYAPPFRAMEWORK_GRAPHICS_FONT_H
