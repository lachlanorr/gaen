//------------------------------------------------------------------------------
// RenderCollection.h - Collection of renderable objects (e.g. sprites)
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

#ifndef GAEN_RENDERERGL_RENDERCOLLECTION_H
#define GAEN_RENDERERGL_RENDERCOLLECTION_H

#include "core/HashMap.h"
#include "core/Map.h"

#include "render_support/render_objects.h"

namespace gaen
{

// RenderCollection maintains a collection of renderable objects and
// supports both fast lookups based on a numeric uid, as well as an
// ordered traversal based on a float order.

// There are two maps to support these operations:
// HashMapT is for fast lookups based on ouid values.
// OrderedMapT is for ordered traversal during rendering.

// class T must supply the following methods:
//   ouid uid()
//   f32 order()
//

// The order() method can be crafted to support arbitrarily complex
// orders.  For example, it may encode a combination of zdepth,
// rendering resources (images/shaders/etc), and transparency to
// create a desirable rendering order.


enum RenderItemStatus
{
    kRIS_Active,
    kRIS_Destroyed
};

template <class T>
class RenderCollection
{
private:
    typedef MultiMap<kMEM_Renderer, f32, UniquePtr<T>> OrderedMapT;
    typedef HashMap<kMEM_Renderer, ouid, typename OrderedMapT::iterator> HashMapT;

public:
    class Iter
    {
        friend class RenderCollection;
    public:
        Iter& operator++()
        {
            ++mOrdIt;
            return *this;
        }

        Iter operator++(int)
        {
            Iter tmp(*this);
            operator++();
            return tmp;
        }

        T * operator*()
        {
            return mOrdIt->second.get();
        }

        const T * operator*() const
        {
            return mOrdIt->second.get();
        }

        T * operator->()
        {
            return mOrdIt->second.get();
        }

        const T * operator->() const
        {
            return mOrdIt->second.get();
        }

        bool operator==(const Iter & rhs)
        {
            return mOrdIt == rhs.mOrdIt;
        }

        bool operator!=(const Iter & rhs)
        {
            return mOrdIt != rhs.mOrdIt;
        }
    private:
        Iter(typename OrderedMapT::iterator ordIt)
          : mOrdIt(ordIt)
        {}

        typename OrderedMapT::iterator mOrdIt;
    }; // class Iter

    Iter begin()       { return Iter(mOrderedMap.begin()); }
    Iter end()         { return Iter(mOrderedMap.end()); }
    u32  size()        { return (u32)mOrderedMap.size(); }
    Iter find(ouid uid)
    {
        auto hashIt = mHashMap.find(uid);
        if (hashIt != mHashMap.end())
            return Iter(hashIt->second);
        else
            return end();
    }

    void insert(UniquePtr<T> pItem)
    {
        ASSERT(mHashMap.find(pItem->uid()) == mHashMap.end());
        auto ordIt = mOrderedMap.emplace(pItem->order(), std::move(pItem));
        mHashMap.emplace(ordIt->second->uid(), ordIt);
        ASSERT(mHashMap.size() == mOrderedMap.size());
    }

    void erase(Iter it)
    {
        mHashMap.erase(it->uid());
        mOrderedMap.erase(it.mOrdIt);
        ASSERT(mHashMap.size() == mOrderedMap.size());
    }

    void reorder(ouid uid)
    {
        ASSERT(mHashMap.size() == mOrderedMap.size());
        auto hashIt = mHashMap.find(uid);
        ASSERT(hashIt != mHashMap.end());

        if (hashIt != mHashMap.end())
        {
            auto ordIt = hashIt->second;
            UniquePtr<T> pItem = std::move(ordIt->second);
            mOrderedMap.erase(ordIt);
            insert(std::move(pItem));
        }
        ASSERT(mHashMap.size() == mOrderedMap.size());
    }

private:
    HashMapT mHashMap;
    OrderedMapT mOrderedMap;

}; // class RenderCollection

} // namespace gaen

#endif //#ifndef GAEN_RENDERERGL_RENDERCOLLECTION_H

