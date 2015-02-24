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
#include "core/String.h"
#include "core/List.h"

namespace gaen
{

template <MemType memType>
class Config
{
public:
    bool read(std::istream & input);
    bool read(const char * path);

    bool write(std::ostream & output);
    bool write(const char * path);


    const char * get(const char * key);
    const char * get(const char * section, const char * key);

    const char * getWithDefault(const char * key, const char * defaultValue);
    const char * getWithDefault(const char * section, const char * key, const char * defaultValue);

    i32 getInt(const char * key);
    i32 getInt(const char * section, const char * key);

    f32 getFloat(const char * key);
    f32 getFloat(const char * section, const char * key);

    List<memType, String<memType>> getList(const char * key);
    List<memType, String<memType>> getList(const char * section, const char * key);

    List<memType, i32> getIntList(const char * key);
    List<memType, i32> getIntList(const char * section, const char * key);

    List<memType, f32> getFloatList(const char * key);
    List<memType, f32> getFloatList(const char * section, const char * key);

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
