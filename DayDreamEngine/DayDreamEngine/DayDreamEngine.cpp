#include <pch.h>
#include <iostream>
#include "DreamGraphics.h"
#include <DreamAllocatorManager.h>
#include <DreamFileIO.h>

int main()
{
	DreamFileIO::OpenFileWrite("word.txt", FileWriteType::OverWrite);
	DreamFileIO::WriteLine("Yaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaahh");
	DreamFileIO::WriteLine("Yoooooooo");
	DreamFileIO::WriteLine("Yaeeeeeeeeeee");
	DreamFileIO::WriteLine("Yweeeeeeeeeeeeeee");
	DreamFileIO::CloseFileWrite();

	if (DreamFileIO::OpenFileRead("word.txt")) {
		char* line;
		while (DreamFileIO::ReadLine(&line)) {
			printf("%s", line);
		}
		DreamFileIO::CloseFileRead();
	}


	printf("%f", DreamMath::sin(89));
	printf("%f", DreamMath::cos(89));
	printf("%f", DreamMath::tan(89));

	DreamAllocatorManager::InitMainStackAllocator();
	DreamMath::DreamVector3* newPosition = DreamAllocatorManager::AllocateOnMainSA<DreamMath::DreamVector3>();

	printf("x: %f, y: %f, z: %f", newPosition->x, newPosition->y, newPosition->z);

	DreamAllocatorManager::ShutDownMainStackAllocator();

	DreamGraphics* graphics = DreamGraphics::GetInstance();

	graphics->InitGraphics();
	DreamPointer* windowPtr = graphics->CreateWindow(800, 600, "Bitch Boiss");
	graphics->CreateContext(windowPtr);
	graphics->FindCorrectFunctionPointers();

	graphics->SetViewPort(0, 0, 800, 600);
	graphics->SetWindowResizeCallBack(windowPtr);
	graphics->SetScreenClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	float vertices[] = {
		-0.5, -0.5, 0.0f,
		 0.5, -0.5, 0.0f,
		-0.0, 0.5, 0.0f
	};

	size_t VBO;

	graphics->GenerateBuffer(1, VBO);
	graphics->BindBuffer(BufferType::ArrayBuffer, VBO);
	graphics->CopyVertexBufferData( 9, &vertices, VertexDataUsage::StaticDraw);

	while (!graphics->CheckWindowClose(windowPtr))
	{
		graphics->ClearScreen();


		graphics->SwapBuffers(windowPtr);
		graphics->CheckInputs();
	}

	graphics->TerminateGraphics();
	delete windowPtr;
	return 0;
}
