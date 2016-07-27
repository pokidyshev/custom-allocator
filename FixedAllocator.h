//
// Created by sandpiturtle on 27.07.16.
//

#ifndef FIXED_ALLOCATOR_H
#define FIXED_ALLOCATOR_H

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <vector>

class FixedAllocator {
public:
    void* Allocate();
    void Deallocate(void* p);

private:
    typedef std::vector<Chunk> Chunks;

    size_t blockSize;
    uint8_t numBlocks;
    Chunks chunks;
    Chunk* allocChunk;
    Chunk* deallocChunk;

    // POD (Plain Old Data)
    struct Chunk {
        void Init(size_t blockSize, uint8_t blocks);
        void* Allocate(size_t blockSize);
        void Deallocate(void* p, std::size_t blockSize);

        uint8_t* pData;
        uint8_t firstAvailableBlock;
        uint8_t blocksAvailable;
    };
};

#endif //FIXED_ALLOCATOR_H
