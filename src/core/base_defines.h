//------------------------------------------------------------------------------
// base_defines.h - Commonly used definitions used throughout Gaen
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

#ifndef GAEN_CORE_BASE_DEFINES_H
#define GAEN_CORE_BASE_DEFINES_H

#include <cstdint>
#include <cstdlib>
#include <utility>

namespace gaen
{

// These HAS/WHEN macros ensure that things are either
// defined as true or false, but aren't false by default
// simply because they weren't defined.
// Use them as such:
//#define RENDERTHREAD_DEBUG_MARKS  HAS_X
//#define TRACK_MEM                 WHEN(HAS(DEV_BUILD))
//#define RT_LOGGING                HAS__
#define HAS__	1
#define HAS_X	2

#define	HAS(pred)     (1 % (pred))
#define	WHEN(pred)    (1 + (pred))



// The following are defined through cmake.
// They're included here for reference.

// One of the following compiler defines
//   IS_COMPILER_MSVC
//   IS_COMPILER_GNU
//   IS_COMPILER_CLANG
//   IS_COMPILER_INTEL

// One of the following build type defines
//   IS_BUILD_Debug
//   IS_BUILD_RelWithDebInfo
//   IS_BUILD_Release
//   IS_BUILD_MinSizeRel
//   IS_BUILD_Profile

// One or more of the following platform defines
//   IS_PLATFORM_WIN32
//   IS_PLATFORM_OSX
//   IS_PLATFORM_POSIX



// Prep some stuff based on compiler/platform.

// LORRTODO - Check to make sure we're amd64... we're using intrinsics and assuming little endian

#if IS_PLATFORM_WIN32
 #ifdef _WIN64
  #define WORD_SIZE_64BIT HAS_X
  #define WORD_SIZE_32BIT HAS__
 #else
  #define WORD_SIZE_64BIT HAS__
  #define WORD_SIZE_32BIT HAS_X
 #endif
#elif IS_PLATFORM_OSX || IS_PLATFORM_IOS
 #ifdef __x86_64__
  #define WORD_SIZE_64BIT HAS_X
  #define WORD_SIZE_32BIT HAS__
 #else
  #define WORD_SIZE_64BIT HAS__
  #define WORD_SIZE_32BIT HAS_X
 #endif
#else
 #error Need macros for word size for this platform
#endif

#if HAS(WORD_SIZE_32BIT)
#define PAD_IF_32BIT u32 PAD_FOR_32BIT;
#define PAD_IF_32BIT_A u32 PAD_FOR_32BIT_A;
#define PAD_IF_32BIT_B u32 PAD_FOR_32BIT_B;
#define PAD_IF_32BIT_C u32 PAD_FOR_32BIT_C;
#else
#define PAD_IF_32BIT
#define PAD_IF_32BIT_A
#define PAD_IF_32BIT_B
#define PAD_IF_32BIT_C
#endif

// Debug and RelWithDebInfo are considered Dev builds.  This means
// they will log and support console connections and compile asserts.
#if IS_BUILD_Debug || IS_BUILD_RelWithDebInfo
#define DEV_BUILD HAS_X
#else
#define DEV_BUILD HAS__
#endif

// LORRTODO - define Profile build type in CMakeLists.txt
#if IS_BUILD_Profile || HAS(DEV_BUILD)
#define PROFILE HAS_X
#else
#define PROFILE HAS__
#endif

// Determines if memory allocs/deallocs are tracked for reporting.
#define TRACK_MEM WHEN(HAS(DEV_BUILD))

// Platform specific way to halt on assert and breatk into debugger.
// If you want to override about/break functions (as you might in unit
// tests) call override_failure_funcs with your own functions.
typedef void (*DebugBreakFunc)();
typedef void (*AbortFunc)();
void debug_break_gaen();
void abort_gaen();
void override_failure_funcs(DebugBreakFunc debugBreakFunc, AbortFunc abortFunc);

#if HAS(DEV_BUILD)
#define HALT() gaen::debug_break_gaen()
#else  // #if HAS(DEV_BUILD)
#define HALT() gaen::abort_gaen()
#endif // #if HAS(DEV_BUILD)

//------------------------------------------------------------------------------
// Assert and Error Macros
//------------------------------------------------------------------------------
// ASSERT will halt if condition is true.
// EXPECT will report an error if condition is true, but not halt.
// PANIC unconditionally halts, supported in all build types so use sparingly.
#define ASSERTS WHEN(HAS(DEV_BUILD))

typedef void (*ReportFailureCB)(const char * msg);
void set_report_failure_cb(ReportFailureCB reportFailureCB);
void report_failure(const char * condition,
                    const char * file,
                    int line,
                    const char * format,
                    ...);

#if HAS(ASSERTS)
 #define ASSERT_MSG(condition, format, ...) \
 do \
 { \
     if (!(condition)) \
     { \
         gaen::report_failure(#condition, __FILE__, __LINE__, format, ##__VA_ARGS__); \
         HALT(); \
     } \
 } while(0)

 #define EXPECT_MSG(condition, format, ...) \
 do \
 { \
     if (!(condition)) \
     { \
         gaen::report_failure(#condition, __FILE__, __LINE__, format, ##__VA_ARGS__); \
     } \
 } while(0)

#else  // #if HAS(ASSERTS)
 #define ASSERT_MSG(condition, ...) do {} while(0)
 #define EXPECT_MSG(condition, ...) do {} while(0)
#endif // #if HAS(ASSERTS)

// Non message versions
#define ASSERT(condition) ASSERT_MSG(condition, "")
#define EXPECT(condition) EXPECT_MSG(condition, "")

#define PANIC(format, ...) \
do \
{ \
    gaen::report_failure("PANIC", __FILE__, __LINE__, format, ##__VA_ARGS__); \
    gaen::abort_gaen(); \
} while(0)

#define PANIC_IF(condition, format, ...) \
do \
{ \
    if (condition) \
    { \
        gaen::report_failure(#condition, __FILE__, __LINE__, format, ##__VA_ARGS__); \
        HALT(); \
    } \
} while(0)

#define ERR(format, ...) \
do \
{ \
    gaen::report_failure("ERROR", __FILE__, __LINE__, format, ##__VA_ARGS__); \
} while(0)

#define ERR_IF(condition, format, ...) \
do \
{ \
    if (condition) \
    { \
        gaen::report_failure(#condition, __FILE__, __LINE__, format, ##__VA_ARGS__); \
    } \
} while(0)

//------------------------------------------------------------------------------
// Assert and Error Macros (END)
//------------------------------------------------------------------------------



// Base types, nice and concise
typedef std::int8_t      i8;
typedef std::uint8_t     u8;

typedef std::int16_t    i16;
typedef std::uint16_t   u16;

typedef std::int32_t    i32;
typedef std::uint32_t   u32;

typedef std::int64_t    i64;
typedef std::uint64_t   u64;

typedef float           f32;
typedef double          f64;

#if IS_LITTLE_ENDIAN
#define FOURCC(s) ((s[3]) << 24 | (s[2]) << 16 | (s[1]) << 8 | (s[0]))
#else // little endian
#define FOURCC(s) ((s[0]) << 24 | (s[1]) << 16 | (s[2]) << 8 | (s[3]))
#endif

// Use this function at runtime with strings.
template <class T>
inline u32 fourcc(const T & s)
{
    PANIC_IF(s.size() != 4, "fourcc called with string length not equal to 4");
    return FOURCC(s);
}

#define BYTESWAP32(val) ((((val) >> 24) & 0xff) | (((val) >> 8)  & 0xff00) | (((u32)(val) << 8)  & 0xff0000) | (((u32)(val) << 24) & 0xff000000))

// Align a value to specified alignment.  Useful to ensure memory
// alignment in custom allocators and things like that.
template <typename T>
inline T align(T val, u32 alignment)
{
    return (val + (T)alignment-1) & ~((T)alignment-1);
}

inline bool is_power_of_two(u32 val)
{
    return (val & (val-1)) == 0;
}

inline u32 next_power_of_two(u32 val)
{
    val--;
    val |= val >> 1;
    val |= val >> 2;
    val |= val >> 4;
    val |= val >> 8;
    val |= val >> 16;
    val++;
    return val;
}

//------------------------------------------------------------------------------
// Singleton wrappers
//
// Use these functions to obtain references to the singleton of each type.
//  singleton - one global instance
//  thread_singleton - one instance per thread
//------------------------------------------------------------------------------
template <class T>
T & singleton()
{
    static T obj;
    return obj;
}


inline f32 clamp(f32 val, f32 min, f32 max)
{
    if (val < min)
        return min;
    else if (val > max)
        return max;
    return val;
}


} // namespace gaen


#endif // GAEN_CORE_BASE_DEFINES_H
