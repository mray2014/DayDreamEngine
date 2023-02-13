#pragma once
#include <DreamMath.h>
#include <vector>
#include "DreamShader.h"
#include "DreamMaterial.h"

class DreamVertexArray;

enum UniformBufferLayout {
	ConstantData = 0,
	MaterialData = 1
};
struct ConstantUniformData {
	DreamMath::DreamMatrix4X4 viewMat;
	DreamMath::DreamMatrix4X4 projMat;
	float totalTime;
};

enum VertexDataUsage {
	StreamDraw, // the data is set only once and used by the GPU at most a few times.
	StaticDraw, // the data is set only once and used many times.
	DynamicDraw // the data is changed a lot and used many times.
};

using namespace DreamMath;
using UniformBindingPoints = std::unordered_map<std::string, unsigned int>;

class DreamShaderLinker;

class DreamGraphics
{
public:
	static DreamGraphics* GetInstance();
	static DreamShaderLinker* GenerateShaderLinker();
	static float GetAspectRatio();

	~DreamGraphics(); // Virtual?

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

	// Size of the window's client area
	unsigned int width = 800;
	unsigned int height = 600;

protected:
	DreamGraphics();
	DreamVector4 clearScreenColor;

	ConstantUniformData matConstData;
	DreamBuffer* matConstDataBuffer;
private:
	static DreamGraphics* myGrpahics;
	
};


class DreamShaderLinker {
protected:
	DreamShaderLinker();
	std::vector<DreamShader*> linkedShaders;
	UniformBindingPoints bindingPoints;
public:
	virtual ~DreamShaderLinker() {}
	virtual void AttachShader(DreamShader* shader) = 0;
	virtual void Finalize() = 0;
	virtual void BindShaderLink() = 0;
	virtual void UnBindShaderLink() = 0;

	template <typename T>
	inline bool UpdateUniform(const std::string& uniformName, T& data) {
		for (int i = 0; i < linkedShaders.size(); i++) {
			if (linkedShaders[i]->shaderUniforms.count(uniformName)) {
				DreamBuffer* buffer = linkedShaders[i]->shaderUniforms[uniformName].buffer;
				DreamGraphics::GetInstance()->UpdateBufferData(buffer, &data, sizeof(T));
				return true;
			}
		}
		return false;
	}

	template <typename T>
	inline bool UpdateUniformMemberData(const std::string& uniformName, const std::string& memberName, T& data) {

		for (int i = 0; i < linkedShaders.size(); i++) {
			if (linkedShaders[i]->shaderUniforms.count(uniformName)) {
				UniformMembers memberInfo = linkedShaders[i]->shaderUniforms[uniformName].uniformMembers;

				if (memberInfo.count(uniformName)) {
					//DreamGraphics::GetInstance()->UpdateBufferData(buffer, &data, sizeof(T));
					return true;
				}
			}
		}

		return false;
	}

	DreamBuffer* matDataBuffer;
	bool isMaterialRdy = false;
private:
	//friend class DreamGraphics;
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
