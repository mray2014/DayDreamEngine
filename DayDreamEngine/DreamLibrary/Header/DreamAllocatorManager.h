#pragma once
#include "DreamStaticStackAllocator.h"
#include "DreamPoolAllocator.h"

class DreamAllocatorManager
{
public:

	template <class T>
	static T* AllocateOnMainSA(AlignmentType type = AlignmentType::_16BitAlign);
	static void InitMainStackAllocator(uint32_t stackSize = MAX_STACK_MEMORY_SIZE);
	static void ShutDownMainStackAllocator();
	static void MarkChunkOnMainSA(const char* memChunkTitle);
	static void PopChunkOffMainSA();
	static void PopOffMainSA();
	static void ClearMainSA();
	static void GetMainStaticAllocatorUsedMemorySize();
	static void PrintMainStaticAllocatorUsedMemorySize();

	static DreamStaticStackAllocator* CreateNewStackAllocator(uint32_t stackSize);
	static DreamPoolAllocator* CreateNewPoolAllocator(uint32_t poolSize);

private:
	static DreamStaticStackAllocator* stackAllocator;
};
template<class T>
T * DreamAllocatorManager::AllocateOnMainSA(AlignmentType type)
{
	size_t classSize = sizeof(T);
	size_t allocMarkSize = sizeof(AllocationMark);

	void* newFront = (void*)((ptrdiff_t)stackAllocator->frontPtr + classSize + allocMarkSize);
	if (newFront > stackAllocator->endPtr) {
		printf("ERROR: Stack Overflow!! Allocate more space on the Main Stack and build again!");
		return nullptr;
	}

	void* lastAlloc = stackAllocator->backPtr;

	stackAllocator->backPtr = stackAllocator->frontPtr;
	stackAllocator->frontPtr = (void*)((ptrdiff_t)stackAllocator->frontPtr + classSize + allocMarkSize);

	AllocationMark* newAllocMark = new(stackAllocator->backPtr) AllocationMark;
	newAllocMark->lastAllocationMark = lastAlloc;

	stackAllocator->backPtr = (void*)((ptrdiff_t)stackAllocator->backPtr + allocMarkSize);

	T* newObj = new(stackAllocator->backPtr) T();

	stackAllocator->usedMemorySize += classSize + allocMarkSize;

	return newObj;
}
