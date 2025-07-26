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

#ifndef PVZ_SEXYAPPFRAMEWORK_SOUND_AUDIERE_MUSIC_INTERFACE_H
#define PVZ_SEXYAPPFRAMEWORK_SOUND_AUDIERE_MUSIC_INTERFACE_H

#include "PvZ/Symbols.h"

namespace Sexy {

class AudiereMusicInterface {
public:
    void SetVolume(double theVolume) {
        reinterpret_cast<void (*)(AudiereMusicInterface *, double)>(Sexy_AudiereMusicInterface_SetVolumeAddr)(this, theVolume);
    }
    void SetSongVolume(int theSongId, double theVolume) {
        reinterpret_cast<void (*)(AudiereMusicInterface *, int, double)>(Sexy_AudiereMusicInterface_SetSongVolumeAddr)(this, theSongId, theVolume);
    }
    void PlayMusic(int theSongId, int theOffset = 0, bool noLoop = false) {
        reinterpret_cast<void (*)(AudiereMusicInterface *, int, int, bool)>(Sexy_AudiereMusicInterface_PlayMusicAddr)(this, theSongId, theOffset, noLoop);
    }
    void StopMusic(int theSongId) {
        reinterpret_cast<void (*)(AudiereMusicInterface *, int)>(Sexy_AudiereMusicInterface_StopMusicAddr)(this, theSongId);
    }
    void SetChannelVolume(int theSongId, int theOffset, double theVolume) {
        reinterpret_cast<void (*)(AudiereMusicInterface *, int, int, double)>(Sexy_AudiereMusicInterface_SetChannelVolumeAddr)(this, theSongId, theOffset, theVolume);
    }
};

} // namespace Sexy


#endif // PVZ_SEXYAPPFRAMEWORK_SOUND_AUDIERE_MUSIC_INTERFACE_H
