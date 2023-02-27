#pragma once
#include "DreamShader.h"
#include "DreamGraphics.h"
#include <stdexcept>

DreamShader::DreamShader()
{

}

DreamShader::~DreamShader()
{
}

void DreamShader::BindShaderData()
{
	DreamGraphics* graphics = DreamGraphics::GetInstance();
}

void DreamShader::CreateVertexInputLayout()
{
	DreamGraphics* graphics = DreamGraphics::GetInstance();
	graphics->BeginVertexLayout();
	graphics->AddVertexLayoutData("TEXCOORD", 3, 0, false, sizeof(DreamVector3));
	layout = graphics->FinalizeVertexLayout();
}



UniformInfo::UniformInfo() {
	bindingIndex = -1;
	maxIndex = -1;
	UniformMembers members = UniformMembers();
}

UniformInfo::UniformInfo(int index, int size, UniformMembers members) {
	bindingIndex = index;
	uniformSize = size;
	maxIndex = -1;
	uniformMembers = members;
}

int UniformInfo::AddUniformBuffer() {
	maxIndex++;
	DreamGraphics* graphics = DreamGraphics::GetInstance();

	if (graphics) {
		int maxFrames = graphics->GetMaxFramesInFlight();
		for (int i = 0; i < maxFrames; i++) {

			DreamBuffer* buff = graphics->GenerateBuffer(UniformBuffer, uniformSize);
			if (buff) {
				buffers.push_back(buff);
			}
		}
	}

	return maxIndex;
}

DreamBuffer* UniformInfo::GetUniformBuffer(int index) {
	DreamGraphics* graphics = DreamGraphics::GetInstance();

	if (graphics) {
		int maxFrames = graphics->GetMaxFramesInFlight();
		int offset = (index * maxFrames) + graphics->currentFrame;
		if (offset >= buffers.size()) {
			throw std::runtime_error("Failed to create a window");
		}
		else {
			return buffers[offset];
		}
	}
	return nullptr;
}

//VertexDreamShader::~VertexDreamShader()
//{
//	DreamGraphics* graphics = DreamGraphics::GetInstance();
//	graphics->ReleaseShader(this);
//}
//
//VertexDreamShader::VertexDreamShader(const wchar_t* file) : DreamShader(file, VertexShader)
//{
//	DreamGraphics* graphics = DreamGraphics::GetInstance();
//	graphics->BeginVertexLayout();
//	graphics->AddVertexLayoutData("POSITION", 3, 0, false, sizeof(DreamVector3));
//	layout = graphics->FinalizeVertexLayout();
//}
//
//void VertexDreamShader::BindShaderData()
//{
//}
//
//PixelDreamShader::~PixelDreamShader()
//{
//}
//
//PixelDreamShader::PixelDreamShader(const wchar_t* file) : DreamShader(file, PixelShader)
//{
//}
//
//void PixelDreamShader::BindShaderData()
//{
//}