//------------------------------------------------------------------------------
// comp_mem.h - Compiler memory manager
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

// To simplify memory freeing from Bison/Flex during error conditions,
// and to make memory allocation effectively free, we allocate a large block
// and reset it at the start of compilation (by calling comp_reset_mem()).
// We never free, just reset the whole thing.
// All compiler code should only allocate memory with these definitions,
// and only use STL containers as defined in this file.

#ifndef GAEN_COMPMEM_COMPILER_H
#define GAEN_COMPMEM_COMPILER_H

#ifdef __cplusplus
extern "C" {
#endif

#define COMP_ALLOC(count)      comp_alloc(count)
#define COMP_REALLOC(p, count) comp_realloc(p, count)
#define COMP_FREE(ptr)         comp_free(ptr)

void * comp_alloc(size_t count);
void * comp_realloc(void * p, size_t count);
void comp_free(void *p);

void comp_reset_mem(); // "free" all memory
size_t comp_avail_mem();

char * comp_strdup(const char * s);

#ifdef __cplusplus
} // extern "C"

#include <cstdlib>
#include <utility>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <cstdio>

#include <string>
#include <list>
#include <vector>
#include <set>
#include <map>

#include "core/base_defines.h"
#include "core/hashing.h"

#define COMP_NEW(type, ...)    new (COMP_ALLOC(sizeof(type))) type(##__VA_ARGS__)
#define COMP_DELETE(ptr)       comp_delete(ptr)

template <class T>
inline void comp_delete(T * ptr)
{
    ptr->~T();
    // we never free memory in the compiler
}

namespace gaen
{


//------------------------------------------------------------------------------
// STL Allocator for Compiler
//
// This Allocator should be used with all STL containers.
//
// E.g.:
//   std::vector<int, gaen::CompAllocator<int>> vecOfInts;
//------------------------------------------------------------------------------
template<class T>
class CompAllocator
{
public:
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef const T* const_pointer;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    template<class U>
    struct rebind { typedef CompAllocator<U> other; };
    typedef std::true_type propagate_on_container_move_assignment;
    CompAllocator() noexcept {}
    CompAllocator (const CompAllocator& alloc) noexcept {}
    template <class U>
    CompAllocator (const CompAllocator<U>& alloc) noexcept {}
    pointer address(reference x) const noexcept             { return &x; }
    const_pointer address(const_reference x) const noexcept { return &x; }
    size_type max_size() const noexcept                     { return size_t(-1) / sizeof(T); }
    template <class U, class... Args>
    void construct (U* p, Args&&... args) const noexcept
    {
        ::new ((void*)p) U (std::forward<Args>(args)...); 
    }
    template <class U>
    void destroy(U* p)
    {
        p->~U();
    }
    pointer allocate(size_type n, const void * hint=0)
    {
        size_t byteCount = n * sizeof(T);
        pointer p = static_cast<pointer>(COMP_ALLOC(byteCount));
//        printf("Compiler custom allocator, allocating %lu bytes\n", byteCount);
        ASSERT_MSG(p, "Unable to allocate with CompAllocator, %lu bytes", byteCount);
        return p;
    }
    void deallocate (pointer p, size_type n)
    {
        // we don't deallocate, comp_reset_mem() should be called before compilation to reset all memory
    }
    bool operator==(const CompAllocator& rhs) const { return true; }
};


//------------------------------------------------------------------------------
// STL Containers
//------------------------------------------------------------------------------
// Define convenience typedefs for various stl containers
typedef std::basic_string<char,
                          std::char_traits<char>,
                          gaen::CompAllocator<char>> CompString;

template <class T>
using CompList = std::list<T, gaen::CompAllocator<T>>;

template <class T>
using CompVector = std::vector<T, gaen::CompAllocator<T>>;

template <class Key,
          class Compare = std::less<Key>>
using CompSet = std::set<Key,
                         Compare,
                         gaen::CompAllocator<Key>>;

template <class Key,
          class T,
          class Compare = std::less<Key>>
using CompMap = std::map<Key,
                         T,
                         Compare,
                         gaen::CompAllocator<std::pair<const Key,T>>>;

//------------------------------------------------------------------------------
// STL Containers (END)
//------------------------------------------------------------------------------

} // namespace gaen

// Overloads of std::hash so we can use our CompString class
// as a key to unordered_map and unordered_set.
namespace std {
template <> struct hash<gaen::CompString>
{
    size_t operator()(const gaen::CompString & str) const
    {
        return gaen::gaen_hash(str.c_str());
    }
};
}

#endif // #ifdef __cplusplus

#endif // #ifndef GAEN_COMPMEM_COMPILER_H
