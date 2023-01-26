#include "DreamDX12Graphics.h"
#include <iostream>

// D3D12 extension library.
#include "d3dx12/d3dx12.h"

#pragma comment(lib, "d3dcompiler.lib")

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

DreamDX12Graphics::DreamDX12Graphics()
{
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

	// Enable debug messages in debug mode.
#if defined(_DEBUG)
	ComPtr<ID3D12InfoQueue> pInfoQueue;
	if (SUCCEEDED(d3d12Device2.As(&pInfoQueue)))
	{
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);
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

	return dxgiSwapChain4;
}

ComPtr<ID3D12DescriptorHeap> DreamDX12Graphics::CreateDescriptorHeap(ComPtr<ID3D12Device2> device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors)
{
	ComPtr<ID3D12DescriptorHeap> descriptorHeap;

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = numDescriptors;
	desc.Type = type;

	ThrowIfFailed(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptorHeap)));

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

	return commandAllocator;
}
ComPtr<ID3D12GraphicsCommandList> DreamDX12Graphics::CreateCommandList(ComPtr<ID3D12Device2> device, ComPtr<ID3D12CommandAllocator> commandAllocator, D3D12_COMMAND_LIST_TYPE type)
{
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ThrowIfFailed(device->CreateCommandList(0, type, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));

	ThrowIfFailed(commandList->Close());

	return commandList;
}

ComPtr<ID3D12Fence> DreamDX12Graphics::CreateFence(ComPtr<ID3D12Device2> device)
{
	ComPtr<ID3D12Fence> fence;

	ThrowIfFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));

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


void DreamDX12Graphics::Update() {
	static uint64_t frameCounter = 0;
	static double elapsedSeconds = 0.0;
	static std::chrono::high_resolution_clock clock;
	static auto t0 = clock.now();

	frameCounter++;
	auto t1 = clock.now();
	auto deltaTime = t1 - t0;
	t0 = t1;
	elapsedSeconds += deltaTime.count() * 1e-9;
	if (elapsedSeconds > 1.0)
	{
		char buffer[500];
		auto fps = frameCounter / elapsedSeconds;
		sprintf_s(buffer, 500, "FPS: %f\n", fps);
		std::wstring debugBuffer(buffer[0], buffer[499]);
		OutputDebugString(debugBuffer.c_str());

		frameCounter = 0;
		elapsedSeconds = 0.0;
	}
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

	g_RTVDescriptorHeap = CreateDescriptorHeap(g_Device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, g_NumFrames);
	g_RTVDescriptorSize = g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	UpdateRenderTargetViews(g_Device, g_SwapChain, g_RTVDescriptorHeap);

	for (int i = 0; i < g_NumFrames; ++i)
	{
		g_CommandAllocators[i] = CreateCommandAllocator(g_Device, D3D12_COMMAND_LIST_TYPE_DIRECT);
	}
	g_CommandList = CreateCommandList(g_Device, g_CommandAllocators[g_CurrentBackBufferIndex], D3D12_COMMAND_LIST_TYPE_DIRECT);
	g_Fence = CreateFence(g_Device);
	g_FenceEvent = CreateEventHandle();

	g_IsInitialized = true;

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
	bool quit = msg.message == WM_QUIT;
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
	// Reset 
	auto commandAllocator = g_CommandAllocators[g_CurrentBackBufferIndex];
	auto backBuffer = g_BackBuffers[g_CurrentBackBufferIndex];

	commandAllocator->Reset();
	g_CommandList->Reset(commandAllocator.Get(), nullptr);

	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		backBuffer.Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	g_CommandList->ResourceBarrier(1, &barrier);

	D3D12_VIEWPORT viewPort{ 0 };
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = width;
	viewPort.Height = height;
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

	g_Device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&buf));

	void* vbibData;
	HRESULT result = buf->Map(0, NULL, &vbibData);
	if (!SUCCEEDED(result))
		printf("Map failed (0x%08X)", result);

	memcpy(vbibData, bufferData, bufferSize);

	buf->Unmap(0, NULL);

	return new DreamBuffer(buf, bufferSize, numOfBuffers, &strides[0], &offests[0]);
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
		vertexView.SizeInBytes = buffer->GetBufferPointer().GetPtrBlockSize();
		vertexView.StrideInBytes = totalStrideSize;

		g_CommandList->IASetVertexBuffers(0, numOfBuffers, &vertexView);
		break;
	}
	case ElementArrayBuffer: {
		D3D12_INDEX_BUFFER_VIEW indexView{};
		indexView.BufferLocation = bufResource->GetGPUVirtualAddress();
		indexView.SizeInBytes = buffer->GetBufferPointer().GetPtrBlockSize();
		indexView.Format = DXGI_FORMAT_R32_UINT;

		g_CommandList->IASetIndexBuffer(&indexView);
		break;
	}			
	}
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

void DreamDX12Graphics::AddVertexLayoutData(std::string dataName, int size, unsigned int dataType, bool shouldNormalize, unsigned int sizeOf)
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
		vertDesc.push_back({ "", 0, (DXGI_FORMAT)format, 0, (const UINT)vertexStrideCount, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });

		vertDesc[vertDesc.size() - 1].SemanticName = new char[dataName.size() + 1];
		memcpy((void*)(vertDesc[vertDesc.size() - 1].SemanticName), dataName.c_str(), sizeof(char) * (dataName.size() + 1));

		vertexStrideCount += sizeOf;
	}
	else {
		printf("ERROR: No Vertex Layout creation process has started! Can't Add Data");
	}
}

DreamBuffer* DreamDX12Graphics::FinalizeVertexLayout()
{


	return nullptr;
}

ID3D12PipelineState* DreamDX12Graphics::CreateGraphicPipeLine(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipeLineDesc) {
	if (layoutStarted) {

		ID3D12RootSignature* rootSig;

		// Creating root signiture
		{
			//D3D12_DESCRIPTOR_RANGE tableRange[1] = {};
			//tableRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
			//tableRange[0].NumDescriptors = 128;
			//
			//D3D12_ROOT_DESCRIPTOR_TABLE table;
			//table.NumDescriptorRanges = 1;
			//table.pDescriptorRanges = tableRange;
			//
			//
			//D3D12_ROOT_PARAMETER rootParam[3] = {};
			//rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
			//rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
			//rootParam[0].Constants.Num32BitValues = 50;
			//rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			//rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			//rootParam[1].DescriptorTable = table;
			//rootParam[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			//rootParam[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
			//rootParam[2].Constants.Num32BitValues = 1;
			//
			//D3D12_STATIC_SAMPLER_DESC sampleDec[1] = {};
			//sampleDec[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			//sampleDec[0].Filter = D3D12_FILTER_ANISOTROPIC;
			//sampleDec[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			//sampleDec[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			//sampleDec[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			//sampleDec[0].MaxLOD = 1000.0f;
			//sampleDec[0].MaxAnisotropy = 16;

			D3D12_ROOT_SIGNATURE_DESC signature;
			signature.NumParameters = 0;
			signature.pParameters = NULL;
			signature.NumStaticSamplers = 0;
			signature.pStaticSamplers = NULL;
			signature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; // D3D12_ROOT_SIGNATURE_FLAG_NONE;

			ID3DBlob* rsBlob;
			ID3DBlob* errorBlob;

			HRESULT hr = D3D12SerializeRootSignature(&signature, D3D_ROOT_SIGNATURE_VERSION_1, &rsBlob, &errorBlob);
			const char* err = "";
			if (!SUCCEEDED(hr))
			{
				err = (const char*)errorBlob->GetBufferPointer();
				printf("D3D12SerializeRootSignature failed (0x%08X) (%s)", hr, errorBlob->GetBufferPointer());
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


		D3D12_INPUT_LAYOUT_DESC layoutDesc{};
		layoutDesc.NumElements = vertDesc.size();
		layoutDesc.pInputElementDescs = &vertDesc[0];

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

		if (result != S_OK) {
			printf("ERROR: Could not create Input Layout!");
		}

		vertDesc.clear();
		vertexStrideCount = 0;


		return graphicsPipeline;
	}
	else {
		printf("ERROR: No Vertex Layout creation process has started! Can't Finalize Data");
	}

	return nullptr;
}

void DreamDX12Graphics::BindGraphicPipeLine(ID3D12PipelineState* pipeline, ID3D12RootSignature* rootSig) {
	g_CommandList->SetGraphicsRootSignature(rootSig);
	g_CommandList->SetPipelineState(pipeline);
}

void DreamDX12Graphics::UnBindBuffer(BufferType type)
{

}


bool DreamDX12Graphics::LoadShader(const wchar_t* file, ShaderType shaderType, DreamPointer& ptr)
{
	std::string outputDir = OUTPUT_DIR;
	std::wstring path(outputDir.begin(), outputDir.end());
	path.append(file);
	path.append(L".cso");

	ID3DBlob* shaderBlob;
	HRESULT hr = D3DReadFileToBlob(path.c_str(), &shaderBlob);
	if (hr != S_OK)
	{
		printf("Failed to open/read Shader file");
		return false;

	}

	void* blobPtr = shaderBlob->GetBufferPointer();
	size_t blobSize = shaderBlob->GetBufferSize();

	ptr = DreamPointer(blobPtr, blobSize);

	return true;
}

void DreamDX12Graphics::ReleaseShader(DreamShader* shader)
{

}

void DreamDX12Graphics::SetShader(DreamShader* shader)
{
	
}

void DreamDX12Graphics::DrawWithIndex(size_t size)
{
	g_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_CommandList->DrawIndexedInstanced(size, 1, 0, 0, 0);
}

void DreamDX12Graphics::DrawWithVertex(size_t size)
{
	g_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_CommandList->DrawInstanced(size, 1, 0, 0);
}

void DreamDX12Graphics::Draw()
{

}

void DreamDX12Graphics::TerminateGraphics()
{

	// Make sure the command queue has finished all commands before closing.
	Flush(g_CommandQueue, g_Fence, g_FenceValue, g_FenceEvent);

	::CloseHandle(g_FenceEvent);
}

void DreamDX12Graphics::DestroyWindow()
{

}

void DreamDX12Graphics::DestroyBuffer(DreamBuffer* buffer)
{

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
	for (size_t i = 0; i < linkedShaders.size(); i++) {
		dx12Graphics->ReleaseShader(linkedShaders[i]);
	}
}

void DreamDX12ShaderLinker::AttachShader(DreamShader* shader)
{
	DreamPointer shaderPointer = shader->GetShaderPtr();
	switch (shader->GetShaderType()) {
	case VertexShader: {
		pipeLineDesc.VS.BytecodeLength = shaderPointer.GetPtrBlockSize();
		pipeLineDesc.VS.pShaderBytecode = shaderPointer.GetStoredPointer();
		break;
	}
	case PixelShader: {
		pipeLineDesc.PS.BytecodeLength = shaderPointer.GetPtrBlockSize();
		pipeLineDesc.PS.pShaderBytecode = shaderPointer.GetStoredPointer();
		break;
	}
	case GeometryShader: {
		pipeLineDesc.GS.BytecodeLength = shaderPointer.GetPtrBlockSize();
		pipeLineDesc.GS.pShaderBytecode = shaderPointer.GetStoredPointer();
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

	linkedShaders.push_back(shader);
}

void DreamDX12ShaderLinker::Finalize()
{
	pipelineState = dx12Graphics->CreateGraphicPipeLine(pipeLineDesc);
}

// TODO: Make a SetGraphicsPipeline function
void DreamDX12ShaderLinker::BindShaderLink()
{
		dx12Graphics->BindGraphicPipeLine(pipelineState, pipeLineDesc.pRootSignature);
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
