//------------------------------------------------------------------------------
// mem.cpp - Memmory management, allocation, deallocation, and reporting
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014 Lachlan Orr
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

#include <cstring>

#if IS_PLATFORM_WIN32
#include <malloc.h>
#endif

#include "core/threading.h"
#include "core/mem.h"

namespace gaen
{

class MemPool;
struct AllocHeader
{
    class MemPool * pMemPool;
    char padding[8];
};

static_assert(sizeof(AllocHeader) == 16, "AllocHeader not 16 bytes, must be to maintain alignments");

static const size_t kAllocHeaderSize = sizeof(AllocHeader);

static const char * sMemTypeText[] = {"Unspecified",
                                      "Debug",
                                      "Engine",
                                      "Texture",
                                      "Model",
                                      "Sound",
                                      "Network",
                                      "Compose"};

static_assert(sizeof(sMemTypeText) / sizeof(char*) == kMEM_COUNT,
              "sMemTypeText should have the same number of entries as MemType enum");


/*
// LORRTODO - figure out a memory allocation solution

//------------------------------------------------------------------------------
// MemPool
//------------------------------------------------------------------------------
void MemPool::init(MemPoolInit memPoolInit)
{
    ASSERT(!mIsInit);
    ASSERT(is_threading_init());

    size_t numThreads = num_threads();

    // In this case, a chunk is the combination of the AllocHeader and the
    // allocated memory.
    size_t chunkCount = numThreads * memPoolInit.count;
    size_t chunkSize = memPoolInit.allocSize + kAllocHeaderSize;

    // Allocate enough memory for our allocs as well as the alloc headers.
    pBuffer = static_cast<u8*>(std::malloc(chunkCount * chunkSize));

    // Run through the chunks of each thread and initialize appropriately
    u8 * pIt = pBuffer;
    for (size_t tid = 0; tid < numThreads; ++tid)
    {
        for (size_t i = 0; i < memPoolInit.count; ++i)
        {
            AllocHeader * pAllocHeader = reinterpret_cast<AllocHeader*>(pIt);

            pAllocHeader->pMemPool = this;
        

            pIt += chunkSize;
        }
    }
}

void MemPool::fin()
{
    
}

void * MemPool::allocate(size_t count)
{
    return nullptr;
}

void MemPool::deallocate(void * ptr)
{

}
//------------------------------------------------------------------------------
// MemPool (END)
//------------------------------------------------------------------------------
*/


//------------------------------------------------------------------------------
// MemMgr
//------------------------------------------------------------------------------
void MemMgr::init(const char * memPoolInit)
{
    ASSERT(!mIsInit);
    ASSERT(is_threading_init());

    mIsInit = true;
}

void MemMgr::fin()
{
    ASSERT(mIsInit);

    mIsInit = false;
}

void * MemMgr::allocate(size_t count)
{
    // LORRTODO

#if IS_PLATFORM_WIN32
    void * pMem = _aligned_malloc(count, 16);
#else
    void * pMem = malloc(count);
#endif

    ASSERT(pMem);
    return pMem;
}

void MemMgr::deallocate(void * ptr)
{
    // LORRTODO
    ASSERT(ptr);
#if IS_PLATFORM_WIN32
    _aligned_free(ptr);
#else
    free(ptr);
#endif
}

#if HAS(TRACK_MEM)
void MemMgr::trackAllocation(MemType memType,
                             size_t count,
                             const char * file,
                             int line)
{
    // LORRTODO
    /*
    printf("ALLOC %lu bytes, %s category, %s:%d\n",
           count,
           sMemTypeText[memType],
           file,
           line);
    */
    return;
}
    
void MemMgr::trackDeallocation(void * ptr,
                               const char * file,
                               int line)
{
    // LORRTODO
    /*
    printf("FREE %s:%d\n",
           file,
           line);
    */
    return;
}
#endif // #if HAS(TRACK_MEM)

//------------------------------------------------------------------------------
// MemMgr (END)
//------------------------------------------------------------------------------



size_t parse_mem_init_str(const char * memInitStr,
                          MemPoolInit * pMemPoolInits,
                          size_t memPoolInitsCount)
{
    if (!memInitStr ||
        memPoolInitsCount > kMaxMemPoolCount ||
        (pMemPoolInits && memPoolInitsCount == 0))
    {
        return 0;
    }

    size_t poolIdx = 0;

    // String will be of form: 16:100,64:100,128:250
    const char * start = memInitStr;
    while (*start)
    {
        if (poolIdx > memPoolInitsCount-1)
            return 0;

        const char * colonPos = strchr(start, ':');
        if (!colonPos)
            return 0;
        
        char * endNum;
        u64 allocSize = strtoul(start, &endNum, 10);
        if (endNum != colonPos ||
            allocSize < kMinPoolAllocSize ||
            allocSize > kMaxPoolAllocSize ||
            allocSize % 16 != 0)
        {
            return 0;
        }

        start = colonPos+1;
        const char * commaPos = strchr(start, ',');
        if (!commaPos)
            commaPos = start + strlen(start);
        u64 count = strtoul(start, &endNum, 10);
        if (endNum != commaPos ||
            count < kMinPoolCount ||
            count > kMaxPoolCount)
        {
            return 0;
        }

        start = commaPos;
        if (start[0] == ',')
        {
            // handle the trailing comma case
            if (start[1] == '\0')
                return 0;
            ++start;
        }
            
        // Ok, we have a valid allocSize and count
        // If we were passed in null, don't set.
        if (pMemPoolInits)
        {
            pMemPoolInits[poolIdx].allocSize = static_cast<u16>(allocSize);
            pMemPoolInits[poolIdx].count = static_cast<u16>(count);
        }
        ++poolIdx;
    }

    return poolIdx;
}

bool is_mem_init_str_valid(const char * memInitStr)
{
    return 0 != parse_mem_init_str(memInitStr, nullptr, 0);
}


} // namespage gaen


