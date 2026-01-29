/*
 * Copyright (C) 2023-2026  PvZ TV Touch Team
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

#ifndef PVZ_SEXYAPPFRAMEWORK_GRAPHICS_COLOR_H
#define PVZ_SEXYAPPFRAMEWORK_GRAPHICS_COLOR_H

namespace Sexy {

struct alignas(1) SexyRGBA {
    unsigned char b, g, r, a;
};

class Color {
public:
    int mRed;
    int mGreen;
    int mBlue;
    int mAlpha;

    static Color Black;
    static Color White;

    constexpr Color() noexcept
        : mRed{0}
        , mGreen{0}
        , mBlue{0}
        , mAlpha{255} {}

    constexpr Color(int theColor) noexcept
        : mRed{(theColor >> 16) & 0xFF}
        , mGreen{(theColor >> 8) & 0xFF}
        , mBlue{theColor & 0xFF}
        , mAlpha{(theColor >> 24) & 0xFF} {
        if (mAlpha == 0) {
            mAlpha = 0xff;
        }
    }

    constexpr Color(int theColor, int theAlpha) noexcept
        : mRed{(theColor >> 16) & 0xFF}
        , mGreen{(theColor >> 8) & 0xFF}
        , mBlue{theColor & 0xFF}
        , mAlpha{theAlpha} {}

    constexpr Color(int theRed, int theGreen, int theBlue) noexcept
        : mRed{theRed}
        , mGreen{theGreen}
        , mBlue{theBlue}
        , mAlpha{0xFF} {}

    constexpr Color(int theRed, int theGreen, int theBlue, int theAlpha) noexcept
        : mRed{theRed}
        , mGreen{theGreen}
        , mBlue{theBlue}
        , mAlpha{theAlpha} {}

    constexpr Color(const SexyRGBA &theColor) noexcept
        : mRed{theColor.r}
        , mGreen{theColor.g}
        , mBlue{theColor.b}
        , mAlpha{theColor.a} {}

    constexpr Color(const unsigned char *theElements) noexcept
        : mRed{theElements[0]}
        , mGreen{theElements[1]}
        , mBlue{theElements[2]}
        , mAlpha{0xFF} {}

    constexpr Color(const int *theElements) noexcept
        : mRed{theElements[0]}
        , mGreen{theElements[1]}
        , mBlue{theElements[2]}
        , mAlpha{0xFF} {}

    constexpr int GetRed() const noexcept {
        return mRed;
    }

    constexpr int GetGreen() const noexcept {
        return mGreen;
    }

    constexpr int GetBlue() const noexcept {
        return mBlue;
    }

    constexpr int GetAlpha() const noexcept {
        return mAlpha;
    }

    constexpr unsigned long ToInt() const noexcept {
        return (mAlpha << 24) | (mRed << 16) | (mGreen << 8) | (mBlue);
    }

    constexpr SexyRGBA ToRGBA() const noexcept {
        SexyRGBA anRGBA;
        anRGBA.r = mRed;
        anRGBA.g = mGreen;
        anRGBA.b = mBlue;
        anRGBA.a = mAlpha;
        return anRGBA;
    }

    constexpr int operator[](int theIdx) const noexcept {
        switch (theIdx) {
            case 0:
                return mRed;
            case 1:
                return mGreen;
            case 2:
                return mBlue;
            case 3:
                return mAlpha;
            default:
                return 0;
        }
    }

    friend constexpr bool operator==(const Color &theColor1, const Color &theColor2) = default;
};

} // namespace Sexy

constexpr Sexy::Color black = {0, 0, 0, 255};

constexpr Sexy::Color gray = {80, 80, 80, 255};

constexpr Sexy::Color white = {255, 255, 255, 255};

constexpr Sexy::Color blue = {0, 255, 255, 255};

constexpr Sexy::Color yellow = {255, 255, 0, 255};

constexpr Sexy::Color brown = {205, 133, 63, 255};

constexpr Sexy::Color green = {0, 250, 154, 255};

constexpr Sexy::Color gZombatarSkinColor[] = {
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

constexpr Sexy::Color gZombatarAccessoryColor[] = {
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

constexpr Sexy::Color gZombatarAccessoryColor2[] = {
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

#endif // PVZ_SEXYAPPFRAMEWORK_GRAPHICS_COLOR_H
