//------------------------------------------------------------------------------
// Config.cpp - Ini-like config file reading and writing
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2016 Lachlan Orr
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

#include "assets/Config.h"

namespace gaen
{

static const u32 kMaxLine = 1024;

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
    return (c == ';');
}

template <MemType memType>
bool Config<memType>::read(std::istream & input)
{
    TLARRAY(char, line, kMaxLine);

    const char * sectionName = addName(kGlobalSection);

    while (!input.eof())
    {
        input.getline(line, kMaxLine);
        if (input.fail())
            break;
        Config<memType>::ProcessResult res = processLine(line);

        switch (res.type)
        {
        case kPRT_KeyVal:
        {
            set(sectionName, res.lhs, res.rhs);
            break;
        }
        case kPRT_SectionStart:
        {
            sectionName = addName(res.lhs);
            break;
        }
        case kPRT_Error:
            ERR("Error parsing config line: %s", line);
            return false;
        }
    }
    return true;
}

template <MemType memType>
bool Config<memType>::read(const char * path)
{
    std::ifstream ifs;
    ifs.open(path, std::ifstream::in | std::ifstream::binary);
    if (!ifs.good())
    {
        ERR("Unable to open config file for reading: %s", path);
        return false;
    }
    return read(ifs);
}

template <MemType memType>
void Config<memType>::writeKeyVal(std::ostream & output,
                                  const char * key,
                                  const char * val)
{
    output.write(key, strlen(key));
    if (val && val[0] != '\0')
    {
        output.put('=');
        output.write(val, strlen(val));
    }
    output.put('\n');
}

template <MemType memType>
bool Config<memType>::write(std::ostream & output)
{
    for (auto section : mSectionNames)
    {
        if (0 != strcmp(section, kGlobalSection))
        {
            output.put('[');
            output.write(section, strlen(section));
            output.put(']');
            output.put('\n');
        }

        for (auto keyValIt : mSectionData[section])
            writeKeyVal(output, keyValIt.first, keyValIt.second);
    }

    return true;
}

template <MemType memType>
bool Config<memType>::write(const char * path)
{
    std::ofstream ofs;
    ofs.open(path, std::ifstream::out | std::ifstream::binary);
    if (!ofs.good())
    {
        ERR("Unable to open config file writing: %s", path);
        return false;
    }
    return write(ofs);
}



template <MemType memType>
typename Config<memType>::StringVec::const_iterator Config<memType>::sectionsBegin() const
{
    return mSectionNames.begin();
}

template <MemType memType>
typename Config<memType>::StringVec::const_iterator Config<memType>::sectionsEnd() const
{
    return mSectionNames.end();
}

template <MemType memType>
typename Config<memType>::StringVec::const_iterator Config<memType>::keysBegin() const
{
    return keysBegin(kGlobalSection);
}

template <MemType memType>
typename Config<memType>::StringVec::const_iterator Config<memType>::keysBegin(const char * section) const
{
    auto it = mSectionKeys.find(section);
    PANIC_IF(it == mSectionKeys.end(), "Section %s does not exist", section);
    return it->second.begin();
}

template <MemType memType>
typename Config<memType>::StringVec::const_iterator Config<memType>::keysEnd() const
{
    return keysEnd(kGlobalSection);
}

template <MemType memType>
typename Config<memType>::StringVec::const_iterator Config<memType>::keysEnd(const char * section) const
{
    auto it = mSectionKeys.find(section);
    PANIC_IF(it == mSectionKeys.end(), "Section %s does not exist", section);
    return it->second.end();
}

template <MemType memType>
bool Config<memType>::isGlobalSection(const char * section) const
{
    ASSERT(section);
    return 0 == strcmp(section, kGlobalSection);
}

template <MemType memType>
bool Config<memType>::hasSection(const char * section) const
{
    auto it = mSectionData.find(section);
    return it != mSectionData.end();
}

template <MemType memType>
bool Config<memType>::hasKey(const char * key) const
{
    return hasKey(kGlobalSection, key);
}

template <MemType memType>
bool Config<memType>::hasKey(const char * section, const char * key) const
{
    auto it = mSectionData.find(section);
    if (it == mSectionData.end())
        return false;
    auto keyIt = it->second.find(key);
    return keyIt != it->second.end();
}

template <MemType memType>
u32 Config<memType>::sectionCount() const
{
    return (u32)mSectionData.size();
}

template <MemType memType>
u32 Config<memType>::sectionKeyCount(const char * section) const
{
    auto it = mSectionData.find(section);
    if (it != mSectionData.end())
        return (u32)it->second.size();
    else
        return 0;
}


template <MemType memType>
const char * Config<memType>::get(const char * key) const
{
    return get(kGlobalSection, key);
}

template <MemType memType>
const char * Config<memType>::get(const char * section, const char * key) const
{
    auto sectionIt = mSectionData.find(section);
    if (sectionIt == mSectionData.end())
        return nullptr;

    auto it = sectionIt->second.find(key);
    if (it == sectionIt->second.end())
        return nullptr;
    
    return it->second;
}

template <MemType memType>
const char * Config<memType>::getWithDefault(const char * key, const char * defaultValue) const
{
    return getWithDefault(kGlobalSection, key, defaultValue);
}

template <MemType memType>
const char * Config<memType>::getWithDefault(const char * section, const char * key, const char * defaultValue) const
{
    auto sectionIt = mSectionData.find(section);
    if (sectionIt == mSectionData.end())
    {
        return defaultValue;
    }

    auto it = sectionIt->second.find(key);
    if (it == sectionIt->second.end())
    {
        return defaultValue;
    }

    return it->second;
}

template <MemType memType>
bool Config<memType>::getBool(const char * key) const
{
    return getBool(kGlobalSection, key);
}

template <MemType memType>
bool Config<memType>::getBool(const char * section,const char * key) const
{
    const char * val = get(section, key);
    if (!val)
        return false;

    static const char * kTrue = "true";
    u32 i;
    for (i = 0; i < 4; ++i)
    {
        if (tolower(val[i]) != kTrue[i])
            return false;
    }
    return val[i] == '\0'; // make sure val didn't just start with "true"
}

template <MemType memType>
i32 Config<memType>::getInt(const char * key) const
{
    return getInt(kGlobalSection, key);
}

template <MemType memType>
i32 Config<memType>::getInt(const char * section, const char * key) const
{
    const char * val = get(section, key);
    return static_cast<i32>(strtol(val, nullptr, 0));
}

template <MemType memType>
f32 Config<memType>::getFloat(const char * key) const
{
    return getFloat(kGlobalSection, key);
}

template <MemType memType>
f32 Config<memType>::getFloat(const char * section, const char * key) const
{
    const char * val = get(section, key);
    return static_cast<float>(strtod(val, nullptr));
}

template <MemType memType>
typename Config<memType>::StringVec Config<memType>::getVec(const char * key) const
{
    return getVec(kGlobalSection, key);
}

template <MemType memType>
typename Config<memType>::StringVec Config<memType>::getVec(const char * section, const char * key) const
{
    StringVec vec;
    const char * val = get(section, key);
    const char * comma = strchr(val, ',');
    if (!comma)
    {
        auto it = mNames.emplace(val);
        vec.emplace_back(it.first->c_str());
    }
    else
    {
        while (comma)
        {
            auto it = mNames.emplace(val, comma-val);
            vec.emplace_back(it.first->c_str());
            val = comma+1;
            comma = strchr(val, ',');

            // grab last value after last comma
            if (!comma && strlen(val) > 0)
            {
                auto itLast = mNames.emplace(val);
                vec.emplace_back(itLast.first->c_str());
            }
        }
    }
    return vec;
}

template <MemType memType>
typename Config<memType>::IntVec Config<memType>::getIntVec(const char * key) const
{
    return getIntVec(kGlobalSection, key);
}

template <MemType memType>
typename Config<memType>::IntVec Config<memType>::getIntVec(const char * section, const char * key) const
{
    StringVec strVec = getVec(section, key);
    IntVec vec;
    for (auto str : strVec)
        vec.push_back(static_cast<i32>(strtol(str, nullptr, 0)));
    return vec;
}

template <MemType memType>
typename Config<memType>::FloatVec Config<memType>::getFloatVec(const char * key) const
{
    return getFloatVec(kGlobalSection, key);
}

template <MemType memType>
typename Config<memType>::FloatVec Config<memType>::getFloatVec(const char * section, const char * key) const
{
    StringVec strVec = getVec(section, key);
    FloatVec vec;
    for (auto str : strVec)
        vec.push_back(static_cast<f32>(strtod(str, nullptr)));
    return vec;
}

template <MemType memType>
void Config<memType>::setValueless(const char * key)
{
    set(key, "");
}

template <MemType memType>
void Config<memType>::setValueless(const char * section, const char * key)
{
    set(section, key, "");
}

template <MemType memType>
void Config<memType>::set(const char * key, const char * value)
{
    set(kGlobalSection, key, value);
}

template <MemType memType>
void Config<memType>::set(const char * section, const char * key, const char * value)
{
    section = addName(section);
    if (!hasSection(section))
        mSectionNames.push_back(section);
    key = addName(key);
    value = addName(value);
    mSectionKeys[section].push_back(key);
    mSectionData[section][key] = value;
}

/*

template <MemType memType>
void setInt(const char * key, i32 value)
{
    setInt(kGlobalSection, key, value);
}

template <MemType memType>
void setInt(const char * section, const char * key, i32 value)
{

}

template <MemType memType>
void setFloat(const char * key, f32 value)
{
    setFloat(kGlobalSection, key, value);
}

template <MemType memType>
void setFloat(const char * section, const char * key, f32 value)
{

}

template <MemType memType>
void setList(const char * key, const List<memType, String<memType>> & value)
{
    setList(kGlobalSection, key, value);
}

template <MemType memType>
void setList(const char * section, const char * key, const List<memType, String<memType>> & value)
{

}

template <MemType memType>
void setIntList(const char * key, const List<memType, i32> & value)
{
    setIntList(kGlobalSection, key, value);
}

template <MemType memType>
void setIntList(const char * section, const char * key, const List<memType, i32> & value)
{

}

template <MemType memType>
void setFloatList(const char * key, const List<memType, f32> & value)
{
    setFloatList(kGlobalSection, key, value);
}

template <MemType memType>
void setFloatList(const char * section, const char * key, const List<memType, f32> & value)
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

static void strip_comment(char * str)
{
    char * s = str;
    while (*s)
    {
        // it's a start comment char and either first char or not
        // preceded by '\'
        if (is_comment_start(*s) && (s == str || s[-1] != '\\'))
        {
            *s = '\0';
            return;
        }
        s++;
    }
}

static char * find_eq(char * line)
{
    // find first equal not prepended with '\'
    char * s = line;
    while (*s)
    {
        // it's '=' and either first char or not preceded by '\'
        if (*s == '=' && (s == line || s[-1] != '\\'))
            return s;
        s++;
    }
    return nullptr;
}

static void de_escape(char * line)
{
    const char * s = line;
    char * d = line;

    while (*s)
    {
        if (*s == '\\')
        {
            switch (s[1])
            {
            case 's':
                *d = ' ';
                break;
            case 't':
                *d = '\t';
                break;
            case 'n':
                *d = '\n';
                break;
            case 'r':
                *d = '\r';
                break;
            case 'f':
                *d = '\f';
                break;
            case 'x':
            {
                PANIC_IF(!s[2] || !s[3], "End of line encountered within \\x character literal");
                char hex[3];
                hex[0] = s[2];
                hex[1] = s[3];
                hex[2] = '\0';
                *d = (char)strtoul(hex, nullptr, 16);
                s+=2; // need to go past 2 digit hex value, \x11, for example
                break;
            }
            default:
                *d = s[1];
                break;
            }
            d++;
            s+=2;
        }
        else
        {
            *d = *s;
            d++;
            s++;
        }
    }
    *d = '\0';
}

template <MemType memType>
const char * Config<memType>::addName(const char * name)
{
    return mNames.emplace(name).first->c_str();
}

template <MemType memType>
typename Config<memType>::ProcessResult Config<memType>::processLine(char * line)
{
    line = strip(line);
    strip_comment(line);
    
    // if it's blank we can skip it
    if (*line == '\0')
        return ProcessResult(kPRT_Ignore);

    // de-escape any escaped chars
    char * eq = find_eq(line);

    // A key/value pair
    if (eq)
    {
        // turn '=' into a null 
        char * lhs = line;
        char * rhs = eq+1;
        *eq = '\0';
        lhs = strip(lhs);
        de_escape(lhs);
        rhs = strip(rhs);
        de_escape(rhs);
        return ProcessResult(kPRT_KeyVal, lhs, rhs);
    }

    // A section header
    size_t lineLen = strlen(line);
    if (lineLen > 3 && line[0] == '[' && line[lineLen-1] == ']' && line[lineLen-2] != '\\')
    {
        line[lineLen-1] = '\0';
        line++;
        line = strip(line);
        de_escape(line);
        return ProcessResult(kPRT_SectionStart, line);
    }

    // A line without a value is interpreted as a valueless key/value pair
    de_escape(line);
    return ProcessResult(kPRT_KeyVal, line, "");
}

// Template definitions for linker.
// Add more kMEM_* mem types here as required.
template class Config<kMEM_Chef>;

} // namespace gaen
