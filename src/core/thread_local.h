//------------------------------------------------------------------------------
// thread_local.h - Thread local storage
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

#ifndef GAEN_CORE_THREAD_LOCAL_H
#define GAEN_CORE_THREAD_LOCAL_H


// Special thread local handling.
// GCC     - thread_local works: as C++11 promises
// Windows - __declspec(thread): almost the same thing
// Clang   - __thread: almost the same thing
// iOS     - Our own ThreadLocal class: baling wire, duct tape, misery
#if IS_PLATFORM_WIN32
#define TL(T, N) static __declspec(thread) T N
#define TLARRAY(T, N, C) static __declspec(thread) T N[C]
#elif IS_PLATFORM_OSX
#define TL(T, N) static __thread T N
#define TLARRAY(T, N, C) static __thread T N[C]
#elif IS_PLATFORM_IOS
#define TL(T, N) ThreadLocal<T, __COUNTER__, __LINE__> N
#define TLARRAY(T, N, C) ThreadLocalArray<T, C, __COUNTER__, __LINE__> N
#else
#define TL(T, N) static thread_local T N
#define TLARRAY(T, N, C) static thread_local T N[C]
#endif


#if IS_PLATFORM_IOS

// IOS ThreadLocal support
// When that platform supports thread_local, or __thread,
// we can get rid of this stuff. But for now, we wrap the
// posix thread local stuff with this class and for the
// most part it acts a lot like a thread_local declared
// variable syntactically.

// The 'int ID' template param is to ensure each template
// isntantiation is unique, which is accomplished by passing
// in __COUNTER__ for that param. See TL and TLARRAY macro
// definitions in base_defines.h.

#include <pthread.h>

#include "core/base_defines.h"
#include "core/mem.h"

namespace gaen
{

template <typename T, int ID, int Line>
class ThreadLocal
{
public:
    typedef T DataType_t;

    ThreadLocal()
    {
        init();
    }

    ThreadLocal(T val)
    {
        //printf("%d/%d(%x) - Construct: %u\n", ID, Line, pthread_mach_thread_np(pthread_self()), *(u32*)&val);
        sInitVal = val;
        init();
    }

    T& operator=(T rhs)
    {
        //printf("%d/%d(%x) - Assignment: %u\n", ID, Line, pthread_mach_thread_np(pthread_self()), *(u32*)&rhs);
        set_value(rhs);
        return get_value();
    }

    bool operator==(T rhs)
    {
        return get_value() == rhs;
    }

    bool operator!=(T rhs)
    {
        return get_value() != rhs;
    }

    operator T&()
    {
        return get_value();
    }

private:
    static void init()
    {
        static pthread_once_t sOnce = PTHREAD_ONCE_INIT;
        pthread_once(&sOnce, key_create);
    }

    static void key_create()
    {
        pthread_key_create(&sKey, 0);
        //printf("%d/%d(%x) - key_create: %u\n", ID, Line, pthread_mach_thread_np(pthread_self()), *(u32*)&sInitVal);
    }

    static T* get_pointer()
    {
        T* pVal = reinterpret_cast<T*>(pthread_getspecific(sKey));
        if (pVal == nullptr)
        {
            pVal = reinterpret_cast<T*>(GALLOC(kMEM_Engine, sizeof(T)));
            *pVal = sInitVal;
            int retval = pthread_setspecific(sKey, pVal);
            ASSERT(retval == 0);
        }
        //printf("%d/%d(%x) - get_pointer: %u\n", ID, Line, pthread_mach_thread_np(pthread_self()), *(u32*)pVal);
        return pVal;
    }

    static T& get_value()
    {
        return *get_pointer();
    }

    static void set_value(T val)
    {
        //printf("%d/%d(%x) - set_value: %u\n", ID, Line, pthread_mach_thread_np(pthread_self()), *(u32*)&val);
        *get_pointer() = val;
    }

    static pthread_key_t sKey;
    static T sInitVal;
};
template <typename T, int ID, int Line>
pthread_key_t ThreadLocal<T, ID, Line>::sKey = 0;
template <typename T, int ID, int Line>
T ThreadLocal<T, ID, Line>::sInitVal;


template <typename T, size_t N, int ID, int Line>
class ThreadLocalArray
{
public:
    typedef T DataType_t;
    static const size_t kCount = N;
    static const size_t kSize = N * sizeof(T);

    ThreadLocalArray()
    {
        init();
    }

    operator T*()
    {
        return get_pointer();
    }

    T& operator[] (int idx)
    {
        return get_value(idx);
    }

public:
    static void init()
    {
        static pthread_once_t sOnce = PTHREAD_ONCE_INIT;
        pthread_once(&sOnce, key_create);
    }

    static void key_create()
    {
        pthread_key_create(&sKey, 0);
    }

    static T* get_pointer()
    {
        T* pVal = reinterpret_cast<T*>(pthread_getspecific(sKey));
        if (pVal == nullptr)
            pVal = reinterpret_cast<T*>(GALLOC(kMEM_Engine, sizeof(T) * N));
        return pVal;
    }

    static T& get_value(size_t idx)
    {
        return get_pointer()[idx];
    }

    static pthread_key_t sKey;
    
};
template <typename T, size_t N, int ID, int Line>
pthread_key_t ThreadLocalArray<T, N, ID, Line>::sKey = 0;

} // namespace gaen

#endif // #if IS_PLATFORM_IOS

#endif // #ifndef GAEN_CORE_THREAD_LOCAL_H
