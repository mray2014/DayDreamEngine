#pragma once
#include <cstdint>

class DreamPoolAllocator
{
public:
	DreamPoolAllocator(uint32_t size);
	DreamPoolAllocator(void* startOfMemory, uint32_t size);
	~DreamPoolAllocator();
private:
	uint32_t* slots;
};

