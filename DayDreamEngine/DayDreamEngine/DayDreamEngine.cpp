#include <pch.h>
#include <assert.h>
#include <iostream>
#include "DreamGraphics.h"
#include "DreamAssetManager.h"
#include "DreamMesh.h"
#include <DreamAllocatorManager.h>
#include <DreamFileIO.h>
#include <DreamUnitTest.h>
#include <DreamTimeManager.h>
#include <Windows.h>
#include "DreamCameraManager.h"
#include "DreamMaterial.h"
#include "DreamTexture.h"

void UnitTestFileIO() {

	std::string testLines[]{
	"Yaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaahh",
	"Yoooooooo",
	"Yaeeeeeeeeeee",
	"Yweeeeeeeeeeeeeee",
	};

	DreamFileIO::OpenFileWrite("word.txt", FileWriteType::OverWrite);

	for (int i = 0; i < sizeof(testLines) / sizeof(std::string); i++) {
		DreamFileIO::WriteLine(testLines[i].c_str());
	}
	DreamFileIO::CloseFileWrite();

	if (DreamFileIO::OpenFileRead("word.txt")) {
		int i = 0;
		std::string line;
		while (DreamFileIO::ReadLine(line)) {

			assert(line == testLines[i]);
			printf("%s\n", line.c_str());
			i++;
		}
		DreamFileIO::CloseFileRead();
	}
}

void UnitTestMath() {

	DreamUnitTest::PrintUnitTestRunName("MATH TEST");

	float num = 89.0f; // result: 0.86006940581

	float sinCalc = DreamMath::D_round(DreamMath::D_sin(num));
	float cosCalc = DreamMath::D_round(DreamMath::D_cos(num));
	float tanCalc = DreamMath::D_round(DreamMath::D_tan(num));

	float sinResult = 1.00f;
	float cosResult = 0.02f;
	float tanResult = 57.29f;

	DreamUnitTest::FloatUnitTest("Sin Math Unit Test", sinResult, sinCalc);
	DreamUnitTest::FloatUnitTest("Cos Math Unit Test", cosResult, cosCalc);
	DreamUnitTest::FloatUnitTest("Tan Math Unit Test", tanResult, tanCalc);

	assert(sinResult == sinCalc);
	assert(cosResult == cosCalc);
	assert(tanResult == tanCalc);
}

void UnitTestAllocators() {
	DreamAllocatorManager::InitMainStackAllocator();
	DreamMath::DreamVector3* newPosition = DreamAllocatorManager::AllocateOnMainSA<DreamMath::DreamVector3>();

	printf("x: %f, y: %f, z: %f\n", newPosition->x, newPosition->y, newPosition->z);

	DreamAllocatorManager::ShutDownMainStackAllocator();
}

void UnitTest() {
	UnitTestFileIO();
	UnitTestMath();
	UnitTestAllocators();
}

// NOTE: Dont run on a Samsung Fridge
int main()
{
	UnitTest();

	// TODO: implement BVH or spatial hash to order visble objects in the scene dynamically
	DreamGraphics* graphics = DreamGraphics::GetInstance();

	graphics->InitWindow(800, 600, "Hey It's a window!");
	graphics->InitGraphics();
	graphics->InitConstData();
	graphics->SetScreenClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	//graphics->SetViewPort(0,0, 200, 200); // TODO: doesnt work halp ;-;

	DreamAssetManager* assetManager = DreamAssetManager::GetInstance();
	assetManager->LoadAssets();

	DreamMaterial* defaultMat = new DreamMaterial(assetManager->GetShaderLinker("defaultShaderLinker"));
	DreamMaterial* TextureMat = new DreamMaterial(assetManager->GetShaderLinker("textureShaderLinker"));
	TextureMat->StoreTexture("texSampler", assetManager->GetTexture("HUH"));

	DreamMaterial* TextureMat2 = new DreamMaterial(assetManager->GetShaderLinker("textureShaderLinker"));
	TextureMat2->StoreTexture("texSampler", assetManager->GetTexture("FrenchFries"));

	std::vector<DreamGameObject*> objList;
	DreamGameObject* triangleObj = new DreamGameObject(assetManager->GetMesh("triangleMesh"), defaultMat);
	DreamGameObject* parallogramObj = new DreamGameObject(assetManager->GetMesh("parallogramMesh"), defaultMat);
	DreamGameObject* squareObj = new DreamGameObject(assetManager->GetMesh("squareMesh"), TextureMat); // TODO: can't use same material on Vulkan Api
	DreamGameObject* squareObj2 = new DreamGameObject(assetManager->GetMesh("squareMesh"), TextureMat2); // TODO: can't use same material on Vulkan Api
	DreamGameObject* cube = new DreamGameObject(assetManager->GetMesh("teapot"), TextureMat); // TODO: can't use same material on Vulkan Api
	objList.push_back(parallogramObj);
	objList.push_back(triangleObj);
	objList.push_back(squareObj);
	objList.push_back(squareObj2);
	objList.push_back(cube);

	parallogramObj->transform.position = DreamVector3(0, 0, -5);
	squareObj->transform.position = DreamVector3(3, 0, -3);
	squareObj->transform.Rotate(DreamVector3(0, 45, 0));

	squareObj2->transform.position = DreamVector3(-3, 0, -3);
	squareObj2->transform.Rotate(DreamVector3(0, -45, 0));

	cube->transform.position = DreamVector3(0, 0, -3);


	DreamCameraManager* camManager = DreamCameraManager::GetInstance();
	DreamCamera* camera = new DreamCamera();
	camManager->SetActiveCamera(camera);

	DreamTimeManager::Init();
	while (!graphics->CheckWindowClose())
	{
		DreamTimeManager::Update();
		
		// Controller
		graphics->CheckInputs(); //TODO: needs to be on input manager

		// Model
		camera->Update();

		// View
		graphics->ClearScreen();
		graphics->Update();

		//NOTE: there is a black line on the side of the drawn triangle, halp
		// TODO: Bind Graphic pipelines instead of shaders individually
		// TODO: Spirv-Cross to handle all shader platform types
		for (unsigned int i = 0; i < objList.size(); i++) {
			objList[i]->Draw();
		}
		graphics->Draw();

		graphics->SwapBuffers();
		graphics->currentFrame = (graphics->currentFrame + 1) % graphics->GetMaxFramesInFlight();
	}

	for (unsigned int i = 0; i < objList.size(); i++) {
		if (objList[i]) {
			delete objList[i];
			objList[i] = nullptr;
		}
		objList.clear();
	}

	// TODO: Garbage Collector? delay deletes till end of frame [MarkForDeletion]
	DreamAssetManager::DestroyInstance();
	graphics->DestroyWindow();
	graphics->TerminateGraphics();

	if (graphics) {
		delete graphics;
		graphics = nullptr;
	}

	return 0;
}
