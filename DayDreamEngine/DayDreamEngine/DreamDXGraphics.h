#pragma once
#include "DreamGraphics.h"
#include <Windows.h>

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

class DreamDXGraphics : public DreamGraphics
{
public:
	DreamDXGraphics();
	~DreamDXGraphics();

	long InitWindow(int w, int h, const char* title) override;
	long InitGraphics() override;
	void SetViewPort(int posX, int posY, int width, int height) override;
	void SetWindowResizeCallBack() override;
	bool CheckWindowClose() override;

	void SetScreenClearColor(DreamMath::DreamVector4 color) override;
	void SetScreenClearColor(float r, float g, float b, float a) override;
	void ClearScreen() override;
	void SwapBuffers() override;
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
	void DrawWithIndex(size_t size) override;
	void DrawWithVertex(size_t size) override;
	void Draw() override;

	void TerminateGraphics() override;

	void DestroyWindow() override;

	static LRESULT CALLBACK WindowProc(
		HWND hWnd,		// Window handle
		UINT uMsg,		// Message
		WPARAM wParam,	// Message's first parameter
		LPARAM lParam	// Message's second parameter
	);
	LRESULT ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	HINSTANCE	hInstance;		// The handle to the application
	HWND		hWnd;			// The handle to the window itself
	std::string titleBarText;	// Custom text in window's title bar
	bool		titleBarStats;	// Show extra stats in title bar?

	// Size of the window's client area
	unsigned int width;
	unsigned int height;

	// DirectX related objects and variables
	D3D_FEATURE_LEVEL		dxFeatureLevel;
	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* context;

	ID3D11RenderTargetView* backBufferRTV;
	ID3D11DepthStencilView* depthStencilView;

private:
};

