
#include "pch.h"
#include <assert.h>
#include "DreamUnitTest.h"
#include "DreamFileIO.h"
#include "DreamMath.h"
#include "DreamPhysics.h"
#include "DreamAllocatorManager.h"
#include <vector>
#include <stack>

using namespace DreamMath;

static void ChunkUnitTestRun() 
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
static void StackAllocatorUnitTestRun() {

	DreamUnitTest::PrintUnitTestRunName("STACK ALLOCATOR UNIT TEST");

	DreamAllocatorManager::InitMainStackAllocator();

	ChunkUnitTestRun();

	DreamStaticStackAllocator* newStack = DreamAllocatorManager::CreateNewStackAllocator(400);
	DreamVector4* newVec4 = new(newStack->Allocate(sizeof(DreamVector4))) DreamVector4();

	void* newptr = newStack->Allocate(250);
	DreamAllocatorManager::MarkChunkOnMainSA("New Stack create here");
	ChunkUnitTestRun();

	DreamVector4* newVec42 = new(newStack->Allocate(sizeof(DreamVector4))) DreamVector4();
	newStack->PopChunk();

	DreamAllocatorManager::ShutDownMainStackAllocator();
}
static void VectorUnitTestRun() {
	DreamVector2 position = DreamVector2(3, 3);
	DreamVector2 position2 = DreamVector2(3, 3);

	position = position - position2;
	printf(position.ToString().c_str()); printf("\n");

	position -= position2;
	printf(position.ToString().c_str()); printf("\n");

	position *= 4;
	printf(position.ToString().c_str()); printf("\n");
}
static void FileIOUnitTestRun() {

	DreamUnitTest::PrintUnitTestRunName("FILE IO UNIT TEST");

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
		std::string line;
		while (DreamFileIO::ReadLine(line)) {
			printf("%s", line.c_str());
		}
		DreamFileIO::CloseFileRead();
	}

}
static void MathUnitTestRun() { 

	DreamUnitTest::PrintUnitTestRunName("MATH UNIT TEST");

	VectorUnitTestRun();

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

	printf("\n"); printf(rot.ToString().c_str()); printf("\n");

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

	DreamUnitTest::ClassUnitTest<DreamVector3>("Vector3 Unit Test 1", position1, DreamVector3());
	DreamUnitTest::ClassUnitTest<DreamVector3>("Vector3 Unit Test 2", position2, DreamVector3(0.4f, 2.0f, 4.8f));
	DreamUnitTest::ClassUnitTest<DreamVector3>("Vector3 Unit Test 3", position3, DreamVector3(0.5f, 2.5f, 6.0f));
	DreamUnitTest::ClassUnitTest<DreamVector3>("Vector3 Unit Test 4", position4, DreamVector3(0.7f, 3.5f, 8.4f));
	DreamUnitTest::ClassUnitTest<DreamVector3>("Vector3 Unit Test 5", position5, DreamVector3(1, 5, 12));

	worldMatrix.Transpose();
	DreamVector4 matrixMul = DreamVector4(1,1,1,1) * worldMatrix;

	printf("\n"); printf(worldMatrix.ToString().c_str()); printf("\n");
	printf("\n"); printf(matrixMul.ToString().c_str()); printf("\n");

	DreamQuaternion rotQuat = DreamQuaternion::MakeQuaternionEuler(0, 20, 90);
	DreamVector3 newFoward = rotQuat.RotateVector(0,0,1).GetNormalizedVector();

	DreamMatrix3X3 newRot = DreamMath::CreateRotationMatrix(0, 20, 90).Get3X3();

	DreamVector3 newFoward2 =  DreamVector3(0,0,1) * newRot;

	DreamMatrix3X3 newRot2 = rotQuat.GetMatrix();

	DreamVector3 newFoward3 = DreamVector3(0, 0, 1) * newRot2;


	DreamMatrix3X3 testMat = DreamMatrix3X3();
	testMat.matrix[0][2] = 1;

	testMat.matrix[1][0] = 2;
	testMat.matrix[1][1] = -1;
	testMat.matrix[1][2] = 3;

	testMat.matrix[2][0] = 1; 
	testMat.matrix[2][1] = 1;
	testMat.matrix[2][2] = 4;

	DreamMatrix3X3 storeMat = testMat;

	float det = testMat.Determinate();

	testMat.Inverse();

	DreamMatrix3X3 finalMat =  testMat * storeMat;

	DreamUnitTest::FloatUnitTest("Math Library Unit Test 1", 0.8660254f, DreamMath::sin(60));
	DreamUnitTest::FloatUnitTest("Math Library Unit Test 2", 0.5f, DreamMath::cos(60));
	DreamUnitTest::FloatUnitTest("Math Library Unit Test 3", 1.732050f, DreamMath::tan(60));
	DreamUnitTest::FloatUnitTest("Math Library Unit Test 4", -1.0f, DreamMath::asin(60), false);
	DreamUnitTest::FloatUnitTest("Math Library Unit Test 5", 1.18100003f, DreamMath::acos(0.38f));
	DreamUnitTest::FloatUnitTest("Math Library Unit Test 6", 0.363147009f, DreamMath::atan(0.38f));
	DreamUnitTest::FloatUnitTest("Math Library Unit Test 7", 1.0f, DreamMath::ceiling(0.5f));
	DreamUnitTest::FloatUnitTest("Math Library Unit Test 8", 64.0f, DreamMath::pow(4,3));
	DreamUnitTest::FloatUnitTest("Math Library Unit Test 9", 0.0f, DreamMath::floor(0.2f));
	DreamUnitTest::FloatUnitTest("Math Library Unit Test 10", 4.0f, DreamMath::sqrtf(16));
}

static void UnitTest() {
	StackAllocatorUnitTestRun();
	FileIOUnitTestRun();
	MathUnitTestRun();
}

int main()
{
	UnitTest();
}
