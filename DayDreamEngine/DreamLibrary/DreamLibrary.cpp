
#include "pch.h"
#include <iostream>
#include "DreamFileIO.h"
#include "DreamMath.h"
#include "DreamPhysics.h"
#include "DreamAllocatorManager.h"

static void ChunkTest() 
{
	DreamVector3* newPosition1 = DreamAllocatorManager::AllocateOnMainSA<DreamVector3>();
	DreamVector3* newPosition2 = DreamAllocatorManager::AllocateOnMainSA<DreamVector3>();
	DreamVector3* newPosition3 = DreamAllocatorManager::AllocateOnMainSA<DreamVector3>();
	DreamVector3* newPosition4 = DreamAllocatorManager::AllocateOnMainSA<DreamVector3>();
	DreamVector3* newPosition5 = DreamAllocatorManager::AllocateOnMainSA<DreamVector3>();
	DreamAllocatorManager::MarkChunkOnMainSA("A bunch on vectors");
	DreamVector3* newPosition6 = DreamAllocatorManager::AllocateOnMainSA<DreamVector3>();
	DreamVector3* newPosition7 = DreamAllocatorManager::AllocateOnMainSA<DreamVector3>();
	DreamVector3* newPosition8 = DreamAllocatorManager::AllocateOnMainSA<DreamVector3>();
	DreamVector3* newPosition9 = DreamAllocatorManager::AllocateOnMainSA<DreamVector3>();
	DreamAllocatorManager::MarkChunkOnMainSA("Second bunch on vectors");
	DreamVector3* newPosition10 = DreamAllocatorManager::AllocateOnMainSA<DreamVector3>();
	DreamVector3* newPosition11 = DreamAllocatorManager::AllocateOnMainSA<DreamVector3>();
	DreamAllocatorManager::PopChunkOffMainSA();
	DreamAllocatorManager::PopChunkOffMainSA();
	DreamAllocatorManager::PopChunkOffMainSA();
	DreamAllocatorManager::PopChunkOffMainSA();
}
static void CreatingNewStackTest() {
	DreamStaticStackAllocator* newStack = DreamAllocatorManager::CreateNewStackAllocator(300);
	DreamVector4* newVec4 = new(newStack->Allocate(sizeof(DreamVector4))) DreamVector4();

	void* newptr = newStack->Allocate(250);
	DreamAllocatorManager::MarkChunkOnMainSA("New Stack create here");
	ChunkTest();

	DreamVector4* newVec42 = new(newStack->Allocate(sizeof(DreamVector4))) DreamVector4();
	newStack->PopChunk();
}

int main()
{
	DreamVector2 position = DreamVector2(3,3);
	DreamVector2 position2 = DreamVector2(3,3);

	position = position - position2;
	printf("x: %f, y: %f\n", position.x, position.y);

	position -= position2;
	printf("x: %f, y: %f\n", position.x, position.y);

	position *= 4;
	printf("x: %f, y: %f\n", position.x, position.y);

	DreamAllocatorManager::InitMainStackAllocator();
	
	//ChunkTest();
	//CreatingNewStackTest();

	DreamAllocatorManager::ShutDownMainStackAllocator();
}
