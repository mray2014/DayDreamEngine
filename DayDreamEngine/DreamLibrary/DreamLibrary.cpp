
#include "pch.h"
#include <iostream>
#include "DreamMath.h"
#include "DreamPhysics.h"
#include "DreamStackAllocator.h"

int main()
{
	DreamVector3 position = DreamVector3(5,2,5);
	printf("x: %f, y: %f, z: %f", position.x, position.y, position.z);

	DreamStackAllocator* stackAlloc = DreamStackAllocator::GetInstance();
	DreamVector3* newPosition = new(stackAlloc->Allocate(sizeof(DreamVector3), AlignmentType::_16BitAlign)) DreamVector3(2,5,4);

	printf("x: %f, y: %f, z: %f", newPosition->x, newPosition->y, newPosition->z);

	DreamStackAllocator::ShutDown();
}
