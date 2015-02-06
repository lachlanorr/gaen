//------------------------------------------------------------------------------
// mem.h - Memmory management, allocation, deallocation, and reporting
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

#ifndef GAEN_CORE_MEM_H
#define GAEN_CORE_MEM_H

#include <cstdlib>
#include <utility>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <cstdio>

#include "core/base_defines.h"

namespace gaen
{

enum MemType
{
    kMEM_Unspecified = 0,
    kMEM_Debug       = 1,
    kMEM_Engine      = 2,  // data structures used in the engine to manage tasks, entities, etc
    kMEM_Renderer    = 3,
    kMEM_Texture     = 4,
    kMEM_Model       = 5,
    kMEM_Sound       = 6,
    kMEM_Network     = 7,
    kMEM_Compose     = 8,

    kMEM_COUNT
};

#define DEFAULT_ALIGNMENT 16

#if !HAS(TRACK_MEM)
#define GALLOC(memType, count)                    gaen::fast_alloc(count)
#define GALLOC_ALIGNED(memType, count, alignment) gaen::fast_alloc(count, alignment)
#define GFREE(ptr)                                gaen::fast_free(ptr)
#define GDELETE(ptr)                              gaen::fast_delete(ptr)
#else  // #if !HAS(DEV_BUILD)
#define GALLOC(memType, count)                    gaen::tracked_alloc(memType, count, DEFAULT_ALIGNMENT, __FILE__, __LINE__)
#define GALLOC_ALIGNED(memType, count, alignment) gaen::tracked_alloc(memType, count, alignment, __FILE__, __LINE__)
#define GFREE(ptr)                                gaen::tracked_free(ptr, __FILE__, __LINE__)
#define GDELETE(ptr)                              gaen::tracked_delete(ptr, __FILE__, __LINE__)
#endif // #if !HAS(DEV_BUILD)
#define GNEW(memType, type, ...)                    new (GALLOC(memType, sizeof(type))) type( __VA_ARGS__ )
#define GNEW_ALIGNED(memType, type, alignment, ...) new (GALLOC_ALIGNED(memType, sizeof(type), alignment)) type( __VA_ARGS__ )


// A pool contains various sizes of pre-allocated buffers.
static const size_t kMinPoolAllocSize = 16;
static const size_t kMaxPoolAllocSize = 16384;

static const size_t kMinPoolCount = 100;
static const size_t kMaxPoolCount = 1000000;


// Maximum number of entries supported in MemPoolInit
// arrays.  Each entry creates a mem pool for that size
// for each thread.
static const size_t kMaxMemPoolCount = 256;
struct MemPoolInit
{
    u16 allocSize;
    u16 count;
};



/*
// LORRTODO - figure out a memory allocation solution
  
static const size_t kBlockSize = 8192;

// Allocates memory of allocSize out of its block
class BlockAllocator
{
public:
    BlockAllocator(u8 * pBlock, u16 allocSize);
                   

private:
    struct BlockHeader
    {
        BlockAllocator * pBlockAllocator;
        u8 * pNextAlloc;
        u16 allocSize;
        u16 allocCount;
        
    };

    static_assert(sizeof(BlockHeader) == 32, "BlockHeader should be 32 bytes");

    thread_id mOwnerTid = kInvalidThreadId;
};

struct ThreadInfo;

class MemPool
{
public:
    void init(MemPoolInit memPoolInit);
    void fin();
    bool isInit() { return mIsInit; }

    void * allocate(size_t count);
    void deallocate(void * ptr);

    void reclaim_freed();

#if HAS(TRACE_MEM)
    void traceAllocations();
#endif

private:
    bool mIsInit = false;
  
    // LORRTODO - use new multi queues
    // push only by main thread, pop by any thread
//    boost::lockfree::spsc_queue<u8*> * pAllocQueues;

    // push by any thread, pop only by main thread
//    boost::lockfree::spsc_queue<u8*> * pFreeQueues;
    u8 * pBuffer;
};
*/

class MemMgr
{

public:
    void init(const char * memPoolInit);
    void fin();
    bool isInit() { return mIsInit; }

    void * allocate(size_t count, u32 alignment = DEFAULT_ALIGNMENT);
    void deallocate(void * ptr);

#if HAS(TRACK_MEM)
    void trackAllocation(MemType memType,
                         size_t count,
                         const char * file,
                         int line);
    
    void trackDeallocation(void * ptr,
                           const char * file,
                           int line);
#endif // #if HAS(TRACK_MEM)
private:
    bool mIsInit = false;

};



// init_memory_manager should be called as early in initialization as
// possible, and fin_memory_manager should be called only after all
// allocations have been freed.
inline void init_memory_manager(const char * memInitStr)
{
    singleton<MemMgr>().init(memInitStr);
}

inline void fin_memory_manager()
{
    singleton<MemMgr>().fin();
}



//------------------------------------------------------------------------------
// MemMgr convenience functions
//------------------------------------------------------------------------------
inline void * fast_alloc(size_t count, u32 alignment = DEFAULT_ALIGNMENT)
{
    return singleton<MemMgr>().allocate(count, alignment);
}
      
inline void fast_free(void * ptr)
{
    singleton<MemMgr>().deallocate(ptr);
}

template <class T>
inline void fast_delete(T * ptr)
{
    ptr->~T();
    singleton<MemMgr>().deallocate(ptr);
}
    
#if HAS(TRACK_MEM)
inline void * tracked_alloc(MemType memType,
                            size_t count,
                            u32 alignment,
                            const char * file,
                            int line)
{
    void * p = fast_alloc(count, alignment);
    singleton<MemMgr>().trackAllocation(memType,
                                        count,
                                        file,
                                        line);
    return p;
}

inline void tracked_free(void * ptr, const char * file, int line)
{
    fast_free(ptr);
    singleton<MemMgr>().trackDeallocation(ptr, file, line);
}

template <class T>
inline void tracked_delete(T * ptr, const char * file, int line)
{
    fast_delete(ptr);
    singleton<MemMgr>().trackDeallocation(ptr, file, line);
}
#endif // #if HAS(TRACK_MEM)
//------------------------------------------------------------------------------
// MemMgr convenience functions (END)
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// STL Allocator
//
// This Allocator should be used with all STL containers.
//
// E.g.:
//   std::vector<int, gaen::Allocator<int>> vecOfInts;
//------------------------------------------------------------------------------
template<MemType memType, class T>
class Allocator
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
    struct rebind { typedef Allocator<memType, U> other; };
    typedef std::true_type propagate_on_container_move_assignment;
    Allocator() noexcept {}
    Allocator (const Allocator& alloc) noexcept {}
    template <class U>
    Allocator (const Allocator<memType, U>& alloc) noexcept {}
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
        pointer p = static_cast<pointer>(GALLOC(memType, byteCount));
//        printf("Custom allocator, allocating %lu bytes\n", byteCount);
        ASSERT_MSG(p, "Unable to allocate with Allocator, %lu bytes", byteCount);
        return p;
    }
    void deallocate (pointer p, size_type n)
    {
        ASSERT(p);
//        printf("Custom deallocator\n");
        GFREE(p);
    }
    bool operator==(const Allocator& rhs) const { return true; }
};


// NOTE: Line numbers won't match up when UniquePtr
// calls this, but the fact that it was freed will
// get tracked properly.
template <class T>
struct deleter
{
    inline void operator() (T * ptr)
    {
        GDELETE(ptr);
    }
};

// unique_ptr that uses our mem management functions
template <class T>
using UniquePtr = std::unique_ptr<T, deleter<T>>;

static_assert(sizeof(UniquePtr<int>) == sizeof(int*), "UnqiuePtr<T> size should be size of normal pointer");


// Returns number of memPoolInits parsed, or 0 if there was an error.
// You can pass in null for pMemPoolInits if you just want to parse
// but not set anything.  In this case, return value will still be
// the number of MemPoolInits that were parsed.
// LORRTODO - depending on what we do with mem allocator, these may no longer be needed
size_t parse_mem_init_str(const char * memInitStr,
                          MemPoolInit * pMemPoolInits,
                          size_t memPoolInitsCount);
bool is_mem_init_str_valid(const char * memInitStr);
                        


} // namespace gaen


#endif // #ifndef GAEN_CORE_MEM_H
