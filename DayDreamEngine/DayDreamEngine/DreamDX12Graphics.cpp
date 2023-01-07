#include "DreamDX12Graphics.h"
#include <iostream>
#include <d3dcompiler.h>

LRESULT DreamDX12Graphics::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

DreamDX12Graphics::DreamDX12Graphics()
{
}


DreamDX12Graphics::~DreamDX12Graphics()
{

}

long DreamDX12Graphics::InitWindow(int w, int h, const char* title)
{
	return 0;
}

long DreamDX12Graphics::InitGraphics()
{
	return 0;
}

void DreamDX12Graphics::SetViewPort(int posX, int posY, int w, int h)
{
	
}

bool DreamDX12Graphics::CheckWindowClose()
{
	return false;
}

void DreamDX12Graphics::ClearScreen()
{

}

void DreamDX12Graphics::SwapBuffers()
{
	
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
	return nullptr;
}

void DreamDX12Graphics::BindBuffer(BufferType type, DreamBuffer* buffer)
{

}
void DreamDX12Graphics::BeginVertexLayout()
{

}

void DreamDX12Graphics::AddVertexLayoutData(std::string dataName, int size, unsigned int dataType, bool shouldNormalize, unsigned int sizeOf)
{

}

DreamBuffer* DreamDX12Graphics::FinalizeVertexLayout()
{
	return nullptr;
}

void DreamDX12Graphics::UnBindBuffer(BufferType type)
{

}


bool DreamDX12Graphics::LoadShader(const wchar_t* file, ShaderType shaderType, DreamPointer& ptr)
{
	return false;
}

void DreamDX12Graphics::ReleaseShader(DreamShader* shader)
{

}

void DreamDX12Graphics::SetShader(DreamShader* shader)
{
	
}

void DreamDX12Graphics::DrawWithIndex(size_t size)
{

}

void DreamDX12Graphics::DrawWithVertex(size_t size)
{

}

void DreamDX12Graphics::Draw()
{

}

void DreamDX12Graphics::TerminateGraphics()
{
}

void DreamDX12Graphics::DestroyWindow()
{

}

void DreamDX12Graphics::DestroyBuffer(DreamBuffer* buffer)
{

}

LRESULT DreamDX12Graphics::ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void DreamDX12Graphics::BindVertexLayout(DreamBuffer* layout)
{

}

void DreamDX12Graphics::UnBindVertexLayout()
{
}

DreamDX12ShaderLinker::DreamDX12ShaderLinker()
{
	
}

DreamDX12ShaderLinker::~DreamDX12ShaderLinker()
{
	for (size_t i = 0; i < linkedShaders.size(); i++) {
		graphics->ReleaseShader(linkedShaders[i]);
	}
}

void DreamDX12ShaderLinker::AttachShader(DreamShader* shader)
{
	linkedShaders.push_back(shader);
}

void DreamDX12ShaderLinker::Finalize()
{
}

void DreamDX12ShaderLinker::BindShaderLink()
{
	DreamDX12Graphics* dxGraphics = (DreamDX12Graphics*)graphics;

	for (size_t i = 0; i < linkedShaders.size(); i++) {
		if (linkedShaders[i]->GetShaderType() == VertexShader) {
			DreamBuffer* layout = ((VertexDreamShader*)linkedShaders[i])->GetInputLayout();
			dxGraphics->BindVertexLayout(layout);
		}
		linkedShaders[i]->BindShaderData();
		dxGraphics->SetShader(linkedShaders[i]);
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
