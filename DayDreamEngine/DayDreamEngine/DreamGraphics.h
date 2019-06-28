#pragma once
#include <DreamMath.h>

class DreamPointer {
public:
	DreamPointer(void* newPtr) {
		ptr = newPtr;
	}
	~DreamPointer() {

	}
	void* GetStoredPointer() {
		return ptr;
	}
private:
	void* ptr;
};

class DreamGraphics
{
public:
	static DreamGraphics* GetInstance();
	~DreamGraphics();

	virtual void InitGraphics() = 0;
	virtual DreamPointer* CreateWindow(int width, int height, const char* title) = 0;
	virtual void CreateContext(DreamPointer* window) = 0;
	virtual void SetViewPort(int posX, int posY, int width, int height) = 0;
	virtual void SetWindowResizeCallBack(DreamPointer* window) = 0;
	virtual bool CheckWindowClose(DreamPointer* window) = 0;
	virtual void FindCorrectFunctionPointers() = 0;

	virtual void SetScreenClearColor(DreamVector4 color) = 0;
	virtual void SetScreenClearColor(float r, float g, float b, float a) = 0;
	virtual void ClearScreen() = 0;
	virtual void SwapBuffers(DreamPointer* window) = 0;
	virtual void CheckInputs() = 0;


	virtual void TerminateGraphics() = 0;
protected:
	DreamGraphics();
private:
	static DreamGraphics* myGrpahics;
	
};

