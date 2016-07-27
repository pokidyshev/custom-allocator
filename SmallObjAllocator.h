//
// Created by sandpiturtle on 27.07.16.
//

#ifndef SMALL_OBJ_ALLOCATOR_H
#define SMALL_OBJ_ALLOCATOR_H

#include <map>

#include "FixedAllocator.h"

class SmallObjAllocator
{
public:
    SmallObjAllocator(size_t chunkSize, size_t maxObjSize);
    void* Allocate(size_t numBytes) {
        if (numBytes > maxObjSize)
            return ::operator new(numBytes);
        auto it = pool.find(chunkSize);
        if (it == pool.end())
            it = pool.insert(std::make_pair(numBytes, FixedAllocator(numBytes))).first;
        return it->second.Allocate();
    }
    void Deallocate(void* p, size_t size);
private:
    std::map<size_t, FixedAllocator> pool;
    size_t chunkSize;
    size_t maxObjSize;
};

#endif //SMALL_OBJ_ALLOCATOR_H
