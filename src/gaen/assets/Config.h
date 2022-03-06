//------------------------------------------------------------------------------
// Config.h - Ini-like config file reading and writing
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

#ifndef GAEN_ASSETS_CONFIG_H
#define GAEN_ASSETS_CONFIG_H

#include "gaen/core/mem.h"
#include "gaen/core/HashMap.h"
#include "gaen/core/HashSet.h"
#include "gaen/core/String.h"
#include "gaen/core/List.h"
#include "gaen/core/Vector.h"
#include "gaen/core/Set.h"

namespace gaen
{

static const char * kGlobalSection = "GLOBAL";

bool is_int(const char * val);
i32 to_int(const char * val);

template <MemType memType>
class Config
{
public:
    Config() = default;
    Config(const Config &) = delete;
    Config & operator=(const Config &) = delete;

    typedef Vector<memType, const char*> StringVec;
    typedef Vector<memType, Vector<memType, const char *>> StringVecVec;
    typedef Vector<memType, i32> IntVec;
    typedef Vector<memType, f32> FloatVec;

    void getLine(char * line, u32 maxLine, std::istream & input);
    bool read(std::istream & input);
    bool read(const char * path);

    bool write(std::ostream & output);
    bool write(const char * path);

    // iteration
    typename StringVec::const_iterator sectionsBegin() const;
    typename StringVec::const_iterator sectionsEnd() const;

    typename StringVec::const_iterator keysBegin() const;
    typename StringVec::const_iterator keysBegin(const char * section) const;
    typename StringVec::const_iterator keysEnd() const;
    typename StringVec::const_iterator keysEnd(const char * section) const;

    bool isGlobalSection(const char * section) const;

    // Check for keys and sections
    bool hasSection(const char * section) const;
    bool hasKey(const char * key) const;
    bool hasKey(const char * section, const char * key) const;

    u32 sectionCount() const;
    u32 sectionKeyCount(const char * section) const;

    // Getters
    const char * get(const char * key) const;
    const char * get(const char * section, const char * key) const;

    const char * getWithDefault(const char * key, const char * defaultValue) const;
    const char * getWithDefault(const char * section, const char * key, const char * defaultValue) const;

    bool getBool(const char * key) const;
    bool getBool(const char * section, const char * key) const;

    i32 getInt(const char * key) const;
    i32 getInt(const char * section, const char * key) const;

    f32 getFloat(const char * key) const;
    f32 getFloat(const char * section, const char * key) const;

    StringVec parseVec(const char * val, char delim) const;

    StringVec getVec(const char * key) const;
    StringVec getVec(const char * section, const char * key) const;

    StringVecVec getVecVec(const char * key) const;
    StringVecVec getVecVec(const char * section, const char * key) const;

    IntVec getIntVec(const char * key) const;
    IntVec getIntVec(const char * section, const char * key) const;

    FloatVec getFloatVec(const char * key) const;
    FloatVec getFloatVec(const char * section, const char * key) const;

    void setValueless(const char * key);
    void setValueless(const char * section, const char * key);
    void set(const char * key, const char * value);
    void set(const char * section, const char * key, const char * value);

/*
    void setInt(const char * key, i32 value);
    void setInt(const char * section, const char * key, i32 value);

    void setFloat(const char * key, f32 value);
    void setFloat(const char * section, const char * key, f32 value);

    void setList(const char * key, const List<memType, String<memType>> & value);
    void setList(const char * section, const char * key, const List<memType, String<memType>> & value);

    void setIntList(const char * key, const List<memType, i32> & value);
    void setIntList(const char * section, const char * key, const List<memType, i32> & value);

    void setFloatList(const char * key, const List<memType, f32> & value);
    void setFloatList(const char * section, const char * key, const List<memType, f32> & value);
*/

private:
    // store all strings here once, as strings so destruction cleans up
    typedef HashSet<memType, String<memType>> Names;

    // in other structures, just store pointers to the Names strings
    typedef HashMap<memType, const char*, StringVec, StrHash, StrcmpEqual> SectionKeys;

    typedef HashMap<memType, const char*, const char*, StrHash, StrcmpEqual> KeyValues;
    typedef HashMap<memType, const char*, KeyValues, StrHash, StrcmpEqual> SectionData;


    enum ProcessResultType
    {
        kPRT_Error,
        kPRT_Ignore,
        kPRT_SectionStart,
        kPRT_KeyVal
    };

    struct ProcessResult
    {
        ProcessResultType type;
        const char * lhs;
        const char * rhs;

        ProcessResult(ProcessResultType type, const char * lhs = nullptr, const char * rhs = nullptr)
          : type(type)
          , lhs(lhs)
          , rhs(rhs)
        {}
    };

    void writeKeyVal(std::ostream & output,
                     const char * key,
                     const char * val);

    const char * addName(const char * name);
    ProcessResult processLine(char * line);

    mutable Names mNames;
    StringVec mSectionNames;
    SectionKeys mSectionKeys;
    SectionData mSectionData;
};

} // namespace gaen

#endif // #ifndef GAEN_CORE_CONFIG_H
