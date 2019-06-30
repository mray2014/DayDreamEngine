#pragma once
#include <cstdint>

#define MAX_STACK_MEMORY_SIZE (uint32_t)50000

enum AlignmentType {
	_4BitAlign,
	_8BitAlign,
	_16BitAlign,
	_32BitAlign,
	_64BitAlign,
};
struct AllocationMark {
	void* lastAllocationMark;
};
struct ChunkMark
{
	void* lastChunkMark;
	const char* chunkTitle;
};

class DreamStaticStackAllocator
{
public:
	void* Allocate(size_t size, AlignmentType type);
	void MarkChunk(const char* memChunkTitle);
	void PopChunk();
	void Pop();
	void Clear();
	uint32_t GetUsedMemorySize();
	uint32_t GetMaxStackSize();
	DreamStaticStackAllocator(uint32_t maxStackSize = MAX_STACK_MEMORY_SIZE);
	~DreamStaticStackAllocator();

	void* startPtr = nullptr;
	void* backPtr = nullptr;
	void* frontPtr = nullptr;
	void* chunkPtr = nullptr;
	void* endPtr = nullptr;

	uint32_t usedMemorySize = 0;
private:
	//
	uint32_t maxStackSize = 0;

};


