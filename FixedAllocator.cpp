//
// Created by sandpiturtle on 27.07.16.
//

#include "FixedAllocator.h"

void FixedAllocator::Chunk::Init(size_t blockSize, uint8_t blocks)
{
    pData = new uint8_t[blockSize * blocks];
    firstAvailableBlock = 0;
    blocksAvailable = blocks;
    uint8_t* p = pData;
    for (uint8_t i = 1; i != blocks; p += blockSize, ++i) {
        *p = i;
    }
}

void *FixedAllocator::Chunk::Allocate(size_t blockSize)
{
    if (!blocksAvailable)
        return nullptr;
    uint8_t* pResult = pData + (firstAvailableBlock * blockSize);
    // Update firstAvailableBlock to point to the next block
    firstAvailableBlock = *pResult;
    --blocksAvailable;
    return pResult;
}

void FixedAllocator::Chunk::Deallocate(void *p, std::size_t blockSize)
{
    assert(p >= pData);
    auto toRelease = static_cast<uint8_t*>(p);
    // Alignment check
    assert((toRelease - pData) % blockSize == 0);
    *toRelease = firstAvailableBlock;
    firstAvailableBlock = static_cast<uint8_t>((toRelease - pData) / blockSize);
    // Truncation check
    assert(firstAvailableBlock == (toRelease - pData) / blockSize);
    ++blocksAvailable;
}

void *FixedAllocator::Allocate()
{
    if (allocChunk == nullptr ||
        allocChunk->blocksAvailable == 0)
    { // No available memory in this chunk
        // Try to find one
        for (Chunk& c : chunks) {
            if (c.blocksAvailable > 0) {
                // Found a chunk
                allocChunk = &c;
                break;
            }
        }
        if (allocChunk == nullptr ||
            allocChunk->blocksAvailable == 0)
        { // All chunks filed-up (chunk not found)
            // Add a new one
            Chunk newChunk;
            newChunk.Init(blockSize, numBlocks);
            chunks.push_back(newChunk);
            allocChunk = deallocChunk = &chunks.back();
        }
    }
    assert(allocChunk != nullptr);
    assert(allocChunk->blocksAvailable > 0);
    return allocChunk->Allocate(blockSize);
}

void FixedAllocator::Deallocate(void *p)
{
    if (p == nullptr)
        return;


}
