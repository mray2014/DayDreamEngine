#pragma once
#include "DreamGraphics.h"

class DreamGLGraphics : public DreamGraphics
{
public:
	DreamGLGraphics();
	~DreamGLGraphics();

	void InitGraphics() override;
	DreamPointer* CreateWindow(int width, int height, const char* title) override;
	void CreateContext(DreamPointer* window) override;
	void SetViewPort(int posX, int posY, int width, int height) override;
	void SetWindowResizeCallBack(DreamPointer* window) override;
	bool CheckWindowClose(DreamPointer* window) override;
	void FindCorrectFunctionPointers() override;

	void SetScreenClearColor(DreamMath::DreamVector4 color) override;
	void SetScreenClearColor(float r, float g, float b, float a) override;
	void ClearScreen() override;
	void SwapBuffers(DreamPointer* window) override;
	void CheckInputs() override;
	void GenerateBuffer(size_t numOfBuffers, size_t& VBO) override;
	void BindBuffer(BufferType type, size_t& VBO) override;
	void CopyVertexBufferData(size_t numOfVerts, void* verts, VertexDataUsage dataUsage) override;

	void TerminateGraphics() override;

	void InitGlad();

private:
};

