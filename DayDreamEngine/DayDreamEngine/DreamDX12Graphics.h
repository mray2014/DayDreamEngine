#pragma once
#include "DreamGraphics.h"

#include "d3dx12/d3dx12.h"

#include <SPIRV/spirv_hlsl.hpp>
#pragma comment(lib, "spirv-cross-hlsld.lib")
#pragma comment(lib, "spirv-cross-glsld.lib")
#pragma comment(lib, "spirv-cross-cored.lib")

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <shellapi.h> // For CommandLineToArgvW
// The min/max macros conflict with like-named member functions.
// Only use std::min and std::max defined in <algorithm>.
#if defined(min)
#undef min
#endif

#if defined(max)
#undef max
#endif

// In order to define a function called CreateWindow, the Windows macro needs to
// be undefined.
#if defined(CreateWindow)
#undef CreateWindow
#endif

// Windows Runtime Library. Needed for Microsoft::WRL::ComPtr<> template class.
#include <wrl.h>
using namespace Microsoft::WRL;

// DirectX 12 specific headers.

#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <d3d12sdklayers.h>
//#include <DirectXMath.h>

#include <algorithm>

#include <cassert>

#include <chrono>



inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw std::exception();
	}

}


class DreamDX12VertexArray : public DreamVertexArray {
public:
	DreamDX12VertexArray(DreamBuffer* vert, DreamBuffer* ind = nullptr);
	~DreamDX12VertexArray();

	void Bind() override;
	void UnBind() override;
};

class DreamDX12Graphics : public DreamGraphics
{
public:
	DreamDX12Graphics();
	~DreamDX12Graphics();

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

	void BindVertexLayout(DreamBuffer* layout);
	void UnBindVertexLayout();

	static LRESULT CALLBACK WindowProc(
		HWND hWnd,		// Window handle
		UINT uMsg,		// Message
		WPARAM wParam,	// Message's first parameter
		LPARAM lParam	// Message's second parameter
	);
	LRESULT ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void EnableDebugLayer();
	ComPtr<IDXGIAdapter4> GetAdapter(bool useWarp);
	ComPtr<ID3D12Device2> CreateDevice(ComPtr<IDXGIAdapter4> adapter);
	ComPtr<ID3D12CommandQueue> CreateCommandQueue(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type);
	bool CheckTearingSupport();
	ComPtr<IDXGISwapChain4> CreateSwapChain(HWND hWnd, ComPtr<ID3D12CommandQueue> commandQueue, uint32_t width, uint32_t height, uint32_t bufferCount);
	ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_DESCRIPTOR_HEAP_FLAGS flags, uint32_t numDescriptors);
	void UpdateRenderTargetViews(ComPtr<ID3D12Device2> device, ComPtr<IDXGISwapChain4> swapChain, ComPtr<ID3D12DescriptorHeap> descriptorHeap);
	ComPtr<ID3D12CommandAllocator> CreateCommandAllocator(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type);
	ComPtr<ID3D12GraphicsCommandList> CreateCommandList(ComPtr<ID3D12Device2> device, ComPtr<ID3D12CommandAllocator> commandAllocator, D3D12_COMMAND_LIST_TYPE type);
	ComPtr<ID3D12Fence> CreateFence(ComPtr<ID3D12Device2> device);
	HANDLE CreateEventHandle();
	uint64_t Signal(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence, uint64_t& fenceValue);
	void WaitForFenceValue(ComPtr<ID3D12Fence> fence, uint64_t fenceValue, HANDLE fenceEvent, std::chrono::milliseconds duration = std::chrono::milliseconds::max());
	void Flush(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence, uint64_t& fenceValue, HANDLE fenceEvent);
	void Render();
	void SetFullscreen(bool fullscreen); // Add this graphic platorm
	ID3D12PipelineState* CreateGraphicPipeLine(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipeLineDesc, std::vector<CD3DX12_DESCRIPTOR_RANGE>& tableRangeList, std::vector<CD3DX12_ROOT_PARAMETER>& rootParametersList, DreamPointer* vertexLayoutPtr);
	void BindGraphicPipeLine(ID3D12PipelineState* pipeline, ID3D12RootSignature* rootSig, unsigned int heapCount = 0);

	void BindDescriptorTable(unsigned int index, unsigned int heapIndex);

protected:
	

private:
	HINSTANCE	hInst{};
	HWND		hWnd{};
	const static uint8_t g_NumFrames = 3;

	// Use WARP adapter
	bool g_UseWarp = false;

	// Set to true once the DX12 objects have been initialized.
	bool g_IsInitialized = false;

	// Window rectangle (used to toggle fullscreen state).
	RECT g_WindowRect;

	// DirectX 12 Objects
	ComPtr<ID3D12Device2> g_Device; // TODO: "Get rid of all the snake case spelling" ~ chat [4/18/2023]
	ComPtr<ID3D12CommandQueue> g_CommandQueue;
	ComPtr<IDXGISwapChain4> g_SwapChain;
	ComPtr<ID3D12Resource> g_BackBuffers[g_NumFrames];
	ComPtr<ID3D12GraphicsCommandList> g_CommandList;
	ComPtr<ID3D12CommandAllocator> g_CommandAllocators[g_NumFrames];
	ComPtr<ID3D12DescriptorHeap> g_RTVDescriptorHeap;
	ComPtr<ID3D12DescriptorHeap> g_DTVDescriptorHeap;
	ComPtr<ID3D12DescriptorHeap> CBV_SRV_UAV_Heap;
	UINT g_RTVDescriptorSize;
	UINT g_DTVDescriptorSize;
	UINT g_CurrentBackBufferIndex;


	// Synchronization objects
	ComPtr<ID3D12Fence> g_Fence;
	uint64_t g_FenceValue = 0;
	uint64_t g_FrameFenceValues[g_NumFrames] = {};
	HANDLE g_FenceEvent;

	// By default, enable V-Sync.
	// Can be toggled with the V key.
	bool g_VSync = true;
	bool g_TearingSupported = false;

	// By default, use windowed mode.
	// Can be toggled with the Alt+Enter or F11
	bool g_Fullscreen = false;
	bool quit = false;

	bool bInitalFrame = true;

	uint32_t m_DescriptorHeapCount = 0;
};

class DreamDX12ShaderLinker : public DreamShaderLinker {
protected:
	DreamDX12ShaderLinker();
	~DreamDX12ShaderLinker() override;
public:
	void AttachShader(DreamShader* shader) override;
	void Finalize() override;
	void BindShaderLink(UniformIndexStore& indexStore, std::unordered_map<std::string, DreamTexture*> texMap) override;
	void UnBindShaderLink() override;
private:
	DreamDX12Graphics* dx12Graphics;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeLineDesc;
	ID3D12PipelineState* pipelineState;
	std::vector<CD3DX12_DESCRIPTOR_RANGE> tableRangeList;
	std::vector<CD3DX12_ROOT_PARAMETER> rootParametersList;
	friend class DreamGraphics;
};

