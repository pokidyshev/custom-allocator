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
    FixedAllocator(size_t blockSize, uint8_t numBlocks);
    void* Allocate();
    void Deallocate(void* pointer);

private:
    // POD (Plain Old Data)
    struct Chunk {
        void Init(size_t blockSize, uint8_t blocks);
        void* Allocate(size_t blockSize);
        void Deallocate(void* p, std::size_t blockSize);

        uint8_t* pData;
        uint8_t firstAvailableBlock;
        uint8_t blocksAvailable;
    };

    typedef std::vector<Chunk> Chunks;

    size_t blockSize;
    uint8_t numBlocks;
    Chunks chunks;
    Chunk* allocChunk;
    Chunks::iterator deallocChunk;

    bool TryDealloc(Chunks::iterator i, uint8_t* toSearch);
};

#endif //FIXED_ALLOCATOR_H
