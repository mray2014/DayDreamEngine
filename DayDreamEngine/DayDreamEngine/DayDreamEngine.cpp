#include <pch.h>
#include<assert.h>
#include <iostream>
#include "DreamGraphics.h"
#include <DreamAllocatorManager.h>
#include <DreamFileIO.h>

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
		char* line;
		while (DreamFileIO::ReadLine(&line)) {

			assert(line == testLines[i]);
			printf("%s\n", line);
			i++;
		}
		DreamFileIO::CloseFileRead();
	}
}

void UnitTestMath() {

	int num = 89; // result: 0.86006940581

	float sinCalc = DreamMath::round(DreamMath::sin(num));
	float cosCalc = DreamMath::round(DreamMath::cos(num));
	float tanCalc = DreamMath::round(DreamMath::tan(num));

	float sinResult = 1.00;
	float cosResult = 0.02;
	float tanResult = 57.29;

	printf("%f\n", sinCalc);
	printf("%f\n", cosCalc);
	printf("%f\n", tanCalc);

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

int main()
{
	UnitTest();

	DreamGraphics* graphics = DreamGraphics::GetInstance();

	graphics->InitGraphics();
	DreamPointer* windowPtr = graphics->CreateWindow(800, 600, "Hey It's a window!");
	graphics->CreateContext(windowPtr);
	graphics->FindCorrectFunctionPointers();

	graphics->SetViewPort(0, 0, 800, 600);
	graphics->SetWindowResizeCallBack(windowPtr);
	graphics->SetScreenClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	unsigned int vertexProgram = graphics->LoadShader("Shaders/vertex.glsl", ShaderType::Vertex);
	unsigned int pixelProgram = graphics->LoadShader("Shaders/pixel.glsl", ShaderType::Pixel);

	graphics->StartShaderProgramCreation();
	graphics->AttachShader(vertexProgram);
	graphics->AttachShader(pixelProgram);
	unsigned int shaderProg = graphics->FinishShaderProgramCreation();

	graphics->SetShader(shaderProg);

	struct Vertex {
		DreamVector3 pos;

		Vertex(DreamVector3 p) {
			pos = p;
		}
		Vertex(float x, float y, float z) {
			pos.x = x;
			pos.y = y;
			pos.z = z;
		}
	};

	Vertex vertices[] = {
		Vertex( -0.5, -0.5, 0.0f),
		Vertex(0.0, 0.5, 0.0f),
		Vertex( 0.5, -0.5, 0.0f)
		
	};

	size_t VBO;

	size_t VAO;

	graphics->GenerateVertexArray(1, VAO);
	graphics->GenerateBuffer(1, VBO);

	graphics->BindVertexArray(VAO);
	graphics->BindBuffer(BufferType::ArrayBuffer, VBO);
	graphics->CopyVertexBufferData(sizeof(Vertex) * 3, &vertices, VertexDataUsage::StaticDraw);
	graphics->AddVertexAttributePointer(3, 0, false, sizeof(Vertex));
	graphics->UnBindVertexArray();

	graphics->BindVertexArray(VAO);
	while (!graphics->CheckWindowClose(windowPtr))
	{
		graphics->ClearScreen();

		graphics->Draw();

		graphics->SwapBuffers(windowPtr);
		graphics->CheckInputs();
	}

	graphics->TerminateGraphics();
	delete windowPtr;
	return 0;
}
