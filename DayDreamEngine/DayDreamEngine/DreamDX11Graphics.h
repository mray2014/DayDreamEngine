#pragma once
#include <SPIRV/spirv_hlsl.hpp>
#pragma comment(lib, "spirv-cross-hlsld.lib")
#pragma comment(lib, "spirv-cross-glsld.lib")
#pragma comment(lib, "spirv-cross-cored.lib")


#include "DreamGraphics.h"
#include <Windows.h>

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

class DreamDX11VertexArray : public DreamVertexArray {
public:
	DreamDX11VertexArray(DreamBuffer* vert, DreamBuffer* ind = nullptr);
	~DreamDX11VertexArray();

	void Bind() override;
	void UnBind() override;
};

class DreamDX11Graphics : public DreamGraphics
{
public:
	DreamDX11Graphics();
	~DreamDX11Graphics();

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
	DreamPointer* GenerateTexture(unsigned char* pixelBuffer, int texWidth, int texHeight) override;
	void UpdateBufferData(DreamBuffer* buffer, void* bufferData = nullptr, size_t bufSize = 0, VertexDataUsage dataUsage = VertexDataUsage::StaticDraw) override;
	void BindBuffer(BufferType type, DreamBuffer* buffer) override;
	void BindTexture(DreamTexture* texture, int bindingPoint, ShaderType shaderType) override;
	void BeginVertexLayout() override;
	void AddVertexLayoutData(std::string dataName, int size, unsigned int location, bool shouldNormalize, unsigned int sizeOf) override;
	DreamPointer* FinalizeVertexLayout() override;
	void UnBindBuffer(BufferType type) override;
	DreamShader* LoadShader(const wchar_t* file, ShaderType shaderType) override;
	void ReleaseShader(DreamShader* shader) override;
	void SetShader(DreamShader* shader);
	void DrawWithIndex(size_t size) override;
	void DrawWithVertex(size_t size) override;
	void Draw() override;

	void TerminateGraphics() override;
	void DestroyWindow() override;
	void DestroyBuffer(DreamBuffer* buffer) override;

	void BindVertexLayout(DreamPointer* layout);
	void UnBindVertexLayout();

	void BindUniformBuffer(ShaderType shaderType, DreamBuffer* buffer, unsigned int slotNum);


	static LRESULT CALLBACK WindowProc(
		HWND hWnd,		// Window handle
		UINT uMsg,		// Message
		WPARAM wParam,	// Message's first parameter
		LPARAM lParam	// Message's second parameter
	);
	LRESULT ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:


private:
	HINSTANCE	hInstance{};		// The handle to the application
	HWND		hWnd{};			// The handle to the window itself
	std::string titleBarText{};	// Custom text in window's title bar
	bool		titleBarStats{};	// Show extra stats in title bar?

	// DirectX related objects and variables
	D3D_FEATURE_LEVEL		dxFeatureLevel{};
	IDXGISwapChain* swapChain = {};
	ID3D11Device* device = {};
	ID3D11DeviceContext* context = {};

	ID3D11RenderTargetView* backBufferRTV = {};
	ID3D11DepthStencilView* depthStencilView = {};

	ID3D11SamplerState* textureSampler;
};

class DreamDX11ShaderLinker : public DreamShaderLinker {
protected:
	DreamDX11ShaderLinker();
	~DreamDX11ShaderLinker();
public:
	void AttachShader(DreamShader* shader) override;
	void Finalize() override;
	void BindShaderLink(UniformIndexStore& indexStore, std::unordered_map<std::string, DreamTexture*> texMap) override;
	void UnBindShaderLink() override;
private:
	DreamGraphics* graphics;
	friend class DreamGraphics;
};

