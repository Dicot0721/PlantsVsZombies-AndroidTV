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

#ifndef PVZ_LAWN_BOARD_PROJECTILE_H
#define PVZ_LAWN_BOARD_PROJECTILE_H

#include "GameObject.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Symbols.h"

class Plant;
class Zombie;
class GridItem;

class ProjectileDefinition {
public:
    ProjectileType mProjectileType;
    int mImageRow;
    int mDamage;
};
extern ProjectileDefinition gProjectileDefinition[NUM_PROJECTILES];

class Projectile : public GameObject {
public:
    int mFrame;                     // 13
    int mNumFrames;                 // 14
    int mAnimCounter;               // 15
    float mPosX;                    // 16
    float mPosY;                    // 17
    float mPosZ;                    // 18
    float mVelX;                    // 19
    float mVelY;                    // 20
    float mVelZ;                    // 21
    float mAccZ;                    // 22
    float mShadowY;                 // 23
    bool mDead;                     // 96
                                    //    short mNewProjectileLastX; // 在对齐空隙新增成员，98 ~ 99
    int mAnimTicksPerFrame;         // 25
    ProjectileMotion mMotionType;   // 26
    ProjectileType mProjectileType; // 27
    int mProjectileAge;             // 28
    int mClickBackoffCounter;       // 29
    float mRotation;                // 30
    float mRotationSpeed;           // 31
    bool mOnHighGround;             // 128
                                    //    short mNewProjectileLastY; // 在对齐空隙新增成员，130 ~ 131
    int mDamageRangeFlags;          // 33
    int mHitTorchwoodGridX;         // 34
    AttachmentID mAttachmentID;     // 35
    float mCobTargetX;              // 36
    int mCobTargetRow;              // 37
    ZombieID mTargetZombieID;       // 38
    int mLastPortalX;               // 39
    int unkInt;                     // 40
    // 大小41个整数

    void Die() {
        reinterpret_cast<void (*)(Projectile *)>(Projectile_DieAddr)(this);
    }
    GridItem *FindCollisionTargetGridItem() {
        return reinterpret_cast<GridItem *(*)(Projectile *)>(Projectile_FindCollisionTargetGridItemAddr)(this);
    }
    void DoImpactGridItem(GridItem *theGridItem) {
        return reinterpret_cast<void (*)(Projectile *, GridItem *)>(Projectile_DoImpactGridItemAddr)(this, theGridItem);
    }
    void DoSplashDamage(Zombie *theZombie, GridItem *theGridItem) {
        reinterpret_cast<void (*)(Projectile *, Zombie *, GridItem *)>(Projectile_DoSplashDamageAddr)(this, theZombie, theGridItem);
    }

    int ProjectileInitialize(int theX, int theY, int theRenderOrder, int theRow, ProjectileType theProjectileType);
    void ConvertToFireball(int theGridX);
    void ConvertToPea(int theGridX);
    void Update();
    void DoImpact(Zombie *theZombie);
    void CheckForCollision();
    Zombie *FindCollisionMindControlledTarget();
    ProjectileDefinition &GetProjectileDef();
    Sexy::Rect GetProjectileRect();
    Plant *FindCollisionTargetPlant();
    bool CantHitHighGround();
    bool IsSplashDamage(Zombie *theZombie);
    void PlayImpactSound(Zombie *theZombie);
    unsigned int GetDamageFlags(Zombie *theZombie);
    bool PeaAboutToHitTorchwood();
    Zombie *FindCollisionTarget();
    void Draw(Sexy::Graphics *g);
};

/***************************************************************************************************************/
// 随机子弹
inline bool randomBullet;
inline int bulletSpinnerChosenNum = -1;
inline bool isOnlyPeaUseable;
inline bool banCobCannon;
inline bool banStar;
inline bool isOnlyTouchFireWood;
inline bool ColdPeaCanPassFireWood;


// inline Plant *(*Projectile_FindCollisionTargetPlant)(Projectile *a1);
//
// inline Zombie *(*Projectile_FindCollisionTarget)(Projectile *a);
//
// inline GridItem *(*Projectile_FindCollisionTargetGridItem)(Projectile *a);


inline int (*old_Projectile_ProjectileInitialize)(Projectile *projectile, int theX, int theY, int theRenderOrder, int theRow, ProjectileType theProjectileType);

inline void (*old_Projectile_ConvertToPea)(Projectile *projectile, int aGridX);

inline void (*old_Projectile_Update)(Projectile *a);

inline void (*old_Projectile_DoImpact)(Projectile *a1, Zombie *a2);

inline void (*old_Projectile_Draw)(Projectile *, Sexy::Graphics *);

#endif // PVZ_LAWN_BOARD_PROJECTILE_H
