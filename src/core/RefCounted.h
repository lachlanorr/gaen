//------------------------------------------------------------------------------
// RefCounted.h - Ref counting template for manually ref counted objects
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2020 Lachlan Orr
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

#ifndef GAEN_CORE_REFCOUNTED_H
#define GAEN_CORE_REFCOUNTED_H

#include "core/mem.h"

namespace gaen
{

template <class T, class DeleteFunctorT>
class RefCounted
{
public:
    RefCounted(T * pObj, DeleteFunctorT releaseFunctor)
      : mpObj(pObj)
      , mRefCount(1)
      , mDeleteFunctor(releaseFunctor)
    {
        ASSERT(mpObj);
    }
    
    T * get() { return mpObj; }
    const T * get() const { return mpObj; }

    T & operator*() { return *mpObj; }
    const T & operator*() const { return *mpObj; }

    void addRef()
    {
        ASSERT(mpObj);
        ++mRefCount;
    }
    
    void release()
    {
        ASSERT(mpObj);
        ASSERT(mRefCount > 0);
        --mRefCount;
        if (mRefCount == 0)
        {
            //mDeleteFunctor(mpObj);
            mpObj = nullptr;
        }
    }
private:
    T * mpObj;
    size_t mRefCount;
    DeleteFunctorT mDeleteFunctor;
};

template <class T,class DeleteFunctorT>
RefCounted<T, DeleteFunctorT> make_ref_counted(T * pObj, DeleteFunctorT releaseFunctor)
{
    return RefCounted<T, DeleteFunctorT>(pObj, releaseFunctor);
}


}

#endif // #ifndef GAEN_CORE_REFCOUNTED_H
