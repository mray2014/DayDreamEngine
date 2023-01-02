#pragma once
#include "DreamGraphics.h"

class DreamGLGraphics : public DreamGraphics
{
public:
	DreamGLGraphics();
	~DreamGLGraphics();

	long InitWindow(int w, int h, const char* title) override;
	long InitGraphics() override;
	void SetViewPort(int posX, int posY, int w, int h) override;
	bool CheckWindowClose() override;

	void ClearScreen() override;
	void SwapBuffers() override;
	void CheckInputs() override;
	//void GenerateVertexArray(size_t numOfBuffers, size_t& VBO) override;
	void GenerateBuffer(BufferType type, size_t& VBO, size_t numOfBuffers = 1, void* bufferData = nullptr, size_t numOfElements = 0, VertexDataUsage dataUsage = VertexDataUsage::StaticDraw) override;
	//void BindVertexArray(size_t& VBO) override;
	void BindBuffer(BufferType type, size_t& VBO) override;
	//void CopyBufferData(BufferType type, size_t numOfElements, void* buffer, VertexDataUsage dataUsage) override;
	void AddVertexAttributePointer(int size, unsigned int dataType, bool shouldNormalize, unsigned int sizeOf) override;
	void UnBindBuffer(BufferType type) override;
	unsigned int LoadShader(const char* file, ShaderType shaderType) override;
	void StartShaderProgramCreation() override;
	void AttachShader(unsigned int shader) override;
	unsigned int FinishShaderProgramCreation() override;
	void SetShader(unsigned int shaderProg) override;
	void DrawWithIndex(size_t size) override;
	void DrawWithVertex(size_t size) override;
	void Draw() override;

	void TerminateGraphics() override;

	void DestroyWindow() override;

	void InitGlad();

private:

	
};

