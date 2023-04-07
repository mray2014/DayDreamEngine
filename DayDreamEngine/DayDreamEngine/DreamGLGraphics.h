#pragma once
#include "DreamGraphics.h"

class DreamGLShaderLinker : public DreamShaderLinker {
protected:
	DreamGLShaderLinker();
public:
	void AttachShader(DreamShader* shader) override;
	void Finalize() override;
	void BindShaderLink(UniformIndexStore& indexStore, std::unordered_map<std::string, DreamTexture*> texMap) override;
	void UnBindShaderLink() override;
	
private:
	size_t prog;
	friend class DreamGraphics;
};

class DreamGLVertexArray : public DreamVertexArray {
public:
	DreamGLVertexArray(DreamBuffer* vert, DreamBuffer* ind = nullptr);
	~DreamGLVertexArray();

	void Bind() override;
	void UnBind() override;

	DreamPointer* VAO;
};

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
	DreamVertexArray* GenerateVertexArray(DreamBuffer* vert, DreamBuffer* ind = nullptr);
	DreamBuffer* GenerateBuffer(BufferType type, void* bufferData = nullptr, size_t numOfElements = 0, std::vector<size_t> strides = { 0 }, std::vector<size_t> offests = { 0 }, VertexDataUsage dataUsage = VertexDataUsage::StaticDraw) override;
	DreamBuffer* GenerateBuffer(BufferType type, size_t bufferSize = 0) override;
	DreamPointer* GenerateTexture(unsigned char* textureData, int texWidth, int texHeight) override;
	void UpdateBufferData(DreamBuffer* buffer, void* bufferData = nullptr, size_t bufSize = 0, VertexDataUsage dataUsage = VertexDataUsage::StaticDraw) override;
	void BindBuffer(BufferType type, DreamBuffer* buffer) override;
	void BindTexture(DreamTexture* texture, int bindingPoint) override;
	void BeginVertexLayout() override;
	void AddVertexLayoutData(std::string dataName, int size, unsigned int location, bool shouldNormalize, unsigned int sizeOf) override;
	DreamPointer* FinalizeVertexLayout() override;
	void UnBindBuffer(BufferType type) override;
	DreamShader* LoadShader(const wchar_t* file, ShaderType shaderType) override;
	void ReleaseShader(DreamShader* shader) override;
	void DrawWithIndex(size_t size) override;
	void DrawWithVertex(size_t size) override;
	void Draw() override;

	void TerminateGraphics() override;
	void DestroyWindow() override;
	void DestroyBuffer(DreamBuffer* buffer) override;

	void InitGlad();

private:

	
};

