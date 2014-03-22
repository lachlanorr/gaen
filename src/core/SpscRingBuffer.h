//------------------------------------------------------------------------------
// SpscRingBuffer.h - Wait free single producer single consumer ring buffer
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

#ifndef GAEN_CORE_SPSCRINGBUFFER_H
#define GAEN_CORE_SPSCRINGBUFFER_H

#include <atomic>

#include "core/mem.h"
#include "core/threading.h"

namespace gaen
{

//------------------------------------------------------------------------------
// Provides a wait free ring buffer where the producer and consumer
// get raw access into the buffer and can write direclty into it.
// Other more general implementations of spsc queues usually used
// move/copy/assignment to push and pop. Since we use this as a
// foundational data structure for concurrent message queues in the
// engine, and since messages can be variable sizes, we opt for this
// lower level approach.
//
// Usage and caveats:
//   1. To push:
//      a. Call pushBegin, which will return an Accessor
//      b. Check there's available space for your message with
//         Accessor::available().
//      c. Use the [] operator on the Accessor set your data.
//         Operator[0] will be the first element, [1] the next,
//         and so on. Operator[] will take care of wrapping around
//         the ring.
//      d. Call pushCommit with the element count you are pushing.
//
//   2. To pop (analagous process to push above):
//      a. Call popBegin, which will return an Accessor
//      b. Check there's available data to read with
//         Accessor::available().
//      c. Use the [] operator on the Accessor read your data.
//         Operator[0] will be the first element, [1] the next,
//         and so on. Operator[] will take care of wrapping around
//         the ring.
//      d. Call popCommit with the element count you are popping.
//------------------------------------------------------------------------------

template <class T>
class SpscRingBuffer
{
    friend class Accessor;
public:

    // Provides raw access to buffer, but respecting ring boundary
    class Accessor
    {
        friend class SpscRingBuffer;
    public:
        Accessor()
          : mpSpscRingBuffer(nullptr)
          , mpStart(nullptr)
          , mAvailable(0)
        {}

        size_t available() const { return mAvailable; }

        T & operator[](size_t index)
        {
            ASSERT(index < mAvailable);
            ASSERT(mpSpscRingBuffer && mpStart);
            return *mpSpscRingBuffer->wrapPointer(mpStart + index);
        }

        const T & operator[](size_t index) const
        {
            ASSERT(index < mAvailable);
            ASSERT(mpSpscRingBuffer && mpStart);
            return *mpSpscRingBuffer->wrapPointer(mpStart + index);
        }

    private:
        SpscRingBuffer<T> * mpSpscRingBuffer;
        T* mpStart;
        size_t mAvailable;
    };

    
    SpscRingBuffer(size_t elemCount, MemType memType)
      : mElemCount(elemCount)
    {
        mpBuffer = static_cast<T*>(GALLOC(sizeof(T) * elemCount, memType));
        mpBufferEnd = mpBuffer + elemCount;
        mpHead.store(mpBuffer);
        mpTail.store(mpBuffer);
    }

    ~SpscRingBuffer()
    {
        GFREE(mpBuffer);
    }

    void pushBegin(Accessor * pAccessor, size_t elemCount)
    {
        ASSERT_MSG(isValidProducer(), "Push from more than one thread");
        ASSERT(elemCount > 0);
        
        T* pTail = mpTail.load(std::memory_order_relaxed); // we're the only thread to modify this, so relax
        T* pHead = mpHead.load(std::memory_order_acquire); // this is modified by the consumer, so acquire

        // Ensure we have available space
        size_t avail = emptyCount(pHead, pTail);
        
        if (elemCount > avail)
            PANIC("Out of space in ring buffer, requested=%d, available=%d", elemCount, avail);

        pAccessor->mpSpscRingBuffer = this;
        pAccessor->mpStart = pTail;
        pAccessor->mAvailable = avail;
    }


    void pushCommit(size_t elemCount)
    {
        ASSERT_MSG(isValidProducer(), "Push from more than one thread");

        T* pTail = mpTail.load(std::memory_order_relaxed); // we're the only thread to modify this, so relax
        pTail = wrapPointer(pTail + elemCount);

        mpTail.store(pTail, std::memory_order_release); // this is read by the consumer, so release
    }

    void popBegin(Accessor * pAccessor)
    {
        ASSERT_MSG(isValidConsumer(), "Pop from more than one thread");

        T* pHead = mpHead.load(std::memory_order_relaxed); // we're the only thread to modify this, so relax
        T* pTail = mpTail.load(std::memory_order_acquire); // this is modified by the producer, so acquire

        size_t avail = filledCount(pHead, pTail);
        
        pAccessor->mpSpscRingBuffer = this;
        pAccessor->mpStart = pHead;
        pAccessor->mAvailable = avail;
    }

    void popCommit(size_t elemCount)
    {
        ASSERT_MSG(isValidConsumer(), "Pop from more than one thread");

        T* pHead = mpHead.load(std::memory_order_relaxed); // we're the only thread to modify this, so relax
        pHead = wrapPointer(pHead + elemCount);

        mpHead.store(pHead, std::memory_order_release); // this is read by the producer, so release
    }

private:
    size_t filledCount(T* pHead, T* pTail) const
    {
        if (pTail >= pHead)
            return pTail - pHead;
        return (pTail - mpBuffer) + (mpBufferEnd - mpHead);
    }

    size_t emptyCount(T* pHead, T* pTail) const
    {
        return mElemCount - filledCount(pHead, pTail);
    }

    // Normalize ptr to be valid for the ring (i.e. make it wrap)
    T * wrapPointer(T * ptr)
    {
        T * wrappedPtr = mpBuffer + ((ptr - mpBuffer) % mElemCount);
        return wrappedPtr;
    }

    size_t mElemCount;
    T * mpBuffer;
    T * mpBufferEnd;

    std::atomic<T*> mpHead;
    std::atomic<T*> mpTail;
    


#if HAS(DEV_BUILD)
    // Verify the identiy of the producer (used just for sanity checks in dev builds)
    bool isValidProducer() const
    {
        thread_id tid = active_thread_id();
        if (mProducerThreadId == kInvalidThreadId)
            mProducerThreadId = tid; // first thread to produce, he become's official
        return (tid == mProducerThreadId);
    }
    bool isValidConsumer() const
    {
        thread_id tid = active_thread_id();
        if (mConsumerThreadId == kInvalidThreadId)
            mConsumerThreadId = tid; // first thread to consume, he become's official
        return (tid == mConsumerThreadId);
    }
    mutable thread_id mProducerThreadId = kInvalidThreadId;
    mutable thread_id mConsumerThreadId = kInvalidThreadId;
#endif // #if HAS(DEV_BUILD)
};


}

#endif // #ifndef GAEN_CORE_SPSCRINGBUFFER_H

