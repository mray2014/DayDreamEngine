#pragma once
#include <DreamMath.h>
#include <vector>

enum BufferType {
	ArrayBuffer,
	ElementArrayBuffer
};
enum VertexDataUsage {
	StreamDraw, // the data is set only once and used by the GPU at most a few times.
	StaticDraw, // the data is set only once and used many times.
	DynamicDraw // the data is changed a lot and used many times.
};
enum ShaderType {
	VertexShader,
	PixelShader,
	GeometryShader,
	ComputeShader,
};

using namespace DreamMath;

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

struct DreamShader {
	const ShaderType type;
	const char* filePath;

	DreamShader(ShaderType pType, const char* pFilePath) : type(pType), filePath (pFilePath) {}
};

struct DreamShaderLinker {
	std::vector<DreamShader> shaderLinks;
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

	virtual void SetScreenClearColor(DreamMath::DreamVector4 color) = 0;
	virtual void SetScreenClearColor(float r, float g, float b, float a) = 0;
	virtual void ClearScreen() = 0;
	virtual void SwapBuffers(DreamPointer* window) = 0;
	virtual void CheckInputs() = 0;
	virtual void GenerateVertexArray(size_t numOfBuffers, size_t& VBO) = 0;
	virtual void GenerateBuffer(size_t numOfBuffers, size_t& VBO) = 0;
	virtual void BindVertexArray(size_t& VBO) = 0;
	virtual void BindBuffer(BufferType type, size_t& VBO) = 0;
	virtual void CopyBufferData(BufferType type, size_t numOfElements, void* buffer, VertexDataUsage dataUsage) = 0;
	virtual void AddVertexAttributePointer(int size, unsigned int dataType, bool shouldNormalize, unsigned int sizeOf) = 0;
	virtual void UnBindVertexArray() = 0;
	virtual unsigned int LoadShader(const char* file, ShaderType shaderType) = 0;
	virtual void StartShaderProgramCreation() = 0;
	virtual void AttachShader(unsigned int shader) = 0;
	virtual unsigned int FinishShaderProgramCreation() = 0;
	virtual void SetShader(unsigned int shaderProg) = 0;
	virtual void DrawWithIndex(size_t size) = 0;
	virtual void DrawWithVertex(size_t size) = 0;
	virtual void Draw() = 0;

	virtual 


	virtual void TerminateGraphics() = 0;
protected:
	DreamGraphics();
private:
	static DreamGraphics* myGrpahics;
	
};

