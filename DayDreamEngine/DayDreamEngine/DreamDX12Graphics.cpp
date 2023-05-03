#include "DreamDX12Graphics.h"
#include <iostream>
#include <memory>
#include <DreamFileIO.h>

// D3D12 extension library.

#include <dxgidebug.h>
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

DreamDX12Graphics* instance = nullptr;

LRESULT DreamDX12Graphics::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (instance) {
		return instance->ProcessMessage(hWnd, uMsg, wParam, lParam);
	}
	else{
		instance = (DreamDX12Graphics*)DreamGraphics::GetInstance(); 
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
	
}

DreamDX12Graphics::DreamDX12Graphics() : DreamGraphics()
{
	MAX_FRAMES_IN_FLIGHT = g_NumFrames;
	EnableDebugLayer();
}


DreamDX12Graphics::~DreamDX12Graphics()
{
	
}

void DreamDX12Graphics::EnableDebugLayer() {
#if defined(_DEBUG)
	// Always enable the debug layer before doing anything DX12 related
	// so all possible errors generated while creating DX12 objects
	// are caught by the debug layer.
	ComPtr<ID3D12Debug> debugInterface;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
	debugInterface->EnableDebugLayer();

	//ComPtr<ID3D12Debug1> spDebugController1;
	//debugInterface->QueryInterface(IID_PPV_ARGS(&spDebugController1));
	//spDebugController1->SetEnableGPUBasedValidation(true);
#endif
}

long DreamDX12Graphics::InitWindow(int w, int h, const char* title)
{
	width = w;
	height = h;

	std::string storeTitle(title);
	std::wstring windowTitle(storeTitle.begin(), storeTitle.end());

	WNDCLASSEXW windowClass = {};
	windowClass.cbSize = sizeof(WNDCLASSEXW);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = DreamDX12Graphics::WindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInst;
	windowClass.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = L"Direct3DWindowClass";
	windowClass.hIconSm = ::LoadIcon(NULL, IDI_APPLICATION);

	static ATOM atom = ::RegisterClassExW(&windowClass);
	assert(atom > 0);

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	RECT windowRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
	int windowWidth = windowRect.right - windowRect.left;
	int windowHeight = windowRect.bottom - windowRect.top;

	// Center the window within the screen. Clamp to 0, 0 for the top-left corner.
	int windowX = std::max<int>(0, (screenWidth - windowWidth) / 2);
	int windowY = std::max<int>(0, (screenHeight - windowHeight) / 2);

	hWnd = CreateWindowExW(NULL,
							windowClass.lpszClassName, //window class name?
							windowTitle.c_str(),
							WS_OVERLAPPEDWINDOW,
							windowX,
							windowY,
							windowWidth,
							windowHeight,
							0,
							0,
							hInst, 
							0);

	// Ensure the window was created properly
	if (hWnd == NULL)
	{
		DWORD error = GetLastError();
		return HRESULT_FROM_WIN32(error);
	}

	ShowWindow(hWnd, SW_SHOW);

	return 0;
}


ComPtr<IDXGIAdapter4> DreamDX12Graphics::GetAdapter(bool useWarp) {

	ComPtr<IDXGIFactory4> dxgiFactory;
	UINT createFactoryFlags = 0;

#if defined(_DEBUG)
	createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
	ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory)));

	ComPtr<IDXGIAdapter1> dxgiAdapter1;
	ComPtr<IDXGIAdapter4> dxgiAdapter4;

	if (useWarp)
	{
		ThrowIfFailed(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&dxgiAdapter1)));
		ThrowIfFailed(dxgiAdapter1.As(&dxgiAdapter4));
	}
	else
	{
		SIZE_T maxDedicatedVideoMemory = 0;
		for (UINT i = 0; dxgiFactory->EnumAdapters1(i, &dxgiAdapter1) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			DXGI_ADAPTER_DESC1 dxgiAdapterDesc1;
			dxgiAdapter1->GetDesc1(&dxgiAdapterDesc1);

			// Check to see if the adapter can create a D3D12 device without actually 
			// creating it. The adapter with the largest dedicated video memory
			// is favored.
			if ((dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 &&
				SUCCEEDED(D3D12CreateDevice(dxgiAdapter1.Get(),
					D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)) &&
				dxgiAdapterDesc1.DedicatedVideoMemory > maxDedicatedVideoMemory)
			{
				maxDedicatedVideoMemory = dxgiAdapterDesc1.DedicatedVideoMemory;
				ThrowIfFailed(dxgiAdapter1.As(&dxgiAdapter4));
			}
		}
	}

	return dxgiAdapter4;
}

ComPtr<ID3D12Device2> DreamDX12Graphics::CreateDevice(ComPtr<IDXGIAdapter4> adapter) {
	ComPtr<ID3D12Device2> d3d12Device2;
	ThrowIfFailed(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&d3d12Device2)));

	d3d12Device2->SetName(L"Graphics Device");

	// Enable debug messages in debug mode.
#if defined(_DEBUG)
	ComPtr<ID3D12InfoQueue> pInfoQueue;
	if (SUCCEEDED(d3d12Device2.As(&pInfoQueue)))
	{
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
		//pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);
		// Suppress whole categories of messages
		//D3D12_MESSAGE_CATEGORY Categories[] = {};

		// Suppress messages based on their severity level
		D3D12_MESSAGE_SEVERITY Severities[] =
		{
			D3D12_MESSAGE_SEVERITY_INFO
		};

		// Suppress individual messages by their ID
		D3D12_MESSAGE_ID DenyIds[] = {
			D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,   // I'm really not sure how to avoid this message.
			D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,                         // This warning occurs when using capture frame while graphics debugging.
			D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,                       // This warning occurs when using capture frame while graphics debugging.
		};

		D3D12_INFO_QUEUE_FILTER NewFilter = {};
		//NewFilter.DenyList.NumCategories = _countof(Categories);
		//NewFilter.DenyList.pCategoryList = Categories;
		NewFilter.DenyList.NumSeverities = _countof(Severities);
		NewFilter.DenyList.pSeverityList = Severities;
		//NewFilter.DenyList.NumSeverities = 0;
		//NewFilter.DenyList.pSeverityList = NULL;
		NewFilter.DenyList.NumIDs = _countof(DenyIds);
		NewFilter.DenyList.pIDList = DenyIds;

		ThrowIfFailed(pInfoQueue->PushStorageFilter(&NewFilter));
	}
#endif

	return d3d12Device2;
}

ComPtr<ID3D12CommandQueue> DreamDX12Graphics::CreateCommandQueue(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type)
{
	ComPtr<ID3D12CommandQueue> d3d12CommandQueue;

	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Type = type;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;

	ThrowIfFailed(device->CreateCommandQueue(&desc, IID_PPV_ARGS(&d3d12CommandQueue)));
	d3d12CommandQueue->SetName(L"Command Queue");

	return d3d12CommandQueue;
}

bool DreamDX12Graphics::CheckTearingSupport()
{
	BOOL allowTearing = FALSE;

	// Rather than create the DXGI 1.5 factory interface directly, we create the
	// DXGI 1.4 interface and query for the 1.5 interface. This is to enable the 
	// graphics debugging tools which will not support the 1.5 factory interface 
	// until a future update.
	ComPtr<IDXGIFactory4> factory4;
	if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4))))
	{
		ComPtr<IDXGIFactory5> factory5;
		if (SUCCEEDED(factory4.As(&factory5)))
		{
			if (FAILED(factory5->CheckFeatureSupport(
				DXGI_FEATURE_PRESENT_ALLOW_TEARING,
				&allowTearing, sizeof(allowTearing))))
			{
				allowTearing = FALSE;
			}
		}
	}

	return allowTearing == TRUE;
}
ComPtr<IDXGISwapChain4> DreamDX12Graphics::CreateSwapChain(HWND hWnd, ComPtr<ID3D12CommandQueue> commandQueue, uint32_t width, uint32_t height, uint32_t bufferCount) {

	ComPtr<IDXGISwapChain4> dxgiSwapChain4;
	ComPtr<IDXGIFactory4> dxgiFactory4;
	UINT createFactoryFlags = 0;
#if defined(_DEBUG)
	createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

	ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory4)));

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = width;
	swapChainDesc.Height = height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.Stereo = FALSE;
	swapChainDesc.SampleDesc = { 1, 0 };
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = bufferCount;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	// It is recommended to always allow tearing if tearing support is available.
	swapChainDesc.Flags = CheckTearingSupport() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

	ComPtr<IDXGISwapChain1> swapChain1;
	ThrowIfFailed(dxgiFactory4->CreateSwapChainForHwnd(
		commandQueue.Get(),
		hWnd,
		&swapChainDesc,
		nullptr,
		nullptr,
		&swapChain1));

	// Disable the Alt+Enter fullscreen toggle feature. Switching to fullscreen
	// will be handled manually.
	ThrowIfFailed(dxgiFactory4->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));

	ThrowIfFailed(swapChain1.As(&dxgiSwapChain4));

	const char* name = "Swap Chain";
	//dxgiSwapChain4->SetPrivateData(WKPDID_D3DDebugObjectName, 0, NULL);
	//dxgiSwapChain4->SetPrivateData(WKPDID_D3DDebugObjectNameW, (UINT)strlen(name), name);

	return dxgiSwapChain4;
}

ComPtr<ID3D12DescriptorHeap> DreamDX12Graphics::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_DESCRIPTOR_HEAP_FLAGS flags, uint32_t numDescriptors)
{
	ComPtr<ID3D12DescriptorHeap> descriptorHeap;

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = numDescriptors;
	desc.Flags = flags;
	desc.Type = type;

	ThrowIfFailed(g_Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptorHeap)));

	descriptorHeap->SetName(L"Descriptor Heap");

	return descriptorHeap;
}

void DreamDX12Graphics::UpdateRenderTargetViews(ComPtr<ID3D12Device2> device, ComPtr<IDXGISwapChain4> swapChain, ComPtr<ID3D12DescriptorHeap> descriptorHeap)
{
	auto rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(descriptorHeap->GetCPUDescriptorHandleForHeapStart());

	for (int i = 0; i < g_NumFrames; ++i)
	{
		ComPtr<ID3D12Resource> backBuffer;
		ThrowIfFailed(swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)));

		device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle);

		g_BackBuffers[i] = backBuffer;

		rtvHandle.Offset(rtvDescriptorSize);
	}
}

ComPtr<ID3D12CommandAllocator> DreamDX12Graphics::CreateCommandAllocator(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type)
{
	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ThrowIfFailed(device->CreateCommandAllocator(type, IID_PPV_ARGS(&commandAllocator)));

	commandAllocator->SetName(L"Command Allocator");

	return commandAllocator;
}
ComPtr<ID3D12GraphicsCommandList> DreamDX12Graphics::CreateCommandList(ComPtr<ID3D12Device2> device, ComPtr<ID3D12CommandAllocator> commandAllocator, D3D12_COMMAND_LIST_TYPE type)
{
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ThrowIfFailed(device->CreateCommandList(0, type, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));

	commandList->SetName(L"Command List");

	ThrowIfFailed(commandList->Close());

	return commandList;
}

ComPtr<ID3D12Fence> DreamDX12Graphics::CreateFence(ComPtr<ID3D12Device2> device)
{
	ComPtr<ID3D12Fence> fence;

	ThrowIfFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));

	fence->SetName(L"Fence");

	return fence;
}


HANDLE DreamDX12Graphics::CreateEventHandle()
{
	HANDLE fenceEvent;

	fenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent && "Failed to create fence event.");

	return fenceEvent;
}

uint64_t DreamDX12Graphics::Signal(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence,uint64_t& fenceValue)
{
	uint64_t fenceValueForSignal = ++fenceValue;
	ThrowIfFailed(commandQueue->Signal(fence.Get(), fenceValueForSignal));

	return fenceValueForSignal;
}
void DreamDX12Graphics::WaitForFenceValue(ComPtr<ID3D12Fence> fence, uint64_t fenceValue, HANDLE fenceEvent, std::chrono::milliseconds duration)
{
	if (fence->GetCompletedValue() < fenceValue)
	{
		ThrowIfFailed(fence->SetEventOnCompletion(fenceValue, fenceEvent));
		::WaitForSingleObject(fenceEvent, static_cast<DWORD>(duration.count()));
	}
}
void DreamDX12Graphics::Flush(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence, uint64_t& fenceValue, HANDLE fenceEvent)
{
	uint64_t fenceValueForSignal = Signal(commandQueue, fence, fenceValue);
	WaitForFenceValue(fence, fenceValueForSignal, fenceEvent);
}

void DreamDX12Graphics::Render() {

}

long DreamDX12Graphics::InitGraphics()
{
	g_TearingSupported = CheckTearingSupport();

	ComPtr<IDXGIAdapter4> dxgiAdapter4 = GetAdapter(g_UseWarp);

	g_Device = CreateDevice(dxgiAdapter4);

	g_CommandQueue = CreateCommandQueue(g_Device, D3D12_COMMAND_LIST_TYPE_DIRECT);

	g_SwapChain = CreateSwapChain(hWnd, g_CommandQueue,
		width, height, g_NumFrames);

	g_CurrentBackBufferIndex = g_SwapChain->GetCurrentBackBufferIndex();

	g_RTVDescriptorHeap = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, g_NumFrames);
	g_RTVDescriptorSize = g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	CBV_SRV_UAV_Heap = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, 1000000);

	UpdateRenderTargetViews(g_Device, g_SwapChain, g_RTVDescriptorHeap);

	for (int i = 0; i < g_NumFrames; ++i)
	{
		g_CommandAllocators[i] = CreateCommandAllocator(g_Device, D3D12_COMMAND_LIST_TYPE_DIRECT);
	}
	g_CommandList = CreateCommandList(g_Device, g_CommandAllocators[g_CurrentBackBufferIndex], D3D12_COMMAND_LIST_TYPE_DIRECT);
	g_Fence = CreateFence(g_Device);
	g_FenceEvent = CreateEventHandle();

	g_IsInitialized = true;

	auto commandAllocator = g_CommandAllocators[g_CurrentBackBufferIndex];
	auto backBuffer = g_BackBuffers[g_CurrentBackBufferIndex];

	commandAllocator->Reset();
	g_CommandList->Reset(commandAllocator.Get(), nullptr);

	return 0;
}

void DreamDX12Graphics::SetFullscreen(bool fullscreen) {
	if (g_Fullscreen != fullscreen)
	{
		g_Fullscreen = fullscreen;

		if (g_Fullscreen) // Switching to fullscreen.
		{
			// Store the current window dimensions so they can be restored 
			// when switching out of fullscreen state.
			::GetWindowRect(hWnd, &g_WindowRect);

			// Set the window style to a borderless window so the client area fills
			// the entire screen.
			UINT windowStyle = WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);

			::SetWindowLongW(hWnd, GWL_STYLE, windowStyle);

			// Query the name of the nearest display device for the window.
			// This is required to set the fullscreen dimensions of the window
			// when using a multi-monitor setup.
			HMONITOR hMonitor = ::MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
			MONITORINFOEX monitorInfo = {};
			monitorInfo.cbSize = sizeof(MONITORINFOEX);
			::GetMonitorInfo(hMonitor, &monitorInfo);

			::SetWindowPos(hWnd, HWND_TOP,
				monitorInfo.rcMonitor.left,
				monitorInfo.rcMonitor.top,
				monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
				monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
				SWP_FRAMECHANGED | SWP_NOACTIVATE);

			::ShowWindow(hWnd, SW_MAXIMIZE);
		}
		else
		{
			// Restore all the window decorators.
			::SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

			::SetWindowPos(hWnd, HWND_NOTOPMOST,
				g_WindowRect.left,
				g_WindowRect.top,
				g_WindowRect.right - g_WindowRect.left,
				g_WindowRect.bottom - g_WindowRect.top,
				SWP_FRAMECHANGED | SWP_NOACTIVATE);

			::ShowWindow(hWnd, SW_NORMAL);
		}
	}
}

void DreamDX12Graphics::SetViewPort(int posX, int posY, int w, int h)
{
	if (width != w || height != h)
	{
		 //Don't allow 0 size swap chain back buffers.
		width = std::max(1u, (unsigned int)w); // TODO: change parameter type later 
		height = std::max(1u, (unsigned int)h);
		
		// Flush the GPU queue to make sure the swap chain's back buffers
		// are not being referenced by an in-flight command list.
		Flush(g_CommandQueue, g_Fence, g_FenceValue, g_FenceEvent);
		
		for (int i = 0; i < g_NumFrames; ++i)
		{
			// Any references to the back buffers must be released
			// before the swap chain can be resized.
			g_BackBuffers[i].Reset();
			g_FrameFenceValues[i] = g_FrameFenceValues[g_CurrentBackBufferIndex];
		}
		
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		ThrowIfFailed(g_SwapChain->GetDesc(&swapChainDesc));
		ThrowIfFailed(g_SwapChain->ResizeBuffers(g_NumFrames, width, height,
			swapChainDesc.BufferDesc.Format, swapChainDesc.Flags));
		
		g_CurrentBackBufferIndex = g_SwapChain->GetCurrentBackBufferIndex();
		
		UpdateRenderTargetViews(g_Device, g_SwapChain, g_RTVDescriptorHeap);
	}
}

bool DreamDX12Graphics::CheckWindowClose()
{
	MSG msg = {};

	if (!quit)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
	return quit;
}

void DreamDX12Graphics::ClearScreen()
{
	auto commandAllocator = g_CommandAllocators[g_CurrentBackBufferIndex];
	auto backBuffer = g_BackBuffers[g_CurrentBackBufferIndex];
	if (!bInitalFrame) {
		// Reset 
		commandAllocator->Reset();
		g_CommandList->Reset(commandAllocator.Get(), nullptr);
	}
	bInitalFrame = false;

	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		backBuffer.Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	g_CommandList->ResourceBarrier(1, &barrier);

	D3D12_VIEWPORT viewPort{ 0 };
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = (float)width;
	viewPort.Height = (float)height;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;

	D3D12_RECT scissorRect{ 0 };
	scissorRect.left = 0;
	scissorRect.top = 0;
	scissorRect.right = width;
	scissorRect.bottom = height;

	// Clear Screen
	FLOAT clearColor[] = { clearScreenColor.x, clearScreenColor.y, clearScreenColor.z, clearScreenColor.w };
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(g_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
		g_CurrentBackBufferIndex, g_RTVDescriptorSize);

	g_CommandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
	g_CommandList->RSSetViewports(1, &viewPort);
	g_CommandList->RSSetScissorRects(1, &scissorRect);
	g_CommandList->OMSetRenderTargets(1, &rtv, FALSE, NULL);
}

void DreamDX12Graphics::SwapBuffers()
{
	auto backBuffer = g_BackBuffers[g_CurrentBackBufferIndex];
	// Present
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		backBuffer.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	g_CommandList->ResourceBarrier(1, &barrier);

	ThrowIfFailed(g_CommandList->Close());
	ID3D12CommandList* const commandLists[] = {
		g_CommandList.Get()
	};


	g_CommandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);
	UINT syncInterval = g_VSync ? 1 : 0;
	UINT presentFlags = g_TearingSupported && !g_VSync ? DXGI_PRESENT_ALLOW_TEARING : 0;


	ThrowIfFailed(g_SwapChain->Present(syncInterval, presentFlags));

	g_FrameFenceValues[g_CurrentBackBufferIndex] = Signal(g_CommandQueue, g_Fence, g_FenceValue);
	g_CurrentBackBufferIndex = g_SwapChain->GetCurrentBackBufferIndex();



	WaitForFenceValue(g_Fence, g_FrameFenceValues[g_CurrentBackBufferIndex], g_FenceEvent);
}

void DreamDX12Graphics::CheckInputs()
{
}

DreamVertexArray* DreamDX12Graphics::GenerateVertexArray(DreamBuffer* vert, DreamBuffer* ind)
{
	return new DreamDX12VertexArray(vert, ind);
}

DreamBuffer* DreamDX12Graphics::GenerateBuffer(BufferType type, void* bufferData, size_t numOfElements, std::vector<size_t> strides, std::vector<size_t> offests, VertexDataUsage dataUsage)
{
	ID3D12Resource* buf;

	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type =  D3D12_HEAP_TYPE_UPLOAD;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProp.CreationNodeMask = 0;
	heapProp.VisibleNodeMask = 0;

	size_t numOfBuffers = strides.size();

	UINT64 bufferSize = 0;
	for (size_t i = 0; i < numOfBuffers; i++) {
		bufferSize += strides[i];
	}

	bufferSize *= numOfElements;
	bufferSize = (bufferSize + 255) & ~255; // CB size is required to be 256-byte aligned.

	D3D12_RESOURCE_DIMENSION dimensionBuffer = D3D12_RESOURCE_DIMENSION_BUFFER;

	D3D12_RESOURCE_DESC bufferDesc{
	bufferDesc.Dimension = dimensionBuffer,
	bufferDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT,
	bufferDesc.Width = bufferSize,
	bufferDesc.Height = 1,
	bufferDesc.DepthOrArraySize = 1,
	bufferDesc.MipLevels = 1,
	bufferDesc.Format = DXGI_FORMAT_UNKNOWN,
	bufferDesc.SampleDesc.Count = 1,
	bufferDesc.SampleDesc.Quality = 0,
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
	bufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE
	};

	HRESULT hr = g_Device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&buf));
	if (!SUCCEEDED(hr))
		printf("Create committed resource failed (0x%08X)", hr);

	if (type != TextureBuffer) {
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = buf->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = bufferSize;

		int size = g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvHandle0(CBV_SRV_UAV_Heap->GetCPUDescriptorHandleForHeapStart(), m_DescriptorHeapCount, size);
		g_Device->CreateConstantBufferView(&cbvDesc, cbvHandle0);
		m_DescriptorHeapCount++;
	}


	//g_Device->CreateConstantBufferView(&cbvDesc, CBV_SRV_UAV_Heap->GetCPUDescriptorHandleForHeapStart());

	void* vbibData;
	HRESULT result = buf->Map(0, NULL, &vbibData);
	if (!SUCCEEDED(result))
		printf("Map failed (0x%08X)", result);

	if (bufferData) {
		memcpy(vbibData, bufferData, bufferSize);
	}
	else {
		memcpy(vbibData, bufferData, 0);
	}
	
	if (type != UniformBuffer) {
		buf->Unmap(0, NULL);
	}

	buf->SetName(L"Buffer");

	DreamBuffer* newBuffer = new DreamBuffer(m_DescriptorHeapCount - 1, buf, vbibData, type, bufferSize, numOfBuffers, &strides[0], &offests[0]);

	return newBuffer;
}

DreamBuffer* DreamDX12Graphics::GenerateBuffer(BufferType type, size_t bufferSize)
{
	return GenerateBuffer(type, nullptr, 1, { bufferSize }, { 0 }, StaticDraw);
}

DreamPointer* DreamDX12Graphics::GenerateTexture(unsigned char* pixelBuffer, int texWidth, int texHeight)
{

	// Creating Texture Resource
	D3D12_RESOURCE_DESC textureDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM,
		static_cast<UINT64>(texWidth),
		static_cast<UINT>(texHeight));

	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	ID3D12Resource* textureResource;

	CD3DX12_HEAP_PROPERTIES props(D3D12_HEAP_TYPE_DEFAULT);
	ThrowIfFailed(g_Device->CreateCommittedResource(
		&props,
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&textureResource)));



	// Creating SRV
	bool is3DTexture = false;
	UINT descriptorSize = g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(CBV_SRV_UAV_Heap->GetCPUDescriptorHandleForHeapStart(), m_DescriptorHeapCount * descriptorSize);
	D3D12_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	shaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	if (is3DTexture)
	{
		shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		shaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		shaderResourceViewDesc.TextureCube.MostDetailedMip = 0;
		shaderResourceViewDesc.TextureCube.MipLevels = 0;
		shaderResourceViewDesc.TextureCube.ResourceMinLODClamp = 0.0f;
	}


	g_Device->CreateShaderResourceView(textureResource, is3DTexture ? &shaderResourceViewDesc : NULL, srvHandle);
	m_DescriptorHeapCount++;


	// Grabbing image footprint
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
	g_Device->GetCopyableFootprints(&textureDesc, 0, 1, 0, &footprint, nullptr, nullptr, nullptr);

	// Creating Buffer to hold data
	ID3D12Resource* bufferData;
	uint32_t bufferSize = ((texWidth * footprint.Footprint.RowPitch) + 255) & ~255; // TODO: align pls by 256

	D3D12_RESOURCE_DESC bufferDesc{
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER,
	bufferDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT,
	bufferDesc.Width = bufferSize,
	bufferDesc.Height = 1,
	bufferDesc.DepthOrArraySize = 1,
	bufferDesc.MipLevels = 1,
	bufferDesc.Format = DXGI_FORMAT_UNKNOWN,
	bufferDesc.SampleDesc.Count = 1,
	bufferDesc.SampleDesc.Quality = 0,
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
	bufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE
	};
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	HRESULT hr = g_Device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&bufferData));
	if (!SUCCEEDED(hr))
		printf("Create committed resource failed (0x%08X)", hr);

	D3D12_TEXTURE_COPY_LOCATION copyLocation;
	copyLocation.pResource = textureResource;
	copyLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	copyLocation.SubresourceIndex = 0;

	D3D12_TEXTURE_COPY_LOCATION srcLocation;
	srcLocation.pResource = bufferData;
	srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	srcLocation.PlacedFootprint = footprint;

	D3D12_BOX texBox;
	texBox.top = 0;
	texBox.left = 0;
	texBox.bottom = texHeight;
	texBox.right = texWidth;
	texBox.front = 0;
	texBox.back = 1;

	// Copying texture data to buffer
	void* vbibData;
	HRESULT result = bufferData->Map(0, NULL, &vbibData);
	if (!SUCCEEDED(result))
		printf("Map failed (0x%08X)", result);

	memset(vbibData, 0, bufferSize);
	uint8_t* uploadStart = (uint8_t*)vbibData;
	uint8_t* sourceStart = pixelBuffer;
	uint32_t sourcePitch = (texWidth * sizeof(uint32_t));
	for (uint32_t i = 0; i < texHeight; i++)
	{
		memcpy(
			uploadStart + (i * footprint.Footprint.RowPitch),
			sourceStart + (i * sourcePitch),
			sourcePitch
		);
	}

	// Copying texture buffer to srv
	g_CommandList->CopyTextureRegion(&copyLocation, 0, 0, 0, &srcLocation, &texBox);

	textureResource->SetName(L"Texture");
	return new DreamPointer(m_DescriptorHeapCount - 1,textureResource);
}

void DreamDX12Graphics::UpdateBufferData(DreamBuffer* buffer, void* bufferData, size_t bufSize, VertexDataUsage dataUsage)
{
	if (buffer->GetBufferType() == UniformBuffer) {
		memcpy(buffer->GetMemoryPointer(), bufferData, bufSize);
	}
	else {
		D3D12_RANGE range;
		range.Begin = 0;
		range.End = bufSize;
		ID3D12Resource* buff = (ID3D12Resource*)buffer->GetBufferPointer().GetStoredPointer();
		buff->WriteToSubresource(0, nullptr, bufferData, bufSize, 0);
	}

	
	//g_CommandList->resource(buff, 0, 0, bufferData, bufSize, 0);
}

void DreamDX12Graphics::BindBuffer(BufferType type, DreamBuffer* buffer)
{

	ID3D12Resource* bufResource = (ID3D12Resource*)buffer->GetBufferPointer().GetStoredPointer();

	size_t numOfBuffers = buffer->GetNumOfBuffers();
	const size_t* strides = buffer->GetBufferStrides();

	size_t totalStrideSize = 0;
	for (int i = 0; i < numOfBuffers; i++) {
		totalStrideSize += strides[i];
	}

	switch (type) {
	case ArrayBuffer: {
		
		D3D12_VERTEX_BUFFER_VIEW vertexView{}; 
		vertexView.BufferLocation = bufResource->GetGPUVirtualAddress();
		vertexView.SizeInBytes = (UINT)buffer->GetBufferPointer().GetPtrBlockSize();
		vertexView.StrideInBytes = (UINT)totalStrideSize;

		g_CommandList->IASetVertexBuffers(0, (UINT)numOfBuffers, &vertexView);
		break;
	}
	case ElementArrayBuffer: {
		D3D12_INDEX_BUFFER_VIEW indexView{};
		indexView.BufferLocation = bufResource->GetGPUVirtualAddress();
		indexView.SizeInBytes = (UINT)buffer->GetBufferPointer().GetPtrBlockSize();
		indexView.Format = DXGI_FORMAT_R32_UINT;

		g_CommandList->IASetIndexBuffer(&indexView);
		break;
	}			
	}
}

void DreamDX12Graphics::BindTexture(DreamTexture* texture, int bindingPoint, ShaderType shaderType)
{
}

void DreamDX12Graphics::BindDescriptorTable(unsigned int index, unsigned int heapIndex) {
	int size = g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_GPU_DESCRIPTOR_HANDLE cbvHandle0(CBV_SRV_UAV_Heap->GetGPUDescriptorHandleForHeapStart(), size, heapIndex);
	g_CommandList->SetGraphicsRootDescriptorTable(index, cbvHandle0);
	//g_CommandList->SetGraphicsRootDescriptorTable(index, CBV_SRV_UAV_Heap->GetGPUDescriptorHandleForHeapStart());
}

bool layoutStarted = false;
std::vector<D3D12_INPUT_ELEMENT_DESC> vertDesc;
size_t vertexStrideCount = 0;

void DreamDX12Graphics::BeginVertexLayout()
{
	if (layoutStarted) {
		printf("ERROR: Vertex Layout creation process has started already!\nCall FinalizeVertexLayout to end the current operation and start a new one");
	}
	else {
		layoutStarted = true;
	}

}

void DreamDX12Graphics::AddVertexLayoutData(std::string dataName, int size, unsigned int location, bool shouldNormalize, unsigned int sizeOf)
{
	if (layoutStarted) {

		size_t format = -1;

		switch (size) {
		case 2: {
			format = DXGI_FORMAT_R32G32_FLOAT;
			break;
		}
		case 3: {
			format = DXGI_FORMAT_R32G32B32_FLOAT;
			break;
		}
		case 4: {
			format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		}
		}
		//D3D11_APPEND_ALIGNED_ELEMENT;
		vertDesc.push_back({ "", location, (DXGI_FORMAT)format, 0, (const UINT)vertexStrideCount, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });

		vertDesc[vertDesc.size() - 1].SemanticName = new char[dataName.size() + 1];
		memcpy((void*)(vertDesc[vertDesc.size() - 1].SemanticName), dataName.c_str(), sizeof(char) * (dataName.size() + 1));

		vertexStrideCount += sizeOf;
	}
	else {
		printf("ERROR: No Vertex Layout creation process has started! Can't Add Data");
	}
}

DreamPointer* DreamDX12Graphics::FinalizeVertexLayout()
{
	int size = vertDesc.size();
	D3D12_INPUT_ELEMENT_DESC* descArray = new D3D12_INPUT_ELEMENT_DESC[size];
	std::copy(vertDesc.begin(), vertDesc.end(), descArray);

	vertDesc.clear();
	vertexStrideCount = 0;
	layoutStarted = false;

	return new DreamPointer(descArray, sizeof(D3D12_INPUT_ELEMENT_DESC) * size);
}

ID3D12PipelineState* DreamDX12Graphics::CreateGraphicPipeLine(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipeLineDesc, std::vector<CD3DX12_DESCRIPTOR_RANGE>& tableRangeList, std::vector<CD3DX12_ROOT_PARAMETER>& rootParametersList, DreamPointer* vertexLayoutPtr) {
	ID3D12RootSignature* rootSig;

	// Creating root signiture
	{
		//CD3DX12_DESCRIPTOR_RANGE  tableRange[3] = {};
		//tableRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
		//tableRange[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);
		//tableRange[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2);
		//
		//CD3DX12_ROOT_PARAMETER rootParameters[3] = {};
		//rootParameters[0].InitAsDescriptorTable(1, &tableRange[0], D3D12_SHADER_VISIBILITY_VERTEX);
		//rootParameters[1].InitAsDescriptorTable(1, &tableRange[1], D3D12_SHADER_VISIBILITY_VERTEX);
		//rootParameters[2].InitAsDescriptorTable(1, &tableRange[2], D3D12_SHADER_VISIBILITY_PIXEL);
		

		//D3D12_ROOT_DESCRIPTOR_TABLE table;
		//table.NumDescriptorRanges = 1;
		//table.pDescriptorRanges = tableRange;
		
		//D3D12_ROOT_PARAMETER rootParam[1] = {};
		//rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		//rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		//rootParam[0].DescriptorTable = table;

		//rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		//rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		//rootParam[1].DescriptorTable = table;
		//rootParam[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		//rootParam[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		//rootParam[2].Constants.Num32BitValues = 1;
		
		//D3D12_STATIC_SAMPLER_DESC sampleDec[1] = {};
		//sampleDec[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		//sampleDec[0].Filter = D3D12_FILTER_ANISOTROPIC;
		//sampleDec[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		//sampleDec[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		//sampleDec[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		//sampleDec[0].MaxLOD = 1000.0f;
		//sampleDec[0].MaxAnisotropy = 16;

		// create a static sampler
		D3D12_STATIC_SAMPLER_DESC sampler = {};
		sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		sampler.MipLODBias = 0;
		sampler.MaxAnisotropy = 0;
		sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		sampler.MinLOD = 0.0f;
		sampler.MaxLOD = D3D12_FLOAT32_MAX;
		sampler.ShaderRegister = 0;
		sampler.RegisterSpace = 0;
		sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		D3D12_ROOT_SIGNATURE_DESC signature;
		signature.NumParameters = rootParametersList.size();
		signature.pParameters = &rootParametersList[0];
		signature.NumStaticSamplers = 1;
		signature.pStaticSamplers = &sampler;
		signature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; // D3D12_ROOT_SIGNATURE_FLAG_NONE;

		ID3DBlob* rsBlob;
		ID3DBlob* errorBlob;

		HRESULT hr = D3D12SerializeRootSignature(&signature, D3D_ROOT_SIGNATURE_VERSION_1, &rsBlob, &errorBlob);
		const char* err = "";
		if (!SUCCEEDED(hr))
		{
			err = (const char*)errorBlob->GetBufferPointer();
			printf("D3D12SerializeRootSignature failed (0x%08X) (%s)", hr, (char*)(errorBlob->GetBufferPointer()));
			
		}

		hr = g_Device->CreateRootSignature(
			0,
			rsBlob->GetBufferPointer(),
			rsBlob->GetBufferSize(),
			IID_PPV_ARGS(&rootSig)
		);

		if (!SUCCEEDED(hr)) {
			printf("CreateRootSignature failed (0x%08X)", hr);
			rsBlob->Release();
			return nullptr;
		}

		rsBlob->Release();
	}

	// Setting up pipelineDesc
	D3D12_RASTERIZER_DESC rasterizeDesc{};
	rasterizeDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rasterizeDesc.CullMode = D3D12_CULL_MODE_BACK;

	D3D12_INPUT_ELEMENT_DESC* vertLayout = (D3D12_INPUT_ELEMENT_DESC*)vertexLayoutPtr->GetStoredPointer();

	D3D12_INPUT_LAYOUT_DESC layoutDesc{};
	layoutDesc.NumElements = vertexLayoutPtr->GetPtrBlockSize() / sizeof(D3D12_INPUT_ELEMENT_DESC);
	layoutDesc.pInputElementDescs = vertLayout;

	D3D12_DEPTH_STENCILOP_DESC depthStencilOpDesc{};
	depthStencilOpDesc.StencilFailOp = D3D12_STENCIL_OP_ZERO;
	depthStencilOpDesc.StencilDepthFailOp = D3D12_STENCIL_OP_ZERO;
	depthStencilOpDesc.StencilPassOp = D3D12_STENCIL_OP_ZERO;


	D3D12_DEPTH_STENCIL_DESC depthDesc{};
	depthDesc.StencilEnable = FALSE;
	depthDesc.DepthEnable = FALSE;
	depthDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthDesc.FrontFace = depthStencilOpDesc;
	depthDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	pipeLineDesc.pRootSignature = rootSig;
	pipeLineDesc.SampleMask = 0xFFFFFFFF;
	pipeLineDesc.RasterizerState = rasterizeDesc;
	pipeLineDesc.InputLayout = layoutDesc;
	pipeLineDesc.DepthStencilState = depthDesc;
	pipeLineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	pipeLineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	pipeLineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pipeLineDesc.NumRenderTargets = 1;
	pipeLineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	pipeLineDesc.SampleDesc.Count = 1;

	ID3D12PipelineState* graphicsPipeline;
	HRESULT result = g_Device->CreateGraphicsPipelineState(&pipeLineDesc, IID_PPV_ARGS(&graphicsPipeline));
	graphicsPipeline->SetName(L"Graphics Pipeline");
	rootSig->SetName(L"Root Signature");

	if (result != S_OK) {
		printf("ERROR: Could not create Input Layout!");
	}

	tableRangeList.clear();

	return graphicsPipeline;
}

void DreamDX12Graphics::BindGraphicPipeLine(ID3D12PipelineState* pipeline, ID3D12RootSignature* rootSig, unsigned int heapCount) {
	g_CommandList->SetGraphicsRootSignature(rootSig);
	g_CommandList->SetPipelineState(pipeline); 
	g_CommandList->SetDescriptorHeaps(1, CBV_SRV_UAV_Heap.GetAddressOf());
}

void DreamDX12Graphics::UnBindBuffer(BufferType type)
{

}


DreamShader* DreamDX12Graphics::LoadShader(const wchar_t* file, ShaderType shaderType)
{
#pragma region ShaderReflection
	DreamShader* shader = nullptr;
	bool hasMat = false;
	DreamShaderResources resources;


	//  Loading SpirV shader file
	std::wstring wfile = L"";
	wfile.append(file);
	std::string convertFile(wfile.begin(), wfile.end());

	std::string spirv_Path = "Shaders/";
	spirv_Path.append(convertFile);
	spirv_Path.append(".spv");


	DreamFileIO::OpenFileRead(spirv_Path.c_str(), std::ios::ate | std::ios::binary);

	char* shaderCode = nullptr;
	size_t length;
	DreamFileIO::ReadFullFileQuick(&shaderCode, length);
	DreamFileIO::CloseFileRead();

	uint32_t* code = reinterpret_cast<uint32_t*>(shaderCode);
	// Read SPIR-V from disk or similar.
	std::vector<uint32_t> spirv_binary;
	spirv_cross::CompilerHLSL hlsl(code, length / sizeof(uint32_t));

	LoadShaderResources(hlsl, resources, hasMat);

	//D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif

	std::string version = "";
	switch (shaderType) {
	case ShaderType::VertexShader: {
		version = "vs";
		break;
	}
	case ShaderType::PixelShader: {
		version = "ps";
		break;
	}
	case ShaderType::GeometryShader: {
		version = "gs";
		break;
	}
	case ShaderType::TessalationShader: { // TODO: tessalation is Domain and Hull shaders... break into seperate enums
		//version = "vs";
		break;
	}
	case ShaderType::ComputeShader: {
		version = "cs";
		break;
	}
	}

	version.append("_5_0");

	// Set some options.
	spirv_cross::CompilerHLSL::Options options;
	options.shader_model = 50;
	hlsl.set_hlsl_options(options);

	//Compiling Shader
	ID3DBlob* shaderBlob;
	std::string source = hlsl.compile();
	HRESULT hr = D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "main", version.c_str(), flags, 0, &shaderBlob, nullptr);
	if (hr != S_OK)
	{
		printf("Failed to open/read Shader file");
		return nullptr;
	}
#pragma endregion


	void* blobPtr = shaderBlob;
	size_t blobSize = shaderBlob->GetBufferSize();

	shader = new DreamShader(shaderType, DreamPointer(blobPtr, blobSize), resources, hasMat);

	if (shaderType == VertexShader) {
		shader->CreateVertexInputLayout();
	}
	return shader;
}

void DreamDX12Graphics::ReleaseShader(DreamShader* shader)
{
	if (shader) {
		ID3DBlob* shaderBlob = (ID3DBlob*)shader->GetShaderPtr().GetStoredPointer();
		shaderBlob->Release();

		if (shader) {
			delete shader;
			shader = nullptr;
		}
	}
}

void DreamDX12Graphics::SetShader(DreamShader* shader)
{
	
}

void DreamDX12Graphics::DrawWithIndex(size_t size)
{
	g_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_CommandList->DrawIndexedInstanced((UINT)size, 1, 0, 0, 0);
}

void DreamDX12Graphics::DrawWithVertex(size_t size)
{
	g_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_CommandList->DrawInstanced((UINT)size, 1, 0, 0);
}

void DreamDX12Graphics::Draw()
{

}

void DreamDX12Graphics::TerminateGraphics()
{
	// Make sure the command queue has finished all commands before closing.
	Flush(g_CommandQueue, g_Fence, g_FenceValue, g_FenceEvent);

	::CloseHandle(g_FenceEvent);

	ComPtr<ID3D12DebugDevice2> g_DebugDevice;
	g_Device->QueryInterface(IID_PPV_ARGS(&g_DebugDevice));

	ComPtr<IDXGIDebug> g_dxgiDebug;
	DXGIGetDebugInterface1(0, IID_PPV_ARGS(&g_dxgiDebug));

	g_Device = nullptr;
	g_CommandQueue = nullptr;
	g_SwapChain = nullptr;
	for (int i = 0; i < g_NumFrames; i++) {
		g_BackBuffers[i] = nullptr;
		g_CommandAllocators[i] = nullptr;
	}
	g_CommandList = nullptr;
	g_RTVDescriptorHeap = nullptr;
	g_DTVDescriptorHeap = nullptr;
	g_Fence = nullptr;

	g_DebugDevice->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
	g_dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_SUMMARY);
}

void DreamDX12Graphics::DestroyWindow()
{
	//Window
}

void DreamDX12Graphics::DestroyBuffer(DreamBuffer* buffer)
{
	if (buffer) {
		ID3D12Resource* buff = (ID3D12Resource*)buffer->GetBufferPointer().GetStoredPointer();

		buff->Release();

		if (buffer) {
			delete buffer;
			buffer = nullptr;
		}
	}
}

LRESULT DreamDX12Graphics::ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (g_IsInitialized)
	{
		switch (uMsg)
		{
		case WM_PAINT: {
			Update();
			return 0;
		}
		case WM_SIZE:
		{
			RECT clientRect = {};
			::GetClientRect(hWnd, &clientRect);
			int wid = clientRect.right - clientRect.left;
			int hei = clientRect.bottom - clientRect.top;
			
			printf("width: %d, height: %d\n", wid, hei);
			
			SetViewPort(0, 0, LOWORD(lParam), HIWORD(lParam));
			return 0;
		}
		case WM_DESTROY:
			quit = true;
			::PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void DreamDX12Graphics::BindVertexLayout(DreamBuffer* layout)
{

}

void DreamDX12Graphics::UnBindVertexLayout()
{
}

DreamDX12ShaderLinker::DreamDX12ShaderLinker()
{
	dx12Graphics = (DreamDX12Graphics*)DreamGraphics::GetInstance();
}

DreamDX12ShaderLinker::~DreamDX12ShaderLinker()
{
	if (pipeLineDesc.pRootSignature) {
		pipeLineDesc.pRootSignature->Release();
		pipeLineDesc.pRootSignature = nullptr;
	}

	if (pipelineState) {
		pipelineState->Release();
		pipelineState = nullptr;
	}
	
	
}

void DreamDX12ShaderLinker::AttachShader(DreamShader* shader)
{
	ID3DBlob* shaderBlob = (ID3DBlob*)shader->GetShaderPtr().GetStoredPointer();

	size_t byteCodeLenghth = shaderBlob->GetBufferSize();
	void* shaderCodePtr = shaderBlob->GetBufferPointer();

	D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL;

	switch (shader->GetShaderType()) {
	case VertexShader: {
		visibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX;
		vertexInputLayout = shader->GetInputLayout();
		pipeLineDesc.VS.BytecodeLength = byteCodeLenghth;
		pipeLineDesc.VS.pShaderBytecode = shaderCodePtr;
		break;
	}
	case PixelShader: {
		visibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL;
		pipeLineDesc.PS.BytecodeLength = byteCodeLenghth;
		pipeLineDesc.PS.pShaderBytecode = shaderCodePtr;
		break;
	}
	case GeometryShader: {
		visibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_GEOMETRY;
		pipeLineDesc.GS.BytecodeLength = byteCodeLenghth;
		pipeLineDesc.GS.pShaderBytecode = shaderCodePtr;
		break;
	}
	case TessalationShader: {
		//pipeLineDesc..BytecodeLength = shaderPointer.GetPtrBlockSize();
		//pipeLineDesc.TS.pShaderBytecode = shaderPointer.GetStoredPointer();
		break;
	}
	case ComputeShader: {
		//pipeLineDesc..BytecodeLength = shaderPointer.GetPtrBlockSize();
		//pipeLineDesc.CS.pShaderBytecode = shaderPointer.GetStoredPointer();
		break;
	}

	}


	for (int i = 0; i < shader->shaderResources.uniforms.size(); i++) {
		uint32_t location = tableRangeList.size();

		tableRangeList.push_back(CD3DX12_DESCRIPTOR_RANGE());
		tableRangeList.back().Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, location);

		rootParametersList.push_back(CD3DX12_ROOT_PARAMETER());
		rootParametersList.back().ShaderVisibility = visibility;
	}
	for (auto& binding : shader->shaderResources.samplerBindings) {

		tableRangeList.push_back(CD3DX12_DESCRIPTOR_RANGE());
		tableRangeList.back().Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, binding.second);

		rootParametersList.push_back(CD3DX12_ROOT_PARAMETER());
		rootParametersList.back().ShaderVisibility = visibility;

		tableRangeList.push_back(CD3DX12_DESCRIPTOR_RANGE());
		tableRangeList.back().Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, binding.second);

		rootParametersList.push_back(CD3DX12_ROOT_PARAMETER());
		rootParametersList.back().ShaderVisibility = visibility;
	}

	linkedShaders.push_back(shader);
}

void DreamDX12ShaderLinker::Finalize()
{

	uint32_t curFrame = DreamGraphics::GetInstance()->currentFrame;
	uint32_t maxFramesInFlight = DreamGraphics::GetInstance()->GetMaxFramesInFlight();
	int numOfBuffers = 0;
	for (int i = 0; i < linkedShaders.size(); i++) {
		for (auto& uniformData : linkedShaders[i]->shaderResources.uniforms) {
			numOfBuffers += uniformData.second.buffers.size();
		}
	}

	for (int i = 0; i < rootParametersList.size(); i++) {
		rootParametersList[i].InitAsDescriptorTable(1, &tableRangeList[i]);
	}
	pipelineState = dx12Graphics->CreateGraphicPipeLine(pipeLineDesc, tableRangeList, rootParametersList, vertexInputLayout);
}

// TODO: Make a SetGraphicsPipeline function
void DreamDX12ShaderLinker::BindShaderLink(UniformIndexStore& indexStore, std::unordered_map<std::string, DreamTexture*> texMap)
{
	dx12Graphics->BindGraphicPipeLine(pipelineState, pipeLineDesc.pRootSignature);

	uint32_t curFrame = DreamGraphics::GetInstance()->currentFrame;
	uint32_t maxFramesInFlight = DreamGraphics::GetInstance()->GetMaxFramesInFlight();
	for (size_t i = 0; i < linkedShaders.size(); i++) {
		for (auto& samplerData : linkedShaders[i]->shaderResources.samplerBindings) {
			int index = (indexStore[samplerData.first] * maxFramesInFlight) + curFrame;
			std::string name = samplerData.first;

			if (!texMap.contains(name)) {
				continue;
			}

			unsigned int bindPoint = bindingPoints[name];
			unsigned int bindIndex = samplerData.second;

			//ID3D12Resource* texResource = (ID3D12Resource*)texMap[name]->GetTexturePointer()->GetStoredPointer();
			dx12Graphics->BindDescriptorTable(bindIndex, texMap[name]->GetTexturePointer()->GetStoredHandle());
		}

		for (auto& uniformData : linkedShaders[i]->shaderResources.uniforms) {
			int index = (indexStore[uniformData.first] * maxFramesInFlight) + curFrame;


			ID3D12Resource* container = (ID3D12Resource*)uniformData.second.buffers[index]->GetBufferPointer().GetStoredPointer();
			std::string name = uniformData.first;
			unsigned int bindPoint = bindingPoints[name];
			unsigned int bindIndex = uniformData.second.bindingIndex;

			dx12Graphics->BindDescriptorTable(bindIndex, uniformData.second.buffers[index]->GetBufferPointer().GetStoredHandle());
		}
	}
}

void DreamDX12ShaderLinker::UnBindShaderLink()
{
}

DreamDX12VertexArray::DreamDX12VertexArray(DreamBuffer* vert, DreamBuffer* ind) : DreamVertexArray(vert, ind)
{
}

DreamDX12VertexArray::~DreamDX12VertexArray()
{
	graphics->DestroyBuffer(vertexBuffer);
	graphics->DestroyBuffer(indexBuffer);
}
void DreamDX12VertexArray::Bind()
{
	graphics->BindBuffer(ArrayBuffer, vertexBuffer);
	if (indexBuffer) {
		graphics->BindBuffer(ElementArrayBuffer, indexBuffer);
	}
}
void DreamDX12VertexArray::UnBind()
{
	graphics->UnBindBuffer(ArrayBuffer);
	graphics->UnBindBuffer(ElementArrayBuffer);
}
