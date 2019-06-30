#include "DreamStaticStackAllocator.h"
#include <stdlib.h>
#include <iostream>

DreamStaticStackAllocator::DreamStaticStackAllocator(uint32_t maxStackSize)
{
	maxStackSize = maxStackSize;

	startPtr = malloc(maxStackSize);
	backPtr = startPtr;
	frontPtr = startPtr;
	chunkPtr = startPtr;
	endPtr = (void*)((ptrdiff_t)startPtr + maxStackSize);
}

DreamStaticStackAllocator::DreamStaticStackAllocator(void * startOfStackMemory, uint32_t maxStackSize)
{
	maxStackSize = maxStackSize;

	startPtr = startOfStackMemory;
	backPtr = startPtr;
	frontPtr = startPtr;
	chunkPtr = startPtr;
	endPtr = (void*)((ptrdiff_t)startPtr + maxStackSize);
}


//4 bytes to Allocatd a AllocMark
//12 bytes to Allocate a Chunk Mark
//16 bytes for marking a Chunk
void * DreamStaticStackAllocator::Allocate(size_t size, AlignmentType type)
{
	size_t allocMarkSize = sizeof(AllocationMark);
	
	void* newFront = (void*)((ptrdiff_t) frontPtr + size + allocMarkSize);
	if (newFront > endPtr) {
		printf("ERROR: Stack Overflow!! Allocate more space for this stack and build again!");
		return nullptr;
	}

	void* lastAlloc = backPtr;
	backPtr = frontPtr;
	frontPtr = newFront;
	AllocationMark* newAllocMark = new(backPtr) AllocationMark;
	newAllocMark->lastAllocationMark = lastAlloc;

	backPtr = (void*)((ptrdiff_t)backPtr + allocMarkSize);

	usedMemorySize += size + allocMarkSize;

	return backPtr;
}

void DreamStaticStackAllocator::MarkChunk(const char * memChunkTitle)
{
	void* newChunkPtr = Allocate(sizeof(ChunkMark), AlignmentType::_16BitAlign);
	ChunkMark* chunkMark = new(newChunkPtr) ChunkMark;
	chunkMark->lastChunkMark = chunkPtr;	
	chunkMark->chunkTitle = memChunkTitle;
	chunkPtr = newChunkPtr;
}

void DreamStaticStackAllocator::PopChunk()
{
	if (chunkPtr == startPtr) {
		if (usedMemorySize == 0) {
			printf("No chunks to pop\n");
			printf("Current used mem size ------- %d bytes\n\n", usedMemorySize);
			return;
		}
		else {
			printf("Clearing the rest of the stack!\n");
			Clear();
			return;
		}
	}

	uint32_t fullChunkAllocSize = sizeof(ChunkMark) + sizeof(AllocationMark);
	uint32_t numOfPops = 0;
	uint32_t sizeOfPoppedOffChunk = (ptrdiff_t)frontPtr - ((ptrdiff_t)chunkPtr + sizeof(ChunkMark));
	uint32_t sizeOfNextChunk = 0;
	const char* title = ((ChunkMark*)chunkPtr)->chunkTitle;

	printf("Current used mem size ------- %d bytes\n", usedMemorySize);
	while (backPtr != chunkPtr)
	{
		Pop();
		numOfPops++;		
	}

	ChunkMark* curChunkPtr = (ChunkMark*)chunkPtr;

	if (curChunkPtr->lastChunkMark != startPtr) {
		sizeOfNextChunk = ((ptrdiff_t)curChunkPtr - sizeof(AllocationMark)) - ((ptrdiff_t)curChunkPtr->lastChunkMark + sizeof(ChunkMark));
	}

	printf("Num of Pops ----------------- %d pops\n", numOfPops);
	printf("Chunk of memory popped off -- %d bytes\n", sizeOfPoppedOffChunk);
	printf("Total memory popped off ----- %d bytes\n", (sizeOfPoppedOffChunk + fullChunkAllocSize));
	printf("[Next Chunk To Pop ~ '%s']\n", title);
	printf("Next Chunk Mem Size --------- %d bytes\n", sizeOfNextChunk);
	chunkPtr = curChunkPtr->lastChunkMark;
	Pop();
	printf("New used mem size ----------- %d bytes\n\n", usedMemorySize);
}

void DreamStaticStackAllocator::Pop()
{
	if (frontPtr == startPtr)
	{
		printf("Nothing on the stack");
		return;
	}
	uint32_t memBlockSize = (ptrdiff_t)frontPtr - (ptrdiff_t)backPtr;
	size_t allocMarkSize = sizeof(AllocationMark);

	backPtr = (void*)((ptrdiff_t)backPtr - allocMarkSize);
	AllocationMark* newAllocMark = (AllocationMark*)backPtr;

	frontPtr = backPtr;
	backPtr = newAllocMark->lastAllocationMark;

	usedMemorySize -= (memBlockSize + allocMarkSize);
}

void DreamStaticStackAllocator::Clear()
{
	if (frontPtr == startPtr) {
		printf("Nothing to clear in stack!\n");
		printf("Current used mem size ------- %d bytes\n\n", usedMemorySize);
		return;
	}
	uint32_t numOfPops = 0;
	uint32_t sizeOfPoppedOffChunk = (ptrdiff_t)frontPtr - (ptrdiff_t)startPtr;
	printf("Current used mem size ------- %d bytes\n", usedMemorySize);
	while (frontPtr != startPtr)
	{
		Pop();
		numOfPops++;
	}
	printf("Num of Pops ----------------- %d pops\n", numOfPops);
	printf("Total memory popped off ----- %d bytes\n", sizeOfPoppedOffChunk);
	printf("New used mem size ------- %d bytes\n\n", usedMemorySize);
}

uint32_t DreamStaticStackAllocator::GetUsedMemorySize()
{
	return usedMemorySize;
}

uint32_t DreamStaticStackAllocator::GetMaxStackSize()
{
	return maxStackSize;
}

DreamStaticStackAllocator::~DreamStaticStackAllocator()
{
	Clear();
	delete startPtr;
}
