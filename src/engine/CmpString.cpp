//------------------------------------------------------------------------------
// CmpString.cpp - String class used through BlockMemory in Compose scripts
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

#include "engine/stdafx.h"

#include "engine/BlockMemory.h"
#include "engine/CmpString.h"

namespace gaen
{

enum SpecParseState
{
    kSPS_Init,
    kSPS_SpecStart,
    kSPS_Width,
    kSPS_Precision
};

CmpString::CmpString(BlockData * pBlockData)
  : mpBlockData(pBlockData)
{
    ASSERT(pBlockData->type == kBKTY_String);
}

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
            c == 'c');
}

// Sanity lengths for badly formed strings
static const u32 kMaxWidth = 128;
static const u32 kMaxPrecision = 128;

const char * find_next_specifier(FormatSpecifier * pFs, const char * str)
{
    if (!str || !*str)
        return nullptr;

    const char * p = str;

    SpecParseState sps = kSPS_Init;
    
    while (*p)
    {
        switch (sps)
        {
        case kSPS_Init:
            if (*p == '%')
            {
                memset(pFs, 0, sizeof(FormatSpecifier));
                pFs->begin = p;
                if (p[1] == '%')
                {
                    // found a double percent, "%%" which we interpret as a literal percent sign
                    pFs->type = '%';
                    pFs->end = p + 2;
                    return pFs->end;
                }
                else
                    sps = kSPS_SpecStart;
            }
            break;
        case kSPS_SpecStart:
            switch (*p)
            {
            case '-':
                pFs->flagMinus = 1;
                break;
            case '+':
                pFs->flagPlus = 1;
                break;
            case ' ':
                pFs->flagSpace = 1;
                break;
            case '#':
                pFs->flagHash = 1;
                break;
            case '0':
                pFs->flagZero = 1;
                break;
            default:
                if (is_type_specifier(*p))
                {
                    pFs->type = *p;
                    pFs->end = p + 1;
                    return pFs->end;
                }
                else if (is_number(*p)) // zero case handled above
                {
                    sps = kSPS_Width;
                    continue;
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
                pFs->type = *p;
                pFs->end = p + 1;
                return pFs->end;
            }
            else if (is_number(*p))
            {
                pFs->width = (pFs->width * 10) + (*p - '0');
                pFs->width = minval(pFs->width, kMaxWidth);
            }
            else if (*p == '.')
            {
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
                pFs->type = *p;
                pFs->end = p + 1;
                return pFs->end;
            }
            else if (is_number(*p))
            {
                pFs->precision = (pFs->precision * 10) + (*p - '0');
                pFs->precision = minval(pFs->precision, kMaxPrecision);
            }
            else
            {
                sps = kSPS_Init;
            }
            break;
        }
        ++p;
    }

    memset(pFs, 0, sizeof(FormatSpecifier));
    return nullptr;
}



} // namespace gaen

