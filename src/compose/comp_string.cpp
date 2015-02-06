//------------------------------------------------------------------------------
// comp_string.cpp - Helper functions related to string processing
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

#include <cstring>

#include "engine/BlockMemory.h"

#include "compose/comp_string.h"

namespace gaen
{

enum SpecParseState
{
    kSPS_Init,
    kSPS_SpecStart,
    kSPS_Width,
    kSPS_Precision
};


static bool is_number(char c)
{
    return c >= '0' && c <= '9';
}

static bool is_type_specifier(char c)
{
    return (c == 'd' ||
            c == 'i' ||
            c == 'u' ||
            c == 'f' ||
            c == 'F' ||
            c == 'x' ||
            c == 'X' ||
            c == 'o' ||
            c == 'c' ||
            c == 's');
}

// Sanity lengths for badly formed strings
static const u32 kMaxWidth = 128;
static const u32 kMaxPrecision = 128;

FormatSpecifier find_next_specifier(const char * str)
{
    FormatSpecifier fs;

    if (!str || !*str)
        return fs;

    const char * p = str;

    SpecParseState sps = kSPS_Init;
    
    while (*p)
    {
        switch (sps)
        {
        case kSPS_Init:
            if (*p == '%')
            {
                fs.reset();
                fs.begin = p;
                if (p[1] == '%')
                {
                    // found a double percent, "%%" which we interpret as a literal percent sign
                    fs.type = '%';
                    fs.end = p + 2;
                    return fs;
                }
                else
                    sps = kSPS_SpecStart;
            }
            break;
        case kSPS_SpecStart:
            switch (*p)
            {
            case '-':
                fs.flagLeftJustify = 1;
                break;
            case '+':
                fs.flagPlus = 1;
                break;
            case ' ':
                fs.flagSpace = 1;
                break;
            case '#':
                fs.flagHash = 1;
                break;
            case '0':
                fs.flagZero = 1;
                break;
            default:
                if (is_type_specifier(*p))
                {
                    fs.type = *p;
                    fs.end = p + 1;
                    return fs;
                }
                else if (is_number(*p)) // zero case handled above
                {
                    fs.width = 0;
                    sps = kSPS_Width;
                    continue;
                }
                else if (*p == '.')
                {
                    fs.precision = 0;
                    sps = kSPS_Precision;
                }
                else
                {
                    sps = kSPS_Init;
                }
                break;
            }
            break;
        case kSPS_Width:
            if (is_type_specifier(*p))
            {
                fs.type = *p;
                fs.end = p + 1;
                return fs;
            }
            else if (*p == '*')
            {
                fs.flagStarWidth = 1;
            }
            else if (is_number(*p))
            {
                fs.width = (fs.width * 10) + (*p - '0');
                fs.width = minval<i16>(fs.width, kMaxWidth);
            }
            else if (*p == '.')
            {
                fs.precision = 0;
                sps = kSPS_Precision;
            }
            else
            {
                sps = kSPS_Init;
            }
            break;
        case kSPS_Precision:
            if (is_type_specifier(*p))
            {
                fs.type = *p;
                fs.end = p + 1;
                return fs;
            }
            else if (*p == '*')
            {
                fs.flagStarPrecision = 1;
            }
            else if (is_number(*p))
            {
                fs.precision = (fs.precision * 10) + (*p - '0');
                fs.precision = minval<i16>(fs.precision, kMaxPrecision);
            }
            else
            {
                sps = kSPS_Init;
            }
            break;
        }
        ++p;
    }

    fs.reset();
    return fs;
}

i16 digit_count(u32 val, u32 base)
{
    if (val == 0)
        return 1;

    i16 count = 0;
    while (val > 0)
    {
        val /= base;
        count++;
    }
    return count;
}

FormatSize * corrected_size(FormatSpecifier * pFs, i32 size)
{
    i32 zeroPadding = pFs->precision > size ? pFs->precision - size : 0;
    size += zeroPadding;
    if (pFs->width > size)
    {
        if (pFs->width > size)
        {
            if (pFs->flagZero)
                pFs->size = FormatSize(pFs->width,
                                       0,
                                       zeroPadding + pFs->width - size);
            else
                pFs->size = FormatSize(pFs->width,
                                       pFs->width - size,
                                       zeroPadding);
        }
    }
    else
        pFs->size = FormatSize(size, 0, zeroPadding);

    return &pFs->size;
}

u32 float_whole_size(f32 val)
{
    if (val < 0.0f) val = -val; // abs

    return digit_count((u32)val, 10);
}


FormatSize * specifier_max_size_sw(FormatSpecifier * pFs, u32 starWidth, i32 val)
{
    ASSERT(pFs->flagStarWidth && !pFs->flagStarPrecision);
    pFs->width = (i16)starWidth;
    return specifier_max_size(pFs, val);
}
FormatSize * specifier_max_size_sp(FormatSpecifier * pFs, u32 starPrecision, i32 val)
{
    ASSERT(!pFs->flagStarWidth && pFs->flagStarPrecision);
    pFs->precision = (i16)starPrecision;
    return specifier_max_size(pFs, val);
}
FormatSize * specifier_max_size_sw_sp(FormatSpecifier * pFs, u32 starWidth, u32 starPrecision, i32 val)
{
    ASSERT(pFs->flagStarWidth && pFs->flagStarPrecision);
    pFs->width = (i16)starWidth;
    pFs->precision = (i16)starPrecision;
    return specifier_max_size(pFs, val);
}
FormatSize * specifier_max_size(FormatSpecifier * pFs, i32 val)
{
    switch (pFs->type)
    {
    case 'd':
    case 'i':
    {
        bool isNeg = val < 0;

        if (isNeg) val = -val; // abs

        i32 size = 0;

        // zero precision with a zero val results in no printing
        if (pFs->precision != 0 || val != 0)
            size = digit_count(val, 10);

        if (isNeg || pFs->flagPlus)
            size++;

        return corrected_size(pFs, size);
    }
    case 'u':
    case 'x':
    case 'X':
    case 'o':
        return specifier_max_size(pFs, (u32)val);
    case 'f':
    case 'F':
        return specifier_max_size(pFs, (f32)val);
    default:
        PANIC("Unexpected format type: %c", pFs->type);
        return nullptr;
    }
}




FormatSize * specifier_max_size_sw(FormatSpecifier * pFs, u32 starWidth, u32 val)
{
    ASSERT(pFs->flagStarWidth && !pFs->flagStarPrecision);
    pFs->width = (i16)starWidth;
    return specifier_max_size(pFs, val);
}
FormatSize * specifier_max_size_sp(FormatSpecifier * pFs, u32 starPrecision, u32 val)
{
    ASSERT(!pFs->flagStarWidth && pFs->flagStarPrecision);
    pFs->precision = (i16)starPrecision;
    return specifier_max_size(pFs, val);
}
FormatSize * specifier_max_size_sw_sp(FormatSpecifier * pFs, u32 starWidth, u32 starPrecision, u32 val)
{
    ASSERT(pFs->flagStarWidth && pFs->flagStarPrecision);
    pFs->width = (i16)starWidth;
    pFs->precision = (i16)starPrecision;
    return specifier_max_size(pFs, val);
}
FormatSize * specifier_max_size(FormatSpecifier * pFs, u32 val)
{
    switch (pFs->type)
    {
    case 'd':
    case 'i':
        return specifier_max_size(pFs, (i32)val);
    case 'u':
    {
        i32 size = 0;

        // zero precision with a zero val results in no printing
        if (pFs->precision != 0 || val != 0)
            size = digit_count(val, 10);

        if (pFs->flagPlus)
            size++;

        return corrected_size(pFs, size);
    }
    case 'x':
    case 'X':
    {
        i32 size = 0;
        i32 zeroPadding = 0;
        i32 spacePadding = 0;

        // zero precision with a zero val results in no printing
        if (pFs->precision != 0 || val != 0)
            size = digit_count(val, 16);

        // add zeroes from precision
        if (pFs->precision > size)
        {
            i32 delta = pFs->precision - size;
            zeroPadding += delta;
            size += delta;
        }

        // add '0x' if hash flag
        if (pFs->flagHash && val != 0)
            size += 2; // for '0x'

        if (pFs->width > size)
        {
            i32 delta = pFs->width - size;
            if (pFs->flagZero)
                zeroPadding += delta;
            else
                spacePadding += delta;

            size += delta;
        }

        pFs->size.totalWidth = size;
        pFs->size.zeroPadding = zeroPadding;
        pFs->size.spacePadding = spacePadding;

        return &pFs->size;
    }
    case 'o':
    {
        i32 size = 0;
        i32 zeroPadding = 0;
        i32 spacePadding = 0;

        // zero precision with a zero val results in no printing
        if (pFs->precision != 0 || val != 0)
            size = digit_count(val, 8);


        // add zeroes from precision
        if (pFs->precision > size)
        {
            i32 delta = pFs->precision - size;
            zeroPadding += delta;
            size += delta;
        }

        // add '0' if hash flag
        if (pFs->flagHash && val != 0)
        {
            size += 1; // for '0'
        }

        if (pFs->width > size)
        {
            i32 delta = pFs->width - size;
            if (pFs->flagZero)
                zeroPadding += delta;
            else
                spacePadding += delta;

            size += delta;
        }

        pFs->size.totalWidth = size;
        pFs->size.zeroPadding = zeroPadding;
        pFs->size.spacePadding = spacePadding;

        return &pFs->size;
    }
    case 'f':
    case 'F':
        return specifier_max_size(pFs, (f32)val);
    default:
        PANIC("Unexpected format type: %c", pFs->type);
        return nullptr;
    }
}

// Add appropriate 0.5 multiple so to round when we truncate
f32 add_round_val(f32 val, i16 precision)
{
    ASSERT(precision >= 0);

    if (val == 0.0f)
        return val;

    if (precision == 0)
        return val + 0.5f;

    // precision > 0
    float roundVal = 0.5f;
    for (i16 i = 0; i < precision; ++i)
        roundVal /= 10.0f;
    return val + roundVal;
}



FormatSize * specifier_max_size_sw(FormatSpecifier * pFs, u32 starWidth, f32 val)
{
    ASSERT(pFs->flagStarWidth && !pFs->flagStarPrecision);
    pFs->width = (i16)starWidth;
    return specifier_max_size(pFs, val);
}
FormatSize * specifier_max_size_sp(FormatSpecifier * pFs, u32 starPrecision, f32 val)
{
    ASSERT(!pFs->flagStarWidth && pFs->flagStarPrecision);
    pFs->precision = (i16)starPrecision;
    return specifier_max_size(pFs, val);
}
FormatSize * specifier_max_size_sw_sp(FormatSpecifier * pFs, u32 starWidth, u32 starPrecision, f32 val)
{
    ASSERT(pFs->flagStarWidth && pFs->flagStarPrecision);
    pFs->width = (i16)starWidth;
    pFs->precision = (i16)starPrecision;
    return specifier_max_size(pFs, val);
}
FormatSize * specifier_max_size(FormatSpecifier * pFs, f32 val)
{
    switch (pFs->type)
    {
    case 'd':
    case 'i':
        return specifier_max_size(pFs, (i32)val);
    case 'u':
        return specifier_max_size(pFs, (u32)val);
    case 'x':
    case 'X':
    case 'o':
        return specifier_max_size(pFs, *reinterpret_cast<u32*>(&val));
    case 'f':
    case 'F':
    {
        u32 fractionDigits = pFs->precision == -1 ? 6 : pFs->precision;

        val = add_round_val(val, fractionDigits);

        u32 wholeDigits = float_whole_size(val);

        i32 size = wholeDigits + fractionDigits;

        if (fractionDigits > 0 || pFs->flagHash)
            size++; // +1 for '.'

        // explicit test for sign bit since '<= -0.0f' doesn't work as we need
        u32 signBit = *reinterpret_cast<u32*>(&val) >> 31 == 1;

        if (pFs->flagPlus || signBit)
            ++size;

        return corrected_size(pFs, size);
    }
    default:
        PANIC("Unexpected format type: %c", pFs->type);
        return nullptr;
    }
}



FormatSize * specifier_max_size_sw(FormatSpecifier * pFs, u32 starWidth, char val)
{
    ASSERT(pFs->flagStarWidth && !pFs->flagStarPrecision);
    pFs->width = (i16)starWidth;
    return specifier_max_size(pFs, val);
}
FormatSize * specifier_max_size_sp(FormatSpecifier * pFs, u32 starPrecision, char val)
{
    ASSERT(!pFs->flagStarWidth && pFs->flagStarPrecision);
    pFs->precision = (i16)starPrecision;
    return specifier_max_size(pFs, val);
}
FormatSize * specifier_max_size_sw_sp(FormatSpecifier * pFs, u32 starWidth, u32 starPrecision, char val)
{
    ASSERT(pFs->flagStarWidth && pFs->flagStarPrecision);
    pFs->width = (i16)starWidth;
    pFs->precision = (i16)starPrecision;
    return specifier_max_size(pFs, val);
}
FormatSize * specifier_max_size(FormatSpecifier * pFs, char val)
{
    switch (pFs->type)
    {
    case 'd':
    case 'i':
        return specifier_max_size(pFs, (i32)val);
    case 'u':
    case 'x':
    case 'X':
    case 'o':
        return specifier_max_size(pFs, (u32)val);
    case 'c':
    {
        i32 size = 1;
        return corrected_size(pFs, size);
    }
    default:
        PANIC("Unexpected format type: %c", pFs->type);
        return nullptr;
    }
}



FormatSize * specifier_max_size_sw(FormatSpecifier * pFs, u32 starWidth, bool val)
{
    ASSERT(pFs->flagStarWidth && !pFs->flagStarPrecision);
    pFs->width = (i16)starWidth;
    return specifier_max_size(pFs, val);
}
FormatSize * specifier_max_size_sp(FormatSpecifier * pFs, u32 starPrecision, bool val)
{
    ASSERT(!pFs->flagStarWidth && pFs->flagStarPrecision);
    pFs->precision = (i16)starPrecision;
    return specifier_max_size(pFs, val);
}
FormatSize * specifier_max_size_sw_sp(FormatSpecifier * pFs, u32 starWidth, u32 starPrecision, bool val)
{
    ASSERT(pFs->flagStarWidth && pFs->flagStarPrecision);
    pFs->width = (i16)starWidth;
    pFs->precision = (i16)starPrecision;
    return specifier_max_size(pFs, val);
}
FormatSize * specifier_max_size(FormatSpecifier * pFs, bool val)
{
    switch (pFs->type)
    {
    case 'd':
    case 'i':
    case 'u':
    case 'x':
    case 'X':
    case 'o':
        return specifier_max_size(pFs, (u32)(val ? 1 : 0));
    case 'b':
    {
        i32 size = val ? 4 : 5; // 'true' or 'false'
        return corrected_size(pFs, size);
    }
    default:
        PANIC("Unexpected format type: %c", pFs->type);
        return nullptr;
    }
}



FormatSize * specifier_max_size_sw(FormatSpecifier * pFs, u32 starWidth, const char * val)
{
    ASSERT(pFs->flagStarWidth && !pFs->flagStarPrecision);
    pFs->width = (i16)starWidth;
    return specifier_max_size(pFs, val);
}
FormatSize * specifier_max_size_sp(FormatSpecifier * pFs, u32 starPrecision, const char * val)
{
    ASSERT(!pFs->flagStarWidth && pFs->flagStarPrecision);
    pFs->precision = (i16)starPrecision;
    return specifier_max_size(pFs, val);
}
FormatSize * specifier_max_size_sw_sp(FormatSpecifier * pFs, u32 starWidth, u32 starPrecision, const char * val)
{
    ASSERT(pFs->flagStarWidth && pFs->flagStarPrecision);
    pFs->width = (i16)starWidth;
    pFs->precision = (i16)starPrecision;
    return specifier_max_size(pFs, val);
}
FormatSize * specifier_max_size(FormatSpecifier * pFs, const char * val)
{
    switch (pFs->type)
    {
    case 's':
    {
        i32 size = (i32)strnlen(val, kMaxCmpStringLength);

        if (pFs->width >= 0)
            size = minval(size, (i32)pFs->width);

        return corrected_size(pFs, size);
    }
    default:
        PANIC("Unexpected format type: %c", pFs->type);
        return nullptr;
    }
}


}
