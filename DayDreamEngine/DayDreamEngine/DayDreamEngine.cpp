#include <pch.h>
#include<assert.h>
#include <iostream>
#include "DreamGraphics.h"
#include "DreamMesh.h"
#include <DreamAllocatorManager.h>
#include <DreamFileIO.h>
#include <DreamUnitTest.h>
#include <DreamTimeManager.h>
#include <Windows.h>
#include "DreamCameraManager.h"



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

	float sinCalc = DreamMath::round(DreamMath::sin(num));
	float cosCalc = DreamMath::round(DreamMath::cos(num));
	float tanCalc = DreamMath::round(DreamMath::tan(num));

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

	DreamGraphics* graphics = DreamGraphics::GetInstance();

	graphics->InitWindow(800, 600, "Hey It's a window!");
	graphics->InitGraphics();
	graphics->SetScreenClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	//graphics->SetViewPort(0,0, 200, 200);

	DreamShader* vertexShader = new VertexDreamShader(L"vertex");
	DreamShader* pixelShader = new PixelDreamShader(L"pixel");

	DreamShaderLinker* mainLink = DreamGraphics::GenerateShaderLinker();
	mainLink->AttachShader(vertexShader);
	mainLink->AttachShader(pixelShader);
	mainLink->Finalize();

	std::vector<DreamVertex> vert = std::vector<DreamVertex>();
	std::vector<uint32_t> indices = std::vector<uint32_t>();

	vert.push_back(DreamVertex(-0.5, -0.5, 0.0f));
	vert.push_back(DreamVertex(0.0, 0.5, 0.0f));
	vert.push_back(DreamVertex(0.5, -0.5, 0.0f));
	vert.push_back(DreamVertex(1.0, 0.5, 0.0f));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);

	//DreamMesh* mesh = new DreamMesh(mainLink, vert);
	DreamMesh* mesh = new DreamMesh(mainLink, vert, indices);

	DreamCameraManager* camManager = DreamCameraManager::GetInstance();
	DreamCamera* camera = new DreamCamera();
	camManager->SetActiveCamera(camera);

	//float zoom = 1.0f;
	//
	//float fovy = 3.14159f * 0.4f / zoom;
	//float aspect = DreamGraphics::GetAspectRatio();
	////float aspect = 1.0f;
	//float zNear = 0.01f;
	//float zFar = 1000.0f;
	//
	//glm::vec3 camPos = glm::vec3(0.0f, 0.0f, -10.0f);
	//glm::vec3 camCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	//glm::mat4 view = glm::lookAt(camPos, camCenter, up);
	//glm::mat4 proj = glm::perspective(fovy, aspect, zNear, zFar);
	//
	//glm::mat4 myTrans = glm::translate(glm::vec3(0, 0, 0));
	//glm::mat4 myScale = glm::scale(glm::vec3(1, 1, 1));
	//glm::mat4 myRot = glm::yawPitchRoll(0, 0, 0);
	//
	//glm::mat4 worldPos = myTrans * myRot * myScale;

	DreamTimeManager::Init();
	while (!graphics->CheckWindowClose())
	{
		DreamTimeManager::Update();

		// Controller
		graphics->CheckInputs();

		// Model
		camera->Update();
		printf("%s[%f]\n", camera->transform.position.ToString().c_str(), camera->transform.GetRotation().y);

		// View
		graphics->ClearScreen();

		//NOTE: there is a black line on the side of the drawn triangle, halp
		// TODO: Bind Graphic pipelines instead of shaders individually
		// TODO: Spirv-Cross to handle all shader platform types

		//glUniformMatrix4fv(1, 1, GL_FALSE, &worldPos[0][0]);
		//glUniformMatrix4fv(2, 1, GL_FALSE, &view[0][0]);
		//glUniformMatrix4fv(3, 1, GL_FALSE, &proj[0][0]);
		mesh->DrawOpaque();
		graphics->Draw();

		graphics->SwapBuffers();
	}

	if (mesh) {
		delete mesh;
		mesh = nullptr;
	}

	// TODO: Garbage Collector? delay deletes till end of frame [MarkForDeletion]
	graphics->ReleaseShader(vertexShader);
	graphics->ReleaseShader(pixelShader);
	graphics->DestroyWindow();
	graphics->TerminateGraphics();

	if (graphics) {
		delete graphics;
		graphics = nullptr;
	}

	return 0;
}
