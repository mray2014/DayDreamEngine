#include "DreamStaticStackAllocator.h"
#include <stdlib.h>
#include <iostream>

DreamStaticStackAllocator* DreamStaticStackAllocator::stackAllocator = nullptr;

template <class T>
T* DreamStaticStackAllocator::AllocateThat(AlignmentType type) {

	size_t classSize = sizeof(T);
	size_t allocMarkSize = sizeof(AllocationMark);
	void* lastAlloc = stackAllocator->backPtr;

	stackAllocator->backPtr = stackAllocator->frontPtr;
	stackAllocator->frontPtr = (void*)((ptrdiff_t)stackAllocator->frontPtr + classSize + allocMarkSize);

	AllocationMark* newAllocMark = new(stackAllocator->backPtr) AllocationMark;
	newAllocMark->lastAllocationMark = lastAlloc;

	stackAllocator->backPtr = (void*)((ptrdiff_t)stackAllocator->backPtr + allocMarkSize);

	T* newObj = new(stackAllocator->backPtr) T();

	return newObj;
}

DreamStaticStackAllocator::DreamStaticStackAllocator()
{
	startPtr = malloc(MAX_STACK_MEMORY_SIZE);
	backPtr = startPtr;
	frontPtr = startPtr;
	chunkPtr = startPtr;
}


DreamStaticStackAllocator * DreamStaticStackAllocator::GetInstance()
{
	if (stackAllocator == nullptr) {
		stackAllocator = new DreamStaticStackAllocator();
	}
	return stackAllocator;
}

void DreamStaticStackAllocator::ShutDown()
{
	delete stackAllocator;
}

void * DreamStaticStackAllocator::Allocate(size_t size, AlignmentType type)
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

void DreamStaticStackAllocator::MarkChunk(const char * memChunkTitle)
{
	void* newChunkPtr = Allocate(sizeof(ChunkMark), AlignmentType::_16BitAlign);
	ChunkMark* chunkMark = new(newChunkPtr) ChunkMark;
	chunkMark->lastChunkMark = chunkPtr;
	chunkMark->chunkSize = (ptrdiff_t)newChunkPtr - (ptrdiff_t)chunkPtr;
	chunkMark->chunkTitle = memChunkTitle;
	chunkPtr = newChunkPtr;
}

void DreamStaticStackAllocator::PopChunk()
{
	while (frontPtr != chunkPtr)
	{
		Pop();
	}
}

void DreamStaticStackAllocator::Pop()
{
	if (frontPtr == startPtr)
	{
		printf("Nothing on the stack");
		return;
	}
	size_t allocMarkSize = sizeof(AllocationMark);

	backPtr = (void*)((ptrdiff_t)backPtr - allocMarkSize);
	AllocationMark* newAllocMark = (AllocationMark*)backPtr;

	frontPtr = backPtr;
	backPtr = newAllocMark->lastAllocationMark;

}

void DreamStaticStackAllocator::Clear()
{
	while (frontPtr != startPtr)
	{
		Pop();
	}
}

DreamStaticStackAllocator::~DreamStaticStackAllocator()
{
	Clear();
	delete startPtr;
}
