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
	void GenerateVertexArray(size_t numOfBuffers, size_t& VBO) override;
	void GenerateBuffer(size_t numOfBuffers, size_t& VBO) override;
	void BindVertexArray(size_t& VBO) override;
	void BindBuffer(BufferType type, size_t& VBO) override;
	void CopyBufferData(BufferType type, size_t numOfElements, void* buffer, VertexDataUsage dataUsage) override;
	void AddVertexAttributePointer(int size, unsigned int dataType, bool shouldNormalize, unsigned int sizeOf) override;
	void UnBindVertexArray() override;
	unsigned int LoadShader(const char* file, ShaderType shaderType) override;
	void StartShaderProgramCreation() override;
	void AttachShader(unsigned int shader) override;
	unsigned int FinishShaderProgramCreation() override;
	void SetShader(unsigned int shaderProg) override;
	void Draw() override;

	void TerminateGraphics() override;

	void InitGlad();

private:
};

