//
// Created by sandpiturtle on 27.07.16.
//

#include <stdexcept>
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
            allocChunk = &chunks.back();
            deallocChunk = chunks.end() - 1;
        }
    }
    assert(allocChunk != nullptr);
    assert(allocChunk->blocksAvailable > 0);
    return allocChunk->Allocate(blockSize);
}

void FixedAllocator::Deallocate(void *pointer)
{
    // Nothing to deallocate
    if (pointer == nullptr)
        return;
    // Check if memory wasn't allocated
    assert(deallocChunk != chunks.end());
    auto toSearch = static_cast<uint8_t*>(pointer);
    // Try to find pointer in deallocChunk
    if (TryDealloc(deallocChunk, toSearch))
        return;
    // Continue searching in chunks vector
    Chunks::iterator left, right;
    left = right = deallocChunk;
    do {
        if (--left >= chunks.begin() && TryDealloc(left, toSearch)) {
            deallocChunk = left;
            return;
        }
        if (++right < chunks.end() && TryDealloc(right, toSearch)) {
            deallocChunk = right;
            return;
        }
    } while (left >= chunks.begin() || right < chunks.end());
    // Pointer wasn't found
    throw std::invalid_argument("no such pointer");
}

FixedAllocator::FixedAllocator(size_t blockSize, uint8_t numBlocks)
    : blockSize(blockSize),
      numBlocks(numBlocks),
      allocChunk(nullptr),
      deallocChunk(chunks.end()) {}

bool FixedAllocator::TryDealloc(Chunks::iterator i, uint8_t *toSearch)
{
    if (i->pData <= toSearch &&
        toSearch <= i->pData + blockSize*numBlocks)
    {
        i->Deallocate(toSearch, blockSize);
        return true;
    }
    return false;
}
