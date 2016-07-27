//
// Created by sandpiturtle on 27.07.16.
//

#include "SmallObjAllocator.h"

SmallObjAllocator::SmallObjAllocator(size_t chunkSize, size_t maxObjSize)
    : chunkSize(chunkSize),
      maxObjSize(maxObjSize) {}
