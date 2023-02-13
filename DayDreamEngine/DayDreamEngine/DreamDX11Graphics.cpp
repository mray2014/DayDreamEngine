#include "DreamDX11Graphics.h"
#include <iostream>
#include <d3dcompiler.h>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

DreamDX11Graphics* instance = nullptr;

LRESULT DreamDX11Graphics::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return instance->ProcessMessage(hWnd, uMsg, wParam, lParam);
}

DreamDX11Graphics::DreamDX11Graphics() : DreamGraphics()
{
	clearScreenColor = DreamVector4(0.4f, 0.6f, 0.75f, 0.0f);
	if (!instance) {
		instance = this;
	}	
}


DreamDX11Graphics::~DreamDX11Graphics()
{
	if (depthStencilView) { depthStencilView->Release(); }
	if (backBufferRTV) { backBufferRTV->Release(); }

	if (swapChain) { swapChain->Release(); }
	if (context) { context->Release(); }
	if (device) { device->Release(); }
}

long DreamDX11Graphics::InitWindow(int w, int h, const char* title)
{
	width = w;
	height = h;
	titleBarText = title;

	// Start window creation by filling out the
	// appropriate window class struct
	WNDCLASS wndClass = {}; // Zero out the memory
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	// Redraw on horizontal or vertical movement/adjustment
	wndClass.lpfnWndProc = DreamDX11Graphics::WindowProc;
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

long DreamDX11Graphics::InitGraphics()
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
	ID3D11Texture2D* backBufferTexture = {};
	swapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(void**)&backBufferTexture);

	// Now that we have the texture, create a render target view
	// for the back buffer so we can render into it.  Then release
	// our local reference to the texture, since we have the view.
	if (backBufferTexture) {
		device->CreateRenderTargetView(
			backBufferTexture,
			0,
			&backBufferRTV);
		backBufferTexture->Release();
	}

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
	ID3D11Texture2D* depthBufferTexture = {};
	device->CreateTexture2D(&depthStencilDesc, 0, &depthBufferTexture);
	if (depthBufferTexture) {
		device->CreateDepthStencilView(depthBufferTexture, 0, &depthStencilView);
		depthBufferTexture->Release();
	}

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

void DreamDX11Graphics::SetViewPort(int posX, int posY, int w, int h)
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
	ID3D11Texture2D* backBufferTexture = {};
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBufferTexture));

	if (backBufferTexture) {
		device->CreateRenderTargetView(backBufferTexture, 0, &backBufferRTV);
		backBufferTexture->Release();
	}

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
	ID3D11Texture2D* depthBufferTexture = {};
	device->CreateTexture2D(&depthStencilDesc, 0, &depthBufferTexture);
	if (depthBufferTexture) {
		device->CreateDepthStencilView(depthBufferTexture, 0, &depthStencilView);
		depthBufferTexture->Release();
	}
	

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

bool DreamDX11Graphics::CheckWindowClose()
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

void DreamDX11Graphics::ClearScreen()
{
	const float color[4] = { clearScreenColor.x, clearScreenColor.y, clearScreenColor.z, clearScreenColor.w };

	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
}

void DreamDX11Graphics::SwapBuffers()
{
	swapChain->Present(0, 0);
}

void DreamDX11Graphics::CheckInputs()
{
}

DreamVertexArray* DreamDX11Graphics::GenerateVertexArray(DreamBuffer* vert, DreamBuffer* ind)
{
	return new DreamDX11VertexArray(vert, ind);
}

DreamBuffer* DreamDX11Graphics::GenerateBuffer(BufferType type, void* bufferData, size_t numOfElements, std::vector<size_t> strides, std::vector<size_t> offests, VertexDataUsage dataUsage)
{
	ID3D11Buffer* buffer = nullptr;

	size_t numOfBuffers = strides.size();

	size_t dataSize = 0;
	for (size_t i = 0; i < numOfBuffers; i++) {
		dataSize += strides[i];
	}
	dataSize *= numOfElements;
	// Create the proper struct to hold the initial vertex data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialBufferData;
	initialBufferData.pSysMem = bufferData;

	switch (type) {
	case BufferType::ArrayBuffer: {
		D3D11_BUFFER_DESC vbd;

		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth =  dataSize;       // 3 = number of vertices in the buffer
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells DirectX this is a vertex buffer
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		// Actually create the buffer with the initial data
		// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
		device->CreateBuffer(&vbd, &initialBufferData, &buffer);
		break;
	}
	case BufferType::ElementArrayBuffer: {
		D3D11_BUFFER_DESC vbd;

		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = dataSize;       // 3 = number of vertices in the buffer
		vbd.BindFlags = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is a vertex buffer
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		// Actually create the buffer with the initial data
		// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
		device->CreateBuffer(&vbd, &initialBufferData, &buffer);
		break;
	}
	}

	return new DreamBuffer((void*)buffer, type, dataSize, numOfBuffers, &strides[0], &offests[0]);
}

DreamBuffer* DreamDX11Graphics::GenerateBuffer(BufferType type, size_t bufferSize)
{
	return GenerateBuffer(type, nullptr, 1, { bufferSize }, { 0 }, StaticDraw);
}

void DreamDX11Graphics::UpdateBufferData(DreamBuffer* buffer, void* bufferData, size_t bufSize, VertexDataUsage dataUsage)
{

}


//void DreamDX11Graphics::BindVertexLayout(DreamBuffer* layout)
//{
//	context->IASetInputLayout((ID3D11InputLayout*)layout->GetBufferPointer().GetStoredPointer());
//}

void DreamDX11Graphics::BindBuffer(BufferType type, DreamBuffer* buffer)
{
	ID3D11Buffer* buff = (ID3D11Buffer*)buffer->GetBufferPointer().GetStoredPointer();
	switch (type) {
	case BufferType::ArrayBuffer: {
		context->IASetVertexBuffers(0,
			buffer->GetNumOfBuffers(),
			&buff,
			(const UINT*)buffer->GetBufferStrides(),
			(const UINT*)buffer->GetBufferOffsets());
		break;
	}
	case BufferType::ElementArrayBuffer: {
		context->IASetIndexBuffer(buff, DXGI_FORMAT_R32_UINT, 0);
		break;
	}
	}
}


ID3DBlob* shaderBlob;

bool layoutStarted = false;
std::vector<D3D11_INPUT_ELEMENT_DESC> vertDesc;
size_t vertexStrideCount = 0;

void DreamDX11Graphics::BeginVertexLayout()
{
	if (layoutStarted) {
		printf("ERROR: Vertex Layout creation process has started already!\nCall FinalizeVertexLayout to end the current operation and start a new one");
	}
	else {
		layoutStarted = true;
	}

}

void DreamDX11Graphics::AddVertexLayoutData(std::string dataName, int size, unsigned int dataType, bool shouldNormalize, unsigned int sizeOf)
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
		vertDesc.push_back({"", 0, (DXGI_FORMAT)format, 0, (const UINT)vertexStrideCount, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		vertDesc[vertDesc.size() - 1].SemanticName = new char[dataName.size() + 1];
		memcpy((void*)(vertDesc[vertDesc.size() - 1].SemanticName), dataName.c_str(), sizeof(char) * (dataName.size() + 1));

		vertexStrideCount += sizeOf;
	}
	else {
		printf("ERROR: No Vertex Layout creation process has started! Can't Add Data");
	}
}

DreamBuffer* DreamDX11Graphics::FinalizeVertexLayout()
{
	if (layoutStarted && shaderBlob) {
		ID3D11InputLayout* vInputLayout;

		//ID3D11ShaderReflection* refl;
		//D3DReflect(
		//	shaderBlob->GetBufferPointer(),
		//	shaderBlob->GetBufferSize(),
		//	IID_ID3D11ShaderReflection,
		//	(void**)&refl);

		//// Get shader info
		//D3D11_SHADER_DESC shaderDesc;
		//refl->GetDesc(&shaderDesc);

		const void* blobPtr = shaderBlob->GetBufferPointer();
		size_t blobSize = shaderBlob->GetBufferSize();

		HRESULT result = device->CreateInputLayout(&vertDesc[0], vertDesc.size(), blobPtr, blobSize, &vInputLayout);

		if (result != S_OK) {
			printf("ERROR: Could not create Input Layout!");
		}

		vertDesc.clear();
		vertexStrideCount = 0;

		shaderBlob->Release();

		return new DreamBuffer(vInputLayout);
	}
	else {
		printf("ERROR: No Vertex Layout creation process has started! Can't Finalize Data");
	}

	return nullptr;
}

void DreamDX11Graphics::UnBindBuffer(BufferType type)
{
	switch (type) {
	case BufferType::ArrayBuffer: {
		break;
	}
	case BufferType::ElementArrayBuffer: {
		break;
	}
	}
}


DreamShader* DreamDX11Graphics::LoadShader(const wchar_t* file, ShaderType shaderType)
{
	DreamShader* shader = nullptr;
	bool hasMatUniform = false;
	bool hasConstDataUniform = false;

	UniformList uniforms;
	UniformMembers uniformMembers;

	//$(OutDir)
	std::string outputDir = OUTPUT_DIR;
	std::wstring path(outputDir.begin(), outputDir.end());
	path.append(file);
	path.append(L".cso");

	HRESULT hr = D3DReadFileToBlob(path.c_str(), &shaderBlob);
	if (hr != S_OK)
	{
		printf("Failed to open/read Shader file");

	}

	// Create the shader - Calls an overloaded version of this abstract
	// method in the appropriate child class

	/*void* stuff = shaderBlob->GetBufferPointer();
	SIZE_T stuff2 = shaderBlob->GetBufferSize();
	void* stuff3 = &vShader;*/
	HRESULT result;

	void* blobPtr = shaderBlob->GetBufferPointer();
	size_t blobSize = shaderBlob->GetBufferSize();

	switch (shaderType) {
	case ShaderType::VertexShader: {
		ID3D11VertexShader* newShader;
		result = device->CreateVertexShader(
			blobPtr,
			blobSize,
			0,
			&newShader);
		shader = new DreamShader(shaderType, DreamPointer(newShader), uniforms, (hasMatUniform && hasConstDataUniform));
		break;
	}
	case ShaderType::PixelShader: {
		ID3D11PixelShader* newShader;
		result = device->CreatePixelShader(
			blobPtr,
			blobSize,
			0,
			&newShader);
		shader = new DreamShader(shaderType, DreamPointer(newShader), uniforms, (hasMatUniform && hasConstDataUniform));
		break;
	}
	case ShaderType::GeometryShader: {
		ID3D11GeometryShader* newShader;
		result = device->CreateGeometryShader(
			blobPtr,
			blobSize,
			0,
			&newShader);
		shader = new DreamShader(shaderType, DreamPointer(newShader), uniforms, (hasMatUniform && hasConstDataUniform));
		break;
	}
	case ShaderType::ComputeShader: {
		ID3D11ComputeShader* newShader;
		result = device->CreateComputeShader(
			blobPtr,
			blobSize,
			0,
			&newShader);

		shader = new DreamShader(shaderType, DreamPointer(newShader), uniforms, (hasMatUniform && hasConstDataUniform));
		break;
	}
	}
	 

	if (result != S_OK)
	{
		printf("Failed to create Shader");
	}

	if (shaderType != ShaderType::VertexShader) {
		shaderBlob->Release();
	}
	
	return shader;
}

void DreamDX11Graphics::ReleaseShader(DreamShader* shader)
{
	const void* ptr = shader->GetShaderPtr().GetStoredPointer();

	if (ptr) {
		switch (shader->GetShaderType()) {
		case ShaderType::VertexShader: {
			ID3D11VertexShader* vShader = (ID3D11VertexShader*)ptr;
			vShader->Release();
			vShader = nullptr;
			break;
		}
		case ShaderType::PixelShader: {
			ID3D11PixelShader* pShader = (ID3D11PixelShader*)ptr;
			pShader->Release();
			pShader = nullptr;
			break;
		}
		case ShaderType::GeometryShader: {
			ID3D11GeometryShader* gShader = (ID3D11GeometryShader*)ptr;
			gShader->Release();
			gShader = nullptr;
			break;
		}
		case ShaderType::ComputeShader: {
			ID3D11ComputeShader* cShader = (ID3D11ComputeShader*)ptr;
			cShader->Release();
			cShader = nullptr;
			break;
		}
		}
	}
}

void DreamDX11Graphics::SetShader(DreamShader* shader)
{
	
	switch (shader->GetShaderType()) {
	case ShaderType::VertexShader: {
		ID3D11VertexShader* vPtr = (ID3D11VertexShader*)shader->GetShaderPtr().GetStoredPointer();
		context->VSSetShader(vPtr, 0, 0);
		break;
	}
	case ShaderType::PixelShader: {
		ID3D11PixelShader* pPtr = (ID3D11PixelShader*)shader->GetShaderPtr().GetStoredPointer();
		context->PSSetShader(pPtr, 0, 0);
		break;
	}
	case ShaderType::GeometryShader: {
		ID3D11GeometryShader* gPtr = (ID3D11GeometryShader*)shader->GetShaderPtr().GetStoredPointer();
		context->GSSetShader(gPtr, 0, 0);
		break;
	}
	case ShaderType::ComputeShader: {
		ID3D11ComputeShader* cPtr = (ID3D11ComputeShader*)shader->GetShaderPtr().GetStoredPointer();
		context->CSSetShader(cPtr, 0, 0);
		break;
	}
	}
	
}

void DreamDX11Graphics::DrawWithIndex(size_t size)
{
	context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->DrawIndexed(size, 0, 0);
}

void DreamDX11Graphics::DrawWithVertex(size_t size)
{
	context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->Draw(size, 0);
}

void DreamDX11Graphics::Draw()
{
	//D3D11_MAPPED_SUBRESOURCE mapped = {};
	//context->Map(instanceWorldMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

	//// Copy to the resource


	//memcpy(mapped.pData, components, sizeof(InstanceData) * numInstances);

	//// Unmap so the GPU can use it again
	//context->Unmap(instanceWorldMatrixBuffer, 0);

	//ID3D11Buffer* vbs[2] = {
	//	assets->meshStorage[meshName]->vertArr,	// Per-vertex data
	//	instanceWorldMatrixBuffer			// Per-instance data
	//};

	//// Two buffers means two strides and two offsets!
	//UINT strides[2] = { sizeof(Vertex), sizeof(InstanceData) };
	//UINT offsets[2] = { 0, 0 };

	//// Set both vertex buffers
	//context->IASetVertexBuffers(0, 2, vbs, strides, offsets);
	//context->IASetIndexBuffer(assets->meshStorage[meshName]->indArr, DXGI_FORMAT_R32_UINT, 0);

	//instanceFVShader->SetMatrix4x4("view", cam->viewMatrix);
	//instanceFVShader->SetMatrix4x4("projection", cam->projectionMatrix);

	//instanceFVShader->SetFloat3("camPosition", cam->transform.position);

	//instanceFVShader->CopyAllBufferData();

	//SetLights(pixelFShader);
	//pixelFShader->SetSamplerState("basicSampler", textureSample);
	//pixelFShader->SetShaderResourceView("skyTexture", skyBoxSVR);
	//pixelFShader->SetFloat("reflectance", 0.0f);
	//pixelFShader->CopyAllBufferData();

	//instanceFVShader->SetShader();
	//pixelFShader->SetShader();

	//context->DrawIndexedInstanced(
	//	assets->meshStorage[meshName]->indCount, // Number of indices from index buffer
	//	count,					// Number of instances to actually draw
	//	0, 0, 0);
}

void DreamDX11Graphics::TerminateGraphics()
{
}

void DreamDX11Graphics::DestroyWindow()
{
	PostMessage(this->hWnd, WM_CLOSE, NULL, NULL);
}

void DreamDX11Graphics::DestroyBuffer(DreamBuffer* buffer)
{
	if (buffer) {
		ID3D11Buffer* dxBuffer = (ID3D11Buffer*)buffer->GetBufferPointer().GetStoredPointer();
		dxBuffer->Release();

		delete buffer;
		buffer = nullptr;
	}
}

// --------------------------------------------------------
// Handles messages that are sent to our window by the
// operating system.  Ignoring these messages would cause
// our program to hang and Windows would think it was
// unresponsive.
// --------------------------------------------------------
LRESULT DreamDX11Graphics::ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

void DreamDX11Graphics::BindVertexLayout(DreamBuffer* layout)
{
	context->IASetInputLayout((ID3D11InputLayout*)layout->GetBufferPointer().GetStoredPointer());
}

void DreamDX11Graphics::UnBindVertexLayout()
{
}

DreamDX11ShaderLinker::DreamDX11ShaderLinker()
{
	graphics = DreamGraphics::GetInstance();
}

DreamDX11ShaderLinker::~DreamDX11ShaderLinker()
{
}

void DreamDX11ShaderLinker::AttachShader(DreamShader* shader)
{
	linkedShaders.push_back(shader);
}

void DreamDX11ShaderLinker::Finalize()
{
}

void DreamDX11ShaderLinker::BindShaderLink()
{
	DreamDX11Graphics* dxGraphics = (DreamDX11Graphics*)graphics;

	for (size_t i = 0; i < linkedShaders.size(); i++) {
		if (linkedShaders[i]->GetShaderType() == VertexShader) {
			DreamBuffer* layout = linkedShaders[i]->GetInputLayout();
			if (layout) {
				dxGraphics->BindVertexLayout(layout);
			}
			else {
				printf("Vertex Input lLayout didnt exsist!");
			}
		}
		linkedShaders[i]->BindShaderData();
		dxGraphics->SetShader(linkedShaders[i]);
	}
}

void DreamDX11ShaderLinker::UnBindShaderLink()
{
}

DreamDX11VertexArray::DreamDX11VertexArray(DreamBuffer* vert, DreamBuffer* ind) : DreamVertexArray(vert, ind)
{
}

DreamDX11VertexArray::~DreamDX11VertexArray()
{
	graphics->DestroyBuffer(vertexBuffer);
	graphics->DestroyBuffer(indexBuffer);
}
void DreamDX11VertexArray::Bind()
{
	graphics->BindBuffer(ArrayBuffer, vertexBuffer);
	if (indexBuffer) {
		graphics->BindBuffer(ElementArrayBuffer, indexBuffer);
	}
}
void DreamDX11VertexArray::UnBind()
{
	graphics->UnBindBuffer(ArrayBuffer);
	graphics->UnBindBuffer(ElementArrayBuffer);
}
