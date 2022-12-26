#include "DreamDXGraphics.h"
#include <iostream>


DreamDXGraphics* instance = nullptr;

LRESULT DreamDXGraphics::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return instance->ProcessMessage(hWnd, uMsg, wParam, lParam);
}

DreamDXGraphics::DreamDXGraphics()
{
	clearScreenColor = DreamVector4(0.4f, 0.6f, 0.75f, 0.0f);
	if (!instance) {
		instance = this;
	}	
}


DreamDXGraphics::~DreamDXGraphics()
{
	if (depthStencilView) { depthStencilView->Release(); }
	if (backBufferRTV) { backBufferRTV->Release(); }

	if (swapChain) { swapChain->Release(); }
	if (context) { context->Release(); }
	if (device) { device->Release(); }
}

long DreamDXGraphics::InitWindow(int w, int h, const char* title)
{
	width = w;
	height = h;
	titleBarText = title;

	// Start window creation by filling out the
	// appropriate window class struct
	WNDCLASS wndClass = {}; // Zero out the memory
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	// Redraw on horizontal or vertical movement/adjustment
	wndClass.lpfnWndProc = DreamDXGraphics::WindowProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;						// Our app's handle
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// Default icon
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);		// Default arrow cursor
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"Direct3DWindowClass";

	// Attempt to register the window class we've defined
	if (!RegisterClass(&wndClass))
	{
		// Get the most recent error
		DWORD error = GetLastError();

		// If the class exists, that's actually fine.  Otherwise,
		// we can't proceed with the next step.
		if (error != ERROR_CLASS_ALREADY_EXISTS)
			return HRESULT_FROM_WIN32(error);
	}

	// Adjust the width and height so the "client size" matches
	// the width and height given (the inner-area of the window)
	RECT clientRect;
	SetRect(&clientRect, 0, 0, width, height);
	AdjustWindowRect(
		&clientRect,
		WS_OVERLAPPEDWINDOW,	// Has a title bar, border, min and max buttons, etc.
		false);					// No menu bar

	// Center the window to the screen
	RECT desktopRect;
	GetClientRect(GetDesktopWindow(), &desktopRect);
	int centeredX = (desktopRect.right / 2) - (clientRect.right / 2);
	int centeredY = (desktopRect.bottom / 2) - (clientRect.bottom / 2);

	// Actually ask Windows to create the window itself
	// using our settings so far.  This will return the
	// handle of the window, which we'll keep around for later
	hWnd = CreateWindow(
		wndClass.lpszClassName,
		std::wstring(titleBarText.begin(), titleBarText.end()).c_str(),
		WS_OVERLAPPEDWINDOW,
		centeredX,
		centeredY,
		clientRect.right - clientRect.left,	// Calculated width
		clientRect.bottom - clientRect.top,	// Calculated height
		0,			// No parent window
		0,			// No menu
		hInstance,	// The app's handle
		0);			// No other windows in our application

	// Ensure the window was created properly
	if (hWnd == NULL)
	{
		DWORD error = GetLastError();
		return HRESULT_FROM_WIN32(error);
	}

	// The window exists but is not visible yet
	// We need to tell Windows to show it, and how to show it
	ShowWindow(hWnd, SW_SHOW);

	// Return an "everything is ok" HRESULT value
	return S_OK;
}

long DreamDXGraphics::InitGraphics()
{
	// This will hold options for DirectX initialization
	unsigned int deviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
	// If we're in debug mode in visual studio, we also
	// want to make a "Debug DirectX Device" to see some
	// errors and warnings in Visual Studio's output window
	// when things go wrong!
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Create a description of how our swap
	// chain should work
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 4;
	swapDesc.BufferDesc.Width = width;
	swapDesc.BufferDesc.Height = height;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.Flags = 0;
	swapDesc.OutputWindow = hWnd;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Windowed = true;

	// Result variable for below function calls
	HRESULT hr = S_OK;

	// Attempt to initialize DirectX
	hr = D3D11CreateDeviceAndSwapChain(
		0,							// Video adapter (physical GPU) to use, or null for default
		D3D_DRIVER_TYPE_HARDWARE,	// We want to use the hardware (GPU)
		0,							// Used when doing software rendering
		deviceFlags,				// Any special options
		0,							// Optional array of possible verisons we want as fallbacks
		0,							// The number of fallbacks in the above param
		D3D11_SDK_VERSION,			// Current version of the SDK
		&swapDesc,					// Address of swap chain options
		&swapChain,					// Pointer to our Swap Chain pointer
		&device,					// Pointer to our Device pointer
		&dxFeatureLevel,			// This will hold the actual feature level the app will use
		&context);					// Pointer to our Device Context pointer
	if (FAILED(hr)) return hr;

	// The above function created the back buffer render target
	// for us, but we need a reference to it
	ID3D11Texture2D* backBufferTexture;
	swapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(void**)&backBufferTexture);

	// Now that we have the texture, create a render target view
	// for the back buffer so we can render into it.  Then release
	// our local reference to the texture, since we have the view.
	device->CreateRenderTargetView(
		backBufferTexture,
		0,
		&backBufferRTV);
	backBufferTexture->Release();

	// Set up the description of the texture to use for the depth buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	// Create the depth buffer and its view, then 
	// release our reference to the texture
	ID3D11Texture2D* depthBufferTexture;
	device->CreateTexture2D(&depthStencilDesc, 0, &depthBufferTexture);
	device->CreateDepthStencilView(depthBufferTexture, 0, &depthStencilView);
	depthBufferTexture->Release();

	// Bind the views to the pipeline, so rendering properly 
	// uses their underlying textures
	context->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);

	// Lastly, set up a viewport so we render into
	// to correct portion of the window
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	context->RSSetViewports(1, &viewport);

	// Return the "everything is ok" HRESULT value
	return S_OK;
}

void DreamDXGraphics::SetViewPort(int posX, int posY, int w, int h)
{
	width = w;
	height = h;

	// Release existing DirectX views and buffers
	if (depthStencilView) { depthStencilView->Release(); }
	if (backBufferRTV) { backBufferRTV->Release(); }

	// Resize the underlying swap chain buffers
	swapChain->ResizeBuffers(
		1,
		width,
		height,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		0);

	// Recreate the render target view for the back buffer
	// texture, then release our local texture reference
	ID3D11Texture2D* backBufferTexture;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBufferTexture));
	device->CreateRenderTargetView(backBufferTexture, 0, &backBufferRTV);
	backBufferTexture->Release();

	// Set up the description of the texture to use for the depth buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	// Create the depth buffer and its view, then 
	// release our reference to the texture
	ID3D11Texture2D* depthBufferTexture;
	device->CreateTexture2D(&depthStencilDesc, 0, &depthBufferTexture);
	device->CreateDepthStencilView(depthBufferTexture, 0, &depthStencilView);
	depthBufferTexture->Release();

	// Bind the views to the pipeline, so rendering properly 
	// uses their underlying textures
	context->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);

	// Lastly, set up a viewport so we render into
	// to correct portion of the window
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	context->RSSetViewports(1, &viewport);
}

bool DreamDXGraphics::CheckWindowClose()
{
	MSG msg = {};
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		// Translate and dispatch the message
		// to our custom WindowProc function
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Our overall game and message loop

	if (msg.message != WM_QUIT)
	{
		return false;
	}

	// We'll end up here once we get a WM_QUIT message,
	// which usually comes from the user closing the window
	return true;
}

void DreamDXGraphics::ClearScreen()
{
	const float color[4] = { clearScreenColor.x, clearScreenColor.y, clearScreenColor.z, clearScreenColor.w };

	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
}

void DreamDXGraphics::SwapBuffers()
{
	swapChain->Present(0, 0);
}

void DreamDXGraphics::CheckInputs()
{
}

void DreamDXGraphics::GenerateVertexArray(size_t numOfBuffers, size_t& VBO)
{
}

void DreamDXGraphics::GenerateBuffer(size_t numOfBuffers, size_t& VBO)
{
}

void DreamDXGraphics::BindVertexArray(size_t& VBO)
{
}

void DreamDXGraphics::BindBuffer(BufferType type, size_t& VBO)
{
}

void DreamDXGraphics::CopyBufferData(BufferType type, size_t numOfElements, void* buffer, VertexDataUsage dataUsage)
{
}

void DreamDXGraphics::AddVertexAttributePointer(int size, unsigned int dataType, bool shouldNormalize, unsigned int sizeOf)
{
}

void DreamDXGraphics::UnBindVertexArray()
{
}

unsigned int DreamDXGraphics::LoadShader(const char* file, ShaderType shaderType)
{
	return 0;
}

void DreamDXGraphics::StartShaderProgramCreation()
{
}

void DreamDXGraphics::AttachShader(unsigned int shader)
{
}

unsigned int DreamDXGraphics::FinishShaderProgramCreation()
{
	return 0;
}

void DreamDXGraphics::SetShader(unsigned int shaderProg)
{
}

void DreamDXGraphics::DrawWithIndex(size_t size)
{
}

void DreamDXGraphics::DrawWithVertex(size_t size)
{
}

void DreamDXGraphics::Draw()
{
}

void DreamDXGraphics::TerminateGraphics()
{
}

void DreamDXGraphics::DestroyWindow()
{
	PostMessage(this->hWnd, WM_CLOSE, NULL, NULL);
}

// --------------------------------------------------------
// Handles messages that are sent to our window by the
// operating system.  Ignoring these messages would cause
// our program to hang and Windows would think it was
// unresponsive.
// --------------------------------------------------------
LRESULT DreamDXGraphics::ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//ImGuiIO& IO = ImGui::GetIO();


	// Check the incoming message and handle any we care about
	switch (uMsg)
	{
		// This is the message that signifies the window closing
	case WM_DESTROY:
		PostQuitMessage(0); // Send a quit message to our own program
		return 0;

		// Prevent beeping when we "alt-enter" into fullscreen
	case WM_MENUCHAR:
		return MAKELRESULT(0, MNC_CLOSE);

		// Prevent the overall window from becoming too small
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

		// Sent when the window size changes
	case WM_SIZE:
		// Save the new client area dimensions.
		//width = LOWORD(lParam);
		//height = HIWORD(lParam);

		// If DX is initialized, resize 
		// our required buffers
		if (device)
			SetViewPort(0,0, LOWORD(lParam), HIWORD(lParam));

		return 0;

		// Mouse button being pressed (while the cursor is currently over our window)
	case WM_LBUTTONDOWN:
		//IO.MouseDown[0] = true;
		//OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MBUTTONDOWN:
		//IO.MouseDown[2] = true;
		//OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_RBUTTONDOWN:
		//IO.MouseDown[1] = true;
		//OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;


		// Mouse button being released (while the cursor is currently over our window)
	case WM_LBUTTONUP:
		//IO.MouseDown[0] = false;
		//OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MBUTTONUP:
		//IO.MouseDown[2] = false;
		//OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_RBUTTONUP:
		//IO.MouseDown[1] = false;
		//OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

		// Cursor moves over the window (or outside, while we're currently capturing it)
	case WM_MOUSEMOVE:
		//IO.MousePos = ImVec2(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		//OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), deltaTime);
		return 0;

		// Mouse wheel is scrolled
	case WM_MOUSEWHEEL:
		//OnMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (wParam < 256)
			//IO.KeysDown[wParam] = 1;
		return 0;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		if (wParam < 256)
			//IO.KeysDown[wParam] = 0;
		return 0;
	case WM_CHAR:
		if (wParam > 0 && wParam < 0x10000)
			//IO.AddInputCharacter((unsigned short)wParam);
		return 0;
	}



	// Let Windows handle any messages we're not touching
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}