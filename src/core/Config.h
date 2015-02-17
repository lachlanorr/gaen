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

#ifndef GAEN_CORE_CONFIG_H
#define GAEN_CORE_CONFIG_H

#include "core/mem.h"
#include "core/HashMap.h"
#include "core/String.h"
#include "core/List.h"

namespace gaen
{

template <MemType memType>
class Config
{
public:
    bool read(std::istream & input);
    bool read(const String<memType> & path);

    bool write(std::ostream & output);
    bool write(const String<memType> & path);


    const String<memType> & get(const String<memType> & key);
    const String<memType> & get(const String<memType> & section, const String<memType> & key);

    i32 getInt(const String<memType> & key);
    i32 getInt(const String<memType> & section, const String<memType> & key);

    f32 getFloat(const String<memType> & key);
    f32 getFloat(const String<memType> & section, const String<memType> & key);

    List<memType, String<memType>> getList(const String<memType> & key);
    List<memType, String<memType>> getList(const String<memType> & section, const String<memType> & key);

    List<memType, i32> getIntList(const String<memType> & key);
    List<memType, i32> getIntList(const String<memType> & section, const String<memType> & key);

    List<memType, f32> getFloatList(const String<memType> & key);
    List<memType, f32> getFloatList(const String<memType> & section, const String<memType> & key);

/*
    void set(const String<memType> & key, const String<memType> & value);
    void set(const String<memType> & section, const String<memType> & key, const String<memType> & value);

    void setInt(const String<memType> & key, i32 value);
    void setInt(const String<memType> & section, const String<memType> & key, i32 value);

    void setFloat(const String<memType> & key, f32 value);
    void setFloat(const String<memType> & section, const String<memType> & key, f32 value);

    void setList(const String<memType> & key, const List<memType, String<memType>> & value);
    void setList(const String<memType> & section, const String<memType> & key, const List<memType, String<memType>> & value);

    void setIntList(const String<memType> & key, const List<memType, i32> & value);
    void setIntList(const String<memType> & section, const String<memType> & key, const List<memType, i32> & value);

    void setFloatList(const String<memType> & key, const List<memType, f32> & value);
    void setFloatList(const String<memType> & section, const String<memType> & key, const List<memType, f32> & value);
*/

private:
    static const String<memType> kGlobalSection;
    static const String<memType> kEmptyValue;

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

    ProcessResult processLine(char * line);

    typedef HashMap<memType, String<memType>, String<memType>> Section;
    typedef HashMap<memType, String<memType>, Section> Sections;

    Sections mSections;
};

} // namespace gaen

#endif // #ifndef GAEN_CORE_CONFIG_H
