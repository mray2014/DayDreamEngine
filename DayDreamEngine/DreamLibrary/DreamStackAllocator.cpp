#include "DreamStackAllocator.h"
#include <stdlib.h>
#include <iostream>

DreamStackAllocator* DreamStackAllocator::stackAllocator = nullptr;

DreamStackAllocator::DreamStackAllocator()
{
	startPtr = malloc(MAX_STACK_MEMORY_SIZE);
	backPtr = startPtr;
	frontPtr = startPtr;
	chunkPtr = startPtr;
}


DreamStackAllocator * DreamStackAllocator::GetInstance()
{
	if (stackAllocator == nullptr) {
		stackAllocator = new DreamStackAllocator();
	}
	return stackAllocator;
}

void DreamStackAllocator::ShutDown()
{
	delete stackAllocator;
}

void * DreamStackAllocator::Allocate(size_t size, AlignmentType type)
{
	size_t allocMarkSize = sizeof(AllocationMark);
	void* lastAlloc = backPtr;

	backPtr = frontPtr;
	frontPtr = (void*)((ptrdiff_t) frontPtr + size + allocMarkSize);

	AllocationMark* newAllocMark = new(backPtr) AllocationMark;
	newAllocMark->lastAllocationMark = lastAlloc;

	backPtr = (void*)((ptrdiff_t)backPtr + allocMarkSize);

	return backPtr;
}

void DreamStackAllocator::MarkChunk(const char * memChunkTitle)
{
	void* newChunkPtr = Allocate(sizeof(ChunkMark), AlignmentType::_16BitAlign);
	ChunkMark* chunkMark = new(newChunkPtr) ChunkMark;
	chunkMark->lastChunkMark = chunkPtr;
	chunkMark->chunkSize = (ptrdiff_t)newChunkPtr - (ptrdiff_t)chunkPtr;
	chunkMark->chunkTitle = memChunkTitle;
	chunkPtr = newChunkPtr;
}

void DreamStackAllocator::PopChunk()
{
	while (frontPtr != chunkPtr)
	{
		Pop();
	}
}

void DreamStackAllocator::Pop()
{
	if (frontPtr == startPtr)
	{
		printf("Nothing on the stack");
		return;
	}
	size_t allocMarkSize = sizeof(AllocationMark);
	delete backPtr;

	backPtr = (void*)((ptrdiff_t)backPtr - allocMarkSize);
	AllocationMark* newAllocMark = (AllocationMark*)backPtr;

	frontPtr = backPtr;
	backPtr = newAllocMark->lastAllocationMark;

	delete newAllocMark;
}

void DreamStackAllocator::Clear()
{
	while (frontPtr != startPtr)
	{
		Pop();
	}
}

DreamStackAllocator::~DreamStackAllocator()
{
	Clear();
	delete startPtr;
}
