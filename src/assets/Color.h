//------------------------------------------------------------------------------
// Color.h - Color datatype
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2021 Lachlan Orr
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

#ifndef GAEN_ASSETS_COLOR_H
#define GAEN_ASSETS_COLOR_H

#include <type_traits>

#include "math/vec3.h"
#include "math/vec4.h"

#include "core/base_defines.h"

namespace gaen
{

static const f32 kRecip255 = 1.0f / 255.0f;

class Color
{
public:
    Color() = default;
    Color(u32 value) { mColorValue.value = value; }
    Color(u8 r, u8 g, u8 b, u8 a) { setChannels(r, g, b, a); }
    Color(u8 r, u8 g, u8 b) { setChannels(r, g, b, 255); }

    void setChannels(u8 r, u8 g, u8 b, u8 a)
    {
        setr(r);
        setg(g);
        setb(b);
        seta(a);
    }

    void setChannels(f32 r, f32 g, f32 b, f32 a)
    {
        setrf(r);
        setgf(g);
        setbf(b);
        setaf(a);
    }

    void setChannels(vec4 vec)
    {
        setChannels(vec.x, vec.y, vec.z, vec.w);
    }

    u32 value() const { return mColorValue.value; }
    void setValue(u32 value) { mColorValue.value = value; }

    u8 r() const { return mColorValue.channel.r; }
    u8 g() const { return mColorValue.channel.g; }
    u8 b() const { return mColorValue.channel.b; }
    u8 a() const { return mColorValue.channel.a; }

    void setr(u8 r) { mColorValue.channel.r = r; }
    void setg(u8 g) { mColorValue.channel.g = g; }
    void setb(u8 b) { mColorValue.channel.b = b; }
    void seta(u8 a) { mColorValue.channel.a = a; }

    float rf() const { return r() * kRecip255; }
    float gf() const { return g() * kRecip255; }
    float bf() const { return b() * kRecip255; }
    float af() const { return a() * kRecip255; }

    void setrf(f32 r) { setr(static_cast<u8>(clamp(r, 0.0f, 1.0f) * 255.0f + 0.5f)); }
    void setgf(f32 g) { setg(static_cast<u8>(clamp(g, 0.0f, 1.0f) * 255.0f + 0.5f)); }
    void setbf(f32 b) { setb(static_cast<u8>(clamp(b, 0.0f, 1.0f) * 255.0f + 0.5f)); }
    void setaf(f32 a) { seta(static_cast<u8>(clamp(a, 0.0f, 1.0f) * 255.0f + 0.5f)); }

    vec3 toVec3() const { return build_vec3(*this); }
    static vec3 build_vec3(const Color & col) { return vec3(col.rf(), col.gf(), col.bf()); }

    vec4 toVec4() const { return build_vec4(*this); }
    static vec4 build_vec4(const Color & col) { return vec4(col.rf(), col.gf(), col.bf(), col.af()); }

    u8 luminance() const
    {
        return luminance(r(), g(), b()) * a();
    }

    static u8 luminance(u8 r, u8 g, u8 b)
    {
        return static_cast<u8>(r * 0.299 + g * 0.587 + b * 0.114 + 0.5);
    }

    bool operator==(Color rhs) const
    {
        return mColorValue.value == rhs.mColorValue.value;
    }

    bool operator!=(Color rhs) const
    {
        return mColorValue.value != rhs.mColorValue.value;
    }

private:
    union ColorValue_
    {
        u32 value;
        struct channel_
        {
            u8 r;
            u8 g;
            u8 b;
            u8 a;
        } channel;
    } mColorValue;

};
static_assert(sizeof(Color)==4,"");

} // namespace gaen

namespace std
{
    template <>
    struct hash<gaen::Color> : public unary_function<gaen::Color, size_t>
    {
        size_t operator()(const gaen::Color& value) const
        {
            return value.value();
        }
    };
} // namespace std

#endif // #ifndef GAEN_ASSETS_COLOR_H
