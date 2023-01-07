#pragma once
#include "DreamGraphics.h"
#include <Windows.h>

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

class DreamDXVertexArray : public DreamVertexArray {
public:
	DreamDXVertexArray(DreamBuffer* vert, DreamBuffer* ind = nullptr);
	~DreamDXVertexArray();

	void Bind() override;
	void UnBind() override;
};

class DreamDXGraphics : public DreamGraphics
{
public:
	DreamDXGraphics();
	~DreamDXGraphics();

	long InitWindow(int w, int h, const char* title) override;
	long InitGraphics() override;
	void SetViewPort(int posX, int posY, int w, int h) override;
	bool CheckWindowClose() override;
	void ClearScreen() override;
	void SwapBuffers() override;
	void CheckInputs() override;
	DreamVertexArray* GenerateVertexArray(DreamBuffer* vert, DreamBuffer* ind = nullptr);
	DreamBuffer* GenerateBuffer(BufferType type, void* bufferData = nullptr, size_t numOfElements = 0, std::vector<size_t> strides = { 0 }, std::vector<size_t> offests = { 0 }, VertexDataUsage dataUsage = VertexDataUsage::StaticDraw) override;
	void BindBuffer(BufferType type, DreamBuffer* buffer) override;
	void BeginVertexLayout() override;
	void AddVertexLayoutData(std::string dataName, int size, unsigned int dataType, bool shouldNormalize, unsigned int sizeOf) override;
	DreamBuffer* FinalizeVertexLayout() override;
	void UnBindBuffer(BufferType type) override;
	bool LoadShader(const wchar_t* file, ShaderType shaderType, DreamPointer& ptr) override;
	void ReleaseShader(DreamShader* shader) override;
	void SetShader(DreamShader* shader);
	void DrawWithIndex(size_t size) override;
	void DrawWithVertex(size_t size) override;
	void Draw() override;

	void TerminateGraphics() override;
	void DestroyWindow() override;
	void DestroyBuffer(DreamBuffer* buffer) override;

	void BindVertexLayout(DreamBuffer* layout);
	void UnBindVertexLayout();


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
};

class DreamDXShaderLinker : public DreamShaderLinker {
protected:
	DreamDXShaderLinker();
	~DreamDXShaderLinker();
public:
	void AttachShader(DreamShader* shader) override;
	void Finalize() override;
	void BindShaderLink() override;
	void UnBindShaderLink() override;
private:
	DreamGraphics* graphics;
	friend class DreamGraphics;
};

