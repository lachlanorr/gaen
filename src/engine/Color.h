//------------------------------------------------------------------------------
// Color.h - Color datatype
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014 Lachlan Orr
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

#ifndef GAEN_ENGINE_COLOR_H
#define GAEN_ENGINE_COLOR_H

#include "engine/math.h"

namespace gaen
{

static const f32 kRecip255 = 1.0f / 255.0f;

class Color
{
public:
    Color() = default;
    Color(u32 value) { mColorValue.value = value; }
    Color(u8 r, u8 g, u8 b, u8 a) { setChannels(r, g, b, a); }

    void setChannels(u8 r, u8 g, u8 b, u8 a)
    {
        setr(r);
        setg(g);
        setb(b);
        seta(a);
    }

    void setChannelsF(f32 r, f32 g, f32 b, f32 a)
    {
        setrf(r);
        setgf(g);
        setbf(b);
        setaf(a);
    }

    u32 value() { return mColorValue.value; }
    void setValue(u32 value) { mColorValue.value = value; }

    u8 r() const { return mColorValue.channel.r; }
    u8 g() const { return mColorValue.channel.r; }
    u8 b() const { return mColorValue.channel.r; }
    u8 a() const { return mColorValue.channel.r; }

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

    static Vec4 build_vec4(const Color & col) { return Vec4(col.rf(), col.gf(), col.bf(), col.af()); }

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

#endif // #ifndef GAEN_ENGINE_COLOR_H
