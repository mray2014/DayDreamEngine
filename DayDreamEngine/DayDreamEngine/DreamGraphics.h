#pragma once
#include <DreamMath.h>
#include <vector>
#include "DreamShader.h"
#include "DreamMaterial.h"
#include "DreamShaderLinker.h"
#include "DreamMesh.h"
#include <SPIRV/spirv_cross.hpp>

class DreamVertexArray;

enum VertexDataUsage {
	StreamDraw, // the data is set only once and used by the GPU at most a few times.
	StaticDraw, // the data is set only once and used many times.
	DynamicDraw // the data is changed a lot and used many times.
};

using namespace DreamMath;

//class DreamShaderLinker;

class DreamGraphics
{
public:
	static DreamGraphics* GetInstance();
	static DreamShaderLinker* GenerateShaderLinker();
	static float GetAspectRatio();

	~DreamGraphics(); // Virtual?
	void InitConstData();

	virtual void Update();

	virtual long InitWindow(int w, int h, const char* title) = 0;
	virtual long InitGraphics() = 0;
	virtual void SetViewPort(int posX, int posY, int w, int h) = 0;
	virtual bool CheckWindowClose() = 0;
	virtual void ClearScreen() = 0;
	virtual void SwapBuffers() = 0;
	virtual void CheckInputs() = 0;
	virtual DreamVertexArray* GenerateVertexArray(DreamBuffer* vert, DreamBuffer* ind = nullptr) = 0;
	virtual DreamBuffer* GenerateBuffer(BufferType type, void* bufferData = nullptr, size_t numOfElements = 0, std::vector<size_t> strides = { 0 }, std::vector<size_t> offests = { 0 }, VertexDataUsage dataUsage = VertexDataUsage::StaticDraw) = 0;
	virtual DreamBuffer* GenerateBuffer(BufferType type, size_t bufferSize = 0) = 0;
	virtual void UpdateBufferData(DreamBuffer* buffer, void* bufferData = nullptr, size_t bufSize = 0, VertexDataUsage dataUsage = VertexDataUsage::StaticDraw) = 0;
	virtual void BindBuffer(BufferType type, DreamBuffer* buffer) = 0;
	virtual void BeginVertexLayout() = 0;
	virtual void AddVertexLayoutData(std::string dataName, int size, unsigned int dataType, bool shouldNormalize, unsigned int sizeOf) = 0;
	virtual DreamBuffer* FinalizeVertexLayout() = 0;
	virtual void UnBindBuffer(BufferType type) = 0;
	virtual DreamShader* LoadShader(const wchar_t* file, ShaderType shaderType) = 0;
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
	int GetMaxFramesInFlight() {
		return MAX_FRAMES_IN_FLIGHT;
	}

	DreamBuffer* CreateVertexInputLayout()
	{
		BeginVertexLayout();
		AddVertexLayoutData("TEXCOORD", 3, 0, false, sizeof(DreamVector3));
		AddVertexLayoutData("TEXCOORD", 3, 1, false, sizeof(DreamVector3));
		AddVertexLayoutData("TEXCOORD", 2, 2, false, sizeof(DreamVector2));
		return FinalizeVertexLayout();
	}


	// Size of the window's client area
	unsigned int width = 800;
	unsigned int height = 600;

	uint32_t currentFrame = 0;
	

protected:
	DreamGraphics();
	void LoadShaderResources(spirv_cross::Compiler& glsl, UniformList& uniList, bool& hasMat);
	DreamVector4 clearScreenColor;
	ConstantUniformData matConstData;
	LightUniformData lightData;
	UniformInfo constDataBufferInfo;
	UniformInfo lightBufferInfo;
	int MAX_FRAMES_IN_FLIGHT = 1;
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
