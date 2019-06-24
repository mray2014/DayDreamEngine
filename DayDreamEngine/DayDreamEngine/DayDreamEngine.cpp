#include <pch.h>
#include <iostream>
#include "DreamGraphics.h"


int main()
{

	DreamGraphics* graphics = DreamGraphics::GetInstance();

	graphics->InitGraphics();
	DreamPointer* windowPtr = graphics->CreateWindow(800, 600, "Bitch Niggas");
	graphics->CreateContext(windowPtr);
	graphics->FindCorrectFunctionPointers();

	graphics->SetViewPort(0, 0, 800, 600);
	graphics->SetWindowResizeCallBack(windowPtr);
	graphics->SetScreenClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	while (!graphics->CheckWindowClose(windowPtr))
	{
		graphics->ClearScreen();


		graphics->SwapBuffers(windowPtr);
		graphics->CheckInputs();
	}

	graphics->TerminateGraphics();
	return 0;
}
