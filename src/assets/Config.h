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

#ifndef GAEN_ASSETS_CONFIG_H
#define GAEN_ASSETS_CONFIG_H

#include "core/mem.h"
#include "core/HashMap.h"
#include "core/HashSet.h"
#include "core/String.h"
#include "core/List.h"
#include "core/Vector.h"
#include "core/Set.h"

namespace gaen
{

template <MemType memType>
class Config
{
public:
    typedef Vector<memType, const char*> StringVec;
    typedef Vector<memType, i32> IntVec;
    typedef Vector<memType, f32> FloatVec;

    bool read(std::istream & input);
    bool read(const char * path);

    bool write(std::ostream & output);
    bool write(const char * path);

    // iteration
    typename StringVec::const_iterator sectionsBegin();
    typename StringVec::const_iterator sectionsEnd();

    typename StringVec::const_iterator keysBegin();
    typename StringVec::const_iterator keysBegin(const char * section);
    typename StringVec::const_iterator keysEnd(const char * section);

    // Check for keys and sections
    bool hasSection(const char * section);
    bool hasKey(const char * key);
    bool hasKey(const char * section, const char * key);

    // Getters
    const char * get(const char * key);
    const char * get(const char * section, const char * key);

    const char * getWithDefault(const char * key, const char * defaultValue);
    const char * getWithDefault(const char * section, const char * key, const char * defaultValue);

    i32 getInt(const char * key);
    i32 getInt(const char * section, const char * key);

    f32 getFloat(const char * key);
    f32 getFloat(const char * section, const char * key);

    StringVec getVec(const char * key);
    StringVec getVec(const char * section, const char * key);

    IntVec getIntVec(const char * key);
    IntVec getIntVec(const char * section, const char * key);

    FloatVec getFloatVec(const char * key);
    FloatVec getFloatVec(const char * section, const char * key);

/*
    void set(const char * key, const char * value);
    void set(const char * section, const char * key, const char * value);

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

    const char * addName(const char * name);
    ProcessResult processLine(char * line);

    Names mNames;
    StringVec mSectionNames;
    SectionKeys mSectionKeys;
    SectionData mSectionData;
};

} // namespace gaen

#endif // #ifndef GAEN_CORE_CONFIG_H
