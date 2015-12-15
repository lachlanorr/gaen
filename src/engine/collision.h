//------------------------------------------------------------------------------
// collision.h - Collision calculations
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2015 Lachlan Orr
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//   1. The origin of this software must not be misrepresented; you must not
//   claim that you wrote the original software. If you use this software
//   in a product, an acknowledgment in the product documentation would be
//   appreciated but is not required.
//
//   2. Altered source versions must be plainly marked as such, and must not be
//   misrepresented as being the original software.
//
//   3. This notice may not be removed or altered from any source
//   distribution.
//------------------------------------------------------------------------------

#ifndef GAEN_ENGINE_COLLISION_H
#define GAEN_ENGINE_COLLISION_H

#include "engine/math.h"

namespace gaen
{
// sqrt(2) - useful for 1-1-sqrt(2) 45-deg right triangles
static const f32 kSqrtOf2 = 1.4142135623730951;

// sqrt(3) - calc half diag of cube = radius * sqrt(3)
static const f32 kSqrtOf3 = 1.7320508075688772;

struct Ray
{
    Ray() {}
    Ray(const Vec3 & pos, const Vec3 & dir)
      : pos(pos)
      , dir(dir)
    {}

    Vec3 pos;
    Vec3 dir;
};

struct Plane
{
    Plane() {}

    Plane(const Vec3 & p0, const Vec3 & p1, const Vec3 & p2)
    {
        norm = Vec3::cross(p1 - p0, p2 - p0);
        norm.normalize();
        dist = Vec3::dot(norm, p0);
    }

    Vec3 norm;  // plane's normalized surface normal
    f32 dist;   // distance from origin
};

struct Sphere
{
    Sphere(const Vec3 & center, f32 radius)
      : cent(center)
      , rad(radius)
    {}

    Vec3 cent; // center point
    f32 rad;   // radius
};

struct AABB_MinMax
{
    Vec3 min; // min coord values along each axis
    Vec3 max; // max coord values along each axis

    AABB_MinMax() {}

    AABB_MinMax(const Vec3 & min, const Vec3 & max)
      : min{min}
      , max{max}
    {}

    AABB_MinMax(Vec3 cent, f32 rad)
      : min{cent.x() - rad, cent.y() - rad, cent.z() - rad}
      , max{cent.x() + rad, cent.y() + rad, cent.z() + rad}
    {}

    AABB_MinMax(f32 rad)
      : min{-rad, -rad, -rad}
      , max{rad, rad, rad}
    {}

    Vec3 center() const
    {
        return min + ((max - min) / 2.0f);
    }
};

struct AABB_MinDiam
{
    Vec3 min;   // min coord values along each axis
    f32 diamX;  // x width
    f32 diamY;  // y width
    f32 diamZ;  // z width
};

struct AABB_CentRad
{
    Vec3 cent; // center point
    f32 radX;  // x half-width
    f32 radY;  // y half-width
    f32 radZ;  // z half-width
};

// Axis-aligned bounding cube
struct AABC
{
    Vec3 cent; // center point
    f32 rad;   // half-width

    // Corner distance from center.
    // Equates to bounding sphere radius.
    f32 diagRad()
    {
        return rad * kSqrtOf3;
    }

    Sphere sphere()
    {
        return Sphere(cent, diagRad());
    }
};

struct OBB
{
    Vec3 cent; // center point
    Vec3 locX; // local x axis
    Vec3 locY; // local y axis
    Vec3 ext;  // positive halfwidth extends along each axis
    
    Vec3 locZ()
    {
        return Vec3::cross(locX, locY);
    }
};

// Oriented bounding cube
struct OBC
{
    Vec3 cent; // center point
    Vec3 locX; // local x axis
    Vec3 locY; // local y axis
    f32 rad;   // half-width

    Vec3 locZ()
    {
        return Vec3::cross(locX, locY);
    }

    // Corner distance from center.
    // Equates to bounding sphere radius.
    f32 diagRad()
    {
        return rad * kSqrtOf3;
    }

    Sphere sphere()
    {
        return Sphere(cent, diagRad());
    }
};

// Determine whether sphere intersects with negative halfspace of plane
inline bool test_sphere_halfspace(const Vec3 & sCent, f32 sRad, const Plane & p)
{
    f32 dist = Vec3::dot(sCent, p.norm) - p.dist;
    return dist <= sRad;
}
inline bool test_sphere_halfpspace(const Sphere & s, const Plane & p)
{
    return test_sphere_halfspace(s.cent, s.rad, p);
}

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_COLLISION_H


