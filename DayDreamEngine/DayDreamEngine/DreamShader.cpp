#pragma once
#include "DreamShader.h"
#include "DreamGraphics.h"

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
	graphics->AddVertexLayoutData("POSITION", 3, 0, false, sizeof(DreamVector3));
	layout = graphics->FinalizeVertexLayout();
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