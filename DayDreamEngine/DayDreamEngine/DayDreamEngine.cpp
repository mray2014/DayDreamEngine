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

	std::vector<Vertex> mesh = std::vector<Vertex>();
	std::vector<unsigned int> indices = std::vector<unsigned int>();

	mesh.push_back(Vertex(-0.5, -0.5, 0.0f));
	mesh.push_back(Vertex(0.0, 0.5, 0.0f));
	mesh.push_back(Vertex(0.5, -0.5, 0.0f));
	mesh.push_back(Vertex(1.0, 0.5, 0.0f));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);

	int vertCount = mesh.size();
	int indicesCount = indices.size();

	size_t VBO; // Vertex Buffer
	size_t IBO; // Index Buffer
	size_t VAO; // Vertex Array

	graphics->GenerateVertexArray(1, VAO);
	graphics->GenerateBuffer(1, VBO);
	graphics->GenerateBuffer(1, IBO);
	
	// Copying vertices
	graphics->BindBuffer(BufferType::ArrayBuffer, VBO);
	graphics->CopyBufferData(BufferType::ArrayBuffer, sizeof(Vertex)* vertCount, &mesh[0], VertexDataUsage::StaticDraw);

	// Copying indices
	graphics->BindBuffer(BufferType::ElementArrayBuffer, IBO);
	graphics->CopyBufferData(BufferType::ElementArrayBuffer, sizeof(unsigned int)* indicesCount, &indices[0], VertexDataUsage::StaticDraw);

	// Setting up how to read vertice data
	graphics->BindVertexArray(VAO);
	graphics->AddVertexAttributePointer(3, 0, false, sizeof(DreamVector3));
	graphics->UnBindVertexArray();

	// Binding mesh to draw
	graphics->BindVertexArray(VAO);
	graphics->BindBuffer(BufferType::ElementArrayBuffer, IBO);
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
