/*
 * Copyright (C) 2023-2025  PvZ TV Touch Team
 *
 * This file is part of PlantsVsZombies-AndroidTV.
 *
 * PlantsVsZombies-AndroidTV is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * PlantsVsZombies-AndroidTV is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * PlantsVsZombies-AndroidTV.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef PVZ_SEXYAPPFRAMEWORK_GRAPHICS_GRAPHICS_H
#define PVZ_SEXYAPPFRAMEWORK_GRAPHICS_GRAPHICS_H

#include "../Misc/Common.h"
#include "../Misc/Rect.h"
#include "../Misc/SexyMatrix.h"
#include "../Misc/TriVertex.h"
#include "PvZ/Symbols.h"

#include "Color.h"
#include "Image.h"

namespace Sexy {

class Font;
class Transform;
class SexyVertex2D;

class GLGraphics {
public:
    int mHorizonWrapMode; // 5
    int mVertWrapMode;    // 6
};

class GraphicsState {
public:
    void CopyStateFrom(const GraphicsState *theState) {
        reinterpret_cast<void (*)(GraphicsState *, const GraphicsState *)>(Sexy_GraphicsState_CopyStateFromAddr)(this, theState);
    }
};

class Graphics : public GraphicsState {
public:
    enum DrawMode {
        DRAWMODE_NORMAL = 0,
        DRAWMODE_ADDITIVE = 1,
    };

    int *vTable;                  // 0
    Image *mDestImage;            // 1
    float mTransX;                // 2
    float mTransY;                // 3
    float mScaleX;                // 4
    float mScaleY;                // 5
    float mScaleOrigX;            // 6
    float mScaleOrigY;            // 7
    Rect mClipRect;               // 8 ~ 11
    int unk2[3];                  // 12 ~ 14
    Color mColorUnknown;          // 15 ~ 18
    Color mColor;                 // 19 ~ 22
    Font *mFont;                  // 23
    DrawMode mDrawMode;           // 24
    bool mColorizeImages;         // 100
    bool mFastStretch;            // 101
    bool mWriteColoredString;     // 102
    bool mLinearBlend;            // 103
    bool mIs3D;                   // 104
    bool mGlobalScale;            // 105
    bool mGlobalTrackDeviceState; // 106
    int *unkPushPopTramsform;     // 27
    int unkInt;                   // 28
    int m3D;                      // 29
    int unk3[6];                  // 30 ~ 35
    // 大小36个整数

    Graphics(const Graphics &theGraphics) {
        CreateGraphics(theGraphics);
    }
    Graphics(Image *theDestImage = NULL) {
        CreateImage(theDestImage);
    }
    void CreateGraphics(const Graphics &theGraphics) {
        reinterpret_cast<void (*)(Graphics *, const Graphics &)>(Sexy_Graphics_GraphicsAddr)(this, theGraphics);
    }
    void CreateImage(Image *theDestImage = NULL) {
        reinterpret_cast<void (*)(Graphics *, Image *)>(Sexy_Graphics_Graphics2Addr)(this, theDestImage);
    }
    ~Graphics() {
        reinterpret_cast<void (*)(Graphics *)>(Sexy_Graphics_DeleteAddr)(this);
    }
    void PushState() {
        reinterpret_cast<void (*)(Graphics *)>(Sexy_Graphics_PushStateAddr)(this);
    }
    void PopState() {
        reinterpret_cast<void (*)(Graphics *)>(Sexy_Graphics_PopStateAddr)(this);
    }
    void ClipRect(int theX, int theY, int theWidth, int theHeight) {
        reinterpret_cast<void (*)(Graphics *, int, int, int, int)>(Sexy_Graphics_ClipRectAddr)(this, theX, theY, theWidth, theHeight);
    }
    void SetClipRect(int theX, int theY, int theWidth, int theHeight) {
        reinterpret_cast<void (*)(Graphics *, int, int, int, int)>(Sexy_Graphics_SetClipRectAddr)(this, theX, theY, theWidth, theHeight);
    }
    void ClearClipRect() {
        reinterpret_cast<void (*)(Graphics *)>(Sexy_Graphics_ClearClipRectAddr)(this);
    }
    void FillRect(const Rect &theRect) {
        reinterpret_cast<void (*)(Graphics *, const Rect &)>(Sexy_Graphics_FillRectAddr)(this, theRect);
    }
    void DrawRect(const Rect &theRect) {
        reinterpret_cast<void (*)(Graphics *, const Rect &)>(Sexy_Graphics_DrawRectAddr)(this, theRect);
    }
    void ClearRect(int theX, int theY, int theWidth, int theHeight) {
        reinterpret_cast<void (*)(Graphics *, int, int, int, int)>(Sexy_Graphics_ClearRectAddr)(this, theX, theY, theWidth, theHeight);
    }
    void DrawString(const pvzstl::string &theString, int theX, int theY) { // SexyString类型尚不明确
        reinterpret_cast<void (*)(Graphics *, const pvzstl::string &, int, int)>(Sexy_Graphics_DrawStringAddr)(this, theString, theX, theY);
    }
    void DrawImage(Image *theImage, int theX, int theY) {
        reinterpret_cast<void (*)(Graphics *, Image *, int, int)>(Sexy_Graphics_DrawImageAddr)(this, theImage, theX, theY);
    }
    void DrawImage(Image *theImage, int theX, int theY, const Rect &theSrcRect) {
        reinterpret_cast<void (*)(Graphics *, Image *, int, int, const Rect &)>(Sexy_Graphics_DrawImage2Addr)(this, theImage, theX, theY, theSrcRect);
    }
    void DrawImageF(Image *theImage, float theX, float theY) {
        reinterpret_cast<void (*)(Graphics *, Image *, float, float)>(Sexy_Graphics_DrawImageFAddr)(this, theImage, theX, theY);
    }
    void DrawImageF(Image *theImage, float theX, float theY, const Rect &theSrcRect) {
        reinterpret_cast<void (*)(Graphics *, Image *, float, float)>(Sexy_Graphics_DrawImageF2Addr)(this, theImage, theX, theY);
    }
    void DrawImageCel(Image *theImageStrip, int theX, int theY, int theCel) {
        reinterpret_cast<void (*)(Graphics *, Image *, int, int, int)>(Sexy_Graphics_DrawImageCelAddr)(this, theImageStrip, theX, theY, theCel);
    }
    void DrawImageCel(Image *theImageStrip, int theX, int theY, int theCelCol, int theCelRow) {
        reinterpret_cast<void (*)(Graphics *, Image *, int, int, int, int)>(Sexy_Graphics_DrawImageCel2Addr)(this, theImageStrip, theX, theY, theCelCol, theCelRow);
    }
    void DrawImageMirror(Image *theImage, int theX, int theY, bool mirror) {
        reinterpret_cast<void (*)(Graphics *, Image *, int, int, bool)>(Sexy_Graphics_DrawImageMirrorAddr)(this, theImage, theX, theY, mirror);
    }
    void DrawImageMirror(Image *theImage, const Rect &theDestRect, const Rect &theSrcRect, bool mirror) {
        reinterpret_cast<void (*)(Graphics *, Image *, const Rect &, const Rect &, bool)>(Sexy_Graphics_DrawImageMirror2Addr)(this, theImage, theDestRect, theSrcRect, mirror);
    }
    void DrawTrianglesTex(Image *theTexture, const SexyVertex2D theVertices[][3], int theNumTriangles) {
        reinterpret_cast<void (*)(Graphics *, Image *, const SexyVertex2D[][3], int)>(Sexy_Graphics_DrawTrianglesTexAddr)(this, theTexture, theVertices, theNumTriangles);
    }
    void DrawImageMatrix(Image *theImage, const SexyMatrix3 &theMatrix, const Rect &theSrcRect, float x, float y, bool theBool) {
        reinterpret_cast<void (*)(Graphics *, Image *, const SexyMatrix3 &, const Rect &, float, float, bool)>(Sexy_Graphics_DrawImageMatrixAddr)(this, theImage, theMatrix, theSrcRect, x, y, theBool);
    }
    void SetWrapMode(int theHorizonWrapMode, int theVertWrapMode) {
        reinterpret_cast<void (*)(Graphics *, int, int)>(Sexy_GLGraphics_SetWrapModeAddr)(this, theHorizonWrapMode, theVertWrapMode);
    }


    void SetFont(Font *theFont);
    Font *GetFont();
    void SetColor(const Color &theColor);
    const Color &GetColor();
    void SetDrawMode(DrawMode theDrawMode);
    int GetDrawMode();
    void SetColorizeImages(bool colorizeImages);
    bool GetColorizeImages();
    void SetLinearBlend(bool linear); // for DrawImageMatrix, DrawImageTransform, etc...
    bool GetLinearBlend();
    void Translate(int theTransX, int theTransY);
    void SetScale(float theScaleX, float theScaleY, float theOrigX, float theOrigY);
    void PushTransform(int *theTransform, bool concatenate);
    void PopTransform();
};

} // namespace Sexy


inline void (*old_Sexy_Graphics_PushTransform)(Sexy::Graphics *, int *, bool);

inline void (*old_Sexy_Graphics_PopTransform)(Sexy::Graphics *graphics);

void Sexy_Graphics_DrawImageColorized(Sexy::Graphics *graphics, Sexy::Image *image, Sexy::Color *color, int x, int y);

void Sexy_Graphics_DrawImageColorizedScaled(Sexy::Graphics *graphics, Sexy::Image *image, Sexy::Color *color, float x, float y, float xScaled, float yScaled);

#endif // PVZ_SEXYAPPFRAMEWORK_GRAPHICS_GRAPHICS_H
