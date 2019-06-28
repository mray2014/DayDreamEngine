#pragma once
#include <cstdint>

#define MAX_STACK_MEMORY_SIZE 50000

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
	uint32_t chunkSize;
	const char* chunkTitle;
};

class DreamStackAllocator
{
public:
	static DreamStackAllocator* GetInstance();
	static void ShutDown();
	void* Allocate(size_t size, AlignmentType type);
	void MarkChunk(const char* memChunkTitle);
	void PopChunk();
	void Pop();
	void Clear();
	~DreamStackAllocator();
private:
	// 
	void* startPtr = nullptr;
	void* backPtr = nullptr;
	void* frontPtr = nullptr;
	void* chunkPtr = nullptr;

	uint32_t usedMemorySize = 0;

	DreamStackAllocator();
	static DreamStackAllocator* stackAllocator;
};