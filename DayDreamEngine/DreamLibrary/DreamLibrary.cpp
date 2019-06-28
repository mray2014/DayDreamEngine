
#include "pch.h"
#include <iostream>
#include "DreamMath.h"
#include "DreamPhysics.h"
#include "DreamStaticStackAllocator.h"

int main()
{
	DreamVector3 position = DreamVector3(5,2,5);
	printf("x: %f, y: %f, z: %f", position.x, position.y, position.z);

	DreamStaticStackAllocator* stackAlloc = DreamStaticStackAllocator::GetInstance();
	DreamVector3* newPosition1 = new(stackAlloc->Allocate(sizeof(DreamVector3), AlignmentType::_16BitAlign)) DreamVector3(5,2,1);
	//DreamVector3* newPosition1 = DreamStaticStackAllocator::AllocateThat<DreamVector3>(AlignmentType::_16BitAlign);
	//int* newPosition1 = DreamStaticStackAllocator::AllocateThat<int>(AlignmentType::_16BitAlign);


	//printf("x: %f, y: %f, z: %f", newPosition1->x, newPosition1->y, newPosition1->z);


	DreamStaticStackAllocator::ShutDown();
}
