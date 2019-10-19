#pragma once
#include <cstdint>

class DreamPoolAllocator
{
public:
	DreamPoolAllocator(uint32_t size);
	DreamPoolAllocator(void* startOfMemory, uint32_t size);
	~DreamPoolAllocator();

	private:
	uint32_t numOfSlots = 0;
	uint32_t numOfIntsInPool = 0;
	uint32_t* poolOfBools = nullptr;
};

