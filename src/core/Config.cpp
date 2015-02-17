//------------------------------------------------------------------------------
// Config.h - Ini-like config file reading and writing
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

#include "core/stdafx.h"

#include "core/base_defines.h"
#include "core/thread_local.h"

#include "core/Config.h"

namespace gaen
{

template <MemType memType>
const String<memType> Config<memType>::kGlobalSection = "GLOBAL";

template <MemType memType>
const String<memType> Config<memType>::kEmptyValue = "";

static inline bool is_whitespace(char c)
{
    return (c == ' ' ||
            c == '\t' ||
            c == '\n' ||
            c == '\r' ||
            c == '\f');
}

static inline bool is_comment_start(char c)
{
    return (c == '#');
}

template <MemType memType>
bool Config<memType>::read(std::istream & input)
{
    static const u32 kMaxLine = 1024;
    TLARRAY(char, line, kMaxLine);

    mSections.clear();

    String<memType> sectionName = kGlobalSection;

    while (!input.eof())
    {
        input.getline(line, kMaxLine);
        if (input.fail())
            break;
        Config<memType>::ProcessResult res = processLine(line);

        switch (res.type)
        {
        case kPRT_KeyVal:
            mSections[sectionName][res.lhs] = res.rhs;
            break;
        case kPRT_SectionStart:
            sectionName = res.lhs;
            break;
        case kPRT_Error:
            ERR("Error parsing config line: %s", line);
            mSections.clear();
            return false;
        }
    }
    return true;
}

template <MemType memType>
bool Config<memType>::read(const String<memType> & path)
{
    std::ifstream ifs;
    ifs.open(path.c_str(), std::ifstream::in | std::ifstream::binary);
    if (!ifs.good())
    {
        ERR("Unable to open config file: %s", path);
        return false;
    }
    return read(ifs);
}

template <MemType memType>
bool Config<memType>::write(std::ostream & output)
{
    PANIC("Not implemented");
    return false;
}

template <MemType memType>
bool Config<memType>::write(const String<memType> & path)
{
    PANIC("Not implemented");
    return false;
}

template <MemType memType>
const String<memType> & Config<memType>::get(const String<memType> & key)
{
    return get(kGlobalSection, key);
}

template <MemType memType>
const String<memType> & Config<memType>::get(const String<memType> & section, const String<memType> & key)
{
    auto sectionIt = mSections.find(section);
    if (sectionIt == mSections.end())
    {
        PANIC("Section %s does not exist", section);
        return kEmptyValue;
    }

    auto it = sectionIt->second.find(key);
    if (it == sectionIt->second.end())
    {
        PANIC("Key %s does not exist in section %s", key, section);
        return kEmptyValue;
    }
    
    return it->second;
}

template <MemType memType>
i32 Config<memType>::getInt(const String<memType> & key)
{
    return getInt(kGlobalSection, key);
}

template <MemType memType>
i32 Config<memType>::getInt(const String<memType> & section, const String<memType> & key)
{
    const String<memType> & val = get(section, key);
    return static_cast<i32>(strtol(val.c_str(), nullptr, 0));
}

template <MemType memType>
f32 Config<memType>::getFloat(const String<memType> & key)
{
    return getFloat(kGlobalSection, key);
}

template <MemType memType>
f32 Config<memType>::getFloat(const String<memType> & section, const String<memType> & key)
{
    const String<memType> & val = get(section, key);
    return static_cast<float>(strtod(val.c_str(), nullptr));
}

template <MemType memType>
List<memType, String<memType>> Config<memType>::getList(const String<memType> & key)
{
    return getList(kGlobalSection, key);
}

template <MemType memType>
List<memType, String<memType>> Config<memType>::getList(const String<memType> & section, const String<memType> & key)
{
    List<memType, String<memType>> list;
    const String<memType> & valStr = get(section, key);
    const char * val = valStr.c_str();
    const char * comma = strchr(val, ',');
    while (comma)
    {
        list.emplace_back(String<memType>(val, comma-val));
        val = comma+1;
        comma = strchr(val, ',');
    }
    return list;
}

template <MemType memType>
List<memType, i32> Config<memType>::getIntList(const String<memType> & key)
{
    return getIntList(kGlobalSection, key);
}

template <MemType memType>
List<memType, i32> Config<memType>::getIntList(const String<memType> & section, const String<memType> & key)
{
    auto strList = getList(section, key);
    List<memType, i32> list;
    for (auto str : strList)
        list.push_back(static_cast<i32>(strtol(str.c_str(), nullptr, 0)));
    return list;
}

template <MemType memType>
List<memType, f32> Config<memType>::getFloatList(const String<memType> & key)
{
    return getFloatList(kGlobalSection, key);
}

template <MemType memType>
List<memType, f32> Config<memType>::getFloatList(const String<memType> & section, const String<memType> & key)
{
    auto strList = getList(section, key);
    List<memType, f32> list;
    for (auto str : strList)
        list.push_back(static_cast<f32>(strtod(str.c_str(), nullptr)));
    return list;
}




/*
template <MemType memType>
void set(const String<memType> & key, const String<memType> & value)
{
    set(kGlobalSection, key, value);
}

template <MemType memType>
void set(const String<memType> & section, const String<memType> & key, const String<memType> & value)
{
    
}

template <MemType memType>
void setInt(const String<memType> & key, i32 value)
{
    setInt(kGlobalSection, key, value);
}

template <MemType memType>
void setInt(const String<memType> & section, const String<memType> & key, i32 value)
{

}

template <MemType memType>
void setFloat(const String<memType> & key, f32 value)
{
    setFloat(kGlobalSection, key, value);
}

template <MemType memType>
void setFloat(const String<memType> & section, const String<memType> & key, f32 value)
{

}

template <MemType memType>
void setList(const String<memType> & key, const List<memType, String<memType>> & value)
{
    setList(kGlobalSection, key, value);
}

template <MemType memType>
void setList(const String<memType> & section, const String<memType> & key, const List<memType, String<memType>> & value)
{

}

template <MemType memType>
void setIntList(const String<memType> & key, const List<memType, i32> & value)
{
    setIntList(kGlobalSection, key, value);
}

template <MemType memType>
void setIntList(const String<memType> & section, const String<memType> & key, const List<memType, i32> & value)
{

}

template <MemType memType>
void setFloatList(const String<memType> & key, const List<memType, f32> & value)
{
    setFloatList(kGlobalSection, key, value);
}

template <MemType memType>
void setFloatList(const String<memType> & section, const String<memType> & key, const List<memType, f32> & value)
{

}
*/


// destructive strip
static char * strip(char * str)
{
    // strip any whitespace at start
    while (*str && is_whitespace(*str))
        ++str;

    // remove any trailing whitespace
    size_t strLen = strlen(str);
    char * end = str + strLen-1;
    while (end > str && is_whitespace(*end))
        --end;

    // null terminate at end of non-whitespace
    end[1] = '\0';

    return str;
}

template <MemType memType>
typename Config<memType>::ProcessResult Config<memType>::processLine(char * line)
{
    line = strip(line);
    
    // if it's blank or a comment, we can skip it
    if (*line == '\0' || is_comment_start(*line))
        return ProcessResult(kPRT_Ignore);

    // Find the equal sign
    char * eq = strchr(line, '=');
    if (eq)
    {
        // turn '=' into a null 
        char * lhs = line;
        char * rhs = eq+1;
        *eq = '\0';
        lhs = strip(lhs);
        rhs = strip(rhs);
        return ProcessResult(kPRT_KeyVal, lhs, rhs);
    }

    size_t lineLen = strlen(line);
    if (line[0] == '[' && line[lineLen-1] == ']')
    {
        line[lineLen-1] = '\0';
        line++;
        return ProcessResult(kPRT_SectionStart, line);
    }

    return ProcessResult(kPRT_Error);
}

// Template definitions for linker.
// Add more kMEM_* mem types here as required.
template class Config<kMEM_Chef>;

} // namespace gaen
