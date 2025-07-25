//
// Created by 28636 on 2025/5/26.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_COLOR_H
#define PLANTSVSZOMBIES_ANDROIDTV_COLOR_H

#include "../Misc/Common.h"

namespace Sexy {

#pragma pack(push, 1)
struct SexyRGBA {
    unsigned char b, g, r, a;
};
#pragma pack(pop)

class Color {
public:
    int mRed;
    int mGreen;
    int mBlue;
    int mAlpha;

    static Color Black;
    static Color White;

    Color();
    Color(int theColor);
    Color(int theColor, int theAlpha);
    Color(int theRed, int theGreen, int theBlue);
    Color(int theRed, int theGreen, int theBlue, int theAlpha);
    Color(const SexyRGBA& theColor);
    Color(const uchar* theElements);
    Color(const int* theElements);

    int GetRed() const;
    int GetGreen() const;
    int GetBlue() const;
    int GetAlpha() const;
    ulong ToInt() const;
    SexyRGBA ToRGBA() const;

    int& operator[](int theIdx);
    int operator[](int theIdx) const;
};

bool operator==(const Color& theColor1, const Color& theColor2);
bool operator!=(const Color& theColor1, const Color& theColor2);

} // namespace Sexy

inline Sexy::Color black = {0, 0, 0, 255};

inline Sexy::Color gray = {80, 80, 80, 255};

inline Sexy::Color white = {255, 255, 255, 255};

inline Sexy::Color blue = {0, 255, 255, 255};

inline Sexy::Color yellow = {255, 255, 0, 255};

inline Sexy::Color brown = {205, 133, 63, 255};

inline Sexy::Color green = {0, 250, 154, 255};

inline Sexy::Color gZombatarSkinColor[] = {
    {134, 147, 122, 255},
    {79, 135, 94, 255},
    {127, 135, 94, 255},
    {120, 130, 50, 255},
    {156, 163, 105, 255},
    {96, 151, 11, 255},
    {147, 184, 77, 255},
    {82, 143, 54, 255},
    {121, 168, 99, 255},
    {65, 156, 74, 255},
    {107, 178, 114, 255},
    {104, 121, 90, 255},
};

inline Sexy::Color gZombatarAccessoryColor[] = {
    {151, 33, 33, 255},
    {199, 53, 53, 255},
    {220, 112, 47, 255},
    {251, 251, 172, 255},
    {240, 210, 87, 255},
    {165, 126, 65, 255},
    {106, 72, 32, 255},
    {72, 35, 5, 255},
    {50, 56, 61, 255},
    {0, 0, 10, 255},
    {197, 239, 239, 255},
    {63, 109, 242, 255},
    {14, 201, 151, 255},
    {158, 183, 19, 255},
    {30, 210, 64, 255},
    {225, 65, 230, 255},
    {128, 47, 204, 255},
    {255, 255, 255, 255},
};

inline Sexy::Color gZombatarAccessoryColor2[] = {
    {238, 19, 24, 255},
    {247, 89, 215, 255},
    {239, 198, 253, 255},
    {160, 56, 241, 255},
    {86, 74, 241, 255},
    {74, 160, 241, 255},
    {199, 244, 251, 255},
    {49, 238, 237, 255},
    {16, 194, 66, 255},
    {112, 192, 33, 255},
    {16, 145, 52, 255},
    {248, 247, 41, 255},
    {227, 180, 20, 255},
    {241, 115, 25, 255},
    {248, 247, 175, 255},
    {103, 85, 54, 255},
    {159, 17, 20, 255},
    {255, 255, 255, 255},
};


#endif // PLANTSVSZOMBIES_ANDROIDTV_COLOR_H
