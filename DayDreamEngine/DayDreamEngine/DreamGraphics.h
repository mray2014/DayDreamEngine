#pragma once
#include <DreamMath.h>
#include <vector>
#include "DreamShader.h"

class DreamVertexArray;

enum VertexDataUsage {
	StreamDraw, // the data is set only once and used by the GPU at most a few times.
	StaticDraw, // the data is set only once and used many times.
	DynamicDraw // the data is changed a lot and used many times.
};

using namespace DreamMath;

class DreamShaderLinker {
protected:
	DreamShaderLinker();
	std::vector<DreamShader*> linkedShaders;
public:
	virtual ~DreamShaderLinker() {}
	virtual void AttachShader(DreamShader* shader) = 0;
	virtual void Finalize() = 0;
	virtual void BindShaderLink() = 0;
	virtual void UnBindShaderLink() = 0;
private:
	//friend class DreamGraphics;
};

class DreamGraphics
{
public:
	static DreamGraphics* GetInstance();
	static DreamShaderLinker* GenerateShaderLinker();
	~DreamGraphics();

	virtual long InitWindow(int w, int h, const char* title) = 0;
	virtual long InitGraphics() = 0;
	virtual void SetViewPort(int posX, int posY, int w, int h) = 0;
	virtual bool CheckWindowClose() = 0;
	virtual void ClearScreen() = 0;
	virtual void SwapBuffers() = 0;
	virtual void CheckInputs() = 0;
	virtual DreamVertexArray* GenerateVertexArray(DreamBuffer* vert, DreamBuffer* ind = nullptr) = 0;
	virtual DreamBuffer* GenerateBuffer(BufferType type, void* bufferData = nullptr, size_t numOfElements = 0, std::vector<size_t> strides = { 0 }, std::vector<size_t> offests = { 0 }, VertexDataUsage dataUsage = VertexDataUsage::StaticDraw) = 0;
	virtual void BindBuffer(BufferType type, DreamBuffer* buffer) = 0;
	virtual void BeginVertexLayout() = 0;
	virtual void AddVertexLayoutData(std::string dataName, int size, unsigned int dataType, bool shouldNormalize, unsigned int sizeOf) = 0;
	virtual DreamBuffer* FinalizeVertexLayout() = 0;
	virtual void UnBindBuffer(BufferType type) = 0;
	virtual bool LoadShader(const wchar_t* file, ShaderType shaderType, DreamPointer& ptr) = 0;
	virtual void ReleaseShader(DreamShader* shader) = 0;
	virtual void DrawWithIndex(size_t size) = 0;
	virtual void DrawWithVertex(size_t size) = 0;
	virtual void Draw() = 0;

	virtual void TerminateGraphics() = 0;
	virtual void DestroyWindow() = 0;
	virtual void DestroyBuffer(DreamBuffer* buffer) = 0;

	void SetScreenClearColor(DreamMath::DreamVector4 color)
	{
		clearScreenColor = color;
	}
	void SetScreenClearColor(float r, float g, float b, float a)
	{
		clearScreenColor.x = r;
		clearScreenColor.y = g;
		clearScreenColor.z = b;
		clearScreenColor.w = a;
	}

	// Size of the window's client area
	unsigned int width = 800;
	unsigned int height = 600;

protected:
	DreamGraphics();
	DreamVector4 clearScreenColor;

private:
	static DreamGraphics* myGrpahics;
	
};

class DreamVertexArray {
protected:
	DreamVertexArray() {
		vertexBuffer = nullptr;
		indexBuffer = nullptr;
	}
	DreamVertexArray(DreamBuffer* vert, DreamBuffer* ind = nullptr)
	{
		graphics = DreamGraphics::GetInstance();
		vertexBuffer = vert;
		indexBuffer = ind;
	}

public:
	~DreamVertexArray()
	{
		if (graphics) {

			if (vertexBuffer) {
				graphics->DestroyBuffer(vertexBuffer);
			}

			if (indexBuffer) {
				graphics->DestroyBuffer(indexBuffer);
			}
			
		}
		else {
			printf("WARNING: Mesh does not have refd to graphics while deleting itself, possible memory leak");
		}
	}

	DreamBuffer* vertexBuffer;
	DreamBuffer* indexBuffer;

	virtual void Bind() = 0;
	virtual void UnBind() = 0;
protected:
	DreamGraphics* graphics;
};
