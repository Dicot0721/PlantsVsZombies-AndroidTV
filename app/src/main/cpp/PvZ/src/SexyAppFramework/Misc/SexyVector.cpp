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

#include "PvZ/SexyAppFramework/Misc/SexyVector.h"

#include <cmath>

using namespace Sexy;

[[nodiscard]] float SexyVector2::Magnitude() const {
    return std::sqrtf((x * x) + (y * y));
}

[[nodiscard]] SexyVector2 SexyVector2::Normalize() const {
    float aMag = Magnitude();
    return (aMag != 0) ? *this / aMag : *this;
}

[[nodiscard]] float SexyVector3::Magnitude() const {
    return std::sqrtf((x * x) + (y * y) + (z * z));
}

[[nodiscard]] SexyVector3 SexyVector3::Normalize() const {
    float aMag = Magnitude();
    return (aMag != 0) ? *this / aMag : *this;
}
