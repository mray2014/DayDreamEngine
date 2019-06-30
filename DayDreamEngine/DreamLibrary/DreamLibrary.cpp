
#include "pch.h"
#include <iostream>
#include "DreamMath.h"
#include "DreamPhysics.h"
#include "DreamAllocatorManager.h"

int main()
{
	DreamVector3 position = DreamVector3(5,2,5);
	//printf("x: %f, y: %f, z: %f\n", position.x, position.y, position.z);
	 
	DreamAllocatorManager::InitMainStackAllocator();
	DreamAllocatorManager::MarkChunkOnMainSA("Start of SA");
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



	DreamAllocatorManager::ShutDownMainStackAllocator();
}
