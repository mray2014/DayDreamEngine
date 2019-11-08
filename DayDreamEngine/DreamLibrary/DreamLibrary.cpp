
#include "pch.h"
#include <assert.h>
#include <iostream>
#include "DreamFileIO.h"
#include "DreamMath.h"
#include "DreamPhysics.h"
#include "DreamAllocatorManager.h"
#include <vector>
#include <stack>

using namespace DreamMath;

static void ChunkUnitTest() 
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
static void CreatingNewStackUnitTest() {
	DreamStaticStackAllocator* newStack = DreamAllocatorManager::CreateNewStackAllocator(300);
	DreamVector4* newVec4 = new(newStack->Allocate(sizeof(DreamVector4))) DreamVector4();

	void* newptr = newStack->Allocate(250);
	DreamAllocatorManager::MarkChunkOnMainSA("New Stack create here");
	ChunkUnitTest();

	DreamVector4* newVec42 = new(newStack->Allocate(sizeof(DreamVector4))) DreamVector4();
	newStack->PopChunk();
}
static void VectorUnitTest() {
	DreamVector2 position = DreamVector2(3, 3);
	DreamVector2 position2 = DreamVector2(3, 3);

	position = position - position2;
	printf("x: %f, y: %f\n", position.x, position.y);

	position -= position2;
	printf("x: %f, y: %f\n", position.x, position.y);

	position *= 4;
	printf("x: %f, y: %f\n", position.x, position.y);
}

static void FileIOUnitTesting() {

	DreamFileIO::OpenFileWrite("word.txt", FileWriteType::OverWrite);
	DreamFileIO::WriteLine("Yahh");
	DreamFileIO::WriteLine("Yo");
	DreamFileIO::WriteLine("Yae");
	DreamFileIO::WriteLine("Ywe");
	DreamFileIO::CloseFileWrite();

	DreamFileIO::OpenFileWrite("word.txt", FileWriteType::OverWrite);
	DreamFileIO::WriteLine("Yaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaahh");
	DreamFileIO::WriteLine("Yoooooooo");
	DreamFileIO::WriteLine("Yaeeeeeeeeeee");
	DreamFileIO::WriteLine("Yweeeeeeeeeeeeeee");
	DreamFileIO::CloseFileWrite();

	if (DreamFileIO::OpenFileRead("wordWEEEE.text")) {
		printf("HOW THE FUCK DID GET IN HERE???!!!!!???");
	}
	if (DreamFileIO::OpenFileRead("word.txt")) {
		char* line;
		while (DreamFileIO::ReadLine(&line)) {
			printf("%s", &line[0]);
		}
		DreamFileIO::CloseFileRead();
	}

}
static void MathUnitTest() { 

	DreamMatrix4X4 indentity = DreamMatrix4X4::Identity();

	DreamMatrix4X4 test = DreamMatrix4X4();

	test.matrix[0][0] = 4;
	test.matrix[0][1] = 4;
	test.matrix[0][2] = 4;
	test.matrix[0][3] = 4;

	test.matrix[1][0] = 4;
	test.matrix[1][1] = 4;
	test.matrix[1][2] = 4;
	test.matrix[1][3] = 4;

	test.matrix[2][0] = 4;
	test.matrix[2][1] = 4;
	test.matrix[2][2] = 4;
	test.matrix[2][3] = 4;

	test *= 2;
	indentity *= 2;

	test *= indentity;

	DreamTransform newTransform = DreamTransform();
	newTransform.position = DreamVector3(20,19,400);
	newTransform.rotation = DreamVector3(0, 20, 90);
	newTransform.scale = DreamVector3(1, 1, 1);

	DreamMatrix4X4 worldMatrix = newTransform.GetWorldMatrix();

	worldMatrix.Transpose();

	DreamMatrix3X3 rot = worldMatrix.Get3X3();

	rot.Transpose();

	DreamMatrix4X4 rot4X4 = rot;

	float lerpedNum = DreamMath::lerp(4, 5, 0.8f);
	assert(DreamMath::FixFloatingPointError(lerpedNum - 4.8f) == 0);

	DreamVector3 vecA = DreamVector3();
	DreamVector3 vecB = DreamVector3(1, 5, 12);

	DreamVector3 position1 = DreamVector3::lerp(vecA, vecB, 0.0f);
	DreamVector3 position2 = DreamVector3::lerp(vecA, vecB, 0.4f);
	DreamVector3 position3 = DreamVector3::lerp(vecA, vecB, 0.5f);
	DreamVector3 position4 = DreamVector3::lerp(vecA, vecB, 0.7f);
	DreamVector3 position5 = DreamVector3::lerp(vecA, vecB, 1.0f);

	position1 += DreamVector3();

	assert(position1 == DreamVector3());
	assert(position2 == DreamVector3(0.4f, 2.0f, 4.8f));
	assert(position3 == DreamVector3(0.5f, 2.5f, 6.0f));
	assert(position4 == DreamVector3(0.7f, 3.5f, 8.4f));
	assert(position5 == DreamVector3(1, 5, 12));

	worldMatrix.Transpose();
	DreamVector4 matrixMul= DreamVector4(1,1,1,1) * worldMatrix;

	DreamQuaternion rotQuat = DreamQuaternion::MakeQuaternionEuler(45, 0, 0);
	DreamVector3 newFoward = rotQuat.RotateVector(0,0,1).GetNormalizedVector();

	DreamVector3 newFoward2 =  DreamVector3(0,0,1) * DreamMath::CreateRotationMatrix(45,0,0).Get3X3();

	assert(newFoward == DreamVector3(0, 0, -1));

	printf("\n%f" , DreamMath::sin(60));	// 0.8660254
	printf("\n%f", DreamMath::cos(60));		// 0.5
	printf("\n%f", DreamMath::tan(60));		// 1.732050
	printf("\n%f", DreamMath::asin(60));	// -1
	printf("\n%f", DreamMath::acos(0.38f));	// 1.18100003
	printf("\n%f", DreamMath::atan(0.38f));	// 0.363147009
	printf("\n%f", DreamMath::ceiling(0.5f)); // 1
	printf("\n%f", DreamMath::pow(4,3));	// 64
	printf("\n%f", DreamMath::floor(0.2f));	// 0
	printf("\n%f", DreamMath::sqrtf(16));	// 4
}

static void UnitTest() {

	VectorUnitTest();
	DreamAllocatorManager::InitMainStackAllocator();


	//ChunkTest();
	//CreatingNewStackTest();


	DreamAllocatorManager::ShutDownMainStackAllocator();

	FileIOUnitTesting();
	MathUnitTest();

}

int main()
{
	UnitTest();

}
