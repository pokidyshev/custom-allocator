//
// Created by sandpiturtle on 27.07.16.
//

#ifndef SMALL_OBJ_ALLOCATOR_H
#define SMALL_OBJ_ALLOCATOR_H

#include "FixedAllocator.h"

class SmallObjAllocator
{
public:
    SmallObjAllocator(size_t chunkSize, size_t maxObjSize);
    void* Allocate(size_t numBytes);
    void Deallocate(void* p, size_t size);
private:
    std::vector<FixedAllocator> pool;
};

#endif //SMALL_OBJ_ALLOCATOR_H
