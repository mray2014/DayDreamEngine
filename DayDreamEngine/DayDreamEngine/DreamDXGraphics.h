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

	// DirectX related objects and variables
	D3D_FEATURE_LEVEL		dxFeatureLevel;
	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* context;

	ID3D11RenderTargetView* backBufferRTV;
	ID3D11DepthStencilView* depthStencilView;

private:
};

