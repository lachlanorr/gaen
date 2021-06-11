//------------------------------------------------------------------------------
// comp_mem.cpp - Compiler memory manager
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

#include <cstdio> // LORRTEMP
#include <cstring>

#include "core/base_defines.h"
#include "core/mem.h"
#include "core/thread_local.h"

#include "compose/comp_mem.h"

using namespace gaen;

const size_t kCompMemSize = 5 * 1024 * 1024;

struct CompMemInfo
{
    u8 * pBuffer;
    u8 * pCurrent;
    u8 * pEnd;
};

static CompMemInfo * mem_info()
{
    TL(CompMemInfo*, spMemInfo) = nullptr;

    CompMemInfo * pMemInfo = (CompMemInfo*)spMemInfo;

    if (!pMemInfo)
    {
        pMemInfo = GNEW(kMEM_Compose, CompMemInfo);
        pMemInfo->pBuffer = static_cast<u8*>(GALLOC(kMEM_Compose, kCompMemSize));
        pMemInfo->pCurrent = pMemInfo->pBuffer;
        pMemInfo->pEnd = pMemInfo->pBuffer + kCompMemSize;
        spMemInfo = pMemInfo;
    }

    return pMemInfo;
}

void * comp_alloc(size_t count)
{
    CompMemInfo * pMI = mem_info();

    count = align(count, 8); // keep our allocations aligned nicely

    if (pMI->pCurrent + count >= pMI->pEnd)
    {
        PANIC("Compiler out of memory, kCompMemSize = %d", kCompMemSize);
        return nullptr;
    }
    
    size_t * pCount = reinterpret_cast<size_t*>(pMI->pCurrent);
    *pCount = count;

    count += 8; // 8 bytes for header
    void * pRet = pMI->pCurrent + 8;

    pMI->pCurrent += count;

    //LORRTEMP
    //printf("comp_alloc %d, available=%d\n", count, comp_avail_mem());

    return pRet;
}

void comp_free(void *p)
{
    // no op, we don't free memory
}

size_t comp_get_size(void *p)
{
    size_t *pCount = reinterpret_cast<size_t*>(p) - 1;
    return *pCount;
}

void * comp_realloc(void * p, size_t count)
{
    void * pNew = comp_alloc(count);
    if (p)
    {
        size_t oldCount = comp_get_size(p);
        size_t minCount = oldCount <= count ? oldCount : count;
        memcpy(pNew, p, minCount);
    }
    return pNew;
}

void comp_reset_mem()
{
    CompMemInfo * pMI = mem_info();
    pMI->pCurrent = pMI->pBuffer;
}

size_t comp_avail_mem()
{
    CompMemInfo * pMI = mem_info();
    return pMI->pEnd - pMI->pCurrent;
}

char * comp_strdup(const char * s)
{
    if (!s)
        return nullptr;
    size_t slen = strlen(s);
    char * sd = (char*)comp_alloc(slen+1);
    strcpy(sd, s);
    sd[slen] = '\0';
    return sd;
}
