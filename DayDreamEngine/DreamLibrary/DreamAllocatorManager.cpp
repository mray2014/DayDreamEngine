#include <iostream>
#include "DreamAllocatorManager.h"

DreamStaticStackAllocator* DreamAllocatorManager::stackAllocator = nullptr;

void DreamAllocatorManager::InitMainStackAllocator(uint32_t maxStackSize)
{
	if (stackAllocator == nullptr) {
		stackAllocator = new DreamStaticStackAllocator(maxStackSize);
	}
}

void DreamAllocatorManager::ShutDownMainStackAllocator()
{
	delete stackAllocator;
}

void DreamAllocatorManager::MarkChunkOnMainSA(const char * memChunkTitle)
{
	stackAllocator->MarkChunk(memChunkTitle);
}

void DreamAllocatorManager::PopChunkOffMainSA()
{
	stackAllocator->PopChunk();
}

void DreamAllocatorManager::PopOffMainSA()
{
	stackAllocator->Pop();
}

void DreamAllocatorManager::ClearMainSA()
{
	stackAllocator->Clear();
}

void DreamAllocatorManager::GetMainStaticAllocatorUsedMemorySize()
{
	stackAllocator->GetUsedMemorySize();
}

void DreamAllocatorManager::PrintMainStaticAllocatorUsedMemorySize()
{
	printf("Main Static Allocator Used Mem Size: %d\n", (int)stackAllocator->GetUsedMemorySize());
}

DreamStaticStackAllocator * DreamAllocatorManager::CreateNewStackAllocator(uint32_t stackSize)
{
	void* newStackPtr = stackAllocator->Allocate(sizeof(DreamStaticStackAllocator));

	DreamStaticStackAllocator* newStack = new(newStackPtr) DreamStaticStackAllocator(stackAllocator->Allocate(stackSize), stackSize);

	return newStack;
}

DreamPoolAllocator * DreamAllocatorManager::CreateNewPoolAllocator(uint32_t poolSize)
{
	void* newPoolPtr = stackAllocator->Allocate(sizeof(DreamPoolAllocator));

	DreamPoolAllocator* newPool = new(newPoolPtr) DreamPoolAllocator(stackAllocator->Allocate(poolSize), poolSize);

	return newPool;
}


